/* The movies' conversion checked (docs/library-and-mods.md): every SFD of the game data split
 * as the import splits it (X.mpg: the stream less its sound; X.flac: the sound), and both read as
 * the player reads them - every video packet's payload the same bytes, in the same order; every
 * sound sample the same (the SFD's ADX decoded as platform/movie.c's adx_pump does, transcribed
 * here, against the FLAC as data/musicsrc.c plays it).
 *
 *   movie-check <data folder> */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <SDL3/SDL.h>

#include "../src/data/flacenc.h"
#include "../src/data/musicsrc.h"
#include "../src/data/sfd.h"

static int sMovies, sBad, sSilent;
static long long sSfd, sMpg, sFlac, sPackets, sSamples;
static size_t sRoot;

typedef struct Mem {
    const uint8_t *p;
    size_t n, at;
} Mem;

static size_t mem_read(void *user, uint8_t *buf, size_t n) {
    Mem *m = user;

    n = n < m->n - m->at ? n : m->n - m->at;
    memcpy(buf, m->p + m->at, n);
    m->at += n;
    return n;
}

static unsigned be16(const uint8_t *b) { return (unsigned)(b[0] << 8 | b[1]); }
static unsigned be32(const uint8_t *b) { return (unsigned)b[0] << 24 | (unsigned)b[1] << 16 | (unsigned)b[2] << 8 | b[3]; }

/* the movie player's ADX decoding (adx_head, adx_pump): the whole stream, interleaved */
static long movie_adx(const uint8_t *a, size_t n, int16_t *out, int *ch_out, int *rate_out) {
    unsigned off, cutoff;
    double x, y, z;
    int coef1, coef2, ch, hist[2][2] = {{0}}, c, i;
    size_t pos;
    long frames = 0;

    if (n < 0x14 || be16(a) != 0x8000 || a[4] != 3 || a[5] != 18 || a[6] != 4 || a[7] < 1 || a[7] > 2) {
        return -1;
    }
    off = be16(a + 2) + 4;
    ch = a[7];
    *ch_out = ch;
    *rate_out = (int)be32(a + 8);
    cutoff = be16(a + 0x10);
    x = sqrt(2.0) - cos(2.0 * M_PI * cutoff / *rate_out);
    y = sqrt(2.0) - 1.0;
    z = (x - sqrt((x + y) * (x - y))) / y;
    coef1 = (int)floor(z * 8192.0);
    coef2 = (int)floor(z * z * -4096.0);
    for (pos = off; pos + 18u * (size_t)ch <= n; pos += 18u * (size_t)ch) {
        int16_t pcm[2][32];

        for (c = 0; c < ch; c++) {
            const uint8_t *b = a + pos + c * 18;
            int scale = (int)be16(b), h1 = hist[c][0], h2 = hist[c][1];

            if (scale & 0x8000) {
                return frames;
            }
            for (i = 0; i < 32; i++) {
                int v = (b[2 + i / 2] >> (i & 1 ? 0 : 4)) & 0xF;

                if (v & 8) {
                    v -= 16;
                }
                v = v * scale + ((coef1 * h1 + coef2 * h2) >> 12);
                v = v > 32767 ? 32767 : v < -32768 ? -32768 : v;
                pcm[c][i] = (int16_t)v;
                h2 = h1;
                h1 = v;
            }
            hist[c][0] = h1;
            hist[c][1] = h2;
        }
        for (i = 0; i < 32; i++) {
            out[(frames + i) * 2] = pcm[0][i];
            out[(frames + i) * 2 + 1] = pcm[ch > 1][i];
        }
        frames += 32;
    }
    return frames;
}

