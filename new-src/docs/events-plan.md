# Plan: the rooms' event scripts as Forth (to discuss)

Not built yet - this is the proposal for the part that matters most for the Forth: replacing the
game's event bytecode.

## What the game has (from the decomp, `src/game/event.c` and `src/game/rooms/`)

- Each room has a handler class (0x110 rooms). Its virtual functions return the room's scripts
  for each phase - entering (`RoomXX_EnterScript`), the character entering, phases 1-5, actions -
  as pointers into the executable's data. Rooms also have their own commands and conditions in C
  (`RoomXX_Command` / `RoomXX_Condition`, e.g. `RoomD7_Cmd00`, `RoomD7_Cond00`).
- The bytecode, big-endian operands:
  - commands 0x00-0xDA (lengths: `sCmdLength` in event.c; 0 = 3 + a string's length);
  - conditions 0x00-0x65 (lengths: the table at 0x3D72C0; 0x11 carries a string);
  - control ops: F0 if, F1 if not, F2/F3 and (not), F4/F5 or (not), each followed by a
    condition; F6/FA open a block, F7 else, F8/FB end, F9 end + skip, FF end of script.
  - Commands can wait (the script resumes next frame), which is what makes them sequences.
- The engine around it: `EventCmd_Run` (a 1300-line switch), `EventCond_Eval` (800 lines).

## The proposal

1. **A converter** (a tool, run once): read the scripts from the player's executable the same way
   `world.c` reads the door table, walk the bytecode with the length tables, and write one Forth
   file per room, e.g. `scripts/rooms/room_000.fs`:

   ```forth
   room: $000
   : enter
       exit-taken 4 = if  4 exit-in fiona-at  else  0 exit-in fiona-at  then
       $12 flag? 0= if  $12 flag-set  $0301 movie  then ;
   : phase-1  ... ;
   ;room
   ```

   Control ops become `if else then` and `and`/`or`; each command becomes a word named for what
   it does (from the decomp's comments: `flag-set`, `movie`, `camera-cut`, `sound`, ...); a
   condition becomes a word leaving a flag. Unknown ones convert as `$5A cmd( 01 02 )` so nothing
   is lost and they can be named later.

2. **Waits are tasks.** A command that waits in the original becomes a word that `wait`s or
   `yield`s; each room script runs as a task (`' enter spawn`), so the sequences read top to
   bottom with no state machine.

3. **The words**: written as needed, most in Forth on top of a few C ones (flags live in a C
   struct, movies and sounds are C). The room-specific C commands become Forth words in the
   room's file.

4. After conversion the Forth files are the source of truth: edited by hand, with the
   converter kept only to check against the original.

## Decisions (2026-10-06)

1. Vocabulary: a general scripting vocabulary that hides the game's quirks (`door-open`, not
   setting a door's flag bit). The decomp's names go in comments, so each word can be checked
   against the original.
2. Files: grouped by area of the castle (not one per room).
3. The converter is Python (new-src/tools/).
4. Conversion is one-time: afterwards the Forth is the source and is edited by hand; the
   converter stays only to check against the original.
5. Every command and condition is understood before converting (no raw placeholders). New
   understanding may be backported to src as names and comments.
6. The rooms' own C commands and conditions are ported to Forth too.
7. Each script phase runs as a Forth task (waits are `wait` / `yield`).
8. C owns the game state (progress, flags, items, doors) as structs; Forth reaches it through
   field words.
9. Match the original's behaviour and timing as closely as possible.
Also: the Forth may grow lists and higher-order words (map, reduce, ...) where they make the
scripts simpler.

## Status (2026-10-06): converted

`tools/events2forth.py` has run: every script of the game (2268: each room's entering, phase,
character-entering and action scripts, and the shared ones) is Forth in `scripts/events/`, in
one file per map Fiona finds (the game's own `kMapRooms`: `map0`..`map4`, `other` for rooms on
no map, `builtin` for the shared scripts). Names and operands come from the decomp's opcode
table (`tools/event_opcodes.py`, `docs/event_opcodes.md`).

- `events/words.fs` holds one word per command and condition, with its description. They are
  **stubs** for now (commands drop their operands, conditions answer `stub-flag`); writing them
  for real, in Forth over C words, is the next step. The room tables (`room-scripts`,
  `action-scripts`, `builtin-scripts`) are real: each file registers its scripts at the end of
  each room.
- Each room's own C commands / conditions are `roomXX.cmdNN` / `roomXX.condNN?` stubs at the top
  of the room, with the C function's description, to be written in Forth.
- Control flow: F9 became `else` chains (`a if X else b if Y else Z then then`); the loop point
  became `begin ... while ... repeat` (883), `begin ... until` (20), `begin ... again` (115), or
  a loop that leaves a flag, `true` out / `false` round (229). Eight action scripts repeat 7-18
  lines in both branches of an if (where an F9 or a loop back leaves from deep inside).
- Going to another script is `['] target goto`; nine scripts reached in a cycle are `defer`red.
  The shared scripts reach room actions by id (`goto-action`, `call-action`).
- Oddities of the original, kept and commented: room 5A's action 2 has an else outside any
  block (the original never runs it); room 34 calls action 9, which it doesn't have (the
  original reads past its table).
- `tests/test_events.fs` loads everything and runs every script as a task three times with the
  stub conditions answering at random: each leaves its stacks as it found them.
- The dictionary is 16 MB (the scripts take 2.7 MB).

## Status (2026-10-06, later): running

- `events/core.fs`: the script tables, flags, the scripts' characters, the 17 action slots (each
  a held task), starting / ending actions (cmd_action, Event_StartAction, Event_CharSlot),
  `goto` (`restart`: the task goes on in the other script), the missing-word report.
