/* The renderer (system +0x460, vtable 0x46AC50, global D_0044E4F0). It builds PS2 DMA / GIF
 * packets in double-buffered arenas inside itself; the PC build interprets those packets where
 * the PS2 would send them (libdma / libgraph), so this code stays as the game had it. */
#include "common.h"
#include "game.h"

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
