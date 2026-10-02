/* The renderer (system +0x460, vtable 0x46AC50, global D_0044E4F0). It builds PS2 DMA / GIF
 * packets in double-buffered arenas inside itself; the PC build interprets those packets where
 * the PS2 would send them (libdma / libgraph), so this code stays as the game had it. */
#include "common.h"
#include "game.h"

extern void *func_00115D20(void *p, s32 c, u32 n);   /* memset */


/* +0x1C */
void func_001BBB20(u8 *r) {
    AT(r, 0x304DE0, u8) = 1;
}

/* +0x18 allocate `n` quadwords from the current buffer of arena 2 (+0x2806D0 + buffer * 0x80000);
 * NULL when full */
void *func_001BBB40(u8 *r, s32 n) {
    u8 *cur = AT(r, 0x304BAC, u8 *);
    u8 *next = cur + n * 16;

    if ((u8 *)r + 0x2806D0 + (AT(r, 0x304BB5, u8) << 19) < next) {
        return NULL;
    }
    AT(r, 0x304BAC, u8 *) = next;
    return cur;
}

/* +0x14 allocate `n` quadwords from the current buffer of arena 1 (+0x1006C0 + buffer * 0x100000),
 * keeping one quadword spare; NULL when full */
void *func_001BBBB0(u8 *r, s32 n) {
    u8 *cur = AT(r, 0x304BA8, u8 *);
    u8 *next = cur + n * 16;

    if (!(next + 0x10 < (u8 *)r + 0x1006C0 + (AT(r, 0x304BB4, u8) << 20))) {
        return NULL;
    }
    AT(r, 0x304BA8, u8 *) = next;
    return cur;
}

/* +0x3C id of render layer `i` (0..10), -1 if out of range */
s32 func_001BB950(u8 *r, u32 i) {
    if (i < 11) {
        return AT(r, 0x304BB8 + i * 4, s32);
    }
    return -1;
}

extern void func_0010BFB0(void);                            /* libgraph: sceGsResetPath */
extern void func_0010BE10(s32 inter, s32 mode, s32 ntsc, s32 ffmd);   /* libgraph: sceGsResetGraph */
extern void func_001B7ED0(u8 *r);
extern void func_001B79F0(u8 *r);
extern void func_001B7370(u8 *r);
extern void func_001B71E0(u8 *r);
extern VObject *D_0044E550;   /* random number generator */

#define RNG_REAL1() VCALL(D_0044E550, 0x18, f32 (*)(VObject *))(D_0044E550)

/* Set up the graphics for video mode `mode` (2: 448 lines), then a table of 16 random
 * (x, y, ..., angle) entries (renderer +0x304C0C). */
void func_001B83D0(u8 *r, s32 mode) {
    f32 *e;

    func_0010BFB0();
    func_0010BE10(0, 1, (u8)mode, 0);
    func_001B7ED0(r);
    AT(r, 0x304BFE, s16) = (u8)mode == 2 ? 448 : 512;
    AT(r, 0x304C09, u8) = mode;
    func_001B79F0(r);
    func_001B7370(r);
    for (e = (f32 *)(r + 0x304C0C); e <= (f32 *)(r + 0x304D38); e += 5) {
        e[0] = -16.0f + 80.0f * (RNG_REAL1() - 0.5f);
        e[1] = -150.0f * RNG_REAL1();
        e[2] = 32.0f + 32.0f * RNG_REAL1();
        e[3] = 32.0f + 32.0f * RNG_REAL1();
        e[4] = 0x1.921fb6p+1f /* pi */ * (360.0f * (RNG_REAL1() - 0.5f)) / 180.0f;
    }
}

extern VObject *D_0044E9A0;   /* VRAM allocator (system +0x30CF40) */

/* Allocate the renderer's VRAM (allocator +0x18: address, pixel format 0x13 = 8-bit indexed,
 * width, height): one 0xFF area (+0x304BE4) and 11 layers (+0x304BB8: 10 of 256x256 below
 * 0xFC000, one of 512x512 at 0xC0000), each kept resident (+0x24). */
void func_001B8250(u8 *r) {
    VObject *v;
    u32 i;

    if (AT(r, 0x304BE4, s32) < 0) {
        v = D_0044E9A0;
        AT(r, 0x304BE4, s32) = VCALL(v, 0x14, s32 (*)(VObject *, s32, s32, s32, s32))(v, 0xFF, 0, 0, 0);
        VCALL(v, 0x24, void (*)(VObject *, s32))(v, AT(r, 0x304BE4, s32));
    }
    v = D_0044E9A0;
    for (i = 0; i < 11; i++) {
        s32 *id = &AT(r, 0x304BB8 + i * 4, s32);

        if (*id < 0) {
            if (i < 10) {
                *id = VCALL(v, 0x18, s32 (*)(VObject *, s32, s32, s32, s32, s32))(v, 0xFC000 - ((i + 2) << 14), 0x13, 0x100, 0x100, 0);
            } else {
                *id = VCALL(v, 0x18, s32 (*)(VObject *, s32, s32, s32, s32, s32))(v, 0xC0000, 0x13, 0x200, 0x200, 0);
            }
            VCALL(v, 0x24, void (*)(VObject *, s32))(v, *id);
        }
    }
}

#include "gs.h"

extern void FlushCache(s32 mode);
extern u32 *_fbss;   /* GIF DMA channel registers (sceDmaGetChan(2)) */
extern void func_0010D6E8(u32 *chan, void *tag);           /* libdma: sceDmaSend */
extern s32 func_0010D988(u32 *chan, s32 mode, s32 timeout);   /* libdma: sceDmaSync */

/* Clear all of VRAM: 16 uploads of a 256x256 32-bit block of zeros (256 KB each). */
void func_001B7ED0(u8 *r) {
    s32 i, base, n;
    u64 *p;

    for (i = 0, base = 0; i < 16; i++, base += 0x10000) {
        func_001B7370(r);
        p = VCALL(r, 0x10, u64 *(*)(u8 *, s32, s32))(r, 0x4008, 1);
        p[0] = DMA_TAG(DMA_CNT, 5, 0);
        p[1] = 0;
        p[2] = GIF_TAG(4, 1, GIF_PACKED, 1);
        p[3] = GIF_REG_AD;
        p[4] = GS_BITBLT_DST(base / 64, 4, GS_PSMCT32);
        p[5] = GS_BITBLTBUF;
        p[6] = 0;
        p[7] = GS_TRXPOS;
        p[8] = 256 | (u64)256 << 32;
        p[9] = GS_TRXREG;
        p[10] = 0;   /* host -> local */
        p[11] = GS_TRXDIR;
        p[12] = DMA_TAG(DMA_CNT, 0x4001, 0);
        p[13] = 0;
        p[14] = GIF_TAG(0x4000, 1, GIF_IMAGE, 0);
        p[15] = 0;
        for (p += 16, n = 0; n < 0x4000; n += 8, p += 16) {   /* the image: zeros */
            p[0] = 0; p[1] = 0; p[2] = 0; p[3] = 0; p[4] = 0; p[5] = 0; p[6] = 0; p[7] = 0;
            p[8] = 0; p[9] = 0; p[10] = 0; p[11] = 0; p[12] = 0; p[13] = 0; p[14] = 0; p[15] = 0;
        }
        FlushCache(0);
        *_fbss &= ~0x40;   /* CHCR.TTE: don't send the tags */
        func_0010D6E8(_fbss, r + AT(r, 0x304BB4, u8) * 0x360 + 0x10);
        func_0010D988(_fbss, 0, 0);
    }
}

