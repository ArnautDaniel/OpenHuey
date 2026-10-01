#!/usr/bin/env python3
"""Find stale pointers by comparing EE RAM of the normal and shifted builds.

The shift build inserts SHIFT bytes after crt0, so everything from 0x00100230
up (code, data, BSS and the heap after _end) lives SHIFT bytes higher. At the
same point in time, a word holding a pointer V in the normal run should hold
V + SHIFT at the shifted location. A word that still holds V (unchanged) points
at the wrong place in the shifted run: a pointer that was never relocated.

Inputs are eeMemory.bin files taken from PCSX2 save states (.p2s are zips).

    tools/ramdiff.py base/eeMemory.bin shift/eeMemory.bin [--shift 0x1000]

Prints suspects grouped by where they are stored (static image vs BSS/heap)
and, for static ones, the containing asm label so they can go into
config/pointers.txt.
"""
import argparse
import bisect
import re
import struct
from collections import Counter
from pathlib import Path

LO = 0x00100230          # first shifted address
DATA_LO = 0x003A1B80     # code ends here; code words are covered by relocations
BLOBS = [(0x003A1B80, 0x003AC780), (0x0044A9A0, 0x0044AD00)]  # VU microcode, cdvdman IRX
IMAGE_END = 0x0047B200   # end of loaded image (start of BSS) in the normal build
RAM_TOP = 0x01FF0000     # ignore the stack area at the top of RAM


def labels() -> tuple[list[int], list[str]]:
    out: dict[int, str] = {}
    for p in Path("asm").rglob("*.s"):
        pending = None
        for line in p.open():
            m = re.match(r"\s*(?:glabel|dlabel|alabel|jlabel)\s+(\S+)", line)
            if m:
                pending = m.group(1)
                continue
            if pending:
                v = re.search(r"/\* [0-9A-F]+ ([0-9A-F]{8})", line)
                if v:
                    out.setdefault(int(v.group(1), 16), pending)
                    pending = None
    a = sorted(out)
    return a, [out[x] for x in a]


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("base")
    ap.add_argument("shifted")
    ap.add_argument("--shift", type=lambda x: int(x, 0), default=0x1000)
    ap.add_argument("--all", action="store_true", help="list every suspect")
    args = ap.parse_args()
    sh = args.shift
    base = Path(args.base).read_bytes()
    shf = Path(args.shifted).read_bytes()

    la, ln = labels()
    exact = set(la)
    static, dynamic = [], []
    for a in range(LO, RAM_TOP - sh, 4):
        (v,) = struct.unpack_from("<I", base, a)
        if not (LO <= v < RAM_TOP) or v % 4:
            continue
        (w,) = struct.unpack_from("<I", shf, a + sh)
        if w != v:  # pointers should read v + sh here
            continue
        if a < IMAGE_END:
            if a >= DATA_LO and not any(x <= a < y for x, y in BLOBS):
                static.append((a, v))
        elif v in exact:  # runtime data: only trust exact label hits
            dynamic.append((a, v))

    print(f"static image suspects: {len(static)}   bss/heap suspects: {len(dynamic)}")
    for a, v in static if args.all else static[:60]:
        i = bisect.bisect_right(la, a) - 1
        where = f"{ln[i]}+0x{a - la[i]:X}" if i >= 0 else "?"
        j = bisect.bisect_right(la, v) - 1
        what = f"{ln[j]}+0x{v - la[j]:X}" if j >= 0 else "?"
        print(f"  0x{a:08X} ({where}) = 0x{v:08X} -> {what}")
    c = Counter(v for _, v in dynamic)
    print("most common stale values in BSS/heap:")
    for v, n in c.most_common(25):
        j = bisect.bisect_right(la, v) - 1
        print(f"  {n:5d} x 0x{v:08X} -> {ln[j]}+0x{v - la[j]:X}" if j >= 0 else f"  {n:5d} x 0x{v:08X}")


if __name__ == "__main__":
    main()
