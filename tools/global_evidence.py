#!/usr/bin/env python3
"""Evidence for naming the globals the game's C still names by address (D_XXXXXXXX).

usage: global_evidence.py [--unit UNIT] [--min-uses N] [--json] > out

For every D_ global in src/lifted, src/hand and src/*.c: its declared C type, the units and
functions that use it (with the functions' names), how often it is written vs read, a few
source lines, the names.csv candidate if any, and its named neighbours by address (globals
next to each other usually belong to the same unit and structure). Grouped by the unit that
uses it most, so a unit's globals can be named together.
"""
import argparse
import csv
import glob
import json
import os
import re
import collections

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
D = re.compile(r"\bD_([0-9A-F]{8})\b")
FUNC = re.compile(r"^(?:static\s+)?[A-Za-z_][\w\s\*]*?\b(\w+)\s*\([^;]*$")
EXTERN = re.compile(r"^extern\s+(.+?)\s*\b(D_[0-9A-F]{8})\b\s*(\[[^\]]*\])?\s*;")


def unit_of(path, _units=[]):
    """The original source unit: the file name, or for src/hand/func_ADDR.c the unit
    config/units.csv places that address in (tools/units.py)."""
    import sys
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import units
    base = os.path.basename(path)
    m = re.match(r"func_([0-9A-F]{8})\.c$", base)
    if "/hand/" in path and m:
        if not _units:
            _units.extend(units.load_units())
        return re.sub(r"\.c$", "", units.unit_of(int(m.group(1), 16), _units)[0])
    return os.path.splitext(base)[0]


def write_use(line, name):
    """True when the line assigns to the global (=, op=, ++/--), not just compares."""
    n = re.escape(name)
    return bool(re.search(r"\b%s\b(\s*\[[^\]]*\])*\s*([-+*/&|^]|<<|>>)?=(?!=)" % n, line) or
                re.search(r"(\+\+|--)\s*\b%s\b|\b%s\b\s*(\+\+|--)" % (n, n), line))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--unit")
    ap.add_argument("--min-uses", type=int, default=0)
    ap.add_argument("--json", action="store_true")
    a = ap.parse_args()

    names = {}
    for r in csv.DictReader(open(os.path.join(ROOT, "config", "names.csv"))):
        if r["kind"] == "global":
            names[int(r["address"], 16)] = r
    symbols = {}
    for ln in open(os.path.join(ROOT, "config", "symbols.txt")):
        m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);\s*(\w*)", ln)
        if m:
            symbols[int(m.group(2), 16)] = (m.group(1), m.group(3))

    info = collections.defaultdict(lambda: {"types": set(), "units": collections.Counter(),
                                            "funcs": collections.Counter(), "writes": 0,
                                            "reads": 0, "lines": []})
    files = [p for g in ("src/lifted/*.c", "src/hand/*.c", "src/*.c")
             for p in glob.glob(os.path.join(ROOT, g))]
    for path in sorted(files):
        unit = unit_of(path)
        func = None
        for line in open(path, encoding="latin1"):
            m = EXTERN.match(line)
            if m:
                info[m.group(2)]["types"].add((m.group(1) + (" []" if m.group(3) else "")).strip())
                continue
            if line[:1].isalpha() and not line.rstrip().endswith(";"):
                fm = FUNC.match(line)
                if fm:
                    func = fm.group(1)
            for g in set("D_" + x for x in D.findall(line)):
                e = info[g]
                e["units"][unit] += 1
                if func:
                    e["funcs"][func] += 1
                if write_use(line, g):
                    e["writes"] += 1
                else:
                    e["reads"] += 1
                if len(e["lines"]) < 6:
                    e["lines"].append("%s: %s" % (func or unit, line.strip()[:150]))

    named = sorted(symbols.items())
    addrs = [x[0] for x in named]
    import bisect
    out = collections.defaultdict(list)
    for g, e in info.items():
        uses = e["reads"] + e["writes"]
        if uses < a.min_uses:
            continue
        addr = int(g[2:], 16)
        i = bisect.bisect_left(addrs, addr)
        neigh = [("%X" % named[j][0], named[j][1][0]) for j in range(max(0, i - 3), min(len(named), i + 3))
                 if named[j][0] != addr]
        main_unit = e["units"].most_common(1)[0][0] if e["units"] else "?"
        if a.unit and main_unit != a.unit:
            continue
        cand = names.get(addr)
        out[main_unit].append({
            "global": g, "types": sorted(e["types"]), "uses": uses, "writes": e["writes"],
            "units": dict(e["units"].most_common(6)), "funcs": dict(e["funcs"].most_common(6)),
            "candidate": (cand["name"], cand["confidence"], cand["evidence"][:200]) if cand else None,
            "neighbours": neigh, "lines": e["lines"]})
    if a.json:
        json.dump(out, open(1, "w"), indent=1)
        return
    for unit in sorted(out, key=lambda u: -len(out[u])):
        print("## %s (%d)" % (unit, len(out[unit])))
        for r in sorted(out[unit], key=lambda r: r["global"]):
            print("%s  %s  uses %d (w %d)  units %s" % (r["global"], "|".join(r["types"]), r["uses"],
                                                       r["writes"], r["units"]))
            print("   funcs %s" % r["funcs"])
            if r["candidate"]:
                print("   candidate %s" % (r["candidate"],))
            print("   near %s" % r["neighbours"])
            for l in r["lines"][:4]:
                print("     " + l)


if __name__ == "__main__":
    main()
