#!/usr/bin/env python3
"""List clean object boundaries for shift bisection.

A clean boundary is a 16-aligned data label that code addresses directly and
that starts right after the previous object ends, so splitting the data there
(an extra splat subsegment + a SHIFT_POINTS anchor in configure.py) and
inserting padding does not cut an object in half.

    tools/bisect_shift.py --list --lo 0x3F8200 --hi 0x408880
    tools/bisect_shift.py 0x400000      # nearest clean boundary
"""
import argparse
import bisect
import re
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def clean_boundaries() -> list[int]:
    """Data labels referenced directly by code that follow an enddlabel."""
    code = (ROOT / "asm/game.s").read_text() + (ROOT / "asm/sinit.s").read_text()
    referenced = set(re.findall(r"%hi\((D_[0-9A-F]{8})\)", code))
    out = []
    for p in sorted((ROOT / "asm/data").glob("*.data.s")):
        prev_end = True
        lines = p.read_text().splitlines()
        for i, line in enumerate(lines):
            if line.startswith("enddlabel"):
                prev_end = True
                continue
            m = re.match(r"dlabel (D_([0-9A-F]{8}))$", line)
            if m:
                if prev_end and m.group(1) in referenced and int(m.group(2), 16) % 16 == 0:
                    out.append(int(m.group(2), 16))
                prev_end = False
            elif line.strip() and not line.startswith(("nonmatching", ".")):
                if "/*" in line:
                    prev_end = False
    return sorted(out)


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("vrams", nargs="*", type=lambda x: int(x, 0))
    ap.add_argument("--list", action="store_true", help="just print clean boundaries in range")
    ap.add_argument("--lo", type=lambda x: int(x, 0), default=0x003AC780)
    ap.add_argument("--hi", type=lambda x: int(x, 0), default=0x0044A9A0)
    args = ap.parse_args()
    b = clean_boundaries()
    if args.list:
        sel = [x for x in b if args.lo <= x < args.hi]
        print(len(sel), "clean boundaries")
        for x in sel[:: max(1, len(sel) // 32)]:
            print(f"0x{x:08X}")
        return
    for v in args.vrams:
        i = bisect.bisect_left(b, v)
        print(f"0x{v:08X} -> nearest clean boundary 0x{b[min(i, len(b) - 1)]:08X}")


if __name__ == "__main__":
    main()
