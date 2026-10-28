/* The music's conversion checked (docs/library-and-mods.md): every ADX of the game data made a
 * FLAC as the import makes it, and both played as the player plays them (data/musicsrc.c) -
 * once through, and looped for three passes - must give the same samples.
 *
 *   music-check <data folder> */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#include <SDL3/SDL.h>

#include "../src/data/flacenc.h"
#include "../src/data/musicsrc.h"
#include "full_only.h"

static int sBad, sTracks;
static long long sAdxBytes, sFlacBytes;
static size_t sRoot;
static long sLead;   /* the samples of second passes' starts kept */

static uint8_t *copy(const uint8_t *p, size_t n) {
    uint8_t *c = malloc(n);

    memcpy(c, p, n);
    return c;
}

/* `count` samples of each played (or until both end): the first that differs, -1 none */
static long compare(const char *name, const uint8_t *adx, size_t asize, const uint8_t *flac, size_t fsize, int loop,
                    long count) {
    MusicSrc *a = musicsrc_open(copy(adx, asize), asize, loop), *b = musicsrc_open(copy(flac, fsize), fsize, loop);
    int16_t pa[2][MUSICSRC_FRAME], pb[2][MUSICSRC_FRAME];
    int na = 0, nb = 0, ia = 0, ib = 0, c;
    long i, differs = -1;

    if (a == NULL || b == NULL || musicsrc_channels(a) != musicsrc_channels(b) || musicsrc_rate(a) != musicsrc_rate(b)) {
        fprintf(stderr, "FAIL %s: the FLAC doesn't open as the ADX does\n", name);
        musicsrc_free(a);
        musicsrc_free(b);
        return 0;
    }
    for (i = 0; i < count && differs < 0; i++) {
        if (ia >= na) {
            na = musicsrc_next(a, pa);
            ia = 0;
        }
        if (ib >= nb) {
            nb = musicsrc_next(b, pb);
            ib = 0;
        }
        if (na <= 0 || nb <= 0) {
            differs = (na <= 0) == (nb <= 0) ? -1 : i;   /* (both ended: the same) */
            break;
        }
        for (c = 0; c < musicsrc_channels(a); c++) {
            if (pa[c][ia] != pb[c][ib]) {
                differs = i;
            }
        }
        ia++;
        ib++;
    }
    musicsrc_free(a);
    musicsrc_free(b);
    return differs;
}

static SDL_EnumerationResult one(void *user, const char *dir, const char *name) {
    char path[1300];
    SDL_PathInfo info;
    size_t len = strlen(name), asize, fsize;
    uint8_t *adx, *flac;
    int16_t *pcm;
    long frames, ls, le, stop, at;
    int ch, rate;
    char start[48], end[48], stopat[48];
    const char *tags[3] = {start, end, stopat};

    snprintf(path, sizeof(path), "%s%s", dir, name);
    if (!SDL_GetPathInfo(path, &info)) {
        return SDL_ENUM_CONTINUE;
    }
    if (info.type == SDL_PATHTYPE_DIRECTORY) {
        strcat(path, "/");
        SDL_EnumerateDirectory(path, one, user);
        return SDL_ENUM_CONTINUE;
    }
    if (len < 5 || strcasecmp(name + len - 4, ".ADX") != 0 || (adx = SDL_LoadFile(path, &asize)) == NULL) {
        return SDL_ENUM_CONTINUE;
    }
    sTracks++;
    if (!musicsrc_adx_pcm(adx, asize, &pcm, &frames, &ch, &rate, &ls, &le, &stop)) {
        fprintf(stderr, "FAIL %s: not read as an ADX\n", path + sRoot);
        sBad++;
        SDL_free(adx);
        return SDL_ENUM_CONTINUE;
    }
    snprintf(start, sizeof(start), "LOOPSTART=%ld", ls);
    snprintf(end, sizeof(end), "LOOPEND=%ld", le);
    snprintf(stopat, sizeof(stopat), "STOPAT=%ld", stop);
    flac = flacenc(pcm, frames, ch, rate, tags, 3, &fsize);
    free(pcm);
    if (getenv("MUSIC_DUMP") != NULL && strstr(path, getenv("MUSIC_DUMP")) != NULL) {   /* (a track's FLAC kept) */
        SDL_SaveFile("music-dump.flac", flac, fsize);
    }
    sAdxBytes += (long long)asize;
    sFlacBytes += (long long)fsize;
    if ((at = compare(path + sRoot, adx, asize, flac, fsize, 0, stop + 1000)) >= 0) {
        fprintf(stderr, "FAIL %s: once through, sample %ld of %ld differs\n", path + sRoot, at, stop);
        sBad++;
    } else if ((at = compare(path + sRoot, adx, asize, flac, fsize, 1, stop + 3 * (le - ls))) >= 0) {
        fprintf(stderr, "FAIL %s: looped, sample %ld differs\n", path + sRoot, at);
        sBad++;
    }
    sLead += frames - stop;
    free(flac);
    SDL_free(adx);
    return SDL_ENUM_CONTINUE;
}

int main(int argc, char **argv) {
    char data[1100];
    clock_t t0 = clock();

    full_only("music-check");

    if (argc != 2) {
        fprintf(stderr, "usage: music-check <data folder>\n");
        return 2;
    }
    if (!SDL_GetPathInfo(argv[1], NULL)) {
        printf("music-check: no game data at %s - skipped\n", argv[1]);
        return 77;
    }
    snprintf(data, sizeof(data), "%s/", argv[1]);
    sRoot = strlen(data);
    SDL_EnumerateDirectory(data, one, NULL);
    printf("music-check: %d tracks (%lld KB ADX) as %lld KB FLAC (%ld samples of loops' second starts), %d failures"
           " (%.1f s)\n", sTracks, sAdxBytes >> 10, sFlacBytes >> 10, sLead, sBad, (double)(clock() - t0) / CLOCKS_PER_SEC);
    return sBad != 0 || sTracks == 0;
}
