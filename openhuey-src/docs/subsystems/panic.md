# Panic

## Purpose

The panic meter: a level 0..100 that frights and the stalker's nearness raise and time lowers.
Its stage drives Fiona: frightened at 2, panicking at 4, when she screams and runs blindly for
a while. Her breath and heartbeat follow it, the screen shakes and greys, and flashes as she panics.
The original's `Panic_*` (`fiona.c`, the object at SceneGame +0x7F8, progress +0x7B8).

## API

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `fright` | `( amount -- )` | in | A fright (a float, `f>cell`): under 10 into this frame's fear, more half of it lasting 30 frames as well (`Panic_Fright`). |
| `fear-in` | `( amount -- )` | in | A fear amount (a float), as from the table: negative a fright. |
| `stalker-doing` | `( room mode sub cond hp hp-max kind alert came -- )` | in | The stalker's kind, mode and condition, for his part of the fear. |
| `fiona-doing` | `( mode sub cond -- )` | in | From Fiona each frame: what she's doing (standing still calms faster; panicking starts only on her feet). |
| `panic-stage!` / `panic-level!` | `( stage -- )` / `( level -- )` | in | The story sets it (`Panic_SetStage`, `Panic_SetLevel`). |
| `panic` | `( stage level -- )` | broadcast, each frame | Its stage (0 calm, 1..3 at 60 / 75 / 90, 4 panicking, 5 calming down) and level (a float). |
| `danger` | | in | Calm (0) calms faster. |
| `hewie-near` | `( near -- )` | in, from Hewie each frame she is with him | How near: 1 within 20, 2 within 50, 3 further (`report_fiona_near`, the progress' +0x7B9) - how the inputs weigh. |
| `calm-add` | `( amount -- )` | in | Calming this frame, weighed as the story's (+0x7E4): Lavender's, from the items. |
| `panic-overlay` | `( thousandths -- )` | in | The screen overlay's level, 0..1000 (`ScreenFade_Level`, +0x34): her breath only when full, the overlay's tint scaled by it (`ScreenFade_Frame`). The game over sets it full and fades it with its tints; the screen's fade steps set it (`ScreenFade_Step`). |
| `overlay-held` | `( -- )` | in, from the screen | A frame of the screen's fade (`Events_ScenePlaying`): the level isn't set back to full this frame. |
| `panic-onset` | `( on -- )` | broadcast | Her panic's start - her scream, its first 30 frames (SceneGame +0xF6CD24 bit 3) - begun (-1) or over (0): the story holds its action scripts and is busy. |

## State

The stage; the level and its lasting and passing parts; this frame's inputs (fear, frights,
calming, recovery) and how they weigh; the hold; the panic's length; the breath timer; the shake;
the screen fade's level.

## Rules

From `Panic_Update`, `Panic_Stage`, `Panic_FearInputs`, `Panic_SetLevel`, `Panic_Fright`,
`Panic_Breath`. Reference: new-src's `scripts/fiona/panic.fs` (faithful; archived at the tag `new-src-final`).
1. Calming: calm danger 0.1 a frame; otherwise 0.033 while she stands still on her own.
   Recovery: 0.167 a frame when not held by a fright.
2. The level is a lasting part (raised by fear, lowered by calming) plus a passing part
   (frights, falling by the recovery). Without a new fright it stays under 100. The passing part
   lasts from frame to frame (held 30 frames by the fright, then 0.167 a frame off): only setting
   the level (`Panic_SetLevel`) clears it.
   The story's and time's inputs weigh by Hewie's nearness to her: within 20 calming x2 and fear
   x0.6, within 50 x1.6 / x0.8, further x1.5 / x0.9, without him x1.
3. The stage by the level: 60 / 75 / 90 give 1 / 2 / 3. At 100 (or set to 4) she panics:
   if she's on her feet she screams (0x42), and after 30 frames the panic lasts 450 frames
   (+300 hurt, +150 down). 500 of fear in a frame calms it to stage 5 (150 more frames). At the
   end the level drops to 50.
