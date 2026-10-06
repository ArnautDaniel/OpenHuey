/* Software model of the PS2 Graphics Synthesizer: 4 MB of VRAM in the GS's own layout, the GIF
 * (packets from DMA / VIF1 DIRECT / sceGsPutDrawEnv), image transfers and a rasterizer for points,
 * lines, triangles and sprites with texturing, alpha / Z tests and alpha blending. The display
 * side (gs_display) turns the frame buffer the PCRTC shows into RGBA for the window.
 *
 * Correctness first: no caches, nearest-neighbour texturing (bilinear to come), no dithering. */
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gs_local.h"

/* ---- VRAM layout ---- */

static uint32_t vram32[1024 * 1024];   /* 4 MB as words */
#define VRAM8 ((uint8_t *)vram32)
#define VRAM16 ((uint16_t *)vram32)

static const uint8_t blockTable32[4][8] = {
    {0, 1, 4, 5, 16, 17, 20, 21}, {2, 3, 6, 7, 18, 19, 22, 23},
    {8, 9, 12, 13, 24, 25, 28, 29}, {10, 11, 14, 15, 26, 27, 30, 31}};
static const uint8_t blockTable32Z[4][8] = {
    {24, 25, 28, 29, 8, 9, 12, 13}, {26, 27, 30, 31, 10, 11, 14, 15},
    {16, 17, 20, 21, 0, 1, 4, 5}, {18, 19, 22, 23, 2, 3, 6, 7}};
static const uint8_t blockTable16[8][4] = {
    {0, 2, 8, 10}, {1, 3, 9, 11}, {4, 6, 12, 14}, {5, 7, 13, 15},
    {16, 18, 24, 26}, {17, 19, 25, 27}, {20, 22, 28, 30}, {21, 23, 29, 31}};
static const uint8_t blockTable16S[8][4] = {
    {0, 2, 16, 18}, {1, 3, 17, 19}, {8, 10, 24, 26}, {9, 11, 25, 27},
    {4, 6, 20, 22}, {5, 7, 21, 23}, {12, 14, 28, 30}, {13, 15, 29, 31}};
static const uint8_t blockTable16Z[8][4] = {
    {24, 26, 16, 18}, {25, 27, 17, 19}, {28, 30, 20, 22}, {29, 31, 21, 23},
    {8, 10, 0, 2}, {9, 11, 1, 3}, {12, 14, 4, 6}, {13, 15, 5, 7}};
static const uint8_t blockTable16SZ[8][4] = {
    {24, 26, 8, 10}, {25, 27, 9, 11}, {16, 18, 0, 2}, {17, 19, 1, 3},
    {28, 30, 12, 14}, {29, 31, 13, 15}, {20, 22, 4, 6}, {21, 23, 5, 7}};
#define blockTable8 blockTable32
#define blockTable4 blockTable16

static const uint16_t columnTable8[16][16] = {
    {0, 4, 16, 20, 32, 36, 48, 52, 2, 6, 18, 22, 34, 38, 50, 54},
    {8, 12, 24, 28, 40, 44, 56, 60, 10, 14, 26, 30, 42, 46, 58, 62},
    {33, 37, 49, 53, 1, 5, 17, 21, 35, 39, 51, 55, 3, 7, 19, 23},
    {41, 45, 57, 61, 9, 13, 25, 29, 43, 47, 59, 63, 11, 15, 27, 31},
    {96, 100, 112, 116, 64, 68, 80, 84, 98, 102, 114, 118, 66, 70, 82, 86},
    {104, 108, 120, 124, 72, 76, 88, 92, 106, 110, 122, 126, 74, 78, 90, 94},
    {65, 69, 81, 85, 97, 101, 113, 117, 67, 71, 83, 87, 99, 103, 115, 119},
    {73, 77, 89, 93, 105, 109, 121, 125, 75, 79, 91, 95, 107, 111, 123, 127},
    {128, 132, 144, 148, 160, 164, 176, 180, 130, 134, 146, 150, 162, 166, 178, 182},
    {136, 140, 152, 156, 168, 172, 184, 188, 138, 142, 154, 158, 170, 174, 186, 190},
    {161, 165, 177, 181, 129, 133, 145, 149, 163, 167, 179, 183, 131, 135, 147, 151},
    {169, 173, 185, 189, 137, 141, 153, 157, 171, 175, 187, 191, 139, 143, 155, 159},
    {224, 228, 240, 244, 192, 196, 208, 212, 226, 230, 242, 246, 194, 198, 210, 214},
    {232, 236, 248, 252, 200, 204, 216, 220, 234, 238, 250, 254, 202, 206, 218, 222},
    {193, 197, 209, 213, 225, 229, 241, 245, 195, 199, 211, 215, 227, 231, 243, 247},
    {201, 205, 217, 221, 233, 237, 249, 253, 203, 207, 219, 223, 235, 239, 251, 255}};

/* 4-bit: built from the 8-bit pattern at init (see gs_init_tables) */
static uint16_t columnTable4[16][32];

static inline uint32_t col32(uint32_t x, uint32_t y) {   /* 8x8 block of words */
    return (y >> 1) * 16 + (y & 1) * 2 + (x >> 1) * 4 + (x & 1);
}

static inline uint32_t col16(uint32_t x, uint32_t y) {   /* 16x8 block of halfwords */
    return (y >> 1) * 32 + (y & 1) * 4 + ((x & 7) >> 1) * 8 + (x & 1) * 2 + (x >> 3);
}

/* word / halfword / byte / nibble index of pixel (x, y) of a buffer at block bp, width bw (in 64s) */
static inline __attribute__((always_inline)) uint32_t addr32(uint32_t bp, uint32_t bw, uint32_t x, uint32_t y, const uint8_t bt[4][8]) {
    uint32_t page = (y >> 5) * bw + (x >> 6);
    return ((bp + page * 32 + bt[(y >> 3) & 3][(x >> 3) & 7]) * 64 + col32(x & 7, y & 7)) & 0xFFFFF;
}

static inline __attribute__((always_inline)) uint32_t addr16(uint32_t bp, uint32_t bw, uint32_t x, uint32_t y, const uint8_t bt[8][4]) {
    uint32_t page = (y >> 6) * bw + (x >> 6);
    return ((bp + page * 32 + bt[(y >> 3) & 7][(x >> 4) & 3]) * 128 + col16(x & 15, y & 7)) & 0x1FFFFF;
}

static inline __attribute__((always_inline)) uint32_t addr8(uint32_t bp, uint32_t bw, uint32_t x, uint32_t y) {
    uint32_t page = (y >> 6) * ((bw + 1) >> 1) + (x >> 7);
    return ((bp + page * 32 + blockTable8[(y >> 4) & 3][(x >> 4) & 7]) * 256 + columnTable8[y & 15][x & 15]) & 0x3FFFFF;
}

static inline __attribute__((always_inline)) uint32_t addr4(uint32_t bp, uint32_t bw, uint32_t x, uint32_t y) {
    uint32_t page = (y >> 7) * ((bw + 1) >> 1) + (x >> 7);
    return ((bp + page * 32 + blockTable4[(y >> 4) & 7][(x >> 5) & 3]) * 512 + columnTable4[y & 15][x & 31]) & 0x7FFFFF;
}

