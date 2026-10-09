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
| P2 | After her: the chase in her room (closing in, holding back, circling, a lunge), his attacks (combos from his move table, each blow a `hit` to whom it reaches), grabbing her (`seize`: led by the hand, walking, dragged; the taunts), dragged off: the game over | Fiona F4 (her reactions) |
| P3 | Struck: her kick and shove (F4), Hewie's bites (H3), slammed doors; flinching, falling, getting up, giving up for a while after enough; losing her (she hides, gets away) | Fiona F4, Hewie H3 |
| P4 | The story's stalker words (put in a room, sent, held, released, loaded), the cutscenes' cast, the other stalkers | story |

## API (P1, a first draft)

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `stalker-in` | `( room tri -- )` | in (the story, the console) | Come into the game in that room, on that triangle. |
| `stalker-out` | `( -- )` | in | Out of the game. |
| `stalker-here` | `( room alert chasing -- )` | out, to danger | In play this frame (alert: he knows of her; chasing). |
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
`route-avoiding`, walkers of `stalker-kind`); each stretch is the next door's triangle number
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

**After her** (mode 0) is, until P2's chase, a stand-in: toward her on foot, his way
re-planned every 30 frames, standing facing her within 15.

## Design notes

- The original's request blocks between characters (Relation_Request, the progress' slots)
  become messages with replies: a blow is `hit` (its kind, damage, a stumble), taken or not; a
  grab is `seize`, answered by Fiona.
- His tables (moves, attack tables by her state, his chances) are data from the executable,
  generated once like Hewie's (`tools/`).
- The old new-src port (`new-src/scripts/pursuer/`) is a behaviour reference only: it keeps the
  original's offsets and vtable; this one is written in game terms.

## Status

P1 done: P1a (in/out, shown, growl, `stalker-here`), P1b (senses), P1c (modes, search route,
walking), P1d (out of sight from room to room, knocking, coming in) - `tests/stalker/test_stalker.fs`
(26). Not yet: his footsteps heard through the walls, the knock frightening her (with her fear),
the rooms a stalker keeps to (+0x314). Next: P2 with Fiona's F4.
