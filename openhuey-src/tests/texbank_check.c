/* The textures' conversion checked (docs/library-and-mods.md): every .TEX bank of the game data
 * converted to PNGs as the import does, read back as the game reads the library, and each texture
 * in each palette compared with the bank's own decoding - they must be the same pixels.
 *
 *   texbank-check <data folder> <scratch folder (made, emptied after)> */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <SDL3/SDL.h>

#include "../src/data/pac.h"
#include "../src/data/tex.h"
#include "../src/data/texbank.h"
#include "../src/platform/disc.h"
#include "full_only.h"

typedef struct Bank {
    char path[512];   /* in the data folder: "SYSTEM/TITLE.TEX" (a room's: "ST_000/ST_003.room/textures.TEX") */
    char pac[512];    /* (a room's: its PAC, and the section) */
    int section;
    uint8_t *raw;
    size_t size;
} Bank;

static Bank *sBanks;
static int sN, sCap;
static size_t sRoot;   /* the data folder's length (paths are what follows it) */

static SDL_EnumerationResult find_one(void *user, const char *dir, const char *name) {
    char path[1300];
    SDL_PathInfo info;
    size_t len = strlen(name);

    snprintf(path, sizeof(path), "%s%s", dir, name);
    if (!SDL_GetPathInfo(path, &info)) {
        return SDL_ENUM_CONTINUE;
    }
    if (info.type == SDL_PATHTYPE_DIRECTORY) {
        strcat(path, "/");
        SDL_EnumerateDirectory(path, find_one, user);
    } else if (len > 4 && strcasecmp(name + len - 4, ".TEX") == 0) {
        if (sN + 2 >= sCap) {
            sCap = sCap ? sCap * 2 : 256;
            sBanks = realloc(sBanks, sizeof(*sBanks) * (size_t)sCap);
        }
        snprintf(sBanks[sN].path, sizeof(sBanks[sN].path), "%s", path + sRoot);
        sBanks[sN].pac[0] = 0;
        sBanks[sN].raw = NULL;
        sN++;
    } else if (len > 4 && strcasecmp(name + len - 4, ".PAC") == 0) {   /* (a room's two banks) */
        int k;

        if (sN + 2 >= sCap) {
            sCap = sCap ? sCap * 2 : 256;
            sBanks = realloc(sBanks, sizeof(*sBanks) * (size_t)sCap);
        }
        for (k = 0; k < 2; k++) {
            snprintf(sBanks[sN].path, sizeof(sBanks[sN].path), "%.*s.room/%s.TEX", (int)(strlen(path + sRoot) - 4),
                     path + sRoot, k == 0 ? "textures" : "effects");
            snprintf(sBanks[sN].pac, sizeof(sBanks[sN].pac), "%s", path + sRoot);
            sBanks[sN].section = k == 0 ? PAC_TEXTURES : PAC_TEXTURES2;
            sBanks[sN].raw = NULL;
            sN++;
        }
    }
    return SDL_ENUM_CONTINUE;
}

static SDL_EnumerationResult remove_one(void *user, const char *dir, const char *name) {
    char path[1300];
    SDL_PathInfo info;

    snprintf(path, sizeof(path), "%s%s", dir, name);
    if (SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_DIRECTORY) {
        strcat(path, "/");
        SDL_EnumerateDirectory(path, remove_one, NULL);
    }
    SDL_RemovePath(path);
    return SDL_ENUM_CONTINUE;
}

static long pngs, bytes;   /* what the library holds instead */

static void wrote(void *user, const char *name, long size) {
    pngs += strstr(name, ".png") != NULL;
    bytes += size;
}

static int make_parents(const char *path) {
    char dir[1300], *slash;

    snprintf(dir, sizeof(dir), "%s", path);
    if ((slash = strrchr(dir, '/')) != NULL) {
        *slash = 0;
        return SDL_CreateDirectory(dir);
    }
    return 1;
}

