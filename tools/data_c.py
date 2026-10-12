#!/usr/bin/env python3
"""Write the compiler-generated part of the game's data as C for the native build (hg):

  - the static constructor table (Sinit_Table .. its 0 terminator D_0046969C),
  - every vtable (the region D_004696A0 .. SceneGame_vtableProgress in sinit_data: the C++
    runtime's own, then the game's, with the pieces split off where code points into them),
  - every pointer-to-member constant (*_ptmf: 16 bytes, PTMF16, with the label the split cut it
    into after its first words).

    tools/data_c.py            (after configure.py --split) -> native/data/group1.c, group1.txt

They are arrays of words in runs, one run per stretch of adjacent labels, each label an alias at
its offset, so everything the code reads across labels (a vtable running on into the next
label, a PTMF's last words) stays where it was. native_gen.py leaves zero-filled space in their
place (native/data/group1.txt), so the rest of the data keeps its layout. The PS2 build keeps
using the disassembly. Run once: the C is the source afterwards; run again only to regenerate it
from a fresh split."""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
WORD = re.compile(r"^\s+/\* [0-9A-F]+ ([0-9A-F]{8}) ([0-9A-F]+) \*/\s+\.(\w+)\s+(.*?)\s*$")


def labels():
    """every data label: name -> (address, [(address, size, bytes, directive, operand)])"""
    out = {}
    for src in sorted((ROOT / "asm/data").glob("*.s")):
        cur = None
        for line in src.read_text().splitlines():
            m = re.match(r"dlabel (\S+)", line)
            if m:
                cur = m.group(1)
                out[cur] = [None, []]
                continue
            if line.startswith("enddlabel"):
                cur = None
                continue
            m = WORD.match(line)
            if m and cur:
                a = int(m.group(1), 16)
                b = bytes.fromhex(m.group(2))
                if out[cur][0] is None:
                    out[cur][0] = a
                out[cur][1].append((a, len(b), b, m.group(3), m.group(4)))
    return {k: v for k, v in out.items() if v[0] is not None}


def group1(labs):
    by_addr = sorted(labs.items(), key=lambda kv: kv[1][0])
    names = [n for n, _ in by_addr]
    sel = {"Sinit_Table", "D_00469480", "D_0046969C"}
    a, b = labs["D_004696A0"][0], labs["SceneGame_vtableProgress"][0]
    sel |= {n for n, (ad, _) in labs.items() if a <= ad <= b}
    odd = []
    for n, (ad, w) in labs.items():
        if not re.search(r"_ptmf\d*$", n):
            continue
        sel.add(n)
        i = names.index(n) + 1
        while i < len(names) and labs[names[i]][0] < ad + 16:
            nxt = names[i]
            end = labs[nxt][1][-1][0] + labs[nxt][1][-1][1]
            if end > ad + 16 and any(int.from_bytes(b, "little") != 0
                                     for a2, _, b, _, _ in labs[nxt][1] if a2 >= ad + 16):
                odd.append((n, nxt))   # runs on past the PTMF with more than padding
                break
            sel.add(nxt)
            i += 1
    # PTMF tables run on past their first named entry through anonymous labels (the code indexes
    # them: PathPlan_StepBreadth_ptmf[k * 4]): while a selected stretch is followed by another
    # PTMF ({0, -1 or a vtable offset, a symbol, 0}), take the labels covering it too
    word = {}
    for n, (ad, w) in labs.items():
        for a, size, b, d, op in w:
            word[a] = (n, int.from_bytes(b, "little"), d, op)
    starts = sorted((ad, n) for n, (ad, _) in labs.items())

    def ptmf_at(p):
        """the labels covering a PTMF at p ({0, -1 or a vtable offset, a symbol, 0}), or None"""
        q = [word.get(p + i * 4) for i in range(4)]
        if None in q or q[0][1] != 0 or q[3][1] != 0 or q[2][2] != "word" or q[2][3].startswith("0x"):
            return None
        if not (q[1][1] == 0xFFFFFFFF or q[1][1] < 0x1000):
            return None
        cover = {q[i][0] for i in range(4)}
        for m in cover:
            me = labs[m][1][-1][0] + labs[m][1][-1][1]
            if labs[m][0] < p or (me > p + 16 and any(int.from_bytes(b, "little") != 0
                                                      for a2, _, b, _, _ in labs[m][1] if a2 >= p + 16)):
                return None
        return cover

    # the PTMF tables without a named entry (the title's states...): every PTMF the labels
    # start at
    for p, n in starts:
        if n not in sel:
            c = ptmf_at(p)
            if c:
                sel |= c
    grew = True
    while grew:
        grew = False
        for n in sorted(sel, key=lambda x: labs[x][0]):
            w = labs[n][1]
            e = w[-1][0] + w[-1][1]
            nxt = next(((ad, m) for ad, m in starts if ad >= e), None)
            if nxt is None or nxt[1] in sel or nxt[0] - e >= 16:
                continue
            p = nxt[0]
            q = [word.get(p + i * 4) for i in range(4)]
            if None in q or q[0][1] != 0 or q[3][1] != 0 or q[2][2] != "word" or q[2][3].startswith("0x"):
                continue
            if not (q[1][1] == 0xFFFFFFFF or q[1][1] < 0x1000):
                continue
            cover = {q[i][0] for i in range(4)}
            for m in cover:
                me = labs[m][1][-1][0] + labs[m][1][-1][1]
                if me > p + 16 and any(int.from_bytes(b, "little") != 0
                                       for a2, _, b, _, _ in labs[m][1] if a2 >= p + 16):
                    break
            else:
                sel |= cover
                grew = True
    return sel, odd


