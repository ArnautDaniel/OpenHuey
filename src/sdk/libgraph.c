/* Sony libgraph (PS2 SDK) helpers whose results end up in the game's GS packets. Decompiled
 * rather than reimplemented from documentation so the register values are exactly the PS2's;
 * the PC build uses them as they are. */
#include "common.h"

/* sceGsGParam: the video mode set by sceGsResetGraph */
typedef struct GsGParam {
    s16 interlace;
    s16 omode;
    s16 ffmode;
    s16 version;
} GsGParam;

extern GsGParam D_003ACD40;

/* sceGsGetGParam */
GsGParam *func_0010BFA0(void) {
    return &D_003ACD40;
}

/* Z buffer base (in 2048-word pages) after a w x h frame buffer of format psm: one page is
 * 64 x 32 pixels (32-bit) or 64 x 64 (16-bit); two buffers unless interlaced frame mode. */
s32 func_0010C500(s32 psm, s32 w, s32 h) {
    GsGParam *gp = func_0010BFA0();
    s32 pw = ((s16)w + 63) / 64;
    s32 ph = ((s16)psm & 2) ? ((s16)h + 63) / 64 : ((s16)h + 31) / 32;
    s32 n = pw * ph;

    if (gp->interlace == 1 && gp->ffmode == 0) {
        return (s16)n;
    }
    return (s16)(n << 1);
}

/* One drawing context's registers (sceGsDrawEnv1 / 2), as A+D pairs. */
typedef struct GsDrawEnv {
    u64 frame, frameAddr;
    u64 zbuf, zbufAddr;
    u64 xyoffset, xyoffsetAddr;
    u64 scissor, scissorAddr;
    u64 prmodecont, prmodecontAddr;
    u64 colclamp, colclampAddr;
    u64 dthe, dtheAddr;
    u64 test, testAddr;
} GsDrawEnv;

static inline u64 GsDrawEnv_Zbuf(s32 psm, s32 w, s32 h, s32 ztest, s32 zpsm) {
    u64 z = (u64)(s64)(s16)func_0010C500(psm, w, h) | (u64)(zpsm & 0xF) << 24;

    return ztest == 0 ? z | (u64)1 << 32 : z;   /* no Z test: don't write Z either */
}

static inline void GsDrawEnv_Common(GsDrawEnv *d, s16 psm, s16 w, s16 h, s32 ztest) {
    d->xyoffset = (u64)((0x800 - (s16)(w >> 1)) * 16) | (u64)(0x800 - (s16)(h >> 1)) << 36;
    d->scissor = (u64)(w - 1) << 16 | (u64)(h - 1) << 48;
    d->prmodecontAddr = 0x1A;
    d->prmodecont |= 1;
    d->colclampAddr = 0x46;
    d->colclamp |= 1;
    d->dtheAddr = 0x45;
    if (psm & 2) {
        d->dthe |= 1;    /* dither 16-bit frame buffers */
    } else {
        d->dthe &= ~(u64)1;
    }
    d->test = ztest != 0 ? (u64)((ztest & 3) << 17) | 0x10000 : 0x30000;
}

/* sceGsSetDefDrawEnv(env, psm, w, h, ztest, zpsm): drawing context 1, frame buffer at 0. */
s32 func_0010C5C8(GsDrawEnv *d, s32 psm, s32 w, s32 h, s32 ztest, s32 zpsm) {
    d->frameAddr = 0x4C;
    d->frame = (u64)((((s16)w + 63) >> 6) & 0x3F) << 16 | (u64)((s16)psm & 0xF) << 24;
    d->zbufAddr = 0x4E;
    d->zbuf = GsDrawEnv_Zbuf(psm, w, h, (s16)ztest, (s16)zpsm);
    d->xyoffsetAddr = 0x18;
    d->scissorAddr = 0x40;
    GsDrawEnv_Common(d, psm, w, h, (s16)ztest);
    d->testAddr = 0x47;
    return 8;
}

/* sceGsSetDefDrawEnv2: the same for drawing context 2 (frame width rounded down). */
s32 func_0010D020(GsDrawEnv *d, s32 psm, s32 w, s32 h, s32 ztest, s32 zpsm) {
    d->frameAddr = 0x4D;
    d->frame = (u64)(((s16)w >> 6) & 0x3F) << 16 | (u64)((s16)psm & 0xF) << 24;
    d->zbufAddr = 0x4F;
    d->zbuf = GsDrawEnv_Zbuf(psm, w, h, (s16)ztest, (s16)zpsm);
    d->xyoffsetAddr = 0x19;
    d->scissorAddr = 0x41;
    GsDrawEnv_Common(d, psm, w, h, (s16)ztest);
    d->testAddr = 0x48;
    return 8;
}

/* sceGsSetDefClear(clear, ztest, x, y, w, h, r, g, b, a, z): a sprite covering the area in the
 * given colour (Z test off while drawing it), then the Z test mode. */
s32 func_0010C7B0(u64 *c, s32 ztest, s32 x, s32 y, s32 w, s32 h, s32 r, s32 g, s32 b, s32 a, u32 z) {
    c[2] = 6;   /* PRIM: sprite */
    c[5] = 1;
    c[4] = (u64)(u8)r | (u64)(u8)g << 8 | (u64)(u8)b << 16 | (u64)(u8)a << 24 | 0x3F80000000000000ULL;
    c[6] = (u64)((s16)x * 16) | (u64)((s16)y * 16) << 16 | (u64)z << 32;
    c[9] = 5;
    c[8] = (u64)(((s16)x + (s16)w) * 16) | (u64)(((s16)y + (s16)h) * 16) << 16 | (u64)z << 32;
    c[11] = 0x47;
    c[1] = 0x47;
    c[0] = 0x30000;   /* TEST: Z always */
    c[3] = 0;
    c[7] = 5;
    c[10] = (s16)ztest != 0 ? (u64)(((s16)ztest & 3) << 17) | 0x10000 : 0x30000;
    return 6;
}

extern void func_0010C440(void *disp);   /* sceGsPutDispEnv */
extern void sceGsPutDrawEnv(void *giftag);

/* sceGsSwapDBuff(db, field): show display environment `field`, send that buffer's drawing
 * environment (the sceGsDBuff layout: two display environments, then two GIF packets). */
void func_0010D200(u8 *db, s32 field) {
    s32 f = field & 1;

    func_0010C440(db + f * 0x28);
    if (f != 0) {
        sceGsPutDrawEnv(db + 0x1C0);
        return;
    }
    sceGsPutDrawEnv(db + 0x50);
}