static void check(const char *name, const uint8_t *sfd, size_t size) {
    uint8_t *mpg, *adx, *flac;
    size_t ms, as, fs;
    SfdReader *a = calloc(1, sizeof(SfdReader)), *b = calloc(1, sizeof(SfdReader));
    Mem ma = {sfd, size, 0}, mb;
    int ida, idb, la, lb, ch, rate;
    const uint8_t *da, *db;
    int16_t *want, *pcm;
    long frames, stop, ls, le, i, got;
    MusicSrc *src;

    sMovies++;
    sSfd += (long long)size;
    if (!sfd_split(sfd, size, &mpg, &ms, &adx, &as)) {
        fprintf(stderr, "FAIL %s: not split\n", name);
        sBad++;
        return;
    }
    sMpg += (long long)ms;
    mb = (Mem){mpg, ms, 0};
    a->read = b->read = mem_read;
    a->user = &ma;
    b->user = &mb;
    for (;;) {   /* the video: the SFD's packets and the .mpg's */
        int ra, rb;

        do {
            ra = sfd_next(a, &ida, &da, &la);
        } while (ra && ida != 0xE0);
        rb = sfd_next(b, &idb, &db, &lb);
        if (rb && idb != 0xE0) {
            fprintf(stderr, "FAIL %s: the .mpg has sound\n", name);
            sBad++;
            break;
        }
        if (ra != rb || (ra && (la != lb || memcmp(da, db, (size_t)la) != 0))) {
            fprintf(stderr, "FAIL %s: video packet %lld differs\n", name, sPackets);
            sBad++;
            break;
        }
        if (!ra) {
            break;
        }
        sPackets++;
    }
    /* the sound: as the import makes the FLAC, against the player's own ADX decoding */
    want = malloc(sizeof(int16_t) * 2 * (as / 18 * 32 + 64));
    frames = movie_adx(adx, as, want, &ch, &rate);
    if (frames < 0 || !sfd_adx_playable(adx, as)) {   /* (the player plays it silent: no FLAC) */
        if ((frames < 0) != !sfd_adx_playable(adx, as)) {
            fprintf(stderr, "FAIL %s: the player and the import disagree on its sound\n", name);
            sBad++;
        }
        sSilent++;
    } else if (!musicsrc_adx_pcm(adx, as, &pcm, &got, &ch, &rate, &ls, &le, &stop)) {
        fprintf(stderr, "FAIL %s: its sound isn't an ADX\n", name);
        sBad++;
    } else {
        flac = flacenc(pcm, stop, ch, rate, NULL, 0, &fs);
        sFlac += (long long)fs;
        src = musicsrc_open(flac, fs, 0);
        for (i = 0, got = 0; src != NULL;) {
            int16_t blk[2][MUSICSRC_FRAME];
            int n = musicsrc_next(src, blk), k;

            if (n <= 0) {
                break;
            }
            for (k = 0; k < n; k++, i++) {
                if (i >= frames || blk[0][k] != want[i * 2] || blk[ch > 1][k] != want[i * 2 + 1]) {
                    got = -1;
                    break;
                }
            }
            if (got < 0) {
                break;
            }
        }
        if (src == NULL || got < 0 || i != frames) {
            fprintf(stderr, "FAIL %s: sound sample %ld differs (of %ld; the FLAC has %ld)\n", name, i, frames, stop);
            sBad++;
        }
        sSamples += frames;
        musicsrc_free(src);
        free(pcm);
    }
    free(want);
    free(mpg);
    free(adx);
    free(a);
    free(b);
}

static SDL_EnumerationResult one(void *user, const char *dir, const char *name) {
    char path[1300];
    SDL_PathInfo info;
    size_t len = strlen(name), size;
    uint8_t *data;

    snprintf(path, sizeof(path), "%s%s", dir, name);
    if (!SDL_GetPathInfo(path, &info)) {
        return SDL_ENUM_CONTINUE;
    }
    if (info.type == SDL_PATHTYPE_DIRECTORY) {
        strcat(path, "/");
        SDL_EnumerateDirectory(path, one, user);
    } else if (len > 4 && strcasecmp(name + len - 4, ".SFD") == 0 && (data = SDL_LoadFile(path, &size)) != NULL) {
        check(path + sRoot, data, size);
        SDL_free(data);
    }
    return SDL_ENUM_CONTINUE;
}

int main(int argc, char **argv) {
    char data[1100];

    if (argc != 2) {
        fprintf(stderr, "usage: movie-check <data folder>\n");
        return 2;
    }
    if (!SDL_GetPathInfo(argv[1], NULL)) {
        printf("movie-check: no game data at %s - skipped\n", argv[1]);
        return 77;
    }
    snprintf(data, sizeof(data), "%s/", argv[1]);
    sRoot = strlen(data);
    SDL_EnumerateDirectory(data, one, NULL);
    printf("movie-check: %d movies (%d silent) (%lld MB) as %lld MB of .mpg and %lld MB of FLAC - %lld video packets, %lld sound "
           "samples compared, %d failures\n", sMovies, sSilent, sSfd >> 20, sMpg >> 20, sFlac >> 20, sPackets, sSamples, sBad);
    return sBad != 0 || sMovies == 0;
}
