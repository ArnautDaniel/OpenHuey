# The stalkers

## Purpose

The pursuers: Debilitas (the first chapters), then Daniella, Riccardo and Lorenzo. One family -
the same senses, search, chase, attacks and reactions - with each one's numbers, animations and
special moves. The original's shared base is `src/game/pursuer.c` (the NPC and Pursuer layers,
~14,000 lines); each stalker overrides part of it (`debilitas.c`, `daniella.c`, `riccardo.c`,
`lorenzo.c`). Debilitas is built first.

By name: the kinds in `scripts/stalker-names.fs`; his actions (the original's kPursuerSteps
numbers), his tables' situations and where his head looks in `stalker/names.fs`; the family's
animations and sounds in `stalker/anims.fs` and `stalker/sounds.fs` (each one's own in his file);
a kit's tables as structures (`/kit`, `/move`, `/row`: `tools/stalkertables.py`); her plight, the
blows' kinds, Hewie's attacks and `seize`'s types in `messages.fs`.

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
| `stalker-summoned` | `( room exit found plan -- )` | in, from the summoner | Come in out of sight by that exit: after her, or waiting about (Pursuer_IntoRoomByEvent) - heading for her, or past an exit at random (`route-at-random`: Pursuer_PlanWhere's mode 3). |
| `stalker-doing` | `( room mode sub cond hp hp-max kind -- )` | out, broadcast each frame in the game | For Hewie: his room, how he moves (3 off screen, 4 struck, 8 taking her), what (9 bitten, $A down, $18 leading her), condition, health, kind. |
| `blow` | `( kind damage how -- )` | in (Fiona: 1 shove, 2 kick, 5 a door - how its exit); out, to Hewie | Struck; his blows (and his throw, $B) strike Hewie. |
| `dog-attack` / `dog-grip` / `dog-refused` | `( kind )` / `( tri x y z yaw )` / `( )` | in / out | Hewie's attack asked; taken (where Hewie leaps to) or not (P3c). |
| `bite` | `( damage hard -- )` | in, from Hewie | A bite while he holds on. |
| `shaken-off` | `( -- )` | in / out | Hewie let go / thrown off. |
| `snap` | `( -- )` | in, from Hewie | Growled or barked at: his temper. |
| `dog-tackle` | `( -- )` | in, from Hewie | The story's tackle (Pursuer_GrabOrder's slot command 2 / 7): the leg's spot answered (`dog-grip`), his behaviour cut short; or `dog-refused`. |
| `stalker-ties` | `( scripted apart linked -- )` | out, broadcast each frame in the game | A script or a scene has him (+0xE0: `scripted`, `scene`); out of contact (+0x2A: his door animation till he lets the door go, pushing it on barred floor, on a ladder, off screen - Fiona isn't pushed out of him); a joint action pending (Progress_IsLinked: his seize of her awaiting her answer; Hewie's attack or tackle the frame he takes it) - for the story's "free" (cond 0x65). |
| `hewie-doing` | | in (Hewie's broadcast) | His condition: down, his chance roll's first row. |
| `noise` | | out, to acoustics | His steps, growls. |
| `heard` | | in, from acoustics | A noise he heard (he listens above a threshold). |
| `fiona-doing`, `hewie-doing` | | in (broadcasts) | What they are doing (whether she can be struck, grabbed...). |

Facts he reads: bodies (where Fiona and Hewie are, their rooms and triangles), the nav mesh (his
paths, what he can see across: a straight walk), the room graph (his route through the doors),
doors (open, locked: `door-state`).

## Rules (from src/game/pursuer.c, debilitas.c)

Debilitas: model `O_DB0/DB0_000`, motion table $3D89A0, radius 5, height 20, head bone $16. His
model is the kind he is loaded as (`debilitas-dress`, Pursuer_ModelFiles): kind 6 (Debilitas2)
`O_DB1/DB1_000`, kinds 7 (Debilitas3) and 27 `O_DB2/DB2_000` - the same skeleton, motions and model
class - with their own textures (the hard setting's `..H`), events and sounds. Loaded as a kind
he is set up as it (`debilitas-setup`, each time - a new Pursuer in the original, health full):
his kit (`tools/stalkertables.py`: each kind's attack tables by the hard setting, moves,
gestures, combos, base chase, temper, animations' sounds, knocks), his numbers and his kind
(`my-kind`, the one the cutscenes' cast finds him by: `char-model`, speech, reflections):

| Kind | Setup | Health (hard) | Chase | Gives up | Rooms | Hewie: tolerance, throw (hard), time | Away, cooldown | After her (hard), down (hard) |
|---|---|---|---|---|---|---|---|---|
| 2 | Debilitas_Setup | 70 (110) | his own | 20 damage | $20 (796) | 20, 10, 1800 | 4500, 90 | 300 (540), 1800 (1350) |
| 6 | Debilitas2_Setup | 90 (135) | the Pursuer's | 80 damage | $20 (796), $02 (794) | 30, 10 (15), 1800 | 10800, 150 | 300, 1800 |
| 7 | Debilitas3_Setup | 110 (165) | the Pursuer's | never | none | 40, 12 (18), 1800 | 9000, 150 | 300, 1350 |
| 27 | Kind27_Setup | 110 (165) | the Pursuer's | never | none | 30, 12 (18), 1800 | 9000, 150 | 300, 1350 |

**Senses** (P1b, `stalker/senses.fs`): he sees 150 ahead, 60 degrees either side, past no wall
(triangles $40080; $40088 while she hides) to her middle or one of 9 points round her far side;
he can reach her within 20 on foot (10 half hidden), straight there; Fiona and Hewie are
measured on foot in turn, a frame each. He hears noises above his threshold (Debilitas 0,
Daniella 12) through the acoustics actor.

**His head** (`stalker/head.fs`; Pursuer_HeadLook, Pursuer_LookAt, Motion_LookAt, Motion_EaseTilt,
HumanModel_AdjustBone): by his action's look mode he looks at Fiona while he sees her, at Hewie
while he sees him (5 above him: the tilt too), at his goal in view within 30 degrees, else
ahead; from his eye (16 up; Lorenzo in his chair 3.5), the turn only for Fiona and the goal, small
turns none, 8 degrees a frame (Pursuer_TurnRateFast). Each frame Pursuer_Think first eases his
head back (no scripted look: Model_EaseTilt) and only then does Pursuer_ModelUpdate's HeadLook turn
it, so in the original a stalker's head turns one step (8 degrees) at most - and his sight, which
looks where his head does, with it. As his model is posed (the engine's `head-bones ( id head
neck -- )`, `head-turn ( id -- ) ( F: pitch yaw -- )`: `humanhead_adjust`) the head bone (+0x8B0)
turns in its own frame by the tilt and half the turn, the bone below it (+0x8B4) the other half
about its own place: Debilitas's kinds $16 / $F, Daniella's $1F / $15, Riccardo's and the tinted
ones' (Kind23Model: 23, 37) $23 / $1A, Lorenzo's other class (10, 39) $2B / $22, kind 12 $13 / $B;
Lorenzo's chair turns his own. Checked in `tests/stalker/test_stalker.fs`.

**Modes** (P1c, `stalker/search.fs`, Pursuer_ModesSearching in the room being played): seeing
her he is after her (mode 0) and his clock restarts (his setup's +0x16D4, Pursuer_Caught:
Debilitas 300 frames, hard 540; the others' in their sections); out of sight it
runs down, then he searches the room (mode 2, 600 frames' clock, 2..5 stops); his route over
he waits about (mode 3, 900 frames), then searches again. Heading for her room (1) he searches
it once in; held off (4) in her room he is after her again when his clock runs out - out of
sight he stays held off, going home, till the summoner takes him (*by design*: held off he keeps
away). She following him into his room held off, he goes on out by his way home - not a search.
Brought in by the summoner his health is full (a stalk afresh); till then a beating stays with
him. Coming into the game
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
a doorway or open - through; shut - through, but into her room he first knocks (a sound by
his kit's knocks, +0x16B0: Debilitas $28 or $23, Daniella $21 / $19 / $1A, ...; none of his
own the Pursuer's $21; bank 7, at the door on her side) and waits 60..120 frames. Into her room he comes
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
(it struck her) or 13 (taunts), held off 90 frames if it struck her, else 30. Round her ($1A), or
his third taunt ($19) with its gesture's flag, her more than 100 off: stalking her for his chase
table's eighth (seen near). Her panic calming down (the panic's stage 5) his chase is the
Pursuer's (Debilitas_ChaseDecision: `stalker/pursue.fs`) - as it begins, and at the end of any
step of his own chase that leaves him free to attack.

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
$1800), lies there his setup's frames (+0x16D0, Pursuer_NoGiveUp: Debilitas 1800, hard 1350;
"hard" throughout is the hard mode, `hard-mode?`: story flag $AF) - a blow then a twitch - and gets up ($1807 /
$1803) with his health back; in a doorway he's left at 1. A stumble ($8000, or her kick's
roll by how worn he is: his health over its most above 0.8 none, 0.6 2 in 100, 0.4 5, else 10)
hurts him 900 frames: he stands 2, walks $202 / $203, turns $405.., and growls ($1F) after
it. At his flinch's key, her blow, he may strike back: his table 8 (her in front) or 9.
At the end of each step - a blow's reaction, his striking back, any move of his chase
(Pursuer_FrameUpdate) - 20 damage or more (kind 6 80: DEBILITAS_GIVE_UP_HITS; not held off, not
in a doorway) and a way home: he gives up - his cry ($20), held off (mode 4, 900 frames), the
damage count back to 0 - and goes out by the first door of his route to the nearer of his own
rooms (Debilitas $20; kind 6 $20, $02; in one of them, to its spot: Debilitas_PickDestination;
there already, he stands). No route home: he doesn't give up. A door of his way home he can't
line up at (no way to his spot by it) is avoided as Pursuer_OnToNextExit marks it, and his way
home planned again without it - the doors avoided kept till he next gives up. While he reels
(struck, down) his search waits - the original picks its behaviour only at a step's end - so
he lies his whole time down. Searched with no gift due (`stalker-down-over`, the story's 0xB6)
his time down is over. The others never do (Pursuer_GivesUp: kinds 7 and 27 too).

**Taking her** (P2b, `stalker/grab.fs`; Pursuer_StateFaceFiona / StateWalkOn, Pursuer_CloseOnFiona):
he walks on along his way while his motion fades in (Pursuer_WalkOn) - by the hand, then his
stand fading in too, turning to her meanwhile - and then looks once: she on a ladder, his attack
tables; within 20 and facing her within 20 degrees (Debilitas_ReachFiona, Debilitas_AttackAngle:
vtable +0x2E4 / +0x2EC) - she may not be had (out of reach, the stalkers blind): his table for
being near her (3); her floor not free for him (flags $80001): his attack tables 6 / 7 (7 from
the panic's stage 4); else he asks her (`seize`: 6 by the hand, 8 walking). Not within 20 or not
facing her: a look round ($1A00, action $16 - Pursuer_StateLookAround: a fright of 20,
Pursuer_ThreatAmount). Refused (`seize-refused`): his attack tables. (He decides at about the
ground he gains - 16 and his next 5 frames' strides - so a take from 15 or so off is the
original's: she is led to her spot by him a fifth of the way a frame, Fiona's led_step.)
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

Also (2026-10-13): the doll draws him (`drawn-to`: searching the room being played, seeing it and
with a way there - his search's one stop, `go-for-doll`); the story's 0x20 through blocked floor
at doors (`noclip`: his floor mask, `my-floor`, to 0 and back); his blow under way told
(`stalker-blow`, after `stalker-doing`); whether he has her in sight or reach (`stalker-sees`, +0x1544:
the story's cond 0x23 from her to him asks it - her crouch, the hiding places' "found"); a door
that won't open for him shakes the pad ($80, 15).
Leads (`stalker/leads.fs`; the original's +0x16C9..+0x16CC): a noise heard in the room being
played pulls him to it when it pulls at least as hard as the lead he follows (`pull-fiona` 5,
`pull-noise` 4, `pull-hewie` 1); a noise already in view is ignored; his alarm is kept at its
highest (`saw-her` 7 .. `heard-hewie` 1). A new lead starts him: a jolt and his start animation
played out (or 90 frames standing, crying $21), then on to it; seeing her, the $1601 gesture
before the chase. Forgotten as he comes in, goes out or is set going. Beaten (+0xC4 2: knocked
out of health) is told to the summoner in `stalker-status`; away from her 5 s, it takes him as
beaten (kind 2).

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
`tests/stalker/test_stalker.fs` (64), `tests/hewie/test_fight.fs` (22), `tests/stalker/test_leads.fs` (7), `tests/stalker/test_tackles.fs` (9). P4a done (below; its opcode words'
own test went with them in 0.2.20). P4b begun: Daniella (below;
`tests/stalker/test_daniella.fs`, 15), his footsteps through the walls and his lair (below; 3
more in `tests/stalker/test_stalker.fs`, 61). Next: P4c.

**His footsteps through the walls** (Pursuer_FootstepsThroughWalls, Character_RouteExitPoint):
out of sight making for a door of her room, within 80 of it, each 10 he closes in a step (his
sounds 0 / 1 in turn, bank 7) out beyond that door - along its spot 2 -> 1, at his way left plus
100 (an open door) or 150 (a shut one); not while silenced or the world is held. (The original's
loudness was meant to grow; it is always full.)

**His lair** (vtable +0x314, Debilitas_PickDestination, Pursuer_TravelOffscreen /
Pursuer_Pace): held off (mode 4), he heads for the nearer of his own rooms by route (Debilitas
castle-2f-2 $20 - its spot triangle 796: kDebilitasRoomSpots; kind 6 that and castle-1f-2 $02 -
794: D_0041BE40; the rest none) and, out
of sight there, travels no further while it lasts; coming into play there held off he walks to
its spot. (The summoner takes him: 5 s held off away from her.)

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
StateWalkThen404) for her - Debilitas keeps his own but in the panic's stage 5: close and in front of her, not panicking
past stage 3 - standing to her; else by her row walking at her ($200) or stalking her ($201) for
its frames; walking, her time out or Fiona past 100 - stalking; stalking, her time out within
100 - her table 10; touching - a step back facing her ($404), and the row again once 5 further
off; standing - turned to her, the row again out of the ground she gains or out of 20 degrees; a
combo or a taunt over - the row again; round her ($1A), or her third taunt ($19) with its
gesture's flag (+4 of her gesture table's row), more than 100 off - stalking for the row's frames,
else over - walking at her for the row's; within reach in front on her floor - her
attack by Fiona's plight. *The door she comes into Fiona's room by* (Daniella_DoorBreak /
ExitArg; Riccardo's and the TintStalker's alike): pulled shut behind her once she is clear of
its area and it stands open (`shut-behind`, the doors' `pull-shut`); in her rage left open
(`door-manner`: Riccardo, the TintStalker `shuts-doors`; Daniella `shuts-doors-calm`; the rest
`leaves-doors`). Her look: the model O_DNL/DNL_000, its textures DNL_000 (hard: DNL_000H -
`daniella-model`). *Her later kinds* (Kind34_* / Kind35_* / Kind36_*; actors `kind34`..`kind36`,
the story's slot 2 mapping each to its own): her chase and look (34 and 36 the model DNL_001,
its sounds too), each its own tables (`kind34-kit`..: tools/stalkertables.py reads their attack
tables from daniella.c's arrays) and numbers - health 120 / 65 / 65 (hard 200 / 80 / 80), what
she stands of Hewie 200 / 120 / 120 (hard 65 / 75 / 65), her time after him 6000 / 3000 / 3000
(hard 6000 / 600 / 600), her time away and cooldown. 34 has her rage and pulls the doors she
comes in by shut while calm (open on the hard setting); 35 and 36 have no rage (their tables by
the hard setting only) and leave doors be. Held off she has no rooms of her own (nor Riccardo and the rest: NPC_PickDestination's
Npc_HeadRandomRoom): out of sight she goes on by a way out at random, not into Fiona's room
(`wander-off`; the original picks a room at random among those near). Her search spots are the
rooms' own, as everyone's. *Her hanging parts* (DaniellaModel_Springs; `src/game/cloth.c`, her
model and her later kinds' - O_DNL/ - alike): her hair, two strands of five side by side
(HairPoint: each follows its root's moves by its stiffness, 0.8 at the root to 0, kept about
0.44 from the one beside it, off her back's five capsules); what hangs from her waist, two
strands of three drawn along bone $1F (HangingPart, off two capsules and two spheres); a part
sprung to bone $17; two bones hanging ($2F, $30); 50 steps to settle afresh - as he is loaded,
put somewhere (`settle` in `stalker/travel.fs`: Pursuer_Place's +0x850 - through an exit, anywhere,
or by the story's `place`) and at a scene's cuts. Like Fiona he blinks and his hands take his
motions' poses (`act.alive`, on as he is dressed: Pursuer_ModelUpdate's Motion_Eyes / Motion_Hands);
Riccardo's right hand's own shape is 8, Daniella's 1 or 2 by which of her parts 17 / 31 she has. Down on her back
($1800..$1803) her back's capsules bend and her hair falls forward, each frame by her animation
(the engine's `cloth_watch`; DaniellaModel_Vt3C); the story sets them flat or standing for its
own moves (its 0xCC op 4, `cloth-back!`; DaniellaModel_BackCapsules). The others' hanging parts likewise (by their model files): Debilitas
(O_DB0, O_DB1, O_DB2: four from bone 10, the last three from the first's end, off two spheres on bone 2);
Riccardo (O_RCG: four in pairs off four spheres; his coat, twelve in pairs following their
anchors and tied about 1.9 to the first pair of their six, off five capsules down his back -
Part60; four more drawn along their bones - Part50); his look (O_RCT: kind 37, the TintStalker)
the same without those last four; Lorenzo (O_LRO: six in threes off eight spheres); his other
class (O_LRY: 24 swaying points, six groups of four, each pair swinging from bone 2 / 3 - $12 /
$13 the odd groups - out to its group's side, 5 .. 20 degrees freer down it: SwayPointB).

**Ladders** (`stalker/ladder.fs`; his action $12 - Pursuer_DoorApproach, WalkToSpot, DoorArmOpen,
Stairs, DoorAnim: for all their names, a ladder). No way to her on foot (Pursuer_TargetOutOfReach:
from his chase's standing, as he sets off at her - Pursuer_CloseIn -, all along the Pursuer's
ChaseTarget, and after her out of sight - WalkToGoal): round by a ladder - the first whose end on
her level has a way to the middle of her triangle and whose end on his has a way from him
(Npc_RoundThroughDoor); else a door between them (Npc_ExitsToTri, his action $B: of his room's
exits with a door, neither open and free nor locked, the first whose spot on one side has a way
to her and on the other a way from him - he walks to his side's spot, lines up, opens it as on
his way out, lets it go open and is after her again; `stalker/doorway.fs`); else, in a room of
two ladders or more, the first he has a way to, each one's ends in turn, the last tried first
(Npc_FindDoor); none - his chase's search of the room (Pursuer_ChaseFionaHere). She on a ladder while he is after her and free (Npc_OpenDoorFionaHides):
to its foot, else its top. He walks to beside that end - out from it by his family's offsets
(vtable +0x9C: beside its foot, beside its top, off at its top, off at its foot - the Pursuer's,
Debilitas's, Daniella's, Riccardo's) -, turns to it at his rate (move mode 3) and gets on ($700
at its top, $704 at its foot); climbs a rung at a time by the moves' own motion, up and down
(`body-climb`: $701 / $702 down, off within 20 of its foot - $703; $705 / $706 up, off within 4
of its top - $707), and is put out from that end. Whoever he touches on it is struck (kind 4, 5
damage: Fiona on it is knocked off). Struck on it he isn't felled (1 health at least). On his
way there - she off the ladders with a way to her on foot - he gives it up and stands. Its
footsteps: the rungs' (10), the floor's getting on or off with the foot over it. Then his chase
again (standing; the Pursuer by his row; the way there gone, the Pursuer searches). While he
goes for one or climbs, his modes wait. (Not here: the original's turn back half way, when another
behaviour re-aims him mid-climb, and its putting him at the nearer end when she leaves the room -
OpenHuey puts him by a door when she comes back.) `tests/fiona/test_ladder.fs` (29).

**P4a - what the story knows of him** (once `scripts/story/words.fs`, now the kit's -
`scripts/story/kit.fs`, `story/base.fs`'s slot 2; src/game/event.c). He tells
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
own sounds). The stalkers' kinds map to their actors: Debilitas 2 6 7 27, Daniella 3, her later kinds 34 35 36 their own (`kind34`..`kind36`).
He broadcasts `moving` at each frame's end (0x7B `char-wait-motion` waits on its event keys).
`char-activate` (0x15) starts Fiona, Hewie or the stalker afresh (a figure of the cast is in once
the story places it); `char-in` (0x98) - OpenHuey loads characters at once - only waits a frame. The
cast's own words are the event characters' (`docs/subsystems/cast.md`).

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
back, `beaten` set: the summoner takes him as beaten). Off screen, the door he makes for slammed
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
stands (Debilitas 20; Daniella 200, hard 80) and free: after Hewie (Pursuer_GoForHewie,
BehaviourHewie) - toward him, and within reach facing him an attack from his table for Hewie
(situation $B: a blow at him, or his grab, a hold of him; his combo at Hewie: `at-dog`); that
over (and his chance roll won), or his time for it up (`dog-limit`, +0x16D8: Debilitas and Riccardo 1800, Daniella 6000,
Lorenzo2 600, kind 39 300 - hard 360 -, kind 37 900, the TintStalker 1800 (TintStalker_Setup),
Lorenzo and kind 12 none), his taunt ($1602) and his
temper back to 0, on as before. Hewie gone from the room: out after him by the exit to his
(`follow-dog`; his temper back to 0, heading for her room after it); none, on as before. His blows reach Hewie
as they reach her (a `blow` with the move's kind, damage and stumble). Hewie's tackles (a `blow`
from him, not a bite or a hold: Pursuer_AddBiteDamage) put his temper up by where they caught
him (`tackled`: his temper table's in front, in front low, behind, behind low). Each stalker's
temper table is his own (`temper-table`: his kit's, hard its second). His animations' sounds
(Pursuer_AnimSounds; his kit's, +0x16AC, by motion): at a sound key ($1 the first, $10 the
second) in the room being played - a blow's swish, a fall's thud; a fall onto water a splash
($1D, bank 6). Hewie letting go of his
leg drops him on his front (`leg-down`: $1709, lying as $1806, up by $1807).

## P4c: Riccardo and Lorenzo (`scripts/stalker/riccardo.fs`, `scripts/stalker.fs`)

Each stalker's own is now his: `own-chase` / `own-run` (his chase's start and step - 0 the
family's: Debilitas's or the Pursuer's), `own-acts` (his own actions, `$1000..` of his tables: a
table of xts - 0 Debilitas's), `attack-range` (vtable +0x2F4; 0 the family's 24) and `own-frame`
(his own each frame). The kits (`tools/stalkertables.py`) carry two more: Riccardo's waits
(+0x1824) and his flurry's chances. Each one's clocks from his Setup (`clocks`: after her
+0x16D4, down when knocked out +0x16D0; normal / hard): Daniella 450 / 450, 750 / 300; kind 34
300 / 360, 900 / 1350; kind 35 600, 450 / 360; kind 36 300 / 600, 900 / 360; Riccardo 300 / 540,
1200 / 900; kind 37 300 / 360, 150; the TintStalker 300, 1200; Lorenzo 600 / 450, 510 / 360;
Lorenzo2 900 / 450, 0 (up at once); kind 39 900, 0; kind 12 29970, 0. None of them gives up
after damage (Pursuer_GivesUp).

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
  his snort ($24) at $600's key; his gun's shot (key $20): smoke off his gun (BoneSmoke, `gun-smoke`), a
  noise ($40) where he stands, and where a shot aimed down strikes - 10 out along his gun (bone $31),
  on the floor - its debris, a brown cloud (DropletFlash, `debris-cloud`); aimed level, where the floor
  ahead gives out. The one his blow strikes, crying out, gets its mark (Riccardo_CryHit: `spray` at a
  height on them, big for $E05 / $1A01); Daniella's at her hand (Daniella_BlowEffect: bone $2D, 1.5
  or 3.5 out) as they cry or at her blow's key, on Fiona for her grabs.

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

**Riccardo's other class** (kind 37; riccardo.c Kind37_*): his chase with its own tables (its
reach the Pursuer's 24: Pursuer_AttackRange, not his 20), near
him at 30 (Riccardo 60), its model O_RCT/RCT_000 (motion table $42AED0), 100 health (hard 125);
struck to 100 damage it is alerted - hurt, its alert tables and waits (the kit's waits by the hard
setting and alerted) - and calms below. **The TintStalker** (kind 23; tintstalker.c): the
Pursuer's chase with its tables, the same look, seen only a while (150 frames) after each turn of
what it does (`unseen`), drawn by its tint (TintStalker_Tint / Fade: the renderer's layer $11 over
the room by its alpha out of 128 - `act.alpha`): shown, fading in to 112 (held whole, 130, while
beaten); its while over, fading out by 4 a frame, then gone. Kind 37's tint (Kind37_Update) pulses:
80 + 80 sin of its phase - whole for 150 frames after each turn of what it does, then its phase on
by 4 cos of it and up to 10 more degrees a frame. **Kind 12** (lorenzo.c Kind12_*): the Pursuer's chase with
its tables, its model O_LRF/LRF_000 (motion table $443FB0), 9999 health and never hurt; in her
room, free to go for her (not in costumes 6 / 7), it seizes her (her fright 100). While it is in
her room the Marker glows on it (Kind12_ShowUp / EnterRoom: the first kind, size 1; `mark-id`),
done once it has gone.

The story's kinds: 4 Riccardo, 37, 23, 11 Lorenzo, 10 Lorenzo's other class, 39, 12. Checked by `tests/stalker/test_riccardo.fs` (27 with kinds 37 and 23), `tests/stalker/test_lorenzo.fs` (12 with
kind 12), `tests/stalker/test_lorenzo2.fs` (13 with kind 39). Lorenzo's other class's effects (Lorenzo2_SlamDust / BlowSparks, LightRing, BurstA): his slam
($2301) eight grey dust clouds at his hand; his blows seven sparks about it (`spark`); where he
sinks away and comes up a ring of light (`sink-ring`); his reach from afar a burst where it lands.
Under the floor he is 15 frames, then up at his goal - as the original (no travel there).
Riccardo's shot at a target (Riccardo_StateBlowFiona with Events_NearestPoint): the story tells
its zones of a kind with bit 1 (`zone-target`: the barriers in the front garden, ...); the one
nearest her - within 50 of her, further than 20 from him, about his height, a straight way to it,
she and Hewie off the line to it - takes his shot instead of her: his point (Pursuer_AttackPoint,
`body-point!`) there 3 frames, where the story's zones see it (it breaks).
*Not yet*: the tints' flares (the renderer's layer
$11 sprites drifting about them: the PC port of the original draws none either). Built: **ways to floor not his** (every walk's
plan, `stalker/moving.fs` `plan`: NPC_PathLengthGoal / Spot / To - the Pursuers' and so his kinds'
GoTo and HeadFor): to a goal on a triangle of his mask the way goes to the walkable point nearest it
(Npc_NearestWalkable, the engine's `walkable`: within the door area of an exit of the room that
doesn't loop back, from the far side of that exit; else from the nearest triangle free of the mask -
the planner's search kind 7, PathPlan_StepCheapNear: cheapest first by the edges' costs, through
masked floor, ended by the first free triangle further than 30 from the goal's - then on toward the
goal while the floor is free, 0.1 short); his ways curved round their corners as the Pursuers'
are (Character_WaypointsCurve). Checked by `tests/stalker/test_walkable.fs`. Built: **the story's moves** (`stalker/scripted.fs`,
checked by `tests/stalker/test_scripted.fs`): the story's action for him (Pursuer_EventState's 5)
takes him at his next frame if he is in the room being played - else it is dropped; held
(+0xE0) his frame is Pursuer_Think's (`held-frame`: his senses, the move his step, his head,
footsteps, his model - no modes, no behaviour, no frame of his own, blows not taken). Taken
(Pursuer_BackToNormal, BackToStance) he lets go of her hand and of a door, Hewie is thrown off him,
the story's look is off, and he stands (his action 1), done once his stance has faded in. Each move
(`scripted-move`, Pursuer_EventCommand) clears his done (+0xE1): 2 standing; 3 / 4 the door at
exit a used from his side - opened if shut, shut if open (his animation $600 + its kind, $601 / $603
shutting; `stalker/doorway.fs` `script-door`), then standing; 5 / 10 walking to the spot on
triangle a (NPC_GoTo: off it, its middle) in the move's walk (b if one of the group's, $200
walking / $201 slowly, else his fast / slow walk: Pursuer_ScriptMove) along his way, turned on the
spot past 80 degrees (WalkToGoal, StepBack, CloseInGoal), there - another triangle a way under 0.1
left: put on it - turned to the move's heading at his turn rate, then standing (StateArrive); 6 /
11 the same to triangle a's middle, done once he is on it, walking on the spot at its end
(StateRunToTri); 7 an animation, 8 blended over b frames, 9 the same with no time of its own, 16
his stance over b frames - done at once, moved by its root motion; 12 / 13 the story's look - at
whom its moves last named (+0x100: 0 Fiona, 1 Hewie, else the look's point) - turning his head
8 degrees a frame before his own look (so standing - his action 0, no look of his own - it turns
fully; walking to his goal his own eases it back again); 14 / 15 turning to a character / the
move's heading in $200, then standing; 1 let go; 17 (the walk with an animation) not the
stalkers' - nothing, never done. Done is "standing faded in" for the stance moves (StateStand /
Move180). Let go (move 1 - the script over, its `scripted` off: Pursuer_EventReset, StanceByFiona,
PickStep) he is through blocked floor no more, his own sounds again, on as before (as after a
blow: struck.fs `on-as-before`). The scripts' 0x9D (`hold-anim`) poses him in an animation, shown
(+0xE0 / +0xE2: Pursuer_PlaceModel - his model alone, his body by his feet, not planted); a script
letting go doesn't free him, the story's 0x15 (`activate`: Character_Activate) does. The story's
0x1F hides and shows him (`show`, +0x29); a cutscene giving him back shows him again (its cast
made active: Cutscene_ReleaseActors). A stalker loaded into a figure's slot (3..5: Debilitas as
castle-1f-10's figure) is placed by the story, posed and shown there; his own behaviour never
runs (Progress_CharsFrame frames slots 0..2 only). As a figure he wears his kind's other model
(Pursuer_ModelFiles out of slot 2: its `_200` pack - the scenes' own animations, $9000 on - with
the play model's textures, `_000` / hard `_000H`; `stalker.fs` `wear-figure`), back in slot 2 his own
(`wear-own`). Debilitas 7 / 27's (`O_DB2/DB2_200.PCK`) isn't on the disc; the tables' second
texture set (`_200.TEX`) isn't used yet. Also: an action's look mode is set as it starts
even when it is "as it was" (0: no look of his own - his head eased back), as Pursuer_StartAction
does. Built: **his legs on
the floor** (`src/game/humanlegs.c`, as Fiona's - `fiona.md`; `stalker.fs` `fit-legs`): each frame
in the room played (Pursuer_ModelUpdate), on the floor but on a ladder or a script's walk through
blocked floor, his body is set at the floor under the lower of his feet (HumanModel_BodyFrames: on a
slope motion two points along his step instead - his Setup's +0x1694 / +0x169C slowly walking
$201, else +0x1698 / +0x16A0: Debilitas, kind 6, Daniella and 34..36, Riccardo, kind 37, the
TintStalker 8 / 1.5, 12 / 1.5; kinds 7 and 27 6 / 1.5, 13 / 0; Lorenzo's classes and kind 12
8 / -8 both) and his feet planted, his legs bent to them (HumanModel_PlantFeet, Model_LegIK); off
it, his model at his place. His legs by his model (each class's Loaded: +0x890..+0x8AC): bones
2..5 and 6..9, O_LRY's left $12..$15 (`leg-bones`). Checked by `tests/stalker/test_legs.fs`.
Built: **Lorenzo in his chair** (O_LRO, `src/game/chair.c`; LorenzoModel_BodyFrames /
_AdjustBone): no feet planted; on the floor (Chair_OnFloor: his height on it or below) his body
is tilted along the floor from the point his stance's `back` behind him to him, the chair's
front (bone $A and what hangs from it) along it from him to the point `front` ahead (8 / -8: both
behind him), each found by walking the nav mesh (Motion_OntoFloor); off it, both as he stands.
His head turns about the front's axes by his look (bone $B 0.3 of the turn; $12 0.4 of the tilt
and 0.3 of the turn about a frame kept upright from $B; $13 0.2 and 0.4 from $12: Mtx_TurnTwo,
Mtx_FrameKeepZ). Mtx_TurnTwo clears the bone's place and turns it by matrices whose place row the
original leaves unset (stack garbage - zeros would put his head at the room's origin); here each
head bone turns about its own place. His walks are on the floor: a path's stride points put on
the mesh (`paths.fs` `onto-floor`, PathPlan_AdvanceOnMesh's set_on) - before, he kept his height
up a slope. The engine's words: `lorenzo-chair ( id on -- )` (at load), `chair-fit ( id tri mask
-- ) ( F: front back pitch yaw -- )` each frame in the room played, `chair-rises ( id -- ) ( F: --
body front )` (how far the tilts raise the forward axis). Checked by `tests/stalker/test_chair.fs`. Built: **his chance roll and rests** (`tools/stalkertables.py`: each
kit's +0x1740 and +0x1748 by the hard setting and his rage - kind 37 alerted -, as his Setup /
SetRage / Kind37_Update set them; Lorenzo2's hard rests are memory nothing writes: zeros). The
rests: held off after a blow that struck (Debilitas 90, Daniella 90, hard 60, kind 35 120 / 150
...), at least after one that missed (30, 60 ...), after a hold that caught (30, 60 ...); letting
go of her hand (Pursuer_StateTauntHold) and her crying out at his blow (Pursuer_CryHeard) the
first. The chance roll (Pursuer_ChanceRoll): {chance, rise} by row - Hewie down, his blow struck
Hewie in this attack, struck by someone, else (Debilitas 100, 100, 75 rising 25, 25 rising 12) -
plus what built up; won, his temper with Hewie gone; lost, what built up rises. Rolled as each
attack at Hewie ends (Pursuer_BehaviourAttack: still past what he stands, at Hewie again, else on
as before - his taunt only when his time for it runs out, his temper still up), struck by her or
powdered while after Hewie (Pursuer_Hit), back on his feet with Hewie down (Pursuer_StateBackOnFeet).
Hewie's attack and tackle are taken only on floor he may stand on (Actor_TriFreeFor). Also built:
**the mini-game's run** (Hewie played): she isn't always in the room played - after her
while she is elsewhere he goes out by the first door of his way to her room (Pursuer_Modes /
BehaviourFollow) and travels after her; coming into the room played he waits 150 frames first
(Pursuer_ThroughDoorEnding); doors are left open behind (Kind34_ExitArg, TintStalker_EndingDoor);
Kind34 and the TintStalker take the run's attack rows for situations 14 / 15 (`run-table`);
their tempers with Hewie are the run's (80, 20, Debilitas3's 16); kind 7 (`stalker-as`) is
Debilitas set up as Debilitas3 (the table above): the Pursuer's chase. Searching out of sight his clock is 1
(Pursuer_Timer15s), and done with it - she not in his room nor next to it - he goes after Hewie
instead (Pursuer_Modes' mode 3: SetTarget(Hewie) - here, to the room played, Hewie's, to search
it: `hunting-dog`). The summoner doesn't bring him in during the run (Summoner_RoomStart). The cutscenes' cast beyond slot 2 (`char-load-2`, `char-hand-over`,
`char-loaded?`) is the event characters' (`docs/subsystems/cast.md`).
