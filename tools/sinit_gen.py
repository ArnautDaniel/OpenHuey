#!/usr/bin/env python3
"""One-off generator: C for the straight-line static initialisers in asm/sinit.s.

Most of the MW __sinit_* functions just copy constants (mostly 12-byte PTMFs) from .data into
.bss globals. This turns each straight-line one (loads / stores / constants only) into copies,
merges consecutive pieces of the same object, and prints C: `dst = src;` for whole PTMFs,
memcpy() / a constant store otherwise. Functions it can't translate are listed on stderr.

    tools/sinit_gen.py > src/game/sinit.c   (then edit by hand; verify with tools/difftest.py)
"""
import re
import sys
from collections import OrderedDict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SIZES = {"ld": 8, "sd": 8, "lw": 4, "sw": 4, "lwc1": 4, "swc1": 4, "lh": 2, "lhu": 2, "sh": 2,
         "lb": 1, "lbu": 1, "sb": 1}


def bodies():
    out, cur = OrderedDict(), None
    for line in (ROOT / "asm/sinit.s").read_text().splitlines():
        m = re.match(r"\s*glabel (\S+)", line)
        if m:
            cur = m.group(1)
            out[cur] = []
            continue
        if line.startswith("endlabel"):
            cur = None
            continue
        if cur is None:
            continue
        m = re.match(r"\s+/\* \w+ \w+ \w+ \*/\s+(\S+)\s*(.*)", line)
        if m:
            out[cur].append((m.group(1), [x.strip() for x in m.group(2).split(",")] if m.group(2) else []))
        elif line.strip().startswith(".L"):
            out[cur].append(("LABEL", []))
    return out


def mem(arg, regs):
    """'%lo(S)(base)' / 'off(base)' -> (sym, off)"""
    m = re.match(r"(%lo\((\w+)\)|-?0x[0-9A-Fa-f]+|-?\d+)\((\$\w+)\)$", arg)
    if not m:
        raise ValueError(arg)
    base = regs.get(m.group(3))
    if m.group(2):
        if base != ("hi", m.group(2)):
            raise ValueError("lo without hi")
        return m.group(2), 0
    if not base or base[0] != "addr":
        raise ValueError("bad base")
    return base[1], base[2] + int(m.group(1), 0)


def translate(ins):
    regs = {"$zero": ("const", 0)}
    ops = []   # ("copy", dst, doff, src, soff, n) / ("const", dst, doff, n, value)
    for op, a in ins:
        if op in ("nop", "jr"):
            continue
        if op == "lui":
            m = re.match(r"%hi\((\w+)\)", a[1])
            regs[a[0]] = ("hi", m.group(1)) if m else ("const", (int(a[1].strip("()").split(">>")[0], 0) if ">>" in a[1] else int(a[1], 0) << 16) & 0xFFFFFFFF)
            if ">>" in a[1]:   # (0x3F800000 >> 16)
                regs[a[0]] = ("const", int(a[1].strip("()").split(">>")[0], 0) & 0xFFFF0000)
        elif op in ("addiu", "ori"):
            src = regs.get(a[1])
            m = re.match(r"%lo\((\w+)\)", a[2])
            if m and src == ("hi", m.group(1)):
                regs[a[0]] = ("addr", m.group(1), 0)
            elif src and src[0] == "const":
                v = int(a[2].strip("()").split("&")[0], 0) if "&" in a[2] else int(a[2], 0)
                if "&" in a[2]:
                    v &= 0xFFFF
                regs[a[0]] = ("const", (src[1] | v) if op == "ori" else (src[1] + v) & 0xFFFFFFFF)
            elif src and src[0] == "addr" and op == "addiu":
                regs[a[0]] = ("addr", src[1], src[2] + int(a[2], 0))
            else:
                raise ValueError(f"{op} {a}")
        elif op == "daddu" and a[2] == "$zero":
            regs[a[0]] = regs[a[1]]
        elif op in ("ld", "lw", "lwc1", "lh", "lhu", "lb", "lbu"):
            s, o = mem(a[1], regs)
            regs[a[0]] = ("load", s, o, SIZES[op])
        elif op in ("sd", "sw", "swc1", "sh", "sb"):
            d, o = mem(a[1], regs)
            v, n = regs.get(a[0]), SIZES[op]
            if v and v[0] == "load" and v[3] == n:
                ops.append(("copy", d, o, v[1], v[2], n))
            elif v and v[0] == "const":
                ops.append(("const", d, o, n, v[1]))
            else:
                raise ValueError(f"store of {v}")
        else:
            raise ValueError(op)
    # merge consecutive copies by address (the disassembly labels every referenced field;
    # within one data file the native layout is the PS2 one)
    def span(sym, off, n):
        return addr(sym) + off, addr(sym) + off + n
    dst = [span(x[1], x[2], x[5] if x[0] == "copy" else x[3]) for x in ops]
    src = [span(x[3], x[4], x[5]) for x in ops if x[0] == "copy"]
    if not any(a < d and c < b for a, b in src for c, d in dst):   # independent: order by destination
        ops = sorted(ops, key=lambda x: addr(x[1]) + x[2])
    merged = []
    for x in ops:
        p = merged[-1] if merged else None
        if (p and x[0] == "copy" and p[0] == "copy"
                and addr(p[1]) + p[2] + p[5] == addr(x[1]) + x[2]
                and addr(p[3]) + p[4] + p[5] == addr(x[3]) + x[4]):
            merged[-1] = ("copy", p[1], p[2], p[3], p[4], p[5] + x[5])
        else:
            merged.append(x)
    return merged


