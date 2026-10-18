/* A sound bank's samples as WAVs: see samples.h. */
#include "samples.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>

#include "../core/files.h"
#include "vag.h"

static uint32_t rd32(const uint8_t *p) { return p[0] | p[1] << 8 | p[2] << 16 | (uint32_t)p[3] << 24; }
static unsigned rd16(const uint8_t *p) { return (unsigned)(p[0] | p[1] << 8); }

/* the Vagi chunk (its offset at +0x30 of the header): +0xC the last index, then the indices'
 * offsets, then 8 bytes an entry - the offset in the .BD, the rate (as seq.c / snddrv.c read it) */
int samples_of_hd(const uint8_t *hd, size_t size, int i, uint32_t *ssa, int *rate) {
    uint32_t vagi, n, at;

    if (hd == NULL || size < 0x34 || (vagi = rd32(hd + 0x30)) + 0x10 > size) {
        return 0;
    }
    n = rd32(hd + vagi + 0xC) + 1;
    if (n > 4096) {
        return 0;
    }
    at = vagi + 0x10 + n * 4 + (uint32_t)i * 8;
    if (i < 0 || (uint32_t)i >= n || at + 6 > size) {
        return i < 0 || (uint32_t)i >= n ? (int)n : 0;
    }
    *ssa = rd32(hd + at);
    *rate = (int)rd16(hd + at + 4);
    return (int)n;
}

void samples_free(Samples *s) {
    int i;

    if (s == NULL) {
        return;
    }
    for (i = 0; s->pcm != NULL && i < s->n; i++) {
        free(s->pcm[i]);
    }
    free(s->ssa);
    free(s->pcm);
    free(s->frames);
    free(s->loop_start);
    free(s);
}

Samples *samples_load(const char *name) {
    char path[300];
    char *text, *line, *save = NULL;
    size_t size;
    Samples *s;
    int format = 0, n = 0;

    snprintf(path, sizeof(path), "%s.samples/bank.txt", name);
    if ((text = (char *)files_read(path, &size)) == NULL || (s = calloc(1, sizeof(*s))) == NULL) {
        free(text);
        return NULL;
    }
    for (line = strtok_r(text, "\n", &save); line != NULL; line = strtok_r(NULL, "\n", &save)) {
        int i, rate;
        unsigned ssa;

        if (sscanf(line, "hga-samples %d", &format) == 1) {
            continue;
        }
        if (sscanf(line, "samples %d", &n) == 1 && s->n == 0 && n > 0 && n <= 4096) {
            s->n = n;
            s->ssa = calloc((size_t)n, sizeof(uint32_t));
            s->pcm = calloc((size_t)n, sizeof(int16_t *));
            s->frames = calloc((size_t)n, sizeof(long));
            s->loop_start = calloc((size_t)n, sizeof(long));
            if (s->ssa == NULL || s->pcm == NULL || s->frames == NULL || s->loop_start == NULL) {
                break;
            }
            continue;
        }
        if (sscanf(line, "sample %d %u %d", &i, &ssa, &rate) == 3 && i >= 0 && i < s->n) {
            uint8_t *wav;
            size_t wsize;

            s->ssa[i] = ssa;
            snprintf(path, sizeof(path), "%s.samples/%d.wav", name, i);
            if ((wav = files_read(path, &wsize)) == NULL ||
                !vag_wav_read(wav, wsize, &s->pcm[i], &s->frames[i], &s->loop_start[i])) {
                fprintf(stderr, "sound: %s isn't a WAV OpenHuey plays (mono, 16-bit)\n", path);
            }
            free(wav);
        }
    }
    free(text);
    if (format != SAMPLES_FORMAT || s->n == 0 || s->pcm == NULL) {
        fprintf(stderr, "sound: %s.samples/bank.txt isn't one this OpenHuey reads\n", name);
        samples_free(s);
        return NULL;
    }
    return s;
}

int samples_find(const Samples *s, uint32_t ssa) {
    int i;

    for (i = 0; s != NULL && i < s->n; i++) {
        if ((s->ssa[i] & ~0xFu) == (ssa & ~0xFu)) {
            return i;
        }
    }
    return -1;
}

int samples_convert(const uint8_t *hd, size_t hd_size, const uint8_t *bd, size_t bd_size, const char *dir,
                    SamplesWrote wrote, void *user) {
    char path[1200], name[32], *text;
    uint32_t ssa;
    int n = samples_of_hd(hd, hd_size, -1, &ssa, &(int){0}), i, rate, ok = 1;
    size_t len = 0, cap;
    FILE *f = NULL;

    if (n <= 0 || !SDL_CreateDirectory(dir) || (text = malloc(cap = 64 + (size_t)n * 48)) == NULL) {
        return 0;
    }
    len += (size_t)snprintf(text + len, cap - len, "hga-samples %d\nsamples %d\n", SAMPLES_FORMAT, n);
    for (i = 0; i < n && ok; i++) {
        int16_t *pcm;
        long frames, loop;
        uint8_t *wav;
        size_t wsize;

        samples_of_hd(hd, hd_size, i, &ssa, &rate);
        if (!vag_pcm(bd, bd_size, ssa, &pcm, &frames, &loop)) {   /* (none there: an empty one) */
            pcm = NULL;
            frames = 0;
            loop = -1;
        }
        wav = vag_wav(pcm, frames, rate > 0 ? rate : 48000, loop, &wsize);
        free(pcm);
        snprintf(name, sizeof(name), "%d.wav", i);
        snprintf(path, sizeof(path), "%s%s", dir, name);
        f = NULL;
        ok = wav != NULL && (f = fopen(path, "wb")) != NULL;
        if (ok) {
            ok = fwrite(wav, 1, wsize, f) == wsize;
            ok = fclose(f) == 0 && ok;
        }
        free(wav);
        if (ok && wrote != NULL) {
            wrote(user, name, (long)wsize);
        }
        len += (size_t)snprintf(text + len, cap - len, "sample %d %u %d\n", i, ssa, rate);
    }
    snprintf(path, sizeof(path), "%sbank.txt", dir);
    if (ok) {
        ok = (f = fopen(path, "wb")) != NULL;
        if (ok) {
            ok = fwrite(text, 1, len, f) == len;
            ok = fclose(f) == 0 && ok;
        }
    }
    if (ok && wrote != NULL) {
        wrote(user, "bank.txt", (long)len);
    }
    free(text);
    return ok;
}