- `events/runner.fs`: phases 0..5, characters entering, the slots a turn each, in the original's
  order (`enter-room`, `run-frame`, `leave-room`; `.slots` for the console).
- `events/words.fs`: the flags, variables, event bits, counters, frame counts and waits,
  messages (the state; the window is a placeholder line), actions, `char-to-exit`, `self-is?`,
  `char-here?`, `exit-taken?`, `chance?`, `result?` are real; the rest are stubs that say so on the
  console the first time they run (`.missing` lists them).
- `events/play.fs`: in the game, while playing: Fiona and Hewie are the scripts' characters 0
  and 1, rooms entered run their scripts (with the exit come in by), each tick runs a frame.
- Waits only wait in an action slot (the original ignores "self" commands in phase scripts).
- `tests/test_runner.fs`: entering room 00 by exit 0 runs its scripts (Fiona put at the exit),
  slots, self, goto, a character's action, flags.

## Status (2026-10-07): the game's camera

- `src/game/camdirector.c`: the camera director ported (CamDirector_*, CamPath_*, Spline_*):
  the room's camera sets (section 5) and paths (section 6, Bezier splines with an eye and a
  look-at track); a new set cuts, a path is ridden nearest the followed character, no path turns
  to keep it in view; the view angle dips and eases after a room start. (Its out-of-view test is
  new-src's own frustum, not the original's matrices.)
- `src/game/areas.c`: the room's event areas (section 2): quads (inside) and gates (crossed).
- `src/game/exits.c`: an exit's spots from its door (section 7: out 4 behind, in 12 ahead,
  through 12 behind, turned away from the exit's area, walked over the nav mesh), else the room
  table. doors.fs uses them too (the door table's triangle was not where characters stand).
- Words: `char-camera`, `camera-follow`, `area-camera`, `chars-area-camera`, `camera-restart`,
  `camera-setup-changed?`, `char-in-area?`, `char-entered-area?`, `char-left-area?`; the
  camera's character logic (Progress_CameraFollow / CameraOn / CameraSetup) in events/core.fs.
  Characters' positions (now and the frame before) are in the scripts' state.
- The runner does the director's steps where SceneGame does (ease, phases, slots, the followed
  character's setup, track, phase 3, update; the room start after the characters enter).
- While playing, the director is the camera; player.fs's chase camera stands in only when the
  followed character has no camera set (a room jumped into, the start).

To check against the original: in room $0E Fiona can run where path 0 loses her (x -94);
whether the original keeps her in view there (or she can't go there) needs the src build.

## Status (2026-10-07): messages

- `src/game/messages.c`: the game's messages (system: SUBSCR/MSG_BASE.BIN, ids with bit 15;
  the second table: MSG_SUB.BIN, bit 14; the room's own: section 10), laid out as pages of
  plain lines with their options (the byte code: include/text.h); parameters as
  Msg_SetParamSystem has them; colours, speeds, waits and the other fonts' glyphs dropped.
- The window is Forth (events/play.fs): a box over the lower screen, Enter turns the page, the
  arrows pick an option, Enter answers (the option's message follows, or the window closes);
  `answer?`, `message-param-room`, `message-close`, `message-closed?`, `wait-message`.
- `key-hold` now also makes `key-pressed?` true on the tick a held key goes down (scripted
  tests can press keys).

## Status (2026-10-07): doors and nav groups

- The progress keeps the 400 doors' states and the closed-off doors. Bit 3 of a door's state is
  its LOCK: the decomp's Progress_UnlockDoor / LockDoor / DoorUnlocked / ExitUnlocked are named
  the wrong way round (a script uses a key, then clears bit 3), and tools/event_opcodes.py had
  the inversion (59 04 is door-lock, 59 05 door-unlock, condition 0B door-locked?): fixed there.
  To backport to src as names / comments.
- exit-check, exit-usable?, exit-door-open?, the door state commands and conditions are real.
  Leaving a room is the scripts' now, as in the game: Space at an exit (if its door isn't
  locked) opens the door and steps Fiona into the doorway; the room's phase 1 sees her there
  (exit-usable?) and takes the exit (exit-check); the door shuts behind.
- The nav mesh takes its flags as the game sets them up (section 16); nav-group (section 14's
  triangle groups), nav-tri-flags (was named nav-group-2: it is one triangle, NavGroups_SetTri)
  and char-in-nav-group? change and read them; Fiona and Hewie walk with the game's blocking
  masks (0x28020018 / 0x29020008).
- door-bits sets which parts of the room's door models are drawn (its middle operand is set /
  clear, not a door): kept for when new-src draws door models.

## Status (2026-10-07): sounds, moves, fades

- sound / char-sound / sound-set play the game's sounds from banks 4 / 5 / 6 (distance and pan
  approximated; char-sound's last operand is a bank).
- Characters' scripted moves (the state in the C characters: move, done, animation, target,
  heading): animations (moves 7 / 8 / 16: self-anim, self-anim-blend, self-move-16), walking /
  running to a point (5 / 10: self-move-to, straight over the nav mesh, then facing), turning
  (self-turn-to-xz), idling; self-wait-anim waits at least a frame (the character takes its move
  on its own update), self-wait-done until the move is done. Placing: char-to-xz (the triangle's
  height), char-to-tri, char-to-tri-facing; char-visible; char-busy?. While scripted, the player
  and Hewie's own control wait.
- The screen fade (fade, wait-fade, fade-finish, fade-over, fading?): black over the picture,
  the message window above.
- Room $00's action 0 (its opening scene) runs: Fiona turns, looks up, says her line.

Next: walking to triangles (moves 6 / 11), looking at things (12 / 13), cutscenes and movies, door
models; the window's look.
