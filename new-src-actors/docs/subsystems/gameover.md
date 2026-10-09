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
| `game-over-kind` | `( kind -- )` | in (the story's 0x8B, `game-over-flag`) | How the next one goes: 0 the movie's, 1 faded out (the default: caught), 2 none (a continue), 3 faded out with its own music. |
| `music-op`, `bgm-want` | | out, to music | The stage music faded; its own track. |

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
3. 64 frames (the original's afterimage - its frames blended with the last, rising: not drawn
   yet), then its music at full: kind 3 track $47; else by the story (`over-music-f` .. `-b`:
   tracks $F..$B; none $A).
4. 60 frames on (kind 3: its sound $39, bank 5), the room's tint goes from where it is to
   purple ($AA0000C8, $FFFFFFFF) and its fog's colours to clear over 60 frames (each byte
   `to + t * (from - to) / 60`).
5. The movie `SYSTEM/GAMEOVER.SFD` (class 6: the three-level key, the strip across the middle);
   none - straight on.
6. Its end: the tint drifts to blue ($82000082) over 1800 frames; a face button cuts it
   short; the music stops.
7. Black over 30 frames; the world held (`world-held`), the movie closed.
8. A fresh game (`soft-reset`): the title. (A continue - kind 2 - goes there at once; the
   original's continue scene, its scene 5, isn't built: the title stands in.)

The kind 0 sequence (the movie's: as these without the freeze, its music track 6 at once, the
playing movie's last frame kept over the screen) and the special scene's (music by progress
variable $2E) are left for when the story's scenes call for them.

## Status

Built: kinds 1 / 2 / 3. `tests/gameover/test_gameover.fs` (5): begun and the world held, the
tint to purple, a face button through the drift to its end, a continue at once.
