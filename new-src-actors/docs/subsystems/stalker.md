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

To be filled in as each part is ported - the modes and their times, the search route, the
sight and hearing ranges, his pace off screen, the chase's choices and the attacks' numbers.
Debilitas: model `O_DB0/DB0_000`, motion table $3D89A0, radius 5, height 20, head bone $16.

## Design notes

- The original's request blocks between characters (Relation_Request, the progress' slots)
  become messages with replies: a blow is `hit` (its kind, damage, a stumble), taken or not; a
  grab is `seize`, answered by Fiona.
- His tables (moves, attack tables by her state, his chances) are data from the executable,
  generated once like Hewie's (`tools/`).
- The old new-src port (`new-src/scripts/pursuer/`) is a behaviour reference only: it keeps the
  original's offsets and vtable; this one is written in game terms.

## Status

Page drafted; P1 next.
