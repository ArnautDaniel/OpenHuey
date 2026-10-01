#!/usr/bin/env python3
"""Print the static call tree below a function (direct jal/j calls; jalr shown as [vcall]).

    tools/calltree.py func_002D1F20 [--depth 3]

Marks library functions (config/libraries.txt) with [lib] and doesn't descend into them,
and already-decompiled functions (defined in src/) with [C].
"""
import argparse
import re
from pathlib import Path

INS = re.compile(r"/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/\s+([\w.]+)\s*(.*)$")


def load():
    funcs = {}
    for p in (Path("asm/game.s"), Path("asm/sinit.s")):
        cur = None
        for line in p.open():
            if line.startswith("glabel "):
                cur = line.split()[1]
                funcs[cur] = {"addr": None, "n": 0, "calls": [], "vcalls": 0}
                continue
            m = INS.search(line)
            if m and cur:
                f = funcs[cur]
                if f["addr"] is None:
                    f["addr"] = int(m.group(1), 16)
                f["n"] += 1
                op, args = m.group(2), m.group(3).strip()
                if op == "jal" or (op == "j" and not args.startswith(".L")):
                    if args not in f["calls"]:
                        f["calls"].append(args)
                elif op == "jalr":
                    f["vcalls"] += 1
    return funcs


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("func")
    ap.add_argument("--depth", type=int, default=3)
    args = ap.parse_args()
    funcs = load()
    libs = [(int(l.split()[0], 16), int(l.split()[1], 16), l.split(None, 3)[3].strip())
            for l in open("config/libraries.txt") if l.startswith("0x")]
    done = set()
    for src in Path("src").rglob("*.c"):
        done.update(re.findall(r"^[A-Za-z_][\w \*]*?\b(\w+)\s*\([^;{}]*\)\s*\{", src.read_text(), re.M))
    seen = set()

    def lib_of(a):
        return next((name.split(":")[0].split("(")[0].strip() for lo, hi, name in libs if lo <= a < hi), None)

    def walk(name, d, prefix):
        f = funcs.get(name)
        if f is None:
            print(f"{prefix}{name} [?]")
            return
        lib = lib_of(f["addr"])
        tags = (" [lib: " + lib + "]") if lib else ""
        tags += " [C]" if name in done else ""
        tags += f" [{f['vcalls']} vcalls]" if f["vcalls"] else ""
        again = name in seen
        print(f"{prefix}{name} ({f['n']} insns){tags}{' (see above)' if again else ''}")
        if again or lib or d >= args.depth:
            return
        seen.add(name)
        for c in f["calls"]:
            walk(c, d + 1, prefix + "  ")

    walk(args.func, 0, "")


if __name__ == "__main__":
    main()