static void gs_init_tables(void) {
    /* The 4-bit column: 32x16 nibbles. Each 8-bit column row pair is split into two nibble rows;
     * the arrangement below matches the GS (same derivation as GSdx's columnTable4). */
    static const uint16_t t[4][32] = {
        {0, 8, 32, 40, 64, 72, 96, 104, 2, 10, 34, 42, 66, 74, 98, 106, 4, 12, 36, 44, 68, 76, 100, 108, 6, 14, 38, 46, 70, 78, 102, 110},
        {16, 24, 48, 56, 80, 88, 112, 120, 18, 26, 50, 58, 82, 90, 114, 122, 20, 28, 52, 60, 84, 92, 116, 124, 22, 30, 54, 62, 86, 94, 118, 126},
        {65, 73, 97, 105, 1, 9, 33, 41, 67, 75, 99, 107, 3, 11, 35, 43, 69, 77, 101, 109, 5, 13, 37, 45, 71, 79, 103, 111, 7, 15, 39, 47},
        {81, 89, 113, 121, 17, 25, 49, 57, 83, 91, 115, 123, 19, 27, 51, 59, 85, 93, 117, 125, 21, 29, 53, 61, 87, 95, 119, 127, 23, 31, 55, 63}};
    int y, x;

    /* rows of 4 columns; odd columns swap the two 4-nibble halves of each group of 8 */
    for (y = 0; y < 16; y++) {
        int c = y >> 2;

        for (x = 0; x < 32; x++) {
            columnTable4[y][x] = (uint16_t)(t[y & 3][(c & 1) ? x ^ 4 : x] + c * 128);
        }
    }
}

/* pixel access by format */
static inline __attribute__((always_inline)) uint32_t vram_read(uint32_t psm, uint32_t bp, uint32_t bw, uint32_t x, uint32_t y) {
    uint32_t a;

    switch (psm) {
    case PSMCT32: case PSMZ32:
        return vram32[addr32(bp, bw, x, y, psm == PSMZ32 ? blockTable32Z : blockTable32)];
    case PSMCT24: case PSMZ24:
        return vram32[addr32(bp, bw, x, y, psm == PSMZ24 ? blockTable32Z : blockTable32)] & 0xFFFFFF;
    case PSMCT16: return VRAM16[addr16(bp, bw, x, y, blockTable16)];
    case PSMCT16S: return VRAM16[addr16(bp, bw, x, y, blockTable16S)];
    case PSMZ16: return VRAM16[addr16(bp, bw, x, y, blockTable16Z)];
    case PSMZ16S: return VRAM16[addr16(bp, bw, x, y, blockTable16SZ)];
    case PSMT8: return VRAM8[addr8(bp, bw, x, y)];
    case PSMT4:
        a = addr4(bp, bw, x, y);
        return (VRAM8[a >> 1] >> ((a & 1) * 4)) & 0xF;
    case PSMT8H: return vram32[addr32(bp, bw, x, y, blockTable32)] >> 24;
    case PSMT4HL: return (vram32[addr32(bp, bw, x, y, blockTable32)] >> 24) & 0xF;
    case PSMT4HH: return vram32[addr32(bp, bw, x, y, blockTable32)] >> 28;
    }
    return 0;
}

static inline __attribute__((always_inline)) void vram_write(uint32_t psm, uint32_t bp, uint32_t bw, uint32_t x, uint32_t y, uint32_t v) {
    uint32_t a;

    switch (psm) {
    case PSMCT32: case PSMZ32:
        vram32[addr32(bp, bw, x, y, psm == PSMZ32 ? blockTable32Z : blockTable32)] = v;
        break;
    case PSMCT24: case PSMZ24:
        a = addr32(bp, bw, x, y, psm == PSMZ24 ? blockTable32Z : blockTable32);
        vram32[a] = (vram32[a] & 0xFF000000) | (v & 0xFFFFFF);
        break;
    case PSMCT16: VRAM16[addr16(bp, bw, x, y, blockTable16)] = (uint16_t)v; break;
    case PSMCT16S: VRAM16[addr16(bp, bw, x, y, blockTable16S)] = (uint16_t)v; break;
    case PSMZ16: VRAM16[addr16(bp, bw, x, y, blockTable16Z)] = (uint16_t)v; break;
    case PSMZ16S: VRAM16[addr16(bp, bw, x, y, blockTable16SZ)] = (uint16_t)v; break;
    case PSMT8: VRAM8[addr8(bp, bw, x, y)] = (uint8_t)v; break;
    case PSMT4:
        a = addr4(bp, bw, x, y);
        VRAM8[a >> 1] = (uint8_t)((VRAM8[a >> 1] & (0xF0 >> ((a & 1) * 4))) | ((v & 0xF) << ((a & 1) * 4)));
        break;
    case PSMT8H:
        a = addr32(bp, bw, x, y, blockTable32);
        vram32[a] = (vram32[a] & 0x00FFFFFF) | (v << 24);
        break;
    case PSMT4HL:
        a = addr32(bp, bw, x, y, blockTable32);
        vram32[a] = (vram32[a] & 0xF0FFFFFF) | ((v & 0xF) << 24);
        break;
    case PSMT4HH:
        a = addr32(bp, bw, x, y, blockTable32);
        vram32[a] = (vram32[a] & 0x0FFFFFFF) | ((v & 0xF) << 28);
        break;
    }
}

/* ---- registers ---- */

typedef struct GsContext {
    uint64_t xyoffset, prmode_unused, tex0, tex1, tex2, clamp, scissor, alpha, test, fba, frame, zbuf;
} GsContext;

static struct {
    GsContext ctx[2];
    uint64_t prim, prmode, prmodecont, rgbaq, st, uv, fog, texclut, scanmsk, texa, fogcol, dimx, dthe,
        colclamp, pabe, bitbltbuf, trxpos, trxreg, trxdir;
    /* vertex queue */
    GsVertex v[3];
    int nv;
    float q;
    /* image transfer state */
    int xfer;
    uint32_t xx, xy;
    uint64_t pending;      /* leftover bits of a CT24 transfer */
    int pendingBits;
    /* CLUT */
    uint32_t clut[256];
    uint64_t cbp0, cbp1;
    /* privileged */
    uint64_t pmode, smode2, dispfb[2], display[2], bgcolor;
} gs;

#define BITS(v, lo, n) ((uint32_t)(((v) >> (lo)) & ((1ULL << (n)) - 1)))

/* ---- image transfer (host -> local) ---- */

static void xfer_start(void) {
    gs.xfer = BITS(gs.trxdir, 0, 2) == 0;
    gs.xx = 0;
    gs.xy = 0;
    gs.pending = 0;
    gs.pendingBits = 0;
    if (BITS(gs.trxdir, 0, 2) == 2) {   /* local -> local */
        uint32_t sbp = BITS(gs.bitbltbuf, 0, 14), sbw = BITS(gs.bitbltbuf, 16, 6), spsm = BITS(gs.bitbltbuf, 24, 6);
        uint32_t dbp = BITS(gs.bitbltbuf, 32, 14), dbw = BITS(gs.bitbltbuf, 48, 6), dpsm = BITS(gs.bitbltbuf, 56, 6);
        uint32_t ssx = BITS(gs.trxpos, 0, 11), ssy = BITS(gs.trxpos, 16, 11);
        uint32_t dsx = BITS(gs.trxpos, 32, 11), dsy = BITS(gs.trxpos, 48, 11);
        uint32_t w = BITS(gs.trxreg, 0, 12), h = BITS(gs.trxreg, 32, 12), x, y;

        for (y = 0; y < h; y++) {
            for (x = 0; x < w; x++) {
                vram_write(dpsm, dbp, dbw, (dsx + x) & 2047, (dsy + y) & 2047,
                           vram_read(spsm, sbp, sbw, (ssx + x) & 2047, (ssy + y) & 2047));
            }
        }
        gs.xfer = 0;
    }
}

