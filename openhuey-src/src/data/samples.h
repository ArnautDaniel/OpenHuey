/* A sound bank's samples as the library holds them (docs/library-and-mods.md): its .BD (the
 * PS-ADPCM samples, one for each of the .HD's "Vagi" entries) as a folder of WAVs - NAME.BD ->
 * NAME.samples/: bank.txt and <i>.wav (data/vag.h: the samples as the SPU plays them, a loop a
 * "smpl" chunk). The .HD and .SDT stay as they are; the voices still find a sample by its place
 * in the .BD (the .HD's), which bank.txt maps to its WAV.
 *
 *   bank.txt:  hga-samples 1
 *              samples <n>
 *              sample <i> <its offset in the .BD> <its rate (the .HD's)>
 */
#ifndef SAMPLES_H
#define SAMPLES_H

#include <stddef.h>
#include <stdint.h>

#define SAMPLES_FORMAT 1

typedef struct Samples {
    int n;
    uint32_t *ssa;
    int16_t **pcm;
    long *frames, *loop_start;
} Samples;

/* bank NAME's samples ("C_0000" -> "C_0000.samples/"), NULL if it has none converted */
Samples *samples_load(const char *name);
void samples_free(Samples *s);
/* the sample that starts at `ssa` in the bank's .BD: its index, or -1 */
int samples_find(const Samples *s, uint32_t ssa);

/* the import's: the bank's samples (its .HD's Vagi entries, out of its .BD) into the folder
 * `dir` (made; ends with "/"), each file written told to `wrote`; 0 if a sample can't be */
typedef void (*SamplesWrote)(void *user, const char *name, long size);
int samples_convert(const uint8_t *hd, size_t hd_size, const uint8_t *bd, size_t bd_size, const char *dir,
                    SamplesWrote wrote, void *user);
/* the .HD's samples: how many, and each one's offset and rate; 0 if it has no Vagi table */
int samples_of_hd(const uint8_t *hd, size_t size, int i, uint32_t *ssa, int *rate);

#endif
