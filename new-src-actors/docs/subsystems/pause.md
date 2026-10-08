# The pause

## Purpose

The pause screen (`src/game/pause.c`, SceneGame +0x73EBA0). Its movie half is built: Start
while a scene's movie plays pauses it and dims the screen with "Unpause game" / "Skip". Start
goes back to it; Cancel skips it - the screen darkens to black and `movie-skipped` is set, which
the scene's script reads to end the scene; the script then sets `pause-wanted`, which closes it.

## API

No messages: it reads the buttons (`keys.fs`: `start-button`, `cross`), the state flags
(`world-held`, `no-pause`, `movie-skipped`, `pause-wanted`) and the scene (`cutscene-active?`,
`movie-status`). `pause-wanted` is cleared each frame, as the original's flag 6 is.

**Draws** on UI layer 5 (over everything): the dimming (alpha 63 of 128 open, to full black
skipping) and the two lines at (320, 400) / (320, 420) of the PS2's 512 x 448.

## Rules

1. It opens only while a scene with a movie has the camera, the world not held, pausing
   allowed (`SceneGame_SubPlay`).
2. Opening and closing take 5 frames (0.2 a frame); the skip's darkening too, then the flag.
3. The world goes on while it is open (`SceneGame_SubMoviePaused`); the movie is paused.

## Status

**Movie pause built** (`scripts/pause.fs`), with the story's S4. Not yet: the play pause ("back"
/ "quit", the confirmation), the sound turned down while open (`duck`), the message-only mode
(no pad).