extern void sceGsDefDispEnv(void *disp, s32 psm, s32 w, s32 h, s32 dx, s32 dy);
extern s32 func_0010C5C8(void *env, s32 psm, s32 w, s32 h, s32 ztest, s32 zpsm);   /* sceGsSetDefDrawEnv */
extern s32 func_0010D020(void *env, s32 psm, s32 w, s32 h, s32 ztest, s32 zpsm);   /* ... context 2 */
extern s32 func_0010C7B0(void *clear, s32 ztest, s32 x, s32 y, s32 w, s32 h, s32 r, s32 g, s32 b,
                         s32 a, u32 z);                                            /* sceGsSetDefClear */

/* set the 9-bit base field (FBP / ZBP) of a GS register to VRAM byte address `addr` */
#define GS_SET_BASE(reg, addr) ((reg) = ((reg) & ~0x1FF) | (((addr) / 2048) & 0x1FF))

/* Display and drawing environments: two setups (+0x3006D0.. and +0x3006F8..), each a display
 * environment and a GIF packet of both drawing contexts plus a clear; then the frame / Z buffer
 * bases from the VRAM layout (+0x304BE8 display, +0x304BEC draw, +0x304BF0 Z). */
void func_001B79F0(u8 *r) {
    s32 dw = AT(r, 0x304C00, s16), dh = AT(r, 0x304C02, s16);
    s32 w = AT(r, 0x304BFC, s16), h = AT(r, 0x304BFE, s16);

    /* setup 0: draw at dw x dh (24-bit Z test), display w x h */
    sceGsDefDispEnv(r + 0x3006D0, 0, w, h, 0, AT(r, 0x304C09, u8) == 0x50 ? 0xF : 0);
    AT(r, 0x300720, u64) = GIF_TAG(0x16, 1, GIF_PACKED, 1);
    AT(r, 0x300728, u64) = GIF_REG_AD;
    func_0010C5C8(r + 0x300730, 0, dw, dh, 2, 0x31);
    func_0010D020(r + 0x3007B0, 0, dw, dh, 2, 0x31);
    func_0010C7B0(r + 0x300830, 2, (s16)(0x800 - (dw >> 1)), (s16)(0x800 - (dh >> 1)), dw, dh, 0, 0, 0, 0, 0);
    /* setup 1: 24-bit frame, w x h, no Z test */
    sceGsDefDispEnv(r + 0x3006F8, 1, w, h, 0, 0);
    AT(r, 0x300890, u64) = GIF_TAG(0x16, 1, GIF_PACKED, 1);
    AT(r, 0x300898, u64) = GIF_REG_AD;
    func_0010C5C8(r + 0x3008A0, 1, w, h, 0, 0x31);
    func_0010D020(r + 0x300920, 1, w, h, 0, 0x31);
    func_0010C7B0(r + 0x3009A0, 0, (s16)(0x800 - (w >> 1)), (s16)(0x800 - (h >> 1)), w, h, 0, 0, 0, 0, 0);

    GS_SET_BASE(AT(r, 0x3006E0, u32), AT(r, 0x304BE8, s32));      /* setup 0 DISPFB */
    GS_SET_BASE(AT(r, 0x3007B0, u64), AT(r, 0x304BEC, s32));      /* context 2 FRAME */
    AT(r, 0x300730, u64) = (AT(r, 0x300730, u64) & ~0x1FF) | (AT(r, 0x3007B0, u64) & 0x1FF);
    GS_SET_BASE(AT(r, 0x3007C0, u64), AT(r, 0x304BF0, s32));      /* context 2 ZBUF */
    AT(r, 0x300740, u64) = (AT(r, 0x300740, u64) & ~0x1FF) | (AT(r, 0x3007C0, u64) & 0x1FF);
    GS_SET_BASE(AT(r, 0x300708, u32), AT(r, 0x304BE8, s32));      /* setup 1 DISPFB */
    GS_SET_BASE(AT(r, 0x300920, u64), AT(r, 0x304BE8, s32));
    AT(r, 0x3008A0, u64) = (AT(r, 0x3008A0, u64) & ~0x1FF) | (AT(r, 0x300920, u64) & 0x1FF);
    GS_SET_BASE(AT(r, 0x300930, u64), AT(r, 0x304BF0, s32));
    AT(r, 0x3008B0, u64) = (AT(r, 0x3008B0, u64) & ~0x1FF) | (AT(r, 0x300930, u64) & 0x1FF);
}

/* Default GS state at the start of each frame, through VIF1 DIRECT: normal alpha blending,
 * TEXA alpha 0x80, bilinear-ish TEX1, clamped UVs, the drawing area, Z buffer (24-bit at page
 * 0xA0), alpha test "not equal 0" + Z test "greater or equal", frame buffer at page 0x110 (512 wide). */
void func_001B71E0(u8 *r) {
    u64 *p = VCALL(r, 0x10, u64 *(*)(u8 *, s32, s32))(r, 0xD, 0);
    s32 dw = AT(r, 0x304C00, s16), dh = AT(r, 0x304C02, s16);

    p[0] = DMA_TAG(DMA_CNT, 12, 0);
    ((u32 *)p)[2] = VIF_NOP;
    ((u32 *)p)[3] = VIF_DIRECT(12);
    p[2] = GIF_TAG(11, 1, GIF_PACKED, 1);
    p[3] = GIF_REG_AD;
    p[4] = 0x44;                    /* ALPHA: (Cs - Cd) * As + Cd */
    p[5] = GS_ALPHA_1;
    p[6] = 0;
    p[7] = GS_FBA_1;
    p[8] = 0;
    p[9] = GS_PABE;
    p[10] = (u64)0x80 << 32;        /* TEXA: TA1 = 0x80 */
    p[11] = GS_TEXA;
    p[12] = 0x60;
    p[13] = GS_TEX1_1;
    p[14] = 5;                      /* CLAMP: clamp both */
    p[15] = GS_CLAMP_1;
    p[16] = (u64)((0x800 - (dw >> 1)) * 16) | (u64)((0x800 - (dh >> 1)) * 16) << 32;
    p[17] = GS_XYOFFSET_1;
    p[18] = (u64)(dw - 1) << 16 | (u64)(dh - 1) << 48;
    p[19] = GS_SCISSOR_1;
    p[20] = 0x310000A0;             /* ZBUF: page 0xA0, PSMZ24 */
    p[21] = GS_ZBUF_1;
    p[22] = 0x5000F;                /* TEST */
    p[23] = GS_TEST_1;
    p[24] = 0x80110;                /* FRAME: page 0x110, width 512, PSMCT32 */
    p[25] = GS_FRAME_1;
}

