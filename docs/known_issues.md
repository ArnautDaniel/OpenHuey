# Known issues and gaps

What to keep in mind when using `src/` as the reference for the original game. Each entry says
whether it is a gap in the *decompilation* (src may describe the original wrongly or not at
all) or only in the *PC port* (src describes the original; the PC build draws or plays it
differently). Last updated 2026-10-06.

## Coverage

- All of the game's own code is C except 28 functions (about 1.2% of the game code), all VU1 / GS
  packet builders: the character model drawer chain (`func_001BDF80` and the senders it calls,
  `func_001BC370` .. `func_001BDDD0`, `func_002B72D0` .. `func_002B89C0`), the placed-object
  senders (`func_00267560` .. `func_00267D60`), the room-mesh layouts (`func_0025E9C0` ..
  `func_0025EF40`), the door shadow builder (`func_002784F0`), renderer layer 10 with an argument
  (`func_001B6CD0`), renderer +0x50 / +0x54 (`func_001BA970` / `func_001BAB80`, nothing calls
  them) and the room +0x340 object (`func_00268180`, its flag is never set). Their drawing is
  done in OpenGL (`src/game/model.c`, `src/game/room.c`, `native/platform/glr.c`); the packets
  themselves are only in `asm/`.
- About 1,250 data labels are still anonymous (`D_xxxxxxxx`): mostly script blobs, tables and
  float constants used by one function. Every vtable is named (`Foo_vtable`, a base's
  `FooBase_vtable`, a second base's `Foo_vtableN` by its offset); a few classes whose role isn't
  known keep their address (`Obj46A9B0_vtable`).
- The event script opcodes are fully described: `docs/event_opcodes.md`, listings with
  `tools/evdis.py ROOM`. Every room command and condition (`RoomXX_CmdNN` / `RoomXX_CondNN`) has a
  description in its file.

## Approximated on PC (not decompiled behaviour)

Marked "PC: an approximation, not decompiled" in their comments.

- **The TVs in rooms 0x31 / 0x32** (`Movie_PageTex0`). The original copies the screen drawn so far
  at half size into a texture page, reading the 32-bit frame buffer as 8-bit indices through
  palette 3 or 4 (a tinted live feed). The PC shows the screen so far in grey
  (`glr_screen_copy`). The palettes and the exact byte the PSMT8 read picks are not worked out.
- **The mirror fragment in room 0x32** (`MirrorFragment_Draw`). The camera mirroring and the
  character redrawn into reflection layer 0x17 follow the original; its GS composite of the
  reflection (strips through page 0x3400 at alpha +0x18) is replaced by glr's reflection pass,
  masked to an 8 x 8 square at the fragment (the original's extent is not known).
- **Layer 0x11** (`func_001B2160`, the tint stalker's / Riccardo's tint). The original draws the
  layer at half size against a halved Z, adds 15 animated flare sprites (renderer +0x304C0C),
  turns their green into alpha on the drawn pixels through palette 6 and lays it over the screen
  tinted by +0x304D4C with +-1 pixel blur copies. The PC treats it as a fading layer by
  +0x304D4C's alpha, without the flares.

## Not ported

- **The lit model microprograms' specular term.** Model parts whose mode is 4, 8 or 0xC use the
  lit programs (e.g. `D_003A2EF0`), which also put a specular term into the vertex alpha (see
  the comment above `light_setup` in `src/game/model.c`). The PC lights them with the plain
  formula and alpha 127, so passes that read frame alpha (the bloom mask) see less on them.
- **The lit room-mesh layout** (mode 1 of room and positioned meshes): no room uses it, and the
  resident room microprogram (`D_003A7530`) would pass its colour words straight to RGBAQ. The
  PC draws room meshes in that layout grey and positioned ones not at all (`glr_todo`).

## Open bugs (from play)

Not yet classified as decompilation errors or PC-only problems; check them before relying on
the code involved.

- **Pushable boxes** (`src/game/room_map.c` obstacles, Fiona's push states in `fiona.c`): the
  push starts (`Fiona_StatePush`) but never steps the box, and in room 0x20 Fiona appears to walk
  through the box. Notes: the ring triangles around a box carry `NAV_PUSHABLE` as expected; the
  box's own reference squares had no blocking bit.
- **The drawer**: after checking a drawer Fiona stays frozen in place (the game keeps running).
- **The plate machine**: one of its animations is missing.
- **The first cutscene** (EV0002, room 0x2A): reported once playing its sound with the picture
  stuck on one frame. Not reproduced since.

## Verification

- `tools/difftest.py` compares each C function against the original asm on random inputs. At the
  last full run 7,211 of 7,226 tests passed; the failures were test-input problems since fixed,
  except `RoomMgr_MakeCurrent` (1 run in 40: a section pointer landing on the manager's own
  table, which real room files can't do).
- **486 test lines pin `0x46BF20` (`SndDriver_vtable`) as a generic vtable** for objects
  whose class doesn't matter to the test, so their virtual calls land on sound-driver methods
  and difftest compares the wrong callee's argument registers: a dropped argument to a virtual
  callee can pass there. Fix a line with the object's real vtable when touching it. (`gSound`
  points 4 bytes into the sound driver; its vtable is `SndDriver_vtable4`, and the lines that
  pin gSound use it.)
- PC-only code (`#ifdef HG_NATIVE` paths and `native/platform/`) is not difftested.
- Behaviour-preserving cleanups were checked with `tools/codesnap.py` (per-function compiled
  code, inlining off) against the commit before the cleanup.

## Looks wrong but is faithful

- Several event commands only call empty methods of the progress object and do nothing in this
  game (0x01, 0x3A, 0x3C, 0x44, 0x6B, 0x6C, 0x8A, part of 0x99): leftovers of the shared engine.
- `cmd_exit` (event command 0x00) takes its result from the progress' +0x10, which is empty and
  returns 0.
- Comparisons of a low byte against 0xFFFF in `Character_ChooseExit` (`actor.c`) are always
  true in the original too.
