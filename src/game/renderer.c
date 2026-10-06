/* The renderer (system +0x460, vtable 0x46AC50, global D_0044E4F0). It builds PS2 DMA / GIF
 * packets in double-buffered arenas inside itself; the PC build interprets those packets where
 * the PS2 would send them (libdma / libgraph), so this code stays as the game had it. */
#include "common.h"
#include "gl2d.h"
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

#ifdef HG_NATIVE
    {
        extern void glr_end_frame(void);   /* native/platform/glr.c */

        glr_end_frame();
    }
#endif
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
#ifdef HG_NATIVE
    {
        extern void glr_layer(s32 layer);   /* native/platform/glr.c: what glr is sent is in `layer` */
        s32 ok;

        glr_layer(layer);
        ok = (u8)VCALL(obj, 0xC, s32 (*)(void *))(obj);
        glr_layer(-1);
        if (!ok) {
            AT(r, 0x304BA8, u64 *) = start;
            return 0;
        }
    }
#else
    if (!(u8)VCALL(obj, 0xC, s32 (*)(void *))(obj)) {
        AT(r, 0x304BA8, u64 *) = start;
        return 0;
    }
#endif
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

#ifdef HG_NATIVE
#include <stdlib.h>

/* Raw images sent to VRAM (+0x48 / +0x4C) that sprites then show: kept decoded to RGBA behind a
 * .TEX-style header (PSMCT32, w x h) so the GL renderer takes them like any texture; by their
 * VRAM block. The header's spare bytes count the uploads (the renderer re-reads it on change). */
typedef struct Gl2dImage {
    u32 block;
    const u8 *src;
    u32 sum;
    u8 *buf;
} Gl2dImage;

static Gl2dImage sImages[4];

static u32 image_sum(const u8 *p, s32 n) {
    u32 c = 2166136261u;
    s32 i;

    for (i = 0; i < n; i += 61) {
        c = (c ^ p[i]) * 16777619u;
    }
    return c;
}

/* the image at VRAM block `block` (NULL: none sent) */
const u8 *gl2d_image(u32 block) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (sImages[i].buf != NULL && sImages[i].block == block) {
            return sImages[i].buf;
        }
    }
    return NULL;
}

/* keep `img` (w x h indexed pixels, `bits` 4 or 8, its CLUT of 32-bit colours right after them
 * in the GS's upload order) as the image at `block` */
static void image_put(u32 block, const u8 *img, s32 w, s32 h, s32 bits) {
    s32 n = w * h, ncol = bits == 4 ? 16 : 256, i, slot = -1;
    const u8 *clut = img + (bits == 4 ? n >> 1 : n);
    u32 sum = image_sum(img, n * bits / 8 + ncol * 4), pal[256];
    Gl2dImage *e;
    u32 *px;

    for (i = 0; i < 4 && slot < 0; i++) {
        if (sImages[i].block == block && sImages[i].buf != NULL) {
            slot = i;
        }
    }
    for (i = 0; i < 4 && slot < 0; i++) {
        if (sImages[i].buf == NULL) {
            slot = i;
        }
    }
    if (slot < 0) {
        slot = 0;
    }
    e = &sImages[slot];
    if (e->buf != NULL && e->block == block && e->src == img && e->sum == sum &&
        AT(e->buf, 4, u16) == w && AT(e->buf, 6, u16) == h) {
        return;   /* (sent again unchanged, as the title does every frame) */
    }
    if (e->buf == NULL || AT(e->buf, 4, u16) * AT(e->buf, 6, u16) < n) {
        free(e->buf);
        e->buf = malloc(16 + (u32)n * 4);
        AT(e->buf, 2, u16) = 0;
    }
    for (i = 0; i < ncol; i++) {   /* 256 colours: the CLUT's CSM1 layout (8..15 / 16..23 swapped) */
        s32 k = bits == 4 ? i : (i % 8 + ((i / 16) % 2) * 8) + ((i / 32) * 2 + (i / 8) % 2) * 16;

        pal[i] = AT(clut, k * 4, u32);
    }
    e->block = block;
    e->src = img;
    e->sum = sum;
    e->buf[0] = 0;   /* PSMCT32 */
    e->buf[1] = 0;
    AT(e->buf, 2, u16) += 1;
    AT(e->buf, 4, u16) = w;
    AT(e->buf, 6, u16) = h;
    AT(e->buf, 8, u16) = (u16)((u32)n * 4 / 16);
    AT(e->buf, 10, u16) = 0;
    AT(e->buf, 12, s32) = 16;
    px = (u32 *)(e->buf + 16);
    for (i = 0; i < n; i++) {
        px[i] = bits == 4 ? pal[(img[i >> 1] >> ((i & 1) * 4)) & 0xF] : pal[img[i]];
    }
}

