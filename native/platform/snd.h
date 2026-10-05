/* The PC sound output (snd_mix.c) and its sources. */
#ifndef HG_SND_H
#define HG_SND_H

#include <stdio.h>

#define SND_RATE 48000

int snd_init(void);
void snd_lock(void);
void snd_unlock(void);
float snd_db10(int db10);

/* sources: add `frames` stereo float frames into `out` (called with the lock held) */
void adx_render(float *out, int frames);
void spu_render(float *out, int frames);
void sfd_render(float *out, int frames);   /* a movie's sound (sofdec.c) */

/* the folder (under the data folder) of a CRI directory listing (crifs.c), "" for the root */
const char *crifs_folder(const void *list);
/* open `name` in `folder` case-insensitively */
FILE *crifs_open(const char *folder, const char *name);

#endif
