/* Sound out: mono 16-bit sounds mixed into one SDL3 audio stream (48 kHz stereo). */
#ifndef SOUND_H
#define SOUND_H

#include <stdint.h>

#define SOUND_RATE 48000

/* open the audio device; 0 if there is none (the game goes on silent) */
int sound_open(void);
void sound_close(void);
/* play `n` samples (copied) at volume 0..1, pan -1 (left) .. 1 (right) */
void sound_play(const int16_t *pcm, int n, float volume, float pan);

#endif