4. Her breath (0x29) at stages 1..5, quieter at the lower ones, every (4 - stage) x 30 frames
   (stages 4 / 5: every (6 - stage) x 7); the frame she panics, at once (the shake a step toward
   60 too). Only with the screen fade's level full (1): it is 0 while the world is held (flag 8),
   and set back to 1 at each frame of play (the heartbeat on the screen goes on regardless).
5. Charms (the sub screen's slot 1) soften frights and speed calming (*with items*).

## Status

**Built** (`scripts/panic.fs`). With each breath the pad beats (Panic_Breath's lists, `breath-rumble`:
a swell over 3 frames to 96 / 144 / 192 / 224 by the stage, at stage 4 then full a moment, at 5
the small motor with it - `rumble-step`, the rumble's sequence). Checked by `tests/panic/test_panic.fs` (with Fiona's side,
31 tests): a level set by the story and its stage; calming 0.1 a frame when calm; a fright
(half lasting, half passing); out of breath from stage 2; panicking (stage 4: her scream, the
panic run, running on blindly); stumbles, and a fall (the dice loaded) that ends it early
(stage 5) - down until it passes, then up and free; whatever happens, it passes; a fright's passing part
held 30 frames, then recovering; Hewie's nearness weighing the calming; no breath while the world
is held; panicking from the level, its first breath at once.

**The screen** (ScreenFade_Frame, its level full in play; hg's glr_negative / glr_zoom_blur /
glr_panic, OpenHuey's post.c): as she starts to panic (its first 30 frames) the screen's negative
(0x80 less each channel); at each breath the frame held and blended in again (the frame blend,
`frame-blend` - Renderer_Additive: the new frame by the pulse / 128, the pulse 0 at the breath and
back to $80 by 4 a frame), its first frames also drawn 16 pixels bigger each side over themselves
at half; and a grey by the shake (1.5 a point of level past 60, $50 panicking, $90 calming down) -
each pixel black whose green is under $30 of 255, else grey $C0, over the screen at shake / 256.
Not while a scene has the camera.

The panic hears Fiona's state as `fiona-doing` each frame. *Note:* she hears its stage a frame
later than in the original (it broadcasts at the frame's end).

The stalker's part (`Panic_FearInputs`) it works out itself each frame: in the room played, by his
kind and how near he is to her (the level distance and three times the rise; both on a ladder, by
the rise alone - not checked that it is the same ladder), not while he is down nor under flag $21.
In the mini-game's run she out of sight with him, her fear grows; away from the room played her
panic stops at 99.

**The overlay's level** (+0x34): set back to full each frame of play (not while a scene has the
camera nor under flag $F), unless the screen's fade stepped that frame (`overlay-held`); the fade's
steps set it - going out 0.58 less 0.005 a point of alpha past 16 (0 by its end), coming in 0.44 up
to full a little before its end (`ScreenFade_Step`). The out kinds' step scripts go on after their
fade, so after a fade out it stays 0 until a fade in or the next room. Under full
(`ScreenFade_Frame`): no breath, no negative, no frame blend or blur - the heartbeat's pulse back to
$80 - and the camera still while she panics or calms down or the pulse was cut short; the grey
scaled by it. Checked by `tests/story/test_story_fades.fs`.

*Not yet*: her panic's start (bit 3) also stops the characters' thinking and the camera director
for its 30 frames in the original (SubPlay: `(*flags & 0xF) == 0`) - only the story holds for it.

Also built: the story's commands (`panic-level` and the rooms' frights), the charms (the earrings
and chokers: `earrings`, `fiona/moving.fs`), the camera's shake (`jolting`), and with each breath
the pad's beat (`breath-rumble`: the table the decomp calls the stage's "tint" is a rumble
pattern, Panic_Breath's call to gRumble).