/* +0x4C a 4-bit image (w x h, its 16-colour CLUT right after the pixels) for VRAM block
 * 0x3400 */
s32 func_001BB010(u8 *r, u8 *img, s32 w, s32 h, s32 layer) {
    image_put(0x3400, img, w, h, 4);
    return 1;
}

/* +0x48 an 8-bit image (w x h, its 256-colour CLUT right after the pixels) for VRAM at byte
 * address `addr` */
s32 func_001BB230(u8 *r, u8 *img, s32 w, s32 h, s32 addr, s32 layer) {
    image_put((u32)addr >> 6, img, w, h, 8);
    return 1;
}
#endif

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

#ifdef HG_NATIVE
/* +0x44 upload texture `t` to its VRAM entry `id`: nothing to do on PC (the GL renderer reads
 * .TEX entries where they are loaded) */
s32 func_001BB470(u8 *r, s32 id, TexHeader *t, s32 layer) {
    return 1;
}
#endif

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

#ifdef HG_NATIVE
/* the texture-cache entry of texture `tex` of group `group` for a 2D draw (NULL: not loaded;
 * the cache keeps it resident) */
static TexHeader *tex2d_entry(s32 tex, s32 group) {
    VObject *tc = D_0044E4E8;

    if (VCALL(tc, 0x8, s32 (*)(VObject *, s32, s32))(tc, tex, group) == -1) {
        return NULL;
    }
    return VCALL(tc, 0xC, TexHeader *(*)(VObject *, s32, s32))(tc, tex, group);
}

/* a sprite colour: an alpha over 0x80 is opaque (drawn at 0x80, unblended), else blended */
static inline u32 sprite_prim(u32 *rgba) {
    if (*rgba >= 0x81000000) {
        *rgba = (*rgba & 0xFFFFFF) | 0x80000000;
        return 0;
    }
    return 0x40;
}

/* +0x7C a sprite: the w x h rectangle at x, y (screen pixels), coloured `rgba` (alpha over 0x80:
 * opaque), textured with the tw x th texels at u, v of texture `tex` of group `group` (-1:
 * untextured; `clut` -1: its own palette, else palette `clut` of its CLUT), in renderer layer
 * `layer`. 0 when the texture isn't loaded. */
s32 func_001B9880(VObject *r, s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 tw, s32 th, u32 rgba,
                  s32 tex, s32 group, s32 layer, s32 clut) {
    TexHeader *t = NULL;
    u32 prim = sprite_prim(&rgba);

    if (tex != -1 && (t = tex2d_entry(tex, group)) == NULL) {
        return 0;
    }
    gl2d_sprite(layer, x, y, x + w, y + h, (u8 *)t, u, v, u + tw, v + th, rgba, clut == -1 ? 0 : clut, prim);
    return 1;
}

/* +0x84 a quad with free corners (x0, y0) .. (x3, y3) (strip order: the texels' top left, top
 * right, bottom left, bottom right), otherwise as +0x7C */
s32 func_001B92F0(VObject *r, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, s32 u, s32 v,
                  s32 tw, s32 th, u32 rgba, s32 tex, s32 group, s32 layer, s32 clut) {
    TexHeader *t = NULL;
    u32 prim = sprite_prim(&rgba);
    f32 xy[8] = {x0, y0, x1, y1, x2, y2, x3, y3}, st[8];
    u8 c[16];

    if (tex != -1 && (t = tex2d_entry(tex, group)) == NULL) {
        return 0;
    }
    if (t != NULL) {
        st[0] = (f32)u / t->w;        st[1] = (f32)v / t->h;
        st[2] = (f32)(u + tw) / t->w; st[3] = (f32)v / t->h;
        st[4] = (f32)u / t->w;        st[5] = (f32)(v + th) / t->h;
        st[6] = (f32)(u + tw) / t->w; st[7] = (f32)(v + th) / t->h;
    }
    gl2d_colors(c, rgba, 4);
    glr_prim2d(layer, GLR_2D_STRIP, 4, xy, st, c, t, clut == -1 ? 0 : clut, prim);
    return 1;
}
#endif

