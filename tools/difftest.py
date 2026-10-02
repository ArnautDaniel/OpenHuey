#!/usr/bin/env python3
"""Differential tester: does a decompiled C function behave like the original?

    tools/difftest.py src/foo.c [func_002D1E40 ...] [--runs N] [--seed 1] [-v]
    tools/difftest.py --list tools/difftest_list.txt [--runs N]

With no function names, every non-static function in the file is tested. The C
file is compiled once per invocation, and the game's symbol table is cached in
build/ (re-read only when the ELF changes), so test many functions per call.
--runs defaults to 20; -j N tests N functions in parallel (default 4).

Runs the original machine code (from the baserom) and the C version (compiled
with the project's GCC, linked against the game's real symbol addresses) in a
small R5900 interpreter on the same random inputs, and compares:

  * the sequence of calls made (target + the argument registers the callee
    actually reads + a digest of memory written so far),
  * memory written (excluding the function's own stack frame),
  * the return value (v0, or f0 for float functions; nothing for void),
  * that callee-saved registers are preserved.

Calls to other functions are not executed: they are recorded and return
deterministic random values, so each function is tested in isolation.
Memory nobody has written reads as deterministic pseudo-random bytes derived
from the address and run seed (the game image reads from the rom), so random
pointers can be followed without knowing any types and both versions see the
same "world". Inputs that make the *original* run away are skipped.

Floating point is modelled loosely after the EE (no denormals, clamping on
overflow); it is the same model for both versions, which is what matters.

Small MW runtime helpers (INLINE_HELPERS, e.g. __ptmf_scall) are executed rather
than stubbed, since C code expresses them inline. Functions that never return
with stubbed callees (main loops) are compared by the calls made before the
step cap. Random values favour 0/1/-1/small numbers, realistic floats and the
constants the function itself compares against (+-2).

Limits: random testing reliably finds wrong constants, operands, offsets,
branches and call sequences, but an off-by-one that only shows when several
conditions coincide (e.g. a counter at exactly N-2 *and* two flags set) can
slip through; review such comparisons by hand. Argument registers of calls to
unknown targets (random vtable entries) are only compared when both versions
set them.
"""
import argparse
import hashlib
import pickle
import random
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

import rabbitizer as rz
from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parent.parent
BASEROM = ROOT / "baserom/SLUS_210.75"
BUILD_ELF = ROOT / "build/SLUS_210.75.elf"
TC = ROOT / "tools/ps2dev/ps2dev/ee/bin/mips64r5900el-ps2-elf-"
IMAGE_LO, IMAGE_HI = 0x00100000, 0x0047B200
C_BASE = 0x0F000000          # where the C version is loaded: same 256 MB jal region as the game, clear of random pointers
PTR_LO, PTR_HI = 0x0A000000, 0x0D000000  # random "heap" pointers handed to functions
STACK_TOP = 0x01FF0000
FRAME = 0x10000              # writes within this much below sp are the function's own frame
RET_MAGIC = 0x0DEAD000       # return address sentinel
DEFAULT_RUNS = 20
MAX_STEPS = 50_000           # per run; --max-steps to change
IRQ_FLAGS: list[int] = []    # --irq: bytes set to 1 every 64 steps (flags interrupt handlers set)
OUTPARAM = 4                 # bytes a stub writes through a stack pointer argument
# Runtime helpers that are executed instead of stubbed: they are part of how the
# original code expresses something C code writes inline (e.g. PTMF calls).
INLINE_HELPERS = ("__ptmf_scall", "__ptmf_test", "__nw__FUiPv")
INLINE_ADDRS: set[int] = set()
# va_list arguments (callee address -> register): a pointer to the saved variadic arguments
# in the caller's frame, which sits at different alignments in the two versions
VA_LIST_ARGS = {0x0026ED98: 7}   # vsnprintf(buf, n, fmt, ap)
# variadic callees (address -> first variadic register): a register there the original didn't
# set for the call is not an argument (the format takes fewer), whatever it holds
VARIADIC_FIRST = {0x00380B80: 7, 0x00384730: 11, 0x00384800: 9, 0x0026EDD0: 7}
CALL_ALIAS: dict[int, int] = {}  # C address -> original address of the file's other game functions
C_ENTRIES: set[int] = set()      # entry points of the C file's functions
C_ALL_ENTRIES: set[int] = set()  # ... including static helpers (a computed jump to one is a tail call
                                 # through a random pointer, not a jump inside the function)
HELPER_RANGES: list[tuple[int, int]] = []
FPR_WRITERS = {"lwc1", "mtc1"}
DIRECT_GPR_WRITERS = {"lq", "pcpyld", "pcpyud", "por", "pand", "pxor", "pnor", "paddub", "pextlw", "pextuw"}

M32, M64, M128 = (1 << 32) - 1, (1 << 64) - 1, (1 << 128) - 1
CODE_LO, CODE_HI = 0x00100230, 0x003A1990
ARG_REGS = list(range(4, 12))  # n32: a0-a7 = r4-r11
FARG_REGS = list(range(12, 20))
CALLEE_SAVED = [16, 17, 18, 19, 20, 21, 22, 23, 28, 29, 30, 31]
SCRAMBLE = [1, 3] + list(range(4, 16)) + [24, 25]  # caller-saved besides v0

rz.config.pseudos_enablePseudos = False


class Unsupported(Exception):
    pass


class Trap(Exception):
    pass


def sx32(v: int) -> int:
    v &= M32
    return (v ^ 0x80000000) - 0x80000000


def sx64(v: int) -> int:
    v &= M64
    return (v ^ (1 << 63)) - (1 << 63)


def u64_of_s32(v: int) -> int:
    return sx32(v) & M64


# ---------------------------------------------------------------- floats (EE-ish)
FMAX = 0x7F7FFFFF


def f2b(x: float) -> int:
    if x != x:
        return FMAX
    try:
        b = struct.unpack("<I", struct.pack("<f", x))[0]
    except OverflowError:
        return FMAX | (0x80000000 if x < 0 else 0)
    if b & 0x7F800000 == 0x7F800000:  # inf -> clamp
        return FMAX | (b & 0x80000000)
    if b & 0x7F800000 == 0:  # denormal -> signed zero
        return b & 0x80000000
    return b


def b2f(b: int) -> float:
    if b & 0x7F800000 == 0:
        return -0.0 if b & 0x80000000 else 0.0
    if b & 0x7F800000 == 0x7F800000:  # EE has no inf/nan: treat as huge
        b = FMAX | (b & 0x80000000)
    return struct.unpack("<f", struct.pack("<I", b & M32))[0]


# ---------------------------------------------------------------- value distribution
STUB_RETURNS: list[int] = []  # --stub-ret: values calls return half the time
STUB_RET_PROB = [0.5]         # --stub-ret-prob
OUTPARAM_BYTES = [OUTPARAM]   # --outparam
STUB_FRETURNS: list[float] = []  # --stub-fret: float values calls return (f0)
DICTIONARY: list[int] = []  # constants from the function under test (and +-1), see harvest_constants()


def interesting(rnd: random.Random, bits: int = 32) -> int:
    """Random value biased towards boundary cases (0, 1, -1, small) so branches get exercised."""
    if DICTIONARY and rnd.random() < 0.20:
        return rnd.choice(DICTIONARY) & ((1 << bits) - 1)
    k = rnd.random()
    if k < 0.30:
        return 0
    if k < 0.40:
        return 1
    if k < 0.48:
        return (1 << bits) - 1
    if k < 0.65:
        return rnd.randrange(2, 16)
    if k < 0.85 and bits == 32:
        # a "realistic" float: game data is coordinates/angles/scales, not random bit patterns
        return f2b(rnd.uniform(-1000.0, 1000.0))
    return rnd.getrandbits(bits)


# ---------------------------------------------------------------- memory
class Memory:
    def __init__(self, rom: bytes, seed: int, overlays: list[tuple[int, bytes]]):
        self.rom = rom
        self.seed = seed
        self.pages: dict[int, bytearray] = {}
        self.written: dict[int, int] = {}
        for base, data in overlays:
            for i, b in enumerate(data):
                self._page(base + i)[(base + i) & 0xFFF] = b

    @staticmethod
    def norm(a: int) -> int:
        a &= M32
        if 0x20000000 <= a < 0x40000000:  # uncached mirrors of RAM
            a &= 0x1FFFFFFF
        return a

    def _page(self, a: int) -> bytearray:
        pn = a >> 12
        p = self.pages.get(pn)
        if p is None:
            lo = pn << 12
            if IMAGE_LO <= lo < IMAGE_HI:
                off = lo - IMAGE_LO + 0x80
                p = bytearray(self.rom[off : off + 0x1000].ljust(0x1000, b"\0"))
            elif STACK_TOP - FRAME <= lo < STACK_TOP:
                # uninitialised locals (e.g. the unused w of a vector) read as 0: random bytes
                # would differ between the two versions' frame layouts
                p = bytearray(0x1000)
            else:
                rnd = random.Random(self.seed * 0x9E3779B1 ^ pn)
                p = bytearray(b"".join(interesting(rnd).to_bytes(4, "little") for _ in range(0x400)))
            self.pages[pn] = p
        return p

    def read(self, a: int, n: int) -> int:
        a = self.norm(a)
        if (a & 0xFFF) + n <= 0x1000:
            p = self._page(a)
            o = a & 0xFFF
            return int.from_bytes(p[o : o + n], "little")
        return int.from_bytes(bytes(self.read(a + i, 1) for i in range(n)), "little")

    def write(self, a: int, n: int, v: int) -> None:
        a = self.norm(a)
        for i, b in enumerate((v & ((1 << (8 * n)) - 1)).to_bytes(n, "little")):
            x = a + i
            self._page(x)[x & 0xFFF] = b
            self.written[x] = b


# ---------------------------------------------------------------- libvu0 high-level emulation
# Game code builds vectors on the stack and passes them through libvu0 (VU0 macro code the
# interpreter doesn't run). Stubbing them leaves outputs as garbage that differs between frame
# layouts, so the common ones are emulated: the call is still recorded and compared, but its
# outputs are real (the same model for both versions; exact VU rounding doesn't matter).
import math

VU0_ADDRS: dict[int, str] = {}


def _rv(c, a):
    return [b2f(c.m.read(a + 4 * i, 4)) for i in range(4)]


