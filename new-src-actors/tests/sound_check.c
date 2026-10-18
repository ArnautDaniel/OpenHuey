/* The sound banks' conversion checked (docs/library-and-mods.md): every sample of every bank
 * (each .HD's Vagi entry, out of its .BD) made a WAV as the import makes it, read back, and both
 * played as the SPU plays them (data/vag.h's cursor) - the same samples, block by block, through
 * three passes of its loop.
 *
 *   sound-check <data folder> */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <SDL3/SDL.h>

#include "../src/data/samples.h"
#include "../src/data/vag.h"

static int sBanks, sSamples, sLooped, sBad, sUnplayable;
static long long sBd, sWav;
static size_t sRoot;

static void check(const char *name, const uint8_t *hd, size_t hs, const uint8_t *bd, size_t bs) {
    uint32_t ssa;
    int rate, n = samples_of_hd(hd, hs, -1, &ssa, &rate), i;

    sBanks++;
    sBd += (long long)bs;
    for (i = 0; i < n; i++) {
        int16_t *pcm, *back;
        long frames, loop, bframes, bloop, blocks, k;
        uint8_t *wav;
        size_t wsize;
        VagCursor a, b;

        samples_of_hd(hd, hs, i, &ssa, &rate);
        sSamples++;
        if (!vag_pcm(bd, bs, ssa, &pcm, &frames, &loop)) {
            sUnplayable++;   /* (nothing there to play: the import writes it empty) */
            continue;
        }
        wav = vag_wav(pcm, frames, rate > 0 ? rate : 48000, loop, &wsize);
        sWav += (long long)wsize;
        if (!vag_wav_read(wav, wsize, &back, &bframes, &bloop) || bframes != frames || bloop != loop) {
            fprintf(stderr, "FAIL %s sample %d: the WAV doesn't read back as written\n", name, i);
            sBad++;
            free(pcm);
            free(wav);
            continue;
        }
        sLooped += loop >= 0;
        memset(&a, 0, sizeof(a));
        a.mem = bd;
        a.size = bs;
        a.ssa = ssa & ~0xFu;
        memset(&b, 0, sizeof(b));
        b.pcm = back;
        b.frames = bframes;
        b.loop_start = bloop;
        vag_start(&a);
        vag_start(&b);
        blocks = frames / VAG_BLOCK + 3 * (loop >= 0 ? (frames - loop) / VAG_BLOCK + 1 : 0) + 2;
        for (k = 0; k < blocks; k++) {
            if (a.ended != b.ended || (!a.ended && memcmp(a.buf, b.buf, sizeof(a.buf)) != 0)) {
                fprintf(stderr, "FAIL %s sample %d: block %ld differs (%s)\n", name, i, k,
                        a.ended != b.ended ? "one ended" : "its samples");
                sBad++;
                break;
            }
            vag_next(&a);
            vag_next(&b);
        }
        free(pcm);
        free(back);
        free(wav);
    }
}

static SDL_EnumerationResult one(void *user, const char *dir, const char *name) {
    char path[1300];
    SDL_PathInfo info;
    size_t len = strlen(name), hs, bs;
    uint8_t *hd, *bd;

    snprintf(path, sizeof(path), "%s%s", dir, name);
    if (!SDL_GetPathInfo(path, &info)) {
        return SDL_ENUM_CONTINUE;
    }
    if (info.type == SDL_PATHTYPE_DIRECTORY) {
        strcat(path, "/");
        SDL_EnumerateDirectory(path, one, user);
        return SDL_ENUM_CONTINUE;
    }
    if (len < 4 || strcasecmp(name + len - 3, ".HD") != 0 || (hd = SDL_LoadFile(path, &hs)) == NULL) {
        return SDL_ENUM_CONTINUE;
    }
    memcpy(path + strlen(path) - 2, "BD", 2);
    if ((bd = SDL_LoadFile(path, &bs)) != NULL) {
        check(path + sRoot, hd, hs, bd, bs);
        SDL_free(bd);
    }
    SDL_free(hd);
    return SDL_ENUM_CONTINUE;
}

int main(int argc, char **argv) {
    char data[1100];

    if (argc != 2) {
        fprintf(stderr, "usage: sound-check <data folder>\n");
        return 2;
    }
    if (!SDL_GetPathInfo(argv[1], NULL)) {
        printf("sound-check: no game data at %s - skipped\n", argv[1]);
        return 77;
    }
    snprintf(data, sizeof(data), "%s/", argv[1]);
    sRoot = strlen(data);
    SDL_EnumerateDirectory(data, one, NULL);
    printf("sound-check: %d banks (%lld KB) - %d samples (%d looped, %d with nothing to play) as %lld KB of WAV, %d "
           "failures\n", sBanks, sBd >> 10, sSamples, sLooped, sUnplayable, sWav >> 10, sBad);
    return sBad != 0 || sBanks == 0;
}
