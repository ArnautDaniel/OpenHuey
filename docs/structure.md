# Game structure (what's known so far)

Addresses are vrams in SLUS_210.75. "vtbl" = Metrowerks layout: +0x0/+0x4 zero (no RTTI),
+0x8 destructor(this, flags), then virtual functions. Names in `config/symbol_addrs.txt`.

## Top level

`main` -> `Game_Run(&gGame)`: `gGame` (0x00487A00, ~21 MB in BSS) runs a state machine of
pointer-to-member functions (`include/ptmf.h`): `Game_StateMain` every frame, `Game_StateShutdown`
at the end. See `src/game/game.c`, `include/game.h`.

`Game` members (from `Game_dtor`):

| Offset | What |
|---|---|
| +0x69AC0 | sub-object with vtable 0x46ADF0, ticked every frame (vtable +0x10); contains members up to +0x3FF800 |
| +0x4009CC | current state (PTMF) |
| +0x400A00 | scene table object; `scenes[4]` at +0x400A04 |
| +0x14D9A40 | scene heap (vtable 0x46A1C0: +0x10 alloc, +0x14 free) |
| +0x14D9DD0, +0x14DC530 | object pools (462x0x14 + 64x0xC; 632x0x50 + 32x0xC) |
| +0x14E8C90 | sub-object, init `func_001F44D0`, shutdown `func_001F4100` |

## Scenes (game modes) - `Game_StartNextScene`

| Mode | Class | Size | |
|---|---|---|---|
| 1 | SceneBoot | 0xC7700 | memory card check, logos (verified at runtime) |
| 2 | SceneTitle | 0x140D00 | opening movie, title screen, menus (verified) |
| 3 | SceneGame | 0x1065080 | gameplay |
| 5 | Scene5 | 0x117540 | unknown |

Scene base (`include/game.h`, `src/game/scene.c`): vtable 0x4699A0 - +0xC `Scene_Update`
(request/status machine, then runs `state`), +0x10 entry state (pure virtual),
+0x14 `Scene_OnSoftReset`.

## SceneGame (mode 3)

Vtable 0x47A7D0: +0x8 dtor `func_00399CA0`, +0xC update `func_003A1990` (frame counter at
+0x73EE40, then `Scene_Update`), +0x10 entry state `func_003A07D0`, +0x14 soft reset
`func_0039FD30`. Second base at +0x40 (vtable 0x47A7E8; the `this-0x40` thunks at 0x3A19B0..).

Members (from `SceneGame_ctor`):

| Offset | Size | Vtable chain | Global | Notes |
|---|---|---|---|---|
| +0x40 | | 0x46A260 -> 0x47A7E8 | | second base |
| +0xC88840 | 0x1AD740 | 0x469C20 -> 0x469C60 -> 0x46AAB0 | `gFiona` (0x44E588) | Fiona: loads O_FIN\FIN_D000.MTN; 37 virtuals |
| +0xE35F80 | 0xF37C0 | 0x469C20 -> 0x469C60 -> 0x46A120 | 0x44E580 | second character, same base as Fiona (Hewie?) |
| +0xF29740 | | 0x46ABB0 | | ? (15 virtuals, 0x1A4970..) |
| +0xF6A940 | | 0x46C520 | | |
| +0xF6AFB0 | | 0x46B3A0 (+0xC: 0x46B3B8) | 0x44E4C8 | has a 3231-instruction method (0x1FC760) |
| +0xF6C1C0 | | 0x46B300 | | |
| +0xF87240 | | 0x47A790 | | shared with SceneTitle/Scene5 |
| +0x1053424 | | 0x46A110 | 0x44E970 | |
| +0x1053480 | | 0x473440 | | |
| (sub-heaps) | | 0x46A1C0 | 0x44E578 | heap class, same as Game.sceneHeap |

## Globals

| Address | Name | What |
|---|---|---|
| 0x44E4E0 | gFileLoader | file loader: +0x34 Load(name, dest), +0xC LoadAsync(name, dest, flags, 0) |
| 0x44E998 | gBootMessage | SceneBoot's message display |
| 0x44E588 | gFiona | Fiona (set by SceneGame_ctor) |
| 0x47E374 | (pad) | buttons held: bit 0 Select, bit 3 Start |
| 0x47E37C | (pad) | buttons pressed? (bit 14 used by boot steps) |

## Platform boundary (to be replaced on PC)

Library code is mapped in `config/libraries.txt`. Game code reaches the PS2 through: the file
loader (CVFS/ADXF), CRI ADX (audio) and Sofdec (movies), libmc (saves, in SceneBoot's steps),
libpad/libdbc (controller), libgraph/libdma/VU microcode (rendering), libkernl/sif (threads, IOP).
