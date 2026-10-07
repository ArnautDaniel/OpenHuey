/* The game's sound banks: NAME.HD (the Sony "SCEI Vers" header: programs, splits, samples,
 * sample offsets), NAME.SDT (Capcom's sound table: 16-byte entries, one or more per sound id)
 * and NAME.BD (the samples, PS2 ADPCM). The sound driver (platform/snddrv.c) plays them. */
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


#endif
