# The cast (event characters)

## Purpose

The story's figures that aren't Fiona, Hewie or a stalker: the crows, Fiona's doubles, the
figures of its scenes (character kinds 8, 9, 13..22, 24..33 and 38). The scripts load them into
character slots 3..5, place them, move and animate them, take them out of the scene and let
them go; the cutscenes find their models by the character's script id.

## API

Declared in `scripts/messages.fs`; to a figure (`cast3`..`cast5`, one a slot).

| Message | Stack | Meaning |
|---|---|---|
| `cast-load` | `( id -- )` | Be character `id`: its kind's model (below) loaded and cast as `id` (`cast-as`); not yet placed. |
| `cast-unload` | `( -- )` | Let it go (model freed, out of the cast). |
| `cast-out` | `( -- )` | Out of the scene, still loaded (the scripts' 0x14: the class's Deactivate). |
| `cast-dress` | `( model -- )` | Wear that model's files (the scripts' hand-over 0xBA, Pursuer_GiveMotionBanks). |
| `place` / `to-exit` / `show` / `hold-anim` / `scripted-move` / `scripted` / `scene` | as for Fiona | The story's placing, showing, its moves, a cutscene having it. |

It broadcasts `moving ( done ended events )` at each frame's end, as the others do.

The story's words (`story/words.fs`): `char-load` (0x12: a stalker by kind, else a figure into
slots 3..5, at that exit of the room or as it is), `char-load-2` (0xB9), `char-remove` (0x40),
`char-done` (0x14), `char-hand-over` (0xBA), `char-loaded?` (cond 0x63); the slots' table
(`slot-actors`, `slot-chars`: the actor and the character's script id in each) finds a character
by its id (`char-slot`, Progress_SlotOfId) and a slot's actor (`char-actor`; `slot-actor-of`
placed or not, for the placing words).

Engine words: `cast-model ( id -- model )` (the model cast as a script id), `model-file ( model
-- addr len )` (the files a model came from).

## The kinds' figures

8 O_GLM, 9 O_LRM, 13 O_FIN (FIN_200), 14 / 15 O_CRW (the crows), 16 O_WHC, 17 O_SGM, 18 O_FIW
(FIW_200), 19 O_FS0, 20 O_FS1, 21 / 22 O_FS2, 24 O_SHT, 25 O_WIR, 26 O_RBT, 28 O_HMA, 29 O_DNT,
30 O_LRC, 31 O_LRH, 32 O_FIM (FIM_200), 33 O_HMB, 38 O_HND (their ModelFiles' first file). Kind 9 (Lorenzo's hanging figure) takes LRM_000H's
textures on the hard setting (Kind09_ModelFiles).

## The kinds' own ways

hg's classes for these kinds are thin: each is put back on the floor as its room comes into play,
takes the story's moves (action 5, its kind's step table) and lets its slot's joint command go.
Kind 9 stands in its idle when it shows up. What sets them apart is their models:

- **Motion tables** (their fades; the model classes' SecondaryMotion): Fiona's doubles (13, 18)
  use hers ($3D5CC0), the crows (14, 15) $429C10, kind 9 $424250, kind 33 $42C730; the plain
  model's kinds have none.
- **The crows' parts** (Kind14Model_Draw): in $9000 / $9001 part 5 shown, 3 and 4 hidden; in
  anything else the other way round. (The draw-form flags set with them are not kept.)
- **The rabbits** (kind 26, Kind26_Behaviour): the story's `animal-go` - grazing, alert, running
  off (`tests/cast/test_animal.fs`).

No sounds of their own: only the stalkers' classes play their animations' sounds
(Pursuer_AnimSounds). Their shadows are the engine's, as everyone's (`char-no-shadow` turns one
off). The hand-over (0xBA) waits in hg while the other's files load; OpenHuey loads them at once.

## Moves

The story's (`scripted-move` kinds; Pursuer_EventCommand): 7 an animation (a), 8 blended (a over b
frames), 9 the same with no time of its own, 16 its stand (0) over b frames - done at once,
moved by its root motion; 5 / 10 walking to the spot, 6 / 11 to triangle a (its middle) - 5 / 6
in b if it is one of the $2xx walks, else $200, 10 / 11 in $201 (Pursuer_ScriptMove,
Pursuer_SlowWalkAnim: its +0x16C8 is 2 from Pursuer_Reset) - turning toward it 10 degrees a
frame, moved by the animation's root motion (a slow step when it has none). To a triangle: done
once on it; at the spot, standing. To the spot (Pursuer_StateArrive): put on it, then turning to
the move's heading at 4 degrees a frame (Pursuer_TurnRate), standing, done. 14 / 15 turning to
the move's heading in $200 at 4 degrees a frame, then standing (0) and done once that has faded
in (Pursuer_StateTurnThenWait); 17 its own animation to the spot (put on it, facing the move's
way, done); 1 standing; the doors' uses: done. `hold-anim`: in it, done.

**Its head** (Pursuer_Think, Pursuer_LookAt; `look`): 12 a look at a character (b its slot; $FF
off - on only if the slot has someone), 13 at a point (+0x1700); done at once. While a look is on
the head turns toward whom the moves last named - the original's +0x100: written by the looks, the
turns to a character (14), the doors' uses (3 / 4: their exit) and the walk with an animation (17:
its animation) - 0 Fiona, 1 Hewie, anything else the look's point (so a look at a point after a
look at Fiona still looks at her, as the original's does); small turns (under 0.05) none; 8
degrees a frame (Pursuer_TurnRateFast). The tilt stays level (Pursuer_LookAt eases it to 0 by
nothing; easing back takes it to 0). No look: eased back, 8 degrees a frame. As the model is posed
(HumanModel_AdjustBone: `head-turn`) the head bone takes half the turn in its own frame and the
bone below it the other half about its own place: Lorenzo's hanging figure (Kind09Model) $2B /
$22, Fiona's doubles (13 EventHumanModel, 18 Kind18Model) $21 / $17; the plain models have none.

**Its legs**: the human figures' feet are planted on the floor as Fiona's (`humanlegs.c`;
Pursuer_ModelUpdate's +0x40 with its stance's points - these kinds set none, 0 0 - and +0x4C): in
the room played, on its triangle, but not while a script walks it through blocked floor; a
scene's not. Lorenzo's hanging figure's legs 2..5 and $12..$15, Fiona's doubles' 2..5 and 6..9
(each class's Loaded: +0x890..+0x8AC).

As the story shows or hides it (`show`) a figure tells (`figure-shown`): the obstacles leave a
hidden one out of their way.

## Status

**Built** (`scripts/cast.fs`). Checked by `tests/cast/test_cast.fs` (23),
`tests/cast/test_animal.fs` (4), `tests/cast/test_cast_legs.fs` (3) and
`tests/cast/test_cast_head.fs` (8). *Not kept*: the models' own cloth (kind 9's strands, Fiona's
doubles' springs, kind 18's per-part draw settings) and kind 33's raised body frames.
