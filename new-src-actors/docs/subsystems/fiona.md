# Fiona

## Purpose

The player. She walks and runs where the stick points (relative to the camera), opens doors,
climbs ladders, pushes things, examines and uses things, commands Hewie, and defends herself
with a kick or a shove. Fear builds as she is chased or frightened and turns to panic: the
picture shakes, she won't obey well, she runs blindly and stumbles. Stalkers knock her down,
seize her and drag her off. The original's `fiona.c` is ~12,000 lines (~190 functions). She is
built in phases.

## Phases

| Phase | What | Needs |
|---|---|---|
| **F1** | Her body and model; the controls; standing, walking, running, turning, resting, idle looks; root motion on the floor; footsteps (sound and noise); going through exits and doors (her door animations, locked doors); the camera follows her | rooms, doors, camera, acoustics |
| F2 | Fear and panic: the meter, calming down, the panic stages, panic runs and stumbles, exhaustion, recovery | danger |
| F3 | Hewie: calling, gestures, commands, praise and scolding | hewie |
| F4 | Defending: kick, shove (F4c, done), strike, slamming doors on someone (F4d, done). Reactions: knocked down, thrown, caught (F4a, done), grabbed, led away, dragged, shaking free (F4b, done), falls | stalkers |
| F5 | The story's moves (walk to a spot, a path, turn, an animation, hold), ladders, pushing, points of interest, items | story, items |

The debug walker (`scripts/walker.fs`) stood in for her until F1 was done.

## API (F1)

Declared in `scripts/messages.fs`.

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `arrived` | `( room exit -- )` | in (rooms broadcast) | Put herself at the arrival exit (or the room's start). |
| `door-held` / `door-refused` | `( room exit -- )` | in (doors) | The answer to her taking hold of a door. |
| `go-through` | `( exit -- )` | to rooms | She went out by an exit. |
| `follow` | `( who -- )` | to camera | Follow her (when she becomes the one controlled). |
| `hold-door`, `use-door`, `let-go-open`, `let-go-shut` | | to doors | At a door. |
| `noise` | | to acoustics | Her steps (and later her screams, falls). Her source: `fiona-noise`. |
| `fiona-doing` | `( mode sub cond cmd -- )` | broadcast at each frame's end | What she is doing as the frame left her, and her command (the panic and Hewie listen). |
| `hewie-doing` | | in (Hewie's broadcast) | What he does: she chooses her commands by it (F3). |
| `command`, `reaction`, `meet-me`, `meet-now`, `meet-off` | | to Hewie | Her commands, their effect on him, the meeting by his side (F3; docs/subsystems/hewie.md). |
| `meet-at`, `meet-on`, `meet-refused` | | in (Hewie) | His answers. |

**Facts she reads:** the controls (keys / pad: the input is a fact, read each frame); her
model's motion (root motion, the motion's event flags, its end); the floor (nav triangles and
their flags, `nav-move`); the camera's yaw and whether it just cut; doors (`door-here?`,
`door-locked?`, the door's spot for her animation).

## State (F1)

Her model; her body (room, triangle, position, heading); what she's doing (her state word and
action); the controls' memory (the stick's last direction, the camera turn the controls are
locked to after a cut, frames still); her heading-to-be; resting and running timers; the door
she is using.

## Rules (F1)

From `Fiona_MoveInput`, `Fiona_StateIdleMove`, `Fiona_StateTurnStanding`,
`Fiona_StateTurnOnSpot`, the door states (`Fiona_StateDoorStart` / `DoorWalk` / `DoorAnim` /
`DoorStepOut`, `Fiona_StateLocked*`), `Fiona_Footsteps`, `Fiona_MotionSounds`. Reference:
new-src's port (archived: tag `new-src-final`, `new-src/scripts/fiona/moves.fs`, `doors.fs`, `core.fs`), faithful in
behaviour.

