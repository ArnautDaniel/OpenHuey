#!/usr/bin/env python3
"""First-draft C for a function via m2c.

    tools/decomp.py func_002D1E40 [--context include/ctx.h] [-- extra m2c args]

Extracts the function from asm/, normalizes it for m2c (n32 -> o32 register
names, numeric %hi/%lo operands produced by tools/offpatch.py) and runs m2c
with the Emotion Engine target.
"""
import argparse
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
M2C = ROOT / "tools/m2c/m2c.py"

# n32 names for $8-$15 -> o32 names (what m2c expects)
N32_TO_O32 = {
    "$a4": "$t0", "$a5": "$t1", "$a6": "$t2", "$a7": "$t3",
    "$t0": "$t4", "$t1": "$t5", "$t2": "$t6", "$t3": "$t7",
}
REG_RE = re.compile(r"\$(a[4-7]|t[0-3])\b")


def hi(v: int) -> int:
    return ((v + 0x8000) >> 16) & 0xFFFF


def lo(v: int) -> int:
    v &= 0xFFFF
    return v - 0x10000 if v & 0x8000 else v


def normalize(line: str) -> str:
    # comment out the "/* rom vram bytes */" prefix is fine for m2c; only fix operands
    line = re.sub(r"%hi\((0x[0-9A-Fa-f]+)\)", lambda m: f"0x{hi(int(m.group(1), 16)):X}", line)
    line = re.sub(r"%lo\((0x[0-9A-Fa-f]+)\)", lambda m: str(lo(int(m.group(1), 16))), line)
    return REG_RE.sub(lambda m: N32_TO_O32["$" + m.group(1)], line)


def extract(name: str) -> str:
    for p in sorted((ROOT / "asm").glob("*.s")):
        text = p.read_text()
        m = re.search(rf"^glabel {re.escape(name)}\n.*?^endlabel {re.escape(name)}\n", text, re.M | re.S)
        if m:
            return m.group(0)
    sys.exit(f"{name} not found in asm/")


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("func")
    ap.add_argument("--context", help="C header with types/prototypes for m2c")
    ap.add_argument("extra", nargs="*", help="extra m2c arguments (after --)")
    args = ap.parse_args()

    func_asm = extract(args.func)
    body = "\n".join(normalize(l) for l in func_asm.splitlines())
    src = f".set noat\n.set noreorder\n.section .text\n{body}\n"
    # jump tables the function uses (m2c needs them to decompile switches)
    tables = sorted(set(re.findall(r"%lo\((jtbl_[0-9A-F]+)\)", func_asm)))
    if tables:
        data = "".join(p.read_text() for p in sorted((ROOT / "asm/data").glob("*.s")))
        src += ".section .rodata\n"
        for t in tables:
            m = re.search(rf"^dlabel {t}\n.*?^enddlabel {t}\n", data, re.M | re.S)
            if m:
                src += m.group(0)
    with tempfile.NamedTemporaryFile("w", suffix=".s", delete=False) as f:
        f.write(src)
    cmd = [sys.executable, str(M2C), "-t", "mipsee-gcc-c", "--valid-syntax"]
    if args.context:
        cmd += ["--context", args.context]
    subprocess.run(cmd + args.extra + [f.name], check=False)


if __name__ == "__main__":
    main()
