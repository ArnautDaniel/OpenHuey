# The stalkers

## Purpose

The pursuers: Debilitas (the first chapters), then Daniella, Riccardo and Lorenzo. One family -
the same senses, search, chase, attacks and reactions - with each one's numbers, animations and
special moves. The original's shared base is `src/game/pursuer.c` (the NPC and Pursuer layers,
~14,000 lines); each stalker overrides part of it (`debilitas.c`, `daniella.c`, `riccardo.c`,
`lorenzo.c`). Debilitas is built first.

One actor per stalker in the game (`stalker`), with a behaviour per kind extending the family's.
It owns its body, mode, what it knows of Fiona and Hewie, its route and its health. It tells the
danger actor it is in play each frame (`stalker-here`), makes noises (steps, growls) and hears
them (acoustics), and acts on the others only by messages: blows (`hit`), grabs (`seize`), its
requests to Fiona.

## Phases

| Phase | What | Needs |
|---|---|---|
| **P1** | Debilitas in the house: his body and model, placed in a room (the story, a console word); his senses (whom he sees, what he hears); his mode (after Fiona, heading for her room, searching it, waiting about, held off) and how long he keeps to it; searching a room (its route of stops); walking (root motion, a path a stride at a time, turning on the spot); from room to room out of sight (his route through the doors, his pace) and coming into the room being played; his steps and growls heard; `stalker-here` each frame | rooms, doors, acoustics, danger, Fiona F1 |
| P2a | After her: the chase in her room (walking at her, stalking her, standing to watch her, turning to keep her in front, taunts, stepping round her), his attacks by his table for how she stands (combos from his move table: each blow a `hit` to whom it reaches, a hold catches her), the threat on her panic | Fiona F4a (knocked down, thrown, caught) |
| P2b | Grabbing her: his hand on her (led away by the hand or walking), his grab and lunge, seized (shaking free), dragged | Fiona F4b (led, grabbed, shaking free) |
| P2c | Dragged off: the game over | the game over |
| P3a | Struck by her: her kick and shove (F4); flinching, falling, down a while, getting up, hurt (a stumble), giving up for a while after enough | Fiona F4 |
| P3b | Doors: his use of the doors in her room (opening, coming through, going out), a door slammed on him | P3a |
| P3c | Hewie's bites | Hewie H3 |
| **P4a** | The story's stalker words: its conditions about him (in the game, his kind, here, his alert, free, his condition, health, the exit he came in by, out of sight) and its orders (after her / searching, his clock, knocked down, rage, started again, condition, health, silent); his `moving` for the scripts' waits | story |
| P4b | Daniella's own tables (moves, attack tables, temper) and behaviour (Daniella_Behaviour), his footsteps heard through the walls, the rooms a stalker keeps to | P4a |
| **P4c** | Riccardo and Lorenzo, and the other classes (kinds 37, 23, 10, 39, 12); the cutscenes' cast beyond slot 2 went to the cast (event characters) | P4b |

## API (P1, a first draft)

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `stalker-in` | `( room tri -- )` | in (the story, the console) | Come into the game in that room, on that triangle. |
| `stalker-out` | `( -- )` | in | Out of the game. |
| `stalker-here` | `( room alert chasing -- )` | out, to danger | In play this frame (alert: he knows of her; chasing). |
| `stalker-status` | `( room mode in-game own away cool -- )` | out, to the summoner | Each frame while in slot 2 (docs/subsystems/summoner.md). |
| `stalker-summoned` | `( room exit found plan -- )` | in, from the summoner | Come in out of sight by that exit: after her, or waiting about (Pursuer_IntoRoomByEvent). |
| `stalker-doing` | `( room mode sub cond hp hp-max kind -- )` | out, broadcast each frame in the game | For Hewie: his room, how he moves (3 off screen, 4 struck, 8 taking her), what (9 bitten, $A down, $18 leading her), condition, health, kind. |
| `blow` | `( kind damage how -- )` | in (Fiona: 1 shove, 2 kick, 5 a door - how its exit); out, to Hewie | Struck; his blows (and his throw, $B) strike Hewie. |
| `dog-attack` / `dog-grip` / `dog-refused` | `( kind )` / `( tri x y z yaw )` / `( )` | in / out | Hewie's attack asked; taken (where Hewie leaps to) or not (P3c). |
| `bite` | `( damage hard -- )` | in, from Hewie | A bite while he holds on. |
| `shaken-off` | `( -- )` | in / out | Hewie let go / thrown off. |
| `snap` | `( -- )` | in, from Hewie | Growled or barked at: his temper. |
| `noise` | | out, to acoustics | His steps, growls. |
| `heard` | | in, from acoustics | A noise he heard (he listens above a threshold). |
| `fiona-doing`, `hewie-doing` | | in (broadcasts) | What they are doing (whether she can be struck, grabbed...). |

