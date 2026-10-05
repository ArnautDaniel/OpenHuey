#!/usr/bin/env python3
"""Functions nothing in the game references: no call / jump / address load from live code, no
data table or vtable entry, no C call site; iterated so code only dead code reaches counts too.

    tools/deadcode.py          print them
    tools/deadcode.py --mark   put MARK above each one defined in C (once)

The linker kept them (whole objects / libraries are linked), so they are possibly dead code.
They are still decompiled, for completeness and safety."""
import re,glob,collections,subprocess
import os, sys
ROOT=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
asm=open(ROOT+'/asm/game.s').read()
refs=collections.defaultdict(set)   # function -> functions it references
funcs=[]
for m in re.finditer(r'glabel (func_[0-9A-F]{8})\n(.*?)endlabel',asm,re.S):
    n=m.group(1); funcs.append(n)
    for r in set(re.findall(r'\bfunc_[0-9A-F]{8}\b',m.group(2))):
        if r!=n: refs[n].add(r)
live=set()
# roots: anything named in data, other asm (crt0 / sinit), or C sources
roots=set()
for p in glob.glob(ROOT+'/asm/data/*.s')+[ROOT+'/asm/crt0.s',ROOT+'/asm/sinit.s']:
    roots|=set(re.findall(r'\bfunc_[0-9A-F]{8}\b',open(p).read()))
cmentions=collections.Counter()
for p in glob.glob(ROOT+'/src/**/*.c',recursive=True)+glob.glob(ROOT+'/native/platform/*.c'):
    cmentions.update(re.findall(r'\bfunc_[0-9A-F]{8}\b',open(p).read()))
# a C mention counts unless it is the function's own definition (its only mention anywhere)
croots={n for n,c in cmentions.items() if c>1}
for p in glob.glob(ROOT+'/src/**/*.c',recursive=True)+glob.glob(ROOT+'/native/platform/*.c'):
    t=open(p).read()
    for n in set(re.findall(r'\bfunc_[0-9A-F]{8}\b',t)):
        if cmentions[n]==1 and not re.search(r'^[A-Za-z_][\w \*]*?\b'+n+r'\s*\([^;]*\)\s*\{',t,re.M):
            croots.add(n)   # a lone mention that isn't a definition (e.g. a call through a table in C)
roots|=croots
stack=[f for f in funcs if f in roots]
live=set(stack)
while stack:
    f=stack.pop()
    for r in refs.get(f,()):
        if r not in live: live.add(r); stack.append(r)
dead=[f for f in funcs if f not in live]
MARK = '/* (possibly dead code: nothing in the game references it) */'
if '--mark' in sys.argv:
    dset = set(dead)
    for p in glob.glob(ROOT + '/src/**/*.c', recursive=True):
        lines = open(p).read().split('\n')
        out = []
        changed = False
        for i, l in enumerate(lines):
            m = re.match(r'^[A-Za-z_][\w \*]*?\b(func_[0-9A-F]{8})\s*\(', l)
            if m and m.group(1) in dset and ';' not in l and not l.startswith(('extern', 'static inline', '#')) and (not out or out[-1] != MARK):
                out.append(MARK)
                changed = True
            out.append(l)
        if changed:
            open(p, 'w').write('\n'.join(out))
else:
    print('\n'.join(dead))
