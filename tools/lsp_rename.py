#!/usr/bin/env python3
"""Rename C identifiers through clangd (a language server), never by text substitution.

usage: lsp_rename.py [-n] OLD=NEW [OLD=NEW ...]
       lsp_rename.py [-n] -f renames.txt          (one OLD=NEW or "OLD NEW" per line)

For each pair: find one occurrence of OLD in the game's C (src/lifted, src/hand, src/*.c,
include/), ask clangd for textDocument/rename there and apply the WorkspaceEdit it returns.
clangd resolves the symbol, so locals, members or macros that merely share the spelling are
left alone. -n prints the edits without writing.

clangd runs with the port's compile_commands.json (build/clangd, made by
`cmake -S port -B build/clangd -DCMAKE_EXPORT_COMPILE_COMMANDS=ON`) and a background index,
which cross-file rename needs; the first run indexes all units (a few minutes, cached in
.cache/clangd afterwards).

After renaming, the caller updates config/symbols.txt and config/names.csv and runs
tools/build-and-verify.sh: names change no code, so the build must stay byte-identical.
"""
import argparse
import glob
import json
import os
import re
import subprocess
import sys
import threading
import time
from urllib.parse import quote, unquote

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
# LSP_CCDIR: another compile database, e.g. build/clangd-orig where macros keep their original bodies
CCDIR = os.environ.get("LSP_CCDIR", os.path.join(ROOT, "build", "clangd"))
CLANGD = os.environ.get("CLANGD", "/opt/homebrew/opt/llvm/bin/clangd")


def sources():
    pats = ["src/lifted/*.c", "src/hand/*.c", "src/*.c", "include/*.h"]
    return sorted(p for g in pats for p in glob.glob(os.path.join(ROOT, g)))


def uri(path):
    return "file://" + quote(os.path.abspath(path))


def path_of(u):
    return unquote(u[len("file://"):])


