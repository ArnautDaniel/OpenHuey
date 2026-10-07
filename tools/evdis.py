#!/usr/bin/env python3
"""Disassemble the rooms' event scripts from the game's ELF.

    tools/evdis.py                 every room's scripts, as listings
    tools/evdis.py ROOM...         those rooms (hex, e.g. 2A)
    tools/evdis.py --stats         per opcode: how many uses, and a sample
    tools/evdis.py --check         decode everything, report scripts that don't end cleanly

A script is a run of commands (0x00-0xDA, each its own length: sCmdLength in
src/game/event.c; 0 = a string, 3 + the byte at +2) and control ops: F0 / F1 if / if not,
F2 / F3 and / and not, F4 / F5 or / or not (each followed by a condition, 0x00-0x65, lengths
from the table at 0x3D72C0; 0x11 carries a string), F6 then, F7 else, F8 end, F9 end and skip
the rest of the enclosing block, FA / FB open / close a plain block, FF the end. Outside any
block a script also ends at 0x24 / 0x25 / 0x2D (loop back, go to, return), and an action script
at 0x06 followed by padding (its character's script ends).
Names and operand layouts come from tools/event_opcodes.py when it has them."""
import argparse
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import vudis  # noqa: E402  (the ELF reader)

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
COND_LEN_ADDR = 0x3D72C0

try:
    import event_opcodes as OPS  # noqa: E402
except ImportError:
    OPS = None


def cmd_lengths():
    src = open(os.path.join(ROOT, 'src/game/event.c')).read()
    body = re.search(r'sCmdLength\[0xDB\] = \{(.*?)\};', src, re.S).group(1)
    body = re.sub(r'/\*.*?\*/', '', body)
    lens = [int(x) for x in re.findall(r'\d+', body)]
    assert len(lens) == 0xDB, len(lens)
    return lens


def symbols():
    out = {}
    for line in open(os.path.join(ROOT, 'config/symbol_addrs.txt')):
        m = re.match(r'^(\w+)\s*=\s*0x([0-9A-Fa-f]+);', line)
        if m:
            out[m.group(1)] = int(m.group(2), 16)
    return out


class Reader:
    def __init__(self):
        self.rd = vudis.load()
        self.clen = list(self.rd(COND_LEN_ADDR, 0x66))
        self.cmdlen = cmd_lengths()

    def byte(self, a):
        return self.rd(a, 1)[0]

    def bytes(self, a, n):
        return self.rd(a, n)

    def cmd_size(self, a):
        op = self.byte(a)
        n = self.cmdlen[op]
        return 3 + self.byte(a + 2) if n == 0 else n

    def cond_size(self, a):
        op = self.byte(a)
        if op == 0x11:
            return 3 + self.byte(a + 2)
        return self.clen[op]


def decode(r, addr, limit=0x4000):
    """[(addr, kind, op, raw bytes, depth)] up to and including FF; kind: 'cmd', 'cond', 'ctl'.
    Raises ValueError on an op that can't be there."""
    out = []
    a = addr
    depth = 0
    end = addr + limit
    while a < end:
        op = r.byte(a)
        if op >= 0xF0:
            if op in (0xFC, 0xFD, 0xFE):
                raise ValueError('control op %02X at %08X' % (op, a))
            out.append((a, 'ctl', op, r.bytes(a, 1), depth))
            a += 1
            if op == 0xFF:
                return out
            if op <= 0xF5:   # (F0 / F1 also open the block, below)
                cop = r.byte(a)
                if cop > 0x65:
                    raise ValueError('condition %02X at %08X' % (cop, a))
                n = r.cond_size(a)
                out.append((a, 'cond', cop, r.bytes(a, n), depth))
                a += n
            if op in (0xF0, 0xF1, 0xF6, 0xFA):
                depth += 1
            elif op in (0xF8, 0xF9, 0xFB):
                depth -= 1
            continue
        if op >= 0xDB:
            raise ValueError('command %02X at %08X' % (op, a))
        n = r.cmd_size(a)
        out.append((a, 'cmd', op, r.bytes(a, n), depth))
        a += n
        if op in ENDS + (0x06,) and (a in STARTS or all(b == 0 for b in r.bytes(a, (-a) % 4 or 4))):
            return out   # (a jump, return or end, then another script, a table or padding)
        if op in ENDS and depth <= 0:
            return out   # (a jump or return outside any block: nothing after it runs)
    raise ValueError('no end within %d bytes of %08X' % (limit, addr))


