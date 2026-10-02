"""Run a float math library function of the original in difftest's interpreter:

    tools/mathprobe.py 0031C5C0 1,0 0,1 pi/4,1     (arguments in f12, f13, ...; math names allowed)
"""
import math, sys
sys.path.insert(0, "tools")
import difftest as d

rom = d.BASEROM.read_bytes()
d.load_helpers()

def call(addr, *args):
    mem = d.Memory(rom, 1, [])
    c = d.CPU(mem, rom, 0x00319B00, 0x0031CAA0, 1)   # the whole library runs, nothing stubbed
    for i, a in enumerate(args):
        c.f[12 + i] = d.f2b(a)
    c.s(29, d.STACK_TOP)
    c.s(31, d.RET_MAGIC)
    c.run(addr)
    return d.b2f(c.f[0])

if __name__ == "__main__":
    addr = int(sys.argv[1], 16)
    for a in sys.argv[2:]:
        xs = [float(eval(v, vars(math))) for v in a.split(",")]
        print(a, "->", call(addr, *xs))