#include "progress.h"
extern Progress *gProgress;

/* +0x5C clear the 128 x 112 work buffer at frame page 0x1F0 to black (layer 0x29) and set
 * the drawing environment back; not when progress flag 0x28 is set. 0: no packet space */
s32 func_001BA090(u8 *r) {
    static const u64 sRegs[14][2] = {
        {0x00000001310000A0ULL, GS_ZBUF_1},
        {0x0000000000030000ULL, GS_TEST_1},
        {0x00000000000201F0ULL, GS_FRAME_1},      /* page 0x1F0, 128 wide */
        {0x00007C8000007C00ULL, GS_XYOFFSET_1},
        {0x006F0000007F0000ULL, GS_SCISSOR_1},    /* 0..127 x 0..111 */
        {0x3F80000000000000ULL, GS_RGBAQ},        /* black, q 1 */
        {6, GS_PRIM},                             /* sprite */
        {0x000000007C807C00ULL, GS_XYZ2},
        {0x0000000083808400ULL, GS_XYZ2},
        {0x0000000000080110ULL, GS_FRAME_1},      /* back to the frame buffer */
        {0x0000720000007000ULL, GS_XYOFFSET_1},
        {0x01BF000001FF0000ULL, GS_SCISSOR_1},
        {0x00000000310000A0ULL, GS_ZBUF_1},
        {0x000000000005000FULL, GS_TEST_1},
    };
    u64 *p;
    s32 i;

    if ((u8)Progress_TestFlag(gProgress, 0x28) == 1) {
        return 1;
    }
    p = VCALL(r, 0x10, u64 *(*)(u8 *, s32, s32))(r, 0x10, 0x29);
    if (p == NULL) {
        return 0;
    }
    AT(r, 0x304BB6, u8) = 1;
#ifdef HG_NATIVE
    {
        extern void glr_glow_clear(void);   /* native/platform/glr.c */

        glr_glow_clear();
    }
#endif
    p[0] = DMA_TAG(DMA_CNT, 15, 0);
    ((u32 *)p)[2] = VIF_NOP;
    ((u32 *)p)[3] = VIF_DIRECT(15);
    p[2] = GIF_TAG(14, 1, GIF_PACKED, 1);
    p[3] = GIF_REG_AD;
    for (i = 0; i < 14; i++) {
        p[4 + i * 2] = sRegs[i][0];
        p[5 + i * 2] = sRegs[i][1];
    }
    return 1;
}


#ifdef HG_NATIVE
/* +0x58 the glow, once a frame (not after +0x5C's clear; layer 0x29): the 128 x 112 work
 * buffer (page 0x1F0, which glow sprites - func_002E3500 - also draw into) goes through the
 * one at page 0x180 at half colour and is blended back 50 / 50, so fades to 3/4, then is
 * stretched over the 512 x 448 frame and added at half strength. The buffer is not cleared
 * between frames, so moving glows leave trails. On PC glr does the passes (the original's
 * packet, 0x96 qwords from +0x10, isn't made: 0 when there was no room for it) */
s32 func_001BA260(u8 *r) {
    extern void glr_glow(void);   /* native/platform/glr.c */

    if (AT(r, 0x304BB6, u8) != 0) {
        return 1;
    }
    AT(r, 0x304BB6, u8) = 1;
    glr_glow();
    return 1;
}
#endif

/* +0x20 the packet for vertex data `key` this frame: open addressing over 2048 slots
 * (+0x300A00, {key, packet}); found: *built = 0 and its packet; new: the slot takes the arena 2
 * cursor (+0x304BAC) and *built = 1 (the caller writes the packet there) */