Facts he reads: bodies (where Fiona and Hewie are, their rooms and triangles), the nav mesh (his
paths, what he can see across: a straight walk), the room graph (his route through the doors),
doors (open, locked: `door-state`).

## Rules (from src/game/pursuer.c, debilitas.c)

Debilitas: model `O_DB0/DB0_000`, motion table $3D89A0, radius 5, height 20, head bone $16.

**Senses** (P1b, `stalker/senses.fs`): he sees 150 ahead, 60 degrees either side, past no wall
(triangles $40080; $40088 while she hides) to her middle or one of 9 points round her far side;
he can reach her within 20 on foot (10 half hidden), straight there; Fiona and Hewie are
measured on foot in turn, a frame each. He hears noises above his threshold (Debilitas 0,
Daniella 12) through the acoustics actor.

**Modes** (P1c, `stalker/search.fs`, Pursuer_ModesSearching in the room being played): seeing
her he is after her (mode 0) and his clock restarts (300 frames, hard 540); out of sight it
runs down, then he searches the room (mode 2, 600 frames' clock, 2..5 stops); his route over
he waits about (mode 3, 900 frames), then searches again. Heading for her room (1) he searches
it once in; held off (4) he is after her again when his clock runs out. Coming into the game
he waits about (3) and walks to one stop at random first. *Simplified so far*: the original's
mode 3 in the played room waits for a re-search request (+0x16F2) from his behaviours rather
than its clock; his clock only runs down there off screen.

**The search route** (Pursuer_AddSearchStops, Pursuer_SearchRoute): up to 8 stops; each new one
is, 60 in 100, the room's next search spot (the room class's table +0x38, generated into
`stalker/spots.fs` by `tools/searchspots.py`: 146 rooms) while it has more, else a triangle
at random he may stand on (not both $100000 and $200000). A stop he has no way to is skipped;
past 8 the route is given up. A stop picked at random gets at most 150 frames' walk.

**At a stop** (Debilitas_AttackTable "searched", $3AFA40): 50 walk on (standing a moment), 20
look about $1302, 20 $1303, 10 $1305 (his gesture table $3AF6F0), played to its end.

**Walking** (`stalker/moving.fs`): by his animation's root motion; along his way a stride at a
time (Npc_WalkPathStride: the point a stride on; 45 degrees or more off with 4 or more left he
only turns, twice his rate), turning 3 degrees a frame; setting off past 80 degrees off he
turns on the spot ($400 left, $401 right, +2 past 160) until it ends or he faces the way; he
slows to a stand within 10 of the stop. His walk (Debilitas_StandAnim): after her $200 when
within 80 on foot and she isn't hiding, else $206; waiting $200; searching or heading $201.

