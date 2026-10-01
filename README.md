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
- [ ] Shiftable build (all pointers symbolized, so functions can change size)
- [ ] Identify SDK / CRI library functions by signature
- [ ] Split `asm/game.s` into per-file / per-function units
- [ ] Start decompiling game code

## Notes

- Register names use the **n32** convention (`$a4`–`$a7` = o32 `$t0`–`$t3`) because the
  ps2dev toolchain is n32. Don't hand-convert asm to o32 names.
