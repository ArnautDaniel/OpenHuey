# openhuey-src: OpenHuey, Haunting Ground as actors

The third codebase, and the one meant to ship: the program `openhuey`. (This folder was
`new-src-actors/` and the program `hga` until 0.1.0; the mods' `requires: hga` and the
`HGA_HOME` variable are still understood.)

- `src/` (top level) is the decompilation: the PS2 executable as faithful C. It is the
  **specification**: when we need to know what the game does, we read it there.
- `new-src/` was the first rewrite (`hg2`): the same C engine, with gameplay written in Forth.
  Its C is solid and was copied here. Its Forth scripts transliterated the decompiled code (raw
  field offsets, vtable slots, the original's control flow), which made them fragile. It is
  **retired and archived**: no longer in the tree, kept at the git tag `new-src-final`
  (`git show new-src-final:new-src/...`). For what the game does, run `hg` (the decompilation's
  native build) - `HG_EVLOG=1` traces the event scripts.
- `openhuey-src/` (here) keeps that engine and redesigns the gameplay as **actors that pass
  messages**. Each subsystem is an actor, or a family of them, with a small vocabulary of
  messages as its API. The game should feel the same; the code shouldn't look decompiled.

Start with `docs/design.md`, then `docs/actors.md` (the kernel) and `docs/subsystems.md` (the
map of the game). Each subsystem gets its own page in `docs/subsystems/` before it is built.
The Forth is ClankForth (`external/clankforth`, a submodule: clone with `--recursive`, or `git submodule update --init`; the language `external/clankforth/docs/forth.md`, OpenHuey's side `docs/forth.md`); the game library (the disc imported, its assets in common formats)
and mods are `docs/library-and-mods.md`; **making a mod: `docs/modding.md`**.

## Building and running

```
cmake -S openhuey-src -B build/openhuey-src -G Ninja
cmake --build build/openhuey-src
build/openhuey-src/openhuey                       # the game: from its library (the first run imports the disc)
build/openhuey-src/openhuey --import disc.iso     # import a disc into the library (library/ beside OpenHuey)
build/openhuey-src/openhuey --update            # a release: updated to the newest build (the title's UPDATE too)
build/openhuey-src/openhuey --mod path/to/mod     # with a mod on (mods/ beside OpenHuey holds the installed ones)
build/openhuey-src/openhuey --hidden --frames 300 --eval "..."   # headless, for tests
ctest --test-dir build/openhuey-src    # the quick tests (OPENHUEY_FULL_TESTS=1: all of them)
build/openhuey-src/openhuey --test openhuey-src/tests/acoustics/test_hearing.fs   # one subsystem's
```

In the game: `` ` `` opens the Forth console (`scripts/debug.fs` has its helpers; Ctrl+V pastes,
each whole line run, Ctrl+C copies the line typed or, with none, what the last one printed,
Ctrl+Shift+C the whole log, Ctrl+X cuts), F1 the settings
(the picture - two presets: *enhanced*, the default, the game with some modern polish, and *as on
the PS2*, the original's picture exactly, its characters lit by its own formula - fullscreen, the
controls - saved in `user/`), F11 or Alt+Return fullscreen, F2 the debug
overlay (Fiona's meters, the actors alive and their ids, OpenHuey's version), F3 the free camera,
F12 a bug report: the moment kept in `user/bugs/<date>_<time>/` (report.txt - where she is, every
actor's state, the last messages, the console's lines - screenshot.png and save.fs), the console
opened with `bug-note ` typed for what happened (into note.txt). All of it is zipped beside the
folder, `user/bugs/<date>_<time>.zip` (again with each note): drag that onto a GitHub issue. At
the console `bug <what happened>` does both at once; `s" <folder>/save.fs" bug-load` plays the moment again. The
version is CMake's `project(OpenHuey VERSION ...)`; the release builds carry it in their names.

Tests: `tests/test_*.fs` are the language and the kernel (the `forth` tool, no game data);
`tests/<subsystem>/test_*.fs` run inside the game headless (`openhuey --test`), with the game's data.
