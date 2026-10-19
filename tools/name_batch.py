#!/usr/bin/env python3
"""Apply one batch of global or function names: names.csv rows, clangd renames, symbols.txt.

Batch file lines:  D_XXXXXXXX | func_XXXXXXXX  new_name  confidence  evidence text...
(# starts a comment line). A name already used by another identifier is refused. Rows for an
address already in names.csv are replaced, others appended; both through the hashline CLI. An
evidence of "=" keeps the existing row's evidence (raising a reviewed candidate as it is).
Then tools/lsp_rename.py renames in the sources and apply_names.py symbols adds the entries.
Run tools/build-and-verify.sh afterwards.

usage: name_batch.py [-n] BATCH
"""
import csv
import io
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PY = os.path.join(ROOT, ".venv", "bin", "python")
NAMES = "config/names.csv"
IDENT = re.compile(r"\b[A-Za-z_]\w*\b")
LINE = re.compile(r"^((?:D|func)_[0-9A-F]{8})\s+([a-z_][a-z0-9_]*)\s+(confirmed|strong|candidate)\s+(.+)$")


def identifiers():
    seen = set()
    for top in ("src", "include", "port"):
        for d, _, fs in os.walk(os.path.join(ROOT, top)):
            for f in fs:
                if f.endswith((".c", ".h")):
                    seen.update(IDENT.findall(open(os.path.join(d, f), errors="replace").read()))
    return seen


def hashline(*args):
    r = subprocess.run(["hashline"] + list(args), cwd=ROOT, capture_output=True, text=True)
    if r.returncode or not r.stdout.startswith(("OK", "[")):
        raise SystemExit("hashline %s: %s%s" % (args[0], r.stdout[:300], r.stderr[:300]))
    return r.stdout


def row(kind, addr, name, conf, ev):
    out = io.StringIO()
    csv.writer(out, lineterminator="").writerow([kind, addr, name, conf, ev])
    return out.getvalue()


def kind(ident):
    return "func" if ident.startswith("func_") else "global"


def main():
    dry = "-n" in sys.argv
    path = [a for a in sys.argv[1:] if a != "-n"][0]
    batch = []
    for ln in open(path):
        ln = ln.strip()
        if not ln or ln.startswith("#"):
            continue
        m = LINE.match(ln)
        if not m:
            raise SystemExit("bad line: " + ln)
        batch.append(m.groups())
    taken = identifiers()
    with open(os.path.join(ROOT, NAMES), newline="") as f:
        rows = list(csv.DictReader(f))
    taken.update(r["name"] for r in rows)
    # a candidate row for the same address may already carry the name (and comments mention it)
    mine = {(kind(b[0]), "0X" + b[0][-8:]) for b in batch}
    taken -= {r["name"] for r in rows if (r["kind"], r["address"].upper()) in mine}
    by_addr = {(r["kind"], r["address"].upper()): r for r in rows}
    names = [b[1] for b in batch]
    bad = sorted({n for n in names if names.count(n) > 1 or n in taken})
    if bad:
        raise SystemExit("names already used: " + " ".join(bad))
    if dry:
        print("%d names OK" % len(batch))
        return
    listing = hashline("read", NAMES).splitlines()
    header, lines = listing[0], listing[1:]
    anchor = {}
    for l in lines:
        num_hash, _, text = l.partition("|")
        if text.startswith(("global,", "func,")):
            anchor[(text.split(",")[0], text.split(",")[1].upper())] = num_hash
    ops, tail = [], []
    for ident, name, conf, ev in batch:
        addr = "0x" + ident[-8:]
        old = by_addr.get((kind(ident), addr.upper()))
        if ev == "=":
            if old is None:
                raise SystemExit("%s: no row to keep the evidence of" % ident)
            ev = old["evidence"]
        new = row(kind(ident), addr, name, conf, ev)
        if (kind(ident), addr.upper()) in by_addr:
            ops.append("SWAP %s\n+%s" % (anchor[(kind(ident), addr.upper())], new))
        else:
            tail.append("+" + new)
    if tail:
        ops.append("INS.TAIL:\n" + "\n".join(tail))
    hashline("patch", NAMES, header + "\n" + "\n".join(ops))
    ren = os.path.join(ROOT, "build", "names", "batch_renames.txt")
    with open(ren, "w") as f:
        f.writelines("%s=%s\n" % (b[0], b[1]) for b in batch)
    r = subprocess.run([PY, "-I", "tools/lsp_rename.py", "-f", ren], cwd=ROOT,
                       capture_output=True, text=True)
    out = r.stdout + r.stderr
    problems = [l for l in out.splitlines() if "STILL" in l or "refused" in l or "0 edits" in l]
    print("\n".join(problems) or "renamed %d" % len(batch))
    subprocess.run([PY, "-I", "tools/apply_names.py", "symbols"], cwd=ROOT, check=True)


if __name__ == "__main__":
    main()
