# The creatures

## Purpose

The house's lesser creatures, placed by the story (or scattered about Fiona's room by a count),
kept across rooms, coming after her. Two classes, as in the original (`src/game/creature.c`):

- **Spirits** (CreatureA, slots 0..6): a glow with a fading trail, bobbing in the air. Out of
  play until Fiona comes within its reach; then it comes after her - straight at her when it
  can, along a path when not, through doors off screen when she leaves. If it reaches her it
  vanishes in a burst of light with a noise (kind $D) the stalkers hear; if its time runs out
  first it fades away. Red ones also leave a noise where they go. While Fiona can't be reached
  (hiding, held) it hangs about near her. Holding item $8D (in the equipment's slot 3) keeps
  them away.
- **Homunculi** (CreatureB, slots 7..9; O_HMB/HMB_000): small walking figures that watch her,
  follow, grab her and can be struck - **C3, not built**.

The kind-$19 creature (an event character, `Kind25_*`) and room $4F's commands (the spirits that
vanish at a spot there) are separate; not built.

The original's objects: the manager gCreatures (10 slots: `Creatures_*`), SceneGame +0x706480
(its resources, the story's +0x74 / +0x78: `SceneGame_PlaceCreature`,
`SceneGame_SetCreatureCount`, `SceneGame_RoomCreatures`), and each creature's own block
(+0x1540). Their state is kept across rooms in the progress (+0x878: 36 bytes a slot,
`CreatureA_Save`, `Creatures_Keep`).

## Actors

`creatures` (`scripts/creatures.fs`): the slots - placing, removing, keeping them in the
progress, the story's questions. Each creature is an actor of its own (`spirit` in
`scripts/creatures/spirit.fs`); it moves itself on the floor (a body: radius 1.5, height 16; it may
stand anywhere on the mesh - its paths keep off $40080, and put somewhere at random it keeps to
its floor of a split room) and draws its glow through the engine's particles (`spirit-new`: the six glows
and trail, the vanishing burst).

## API

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `creature-place` | `( room level tri kind reach which yaw -- )` | in, the story (0x8A) | A creature of `kind` (bit 7: a homunculus) in `room` on `tri`, on `level` (0 / 1: which floor of a split room; 2 both), coming when Fiona is within `reach` x 10 (0: at once); `which` -1 a free slot 0..5, else slot 6. Out of the room being played: no triangle. |
| `creatures-clear` | `( which -- )` | in, the story (0xCB) | 0 all, 1 slots 0..6, 2 slots 7..9: gone. |
| `creature-count` | `( n other -- )` | in, the story (0x99) | How many to scatter about the next rooms (other bit 7: homunculi). |
| `creature-doing` | `( slot room mode up -- )` | out, broadcast by each creature each frame | Where it is and its move mode (2: hanging about her) - for the story's 0x53 and 0x58. |
| `creature-gone` | `( slot -- )` | out, to `creatures` | It has vanished or faded: its slot is free. |
| `exit-taken` / `entered-room` / `arrived` | | in (broadcasts) | Fiona leaving by an exit (they follow); a room coming in (back on the floor, in play or not). |

The story's words: `creature-place`, `creatures-clear`, `creature-count`, `creature-action?`
(0x53: one in this room in that move mode), `self-near-creature?` (0x58: touching one of 7..9).

## Rules (spirits: CreatureA_*)

**Kinds** (D_004164F0, 37 entries; the pattern behind them): $00..$11 blue, $12..$23 red, $24
blue (it comes after 61 frames, whatever her distance; it floats 9 lower). Within each colour
(k = kind mod 18): k >= 9 comes straight at her only from her own triangle; its time is 1800
x 1 / 2 / 4 frames by (k mod 9) / 3, its speed 0.125 x (k mod 3 + 1).

**Set up** (CreatureA_Setup, _SetupKind): bob 0.5 / 0.05, fade $80, turning every 20 frames;
it gives up straight at her on another level after 60 tries (30 if straight-only). From its
progress slot when that one is in use: its time, reach, out, come.

**Coming** (CreatureA_FionaDistance): her distance across the floor; it comes once she is on
a triangle it reaches and within its reach (or reach 0, or kind $24 after 61 frames). Out
(sent after her off screen): its rest of 1800 x (1..4) frames runs down; then, still elsewhere,
it comes.

**Each frame** (CreatureA_Frame): its trail (three points in turn: its place 12 above its bob),
whether she holds $8D; thinking; bobbing; touching her; then its state:
- not come: idle (holding $8D: its fade state);
- in the room being played: its time up - it fades away (a sound $8C); else on;
- elsewhere: its time up - gone; else travelling.

