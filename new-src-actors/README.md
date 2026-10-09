# new-src-actors: Haunting Ground as actors

The third codebase, and the one meant to ship.

- `src/` (top level) is the decompilation: the PS2 executable as faithful C. It is the
  **specification**: when we need to know what the game does, we read it there.
- `new-src/` was the first rewrite (`hg2`): the same C engine, with gameplay written in Forth.
  Its C is solid and was copied here. Its Forth scripts transliterated the decompiled code (raw
  field offsets, vtable slots, the original's control flow), which made them fragile. It is
  **retired and archived**: no longer in the tree, kept at the git tag `new-src-final`
  (`git show new-src-final:new-src/...`). For what the game does, run `hg` (the decompilation's
  native build) - `HG_EVLOG=1` traces the event scripts.
- `new-src-actors/` (here) keeps that engine and redesigns the gameplay as **actors that pass
  messages**. Each subsystem is an actor, or a family of them, with a small vocabulary of
  messages as its API. The game should feel the same; the code shouldn't look decompiled.

Start with `docs/design.md`, then `docs/actors.md` (the kernel) and `docs/subsystems.md` (the
map of the game). Each subsystem gets its own page in `docs/subsystems/` before it is built.

## Building and running

```
cmake -S new-src-actors -B build/new-src-actors -G Ninja
cmake --build build/new-src-actors
build/new-src-actors/hga                 # the game (data: ../Haunting Ground (USA)/data)
build/new-src-actors/hga --hidden --frames 300 --eval "..."   # headless, for tests
ctest --test-dir build/new-src-actors    # all the tests
build/new-src-actors/hga --test new-src-actors/tests/acoustics/test_hearing.fs   # one subsystem's
```

Tests: `tests/test_*.fs` are the language and the kernel (the `forth` tool, no game data);
`tests/<subsystem>/test_*.fs` run inside the game headless (`hga --test`), with the game's data.
