# Panic

## Purpose

The panic meter: a level 0..100 that frights and the stalker's nearness raise and time lowers.
Its stage drives Fiona: frightened at 2, panicking at 4, when she screams and runs blindly for
a while. Her breath and heartbeat follow it, the screen shakes and tints (*with effects*).
The original's `Panic_*` (`fiona.c`, the object at SceneGame +0x7F8, progress +0x7B8).

## API

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `fright` | `( amount -- )` | in | A fright (a float, `f>cell`): under 10 into this frame's fear, more half of it lasting 30 frames as well (`Panic_Fright`). |
| `fear-in` | `( amount -- )` | in | This frame's fear (a float): the stalker's nearness (`Panic_FearInputs`). |
| `fiona-doing` | `( mode sub cond -- )` | in | From Fiona each frame: what she's doing (standing still calms faster; panicking starts only on her feet). |
| `panic-stage!` / `panic-level!` | `( stage -- )` / `( level -- )` | in | The story sets it (`Panic_SetStage`, `Panic_SetLevel`). |
| `panic` | `( stage level -- )` | broadcast, each frame | Its stage (0 calm, 1..3 at 60 / 75 / 90, 4 panicking, 5 calming down) and level (a float). |
| `danger` | | in | Calm (0) calms faster. |

## State

The stage; the level and its lasting and passing parts; this frame's inputs (fear, frights,
calming, recovery); the hold; the panic's length; the breath timer; the shake.

## Rules

From `Panic_Update`, `Panic_Stage`, `Panic_FearInputs`, `Panic_SetLevel`, `Panic_Fright`,
`Panic_Breath`. Reference: new-src's `scripts/fiona/panic.fs` (faithful).
1. Calming: calm danger 0.1 a frame; otherwise 0.033 while she stands still on her own.
   Recovery: 0.167 a frame when not held by a fright.
2. The level is a lasting part (raised by fear, lowered by calming) plus a passing part
   (frights, falling by the recovery). Without a new fright it stays under 100.
3. The stage by the level: 60 / 75 / 90 give 1 / 2 / 3. At 100 (or set to 4) she panics:
   if she's on her feet she screams (0x42), and after 30 frames the panic lasts 450 frames
   (+300 hurt, +150 down). 500 of fear in a frame calms it to stage 5 (150 more frames). At the
   end the level drops to 50.
4. Her breath (0x29) at stages 1..5, quieter at the lower ones, every (4 - stage) x 30 frames
   (stages 4 / 5: every (6 - stage) x 7).
5. Charms (the sub screen's slot 1) soften frights and speed calming (*with items*).

## Status

**Built** (`scripts/panic.fs`). Checked by `tests/panic/test_panic.fs` (with Fiona's side,
17 tests): a level set by the story and its stage; calming 0.1 a frame when calm; a fright
(half lasting, half passing); out of breath from stage 2; panicking (stage 4: her scream, the
panic run, running on blindly); stumbles, and a fall (the dice loaded) that ends it early
(stage 5) - down until it passes, then up and free; whatever happens, it passes.

The panic hears Fiona's state as `fiona-doing` each frame. *Note:* she hears its stage a frame
later than in the original (it broadcasts at the frame's end).

Left: the stalker's fear (`fear-in`, from the stalkers), the story's commands (with the story),
charms (items), the screen's shake and tint (effects).