static void xfer_pixel(uint32_t v) {
    uint32_t dbp = BITS(gs.bitbltbuf, 32, 14), dbw = BITS(gs.bitbltbuf, 48, 6), dpsm = BITS(gs.bitbltbuf, 56, 6);
    uint32_t w = BITS(gs.trxreg, 0, 12), h = BITS(gs.trxreg, 32, 12);
    uint32_t dsx = BITS(gs.trxpos, 32, 11), dsy = BITS(gs.trxpos, 48, 11);

    if (gs.xy >= h || w == 0) {
        gs.xfer = 0;
        return;
    }
    vram_write(dpsm, dbp, dbw, (dsx + gs.xx) & 2047, (dsy + gs.xy) & 2047, v);
    if (++gs.xx >= w) {
        gs.xx = 0;
        if (++gs.xy >= h) {
            gs.xfer = 0;
        }
    }
}

/* one quadword of IMAGE data */
static void xfer_qword(const uint64_t *q) {
    uint32_t dpsm = BITS(gs.bitbltbuf, 56, 6);
    int i, bits;

    if (!gs.xfer) {
        return;
    }
    bits = (dpsm == PSMCT32 || dpsm == PSMZ32) ? 32 : (dpsm == PSMCT24 || dpsm == PSMZ24) ? 24
         : (dpsm == PSMT8 || dpsm == PSMT8H) ? 8 : (dpsm == PSMT4 || dpsm == PSMT4HL || dpsm == PSMT4HH) ? 4 : 16;
    if (bits == 24) {   /* 24-bit pixels span the 64-bit halves */
        for (i = 0; i < 2 && gs.xfer; i++) {
            uint64_t d = q[i];
            int have = 64;

            while (gs.xfer && gs.pendingBits + have >= 24) {
                int take = 24 - gs.pendingBits;
                uint32_t v = (uint32_t)(gs.pending | ((d & ((1ULL << take) - 1)) << gs.pendingBits));

                d >>= take;
                have -= take;
                gs.pending = 0;
                gs.pendingBits = 0;
                xfer_pixel(v);
            }
            gs.pending |= d << gs.pendingBits;
            gs.pendingBits += have;
        }
        return;
    }
    for (i = 0; i < 2; i++) {
        uint64_t d = q[i];
        int n;

        for (n = 0; n < 64 / bits && gs.xfer; n++) {
            xfer_pixel((uint32_t)(d & ((1ULL << bits) - 1)));
            d >>= bits;
        }
    }
}

/* ---- textures ---- */

static uint32_t expand16(uint32_t c, uint64_t texa, int tcc) {
    uint32_t r = (c & 0x1F) << 3, g = ((c >> 5) & 0x1F) << 3, b = ((c >> 10) & 0x1F) << 3;
    uint32_t a = (c & 0x8000) ? BITS(texa, 32, 8) : ((c & 0x7FFF) == 0 && BITS(texa, 15, 1)) ? 0 : BITS(texa, 0, 8);

    (void)tcc;
    return r | g << 8 | b << 16 | a << 24;
}

/* load the CLUT if TEX0's CLD says so */
static void clut_load(uint64_t tex0) {
    uint32_t cbp = BITS(tex0, 37, 14), cpsm = BITS(tex0, 51, 4), csm = BITS(tex0, 55, 1);
    uint32_t csa = BITS(tex0, 56, 5), cld = BITS(tex0, 61, 3), psm = BITS(tex0, 20, 6);
    int n = (psm == PSMT8 || psm == PSMT8H) ? 256 : 16, i;
    int load = 0;

    switch (cld) {
    case 1: load = 1; break;
    case 2: load = 1; gs.cbp0 = cbp; break;
    case 3: load = 1; gs.cbp1 = cbp; break;
    case 4: load = gs.cbp0 != cbp; gs.cbp0 = cbp; break;
    case 5: load = gs.cbp1 != cbp; gs.cbp1 = cbp; break;
    }
    if (!load || (psm != PSMT8 && psm != PSMT8H && psm != PSMT4 && psm != PSMT4HL && psm != PSMT4HH)) {
        return;
    }
    for (i = 0; i < n; i++) {
        uint32_t x, y, c;

        if (csm == 0) {
            /* CSM1: 256-entry CLUTs are 16x16 with entries 8-15 and 16-23 of each 32 swapped
             * (8x2 blocks); 16-entry ones are 8x2 */
            if (n == 256) {
                x = (uint32_t)(i % 8 + ((i / 16) % 2) * 8);
                y = (uint32_t)((i / 32) * 2 + (i / 8) % 2);
            } else {
                x = (uint32_t)(i & 7);
                y = (uint32_t)(i >> 3);
            }
        } else {
            x = (uint32_t)i;
            y = 0;
        }
        if (cpsm == PSMCT32 || cpsm == PSMCT24) {
            c = vram_read(PSMCT32, cbp, 1, x, y);
        } else {
            c = vram_read(cpsm == PSMCT16S ? PSMCT16S : PSMCT16, cbp, 1, x, y);
            c = expand16(c, gs.texa, 1);
        }
        gs.clut[(n == 16 ? csa * 16 : 0) + i] = c;
    }
}

/* ---- per-primitive state: the registers a primitive's pixels need, decoded once ---- */

typedef struct TexState {
    uint32_t tbp, tbw, psm, tw, th, tcc, tfx, wms, wmt, minu, maxu, minv, maxv, csa16;
} TexState;

static void tex_setup(TexState *t, uint64_t tex0, uint64_t clamp) {
    t->tbp = BITS(tex0, 0, 14);
    t->tbw = BITS(tex0, 14, 6);
    t->psm = BITS(tex0, 20, 6);
    t->tw = 1u << BITS(tex0, 26, 4);
    t->th = 1u << BITS(tex0, 30, 4);
    t->tcc = BITS(tex0, 34, 1);
    t->tfx = BITS(tex0, 35, 2);
    t->csa16 = BITS(tex0, 56, 5) * 16;
    t->wms = BITS(clamp, 0, 2);
    t->wmt = BITS(clamp, 2, 2);
    t->minu = BITS(clamp, 4, 10);
    t->maxu = BITS(clamp, 14, 10);
    t->minv = BITS(clamp, 24, 10);
    t->maxv = BITS(clamp, 34, 10);
}

/* texel (RGBA8) at integer (u, v) */
static inline __attribute__((always_inline)) uint32_t tex_fetch(const TexState *t, int u, int v) {
    switch (t->wms) {
    case 0: u &= (int)t->tw - 1; break;
    case 1: u = u < 0 ? 0 : u >= (int)t->tw ? (int)t->tw - 1 : u; break;
    case 2: u = u < (int)t->minu ? (int)t->minu : u > (int)t->maxu ? (int)t->maxu : u; break;
    case 3: u = (int)((u & t->minu) | t->maxu); break;
    }
    switch (t->wmt) {
    case 0: v &= (int)t->th - 1; break;
    case 1: v = v < 0 ? 0 : v >= (int)t->th ? (int)t->th - 1 : v; break;
    case 2: v = v < (int)t->minv ? (int)t->minv : v > (int)t->maxv ? (int)t->maxv : v; break;
    case 3: v = (int)((v & t->minv) | t->maxv); break;
    }
    switch (t->psm) {
    case PSMCT32:
        return vram_read(t->psm, t->tbp, t->tbw, (uint32_t)u, (uint32_t)v);
    case PSMCT24: {
        uint32_t c = vram_read(t->psm, t->tbp, t->tbw, (uint32_t)u, (uint32_t)v);

        return c | (uint32_t)(((c & 0xFFFFFF) == 0 && BITS(gs.texa, 15, 1)) ? 0 : BITS(gs.texa, 0, 8)) << 24;
    }
    case PSMCT16: case PSMCT16S:
        return expand16(vram_read(t->psm, t->tbp, t->tbw, (uint32_t)u, (uint32_t)v), gs.texa, t->tcc);
    case PSMT8: case PSMT8H:
        return gs.clut[vram_read(t->psm, t->tbp, t->tbw, (uint32_t)u, (uint32_t)v) & 0xFF];
    case PSMT4: case PSMT4HL: case PSMT4HH:
        return gs.clut[t->csa16 + (vram_read(t->psm, t->tbp, t->tbw, (uint32_t)u, (uint32_t)v) & 0xF)];
    }
    return 0xFF00FFFF;
}

