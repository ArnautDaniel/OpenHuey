# The Forth

A small Forth in the spirit of jonesforth, written in C (`src/forth/forth.c`) with the rest of
the language in Forth (`scripts/prelude.fs`). It is not ANS Forth; it is a scripting language
for this game. Names are case-insensitive.

## The model

- **Cells** are pointer-sized integers, and addresses are real addresses: `@` and `!` reach C
  structs directly. Memory words come in sizes: `c@ w@ sw@ l@ sl@ @` (8/16/32 bits, signed
  variants) and their stores.
- **Floats** have their own stack (doubles). A number with a `.` or an exponent is a float:
  `1.5`, `2e`, `-0.25e3`. In memory floats are 32-bit like the game's: `sf@ sf!` (and `f@ f!` for
  a Forth `fvariable`, which holds a double).
- **Words** are indirect threaded: each has a code field (a C function) and a body. `see word`
  decompiles a colon definition.
- **Tasks**: every running piece of Forth has its own stacks. `' word spawn` starts one (it gives
  an id); the engine runs each ready task a little every tick. Inside a task, `yield` gives way
  until the next tick and `n wait` for n ticks. `id kill` stops one, `.tasks` lists them.
- **Errors** print a message with the file and line and unwind: at the prompt the stacks are
  emptied; a task that errs is stopped; an `on-tick` hook that errs is removed. `' word catch`
  runs a word and gives -1 instead of unwinding (`error-message` has the text).

## Writing words

```forth
: square ( n -- n*n ) dup * ;
variable lives   3 lives !
10 constant max-items
0 value score   score 1+ to score
fvariable speed  2.5e speed f!
defer on-hit   ' noop is on-hit
: array ( n "name" -- ) create cells allot does> ( i -- addr ) swap cells + ;
```

Control: `if else then`, `begin until`, `begin while repeat`, `begin again`,
`do loop`, `?do loop`, `+loop`, `i j leave unloop`, `case of endof endcase`, `exit`, `recurse`.

Strings: `s" text"` gives `addr len`; `." text"` prints; `type`, `s=`, `compare`.

Comments: `( ... )` and `\ to the end of the line`.

## Game words (bind_engine.c)

| Word | Stack | |
|---|---|---|
| `room` | `id --` | load a room (`$03 room`) |
| `room-id`, `room-exists?` | `-- id`, `id -- flag` | |
| `room-bounds` | `F: -- lx ly lz hx hy hz` | the solid part's box |
| `room-cameras`, `room-camera` | `-- n`, `i -- F: ex ey ez fov tx ty tz` | the room's camera setups |
| `floor-below` | `F: x y z -- y'` `-- flag` | the solid surface under a point (the drawn mesh) |
| `nav-tris` | `-- n` | the nav mesh's size (0: the room has none) |
| `nav-move` | `F: x y z dx dz climb radius -- x' y' z'` | a step on the nav mesh, sliding along walls |
| `nav-at` | `F: x y z climb -- [y']` `-- flag` | on the nav mesh? (and its height) |
| `nav-nearest` | `F: x y z -- x' y' z'` | the middle of the nearest nav triangle |
| `room-group!` | `group flag --` | show or hide a group of the room mesh |
| `.room` | | a summary |
| `camera` | `-- addr` | with `cam.x cam.y cam.z cam.yaw cam.pitch cam.fov cam.near cam.far cam.up` (sf@ / sf!) |
| `actor-load` | `addr len -- id` | `s" O_FIN/FIN_000" actor-load` |
| `actor` | `id -- addr` | with `act.x act.y act.z act.yaw act.scale act.frame act.rate` (sf@) and `act.loop act.visible` (l@) |
| `motion!`, `motion@` | `id motion --`, `id -- motion` | play a motion by the game's number (`fiona @ $200 motion!`) |
| `motion-done?`, `motion-frames`, `.motions` | | |
| `key:` | `"name" -- scancode` | `key: W`, `key: Left_Shift` (SDL key names, `_` for spaces) |
| `key-down?`, `key-pressed?` | `scancode -- flag` | held now / went down this tick |
| `key-hold` | `scancode flag --` | hold a key from a script (demos, tests) |
| `mouse-dx`, `mouse-dy`, `mouse-down?` | `F: -- x`, `button -- flag` | |
| `on-tick`, `off-tick` | `xt --` | run a word every tick |
| `ticks`, `dt` | `-- n`, `F: -- seconds` | |
| `clear-color` | `F: r g b --` | |
| `screenshot` | `addr len --` | save the next frame as a PNG |
| `console!` | `flag --` | open or close the console |

## The scripts

`prelude.fs` (the language), then `game.fs`, which loads the rest with `script name.fs`:
`vectors.fs` (float vectors, the camera's position and angles), `freecam.fs`, `views.fs` (the
room's camera setups), `rooms.fs` (stepping through rooms), `player.fs` (playing as Fiona).

The console (`` ` ``) evaluates whatever is typed, so any of this can be changed while the game
runs: redefine a word and the next tick uses it (words already compiled into others keep the
old one; hooks are looked up by execution token, so re-register with `on-tick` after a change,
or go through a `defer`).

## Tests

`tests/test_*.fs` run under `tests/tester.fs` (`T{ 1 2 + -> 3 }T`), from `ctest`.
