/* Sound out: one SDL3 audio stream (48 kHz stereo) that streams are mixed into - a movie's
 * sound, the music, the sound processor's voices (spu.c). */
#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>

#define SOUND_RATE 48000

/* open the audio device; 0 if there is none (the game goes on silent) */
int sound_open(void);
void sound_close(void);
/* a stream mixed in as well (a movie's sound): it adds `frames` stereo frames into `out`;
 * NULL none */
void sound_stream(void (*mix)(int16_t *out, int frames));
/* more streams (slots 1..3: 1 the music, 2 the sound processor), mixed after the movie's */
void sound_stream_slot(int slot, void (*mix)(int16_t *out, int frames));

#endif
