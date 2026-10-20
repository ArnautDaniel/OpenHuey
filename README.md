
# OpenHuey
<img width="300" height="400" alt="h" src="https://github.com/user-attachments/assets/224650eb-86ee-4886-8f7a-74fb6e9bcf45" />

Another kind of Hewie (Discord setup at https://discord.gg/7sMGvfBAE)

**Download:** [OpenHuey latest](https://github.com/ArnautDaniel/OpenHuey/releases/tag/openhuey-latest)
(Linux and Windows, rebuilt on every merge) · [all releases](https://github.com/ArnautDaniel/OpenHuey/releases).
You need your own copy of Haunting Ground (USA) for the PS2: start `openhuey` and pick its disc
image (`.iso`) the first time - it is imported once into `library/` beside it. **No game data is
included.** Mods: `mods/` beside it ([`openhuey-src/docs/modding.md`](openhuey-src/docs/modding.md)).

OpenHuey rebuilds **Haunting Ground** (Capcom, PS2, 2005, `SLUS_210.75`) for PC, in three parts:

| Part | Path | What it is |
|---|---|---|
| The decompilation | `src/`, `include/` | The executable as C, each function checked against the original in an emulated PS2 CPU (`tools/difftest.py`). Functional, not byte-matching. [`docs/decomp.md`](docs/decomp.md) |
| `hg` | `native/` | That C running natively (SDL3, OpenGL, libavcodec): the **reference** for how the game behaves. |
| `openhuey` (once `hga`) | `openhuey-src/` (once `new-src-actors/`) | The game rewritten for clarity and modding: a C engine, with Fiona, Hewie, the stalkers and the story as actors passing messages, written in Forth. A console runs Forth live. [`openhuey-src/README.md`](openhuey-src/README.md) |

`openhuey` plays the whole story: the title, the rooms and their scripts and cutscenes, Fiona,
Hewie, the stalkers, items and the menu, saving, the game over and the endings, and the mini-game
where you play as Hewie. Not yet: the extras' movie and gallery menus.

## Building

Needs CMake, a C compiler and SDL3 (movies: libavcodec, loaded at run time). Data: `openhuey`
imports the disc image (`.iso`) once into its library (chosen in a file dialog the first time, or
`openhuey --import disc.iso`); a disc image, its `DATA.CVM` or the extracted folder can also be
given as an argument. `hg` reads the disc image, `DATA.CVM` or folder as an argument, or chosen
and remembered.

```
# OpenHuey
cmake -S openhuey-src -B build/openhuey-src -G Ninja
cmake --build build/openhuey-src
build/openhuey-src/openhuey                # (or --import disc.iso, or [disc.iso | data folder])
ctest --test-dir build/openhuey-src        # the tests, headless

# hg (first: tools/setup.sh, then .venv/bin/python configure.py --split; the executable
# SLUS_210.75, sha1 e0867d2ec83e6e2fccf147ad9e645a9a662d1c15, in baserom/)
cmake -S native -B build/native/cmake
cmake --build build/native/cmake -j8
build/native/cmake/hg [disc.iso | data folder]
```

In `openhuey`: `` ` `` opens the Forth console (`free-play` skips the opening, `castle-2f-3 room!` goes
to a room, more in `openhuey-src/scripts/debug.fs`); **F1** the settings (the picture, fullscreen,
the controls); **F11** or Alt+Return fullscreen; **F2** shows the debug meters, the actors and the
version; **F3** frees the camera. In `hg`: **F5** / **F9** quicksave and load; `HG_EVLOG=1`
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