**Out of sight** (P1d, `stalker/travel.fs`): his route through the doors (the engine's
`route-sides`, walkers of `stalker-kind`) from the half of his room he is on - some rooms are
two halves joined only through other rooms; his is the one the exit he came in by opens onto,
where the door marks it (`exit-way`, checked: Character_Route with no side). The half wanted in
the goal room is "either", but going round to her across his own room (below): hers. Each stretch is the next door's triangle number
on his side (plus the one he came in by - the original's own measure, Npc_NodeDistance),
counted down at his pace: after her 1.6..1.8 a frame, searching or waiting 0.6, heading for her
room or held off 1, while she hides 2. A door as he finds it (Npc_ExitKind): held shut - he
waits; locked against him, locked or stuck - not that way again (a fresh route avoiding it);
a doorway or open - through; shut - through, but into her room he first knocks (his sound
$28 or $23, bank 7, at the door on her side) and waits 60..120 frames. Into her room he comes
just inside the exit, facing in; a shut door he came by is taken (`hold-door`) and swung open.
His modes out there: after her he heads for her room (a new route as she moves on), his
clock out - heading for it (1); come to the room he heads for he searches it (its stops x 150
frames), then waits about (900); his wait over he heads for her room unless she is in his or
one next to it - then out by a door at random. When she leaves his room he is out of sight at
once (after her: routed to her room); when she comes into his (Pursuer_BackOnMesh) he is put
on its floor - by the door he made for if within 80 of it, facing it; else by the door he came
in by, facing in; else anywhere he may stand. Console: `castle-1f-10 debilitas-hunt`.

**After her, not seeing her** (mode 0; Pursuer_ChaseFiona): toward her on foot, his way
re-planned every 30 frames, standing facing her within 15.

**The chase** (P2a, `stalker/chase.fs`, Debilitas_Behaviour / Debilitas_Chase; his chase table
`chase-frames` / `chase-chances` in `stalker/tables.fs`, generated by `tools/stalkertables.py`):
seeing her he either stands to watch her (near and she faces him) or walks at her ($200) /
stalks her ($201) for 60 or 90 frames by his table's chances - whether she faces him (within
his sight's range of her, 90 degrees of her heading) and is within his close reach (50, hard
40). Standing, he turns on the spot past 60 degrees off her, every 90 frames may taunt her (the
chance by how near: within 15 100, 30 40, 60 20 - his table 10), walks or stalks again when she
turns from him or is 20 past his close reach, else after 90 frames of watching walks at her.
Walking or stalking, near and faced he stops to watch; his time out (counted only within 100)
he taunts or picks again. Within 24 - and within the ground he gains in 5 frames or 10 -
facing her within 20 degrees on her floor, and not held off, he attacks: his table for how she
stands (`fiona-plight`: 0 calm, 1 fleeing, 2 panicking, 4 / 5 the panic's stages; 3 out of
reach - none). Rows: a combo ($13), a taunt ($17..$19: his gesture table), round her ($1A /
$1B: 5 degrees a step at his distance, the way she isn't facing; 30 of it or 120 frames),
taking her ($14 by the hand, $15 walking), his lunge ($1002), his grab ($1003). After a combo his table 12
(it struck her) or 13 (taunts), held off 90 frames if it struck her, else 30.

**The attacks** (`stalker/attack.fs`): a combo is up to 4 moves; each needs her within 50 (or
the ground he gains; a hold any distance). A blow ($E00..$E03, $205: kind 2 knocks her down, 4
throws her) swings at its animation's key: his swing sound ($10), and whoever its bones reach
(within her height and radius widened by its reach, in front of him) is sent a `hit` (its
stumble by its chance, its fright); a wall in the way: a thud ($2B), the combo over. Missed,
half its fright still reaches her (Pursuer_Threat: x 0.75 each 10 off, none past 40). A hold
($2300..$2302: kind 6) catches whoever is within its reach of him at its key (the grip: its
bone). Struck home (`hit-taken`): his cry ($B / $C / $D, bank 7), held still 5 frames.

**Struck** (P3a, `stalker/struck.fs`; Pursuer_Hit, Pursuer_StateFlinch / StateKnockedDown /
StateHurt, Pursuer_FrameUpdate, Debilitas_GivesUp): a `blow` takes its damage from his health
and adds it to the damage he's had (to 999); a killing one (3) all of it. Its side - in front
or behind him, +2 a kick or hard one - picks his flinch ($1001 / $1000 / $1004 / $1003; in a
doorway $1002 / $1005); in a move that leaves him unshaken (his move table's byte $1D) a shove
only gets a grunt ($1C). Out of health he falls (forward $1804 with floor ahead, else back
$1800), lies there 1800 frames (hard 1350) - a blow then a twitch - and gets up ($1807 /
$1803) with his health back; in a doorway he's left at 1. A stumble ($8000, or her kick's
roll by how worn he is: his health over its most above 0.8 none, 0.6 2 in 100, 0.4 5, else 10)
hurts him 900 frames: he stands 2, walks $202 / $203, turns $405.., and growls ($1F) after
it. At his flinch's key, her blow, he may strike back: his table 8 (her in front) or 9.
After a blow's reaction, 20 damage or more (not held off, not in a doorway, a way out): he
gives up - his cry ($20), held off (mode 4, 900 frames), out by a door he can go by, the
damage count back to 0.

**Taking her** (P2b, `stalker/grab.fs`; Pursuer_StateFaceFiona / StateWalkOn, Pursuer_CloseOnFiona):
her floor not free for him (flags $80001) - his attack tables 6 / 7 instead (7 from the panic's
stage 4). Else he closes on her; within 20, facing her within 20 degrees, he asks her (`seize`:
6 by the hand, 8 walking) - she may not be had (out of reach, the stalkers blind): his table for
being near her (3); given up after 120 frames. Refused (`seize-refused`): his attack tables.
Taken by the hand: his taunting hold - $1900, then $1901 again and again, a fright to her
each time (10 the first, 5 after); she breaks free (`broke-free`) - he lets go ($1903, held off
90); the sixth - he holds on ($1904) while she's dragged off. Taken walking: his grab ($1A01),
holding on while she's carried off. Leaving any of it he lets her go (`unhand`).
His lunge ($1306): in the panic's stage 5 on into his seize (combo 8: $E05, kind 3 - she's
seized outright). His grab ($E06): at its key, Hewie by his hand (bone $1E, within 5) is held
(a `hit` of kind 6, from behind); over at its end, losing her, or her 60 off.

