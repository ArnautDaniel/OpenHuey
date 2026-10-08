#!/usr/bin/env python3
"""State flags by name: in the given Forth files, `n state-flag?` / `-set` / `-clear` (and the
tests' `n true flag`, `n set-flag`) with a number become the name from scripts/flag-names.fs, and
`flag-names` joins the file's USING:. usage: flag_names.py file.fs ..."""
import re, sys, os
here = os.path.dirname(os.path.abspath(__file__))
names = {}
for l in open(os.path.join(here, '..', 'scripts', 'flag-names.fs')):
    m = re.match(r'\$([0-9A-F]+)\s+constant\s+(\S+)', l)
    if m:
        names[int(m.group(1), 16)] = m.group(2)
pat = re.compile(r'(?<![\w$-])(\$[0-9A-Fa-f]+|\d+)(\s+)(state-flag\?|state-flag-set|state-flag-clear|true flag|false flag|set-flag)(?=\s|$)')
for f in sys.argv[1:]:
    s = open(f).read()
    def sub(m):
        n = int(m.group(1)[1:], 16) if m.group(1).startswith('$') else int(m.group(1))
        return names[n] + m.group(2) + m.group(3) if n in names else m.group(0)
    t = pat.sub(sub, s)
    if t != s and 'flag-names' not in t.split('USING:', 1)[-1].split(';', 1)[0]:
        t = re.sub(r'^(USING:[^;]*?)\s*;', r'\1 flag-names ;', t, count=1, flags=re.M)
    if t != s:
        open(f, 'w').write(t)
        print('named:', f)
