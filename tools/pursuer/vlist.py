"""Functions of the given vtables (labels, e.g. D_00469D10) not yet in C: size, name, slot, callers."""
import re, glob, sys, collections

asm = open('asm/game.s').read()
data = ''.join(open(f).read() for f in glob.glob('asm/data/*.s'))
blocks = dict(re.findall(r'dlabel (\w+)\n(.*?)enddlabel', data, re.S))
defined = set()
for f in glob.glob('src/**/*.c', recursive=True) + glob.glob('src/**/*.inc', recursive=True):
    defined |= set(re.findall(r'^(?!static\b|typedef\b|extern\b)[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{',
                              open(f).read(), re.M))
# a file that builds a shared body under other names (#define A B before including it)
for f in glob.glob('src/**/*.c', recursive=True):
    for a, b in re.findall(r'^#define\s+(func_[0-9A-F]{8})\s+(func_[0-9A-F]{8})\s*$', open(f).read(), re.M):
        if a in defined:
            defined.add(b)
size = {}
addr = {}
for n, b in re.findall(r'glabel (\w+)\n(.*?)endlabel', asm, re.S):
    size[n] = len(re.findall(r'^\s*/\*', b, re.M))
    m = re.search(r'/\* [0-9A-F]+ ([0-9A-F]+) ', b)
    if m:
        addr[n] = int(m.group(1), 16)
calls = collections.Counter(re.findall(r'jal\s+(func_\w+)', asm))
for vt in sys.argv[1:]:
    words = re.findall(r'\.word (\w+)', blocks.get(vt, ''))
    seen = {}
    for i, w in enumerate(words):
        if w.startswith('func_') and w not in defined and w not in seen:
            seen[w] = hex(i * 4)
    tot = sum(size.get(w, 0) for w in seen)
    lo = min((addr[w] for w in seen if w in addr), default=0)
    hi = max((addr[w] for w in seen if w in addr), default=0)
    print(f'== {vt}: {len(seen)} functions, {tot} insns, {lo:#x}..{hi:#x}')
    for w, s in sorted(seen.items(), key=lambda x: size.get(x[0], 0)):
        print(f'  {size.get(w, 0):5} {w} {s} calls={calls[w]}')
