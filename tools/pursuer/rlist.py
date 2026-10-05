"""Functions in an address range: size, name, address, in C or not.  rlist.py LO HI [--todo]"""
import re, glob, sys

asm = open('asm/game.s').read()
defined = set()
for f in glob.glob('src/**/*.c', recursive=True) + glob.glob('src/**/*.inc', recursive=True):
    defined |= set(re.findall(r'^(?!static\b|typedef\b|extern\b)[A-Za-z_][\w \t\*]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{',
                              open(f).read(), re.M))
# a file that builds a shared body under other names (#define A B before including it)
for f in glob.glob('src/**/*.c', recursive=True):
    for a, b in re.findall(r'^#define\s+(func_[0-9A-F]{8})\s+(func_[0-9A-F]{8})\s*$', open(f).read(), re.M):
        if a in defined:
            defined.add(b)
lo, hi = int(sys.argv[1], 16), int(sys.argv[2], 16)
todo = '--todo' in sys.argv
tot = n = 0
for name, b in re.findall(r'glabel (\w+)\n(.*?)endlabel', asm, re.S):
    m = re.search(r'/\* [0-9A-F]+ ([0-9A-F]+) ', b)
    if not m:
        continue
    a = int(m.group(1), 16)
    if lo <= a < hi:
        k = len(re.findall(r'^\s*/\*', b, re.M))
        done = name in defined
        if todo and done:
            continue
        if not done:
            tot += k
            n += 1
        print(f'{a:#08x} {k:5} {"C " if done else "  "}{name}')
print(f'-- {n} not in C, {tot} insns')