**Thinking** (CreatureA_Think): elsewhere - out of play, travelling once come, else idle. In her
room: not come - idle (on its triangle's centre the first time); come - its time runs; she
can't be reached (state flag 9, or her mode 3) - hanging about her (move mode 2), unless she
holds $8D; she hidden and busy - idle; else after a door it may use (CreatureA_FirstDoor) when
one leads her way, else after her (from afar with $8D only within 40).

**After her** (_StateAfterFiona, _StraightAtFiona): straight at her at its speed when her
triangle is reachable (not when she is held right by it); on another level it gives up after
its tries while within 2; on a triangle flagged $20000 that it can't cross it gives up at once
(its time ends); else along a path to her triangle.

**Hanging about** (_StateHangAbout): a spot 10..25 before or behind her, turned at random, on a
triangle not its own nor blocked ($04020028) - 17 tries - and it walks there; again when done.

**Touching her** (_Seen): come, she not hidden nor held, their bodies touching: a noise ($D, at
it), a burst of light 12 above it (red / blue) with a sound ($8B) unless in an event; it sinks
(0.1 faster each frame) to 12 below and fades 8 a frame; red ones then make a noise (3) at their
place, blue ones a level $80 noise in their room; gone.

**Following her out** (_FionaLeft): in her room, it heads for the exit she took (or another that
leads on); unless she is just beyond it, it goes off screen: its route to her room, each leg's
length counted down at its speed (2/3 with company); at each door, through it if the way is
open, else that door marked closed and the trip ends. Into the room being played: at the exit's
spot, facing in.

**Drawn** (CreatureA_Draw; GAME_FIX 32 x 32 cells): its core (cell $40,64: white, 2.5), its colour
(cell $A0,64: red / blue, 2.5 + its glow), three trail points fading and shrinking, its body
(cell $60,64, palette 3; vanishing: in its colour); all additive but the body; vanishing: only
the body. Layer $19.

**Kept** (CreatureA_Save, Creatures_Keep): each slot's state into the progress when the game is
saved and as rooms change; an empty slot cleared.

## Rules (homunculi: CreatureB_*)

**A homunculus** (slots 7..9; O_HMB/HMB_000, walking by its animations' root motion; a body 3 x
6, kept off triangles $04020028): kind $80.. (the story's `kind`; bit 7 set), its sight (`reach`
x 10), its heading, and - placed with a height (the story's `f`) - dropped from that high.
Strength by kind (D_0042C460): $80 5, $81 6, $82 7, the rest 5; its time out 9000 frames for
all. Kinds $88 / $89 walk anywhere (no blocked triangles; paths kept off $40080).

**Waking** (_Distances, _Notices): resting, it notices Fiona on a triangle it reaches within its
sight, Hewie in its room on his within 30, a blow, or anything at all with no sight: a cry
(sound 1), and it is out in state $D (turn to her; $E for kinds $88 / $89: to its spot).
Dropped, it falls first (state $B: animation $1001, falling 0.085 faster each frame, a thud -
sound 5 - then up, $1800, a sound at frame 34) and wakes once it notices her.

**Its states** (+0x69 the wanted one, +0x62 the one running; each with its first animation):
- 0 / 1 **watching** her (idle 0 quiet / $1C00, a chatter - sound 2 - every 44 frames), turning to
  face her; every 150 frames with state flag 9 set - 9, her caught - $C, out of her sight - $10;
  in sight every 90 frames - 5 (or, having held off before, 7 or the other watch by a coin).
- 5 **after her** (walking, $200): flag 9 - 9; her caught - $C; she hidden or busy (no flag $A)
  - $F; across the divider - $12; her in move 4 / 3 - 7. Straight at her when it can (not on
  another level after 30 tries close by); within 10 on her triangle it grabs (3) - not when
  holding her before, nor her in move 8; else its path to her (re-planned; her triangle blocked
  - $A; no path and no way between - $10). At 40..80 from her, every 150 / 300 / 450 frames it
  settles to watch (0 or 1 by a coin).
- 7 / 9 / $A / $C / $10 **approaching** a spot by her (10 / 20 / 5 / 20 / 10 away): a spot 10..25
  before or behind her, turned at random, reachable and off its own triangle; turn to it within
  30 degrees, walk there; turn to her, then watch (0 or 1). Each holds while its reason does
  (flag 9, her off the mesh, her caught, out of sight), else back to 0 / 5.
- $D **turn to her** (within 20 degrees: 5); $E its **spot** (room $8F triangle $E; room $91 $D0 /
  $9F for kinds $88 / $89), then turn to her; $F **waiting** while she's hidden or busy (flag 9 -
  9; flag $A or her free - 5); $12 **held off** across the divider (idles 0 and $1C00 every 90
  frames, facing her; on her side again - 0).
- 3 **grabbing** her: a spot round her in turn - her side it is on (2 before or 2 behind her),
  the other, 2 right, 2 left - walked to (move 8; blocked: a request $C to her); with her held
  (her move 4, sub $12): turn to her, a sound (8), $1900; then sounds 9 and 4, $1901, chattering
  (9 every 35, 4 every 70 frames) while she's held; let go - $1902, the threat up 75, a noise
  ($80); at its end it leaves (4), never to grab again.
- 2 **hit** (a blow of kind 1, 2 or 4 - her shove, kick; Hewie's bite): facing the blow, $1001 from
  the front, $1000 from behind, a sound (3), its strength down by the damage; out of reach while
  it reels; no strength left - it leaves (4); else 30 frames, up ($1800, a sound at frame 34), 5.
- 4 **leaving**: $1801, out of reach, fading 2 a frame from $FF, a sound ($A) at frame 65; at its
  end gone, leaving a bubbling (64 bubbles and 32 drops welling up; at frame 100 a splat on the
  floor). Its time out off screen: gone the same way.

**Following her out** (_FionaLeft, _Travel): as the spirits, at 0.35 a frame; a way shut for
kind 2 sends it back to its room's start (+0x8 2).

**Drawn**: its model (lit as the room; dimmed while leaving). Resting and not out: drawn.

## Status

**C1, C2, C3 built.** (A spirit leaves her be while she is out of reach - `fiona-plight` 3: struck,
seized, set aside by the story's 0x21 - as the original's +0x2D.) Spirits (`creatures/spirit.fs`): placing (0x8A), clearing (0xCB), the
story's 0x53, kept in the progress and back from a loaded game (`game-loaded`); waiting for her
within reach, after her straight or by a path, giving up on another level, hanging about her
while she hides, touching her (the burst; `fiona-request` $D), fading at its time; drawn by the
engine (`spirit-new`). Homunculi (`creatures/homunculus.fs`): resting until they notice her (or
Hewie), dropped from a height, watching, after her, approaching spots by her for each reason
(hidden, unseen, out of sight, off the mesh, held), held off across a divider, grabbing her
(the four spots round her; `fiona-request` $C on the way), struck by her blows (`fiona-blow`,
once a blow; `blow-taken` back), leaving with no strength left or at their time; the story's
0x58. Both off screen by `creatures/trip.fs`. Fiona's side: `fiona-unseen` (item $89, still 60
frames), her blows numbered. Hewie's side (Hewie_PickTarget, Hewie_Bite): each creature
tells `creature-doing ( slot room mode up )` - up: a spirit after her, a homunculus awake; he
goes for a homunculus holding her down, else the stalker, else the nearest creature up and not
struck; his rush and his pounce bite the homunculi his mouth comes within 10 of (once each,
`fiona-blow` numbered below 0), and a bite landed eases his temper with her (sound $6C). The
spirits take no bites. Doors (`creatures/trip.fs` door-watch; CreatureA_FirstDoor / _Think,
CreatureB_HeadForDoor - its states 8 and 6): one standing in an exit's area as its door is played
waits there while the door stands open, carrying on once the door is still; the door shut, it goes
through after whoever used it. A homunculus after her holds off while the stalker stands at the
front of a door it is by ($11, CreatureB_StateDoor). A homunculus's footsteps (CreatureB_Footsteps):
walking, sound 6 / 7 as each foot comes down. Leaving, it fades and bubbles well up where it sinks
- 64 bubbles, 32 drops thrown up, a stain on the floor at frame 100 (the engine's `welling`,
Cr19Bubbles). The story's count (0x99, SceneGame_RoomCreatures; `creatures-on`): as each room
comes in (and after a loaded game, 5), each free slot of the first `n` gets a spirit of any kind
in a room next to hers (within two open doors), sent after her - out, resting 1..4 minutes, then
on its way; with company (the stalker or a figure of the story up in her room) at 2/3 the pace. Checked by `tests/creatures/test_spirits.fs` (28) and
`test_homunculi.fs` (21), Hewie's in `tests/hewie/test_pounce.fs`; seen on screen.

Not yet:
- The special spots of rooms $8F / $91 checked only by the numbers.
- Spirits: heading for her exit only when it can reach it; placing at random away from the
  doors' event spots. The story's count of homunculi (0x99 with bit 7: no room asks for it).
- Kind $19 (an event character); room $4F's commands.