def _wv(c, a, v):
    for i, x in enumerate(v):
        c.m.write(a + 4 * i, 4, f2b(x))


def _rm(c, a):
    return [_rv(c, a + 16 * k) for k in range(4)]


def _wm(c, a, m):
    for k in range(4):
        _wv(c, a + 16 * k, m[k])


def _apply(m, v):
    return [sum(m[j][i] * v[j] for j in range(4)) for i in range(4)]


def _mulm(a, b):
    return [_apply(a, b[k]) for k in range(4)]


def _rot(axis, t):
    c, s = math.cos(t), math.sin(t)
    if axis == "X":
        return [[1, 0, 0, 0], [0, c, s, 0], [0, -s, c, 0], [0, 0, 0, 1]]
    if axis == "Y":
        return [[c, 0, -s, 0], [0, 1, 0, 0], [s, 0, c, 0], [0, 0, 0, 1]]
    return [[c, s, 0, 0], [-s, c, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]]


def vu0_hle(c, name: str) -> bool:
    """Run libvu0 function `name` on the CPU state; False if it isn't emulated."""
    a0, a1, a2 = (c.g(r) & M32 for r in (4, 5, 6))
    f12 = b2f(c.f[12])
    if name == "sceVu0CopyVector":
        c.m.write(a0, 8, c.m.read(a1, 8)); c.m.write(a0 + 8, 8, c.m.read(a1 + 8, 8))
    elif name == "sceVu0CopyMatrix":
        for i in range(0, 64, 8):
            c.m.write(a0 + i, 8, c.m.read(a1 + i, 8))
    elif name in ("sceVu0AddVector", "sceVu0SubVector", "sceVu0MulVector"):
        x, y = _rv(c, a1), _rv(c, a2)
        op = {"sceVu0AddVector": lambda p, q: p + q, "sceVu0SubVector": lambda p, q: p - q,
              "sceVu0MulVector": lambda p, q: p * q}[name]
        _wv(c, a0, [op(p, q) for p, q in zip(x, y)])
    elif name == "sceVu0ScaleVector":
        _wv(c, a0, [p * f12 for p in _rv(c, a1)])
    elif name == "sceVu0InnerProduct":
        x, y = _rv(c, a0), _rv(c, a1)
        c.f[0] = f2b(x[0] * y[0] + x[1] * y[1] + x[2] * y[2])
    elif name == "sceVu0OuterProduct":
        x, y = _rv(c, a1), _rv(c, a2)
        _wv(c, a0, [x[1] * y[2] - x[2] * y[1], x[2] * y[0] - x[0] * y[2], x[0] * y[1] - x[1] * y[0], 0.0])
    elif name == "sceVu0Normalize":
        v = _rv(c, a1)
        n = math.sqrt(v[0] ** 2 + v[1] ** 2 + v[2] ** 2)
        k = 1.0 / n if n else 0.0
        _wv(c, a0, [v[0] * k, v[1] * k, v[2] * k, v[3]])
    elif name == "sceVu0UnitMatrix":
        _wm(c, a0, [[1.0 if i == k else 0.0 for i in range(4)] for k in range(4)])
    elif name == "sceVu0ApplyMatrix":
        _wv(c, a0, _apply(_rm(c, a1), _rv(c, a2)))
    elif name == "sceVu0MulMatrix":
        _wm(c, a0, _mulm(_rm(c, a1), _rm(c, a2)))
    elif name == "sceVu0TransposeMatrix":
        m = _rm(c, a1)
        _wm(c, a0, [[m[i][k] for i in range(4)] for k in range(4)])
    elif name in ("sceVu0RotMatrixX", "sceVu0RotMatrixY", "sceVu0RotMatrixZ"):
        _wm(c, a0, _mulm(_rot(name[-1], f12), _rm(c, a1)))
    elif name == "sceVu0TransMatrix":
        m, v = _rm(c, a1), _rv(c, a2)
        m[3] = [m[3][0] + v[0], m[3][1] + v[1], m[3][2] + v[2], m[3][3]]
        _wm(c, a0, m)
    else:
        return False
    return True


# ---------------------------------------------------------------- argument usage of callees
_arg_cache: dict[int, tuple[list[int], list[int]]] = {}


