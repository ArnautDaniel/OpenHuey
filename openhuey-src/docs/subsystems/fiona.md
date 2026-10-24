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

A debug walker stood in for her until F1 was done.

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
- Hewie-controlled play (the original's `+0x1FBEC1`): built with the mini-game - she is his
  companion then (`fiona/companion.fs`).

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
voice.

**Keeping apart** (Fiona_KeepApart, fiona.fs `keep-apart`): free and overlapping the stalker,
she is pushed out to their radii apart - along his step this frame (`stalker-stride`, his root
motion while it takes him ahead) 20 on plus the way from him to her when he steps at her, else
straight away; a push off her floor leaves her where she is, out of contact (+0x2A) - no more
pushing until she touches neither; nor while he is out of contact himself (his +0x2A, as he
tells it: `stalker-ties` - through a door, on a ladder). Overlapping Hewie she is put out of him (`push-out`,
Actor_PushOut: their radii + 0.01 apart, straight away or turned 2, 4 .. 180 degrees either way
until reached over her floor), else back to where she was. None of it when she touched Hewie
(and not the stalker) as the frame began. `idle` (Fiona_ToIdle) sets the stick back to the
camera's (+0x1AD588 0); arriving afresh by an exit keeps a door's held stick (turn mode 3).
These, and the buttons' refusals below, are checked by `tests/fiona/test_rules.fs`.

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
| `fiona-plight` | `( state ladder -- )` | broadcast each frame | The ladder she is on (-1 none: the stalker goes to beside it). How she stands for an attack (Pursuer_FionaState): 0 calm, 1 fleeing, 2 panicking (fear over 90, the panic attack), 3 out of reach (struck, seized), 4 / 5 the panic's stages. |

Fiona_Reaction / Fiona_React: none while a reaction is under way (mode 4, floored $A), on a
ladder (her fall from it: with the ladders) or with the world held. A blow knocks her down -
on a step or in a doorway (floor $80003) the short falls $100E / $100F, else $1000 / $1004 by the
side it came from (front, behind +1, her right +2, her left +3), her cry $3E / $3F; up again, 30
frames before she may fall again. A hard blow throws her: facing him back along her heading
($1008, then up $100A), from behind forwards ($100B, up $100D) - the get-ups cut in
(Motion_Play); panicking she is flung down ($B04) and stays down as from a panic fall ($B01, her
struggle and shakes started over; up when her breath is back); moved by the motion turned toward
him, 20 degrees a frame, along walls - not when thrown (sub $A) in an exit's area. Thrown, after
it and getting up, nav flag 1 blocks her too; after a door's throw (sub $B) her floor is the
door's ($8000018). The side of a knock-down: from whoever struck; nobody (a creature's request),
from her request's point; a striker gone meanwhile, as from behind. A hold catches her: her gasp $43, a flinch by
his grip ($F02 by the arm looking at him - only facing him on open floor -, $F04, $F03 from
behind, $F05), then free. Each takes its fright to her panic; a blow tells the danger she was
struck (`danger-signal` 1); a stumble resets her recovery (`reset-recovery`: 1800 frames to calm
again - it had been set to 0, as if she had calmed at once; the chokers spare her).
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
(160 - fear) / 120) reaches 2 round her hand (bone 9) at its key: a `blow` 1, damage 1; with a
wall between her and her hand (the walk from her triangle to it, over flags $20018: Actor_TriTo)
it ends: blended into standing over 10 (Motion_PlayWith flags $101), and once that has faded
in Hewie is told (`reaction` 10, unless she struck him) and she stands - the shove's own end
tells him nothing. The kick ($E01: her cry $3D, a step of 1.0756 a frame while it fades in)
meets whom she touches while its key is clear: a `blow` 2, damage 5, he may stumble (`how` 1:
rolled by him); at its end her fear up 10. Each reaches the stalker and Hewie once a blow;
struck home (`blow-taken`) she's held still 5 frames ($90 kicked, $8F shoved). Hewie is told
(`reaction` 11) when her kick ends without striking him. Not while she is turning to push, in a
panic attack or out of breath (Fiona_IsIdle). Her gestures to Hewie are read during a shove only
as it comes round (Fiona_ReadsPad: mode 8, her action 5, the motion's flag $20).

## F5: the story's words about her (`scripts/story/words.fs`)

She tells, in `fiona-doing`, her action too (+0x1AD580: `her-doing`). Conditions 0x3F her action,
0x3D free (Fiona_IsIdle: her mode 0, not 1 / $E / $F; the panic's stage below 4), 0x26 the action
she started this frame (Fiona_MarkActionStart: her command to Hewie, cleared each frame - OpenHuey's
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
(a slam on him in the doorway). The PursuerGroup bit 8 ("may pass") is taken as standing on the
door's passage triangles.
## F5: ladders (`scripts/fiona/ladder.fs`)

A ladder is a nav link of the room (NavMesh_FindDoorRegions: two triangles sharing a link group,
bits 9..13; `nav-link-*`): side 0 its foot (the lower triangle), side 1 its top, each with a spot
(the edge toward the other) and a facing (across it, toward the ladder). Free (her mode 0, her
action 0, the panic below stage 4), in its region on one side (`nav-link-at?`, `nav-link-side`),
facing along that side (Actor_FacingDoor), the stalker not on that side of it nor touching her - the
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
moved by her animations' root motion, up and down too, off the floor. In full panic (her fear
bit 2) she lets go - knocked off as by a blow (mode 4, sub 8: Fiona_StateCaughtCrawling). Rooms with ladders:
castle-1f-2 (two), room 3 (two), 29, 37, 40, 47, 48, 84. Checked by
`tests/fiona/test_ladder.fs` (11): on from the top and down, off on the ground floor; turned
from it nothing; on from the foot and up, off on the floor above; the stalker at its foot - she
holds on; struck on it, knocked off (`knocked-off`, Fiona_StateCaughtCrawling); panicking on it,
let go. Not yet: the
region's slot bits for Hewie.

Also fixed with it: `walk-anim` (Fiona_StateCmdWalkAnim) read the animation's key events for
`MOTION_EVENTS` $20; it is the motion's come-round flag (`ended?`).
`play-cut` (Motion_Play) cuts in with no blend; it had taken the motion table's fade.

## F5: pushing (`scripts/fiona/push.fs`)

Fiona_FindPushable, tried each frame she walks or runs in her own moves (move mode 0) (her
step's reach ahead, from where she was): the walk toward that point meets a wall (an edge, or her floor's blocks; `v-wall`), and
the floor 2 to each side of it is pushable ($800000: an obstacle's ring). She turns square to
the wall, from her side of it (Fiona_StateTurnStick: 10 degrees a frame, her action 1, sub 5),
while the stick holds within 0.6 of that way, then leans on it ($1200: 2 back). As each move
comes round (Fiona_StatePush): the obstacle on both sides (the lowest; looked for again at 2.2)
pushed if it can go - `push-obstacle`, her push ($1201: 5 on, her sound $45; Fiona_StatePushStep)
- else she strains ($1203). Pushing on, each push come round: `obstacle-moved`; the stick still
that way and the floor 2.2 ahead pushable, pushed again, else she strains. The stick let go:
straining she lets go ($1202, then free), else free. The stalker touching her: free at once.
She lets go when the stalker touches her or her panic attack takes her (Fiona_PushBlocked: FI
0x1AD584 bit 2). Checked by `tests/obstacles/test_obstacles.fs`.

## Her legs on the floor (`src/game/humanlegs.c`)

Her model stands on the floor by her feet, as the human models' do (`model.c`
HumanModel_BodyFrames, HumanModel_PlantFeet, Model_LegIK; Fiona_AnimUpdate decides when, `fiona.fs`
`fit-legs`). Each frame she is on her feet (+0x1AD5BC: set as her frame starts, cleared by the
states she isn't - thrown, down and getting up, seized, led, dragged, held, on a ladder and
falling off it, the story's step while through blocked floor; `feet-off-acts`):
- **her body** goes to the floor under the lower of her feet (y up: the lesser), eased a quarter of
  the way a frame (not the first after the model is loaded) - the floor found by walking her nav
  mesh from her triangle to under each foot (Motion_OntoFloor: at a wall, its slope carried on);
  with motion flag $100 a foot not down counts at her height; on a slope motion (flag 4) the floor
  at two points along her step instead: 11 back and 1 ahead, 5 and 0.5 running, turning to flee,
  kicking or out of breath (Fiona_AnimGroup 2, 9, 10, 3);
- **her feet** are put on the floor under them (on a slope motion their height over her eased by
  how much the slope faces her way: Model_SlopeKeep), and her legs - hip, knee, ankle (bones 2..4,
  6..8; feet 5, 9) - bent to reach them, two-bone, the knee to the side of the hip's Z (IK2); the
  ankle and foot moved as far as the solved ankle went. Through the pose hook, before her hanging
  parts.

Off her feet her model is at her place, unfitted; off the nav mesh, at her place (Model_BodyFrames).
The engine's words: `human-legs ( id right left -- )` (the bone chains; at load), `legs-fit ( id tri
mask plant -- ) ( F: back front -- )` each frame after she is placed, `legs-height` (her body's
height). Put somewhere - into a room by an exit, by the story (Fiona_PlaceOn: `put`) - her body is
at her feet's height at once, not eased from where she was (the model's FeetUp: the engine's
`actor-settle`, as for the stalkers' Pursuer_Place, the figures' and Hewie's Hewie_PlaceOn - his
feet planted afresh): coming out of a door into a room on another level nobody rises through the
floor or comes down from above. Checked by `tests/fiona/test_legs.fs`, `tests/fiona/test_arrive.fs`.

Her head (`fiona/looks.fs`) turns as her model is posed as the human models' do
(HumanModel_AdjustBone; the engine's `head-bones`, `head-turn`: `humanhead_adjust`): the head bone
in its own frame by the tilt and half the turn, the bone below it the other half about its own
place. Its bones by her costume's model class (each Loaded's +0x8B0 / +0x8B4): her clothes and
costume 2 $21 / $17, the slip (FionaModel) $35 / $2B, costume 3 $1D / $13, 6 $14 / $A, 7 $16 / $A,
8 $13 / $B (before, every costume but the slip took $21 / $17). Checked by
`tests/fiona/test_costumes.fs` (each head at head height, above the bone below it). The nav mesh keeps section 16's bytes for it
(`NavMesh.marks`: bit $80 a wall to the feet where the mask blocks).

## Her hanging parts (`src/game/cloth.c`)

What hangs and sways on her model is C's, as posing is: the original's spring sets
(`model.c` `SpringSet_*` and the point classes; `FionaModel_Setup`, `EventHumanModel_Setup`). Each
part is a point with a velocity in the room, stepped once a game frame (so it trails as she moves
and turns), held at its length from what it hangs from, pushed off spheres on her bones; then its
bone is aimed at it before the model is skinned (the pose hook, as Hewie's legs).

- **The slip** (`O_FIS`): eight strips of four from her hips (bones $B..$2A), each swinging from
  hip bone 2 or 6 within a cone that widens down the strip and kept to its outer side; a bone
  hanging from her head ($3D); her hair (four, $39..$3C, off spheres on her head and shoulders);
  her chest sprung to its bone ($2D).
- **Her clothes** (`O_FIN`): the skirt's six pairs from her hips; her hair ($25..$28); two bones
  hanging ($29, $30); her chest ($19).

Settled afresh (30 steps) when the model is loaded, when she is put somewhere (`put`: the story's
`place`, an exit - Fiona_PlaceOn's +0x850; the engine's `actor-settle`) and at a scene's cuts (its
signal 0: Fiona_FollowAnim - the cutscene does it for everyone it drives). Settling puts the parts
at rest under their anchors first (OpenHuey's own: the original's points start at the origin and
its 30 steps bring them in).
Her other costumes' sets are there too (`costume_hang16` / `costume_hang3`: O_FIH, O_FIO; O_FIB,
O_FIC), and the stalkers' (Daniella, Debilitas, Riccardo, Lorenzo's two). Not yet: the cast's
(Lorenzo's hanging figure's strands, her doubles' springs).

## F4e: the creatures' requests (`fiona/hurt.fs`, `fiona/seized.fs`)

`fiona-request ( kind x y z )` from a creature (Progress_Noise; the point it came from), told on
as `fiona-requested ( kind )` (the story's cond 0x64 reads the progress' +0x1050 that frame):
- **$D** a spirit touched her: **falls** ($13, Fiona_StateFall13) away from the point - on open
  floor by the side it came from ($1004 ahead, $1005 behind, $1006 / $1007 the sides), on a step
  or in a doorway straight down ($100F); her cry ($3F), a noise ($5F); at the fall's last frame
  the threat up 75; up at its end.
- **3** a red one's going: **grabbed** ($10), as a stalker's grab.
- **$C** homunculi: **held down** ($12, Fiona_StateHold / StateStruggle): $1405, $1406; each shake
  takes a frame off the hold (180) while shakes count (60); up ($1407) when the hold runs out or no
  homunculus in her room is holding (move 8, from `creature-doing`).
Not while a reaction is under way, on a ladder or floored (but held again by them while held).
**Unseen** (+0x1AD630): item $89 in the equipment's slot 1, standing still 60 frames - she fades
out of sight (8 a frame; back in when she moves) and `fiona-unseen` is told. Her motion flags
($20 come round, $400 at its last frame) are the original's MOTION_EVENTS. The story's 0xB2
(`throw-down`: thrown by a script), 0x96 (`look-hold`), 0xBC, cond 0x59.


## F6: the rest of her (2026-10-13)

- **Her voice and motion sounds** (`fiona/voice.fs`; Fiona_Voice, Fiona_MotionSounds): on her
  motions' key events - letting go ($3C), the shove's grunt and noise ($1F; the special costumes'
  own), the flee turn ($F), pulling free / dragged / carried ($7D), the panic fall's slide ($7E),
  falls ($80), a locked door's rattle ($71). Her cries go through `voice`: muffled ($1C, bank 6)
  when a water triangle ($2008000) lies between her feet and her head (`nav-crosses?`).
- **Locked doors** (`fiona/doors.fs`; Fiona_StateLockedQuick / Slow / Spot / Try): quick - at her
  spot the try, then the built-in script's message, "The door is locked from the other side..."
  (system message $8001, through the room: `fiona-message`), standing again once it is read; chased
  - from the spot, no message; panicking (her fear's bit 2) - from where she stands, sliding to the
  spot a seventh of the way a frame as the try fades in.
- **Struck while using a door** (Fiona_StartInDoor): the door she holds is let go as she meant it.
- **The story's door use** (moves 3 / 4 of `self-move-slot`, Fiona_Requests): through the door at
  that exit, opening or shutting it; her move done as the use ends.
- **Hit by a door** (request 5, Fiona_StateHitByDoor): the stalker waiting at a door she is behind
  (its area 2) or holds, the door 10..80 open, pushes it into her - thrown along its swing,
  forward ($1008) or back ($100B; panicking flung down, $B04, her panic full), cut in; she
  slides along it turning to it 20 degrees a frame (not flung down).
- **Not at a door with the stalker touching her** (Fiona_StateBlock 2 / 9): the action button
  does nothing there.
- **Knocked off a ladder** (reaction 8, Fiona_StateCaughtCrawling / CrawlDrop / Pull / Fall / Down):
  to the spot 10 out from its top - within 20 of it pulled off ($F02), else dropped there ($70A /
  $70B, then down $70C and up $100D), falling 0.5 faster a frame; her cry ($7D / $81).
- **The charm** (item $88 in slot 1): a hold doesn't take her (the fright still does).
- **The short falls** on a step, in a doorway's triangles, within 20 of a door, or in a ladder's
  region (Progress_WhoIsWhere's bits).
- **Keeping out of the others** (Fiona_KeepApart): free, overlapping the stalker she is pushed out
  to their radii apart (along his heading when he comes at her); overlapping Hewie, back to where
  she was.
- **Her costumes** (`wear`; the story's 0x97, event.c fiona_model): 0 O_FIN (her clothes), 1 O_FIS
  (the slip), 2 O_FIH, 3 O_FIO, 6 O_FIB, 7 O_FIC, 8 O_FIF; worn by the story - variants 1..6 by the
  last of story flags $41, $6F, $82, $88, $94, $95 (Fiona_LoadFiles); textures their _000's; the
  markers their own (0, 6, 7) or her clothes'.
- **Her earrings and choker** (`show-equipment`; Fiona_ShowEquipment, as she is dressed and as the
  sub screen changes what she wears - the `equipment` broadcast): on her clothes (O_FIN:
  EventHumanModel_ShowParts) and O_FIH (Costume2Model_ShowParts) the earrings she wears ($86..$89,
  slot 1) shown - parts 32 / 36 / 37 / 33 (O_FIH 43 / 47 / 48 / 44) - and the other pairs hidden;
  her choker's ($8A..$8D, slot 3) parts the same, with none her bare neck's (3, 4; O_FIH 1, 2). Her
  other costumes have no such parts (their ShowParts is empty).
- **Her face and hands** (the engine's, `actor.c`; `act.alive` on as she is dressed): out of
  scenes she blinks (Motion_Eyes: her eyelids' shapes down and up over 14 frames by the game's
  curve, another from 30 frames on at random, surely by 150) and her hands take her motions' poses
  (Motion_Hands: the motion table's pose, shaped over the five frames its events say); in a scene
  its motion's channels move her mouth and hands (Motion_ExtraChannels: tracks -7..-12). Her
  hands' own shape is 1 (CharModel_Loaded). Drawn as the original's: out of scenes the face its
  rest, mouth and eyelids blended (shapes 0, 1, 7, 8: func_001BC8D0); while a scene has her every
  one of its 16 shapes by its weight, the rest what is left (the model's +0x30 set by
  Motion_Disable: func_001BCD30 - the brows, eyes and mouth the scene's tracks move); a hand from
  one shape to another. Taken by a scene and given back her face, hands and blink are cleared
  (Motion_Disable, Model_ClearDraw: no blink left half shut through a scene). Checked by
  `tests/story/test_scene_faces.fs`.
- **Her shove and the special strikes** (Fiona_StateStrike): costume 6's strike 6 ahead of bone
  $21 (reach 3, damage 5, its own sound $22), costume 7's 10 aside of bone $23 (reach 10, a hard
  blow of 10, a blood spray on whom it meets); her boots in slot 0 - metal ($81: 1 in 10 a
  stumble, damage 2), silver ($82: a hard blow of 5, 1 in 5 a stumble), burst shoes ($83: 1 in 10
  a hard blow of 50 with a burst on him); Fortis's boost x3. (The strike's offsets are turned by her
  heading from the bone - the engine keeps bone positions, not their frames.)
- **Her blow's point** (Fiona_PointOfInterest, `body-point!`): her kick's (10 up, her radius
  ahead) or her shove's at its key - the story's zones test it (cond 0x37 `char-in-zone?`), and
  the things she kicks.
- **Her call while followed** (Fiona_CallAction): a creature within 10 ahead (45 degrees), reached
  straight from her - "go for it" ($2D).
- **The story's 0x20 / 0x21 for her**: through blocked floor (`noclip`), left alone - out of reach
  of attacks (`aside`: her plight 3).
- **Unseen, the stalker walks into her**: he has her (a hold, how 3).
- **The flee turn**: toward the stalker within 30 (the turn spins her from him); at its end Hewie
  told (reaction 9).
- **Letting go of things** (`fiona/letgo.fs`; Fiona_StateLetGoStart / LetGo): the triangle with a
  throwable in slot 2, not panicking (the threat at 4) - thrown ($D00), aimed at the stalker within 80 and 60 degrees ($D01), set
  down ($D02); at the key the thing leaves her hand (4 / 6 / still) unless her hand is past a wall;
  one of the item used; a throw tells Hewie (12). What becomes of them: `docs/subsystems/things.md`.

**The rumble** (`scripts/rumble.fs`, the engine's src/game/rumble.c): her jolts - the pad's
large motor for some frames - as the original's: knocked down ($80 / $A0, 8), thrown or a door
into her ($D0, 12), seized ($FF, 16), caught ($80, 8), a spirit's fall ($A0, 8), off a ladder
($D0, 12), a door slammed ($90, 8), her kick landing ($C0, 12), each push ($40, 16), each tug led
or dragged ($60, 8). The creatures leave her be while she is out of reach (`fiona-plight` 3:
struck, seized, set aside by the story's 0x21). She never fades in doorways (Fiona_LightChange
keeps her layer; the others do: `doorway_faded`).

**Her other costumes' hanging parts** (Costume2 / 3 / 6 / 7 Model_Setup; src/game/cloth.c): O_FIH and
O_FIO - a skirt (as her clothes'), a bone hanging, her chest, another bone hanging, sixteen hanging
points (`CLOTH_COSTUME`: those on a bone swing within 30 / 35 degrees, the rest 60, some going on
straight from the one before) and three blown by a wind off a bone (`CLOTH_WIND`); O_FIB - a bone
hanging, her hair, her chest; O_FIC - five bones hanging, two sprung, one more hanging. (O_FIF has
none.) **Unseen in her camouflage** (item $89 in the equipment's slot 1; Fiona_LightChange): she
fades out, and once gone shows softened - layer $1C, drawn at half size and laid over the scene
before the room's look (`post_soft_*`).

**The mini-game's run: the companion** (`scripts/fiona/companion.fs`; Fiona_MoodAI,
Fiona_Timers, Fiona_OffscreenFrame, the run's parts of Fiona_MoveInput, Vt34 and Vt38,
Fiona_StateBlock's 0xD). While Hewie is played she goes with him by herself. Her moods: by him,
waiting, following him, fleeing (to an exit, through it), off by an exit at random, after him by
an exit, sent to open an exit, at the stalker; out of sight: after him, waiting, wandering,
fleeing. By turns she keeps with him (5..20 seconds) and goes off on her own (10..21) - the
second shorter the longer he keeps by her, him lying flat (25 in 100) cutting it short. The
stalker in sight she may run, by her panic (always past 80; one in five past 60 ... one in twenty
past 10), out by the exit she reaches before him (not into his room on his half); panicking, at
once. Running into his reach she goes at him (her shove or kick by her pose: `defend-by-pose`).
Hewie barking low by a shut exit sends her to open it (at its front: her own door use); barking
elsewhere she keeps with him longer. When he goes out by an exit and she is by him or following,
she comes through with him if she is nearer its doorway than he is (or already in it), else
after him out of sight. Out of sight she walks the house door by door at her pace (following or
fleeing 0.75..1, waiting or wandering 0.3..0.45, by her fear; calming down 0.1): after him by his
way, away from the stalker's room fleeing, at random wandering; a shut door on her way opened
from afar (`open-from-afar`), a locked or held one waited at; her fear growing while she follows
or flees, easing while she waits; into the room played by its door - in its doorway facing out,
or opening it. The pad isn't hers: her calls to him (now and then: go, come back, stay) come from
her mind and don't command him, she doesn't struggle (Fiona_Shakes 0); dragged off she breaks free
after as many drags as her panic's stage, and cries for him now and then.
