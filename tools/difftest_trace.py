#!/usr/bin/env python3
"""Print every stub call's target and return value for one difftest run.

    tools/difftest_trace.py LIST SEED

LIST is a difftest list file (usually one line); SEED is the run's seed as a FAIL line
prints it. Both versions' stubs are listed, the original's first: the harness RNG feeds
stub returns, so where the two lists part shows which call shifted it.
"""
import sys
want=int(sys.argv[2])
sys.argv = ['difftest.py','--list',sys.argv[1],'-j','1','-v']
sys.path.insert(0, str(__import__('pathlib').Path(__file__).parent))
import difftest as d
cur=[None]
ro=d.run_one
def run_one(rom, overlays, entry, frange, seed, max_steps=None):
    cur[0]=seed
    if seed==want: print('--- run',hex(entry),file=sys.stderr)
    return ro(rom, overlays, entry, frange, seed, max_steps)
d.run_one=run_one
orig=d.CPU.stub_call
def sc(self,target,kind):
    orig(self,target,kind)
    if cur[0]==want:
        print('  stub',hex(target),'v0=',hex(self.g(2)&0xFFFFFFFF),file=sys.stderr)
d.CPU.stub_call=sc
d.main()