/* floor without a libm call */
static inline int ifloor(float f) {
    int i = (int)f;

    return i - (f < (float)i);
}

static inline int clamp255(int v) { return v < 0 ? 0 : v > 255 ? 255 : v; }

/* the texture function: modulate / decal / highlight / highlight2 */
static inline __attribute__((always_inline)) void shade(const TexState *t, int *r, int *g, int *b, int *a, uint32_t tx) {
    int tr = (int)(tx & 0xFF), tg = (int)((tx >> 8) & 0xFF), tb = (int)((tx >> 16) & 0xFF), ta = (int)(tx >> 24);

    switch (t->tfx) {
    case 0:   /* MODULATE */
        *r = clamp255(tr * *r >> 7);
        *g = clamp255(tg * *g >> 7);
        *b = clamp255(tb * *b >> 7);
        if (t->tcc) {
            *a = clamp255(ta * *a >> 7);
        }
        break;
    case 1:   /* DECAL */
        *r = tr;
        *g = tg;
        *b = tb;
        if (t->tcc) {
            *a = ta;
        }
        break;
    case 2:   /* HIGHLIGHT */
        *r = clamp255((tr * *r >> 7) + *a);
        *g = clamp255((tg * *g >> 7) + *a);
        *b = clamp255((tb * *b >> 7) + *a);
        *a = t->tcc ? clamp255(ta + *a) : *a;
        break;
    case 3:   /* HIGHLIGHT2 */
        *r = clamp255((tr * *r >> 7) + *a);
        *g = clamp255((tg * *g >> 7) + *a);
        *b = clamp255((tb * *b >> 7) + *a);
        if (t->tcc) {
            *a = ta;
        }
        break;
    }
}

/* ---- pixel pipeline ---- */

unsigned gs_stat_prims, gs_stat_pixels, gs_stat_written;

typedef struct PixState {
    uint32_t fbp, fbw, fpsm, fbmsk, zbp, zpsm;
    int fb16, fb24;
    int sx0, sx1, sy0, sy1;           /* scissor (inclusive) */
    int ate, atst, aref, afail;       /* alpha test */
    int zte, ztst;                    /* depth test */
    int zwrite;
    int date, datm;                   /* destination alpha test */
    int abe, sa, sb, sc, sd, fix, pabe;
    int colclamp, fba;
    int fge, fogr, fogg, fogb;
} PixState;

static void pix_setup(PixState *p, int ctxi, int abe, int fge) {
    GsContext *c = &gs.ctx[ctxi];
    uint64_t test = c->test, al = c->alpha;

    p->fbp = BITS(c->frame, 0, 9) * 32;
    p->fbw = BITS(c->frame, 16, 6);
    p->fpsm = BITS(c->frame, 24, 6);
    p->fbmsk = BITS(c->frame, 32, 32);
    p->fb16 = p->fpsm == PSMCT16 || p->fpsm == PSMCT16S;
    p->fb24 = p->fpsm == PSMCT24;
    p->zbp = BITS(c->zbuf, 0, 9) * 32;
    p->zpsm = BITS(c->zbuf, 24, 4) | 0x30;
    p->zwrite = !BITS(c->zbuf, 32, 1);
    p->sx0 = (int)BITS(c->scissor, 0, 11);
    p->sx1 = (int)BITS(c->scissor, 16, 11);
    p->sy0 = (int)BITS(c->scissor, 32, 11);
    p->sy1 = (int)BITS(c->scissor, 48, 11);
    p->ate = (int)BITS(test, 0, 1);
    p->atst = (int)BITS(test, 1, 3);
    p->aref = (int)BITS(test, 4, 8);
    p->afail = (int)BITS(test, 12, 2);
    p->date = (int)BITS(test, 14, 1);
    p->datm = (int)BITS(test, 15, 1);
    p->zte = (int)BITS(test, 16, 1);
    p->ztst = (int)BITS(test, 17, 2);
    p->abe = abe;
    p->sa = (int)BITS(al, 0, 2);
    p->sb = (int)BITS(al, 2, 2);
    p->sc = (int)BITS(al, 4, 2);
    p->sd = (int)BITS(al, 6, 2);
    p->fix = (int)BITS(al, 32, 8);
    p->pabe = (int)BITS(gs.pabe, 0, 1);
    p->colclamp = (int)BITS(gs.colclamp, 0, 1);
    p->fba = (int)BITS(c->fba, 0, 1);
    p->fge = fge;
    p->fogr = (int)BITS(gs.fogcol, 0, 8);
    p->fogg = (int)BITS(gs.fogcol, 8, 8);
    p->fogb = (int)BITS(gs.fogcol, 16, 8);
}

