import re,sys
stubs=set(open(sys.argv[1]).read().split())
src=open('asm/game.s').read().split('\n')
funcs={}; cur=None
for l in src:
    m=re.match(r'glabel (func_[0-9A-F]{8})$',l)
    if m: cur=m.group(1); funcs[cur]=[]; continue
    if l.startswith('endlabel'): cur=None; continue
    if cur:
        m=re.match(r'\s*/\* [0-9A-F]+ [0-9A-F]+ [0-9A-F]+ \*/\s+(.*)$',l)
        if m: funcs[cur].append(re.sub(r'\s+',' ',m.group(1).strip()))
        elif l.strip() and not l.strip().startswith('.L') and not l.strip().startswith('/*'): funcs[cur].append('?'+l.strip())
P1=re.compile(r'^lui \$v0, %hi\((\w+)\)\|jr \$ra\|addiu \$v0, \$v0, %lo\(\1\)$')
P2=re.compile(r'^lui \$v0, %hi\((\w+)\)\|sll \$v1, \$a1, 2\|addiu \$v0, \$v0, %lo\(\1\)\|addu \$v0, \$v0, \$v1\|jr \$ra\|lw \$v0, 0x0\(\$v0\)$')
P3=re.compile(r'^andi \$v1, \$a1, 0xFF\|sll \$v0, \$v1, 1\|daddu \$a1, \$a2, \$zero\|addu \$v1, \$v0, \$v1\|daddu \$a2, \$a3, \$zero\|lui \$v0, %hi\((\w+)\)\|sll \$v1, \$v1, 2\|addiu \$v0, \$v0, %lo\(\1\)\|j __ptmf_scall\|addu \$t9, \$v0, \$v1$')
ext={}; body=[]
for f in sorted(funcs):
    if f not in stubs: continue
    s='|'.join(funcs[f])
    m=P1.match(s)
    if m:
        ext[m.group(1)]='extern u8 %s[];'%m.group(1)
        body.append('void *%s(void) {\n    return %s;\n}\n'%(f,m.group(1))); continue
    m=P2.match(s)
    if m:
        ext[m.group(1)]='extern void *%s[];'%m.group(1)
        body.append('void *%s(void *self, s32 i) {\n    return %s[i];\n}\n'%(f,m.group(1))); continue
    m=P3.match(s)
    if m:
        ext[m.group(1)]='extern PTMF %s[];'%m.group(1)
        body.append('/* (self->*%s[i])(a, b) */\ns32 %s(void *self, u32 i, s32 a, s32 b) {\n    return ptmf_scall_r2(self, &%s[i & 0xFF], a, b);\n}\n'%(m.group(1),f,m.group(1))); continue
print('/* Generated (tools/gen_getters.py): the per-room classes\' and other trivial getters -\n * the address of a static table (room event scripts etc.), one entry of a table, or a call\n * through a member-function-pointer table. */\n#include "common.h"\n#include "ptmf.h"\n')
print('\n'.join(ext[k] for k in sorted(ext))); print()
print('\n'.join(body))
print(len(body),file=sys.stderr)