/* the draw buffer (0 / 1) and its DMA chain: 53 layer slots, then an end tag */
#define REND_BUF(r) AT(r, 0x304BB4, u8)
#define REND_CHAIN(r, b) ((r) + (b) * 0x360 + 0x10)

/* Flip to the other draw buffer: rebuild its chain of 53 layer slots (each a DMA "next" tag to
 * the following slot; layers append their packets after their slot), reset its packet arena;
 * the second arena flips too when requested (+0x304DE0); then the frame-start state. The DMA
 * address field is 28 bits: on PC the build is 32-bit and its data lies below 0x10000000. */
void func_001B7370(u8 *r) {
    s32 i;

    REND_BUF(r) ^= 1;
    for (i = 0; i < 53; i++) {
        u64 *tag = (u64 *)(REND_CHAIN(r, REND_BUF(r)) + i * 16);

        tag[0] = DMA_TAG(DMA_NEXT, 0, (u32)(REND_CHAIN(r, REND_BUF(r)) + (i + 1) * 16) & 0x0FFFFFFF);
        tag[1] = 0;
        AT(r, 0x304A00 + REND_BUF(r) * 0xD4 + i * 4, u8 *) = REND_CHAIN(r, REND_BUF(r)) + i * 16;
    }
    AT(REND_CHAIN(r, REND_BUF(r)), 53 * 16, u64) = DMA_TAG(DMA_END, 0, 0);
    AT(REND_CHAIN(r, REND_BUF(r)), 53 * 16 + 8, u64) = 0;
    AT(r, 0x304BA8, u8 *) = r + (REND_BUF(r) << 20) + 0x6D0;
    if (AT(r, 0x304DE0, u8)) {
        AT(r, 0x304BB5, u8) ^= 1;
        AT(r, 0x304BAC, u8 *) = r + (AT(r, 0x304BB5, u8) << 19) + 0x2006D0;
        func_00115D20(r + 0x300A00, 0, 0x4000);
        AT(r, 0x304DE0, u8) = 0;
    }
    func_001B71E0(r);
    AT(r, 0x304BB6, u8) = 0;
    AT(r, 0x304C05, u8) = 0;
    for (i = 0; i < 16; i++) {
        AT(r, 0x304D5C + i * 8, s32) = -1;
        AT(r, 0x304D60 + i * 8, s32) = 0;
    }
}

extern s32 func_001B4F30(u8 *r);   /* layer 6 setup (u8: 0 = skip the packet) */
extern s32 func_001B1E50(u8 *r);   /* layer 38 setup (u8) */

#define REND_LAYER_TAIL(r, l) AT(r, 0x304A00 + REND_BUF(r) * 0xD4 + (l) * 4, u64 *)

/* +0x10 add a packet of `n` quadwords to layer `layer` (0..52) of this frame: allocated from
 * arena 1, linked after the layer's last packet, followed by a tag back to the next layer.
 * The first packet of layer 6 / 38 runs that layer's setup first. NULL if it can't be added. */
u64 *func_001BBC20(u8 *r, s32 n, s32 layer) {
    u64 *p, *back;

    if (n <= 0 || layer >= 53) {
        return NULL;
    }
    if (layer == 6) {
        if (REND_LAYER_TAIL(r, 6) == (u64 *)(REND_CHAIN(r, REND_BUF(r)) + 6 * 16) && !(u8)func_001B4F30(r)) {
            return NULL;
        }
    } else if (layer == 38) {
        if (REND_LAYER_TAIL(r, 38) == (u64 *)(REND_CHAIN(r, REND_BUF(r)) + 38 * 16) && !(u8)func_001B1E50(r)) {
            return NULL;
        }
    }
    p = VCALL(r, 0x14, u64 *(*)(u8 *, s32))(r, n);
    if (p == NULL) {
        return NULL;
    }
    REND_LAYER_TAIL(r, layer)[0] = DMA_TAG(DMA_NEXT, 0, (u32)p & 0x0FFFFFFF);
    REND_LAYER_TAIL(r, layer)[1] = 0;
    back = AT(r, 0x304BA8, u64 *);
    back[0] = DMA_TAG(DMA_NEXT, 0, (u32)(REND_CHAIN(r, REND_BUF(r)) + (layer + 1) * 16) & 0x0FFFFFFF);
    AT(r, 0x304BA8, u64 *)[1] = 0;
    REND_LAYER_TAIL(r, layer) = AT(r, 0x304BA8, u64 *);
    AT(r, 0x304BA8, u8 *) += 16;
    return p;
}

extern s32 func_001B4330(u8 *r);
extern s32 func_001B2160(u8 *r);
extern s32 func_001B18E0(u8 *r);
extern s32 func_001B0D40(u8 *r);
extern s32 func_001AF3B0(u8 *r);
extern s32 func_001AC0D0(u8 *r);
extern s32 func_001AB960(u8 *r);
extern s32 func_001AB3F0(u8 *r);

/* Make layer `layer`'s state packet: for these layers, the first draw of a frame runs a setup
 * function that fills the layer before (layer - 1). If the setup fails, the arena is rolled
 * back and the layers before and after are skipped (their slots chain straight on); 0 is
 * returned. Layers 15 and 26 run their own checks. Other layers need nothing. */
