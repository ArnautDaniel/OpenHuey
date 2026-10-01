#!/usr/bin/env python3
"""Preserve data alignment when things move (run by configure.py after splat).

splat's asm only records where each object happened to sit. The original
compiler aligned some objects to 64 bytes (cache lines / DMA), and the game
breaks if they shift by a non-multiple (verified: a 16-byte shift renders a
black screen, 64-byte shifts work). Before every data/rodata/bss object we emit
`.balign A`, where A is the alignment of its original address, capped at
MAX_ALIGN and at the alignment of its file's start address (the file's
section alignment becomes the largest .balign in it, and the file must still
land on its original address in the matching build).

Functions in the code asm (game, sinit) that started on a 16-byte boundary get
`.balign 16` too: code embeds 16-byte constants read with `lq` (which ignores
the low address bits), so a function's offset mod 16 must be preserved.

configure.py also drops splat's SUBALIGN(16), which would override this.
"""
import re
from pathlib import Path

MAX_ALIGN = 64
VRAM_RE = re.compile(r"/\* [0-9A-F]+ ([0-9A-F]{8})")


def lowbit(a: int) -> int:
    return a & -a if a else 1 << 31


def main() -> None:
    total = 0
    for p in sorted(Path("asm/data").glob("*.s")):
        if p.name.startswith("elf_header"):
            continue
        lines = p.read_text().split("\n")
        start = next((int(m.group(1), 16) for l in lines if (m := VRAM_RE.search(l))), None)
        if start is None:
            continue
        cap = min(MAX_ALIGN, lowbit(start))
        out = []
        i = 0
        while i < len(lines):
            line = lines[i]
            if line.startswith("nonmatching ") and i + 2 < len(lines) and lines[i + 2].startswith("dlabel "):
                # find this object's address from its first data line
                addr = next(
                    (int(m.group(1), 16) for l in lines[i + 3 : i + 8] if (m := VRAM_RE.search(l))), None
                )
                if addr is not None:
                    a = min(cap, lowbit(addr))
                    if a >= 8:
                        out.append(f".balign {a}")
                        total += 1
            out.append(line)
            i += 1
        p.write_text("\n".join(out))
    for p in (Path("asm/game.s"), Path("asm/sinit.s")):
        lines = p.read_text().split("\n")
        out = []
        for i, line in enumerate(lines):
            if line.startswith("nonmatching ") and i + 2 < len(lines) and lines[i + 2].startswith("glabel "):
                addr = next(
                    (int(m.group(1), 16) for l in lines[i + 3 : i + 12] if (m := VRAM_RE.search(l))), None
                )
                if addr is not None and addr % 16 == 0:
                    out.append(".balign 16")
                    total += 1
            out.append(line)
        p.write_text("\n".join(out))
    print(f"align_data: {total} .balign directives")


if __name__ == "__main__":
    main()
