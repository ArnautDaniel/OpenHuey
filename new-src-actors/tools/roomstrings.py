#!/usr/bin/env python3
"""The rooms' string tables (RoomXX_ObjectNames: what event commands name by number - scenes,
movies, room objects) read from the executable into scripts/story/strings.fs's `room-string
( i room -- addr len )`, keyed by room name (scripts/room-names.fs). From new-src's
tools/roomstrings.py, which stopped a table at its first empty entry: a table may start with
empty entries (room $32's first eight), so empty ones are kept as "" up to the next symbol.
Only the room-string definition is replaced; the music tables after it stay.

usage: roomstrings.py [repo root]"""
import os
import re
import struct
import sys

root = sys.argv[1] if len(sys.argv) > 1 else os.path.join(os.path.dirname(__file__), '..', '..')
elf = open(os.path.join(root, 'baserom', 'SLUS_210.75'), 'rb').read()
syms, addrs = {}, []
for line in open(os.path.join(root, 'config', 'symbol_addrs.txt')):
    m = re.match(r'\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);', line)
    if m:
        syms[m.group(1)] = int(m.group(2), 16)
        addrs.append(int(m.group(2), 16))
addrs.sort()
e_phoff, = struct.unpack_from('<I', elf, 0x1C)
e_phnum, = struct.unpack_from('<H', elf, 0x2C)
segs = [struct.unpack_from('<5I', elf, e_phoff + i * 32)[1:] for i in range(e_phnum)]
segs = [(vaddr, off, filesz) for off, vaddr, _, filesz in segs]


def at(va, n):
    for vaddr, off, size in segs:
        if vaddr <= va and va + n <= vaddr + size:
            return elf[off + va - vaddr: off + va - vaddr + n]
    return None


def cstring(va):
    out = b''
    while len(out) < 64:
        c = at(va + len(out), 1)
        if c is None:
            return None
        if c == b'\0':
            break
        out += c
    if not out or any(b < 0x20 or b > 0x7E for b in out):
        return None
    return out.decode()


def table(va):
    nxt = next((a for a in addrs if a > va), va + 0x100)
    out = []
    while va + 4 * len(out) < nxt:
        p = at(va + 4 * len(out), 4)
        if p is None:
            break
        ptr, = struct.unpack('<I', p)
        if ptr == 0:
            out.append('')            # an empty entry: kept, its index matters
            continue
        s = cstring(ptr)
        if s is None:
            break
        out.append(s)
    while out and out[-1] == '':
        out.pop()
    return out


names = {}
for line in open(os.path.join(os.path.dirname(__file__), '..', 'scripts', 'room-names.fs')):
    m = re.match(r'\$([0-9A-Fa-f]+)\s+constant\s+(\S+)', line)
    if m:
        names[int(m.group(1), 16)] = m.group(2)
rooms = {}
for name, va in syms.items():
    m = re.match(r'Room([0-9A-F]{2})_ObjectNames$', name)
    if m:
        rooms[int(m.group(1), 16)] = table(va)

lines = [': room-string ( i room -- addr len )   \\ string i of that room ("" none)', '    case']
for room in sorted(rooms):
    if not rooms[room] or room not in names:
        continue
    lines.append('    %s of  case' % names[room])
    for i, s in enumerate(rooms[room]):
        if s:
            lines.append('        %d of  s" %s" endof' % (i, s.replace('"', '')))
    lines.append('        >r s" " r>  endcase  endof')
lines.append('    >r drop s" " r>  endcase ;')
path = os.path.join(os.path.dirname(__file__), '..', 'scripts', 'story', 'strings.fs')
src = open(path).read()
a = src.index(': room-string (')
b = src.index('endcase ;', src.index('>r drop s" " r>  endcase ;', a)) + len('endcase ;')
open(path, 'w').write(src[:a] + '\n'.join(lines) + src[b:])
print(path, sum(1 for r in rooms if rooms[r] and r in names), 'rooms')