static inline __attribute__((always_inline)) void draw_pixel(const PixState *p, int x, int y, uint32_t z, int r, int g, int b, int a, int fog) {
    int fbwrite = 1, zwrite = p->zwrite, pass;
    uint32_t dst, fbmsk = p->fbmsk;

    gs_stat_pixels++;
    if (x < p->sx0 || x > p->sx1 || y < p->sy0 || y > p->sy1) {
        return;
    }
    if (p->fge) {
        r = (r * fog + p->fogr * (255 - fog)) >> 8;
        g = (g * fog + p->fogg * (255 - fog)) >> 8;
        b = (b * fog + p->fogb * (255 - fog)) >> 8;
    }
    /* alpha test */
    if (p->ate) {
        switch (p->atst) {
        case 0: pass = 0; break;
        case 1: pass = 1; break;
        case 2: pass = a < p->aref; break;
        case 3: pass = a <= p->aref; break;
        case 4: pass = a == p->aref; break;
        case 5: pass = a >= p->aref; break;
        case 6: pass = a > p->aref; break;
        default: pass = a != p->aref; break;
        }
        if (!pass) {
            switch (p->afail) {
            case 0: return;                  /* KEEP */
            case 1: zwrite = 0; break;       /* FB_ONLY */
            case 2: fbwrite = 0; break;      /* ZB_ONLY */
            case 3: zwrite = 0; fbmsk |= 0xFF000000; break;   /* RGB_ONLY */
            }
        }
    }
    /* Z test */
    if (p->zte) {
        uint32_t zb = vram_read(p->zpsm, p->zbp, p->fbw, (uint32_t)x, (uint32_t)y);

        if (p->zpsm == PSMZ24) {
            z = z > 0xFFFFFF ? 0xFFFFFF : z;
        } else if (p->zpsm == PSMZ16 || p->zpsm == PSMZ16S) {
            z = z > 0xFFFF ? 0xFFFF : z;
        }
        switch (p->ztst) {
        case 0: return;
        case 1: break;
        case 2: if (z < zb) return; break;
        case 3: if (z <= zb) return; break;
        }
    }
    dst = vram_read(p->fpsm == PSMCT16S ? PSMCT16S : p->fb16 ? PSMCT16 : PSMCT32, p->fbp, p->fbw, (uint32_t)x, (uint32_t)y);
    if (p->fb16) {
        dst = expand16(dst, 0x8000ULL << 32 | 0, 1);
        dst = (dst & 0xFFFFFF) | ((dst >> 31) ? 0x80000000u : 0);
    } else if (p->fb24) {
        dst = (dst & 0xFFFFFF) | 0x80000000u;
    }
    /* destination alpha test */
    if (p->date && (int)(dst >> 31) != p->datm) {
        return;
    }
    /* alpha blending: ((A - B) * C >> 7) + D */
    if (p->abe && !(p->pabe && a < 0x80)) {
        int cs[3] = {r, g, b}, cd[3] = {(int)(dst & 0xFF), (int)((dst >> 8) & 0xFF), (int)((dst >> 16) & 0xFF)};
        int ca = p->sc == 0 ? a : p->sc == 1 ? (int)(dst >> 24) : p->fix, i;
        int out[3];

        for (i = 0; i < 3; i++) {
            int va = p->sa == 0 ? cs[i] : p->sa == 1 ? cd[i] : 0;
            int vb = p->sb == 0 ? cs[i] : p->sb == 1 ? cd[i] : 0;
            int vd = p->sd == 0 ? cs[i] : p->sd == 1 ? cd[i] : 0;

            out[i] = (((va - vb) * ca) >> 7) + vd;
        }
        r = out[0];
        g = out[1];
        b = out[2];
    }
    if (p->colclamp) {
        r = clamp255(r);
        g = clamp255(g);
        b = clamp255(b);
    } else {
        r &= 0xFF;
        g &= 0xFF;
        b &= 0xFF;
    }
    a |= p->fba << 7;
    gs_stat_written += fbwrite;
    if (fbwrite) {
        uint32_t px = (uint32_t)r | (uint32_t)g << 8 | (uint32_t)b << 16 | (uint32_t)(a & 0xFF) << 24;

        if (p->fb16) {
            uint32_t p16 = (px >> 3 & 0x1F) | (px >> 11 & 0x1F) << 5 | (px >> 19 & 0x1F) << 10 | (px >> 31) << 15;
            uint32_t m16 = (fbmsk >> 3 & 0x1F) | (fbmsk >> 11 & 0x1F) << 5 | (fbmsk >> 19 & 0x1F) << 10 | (fbmsk >> 31) << 15;
            uint32_t old = vram_read(p->fpsm, p->fbp, p->fbw, (uint32_t)x, (uint32_t)y);

            vram_write(p->fpsm, p->fbp, p->fbw, (uint32_t)x, (uint32_t)y, (old & m16) | (p16 & ~m16));
        } else {
            uint32_t old;

            if (p->fb24) {
                fbmsk |= 0xFF000000;
            }
            if (fbmsk == 0) {
                vram_write(PSMCT32, p->fbp, p->fbw, (uint32_t)x, (uint32_t)y, px);
            } else {
                old = vram_read(PSMCT32, p->fbp, p->fbw, (uint32_t)x, (uint32_t)y);
                vram_write(PSMCT32, p->fbp, p->fbw, (uint32_t)x, (uint32_t)y, (old & fbmsk) | (px & ~fbmsk));
            }
        }
    }
    if (zwrite && p->zte) {
        vram_write(p->zpsm, p->zbp, p->fbw, (uint32_t)x, (uint32_t)y, z);
    }
}

/* ---- primitives ---- */

static uint64_t prim_attr(void) {
    return BITS(gs.prmodecont, 0, 1) ? gs.prim : (gs.prim & 7) | (gs.prmode & ~7ULL);
}

static void vertex_uv(const GsVertex *v, uint64_t tex0, int fst, float *u, float *vv) {
    uint32_t tw = 1u << BITS(tex0, 26, 4), th = 1u << BITS(tex0, 30, 4);

    if (fst) {
        *u = v->u / 16.0f;
        *vv = v->v / 16.0f;
    } else {
        float q = v->q != 0.0f ? v->q : 1.0f;

        *u = v->s / q * (float)tw;
        *vv = v->t / q * (float)th;
    }
}

static void draw_sprite(const GsVertex *a, const GsVertex *b, uint64_t attr) {
    int ctxi = (int)BITS(attr, 9, 1);
    GsContext *c = &gs.ctx[ctxi];
    int ox = (int)BITS(c->xyoffset, 0, 16), oy = (int)BITS(c->xyoffset, 32, 16);
    int tme = (int)BITS(attr, 4, 1), abe = (int)BITS(attr, 6, 1), fst = (int)BITS(attr, 8, 1), fge = (int)BITS(attr, 5, 1);
    int x0 = (a->x - ox), y0 = (a->y - oy), x1 = (b->x - ox), y1 = (b->y - oy), x, y, xs, xe, ys, ye;
    float u0, v0, u1, v1, du, dv;
    const GsVertex *col = b;   /* sprites take the second vertex's colour */
    PixState ps;
    TexState ts;

    vertex_uv(a, c->tex0, fst, &u0, &v0);
    vertex_uv(b, c->tex0, fst, &u1, &v1);
    if (x0 > x1) {
        int t = x0; x0 = x1; x1 = t;
        float f = u0; u0 = u1; u1 = f;
    }
    if (y0 > y1) {
        int t = y0; y0 = y1; y1 = t;
        float f = v0; v0 = v1; v1 = f;
    }
    pix_setup(&ps, ctxi, abe, fge);
    if (tme) {
        clut_load(c->tex0);
        tex_setup(&ts, c->tex0, c->clamp);
    }
    /* pixel centres: covered if x0 <= px*16 + 8 < x1 (top-left rule); only the scissored ones */
    xs = (x0 + 15) >> 4;
    xe = (x1 + 15) >> 4;   /* exclusive: the first x with x*16 >= x1 */
    ys = (y0 + 15) >> 4;
    ye = (y1 + 15) >> 4;
    if (xs < ps.sx0) xs = ps.sx0;
    if (xe > ps.sx1 + 1) xe = ps.sx1 + 1;
    if (ys < ps.sy0) ys = ps.sy0;
    if (ye > ps.sy1 + 1) ye = ps.sy1 + 1;
    if (!tme && !abe && !fge && !ps.ate && !ps.date && ps.fbmsk == 0 && !ps.fb16 && !ps.fb24
        && (!ps.zte || ps.ztst == 1)) {
        /* a plain fill (e.g. a clear): colour (and Z) straight in */
        uint32_t px = (uint32_t)(int)col->r | (uint32_t)(int)col->g << 8 | (uint32_t)(int)col->b << 16
                      | (uint32_t)(((int)col->a | ps.fba << 7) & 0xFF) << 24;
        uint32_t z = b->z;
        int zw = ps.zte && ps.zwrite;

        if (zw && ps.zpsm == PSMZ24) {
            z = z > 0xFFFFFF ? 0xFFFFFF : z;
        } else if (zw && (ps.zpsm == PSMZ16 || ps.zpsm == PSMZ16S)) {
            z = z > 0xFFFF ? 0xFFFF : z;
        }
        if (!ps.colclamp) {
            px = (px & 0xFF000000) | (px & 0xFFFFFF);
        }
        for (y = ys; y < ye; y++) {
            for (x = xs; x < xe; x++) {
                vram_write(PSMCT32, ps.fbp, ps.fbw, (uint32_t)x, (uint32_t)y, px);
                if (zw) {
                    vram_write(ps.zpsm, ps.zbp, ps.fbw, (uint32_t)x, (uint32_t)y, z);
                }
            }
        }
        gs_stat_pixels += (unsigned)((xe > xs ? xe - xs : 0) * (ye > ys ? ye - ys : 0));
        return;
    }
    du = x1 != x0 ? (u1 - u0) / (float)(x1 - x0) : 0.0f;
    dv = y1 != y0 ? (v1 - v0) / (float)(y1 - y0) : 0.0f;
    {
        /* u in 16.16 fixed point, stepped across the row */
        int ustart = (int)floor((double)(u0 + du * (float)((xs << 4) - x0)) * 65536.0);
        int ustep = (int)((double)du * 16.0 * 65536.0);
        int cr = (int)col->r, cg = (int)col->g, cb = (int)col->b, ca = (int)col->a, fog = (int)b->fog;

        for (y = ys; y < ye; y++) {
            int tv = ifloor(v0 + dv * (float)((y << 4) - y0));
            int uf = ustart;

            for (x = xs; x < xe; x++, uf += ustep) {
                int r = cr, g = cg, bb = cb, al = ca;

                if (tme) {
                    shade(&ts, &r, &g, &bb, &al, tex_fetch(&ts, uf >> 16, tv));
                }
                draw_pixel(&ps, x, y, b->z, r, g, bb, al, fog);
            }
        }
    }
}

