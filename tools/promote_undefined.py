#!/usr/bin/env python3
"""Promote image addresses left in linker/undefined_syms_auto.txt to real symbols.

splat leaves a reference undefined when the target has no label, e.g. when
code in one segment points into another segment's data, or at an odd offset
into a table. An undefined symbol becomes an absolute address in the link and
won't move, so we declare it in config/symbol_addrs.txt instead. The type comes
from how the code accesses it, which makes spimdisasm split the data there.

Re-run `configure.py --split` afterwards.
"""
import re
from pathlib import Path

LO, HI = 0x00100000, 0x01992000
WIDTH = {
    "lb": "s8", "lbu": "u8", "sb": "u8",
    "lh": "s16", "lhu": "u16", "sh": "u16",
    "lw": "s32", "lwu": "u32", "sw": "s32", "lwc1": "f32", "swc1": "f32",
    "ld": "s64", "sd": "s64", "lq": "s128", "sq": "s128",
}
ALIGN = {"s8": 1, "u8": 1, "s16": 2, "u16": 2, "s32": 4, "u32": 4, "f32": 4, "s64": 8, "s128": 16}


def main() -> None:
    # build/ copy is already filtered against labels the asm defines (configure.py)
    syms = []
    for line in Path("build/undefined_syms_auto.txt").read_text().splitlines():
        m = re.match(r"(\w+) = 0x([0-9A-Fa-f]+);", line)
        if m and LO <= int(m.group(2), 16) < HI:
            syms.append((m.group(1), int(m.group(2), 16)))
    if not syms:
        print("nothing to promote")
        return

    asm = "".join(p.read_text() for p in Path("asm").glob("*.s"))
    existing = Path("config/symbol_addrs.txt").read_text()
    out = ["", "// Promoted by tools/promote_undefined.py (references with no label)"]
    for name, addr in syms:
        if re.search(rf"^{name} =", existing, re.M):
            continue
        i = max(asm.find(f"%lo({name})"), asm.find(f"%gp_rel({name})"))
        t = "u8"
        if i >= 0:
            line_start = asm.rfind("\n", 0, i) + 1
            m = re.search(r"\*/\s+([\w.]+)\s", asm[line_start:i])
            if m and m.group(1) in WIDTH:
                t = WIDTH[m.group(1)]
            else:  # address formed with addiu: look at the next dereference
                m = re.search(r"\*/\s+(l[bhwdq]u?|s[bhwdq]|lwc1|swc1)\s", asm[i : i + 600])
                t = WIDTH.get(m.group(1), "u8") if m else "u8"
        if addr % 4 == 0:
            # Aligned: just a label; let spimdisasm infer (a forced type would
            # stop it from seeing pointers in the object).
            out.append(f"{name} = 0x{addr:08X};")
            continue
        if addr % ALIGN[t]:
            t = "u8"
        out.append(f"{name} = 0x{addr:08X}; // type:{t}")
    with open("config/symbol_addrs.txt", "a") as f:
        f.write("\n".join(out) + "\n")
    print(f"promoted {len(out) - 2} symbols")


if __name__ == "__main__":
    main()