void *func_001BBAA0(u8 *r, void *key, u8 *built) {
    u32 i = ((u32)key >> 4) & 0x7FF;
    u8 *slot;

    for (;;) {
        slot = r + 0x300A00 + i * 8;
        if (AT(slot, 0x0, void *) == NULL) {
            AT(slot, 0x0, void *) = key;
            AT(slot, 0x4, void *) = AT(r, 0x304BAC, void *);
            *built = 1;
            return AT(slot, 0x4, void *);
        }
        if (AT(slot, 0x0, void *) == key) {
            *built = 0;
            return AT(slot, 0x4, void *);
        }
        i = (i + 1) & 0x7FF;
    }
}

/* +0x8C the floor effect's value (+0x304DDC) */
void func_001B9250(u8 *r, s32 v) {
    AT(r, 0x304DDC, s32) = v;
#ifdef HG_NATIVE
    {
        extern void glr_refl_flip(s32 flip);   /* native/platform/glr.c: the reflection's mirror */

        glr_refl_flip(v);
    }
#endif
}

/* ---- small renderer methods (2026-10-05) ---- */

static inline u32 grey_rgba(u32 c, u32 a) {
    return c | c << 8 | c << 16 | a;
}

/* palette entry i of the layer-0x11 alpha table: 0x80 - clamp(((255-a)>>7) x ((255-a)&127) +
 * 256 - i - a, 0, 128), a = *alpha; the same in all four channels */
u32 func_001B87F0(void *r, s32 i, s32 *alpha) {
    s32 t = 0xFF - *alpha;
    s32 v = (t >> 7) * (t & 0x7F) + 0x100 - (i + *alpha);

    if (v < 0) {
        v = 0;
    } else if (!(v < 0x81)) {
        v = 0x80;
    }
    v = 0x80 - v;
    return (u32)v << 24 | grey_rgba(v, 0);
}

/* palette entry i: black below *limit, else grey 0xC0 (alpha 0x80) */
u32 func_001B8860(void *r, u32 i, u32 *limit) {
    if (i < *limit) {
        return 0x80000000;
    }
    return 0x80C0C0C0;
}

/* palette generator 2: entry i of the ramp from colour c[1] (index 0) to c[0] (index 255), each
 * channel rounded, alpha halved */
static inline u32 ramp_ch(f32 t, f32 u, u32 a, u32 b) {
    return (u32)(0.5f + (u * (f32)b + t * (f32)a));
}

u32 func_001B8910(void *r, u32 i, const u32 *c) {
    f32 t = (f32)i / 255.0f;
    f32 u = 1.0f - t;
    u32 g, rb;

    g = ramp_ch(t, u, (c[0] >> 8) & 0xFF, (c[1] >> 8) & 0xFF) << 8;
    rb = g | ramp_ch(t, u, c[0] & 0xFF, c[1] & 0xFF);
    rb |= ramp_ch(t, u, (c[0] >> 16) & 0xFF, (c[1] >> 16) & 0xFF) << 16;
    return (ramp_ch(t, u, c[0] >> 24, c[1] >> 24) >> 1) << 24 | rb;
}

/* palette entry i: the grey ((2i + 5)(i + 1)) & 0xFF */
u32 func_001B8890(void *r, s32 i) {
    return grey_rgba(((i * 2 + 5) * (i + 1)) & 0xFF, 0x80000000);
}

/* palette entry: a random grey */
u32 func_001B88C0(void) {
    return grey_rgba(VCALL(D_0044E550, 0x10, s32 (*)(VObject *))(D_0044E550) & 0xFF, 0x80000000);
}

/* +0x64 (and others): the layer-0x11 / special colours */
void func_001B9D20(u8 *r, u32 c) {
    AT(r, 0x304D58, u32) = c;
}

u32 func_001B9D30(u8 *r) {
    return AT(r, 0x304D54, u32);
}

void func_001B9D40(u8 *r, u32 c) {
    AT(r, 0x304D54, u32) = c;
}

u32 func_001BA000(u8 *r) {
    return AT(r, 0x304D4C, u32);
}

extern u8 *D_0044F808;   /* the characters' slot 2 */

/* the layer-0x11 tint (+0x304D4C) and its model (+0x304D50; none given: the slot-2 character's,
 * once the game runs) */
