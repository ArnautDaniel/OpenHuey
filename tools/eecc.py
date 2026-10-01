#!/usr/bin/env python3
"""Compile C for the mixed C/asm game: GCC with the original's EABI, 16-byte stack frames.

    tools/eecc.py <gcc> <flags...> -c -o out.o in.c   (same arguments as gcc)

The original code is Metrowerks EABI-style (int and float args fill a0../f12.. in order),
so C is compiled with -mabi=eabi. But GCC's EABI only keeps the stack 8-byte aligned,
while the original code saves registers with `sq` (128-bit, ignores the low address
bits) and needs 16. GCC has no option for that, so: compile to assembly, round every
stack frame up to a multiple of 16 (prologue, epilogues, .frame, and accesses to the
caller's incoming-argument area above the frame), then assemble.
"""
import re
import subprocess
import sys
from pathlib import Path

FRAME_RE = re.compile(r"^(\s+addiu\s+\$sp,\$sp,)(-?\d+)\s*$")
SP_MEM_RE = re.compile(r"(-?\d+)\(\$sp\)")


def fix_function(lines: list[str]) -> list[str]:
    size = None
    for line in lines:
        m = FRAME_RE.match(line)
        if m and int(m.group(2)) < 0:
            size = -int(m.group(2))
            break
    if size is None or size % 16 == 0:
        return lines
    delta = 16 - size % 16
    new = size + delta
    out = []
    for line in lines:
        m = FRAME_RE.match(line)
        if m and abs(int(m.group(2))) == size:
            line = f"{m.group(1)}{-new if int(m.group(2)) < 0 else new}\n"
        elif re.match(r"^\s+\.frame\s+\$sp,", line):
            line = re.sub(r"(\.frame\s+\$sp,)(\d+)", lambda mm: f"{mm.group(1)}{new}", line)
        elif "($sp)" in line:
            # offsets >= the old frame size address the caller's area: shift them too
            line = SP_MEM_RE.sub(lambda mm: f"{int(mm.group(1)) + delta}($sp)" if int(mm.group(1)) >= size else mm.group(0), line)
        out.append(line)
    return out


def fixup(asm: str) -> str:
    lines = asm.splitlines(keepends=True)
    out, func = [], None
    for line in lines:
        if re.match(r"^\s+\.ent\s+", line):
            func = []
        if func is not None:
            func.append(line)
            if re.match(r"^\s+\.end\s+", line):
                out += fix_function(func)
                func = None
        else:
            out.append(line)
    return "".join(out)


def main() -> None:
    args = sys.argv[1:]
    gcc, flags = args[0], args[1:]
    out = flags[flags.index("-o") + 1]
    src = flags[-1]
    asm = Path(out + ".s")
    sflags = [f for f in flags if f != "-c"]
    sflags[sflags.index("-o") + 1] = str(asm)
    subprocess.run([gcc, "-S", *sflags], check=True)
    asm.write_text(fixup(asm.read_text()))
    # assemble with the same flags, minus the source and dependency options
    cleaned, skip = [], False
    for f in flags:
        if skip:
            skip = False
            continue
        if f in ("-MF", "-MT", "-MQ"):
            skip = True
            continue
        if f.startswith("-M") or f == src:
            continue
        cleaned.append(f)
    subprocess.run([gcc, *[c for c in cleaned], str(asm)], check=True)


if __name__ == "__main__":
    main()
