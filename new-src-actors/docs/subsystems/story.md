# The story

## Purpose

The rooms' event scripts: what happens in each room. They place the characters as they come
in, give the camera its setups as characters move through a room's parts, open and close ways,
take exits, start scenes, show messages, hand out items, and run the characters' scripted
actions. The original runs them from bytecode (`event.c`); new-src converted all 170 rooms'
scripts to Forth once (`tools/events2forth.py`). They are data written in a language of their
own: ~460 words, one per command and condition of the bytecode.

## Structure

| Actor | What |
|---|---|
| `story` (one) | Spawns an actor for each room as it comes in (`arrived`), tells it when it is being left (`leaving-room`) and when the next room is in, and retires it once it reports done. |
| a room (named for it: `front-garden-2`) | The played room's scripts run as this actor. Its fields are the event state. |

- **The phases are its handlers.** Entering (`room-enter`): phase 0 (the event state anew),
  each character's entering script, phase 3. Each `tick`: phase 1 and the shared
  after-phase-1 script, phase 2 and after-phase-2, then the action scripts. At `frame-end`:
  phase 3, and an exit asked for is taken (`go-through` to the rooms). Leaving (`room-leave`):
  phase 4. Left (`room-left`, the next room in): phase 5, then `room-done`.
- **The action scripts are its coroutines**, in 17 slots (0..5 the characters', 6..16 the
  scenes' 0xF0..0xFA), each given one turn a frame in slot order. They share the room's state
  as the original's scripts share the event object, without another actor's fields being touched.
- The order matches the original's (`scene_game.c`): the characters act first (they were
  spawned before any room), then phases 1 and 2, the scripts, and phase 3 last.

