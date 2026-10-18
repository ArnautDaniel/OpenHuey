/* The rooms' conversion checked (docs/library-and-mods.md): every room's PAC as the library holds
 * it - less the sections converted (the texture banks 9 and 11, the text 10) - has every other
 * section the same bytes, the same size, as the disc's; the dropped ones none. (The converted
 * sections themselves: texbank-check, messages-check.)
 *
 *   room-check <data folder> */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <SDL3/SDL.h>

#include "../src/data/pac.h"

static int sRooms, sBad;
static long long sBefore, sAfter;

static void check(const char *name, uint8_t *data, size_t size) {
    Pac a = {data, size}, b;
    unsigned drop = 1u << PAC_TEXTURES | 1u << PAC_TEXT | 1u << PAC_TEXTURES2;
    int i;

    b.data = pac_strip(&a, drop, &b.size);
    sRooms++;
    if (b.data == NULL) {
        fprintf(stderr, "FAIL %s: not stripped\n", name);
        sBad++;
        return;
    }
    sBefore += (long long)size;
    sAfter += (long long)b.size;
    for (i = 0; i < PAC_SECTIONS; i++) {
        size_t na = 0, nb = 0;
        const uint8_t *sa = pac_section(&a, i, &na), *sb = pac_section(&b, i, &nb);

        if (drop >> i & 1) {
            if (sb != NULL) {
                fprintf(stderr, "FAIL %s: section %d still there\n", name, i);
                sBad++;
            }
        } else if ((sa == NULL) != (sb == NULL) || na != nb || (sa != NULL && memcmp(sa, sb, na) != 0)) {
            fprintf(stderr, "FAIL %s: section %d differs (%zu bytes, %zu before)\n", name, i, nb, na);
            sBad++;
        } else if (sb != NULL && ((size_t)(sb - b.data) & 15) != ((size_t)(sa - a.data) & 15)) {
            fprintf(stderr, "FAIL %s: section %d moved off its alignment\n", name, i);
            sBad++;
        }
    }
    free(b.data);
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
    } else if (len > 4 && strcasecmp(name + len - 4, ".PAC") == 0 && (data = SDL_LoadFile(path, &size)) != NULL) {
        check(name, data, size);
        SDL_free(data);
    }
    return SDL_ENUM_CONTINUE;
}

int main(int argc, char **argv) {
    char data[1100];

    if (argc != 2) {
        fprintf(stderr, "usage: room-check <data folder>\n");
        return 2;
    }
    if (!SDL_GetPathInfo(argv[1], NULL)) {
        printf("room-check: no game data at %s - skipped\n", argv[1]);
        return 77;
    }
    snprintf(data, sizeof(data), "%s/", argv[1]);
    SDL_EnumerateDirectory(data, one, NULL);
    printf("room-check: %d rooms (%lld MB) as %lld MB of PAC less what is converted, %d failures\n", sRooms,
           sBefore >> 20, sAfter >> 20, sBad);
    return sBad != 0 || sRooms == 0;
}
