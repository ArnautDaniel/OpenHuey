# The screen (fades)

## Purpose

Black over the picture, faded in or out over some frames: the rooms' scripts fade out before a
scene or a change and in after it, and wait for the fade. The original's event fade (+0x20,
`Events_Fade`, the scripts' 0x5C..0x5F).

## API

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `fade` | `( frames kind -- )` | in (a room) | Fade over `frames` frames; kind & 0xF: even out, odd in - 0 / 4 to black, 1 / 5 from black, 2 / 3 to / from white, 6 to half dark, 7 back from it (ScreenFade_Step). 0 frames: at once. |
| `fade-step` | `( -- )` | in | A room's own 0x5D: the fade a step on (while on), a fade's frame for the panic. |
| `fade-stop` | `( -- )` | in | A room's own 0x5E: the fade counts as over where it is. |
| `fade-done` | `( -- )` | out, to who asked | The fade is over. |

**Draws** a black rectangle on UI layer 1 (under the message window), as opaque as the level
(0 clear .. 1000 black).

## Rules

1. The level goes in a straight line from where it starts (1000 in, 0 out) to where it goes.
2. A new fade starts from its own start, not from the level reached.
3. It goes by its kind's step script (D_003D6A40, run by the screen; its first step in the frame it
   starts): a step a frame while its steps haven't passed its frames (frames + 1 steps), then over
   (`fade-done`). Kind 4 has 15 steps more before it is over ($BC), kind 5 its first 16 steps
   before the check ($BD: over no sooner than its 17th frame). The out kinds' scripts (0 2 4 6) go
   on after, a fade's frame each frame without stepping, until the next fade or a room entered.
4. Each step sets the panic's overlay level (kinds 0..5: `panic-overlay`, ScreenFade_Step's
   formula by alpha = 128 / frames (whole) x steps) and each frame of the script tells it
   `overlay-held` (Events_ScenePlaying).

## Status

**Built** (`scripts/screen.fs`), with the story's S4. The kinds' bits $C0 fade the stage music
with it (the story's `fade-music`: `music-cut` at once, `music-eased` over 90 frames - silent going
to black, full coming back). Bits $30 set a volume ramp (Progress +0x1120) that nothing in the
original reads: left out. Checked by `tests/story/test_finishing.fs`, `test_scene.fs`,
`test_story_fades.fs`. *Note:* the drawn level goes linearly to its end; the original's alpha
(128 / frames whole) stops short of full for some frames (15: 120 of 128) - not drawn so.
*Not yet*: during her panic's start the original's step script (a character script) is held too.
