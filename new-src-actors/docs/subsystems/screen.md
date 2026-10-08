# The screen (fades)

## Purpose

Black over the picture, faded in or out over some frames: the rooms' scripts fade out before a
scene or a change and in after it, and wait for the fade. The original's event fade (+0x20,
`Events_Fade`, the scripts' 0x5C..0x5F).

## API

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `fade` | `( frames kind -- )` | in (a room) | Fade over `frames` frames; kind & 0xF 0 / 1 in (from black), else out (to black). 0 frames: at once. |
| `fade-finish` | `( -- )` | in | The fade is over now (where it was going). |
| `fade-done` | `( -- )` | out, to who asked | The fade is over. |

**Draws** a black rectangle on UI layer 1 (under the message window), as opaque as the level
(0 clear .. 1000 black).

## Rules

1. The level goes in a straight line from where it starts (1000 in, 0 out) to where it goes.
2. A new fade starts from its own start, not from the level reached.

## Status

**Built** (`scripts/screen.fs`), with the story's S4. Not yet: the kinds' bits 0xC0 (the music
faded with it: 0x80 at once, 0x40 over 90 frames) and 0x30 (the volume ramped).
