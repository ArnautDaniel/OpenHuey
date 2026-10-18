/* A music track's samples as the player plays them (platform/music.c), from the disc's ADX or
 * the library's FLAC (docs/library-and-mods.md): looped as the original loops it - from its
 * loop end back to its loop start (an ADX's loop points; the FLAC's LOOPSTART / LOOPEND tags),
 * or from its end to its start if it has none.
 *
 * ADX: a big-endian header - 0x8000, the data's offset - 4 at +0x2, encoding +0x4 (3), block
 * size +0x5 (18), channels +0x7, rate +0x8, samples +0xC, the high-pass cutoff +0x10, version
 * +0x12 (3: loop info from +0x18, 4: from +0x24 - enabled, start sample, start byte, end
 * sample, end byte); then per 32 samples a block per channel: a scale and 16 bytes of 4-bit
 * deltas, predicted from the two samples before. The player goes by the loop's bytes (whole
 * blocks), and a block's prediction carries on across the jump - so after a jump the loop
 * doesn't start as it did the first time (for up to a few thousand samples), the same every time
 * after. The FLAC holds that too: the first pass, then the start of the second as far as it
 * differs (K samples), its LOOPSTART K samples into the loop; STOPAT where it ends not looped. */
#ifndef MUSICSRC_H
#define MUSICSRC_H

#include <stddef.h>
#include <stdint.h>

#define MUSICSRC_FRAME 32   /* samples at most a step gives */

typedef struct MusicSrc MusicSrc;

/* the track in `data` (taken: freed with it), ADX or FLAC; NULL if it is neither */
MusicSrc *musicsrc_open(uint8_t *data, size_t size, int loop);
void musicsrc_free(MusicSrc *m);
int musicsrc_rate(const MusicSrc *m);
int musicsrc_channels(const MusicSrc *m);   /* 1 or 2 */
/* the next samples (pcm[channel][i]): how many, 0 at the end (a track not looped) */
int musicsrc_next(MusicSrc *m, int16_t pcm[2][MUSICSRC_FRAME]);

/* an ADX decoded whole as the FLAC holds it (interleaved, malloc'd; see above): its loop and
 * where it stops not looped, in samples - the import's. 0: not an ADX, or one whose passes
 * after a jump aren't all alike (none of the game's) */
int musicsrc_adx_pcm(const uint8_t *data, size_t size, int16_t **pcm, long *frames, int *channels, int *rate,
                     long *loop_start, long *loop_end, long *stop);

#endif
