# The game over

## Purpose

When the game ends - Fiona dragged or carried off, held by a stalker, or the story's own ends -
the world stands still behind a short sequence: its music, the room going purple, the
GAMEOVER title, a slow drift to blue, black. Then a fresh game from the title, as the original
builds its title scene anew.

## API

| Message / fact | Stack | Direction | Meaning |
|---|---|---|---|
| `caught` (state flag $C) | | fact, set by Fiona (`fiona/seized.fs`) or the story | The game ends: it starts (the flag taken at once). |
| `game-over-kind` | `( kind -- )` | in (the story's 0x8B, `game-over-flag`) | How the next one goes: 0 the movie's, 1 faded out (the default: caught), 2 the story's end - none, the ending follows (`the-ending`, `ending.md`), 3 faded out with its own music. |
| `music-op`, `bgm-want` | | out, to music | The stage music faded; its own track. |
| `panic-overlay` | `( thousandths -- )` | out, to the panic | Its screen overlay's level (ScreenFade_Level of the progress' +0x7B8): full as the tints start, fading with them; 0 when a movie death keeps its picture. (The panic's handler is another change's: until it is in, the message is dropped.) |
| `panic-stage!`, `door-opened`, `lock-room-for`, `hewie-place`, `go-to-room` | | out | The mini-game's run started again in place (below). |

Engine words it uses: `still!` (the models and the room stand still, the actors running),
`look@` / `look-set` (the room's tint $1F and fog $1D), `movie-open`, `ui-movie`, `ui-rect`,
`soft-reset` (a fresh game at the frame's end: the actors, models, room, effects, 2D, sounds,
music, movie and progress let go, the scripts started again - the title).

## State

`kind`, `step` (-1 idle, 0..11 the sequence, 12 done), `timer`, the tint's and fog's colours
it fades from and to.

## Rules (src/game/gameover.c GameOver_StateOthers / StateMode3 - game_over; SceneGame_SubTransition)

1. The stage music faded over 30 frames; any movie stopped.
2. The world held: the panic (`panic-held`), the models and the room still.
3. 64 frames of the afterimage (Renderer_Additive: each frame blended with the last, by 0 .. 63 of
   128 rising - `frame-blend`), then its music at full: kind 3 track $47; else by the story (`over-music-f` .. `-b`:
   tracks $F..$B; none $A).
4. 60 frames on (kind 3: its sound $39, bank 5), the room's tint goes from where it is to
   purple ($AA0000C8, $FFFFFFFF) and its fog's colours to clear over 60 frames (each byte
   `to + t * (from - to) / 60`); the panic's screen overlay set full, then fading with them
   (`timer / 60`: `panic-overlay`).
5. The movie `SYSTEM/GAMEOVER.SFD` (class 6: the three-level key, the strip across the middle);
   none - straight on.
6. Its end: the tint drifts to blue ($82000082) over 1800 frames; a face button cuts it
   short; the music stops.
7. Black over 30 frames; the world held (`world-held`), the movie closed.
8. A fresh game (`soft-reset`): the title. In the mini-game's run (Hewie played,
   SceneGame_SubTransition) it starts again in place instead: Fiona played again, the camera at
   its pace, the panic's stage 0, door $10C shut (its open bit), this room's doors unlocked to
   Hewie, him put in the extras room ($37, no triangle), the stalker gone, the extras room
   entered by exit $81 (its script: Fiona's action $C); the freeze let go. (Kind 2 - the story's end - is over at once: it
   broadcasts `the-ending`, and the ending (the original's scene 5, `ending.md`) takes over.)

The kind 0 sequence (the movie's: as these without the freeze, its music track 6 at once, the
playing movie's last frame kept over the screen) and the special scene's (music by progress
variable $2E) are left for when the story's scenes call for them.

## Status

Built: kinds 1 / 2 / 3. In the mini-game's run any kind is the run's own (GameOver_StateSpecial:
faded out, its music by the challenge - A $A, B $B, C $F, D $C). `tests/gameover/test_gameover.fs` (5): begun and the world held, the
tint to purple, a face button through the drift to its end, the story's end (kind 2) at once,
the movie death, the run's game over back in the extras room.
