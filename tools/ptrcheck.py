#!/usr/bin/env python3
"""Find things that would break if code/data moved (shiftability audit).

Reads build/SLUS_210.75.elf (linked with --emit-relocs) and reports:
  data   - aligned words in .main data that hold an image address but have no reloc
  hi     - `lui` loading the upper half of an image address with no HI16 reloc
  pinned - relocations that resolve to absolute symbols (linker/undefined_*_auto.txt
           or ABS), i.e. addresses that will not follow a shift

Usage: tools/ptrcheck.py [--list KIND] [--limit N]
"""
import argparse
import re
import struct
from collections import Counter

import rabbitizer
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

ELF = "build/SLUS_210.75.elf"
LO, HI = 0x00100000, 0x01992000  # loaded image + BSS
R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16, R_MIPS_GPREL16 = 2, 4, 5, 6, 7


def load_ignored() -> set[int]:
    out = set()
    for line in open("config/ignore_addrs.txt"):
        m = re.match(r"\w+ = 0x([0-9A-Fa-f]+);.*ignore:True(?:.*size:0x([0-9A-Fa-f]+))?", line)
        if m:
            a = int(m.group(1), 16)
            out.update(range(a, a + int(m.group(2) or "1", 16)))
    return out


def pair_lo(blob: bytes, off: int, imm: int) -> int | None:
    """Follow a lui forward to the instruction consuming its register.

    Returns the full 32-bit value if it is used as an address (addiu/load/store),
    None if it is built as a constant (ori) or can't be paired.
    """
    (w,) = struct.unpack_from("<I", blob, off)
    rt = (w >> 16) & 31
    for o in range(off + 4, min(off + 4 * 24, len(blob) - 3), 4):
        (w2,) = struct.unpack_from("<I", blob, o)
        ins = rabbitizer.Instruction(w2, category=rabbitizer.InstrCategory.R5900)
        op = w2 >> 26
        rs, rt2 = (w2 >> 21) & 31, (w2 >> 16) & 31
        lo = ins.getProcessedImmediate() if ins.hasOperandAlias(rabbitizer.OperandType.cpu_immediate) else None
        if rs == rt and op == 0x0D:  # ori -> constant
            return None
        if rs == rt and lo is not None and (ins.doesLoad() or ins.doesStore() or op in (0x09, 0x19)):
            return ((imm << 16) + lo) & 0xFFFFFFFF
        if ins.modifiesRd() and ins.rd == rt or ins.modifiesRt() and rt2 == rt and not ins.doesStore():
            return None  # register overwritten (incl. addu base+reg -> offset use)
        if ins.isJump() and not ins.isJrRa() and not ins.doesLink():
            return None
    return None


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--list", choices=["data", "hi", "pinned"], help="print every hit")
    ap.add_argument("--limit", type=int, default=0)
    args = ap.parse_args()

    elf = ELFFile(open(ELF, "rb"))
    main = elf.get_section_by_name(".main")
    base, blob = main["sh_addr"], main.data()
    symtab = elf.get_section_by_name(".symtab")

    syms = [s for s in symtab.iter_symbols() if s.name and s["st_info"]["type"] != "STT_FILE"]
    # Symbols that are absolute (from undefined_*_auto.txt) cannot shift.
    # Hardware/kernel addresses outside the image are pinned on purpose, and
    # config/extra_syms.ld symbols are defined relative to a section.
    extra = set(re.findall(r"^(\w+)\s*=", open("config/extra_syms.ld").read(), re.M))
    abs_syms = {
        s.name
        for s in syms
        if s["st_shndx"] == "SHN_ABS"
        and LO <= s["st_value"] < HI
        and s.name not in extra | {"_gp", "_end"}
    }

    relocs: dict[int, tuple[int, str]] = {}
    for sec in elf.iter_sections():
        if isinstance(sec, RelocationSection) and sec.name.endswith(".main"):
            for r in sec.iter_relocations():
                name = symtab.get_symbol(r["r_info_sym"]).name
                relocs[r["r_offset"]] = (r["r_info_type"], name)

    ignored = load_ignored()
    text_end = next(s["st_value"] for s in syms if s.name == "main_TEXT_END")

    # Opaque blobs (assets/*.bin): contents are not pointers by construction.
    ends = {s.name[: -len("_bin_end")]: s["st_value"] for s in syms if s.name.endswith("_bin_end")}
    blobs = [(s["st_value"], ends[s.name]) for s in syms if s.name in ends]

    hits: dict[str, list[str]] = {"data": [], "hi": [], "pinned": []}
    for off in range(0, len(blob) - 3, 4):
        addr = base + off
        (w,) = struct.unpack_from("<I", blob, off)
        rel = relocs.get(addr)
        if rel and rel[1] in abs_syms:
            hits["pinned"].append(f"{addr:08x} type={rel[0]} -> {rel[1]}")
        if addr < text_end:
            if (w >> 26) == 0x0F and rel is None:  # lui
                imm = w & 0xFFFF
                if LO >> 16 <= imm <= HI >> 16:
                    full = pair_lo(blob, off, imm)
                    if full is not None and LO <= full < HI and full not in ignored:
                        hits["hi"].append(f"{addr:08x} lui 0x{imm:04x} -> 0x{full:08x}")
        elif LO <= w < HI and rel is None and not any(a <= addr < b for a, b in blobs):
            hits["data"].append(f"{addr:08x} .word 0x{w:08x}")

    for kind, lst in hits.items():
        print(f"{kind:7s} {len(lst):7d}")
    if args.list:
        lst = hits[args.list]
        if args.list == "pinned":
            c = Counter(l.split("-> ")[1] for l in lst)
            for name, n in c.most_common(args.limit or None):
                print(f"  {n:5d} {name}")
        else:
            for l in lst[: args.limit or None]:
                print(" ", l)


if __name__ == "__main__":
    main()
