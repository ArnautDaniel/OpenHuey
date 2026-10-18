/* Texture banks (texbank.h): a .TEX bank decoded as it is read (tex.c), or the library's PNGs. */
#include "texbank.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <SDL3/SDL.h>

#include "../core/files.h"
#include "../third_party/stb_image.h"
#include "../third_party/stb_image_write.h"
#include "tex.h"

#define PALETTES 32

typedef struct Image {   /* a converted texture's PNG (as read) */
    uint8_t *png;
    size_t size;
} Image;

typedef struct Texture {
    int w, h, psm;
    int nimages;
    Image *images;
    uint8_t palette[PALETTES];   /* the image for each palette */
} Texture;

struct TexBank {
    const uint8_t *raw;   /* a .TEX bank: decoded on asking */
    size_t size;
    uint8_t *owned;       /* ... read here (freed with it) */
    int n;
    Texture *t;           /* the converted form's textures */
};

/* ---- the .TEX bank as it is ---- */

TexBank *texbank_wrap(const uint8_t *bank, size_t size) {
    TexBank *b;
    int n = tex_count(bank, size);

    if (n == 0 || (b = calloc(1, sizeof(*b))) == NULL) {
        return NULL;
    }
    b->raw = bank;
    b->size = size;
    b->n = n;
    return b;
}

/* ---- the converted form ---- */

static void folder_of_bank(char *out, size_t n, const char *path) {   /* "A/B.TEX" -> "A/B.texbank/" */
    size_t len = strlen(path);

    if (len > 4 && strcasecmp(path + len - 4, ".TEX") == 0) {
        len -= 4;
    }
    snprintf(out, n, "%.*s.texbank/", (int)len, path);
}

static TexBank *load_converted(const char *path) {
    char dir[600], file[700];
    size_t size;
    char *text, *line, *save = NULL;
    TexBank *b;
    int format = 0, n = 0;

    folder_of_bank(dir, sizeof(dir), path);
    snprintf(file, sizeof(file), "%sbank.txt", dir);
    if ((text = (char *)files_read(file, &size)) == NULL) {
        return NULL;
    }
    b = calloc(1, sizeof(*b));
    for (line = strtok_r(text, "\n", &save); line != NULL && b != NULL; line = strtok_r(NULL, "\n", &save)) {
        int i, w, h, psm, k, used;
        char *p;

        if (sscanf(line, "hga-texbank %d", &format) == 1) {
            continue;
        }
        if (sscanf(line, "textures %d", &n) == 1) {
            if (n <= 0 || n > 256 || (b->t = calloc((size_t)n, sizeof(*b->t))) == NULL) {
                break;
            }
            b->n = n;
            continue;
        }
        if (sscanf(line, "texture %d %d %d %d %d", &i, &w, &h, &psm, &k) == 5) {
            if (i < 0 || i >= b->n || k < 1 || k > PALETTES) {
                continue;
            }
            b->t[i].w = w;
            b->t[i].h = h;
            b->t[i].psm = psm;
            b->t[i].nimages = k;
            b->t[i].images = calloc((size_t)k, sizeof(Image));
            for (k = 0; k < b->t[i].nimages; k++) {
                if (k == 0) {
                    snprintf(file, sizeof(file), "%s%d.png", dir, i);
                } else {
                    snprintf(file, sizeof(file), "%s%d-%d.png", dir, i, k);
                }
                b->t[i].images[k].png = files_read(file, &b->t[i].images[k].size);
            }
            continue;
        }
        if (sscanf(line, "palettes %d%n", &i, &used) == 1 && i >= 0 && i < b->n) {
            p = line + used;
            for (k = 0; k < PALETTES; k++) {
                int v = (int)strtol(p, &p, 10);

                b->t[i].palette[k] = (uint8_t)(v >= 0 && v < b->t[i].nimages ? v : 0);
            }
        }
    }
    free(text);
    if (b != NULL && (format != TEXBANK_FORMAT || b->t == NULL)) {
        fprintf(stderr, "texbank: %sbank.txt isn't one this hga reads\n", dir);
        texbank_free(b);
        return NULL;
    }
    return b;
}

TexBank *texbank_load(const char *path) {
    TexBank *b = load_converted(path);
    uint8_t *raw;
    size_t size;

    if (b != NULL) {
        return b;
    }
    if ((raw = files_read(path, &size)) == NULL) {
        return NULL;
    }
    if ((b = texbank_wrap(raw, size)) == NULL) {
        free(raw);
        return NULL;
    }
    b->owned = raw;
    return b;
}

void texbank_free(TexBank *b) {
    int i, k;

    if (b == NULL) {
        return;
    }
    for (i = 0; b->t != NULL && i < b->n; i++) {
        for (k = 0; k < b->t[i].nimages; k++) {
            free(b->t[i].images[k].png);
        }
        free(b->t[i].images);
    }
    free(b->t);
    free(b->owned);
    free(b);
}

int texbank_count(const TexBank *b) { return b != NULL ? b->n : 0; }

int texbank_info(const TexBank *b, int i, int *w, int *h, int *psm) {
    if (b == NULL || i < 0 || i >= b->n) {
        return 0;
    }
    if (b->t == NULL) {
        const TexEntry *t = tex_entry(b->raw, i);

        *w = t->w;
        *h = t->h;
        *psm = t->psm;
    } else {
        *w = b->t[i].w;
        *h = b->t[i].h;
        *psm = b->t[i].psm;
    }
    return *w > 0 && *h > 0;
}

