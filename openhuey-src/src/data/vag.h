/* A sound bank's sample as the SPU plays it (platform/spu.c; psx-spx "SPU ADPCM"), from the
 * bank's PS-ADPCM (NAME.BD) or the library's WAV of it (docs/library-and-mods.md).
 *
 * PS-ADPCM: 16-byte blocks of 28 samples - a shift and filter byte, a flags byte (1 the end, 2
 * looped, 4 the loop's start), 14 bytes of 4-bit deltas, each predicted from the two samples
 * before. A sample plays from its start to the block flagged its end, then goes back to the
 * last loop-start block met (or stops); the prediction carries on across the jump, so the loop
 * doesn't start the second time as it did the first - the same every time after. The WAV holds
 * that too (as the library's music does): the first pass, then the second pass's start as far as
 * it differs (whole blocks), its loop (a "smpl" chunk) that much into the loop. */
#ifndef VAG_H
#define VAG_H

#include <stddef.h>
#include <stdint.h>

#define VAG_BLOCK 28   /* samples a block */

typedef struct VagCursor {
    const uint8_t *mem;           /* PS-ADPCM: the bank's samples */
    size_t size, ssa, lsa, nax;   /* bytes into mem: the start, the loop, the block playing */
    const int16_t *pcm;           /* or the samples themselves (mem NULL) */
    long frames, loop_start;      /* (loop_start < 0: it stops at its end) */
    int s1, s2;
    int16_t buf[VAG_BLOCK];       /* the block playing */
    int ended;                    /* it has stopped (or couldn't start) */
} VagCursor;

/* from the start: its first block in buf */
void vag_start(VagCursor *c);
/* the next block in buf (back to the loop after the last), or ended */
void vag_next(VagCursor *c);

/* the sample at `ssa` in a bank's .BD as the WAV holds it (malloc'd) and its loop (-1: none);
 * 0 if it doesn't play the same each time after its first (none of the game's) */
int vag_pcm(const uint8_t *bd, size_t size, size_t ssa, int16_t **pcm, long *frames, long *loop_start);
/* a WAV file of it (malloc'd): mono, 16-bit, at `rate`, its loop a "smpl" chunk */
uint8_t *vag_wav(const int16_t *pcm, long frames, int rate, long loop_start, size_t *size);
/* a WAV file read (mono, 16-bit PCM): its samples (malloc'd: up to its loop's end) and loop
 * (-1: none); 0 if it isn't one */
int vag_wav_read(const uint8_t *data, size_t size, int16_t **pcm, long *frames, long *loop_start);

#endif
