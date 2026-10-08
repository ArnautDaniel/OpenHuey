# Music

## Purpose

The game's two kinds of music: the background track (`BgmCtl`: a streamed ADX track the rooms'
scripts want, faded in and out) and the stage music (`MusicDir`: a stage set's bank and four
sequences - the calm music's two parts, the chase, the panic - mixed by their volumes). The
rooms' scripts ask; this actor plays them each frame.

## API

| Message | Stack | Direction | Meaning |
|---|---|---|---|
| `bgm-want` | `( track pause level -- )` | in (a room) | Track `track` wanted (`$FF`: none - the playing one fades out over 30 frames), paused or not, at `level` (a float in a cell). A new track starts at once; wanted again, a fading one comes back. |
| `bgm-resume` | `( -- )` | in | The stream goes on (the scripts' `$FF` pause). |
| `music-op` | `( op a b -- )` | in | The stage music (0x6A): 0 the global volume to a over b frames; 2 hold, 4 release (it starts once released and loaded); 5 silence. |
| `music-stage` | `( stage -- )` | in | The stage music made for stage set `stage` (0..3: `BGM/STAGEn_BANK` and its sequences; -1 ended). |

**Facts:** `music-play` / `-stop` / `-pause` / `-volume!` (the stream), `seq-*` (the sequencer),
the tables in `story/strings.fs` (`bgm-track`, `stage-table`, `stage-file`).

## Rules

1. The stream's loudness is the original's 100 log10 v tenths of a dB: an amplitude of the
   square root of the fade times the level.
2. The stage music started: the calm parts full, the chase and panic silent, the global volume
   fading in over 90 frames; each sequence's volume is its own times the global one.

## Status

**Built** (`scripts/music.fs`), with the story's S4, ported from new-src's `play.fs`. Not yet:
the chase and panic parts following the danger, op 1 (the stage's channels), op 3 (waiting for
the banks), the pause turning the music down.
