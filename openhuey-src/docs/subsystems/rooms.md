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
4. The room's sounds (bank 6) are swapped on leaving and entering, unless `keep-room-sounds`
   says to keep them (*to name*).
5. A forced exit ($80 and up: the scripts' cmd_exit with bit 7) goes to the room loaded ahead
   into the spare slot - `room-preload` (event 0x3A) or `exit-prepare` (0x01: the room behind an
   exit) - arriving by that same number (SceneGame_LeaveRoom). The room left stays in the spare
   slot (the original's two slots swap), so a forced exit with nothing else loaded ahead goes back
   to it; with no room left either (a new game, a jump) it takes exit 0's door.

## Drawing

The room draws in the original's renderer layers, in order with the characters' (theirs $A;
`main.c` `draw_layer`, `room.c` `room_draw_layer`; `RoomMgr_Draw`, `Doors_Draw`,
`PlacedObjects_Draw`):

| Layer | What |
|---|---|
| 1 | the solid part (0), the doors, the plain placed objects |
| $19 (8 with the fog drawn early) | the see-through part (1), the flip books (4); "a_" objects always $19 |
| $1F | the glows (part 5, no depth writes) |
| $26 | the bloom mask (part 2) and the "g_" objects: they mark the mask only, no colour |

Part 3 is never drawn. Characters behind glass are drawn before it, so its depth writes don't
hide them. Room, door and placed draws drop a triangle with a corner outside the PS2's clip
volume, as their microprograms do (`D_003A7530` and the placed kinds': CLIP, FCAND, ADC -
`render_mesh_ps2_clip`).

The room's lights (section 4): without one, no lights and the ambient 128 (`Lights_Reset`). A
character off the nav mesh takes light 0 alone, wherever it reaches (`Lights_ForModel`).

## Design notes

- The original loaded the next room into a second slot ahead of time (`exit-prepare`, event
  0x01). We load at once; the two-slot streaming is not needed on PC.
- The fade is not part of the change in the original: the door animation and the scripts do
  it. It is the same here.
- Room changes the story makes (a cutscene moving Fiona somewhere) are `go-to-room`.

## Status

**Built** (`scripts/rooms.fs`). Checked by `tests/rooms/test_rooms.fs` (21 tests: a new game in
the cage room; out by an exit - leaving / arrived / entered in order, with both exits; back
again; an exit to nowhere; a jump; the fog drawn early by its 26 bytes, past its section; no
lights section; light 0 off the mesh). The placed objects' layers: `tests/graphics`. Listeners subscribe to `leaving-room`, `arrived` and
`entered-room`.

The rooms' own sounds come with their scripts (`room-sounds`). The sections where Hewie is the
one played (the mini-game: `hewie-controlled?` is `hewie-played?`) are built:
`docs/subsystems/hewie.md`, `docs/subsystems/story.md`.
