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

## Vocabularies (as in Factor)

Words live in vocabularies. A file starts by saying where its definitions go and what it uses:

```forth
\ scripts/player.fs
IN: player
USING: engine keys vectors views state ;
```

- `USING: a b ;` loads each vocabulary that isn't loaded yet from its file - `a` is
  `scripts/a.fs`, `a.b` is `scripts/a/b.fs` - and makes its words visible here. `USE: a` does one.
- A file sees only its own words, the vocabularies it lists, and the core (`forth`: the
  language itself, from forth.c and prelude.fs). Nothing leaks in from files it didn't ask for.
- Its own definitions win over everything; a used vocabulary's win over the core. If two used
  vocabularies define the same name, using it is an error: say which, `vocab:name`.
- `<PRIVATE ... PRIVATE>` puts helpers in `name.private`: visible in the file, not to its users
  (`name.private:word` still reaches one, for debugging).
- Vocabularies loading each other (a cycle of USING:s) is an error.
- The prompt (console, `--eval`, the `forth` tool) sees every loaded vocabulary; `USING:` there
  narrows nothing, it loads. `vocabs` lists them, `vocab-words name` lists one, `words` lists
  what the current source sees.
- The engine's C words are the `engine` vocabulary.

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

Quotations: `[: ... ;]` leaves anonymous code as an execution token, inside definitions or at
the prompt (Factor's `[ ... ]`): `{ 1 2 3 } [: dup * ;] map`.

Lists: growable arrays of cells. `{ 1 2 3 }` makes one (in a definition, each time it runs),
`list` an empty one; `push ( x l -- )`, `pop`, `nth ( i l -- x )` (negative counts from the end),
`nth!`, `length`, `list-clear`, `list-free`. Higher-order words (prelude.fs): `each`, `map`,
`filter`, `reduce ( l acc xt -- acc' )`, `find ( l xt -- x true | false )`, `any?`, `all?`, `count`,
`contains? ( x l -- flag )`, `range ( n -- l )`, `.list`. Lists made while loading live for good;
free the ones made over and over at run time.

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
| `nav-tri`, `tri-center` | `F: x y z -- ` `-- tri`, `tri -- F: x y z` | nav triangles |
| `exit-tri` | `exit which -- tri` | this room's exit triangles: 0 out, 1 in, 2 through |
| `exit-leads` | `exit -- room exit'` | where an exit goes (-1 -1: nowhere) - the game's door table |
| `hud` | `addr len --` | a line of text at the bottom of the screen (`0 0 hud` clears) |
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
| `gfx` | `-- addr` | the picture's settings: `gfx.msaa gfx.aspect gfx.ssao gfx.bloom gfx.fog gfx.shadows gfx.tonemap gfx.debug gfx.room-fog gfx.room-tint gfx.room-bloom gfx.room-lights gfx.shadow-maps` (l@ l!), `gfx.scale gfx.anisotropy gfx.ssao-radius gfx.ssao-strength gfx.bloom-threshold gfx.bloom-strength gfx.fog-density gfx.fog-start gfx.fog-r/g/b gfx.exposure gfx.saturation gfx.contrast gfx.vignette gfx.grain gfx.light-x/y/z gfx.light-r/g/b gfx.ambient-r/g/b gfx.rim gfx.character-light gfx.shadow-strength` (sf@ sf!) |
| `room-look`, `room-look-reset` | `-- addr` | the room's look now (`look.fog look.tint look.bloom look.dof look.bloom-mode` l@, `look.fog-near look.fog-far` sf@, `look.fog-near-color look.fog-far-color look.tint-glow look.tint-contrast look.bloom-color look.dof-range`: 4 floats); back to the file's |
| `on-draw`, `off-draw` | `xt --` | run a word every frame to draw 2D |
| `pen-color`, `pen-scale` | `rgba --`, `n --` | colour (0xRRGGBBAA) and text size for the drawing words |
| `draw-text`, `draw-rect` | `addr len x y --`, `x y w h --` | in window pixels from the top left |
| `screen-size`, `char-size` | `-- w h` | |
| `n>s`, `f>s$` | `n -- addr len`, `places F: x -- addr len` | numbers as text |
| `user-dir`, `file-exists?` | `-- addr len`, `addr len -- flag` | the player's settings folder |
| `to-file`, `end-file` | `addr len --`, `--` | what Forth prints goes into a file in between |
| `xt>name` | `xt -- addr len` | a word's name |
| `act.shadow` | | an actor's contact-shadow radius (sf@ sf!) |

## The scripts

`prelude.fs` (the language, `IN: forth`), then `game.fs` (`IN: game`), whose `USING:` pulls in the
rest; each file is a vocabulary of the same name - `state` (shared flags), `keys`, `strings`,
`vectors.fs` (float vectors, the camera's position and angles), `freecam.fs`, `views.fs` (the
room's camera setups), `rooms.fs` (stepping through rooms), `player.fs` (playing as Fiona),
`doors.fs` (Space at an exit goes through to the room it leads to; `.exits` lists them),
`look.fs` (changing a room's look: `fog-range`, `fog-colors`, `fade-fog` - a task), `hewie.fs` (Hewie keeps up with her: trots, runs, stands watching; arrives at her side in a new
room), `graphics.fs` (presets, the F1 menu - a small menu system in Forth - and saving them).

The console (`` ` ``) evaluates whatever is typed, so any of this can be changed while the game
runs: redefine a word and the next tick uses it (words already compiled into others keep the
old one; hooks are looked up by execution token, so re-register with `on-tick` after a change,
or go through a `defer`).

The rooms' event scripts are `scripts/events/` (converted from the game's bytecode: see
`docs/events-plan.md`): `events.words` (the command and condition words, stubs for now) and one
vocabulary per map of the castle, `events.map0`..`map4`, `events.other`, `events.builtin`.

## Tests

`tests/test_*.fs` are vocabularies that say `USING: tester ;` (`T{ 1 2 + -> 3 }T`); `ctest` runs each.