static s32 layer_begin(u8 *r, s32 layer, s32 (*setup)(u8 *r), u8 *saved) {
    u8 *chain;

    chain = REND_CHAIN(r, REND_BUF(r));
    if (REND_LAYER_TAIL(r, layer - 1) != (u64 *)(chain + (layer - 1) * 16) || (u8)setup(r)) {
        return 1;
    }
    AT(r, 0x304BA8, u8 *) = saved;
    AT(REND_CHAIN(r, REND_BUF(r)), (layer - 1) * 16, u64) =
        DMA_TAG(DMA_NEXT, 0, (u32)(REND_CHAIN(r, REND_BUF(r)) + layer * 16) & 0x0FFFFFFF);
    AT(REND_CHAIN(r, REND_BUF(r)), (layer - 1) * 16 + 8, u64) = 0;
    REND_LAYER_TAIL(r, layer - 1) = (u64 *)(REND_CHAIN(r, REND_BUF(r)) + (layer - 1) * 16);
    AT(REND_CHAIN(r, REND_BUF(r)), (layer + 1) * 16, u64) =
        DMA_TAG(DMA_NEXT, 0, (u32)(REND_CHAIN(r, REND_BUF(r)) + (layer + 2) * 16) & 0x0FFFFFFF);
    AT(REND_CHAIN(r, REND_BUF(r)), (layer + 1) * 16 + 8, u64) = 0;
    REND_LAYER_TAIL(r, layer + 1) = (u64 *)(REND_CHAIN(r, REND_BUF(r)) + (layer + 1) * 16);
    return 0;
}

s32 func_001B5EC0(u8 *r, s32 layer) {
    u8 *saved = AT(r, 0x304BA8, u8 *);

    switch (layer) {
    case 0x1A: return (u8)func_001AB3F0(r) ? 1 : 0;
    case 0x1C: return layer_begin(r, layer, func_001AB960, saved);
    case 0x23: return layer_begin(r, layer, func_001AC0D0, saved);
    case 0x14: return layer_begin(r, layer, func_001AF3B0, saved);
    case 0x17: return layer_begin(r, layer, func_001B0D40, saved);
    case 0x0F: return (u8)func_001B18E0(r) ? 1 : 0;
    case 0x26: return layer_begin(r, layer, func_001B1E50, saved);
    case 0x11: return layer_begin(r, layer, func_001B2160, saved);
    case 0x0D: return layer_begin(r, layer, func_001B4330, saved);
    case 0x06: return layer_begin(r, layer, func_001B4F30, saved);
    }
    return 1;
}

extern void func_001B6CD0(u8 *r, u64 *packet, void *arg);
extern s32 func_001B1370(u8 *r);
extern s32 func_001AAE80(u8 *r);

/* +0xC draw `obj` into layer `layer` (0..52): its +0xC method writes its packets at the arena
 * cursor, which are then linked into the layer (layer 10 with `arg` goes through
 * func_001B6CD0 instead). 0 if the layer can't be drawn or the object drew nothing; layers 15
 * and 26 finish with their own step. */
s32 func_001BBE60(u8 *r, void *obj, s32 layer, void *arg) {
    u64 *start;

    if (obj == NULL || layer >= 53 || !(u8)func_001B5EC0(r, layer)) {
        return 0;
    }
    start = AT(r, 0x304BA8, u64 *);
    if (!(u8)VCALL(obj, 0xC, s32 (*)(void *))(obj)) {
        AT(r, 0x304BA8, u64 *) = start;
        return 0;
    }
    if (layer == 10 && arg != NULL) {
        func_001B6CD0(r, start, arg);
    } else {
        u64 *back;

        REND_LAYER_TAIL(r, layer)[0] = DMA_TAG(DMA_NEXT, 0, (u32)start & 0x0FFFFFFF);
        REND_LAYER_TAIL(r, layer)[1] = 0;
        back = AT(r, 0x304BA8, u64 *);
        back[0] = DMA_TAG(DMA_NEXT, 0, (u32)(REND_CHAIN(r, REND_BUF(r)) + (layer + 1) * 16) & 0x0FFFFFFF);
        AT(r, 0x304BA8, u64 *)[1] = 0;
        REND_LAYER_TAIL(r, layer) = AT(r, 0x304BA8, u64 *);
        AT(r, 0x304BA8, u8 *) += 16;
    }
    if (layer == 0x1A) {
        return (u8)func_001AAE80(r) != 0;
    }
    if (layer == 0x0F) {
        return (u8)func_001B1370(r) != 0;
    }
    return 1;
}

/* +0x4C upload a 4-bit image (w x h, its 16-colour CLUT right after the pixels) to VRAM block
 * 0x3400 (CLUT to 0x3F00), in layer `layer`. The transfer is w * h / 16 quadwords (twice the
 * pixels: the rest is ignored by the GS once the area is full). 0 if there's no room. */
s32 func_001BB010(u8 *r, u8 *img, s32 w, s32 h, s32 layer) {
    s32 n = w * h;
    s32 qwc = n >> 4;
    u64 *p = VCALL(r, 0x10, u64 *(*)(u8 *, s32, s32))(r, 0x13, layer);

    if (p == NULL) {
        return 0;
    }
    p[0] = DMA_TAG(DMA_CNT, 6, 0);
    AT(p, 0x8, u32) = 0;
    AT(p, 0xC, u32) = 0x50000006;
    p[2] = 0x8004 | (1ULL << 60);
    p[3] = 0xE;
    p[4] = ((u64)(s64)(w >> 6) << 48) | (0x14003400ULL << 32);   /* PSMT4 at 0x3400 */
    p[5] = GS_BITBLTBUF;
    p[6] = 0;
    p[7] = GS_TRXPOS;
    p[8] = (u64)(s64)w | ((u64)(s64)h << 32);
    p[9] = GS_TRXREG;
    p[10] = 0;
    p[11] = GS_TRXDIR;
    p[12] = (u64)(s64)qwc | 0x8000 | (0x08ULL << 56);
    p[13] = 0;
    p[14] = (u64)(u32)(qwc | 0x30000000) | ((u64)((u32)img & 0x0FFFFFFF) << 32);
    AT(p, 0x78, u32) = 0;
    AT(p, 0x7C, u32) = qwc | 0x50000000;
    /* the CLUT: 8 x 2 PSMCT32 */
    p[16] = DMA_TAG(DMA_CNT, 6, 0);
    AT(p, 0x88, u32) = 0;
    AT(p, 0x8C, u32) = 0x50000006;
    p[18] = 0x8004 | (1ULL << 60);
    p[19] = 0xE;
    p[20] = 0x13F00ULL << 32;
    p[21] = GS_BITBLTBUF;
    p[22] = 0;
    p[23] = GS_TRXPOS;
    p[24] = 8 | (2ULL << 32);
    p[25] = GS_TRXREG;
    p[26] = 0;
    p[27] = GS_TRXDIR;
    p[28] = 0x8004 | (0x08ULL << 56);
    p[29] = 0;
    p[30] = 0x30000004 | ((u64)((u32)(img + (n >> 1)) & 0x0FFFFFFF) << 32);
    AT(p, 0xF8, u32) = 0;
    AT(p, 0xFC, u32) = 0x50000004;
    p[32] = DMA_TAG(DMA_CNT, 2, 0);
    AT(p, 0x108, u32) = 0;
    AT(p, 0x10C, u32) = 0x50000002;
    p[34] = 0x8001 | (1ULL << 60);
    p[35] = 0xE;
    p[36] = 0;
    p[37] = GS_TEXFLUSH;
    return 1;
}

