# new-src: Haunting Ground, rewritten

`src/` is the decompilation: a faithful C version of the PS2 executable. It is frozen apart from
cleanup and labelling, and it is the reference for everything here.

`new-src/` is a fresh codebase built from that knowledge. It does not copy the original's shape.
It keeps the game's behaviour and data, and is free to restructure everything else:

- **No PS2.** No VU0/VU1, GS packets, IOP, fixed RAM addresses or 21 MB static game object.
  Data structures are plain C structs; the GPU is OpenGL; memory is malloc.
- **C does the heavy lifting**: platform (SDL3), rendering (OpenGL 4.6), file formats, maths,
  collision, the data structures (all struct definitions live in C).
- **Forth does the gameplay**: game logic, room scripts, events, AI decisions and tuning are
  Forth words. The Forth is our own small dialect, written in C, inspired by jonesforth, and
  shaped for game scripting (cooperative tasks, floats, vectors, struct field words).
- **Clarity first.** Simple code that reads top to bottom. Optimise later, when measured.

Nothing from `new-src/` goes back into `src/`.

## Building and running

```
cmake -S new-src -B build/new-src -DCMAKE_BUILD_TYPE=Debug
cmake --build build/new-src -j3
build/new-src/hg2 [data-dir]        # the game (data dir: the extracted DATA.CVM folder)
build/new-src/forth [file.fs ...]   # the Forth on its own: runs files, then a prompt
ctest --test-dir build/new-src      # the Forth test suite (and anything else under tests/)
```

The data dir defaults to `../Haunting Ground (USA)/data` next to the repository, or `$HG_DATA`.
The game's executable (`SLUS_210.75`, for the door and room tables) is looked for next to the data
folder, in it, in `baserom/`, or at `$HG_EXE`.
It builds as 64-bit (or 32-bit); it needs SDL3 and OpenGL 4.6.

The game starts at its title (`scripts/title.fs`): Hewie lying in the entrance hall, in real
time; New Game (Enter) has him get up and bark (the game's own bark, sound 0x65 of the common
bank), then Fiona takes over in the first room. Sound comes from the game's banks
(`src/data/soundbank.c`: the .HD / .SDT / .BD as the IOP driver plays them) through SDL3.

Keys (all defined in `scripts/`, not in C):

| Key | |
|---|---|
| <kbd>`</kbd> | the Forth console |
| Tab | play as Fiona / free camera |
| W A S D, Shift | walk (relative to the view), run - or fly the free camera |
| Q E, arrows, right mouse | free camera: down / up, look around |
| Space | at an exit: go through to the next room |
| F1 | graphics: presets (as on the PS2 / enhanced / cinematic) and every setting; arrows to change |
| [ ] | the room's own camera setups |
| PageUp / PageDown | the next / previous room |

## Layout

| Path | What |
|---|---|
| `src/main.c` | start-up and the main loop |
| `src/platform/` | SDL3 window, OpenGL loader, input |
| `src/render/` | the renderer: shaders, textures, meshes, 2D text |
| `src/data/` | the game's file formats: `.PAC` rooms, `.TEX` texture banks, room meshes |
| `src/game/` | engine-side game objects: the room, the camera, the console |
| `src/forth/` | the Forth system: the VM and compiler (`forth.c`), the C bindings (`bind_*.c`) |
| `scripts/` | Forth: `prelude.fs` (the language itself, built on the C core), then the game |
| `tests/` | tests (Forth test files run by the `forth` tool) |
| `docs/` | `design.md` (architecture and conventions), `forth.md` (the dialect) |

See `docs/design.md` for how the parts fit together.
