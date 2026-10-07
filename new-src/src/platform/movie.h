/* The game's movies (CRI Sofdec .SFD): MPEG program streams of MPEG-2 video (stream 0xE0) and ADX
 * sound (0xC0). Demuxed and the sound decoded here; the video by FFmpeg's libavcodec, loaded at
 * run time (without it there are no movies: they end at once). Ported from the decomp's
 * native/platform/sofdec.c. One movie plays at a time; its sound goes into the mixer. */
#ifndef MOVIE_H
#define MOVIE_H

#include <stdint.h>

enum { MOVIE_NONE = 0, MOVIE_PLAYING, MOVIE_ENDED };

/* start the movie at `path` (in the data folder, e.g. "OPENING.SFD"); 0 if it can't */
int movie_open(const char *path);
void movie_close(void);
/* each tick: the clock on, frames decoded up to it */
void movie_update(void);
int movie_status(void);
/* the frame shown (from 0; -1 none yet) */
int movie_frame(void);
/* the frame's picture (RGBA, w x h; NULL none); *fresh: new since the last call */
const uint8_t *movie_picture(int *w, int *h, int *fresh);
void movie_pause(int on);
int movie_paused(void);
void movie_volume(float v);   /* 0..1 */

#endif
