# The Forth

The language is **ClankForth** (`external/clankforth`, a git submodule shared with OpenMizzurna):
its manual is `external/clankforth/docs/forth.md` (in a release: `docs/clankforth.md`) - the model, vocabularies, writing words,
locals, structures, tasks, errors and their call chains, and its Pitfalls. Read that first. A fix
to the language goes there (its own repository, a pull request; then this repository moves its
submodule forward and runs its tests), not here.

This page is OpenHuey's side: its words, its scripts, its tests.

- **The order of loading**: ClankForth's prelude (built in), `scripts/base.fs` (OpenHuey's words for
  every script: `seconds`, 30 frames a second), then `game.fs`.
- **Its warnings**: a file's word hiding one of a vocabulary it uses (`x hides vocab:x`), a word
  defined twice in one vocabulary (`x is defined again in v` - a real bug: the later wins from
  there on). Errors print the words that were running; so does a crash.
- **Messages**: a handler takes its message's arguments, all of them (docs/actors.md).
- `/vec` (vectors.fs) is the engine's three 32-bit floats (`v.x v.y v.z`).

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
| `room-group?` | `group -- flag` | that group of the room mesh is shown |
| `.room` | | a summary |
| `nav-tri`, `tri-center` | `F: x y z -- ` `-- tri`, `tri -- F: x y z` | nav triangles |
| `push-out` | `v tri other mask -- tri'` | v put out of body `other` to their radii + 0.01 apart, straight or turned up to 180 degrees, reached over the floor (Actor_PushOut); -1 nowhere |
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
| `screenshot` | `addr len --` | save the next frame as a PNG (the game's picture: the console isn't in it) |
| `screenshot-pending?` | `-- flag` | one asked for, not yet drawn |
| `zip-files` | `names len dir len zip len -- flag` | the files named (by spaces) in that folder zipped as `<folder>/<file>`; those not there left out (a bug report's zip) |
| `zip-has?` | `name len zip len -- flag` | that zip has the file `folder/file` |
| `console!` | `flag --` | open or close the console |
| `console-type` | `addr len --` | put text in the console's line, as if typed |
| `console-log` | `n --` | print the console's last n lines |
| `gfx` | `-- addr` | the picture's settings: `gfx.msaa gfx.aspect gfx.ssao gfx.bloom gfx.fog gfx.shadows gfx.tonemap gfx.debug gfx.room-fog gfx.room-tint gfx.room-bloom gfx.room-lights gfx.ps2-lighting gfx.shadow-maps` (l@ l!; `gfx.ps2-lighting` 0 OpenHuey's point lights, 1 the PS2's formula per vertex, 2 per pixel), `gfx.scale gfx.anisotropy gfx.ssao-radius gfx.ssao-strength gfx.bloom-threshold gfx.bloom-strength gfx.fog-density gfx.fog-start gfx.fog-r/g/b gfx.exposure gfx.saturation gfx.contrast gfx.vignette gfx.grain gfx.light-x/y/z gfx.light-r/g/b gfx.ambient-r/g/b gfx.rim gfx.character-light gfx.shadow-strength` (sf@ sf!) |
| `room-look`, `room-look-reset` | `-- addr` | the room's look now (`look.fog look.fog-early look.tint look.bloom look.dof look.bloom-mode` l@, `look.fog-near look.fog-far` sf@, `look.fog-near-color look.fog-far-color look.tint-glow look.tint-contrast look.bloom-color look.dof-range`: 4 floats); back to the file's |
| `on-draw`, `off-draw` | `xt --` | run a word every frame to draw 2D |
| `pen-color`, `pen-scale` | `rgba --`, `n --` | colour (0xRRGGBBAA) and text size for the drawing words |
| `draw-text`, `draw-rect` | `addr len x y --`, `x y w h --` | in window pixels from the top left |
| `screen-size`, `char-size` | `-- w h` | |
| `n>s`, `f>s$` | `n -- addr len`, `places F: x -- addr len` | numbers as text |
| `user-dir`, `file-exists?` | `-- addr len`, `addr len -- flag` | the player's settings folder |
| `.mods`, `mod-on?`, `mod-api` | `--`, `addr len -- flag`, `-- n` | the mods in load order (on, or off and why); a mod on; the mod API (major * 100 + minor) - docs/library-and-mods.md |
| `exe-table`, `motion-table` | `addr len -- addr n`, `id addr len --` | the executable's table of that name (src/data/exetables.c: "deal-rows" ...); a model's motion table by name ("motions-hewie" ...) |
| `to-file`, `end-file` | `addr len --`, `--` | what Forth prints goes into a file in between |
| `append-file` | `addr len --` | the same, added to the file's end |
| `make-dir` | `addr len -- flag` | a folder made (there already: true) |
| `date-time` | `-- addr len` | the local time now: `2026-10-19 14:03:22` |
| `xt>name` | `xt -- addr len` | a word's name |
| `act.shadow` | | an actor's contact-shadow radius (sf@ sf!) |

## The scripts

ClankForth's prelude (the language, `IN: forth`), `base.fs`, then `game.fs` (`IN: game`), which uses `subsystems.fs` to pull in the
rest and spawns their actors by qualified name (`stalker:riccardo-spawn`), so its handles (`game:fiona`) hide
none of theirs; each file is a vocabulary of the same name. Shared words: `common.fs`, `vectors.fs`,
`keys.fs`, `facts.fs` (the progress' flags), `messages.fs` (every message, declared once, and the actors by name: `fiona-id`, `doors-id` ...). Each
subsystem is its own file (`fiona.fs`, `stalker.fs`, ... - the map: `docs/subsystems.md`), with a
folder of the same name when it is bigger (`fiona/`, `hewie/`, `stalker/`, `story/`). Also
`graphics.fs` (the presets and the options menu, saved), `debug.fs` (the console's helpers, F2's
overlay, F12's bug reports).

The console (`` ` ``) evaluates whatever is typed, so any of this can be changed while the game
runs: redefine a word and the next tick uses it (words already compiled into others keep the
old one; hooks are looked up by execution token, so re-register with `on-tick` after a change,
or go through a `defer`).

Sound effects (the original's sound driver, src/platform/snddrv.c, on the SPU2-like voices of
spu.c): `bank-sound ( id bank -- )` plays sound `id` of a bank plainly, `sound-at ( id bank vol
pitch -- ) ( F: x y z -- )` at a point of the room (Actor_PlaySound: placed by the camera with
the original's distance curves and pan; vol x / 128 louder or softer, pitch semitones up),
`bank-sound-at ( id bank -- ) ( F: x y z -- )` the same with no offsets, `common-sound ( id -- )`
from the common bank. Banks: 4 the sound set (`sound-set! ( set -- )`), 5 the common sounds
(C_0000: Fiona's, Hewie's, the house's), 6 the room's (`sound-load ( k -- )` loads 4 / 5 / 6 now,
else they load when first used), 7 the pursuer's, any as `sound-bank ( k addr len -- flag )`
names it; `sound-loaded? ( k -- flag )`. A sound's entries go on voices of their own at a
priority, so a new sound takes over an older one on the same voice. `stop-sound ( id bank -- )`
releases one, `sound-stop-voices ( core0 core1 -- )` the voices in the masks, `sound-stop-all`.
`sound-volume! ( F: sound master -- )`, `sound-fade ( 0..255 -- )` (the pause screen's),
`sound-scale! ( F: v -- )` (the scripts' scale for sounds with id bit 30), `sound-reverb! ( core v
-- )` (core 0 the music's, 1 the effects': 0..0x3FFF; the rooms set it). `.voices` lists the
effect voices sounding.

`game-state` (C, `src/game/progress.c`; in the `forth` tool too) holds what the event scripts
share: `progress` (`pr.story pr.state pr.vars pr.resident pr.visited`: flag words, l@ / c@) and
`event-state` (`ev.room ev.vars ev.bits ev.counter ev.exit ev.message ev.slots ev.chars ...`;
`slot.task slot.who slot.id slot.frames`, `char.present char.id char.room char.scripted
char.actor`), with their sizes (`story-flags`, `script-slots`, `/slot`, `/char`, ...).

What happens in each room is its place, `scripts/story/places/<name>.fs`: one vocabulary per room,
`story.places.<name>`, on the words of the kit, `scripts/story/kit.fs` (docs/subsystems/story.md).

## Tests

`tester` (ClankForth's, built in: `USING: tester`): `T{ code -> results }T` runs the code and
checks the stack against the results; `testing name` labels what follows; `test-summary` prints
the totals and sets the exit status.

- The language's own tests (ClankForth's `tests/test_*.fs`) run with ours (`ctest`).
- `tests/test_*.fs`: the actor kernel's, run by the `forth` tool (ClankForth's `forth_tool_main` with
  the game state and the kernel bound; no game).
- `tests/<subsystem>/test_*.fs`: the game's, headless - `openhuey --test file.fs` loads the scripts,
  starts a new game at once, runs the file and exits with its status. `game-tick` runs a tick,
  `deliver` delivers what is queued; `key: X true key-hold` holds a key. Run them with
  `SDL_AUDIO_DRIVER=dummy` (and `--hidden` for other headless runs).
- `ctest` runs both, and the C checks of the library's conversions (`tests/*_check.c`).
- Mods' tests: `docs/modding.md`.