## Design notes

- The original's request blocks between characters (Relation_Request, the progress' slots)
  become messages with replies: a blow is `hit` (its kind, damage, a stumble), taken or not; a
  grab is `seize`, answered by Fiona.
- His tables (moves, attack tables by her state, his chances) are data from the executable,
  generated once like Hewie's (`tools/`).
- The old new-src port (archived: tag `new-src-final`, `new-src/scripts/pursuer/`) is a behaviour reference only: it keeps the
  original's offsets and vtable; this one is written in game terms.

## Status

P1 done: P1a (in/out, shown, growl, `stalker-here`), P1b (senses), P1c (modes, search route,
walking), P1d (out of sight from room to room, knocking, coming in). P2a done: the chase and his
attacks, with Fiona's F4a reactions. P2b done: taking her (by the hand, walking), his lunge
and grab, her being led, dragged, shaking free, seized outright, with Fiona's F4b.
`tests/stalker/test_stalker.fs` (42). Later parts kept with
their phases: his footsteps heard through the walls and the rooms a stalker keeps to (+0x314:
P4), the knock frightening her (with her fear's sounds), Hewie held (H3). P2c done: the game
over (`docs/subsystems/gameover.md`) - dragged or carried off, held, the sequence, back to the
title. P3a done: struck by her
(`fiona/defend.fs`, `stalker/struck.fs`; 48 tests). P3b done (`stalker/doorway.fs`): his use of
the doors in her room, round to her across his own room, slammed doors; also the coming-in
spot fixed (below). P3c done (`stalker/bitten.fs`, with Hewie's H3): Hewie's attacks taken or
refused, bitten, throwing him off, his temper and going for Hewie; his blows reach Hewie too.
`tests/stalker/test_stalker.fs` (58), `tests/hewie/test_fight.fs` (19). P4a done (below;
`tests/story/test_stalker_words.fs`, 13). P4b begun: Daniella (below;
`tests/stalker/test_daniella.fs`, 7), his footsteps through the walls and his lair (below; 3
more in `tests/stalker/test_stalker.fs`, 61). Next: P4c.

**His footsteps through the walls** (Pursuer_FootstepsThroughWalls, Character_RouteExitPoint):
out of sight making for a door of her room, within 80 of it, each 10 he closes in a step (his
sounds 0 / 1 in turn, bank 7) out beyond that door - along its spot 2 -> 1, at his way left plus
100 (an open door) or 150 (a shut one); not while silenced or the world is held. (The original's
loudness was meant to grow; it is always full.)

**His lair** (vtable +0x314, Debilitas_PickDestination, Pursuer_TravelOffscreen /
Pursuer_Pace): held off (mode 4), he heads for the nearer of his own rooms by route (Debilitas
castle-2f-2 $20 - its spot triangle 796 - and castle-1f-2 $02 - 794; Daniella none) and, out
of sight there, travels no further while it lasts; coming into play there held off he walks to
its spot. (The summoner may still take him: 5 s held off away from her.)

**P4b - Daniella** (src/game/daniella.c; her vtable differs from Debilitas's in about 45
places). *Her kit* (`stalker/tables.fs`, generated by `tools/stalkertables.py` with a set for
each stalker; his `kit` field, the words `moves`, `combos`, `gestures`, `table`, `temper-table`,
`pursue-row`, `throw-moves`, `grip-offsets` read through it): her attack tables by the hard
setting and her rage (Daniella_AttackTable / SetRage: 4 x 17), 13 moves, 18 combos, 6 gestures,
her temper with Hewie (normal and hard), her chase table by the panic's stage (+0x1730: four, by
the hard setting and her rage), her throws off Hewie (moves 11 / 12), Hewie's grip spots by her
(Daniella_ActionOffsets). *Her numbers*: a turn of 4 degrees a frame (Pursuer_TurnRate), her
reach for Hewie 12, never giving up after damage (Pursuer_GivesUp), her walks the Pursuer's
(slow in every mode: $206 after her, $201 else; $205 in a rage - Debilitas too). *The
Pursuer's chase* (`stalker/pursue.fs`; Pursuer_ChaseDecision, BehaviourStalk, HoldOff,
StateWalkThen404) for her - Debilitas keeps his own: close and in front of her, not panicking
past stage 3 - standing to her; else by her row walking at her ($200) or stalking her ($201) for
its frames; walking, her time out or Fiona past 100 - stalking; stalking, her time out within
100 - her table 10; touching - a step back facing her ($404), and the row again once 5 further
off; standing - turned to her, the row again out of the ground she gains or out of 20 degrees; a
combo, a taunt or her way round over - the row again; within reach in front on her floor - her
attack by Fiona's plight. Not yet: her model's own parts (hair, the hanging parts: DaniellaModel),
her door behaviour (Daniella_DoorBreak), her later kinds (34..36) and their files, her own
search spots and own rooms.

