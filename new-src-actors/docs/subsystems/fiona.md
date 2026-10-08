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
| F4 | Defending: kick, shove, strike, slamming doors on someone. Reactions: knocked down, thrown, caught, grabbed, led away, dragged, shaking free, falls | stalkers |
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
new-src's port (`new-src/scripts/fiona/moves.fs`, `doors.fs`, `core.fs`), faithful in
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
4. **Running** needs the run button, and story flag 0x1E not set.
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

*Stand-ins until the story:*
- the exit check (in an exit's area and free: the room scripts do it). It waits until she has
  been out of every exit's area since arriving.
- her arrival on the exit's outside spot, facing in (the scripts' `char-to-exit`).

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
up, 2 down, 3 R3, 4 right - held for more praise, 5 left), read under state flag 0xD and not
0x2B (Fiona_ReadsPad) and acted on the next frame. The code by what Hewie is doing and where
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
