# Hewie

## Purpose

Fiona's dog. He keeps near her, roams, sniffs, does tricks, comes when called, waits, attacks
when told, and defends her from the stalkers on his own as his trust grows. He has moods
(pleased, upset, angry), health, and a memory of how he's been treated. The original's
`hewie.c` is ~13,000 lines (~265 functions). He is built in phases.

His actions (the original's numbers, which this page still quotes) have names in
`scripts/hewie-names.fs` (`hewie-action`), with his move modes, moods, animation groups and
what he tells the story he is doing; his animations and sounds are named in `hewie/anims.fs`
and `hewie/sounds.fs`, the bits of what may break in on an action in `hewie/state.fs`.

## Phases

| Phase | What | Needs |
|---|---|---|
| **H1** | His body and model (legs fitted to the floor; head, ears and tail overlays; his neck turned where he looks); moving (strides, turning with his head leading, the freest heading); his mind: the danger's moods (calm / wary / tense), what he does next by his trust (the weighted lists), his own decisions; being with Fiona (keeping near, coming to her, stepping away, waiting by her); roaming, sniffing, tricks, barking; his upkeep (health, mood, obedience); his sounds and steps (heard); going from room to room with her and finding his way off-screen | Fiona F1-F2, rooms, doors, acoustics, danger, panic |
| H2 | Fiona's side (F3): her calls, gestures and commands (come, wait, attack, praise, scold, pet), trust and obedience as she treats him | — |
| **H3** | The stalkers: noticing them, growling, going for them, leaps, bites, tackles; being struck, knocked down, yelping | stalkers |
| **H4** | The story's moves (walk to a spot, turn, an animation, a leap), fetching, played (the mini-game's run) | story |

## API (H1)