def callee_args(rom: bytes, target: int) -> tuple[list[int], list[int]]:
    """Which a0-a7 / f12-f19 a callee reads before writing (linear scan of the original code)."""
    if target in _arg_cache:
        return _arg_cache[target]
    def is_thunk(t: int) -> bool:   # `j f` (+ a this adjustment): a vtable thunk
        if not IMAGE_LO <= t < IMAGE_HI:
            return False
        w0 = struct.unpack_from("<I", rom, t - IMAGE_LO + 0x80)[0]
        return w0 >> 26 == 2 and ((t & 0xF0000000) | ((w0 & 0x3FFFFFF) << 2)) in FUNC_STARTS

    if target not in FUNC_STARTS and not is_thunk(target):  # random/mid-function target
        res = (ARG_REGS, FARG_REGS)
        _arg_cache[target] = res
        return res
    used, fused, written, fwritten = set(), set(), set(), set()
    a = target
    tail = None   # a tail call (thunk: `j f` with an adjustment in the delay slot): follow it
    for _ in range(300):
        if tail is not None and a == tail[0] + 8:
            a = tail[1]
            tail = None
        w = struct.unpack_from("<I", rom, a - IMAGE_LO + 0x80)[0]
        ins = rz.Instruction(w, vram=a, category=rz.InstrCategory.R5900)
        op = w >> 26
        rs, rt, rd = (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
        fs, ft = (w >> 11) & 31, (w >> 16) & 31
        if ins.readsRs() and rs not in written:
            used.add(rs)
        if ins.readsRt() and rt not in written:
            used.add(rt)
        if ins.isFloat() or op in (0x31, 0x39):  # cop1 / lwc1 / swc1
            if ins.readsFs() and fs not in fwritten:
                fused.add(fs)
            if ins.readsFt() and ft not in fwritten:
                fused.add(ft)
            if op == 0x39 and ft not in fwritten:  # swc1 reads ft
                fused.add(ft)
            if ins.modifiesFd():
                fwritten.add((w >> 6) & 31)
            if ins.modifiesFt() or op == 0x31:
                fwritten.add(ft)
        if ins.modifiesRt():
            written.add(rt)
        if ins.modifiesRd():
            written.add(rd)
        if op == 2 and tail is None and ((a & 0xF0000000) | ((w & 0x3FFFFFF) << 2)) in FUNC_STARTS:
            tail = (a, (a & 0xF0000000) | ((w & 0x3FFFFFF) << 2))   # after the delay slot
        elif ins.isJrRa() or (ins.isJump() and not ins.doesLink() and not ins.isBranch()):
            break
        a += 4
    res = ([r for r in ARG_REGS if r in used], [f for f in FARG_REGS if f in fused])
    _arg_cache[target] = res
    return res


# ---------------------------------------------------------------- decoding
def decode(w: int, pc: int) -> tuple:
    """(opcode name, word, GPRs read, FPRs read)"""
    ins = rz.Instruction(w, vram=pc, category=rz.InstrCategory.R5900)
    reads = set()
    if ins.readsRs():
        reads.add((w >> 21) & 31)
    if ins.readsRt():
        reads.add((w >> 16) & 31)
    if ins.readsRd():
        reads.add((w >> 11) & 31)
    freads = set()
    if ins.readsFs():
        freads.add((w >> 11) & 31)
    if ins.readsFt():
        freads.add((w >> 16) & 31)
    if (w >> 26) == 0x39:  # swc1 reads ft
        freads.add((w >> 16) & 31)
    return ins.getOpcodeName(), w, frozenset(reads), frozenset(freads)


# ---------------------------------------------------------------- CPU
class CPU:
    def __init__(self, mem: Memory, rom: bytes, func_lo: int, func_hi: int, seed: int):
        self.m = mem
        self.rom = rom
        self.r = [0] * 32
        self.f = [0] * 32
        self.acc = 0
        self.fcc = False
        self.hi = self.lo = self.hi1 = self.lo1 = 0
        self.save_addrs: set[int] = set()
        self.sa = 0
        self.func_lo, self.func_hi = func_lo, func_hi
        self.events: list[tuple] = []
        self.rng = random.Random(seed * 7919 + 17)
        self.call_n = 0
        self.steps = 0
        self.decoded: dict[int, tuple] = {}
        self.visited: set[int] = set()
        self.max_steps = MAX_STEPS
        self.wset: set[int] = set()   # GPRs written by the function and not read since (entry/last call)
        self.wall: set[int] = set()   # GPRs written by the function at all since entry/last call
        self.fwset: set[int] = set()  # same for FPRs
        self.in_func = False
        self.helper_ranges: list[tuple[int, int]] = HELPER_RANGES
        self.done = False

    # -- register helpers (writes keep the upper 64 bits of the 128-bit GPR)
    def g(self, i: int) -> int:
        return self.r[i] & M64

    def s(self, i: int, v: int) -> None:
        if i:
            self.r[i] = (self.r[i] & ~M64 & M128) | (v & M64)
            if self.in_func:
                self.wset.add(i)
                self.wall.add(i)

    def s32(self, i: int, v: int) -> None:
        self.s(i, u64_of_s32(v))

    def ff(self, i: int) -> float:
        return b2f(self.f[i])

    def sf(self, i: int, x: float) -> None:
        self.f[i] = f2b(x)

    # -- calls
    def stub_call(self, target: int, kind: str) -> None:
        ints, floats = callee_args(self.rom, target)
        # pointers into the frame compare as "a stack pointer": layouts differ between compilers
        # all argument values the callee might read, plus which ones this version set for the
        # call (written and not read since: a caller-saved value left for the call)
        args = {r: self.arg_value(self.g(r) & M32) for r in ints}
        va = VA_LIST_ARGS.get(target)
        if va in args and str(args[va]).startswith("stack"):
            args[va] = "stack"
        args["pending"] = frozenset(r for r in ints if r in self.wset)
        args["written"] = frozenset(r for r in ints if r in self.wall)
        fargs = {r: self.f[r] for r in floats}
        fargs["pending"] = frozenset(r for r in floats if r in self.fwset)
        dig = hashlib.blake2b(
            repr(sorted(self.visible_writes().items())).encode(), digest_size=6
        ).hexdigest()
        self.events.append((kind, target, args, fargs, dig))
        self.in_func = False  # the stub's own register writes below don't count as the function's
        if target in VU0_ADDRS:
            f0 = self.f[0]
            if vu0_hle(self, VU0_ADDRS[target]):
                keep_f0 = VU0_ADDRS[target] == "sceVu0InnerProduct"
                res_f0 = self.f[0]
                self.call_n += 1
                for r in SCRAMBLE:
                    self.s(r, self.rng.getrandbits(64))
                self.f[0] = res_f0 if keep_f0 else f0
                self.wset = set()
                self.wall = set()
                self.fwset = set()
                return
        # Out-parameters: a pointer into the stack frame gets a deterministic
        # value written through it (frame layouts differ between compilers, so
        # leftover stack bytes would otherwise differ between the versions).
        seen = set()
        for k, r in enumerate(ints):
            p = self.g(r) & M32
            if STACK_TOP - FRAME <= p < STACK_TOP and p not in seen:
                seen.add(p)   # (a stale copy of the same pointer in a later register isn't a second output)
                n = OUTPARAM_BYTES[0]
                v = random.Random(self.call_n * 1009 + k * 13 + 7).getrandbits(8 * n)
                self.m.write(p, n, v)
                for i in range(n):  # not the function's own store (see arg_value)
                    self.m.written.pop(Memory.norm(p + i), None)
        self.call_n += 1
        rv = interesting(self.rng)
        if STUB_RETURNS and self.rng.random() < STUB_RET_PROB[0]:
            rv = self.rng.choice(STUB_RETURNS)
        self.s32(2, rv)
        self.s32(3, self.rng.getrandbits(32))
        self.f[0] = f2b(self.rng.uniform(-100, 100))
        if STUB_FRETURNS and self.rng.random() < STUB_RET_PROB[0]:
            self.f[0] = f2b(self.rng.choice(STUB_FRETURNS))
        for r in SCRAMBLE:
            if r != 3:
                self.s(r, self.rng.getrandbits(64))
        self.wset = set()
        self.wall = set()
        self.fwset = set()

    def arg_value(self, v: int):
        """How an argument compares: stack pointers as "stack", C strings by content, else the value."""
        v = CALL_ALIAS.get(v, v)   # the address of another function of the C file: as the original's
        if STACK_TOP - FRAME <= v < STACK_TOP:
            # a local passed by reference: frame layouts differ, so compare what the function
            # stored there (a 16-byte aligned pointer is taken as a vector; unwritten bytes
            # don't count) rather than the address
            if v % 16 == 0:
                w = self.m.written
                # (unwritten stack reads as 0, so "never set" and "set to 0" compare equal)
                content = tuple(None if v + i in self.save_addrs else w.get(v + i, self.m.read(v + i, 1))
                                for i in range(16))
                if any(b is not None for b in content):
                    words = []
                    for i in range(0, 16, 4):
                        bs = content[i:i + 4]
                        if None in bs:
                            words.append("?" if all(b is None for b in bs) else "".join("??" if b is None else f"{b:02X}" for b in reversed(bs)))
                        else:
                            words.append(f"{b2f(int.from_bytes(bytes(bs), 'little')):g}")
                    return "stack{" + ",".join(words) + "}"
            return "stack"
        if IMAGE_LO <= v < IMAGE_HI or C_BASE <= v < C_BASE + 0x100000:
            bs = bytearray()
            for i in range(256):
                b = self.m.read(v + i, 1)
                if b == 0:
                    break
                bs.append(b)
            if 1 <= len(bs) < 256 and all(32 <= b < 127 for b in bs):
                return '"' + bs.decode() + '"'
        return v

    def visible_writes(self) -> dict[int, int]:
        sp0 = STACK_TOP
        # the function's frame and its incoming stack arguments (callee-owned in the EABI: GCC
        # may keep a variable in an argument's slot) are private
        w = {a: b for a, b in self.m.written.items() if not (sp0 - FRAME <= a < sp0 + 0x80)}
        if CALL_ALIAS:
            # a stored address of another function of the C file (a state, a callback): as the
            # original's
            for a in [a for a in w if a % 4 == 0]:
                if a + 1 in w and a + 2 in w and a + 3 in w:
                    v = w[a] | w[a + 1] << 8 | w[a + 2] << 16 | w[a + 3] << 24
                    if v in CALL_ALIAS:
                        for i, b in enumerate(CALL_ALIAS[v].to_bytes(4, "little")):
                            w[a + i] = b
        # a stored address of one of the function's own locals (frames differ between the two)
        for a in [a for a in w if a % 4 == 0]:
            if a + 1 in w and a + 2 in w and a + 3 in w:
                v = w[a] | w[a + 1] << 8 | w[a + 2] << 16 | w[a + 3] << 24
                if sp0 - FRAME <= v < sp0:
                    for i in range(4):
                        w[a + i] = 0x5F
        return w

    SAVE_STORES = {"sq": 16, "sd": 8, "sw": 4, "swc1": 4}
    SAVED_REGS = set(range(16, 24)) | {30, 31}

    def note_save(self, name: str, w: int) -> None:
        """Remember where the prologue saves callee-saved registers (not data for arg_value)."""
        if name in self.SAVE_STORES and _rs(w) == 29 and (_rt(w) >= 20 if name == "swc1" else _rt(w) in self.SAVED_REGS):
            a = (self.g(29) + _imm(w)) & M32
            self.save_addrs.update(range(a, a + self.SAVE_STORES[name]))

    # -- main loop
    def run(self, entry: int) -> None:
        pc, npc = entry, entry + 4
        while not self.done:
            if pc == RET_MAGIC:
                return
            self.steps += 1
            if self.steps > self.max_steps:
                raise TimeoutError
            if IRQ_FLAGS and self.steps % 64 == 0:
                for a in IRQ_FLAGS:   # an "interrupt": not one of the function's own writes
                    self.m.write(a, 1, 1)
                    self.m.written.pop(Memory.norm(a), None)
            self.visited.add(pc)
            d = self.decoded.get(pc)
            if d is None:
                w = self.m.read(pc, 4)
                d = decode(w, pc)
                self.decoded[pc] = d
            name, w, reads, freads = d
            h = HANDLERS.get(name)
            if h is None:
                raise Unsupported(f"{name} at 0x{pc:08X}")
            self.in_func = self.func_lo <= pc < self.func_hi
            if self.in_func:
                self.note_save(name, w)
                # a value read again before the call is scratch, not an argument
                self.wset -= reads
                self.fwset -= freads
            if self.in_func and (name.endswith(".s") or name in FPR_WRITERS):
                before = list(self.f)
                nxt = h(self, w, pc)
                self.fwset.update(i for i in range(32) if self.f[i] != before[i] or (name in FPR_WRITERS and i == _ft(w)))
            else:
                nxt = h(self, w, pc)
            if self.in_func and name in DIRECT_GPR_WRITERS:
                self.wset.add(_rd(w) if name != "lq" else _rt(w))
                self.wall.add(_rd(w) if name != "lq" else _rt(w))
            if nxt is None:
                pc, npc = npc, npc + 4
                continue
            # branch/jump: nxt = (taken, target, likely, link_reg, kind)
            taken, target, likely, link, kind = nxt
            if likely and not taken:
                pc, npc = npc + 4, npc + 8  # skip delay slot
                continue
            # execute delay slot
            ds = npc
            if link is not None:
                self.s(link, (pc + 8) & M64)
            dd = self.decoded.get(ds)
            if dd is None:
                dd = decode(self.m.read(ds, 4), ds)
                self.decoded[ds] = dd
            dname, dw, dreads, dfreads = dd
            dh = HANDLERS.get(dname)
            if dh is None:
                raise Unsupported(f"{dname} at 0x{ds:08X}")
            self.visited.add(ds)
            self.in_func = self.func_lo <= ds < self.func_hi
            if self.in_func:
                self.note_save(dname, dw)
                self.wset -= dreads
                self.fwset -= dfreads
            if self.in_func and (dname.endswith(".s") or dname in FPR_WRITERS):
                self.fwset.add(_fd(dw) if dname.endswith(".s") else _ft(dw) if dname == "lwc1" else _fs(dw))
            if self.in_func and dname in DIRECT_GPR_WRITERS:
                self.wset.add(_rd(dw) if dname != "lq" else _rt(dw))
                self.wall.add(_rd(dw) if dname != "lq" else _rt(dw))
            if dh(self, dw, ds) is not None:
                raise Unsupported(f"branch in delay slot at 0x{ds:08X}")
            self.steps += 1
            if not taken:
                pc, npc = ds + 4, ds + 8
                continue
            if target in CALL_ALIAS and name in ("jal", "j"):
                # another game function the C file also defines: the original calls the
                # original, so the C side's call is stubbed under the original's address
                self.stub_call(CALL_ALIAS[target], "call")
                if kind == "call":
                    pc, npc = pc + 8, pc + 12
                else:
                    pc, npc = self.g(31) & M32, (self.g(31) & M32) + 4
            elif kind == "call" and (target in INLINE_ADDRS or self.func_lo <= target < self.func_hi):
                # direct call to a runtime helper, our own static helper, or recursion: run it
                pc, npc = target, target + 4
            elif kind in ("call", "vcall"):  # indirect calls (vtables, function pointers) are always stubbed
                self.stub_call(target, "call")
                pc, npc = pc + 8, pc + 12
            elif kind == "jump" and target in INLINE_ADDRS and name == "j":
                # (only a direct jump: a computed one landing there came from random memory)
                pc, npc = target, target + 4
            elif kind == "jump" and target != RET_MAGIC and (
                any(lo <= pc < hi for lo, hi in self.helper_ranges)  # helper's final jump (e.g. jr $t9)
                or not (self.func_lo <= target < self.func_hi)       # tail call out of the function
                or (name == "jr" and target in C_ALL_ENTRIES)       # computed tail call to a function
                or (name == "jr" and target & 3)                   # (misaligned: through a random pointer)
            ):
                # the callee returns straight to our caller. A jump out of __ptmf_scall is a
                # pointer-to-member call: a member function taking only `this`.
                in_helper = any(lo <= pc < hi for lo, hi in self.helper_ranges)
                self.stub_call(target, "ptmf" if in_helper else "call")
                pc, npc = self.g(31) & M32, (self.g(31) & M32) + 4
            else:
                pc, npc = target & M32, (target + 4) & M32


# ---------------------------------------------------------------- instruction handlers
def _rs(w): return (w >> 21) & 31
def _rt(w): return (w >> 16) & 31
def _rd(w): return (w >> 11) & 31
def _sa(w): return (w >> 6) & 31
def _imm(w): return (w & 0xFFFF) - 0x10000 if w & 0x8000 else w & 0xFFFF
def _uimm(w): return w & 0xFFFF
def _fs(w): return (w >> 11) & 31
def _ft(w): return (w >> 16) & 31
def _fd(w): return (w >> 6) & 31


def _br(c, w, pc, cond, likely=False, link=None):
    return (cond, (pc + 4 + (_imm(w) << 2)) & M32, likely, link, "branch")


def _addr(c, w):
    return (c.g(_rs(w)) + _imm(w)) & M32


HANDLERS = {}


def H(*names):
    def deco(fn):
        for n in names:
            HANDLERS[n] = fn
        return fn
    return deco


@H("nop", "sync", "sync.p", "sync.l", "ei", "di", "cache", "pref", "ssnop")
def _nop(c, w, pc): return None


@H("addiu", "addi")
def _addiu(c, w, pc): c.s32(_rt(w), c.g(_rs(w)) + _imm(w))


@H("daddiu", "daddi")
def _daddiu(c, w, pc): c.s(_rt(w), c.g(_rs(w)) + _imm(w))


@H("lui")
def _lui(c, w, pc): c.s32(_rt(w), _uimm(w) << 16)


@H("ori")
def _ori(c, w, pc): c.s(_rt(w), c.g(_rs(w)) | _uimm(w))


@H("andi")
def _andi(c, w, pc): c.s(_rt(w), c.g(_rs(w)) & _uimm(w))


@H("xori")
def _xori(c, w, pc): c.s(_rt(w), c.g(_rs(w)) ^ _uimm(w))


@H("slti")
def _slti(c, w, pc): c.s(_rt(w), int(sx64(c.g(_rs(w))) < _imm(w)))


@H("sltiu")
def _sltiu(c, w, pc): c.s(_rt(w), int(c.g(_rs(w)) < (_imm(w) & M64)))


@H("addu", "add")
def _addu(c, w, pc): c.s32(_rd(w), c.g(_rs(w)) + c.g(_rt(w)))


@H("subu", "sub")
def _subu(c, w, pc): c.s32(_rd(w), c.g(_rs(w)) - c.g(_rt(w)))


@H("daddu", "dadd")
def _daddu(c, w, pc): c.s(_rd(w), c.g(_rs(w)) + c.g(_rt(w)))


@H("dsubu", "dsub")
def _dsubu(c, w, pc): c.s(_rd(w), c.g(_rs(w)) - c.g(_rt(w)))


@H("and")
def _and(c, w, pc): c.s(_rd(w), c.g(_rs(w)) & c.g(_rt(w)))


@H("or")
def _or(c, w, pc): c.s(_rd(w), c.g(_rs(w)) | c.g(_rt(w)))


@H("xor")
def _xor(c, w, pc): c.s(_rd(w), c.g(_rs(w)) ^ c.g(_rt(w)))


@H("nor")
def _nor(c, w, pc): c.s(_rd(w), ~(c.g(_rs(w)) | c.g(_rt(w))))


@H("slt")
def _slt(c, w, pc): c.s(_rd(w), int(sx64(c.g(_rs(w))) < sx64(c.g(_rt(w)))))


@H("sltu")
def _sltu(c, w, pc): c.s(_rd(w), int(c.g(_rs(w)) < c.g(_rt(w))))


@H("movz")
def _movz(c, w, pc):
    if c.g(_rt(w)) == 0:
        c.s(_rd(w), c.g(_rs(w)))


@H("movn")
def _movn(c, w, pc):
    if c.g(_rt(w)) != 0:
        c.s(_rd(w), c.g(_rs(w)))


@H("sll")
def _sll(c, w, pc): c.s32(_rd(w), c.g(_rt(w)) << _sa(w))


@H("srl")
def _srl(c, w, pc): c.s32(_rd(w), (c.g(_rt(w)) & M32) >> _sa(w))


@H("sra")
def _sra(c, w, pc): c.s32(_rd(w), sx32(c.g(_rt(w))) >> _sa(w))


@H("sllv")
def _sllv(c, w, pc): c.s32(_rd(w), c.g(_rt(w)) << (c.g(_rs(w)) & 31))


@H("srlv")
def _srlv(c, w, pc): c.s32(_rd(w), (c.g(_rt(w)) & M32) >> (c.g(_rs(w)) & 31))


@H("srav")
def _srav(c, w, pc): c.s32(_rd(w), sx32(c.g(_rt(w))) >> (c.g(_rs(w)) & 31))


@H("dsll")
def _dsll(c, w, pc): c.s(_rd(w), c.g(_rt(w)) << _sa(w))


@H("dsrl")
def _dsrl(c, w, pc): c.s(_rd(w), c.g(_rt(w)) >> _sa(w))


@H("dsra")
def _dsra(c, w, pc): c.s(_rd(w), sx64(c.g(_rt(w))) >> _sa(w))


@H("dsll32")
def _dsll32(c, w, pc): c.s(_rd(w), c.g(_rt(w)) << (_sa(w) + 32))


@H("dsrl32")
def _dsrl32(c, w, pc): c.s(_rd(w), c.g(_rt(w)) >> (_sa(w) + 32))


@H("dsra32")
def _dsra32(c, w, pc): c.s(_rd(w), sx64(c.g(_rt(w))) >> (_sa(w) + 32))


@H("dsllv")
def _dsllv(c, w, pc): c.s(_rd(w), c.g(_rt(w)) << (c.g(_rs(w)) & 63))


@H("dsrlv")
def _dsrlv(c, w, pc): c.s(_rd(w), c.g(_rt(w)) >> (c.g(_rs(w)) & 63))


@H("dsrav")
def _dsrav(c, w, pc): c.s(_rd(w), sx64(c.g(_rt(w))) >> (c.g(_rs(w)) & 63))


def _mult(c, w, signed, pipe1=False):
    a, b = c.g(_rs(w)) & M32, c.g(_rt(w)) & M32
    if signed:
        a, b = sx32(a), sx32(b)
    p = a * b
    lo, hi = u64_of_s32(p), u64_of_s32(p >> 32)
    if pipe1:
        c.lo1, c.hi1 = lo, hi
    else:
        c.lo, c.hi = lo, hi
    if _rd(w):
        c.s(_rd(w), lo)


@H("mult")
def _h_mult(c, w, pc): _mult(c, w, True)


@H("multu")
def _h_multu(c, w, pc): _mult(c, w, False)


@H("mult1")
def _h_mult1(c, w, pc): _mult(c, w, True, True)


@H("multu1")
def _h_multu1(c, w, pc): _mult(c, w, False, True)


def _madd(c, w, signed, pipe1=False):
    a, b = c.g(_rs(w)) & M32, c.g(_rt(w)) & M32
    if signed:
        a, b = sx32(a), sx32(b)
    lo, hi = (c.lo1, c.hi1) if pipe1 else (c.lo, c.hi)
    accv = ((hi & M32) << 32) | (lo & M32)
    accv = (accv + a * b) & M64
    lo, hi = u64_of_s32(accv), u64_of_s32(accv >> 32)
    if pipe1:
        c.lo1, c.hi1 = lo, hi
    else:
        c.lo, c.hi = lo, hi
    if _rd(w):
        c.s(_rd(w), lo)


@H("madd")
def _h_madd(c, w, pc): _madd(c, w, True)


@H("maddu")
def _h_maddu(c, w, pc): _madd(c, w, False)


@H("madd1")
def _h_madd1(c, w, pc): _madd(c, w, True, True)


def _div(c, w, signed, pipe1=False):
    a, b = c.g(_rs(w)) & M32, c.g(_rt(w)) & M32
    if signed:
        a, b = sx32(a), sx32(b)
    if b == 0:
        q, r = (1 if (signed and a < 0) else -1), a
    elif signed and a == -0x80000000 and b == -1:
        q, r = a, 0
    else:
        q = abs(a) // abs(b) * (1 if (a < 0) == (b < 0) else -1)
        r = a - q * b
    lo, hi = u64_of_s32(q), u64_of_s32(r)
    if pipe1:
        c.lo1, c.hi1 = lo, hi
    else:
        c.lo, c.hi = lo, hi


@H("div")
def _h_div(c, w, pc): _div(c, w, True)


@H("divu")
def _h_divu(c, w, pc): _div(c, w, False)


@H("div1")
def _h_div1(c, w, pc): _div(c, w, True, True)


@H("divu1")
def _h_divu1(c, w, pc): _div(c, w, False, True)


@H("mflo")
def _mflo(c, w, pc): c.s(_rd(w), c.lo)


@H("mfhi")
def _mfhi(c, w, pc): c.s(_rd(w), c.hi)


@H("mflo1")
def _mflo1(c, w, pc): c.s(_rd(w), c.lo1)


@H("mfhi1")
def _mfhi1(c, w, pc): c.s(_rd(w), c.hi1)


@H("mtlo")
def _mtlo(c, w, pc): c.lo = c.g(_rs(w))


@H("mthi")
def _mthi(c, w, pc): c.hi = c.g(_rs(w))


@H("mtlo1")
def _mtlo1(c, w, pc): c.lo1 = c.g(_rs(w))


@H("mthi1")
def _mthi1(c, w, pc): c.hi1 = c.g(_rs(w))


@H("mfsa")
def _mfsa(c, w, pc): c.s(_rd(w), c.sa)


@H("mtsa")
def _mtsa(c, w, pc): c.sa = c.g(_rs(w)) & 0xF


# -- loads/stores
@H("lb")
def _lb(c, w, pc):
    v = c.m.read(_addr(c, w), 1)
    c.s(_rt(w), (v - 0x100 if v & 0x80 else v) & M64)


@H("lbu")
def _lbu(c, w, pc): c.s(_rt(w), c.m.read(_addr(c, w), 1))


@H("lh")
def _lh(c, w, pc):
    v = c.m.read(_addr(c, w), 2)
    c.s(_rt(w), (v - 0x10000 if v & 0x8000 else v) & M64)


@H("lhu")
def _lhu(c, w, pc): c.s(_rt(w), c.m.read(_addr(c, w), 2))


@H("lw")
def _lw(c, w, pc): c.s32(_rt(w), c.m.read(_addr(c, w), 4))


@H("lwu")
def _lwu(c, w, pc): c.s(_rt(w), c.m.read(_addr(c, w), 4))


@H("ld")
def _ld(c, w, pc): c.s(_rt(w), c.m.read(_addr(c, w), 8))


@H("lq")
def _lq(c, w, pc):
    if _rt(w):
        c.r[_rt(w)] = c.m.read(_addr(c, w) & ~0xF, 16)


@H("sb")
def _sb(c, w, pc): c.m.write(_addr(c, w), 1, c.g(_rt(w)))


@H("sh")
def _sh(c, w, pc): c.m.write(_addr(c, w), 2, c.g(_rt(w)))


@H("sw")
def _sw(c, w, pc): c.m.write(_addr(c, w), 4, c.g(_rt(w)))


@H("sd")
def _sd(c, w, pc): c.m.write(_addr(c, w), 8, c.g(_rt(w)))


@H("sq")
def _sq(c, w, pc): c.m.write(_addr(c, w) & ~0xF, 16, c.r[_rt(w)])


def _unaligned(c, w, size, left, store):
    a = _addr(c, w)
    base = a & ~(size - 1)
    k = a & (size - 1)
    mem = c.m.read(base, size)
    reg = c.g(_rt(w))
    bits = 8 * size
    if not store:
        if left:  # lwl/ldl: high bytes of reg <- mem bytes [0..k]
            n = k + 1
            mask = ((1 << (8 * n)) - 1) << (bits - 8 * n)
            v = (reg & ~mask) | ((mem << (bits - 8 * n)) & mask)
        else:     # lwr/ldr: low bytes of reg <- mem bytes [k..size-1]
            n = size - k
            mask = (1 << (8 * n)) - 1
            v = (reg & ~mask) | ((mem >> (8 * k)) & mask)
        if size == 4:
            c.s32(_rt(w), v)
        else:
            c.s(_rt(w), v)
    else:
        if left:  # swl/sdl
            n = k + 1
            mask = (1 << (8 * n)) - 1
            v = (mem & ~mask) | ((reg >> (bits - 8 * n)) & mask)
        else:     # swr/sdr
            n = size - k
            mask = ((1 << (8 * n)) - 1) << (8 * k)
            v = (mem & ~mask) | ((reg << (8 * k)) & mask)
        c.m.write(base, size, v)


@H("lwl")
def _lwl(c, w, pc): _unaligned(c, w, 4, True, False)


@H("lwr")
def _lwr(c, w, pc): _unaligned(c, w, 4, False, False)


@H("swl")
def _swl(c, w, pc): _unaligned(c, w, 4, True, True)


@H("swr")
def _swr(c, w, pc): _unaligned(c, w, 4, False, True)


@H("ldl")
def _ldl(c, w, pc): _unaligned(c, w, 8, True, False)


@H("ldr")
def _ldr(c, w, pc): _unaligned(c, w, 8, False, False)


@H("sdl")
def _sdl(c, w, pc): _unaligned(c, w, 8, True, True)


@H("sdr")
def _sdr(c, w, pc): _unaligned(c, w, 8, False, True)


@H("lwc1")
def _lwc1(c, w, pc): c.f[_ft(w)] = c.m.read(_addr(c, w), 4)


@H("swc1")
def _swc1(c, w, pc): c.m.write(_addr(c, w), 4, c.f[_ft(w)])


# -- branches / jumps
@H("beq")
def _beq(c, w, pc): return _br(c, w, pc, c.g(_rs(w)) == c.g(_rt(w)))


@H("bne")
def _bne(c, w, pc): return _br(c, w, pc, c.g(_rs(w)) != c.g(_rt(w)))


@H("beql")
def _beql(c, w, pc): return _br(c, w, pc, c.g(_rs(w)) == c.g(_rt(w)), True)


@H("bnel")
def _bnel(c, w, pc): return _br(c, w, pc, c.g(_rs(w)) != c.g(_rt(w)), True)


@H("blez")
def _blez(c, w, pc): return _br(c, w, pc, sx64(c.g(_rs(w))) <= 0)


@H("bgtz")
def _bgtz(c, w, pc): return _br(c, w, pc, sx64(c.g(_rs(w))) > 0)


@H("blezl")
def _blezl(c, w, pc): return _br(c, w, pc, sx64(c.g(_rs(w))) <= 0, True)


@H("bgtzl")
def _bgtzl(c, w, pc): return _br(c, w, pc, sx64(c.g(_rs(w))) > 0, True)


@H("bltz")
def _bltz(c, w, pc): return _br(c, w, pc, sx64(c.g(_rs(w))) < 0)


@H("bgez")
def _bgez(c, w, pc): return _br(c, w, pc, sx64(c.g(_rs(w))) >= 0)


@H("bltzl")
def _bltzl(c, w, pc): return _br(c, w, pc, sx64(c.g(_rs(w))) < 0, True)


@H("bgezl")
def _bgezl(c, w, pc): return _br(c, w, pc, sx64(c.g(_rs(w))) >= 0, True)


@H("bltzal")
def _bltzal(c, w, pc):
    t = _br(c, w, pc, sx64(c.g(_rs(w))) < 0)
    return (t[0], t[1], False, 31, "call")


@H("bgezal")
def _bgezal(c, w, pc):
    t = _br(c, w, pc, sx64(c.g(_rs(w))) >= 0)
    return (t[0], t[1], False, 31, "call")


@H("bc1t")
def _bc1t(c, w, pc): return _br(c, w, pc, c.fcc)


@H("bc1f")
def _bc1f(c, w, pc): return _br(c, w, pc, not c.fcc)


@H("bc1tl")
def _bc1tl(c, w, pc): return _br(c, w, pc, c.fcc, True)


@H("bc1fl")
def _bc1fl(c, w, pc): return _br(c, w, pc, not c.fcc, True)


@H("j")
def _j(c, w, pc): return (True, (pc & 0xF0000000) | ((w & 0x3FFFFFF) << 2), False, None, "jump")


@H("jal")
def _jal(c, w, pc): return (True, (pc & 0xF0000000) | ((w & 0x3FFFFFF) << 2), False, 31, "call")


@H("jr")
def _jr(c, w, pc):
    t = c.g(_rs(w)) & M32
    if _rs(w) == 31 or t == RET_MAGIC:
        return (True, t, False, None, "return")
    return (True, t, False, None, "jump")


@H("jalr")
def _jalr(c, w, pc): return (True, c.g(_rs(w)) & M32, False, _rd(w), "vcall")
# (an indirect call in the C version is recorded as "call"; see stub_call)


@H("syscall")
def _syscall(c, w, pc):
    c.stub_call(0xFFFF0000 | (c.g(3) & 0xFFFF), "syscall")


@H("break", "teq", "tne", "tlt", "tltu", "tge", "tgeu", "teqi", "tnei", "tlti", "tltiu", "tgei", "tgeiu")
def _trap(c, w, pc):
    name = rz.Instruction(w, category=rz.InstrCategory.R5900).getOpcodeName()
    a, b = sx64(c.g(_rs(w))), sx64(c.g(_rt(w)))
    cond = {"break": True, "teq": a == b, "tne": a != b, "tlt": a < b, "tge": a >= b,
            "tltu": c.g(_rs(w)) < c.g(_rt(w)), "tgeu": c.g(_rs(w)) >= c.g(_rt(w))}.get(name, False)
    if cond:
        raise Trap(name)


# -- FPU
@H("mtc1")
def _mtc1(c, w, pc): c.f[_fs(w)] = c.g(_rt(w)) & M32


@H("mfc1")
def _mfc1(c, w, pc): c.s32(_rt(w), c.f[_fs(w)])


@H("ctc1", "cfc1")
def _cxc1(c, w, pc):
    if (w >> 21) & 31 == 2:  # cfc1
        c.s32(_rt(w), (0x800000 if c.fcc else 0) | 1)


@H("mov.s")
def _movs(c, w, pc): c.f[_fd(w)] = c.f[_fs(w)]


@H("neg.s")
def _negs(c, w, pc): c.f[_fd(w)] = c.f[_fs(w)] ^ 0x80000000


@H("abs.s")
def _abss(c, w, pc): c.f[_fd(w)] = c.f[_fs(w)] & 0x7FFFFFFF


@H("add.s")
def _adds(c, w, pc): c.sf(_fd(w), c.ff(_fs(w)) + c.ff(_ft(w)))


@H("sub.s")
def _subs(c, w, pc): c.sf(_fd(w), c.ff(_fs(w)) - c.ff(_ft(w)))


@H("mul.s")
def _muls(c, w, pc): c.sf(_fd(w), c.ff(_fs(w)) * c.ff(_ft(w)))


@H("div.s")
def _divs(c, w, pc):
    a, b = c.ff(_fs(w)), c.ff(_ft(w))
    c.sf(_fd(w), a / b if b != 0 else (3.4e38 if (a >= 0) == (str(b)[0] != "-") else -3.4e38))


@H("sqrt.s")
def _sqrts(c, w, pc): c.sf(_fd(w), abs(c.ff(_ft(w))) ** 0.5)


@H("c1")
def _c1(c, w, pc):
    # rabbitizer names some EE COP1 ops just "c1"; function 4 is the EE's sqrt.s fd, ft
    if (w >> 21) & 31 == 16 and w & 0x3F == 4:
        return _sqrts(c, w, pc)
    raise Unsupported(f"c1 0x{w:08X} at 0x{pc:08X}")


@H("rsqrt.s")
def _rsqrts(c, w, pc):
    b = abs(c.ff(_ft(w))) ** 0.5
    c.sf(_fd(w), c.ff(_fs(w)) / b if b else 3.4e38)


@H("max.s")
def _maxs(c, w, pc): c.sf(_fd(w), max(c.ff(_fs(w)), c.ff(_ft(w))))


@H("min.s")
def _mins(c, w, pc): c.sf(_fd(w), min(c.ff(_fs(w)), c.ff(_ft(w))))


@H("adda.s")
def _addas(c, w, pc): c.acc = f2b(c.ff(_fs(w)) + c.ff(_ft(w)))


@H("suba.s")
def _subas(c, w, pc): c.acc = f2b(c.ff(_fs(w)) - c.ff(_ft(w)))


@H("mula.s")
def _mulas(c, w, pc): c.acc = f2b(c.ff(_fs(w)) * c.ff(_ft(w)))


# multiply-accumulate: like hardware (and PCSX2), the product is rounded to single first
def _prod(c, w): return b2f(f2b(c.ff(_fs(w)) * c.ff(_ft(w))))


@H("madda.s")
def _maddas(c, w, pc): c.acc = f2b(b2f(c.acc) + _prod(c, w))


@H("msuba.s")
def _msubas(c, w, pc): c.acc = f2b(b2f(c.acc) - _prod(c, w))


@H("madd.s")
def _madds(c, w, pc): c.sf(_fd(w), b2f(c.acc) + _prod(c, w))


@H("msub.s")
def _msubs(c, w, pc): c.sf(_fd(w), b2f(c.acc) - _prod(c, w))


@H("cvt.s.w")
def _cvtsw(c, w, pc): c.sf(_fd(w), float(sx32(c.f[_fs(w)])))


@H("cvt.w.s")
def _cvtws(c, w, pc):
    x = c.ff(_fs(w))
    v = int(x)  # truncate
    c.f[_fd(w)] = max(-0x80000000, min(0x7FFFFFFF, v)) & M32


@H("c.eq.s")
def _ceq(c, w, pc): c.fcc = c.ff(_fs(w)) == c.ff(_ft(w))


@H("c.lt.s")
def _clt(c, w, pc): c.fcc = c.ff(_fs(w)) < c.ff(_ft(w))


@H("c.le.s")
def _cle(c, w, pc): c.fcc = c.ff(_fs(w)) <= c.ff(_ft(w))


@H("c.f.s")
def _cf(c, w, pc): c.fcc = False


# -- MMI (only what the game uses; extend as needed)
@H("pcpyld")
def _pcpyld(c, w, pc):
    if _rd(w):
        c.r[_rd(w)] = ((c.r[_rs(w)] & M64) << 64) | (c.r[_rt(w)] & M64)


@H("pcpyud")
def _pcpyud(c, w, pc):
    if _rd(w):
        c.r[_rd(w)] = (c.r[_rs(w)] >> 64) | ((c.r[_rt(w)] >> 64) << 64)


@H("por")
def _por(c, w, pc):
    if _rd(w):
        c.r[_rd(w)] = c.r[_rs(w)] | c.r[_rt(w)]


@H("pand")
def _pand(c, w, pc):
    if _rd(w):
        c.r[_rd(w)] = c.r[_rs(w)] & c.r[_rt(w)]


@H("pxor")
def _pxor(c, w, pc):
    if _rd(w):
        c.r[_rd(w)] = c.r[_rs(w)] ^ c.r[_rt(w)]


@H("pnor")
def _pnor(c, w, pc):
    if _rd(w):
        c.r[_rd(w)] = ~(c.r[_rs(w)] | c.r[_rt(w)]) & M128


@H("paddub")
def _paddub(c, w, pc):
    if _rd(w):
        a, b = c.r[_rs(w)], c.r[_rt(w)]
        c.r[_rd(w)] = sum(min(255, ((a >> (8 * i)) & 255) + ((b >> (8 * i)) & 255)) << (8 * i) for i in range(16))


@H("pextlw")
def _pextlw(c, w, pc):
    if _rd(w):
        a, b = c.r[_rs(w)], c.r[_rt(w)]
        lw = lambda x, i: (x >> (32 * i)) & M32
        c.r[_rd(w)] = lw(b, 0) | (lw(a, 0) << 32) | (lw(b, 1) << 64) | (lw(a, 1) << 96)


@H("pextuw")
def _pextuw(c, w, pc):
    if _rd(w):
        a, b = c.r[_rs(w)], c.r[_rt(w)]
        lw = lambda x, i: (x >> (32 * i)) & M32
        c.r[_rd(w)] = lw(b, 2) | (lw(a, 2) << 32) | (lw(b, 3) << 64) | (lw(a, 3) << 96)


# ---------------------------------------------------------------- building the C side
def cflags() -> str:
    sys.path.insert(0, str(ROOT))
    import configure  # noqa: E402  (main() only runs as a script)

    return configure.CFLAGS


SYM_CACHE = ROOT / "build/difftest_syms.pickle"
_SYMS: list[tuple[str, int, int, str, str]] | None = None


def game_symbols() -> list[tuple[str, int, int, str, str]]:
    """(name, value, size, bind, type) of every symbol of the game ELF. Reading the symtab
    with pyelftools takes ~30 s, so the result is cached and redone when the ELF changes."""
    global _SYMS
    if _SYMS is not None:
        return _SYMS
    st = BUILD_ELF.stat()
    key = (st.st_mtime_ns, st.st_size)
    try:
        with open(SYM_CACHE, "rb") as f:
            k, syms = pickle.load(f)
        if k == key:
            _SYMS = syms
            return syms
    except (OSError, EOFError, ValueError, pickle.UnpicklingError):
        pass
    with open(BUILD_ELF, "rb") as f:
        syms = [(s.name, s["st_value"], s["st_size"], s["st_info"]["bind"], s["st_info"]["type"])
                for s in ELFFile(f).get_section_by_name(".symtab").iter_symbols()]
    tmp = SYM_CACHE.with_suffix(".tmp")
    with open(tmp, "wb") as f:
        pickle.dump((key, syms), f)
    tmp.replace(SYM_CACHE)
    _SYMS = syms
    return syms


def build_c(src: Path, workdir: Path) -> tuple[list[tuple[int, bytes]], dict[str, int], tuple[int, int]]:
    """Compile and link a C file once: (loadable sections, address of each global function, .text range)."""
    obj = workdir / "t.o"
    elf = workdir / "t.elf"
    cxx = src.suffix in (".cpp", ".cc")
    comp = f"{TC}{'g++' if cxx else 'gcc'}"
    flags = cflags().split() + (["-fno-exceptions", "-fno-rtti"] if cxx else [])
    # a function the C file calls must stay a call, to compare with the original's call
    flags += ["-fno-inline-small-functions", "-fno-inline-functions", "-fno-inline-functions-called-once",
              "-fno-ipa-cp", "-fno-ipa-sra", "-fno-ipa-icf", "-fno-ipa-pure-const", "-fno-ipa-modref", "-fno-ipa-ra",
              "-fno-ipa-vrp", "-fno-ipa-bit-cp"]
    subprocess.run([sys.executable, str(ROOT / "tools/eecc.py"), comp, "-c", *flags, "-I", str(ROOT / "include"),
                    "-I", str(ROOT / "src"), "-o", str(obj), str(src)], check=True, cwd=ROOT)
    defined = set(subprocess.run([f"{TC}nm", "--defined-only", "-j", str(obj)], capture_output=True,
                                 text=True, check=True).stdout.split())
    # every game symbol the C may reference, except the ones the C defines itself
    syms = [f"{name} = 0x{value:08X};" for name, value, _, bind, _ in game_symbols()
            if name and bind == "STB_GLOBAL" and name not in defined and re.fullmatch(r"[A-Za-z_.$][\w.$]*", name)]
    ld = workdir / "t.ld"
    ld.write_text(
        "SECTIONS {\n"
        f"  . = 0x{C_BASE:08X};\n"
        "  .text : { *(.text*) }\n  .rodata : { *(.rodata*) }\n"
        "  .data : { *(.data*) *(.sdata*) }\n  .bss : { *(.bss*) *(.sbss*) *(COMMON) }\n"
        "  /DISCARD/ : { *(.MIPS.abiflags) *(.reginfo) *(.comment) *(.pdr) *(.gnu.attributes) *(.mdebug*) }\n"
        "}\n" + "\n".join(syms) + "\n"
    )
    subprocess.run([f"{TC}ld", "-EL", "-m", "elf32lr5900n32", "--no-warn-mismatch", "-T", str(ld), "-o", str(elf), str(obj)],
                   check=True, cwd=ROOT)
    with open(elf, "rb") as f:
        e = ELFFile(f)
        overlays = [(s["sh_addr"], s.data()) for s in e.iter_sections()
                    if s["sh_flags"] & 2 and s["sh_type"] == "SHT_PROGBITS" and s["sh_size"]]
        funcs = {s.name: s["st_value"] for s in e.get_section_by_name(".symtab").iter_symbols()
                 if s["st_info"]["type"] == "STT_FUNC" and s["st_info"]["bind"] == "STB_GLOBAL"}
        C_ALL_ENTRIES.clear()
        C_ALL_ENTRIES.update(s["st_value"] for s in e.get_section_by_name(".symtab").iter_symbols()
                             if s["st_info"]["type"] == "STT_FUNC")
        C_ALL_ENTRIES.update(INLINE_ADDRS)
        text = e.get_section_by_name(".text")
        # the whole .text counts as "the function": static helpers GCC didn't inline are part of it
        trange = (text["sh_addr"], text["sh_addr"] + text["sh_size"])
    return overlays, dict(sorted(funcs.items(), key=lambda kv: kv[1])), trange


def harvest_constants(rom: bytes, lo: int, hi: int) -> None:
    """Immediates used by the original function (compare thresholds, masks...) and their +-2 neighbours
    (a counter is often incremented before it is compared)."""
    vals = set()
    for a in range(lo, hi, 4):
        w = struct.unpack_from("<I", rom, a - IMAGE_LO + 0x80)[0]
        op = w >> 26
        rs = (w >> 21) & 31
        # comparisons and masks (slti/sltiu/andi/ori/xori), and constants loaded with
        # addiu/daddiu from $zero; not offsets added to a base register
        if op in (0x0A, 0x0B, 0x0C, 0x0D, 0x0E) or (op in (0x09, 0x19) and rs == 0):
            imm = w & 0xFFFF
            simm = imm - 0x10000 if imm & 0x8000 else imm
            for v in {imm, simm}:
                if abs(v) > 1:
                    vals.update({v - 2, v - 1, v, v + 1, v + 2})
    DICTIONARY[:] = sorted(v & M32 for v in vals)


FUNC_STARTS: set[int] = set()


def load_helpers() -> None:
    if FUNC_STARTS:
        return
    for name, value, size, _, typ in game_symbols():
        if name.startswith("sceVu0") and "." not in name:
            VU0_ADDRS[value] = name
        if typ == "STT_FUNC" and CODE_LO <= value < CODE_HI:
            FUNC_STARTS.add(value)
        if name in INLINE_HELPERS:
            INLINE_ADDRS.add(value)
            HELPER_RANGES.append((value, value + max(size, 4)))


def original_range(func: str) -> tuple[int, int] | None:
    for name, value, size, _, _ in game_symbols():
        if name == func:
            return value, value + max(size, 4)
    return None


def return_kind(src: Path, func: str) -> str:
    m = re.search(rf"^\s*(?:extern\s+|static\s+)?([\w\s\*]+?)\s*\b{re.escape(func)}\s*\(", src.read_text(), re.M)
    t = m.group(1).strip() if m else "s32"
    if t == "void":
        return "none"
    if t in ("f32", "float"):
        return "f0"
    if t in ("s64", "u64", "long long", "unsigned long long"):
        return "v0_64"
    return "v0"


# ---------------------------------------------------------------- one run
def make_inputs(seed: int) -> tuple[list[int], list[int]]:
    rnd = random.Random(seed)

    def ival() -> int:
        k = rnd.random()
        if k < 0.35:
            return rnd.randrange(PTR_LO, PTR_HI) & ~3
        if k < 0.6:
            return rnd.randrange(-4, 16) & M64
        if k < 0.75:
            return rnd.randrange(0x00400000, 0x00480000) & ~3  # into game data
        return rnd.getrandbits(32)

    ints = [ival() for _ in ARG_REGS]
    floats = [f2b(rnd.choice([0.0, 1.0, -1.0, 0.5, rnd.uniform(-1000, 1000)])) for _ in FARG_REGS]
    return ints, floats


PRE_BYTES: list[tuple[int, int, bytes]] = []   # (arg reg or 0 for absolute, offset, bytes)
REL_BASE = -(1 << 40)   # lo = REL_BASE - register: the value is that argument + hi
PRECONDITIONS: list[tuple[int, int, int, int]] = []  # (arg reg, offset, lo, hi): *(u32 *)(arg + off) in lo..hi


def parse_pre(spec: str) -> tuple[int, int | None, int, int]:
    """a0+0x18=0..8: u32 at arg0+0x18 in 0..8; a1=0..3: the argument itself (offset None)."""
    b = re.fullmatch(r"(?:a([0-7])\+|@)(0x[0-9A-Fa-f]+|\d+)=bytes:((?:[0-9A-Fa-f]{2}|\?\?)+)", spec)
    if b:  # exact bytes (e.g. a script for a byte-code interpreter): a0+0x100=bytes:41421700
        # "??": a random byte (per run)
        PRE_BYTES.append((4 + int(b.group(1)) if b.group(1) else 0, int(b.group(2), 0), b.group(3)))
        return None
    r = re.fullmatch(r"a([0-7])\+(0x[0-9A-Fa-f]+|\d+)=a([0-7])\+(0x[0-9A-Fa-f]+|\d+)", spec)
    if r:  # a pointer into an argument (3 runs in 4): a0+0x304A14=a0+0x60; lo = REL_BASE - register marks it
        return 4 + int(r.group(1)), int(r.group(2), 0), REL_BASE - (4 + int(r.group(3))), int(r.group(4), 0)
    g = re.fullmatch(r"@(0x[0-9A-Fa-f]+)=(-?\w+)\.\.(-?\w+)", spec)
    if g:  # a global: u32 at an absolute address (register 0 + address)
        return 0, int(g.group(1), 16), int(g.group(2), 0), int(g.group(3), 0)
    m = re.fullmatch(r"a([0-7])(?:\+(0x[0-9A-Fa-f]+|\d+))?=(-?\w+)\.\.(-?\w+)", spec)
    if not m:
        sys.exit(f"bad --pre {spec!r}: expected e.g. a0+0x18=0..8, a1=0..3 or @0x44E568=lo..hi")
    off = int(m.group(2), 0) if m.group(2) is not None else None
    return 4 + int(m.group(1)), off, int(m.group(3), 0), int(m.group(4), 0)


def run_one(rom, overlays, entry, frange, seed, max_steps=None):
    mem = Memory(rom, seed, overlays)
    c = CPU(mem, rom, frange[0], frange[1], seed)
    if max_steps:
        c.max_steps = max_steps
    ints, floats = make_inputs(seed)
    for r, v in zip(ARG_REGS, ints):
        c.s(r, v)
    for reg, off, lo, hi in PRECONDITIONS:
        # a constrained input: write it without logging it as a function write
        rnd = random.Random(seed * 7 + (off if off is not None else 0x7FFF0000 + reg))
        if lo <= REL_BASE - 4:
            if rnd.random() < 0.75:
                v = ((c.g(REL_BASE - lo) & M32) + hi) & M32
                a = Memory.norm((c.g(reg) & M32) + off)
                for i, b in enumerate(v.to_bytes(4, "little")):
                    mem._page(a + i)[(a + i) & 0xFFF] = b
            continue
        # half the time a boundary or one of the function's own constants within the range
        special = sorted({lo, lo + 1, hi, hi - 1} | {d for d in DICTIONARY if lo <= d <= hi})
        special = [x for x in special if lo <= x <= hi]
        v = (rnd.choice(special) if rnd.random() < 0.5 else rnd.randint(lo, hi)) & M32
        if off is None:
            c.s32(reg, v)
            continue
        a = Memory.norm((c.g(reg) & M32) + off)
        for i, b in enumerate(v.to_bytes(4, "little")):
            mem._page(a + i)[(a + i) & 0xFFF] = b
    for k, (reg, off, hexs) in enumerate(PRE_BYTES):
        a = Memory.norm(((c.g(reg) & M32) if reg else 0) + off)
        rnd = random.Random(seed * 7919 + k)
        for i in range(len(hexs) // 2):
            h = hexs[i * 2:i * 2 + 2]
            mem._page(a + i)[(a + i) & 0xFFF] = rnd.getrandbits(8) if h == "??" else int(h, 16)
    for r, v in zip(FARG_REGS, floats):
        c.f[r] = v
    for r in CALLEE_SAVED + [1, 2, 3, 12, 13, 14, 15, 24, 25]:
        if r not in (28, 29, 31):
            c.s(r, random.Random(seed * 31 + r).getrandbits(64))
    c.s(28, 0x004828F0)       # gp
    c.s(29, STACK_TOP)        # sp
    c.s(31, RET_MAGIC)        # ra
    saved = {r: c.g(r) for r in CALLEE_SAVED}
    for i in range(20, 32):
        c.f[i] = f2b(random.Random(seed * 37 + i).uniform(-10, 10))
    fsaved = {i: c.f[i] for i in range(20, 32)}
    c.timed_out = False
    try:
        c.run(entry)
    except TimeoutError:
        c.timed_out = True
    return c, saved, fsaved


def event_equal(a, b) -> bool:
    """Same call, same digest, same arguments (a = original, b = C).

    "pending" = written and not read again before the call (caller-saved, so an argument
    or a dead write). For a known callee only the registers it actually reads count."""
    (ka, ta, aa, fa, da), (kb, tb, ab, fb, db) = a, b
    if "ptmf" in (ka, kb):
        # pointer-to-member call (original via __ptmf_scall, C inline): only `this` is an argument
        return (ta, da) == (tb, db) and aa.get(4) == ab.get(4)
    if (ka, ta, da) != (kb, tb, db):
        return False
    # a = original, b = C. Compare what the original left for the call, plus what the C left
    # for it if the original also wrote that register (MW reuses e.g. a compare constant as
    # an argument; GCC can leave dead loop-invariant writes, which the original won't match)
    regs = aa["pending"] | (ab["pending"] & aa["written"])
    if ta in FUNC_STARTS:
        # a known callee reads exactly these: one the original passes through untouched
        # (e.g. an argument of its own) must reach the callee unchanged in the C too
        first_va = VARIADIC_FIRST.get(ta, 99)
        regs |= {r for r in aa if isinstance(r, int) and r not in aa["written"] and r < first_va}
    fregs = fa["pending"] | fb["pending"]
    return all(_arg_eq(aa.get(r), ab.get(r)) for r in regs) and all(fa.get(r) == fb.get(r) for r in fregs)


def _arg_eq(x, y) -> bool:
    """Argument values equal; a stack pointer compares by content only if both are vectors."""
    if isinstance(x, str) and isinstance(y, str) and x.startswith("stack") and y.startswith("stack"):
        if x == "stack" or y == "stack":
            return True
    return x == y


def events_equal(ea, eb) -> bool:
    return len(ea) == len(eb) and all(event_equal(a, b) for a, b in zip(ea, eb))


def compare(seed, orig, new, ret_kind):
    (co, so, fso), (cn, sn, fsn) = orig, new
    diffs = []
    if not events_equal(co.events, cn.events):
        for i, (a, b) in enumerate(zip(co.events, cn.events)):
            if not event_equal(a, b):
                diffs.append(f"call #{i}: original {fmt_ev(a, b)}\n               C       {fmt_ev(b, a)}")
                break
        else:
            diffs.append(f"call count: original {len(co.events)}, C {len(cn.events)}"
                         + (f" (next original: {fmt_ev(co.events[len(cn.events)])})" if len(co.events) > len(cn.events) else
                            f" (next C: {fmt_ev(cn.events[len(co.events)])})"))
    wo, wn = co.visible_writes(), cn.visible_writes()
    if wo != wn:
        keys = sorted(set(wo) | set(wn))
        bad = [k for k in keys if wo.get(k) != wn.get(k)]
        k = bad[0]
        diffs.append(f"memory: {len(bad)} byte(s) differ, first at 0x{k:08X}: "
                     f"original {wo.get(k, 'unwritten')} vs C {wn.get(k, 'unwritten')}")
    if ret_kind == "v0" and (co.g(2) & M32) != (cn.g(2) & M32):
        diffs.append(f"return v0: original 0x{co.g(2) & M32:08X}, C 0x{cn.g(2) & M32:08X}")
    if ret_kind == "v0_64" and co.g(2) != cn.g(2):
        diffs.append(f"return v0: original 0x{co.g(2):016X}, C 0x{cn.g(2):016X}")
    if ret_kind == "f0" and co.f[0] != cn.f[0]:
        diffs.append(f"return f0: original {b2f(co.f[0])!r}, C {b2f(cn.f[0])!r}")
    for r in CALLEE_SAVED:
        if cn.g(r) != sn[r]:
            diffs.append(f"C version clobbered callee-saved r{r}")
    return diffs


def fmt_ev(e, other=None):
    kind, tgt, args, fargs, dig = e
    if other is not None:  # show everything the comparison looked at
        args = dict(args, pending=args.get("pending", frozenset()) | other[2].get("pending", frozenset()))
        fargs = dict(fargs, pending=fargs.get("pending", frozenset()) | other[3].get("pending", frozenset()))
    pend, fpend = args.get("pending", ()), fargs.get("pending", ())
    a = ", ".join(f"a{r - 4}=" + (x if isinstance(x, str) else f"0x{x:X}")
                  for r, x in sorted((k, v) for k, v in args.items() if k in pend))
    fa = (" f:" + ", ".join(f"f{r}={b2f(x)!r}(0x{x:08X})" for r, x in sorted((k, v) for k, v in fargs.items() if k in fpend))) if fpend else ""
    return f"{kind} 0x{tgt:08X}({a}){fa} [writes {dig}]"


def option_parser() -> argparse.ArgumentParser:
    """Per-function options (also accepted after the function name in a --list file)."""
    ap = argparse.ArgumentParser(add_help=False)
    ap.add_argument("--runs", type=int, default=None, help="default: 20")
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--ret", choices=["auto", "none", "v0", "v0_64", "f0"], default="auto")
    ap.add_argument("-v", "--verbose", action="store_true")
    ap.add_argument("--pre", action="append", default=[],
                    help="input precondition: a0+0x18=0..8 (u32 at arg0+0x18), a1=0..3 (argument), @0x44E568=lo..hi (global), a0+0x10=a0+0x60 (a pointer into an argument, 3 runs in 4)")
    ap.add_argument("--max-steps", type=int, default=MAX_STEPS)
    ap.add_argument("--stub-ret", action="append", default=[], type=lambda x: int(x, 0),
                    help="a value stubbed calls return half the time (e.g. a 'done' status), repeatable")
    ap.add_argument("--stub-fret", action="append", default=[], type=float,
                    help="a float value stubbed calls return half the time, repeatable")
    ap.add_argument("--outparam", type=int, default=OUTPARAM,
                    help="bytes stubs write through stack pointer arguments (16: a whole vector)")
    ap.add_argument("--irq", action="append", default=[], type=lambda x: int(x, 0),
                    help="byte address an interrupt sets to 1 every 64 steps (a polled flag)")
    ap.add_argument("--stub-ret-prob", type=float, default=0.5,
                    help="how often stubs return a --stub-ret value (1.0: only those)")
    return ap


def test_function(rom: bytes, build, src: Path, func: str, opts) -> int:
    """0 = pass, 1 = fail, 2 = could not test."""
    overlays, funcs, c_range = build
    orig_range = original_range(func)
    if orig_range is None:
        print(f"ERROR {func}: not found in {BUILD_ELF}")
        return 2
    if func not in funcs:
        print(f"ERROR {func}: not defined (non-static) in {src}")
        return 2
    c_entry = funcs[func]
    CALL_ALIAS.clear()
    C_ENTRIES.clear()
    C_ENTRIES.update(funcs.values())
    for name, addr in funcs.items():
        o = original_range(name)
        if name != func and o is not None:
            CALL_ALIAS[addr] = o[0]
    ret_kind = return_kind(src, func) if opts.ret == "auto" else opts.ret
    runs = opts.runs if opts.runs is not None else DEFAULT_RUNS
    PRE_BYTES.clear()
    PRECONDITIONS[:] = [x for x in (parse_pre(p) for p in opts.pre) if x is not None]
    STUB_RETURNS[:] = opts.stub_ret
    STUB_RET_PROB[0] = opts.stub_ret_prob
    IRQ_FLAGS[:] = opts.irq
    OUTPARAM_BYTES[0] = opts.outparam
    STUB_FRETURNS[:] = opts.stub_fret
    harvest_constants(rom, *orig_range)
    ok = skipped = runaway_ok = 0
    covered: set[int] = set()
    for i in range(runs):
        seed = opts.seed * 100003 + i
        try:
            o = run_one(rom, overlays, orig_range[0], orig_range, seed, opts.max_steps)  # same world: C mapped in both
        except Trap:
            skipped += 1
            continue
        except Unsupported as e:
            print(f"ERROR {func}: original uses an unsupported instruction {e}")
            return 2
        try:
            n = run_one(rom, overlays, c_entry, c_range, seed, opts.max_steps)
        except Trap as t:
            print(f"FAIL {func} seed {seed}: C version trapped ({t})")
            return 1
        except Unsupported as e:
            print(f"ERROR {func}: C version uses an unsupported instruction {e}")
            return 2
        covered |= {a for a in o[0].visited if orig_range[0] <= a < orig_range[1]}
        if o[0].timed_out or n[0].timed_out:
            if not (o[0].timed_out and n[0].timed_out):
                who = "C version" if n[0].timed_out else "original"
                print(f"FAIL {func} seed {seed}: only the {who} ran away")
                return 1
            # both loop forever (e.g. a main loop with stubbed callees): compare the calls made
            k = min(len(o[0].events), len(n[0].events))
            if k < 3 or not events_equal(o[0].events[:k], n[0].events[:k]):
                if k < 3:
                    skipped += 1
                    continue
                i = next(i for i in range(k) if not event_equal(o[0].events[i], n[0].events[i]))
                print(f"FAIL {func} seed {seed}: endless loop, call #{i} differs:\n  original {fmt_ev(o[0].events[i])}\n  C        {fmt_ev(n[0].events[i])}")
                return 1
            runaway_ok += 1
            ok += 1
            continue
        diffs = compare(seed, o, n, ret_kind)
        if diffs:
            print(f"FAIL {func} seed {seed} (run {i}):")
            for d in diffs:
                print("  " + d)
            if opts.verbose:
                k = max(len(o[0].events), len(n[0].events))
                oe = o[0].events + [None] * (k - len(o[0].events))
                ne = n[0].events + [None] * (k - len(n[0].events))
                print("  original calls:", *[fmt_ev(a, b) if a else "-" for a, b in zip(oe, ne)], sep="\n    ")
                print("  C calls:", *[fmt_ev(b, a) if b else "-" for a, b in zip(oe, ne)], sep="\n    ")
            return 1
        ok += 1
        if opts.verbose and i < 3:
            print(f"seed {seed}: {o[0].steps} steps, {len(o[0].events)} calls, {len(o[0].visible_writes())} bytes written")
    addrs = list(range(orig_range[0], orig_range[1], 4))
    # trailing alignment padding (zero words never executed) doesn't count
    while addrs and addrs[-1] not in covered and struct.unpack_from("<I", rom, addrs[-1] - IMAGE_LO + 0x80)[0] == 0:
        addrs.pop()
    total = len(addrs)
    missed = sorted(set(addrs) - covered)
    print(f"PASS {func}: {ok} runs identical ({skipped} inputs skipped: original ran away or trapped), "
          f"return={ret_kind}, coverage {len(covered)}/{total} instructions"
          + (f" ({runaway_ok} endless-loop runs compared by their first calls)" if runaway_ok else ""))
    if opts.verbose and missed:
        print("  never executed:", " ".join(f"0x{a:08X}" for a in missed))
    return 0




_WORK = None  # (rom, build, src) of the file being tested, shared with forked workers


def _test_captured(item) -> tuple[int, str]:
    import contextlib
    import io

    func, opts = item
    rom, build, src = _WORK
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        r = test_function(rom, build, src, func, opts)
    return r, buf.getvalue()


def main() -> None:
    ap = argparse.ArgumentParser(parents=[option_parser()], description=__doc__.split("\n")[0])
    ap.add_argument("src", type=Path, nargs="?")
    ap.add_argument("funcs", nargs="*", help="functions to test (default: all non-static ones in src)")
    ap.add_argument("--list", type=Path, help="file of 'source function [options]' lines")
    ap.add_argument("-j", "--jobs", type=int, default=4, help="functions tested in parallel")
    args = ap.parse_args()

    # (source, function, options) jobs, grouped by source so each file is compiled once
    jobs: dict[Path, list[tuple[str | None, argparse.Namespace]]] = {}
    if args.list:
        sub = option_parser()
        for line in args.list.read_text().splitlines():
            parts = line.split()
            if not parts or parts[0].startswith("#"):
                continue
            opts = sub.parse_args(parts[2:])
            if args.runs is not None and opts.runs is None:
                opts.runs = args.runs
            opts.verbose |= args.verbose
            jobs.setdefault(Path(parts[0]), []).append((parts[1], opts))
    elif args.src:
        jobs[args.src] = [(f, args) for f in args.funcs] or [(None, args)]
    else:
        ap.error("give a source file or --list")

    rom = BASEROM.read_bytes()
    load_helpers()
    worst = passed = total = 0
    for src, items in jobs.items():
        with tempfile.TemporaryDirectory() as td:
            try:
                build = build_c(src.resolve(), Path(td))
            except subprocess.CalledProcessError:
                print(f"ERROR {src}: does not compile/link")
                worst = 2
                continue
        if items[0][0] is None:
            items = [(f, items[0][1]) for f in build[1]]
        global _WORK
        _WORK = (rom, build, src)
        if args.jobs > 1 and len(items) > 1:
            import multiprocessing as mp
            with mp.get_context("fork").Pool(min(args.jobs, len(items))) as pool:
                results = pool.imap(_test_captured, items)
                for r, out in results:
                    print(out, end="", flush=True)
                    total += 1
                    passed += r == 0
                    worst = max(worst, r)
        else:
            for item in items:
                r, out = _test_captured(item)
                print(out, end="", flush=True)
                total += 1
                passed += r == 0
                worst = max(worst, r)
    if total > 1:
        print(f"{passed}/{total} passed")
    sys.exit(worst)


if __name__ == "__main__":
    main()
