# Hewie

## Purpose

Fiona's dog. He keeps near her, roams, sniffs, does tricks, comes when called, waits, attacks
when told, and defends her from the stalkers on his own as his trust grows. He has moods
(pleased, upset, angry), health, and a memory of how he's been treated. The original's
`hewie.c` is ~13,000 lines (~265 functions). He is built in phases.

## Phases

| Phase | What | Needs |
|---|---|---|
| **H1** | His body and model (legs fitted to the floor; head, ears and tail overlays; his neck turned where he looks); moving (strides, turning with his head leading, the freest heading); his mind: the danger's moods (calm / wary / tense), what he does next by his trust (the weighted lists), his own decisions; being with Fiona (keeping near, coming to her, stepping away, waiting by her); roaming, sniffing, tricks, barking; his upkeep (health, mood, obedience); his sounds and steps (heard); going from room to room with her and finding his way off-screen | Fiona F1-F2, rooms, doors, acoustics, danger, panic |
| H2 | Fiona's side (F3): her calls, gestures and commands (come, wait, attack, praise, scold, pet), trust and obedience as she treats him | — |
| H3 | The stalkers: noticing them, growling, going for them, leaps, bites, tackles; being struck, knocked down, yelping | stalkers |
| H4 | The story's moves (walk to a spot, turn, an animation, a leap), fetching, being steered (the Hewie-control sections) | story |

## API (H1)

Declared in `scripts/messages.fs`.

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `join-fiona` | `( -- )` | in (the story; the console) | He comes into the game at her heel. He is spawned with the game but has no body until then. |
| `part-from-fiona` | `( -- )` | in | He leaves the game (no body, not shown). |
| `fiona-doing` | `( mode sub cond -- )` | in (Fiona's broadcast, each frame) | What she is doing: her move mode, its part, her condition (2 down). The panic hears it too. |
| `danger` | `( level -- )` | in (broadcast) | His mode's behaviour (calm / wary / tense) and his lists follow it. |
| `panic` | `( stage level -- )` | in (broadcast) | Her panic at 4 or 5 may bring him to her, by his trust (in her room, or from off screen). |
| `leaving-room` | `( room exit -- )` | in (rooms) | She leaves his room by that exit: he follows her that way, by another exit, or stays (Hewie_Vt34). |
| `arrived` | `( room exit -- )` | in (rooms) | Where she is now. He comes in by himself, off screen through the doors (Hewie_Arrive / Hewie_StateToDoor). |
| `noise` | | to acoustics | His loud barks and yelps (0x80) and growls / barking at someone (0x1B). Source: `hewie-noise`. |
| `danger-signal` | `4` | to danger | He's alert to a stalker (H3: none yet). |

**Facts he reads:** his own body (moved by the engine's body operations: `body-move-local`,
`body-turn-toward`, `body-free`, `body-tri-to`, `path-plan`), Fiona's body (`body-at`,
`body-yaw`, `body-tri`, `body-room`), the nav mesh (flags, floor, normals), the room graph and
the doors (`route-avoiding`, `door-open?`, `door-exit-in`, `door-sides`, `exit-spot`), his
model's motion (root motion, foot contacts, the end and fade), the difficulty (progress var
0x27). **Facts he keeps:** how he's been treated (progress `pr.hewie-mistreated`,
`pr.hewie-pleased`, `pr.hewie-downs`, for the dog's level at the end), state flags 0x11 / 0x1D
cleared by his trust.

## State

In `scripts/hewie/state.fs` (fields `his-*`; `hewie enter` shows them). His action and its working values (timers, target, the place he heads for), his look, his
mood and its time, trust (points and level), obedience (obeying / waiting), health and
condition, what he's alert to, his overlays, his sounds, the counters of how he's treated,
where he is off-screen (the door he makes for, how far to it).

## Rules

From `Hewie_Update`, `Hewie_SetAction` / `AdjustAction` (~90 actions), the `Hewie_State*`
behaviours, `Hewie_WhatNext` / `WhenIdle` (the weighted lists, from the executable),
`Hewie_OwnDecisions`, `Hewie_Upkeep`, `Hewie_Obedience`, `Hewie_TurnHead`, `Hewie_Vt34` /
`Hewie_Arrive` (following through rooms). Reference: new-src's `scripts/partner/*.fs`
(faithful). The rules are recorded in his code's comments, and here as they matter:

1. **What next.** When nothing drives him, he picks from a list weighted by his trust (0..7):
   the list for the danger (calm / followed / tense; waiting or obeying), and her distance.
2. **Obedience.** He obeys her for a while (by trust), then waits (does as he likes) for a while.
3. **His moods.** Pleased (1800 frames) when praised from afar, upset (1800) when scolded too
   often, angry (450) when she hits him. Each changes what he does with an action.
4. **Off-screen** nothing moves him but time: each stretch to the next door takes its length
   at his pace (1.6 a frame, 0.38 hurt, 10 hurrying to her panic).

## Design notes

- His actions keep their numbers for now (the weighted lists name them). As each is ported it
  gets a name (`come-to-fiona`), and the lists are named through a table.
- His position is his body (the engine's body operations move it). There's no copy in his state.
- Fiona's commands and his reactions to them pass as messages (H2), not through a shared
  request block.
- Fiona's room is a fact he is told (`fiona-room`: on `leaving-room` the room she is going to,
  on `arrived` where she is). When she leaves, the room being played is still the old one, so
  he must not head for "the played room" then.
- Whom he targets is an actor id (-1 none), not a character slot.
- The room planner takes his doors to avoid (`route-avoiding`: a door that let him down isn't
  tried again until he follows her afresh). The doors' one-way flags count for his kind
  (`hewie-kind`). Not modelled: Progress_DoorPassable's side (the planner treats doors as
  passable from any side) and Progress_CurRoomFlag (waiting at a door the story holds).
- Off screen, the way to the first door is counted down by its length (the path's rest when he
  set off from her room; else, as the original, the door's triangle number).

## Status

**H1 built** (`scripts/hewie.fs`, `scripts/hewie/`: `state` his fields, `body` him and the
others, his way, chance, `want`; `tables` the weighted lists and chance tables (generated from
the executable); `model` animations, groups, poses, stand / walk, voice, bark; `moving` root
motion, the floor fit, turning, best heading, strides, where her commands put him, his head;
`states` the behaviours; `offscreen` following through the house; `actions` SetAction /
AdjustAction; `mind` upkeep, obedience, what next, his own decisions, mode behaviours,
overlays, neck, feet, sounds, the frame). Checked by `tests/hewie/test_hewie.fs`:
- before joining no body; joining at her heel, in her room, shown, well, trust 0;
- left to himself, one action after another (at least three in 1200 frames), on the floor;
- trust points to levels;
- barking at her (0xB) heard as 0x1B;
- angry for 450 frames, then calm;
- she goes out by a door: he follows off screen and comes in after her;
- parted, gone.

Actions of the later phases fall back to the default (or to sniffing about). Left for H1:
water steps (with the room effects), the story's use of `his-near` / `his-sub`.
