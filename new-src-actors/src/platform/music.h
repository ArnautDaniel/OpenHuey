/* Streamed music: the game's background tracks (CRI ADX files, ADX00..02\AD_nn.ADX), one at a
 * time, decoded as they play into the mixer (the original streams them from the disc with CRI's
 * ADXT: src/game/music.c Bgm_*). A track loops between its header's loop points, or from the
 * start, if asked to. */
#ifndef MUSIC_H
#define MUSIC_H

/* start the file at `path` (in the data folder); 0 if it can't be read */
int music_play(const char *path, int loop, int paused);
void music_stop(void);
void music_pause(int on);
/* 0..1 */
void music_volume(float v);
/* a track is loaded and hasn't come to its end */
int music_playing(void);

#endif