**P4a - the story's words about him** (`scripts/story/words.fs`, src/game/event.c). He tells
each frame (`stalker-doing`) his room, move mode and part, condition, health and its most, kind, his alert (his mode: Progress_StalkerAlert) and the exit he
came in by (+0x14D4); the room keeps it, with Fiona's and Hewie's conditions from their own
broadcasts. Conditions: 0x55 in the game (told within a frame), 0x54 his kind (the kind
partner-load put in slot 2: `slot2-kind`), 0x3C his kind and in this room, 0x10 his alert ($FF
not in the game), 0x65 free (here, not leading her, not bitten or down - his triangle not
checked), 0x41 a condition (Fiona, Hewie, the stalker), 0x1E / 0x52 out of health / at its most
(the stalker), 0x30 the exit came in by (Fiona: the room's; the stalker), 0x45 out of sight (not
here, or off the window: `on-camera?` over `to-screen`; `point-on-camera?` too). Orders, sent to
him (`stalker-order` etc.): 0x2F 0 after her (Pursuer_GoForFionaStance0: in her room his chase,
elsewhere to her room), 2 searching here / to her room (ChaseFionaHere), 3 searching here /
waiting about away (StartSearch) - up from down; while struck, bitten or leading her his mode
only; 0x2A his mode's clock; 0x30 knocked down out of health (Pursuer_KnockedDown: in her room);
0x31 rage (stance 3, walk $204); 0x15 started again (Pursuer_Activate: waiting about, 900, no
rage, health back if none); 0x47 his condition (Hewie too); 0x48 / 0xB8 health; 0x3D silent (his
own sounds). The stalkers' kinds map to their actors: Debilitas 2 6 7 27, Daniella 3 34 35 36.
He broadcasts `moving` at each frame's end (0x7B `char-wait-motion` waits on its event keys).
Not yet: `char-activate` for Hewie and the cast, `char-in`, the cast's words.

