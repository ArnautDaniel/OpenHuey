# Acoustics: noises and who hears them

## Purpose

Characters make noises (footsteps, screams, barks, doors, a dropped thing). Stalkers hear them
and come to look. This subsystem decides, each frame, who heard which noise.

## API

Declared in `scripts/messages.fs`.

| Message | Stack | To | Meaning |
|---|---|---|---|
| `noise` | `( loud room tri door source -- )` | acoustics | A noise this frame. `loud` 1..255; `room`; at nav triangle `tri` (-1 at a door), or at `door` (-1: not a door); `source` its kind (below). |
| `listen` | `( threshold source -- )` | acoustics | The sender wants to hear noises louder than `threshold`, never its own `source`'s. |
| `stop-listening` | `( -- )` | acoustics | No more `heard`. |
| `heard` | `( loud room tri door source -- )` | a listener | What it heard this frame (sent at the frame's end). |
| `heard-nothing` | `( -- )` | a listener | Nothing this frame. |
| `noise-setting` | `( n -- )` | acoustics | The story's setting, 0..3: quiet noises are quieter (event 0x91). |

**Sources**, in the order they are listened to: `fiona-noise` 0, `hewie-noise` 1,
`stalker-noise` 2, `world-noise` 3 (the story, creatures, things). A door's noise is its
opener's source.

**Facts read:** the listener's *body* (room, position), nav triangle centres, where a door
stands in the played room (`exit-stand`), the room graph and routes through it (`route`), door
states (`door-open?`), the played room.

## State

- per source: the loudest noise this frame (loud, room, tri, door);
- the listeners: actor id, threshold, own source, in the order they asked.

## Rules

From `Noise_Make`, `Character_Hearing` (`src/game/actor.c`), `Progress_PursuerRequest`.

1. **Making a noise.** It is ignored if `loud` is 0 or `room` is -1, or if a louder noise of the
   same source was already made this frame. A noise at a door has no triangle.
2. **When.** Listeners hear at the end of the frame, from all the noises made in it. Then the
   noises are forgotten.
3. **Which noise.** The sources are tried in order 0..3, skipping the listener's own. The first
   one still louder than the listener's threshold, once weakened (4-6), is heard. Its original
   loudness is passed on. (The order, not the loudness, decides: a footstep of Fiona's beats a
   louder bark from Hewie.)
4. **The setting.** A noise below 0x80 is weakened by 0x1F, 0x3F or 0x5F under setting 1, 2 or 3.
   Below 1 it isn't heard at all.
5. **Reaching the listener.** A noise reaches a listener directly if it is in the listener's
   room, or if it is at a door between the noise's room and the listener's room (a door with
   no exit in the noise's room counts too, *quirk*). Then, *only if the listener is in the
   played room*, it is weakened by a tenth of the distance (the level distance plus three
   times the height difference, truncated). The distance is measured from the noise's triangle
   centre, else from where its door stands in that room, else from the listener itself.
6. **Through the house.** Otherwise the route from the listener's room to the noise's room
   counts (through doors not locked, closed off or one-way against it, at most 2 doors; a door
   noise doesn't count its own door when the route ends at it):
   - no such route: heard only if at least 0x60;
   - weaker than 32 a door: not heard;
   - below 0x41, and the last door on the route is shut: not heard.
   (No distance weakening here.)
7. *Quirk:* in the original, the exit found for a door noise in step 5 stays remembered across
   the sources tried in that frame, and is the one compared in step 6.

## Design notes

- The original wrote the heard noise into the listener (`heardSlot`, `heard`). Here it is a
  message, and the listener keeps what it needs.
- Every listener gets `heard` or `heard-nothing` each frame. Stalkers act on "nothing heard"
  too: they forget what drew them.
- The original also kept last frame's noises for the summoner (`+0x10D4`). When the summoner is
  built, acoustics will send it a `noises-last-frame` message, or the summoner will listen.

## Status

**Built** (`scripts/acoustics.fs`, the messages in `scripts/messages.fs`). **Rules checked** by
`tests/acoustics/test_hearing.fs` (28 tests, in the front garden: distance, order of
sources, own source, loudest per source, setting, threshold, one door shut / open / a doorway,
beyond two doors, a door's noise, no body).

The C facts it reads, added for it: bodies (`body-place`, `body-room`, `body-pos`...: the
kernel), `route` / `route-door` (world.c `world_route`, RoutePlanner's search), `door-open?`,
`door-exit-in`, `door-leads`, `exit-stand`.

Left: nobody makes noises yet. Fiona, Hewie, the stalkers, doors and the story send `noise`
once they are built. The summoner's copy of last frame's noises waits for the summoner.