int main(int argc, char **argv) {
    char data[1100], scratch[1100], dir[1700];
    long compared = 0, raw_bytes = 0;
    int i, bad = 0, converted = 0;

    full_only("texbank-check");

    if (argc != 3) {
        fprintf(stderr, "usage: texbank-check <data folder> <scratch folder>\n");
        return 2;
    }
    snprintf(data, sizeof(data), "%s/", argv[1]);
    snprintf(scratch, sizeof(scratch), "%s/", argv[2]);
    if (!SDL_GetPathInfo(argv[1], NULL)) {
        printf("texbank-check: no game data at %s - skipped\n", argv[1]);
        return 77;
    }
    sRoot = strlen(data);
    SDL_EnumerateDirectory(data, find_one, NULL);
    SDL_EnumerateDirectory(scratch, remove_one, NULL);
    SDL_CreateDirectory(scratch);

    if (!disc_open(argv[1])) {   /* the banks as the game reads them, converted as the import does */
        fprintf(stderr, "texbank-check: %s\n", disc_error());
        return 1;
    }
    for (i = 0; i < sN; i++) {
        Bank *b = &sBanks[i];
        size_t len = strlen(b->path) - 4;

        if (b->pac[0] != 0) {   /* (a room's: its section, copied out) */
            Pac pac;
            const uint8_t *sec;

            pac.data = disc_read_all(b->pac, &pac.size);
            b->raw = NULL;
            if (pac.data != NULL && (sec = pac_section(&pac, b->section, &b->size)) != NULL) {
                b->raw = malloc(b->size);
                memcpy(b->raw, sec, b->size);
            }
            free(pac.data);
        } else {
            b->raw = disc_read_all(b->path, &b->size);
        }
        if (b->raw == NULL || tex_count(b->raw, b->size) == 0) {
            continue;   /* (not a bank: the import copies it as it is) */
        }
        raw_bytes += (long)b->size;
        snprintf(dir, sizeof(dir), "%s%.*s.texbank/", scratch, (int)len, b->path);
        if (!make_parents(dir) || !texbank_convert(b->raw, b->size, dir, wrote, NULL)) {
            fprintf(stderr, "FAIL %s: not converted\n", b->path);
            bad++;
            continue;
        }
        converted++;
    }
    snprintf(dir, sizeof(dir), "%sSYSTEM", scratch);
    SDL_CreateDirectory(dir);   /* (what the disc layer wants of a data folder) */
    if (!disc_open(argv[2])) {   /* the converted form, read back through files_read */
        fprintf(stderr, "texbank-check: %s\n", disc_error());
        return 1;
    }
    for (i = 0; i < sN; i++) {
        Bank *b = &sBanks[i];
        TexBank *tb;
        int n, t;

        if (b->raw == NULL || (n = tex_count(b->raw, b->size)) == 0) {
            continue;
        }
        tb = texbank_load(b->path);
        if (tb == NULL || texbank_count(tb) != n) {
            fprintf(stderr, "FAIL %s: read back with %d textures, the bank has %d\n", b->path, texbank_count(tb), n);
            bad++;
            texbank_free(tb);
            continue;
        }
        for (t = 0; t < n; t++) {
            const TexEntry *e = tex_entry(b->raw, t);
            int w, h, psm, csa, palettes = e->psm == TEX_T4 ? 32 : 1;
            size_t px = (size_t)e->w * e->h * 4;
            uint8_t *want, *got;

            if (!texbank_info(tb, t, &w, &h, &psm)) {
                if (e->w != 0 && e->h != 0) {
                    fprintf(stderr, "FAIL %s %d: no size read back\n", b->path, t);
                    bad++;
                }
                continue;
            }
            if (w != e->w || h != e->h || psm != e->psm) {
                fprintf(stderr, "FAIL %s %d: %dx%d psm %d read back, the bank's %dx%d psm %d\n", b->path, t, w, h, psm,
                        e->w, e->h, e->psm);
                bad++;
                continue;
            }
            want = malloc(px);
            got = malloc(px);
            for (csa = -1; csa < palettes; csa++) {
                int a = tex_decode_csa(b->raw, b->size, t, csa, want), c = texbank_rgba(tb, t, csa, got);

                if (a != c || (a && memcmp(want, got, px) != 0)) {
                    fprintf(stderr, "FAIL %s %d palette %d: the pixels differ\n", b->path, t, csa);
                    bad++;
                    break;
                }
                compared++;
            }
            free(want);
            free(got);
        }
        texbank_free(tb);
    }
    printf("texbank-check: %d banks (%ld KB) converted to %ld PNGs (%ld KB), %ld texture palettes compared, %d failures\n",
           converted, raw_bytes >> 10, pngs, bytes >> 10, compared, bad);
    SDL_EnumerateDirectory(scratch, remove_one, NULL);
    SDL_RemovePath(argv[2]);
    for (i = 0; i < sN; i++) {
        free(sBanks[i].raw);
    }
    free(sBanks);
    return bad != 0;
}