**P3b - the doors in her room** (`stalker/doorway.fs`; Pursuer_BehaviourDoors' steps:
Pursuer_NextPathExit, Door1C8, DoorLineUp, DoorGoThrough, DoorFace, DoorOpen, DoorThrough,
DoorPush, Door1C4, ExitGoTo, Debilitas_ExitDone, DoorWait). Out of her room on foot by an exit
(giving up, held off, or going round): walked to its spot 12 inside (`exit-spot` 2; the spots
are 0 4 inside, 1 12 beyond the door, 2 12 inside - the frame faces the exit's event area,
beyond the door; with no spot inside, straight to the door), then the door as he finds it
(Npc_ExitKind): open or a doorway - on to its spot beyond (1, else 0), through; shut - lined up
at his spot for its animation by the side he is on (Doors_Side: 0 behind it, kind 0; 1 in
front, kind 2), turned to its heading, its handle asked for (`hold-door`), his animation
($600 / $602) with the door swinging along it (`use-door`) until it rests, let go open, through;
held by someone - he waits by it, looking again every 15 frames, 300 and he gives up on it;
not for him (locked against him, locked, stuck) - not that way again, and on as after a blow.
*Fixed with it*: coming into her room by an exit he is put on spot 2 (12 inside) facing in, by
the door he made for facing it (Pursuer_BackOnMesh: Rooms_ExitTriThrough) - before, spot 1,
which is beyond the door.

**Round to her** (Debilitas_HeadForFiona, Npc_SameRoomOtherSide): she across his own room -
the halves the exits they came in by open onto differ (`exit-way`; hers from `entered-room`) -
after her and no way to her on foot, his route to her half (`route-sides`, want: her half), out
by its first door on foot; seeing her on the way, after her.

**Slammed** (Pursuer_Hit kind 5; Pursuer_StateHitAtDoor / StateBargedThrough): a `blow` 5 (3
damage, how: the exit) - after her again (mode 0). In its way (within 9 of the door, or on its
passage) and it not locked against him: knocked on through it ($1004), pushed 3 a frame away
from her side until clear of it; where it leads out of the room, out (Pursuer_ThroughDoor),
stunned there 150 frames (+0x1664); between his room's halves, on. Else staggered back from it
($1005) and on. Out of health: not below 1 (knocked out of the room: as he gives up, health
back, held off - his beaten state not modelled). Off screen, the door he makes for slammed
with him within 15 of it: its damage (not below 1), after her.

