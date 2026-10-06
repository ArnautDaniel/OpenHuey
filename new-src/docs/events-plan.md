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

- Vocabulary: a general scripting vocabulary that hides the game's quirks (`door-open`, not
  setting a door's flag bit), rather than the decomp's own terms. The decomp's names belong in
  comments, so each word can still be checked against the original.

## Questions for you

- The vocabulary: should the words read like the game's terms (`flag-set`, `exit-in`) or a
  more general scripting vocabulary?
- One file per room (as above), or grouped by area of the castle?
- Should the converter live in `new-src/tools/` (C or Python), or in Forth itself?
