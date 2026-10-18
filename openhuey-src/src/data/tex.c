#include "tex.h"

#include <string.h>

int tex_count(const uint8_t *bank, size_t size) {
    uint32_t n;

    if (bank == NULL || size < 16) {
        return 0;
    }
    memcpy(&n, bank, 4);
    if (n == 0 || n > 256 || 16 + (size_t)n * 16 > size) {
        return 0;
    }
    return (int)n;
}

const TexEntry *tex_entry(const uint8_t *bank, int i) {
    return (const TexEntry *)(bank + 16 + (size_t)i * 16);
}

/* the GS alpha range: 0x80 is opaque */
static uint8_t alpha8(uint8_t a) {
    return a >= 0x80 ? 255 : (uint8_t)(a * 2);
}

static void put(uint8_t *out, uint32_t abgr) {   /* little-endian R, G, B, A */
    out[0] = (uint8_t)abgr;
    out[1] = (uint8_t)(abgr >> 8);
    out[2] = (uint8_t)(abgr >> 16);
    out[3] = alpha8((uint8_t)(abgr >> 24));
}

static uint32_t ct16(uint16_t c) {   /* 5:5:5:1 to 8:8:8:8 (the 1-bit alpha as 0x80) */
    return (uint32_t)(c & 0x1F) << 3 | (uint32_t)((c >> 5) & 0x1F) << 11 | (uint32_t)((c >> 10) & 0x1F) << 19 |
           (c & 0x8000 ? 0x80000000u : 0);
}

/* palette entry i (of n); a 16-colour one from palette `csa` of those in the 16-wide CLUT image
 * (each an 8 x 2 block: the GS's CSA) */
static uint32_t palette(const TexEntry *t, const uint8_t *clut, int i, int n, int csa) {
    size_t bytes = (size_t)t->clut_qwc * 16, size = t->cpsm == TEX_CT16 ? 2 : 4;
    int k;

    if (n == 256) {
        k = (i & ~0x18) | (i & 0x08) << 1 | (i & 0x10) >> 1;   /* CSM1: swap bits 3 and 4 */
    } else {
        k = (i & 7) + (csa & 1) * 8 + ((i >> 3) + (csa >> 1) * 2) * 16;   /* a 16-colour palette: an 8 x 2 block of a 16-wide image */
        if ((size_t)(k + 1) * size > bytes) {
            k = i + csa * 16;          /* ... unless the palettes were stored on their own */
        }
    }
    if ((size_t)(k + 1) * size > bytes) {
        return 0;
    }
    if (size == 2) {
        return ct16((uint16_t)(clut[k * 2] | clut[k * 2 + 1] << 8));
    }
    return (uint32_t)clut[k * 4] | (uint32_t)clut[k * 4 + 1] << 8 | (uint32_t)clut[k * 4 + 2] << 16 |
           (uint32_t)clut[k * 4 + 3] << 24;
}

int tex_decode(const uint8_t *bank, size_t size, int i, uint8_t *rgba) {
    return tex_decode_csa(bank, size, i, 0, rgba);
}

int tex_decode_csa(const uint8_t *bank, size_t size, int i, int csa, uint8_t *rgba) {
    const TexEntry *t = tex_entry(bank, i);
    size_t at = (size_t)((const uint8_t *)t - bank) + (size_t)t->data;
    size_t n = (size_t)t->w * t->h, k;
    const uint8_t *img, *clut;
    uint32_t pal[256];

    if (t->data <= 0 || at + (size_t)(t->image_qwc + t->clut_qwc) * 16 > size || n == 0) {
        return 0;
    }
    img = bank + at;
    clut = img + (size_t)t->image_qwc * 16;
    switch (t->psm) {
    case TEX_T8:
    case TEX_T4: {
        int colours = t->psm == TEX_T8 ? 256 : 16, c;

        if ((t->psm == TEX_T8 ? n : n / 2) > (size_t)t->image_qwc * 16) {
            return 0;
        }
        for (c = 0; c < colours; c++) {
            pal[c] = palette(t, clut, c, colours, csa < 0 ? 0 : csa);
        }
        for (k = 0; k < n; k++) {
            put(rgba + k * 4, t->psm == TEX_T8 ? pal[img[k]] : pal[(img[k >> 1] >> ((k & 1) * 4)) & 0xF]);
        }
        return 1;
    }
    case TEX_CT32:
        if (n * 4 > (size_t)t->image_qwc * 16) {
            return 0;
        }
        for (k = 0; k < n; k++) {
            put(rgba + k * 4, (uint32_t)img[k * 4] | (uint32_t)img[k * 4 + 1] << 8 | (uint32_t)img[k * 4 + 2] << 16 |
                                  (uint32_t)img[k * 4 + 3] << 24);
        }
        return 1;
    case TEX_CT24:
        for (k = 0; k < n && k * 3 + 2 < (size_t)t->image_qwc * 16; k++) {
            put(rgba + k * 4, (uint32_t)img[k * 3] | (uint32_t)img[k * 3 + 1] << 8 | (uint32_t)img[k * 3 + 2] << 16 | 0x80000000u);
        }
        return 1;
    case TEX_CT16:
        for (k = 0; k < n && k * 2 + 1 < (size_t)t->image_qwc * 16; k++) {
            put(rgba + k * 4, ct16((uint16_t)(img[k * 2] | img[k * 2 + 1] << 8)));
        }
        return 1;
    }
    return 0;
}
