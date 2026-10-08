# Doors

## Purpose

About 400 doors and doorways join the rooms. A door can be locked (for everyone, or for some
kinds of character, from one side or both), shut or open, held by whoever is going through,
and closed off for good by the story. In the played room a door is also a model that swings,
in step with the animation of whoever opens it, and that makes noise.

## API

Declared in `scripts/messages.fs`. Kinds of character (for locks): `fiona-kind` 0,
`hewie-kind` 1, `stalker-kind` 2. A `source` is the user's noise source (acoustics).

| Message | Stack | To | Meaning |
|---|---|---|---|
| `lock` / `unlock` | `( door -- )` | doors | For everyone (the story, keys). |
| `lock-for` | `( door kind on -- )` | doors | Locked against a kind of character, or not (`Progress_LockDoorFor`). |
| `close-off` | `( door -- )` | doors | Gone for good: no route goes through it. |
| `hold-door` | `( room exit kind -- )` | doors | Take hold of a door before going through (any room). Answered `door-held` or `door-refused` `( room exit -- )`. |
| `let-go-open` / `let-go-shut` | `( room exit source -- )` | doors | Let it go open or shut (off-screen: heard). |
| `use-door` | `( exit anim source -- )` | doors | The played room's door swings along the user's animation; at rest it is let go. |
| `swing-door` | `( exit open source -- )` | doors | Swung open / shut by 5 degrees a frame. |
| `slam` | `( exit source -- )` | doors | Slammed shut (15 a frame, a loud latch). |
| `door-changed` | `( door -- )` | broadcast | Its state changed. |
| `noise` | | acoustics | The doors are heard. |

**Facts** (C; changed only here): each door's state word (`progress pr.doors`: held 1, open 2,
stuck 4, locked 8, the kinds it's locked against in bits 4..7), closed off
(`pr.closed-off`), `door-open?`, `door-locked?` (doors.fs), a doorway (`door-flags` bit 0).
In the played room, by exit: `door-here?`, `exit-stand`, `door-events` (this frame's sound,
its loudness, at rest), the passage on the floor (`door-passage`).

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
6. Holding (`DoorHold_Take`): refused for a doorway, a door locked against the holder's kind,
   or one already held. *Quirk:* a door locked for everyone **can** be held, but letting it go
   open fails and it stays held. Users check `door-locked?` first, as Fiona's "it's locked" does.
7. Letting go (`DoorHold_Open` / `DoorHold_Shut`): only a held, unlocked, non-doorway door;
   open also fails if it's stuck (bit 2). Either way it's no longer held. Off-screen, the door is
   heard (0xF at the door, from the user's source); in the played room its own sounds are.
8. In the played room a door model swings (`Door_Swing`): along its user's animation, by 5
   degrees a frame, or slammed by 15. The creak (0x27) as it starts to open along an animation
   and the latch (0x28) as it shuts past -6 degrees are heard at 0xF. A slam's latch is heard at
   0x5F, and a plain swing's latch is silent. When it comes to rest, the door is let go open or
   shut by its user.
9. *To read when built:* slamming, held doors and who may pass, the "locked" message and keys
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

**Built** (`scripts/doors.fs`; the swing in C `room.c` now follows `Door_Swing`'s three modes and
reports each frame's sound and rest). Checked by `tests/doors/test_doors.fs` (25 tests: locking;
the locked-door quirk; holding once; letting go open / shut / not held; locked against a kind; a
doorway; a door used off-screen heard at 0xF from its user's source; a held door swung open in
the played room settles open; a slam's latch heard at 0x5F and the door left shut).

Fiona uses doors (docs/subsystems/fiona.md): she holds a door, opens or shuts it by hand along
her animation, and lets it go as she steps out.

Left: the event commands' door bits (with the story); `use-door` with Fiona's animations; a door
used off-screen with no one to hear it; barging a door open (`Doors_SetOpened`, a loud 0x91);
the stalkers' knocking and breaking; names for doors.
