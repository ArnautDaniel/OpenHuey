# Doors

## Purpose

About 400 doors and doorways join the rooms. A door can be locked (for everyone, or for some
kinds of character, from one side or both), shut or open, held by whoever is going through,
and closed off for good by the story. In the played room a door is also a model that swings,
in step with the animation of whoever opens it, and that makes noise.

## API

| Message | Stack | To | Meaning |
|---|---|---|---|
| `lock` / `unlock` | `( door -- )` | doors | For everyone (story, keys). |
| `lock-for` | `( door kind on -- )` | doors | For a kind of character: 0 Fiona, 1 Hewie, 2-5 the stalkers (`Progress_LockDoorFor`). |
| `close-off` | `( door -- )` | doors | Gone for good: no route goes through it. |
| `use-door` | `( exit anim -- )` | doors | The sender opens / goes through the played room's door at `exit` with its animation `anim`: the door swings along it. |
| `hold-door` / `let-go` | `( exit -- )` | doors | The sender holds it open / lets it go (it swings shut, or stays). |
| `slam` | `( exit -- )` | doors | Shut hard on whoever is behind (later: hits them). |
| `door-state` | `( door state -- )` | broadcast | Its state changed (open, shut, locked). |
| `noise` | | acoustics | Doors are heard: opening or shutting, from the user's source. |

**Facts** (C, read by anyone, changed only here): each door's lock bits and sides, open,
closed off (`door-open?`, `door-locked?`, `door-closed-off?`); in the played room, each exit's
door model: where it stands (`exit-stand`), its swing, its sides' nav triangles, which side a
point is on, whether it blocks the floor (the passage).

## State

Each door's state word (the original's `+0x124 + door x 4`: held, open, passable, locked, the
kinds it is locked against). The played room's doors: who uses / holds each.

## Rules

From `doors.c` (`Doors_*`), `progress.c` (`Progress_*Door*`), `room_map.c` (closed off).

1. A shut door blocks its passage: the nav triangles on its two sides are cut off from each
   other (`Doors_Passage`). Open, they join. A doorway is always open.
2. A door is passable for a kind unless locked against it (`door_passable`: 0 Fiona, 1 Hewie,
   2-5 stalkers each a bit).
3. Using a door: the door swings along the user's animation (the door records in
   `O_FIN/FIN_D000.MTN`) and the user is put on the door's spot for that animation
   (`Doors_AnimUserSpot`). At the end it is left open or shut by the animation.
4. A door's creak / latch sounds play once per swing, and are heard as noise 0xF from the
   user's source (`door_heard`).
5. When a room is entered, its doors are set from their states: open ones open, shut ones
   shut (`Doors_RoomIn`). The swing angle of each door is kept when left (`Doors_SaveDoor`).
6. *To read when built:* slamming, held doors and who may pass, the "locked" message and keys
   (Fiona's side), the stalkers' knocking and breaking.

## Design notes

- The lock state lives in C because routes through the house read it. Only this actor writes
  it, through words that also refresh the played room's passages.
- The door's user drives the animation (Fiona plays her door motion and sends `use-door`). The
  doors actor only swings the door, so the two stay in step the way the original's do (the door
  reads the user's motion frame).
- Door ids get names as rooms do (`front-garden-2-exit-0`, until they're named for what they
  are).

## Status

Spec.
