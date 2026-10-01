#!/usr/bin/env python3
"""Survey code regions: compiler fingerprint + strings referenced, per address range.

    tools/libmap.py                    # GCC-compiled blocks with sample strings
    tools/libmap.py 0x26C630 0x2783E8  # strings referenced from a range

Metrowerks (game code) saves callee-saved registers with `sq`; GCC 2.96
(Sony SDK, CRI middleware) uses `sd`. Libraries are linked as contiguous blocks.
"""
import collections
import re
import sys
from pathlib import Path

INS = re.compile(r"/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/\s+([\w.]+)\s*(.*)$")
SAVED = {"$ra", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$fp", "$s8"}


def strings() -> dict[str, str]:
    out = {}
    for p in Path("asm/data").glob("*.s"):
        label = None
        for line in p.open():
            m = re.match(r"dlabel (\S+)", line)
            if m:
                label = m.group(1)
                continue
            m = re.search(r'\.asciz "(.*)"$', line)
            if m and label:
                out.setdefault(label, m.group(1))
                label = None
            elif line.startswith("enddlabel"):
                label = None
    return out


def functions():
    """yield (addr, name, style, referenced symbols)"""
    for p in [Path("asm/game.s")]:
        cur = None
        for line in p.open():
            if line.startswith("glabel "):
                if cur and cur[0] is not None:
                    yield cur
                cur = [None, line.split()[1], collections.Counter(), set()]
                continue
            m = INS.search(line)
            if not (m and cur):
                continue
            if cur[0] is None:
                cur[0] = int(m.group(1), 16)
            op, args = m.group(2), m.group(3)
            if args.endswith("($sp)") and op in ("sq", "sd") and args.split(",")[0] in SAVED:
                cur[2][op] += 1
            cur[3].update(re.findall(r"%lo\((\w+)\)", args))
        if cur and cur[0] is not None:
            yield cur


def main() -> None:
    strs = strings()
    funcs = list(functions())
    if len(sys.argv) == 3:
        lo, hi = int(sys.argv[1], 0), int(sys.argv[2], 0)
        seen = []
        for a, n, c, refs in funcs:
            if lo <= a < hi:
                for r in sorted(refs):
                    if r in strs and strs[r] not in seen:
                        seen.append(strs[r])
                        print(f"{a:08x} {n}: {strs[r][:100]}")
        return
    runs = []
    for a, n, c, refs in funcs:
        style = "MW" if c["sq"] else "GCC" if c["sd"] else None
        if style is None:
            continue
        if runs and runs[-1][2] == style:
            runs[-1][1] = a
            runs[-1][3] += 1
        else:
            runs.append([a, a, style, 1])
    for lo, hi, style, n in runs:
        if style == "GCC" and n >= 8:
            ss = [strs[r] for a, _, _, refs in funcs if lo <= a <= hi for r in refs if r in strs]
            print(f"{lo:08x}-{hi:08x} GCC n={n}: " + " | ".join(dict.fromkeys(s[:40] for s in ss)) if ss else f"{lo:08x}-{hi:08x} GCC n={n}: (no strings)")
            print()


if __name__ == "__main__":
    main()
