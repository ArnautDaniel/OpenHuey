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
| +0x40 | | 0x46A260 -> 0x47A7E8 | gProgress (0x44E4D8) | second base: Progress (flags at +0x8, byte vars at +0x9C, bit set at +0x100, room flags at +0x124; extends to at least +0x6FC214) |
| +0xC88840 | 0x1AD740 | 0x469C20 -> 0x469C60 -> 0x46AAB0 | `gCharPlayer` (character slot 0) | Fiona: loads O_FIN\FIN_D000.MTN; 37 virtuals |
| +0xE35F80 | 0xF37C0 | 0x469C20 -> 0x469C60 -> 0x46A120 | `gCharPartner` (slot 1) | Hewie |
| +0xF29740 | | 0x46ABB0 | `gSceneGameF29740` (0x44E588) | ? (15 virtuals, 0x1A4970..) |
| +0xF6A940 | | 0x46C520 | | |
| +0xF6AFB0 | | 0x46B3A0 (+0xC: 0x46B3B8) | 0x44E4C8 | has a 3231-instruction method (0x1FC760) |
| +0xF6C1C0 | | 0x46B300 | | |
| +0xF87240 | | 0x47A790 | | shared with SceneTitle/Scene5 |
| +0x1053424 | | 0x46A110 | 0x44E970 | |
| +0x1053480 | | 0x473440 | | |
| (sub-heaps) | | 0x46A1C0 | 0x44E578 | heap class, same as Game.sceneHeap |

### SceneGame per-frame flow

`SceneGame_StateEntry` -> state `D_0044C7B0` = `func_003A06E0` every frame: frame counter at
+0x1065040, then the sub-state PTMF at +0x1053450 if set (`D_0044C7A0` = `func_003A04A0`: room
load, returns 1 while busy), else the gameplay tick `func_003A0160`:

1. `func_00225550(+0xF6CBB0)`, `func_001792C0(Progress, 0)`, `func_0039D310(game)` (416 insns:
   events, items, camera? - calls Progress flags, `func_00124F20(gCharPlayer, ...)`)
2. **characters**: vtable +0x38 (per-frame update) on `gCharacters[0..2]` whose byte +0x28 == 1
3. `func_002E2650(+0x706480)`, game vtable +0xDC, `func_00175430(Progress)` (620 insns),
   `func_002252B0(+0xF6CBB0, ...)` (camera/collision?)

## Characters

`Characters_Register(prog, slot, chr)` (0x16D5E0) fills `gCharacters[6]` (0x44F800) and sets
`chr+0x20 = slot`. Shortcuts: `gCharPlayer` (0x44F818) = slot 0 Fiona (2934 references),
`gCharPartner` (0x44F820) = slot 1 Hewie, `gCharPursuer` (0x44F828) = slot 2.

Vtables (Metrowerks, +0x8 dtor): base `0x469C20` -> `0x469C60` (14 entries, code ~0x120F80..0x127660)
-> Fiona `0x46AAB0` / Hewie `0x46A120` (37 entries each).

| vtbl | Base 0x469C60 | Fiona (0x19A300..0x1A4340) | Hewie (0x130A70..0x168A10) |
|---|---|---|---|
| +0x08 | dtor 0x124E60 | 0x17FCD0 | 0x130A70 |
| +0x28 | 0x125AD0 | 0x1A3A80 (259) | 0x1683D0 |
| +0x30 | 0x123F50 (empty) | 0x1A3110 (543) | 0x167BC0 (528) |
| +0x34 | 0x123D10 (empty) | 0x19B4F0 (639) | 0x166DF0 (567) |
| +0x38 | 0x120F80 (empty) - **per-frame update** | 0x19AF20 (403) | 0x166CE0 (72) |
| +0x84 | - | 0x1A0370 (1059) | 0x163DC0 (748) |
| +0x88 | - | 0x19F8A0 (736) | 0x1635B0 (572) |

### Actor / Character base classes (`src/game/actor.c`, `include/actor.h`)

- **Actor** (vtable 0x469C20, 0xE0 bytes): position/previous position, heading + rotation
  matrix, room, nav-mesh triangle, collision cylinder (radius +0xC8, height +0xCC), flags.
  Placement (+0x28), reset (+0xC), mesh queries and moves (walk, slide, push-out, contact).
- **Character** (vtable 0x469C60): Actor + animation player (+0xF0, root motion), path
  planning through `gSceneGameF29740` (request at +0x1330, waypoints at +0x12C), room exit
  choice (+0x14D4), hearing of noise events (gProgress +0x778, 4 x 0x10, one per slot;
  threshold +0x152A, result +0x14D5..), water footstep effects, state blocks (+0x14E8, +0x1508).
- **Nav mesh** (`include/navmesh.h`, `D_0044E570`): 0x50-byte triangles (corners, neighbours,
  flags), doors (count +0x14) with two sides; vtable +0x20/+0x24 segment exit test, +0x40 slide.
- Other managers used: `D_0044E568` rooms (exits 0..7 per room), `D_0044E558` doors,
  `D_0044E4D0` room objects, `D_0044E550` random numbers, `D_0044E578` effects
  (0x400 slots), `D_0044E4F0` GS manager (platform), `D_0044E560` sound.

## Globals

| Address | Name | What |
|---|---|---|
| 0x44E4E0 | gFileLoader | file loader: +0x34 Load(name, dest), +0xC LoadAsync(name, dest, flags, 0) |
| 0x44E998 | gBootMessage | SceneBoot's message display |
| 0x44E588 | gSceneGameF29740 | SceneGame +0xF29740 (was misnamed gFiona) |
| 0x44F800 | gCharacters | character slots; `gCharPlayer`/`gCharPartner`/`gCharPursuer` at 0x44F818/820/828 |
| 0x44E4D8 | gProgress | game progress (SceneGame+0x40): story flags, byte vars - `src/game/progress.c` |
| 0x44E568 | (room manager?) | vtable +0x10 returns the current room number |
| 0x47E374 | (pad) | buttons held: bit 0 Select, bit 3 Start |
| 0x47E37C | (pad) | buttons pressed? (bit 14 used by boot steps) |

## Platform boundary (to be replaced on PC)

Library code is mapped in `config/libraries.txt`. Game code reaches the PS2 through: the file
loader (CVFS/ADXF), CRI ADX (audio) and Sofdec (movies), libmc (saves, in SceneBoot's steps),
libpad/libdbc (controller), libgraph/libdma/VU microcode (rendering), libkernl/sif (threads, IOP).
