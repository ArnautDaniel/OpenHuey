/* Overlay: a full-screen colour rectangle drawn in a renderer layer (the boot scene dims the
 * screen behind its dialogs with one). vtable D_0046F350: +0x8 dtor, +0xC draw.
 *
 *   +0x08 u64  stamp (func_002B71D0, refreshed when +0x24 changes)
 *   +0x10 s32  +0x24 when the stamp was taken
 *   +0x70 .. +0xE0  camera vectors: +0x70 eye (camera +0x24), +0x80 / +0x90 camera axes
 *        (+0xA4 / +0xA0), +0xA0 their cross product, +0xB0 .. +0xD0 scratch, +0xE0 a point in
 *        front of the camera (camera +0x20 + 1.4 * cross + 1.05 * axis). Computed but the
 *        rectangle itself is drawn in screen space.
 *   +0xF8 u32  colour (RGBA, alpha in the top byte) */
#include "common.h"
#include "gs.h"
#include "ptmf.h"
#include "sce/libvu0.h"

#define V(p, off) ((f32 *)((u8 *)(p) + (off)))

extern void *D_0044E4B8;   /* camera */
extern void *D_0044E4F0;   /* renderer */
extern u64 func_002B71D0(s32);

/* the rectangle's corners: x, y (0 / 1: 512 units from the 0x700 origin), kick */
extern s32 D_00414310[4][3];

/* set the colour (RGBA, alpha in the top byte) */
void func_002CF390(void *ov, u32 rgba) {
    AT(ov, 0xF8, u32) = rgba;
}

/* write the packet: a blended triangle strip over the corners in the overlay colour */
void func_002CF700(void *ov) {
    u64 *p = VCALL(D_0044E4F0, 0x14, u64 *(*)(void *, s32))(D_0044E4F0, 12);
    u8 *v = (u8 *)(p + 8);
    f32 rel[4], tmp[4];
    s32 i;

    p[0] = DMA_TAG(DMA_CNT, 11, 0);
    AT(p, 0x8, u32) = 0;
    AT(p, 0xC, u32) = 0x5000000B;   /* VIF DIRECT 11 */
    p[2] = 0x800A | (1ULL << 60);   /* GIF tag: 10 A+D, EOP */
    p[3] = 0xE;
    p[4] = 0x4C;                    /* PRIM: triangle strip, Gouraud, blended */
    p[5] = GS_PRIM;
    p[6] = 0x44;                    /* ALPHA_1: (Cs - Cd) * As + Cd */
    p[7] = GS_ALPHA_1;
    for (i = 0; i < 4; i++) {
        s32 x = D_00414310[i][0], y = D_00414310[i][1], kick = D_00414310[i][2];

        sceVu0ScaleVector(V(ov, 0xC0), V(ov, 0xA0), 0.0f + (f32)x);
        sceVu0ScaleVector(V(ov, 0xD0), V(ov, 0x80), 0.0f + (f32)y);
        sceVu0AddVector(V(ov, 0xB0), V(ov, 0xC0), V(ov, 0xD0));
        sceVu0SubVector(tmp, V(ov, 0xE0), V(ov, 0xB0));
        sceVu0SubVector(rel, V(ov, 0x70), tmp);
        AT(v, 0x0, u32) = AT(ov, 0xF8, u32);
        AT(v, 0x4, u32) = 0x3F800000;   /* Q = 1.0 */
        AT(v, 0x8, u64) = GS_RGBAQ;
        AT(v, 0x10, u64) = (u64)(u32)(((x << 9) + 0x700) << 4) | ((u64)(u32)(((y << 9) + 0x700) << 4) << 16)
                           | 0xFFFFFFFF00000000ULL;
        AT(v, 0x18, u64) = kick ? GS_XYZ3 : GS_XYZ2;
        v += 0x20;
    }
}

/* +0xC draw: place the camera vectors (when +0x24 changed, refresh the stamp first), then the
 * packet. Always draws. */
s32 func_002CF8C0(void *ov) {
    void *cam;

    if (AT(ov, 0x24, s32) != AT(ov, 0x10, s32)) {
        AT(ov, 0x8, u64) = func_002B71D0(0);
    }
    cam = D_0044E4B8;
    VCALL(cam, 0x24, void (*)(void *, f32 *))(cam, V(ov, 0x70));
    VCALL(cam, 0xA4, void (*)(void *, f32 *))(cam, V(ov, 0x80));
    VCALL(cam, 0xA0, void (*)(void *, f32 *))(cam, V(ov, 0x90));
    sceVu0OuterProduct(V(ov, 0xA0), V(ov, 0x80), V(ov, 0x90));
    sceVu0ScaleVector(V(ov, 0xC0), V(ov, 0xA0), 0x1.666666p+0f);   /* 1.4 */
    sceVu0ScaleVector(V(ov, 0xD0), V(ov, 0x80), 0x1.0cccccp+0f);   /* 1.05 */
    sceVu0AddVector(V(ov, 0xB0), V(ov, 0xC0), V(ov, 0xD0));
    VCALL(cam, 0x20, void (*)(void *, f32 *))(cam, V(ov, 0xE0));
    sceVu0AddVector(V(ov, 0xE0), V(ov, 0xE0), V(ov, 0xB0));
    func_002CF700(ov);
    AT(ov, 0x10, s32) = AT(ov, 0x24, s32);
    return 1;
}
