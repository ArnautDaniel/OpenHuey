# Progress / where we left off

Last updated 2026-10-01.

## Decompilation

- ~1,500 functions in C (1,252 leaf functions in `src/leaf/`, plus `src/game/*.c`). Build still
  matches byte-for-byte; the differential tester suite (`tools/difftest_all.sh`, 1,475 entries)
  passes.
- Working subsystem by subsystem (game logic first; PS2 platform code is only identified and
  shimmed, see `docs/structure.md`).

| Subsystem | File | State |
|---|---|---|
| SceneGame frame, managers | `docs/structure.md` | mapped |
| Actor / Character base classes | `src/game/actor.c`, `include/actor.h` | done (~108 functions, 0x120F80..0x127660) |
| Fiona (player) | `src/game/fiona.c`, `include/fiona.h` | **done**: all 46 functions of her class |
| Hewie (partner AI) | `src/game/hewie.c`, `include/hewie.h` | **paused, ~60% of functions**: 154 of 255 done (all difftest-verified); 101 left, ~35k instructions |
| Stalkers | - | not started |
| Model / motion code | - | format fully understood (`docs/model_format.md`), C not written yet |

### Picking up again

**Current focus (2026-10-01): native PC slice to the title menu** (see the plan in the git log /
next session). Hewie is paused, not abandoned.

**Hewie, where it stopped** (last commit `116875d`):

- Method: bottom-up by size. List what's left with the snippet in the git log of `116875d`, or:
  every `glabel` in `asm/game.s` within 0x130A70..0x168A10 not defined in `src/game/hewie.c`.
- Next small ones: `func_00139460` (147), `func_0013F220` (147, path distance to a character,
  already called as `f32 func_0013F220(Hewie *, Character *)`), `func_0013A1C0`,
  `func_0013EFB0` (fills a target point, -1 none), `func_0013FDE0`, `func_0014C210`, ...
- The big ones, last: `func_00130AF0` (5759 instructions: start action N, 129 callers; split by
  action ranges to test), `func_00141C00` (1404, move kind N), `func_00140CD0` (969, target
  check), vtable `func_00167BC0` (+0x30 main update), `func_00166DF0` (+0x34 door),
  `func_001635B0` (+0x88), `func_00163DC0` (+0x84).
- After all functions: the cleanup pass (real struct fields for the `HW(h, 0xF3xxx, T)`
  accesses, enums for actions / modes / behaviours, names). Agreed: no restructure before
  every function is in.

Gotchas found on the way (all in the code/tester now):
- This GCC rounds decimal float literals toward zero: write inexact constants in hex
  (`0x1.99999ap-5f /* 0.05 */`).
- RNG rolls (`RNG01()`) mapped through a switch can leave a register uninitialised in the
  original on out-of-range rolls: test with `--stub-ret-prob=1` and in-range `--stub-fret`s.
- When a function lands in `src/game/*.c`, delete its copy in `src/leaf/*.c` (and the leaf
  list entry) or the decomp ELF fails to link; re-run `configure.py` before `ninja`.
- Inline helpers (`Hewie_CharHere`, `Hewie_FreeForCommand`, `Hewie_Feeling`) instead of
  calling the real function where the original inlined it (a call would show up in the trace).

Also open: the model/motion code found while decoding the formats (skeleton init
`func_001F7C40`, motion lookup `func_001F4B80`, track setup `func_001F4C10`, sampler
`func_001F36B0`, pose `func_001F5930`, Euler matrix `func_002E2E00`, mesh draw `func_001BDF80`
-> `func_001BDDD0` / `func_001BD830`, VU1 packets via `func_002B89C0`).

### Work-style notes

- Single agent; difftest at `-j 4`, 20 runs default; no PCSX2 boot tests unless asked.
- Tester gotchas fixed so far are in the git log (`difftest:` commits).

## Model extraction (side project)

Done (2026-10-02): all 115 character/prop models convert (`tools/hg_export_all.py`) with
skeleton, skin, faces/hands (morph targets), eyes, textures and motions;
`tools/viewer/hgview.c` (raylib) browses, poses and plays them. Format and usage:
`docs/model_format.md` (includes the open gaps: root motion, playback rate, names, .MRK,
shadow volumes, O_T00).
