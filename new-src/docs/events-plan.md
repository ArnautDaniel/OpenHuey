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

Next: the words, a script runner (a task per action slot, `goto` restarting it, the phases
called from the room loop), then the room words.
