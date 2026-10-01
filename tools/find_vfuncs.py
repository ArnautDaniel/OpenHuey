#!/usr/bin/env python3
"""Discover functions that are only reached through vtables / function tables.

spimdisasm finds functions from jal targets and code flow; a C++ virtual method
that is never called directly gets merged into the previous function, and the
vtable slot pointing at it stays a raw number (so it would not move on a shift).

Rule: a raw data word pointing into .text that sits next to (+-2 words) a word
relocated against a function symbol, and whose target directly follows nop
padding (i.e. a clean function boundary), is a function start. New starts are
appended to config/symbol_addrs.txt; re-run `configure.py --split` and this tool
until it reports nothing new.

Needs a current build (uses build/SLUS_210.75.elf relocations).
"""
import re
import struct
import subprocess
import sys
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

ELF = "build/SLUS_210.75.elf"
BASEROM = "baserom/SLUS_210.75"
TEXT_LO, TEXT_HI = 0x00100230, 0x003A1990
MARK = "// Functions reached only via vtables/function tables (tools/find_vfuncs.py)"


def main() -> None:
    rom = Path(BASEROM).read_bytes()

    def word(a: int) -> int:
        return struct.unpack_from("<I", rom, a - 0x100000 + 0x80)[0]

    elf = ELFFile(open(ELF, "rb"))
    symtab = elf.get_section_by_name(".symtab")
    func_relocs = set()
    for sec in elf.iter_sections():
        if isinstance(sec, RelocationSection):
            for r in sec.iter_relocations():
                if r["r_info_type"] == 2 and symtab.get_symbol(r["r_info_sym"])["st_info"]["type"] == "STT_FUNC":
                    func_relocs.add(r["r_offset"])

    res = subprocess.run(
        [sys.executable, "tools/ptrcheck.py", "--list", "data"], capture_output=True, text=True, check=True
    )
    found = set()
    for line in res.stdout.splitlines():
        m = re.match(r"\s+([0-9a-f]{8}) \.word 0x([0-9a-f]{8})", line)
        if not m:
            continue
        a, w = int(m.group(1), 16), int(m.group(2), 16)
        if not (TEXT_LO <= w < TEXT_HI) or w % 4:
            continue
        if not any(a + d in func_relocs for d in (-8, -4, 4, 8)):
            continue
        if word(w - 4) != 0:  # must follow padding/delay-slot nop
            continue
        found.add(w)

    p = Path("config/symbol_addrs.txt")
    text = p.read_text()
    known = {int(x, 16) for x in re.findall(r"= 0x([0-9A-Fa-f]+);", text)}
    new = sorted(found - known)
    if not new:
        print("find_vfuncs: nothing new")
        return
    if MARK not in text:
        text = text.rstrip("\n") + "\n\n" + MARK + "\n"
    text += "".join(f"func_{w:08X} = 0x{w:08X}; // type:func\n" for w in new)
    p.write_text(text)
    print(f"find_vfuncs: +{len(new)} functions")


if __name__ == "__main__":
    main()