Declared in `scripts/messages.fs`.

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `join-fiona` | `( -- )` | in (the story; the console) | He comes into the game at her heel. He is spawned with the game but has no body until then. |
| `part-from-fiona` | `( -- )` | in | He leaves the game (no body, not shown). |
| `fiona-doing` | `( mode sub cond -- )` | in (Fiona's broadcast, each frame) | What she is doing: her move mode, its part, her condition (2 down). The panic hears it too. |
| `danger` | `( level -- )` | in (broadcast) | His mode's behaviour (calm / wary / tense) and his lists follow it. |
| `panic` | `( stage level -- )` | in (broadcast) | Her panic at 4 or 5 may bring him to her, by his trust (in her room, or from off screen). |
| `leaving-room` | `( room exit -- )` | in (rooms) | She leaves his room by that exit: he follows her that way (nearer its door than she is: through it with her at once, 0x88), by another exit, or stays; at the stalker then (leaping, biting), he waits off to the side (0x38); down, he is up again (hurt) only in a hard game or where he may not lie (Hewie_Vt34). |
| `arrived` | `( room exit -- )` | in (rooms) | Where she is now. He comes in by himself, off screen through the doors (Hewie_Arrive / Hewie_StateToDoor). |
| `noise` | | to acoustics | His loud barks and yelps (0x80) and growls / barking at someone (0x1B). Source: `hewie-noise`. |
| `danger-signal` | `4` | to danger | He's alert to a stalker (Hewie_Alert: in his room within 200, ahead of his head; elsewhere, an exit of his room leads into the stalker's - that exit his target; or the stalker heard). |
| `listen` / `heard` / `heard-nothing` | `( 0 hewie-noise )` / `( loud room tri door source )` / `( )` | to / from acoustics | He hears any noise but his own (Character_Hearing, threshold 0): the source and room of last frame's, for his alert (the stalker's) and off screen (a noise from another room may send him there, 0x33). |
| `danger-signals` | `( bits -- )` | in (the danger's broadcast, at the frame's end) | The frame's signals (the original's condition bits): off screen, Fiona struck (bit 1) has him choose again where his action lets the story break in. |
| `fiona-struck-by` | `( who -- )` | in (Fiona's broadcast) | Who has hold of her (her +0x100): held by the stalker he harries him; held down by another, he rushes whom he picks. |
| `stalker-doing` | `( room mode sub cond hp hp-max kind -- )` | in (the stalker's broadcast) | The stalker in the game, as he tells it each frame (`pursuer`). |
| `dog-attack` | `( kind -- )` | to the stalker | His attack asked (10..15); answered `dog-grip ( tri x y z yaw )` or `dog-refused`. |
| `dog-tackle` | `( -- )` | to the stalker | The story's tackle (0x75) asked: answered `dog-grip` (the spot by his leg) or `dog-refused`. |
| `stalker-ties` | `( scripted apart linked -- )` | in (the stalker's broadcast) | A script or a scene has him (+0xE0): no scuffle off screen. |
| `bite` / `shaken-off` / `snap` | | to the stalker | A bite while holding on; he let go; growled or barked at him. |
| `blow` | `( kind damage how -- )` | in | Struck: by the stalker (his blows, his throw $B), by her (shove, kick: answered `blow-taken`), by a door (5). |
| `hewie-doing` | `( here action mode sub cond mood group -- )` | broadcast, each frame (and again when a message changes what he does) | here: 0 out of the game, 1 in another room, 2 with her; his action, move mode and its part, condition, mood, pose group. Fiona chooses her commands by it. |
| `command` | `( code tri yaw x z -- )` | in (Fiona) | Her command (the original's request 13). 0x23 "go there" carries the spot (floats `f>cell`). |
| `reaction` | `( n -- )` | in (Fiona) | How her doing strikes him (Fiona_HewieReact's row): patience spent while he waits, a praise due. |
| `meet-me` / `meet-at` / `meet-refused` | `( type )` / `( tri x y z face )` / `( )` | in / out | The meeting by his side, first part (type 0 scold, 2 praise, 4 stay): he finds her place and turns to her (0x48), or refuses. |
| `meet-now` / `meet-on` / `meet-refused` | `( type )` / `( )` / `( )` | in / out | Second part (1 / 3 / 5): sitting, settled, facing the agreed way: scolded close up (0x49), petted (0x4A), patted (0x4B). |
| `meet-off` | `( -- )` | in | She gave it up. |
| `hewie-restart` | `( -- )` | in (the run's clock) | Him afresh - played, in the mini-game's run (Hewie_Restart). |
| `hewie-gesture` | `( g -- )` | broadcast, each frame while played | His gesture this frame (the scripts' 0x21). |
| `hewie-told` | `( what arg -- )` | to Fiona | Barking low (Hewie_StateLookLow): 0 a shut exit `arg` he is at the front of, to open; 1 where he is (`arg` his triangle). |

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
   at his pace (1.6 a frame, 0.38 hurt, 10 hurrying to her panic). Hidden (Hewie_HiddenFrame),
   where his action lets them: Fiona struck (the danger's signal) has him choose again; a noise
   heard from a room other than the last he went to sends him there by his trust's chance
   (`noise-chance`, 0x33).
5. **Coming in** (Hewie_Arrive): through the exit with her (0x88) at its spot in her room; on
   his way to the door in the room still (0x2C / 0x2D / 0x39), where he is (on his triangle,
   else anywhere on his half) trotting (limping; to her cry running); mid-grip when she left
   (0x38 after at a flank), letting go of the stalker to the animation's end (0x73).

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

Actions of the later phases fall back to the default (or to sniffing about). His steps in water ripple and
splash (`wet-feet`, `hewie/fight.fs`). Left for H1: the story's use of `his-near` / `his-sub`.

**H2 built** (`scripts/hewie/commands.fs`; Fiona's side `scripts/fiona/commands.fs`): her
commands taken (Hewie_FionaCommand / CommandAction / ActOnCommand: stay, come, come back, go
there, go for it, praise and scold from afar with his mood, her cry for help by his trust),
her reactions, the meeting (Hewie_FindSpot, his calls 12), the behaviours for them (turning
with her, praised / petted / patted, scolded, won't, walking to the spot she showed, readying).
Checked by `tests/hewie/test_commands.fs` (17 tests): no gestures without `hewie-commandable`; her
reading his broadcast; come back (0x2C → 0x1D → 0xD); go there (0x23 → 0x63, the spot kept);
the meeting (praise close: 0x48, sits, 0x4A petted, her 0xC07); her cry for help (0x30); a
reaction spending his patience.

Her head turns to him after a command (her looks: `fiona/looks.fs`, the look held a while); the
scripts' "can she command him" is the story's cond 0x4C (`hewie-can-command?`).

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
flank kind 2 or 3 / 5 or 10 readied, 1 or 3 the far one, head on 4 / 10; doubled in his black German Shepherd costume (variable $27 = 1); hard
by chance - `hard-bite-chance` by the stalker's health band - Hewie_ReactionRoll), $6B, on the
spot, the bite animation, the chance to hold on (his feeling + 10) x 5. Holding on
(StateBiteHold): each time round a shake roll (his bites left; the chance, -10 each time held)
- failing it he lets go (`shaken-off`, $2219 / $2202 / $220B / $2216, StateClamped, then on with
his-pet-time 300); else bites again (the leg 5 / 10, a flank 2 or 3 / 2 or 3, head on 4 / 3);
let go when she calls him off (0x27 / 0x2C) or the stalker is down. Thrown off (the stalker's
`shaken-off`): at a flank (0x1F / 0x20) hanging ($2203 / $220C) - then held on if he dares
(Hewie_Dares: from trust 2, a 1-in-16 roll under his level + 1 + how often he has leapt at that
stalker / 5, at most 8 - Hewie_CountEncounter, counted at each leap to 100 and kept in the save's
`hewie-met` line) or flung (10 damage), $2204 / $220D or $2207 / $2210, along his last root motion,
falling 0.5 a frame, landing $2212 / $2213 (StateHanging, StateJumpArc); else letting go.
Harrying (0x59 / 0x5A: near him, 0x53, then 0x23 closing in - his-wanted times, 3 for 0x5A).
Struck (Hewie_TakeBlow, `blow`): down - 0x74 (not at the stalker's grab, 6); already struck or in
a joint action - nothing; its damage (hers twice); angry, whoever struck him calls him
(`his-call`: her 0, another 2 - answered at his next bite or leap); by another than her a quarter
of the time his feeling for the stalker down and her hits forgiven; by her scolded
(praise-scold), hit lately counted (to 6; one less every 300 frames) and angry by chance
(`angry-chance`: no call then); his yelp ($1000, $65 a shove, else $66), pushed back from whoever
struck him 1.5 less 0.5 a frame (0x69, StateMoveAlong). A door only while it swings: she not in
front of it, back along its turn through blocked floor (0x6B); she in front and he on its sides
where he may pass, out by it (0x6C); else, off its gap, a quarter turn aside (0x6B) - 5 less 1;
then at its event (Hewie_AfterYelp) down (0x52, his feeling for the stalker -3) or -
her blow, by her, hit lately - cowering (9 the first time, 0xB), else on. Her blows struck home
are answered `blow-taken`. Checked by `tests/hewie/test_fight.fs` (19 tests).

His scuffles off screen (Hewie_Scuffle): the stalker hunting (its mode 1..3) in the room he is
left in, once a meeting and not within 900 frames - by chance (15 / 45 / 15 / 25 in 100) both
hurt, him only, the stalker only, or nothing: him a tenth of his health (not below 1); his yelp
($66, $69 unhurt) heard if his room is next to hers; the stalker held 30..180 frames (`blow` 8:
its `stun-t`, its cry $1D from the door it would come in by) and hurt a tenth of its health
(twice in the black shepherd costume) unless he may not attack; then he waits it out ($30). His feelings are kept by the stalker's kind (Hewie_Feeling): Debilitas (2, 6, 7, 27), Daniella
(3, 34..36), Lorenzo (10..12, 39), Riccardo (4, 23 the TintStalker, 37); none for another. His feeling about the
stalker (Hewie_FeelPursuer): calm, a stalker about, the more he dislikes him the likelier he is
upset (mood 2: 14 in 16 at -10 down to 1 at 0) - as his feeling changes and as the danger turns
wary or tense. A bite's mark at his mouth (Hewie_BiteEffect: bone $25, a big one too while his
grip holds). Held by the stalker's grab (`hit` 6): its damage, not taken as a blow - as the
original. (The tackles, growling at the
stalker, the door pushing him out and the creatures came with H5, below.)

**H4: the story's words about him** (`scripts/story/words.fs`; EventCmd_Hewie, event.c's
conditions). He tells each frame, in `hewie-doing`, besides what he does: whether she may command
him (Hewie_FionaCanCommand: with her, not scripted, idle, not angry, a command word that takes
one, no meeting), near enough for it (Hewie_FionaNearCommand: within 150 by his way over the floor,
planned apart - Hewie_WalkDistance), whether he can reach her (Hewie_FionaReachable: only while scripted, a way planned apart -
`his-probe`), whether he may be broken off (Hewie_MayBreakOff, not `once`: its mark not kept),
his half of the room, his trust level. Conditions 0x24 his action, 0x4C, 0x0F, 0x5A, 0x31 (the
converter's `hewie-stays?`: he *may* be broken off), 0x33, 0x61. Orders: 0x39 his action
(`do-action`), 0x85 trust, 0xC4 his mood, 0xD0 his half, 0xBD his-pet-time 300, 0x3F his place
(`hewie-place`: in a room on a triangle, on its half; in the played room his default action),
0xC5 / 0xC6 a point to look to if he has none (a zone's - in a script variable - or a
character's place, raised: `scent-at`), his moves 0x78 bark, 0x7A / 0xB0 to a point (Hewie_Requests
0x12, 0x13, 0x14, 0x0D), and as the script's own 0x20 through blocked floor (`his-2b`), 0x21 left
alone (`his-2d`). Checked by `tests/story/test_hewie_fiona_words.fs`. His model swapped (0xB7
`hewie-model`), 0xBB's forced choice (`hewie-wait-5`), his animations (0x77 `hewie-anim`, 0xC3
`hewie-anim-root`) are built; 0x63 (his heading, then action $72) is used by no room's script.

**H4 begun** (with the story's S3): the story's moves (`scripted-move` → his actions 0x3B..0x47,
0x7F; `scripted` on / off: Hewie_Think while a script has him; `hold-anim`, `show`; `moving`
reported each frame). Behind Fiona (0x72) and fetching came with H5; the Hewie-controlled
mode is still to come (below).

**H5: the rest of his actions.**
- His own decisions (Hewie_OwnDecisions) also: where his action lets it (0x100), the stalker - or a
  creature up - in his room within 10, its back to him: near it (0x7C); Fiona newly held, by chance
  at whoever holds her - the stalker harried (0x5A), else whom he picks rushed (0x6D).
- `hewie/more.fs`: the story's sit-then-on (0x80), behind her (0x72), limp (0x74), a scene's
  pose (0x79), back on his floor (0x86), waiting after an attack (0x38), stepping away from his
  target (0x82), pushed back (0x68) or out by a door (0x6C), at one of the cast (0x5B..0x60),
  near his target (0x7C), and round to its flank (0x12, Hewie_StateFlank: behind it and facing
  its way he lines up with it, within 40 settled; else the side picked once on its triangle,
  and for the point 45 degrees round from straight out, 20 out - up to 40 the more squarely
  behind - as far as is free).
- `hewie/pounce.fs`: harrying (st-close-on) within 30 - on a step (floor flag 1) he closes in
  (0x6D), on open floor facing it (60 degrees) and on its triangle he pounces: the leap
  ($1E01, 2.8 a frame, up to 5 above it then falling 0.5 faster a frame, landing $1E03), and
  whoever his mouth (bone $1F) reaches on the way is bitten - the stalker a blow (5 / 8 from
  behind, charged 10 / 16; the black shepherd costume doubles, the boost half again), else Fiona knocked down; the
  homunculi it reaches as a blow there. 0x6D: running at it, the bite once his mouth reaches.
- `hewie/fetch.fs`: fetching (0x78). Her ball thrown while all is calm is his to fetch (the
  things actor tells him, `fetch-this`); her command 0x23 sends him for the ball nearest in
  front of her he can get to (Hewie_NearestThing). His six skills (out of 32, 16 to start,
  kept in the save's `hewie-skills` line) are rolled against: going after it (else watching it
  go, his eyes on it), eager (his gait), at it picking it up (`thing-taken`: gone, hers again if
  she has none) and bringing it, else telling her (a bark by her), else leaving it; lost (gone,
  or lying where he can't get) coming back to her. Sat by her after, his doing counts as
  praised by 1; her praise or scolding moves the skills he used. His own decisions take it up
  when a ball he follows is going, ahead of his head's way and straight to him.
- `hewie/exits.fs`: out by an exit on screen (0x24 away from her, 0x25 anywhere:
  Hewie_FleeExit - the open exits nearest first, those within 20 of whom he leaves last, the
  first he can plan a way to; StateGoToExit). While she hides (fiona-hidden / -half-hidden)
  and the stalker is here, by chance (`growl-chance`) he draws it off (0x7B): the spot 80 along
  the stalker's own way to an exit, there first (RunSpot), squared up to it (PursuerNear), on
  guard barking while it is 60..100 away by foot (Guard), re-planned when it isn't; the second
  time round out by the exit and a wait (0x31, `flee-wait` by trust).
- `what-next` (Hewie_WhatNext): the growl first; a creature up and about in his room
  (`creature-doing`, as Character +0x3C) takes the chase lists; the chase's situation counts
  the stalker hurt or held off (1), within 30 of him (2) or of her (3).
- `blow-taken` / `hit-taken` replies are taken quietly; `silent` (the scripts' 0x3D) hushes his
  own sounds (Actor_PlaySound's +0x2C), as the stalker's.
- `hewie/fight.fs`: the story's tackle (0x75; Hewie_StateTargetPursuer / GoForPursuer,
  LeapLaunch, LeapTakeOff, LeapAtPursuer): seeing the stalker, not off screen, bitten or down, it
  is asked of him (`dog-tackle`: the original's slot command 2 / 7, Pursuer_GrabOrder) - taken, the
  spot by his leg (`dog-grip`; the stalker's behaviour cut short), refused, his default. The leap
  there along his way: a third of its length in frames (19 at least), turning to the grip's
  heading, rising to land at its height under 0.3 a frame of fall, never below the floor ($1E01,
  his snarl $68); the spring faded in, the leg's bite ($2218) faded in over the frames left, its
  time held meanwhile; 13 frames before he lands the leg's attack is asked (`dog-attack` 15: the
  stalker bitten, the spot anew - unless the story's flags $13 / $2B). Landed: on the spot, the
  bite of 30 (`bite`, hard by the leg's chance; the black shepherd twice, Sylvesta Esca half
  again), its big mark, then holding on as at the leg with one bite left - he lets go ($2219) at
  its end or when she calls him off. His hold by his mood (angry 110, upset none, else (his
  feeling + 10) x 5) is set, as in the original, but never rolled against.
- His scuffle off screen skips a stalker a script or a scene of the room has (+0xE0: the
  stalker's `stalker-ties`).
Checked by `tests/hewie/test_tackle.fs` (20: the tackle, refused, the stalker's ties, the scuffle
skipped, Fiona kept apart), `tests/hewie/test_more.fs`, `test_pounce.fs` (the pounce, the rush, the flank),
`test_fetch.fs`, `test_exits.fs`, `test_alert.fs` (his alerts, what he hears, his feelings and
leaps by kind, struck, near her by his way, coming in, left down), and the save's skills and
leaps in `tests/saves/test_saves.fs`.

Found on the way: the ball rolled with no friction on level floor (things.c had the slope's
sine and cosine the wrong way round - NavTri_Slope gives the sine); fixed.

**In a door's way** (`hewie/exits.fs`; Hewie_FirstDoor, by Progress_WhoIsWhere's bits for him
at each exit - asked of the doors as he needs them: on the exit's own area, at the door, on its
sides' triangles, in its gap's area, in front of it): on the exit's area at a door he may pass
(not locked against him; at rest, shut) he is out through it ($66 - the original walks the step
there first); in its gap, or in front of it while it swings, he squeezes out to 24 from where it
stands on his side ($65, Hewie_StateSqueeze: 90 frames at most, done once off the gap or away
from the door). `tests/hewie/test_exits.fs` (9).

**H4: Hewie played** (`scripts/hewie/steered.fs`; `tests/hewie/test_played.fs` (9),
`test_run.fs` (8)). The mini-game's run - the extras room's door 0, or the title's MINI-GAME:
Hewie guiding Fiona through the rooms $D0..$E9 against the clock - is the one section where he is
the one played (the original's progress +0x1FBEC1, `facts.fs` `hewie-played`: set by the run's
clock as it starts - `story/clock.fs` `clock-start`, which restarts him: `hewie-restart` - and
cleared as it stops). Played:
- the stick runs him camera-relative, as it walks her (Hewie_LeftStick: after a camera cut the
  old camera steers while the stick is held, then he turns on the spot toward its heading, a
  little faster each frame up to 3 degrees, while the stick holds within 15 degrees); still,
  he stands low (pose 4); cross held he runs flat out (`steered-act`, Hewie_StateSteered);
- the right stick's gesture is read each frame (`keys.fs` `gesture@`) and told the story
  (`hewie-gesture`: the scripts' 0x21); any one, with what he does open to her commands, barks
  (`bark-act`: at her within 150, else the stalker within 150). Barking low he tells her
  (`hewie-told`, Hewie_StateLookLow): each shut exit he is at the front of - for her to open -
  or, none, where he is (she keeps with him longer);
- his mode's behaviour is Hewie_StatePending (asked to choose: steered again), not calm / wary
  / tense; his own decisions are only: down while steered, knocked down; in the sweep of a door
  swinging, off it to the exit's far point ($86);
- where he would have gone for the stalker and couldn't, a run-up and a leap (`run-up-jump-act`,
  $87: hewie/pounce.fs's tackle from 5 above him, 20 ahead); nothing to do, himself again (not
  sniffing about); lying flat ($84) until the pad is touched (a face button or shoulder, the
  stick past 0.8, a gesture: Hewie_PlayerSteering) - as crouched readying (Hewie_State2388);
- biting, cross held lets go, five squares bite anew; his actions are taken as asked
  (Hewie_AdjustAction keeps them); he takes half as much again as the black shepherd; he never
  cowers from her; down, the game is over (kind 1);
- out by an exit he goes with the room, arriving at the exit he came in by (his Hewie_Vt34 /
  Vt38 in the run); `to-exit` (the scripts' Character_ChooseExit) only sets that exit.
- In tests, front-garden-2's floor is mostly closed to him (flag 8) till its room script is
  written; tests that need him to plan their way use castle-1f-1 / castle-1f-2.

## Hard mode and his costume

hg reads progress variable $27 - his costume (0 the white German Shepherd, 1 the black, 2 the
stuffed toy) - for his harder bites, his obeying and waiting times and his lists when tense
(`black-shepherd?`; the tables `obey-time-shepherd`, `wait-time-shepherd`,
`tense-wait-shepherd-lists`). These were named "hard" before; the real hard mode is the story's
flag $AF (`hard-mode?`, facts.fs: the original's progress +0x30 bit 15), which the stalkers and
the cast read. In a hard game (Hewie_StateKnockedDown) once he lies down the game is over (game
over kind 3) - unless a scene has them or the story keeps the game from ending (`capture-no-end`:
then he is left at 1 health). Each time he is knocked down (Hewie_AfterYelp) it is counted for
the dog's level at the end (`pr.hewie-downs`, progress +0xFBA, to 9999) - not in a hard game.
Checked in `tests/hewie/test_fight.fs` (27).
