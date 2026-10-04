# Move the engine (non-pursuer) function declarations of the pursuer sources into one block of
# include/pursuer.h, deduplicated (first spelling wins; fix conflicts by hand first).
import re
SRCS=['src/game/pursuer.c','src/game/pursuer_ai.c']
H='include/pursuer.h'
RX=re.compile(r'^extern ([^;\n]*?\b(func_[0-9A-F]{8}|sce\w+)\([^;\n]*\));[^\n]*\n',re.M)
h=open(H).read()
old=re.search(r'/\* ---- engine functions used.*?/\* ---- end engine ---- \*/\n',h,re.S)
decl={}
if old:
    for m in RX.finditer(old.group(0)): decl[m.group(2)]=m.group(1)
for s in SRCS:
    t=open(s).read()
    for m in RX.finditer(t): decl.setdefault(m.group(2),re.sub(r'\s+',' ',m.group(1)))
    open(s,'w').write(RX.sub('',t))
DEF=re.compile(r'^(?!static|extern)[A-Za-z][\w \*]*?\b(func_[0-9A-F]{8})\([^;{]*\)\s*\{',re.M)
defined=set()
for s in SRCS: defined|=set(DEF.findall(open(s).read()))
for k in list(decl):
    if k in defined: del decl[k]
block='/* ---- engine functions used (tools/pursuer/externs.py) ---- */\n'+''.join('extern %s;\n'%decl[k] for k in sorted(decl))+'/* ---- end engine ---- */\n'
if old: h=h.replace(old.group(0),block)
else: h=h.replace('/* ---- generated',block+'\n/* ---- generated',1)
open(H,'w').write(h)
print(len(decl),'engine declarations')