Files: `scripts/story.fs` (the story actor), `scripts/story/room.fs` (a room's behaviour),
`scripts/story/state.fs` (its fields), `scripts/story/words.fs` (the words),
`scripts/story/shared.fs` (the shared scripts 0x80..), `scripts/story/rooms/<name>.fs` (each
room's scripts: `<name>.enter`, `.char-enter`, `.phase1`..`.phase5`, `.act00`.., `.cmd00`..,
registered by name), `scripts/story/rooms.fs` (all loaded). Generated once by
`tools/story_convert.py` from new-src; edited by hand since. `tools/story_stubs.py` keeps the
stubs of the words not written yet (below the marker in `words.fs`): run it after writing words.

## API

Declared in `scripts/messages.fs`.

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `arrived` / `leaving-room` | | in (rooms broadcast) | A room comes in / is being left. |
| `room-enter` | `( room exit -- )` | story → room | The room is in, entered by `exit`. |
| `room-leave` / `room-left` | | story → room | Phase 4 / phase 5. |
| `room-done` | | room → story | Finished: it may go. |
| `story-stop` | | → story | No more room scripts (the others' tests; the console). |
| `danger`, `panic`, `fiona-doing`, `hewie-doing` | | in (broadcasts) | Facts the conditions read (game mode, panic stage, Fiona's mode). |
| `go-through` | `( exit -- )` | → rooms | An exit taken (`exit-check`), after the frame. |
| `to-exit` | `( exit -- )` | → a character | Put yourself on the exit's outside spot, facing in (`char-to-exit`). |
| `camera-setup`, `follow`, `camera-restart` | | → camera | `char-camera`, `area-camera`, `chars-area-camera`, `camera-follow`. |
| `lock`, `unlock` | | → doors | `door-lock`, `door-unlock`. |
| `noise` | | → acoustics | A noise of the room's (source `world-noise`). |
| `panic-stage!` | | → panic | `panic-stage`. |

**Facts read:** the progress (story, state and resident flags, the byte variables, the
visited rooms, door states), bodies (where the characters are, their triangles and headings),
the room's event areas (crossing tests), the nav mesh. **Facts kept (the story owns them):** the
story, state and resident flags and variables the scripts set; the room's nav-mesh groups.

## State

A room's fields (`story/state.fs`): its room and the exit it was entered by; the script
variables (32), event bits, the counter, the exit taken, frames, the result; the message
window's text, answer and owner; the 17 slots (task, character, id, frames); the running
context (slot, character, id); each character's place at the last frame's end (for areas
entered / left) and whether a script of this room has it (scripted); who the camera follows;
what it was told (danger, panic, Fiona's mode, Hewie's action).

## Rules

From `event.c` (`Events_RunPhase`, `Events_CharEnter`, `Events_RunCharScripts`, the commands and
conditions), `scene_game.c` (the order), `progress.c`.

1. **Characters by slot:** 0 Fiona, 1 Hewie, 2.. the stalkers and the others (later); 0xFF the
   script's own character; 0xFE the active stalker.
2. **Areas entered / left** compare where a character was at the last frame's end with where
   it is now (the original's previous position).
3. **Taking an exit** (cmd 0x00): with bit 7, at once; otherwise if its door isn't locked, is
   passable, and Fiona isn't in a scripted action. **An exit usable** (cond 0x07): Fiona free
   (no script; moving on her own, or mode 0xA), in the exit's area, its door open, and the door
   not held. Arrivals are on the exit's outside spot, outside its area: nothing else stops a
   bounce, as in the original.
4. **Actions** (cmd 0x05 / 0x92): a scene's script in its slot if free (0x92: always); a
   character's in its slot, unless it already has one (0x92: always), the character then
   scripted until its script ends (`self-idle-or-end`). No new action once an exit is taken.
5. **`events-held`** runs only phase 3 and a shared script for characters entering.

## Design notes

- The rooms are named (`scripts/room-names.fs`): words and registrations use the names. Rooms
  on no page of the pause map are `off-map-XX` until named.
- Words not written yet are stubs: each says so once (`story: <word> - not written yet`), does
  nothing, and a condition is false. A waiting word's stub still waits a frame, so a script
  looping on it gives up its turn. `.missing` lists the ones met.
- The words are re-written for actors, not ported: what they changed directly in the original
  (a character's fields, the camera director, a door's state) is now a message to its owner.
- Room numbers inside the scripts (arguments of `stalker-to-room`, `hewie-to-room`, ...) are
  still numbers.
- `door-bits` (which parts of the room's door models are drawn) does nothing until door models
  are drawn by part.
- The camera's nearest-set stand-in stays as a fallback for a character the scripts give no
  setup.

## Status

**S1 built**: the story and room actors, the phases, the action scripts as coroutines; the
words for flags, variables, bits, the counter, chance; the script context and its waits;
actions; characters in areas, on triangles, their headings and rooms; exits (taking, usable,
arriving at); the camera's setups and who it follows; locking doors; the nav mesh's groups;
sounds and noises. Fiona's stand-in exit check is gone: the rooms' scripts take the exits.
Checked by `tests/story/test_story.fs` (12 tests): the new game's room actor and its entering
script (Fiona's camera setup, followed); a room change swaps the actors; front-garden-2's phase 1
takes exit 2 when Fiona is in its area, she arrives and isn't sent back; a door opened with her
animation and the exit taken; an action script started as a coroutine.

Since: placing characters (`char-to-tri`, `char-to-tri-facing`, `char-to-xz`, `char-to-xyz`:
a `place` message to the character), the room's look (`effect-string`: its fog and colours to
the renderer's `look-set` - without it every room kept its file's default, darker look). A
second arrival without a leaving between (a jump) retires the room still current.

**Free play** (`hga --eval free-play`, or at the console): the world as the opening leaves it,
until the opening can play (it needs the scripted moves, Hewie's squeeze through the grate, and
the cutscenes): `opening-done`, `grate-open`, the world not held, Hewie with her and hers to
command. The new game stays the default start.

**S2 built** (2026-10-08): examining and messages, items, zones.
- **Examining** (`cmd_scene_change`, `Progress_PlayerButtons`): the scripts prepare an action
  each frame (`scene-change`; cleared after phase 1, as `Progress_CharRequests` does). After
  phase 2 the room offers a "scene 5" one to Fiona (`offer`) and shows the prompt; with the
  action button pressed that frame, free, she takes it (`take-offer`): her action script starts
  and she is scripted (`scripted` on) until it ends (`scripted` off) - every script that takes a
  character tells it so, and a room releases its characters as it stops or is left. Other
  scenes (changes of scene, endings): S4.
- **Messages:** `message` (the window shows it; `text-closed` answers), `wait-message`,
  `message-close`, `answer?`, `message-prepare` (S4's subtitles), `message-param-room`.
- **Her pad:** `control-action?` (her gesture this frame, from `fiona-doing`), `pad?` (the
  buttons, `keys.fs`).
- **Facing:** `char-faces-xz?`, `char-faces-area?`; **move modes:** `char-action?`.
- **Zones:** `zone`, `zone-rect`, `char-zone-bits?`, `char-zone-bits-before?` (all off as phase
  1 starts). `char-in-zone?` is about attack points (her kick, his bite): false until the kicks
  (F4) and his bites (H3).
- **Items:** `item-give` (a file, with the pickup sound), `item-add`, `item-count?`,
  `item-give-count`, `item-use`; `script-item` (Events_ScriptRoom: `items-40-as-70`). The story
  writes the inventory in the progress until the items subsystem owns it.

Checked by `tests/story/test_examine.fs` (10 tests): in front of the hole the action is offered,
the button starts her script and the window shows its text, she can't move meanwhile, the button
closes it and frees her; an answer; items given and counted; a zone against her body.

**S3 built** (2026-10-08): the characters' moves. A script's move (the original's +0xF4) is a
`scripted-move ( kind a b x y z yaw )` to the character, which carries it out itself, as the
original's do; each character broadcasts `moving ( done ended )` at each frame's end (+0xE1 and
its animation's end), which the room keeps for `self-wait-done` / `self-wait-anim` /
`wait-char-anim`. Done is true while idle (a character with no move is done). Words: `self-move-to`,
`self-move-tri`, `self-idle`, `self-anim`, `self-anim-blend`, `self-anim-9`, `self-move-16`,
`self-turn-angle`, `self-turn-to-xz`, `self-turn-to`, `self-look-at`, `self-look-at-point`,
`self-move-slot`, `self-walk-anim`, `char-anim-hold` (`hold-anim`), `char-visible` (`show`).
- Fiona (`scripts/fiona/moves.fs`, from Fiona_Requests and the Fiona_StateCmd* states): straight
  to a spot (her door walk), along a path turning 10 degrees a frame and stepping by her root
  motion, turning on the spot (0x400 / 0x401), animations, her idle, the walk with an
  animation arriving at its event. Not yet: the paths' curve smoothing (Character_WaypointsCurve),
  her looks (12 / 13 do nothing yet), a door's use from a script (3 / 4).
- Hewie (`scripts/hewie/scripted.fs`, Hewie_Requests): the moves become his actions 0x3B..0x47
  and 0x7F (root-motion animations, walking a path, turning on the spot, heading for a spot and
  milling about, the scripts' bark, the leap); his looks at a character or a point. While a
  script has him (`scripted`) his frame is Hewie_Think; let go, his full stop and the default.
Checked by `tests/story/test_moves.fs` (11 tests) and `test_examine.fs` (her walk to the spot
and the kneel around the message).

**S4 built** (2026-10-08): fades, movies, cutscenes, music, motion events.
- **Fades** go to the `screen` actor (`fade`, `fade-finish`, `fade-over`); it answers `fade-done`,
  and `wait-fade` waits on the room's `fading`. (The kinds' music bits 0xC0 / 0x30: not yet.)
- **Movies** are the engine's (one at a time): `movie-play` opens the room's string paused (its
  class picks the compositing and whether it is drawn), `movie-volume` starts it, `movie-param`,
  `movie-playing?`. The room draws a drawn class's picture on UI layer 0 (`ui-movie`).
- **Cutscenes** are the C director's, cast from the characters' models: each character says
  which it is (`cast-as`: Fiona at her spawn, Hewie as he joins and parts). `cutscene-start`
  loads the room's scene and broadcasts `scene` on (Fiona is hands-off); `cutscene-control`'s 13
  ops step it, its cues, the movie, the subtitles (op 12 turns the prepared message's pages -
  drawn on layer 4), and op 8 ends it (`scene` off, the camera back). The conditions:
  `cutscene-mode?`, `-cue-reached?`, `-event?`, `-passed?`, `-near-end?`, `-shot?`.
- **Music** goes to the `music` actor: `bgm`, `music`, `music-stage`, `music-stage-end`.
- **Motion events:** each character's `moving` now carries its motion's event keys
  (`moving ( done ended events )`): `char-at-motion-event?`, `self-at-motion-event?`.
- Skipping: the movie pause (`pause` actor) sets `movie-skipped`, which the scenes' scripts read.
A room left or stopped closes its movie and ends its scene.
Checked by `tests/story/test_scene.fs` (11 tests): the cage room's opening (its stage music, the
background track, faded in); a fade out and in with `fade-done`; the scene at the cage (act00) -
the movie, the cutscene, Fiona in it, its subtitles - skipped from the movie pause, and played to
its end (Fiona free, the track back). Seen on screen: the scene's camera and letterbox, the
subtitles, the pause and the skip's darkening.

**Debugging** (`scripts/debug.fs`, at the console and in tests): `act` (a room action script as Fiona's), `room!`, `room-by!`, `tp`,
`tp-facing`, `tp-area`, `tp-area-facing`, `tp-exit`, `tp-tri`, `walk`, `press`, `.here`.

Next, by how much the scripts use them:

| Stage | Words |
|---|---|
| S4 | (built) The rest: scene changes (`scene-change`'s other scenes), endings, `movie-loop`, the half-black screen (op 11). |
| S5 | Effects, placed objects, lights (`object-show`, `effect-*`, `specks`, `lights-doorway`). |
| S6 | The stalkers' and creatures' words, with them. |
