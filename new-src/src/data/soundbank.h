/* The game's sound banks: NAME.HD (the Sony "SCEI Vers" header: programs, splits, samples,
 * sample offsets), NAME.SDT (Capcom's sound table: 16-byte entries, one or more per sound id)
 * and NAME.BD (the samples, PS2 ADPCM). A sound is rendered here to plain PCM the way the IOP
 * sound driver plays it (native/platform/snddrv.c in the decomp: play_sound, read_tone): its
 * entries, each a program / split of the header whose layers are samples, at the note and
 * volume the entry and the header give. Envelopes, loops and 3-D placement are not kept yet. */
#ifndef SOUNDBANK_H
#define SOUNDBANK_H

#include <stddef.h>
#include <stdint.h>

typedef struct SoundBank {
    uint8_t *hd, *sdt, *bd;
    size_t hd_size, sdt_size, bd_size;
} SoundBank;

/* load NAME.HD / .SDT / .BD from the data folder ("C_0000": the common bank); 0 on failure */
int soundbank_load(SoundBank *b, const char *name);
void soundbank_free(SoundBank *b);
/* sound `id` as mono 16-bit samples at `rate` Hz (malloc'd; NULL if it has none); *n samples */
int16_t *soundbank_render(const SoundBank *b, int id, int rate, int *n);

#endif