**P3c - Hewie** (`stalker/bitten.fs`; Pursuer_EventConcerns kind 9, Pursuer_GrabOrder /
GrabHewieSpot, Pursuer_StateHewieBites / TurnAnim / HewieHolds / BackOnFeet, Pursuer_Hit
kinds 6 and $B, Pursuer_BehaviourHewie). Hewie's attack asked (`dog-attack`, its kind 10..15):
in her room, not bitten, down, reeling or leading her, he takes it - the spot by him Hewie
leaps to (`dog-grip`: his offset for the kind - Debilitas_ActionOffsets - turned round from
Hewie's side, the other side head on and at the leg, 10 degrees at a time either way, until
one is on the floor and Hewie can run straight there) - else `dog-refused`. Bitten: his
reaction for the kind ($1700 an arm 10 / 11, $1703 the other 12 / 13, $1706 head on 14, $1708
the leg 15), turned to the grip's heading in 8 frames' worth (the leg 12), his temper up (his
table: `temper-table`); each time round it his temper up again; at its last frame (kinds 10..13,
not blind) he may throw Hewie off - the chance the holds so far x 10 (+0x16E8) + 25 x his
temper over what he stands - his throw (+2: $1702 / $1705) striking Hewie (a `blow` $B, its
move's damage 15). Hewie letting go (`shaken-off`): back on his feet (+1), down if out of health
(the leg: down at once), then on as after a blow. Hewie's bites (`bite`): their damage, a hard
one a stumble (hurt 900), his cry $1D. Growled or barked at (`snap`; within 100 of Hewie, not
struck, searching or waiting about): temper +1 (+3 while she hides). His temper past what he
stands (Debilitas 20; Daniella 200, hard 80) and free: after Hewie - toward him, a blow ($E00 or
$E01, his combo at Hewie: `at-dog`) within reach facing him; the blow over, Hewie gone, or 300
frames - his temper back to 0, on. His blows reach Hewie as they reach her (a `blow` with the
move's kind, damage and stumble). *Not yet*: his attack tables at Hewie (his `$1602` taunt, the
holds), Daniella's own temper table (Debilitas's for now), Hewie's tackles adding to his temper
(Pursuer_AddBiteDamage), the leg's down animation ($1709: his fall instead).

## P4c: Riccardo and Lorenzo (`scripts/stalker/riccardo.fs`, `scripts/stalker.fs`)

Each stalker's own is now his: `own-chase` / `own-run` (his chase's start and step - 0 the
family's: Debilitas's or the Pursuer's), `own-acts` (his own actions, `$1000..` of his tables: a
table of xts - 0 Debilitas's), `attack-range` (vtable +0x2F4; 0 the family's 24) and `own-frame`
(his own each frame). The kits (`tools/stalkertables.py`) carry two more: Riccardo's waits
(+0x1824) and his flurry's chances.

**Riccardo** (kind 4; riccardo.c): 100 health (hard 125), 3 across, 17 tall, his model O_RCG/RCG_000
(its 74 motions; motion table $41A2C0), reach 20, reach for Hewie 12. His walks: standing 1
(hurt 2), $200 (in his rage too), slow $206 seeing her within 100, else $201.
- His chase (Riccardo_ChaseDecision / StateBehaviour / Riccardo_Chase): at the panic's stage 5 the
  Pursuer's. Out of sight of her: after her. Further than 100: stalking her ($6) his far wait
  (300); near (60) and seen by her: standing to her (1), coming on his waits' count (60) of times
  before walking at her; else by his chance (20) walking at her (60) or stalking her (150).
  Standing: every 60 frames a lunge by his lunge table (75 within 150: his table 10); turning to
  keep her in front. A wait run out within 100: a lunge, or walk / stalk again. Within his reach,
  in front, on her floor: his attack table.
- His own actions: `$1000` his flurry (Riccardo_StartFlurry: 1..7 blows by his chances; on her
  floor his swing $E00 / $E04 at her, off it a slam down at her $2302 / $2304, one blow; at
  Hewie on his floor his swing $E06). At a blow's key (Riccardo_StateBlowFiona / BlowHewie /
  Blow): Hewie in the way of her takes it; her by how near (Riccardo_AttackForDistance: within 10
  his grab 3, else 6 caught - below stage 4 - or 4 a hard blow, a stumble three times in four;
  6 the heavy move with its stun); crouched under it, it passes over; after it another while
  she's in front. `$1001` his lunge ($1300; at stage 5 into his move 5).
