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
off). The hand-over (0xBA) waits in hg while the other's files load; hga loads them at once.

## Moves

The story's (`scripted-move` kinds): 7 / 8 / 9 an animation, 16 its idle (b), done at once,
moved by its root motion; 5 / 10 walking ($200), 6 / 11 running ($202), 17 its own animation, to
the spot - turning toward it 10 degrees a frame, moved by the animation's root motion (a slow step
when it has none), there: put on it, facing the move's way, done; 14 / 15 turning to the move's
heading; 1 standing; the looks and the doors' uses: done. `hold-anim`: in it, done.

## Status

**Built** (`scripts/cast.fs`). Checked by `tests/cast/test_cast.fs` (14) and
`tests/cast/test_animal.fs` (4). *Not kept*: the models' own cloth (kind 9's strands, Fiona's
doubles' springs, kind 18's per-part draw settings) and kind 33's raised body frames.
