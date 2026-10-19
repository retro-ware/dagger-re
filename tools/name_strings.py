#!/usr/bin/env python3
"""Name the D_ globals that are string literals after their text.

usage: name_strings.py RENAMES_OUT ROWS_OUT

A D_ global declared `char []` that the game never writes and whose bytes in FALL.EXE are a
printable NUL-terminated string is a string literal the matching build keeps at its address.
Its name is `str_` + a slug of the text ("pris00i0.img" -> str_pris00i0_img, "%d days until
freedom." -> str_days_until_freedom; printf specifiers are left out); a text with no letters or
digits besides them is named by its format characters (str_fmt_ld). Names are made unique
against every identifier in the sources
and config/names.csv (a numeric suffix).

Writes the OLD=NEW list for tools/lsp_rename.py and the config/names.csv rows (confidence
confirmed, evidence = the text).
"""
import csv
import glob
import json
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from le import LE  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
IDENT = re.compile(r"\b[A-Za-z_][A-Za-z0-9_]*\b")
MAXLEN = 40
FORMAT = re.compile(r"%[-+ #0-9.*]*(l?[a-zA-Z])")


def slug(text):
    words = re.findall(r"[a-z0-9]+", FORMAT.sub(" ", text).lower())
    out = ""
    for w in words:
        if len(out) + len(w) + 1 > MAXLEN and out:
            break
        out = out + "_" + w if out else w
    if out:
        return out
    fmt = FORMAT.findall(text)
    if fmt:
        return "fmt_" + "_".join(f.lower() for f in fmt)[:MAXLEN]
    return None


def main():
    renames_out, rows_out = sys.argv[1:3]
    le = LE(os.path.join(ROOT, "orig", "1.07.213", "FALL.EXE"))
    img = le.load()
    ev = json.loads(subprocess.run([sys.executable, "-I", os.path.join(ROOT, "tools", "global_evidence.py"),
                                    "--json"], capture_output=True, text=True, check=True).stdout)
    taken = set()
    for p in glob.glob(os.path.join(ROOT, "src", "**", "*.[ch]"), recursive=True) + \
            glob.glob(os.path.join(ROOT, "include", "*.h")):
        taken.update(IDENT.findall(open(p, encoding="latin1").read()))
    taken.update(r["name"] for r in csv.DictReader(open(os.path.join(ROOT, "config", "names.csv"))))

    renames, rows = [], []
    for unit, recs in ev.items():
        for r in recs:
            t = "|".join(r["types"])
            if r["writes"] or "char" not in t or "[]" not in t:
                continue
            va = int(r["global"][2:], 16)
            o = le.obj_of_va(va)
            buf = img[o.index]
            i = va - o.base
            raw = bytes(buf[i:buf.index(0, i)])
            if not raw or not all(32 <= c < 127 or c in (9, 10, 13) for c in raw):
                continue
            text = raw.decode("ascii")
            s = slug(text)
            if not s:
                continue
            name = "str_" + s
            k = 2
            while name in taken:
                name = "str_%s_%d" % (s, k)
                k += 1
            taken.add(name)
            renames.append("%s=%s" % (r["global"], name))
            ev_text = "String literal %s, used by %s." % (json.dumps(text)[:160],
                                                         ", ".join(r["funcs"]) or unit)
            rows.append(["global", "0x%08X" % va, name, "confirmed", ev_text])
    renames.sort()
    rows.sort(key=lambda x: x[1])
    open(renames_out, "w").write("\n".join(renames) + "\n")
    with open(rows_out, "w", newline="") as f:
        csv.writer(f, lineterminator="\n").writerows(rows)
    print("%d string literals named" % len(rows))


if __name__ == "__main__":
    main()