1. **The controls are camera-relative.** Up is where the camera looks. After a camera cut,
   while the stick is held, the old camera keeps steering (for 3 frames exactly, then until
   the stick moves). Then, while the direction stays within 15 degrees, she turns towards the
   old camera's heading, accelerating 0.0013 radians a frame up to 3 degrees. A different
   direction ends it. A tilt under half counts as no stick; 6 frames still counts as let go.
2. **Moving is root motion.** Her walk (0x200), run (0x202) and other animations move her by
   their own root movement, turned by her heading. It is reduced the further the stick points
   from where she faces ((1 + cos of the difference) / 2), and on slopes it is reduced along
   the fall of the floor. She turns towards the stick 10 degrees a frame. Her floor is the nav
   mesh less the triangles her mask blocks (0x28020018). If a step takes her backwards (pushed
   along a wall corner), she stays where she was.
3. **Standing.** Let go of the stick: walking eases to a stop, running to a stop. Standing
   still a while she rests (after 90 frames her resting animation), but only while followed
   (tense), not afraid, and not long recovered. Otherwise she plays her idle.
4. **Running** needs the run button, and `no-running` not set.
5. **Footsteps** follow her animation's foot contacts. Each plays its sound (by the floor's
   material) and is heard as a noise.
6. **Doors.** With the action button by an exit:
   - a doorway: she just goes on through;
   - a locked door: she tries it (her rattle animation) and gives up;
   - a door: she takes hold of it (`hold-door`), walks to her spot for the door's animation,
     and plays her door animation (0x600 + its kind) as the door swings with her
     (`use-door`). Then she steps out, and the room changes (`go-through`). Quick (no fear,
     not chased) or slow, as her state is.
7. **Arriving** by an exit: at the exit's spot inside the room, facing in, already walking in
   (*to read: `Fiona_Vt34`, `Fiona_PostExit`*).

## Design notes

- Her phases share one actor. Each phase adds behaviours (`become`) and handlers. States the
  original runs as PTMFs are behaviours, or words a behaviour's tick runs.
