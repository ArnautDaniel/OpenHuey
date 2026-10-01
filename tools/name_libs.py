#!/usr/bin/env python3
"""Name library functions automatically and append them to config/symbol_addrs.txt.

1. Syscall stubs (`addiu $v1, $zero, N; syscall; jr $ra`) are named from
   ps2sdk's syscallnr.h (negative N = interrupt-context variant, prefixed `i`
   when the header has no explicit entry).
2. Functions inside library ranges (config/libraries.txt) whose referenced
   strings name exactly one API function, e.g.
       "sceDbcCreateSocket: rpc error"            (Sony style: name + ':')
       "E9040813:'sj'is NULL.(ADXF_ReadSj32)"     (CRI style: name in parens)
       "E2012 mwPlyCreate:can't create SFD"       (CRI style: code + name + ':')
   A name claimed by more than one function is skipped (ambiguous).

Only `func_XXXXXXXX` symbols are renamed; existing names are kept. Re-run
`configure.py --split` afterwards. Prints what it would add with --dry-run.
"""
import argparse
import re
from collections import defaultdict
from pathlib import Path

INS = re.compile(r"/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/\s+([\w.]+)\s*(.*)$")
SYSCALLNR = Path("tools/ps2dev/ps2dev/ps2sdk/ee/include/syscallnr.h")
MARK = "// Library functions named by tools/name_libs.py (syscall stubs, API names from error strings)"
API = r"(?:sce|_sce|Sce|sif|Sif|mw|MW|ADX|adx|cv|CV|SFD|sfd|SJ|sj|PS2RNA|DTX|DVCI|AFS|ROFS|RSU|LSC|SVM|PL2|SUD|MPV|M2V)[A-Za-z0-9_]*"
PATTERNS = [
    re.compile(rf"^(?:E\w*\s+)?({API})\s*(?::|\(\))"),   # "sceFoo: ...", "E123 mwPlyCreate: ..."
    re.compile(rf"\(({API})\)\.?\s*(?:\\n)?$"),          # "... (ADXF_ReadSj32)"
]


def syscalls() -> dict[int, str]:
    out = {}
    for m in re.finditer(r"#define __NR_(\w+)\s+(-?0x[0-9A-Fa-f]+|-?\d+)", SYSCALLNR.read_text()):
        out.setdefault(int(m.group(2), 0), m.group(1))
    return out


def ranges() -> list[tuple[int, int]]:
    out = []
    for line in Path("config/libraries.txt").read_text().splitlines():
        if line.startswith("0x"):
            a, b = line.split()[:2]
            out.append((int(a, 16), int(b, 16)))
    return out


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


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--dry-run", action="store_true")
    args = ap.parse_args()

    sc = syscalls()
    libs = ranges()
    strs = strings()
    names: dict[int, str] = {}
    claims: dict[str, set[int]] = defaultdict(set)

    for p in [Path("asm/game.s")]:
        name, addr, body, refs = None, None, [], set()

        def finish() -> None:
            if name is None or not name.startswith("func_") or addr is None:
                return
            # 1. syscall stub
            ops = [(o, a) for o, a in body]
            if 3 <= len(ops) <= 4 and ops[0][0] == "addiu" and ops[0][1].startswith("$v1, $zero,") and ops[1][0] == "syscall":
                n = int(ops[0][1].split(",")[2], 0)
                nm = sc.get(n) or (("i" + sc[-n]) if -n in sc else None)
                if nm:
                    claims[nm].add(addr)
                    return
            # 2. API name from strings, library code only
            if not any(lo <= addr < hi for lo, hi in libs):
                return
            found = set()
            for r in refs:
                s = strs.get(r)
                if not s:
                    continue
                for pat in PATTERNS:
                    m = pat.search(s)
                    if m:
                        found.add(m.group(1))
            if len(found) == 1:
                claims[found.pop()].add(addr)

        for line in p.open():
            if line.startswith("glabel "):
                finish()
                name, addr, body, refs = line.split()[1], None, [], set()
                continue
            m = INS.search(line)
            if m and name:
                if addr is None:
                    addr = int(m.group(1), 16)
                body.append((m.group(2), m.group(3).strip()))
                refs.update(re.findall(r"%lo\((\w+)\)", m.group(3)))
        finish()

    for nm, addrs in claims.items():
        if len(addrs) == 1:
            names[addrs.pop()] = nm

    sym = Path("config/symbol_addrs.txt")
    text = sym.read_text()
    have_names = set(re.findall(r"^(\w+) =", text, re.M))
    have_addrs = {int(x, 16) for x in re.findall(r"^\w+ = 0x([0-9A-Fa-f]+);", text, re.M)}
    new = {a: n for a, n in names.items() if n not in have_names}
    lines = []
    for a, n in sorted(new.items()):
        if a in have_addrs:
            # replace an existing auto entry (e.g. vtable-discovered func_XXXXXXXX)
            text = re.sub(rf"^func_{a:08X} = 0x{a:08X}; // type:func\n", "", text, flags=re.M)
        lines.append(f"{n} = 0x{a:08X}; // type:func")
    print(f"name_libs: {len(new)} names ({sum(1 for n in new.values() if n in sc.values() or n[1:] in sc.values())} syscalls)")
    if args.dry_run:
        print("\n".join(lines))
        return
    if lines:
        if MARK not in text:
            text = text.rstrip("\n") + "\n\n" + MARK + "\n"
        text += "\n".join(lines) + "\n"
        sym.write_text(text)


if __name__ == "__main__":
    main()
