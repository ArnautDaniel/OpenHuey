/* The game's data from SLUS_210.75. A release build of hg leaves the data the PS2 executable
 * holds (strings, tables, numbers) out (tools/native_gen.py --strip): at startup it is copied
 * from the player's own SLUS_210.75, part by part into the same layout, keeping what the build
 * put there itself (pointers into the native program, the space of labels written in C).
 *
 * HG_SLUS_CHECK=1 (a build with the data in): compare the build's data with SLUS_210.75's and
 * report the differences, instead of copying. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>

#include "disc.h"

extern const uint32_t *const hg_slus_tables[];   /* (generated slus.c) */
extern const uint32_t hg_slus_size;
extern const uint64_t hg_slus_fnv;
extern const int hg_slus_stripped;

static uint8_t *read_exe(char *err, size_t errLen) {
    HgFile *f = hg_disc_exe();
    uint8_t *buf;
    uint64_t h = 0xCBF29CE484222325ull;
    uint32_t i;

    if (f == NULL) {
        snprintf(err, errLen, "the game's program SLUS_210.75 isn't on the disc (or next to the data)");
        return NULL;
    }
    if ((uint32_t)hg_file_size(f) != hg_slus_size || (buf = malloc(hg_slus_size)) == NULL ||
        hg_file_read(f, buf, hg_slus_size) != hg_slus_size) {
        snprintf(err, errLen, "%s isn't the SLUS_210.75 of Haunting Ground (USA) (size %ld)", hg_disc_exe_path(),
                 hg_file_size(f));
        hg_file_close(f);
        return NULL;
    }
    hg_file_close(f);
    for (i = 0; i < hg_slus_size; i++) {
        h = (h ^ buf[i]) * 0x100000001B3ull;
    }
    if (h != hg_slus_fnv) {
        snprintf(err, errLen, "%s isn't the SLUS_210.75 of Haunting Ground (USA): a different version of the game?",
                 hg_disc_exe_path());
        free(buf);
        return NULL;
    }
    return buf;
}

/* one table (native_gen.py): start, offset, length, parts to keep, then (offset, size) each */
static long sCompared, sKept;

static long apply(const uint32_t *t, const uint8_t *exe, int check) {
    uint8_t *dst = (uint8_t *)(uintptr_t)t[0];
    const uint8_t *src = exe + t[1];
    uint32_t len = t[2], n = t[3], at = 0, k;
    long diff = 0;

    for (k = 0; k <= n; k++) {
        uint32_t end = k < n ? t[4 + 2 * k] : len;
        uint32_t skip = k < n ? t[5 + 2 * k] : 0;

        sKept += skip;
        if (end > at) {
            sCompared += end - at;
            if (check) {
                uint32_t i;

                for (i = at; i < end; i++) {
                    if (dst[i] != src[i] && diff++ < 20) {
                        fprintf(stderr, "slus: data +0x%X (SLUS 0x%X) is %02X, the disc has %02X\n", i, t[1] + i,
                                dst[i], src[i]);
                    }
                }
            } else {
                memcpy(dst + at, src + at, end - at);
            }
        }
        at = end + skip > at ? end + skip : at;
    }
    return diff;
}

int hg_slus_load(void) {
    char err[1200];
    int check = getenv("HG_SLUS_CHECK") != NULL;
    uint8_t *exe;
    long diff = 0;
    int i;

    if (!hg_slus_stripped && !check) {
        return 1;   /* (the data is in the build) */
    }
    if ((exe = read_exe(err, sizeof(err))) == NULL) {
        fprintf(stderr, "hg: %s\n", err);
        if (getenv("HG_HEADLESS") == NULL) {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Haunting Ground", err, NULL);
        }
        return !hg_slus_stripped;
    }
    for (i = 0; hg_slus_tables[i] != NULL; i++) {
        diff += apply(hg_slus_tables[i], exe, check && !hg_slus_stripped);
    }
    free(exe);
    if (check && !hg_slus_stripped) {
        fprintf(stderr, "slus: %ld of %ld bytes of the build's data differ from SLUS_210.75 (%ld its own)\n", diff,
                sCompared, sKept);
    }
    return 1;
}
