/* Mods' files and manifests checked (docs/library-and-mods.md): the load order (order.txt,
 * after:), what turns a mod off (off:, requires:), the files layer (the later mod's file over
 * the earlier's over the data) and a mod's PNG over a texture - of a bank as it is and of one
 * converted.
 *
 *   mods-check <data folder> <scratch folder (made, emptied after)> */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>

#include "../src/core/files.h"
#include "../src/data/tex.h"
#include "../src/data/texbank.h"
#include "../src/game/mods.h"
#include "../src/third_party/stb_image_write.h"

#define BANK "ITEM00/ITEM_000"   /* a 16-colour texture with more than one palette */

static int sBad, sChecks;
static char sScratch[1100];

#define CHECK(cond, ...)                         \
    do {                                         \
        sChecks++;                               \
        if (!(cond)) {                           \
            fprintf(stderr, "FAIL: " __VA_ARGS__); \
            fprintf(stderr, "\n");               \
            sBad++;                              \
        }                                        \
    } while (0)

static void put(const char *rel, const char *text) {   /* a file in the scratch folder */
    char path[1300], dir[1300], *slash;
    FILE *f;

    snprintf(path, sizeof(path), "%s%s", sScratch, rel);
    snprintf(dir, sizeof(dir), "%s", path);
    if ((slash = strrchr(dir, '/')) != NULL) {
        *slash = 0;
        SDL_CreateDirectory(dir);
    }
    if ((f = fopen(path, "wb")) != NULL) {
        fputs(text, f);
        fclose(f);
    }
}

static void to_file(void *f, void *data, int n) { fwrite(data, 1, (size_t)n, f); }

static void put_png(const char *rel, int w, int h, uint32_t rgba) {
    char path[1300], dir[1300], *slash;
    uint32_t *px = malloc((size_t)w * h * 4);
    int i;

    for (i = 0; i < w * h; i++) {
        px[i] = rgba;
    }
    snprintf(path, sizeof(path), "%s%s", sScratch, rel);
    snprintf(dir, sizeof(dir), "%s", path);
    if ((slash = strrchr(dir, '/')) != NULL) {
        *slash = 0;
        SDL_CreateDirectory(dir);
    }
    {
        FILE *f = fopen(path, "wb");

        if (f != NULL) {
            stbi_write_png_to_func(to_file, f, w, h, 4, px, w * 4);
            fclose(f);
        }
    }
    free(px);
}

unsigned char *stbi_zlib_compress(unsigned char *data, int data_len, int *out_len, int quality);   /* (stb.c's, not in its header) */

/* ---- a zip written: each file stored, or deflated (stb_image_write's zlib, its header and
 * checksum taken off) ---- */
static uint32_t crc32_of(const uint8_t *p, size_t n) {
    uint32_t c = 0xFFFFFFFFu;
    size_t i;
    int k;

    for (i = 0; i < n; i++) {
        c ^= p[i];
        for (k = 0; k < 8; k++) {
            c = (c >> 1) ^ (0xEDB88320u & (0u - (c & 1)));
        }
    }
    return ~c;
}

static void le(uint8_t *p, uint32_t v, int n) {
    int i;

    for (i = 0; i < n; i++) {
        p[i] = (uint8_t)(v >> (8 * i));
    }
}

