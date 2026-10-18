/* The messages' conversion checked (docs/library-and-mods.md): every message table of the game
 * data (SUBSCR/MSG_*.BIN, each room's .PAC section 10) written as text as the import writes it,
 * read back as the game reads it, and each message compared - the same bytes, the same count.
 *
 *   messages-check <data folder> */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <SDL3/SDL.h>

#include "../src/data/msgtext.h"
#include "../src/data/pac.h"

static int sTables, sMessages, sBad;
static long sBytes, sText;
static size_t sRoot;

static uint16_t u16(const uint8_t *p) { return (uint16_t)(p[0] | p[1] << 8); }

/* a message's length as the game reads it - transcribed here from the decomp's reader
 * (src/game/text.c TextCursor_Step), apart from msgtext.c's own table, so that a wrong operand
 * count there shows (both sides measured alike would agree with themselves) */
static long game_length(const uint8_t *p, size_t max) {
    size_t i = 0;

    while (i < max && p[i] != 0) {
        switch (p[i]) {
        case 0x0B: case 0x0E: case 0x0F: case 0x19: case 0x11: case 0x15:
            i += 3;
            break;
        case 0x03: case 0x07: case 0x09: case 0x0A: case 0x0C: case 0x10: case 0x08: case 0x13:
        case 0x1A: case 0x1B: case 0x1C: case 0x1D:
            i += 2;
            break;
        default:
            i += 1;
        }
    }
    return i < max ? (long)i : -1;
}

static void check(const char *name, const uint8_t *t, size_t size) {
    char *text;
    uint8_t *back;
    size_t len, bsize, i;
    int n, id;

    if ((text = msgtext_write(t, size, name, &len)) == NULL) {
        fprintf(stderr, "FAIL %s: not written as text\n", name);
        sBad++;
        return;
    }
    for (i = 0; i < len; i++) {
        if ((unsigned char)text[i] < 0x20 && text[i] != '\n') {
            fprintf(stderr, "FAIL %s: the text has a byte %02X\n", name, (unsigned char)text[i]);
            sBad++;
            break;
        }
    }
    back = msgtext_read(text, len, name, &bsize);
    sTables++;
    sBytes += (long)size;
    sText += (long)len;
    if (back == NULL || u16(back) != u16(t)) {
        fprintf(stderr, "FAIL %s: read back %s\n", name, back == NULL ? "not at all" : "with another count");
        sBad++;
        free(text);
        free(back);
        return;
    }
    n = u16(t);
    for (id = 0; id < n; id++) {
        size_t a = u16(t + 2 + id * 2), b = u16(back + 2 + id * 2);
        long la = a < size ? game_length(t + a, size - a) : -1, lb = b < bsize ? game_length(back + b, bsize - b) : -1;

        sMessages++;
        if (la != lb || (la >= 0 && memcmp(t + a, back + b, (size_t)la) != 0)) {
            fprintf(stderr, "FAIL %s: message %d differs read back\n", name, id);
            sBad++;
            break;
        }
    }
    free(text);
    free(back);
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
        return SDL_ENUM_CONTINUE;
    }
    if (len > 4 && strcasecmp(name + len - 4, ".PAC") == 0 && (data = SDL_LoadFile(path, &size)) != NULL) {
        Pac pac = {data, size};
        size_t tsize;
        const uint8_t *t = pac_section(&pac, PAC_TEXT, &tsize);

        if (t != NULL) {
            check(path + sRoot, t, tsize);
        }
        SDL_free(data);
    } else if (strncasecmp(name, "MSG_", 4) == 0 && len > 4 && strcasecmp(name + len - 4, ".BIN") == 0 &&
               (data = SDL_LoadFile(path, &size)) != NULL) {
        check(path + sRoot, data, size);
        SDL_free(data);
    }
    return SDL_ENUM_CONTINUE;
}

int main(int argc, char **argv) {
    char data[1100];
    static const char kBroken[] = "#0\nfine\n#1\nan {unknown} word\n";
    static const char kLetters[] = "#0\nrisqu\xC3\xA9 DUALSHOCK\xC2\xAE" "2 {cross}\n";   /* é 0xA9, ® 0x7B, cross 0xC0+3 */
    static const uint8_t kWant[] = {'r', 'i', 's', 'q', 'u', 0xA9, ' ', 'D', 'U', 'A', 'L', 'S', 'H', 'O', 'C', 'K', 0x7B, '2', ' ', 0xC3, 0};
    size_t size;
    uint8_t *t;

    if (argc != 2) {
        fprintf(stderr, "usage: messages-check <data folder>\n");
        return 2;
    }
    if ((t = msgtext_read(kBroken, sizeof(kBroken) - 1, "(a broken one)", &size)) != NULL) {   /* (said, not read) */
        fprintf(stderr, "FAIL: a table with an unknown {word} was read\n");
        sBad++;
        free(t);
    }
    t = msgtext_read(kLetters, sizeof(kLetters) - 1, "(letters)", &size);   /* (the font's letters by name) */
    if (t == NULL || size != 4 + sizeof(kWant) || memcmp(t + 4, kWant, sizeof(kWant)) != 0) {
        fprintf(stderr, "FAIL: \"risqu\xC3\xA9 DUALSHOCK\xC2\xAE" "2 {cross}\" isn't read as the game's bytes\n");
        sBad++;
    }
    free(t);
    if (!SDL_GetPathInfo(argv[1], NULL)) {
        printf("messages-check: no game data at %s - skipped\n", argv[1]);
        return 77;
    }
    snprintf(data, sizeof(data), "%s/", argv[1]);
    sRoot = strlen(data);
    SDL_EnumerateDirectory(data, one, NULL);
    printf("messages-check: %d tables (%ld KB) as %ld KB of text, %d messages compared, %d failures\n", sTables,
           sBytes >> 10, sText >> 10, sMessages, sBad);
    return sBad != 0 || sTables == 0;
}
