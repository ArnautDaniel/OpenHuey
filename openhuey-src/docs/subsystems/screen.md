# The screen (fades)

## Purpose

Black over the picture, faded in or out over some frames: the rooms' scripts fade out before a
scene or a change and in after it, and wait for the fade. The original's event fade (+0x20,
`Events_Fade`, the scripts' 0x5C..0x5F).

## API

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `fade` | `( frames kind -- )` | in (a room) | Fade over `frames` frames; kind & 0xF: even out, odd in - 0 / 4 to black, 1 / 5 from black, 2 / 3 to / from white, 6 to half dark, 7 back from it (ScreenFade_Step). 0 frames: at once. |
| `fade-finish` | `( -- )` | in | The fade is over now (where it was going). |
| `fade-done` | `( -- )` | out, to who asked | The fade is over. |

**Draws** a black rectangle on UI layer 1 (under the message window), as opaque as the level
(0 clear .. 1000 black).

## Rules

1. The level goes in a straight line from where it starts (1000 in, 0 out) to where it goes.
2. A new fade starts from its own start, not from the level reached.

## Status

**Built** (`scripts/screen.fs`), with the story's S4. The kinds' bits $C0 fade the stage music
with it (the story's `fade-music`: `music-cut` at once, `music-eased` over 90 frames - silent going
to black, full coming back). Bits $30 set a volume ramp (Progress +0x1120) that nothing in the
original reads: left out. Checked by `tests/story/test_finishing.fs`.
