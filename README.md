
# OpenHuey
<img width="300" height="400" alt="h" src="https://github.com/user-attachments/assets/224650eb-86ee-4886-8f7a-74fb6e9bcf45" />

Another kind of Hewie (Discord setup at https://discord.gg/7sMGvfBAE)

This started with me really freaking wanting the Hewie model from Haunting Grounds because it's one of my favorite games.  It finally took me just getting Claude to run a decompile, about halfway through I had it make a model viewer and had my model but decided to keep going.  

This project covers 99% of the disassembly for Haunting Grounds converted to C.  The 1% are specific graphics functions that were converted to C when I had it reimplement the graphics calls on OpenGL instead of trying to very dumbly write a PS2 emulator (which it tried to do!)  It's probably a little early for primetime.

In new-src-actors I'm working on a more readable codebase conversion using Forth as the scripting language and a sort of actor philosophy for running the game objects.  C is used for the "engine" and renderer. It seems to work pretty well but it's not complete.  The plan is to take that more readable conversion and then hand write the final codebase (Or at least the bits that wouldn't be considered boilerplate)

My goal is to get a working Haunting Grounds on PC with some modern enhancements (maybe widescreen if it doesnt break everything visually, old games liked to hide things in the sides sometimes).  Secondly, I think it'd be fun to have a scriptable/modable version of the game.  Maybe you want to try stalker royale or have 10 hewies for some crazy reason.

------------- AI BELOW ------------------

OpenHuey is a project to understand, preserve and rebuild **Haunting Ground** (Capcom, PS2, 2005,
US release `SLUS_210.75`) for modern PCs. It has three parts:

1. **The decompilation** (`src/`): the game's own code, recovered as C.
2. **`hg`** (`native/`): that C running natively on PC.
3. **`hga`** (`new-src-actors/`): a rewrite of the game on the same engine, where Fiona, Hewie,
   the stalkers and the story are *actors* that pass messages, scripted in Forth.

**No game data is included.** Everything here needs your own copy of the game.

## The three parts

### The decompilation: `src/`, `include/`

The executable decompiled function by function into C. Each function is checked against the
original by `tools/difftest.py`, which runs both on random inputs in an emulated PS2 CPU (R5900)
and compares their calls, memory writes and results. It's a *functional* decompilation, not a
byte-matching one. The C builds back into a PS2 ELF together with the remaining assembly.

How to work on it: [`docs/decomp.md`](docs/decomp.md).

### `hg`: the game on PC (`native/`)

The decompiled game compiled for PC, with the PS2 hardware replaced: SDL3 for the window, input
and sound, OpenGL instead of the PS2's graphics chips, the movies decoded with libavcodec. It's
the original game's logic running natively, and the **reference** for how the game behaves:
when `hga` and `hg` disagree, `hg` is right (unless we find a bug in it).

### `hga`: the actor rewrite (`new-src-actors/`)

The game rebuilt for clarity and for modding. It keeps the engine (C: rendering, rooms, nav mesh,
models, sound, movies, the cutscene director) and rewrites the gameplay as **actors**. Each part
of the game (Fiona, Hewie, each stalker, the doors, the danger, the panic, each room's story) is
an actor with its own state and a small vocabulary of messages, written in a Forth built into the
engine. A console in the game runs Forth live. The room scripts are the original's event scripts
converted to Forth.

Where it stands:
- **Working:**
  - the title (new game, load, options; hard mode and the extras after a clear), the opening
    movie and the story's rooms, with their scripts and cutscenes (skip a movie: Start, then
    Triangle);
  - Fiona: moving, doors, ladders, crouching, fear and panic, shoving and kicking, throwing,
    being struck, grabbed and dragged;
  - Hewie: following her, his own mind, her commands, his attacks;
  - the stalkers: searching, travelling between rooms, chasing, attacking, grabbing, being struck;
  - items and the menu (items, files, the map, her comments, options, costumes), saving and
    loading (8 slots);
  - the message window, the tutorials and the pause screen, scaled to the window;
  - the game over and the endings.
- **Not yet:** about 20 of the story's script commands (they print "not written yet" when
  reached), and the game mode where you play as Hewie.
- **For fun:** `dance-party` in the console lines the golems up dancing; **F3** frees the camera
  (arrows move it, Page Up / Down raise and lower it, the right mouse button turns it).

Design and progress: [`new-src-actors/README.md`](new-src-actors/README.md) and
[`new-src-actors/docs/`](new-src-actors/docs/), one page per subsystem.

An earlier rewrite, `new-src/` (`hg2`), is retired. It is kept at the git tag `new-src-final`.

## Getting started

You need:
- the US executable `SLUS_210.75` (sha1 `e0867d2ec83e6e2fccf147ad9e645a9a662d1c15`), in
  `baserom/`;
- the game's data: for `hg`, the disc image (`.iso`) itself, its `DATA.CVM`, or a folder
  `DATA.CVM` was extracted to. `hg` takes it as an argument or in `HG_DATA`; else it uses the
  disc chosen last time, then `../Haunting Ground (USA)/data` next to this repository, and if
  none of those is there it asks for the disc image with a file dialog (and remembers it).
  `hga` needs the extracted folder (default `../Haunting Ground (USA)/data`, or an argument).

Building needs CMake, a C compiler and SDL3. Movies need libavcodec (loaded at run time; without
it, movies are skipped). The PS2 build of the decompilation also needs the ps2dev toolchain:
`tools/setup.sh` (see `docs/decomp.md`).

### `hg`

`hg` takes the game's data (vtables, tables, strings) and a stub for every unwritten function from
the disassembly, so split the executable first (once, and again after changing `config/`):

```
tools/setup.sh
.venv/bin/python configure.py --split
```

Then:

```
cmake -S native -B build/native/cmake
cmake --build build/native/cmake -j8
build/native/cmake/hg [data folder]
```

Keys: see [Controls](#controls) below.

Useful environment variables:
- `HG_EVLOG=1` traces the room scripts (actions, scene requests, movies, cutscenes, flags,
  doors, rooms entered, Fiona's position).
- `HG_ROOMLOG=1` traces room changes and placements.
- `HG_ROOM=2A` starts a new game at entry `2A`.
- `HG_FASTBOOT=1` skips the logos.
- `HG_HEADLESS=1` with `HG_MAXFRAMES=n` runs without a window.
- `HG_QUICKLOAD=1` starts from the quicksave instead of the title.

In play the room is shown in the top right corner (hex, as `HG_ROOM` takes it). **F5** quicksaves
(the game's own save, taken anywhere in play, into `save/quicksave.bin`); **F9** loads it, in
play or on the title. The options menu (`` ` ``) has 9 more save slots: SLOT picks one (its room
is shown), SAVE / LOAD write or read it (`save/slot_N.bin`; `HG_QUICKLOAD=s3` starts from slot 3).
Quicksaves and slots never touch the memory card saves.

### `hga`

```
cmake -S new-src-actors -B build/new-src-actors -G Ninja
cmake --build build/new-src-actors
build/new-src-actors/hga [data folder]
```

Keys: see [Controls](#controls) below. The backquote key (`) opens the Forth console.

In the console:
- `free-play` skips the opening into the first room.
- `castle-2f-3 room!` goes to a room.
- `.here` says where Fiona is.
- `castle-1f-10 debilitas-hunt` sends Debilitas after her from another room.

More console words are listed in `new-src-actors/scripts/debug.fs`.

Tests run headless: `ctest --test-dir build/new-src-actors`, or one suite:
`build/new-src-actors/hga --test new-src-actors/tests/stalker/test_stalker.fs`.

## Controls

`hg` and `hga` share one keyboard layout. A gamepad works too, through SDL.

| Key | Pad button | In play | In menus |
|---|---|---|---|
| WASD | Left stick | Move | |
| Arrows | D-pad | | Move the cursor |
| X / Space | Cross | Hold to run | Confirm, read on |
| C / Backspace | Circle | Act: examine, doors | |
| Z | Square | Shove / kick | |
| V | Triangle | Throw / set down what she holds | Cancel; skip a paused movie |
| Q / E | L1 / R1 | Q: hold to crouch; E: flee | Turn the menu's pages |
| 1 / 3 | L2 / R2 | | |
| I J K L | Right stick | Hewie's commands | |
| F | R3 | Hewie's command | |
| Return | Start | Pause | Start; pause a movie |
| Tab | Select | The menu | Close the menu |

## Repository layout

| Path | What it is |
|---|---|
| `src/`, `include/` | The decompilation |
| `native/` | `hg`: the PC platform layer for the decompilation |
| `new-src-actors/` | `hga`: the actor rewrite (engine C in `src/`, Forth in `scripts/`, tests, docs, tools) |
| `tools/`, `config/` | The splat / build pipeline, the decompilation helpers, the format tools |
| `docs/` | The decompilation's docs: workflow (`decomp.md`), progress, known issues, the event script opcodes, the model format, a design guide to the game |
| `baserom/` | Where your copy of the executable goes (ignored by git) |