static void put_zip(const char *rel, const char *const *names, const char *const *texts, int n, int deflate) {
    char path[1300];
    uint8_t cd[4096], h[46];
    uint32_t at = 0, cdlen = 0;
    FILE *f;
    int i;

    snprintf(path, sizeof(path), "%s%s", sScratch, rel);
    if ((f = fopen(path, "wb")) == NULL) {
        return;
    }
    for (i = 0; i < n; i++) {
        uint32_t len = (uint32_t)strlen(texts[i]), nlen = (uint32_t)strlen(names[i]), plen = len;
        const uint8_t *data = (const uint8_t *)texts[i];
        uint8_t *z = NULL;

        if (deflate) {
            int zl;

            z = stbi_zlib_compress((uint8_t *)texts[i], (int)len, &zl, 8);
            data = z + 2;
            plen = (uint32_t)zl - 6;
        }
        memset(h, 0, sizeof(h));
        le(h, 0x04034b50, 4);
        le(h + 4, 20, 2);
        le(h + 8, deflate ? 8 : 0, 2);
        le(h + 14, crc32_of((const uint8_t *)texts[i], len), 4);
        le(h + 18, plen, 4);
        le(h + 22, len, 4);
        le(h + 26, nlen, 2);
        fwrite(h, 1, 30, f);
        fwrite(names[i], 1, nlen, f);
        fwrite(data, 1, plen, f);
        memmove(h + 16, h + 14, 14);   /* (the central record: the same fields two bytes on) */
        le(h, 0x02014b50, 4);
        le(h + 4, 20, 2);
        le(h + 6, 20, 2);
        le(h + 10, deflate ? 8 : 0, 2);
        le(h + 12, 0, 4);
        le(h + 30, 0, 16);
        le(h + 42, at, 4);
        memcpy(cd + cdlen, h, 46);
        memcpy(cd + cdlen + 46, names[i], nlen);
        cdlen += 46 + nlen;
        at += 30 + nlen + plen;
        free(z);
    }
    fwrite(cd, 1, cdlen, f);
    memset(h, 0, 22);
    le(h, 0x06054b50, 4);
    le(h + 8, (uint32_t)n, 2);
    le(h + 10, (uint32_t)n, 2);
    le(h + 12, cdlen, 4);
    le(h + 16, at, 4);
    fwrite(h, 1, 22, f);
    fclose(f);
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

static const char *order(void) {   /* "name name ..." of those on, "-name" off */
    static char s[512];
    int i, on;
    const char *why;

    s[0] = 0;
    for (i = 0; i < mods_count(); i++) {
        const char *n = mods_name(i, &on, &why);

        snprintf(s + strlen(s), sizeof(s) - strlen(s), "%s%s%s", i ? " " : "", on ? "" : "-", n);
    }
    return s;
}

static int all_of(const uint8_t *rgba, int w, int h, uint32_t c) {
    int i;

    for (i = 0; i < w * h; i++) {
        if (memcmp(rgba + i * 4, &c, 4) != 0) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char **argv) {
    char mods[1300], lib[1300];
    size_t size;
    uint8_t *text, *raw, *rgba, *want;
    TexBank *b;
    int w, h, psm, csa;
    const uint32_t kGreen = 0xFF20C040u;   /* (bytes R G B A: 40 C0 20 FF) */

    if (argc != 3) {
        fprintf(stderr, "usage: mods-check <data folder> <scratch folder>\n");
        return 2;
    }
    if (!SDL_GetPathInfo(argv[1], NULL)) {
        printf("mods-check: no game data at %s - skipped\n", argv[1]);
        return 77;
    }
    snprintf(sScratch, sizeof(sScratch), "%s/", argv[2]);
    SDL_EnumerateDirectory(sScratch, remove_one, NULL);
    SDL_CreateDirectory(sScratch);
    snprintf(mods, sizeof(mods), "%smods/", sScratch);

    /* ---- the order and what turns a mod off ---- */
    put("mods/alpha/mod.fs", "mod: alpha  version: 1 0\nafter: beta   \\ (after beta)\n");
    put("mods/beta/mod.fs", "\\ a comment line\nmod: beta\nversion: 1 2\nrequires: openhuey 1 0\n");   /* (the engine by its name) */
    put("mods/gamma/mod.fs", "mod: gamma\nrequires: hga 9 0\n");
    put("mods/delta/mod.fs", "mod: delta\nrequires: gamma 1 0\n");
    put("mods/eps/mod.fs", "mod: eps\nrequires: beta 1 3\n");
    put("mods/zeta/mod.fs", "mod: zeta\nrequires: beta 1 1\n");
    put("mods/eta/mod.fs", "mod: eta\n");
    put("mods/not-a-mod/readme.txt", "no manifest\n");
    put("mods/order.txt", "on: zeta\noff: eta\n");
    mods_find(mods, NULL, 0);
    CHECK(strcmp(order(), "zeta -eta beta alpha -delta -eps -gamma") == 0, "the order: %s", order());
    {
        int on;
        const char *why;

        mods_name(4, &on, &why);
        CHECK(strstr(why, "gamma, off") != NULL, "delta off because gamma is: %s", why);
        mods_name(6, &on, &why);
        CHECK(strstr(why, "OpenHuey's mod API 9.0") != NULL, "gamma off for the API, by the old name hga: %s", why);
    }

    /* ---- the files: the later mod's over the earlier's over the data ---- */
    if (!disc_open(argv[1])) {
        fprintf(stderr, "mods-check: %s\n", disc_error());
        return 1;
    }
    put("mods/beta/library/data/SYSTEM/MODDED.TXT", "beta");
    put("mods/alpha/library/data/SYSTEM/MODDED.TXT", "alpha");
    put("mods/beta/library/data/SYSTEM/BETA.TXT", "beta's own");
    put("mods/gamma/library/data/SYSTEM/GAMMA.TXT", "an off mod's");
    mods_find(mods, NULL, 0);
    text = files_read("SYSTEM/MODDED.TXT", &size);
    CHECK(text != NULL && strcmp((char *)text, "alpha") == 0, "the later mod's file: %s", text ? (char *)text : "none");
    free(text);
    text = files_read("SYSTEM/beta.txt", &size);   /* (the file's name: case doesn't matter) */
    CHECK(text != NULL && strcmp((char *)text, "beta's own") == 0, "a mod's file of its own");
    free(text);
    CHECK(!files_exist("SYSTEM/GAMMA.TXT"), "an off mod's file is not seen");
    CHECK(files_exist(BANK ".TEX"), "the data's own file is still there");

    /* ---- zipped mods: mod.fs at the top (stored), or in a folder (deflated) ---- */
    {
        static const char *const kNames[] = {"mod.fs", "library/data/SYSTEM/MODDED.TXT", "library/HELLO.TXT"};
        static const char *const kTexts[] = {"mod: kappa\nafter: alpha\n", "kappa", "beside the data"};
        static const char *const kNames2[] = {"lambda/mod.fs", "lambda/library/data/SYSTEM/LAMBDA.TXT"};
        static const char *const kTexts2[] = {"mod: lambda\nversion: 2 1\n", "lambda's own, deflated"};
        int i, on, found = 0;
        const char *why;

        put_zip("mods/kappa.zip", kNames, kTexts, 3, 0);
        put_zip("mods/zipped-lambda.zip", kNames2, kTexts2, 2, 1);
        put("mods/damaged.zip", "PK this isn't a zip");
        mods_find(mods, NULL, 0);
        for (i = 0; i < mods_count(); i++) {
            const char *name = mods_name(i, &on, &why);

            found |= (strcmp(name, "kappa") == 0 && on) | (strcmp(name, "lambda") == 0 && on) << 1 |
                     (strcmp(name, "damaged") == 0) << 2;
        }
        CHECK(found == 3, "the zips found: kappa and lambda on, the damaged one not a mod (%d)", found);
        text = files_read("SYSTEM/MODDED.TXT", &size);
        CHECK(text != NULL && strcmp((char *)text, "kappa") == 0, "a zip's file over the folder mod's: %s",
              text ? (char *)text : "none");
        free(text);
        text = files_read("system/lambda.txt", &size);   /* (in a zip, case doesn't matter at all) */
        CHECK(text != NULL && strcmp((char *)text, "lambda's own, deflated") == 0, "a deflated file in a zip's folder");
        free(text);
        text = files_read("../HELLO.TXT", &size);
        CHECK(text != NULL && strcmp((char *)text, "beside the data") == 0, "a zip's file beside the data");
        free(text);
        SDL_RemovePath(strcat(strcpy((char *)lib, mods), "kappa.zip"));
        SDL_RemovePath(strcat(strcpy((char *)lib, mods), "zipped-lambda.zip"));
        SDL_RemovePath(strcat(strcpy((char *)lib, mods), "damaged.zip"));
    }

    /* ---- a texture: a bank as it is ---- */
    raw = files_read(BANK ".TEX", &size);
    b = texbank_load(BANK ".TEX");
    CHECK(b != NULL && texbank_info(b, 0, &w, &h, &psm) && psm == TEX_T4, BANK " is a 16-colour texture");
    texbank_free(b);
    rgba = malloc((size_t)w * h * 4);
    want = malloc((size_t)w * h * 4);
    put_png("mods/alpha/library/data/" BANK ".texbank/0.png", w, h, kGreen);
    mods_find(mods, NULL, 0);
    b = texbank_load(BANK ".TEX");
    for (csa = 0; csa < 32; csa++) {
        CHECK(texbank_rgba(b, 0, csa, rgba) && all_of(rgba, w, h, kGreen), "the mod's PNG over the bank, palette %d", csa);
    }
    texbank_free(b);

    /* ---- a texture: a bank converted (the library) - the mod's 0.png over the library's ---- */
    snprintf(lib, sizeof(lib), "%slibrary/data/", sScratch);
    put("library/data/SYSTEM/.keep", "");
    snprintf(mods, sizeof(mods), "%slibrary/data/" BANK ".texbank/", sScratch);
    put("library/data/ITEM00/.keep", "");
    CHECK(texbank_convert(raw, size, mods, NULL, NULL), "the bank converted");
    snprintf(mods, sizeof(mods), "%smods/", sScratch);
    lib[strlen(lib) - 1] = 0;
    CHECK(disc_open(lib), "the library opened");
    mods_find(mods, NULL, 0);
    b = texbank_load(BANK ".TEX");
    CHECK(b != NULL && texbank_rgba(b, 0, 0, rgba) && all_of(rgba, w, h, kGreen), "the mod's PNG over the library's");
    for (csa = 1; csa < 32; csa++) {   /* (a palette whose picture is another: the library's still) */
        tex_decode_csa(raw, size, 0, csa, want);
        if (memcmp(want, rgba, (size_t)w * h * 4) != 0) {
            break;
        }
    }
    CHECK(csa < 32, "the bank has a palette that looks different");
    if (csa < 32) {
        CHECK(texbank_rgba(b, 0, csa, rgba) && memcmp(rgba, want, (size_t)w * h * 4) == 0,
              "palette %d: the library's picture (the mod gave only 0.png)", csa);
    }
    texbank_free(b);
    put_png("mods/alpha/library/data/" BANK ".texbank/0.png", 4, 2, kGreen);   /* (another size: what fits) */
    mods_find(mods, NULL, 0);
    b = texbank_load(BANK ".TEX");
    CHECK(texbank_rgba(b, 0, 0, rgba) && all_of(rgba, 4, 1, kGreen) && rgba[(size_t)w * 4 * 2 + 3] == 0,
          "a PNG of another size: as much as fits, the rest clear");
    texbank_free(b);

    printf("mods-check: %d checks, %d failed\n", sChecks, sBad);
    free(raw);
    free(rgba);
    free(want);
    SDL_EnumerateDirectory(sScratch, remove_one, NULL);
    SDL_RemovePath(argv[2]);
    return sBad != 0;
}
