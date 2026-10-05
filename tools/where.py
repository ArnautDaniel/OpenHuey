#!/usr/bin/env python3
"""Which source file a function at an address belongs in: the decompiled functions nearest
below and above it, with their files.

    tools/where.py func_0025F810 [0x260BB0 ...] [-n 2]
"""
import argparse
import bisect
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DEF = re.compile(r"^(?!static\b|typedef\b|extern\b)[A-Za-z_][\w \t\*]*?\bfunc_([0-9A-F]{8})\s*\([^;{}]*\)\s*\{", re.M)
THUNK = re.compile(r"^THUNKV?\(\s*func_([0-9A-F]{8})", re.M)


def defined() -> list[tuple[int, str]]:
    out = []
    for p in (ROOT / "src").rglob("*.c"):
        t = p.read_text(errors="replace")
        rel = str(p.relative_to(ROOT))
        out += [(int(a, 16), rel) for a in DEF.findall(t) + THUNK.findall(t)]
    return sorted(out)


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("addrs", nargs="+")
    ap.add_argument("-n", type=int, default=2, help="neighbours on each side")
    a = ap.parse_args()
    defs = defined()
    keys = [d[0] for d in defs]
    for s in a.addrs:
        addr = int(s.removeprefix("func_"), 16)
        i = bisect.bisect_left(keys, addr)
        below = defs[max(0, i - a.n):i]
        above = defs[i:i + a.n]
        print(f"func_{addr:08X}:")
        for d, f in below:
            print(f"  -0x{addr - d:<6X} func_{d:08X}  {f}")
        for d, f in above:
            print(f"  +0x{d - addr:<6X} func_{d:08X}  {f}")


if __name__ == "__main__":
    main()
