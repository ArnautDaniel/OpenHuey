/* Writing FLAC (the import's music, docs/library-and-mods.md): 16-bit, one or two channels,
 * lossless - read back, the same samples. Blocks of 4096; in each the fixed predictor and the
 * stereo coding (left/right, left/side, side/right, mid/side) that come out smallest, the
 * residuals Rice-coded. Tags ("NAME=value") in a Vorbis comment. */
#ifndef FLACENC_H
#define FLACENC_H

#include <stddef.h>
#include <stdint.h>

/* the FLAC file (malloc'd) of `frames` samples of `channels` (interleaved); NULL if out of memory */
uint8_t *flacenc(const int16_t *pcm, long frames, int channels, int rate, const char *const *tags, int ntags,
                 size_t *size);

#endif
