# The summoner

## Purpose

Brings the stalker into play from off stage and takes him back out. While the story allows it
(state flag `summoner-on`), a stalker who is loaded but not in the game comes in after a random
time away, or sooner when Fiona makes enough noise; he comes in out of sight, a room or two
from hers, and heads for her or waits about. In play, he is taken back out once he has stopped
hunting her for long enough, away from her room. The story also takes him out, and sets how
long before he can come back (the cooldown: the items that keep him away).

The original: `src/game/pursuer.c` (`Summoner_*`, `summon_via`, `summoner_take`); the object
is SceneGame +0x7A4 (progress +0x764). It runs at the start of play in a room and each frame
(SceneGame vtable +0xDC, `SceneGame_PlayStart`), and on the frame's noise
(`Progress_PursuerRequest` -> `Summoner_Noise`).

One actor, `summoner` (`scripts/summoner.fs`).

## API

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `stalker-status` | `( room mode in-game own away cool -- )` | in, from the stalker in slot 2, each frame while loaded | Where he is, his mode (0..4), whether he is in the game, whether he is in one of his own rooms (+0x314: Debilitas 0x20, 0x02; Daniella none), his time-away base (frames) and cooldown (seconds). |
| `noise-made` | `( loud room door -- )` | in, broadcast by acoustics at the frame's end | Fiona's loudest noise this frame (the original's pursuer request: source 0). |
| `entered-room` | `( room exit -- )` | in (broadcast) | Fiona's room and the exit she came in by. |
| `danger` | `( level -- )` | in (broadcast) | The game mode (1: followed). |
| `summoner-take` | `( kind -- )` | in, from the story (0x84, 0x14) | Take the stalker out now. |
| `summoner-cooldown` | `( secs -- )` | in, the story (0x5A, 0x5B) | Not before `secs` seconds. |
| `summoner-cooldown-less` | `( secs -- )` | in, the story (0x5A) | That much sooner. |
| `summoner-reset` | `( -- )` | in, the story (0x11: a stalker loaded) | Its state afresh. |
| `write-save` | `( -- )` | in (broadcast by saves) | Prints its record (`summoner-was`): kept in a save as the original's progress +0x764 is. |
| `summoner-restore` | `( away called cooldown room waited mode kind -- )` | in, from saves (a save loaded, after the stalker's `summoner-reset`) | Its record as the save had it. |
| `stalker-summoned` | `( room exit found plan -- )` | out, to the stalker | Come in out of sight in `room`, by `exit`; `found` (called by a noise): after her; else waiting about, `plan` heading for her. |
| `stalker-out` | `( -- )` | out, to the stalker | Taken back out of the game. |
| `danger-signal` | `( 6 -- )` | out, to danger | Hunted (a noise heard off stage). |

## Rules (pursuer.c)

**State** (+0 .. +0x11): his time away (frames; bit 31: called by a noise), the cooldown
(frames), the room he was taken from, the frames waited, his last mode, the kind of the last
taking (0 by the story or a wait, 1 held off, 2 beaten).

**Off stage** (Summoner_Offstage; `summoner-on` set, the stalker loaded and not in the game):
the first time (`summoner-timing` clear) his time away is rolled - his base (+0x16E0: Debilitas 4500; Daniella 3600, hard 6000; her later
stages' numbers not yet) plus 0..3600 frames - and `summoner-timing` set. While
`summoner-waiting` is clear and time is left, it counts down (and the wait up). Then he tries to
come in by one of three ways, from a random first one on: next door with a plan (Via1B), next
door (Via1), further off (Via0). Come in: the wait restarts, his mode noted, `summoner-waiting`
cleared and the cooldown set to his own (+0x2D4: his +0x16E4 seconds - Debilitas 90; Daniella
140, hard 100).

**A way in** (summon_via): the rooms within two open doors of Fiona's (SceneGame_RoomsNear, `rooms-near` in doors.fs -
shared with the creatures:
open - not locked, passable from all three sides), not hers; `near` 0: those not next to hers;
else those next to hers (once per exit that leads there) with an open exit elsewhere
(Rooms_NeighbourCount); one at random; its first exit to another room with a route from the
half of Fiona's room she came in by to that exit's half (`route-sides`: walkers' one-way flags
checked; at most 1 door next door, 2 further off), next door not through an exit onto half 0
or 1. He comes in there (Pursuer_IntoRoomByEvent): called by a noise - after her (mode 1,
heading for her room); else waiting about (mode 3) and, with a plan, heading for her, without
one off by an exit at random.

**In play** (Summoner_InPlay): the cooldown runs down. His mode changed: the wait restarts.
Fiona in his room, or he next to hers where he isn't hunting her (not mode 4, or not in his own
rooms): likewise. Else the wait counts up; after 5 s beaten (knocked out of health: kind 2), after 5 s held
off (mode 4: kind 1), or with the
cooldown over and 3 s waiting about (mode 3: kind 0), he is taken.

**At the start of play in a room** (Summoner_RoomStart), with a stalker loaded, not while the
world is held (state flag $17), not ($18) with him away from her room: in a room every exit of
which is closed to stalkers - unless he is in hers, he is taken (kind 1) when held off in one of
his own rooms; else, unless an event drives the camera, off stage or in play as above.

**Taken** (summoner_take): if he is in the game, the room noted, his time away rolled afresh,
the wait restarted, the kind kept; he is out of the game (Progress_CharDone).

**Noise** (Summoner_Noise): Fiona's loudest noise this frame, in her room only. Its level by
loudness: $80 up 4, $60 3, $40 2, $20 1, else 0 - one less (not below 0) away from a door with
every door of the room shut - at most 3. Its chances (Pursuer_NoiseChances, $47AC90): to call
him (0, 5, 25, 100 percent) and of the hunt (0, 10, 50, 0). With a stalker loaded and off stage:
the call rolled, and it counts with `summoner-on`, not `summoner-waiting`, 3 s waited and the
last taking's wait (D_00419DC0: 0, 10, 30, 30 s by kind) - then his time away is over (bit 31:
called). Otherwise the hunt's chance of the rest (out of 100 - the call's) sets hunted (danger
signal 6), as does any chance at all while the danger is 1 (followed).

## Not modelled

- Order within a frame: the original handles the frame's noise before the summoner's frame; here
  acoustics tells it at the frame's end, so a noise's call brings him in a frame later.
- The progress' second room bit set (+0x100), which the way in requires clear: nothing in the
  decompilation writes it; taken as clear.
- The Hewie-controlled mode (+0x1FBEC1; docs/subsystems/hewie.md): its summoner's ways are left
  with that mode - taken as off.
- The camera driven by an event (`cam-event?`): the camera actor sets it (`cam-event!`) while an
  event's shot has the camera (the story's 0x82) - no summons meanwhile.

## Status

Built: `scripts/summoner.fs`; the stalker's status and summons (`stalker.fs`), the story's
words (0x11, 0x14, 0x5A, 0x5B, 0x84), acoustics' `noise-made`. Tests:
`tests/summoner/test_summoner.fs` (16): a noise's level, the rooms near hers, off stage (time
away rolled and counted down), coming in out of sight and waiting about, taken by the story,
the cooldowns, a loud noise calling him in after her, held off and beaten in play. Not tested yet: the
hunt's chance (random), a room closed to stalkers.