int texbank_rgba(const TexBank *b, int i, int csa, uint8_t *rgba) {
    const Texture *t;
    const Image *im;
    uint8_t *px;
    int w, h, ch;

    if (b == NULL || i < 0 || i >= b->n) {
        return 0;
    }
    if (b->t == NULL) {
        return tex_decode_csa(b->raw, b->size, i, csa, rgba);
    }
    t = &b->t[i];
    if (t->nimages == 0) {
        return 0;
    }
    im = &t->images[t->palette[csa < 0 ? 0 : csa & (PALETTES - 1)]];
    if (im->png == NULL) {
        return 0;
    }
    px = stbi_load_from_memory(im->png, (int)im->size, &w, &h, &ch, 4);
    if (px == NULL) {
        return 0;
    }
    if (w != t->w || h != t->h) {   /* (a mod's of another size: what fits, the rest clear) */
        int y, cw = w < t->w ? w : t->w;

        memset(rgba, 0, (size_t)t->w * t->h * 4);
        for (y = 0; y < h && y < t->h; y++) {
            memcpy(rgba + (size_t)y * t->w * 4, px + (size_t)y * w * 4, (size_t)cw * 4);
        }
    } else {
        memcpy(rgba, px, (size_t)w * h * 4);
    }
    stbi_image_free(px);
    return 1;
}

/* ---- the import's conversion ---- */

typedef struct Sink {
    FILE *f;
    long size;
    int ok;
} Sink;

static void sink_write(void *user, void *data, int n) {
    Sink *s = user;

    if (s->ok && fwrite(data, 1, (size_t)n, s->f) != (size_t)n) {
        s->ok = 0;
    }
    s->size += n;
}

static int write_png(const char *dir, const char *name, const uint8_t *rgba, int w, int h, TexbankWrote wrote, void *user) {
    char path[1200];
    Sink s = {0};

    snprintf(path, sizeof(path), "%s%s", dir, name);
    if ((s.f = fopen(path, "wb")) == NULL) {
        return 0;
    }
    s.ok = 1;
    if (!stbi_write_png_to_func(sink_write, &s, w, h, 4, rgba, w * 4)) {
        s.ok = 0;
    }
    s.ok = fclose(s.f) == 0 && s.ok;
    if (s.ok && wrote != NULL) {
        wrote(user, name, s.size);
    }
    return s.ok;
}

int texbank_convert(const uint8_t *bank, size_t size, const char *dir, TexbankWrote wrote, void *user) {
    int n = tex_count(bank, size), i, ok = 1;
    char text[65536], name[64], path[1200];
    size_t len = 0;
    FILE *f;

    if (n == 0 || !SDL_CreateDirectory(dir)) {
        return 0;
    }
    len += (size_t)snprintf(text + len, sizeof(text) - len, "hga-texbank %d\ntextures %d\n", TEXBANK_FORMAT, n);
    for (i = 0; i < n && ok && len < sizeof(text) - 256; i++) {   /* (a line each, at most ~140 bytes) */
        const TexEntry *t = tex_entry(bank, i);
        size_t bytes = (size_t)t->w * t->h * 4;
        uint8_t *px[PALETTES] = {0};
        uint8_t map[PALETTES] = {0};
        int images = 0, csa, k, palettes = t->psm == TEX_T4 ? PALETTES : 1;

        if (t->w == 0 || t->h == 0 || bytes > (size_t)64 << 20) {
            len += (size_t)snprintf(text + len, sizeof(text) - len, "texture %d %d %d %d 0\n", i, t->w, t->h, t->psm);
            continue;
        }
        for (csa = 0; csa < palettes; csa++) {   /* (each palette's picture; one kept per different one) */
            uint8_t *rgba = malloc(bytes);

            if (rgba == NULL || !tex_decode_csa(bank, size, i, csa, rgba)) {
                free(rgba);
                map[csa] = 0;
                continue;
            }
            for (k = 0; k < images && memcmp(px[k], rgba, bytes) != 0; k++) {
            }
            if (k < images) {
                free(rgba);
            } else {
                px[images++] = rgba;
            }
            map[csa] = (uint8_t)k;
        }
        for (k = 0; k < images && ok; k++) {
            if (k == 0) {
                snprintf(name, sizeof(name), "%d.png", i);
            } else {
                snprintf(name, sizeof(name), "%d-%d.png", i, k);
            }
            ok = write_png(dir, name, px[k], t->w, t->h, wrote, user);
        }
        len += (size_t)snprintf(text + len, sizeof(text) - len, "texture %d %d %d %d %d\n", i, t->w, t->h, t->psm, images);
        if (images > 1) {
            len += (size_t)snprintf(text + len, sizeof(text) - len, "palettes %d", i);
            for (csa = 0; csa < PALETTES; csa++) {
                len += (size_t)snprintf(text + len, sizeof(text) - len, " %d", map[csa]);
            }
            len += (size_t)snprintf(text + len, sizeof(text) - len, "\n");
        }
        for (k = 0; k < images; k++) {
            free(px[k]);
        }
    }
    if (!ok || i < n) {
        return 0;
    }
    snprintf(path, sizeof(path), "%sbank.txt", dir);
    if ((f = fopen(path, "wb")) == NULL || fwrite(text, 1, len, f) != len) {
        if (f != NULL) {
            fclose(f);
        }
        return 0;
    }
    if (fclose(f) != 0) {
        return 0;
    }
    if (wrote != NULL) {
        wrote(user, "bank.txt", (long)len);
    }
    return 1;
}
