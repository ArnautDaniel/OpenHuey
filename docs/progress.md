# Progress / where we left off

Last updated 2026-10-02.

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
| Hewie (partner AI) | - | **next**: vtable 0x46A120, code 0x130A70..0x168A10 (~266 functions, ~58k instructions) |
| Stalkers | - | not started |
| Model / motion code | - | format fully understood (`docs/model_format.md`), C not written yet |

### Picking up again

1. Hewie's class: start from his vtable 0x46A120 (37 entries, listed in `docs/structure.md`),
   same method as Fiona: m2c draft (`tools/decomp.py`), read the asm, write C in
   `src/game/hewie.c`, add `tools/difftest_list.txt` entries, run the suite, commit.
2. The model/motion code found while decoding the formats is a good, self-contained follow-up
   (the PC port needs it): skeleton init `func_001F7C40`, motion lookup `func_001F4B80`,
   track setup `func_001F4C10`, sampler `func_001F36B0`, pose `func_001F5930`, Euler matrix
   `func_002E2E00`, mesh draw `func_001BDF80` -> `func_001BDDD0` / `func_001BD830` (platform: VU1
   packets via `func_002B89C0`, to be replaced by a PC renderer).

### Work-style notes

- Single agent; difftest at `-j 4`, 20 runs default; no PCSX2 boot tests unless asked.
- Tester gotchas fixed so far are in the git log (`difftest:` commits).

## Model extraction (side project)

Done (2026-10-02): all 115 character/prop models convert (`tools/hg_export_all.py`) with
skeleton, skin, faces/hands (morph targets), eyes, textures and motions;
`tools/viewer/hgview.c` (raylib) browses, poses and plays them. Format and usage:
`docs/model_format.md` (includes the open gaps: root motion, playback rate, names, .MRK,
shadow volumes, O_T00).