/* +0x48 upload an 8-bit image (w x h, its 256-colour CLUT right after the pixels) to VRAM at
 * byte address `addr` (CLUT to block 0x3F00), in layer `layer`. 0 if there's no room. */
s32 func_001BB230(u8 *r, u8 *img, s32 w, s32 h, s32 addr, s32 layer) {
    s32 n = w * h;
    s32 qwc = n >> 4;
    u64 *p = VCALL(r, 0x10, u64 *(*)(u8 *, s32, s32))(r, 0x13, layer);

    if (p == NULL) {
        return 0;
    }
    /* the pixels */
    p[0] = DMA_TAG(DMA_CNT, 6, 0);
    AT(p, 0x8, u32) = 0;
    AT(p, 0xC, u32) = 0x50000006;   /* VIF DIRECT 6 */
    p[2] = 0x8004 | (1ULL << 60);   /* GIF tag: 4 A+D, EOP */
    p[3] = 0xE;
    p[4] = ((u64)(s64)(addr >> 6) << 32) | ((u64)(s64)(w >> 6) << 48) | (0x13ULL << 56);   /* PSMT8 */
    p[5] = GS_BITBLTBUF;
    p[6] = 0;
    p[7] = GS_TRXPOS;
    p[8] = (u64)(s64)w | ((u64)(s64)h << 32);
    p[9] = GS_TRXREG;
    p[10] = 0;
    p[11] = GS_TRXDIR;
    p[12] = (u64)(s64)qwc | 0x8000 | (0x08ULL << 56);   /* GIF tag: image, EOP */
    p[13] = 0;
    p[14] = (u64)(u32)(qwc | 0x30000000) | ((u64)((u32)img & 0x0FFFFFFF) << 32);   /* DMA ref */
    AT(p, 0x78, u32) = 0;
    AT(p, 0x7C, u32) = qwc | 0x50000000;
    /* the CLUT: 16 x 16 PSMCT32 */
    p[16] = DMA_TAG(DMA_CNT, 6, 0);
    AT(p, 0x88, u32) = 0;
    AT(p, 0x8C, u32) = 0x50000006;
    p[18] = 0x8004 | (1ULL << 60);
    p[19] = 0xE;
    p[20] = 0x13F00ULL << 32;
    p[21] = GS_BITBLTBUF;
    p[22] = 0;
    p[23] = GS_TRXPOS;
    p[24] = 0x10 | (0x10ULL << 32);
    p[25] = GS_TRXREG;
    p[26] = 0;
    p[27] = GS_TRXDIR;
    p[28] = 0x8040 | (0x08ULL << 56);
    p[29] = 0;
    p[30] = 0x30000040 | ((u64)((u32)(img + n) & 0x0FFFFFFF) << 32);
    AT(p, 0xF8, u32) = 0;
    AT(p, 0xFC, u32) = 0x50000040;
    /* then flush the texture cache */
    p[32] = DMA_TAG(DMA_CNT, 2, 0);
    AT(p, 0x108, u32) = 0;
    AT(p, 0x10C, u32) = 0x50000002;
    p[34] = 0x8001 | (1ULL << 60);
    p[35] = 0xE;
    p[36] = 0;
    p[37] = GS_TEXFLUSH;
    return 1;
}

/* +0x28 video mode (2: NTSC 448 lines) */
u8 func_001BB9D0(u8 *r) { return AT(r, 0x304C09, u8); }

/* +0x2C display settings (+0x1F / +0x20: screen offset) */
u8 *func_001BB9C0(u8 *r) { return r + 0x304BE8; }

/* +0x30 set the screen offset */
void func_001BB9A0(u8 *r, s32 x, s32 y) {
    AT(r, 0x304C07, u8) = x;
    AT(r, 0x304C08, u8) = y;
}

/* +0x34 */
void func_001BB990(u8 *r, s32 v) { AT(r, 0x304BF8, s32) = v; }

/* +0x38 the renderer's own VRAM entry */
s32 func_001BB980(u8 *r) { return AT(r, 0x304BE4, s32); }

extern void func_0010D200(void *db, s32 field);   /* sceGsSwapDBuff */
extern void sceGsSyncPath(s32 mode, s32 timeout);
extern u64 D_0047D300[];   /* the frame's final packet: draw buffer -> display buffer */

#define REND_VIF1_CHAN(r) AT(r, 0x304BB0, u32 *)

/* wait for the previous frame's chain (VIF1 DMA) */
void func_001B87D0(u8 *r) {
    func_0010D988(REND_VIF1_CHAN(r), 0, 0);
}

/* show buffer 1's environment and send the final copy packet */
void func_001B8750(u8 *r) {
    FlushCache(0);
    func_0010D200(r + 0x3006D0, 1);
    FlushCache(0);
    *_fbss &= ~0x40;
    func_0010D6E8(_fbss, D_0047D300);
}

/* after the GIF finished: clear colour (+0x304BF8), clear on (XYZ2) or off (XYZ3, +0x304C04),
 * then buffer 0's environment */
void func_001B86A0(u8 *r) {
    func_0010D988(_fbss, 0, 0);
    sceGsSyncPath(0, 0);
    AT(r, 0x300850, s32) = AT(r, 0x304BF8, s32);
    AT(r, 0x300878, u64) = AT(r, 0x304C04, u8) == 0 ? GS_XYZ2 : 0xD;
    FlushCache(0);
    func_0010D200(r + 0x3006D0, 0);
}

extern void func_001B75E0(u8 *r);

/* send this frame's layer chain over VIF1, set up the next frame's final packet, flip */
void func_001B85B0(u8 *r) {
    FlushCache(0);
    *REND_VIF1_CHAN(r) = (*REND_VIF1_CHAN(r) & ~0x40) | 0x40;   /* CHCR.TTE: VIF codes in the tags */
    func_0010D6E8(REND_VIF1_CHAN(r), REND_CHAIN(r, REND_BUF(r)));
    AT(r, 0x3009C0, s32) = AT(r, 0x304BF8, s32);
    AT(r, 0x3009E8, u64) = AT(r, 0x304C05, u8) == 1 ? 0xD : GS_XYZ2;
    func_001B75E0(r);
    func_001B7370(r);
}