- The input is a fact (engine words). The controls are hers, so no input message.
- Hewie-controlled play (the original's `+0x1FBEC1`): later, with Hewie.

## Status

**F1 built** (`scripts/fiona.fs`, `scripts/fiona/`: `state` her fields, `model` her animations
and looks, `controls` the stick, `moving` standing / walking / running / root motion /
footsteps, `spots` walking to a spot, `doors` her door use; shared: `scripts/paths.fs`,
`scripts/common.fs`). Checked by `tests/fiona/test_fiona.fs` (17 tests):
- a new game: in the cage room, standing, followed by the camera;
- walking (0x200), running (0x202), stopping;
- footsteps heard (walking 4, running 0x14);
- at a door, the action button: she holds it, plays her door animation (0x600), lets it go
  open; her animation takes her through and the exit is taken;
- a locked door: she tries it (0x609) and gives up, the door never held;
- going out by an exit's area; arriving, not sent straight back.

Exits are the story's now: the rooms' scripts take them (`exit-check`) and place her as she
arrives (`to-exit`: the exit's outside spot, facing in). Her own placement on `arrived` stays
for a room whose scripts don't place her.

The debug walker is gone.

Left for F1: the slow locked-door try (with the panic, F2); the camera-cut steering is built
but untested (a cut needs the story's camera setups); her motion sounds (key-frame sounds) and
voice; keeping clear of others (with Hewie).

**F2 built** (`scripts/fiona/fear.fs`, with `scripts/danger.fs` and `scripts/panic.fs`):
- her fear meter, rising as she runs and falling as she walks or stands (faster when calm);
- out of breath from the panic's stage 2;
- panicking at 4, with a scream heard (0x6F) and the panic run (0x206) until out of breath (0x207);
- stumbling (0x1001) as she runs into something, and one time in five a fall (0xB00): she
  slides along the wall (0xB01) and gets up (0xB02) once the panic has passed;
- the panic attack at full fear (150 frames);
- turning to flee (Q: 0x403, fear up 10);
- her looks follow the danger and the panic (the chased walk 0x208, the shaken and frightened
  blends).

Found with Hewie's tests: placing her on front-garden-2's exit 0 spot (`0 0 exit-spot`) right
after the room is entered puts her outside the exit's area (the spot is 64 above her floor), so
the stand-in exit check doesn't fire; Fiona's own test reaches it after the door tests. To look at.

Left for F2: the slow locked-door try; shaking the panic off faster while down (with the
shake-free of F4); the flee's door slam and Hewie's reaction; charms (with items).

**F3 built** (`scripts/fiona/commands.fs`, with Hewie's H2). The right stick's gestures (keys 1
up, 2 down, 3 R3, 4 right - held for more praise, 5 left), read under `hewie-commandable` and not
`fiona-occupied` (Fiona_ReadsPad) and acted on the next frame. The code by what Hewie is doing and where
(Fiona_HewieCommandAction): go there / go for it, come back, stay / come, praise, scold (from
afar or close up). Her gesture (mode 0xD: 0xC00..0xC0F), her lines on its key frames
(Fiona_MotionSounds' event 1; Fiona_CallHewie, Fiona_OrderLine), the command on its event 2
(Fiona_CommandHewie: for "go there" the spot 5 short of where he would stop). "Come back" and
"go for it" on the move: no gesture. Panicking or held: a cry for help (0x38). The meeting
(stay / praise / scold by his side): she asks, walks to the place he finds (walk-to-spot),
waits for him to sit, the second part and her gesture (0xC06 stay, 0xC07..0xC09 praise with
its repeats, 0xC0D scold). Refused when asked: from afar instead (praise 0x29, scold 0x2F).

Left for F3: her head turned to Hewie after a command (her looks aren't built); a creature
ahead for "go for it" while followed (with the creatures).

**Fixed (2026-10-07):** her model had no motion table (`$3D5CC0`, CharModel_SecondaryMotion): no
animation looped or blended - the run stopped on its last frame and she slid in that pose.

**To check by eye (with the user):** her heading as she arrives through a door. She was facing
back into the door; now she keeps her heading from going through (as the original: Fiona_Vt34
saves it, the scripts' 0x04 places without turning). Unverified on screen: the screenshot
angles didn't show it clearly. If it is still wrong, the stick-relative turn of Fiona_Vt34
(the old camera's frame) is the next suspect.

**`go-to`** `( x y z -- )` (floats `f>cell`): she walks there over the nav mesh (walk-to-spot)
and stands. At the console: `10e 0e 20e walk` (scripts/debug.fs). For debugging, and the base of the story's
scripted moves.

**F5 begun** (with the story's S3): the story's moves (`scripted-move`: `scripts/fiona/moves.fs`),
`scripted` on / off (her hands off the controls), `hold-anim`, `show`, `moving` each frame;
examining (`offer` / `take-offer`), messages on screen (`text-shown`: she stands).

## F4a: struck and caught (`scripts/fiona/hurt.fs`)

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `hit` | `( kind how fright -- )` | in (a stalker) | A blow or hold from the sender: kind 1 / 2 a blow, 4 a hard one, 6 a hold (how: the grip; 5 thrown off); `$8000` in how a stumble; fright a float cell. |
| `hit-taken` | `( kind -- )` | out, to the sender | It struck home. |
| `fiona-plight` | `( state -- )` | broadcast each frame | How she stands for an attack (Pursuer_FionaState): 0 calm, 1 fleeing, 2 panicking (fear over 90, the panic attack), 3 out of reach (struck, seized), 4 / 5 the panic's stages. |

Fiona_Reaction / Fiona_React: none while a reaction is under way (mode 4, floored $A), on a
ladder (her fall from it: with the ladders) or with the world held. A blow knocks her down -
on a step or in a doorway (floor $80003) the short falls $100E / $100F, else $1000 / $1004 by the
side it came from (front, behind +1, her right +2, her left +3), her cry $3E / $3F; up again, 30
frames before she may fall again. A hard blow throws her: facing him back along her heading
($1008, then up $100A), from behind forwards ($100B, up $100D); panicking she is flung down
($B04) and stays down as from a panic fall ($B01, up when her breath is back); moved by the
motion turned toward him, 20 degrees a frame. A hold catches her: her gasp $43, a flinch by
his grip ($F02 by the arm looking at him - only facing him on open floor -, $F04, $F03 from
behind, $F05), then free. Each takes its fright to her panic; a blow tells the danger she was
struck (`danger-signal` 1); a stumble resets her recovery.
Her costume 1 (the slip) lacks these animations - they come with the story's second motion set
(event $A7); her clothes (costume 0) have them.

## F4b: seized (`scripts/fiona/seized.fs`)

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `seize` | `( type -- )` | in (a stalker) | Taken to the sender's side: 6 by the hand, 8 walking. |
| `seize-taken` / `seize-refused` | | out, to the sender | Her answer. |
| `broke-free` | | out, to him | She shook free. |
| `unhand` | | in | He no longer leads her. |
| `hit` kind 3 | | in | Seized outright. |

Fiona_JointAction kind 1: only while free (mode 0), the world not held; her spot by him is his
place plus kFionaMeetOffsets ($3B2460, by her costume and the type) turned by his heading, at
its heading - reachable straight from him and from her, her floor free ($80001), a way there
(in her own triangle: straight). Led there (her gasp $43): by the hand $1400, walking $1500 - a
fifth of the way and of the turn each step while it fades in; then on the spot. By the hand:
at his hand, then dragged - $1401 again and again; she shakes (Fiona_Shakes: the stick swung
past 120 degrees or out from rest, and each button but Start), each drag letting ten more
count; past her panic's stage's count (8 12 23 35 70, calming 0) she breaks away ($1403, he's
told), else the sixth drag she's dragged off ($1404) - at its end the game's over. Walking:
carried off ($1500's end). Seized outright (kind 3, Fiona_StateGrabbed): rising from a fall she
is caught as she rises ($1503 / $B03), else held ($1100; pulled back - $1101 - when 17 ahead of her
is free); her cry $41; at its end the game's over. He lets go (`unhand`, or gone): she pulls
free ($F02). The game's over is the `caught` state flag (once; not with `capture-no-end`);
`fiona-occupied` is set while she's held or dragged off.

## F4c: defending (`scripts/fiona/defend.fs`)

The square button (E), while she's free (mode 0), not past the panic's stage 3 and not under
`no-flee` (Progress_PlayerButtons; Fiona_StateBlock 8): standing, walking, resting, or just set
off running (her recovery under way, or under $3D frames of running) - a shove; running, or
turning to flee - a kick. The shove ($E00, its pace slower the longer she's been shaken -
recovery past $1C3: (3150 - recovery) / 1800 x 1.5 - and the more frightened - fear past 40:
(160 - fear) / 120) reaches 2 round her hand (bone 9) at its key: a `blow` 1, damage 1; into a
wall it ends ($101). The kick ($E01: her cry $3D, a step of 1.0756 a frame while it fades in)
meets whom she touches while its key is clear: a `blow` 2, damage 5, he may stumble (`how` 1:
rolled by him); at its end her fear up 10. Each reaches the stalker and Hewie once a blow;
struck home (`blow-taken`) she's held still 5 frames ($90 kicked, $8F shoved). Hewie is told
(`reaction` 10 / 11) when she shoves or kicks without striking him.

## F5: the story's words about her (`scripts/story/words.fs`)

She tells, in `fiona-doing`, her action too (+0x1AD580: `her-doing`). Conditions 0x3F her action,
0x3D free (Fiona_IsIdle: her mode 0, not 1 / $E / $F; the panic's stage below 4), 0x26 the action
she started this frame (Fiona_MarkActionStart: her command to Hewie, cleared each frame - hga's
`cmd`). Orders: 0x94 calmer by n / 30 (Fiona_CalmDown: off her fear), 0x95 her recovery shorter by
60 / n frames (Fiona_LowerRecovery). The panic's (`docs/subsystems/panic.md`): 0x43 the threat
(Threat_Raise - as a fright, into the weighted parts; `threat`), 0xC0 its accumulator, 0x93
calming (the converter's `panic-grow`: bit $80 128 at once, else v / 30), 0xC9 the level set to
one it hasn't passed. Checked by `tests/story/test_hewie_fiona_words.fs` (13, with Hewie's).

## F4d: slamming a door (`scripts/fiona/slam.fs`)

Fiona_SlamDoor, tried by her flee turn (at its event $20, her radius behind her, looking back),
her kick (each frame of its window, her radius ahead) and her shove (at its key, her hand). A
door of the room at rest that she faces within 45 degrees, on her way to a point on a door's
passage (triangle flag $20000): shut, her in its area 4, neither locked nor stuck - burst open
(`barge`); open, her in its area 1, faced side on - slammed shut (`slam`, her sound $91). Not one
somebody holds or locked against her. She's held still 5 frames. Shut out by it: Hewie and the
stalker at its front (area 0) on its far side (Doors_Side 1; slammed shut, or in its way - on its
passage) - each a `blow` 5, 3 damage, how: the exit. Off screen, she being chased and the door
burst open, the stalker decides (`blow` all the same: struck only when he is right behind it).
Any shut out: the kick's and the shove's blow is spent. Checked by `tests/stalker/test_stalker.fs`
(a slam on him in the doorway). Not yet: the rumble; the PursuerGroup bit 8 ("may pass") is
taken as standing on the door's passage triangles.
## F5: ladders (`scripts/fiona/ladder.fs`)

A ladder is a nav link of the room (NavMesh_FindDoorRegions: two triangles sharing a link group,
bits 9..13; `nav-link-*`): side 0 its foot (the lower triangle), side 1 its top, each with a spot
(the edge toward the other) and a facing (across it, toward the ladder). Free (her mode 0, her
action 0, the panic below stage 4), in its region on one side (`nav-link-at?`, `nav-link-side`),
facing along that side (Actor_FacingDoor), the stalker not in the region nor touching her - the
action button (Fiona_StateLadder): she walks to its end's spot (Actor_DoorFront with
kFionaMeetOffsets - 12-byte entries x, z, degrees: the top 1.7 back from it, the foot 2.1 out),
facing along it, and gets on ($700 from the top, $704 from the foot; her mode 3, sub 7). On it
(Fiona_StateOnLadder), when a move has come round (the motion's flag $20 - Motion's
`MOTION_EVENTS` are the slot's flags, not the key events) or while holding, the stick (W / S):
down - the next rung ($701, its end $702), off at the foot within 18 of it ($703); up - the
next rung ($705, its end $706), off at the top within 3 of it ($707); let go - holding ($708 /
$709). After each move her triangle is the end's. The stalker in the region at its foot keeps
her from getting off there (she holds on); at its top, within his height above her, from going
up (Progress_WhoIsWhere's region bits: 4 side 0, 2 side 1). Off (Fiona_StateLadderOff): at the
move's flag $20 she is put by that end (the foot 2.1 out, the top 1.7 back) and free. She is
moved by her animations' root motion, up and down too, off the floor. Rooms with ladders:
castle-1f-2 (two), room 3 (two), 29, 37, 40, 47, 48, 84. Checked by
`tests/fiona/test_ladder.fs` (11): on from the top and down, off on the ground floor; turned
from it nothing; on from the foot and up, off on the floor above; the stalker at its foot - she
holds on. Not yet: knocked off it (FI 0x1AD584 bit 2: Fiona_StateCaughtCrawling), the region's
slot bits for Hewie.

Also fixed with it: `walk-anim` (Fiona_StateCmdWalkAnim) read the animation's key events for
`MOTION_EVENTS` $20; it is the motion's come-round flag (`ended?`).
