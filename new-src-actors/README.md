# new-src-actors: Haunting Ground as actors

The third codebase, and the one meant to ship.

- `src/` (top level) is the decompilation: the PS2 executable as faithful C. It is the
  **specification**: when we need to know what the game does, we read it there.
- `new-src/` was the first rewrite: the same C engine, with gameplay written in Forth. Its C
  is solid and was copied here. Its Forth scripts transliterated the decompiled code (raw
  field offsets, vtable slots, the original's control flow), which made them fragile, so they
  stay behind as a **reference only**. Read them to find out what something does; don't copy
  their style.
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
ctest --test-dir build/new-src-actors    # the Forth tests
```