/* The final packet: copy the draw buffer (32-bit, +0x304BEC) onto the display buffer as one
 * sprite at the screen offset, blended with the previous frame by FIX alpha +0x304C06 when
 * +0x304C05 is set (ABE). */
void func_001B75E0(u8 *r) {
    u64 *p = D_0047D300;
    s32 dw = AT(r, 0x304C00, s16), dh = AT(r, 0x304C02, s16);
    s32 w = AT(r, 0x304BFC, s16), h = AT(r, 0x304BFE, s16);
    s32 base = AT(r, 0x304BEC, s32);
    s32 ox = AT(r, 0x304C07, s8), oy = AT(r, 0x304C08, s8);
    s32 u = 0, v = 0;

    p[0] = DMA_TAG(DMA_CNT, 11, 0);
    ((u32 *)p)[2] = VIF_NOP;
    ((u32 *)p)[3] = VIF_DIRECT(11);
    p[2] = GIF_TAG(5, 1, GIF_PACKED, 1);
    p[3] = GIF_REG_AD;
    p[4] = 0x310000A0 | (u64)1 << 32;   /* ZBUF: no Z writes */
    p[5] = GS_ZBUF_1;
    p[6] = 0x30000;                     /* TEST: Z always */
    p[7] = GS_TEST_1;
    p[8] = 0;
    p[9] = GS_TEXFLUSH;
    p[10] = 0x60;
    p[11] = GS_TEX1_1;
    p[12] = (u64)AT(r, 0x304C06, u8) << 32 | 0x64;   /* ALPHA: (Cs - Cd) * FIX + Cd */
    p[13] = GS_ALPHA_1;
    p[14] = GIF_TAG(1, 1, GIF_REGLIST, 8);
    p[15] = GIF_REGS(GIF_CLAMP_1, GIF_TEX0_1, GIF_PRIM, GIF_RGBAQ, GIF_UV, GIF_XYZ2, GIF_UV, GIF_XYZ2);
    p[16] = (u64)(s64)(dw - 1) << 14 | 0xA | (u64)(s64)(dh - 1) << 34;   /* region clamp */
    if (base == 0x50000) {
        p[17] = (u64)(s64)(base >> 6) | 0xA8040000 | (u64)6 << 32;
    } else if ((u32)dw <= 0x200 && (u32)dh <= 0x1C0) {
        p[17] = (u64)(s64)(base >> 6) | (u64)(s64)(dw >> 6) << 14 | 0x64000000 | (u64)6 << 32;
    } else {
        p[17] = (u64)(s64)(base >> 6) | (u64)(s64)(dw >> 6) << 14 | 0xA9300000 | (u64)0x2007E006 << 32;
    }
    p[18] = (u64)AT(r, 0x304C05, u8) << 6 | 0x116;   /* PRIM: sprite, textured, UV */
    p[19] = AT(r, 0x304BF4, u32);
    if (w < dw) {
        u = (dw - w) * 8;
    }
    if (h < dh) {
        v = (dh - h) * 8;
    }
    p[20] = (u64)(s64)(u + 8) | (u64)(s64)(v + 8) << 16;
    p[21] = (u64)(((0x800 - (w >> 1)) + ox) * 16) | (u64)(((0x800 - (h >> 1)) + oy) * 16) << 16;
    p[22] = (u64)(s64)(dw * 16 - u + 8) | (u64)(s64)(dh * 16 - v + 8) << 16;
    p[24] = DMA_TAG(DMA_END, 0, 0);
    p[25] = 0;
    p[23] = (u64)(((w >> 1) + 0x800 + ox) * 16) | (u64)(((h >> 1) + 0x800 + oy) * 16) << 16;
}

/* a texture's entry in a .TEX file */
typedef struct TexHeader {
    /* 0x0 */ u8 psm;
    /* 0x1 */ u8 cpsm;      /* CLUT format */
    /* 0x2 */ u8 pad2[2];
    /* 0x4 */ u16 w;
    /* 0x6 */ u16 h;
    /* 0x8 */ u16 imageQwc;
    /* 0xA */ u16 clutQwc;
    /* 0xC */ s32 data;     /* image then CLUT, from this entry */
} TexHeader;

#define VRAM_TEX_ADDR(v, id) VCALL(v, 0x68, u32 (*)(VObject *, s32))(v, id)    /* in 64-word blocks */
#define VRAM_CLUT_ADDR(v, id) VCALL(v, 0x6C, u32 (*)(VObject *, s32))(v, id)

/* one upload: the tags (written first) */
static inline void Rend_UploadTags(u64 *p) {
    p[0] = DMA_TAG(DMA_CNT, 6, 0);
    ((u32 *)p)[2] = VIF_NOP;
    ((u32 *)p)[3] = VIF_DIRECT(6);
    p[2] = GIF_TAG(4, 1, GIF_PACKED, 1);
    p[3] = GIF_REG_AD;
}

/* ... then BITBLTBUF / TRXPOS / TRXREG / TRXDIR, the IMAGE tag and a DMA ref to the data */
static inline void Rend_UploadRegs(u64 *p, u64 bitblt, u32 w, u32 h, u32 qwc, u8 *data) {
    p[4] = bitblt;
    p[5] = GS_BITBLTBUF;
    p[6] = 0;
    p[7] = GS_TRXPOS;
    p[8] = w | (u64)h << 32;
    p[9] = GS_TRXREG;
    p[10] = 0;
    p[11] = GS_TRXDIR;
    p[12] = GIF_TAG(qwc, 1, GIF_IMAGE, 0);
    p[13] = 0;
    p[14] = DMA_TAG(DMA_REF, qwc, (u32)data & 0x0FFFFFFF);
    ((u32 *)p)[30] = VIF_NOP;
    ((u32 *)p)[31] = VIF_DIRECT(qwc);
}

/* +0x44 upload texture `t` (CLUT, image) to its VRAM entry `id` (bit 31 ignored), on layer
 * `layer` (-1: right away, unlinked). 0 if there's no room. */
