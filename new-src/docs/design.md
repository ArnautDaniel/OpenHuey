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
draw     room mesh, actors (skinned on the CPU, contact shadows) into the HDR scene target
         -> post (render/post.c) onto the window -> on-draw hooks, hint, console (2D)
```

## The picture

- The scene is drawn in linear colour into an RGBA16F target, multisampled (MSAA, alpha to
  coverage for cut-out textures). The game's colours are display values: textures are sRGB,
  vertex colours raised to 2.2 - with every effect off the result matches the original.
- After the scene (`post.c`): ambient occlusion from the depth buffer, bloom (a 6-level chain),
  distance fog, exposure, tone mapping (clip, a soft shoulder that keeps the original look,
  or ACES), saturation, contrast, vignette, grain, then display encoding.
- Widescreen: the camera's field of view is vertical, so a wider window shows more at the sides
  (the game's own fovs span the width of a 4:3 picture and are converted). `gfx.aspect` 1 gives
  the original 4:3 picture with bars.
- The room's own look (PAC section 13, render.h `RoomLook`): its fog ramp, its two-colour tint
  (a blurred copy of the screen added, then contrast against it) and its bloom over the areas the
  mesh's bloom-mask part marks (a second colour target) - done on display values as the PS2 did,
  before the modern passes.
- Moving geometry (`roommesh.c` `DynBatch`): parallax layers, billboards and flip books are
  rebuilt each frame; the room draws solid and see-through parts, moving batches, glows, then the
  bloom mask.
- Characters are lit like the game lights them: the room's ambient plus the three brightest of
  its lights reaching their nav triangle (PAC section 4, `room_lights_at`), per pixel, with a rim.
  The strongest casts a shadow map (one 1024 layer of a depth array per character, 3 x 3 PCF)
  onto the room and the characters (each other and themselves, the lookup moved out along the
  normal against self-shadowing acne), fading a few sizes away.
- The room's depth of field (section 13's fourth entry: blurred nearer than a, sharp b..c,
  blurred again by d) mixes a blurred half-size copy of the scene in by view depth.
- The room's look changes at run time: the bloom's pulsing colour modes tick in C, and scripts
  reach every value (`room-look look.*`, look.fs). Shadow passes run before the scene each frame.
- Every setting is a field of `RenderSettings` (render.h), reached from Forth as `gfx gfx.*`;
  `graphics.fs` has the presets, the F1 menu, and saving (the settings written out as a Forth
  script in the player's folder, read back at start-up).

The game logic runs at a fixed 60 ticks a second whatever the display rate, so scripts can count
ticks (`wait`, `dt`).

## Modules

- `core/`: `files` (the data folder: `ST_000/ST_003.PAC`; DOS paths work), `mathx.h` (Vec3, Mat4
  column-major).
- `platform/`: SDL3 window + GL 4.6 core context (`platform.c`), the GL function loader (`gl.c`).
- `render/`: OpenGL 4.5+ style throughout - objects made and changed by name (direct state
  access: `glCreate*`, `glNamedBuffer*`, `glTexture*`, `glVertexArray*`, `glProgramUniform*`),
  immutable storage for what never changes, fixed uniform locations and sampler bindings in the
  GLSL (`#version 460 core`). One mesh shader (the PS2's colour model: texel times vertex colour,
  0x80 = 1.0), 2D text and rectangles, PNG screenshots. Draws are described by `MeshDraw` (texture, blending,
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

- Rooms: no event scripts yet, so nothing changes a room's look on its own beyond the pulsing
  bloom colours; scripts can (look.fs: fields, fade-fog, room-look-reset).
- Characters: no root motion, no blending between motions, the faces and hands in their rest
  shape.
- Collision is the nav mesh only (no other characters). Exits all lead through (no locked
  doors, no door animations, no transition effects); no events, items, sound.
- The camera "director" is a stand-in (the nearest room setup); the game's real camera zones
  are in the room data and the decomp (`CamDirector_*`).
