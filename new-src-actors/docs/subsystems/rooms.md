# Rooms: the room being played, and going from room to room

## Purpose

The house is about 140 rooms joined by doors and doorways. One room is played at a time (it is
drawn, and its characters act fully); the others still exist (characters elsewhere move
through them off-screen). This subsystem owns which room is played and the change from one to
the next when the controlled character goes through an exit.

## API

| Message | Stack | To | Meaning |
|---|---|---|---|
| `go-through` | `( exit -- )` | rooms | The sender (the controlled character) has gone out by this exit of the played room. |
| `go-to-room` | `( room exit -- )` | rooms | Play `room`, arriving by `exit` (-1: no exit - a new game, a jump, the debug console). |
| `leaving-room` | `( room exit -- )` | broadcast | The played room is being left by `exit` (the story's phase 4). |
| `arrived` | `( room exit -- )` | broadcast | The new room is loaded and its characters arrive by `exit`, the one on this side (the story's phase 5; each character puts itself at that exit if it came with the player). |
| `entered-room` | `( room exit -- )` | broadcast | The room is in play: the camera starts on it, the story runs its entry (phase 0). |

**Facts:** `room-id` (the played room), the room graph (`room-exit-leads`, `route`...), whether
a room exists, its exits' spots (`exit-spot`, `exit-stand`).

Names: rooms are named in `scripts/room-names.fs` (`front-garden-2`).

## State

The played room; the exit it was entered by; a change in progress.

## Rules

From `SceneGame_LeaveRoom`, the play sub-state (`scene_game.c` ~1033-1095), `SceneGame_RoomIn`,
`SceneGame_EnterRoom`.

1. Only the controlled character's going out changes the played room. Others going out of it
   just go (their own off-screen travel).
2. Leaving by exit E: the room behind E is the next. The exit on its side (`rooms +0x14`, the
   other side's exit) is the arrival exit, which the next room's scripts read as "the exit
   taken".
3. The order of a change:
   1. `leaving-room` (the scripts' phase 4);
   2. the next room loads (synchronous here: the original kept running phase 3 while it
      streamed in);
   3. `arrived` (phase 5); every active character is told the arrival exit and forgets its
      camera setup;
   4. the camera's new room (no set, no path, no target);
   5. `entered-room`: sounds, then the scripts' phase 0. Then the characters in it enter, and
      the camera takes the room's sets and paths.
4. The room's sounds (bank 6) are swapped on leaving and entering, unless story flag 0x27
   says to keep them (*to name*).

## Design notes

- The original loaded the next room into a second slot ahead of time (`exit-prepare`, event
  0x01). We load at once; the two-slot streaming is not needed on PC.
- The fade is not part of the change in the original: the door animation and the scripts do
  it. It is the same here.
- Room changes the story makes (a cutscene moving Fiona somewhere) are `go-to-room`.

## Status

Spec.
