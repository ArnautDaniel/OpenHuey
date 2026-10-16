
# OpenHuey
<img width="300" height="400" alt="h" src="https://github.com/user-attachments/assets/224650eb-86ee-4886-8f7a-74fb6e9bcf45" />

Another kind of Hewie (Discord setup at https://discord.gg/7sMGvfBAE)

This started with me really freaking wanting the Hewie model from Haunting Grounds because it's one of my favorite games.  It finally took me just getting Claude to run a decompile, about halfway through I had it make a model viewer and had my model but decided to keep going.  

This project covers 99% of the disassembly for Haunting Grounds converted to C.  The 1% are specific graphics functions that were converted to C when I had it reimplement the graphics calls on OpenGL instead of trying to very dumbly write a PS2 emulator (which it tried to do!)  It's probably a little early for primetime.

In new-src-actors I'm working on a more readable codebase conversion using Forth as the scripting language and a sort of actor philosophy for running the game objects.  C is used for the "engine" and renderer. It seems to work pretty well but it's not complete.  The plan is to take that more readable conversion and then hand write the final codebase (Or at least the bits that wouldn't be considered boilerplate)

My goal is to get a working Haunting Grounds on PC with some modern enhancements (maybe widescreen if it doesnt break everything visually, old games liked to hide things in the sides sometimes).  Secondly, I think it'd be fun to have a scriptable/modable version of the game.  Maybe you want to try stalker royale or have 10 hewies for some crazy reason.

------------- AI BELOW ------------------

**Download:** [hga latest](https://github.com/ArnautDaniel/OpenHuey/releases/tag/hga-latest)
(Linux and Windows, rebuilt on every merge) · [all releases](https://github.com/ArnautDaniel/OpenHuey/releases).
You need your own copy of Haunting Ground (USA) for the PS2: start `hga` and pick its disc image
(`.iso`) the first time. **No game data is included.**

OpenHuey rebuilds **Haunting Ground** (Capcom, PS2, 2005, `SLUS_210.75`) for PC, in three parts:

| Part | Path | What it is |
|---|---|---|
| The decompilation | `src/`, `include/` | The executable as C, each function checked against the original in an emulated PS2 CPU (`tools/difftest.py`). Functional, not byte-matching. [`docs/decomp.md`](docs/decomp.md) |
| `hg` | `native/` | That C running natively (SDL3, OpenGL, libavcodec): the **reference** for how the game behaves. |
| `hga` | `new-src-actors/` | The game rewritten for clarity and modding: a C engine, with Fiona, Hewie, the stalkers and the story as actors passing messages, written in Forth. A console runs Forth live. [`new-src-actors/README.md`](new-src-actors/README.md) |

`hga` plays the whole story: the title, the rooms and their scripts and cutscenes, Fiona, Hewie,
the stalkers, items and the menu, saving, the game over and the endings. Not yet: the parts where
you play as Hewie, and the extras' movie and gallery menus.

## Building

Needs CMake, a C compiler and SDL3 (movies: libavcodec, loaded at run time). Data: the disc image
(`.iso`), its `DATA.CVM`, or the extracted folder - as an argument, or chosen in a file dialog
and remembered.

```
# hga
cmake -S new-src-actors -B build/new-src-actors -G Ninja
cmake --build build/new-src-actors
build/new-src-actors/hga [disc.iso | data folder]
ctest --test-dir build/new-src-actors        # the tests, headless

# hg (first: tools/setup.sh, then .venv/bin/python configure.py --split; the executable
# SLUS_210.75, sha1 e0867d2ec83e6e2fccf147ad9e645a9a662d1c15, in baserom/)
cmake -S native -B build/native/cmake
cmake --build build/native/cmake -j8
build/native/cmake/hg [disc.iso | data folder]
```

In `hga`: `` ` `` opens the Forth console (`free-play` skips the opening, `castle-2f-3 room!` goes
to a room, more in `new-src-actors/scripts/debug.fs`); **F2** shows the debug meters, the actors
and the version; **F3** frees the camera. In `hg`: **F5** / **F9** quicksave and load; `HG_EVLOG=1`
traces the room scripts, `HG_ROOM=2A` starts at a room, `HG_HEADLESS=1 HG_MAXFRAMES=n` runs
without a window.

## Controls

A gamepad works too, through SDL.

| Key | Pad | In play | In menus |
|---|---|---|---|
| WASD | Left stick | Move | |
| Arrows | D-pad | | Move the cursor |
| X / Space | Cross | Hold to run | Confirm |
| C / Backspace | Circle | Act: examine, doors | |
| Z | Square | Shove / kick | |
| V | Triangle | Throw / set down | Cancel; skip a paused movie |
| Q / E | L1 / R1 | Q: crouch; E: flee | Turn pages |
| I J K L, F | Right stick, R3 | Hewie's commands | |
| Return | Start | Pause | Pause a movie |
| Tab | Select | The menu | Close it |

The earlier rewrite `new-src/` (`hg2`) is retired, kept at the tag `new-src-final`.
