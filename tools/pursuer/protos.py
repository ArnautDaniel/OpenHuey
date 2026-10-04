# Regenerate the prototype block in include/pursuer.h from the definitions in the pursuer
# sources, and drop their now redundant extern declarations from those sources.
import re
SRCS=['src/game/pursuer.c','src/game/pursuer_ai.c']
H='include/pursuer.h'
DEF=re.compile(r'^((?!static|extern)[A-Za-z][\w \*]*?\b(func_[0-9A-F]{8})\(([^;{]*?)\))\s*\{',re.M)
protos={}
for s in SRCS:
    for full,name,args in DEF.findall(open(s).read()):
        protos[name]=re.sub(r'\s+',' ',full)+';'
for s in SRCS:
    t=open(s).read()
    t=re.sub(r'^extern [^;\n]*\b(func_[0-9A-F]{8})\([^;]*\);[^\n]*\n',lambda m:'' if m.group(1) in protos else m.group(0),t,flags=re.M)
    open(s,'w').write(t)
h=open(H).read()
block='/* ---- generated from the definitions (tools: protos.py) ---- */\n'+'\n'.join(protos[k] for k in sorted(protos))+'\n/* ---- end generated ---- */\n'
if '/* ---- generated' in h:
    h=re.sub(r'/\* ---- generated.*?/\* ---- end generated ---- \*/\n',block,h,flags=re.S)
else:
    h=h.replace('\n#endif',  '\n'+block+'\n#endif')
open(H,'w').write(h)
print(len(protos),'prototypes')
