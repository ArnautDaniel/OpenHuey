/* The renderer (system +0x460, vtable 0x46AC50, global D_0044E4F0). It builds PS2 DMA / GIF
 * packets in double-buffered arenas inside itself; the PC build interprets those packets where
 * the PS2 would send them (libdma / libgraph), so this code stays as the game had it. */
#include "common.h"
#include "game.h"

extern void *func_00115D20(void *p, s32 c, u32 n);   /* memset */

#define AT(p, off, type) (*(type *)((u8 *)(p) + (off)))

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