STARTS = set()   # every script's start address (scripts())
ENDS = (0x24, 0x25, 0x2D)   # back to the loop point, go to a script, return

CTL = {0xF0: 'if', 0xF1: 'if not', 0xF2: 'and', 0xF3: 'and not', 0xF4: 'or', 0xF5: 'or not',
       0xF6: 'block (always)', 0xF7: 'else', 0xF8: 'end', 0xF9: 'end, skip the rest', 0xFA: 'block',
       0xFB: 'end block', 0xFF: 'end of script'}


def scripts(syms, rooms=None):
    """(room, name, address) of every script: the named phase scripts and each room's action
    scripts (its table runs up to the next label)"""
    addrs = sorted(set(syms.values()))
    out = []
    for name, a in sorted(syms.items(), key=lambda kv: kv[1]):
        m = re.match(r'Room([0-9A-F]+)_(\w+)$', name)
        if not m or (rooms and m.group(1) not in rooms):
            continue
        room, what = m.group(1), m.group(2)
        if what.endswith('Script_data'):
            out.append((room, what[:-5], a))
        elif what == 'ActionScripts':
            nxt = next((x for x in addrs if x > a), a + 4)
            r = READER
            for i in range((nxt - a) // 4):
                p = int.from_bytes(r.bytes(a + i * 4, 4), 'little')
                if 0x3E0000 <= p < 0x480000:
                    out.append((room, 'Action%d' % i, p))
    return out


def fmt(e):
    a, kind, op, raw, depth = e
    hexs = ' '.join('%02X' % b for b in raw)
    if kind == 'ctl':
        return '%08X  %s%-14s %s' % (a, '  ' * depth, CTL.get(op, '?'), hexs)
    info = OPS.describe(kind, op, raw) if OPS else None
    name = info or ('%s %02X' % (kind, op))
    return '%08X  %s%s  [%s]' % (a, '  ' * (depth + (1 if kind == 'cond' else 0)), name, hexs)


READER = None


def main():
    global READER
    ap = argparse.ArgumentParser()
    ap.add_argument('rooms', nargs='*')
    ap.add_argument('--stats', action='store_true')
    ap.add_argument('--check', action='store_true')
    a = ap.parse_args()
    READER = Reader()
    syms = symbols()
    rooms = set(x.upper() for x in a.rooms) or None
    bad = 0
    uses = {}
    STARTS.update(addr for _, _, addr in scripts(syms))
    STARTS.update(syms.values())   # (any label: a script can end right before a table)
    for room, name, addr in scripts(syms, rooms):
        try:
            ev = decode(READER, addr)
        except ValueError as e:
            bad += 1
            print('room %s %s at %08X: %s' % (room, name, addr, e))
            continue
        for e in ev:
            if e[1] != 'ctl':
                key = (e[1], e[2])
                u = uses.setdefault(key, [0, e[3], room])
                u[0] += 1
        if not (a.stats or a.check):
            print('== room %s %s (%08X)' % (room, name, addr))
            for e in ev:
                print(fmt(e))
    if a.stats:
        for (kind, op), (n, raw, room) in sorted(uses.items()):
            print('%-4s %02X  %5d uses  e.g. room %s: %s' % (kind, op, n, room, ' '.join('%02X' % b for b in raw)))
    if a.check or a.stats:
        print('%d scripts failed to decode' % bad, file=sys.stderr)
        print('%d commands and %d conditions used' % (len([k for k in uses if k[0] == 'cmd']),
                                                     len([k for k in uses if k[0] == 'cond'])), file=sys.stderr)


if __name__ == '__main__':
    main()
