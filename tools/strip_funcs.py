#!/usr/bin/env python3
"""Copy an asm file without the functions that have been decompiled to C.

    tools/strip_funcs.py asm/game.s build/decomp/game.s func_A func_B ...

Removes each function's `nonmatching`/`glabel`...`endlabel` block (and the
`.balign` emitted by tools/align_data.py right before it). Jump table labels (jlabel) are kept, so the now-unused table in rodata still
links. Refuses if a removed block has alternate entry points (alabel/ehlabel) that aren't
themselves in the list of decompiled functions.
"""
import re
import sys
from pathlib import Path


def main() -> None:
    src, dst, *funcs = sys.argv[1:]
    want = set(funcs)
    out: list[str] = []
    found: set[str] = set()
    lines = Path(src).read_text().splitlines(keepends=True)
    i = 0
    while i < len(lines):
        line = lines[i]
        m = re.match(r"nonmatching (\S+), \w+$", line)
        if not (m and m.group(1) in want):
            out.append(line)
            i += 1
            continue
        name = m.group(1)
        if out and re.match(r"\.balign \d+$", out[-1]):
            out.pop()
        j = i
        while not lines[j].startswith(f"endlabel {name}\n"):
            j += 1
            if j == len(lines):
                sys.exit(f"strip_funcs: no endlabel for {name} in {src}")
        block = lines[i : j + 1]
        # alternate entry points must be decompiled too (C defines them as functions)
        inner = [l.split()[:2] for l in block if re.match(r"\s*(alabel|ehlabel) ", l)]
        missing_entries = [lbl for kind, lbl in inner if kind != "alabel" or lbl not in want]
        if missing_entries:
            sys.exit(f"strip_funcs: {name} has labels referenced from elsewhere: {missing_entries}")
        # Jump table targets: the table in rodata is unused once the C replaces the
        # function, but its entries must still resolve, so keep the labels (all at the
        # place the function was).
        out += [f"  jlabel {l.split()[1]}\n" for l in block if re.match(r"\s*jlabel ", l)]
        found.add(name)
        found.update(lbl for kind, lbl in inner if kind == "alabel")
        i = j + 1
    missing = want - found
    if missing:
        sys.exit(f"strip_funcs: not found in {src}: {sorted(missing)}")
    Path(dst).parent.mkdir(parents=True, exist_ok=True)
    Path(dst).write_text("".join(out))


if __name__ == "__main__":
    main()
