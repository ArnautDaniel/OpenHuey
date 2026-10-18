# OpenHuey (openhuey-src): notes for an AI assistant

OpenHuey is Haunting Ground rebuilt: a C engine (`src/`) with the gameplay as Forth actors passing
messages (`scripts/`). The decompiled original game (`../src/`, C) is the reference for what the
game does. Read `README.md` first, then the doc for your task:

| Task | Read |
|---|---|
| any gameplay change | `docs/design.md`, `docs/actors.md`, `docs/subsystems.md`, the subsystem's `docs/subsystems/<name>.md` |
| writing Forth | `docs/forth.md` (the dialect: vocabularies, locals, structures, enums, quotations; its Pitfalls section) |
| a mod, or the mods system | `docs/modding.md` (how), `docs/library-and-mods.md` (why) |
| the release builds, the updater | `tools/hga_release.sh`, `.github/workflows/hga-release.yml`, `docs/library-and-mods.md` ("Updating") |
| the game's data, the library, a file format | `docs/library-and-mods.md`, the header of the `src/data/*.h` for that format |

**Names**: the program is `openhuey` (built from the CMake target `hga`), this folder
`openhuey-src/` - until 0.1.0 they were `hga` and `new-src-actors/`. Internal names keep `hga`
on purpose: the `hga-version` word, `HGA_*` macros, the `hgadata` library, `tools/hga_release.sh`,
the library files' `hga-...` format lines and the glTF extras key `"hga"` (changing those would
break existing libraries), the save files `hga-*.fs` (players' saves). Mods may say
`requires: openhuey` or `requires: hga`; `OPENHUEY_HOME` or `HGA_HOME`.

## Building and testing

```sh
cmake -S . -B build && cmake --build build -j
cd build && SDL_AUDIO_DRIVER=dummy ctest -j8          # everything (needs the game's data)
SDL_AUDIO_DRIVER=dummy ./openhuey --test ../tests/<sub>/test_<x>.fs   # one test file
SDL_AUDIO_DRIVER=dummy ./openhuey --hidden --frames 3 --eval "..."    # headless; `screenshot` for a PNG
```

- Always `SDL_AUDIO_DRIVER=dummy`, and `--hidden` for headless runs (no window, no sound).
- The game's data: the library beside OpenHuey (`OPENHUEY_HOME=<folder>` names another place - use one for
  experiments: runs write saves and settings there), else the developer's extracted data folder
  set at build time, else OpenHuey asks for the disc. `openhuey --import disc.iso` makes a library.
- A run outside `--test` writes the player's system data (unlocks, best times) in `<home>/user/`:
  use a scratch `OPENHUEY_HOME` for screenshots and probes.

## How this project works

- **Match the original's behaviour exactly**; any difference from the decompiled game is a bug.
  Read the original function (the scripts cite them: `Panic_Breath`, `Items_Use` ...) before
  changing behaviour.
- **Readable Forth, not transliterated C**: named words and structures (`begin-structure`,
  `state:` fields, `enum:`), locals over stack juggling, magic numbers named. Cumbersome stack
  work becomes a C helper. Gauge the original's intent; don't port its control flow line by line.
- **Everything is actors passing messages**; an actor never writes another's state. New messages
  are declared in `scripts/messages.fs` with their stack comment.
- **Docs move with the code**: a subsystem's page (`docs/subsystems/<name>.md`) records its
  messages, rules and status - update it in the same change. Keep "Not yet" / "Left" lists true.
- **Tests with every change**: a `T{ ... -> ... }T` test in the subsystem's `tests/` folder;
  before / after screenshots for anything drawn.
- **Library conversions are exact**: a converted format must give the engine the same data as
  the original (each kind has a `tests/*_check.c` that proves it).
- One branch and one pull request per piece of work.