static void draw_triangle(const GsVertex *v0, const GsVertex *v1, const GsVertex *v2, uint64_t attr) {
    int ctxi = (int)BITS(attr, 9, 1);
    GsContext *c = &gs.ctx[ctxi];
    float ox = (float)BITS(c->xyoffset, 0, 16), oy = (float)BITS(c->xyoffset, 32, 16);
    int iip = (int)BITS(attr, 3, 1), tme = (int)BITS(attr, 4, 1), fge = (int)BITS(attr, 5, 1);
    int abe = (int)BITS(attr, 6, 1), fst = (int)BITS(attr, 8, 1);
    const GsVertex *vs[3] = {v0, v1, v2};
    float px[3], py[3], area, inv, minx, maxx, miny, maxy;
    float su[3], sv[3], sq[3];
    int i, x, y, xs, xe, ys, ye;
    uint32_t tw = 1u << BITS(c->tex0, 26, 4), th = 1u << BITS(c->tex0, 30, 4);
    PixState ps;
    TexState ts;

    for (i = 0; i < 3; i++) {
        px[i] = ((float)vs[i]->x - ox) / 16.0f;
        py[i] = ((float)vs[i]->y - oy) / 16.0f;
        if (fst) {
            su[i] = vs[i]->u / 16.0f;
            sv[i] = vs[i]->v / 16.0f;
            sq[i] = 1.0f;
        } else {
            su[i] = vs[i]->s * (float)tw;
            sv[i] = vs[i]->t * (float)th;
            sq[i] = vs[i]->q;
        }
    }
    area = (px[1] - px[0]) * (py[2] - py[0]) - (px[2] - px[0]) * (py[1] - py[0]);
    if (area == 0.0f) {
        return;
    }
    inv = 1.0f / area;
    pix_setup(&ps, ctxi, abe, fge);
    if (tme) {
        clut_load(c->tex0);
        tex_setup(&ts, c->tex0, c->clamp);
    }
    minx = fminf(px[0], fminf(px[1], px[2]));
    maxx = fmaxf(px[0], fmaxf(px[1], px[2]));
    miny = fminf(py[0], fminf(py[1], py[2]));
    maxy = fmaxf(py[0], fmaxf(py[1], py[2]));
    if (minx < 0) minx = 0;
    if (miny < 0) miny = 0;
    if (maxx > 2047) maxx = 2047;
    if (maxy > 2047) maxy = 2047;
    xs = (int)ceilf(minx);
    ys = (int)ceilf(miny);
    xe = (int)ceilf(maxx);   /* exclusive: x < maxx */
    ye = (int)ceilf(maxy);
    if (xs < ps.sx0) xs = ps.sx0;
    if (xe > ps.sx1 + 1) xe = ps.sx1 + 1;
    if (ys < ps.sy0) ys = ps.sy0;
    if (ye > ps.sy1 + 1) ye = ps.sy1 + 1;
    for (y = ys; y < ye; y++) {
        for (x = xs; x < xe; x++) {
            float w0 = ((px[1] - (float)x) * (py[2] - (float)y) - (px[2] - (float)x) * (py[1] - (float)y)) * inv;
            float w1 = ((px[2] - (float)x) * (py[0] - (float)y) - (px[0] - (float)x) * (py[2] - (float)y)) * inv;
            float w2 = 1.0f - w0 - w1;
            int r, g, b, a, fog;
            uint32_t z;

            if (w0 < 0 || w1 < 0 || w2 < 0) {
                continue;
            }
            if (iip) {
                r = (int)(w0 * v0->r + w1 * v1->r + w2 * v2->r);
                g = (int)(w0 * v0->g + w1 * v1->g + w2 * v2->g);
                b = (int)(w0 * v0->b + w1 * v1->b + w2 * v2->b);
                a = (int)(w0 * v0->a + w1 * v1->a + w2 * v2->a);
            } else {
                r = (int)v2->r; g = (int)v2->g; b = (int)v2->b; a = (int)v2->a;
            }
            fog = (int)(w0 * v0->fog + w1 * v1->fog + w2 * v2->fog);
            z = (uint32_t)(w0 * (float)v0->z + w1 * (float)v1->z + w2 * (float)v2->z);
            if (tme) {
                float q = w0 * sq[0] + w1 * sq[1] + w2 * sq[2];
                float u = (w0 * su[0] + w1 * su[1] + w2 * su[2]) / (q != 0 ? q : 1);
                float v = (w0 * sv[0] + w1 * sv[1] + w2 * sv[2]) / (q != 0 ? q : 1);

                shade(&ts, &r, &g, &b, &a, tex_fetch(&ts, ifloor(u), ifloor(v)));
            }
            draw_pixel(&ps, x, y, z, r, g, b, a, fog);
        }
    }
}

static void draw_line(const GsVertex *a, const GsVertex *b, uint64_t attr) {
    int ctxi = (int)BITS(attr, 9, 1);
    GsContext *c = &gs.ctx[ctxi];
    int ox = (int)BITS(c->xyoffset, 0, 16), oy = (int)BITS(c->xyoffset, 32, 16);
    int x0 = (a->x - ox) >> 4, y0 = (a->y - oy) >> 4, x1 = (b->x - ox) >> 4, y1 = (b->y - oy) >> 4;
    int n = abs_i(x1 - x0) > abs_i(y1 - y0) ? abs_i(x1 - x0) : abs_i(y1 - y0), i;
    PixState ps;

    pix_setup(&ps, ctxi, (int)BITS(attr, 6, 1), 0);
    for (i = 0; i <= n; i++) {
        float t = n ? (float)i / (float)n : 0.0f;

        draw_pixel(&ps, x0 + (int)((x1 - x0) * t), y0 + (int)((y1 - y0) * t), b->z, (int)b->r, (int)b->g, (int)b->b,
                   (int)b->a, 255);
    }
}

