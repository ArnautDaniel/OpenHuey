# Haunting Ground (USA) decompilation

A **functional, non-matching** decompilation of *Haunting Ground* (PS2, Capcom, 2005),
`SLUS_210.75` v1.01.

The goal is C/C++ source that builds into a working ELF. It does not need to be
byte-identical, so the code is built with modern GCC (the ps2dev toolchain). The original
was built with Metrowerks CodeWarrior (`MW MIPS C Compiler 2.4.1.01`).

## Setup

1. Put the game executable at `baserom/SLUS_210.75`
   (sha1 `e0867d2ec83e6e2fccf147ad9e645a9a662d1c15`).
2. `tools/setup.sh`: creates `.venv` and downloads the ps2dev toolchain into `tools/ps2dev`.
3. `.venv/bin/python configure.py --split`: disassembles the ELF and writes `build.ninja`.
4. `ninja`: builds `build/SLUS_210.75.elf` and checks it against the baserom.

## Binary layout

| Range (vram)              | Contents |
|---------------------------|----------|
| `0x00100000`–`0x00100230` | SDK `crt0` |
| `0x00100230`–`0x003A1990` | Code: game (C++), Sony SDK 3.0.2 libs (GCC 2.96), CRI ADX/Sofdec middleware |
| `0x003A1990`–`0x003A1B80` | C++ `this`-adjusting thunks |
| `0x003A1B80`–`0x0047B200` | data / rodata (incl. embedded `cdvdman` IRX at `0x0044A9A0`) |
| `0x0047B200`–`0x01992000` | BSS (`_gp` = `0x004828F0`) |

## Status

- [x] Full-binary split with splat; assemble and link round-trip is byte-identical
- [x] **Shiftable build**: with padding inserted after crt0 (4 bytes, 16 bytes, or 4 KB),
      the game boots, shows the memory-card check and the Capcom logo with correct colours,
      and plays the intro movie
- [x] Library code mapped (`config/libraries.txt`): ~4,350 functions / 616 KB are Sony SDK,
      CRI ADX/Sofdec, MSL runtime and libm; ~6,950 functions / 2 MB are Capcom game code.
      237 library functions named (`tools/name_libs.py`: syscall stubs + API names from error strings)
- [ ] Split `asm/game.s` into per-file / per-function units
- [ ] Start decompiling game code

## Shiftability pipeline

`configure.py --split` runs splat, then fixes up its output:

| Step | What it fixes |
|---|---|
| `config/ignore_addrs.txt` | large struct offsets that look like addresses inside .text |
| `tools/ptrpatch.py` + `config/pointers*.txt` | data words that are pointers but stayed raw, or numbers that were wrongly symbolized (`raw`) |
| `tools/offpatch.py` | `lui/addu base/%lo` struct-field offsets turned back into numbers |
| `tools/align_data.py` | preserves 64-byte data alignment and 16-byte function alignment |

Helpers to keep these lists current (run after a build; then `configure.py --split` again):

- `tools/find_pointers.py`: regenerates `config/pointers_auto.txt` (pointer-to-member
  records, pointer tables, jumptable tails, pointers decoded as strings, false BSS/colour pointers)
- `tools/find_vfuncs.py`: functions reached only through vtables
- `tools/promote_undefined.py`: unlabeled references that need symbols
- `tools/libmap.py`: compiler fingerprint + strings per address range (library identification)
- `tools/name_libs.py`: names syscall stubs and library functions from their error strings
- `tools/decomp.py <func>`: first-draft C via m2c
- `tools/ptrcheck.py`: static audit (unrelocated data pointers / `lui`, pinned addresses)

Testing:

- `ninja shift` links `build/SLUS_210.75.shift-<point>.elf` with padding at several points
  (`configure.py --shift 0x4` to change the amount)
- `tools/boottest.py <elf>` boots it in PCSX2 (private data dir under `build/pcsx2`) and
  checks the frame at 20 s; `tools/ramdiff.py` compares EE RAM from two save states
- `tools/bisect_shift.py` lists clean split points for bisecting a broken data range

## Notes

- Register names use the **n32** convention (`$a4`–`$a7` = o32 `$t0`–`$t3`) because the
  ps2dev toolchain is n32. Don't hand-convert asm to o32 names.
