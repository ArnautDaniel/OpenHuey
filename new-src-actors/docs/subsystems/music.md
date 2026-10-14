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
| `music-op` | `( op a b -- )` | in | The stage music (0x6A): 0 the global volume to a over b frames; 1 the stage's chase channels by the story's flags; 2 hold, 3 wait for the banks (loaded at once here), 4 release (it starts once released and loaded); 5 silence. |
| `danger` / `panic` / `stalker-doing` | | in (broadcasts) | What the stage music follows: the danger's level, the panic's stage, the stalker (his alert: 0 he sees her; his room). |
| `music-stage` | `( stage -- )` | in | The stage music made for stage set `stage` (0..3: `BGM/STAGEn_BANK` and its sequences; -1 ended). |

**Facts:** `music-play` / `-stop` / `-pause` / `-volume!` (the stream), `seq-*` (the sequencer),
the tables in `story/strings.fs` (`bgm-track`, `stage-table`, `stage-file`).

## Rules

1. The stream's loudness is the original's 100 log10 v tenths of a dB: an amplitude of the
   square root of the fade times the level.
2. The stage music started: the calm parts full, the chase and panic silent, the global volume
   fading in over 90 frames; each sequence's volume is its own times the global one.

3. Following the danger (MusicDir_StateFrame; not while a scene plays or the world is held):
   calm - the calm parts full (over 90 frames), the panic and chase silent; followed - the calm
   parts fade out; chased - the chase from its start (at once, or over 90 frames after a panic),
   the calm parts out: while the stalker sees her (his alert 0) loud (255) with its sequence
   volume up and the stage's "seen" channels bent full (MusicStageN_Seen); else (Lost: the
   volume back, those channels centred) by how near he is - MusicDir_ChaseStep's table, the
   same for every stage: within 30 210 and bend 111, ... within 150 115 / 48, further 105 / 43;
   no stalker in the game, silent - fading there at 2.5 a frame, the stage's flagged channels
   bent along (channel 0; stage 3 channel 5 too). At the panic's stage 4 the panic alone at
   once, its channels 3..8 silent; at stage 5 they are back; below 4, back to the danger's music.
4. Op 1 (MusicStageN_StageChans): the chase's channel sets by the story's flags - stage 1 $1B
   (channels 2..5 or 6..8), stage 3 $89 (5, 6), stage 4 $95 (3, 4 or 5..7).

## Status

**Built** (`scripts/music.fs`), with the story's S4, ported from new-src's `play.fs`; the danger
followed. The pause turns it down (`pause.fs`). Checked by `tests/music/test_music.fs` (11).