static void vertex_kick(int drawing) {
    uint64_t attr = prim_attr();

    static int debug = -1;

    if (debug < 0) {
        debug = getenv("HG_GSDEBUG") != NULL;
    }
    if (drawing && debug) {
        static uint64_t seen[64][2];
        static int nseen;
        uint64_t k0 = gs.prim & 0x7FF, k1 = BITS(gs.prim, 4, 1) ? gs.ctx[0].tex0 : 0;
        int i;

        for (i = 0; i < nseen && (seen[i][0] != k0 || seen[i][1] != k1); i++) {
        }
        if (i == nseen && nseen < 64) {
            seen[nseen][0] = k0;
            seen[nseen][1] = k1;
            nseen++;
            fprintf(stderr, "new prim %llx tex0 %llx frame %llx (kick %u)\n", (unsigned long long)k0,
                    (unsigned long long)k1, (unsigned long long)gs.ctx[0].frame, gs_stat_prims);
        }
    }
    gs_stat_prims += drawing;
    int type = (int)(gs.prim & 7);
    GsVertex *v = gs.v;

    switch (type) {
    case 0:   /* point */
        if (drawing) {
            draw_line(&v[0], &v[0], attr);
        }
        gs.nv = 0;
        break;
    case 1:   /* line */
        if (gs.nv == 2) {
            if (drawing) draw_line(&v[0], &v[1], attr);
            gs.nv = 0;
        }
        break;
    case 2:   /* line strip */
        if (gs.nv == 2) {
            if (drawing) draw_line(&v[0], &v[1], attr);
            v[0] = v[1];
            gs.nv = 1;
        }
        break;
    case 3:   /* triangle */
        if (gs.nv == 3) {
            if (drawing) draw_triangle(&v[0], &v[1], &v[2], attr);
            gs.nv = 0;
        }
        break;
    case 4:   /* triangle strip */
        if (gs.nv == 3) {
            if (drawing) draw_triangle(&v[0], &v[1], &v[2], attr);
            v[0] = v[1];
            v[1] = v[2];
            gs.nv = 2;
        }
        break;
    case 5:   /* triangle fan */
        if (gs.nv == 3) {
            if (drawing) draw_triangle(&v[0], &v[1], &v[2], attr);
            v[1] = v[2];
            gs.nv = 2;
        }
        break;
    case 6:   /* sprite */
        if (gs.nv == 2) {
            if (drawing) draw_sprite(&v[0], &v[1], attr);
            gs.nv = 0;
        }
        break;
    default:
        gs.nv = 0;
        break;
    }
}

static void push_vertex(uint64_t xyz, int hasfog, int kick) {
    GsVertex *v;

    if (gs.nv >= 3) {
        gs.nv = 0;
    }
    v = &gs.v[gs.nv++];
    v->x = (int)BITS(xyz, 0, 16);
    v->y = (int)BITS(xyz, 16, 16);
    v->z = hasfog ? BITS(xyz, 32, 24) : (uint32_t)(xyz >> 32);
    v->fog = hasfog ? (float)BITS(xyz, 56, 8) : (float)BITS(gs.fog, 56, 8);
    v->r = (float)BITS(gs.rgbaq, 0, 8);
    v->g = (float)BITS(gs.rgbaq, 8, 8);
    v->b = (float)BITS(gs.rgbaq, 16, 8);
    v->a = (float)BITS(gs.rgbaq, 24, 8);
    {
        uint32_t qb = (uint32_t)(gs.rgbaq >> 32), sb = (uint32_t)gs.st, tb = (uint32_t)(gs.st >> 32);

        memcpy(&v->q, &qb, 4);
        memcpy(&v->s, &sb, 4);
        memcpy(&v->t, &tb, 4);
    }
    v->u = (float)BITS(gs.uv, 0, 14);
    v->v = (float)BITS(gs.uv, 16, 14);
    vertex_kick(kick);
}

/* write GS register `reg` (A+D address) */
void gs_write_reg(uint32_t reg, uint64_t v) {
    switch (reg) {
    case 0x00:   /* PRIM */
        gs.prim = v;
        gs.nv = 0;
        break;
    case 0x01: gs.rgbaq = v; break;
    case 0x02: gs.st = v; break;
    case 0x03: gs.uv = v; break;
    case 0x04: push_vertex(v, 1, 1); break;   /* XYZF2 */
    case 0x05: push_vertex(v, 0, 1); break;   /* XYZ2 */
    case 0x06: case 0x07:                     /* TEX0_1 / _2 */
        gs.ctx[reg - 6].tex0 = v;
        clut_load(v);
        break;
    case 0x08: case 0x09: gs.ctx[reg - 8].clamp = v; break;
    case 0x0A: gs.fog = v; break;
    case 0x0C: push_vertex(v, 1, 0); break;   /* XYZF3 */
    case 0x0D: push_vertex(v, 0, 0); break;   /* XYZ3 */
    case 0x14: case 0x15: gs.ctx[reg - 0x14].tex1 = v; break;
    case 0x16: case 0x17:                     /* TEX2: TEX0 without the base */
        gs.ctx[reg - 0x16].tex0 = (gs.ctx[reg - 0x16].tex0 & 0x3FFFFFFFFULL & ~0x3F00000ULL) | (v & ~0x3FFFFFFFFULL) | (v & 0x3F00000);
        clut_load(gs.ctx[reg - 0x16].tex0);
        break;
    case 0x18: case 0x19: gs.ctx[reg - 0x18].xyoffset = v; break;
    case 0x1A: gs.prmodecont = v; break;
    case 0x1B: gs.prmode = v; break;
    case 0x1C: gs.texclut = v; break;
    case 0x22: gs.scanmsk = v; break;
    case 0x3B: gs.texa = v; break;
    case 0x3D: gs.fogcol = v; break;
    case 0x3F: break;                         /* TEXFLUSH */
    case 0x40: case 0x41: gs.ctx[reg - 0x40].scissor = v; break;
    case 0x42: case 0x43: gs.ctx[reg - 0x42].alpha = v; break;
    case 0x44: gs.dimx = v; break;
    case 0x45: gs.dthe = v; break;
    case 0x46: gs.colclamp = v; break;
    case 0x47: case 0x48: gs.ctx[reg - 0x47].test = v; break;
    case 0x49: gs.pabe = v; break;
    case 0x4A: case 0x4B: gs.ctx[reg - 0x4A].fba = v; break;
    case 0x4C: case 0x4D: gs.ctx[reg - 0x4C].frame = v; break;
    case 0x4E: case 0x4F: gs.ctx[reg - 0x4E].zbuf = v; break;
    case 0x50: gs.bitbltbuf = v; break;
    case 0x51: gs.trxpos = v; break;
    case 0x52: gs.trxreg = v; break;
    case 0x53: gs.trxdir = v; xfer_start(); break;
    case 0x54: {   /* HWREG: 64 bits of transfer data */
        uint32_t dpsm = BITS(gs.bitbltbuf, 56, 6);
        int bits = (dpsm == PSMCT32 || dpsm == PSMZ32) ? 32 : (dpsm == PSMT8 || dpsm == PSMT8H) ? 8
                 : (dpsm == PSMT4 || dpsm == PSMT4HL || dpsm == PSMT4HH) ? 4 : 16, n;

        for (n = 0; n < 64 / bits && gs.xfer; n++) {
            xfer_pixel((uint32_t)(v & ((1ULL << bits) - 1)));
            v >>= bits;
        }
        break;
    }
    default: break;
    }
}

/* ---- GIF ---- */