s32 func_001BB470(u8 *r, s32 id, TexHeader *t, s32 layer) {
    u64 *p;
    VObject *v;
    u8 *image, *clut;
    u32 vid;

    if (layer == -1) {
        p = VCALL(r, 0x14, u64 *(*)(u8 *, s32))(r, 0x13);
    } else {
        p = VCALL(r, 0x10, u64 *(*)(u8 *, s32, s32))(r, 0x13, layer);
    }
    if (p == NULL) {
        return 0;
    }
    v = D_0044E9A0;
    vid = (u32)id & 0x7FFFFFFF;
    image = (u8 *)t + t->data;
    clut = image + t->imageQwc * 16;
    Rend_UploadTags(p);
    Rend_UploadRegs(p, (u64)t->cpsm << 56 | (u64)VRAM_CLUT_ADDR(v, vid) << 32 | (u64)1 << 48, 16, 16, t->clutQwc, clut);
    Rend_UploadTags(p + 16);
    Rend_UploadRegs(p + 16, (u64)t->psm << 56 | (u64)(s64)((t->w + 63) >> 6) << 48 | (u64)VRAM_TEX_ADDR(v, vid) << 32,
                    t->w, t->h, t->imageQwc, image);
    p[32] = DMA_TAG(DMA_CNT, 2, 0);
    ((u32 *)p)[66] = VIF_NOP;
    ((u32 *)p)[67] = VIF_DIRECT(2);
    p[34] = GIF_TAG(1, 1, GIF_PACKED, 1);
    p[35] = GIF_REG_AD;
    p[36] = 0;
    p[37] = GS_TEXFLUSH;
    return 1;
}

#include "ptmf.h"

extern VObject *D_0044E9A0;   /* the VRAM manager */
extern PTMF D_0047E300[];     /* palette generators by mode: (this, index, arg) -> RGBA */

/* +0x94: build a 256-colour palette with generator `mode` and send it to VRAM slot `slot`'s
 * CLUT (in renderer layer `layer`, -1 the immediate list); colours go as HWREG writes, two per
 * quadword, in the CLUT's entry order (bit 3 and 4 of the index swapped) */
s32 func_001B8D30(VObject *r, s32 slot, s32 mode, s32 layer, s32 arg) {
    u64 *p;
    const PTMF *gen;
    u32 i, k, s;
    static const s8 sStep[4] = {8, -16, 8, 0};

    if (layer == -1) {
        p = VCALL(r, 0x14, u64 *(*)(VObject *, s32))(r, 0x86);
    } else {
        p = VCALL(r, 0x10, u64 *(*)(VObject *, s32, s32))(r, 0x86, layer);
    }
    if (p == NULL) {
        return 0;
    }
    p[0] = 0x10000085;              /* DMA cnt 0x85 */
    AT(p, 0x8, u32) = 0;
    AT(p, 0xC, u32) = 0x50000085;   /* VIF DIRECT 0x85 */
    p[2] = 0x8084 | (0x10000000ULL << 32);   /* GIF tag: 0x84 A+D, EOP */
    p[3] = 0xE;
    p[4] = ((u64)(u32)VCALL(D_0044E9A0, 0x6C, s32 (*)(VObject *, s32))(D_0044E9A0, slot) << 32) | (0x10000ULL << 32);
    p[5] = GS_BITBLTBUF;            /* to the CLUT's place, width 64 */
    p[6] = 0x10 | (0x10ULL << 32);
    p[7] = GS_TRXREG;               /* 16 x 16 */
    p[8] = 0;
    p[9] = GS_TRXPOS;
    p[10] = 0;
    p[11] = GS_TRXDIR;
    p += 12;
    gen = &D_0047E300[mode];
    i = 0;
    do {
        for (s = 0; s < 4; s++) {
            for (k = 0; k < 8; k += 2) {
                AT(p, 0x0, u32) = ptmf_scall_r2(r, gen, i, arg);
                AT(p, 0x4, u32) = ptmf_scall_r2(r, gen, i + 1, arg);
                p[1] = 0x54;        /* HWREG */
                i += 2;
                p += 2;
            }
            i += sStep[s];
        }
    } while (i < 0x100);
    return 1;
}

/* palette generator 0: grey levels squeezed to 0x7E..0x81 around the middle (a nearly flat
 * ramp, the index clamped to 0x7E..0x80, plus one), in all four channels */
u32 func_001B8CE0(VObject *r, u32 i) {
    u32 v;

    if (i > 0x80) {
        v = 0x81;
    } else if (i < 0x7E) {
        v = 0x7E;
    } else {
        v = i + 1;
    }
    return v | v << 8 | v << 16 | v << 24;
}

/* palette generator 1: the same, the index clamped to 0x7F..0x81, minus one */
u32 func_001B8C90(VObject *r, u32 i) {
    u32 v;

    if (i < 0x7F) {
        v = 0x7E;
    } else if (i >= 0x82) {
        v = 0x81;
    } else {
        v = i - 1;
    }
    return v | v << 8 | v << 16 | v << 24;
}

/* the 3D layers' start (layer 0x25): clear the frame's alpha (a sprite over the screen writing only
 * alpha 0, in strips of 64 pixels), then Z test on, alpha test (frame alpha marks what's
 * drawn), the full scissor, bilinear textures, FBA on; at layer 0x27 the frame's mask and FBA
 * are put back */
s32 func_001B1E50(u8 *rp) {
    VObject *r = (VObject *)rp;
    static const u64 sStrips[8] = {
        0x01BF0000003F0000ULL, 0x01BF0000007F0040ULL, 0x01BF000000BF0080ULL, 0x01BF000000FF00C0ULL,
        0x01BF0000013F0100ULL, 0x01BF0000017F0140ULL, 0x01BF000001BF0180ULL, 0x01BF000001FF01C0ULL,
    };
    u64 *p;
    s32 i;

    p = VCALL(r, 0x10, u64 *(*)(VObject *, s32, s32))(r, 0x25, 0x25);

    if (p == NULL) {
        return 0;
    }
    p[0] = 0x10000024;
    AT(p, 0x8, u32) = 0;
    AT(p, 0xC, u32) = 0x50000024;   /* DIRECT 0x24 */
    p[2] = 0x8023 | (0x10000000ULL << 32);
    p[3] = 0xE;
    p[4] = 0x310000A0 | (1ULL << 32);   /* ZBUF_1: Z24 at 0xA0, no Z writes */
    p[5] = GS_ZBUF_1;
    p[6] = 0x30000;                     /* TEST_1: Z always */
    p[7] = GS_TEST_1;
    p[8] = 0x80110 | (0xFFFFFFULL << 32);   /* FRAME_1: 512 wide at 0x110, only alpha written */
    p[9] = GS_FRAME_1;
    p[10] = 0;
    p[11] = GS_TEX1_1;
    p[12] = (u64)0x3F800000 << 32;      /* RGBAQ: 0, Q 1 */
    p[13] = GS_RGBAQ;
    p[14] = 6;                          /* PRIM: sprite */
    p[15] = GS_PRIM;
    for (i = 0; i < 8; i++) {
        p[16 + i * 6] = sStrips[i];     /* SCISSOR_1: 64 pixels wide */
        p[17 + i * 6] = GS_SCISSOR_1;
        p[18 + i * 6] = 0x72007000;     /* (0, 0) */
        p[19 + i * 6] = GS_XYZ2;
        p[20 + i * 6] = 0x8E009000;     /* (512, 448) */
        p[21 + i * 6] = GS_XYZ2;
    }
    p[64] = 0x310000A0;                 /* ZBUF_1: Z writes on */
    p[65] = GS_ZBUF_1;
    p[66] = 0x5000F;                    /* TEST_1: alpha test, Z test GEQUAL */
    p[67] = GS_TEST_1;
    p[68] = 0x01BF000001FF0000ULL;      /* SCISSOR_1: the screen */
    p[69] = GS_SCISSOR_1;
    p[70] = 0x60;                       /* TEX1_1: bilinear */
    p[71] = GS_TEX1_1;
    p[72] = 1;                          /* FBA_1 */
    p[73] = GS_FBA_1;
    p = VCALL(r, 0x10, u64 *(*)(VObject *, s32, s32))(r, 0x4, 0x27);
    if (p == NULL) {
        return 0;
    }
    p[0] = 0x10000003;
    AT(p, 0x8, u32) = 0;
    AT(p, 0xC, u32) = 0x50000003;
    p[2] = 0x8002 | (0x10000000ULL << 32);
    p[3] = 0xE;
    p[4] = 0x80110;                     /* FRAME_1: all channels */
    p[5] = GS_FRAME_1;
    p[6] = 0;                           /* FBA_1 off */
    p[7] = GS_FBA_1;
    return 1;
}