void func_001BA010(u8 *r, u32 c, void *model) {
    AT(r, 0x304D4C, u32) = c;
    if (model == NULL && gProgress != NULL) {
        AT(r, 0x304D50, void *) = AT(D_0044F808, 0xF0, void *);
        return;
    }
    AT(r, 0x304D50, void *) = model;
}

/* sprites additive (+0x304C05) with the given mode (+0x304C06) */
void func_001BA070(u8 *r, u8 mode) {
    AT(r, 0x304C05, u8) = 1;
    AT(r, 0x304C06, u8) = mode;
}

#ifdef HG_NATIVE
/* +0x90 the whole screen in colour `rgba` (blended by its alpha), layer 0x31; 1 if drawn */
s32 func_001B9000(VObject *r, u32 rgba) {
    gl2d_sprite(0x31, 0, 0, 512, 448, NULL, 0, 0, 0, 0, rgba, 0, 0x40);
    return 1;
}
#endif

/* the 17-word argument block handed on to +0x84 */
void func_001B9260(VObject *r, s32 *a) {
    VCALL(r, 0x84, void (*)(VObject *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32,
                            s32, s32, s32))(r, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10],
                                            a[11], a[12], a[13], a[14], a[15], a[16]);
}

extern f32 func_0031C058(f32 x);   /* cosf */
extern f32 func_0031C248(f32 x);   /* sinf */

/* +0x6C: the layer-0x11 flares (16 x { x0, y0, x1, y1, phase } at +0x304C0C) drift - each
 * phase turns by up to 2 degrees at random, its sine and cosine (halved) nudge the corners,
 * kept to x0 -56..24, y0 -150..0, x1 / y1 32..64 */
void func_001B9D50(u8 *r) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kTwoPi = {0x40C90FDB};
    VObject *rnd = D_0044E550;
    f32 *e = (f32 *)(r + 0x304C0C);
    u32 i;

    for (i = 0; i < 16; i++, e += 5) {
        f32 c, s;

        e[4] = e[4] + kPi.f * (2.0f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd)) / 180.0f;
        if (!(e[4] <= kPi.f)) {
            e[4] = e[4] - kTwoPi.f;
        }
        c = 0.5f * func_0031C058(e[4]);
        s = 0.5f * func_0031C248(e[4]);
        if (i < 8) {
            e[0] = e[0] + c;
        } else {
            e[0] = e[0] + s;
        }
        if (e[0] < -56.0f) {
            e[0] = -56.0f;
        } else if (!(e[0] <= 24.0f)) {
            e[0] = 24.0f;
        }
        if (i & 1) {
            e[1] = e[1] + s;
        } else {
            e[1] = e[1] + c;
        }
        if (e[1] < -150.0f) {
            e[1] = -150.0f;
        } else if (!(e[1] <= 0.0f)) {
            e[1] = 0.0f;
        }
        if (i & 1) {
            e[2] = e[2] - c;
        } else {
            e[2] = e[2] - s;
        }
        if (e[2] < 32.0f) {
            e[2] = 32.0f;
        } else if (!(e[2] <= 64.0f)) {
            e[2] = 64.0f;
        }
        if (i < 8) {
            e[3] = e[3] - s;
        } else {
            e[3] = e[3] - c;
        }
        if (e[3] < 32.0f) {
            e[3] = 32.0f;
        } else if (!(e[3] <= 64.0f)) {
            e[3] = 64.0f;
        }
    }
}

/* +0x14: the video mode (2 NTSC, 3 PAL; 0x50 progressive 480p): the GS reset for it, the
 * frame height (+0x304BFE: 448 / 512), the mode kept (+0x304C09), the display set up again */
void func_001BB9E0(u8 *r, u8 mode) {
    if (mode == 0x50) {
        func_0010BE10(0, 0, 0x50, 0);
        AT(r, 0x304BFE, s16) = 0x1C0;
    } else {
        func_0010BE10(0, 1, mode, 0);
        if (mode == 2) {
            AT(r, 0x304BFE, s16) = 0x1C0;
        } else {
            AT(r, 0x304BFE, s16) = 0x200;
        }
    }
    AT(r, 0x304C09, u8) = mode;
    func_001B79F0(r);
}