/* one PACKED register: descriptor d, the quadword (lo, hi) */
static void gif_packed(uint32_t d, uint64_t lo, uint64_t hi) {
    switch (d) {
    case 0x0: gs_write_reg(0x00, lo); break;
    case 0x1: {   /* RGBAQ: 32-bit fields; Q from the last ST */
        uint32_t qb;

        memcpy(&qb, &gs.q, 4);
        gs_write_reg(0x01, (lo & 0xFF) | ((lo >> 32) & 0xFF) << 8 | (hi & 0xFF) << 16 | ((hi >> 32) & 0xFF) << 24 |
                               (uint64_t)qb << 32);
        break;
    }
    case 0x2:     /* ST (Q kept for RGBAQ) */
        memcpy(&gs.q, (const uint8_t *)&hi, 4);
        gs_write_reg(0x02, lo);
        break;
    case 0x3: gs_write_reg(0x03, (lo & 0x3FFF) | ((lo >> 32) & 0x3FFF) << 16); break;
    case 0x4: case 0x5: {   /* XYZF2 / XYZ2 (ADC: no drawing kick) */
        uint64_t xy = (lo & 0xFFFF) | ((lo >> 32) & 0xFFFF) << 16;
        int adc = (int)BITS(hi, 47, 1);

        if (d == 0x4) {
            xy |= ((hi >> 4) & 0xFFFFFF) << 32 | ((hi >> 36) & 0xFF) << 56;
            gs_write_reg(adc ? 0x0C : 0x04, xy);
        } else {
            xy |= (hi & 0xFFFFFFFF) << 32;
            gs_write_reg(adc ? 0x0D : 0x05, xy);
        }
        break;
    }
    case 0xA: gs_write_reg(0x0A, ((hi >> 36) & 0xFF) << 56); break;   /* FOG */
    case 0xE: gs_write_reg((uint32_t)hi & 0xFF, lo); break;           /* A+D */
    case 0xF: break;                                                 /* NOP */
    default: gs_write_reg(d, lo); break;
    }
}

/* The GIF: a stream of tags and their data. Packets can be split across transfers (an IMAGE
 * tag in one VIF1 DIRECT, its data in the next), so the current tag is kept between calls. */
static struct {
    int active;
    uint64_t regs;
    uint32_t nloop, nreg, reg, flg;
} gif;

uint32_t gs_gif(const uint64_t *qw, uint32_t n) {
    uint32_t i;

    for (i = 0; i < n; i++) {
        uint64_t lo = qw[i * 2], hi = qw[i * 2 + 1];
        int half;

        if (!gif.active) {
            gif.nloop = BITS(lo, 0, 15);
            gif.flg = BITS(lo, 58, 2);
            gif.nreg = BITS(lo, 60, 4) ? BITS(lo, 60, 4) : 16;
            gif.regs = hi;
            gif.reg = 0;
            if (BITS(lo, 46, 1) && gif.flg == 0) {   /* PRE: PRIM from the tag */
                gs_write_reg(0x00, BITS(lo, 47, 11));
            }
            gif.active = gif.nloop != 0;
            continue;
        }
        switch (gif.flg) {
        case 0:   /* PACKED */
            gif_packed((uint32_t)(gif.regs >> (gif.reg * 4)) & 0xF, lo, hi);
            if (++gif.reg == gif.nreg) {
                gif.reg = 0;
                gif.active = --gif.nloop != 0;
            }
            break;
        case 1:   /* REGLIST: two registers per quadword */
            for (half = 0; half < 2 && gif.active; half++) {
                uint32_t d = (uint32_t)(gif.regs >> (gif.reg * 4)) & 0xF;

                if (d != 0xE && d != 0xF) {
                    gs_write_reg(d, half ? hi : lo);
                }
                if (++gif.reg == gif.nreg) {
                    gif.reg = 0;
                    gif.active = --gif.nloop != 0;
                }
            }
            break;
        default:  /* IMAGE */
            xfer_qword(&qw[i * 2]);
            gif.active = --gif.nloop != 0;
            break;
        }
    }
    return n;
}

/* ---- privileged registers / display ---- */

void gs_set_display(uint64_t pmode, uint64_t smode2, uint64_t dispfb, uint64_t display, uint64_t bgcolor) {
    gs.pmode = pmode;
    gs.smode2 = smode2;
    gs.dispfb[1] = dispfb;
    gs.display[1] = display;
    gs.dispfb[0] = dispfb;
    gs.display[0] = display;
    gs.bgcolor = bgcolor;
}

/* RGBA8 of the displayed frame; returns its size */
void gs_display(uint32_t *out, int maxw, int maxh, int *w, int *h) {
    int c = BITS(gs.pmode, 1, 1) ? 1 : 0;
    uint64_t fb = gs.dispfb[c], disp = gs.display[c];
    uint32_t fbp = BITS(fb, 0, 9) * 32, fbw = BITS(fb, 9, 6), psm = BITS(fb, 15, 5);
    uint32_t dbx = BITS(fb, 32, 11), dby = BITS(fb, 43, 11);
    int dw = (int)(BITS(disp, 32, 12) + 1) / (int)(BITS(disp, 23, 4) + 1), dh = (int)BITS(disp, 44, 11) + 1, x, y;

    if (getenv("HG_GSDEBUG")) {
        static uint64_t last[3];

        if (last[0] != gs.pmode || last[1] != fb || last[2] != disp) {   /* when the mode changes */
            last[0] = gs.pmode;
            last[1] = fb;
            last[2] = disp;
            fprintf(stderr, "gs_display: pmode %llx dispfb %llx display %llx -> %dx%d\n",
                    (unsigned long long)gs.pmode, (unsigned long long)fb, (unsigned long long)disp, dw, dh);
        }
    }
    if (dw > maxw) dw = maxw;
    if (dh > maxh) dh = maxh;
    for (y = 0; y < dh; y++) {
        for (x = 0; x < dw; x++) {
            uint32_t p;

            if (psm == PSMCT16 || psm == PSMCT16S) {
                p = expand16(vram_read(psm, fbp, fbw, dbx + (uint32_t)x, dby + (uint32_t)y), 0, 1);
            } else {
                p = vram_read(PSMCT32, fbp, fbw, dbx + (uint32_t)x, dby + (uint32_t)y);
            }
            out[y * maxw + x] = p | 0xFF000000;
        }
    }
    *w = dw;
    *h = dh;
}

/* ---- self test: the 8-bit layout seen through 32-bit writes (the classic "unswizzle8") ---- */

int gs_selftest(void) {
    uint32_t x, y, bad = 0, width = 128;

    gs_init_tables();
    memset(vram32, 0, sizeof(vram32));
    /* write bytes as PSMT8 128x64, read them back through the 32-bit layout */
    for (y = 0; y < 64; y++) {
        for (x = 0; x < 128; x++) {
            vram_write(PSMT8, 0, 2, x, y, (x * 7 + y * 13) & 0xFF);
        }
    }
    for (y = 0; y < 64; y++) {
        for (x = 0; x < 128; x++) {
            uint32_t block = (y & ~0xFu) * width + (x & ~0xFu) * 2;
            uint32_t swap = (((y + 2) >> 2) & 1) * 4;
            uint32_t posY = (((y & ~3u) >> 1) + (y & 1)) & 7;
            uint32_t column = posY * width * 2 + ((x + swap) & 7) * 4;
            uint32_t byte = ((y >> 1) & 1) + ((x >> 2) & 2);
            uint32_t s = block + column + byte;   /* byte offset in a linear 32-bit 64x32 image */
            uint32_t wx = (s / 4) % 64, wy = (s / 4) / 64;
            uint32_t got = (vram_read(PSMCT32, 0, 1, wx, wy) >> ((s & 3) * 8)) & 0xFF;

            if (got != ((x * 7 + y * 13) & 0xFF)) {
                bad++;
            }
        }
    }
    memset(vram32, 0, sizeof(vram32));
    return (int)bad;
}

void gs_init(void) {
    gs_init_tables();
    memset(&gs, 0, sizeof(gs));
}
