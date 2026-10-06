# Progress / where we left off

Last updated 2026-10-02.

## Native PC build (the "slice to the menu")

`cmake -S native -B build/native/cmake && cmake --build build/native/cmake -j4`, then
`build/native/cmake/hg [extracted DATA.CVM folder]` from any directory (default: the absolute
path of `../Haunting Ground (USA)/data` next to the repository, fixed at configure time; or
`$HG_DATA`). Without the data it exits with a message.
It boots through the controller check, the memory card check, the Dolby logo (the Capcom movie
ends at once: no Sofdec yet), the caution screen, the title (fade in, PRESS START) and the main
menu (New Game / Load Game / Options with their descriptions), at 60 fps with the software GS.
Options opens, navigates and closes; Load Game lists the saves on the card (place, date, play
time) and loads one, after which the title hands over to the game scene.
Screenshots: `docs/img/native_*.png`.

- Keys: arrows (D-pad), X / Space = cross, C / Backspace = circle, Z = square, V = triangle,
  Q / E = L1 / R1, 1 / 3 = L2 / R2, Return = Start, Tab = Select, WASD = left stick; SDL gamepads.
- Environment: `HG_HEADLESS=1` (no window), `HG_DUMP=dir` (every 30th frame as PPM),
  `HG_MAXFRAMES=n` (exit after n frames), `HG_AUTOCROSS=1` (press cross every 2 s),
  `HG_INPUT="frame:button,..."` (scripted presses held for 6 video frames, e.g.
  `1700:start,1900:down,1960:down,2020:cross` opens Options; with a save on the card the menu
  starts on Load Game), `HG_SAVE=dir` (the memory card in slot 1, default `./save`),
  `HG_GSDEBUG=1`.
- Saves: the file `BASLUS-21075HG/BASLUS-21075HG` (0x12D70 bytes: system data, 12 save headers,
  12 saves) from a PCSX2 folder memory card can be copied into `$HG_SAVE` as is.
- Silent: ADX / SPU sound and music are shims (the game-side music logic runs). Movies: Sofdec is
  a shim whose player creation fails, which the game treats as "no movie".
- Where it stops now: the game scene's constructor (`SceneGame_ctor`, 0x2D05A0), reached by New
  Game or by loading a save. The Options screen is complete (list, the five editors, restore
  defaults).
- To check against the real game: the dark boxes behind the menu entries (they come from the
  entries' CLUT 1 background colour); the Dolby logo sits left of centre.
- Untranscribed but known: the base-class destructor of the block pool (`D_004699E0` +8 points
  at 0x00120EF0, inside `BlockPool_Init`'s label: needs a symbol before it can be called natively);
  the movie states `func_002B6710`, `func_002B68B0`, `func_002B69B0`.
- New this round (all difftested): the message / dialog system (`task.c`), the boot memory card
  check (`bootcard.c`), the memory card manager (`memcard.c`), movies (`movie.c`), music
  (`bgm.c`), the camera's small methods (`camera.c`), the title scene (`scene_title.c`), the sub
  screen setup (`subscreen.c`), renderer image uploads / object drawing / layer setup, VRAM
  TEX0 / TEX2 builders, the block pool (`heap.c`). Math library natives identified with
  `tools/mathprobe.py`.

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
- The big ones, last: `Hewie_SetAction` (5759 instructions: start action N, 129 callers; split by
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