/* +0x80 draw a box described by 13 words (+0x7C with them as arguments) */
void func_001B9810(VObject *r, const s32 *b) {
    VCALL(r, 0x7C, void (*)(VObject *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32))(
        r, b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7], b[8], b[9], b[10], b[11], b[12]);
}

extern VObject *D_0044E4E8;   /* the texture cache */
extern VObject *D_0044E4F0;   /* the renderer (this one) */

#define SX32(x) ((s64)(s32)(u32)(x))

/* +0x7C a sprite: the w x h rectangle at x, y (screen pixels), coloured `rgba` (alpha over 0x80:
 * opaque), textured with the tw x th texels at u, v of texture `tex` of group `group` (-1:
 * untextured; `clut` -1: its own palette, else CLUT `clut`), in renderer layer `layer` */
s32 func_001B9880(VObject *r, s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 tw, s32 th, u32 rgba,
                  s32 tex, s32 group, s32 layer, s32 clut) {
    s32 textured = tex != -1, n, opaque;
    u64 tex0 = 0, tex2 = 0;
    u64 *p;

    if (textured) {
        VObject *tc = D_0044E4E8, *vram;
        s32 slot = VCALL(tc, 0x8, s32 (*)(VObject *, s32, s32))(tc, tex, group);
        u8 *hdr;

        if (slot == -1) {
            return 0;
        }
        hdr = VCALL(tc, 0xC, u8 *(*)(VObject *, s32, s32))(tc, tex, group);
        if (slot & 0x80000000) {
            slot &= 0x7FFFFFFF;
            if (!(u8)VCALL(D_0044E4F0, 0x44, s32 (*)(VObject *, s32, void *, s32))(D_0044E4F0, slot, hdr, layer)) {
                return 0;
            }
        }
        vram = D_0044E9A0;
        if (clut == -1) {
            tex0 = VCALL(vram, 0x28, u64 (*)(VObject *, s32, s32, s32, s32, s32))(
                vram, slot, hdr[0], AT(hdr, 4, u16), AT(hdr, 6, u16), hdr[1]);
        } else {
            tex0 = VCALL(vram, 0x2C, u64 (*)(VObject *, s32, s32, s32, s32))(
                vram, slot, AT(hdr, 4, u16), AT(hdr, 6, u16), hdr[1]);
            tex2 = VCALL(vram, 0x34, u64 (*)(VObject *, s32, s32, s32, s32))(vram, slot, clut, hdr[0], hdr[1]);
        }
    }
    n = clut != -1 ? 5 : 4;
    p = VCALL(D_0044E4F0, 0x10, u64 *(*)(VObject *, s32, s32))(D_0044E4F0, n + 9, layer);
    if (p == NULL) {
        return 0;
    }
    p[0] = (u32)((n + 8) | 0x10000000);
    AT(p, 0x8, u32) = 0;
    AT(p, 0xC, u32) = (n + 8) | 0x50000000;   /* DIRECT */
    p[2] = (u64)(s64)n | 0x8000 | (0x10000000ULL << 32);
    p[3] = 0xE;
    p[4] = 0x310000A0 | (1ULL << 32);          /* ZBUF_1: no Z writes */
    p[5] = GS_ZBUF_1;
    p[6] = tex0;
    p[7] = GS_TEX0_1;
    p += 8;
    if (clut != -1) {
        p[0] = tex2;
        p[1] = 0x16;                           /* TEX2_1 */
        p += 2;
    }
    p[0] = 0x44;                               /* ALPHA_1: (Cs - Cd) * As + Cd */
    p[1] = GS_ALPHA_1;
    /* PRIM: sprite, UV (textured), blended; an alpha over 0x80 means opaque (alpha 0x80) */
    opaque = rgba >= 0x81000000;
    p[2] = ((u64)(s64)textured << 4) | (opaque ? 0x106 : 0x146);
    p[7] = opaque ? (rgba & 0xFFFFFF) | 0x80000000 : rgba;
    p[3] = GS_PRIM;
    p[4] = 0x8001 | (0x84ULL << 56);          /* reglist: CLAMP RGBAQ UV XYZ3 UV XYZ2 CLAMP NOP */
    p[5] = 0xFFFFFFFFF853D318ULL;
    /* (32-bit arithmetic, sign-extended, as the original) */
    p[6] = 0xA | ((u64)SX32(u) << 4) | ((u64)SX32((u32)u + tw - 1) << 14) | ((u64)SX32(v) << 24)
           | ((u64)SX32((u32)v + th - 1) << 34);
    p[8] = (u64)SX32((u32)u * 16 + 8) | ((u64)SX32((u32)v * 16 + 8) << 16);
    p[9] = gs_xyz2(x + 0x700, y + 0x720);
    p[10] = (u64)SX32(((u32)u + tw) * 16 + 8) | ((u64)SX32(((u32)v + th) * 16 + 8) << 16);
    p[11] = gs_xyz2(x + 0x700 + w, y + 0x720 + h);
    p[12] = 5;                                 /* CLAMP_1: clamp */
    p[13] = 0;
    p[14] = 0x8001 | (0x10000000ULL << 32);
    p[15] = 0xE;
    p[16] = 0x310000A0;                        /* ZBUF_1: Z writes on */
    p[17] = GS_ZBUF_1;
    return 1;
}
