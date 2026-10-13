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
| **H3** | The stalkers: noticing them, growling, going for them, leaps, bites, tackles; being struck, knocked down, yelping | stalkers |
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
| `danger-signal` | `4` | to danger | He's alert to a stalker (Hewie_Alert: in his room within 200, ahead of his head). |
| `stalker-doing` | `( room mode sub cond hp hp-max kind -- )` | in (the stalker's broadcast) | The stalker in the game, as he tells it each frame (`pursuer`). |
| `dog-attack` | `( kind -- )` | to the stalker | His attack asked (10..15); answered `dog-grip ( tri x y z yaw )` or `dog-refused`. |
| `bite` / `shaken-off` / `snap` | | to the stalker | A bite while holding on; he let go; growled or barked at him. |
| `blow` | `( kind damage how -- )` | in | Struck: by the stalker (his blows, his throw $B), by her (shove, kick: answered `blow-taken`), by a door (5). |
| `hewie-doing` | `( here action mode sub cond mood group -- )` | broadcast, each frame (and again when a message changes what he does) | here: 0 out of the game, 1 in another room, 2 with her; his action, move mode and its part, condition, mood, pose group. Fiona chooses her commands by it. |
| `command` | `( code tri yaw x z -- )` | in (Fiona) | Her command (the original's request 13). 0x23 "go there" carries the spot (floats `f>cell`). |
| `reaction` | `( n -- )` | in (Fiona) | How her doing strikes him (Fiona_HewieReact's row): patience spent while he waits, a praise due. |
| `meet-me` / `meet-at` / `meet-refused` | `( type )` / `( tri x y z face )` / `( )` | in / out | The meeting by his side, first part (type 0 scold, 2 praise, 4 stay): he finds her place and turns to her (0x48), or refuses. |
| `meet-now` / `meet-on` / `meet-refused` | `( type )` / `( )` / `( )` | in / out | Second part (1 / 3 / 5): sitting, settled, facing the agreed way: scolded close up (0x49), petted (0x4A), patted (0x4B). |
| `meet-off` | `( -- )` | in | She gave it up. |

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
- The room planner takes his doors to avoid (`route-sides`: a door that let him down isn't
  tried again until he follows her afresh) and starts from the half of his room he is on,
  `his-way` (the original's +0xF3668: set by the exit he comes through or arrives by, 2 -
  either - when he comes into the game; -1 looks it up from the exit he came in by, and then
  the doors' one-way flags count for his kind, `hewie-kind`). Not modelled: Progress_DoorPassable's side (the planner treats doors as
  passable from any side) and Progress_CurRoomFlag (waiting at a door the story holds).
- The original's relations table (slot commands, request blocks 12 / 13, "hold" 7) is replaced
  by the meeting messages and `command`: what Hewie_JointAction checked each frame he checks
  when asked. A refusal answers at once (where the original cancelled and held her the next
  frame).
- What he does is told the moment a message changes it (`tell-doing` after `command` and the
  meeting): she reads it before her next frame, as the original's shared fields allowed.
- Following her, he may take a long while: every action gets a random wait (90..360 frames)
  and 0xCF's travel re-picks from the idle list when it runs out (faithful).
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

**H2 built** (`scripts/hewie/commands.fs`; Fiona's side `scripts/fiona/commands.fs`): her
commands taken (Hewie_FionaCommand / CommandAction / ActOnCommand: stay, come, come back, go
there, go for it, praise and scold from afar with his mood, her cry for help by his trust),
her reactions, the meeting (Hewie_FindSpot, his calls 12), the behaviours for them (turning
with her, praised / petted / patted, scolded, won't, walking to the spot she showed, readying).
Checked by `tests/hewie/test_commands.fs` (17 tests): no gestures without `hewie-commandable`; her
reading his broadcast; come back (0x2C → 0x1D → 0xD); go there (0x23 → 0x63, the spot kept);
the meeting (praise close: 0x48, sits, 0x4A petted, her 0xC07); her cry for help (0x30); a
reaction spending his patience.

Not yet: her head turned to him after a command (Fiona's looks, later), the scripts' "can she
command him" (H4).

**H3 built** (`scripts/hewie/fight.fs`; the stalker's side `scripts/stalker/bitten.fs`, P3c).
The stalker as he tells it (`stalker-doing`: `pursuer`, his mode and condition) - Hewie_Alert,
Hewie_PickTarget, the tense lists' barking at him and facing him now find him. Barking at him
(st-bark-at-target, st-bark) or growling: `snap` (his temper). At him (actions 0x1F..0x22;
after her "go for it" - Hewie_AfterComingOut: his leg 0x21 when readied and coming at him head
on, else 0x1F; after a call 0x1F / 0x20; Fiona held: by chance 0x5A): seeing him, not off
screen, not bitten or down - at him at a trot or a run (StateRunAtPursuer); within 23, nothing
breaks in, the side he comes from (Hewie_AttackSide: from behind his back 0, unless timid;
else a flank, 1 / 2, Daniella the other way) picks the grip and the action, and the attack is
asked (`dog-attack`: 15 the leg, a flank 10 / 11 readied, 12 / 13, head on 14). Refused: 0x5A.
Taken (`dog-grip`): the leap (StateLeapGrip: his bites by action, grip and trust - the
generated `bites-*` tables; his angry count eased; a third of the time his feeling for him up;
$2217 / $2200 / $2209 turned about / $2214, his way and the turn covered in 8 frames - the leg
12 - at the stalker's height, $68). Landed (StateLeapLands): the first bite (the leg 5 / 30, a
flank kind 2 or 3 / 5 or 10 readied, 1 or 3 the far one, head on 4 / 10; doubled on hard; hard
by chance - `hard-bite-chance` by the stalker's health band - Hewie_ReactionRoll), $6B, on the
spot, the bite animation, the chance to hold on (his feeling + 10) x 5. Holding on
(StateBiteHold): each time round a shake roll (his bites left; the chance, -10 each time held)
- failing it he lets go (`shaken-off`, $2219 / $2202 / $220B / $2216, StateClamped, then on with
his-pet-time 300); else bites again (the leg 5 / 10, a flank 2 or 3 / 2 or 3, head on 4 / 3);
let go when she calls him off (0x27 / 0x2C) or the stalker is down. Thrown off (the stalker's
`shaken-off`): at a flank (0x1F / 0x20) hanging ($2203 / $220C) - then held on if he dares
(Hewie_Dares) or flung (10 damage), $2204 / $220D or $2207 / $2210, along his last root motion,
falling 0.5 a frame, landing $2212 / $2213 (StateHanging, StateJumpArc); else letting go.
Harrying (0x59 / 0x5A: near him, 0x53, then 0x23 closing in - his-wanted times, 3 for 0x5A).
Struck (Hewie_TakeBlow, `blow`): down - 0x74; already struck or in a joint action - nothing; its
damage (hers twice, not a door's); by the stalker a quarter of the time his feeling down; by
her scolded (praise-scold), hit lately counted (to 6; one less every 300 frames) and angry by
chance (`angry-chance`); his yelp ($1000, $65 a shove, else $66), pushed back from whoever
struck him 1.5 less 0.5 a frame (0x69, StateMoveAlong); a door, from her side of it 5 less 1
(0x6B); then at its event (Hewie_AfterYelp) down (0x52, his feeling for the stalker -3) or -
her blow, by her, hit lately - cowering (9 the first time, 0xB), else on. Her blows struck home
are answered `blow-taken`. Checked by `tests/hewie/test_fight.fs` (19 tests).

Not yet (H3): his scuffles off screen (Hewie_Scuffle), his tackles (0x75; Hewie_StateTackle),
growling at the stalker (0x7B and its flight, Hewie_FleeExit), the bite's hit effect, his
feeling's mood (Hewie_FeelPursuer), held by the stalker's grab (`hit` 6), the door through
which he is pushed (0x6C), the creatures.

**H4 begun** (with the story's S3): the story's moves (`scripted-move` → his actions 0x3B..0x47,
0x7F; `scripted` on / off: Hewie_Think while a script has him; `hold-anim`, `show`; `moving`
reported each frame). Left for H4: fetching (0x78), the Hewie-controlled sections, behind Fiona
(0x72), the scripts' other Hewie commands (`hewie-action`, `hewie-to-room`...).
