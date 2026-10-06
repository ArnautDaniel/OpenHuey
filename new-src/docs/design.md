# new-src design

## The split

| C (`src/`) | Forth (`scripts/`) |
|---|---|
| the platform: window, GL context, input, timing | what keys do |
| file formats: rooms, textures, models, motions | which room, which motion, which view |
| rendering, skinning, posing | where things go, how fast, when |
| geometry queries (`floor-below`) | the rules that use them |
| the data: every struct is defined in C | reads and writes those structs through field words |
| the scheduler for Forth tasks | the tasks |

A rule of thumb: if it is a loop over thousands of things, or talks to the hardware, or parses a
file format, it is C. If it is a decision, a rule, a sequence of events or a number someone will
want to tune, it is Forth.

C never calls into game logic by name. It runs the Forth hooks (`on-tick`) and tasks each tick and
draws whatever state the scripts left behind.

## A frame

```
main.c   poll input -> console (if open) -> 0..4 ticks at 60 Hz -> draw
tick     Forth tasks (forth_run_tasks), then on-tick hooks, then actors' motions advance
draw     room mesh, actors (skinned on the CPU), console / 2D
```

The game logic runs at a fixed 60 ticks a second whatever the display rate, so scripts can count
ticks (`wait`, `dt`).

## Modules

- `core/`: `files` (the data folder: `ST_000/ST_003.PAC`; DOS paths work), `mathx.h` (Vec3, Mat4
  column-major).
- `platform/`: SDL3 window + GL 4.6 core context (`platform.c`), the GL function loader (`gl.c`).
- `render/`: one mesh shader (the PS2's colour model: texel times vertex colour, 0x80 = 1.0), 2D
  text and rectangles, PNG screenshots. Draws are described by `MeshDraw` (texture, blending,
  depth writes, visibility group).
- `data/`: the formats, parsed into plain arrays with every offset checked:
  - `pac` - a room file's 17 sections;
  - `tex` - texture banks (indexed 4/8-bit with CLUTs, direct colour);
  - `roommesh` - section 3, batches of triangle strips in batch-local space, flattened into room
    space triangle lists at load;
  - `navmesh` - section 0, where characters may stand (moves slide along its edges);
  - `exe` - the game's executable, for the tables that live only there (read from the player's
    copy at start-up, never stored in the repository);
  - `model` - character .PCK files (skeleton, skinned/rigid/morph parts, motion banks) and the
    pose (motion sampling, bone matrices).
- `game/`: `world` (how rooms connect: the door and room tables), `room` (a loaded room: file,
  GPU mesh, textures, groups, floor query, nav mesh), `actor`
  (characters in the world), `camera`, `console`, `engine.h` (all the engine state in one struct).
- `forth/`: `forth.c` (the language), `bind_engine.c` (the engine's words), `repl.c` (the
  standalone tool).

## Conventions

- Readable first: plain structs, functions that do one thing, comments that say why. No macros
  where a function would do. Optimise when something is measured to be slow.
- The decomp in `../src` is the reference for formats and behaviour. Cite it in comments by
  function name (`src/game/room.c RoomMgr_MakeCurrent`), never copy its structure: no `AT(p, 0x1C)`,
  no vtables, no PS2 machinery.
- Units: room space as the game has it (about 10 cm to the unit, y up). Characters are drawn at
  scale 1. Angles in radians. Motions run at 30 frames a second (`rate` 0.5 a tick).
- Headless runs for tests and screenshots: `hg2 --hidden --frames N --eval "..."`. The
  `screenshot` word saves a PNG, and `key-hold` stands in for a player.

## Known gaps (the first pass)

- Room meshes: the animated (flip-book) parts and placed/billboard batches are not drawn
  specially; the bloom mask, fog and the game's lights are not done.
- Characters: no root motion, no blending between motions, the faces and hands in their rest
  shape; lighting is one fixed light.
- Collision is the nav mesh only (no other characters). Exits all lead through (no locked
  doors, no door animations, no transition effects); no events, items, sound.
- The camera "director" is a stand-in (the nearest room setup); the game's real camera zones
  are in the room data and the decomp (`CamDirector_*`).
