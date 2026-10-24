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
| `set-door-opened` | `( exit open -- )` | doors | A room's script sets it swinging (`Doors_SetOpened`): open with a loud creak (0x91, heard 0x5F from Fiona's source) or shut, by 15 degrees a frame; its passage moves over at once; its state isn't changed (nobody holds it). |
| `slam` | `( exit source -- )` | doors | Slammed shut (15 a frame, a loud latch); taken hold of first, let go at rest. |
| `barge` | `( exit source -- )` | doors | Burst open (15 a frame); taken hold of first, let go at rest (Fiona_SlamDoor). |
| `pull-shut` | `( exit source -- )` | doors | Taken hold of and swung shut by 5 degrees a frame, let go shut at rest: a stalker's door behind him (his DoorBreak: DoorHold_Take, DoorHold_Shut). |
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
   Only a door with a nav triangle (section 7's first word, not -1) is drawn or swung
   (`Doors_TakeRoom` +0x70: water-tower-7f-1's exit 0 has a model but none).
10. A room's script setting a door opened (`Doors_SetOpened`, room 0x91's cmd00): not while it
   swings along an animation; open - a loud creak 0x91 at once (0x5F from the opener 0, Fiona's
   source), the passage flags $60000 off the near side and on the far one, then a slam's swing
   (15 a frame) to -90; shut - the flags back, the swing to 0 and a slam's loud latch. The door's
   state word is untouched: at rest nobody holds it, so letting go changes nothing.
11. *To read when built:* slamming, held doors and who may pass, the "locked" message and keys
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
reports each frame's sound and rest). Checked by `tests/doors/test_doors.fs` (39 tests: locking;
the locked-door quirk; holding once; letting go open / shut / not held; locked against a kind; a
doorway; a door used off-screen heard at 0xF from its user's source; a held door swung open in
the played room settles open; a slam's latch heard at 0x5F and the door left shut; set opened by
a room's script - 15 a frame, the passage at once, the creak and latch heard at 0x5F; a door
without a nav triangle not swung).

Also built: the open door's light (below), checked by `tests/graphics/test_graphics.fs` (its
shadow: none shut, castle-1f-1's door 0 from the moon at 18) and `tests/doors/test_doors.fs` (its
blockers: two past 78.75 degrees, none at 70; a point behind the leaf from light 4 shaded, one
beside it or before it not; a room script's blocker lasting its frame).

### The open door's light

Two things in the original, both read from `lights.c` / `doors.c` (Door_Draw) and hg's `glr.c`:

1. **Its shadow** (`DoorShadow_Queue`, `DoorShadow_FromLight`, `DoorShadow_Draw`; layer 6). A door
   shown, its leaf's part shown (`PlacedMesh_LastShown`) and swung open at all, where a shadow may
   fall (the eye above its nav triangle's plane), for each of up to 3 lights at it
   (`Lights_ShadowLights`): the leaf (12 x 22 x 1, `D_003EC080`) face away from the light (5 above
   it) is swept away from the light by 3/4 of the light's shadows' reach into a box. Its strength
   is `4 cos^2 x falloff x intensity x (64 + the ambient's mean)`, at most $80 (falloff
   `((range^2 - d^2) / range^2)^4` at the face's top middle; out of reach, none). None if a corner
   is outside the PS2's clip volume. The box's four sides, those facing the eye, are drawn into the
   half-size shadow buffer against the scene's depth so far: gouraud strips, the door's two corners
   grey $80 at the strength, the far two nothing (so it falls off as the square), each blended over
   the buffer. At layer 6's end the buffer is blurred (each texel plus 1/8 of its 8 neighbours,
   kept to $FF) and taken off the screen at half on its display values. The characters (layer $A)
   come after it: the door's shadow darkens the room and the door, not them.
   OpenHuey: `room_door_shades` (room.c) and `render_shade_*` / `post_shade_*` (a half-size
   target, the same blur and subtraction), drawn as layer 6 begins (`main.c draw_door_shades`), with
   `gfx.shadow-maps`. One difference from hg: glr draws the strips' grey at $FF (its mesh shader
   scales vertex colours by 255/128); the GS writes $80, as here.
2. **Blocking the lights** (`door_portal`, `Lights_AddDoorway`; `Shadow_BlockerVolumes`). Past
   -78.75 degrees the leaf's two faces, when wholly in view, are added to the lights' blockers for
   the frame (as the room scripts' 0x66 quads - walls, ceilings - are). Each character shadow's
   volume count is then made against the blockers facing away from its light: their volumes
   (behind the quad, away from the light, the light's shadows' reach long - and again 1/5 of it)
   count the other way, so **no character's shadow falls where the door (or wall) already shades
   that light**. (Worked out from the windings: the bone boxes' faces and the leaf's are both wound
   outward, and the character's volume must count +1 to show; the near cap - the leaf itself - is
   left out, being solid.) OpenHuey's shadow maps take only the characters as casters, so the
   room's walls and doors don't stop them by themselves: the frame's blockers (`room_door_blockers`,
   `light-blocker`, max 16) go to the scene's shader, which leaves shadow i out at a point inside a
   blocker's volume from shadow i's light (`room_blocked` is the same test in C).
   Not reproduced, on purpose: the original drops a character's whole shadow from a light when a
   blocker's far corners are out of view (the volumes can't be drawn clipped - its own bone boxes
   have the same all-or-nothing); the second, shorter volume (it only matters where two of the
   character's boxes overlap, a count of 2).

Fiona uses doors (docs/subsystems/fiona.md): she holds a door, opens or shuts it by hand along
her animation, and lets it go as she steps out.

Used by the stalker in her room too (`stalker/doorway.fs`: his door animation with `use-door`),
and slammed or burst open by Fiona (`fiona/slam.fs`). Also built: the event
commands' door bits (the story's `door-lock`, `door-close-off-lock`, `door-lock-for`,
`room-doors-state`), `Doors_SetOpened` (`set-door-opened`), a stalker breaking a door open and
pulling it shut behind him (his DoorBreak: `stalker/doorway.fs` `shut-behind`). Left: names for
doors.
