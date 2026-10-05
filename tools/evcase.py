#!/usr/bin/env python3
"""Print the asm of event command (or condition) opcodes in their interpreter's jump table.
   tools/evcase.py [--func func_002029B0 --jtbl jtbl_00456A60] OP... (hex)
   Each case runs from its jump-table label to the next case label (other cases' code may
   be shared further on: follow the branches)."""
import re, sys, argparse
ap = argparse.ArgumentParser()
ap.add_argument('--func', default='func_002029B0')
ap.add_argument('--jtbl', default='jtbl_00456A60')
ap.add_argument('--sizes', action='store_true', help='just list op: label size')
ap.add_argument('ops', nargs='*')
a = ap.parse_args()
asm = open('asm/game.s').read().splitlines()
st = next(i for i, l in enumerate(asm) if re.search(r'glabel %s\b' % a.func, l))
en = next(i for i in range(st, len(asm)) if re.search(r'endlabel %s\b' % a.func, asm[i]))
body = asm[st:en]
labels = {m.group(1): i for i, l in enumerate(body) if (m := re.match(r'\s*(?:jlabel )?(\.L[0-9A-F]+):?\s*$', l))}
ro = None
import glob
for p in glob.glob('asm/data/*.rodata.s'):
    t = open(p).read()
    if 'dlabel ' + a.jtbl in t:
        ro = t.split('dlabel ' + a.jtbl, 1)[1].split('enddlabel', 1)[0]
tbl = re.findall(r'\.word (\.L[0-9A-F]+|0x[0-9A-F]+|\w+)', ro)
starts = sorted(set(labels[t] for t in tbl if t in labels))
def block(t):
    i = labels[t]
    nxt = next((s for s in starts if s > i), len(body))
    return i, nxt
if a.sizes:
    for op, t in enumerate(tbl):
        if t not in labels:
            continue
        i, n = block(t)
        print('%02X %s %d' % (op, t, n - i))
    sys.exit()
for o in a.ops:
    if o.startswith('.L'):   # an out-of-line tail: up to its first `b .L002073F4` / jr
        i = labels[o]
        print('==== ' + o)
        for l in body[i:]:
            print(re.sub(r'^\s*/\* [0-9A-F]+ ([0-9A-F]+) [0-9A-F]+ \*/', r'\1', l))
            if re.search(r'\bb\s+\.L002073F4|\bjr\s+\$ra', l):
                print(body[body.index(l) + 1].strip()); break
        continue
    op = int(o, 16)
    t = tbl[op]
    same = [ '%02X' % k for k, x in enumerate(tbl) if x == t]
    i, n = block(t)
    print('==== op %02X -> %s (shared by %s)' % (op, t, ' '.join(same)))
    for l in body[i:n]:
        print(re.sub(r'^\s*/\* [0-9A-F]+ ([0-9A-F]+) [0-9A-F]+ \*/', r'\1', l))