SYMS = {}
for _l in (ROOT / "config/symbol_addrs.txt").read_text().splitlines():
    _m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);", _l)
    if _m:
        SYMS[_m.group(1)] = int(_m.group(2), 16)


def addr(sym):
    if re.fullmatch(r"D_[0-9A-F]{8}", sym):
        return int(sym[2:], 16)
    return SYMS[sym]


def main():
    ok, fail = {}, []
    order = [l.split()[-1] for l in (ROOT / "asm/data/sinit_ctors.data.s").read_text().splitlines()
             if ".word" in l] + [l.split()[-1] for l in (ROOT / "asm/data/sinit_data.data.s").read_text().split("D_0046969C")[0].splitlines() if ".word" in l]
    all_b = bodies()
    for name in order:
        try:
            ok[name] = translate(all_b[name])
        except (ValueError, KeyError, AttributeError) as e:
            fail.append((name, str(e)))
    # how each symbol is used: whole-PTMF only -> typed, else bytes
    uses = {}
    for ops in ok.values():
        for x in ops:
            if x[0] == "copy":
                for s, o in ((x[1], x[2]), (x[3], x[4])):
                    uses.setdefault(s, set()).add((o, x[5]))
            else:
                uses.setdefault(x[1], set()).add((x[2], x[3]))
    ptmf = {s for s, u in uses.items() if u == {(0, 12)}}
    srcs = {x[3] for ops in ok.values() for x in ops if x[0] == "copy"}
    print("/* Static initialisers (MW __sinit_*), run before main by the runtime (native: native/platform/main.c).")
    print(" * Generated by tools/sinit_gen.py from the straight-line ones: they copy constant state")
    print(" * PTMFs (and a few other values) from .data into .bss globals. */")
    print('#include <string.h>\n#include "common.h"\n#include "ptmf.h"\n')
    for s in sorted(uses):
        const = "const " if s in srcs else ""
        print(f"extern {const}PTMF {s};" if s in ptmf else f"extern {const}u8 {s}[];")
    print()
    for name, ops in ok.items():
        print(f"void {name}(void) {{")
        for x in ops:
            if x[0] == "copy":
                _, d, do, s, so, n = x
                if d in ptmf and s in ptmf:
                    print(f"    {d} = {s};")
                else:
                    dd = f"&{d}" if d in ptmf else (f"{d} + 0x{do:X}" if do else d)
                    ss = f"&{s}" if s in ptmf else (f"{s} + 0x{so:X}" if so else s)
                    print(f"    memcpy({dd}, {ss}, {n});")
            else:
                _, d, do, n, v = x
                t = {1: "u8", 2: "u16", 4: "u32", 8: "u64"}[n]
                print(f"    *({t} *)({d} + 0x{do:X}) = 0x{v:X};")
        print("}\n")
    for n, e in fail:
        print(f"not translated: {n}: {e}", file=sys.stderr)


if __name__ == "__main__":
    main()