- His frame (Riccardo_Update): his rage at the panic's stage 5 (his roar $26), out of it below;
  his snort ($24) at $600's key; his hammer's impact (key $20): a noise ($40) where he stands.

**Lorenzo** (kind 11; lorenzo.c): the Pursuer's chase with his tables; 50 health (hard 75), 5
across, 20 tall, his model O_LRO/LRO_000 (motion table $4246C0), turning 2 degrees a frame,
reach 18, reach for Hewie 12; no temper with Hewie in his tables (the game fills them at run
time), no throws of Hewie.

**Lorenzo's other class** (kind 10; lorenzo.c Lorenzo2_*, `scripts/stalker/lorenzo2.fs`): 250
health (hard 400), 4 across, 18 tall, his model O_LRY/LRY_000 (motion table $424490); the
Pursuer's chase with his kit (his moves by the hard setting: a kit each). From the panic's first
stage his "rage" (mode 2: his rage tables, his reach 18, for Hewie 12); else reach 70 (hard 40),
for Hewie 60 (30) - his attacks from afar. His own actions: `$1000` sinking ($1303) to come up 10
short of her ($1305; nearer than 30, his move 1, or with no way to her a look about), `$1001`
sinking ($1304) to come up by her (5 short) in his sweep ($E02: at its key she and Hewie within
its reach, over his floor, are struck once each - a stumble by its chance), `$1002` his reach
from afar ($E01: aimed where she is at its 12th frame; at its key whoever is within its reach
of there). Under the floor 15 frames, unseen. **Kind 39** (lorenzo.c Kind39_*): the same class
with its own tables, 250 health (hard 300), no "rage" (reach 18, for Hewie 12); its sweep
strikes whoever it sees.

**Riccardo's other class** (kind 37; riccardo.c Kind37_*): his chase with its own tables, near
him at 30 (Riccardo 60), its model O_RCT/RCT_000 (motion table $42AED0), 100 health (hard 125);
struck to 100 damage it is alerted - hurt, its alert tables and waits (the kit's waits by the hard
setting and alerted) - and calms below. **The TintStalker** (kind 23; tintstalker.c): the
Pursuer's chase with its tables, the same look, seen only a while (150 frames) after each turn of
what it does, then gone (`unseen`). **Kind 12** (lorenzo.c Kind12_*): the Pursuer's chase with
its tables, its model O_LRF/LRF_000 (motion table $443FB0), 9999 health and never hurt; in her
room, free to go for her (not in costumes 6 / 7), it seizes her (her fright 100).

The story's kinds: 4 Riccardo, 37, 23, 11 Lorenzo, 10 Lorenzo's other class, 39, 12. Checked by `tests/stalker/test_riccardo.fs` (21 with kinds 37 and 23), `tests/stalker/test_lorenzo.fs` (10 with
kind 12), `tests/stalker/test_lorenzo2.fs` (12 with kind 39). *Not yet*: Riccardo's cry hits and his hammer's debris and
jolt (effects), his blow at a room object (gEvents +0x68), going to the door she hides behind;
Lorenzo's footsteps only in his walks (the stalkers' steps on screen aren't written yet); the sparks, dust,
the sink's rings and the burst of Lorenzo's other class (effects), his travel under the floor
(he comes up at once after 15 frames); the screen tints of kinds 37 and 23 (the TintStalker's
fade in and out: shown or not), kind 12's marker; the situations of the ending (the TintStalker's
table 15, its doors). The cutscenes' cast beyond slot 2 (`char-load-2`, `char-hand-over`,
`char-loaded?`) goes with the event characters (the cast): every use is of figures that aren't
stalkers (kinds 15, 28), loaded into slots 3..5 as the cast is.