class Clangd:
    def __init__(self):
        self.p = subprocess.Popen(
            [CLANGD, "--compile-commands-dir=" + CCDIR, "--background-index",
             "--rename-file-limit=0", "--log=error", "-j=8"],
            stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
        self.id = 0
        self.results = {}
        self.indexing = set()
        self.index_seen = False
        self.cv = threading.Condition()
        threading.Thread(target=self.reader, daemon=True).start()
        self.request("initialize", {
            "processId": os.getpid(), "rootUri": uri(ROOT),
            "capabilities": {"window": {"workDoneProgress": True},
                             "workspace": {"workspaceEdit": {"documentChanges": False}}},
            "initializationOptions": {}})
        self.notify("initialized", {})

    def send(self, msg):
        body = json.dumps(msg).encode()
        self.p.stdin.write(b"Content-Length: %d\r\n\r\n" % len(body) + body)
        self.p.stdin.flush()

    def reader(self):
        f = self.p.stdout
        while True:
            n = 0
            while True:
                line = f.readline()
                if not line:
                    return
                if line in (b"\r\n", b"\n"):
                    break
                if line.lower().startswith(b"content-length:"):
                    n = int(line.split(b":")[1])
            msg = json.loads(f.read(n))
            with self.cv:
                if "id" in msg and "method" not in msg:
                    self.results[msg["id"]] = msg
                elif msg.get("method") == "window/workDoneProgress/create":
                    self.send({"jsonrpc": "2.0", "id": msg["id"], "result": None})
                elif msg.get("method") == "$/progress":
                    tok, kind = msg["params"]["token"], msg["params"]["value"]["kind"]
                    if tok == "backgroundIndexProgress":
                        if kind == "begin":
                            self.indexing.add(tok)
                            self.index_seen = True
                        elif kind == "end":
                            self.indexing.discard(tok)
                elif "id" in msg and "method" in msg:
                    self.send({"jsonrpc": "2.0", "id": msg["id"], "result": None})
                self.cv.notify_all()

    def request(self, method, params, timeout=600):
        with self.cv:
            self.id += 1
            rid = self.id
        self.send({"jsonrpc": "2.0", "id": rid, "method": method, "params": params})
        with self.cv:
            if not self.cv.wait_for(lambda: rid in self.results, timeout):
                raise SystemExit("clangd: no answer to " + method)
            msg = self.results.pop(rid)
        if "error" in msg:
            raise RuntimeError(msg["error"].get("message"))
        return msg.get("result")

    def notify(self, method, params):
        self.send({"jsonrpc": "2.0", "method": method, "params": params})

    def wait_index(self, settle=5, files=None):
        """Wait until the background index has a shard for every source file (`files`) and
        has stopped growing. clangd's progress events alone proved unreliable: the first run
        renamed in 1 of 4 files because only 14 shards existed yet."""
        t0 = time.time()
        shards = os.path.join(CCDIR, ".cache", "clangd", "index")
        want = set(os.path.basename(f) for f in files or [])
        last, stable = -1, 0
        while stable < 4 and time.time() - t0 < 3600:
            have = os.listdir(shards) if os.path.isdir(shards) else []
            done = set(h.split(".")[0] + "." + h.split(".")[1] for h in have if h.count(".") >= 2)
            missing = len(want - done)
            stable = stable + 1 if len(have) == last and missing == 0 else 0
            last = len(have)
            time.sleep(settle if stable else 2)
        time.sleep(settle)
        return time.time() - t0

    def open(self, path):
        self.notify("textDocument/didOpen", {"textDocument": {
            "uri": uri(path), "languageId": "c", "version": 1,
            "text": open(path, encoding="latin1").read()}})

    def close(self, path):
        self.notify("textDocument/didClose", {"textDocument": {"uri": uri(path)}})


def find_occurrence(name, files):
    """First (file, line, utf-16 column) where `name` appears as a whole token, outside
    comments and strings as far as a quick scan can tell; C files before headers."""
    pat = re.compile(r"\b%s\b" % re.escape(name))
    for path in sorted(files, key=lambda p: p.endswith(".h")):
        for ln, text in enumerate(open(path, encoding="latin1").read().split("\n")):
            code = re.sub(r'//.*|"(\\.|[^"\\])*"', lambda m: " " * len(m.group(0)), text)
            m = pat.search(code)
            if m:
                return path, ln, m.start()
    return None


def apply_edit(edit, dry):
    changes = edit.get("changes") or {}
    for dc in edit.get("documentChanges") or []:
        changes.setdefault(dc["textDocument"]["uri"], []).extend(dc["edits"])
    total = 0
    for u, edits in sorted(changes.items()):
        path = path_of(u)
        lines = open(path, encoding="latin1").read().split("\n")
        for e in sorted(edits, key=lambda e: (e["range"]["start"]["line"],
                                             e["range"]["start"]["character"]), reverse=True):
            s, t = e["range"]["start"], e["range"]["end"]
            assert s["line"] == t["line"], "multi-line rename edit"
            ln = lines[s["line"]]
            lines[s["line"]] = ln[:s["character"]] + e["newText"] + ln[t["character"]:]
        total += len(edits)
        if dry:
            print("  %s: %d" % (os.path.relpath(path, ROOT), len(edits)))
        else:
            with open(path, "w", encoding="latin1", newline="") as f:
                f.write("\n".join(lines))
    return len(changes), total, [path_of(u) for u in changes]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("-n", action="store_true", help="show the edits, write nothing")
    ap.add_argument("-f", help="file of OLD=NEW pairs")
    ap.add_argument("pairs", nargs="*")
    a = ap.parse_args()
    pairs = list(a.pairs)
    if a.f:
        pairs += [l for l in open(a.f).read().splitlines() if l.strip() and not l.startswith("#")]
    pairs = [tuple(re.split(r"[=\s]+", p.strip())[:2]) for p in pairs]
    files = sources()
    cd = Clangd()
    # clangd loads compile_commands.json, and so starts the background index, only once a
    # file is open
    first = [p for p in files if p.endswith(".c")][0]
    cd.open(first)
    print("indexing: %.0fs" % cd.wait_index(files=files), file=sys.stderr)
    failed = 0
    for old, new in pairs:
        occ = find_occurrence(old, files)
        if not occ:
            print("%s: not found" % old)
            failed += 1
            continue
        path, ln, col = occ
        cd.open(path)
        try:
            edit = cd.request("textDocument/rename", {
                "textDocument": {"uri": uri(path)},
                "position": {"line": ln, "character": col}, "newName": new})
        except RuntimeError as e:
            print("%s: clangd refused: %s" % (old, e))
            failed += 1
            cd.close(path)
            continue
        cd.close(path)
        nfiles, nedits, changed = apply_edit(edit or {}, a.n)
        if changed and not a.n:
            # tell clangd the files changed; it reads unopened files from disk and re-matches
            # its index ranges to their current text, so no re-index wait is needed
            cd.notify("workspace/didChangeWatchedFiles", {"changes": [
                {"uri": uri(p), "type": 2} for p in changed]})
        left = [p for p in files if re.search(r"\b%s\b" % re.escape(old),
                                              open(p, encoding="latin1").read())] if not a.n else []
        print("%s -> %s: %d edits in %d files%s" % (old, new, nedits, nfiles,
              "" if not left else "; STILL in %d files: %s" % (len(left), left[:3])))
        if left:
            failed += 1
    cd.request("shutdown", None)
    cd.notify("exit", None)
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