def operand_c(op):
    op = op.strip("() ")
    m = re.fullmatch(r"([A-Za-z_]\w*)\s*([+-])\s*(0x[0-9A-Fa-f]+|\d+)", op)
    if m:
        return m.group(1), f"(void *)({m.group(1)} {m.group(2)} {m.group(3)})"
    if re.fullmatch(r"[A-Za-z_]\w*", op):
        return op, f"(void *){op}"
    return None, None


def main():
    labs = labels()
    sel, odd = group1(labs)
    if odd:
        sys.exit(f"PTMFs cut into a label that runs on past them: {odd}")
    # runs of adjacent selected labels (only alignment padding between them)
    order = sorted(sel, key=lambda n: labs[n][0])
    runs, cur = [], None
    for n in order:
        ad, w = labs[n]
        end = w[-1][0] + w[-1][1]
        others = [m for m in labs if m not in sel and cur and cur[1] <= labs[m][0] < ad]
        if cur and ad - cur[1] < 16 and not others:
            cur[1] = end
            cur[2].append(n)
        else:
            cur = [ad, end, [n]]
            runs.append(cur)
    defined = set(sel)
    externs = set()
    body = []
    for start, end, names in runs:
        words = {}
        for n in names:
            for a, size, b, d, op in labs[n][1]:
                if size != 4:
                    sys.exit(f"{n}: a {size}-byte .{d} - only words are handled")
                words[a] = (b, d, op)
        start4 = start & ~3
        n_words = (end - start4 + 3) // 4
        align = 1
        while align < 128 and start4 % (align * 2) == 0:
            align *= 2
        items = []
        for i in range(n_words):
            a = start4 + i * 4
            if a not in words:
                items.append("0")
                continue
            b, d, op = words[a]
            sym, c = operand_c(op) if d == "word" else (None, None)
            if c:
                if sym not in defined:
                    externs.add(sym)
                items.append(c)
            else:
                items.append(f"(void *)0x{int.from_bytes(b, 'little'):08X}u")
        arr = f"g1_{start4:08X}"
        body.append(f"/* 0x{start4:08X}: {', '.join(names[:6])}{' ...' if len(names) > 6 else ''} */")
        body.append(f"void *{arr}[{n_words}] __attribute__((aligned({max(align, 4)}), used)) = {{")
        for i in range(0, n_words, 4):
            body.append("    " + ", ".join(items[i:i + 4]) + ",")
        body.append("};")
        sets = "".join(f'".globl {n}\\n.type {n}, @object\\n.set {n}, {arr} + 0x{labs[n][0] - start4:X}\\n"'
                       for n in names)
        body.append(f"__asm__({sets});")
        body.append("")
    head = [
        "/* The compiler-generated part of the game's data, for the native build (hg): the static",
        " * constructor table, the vtables and the pointer-to-member constants, as written by",
        " * tools/data_c.py from the split executable. Each run is the words of a stretch of adjacent",
        " * labels at their PS2 offsets; each label is an alias into it (see tools/data_c.py). Not",
        " * compiled into the PS2 build, which keeps the disassembly. */",
        "",
    ]
    head += [f"extern char {s}[];" for s in sorted(externs)]
    out = ROOT / "native/data"
    out.mkdir(exist_ok=True)
    (out / "group1.c").write_text("\n".join(head + [""] + body))
    (out / "group1.txt").write_text(
        "# labels tools/data_c.py wrote as C (native/data/group1.c): native_gen.py leaves their space empty\n"
        + "\n".join(order) + "\n")
    print(f"{len(sel)} labels in {len(runs)} runs, {sum(len(labs[n][1]) for n in sel)} words, {len(externs)} symbols referenced")


if __name__ == "__main__":
    main()
