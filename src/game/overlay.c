/* Overlay: a full-screen colour rectangle drawn in a renderer layer (the boot scene dims the
 * screen behind its dialogs with one). vtable Overlay_vtable: +0x8 dtor, +0xC draw.
 *
 *   +0x08 u64  stamp (TexCache_Tex0, refreshed when +0x24 changes)
 *   +0x10 s32  +0x24 when the stamp was taken
 *   +0x70 .. +0xE0  camera vectors: +0x70 eye (camera +0x24), +0x80 / +0x90 camera axes
 *        (+0xA4 / +0xA0), +0xA0 their cross product, +0xB0 .. +0xD0 scratch, +0xE0 a point in
 *        front of the camera (camera +0x20 + 1.4 * cross + 1.05 * axis). Computed but the
 *        rectangle itself is drawn in screen space.
 *   +0xF8 u32  colour (RGBA, alpha in the top byte) */
#include "common.h"
#include "game.h"
#include "gs.h"
#include "ptmf.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "overlay.h"
#include "texcache.h"

#define V(p, off) ((f32 *)((u8 *)(p) + (off)))

/* the rectangle's corners: x, y (0 / 1: 512 units from the 0x700 origin), kick */
extern s32 D_00414310[4][3];

/* set the colour (RGBA, alpha in the top byte) */
/* 0x002CF390 */
void Overlay_SetColor(void *ov, u32 rgba) {
    AT(ov, 0xF8, u32) = rgba;
}

#ifdef HG_NATIVE
#include "gl2d.h"

/* the rectangle: a blended strip over the corners (0 / 1: 512 game pixels, from y -32) in the
 * overlay colour, in the layer being drawn; the camera vectors placed per corner as the
 * original does */
/* 0x002CF700 */
void Overlay_DrawRect(void *ov) {
    f32 rel[4], tmp[4], xy[8];
    u8 c[16];
    s32 i;

    for (i = 0; i < 4; i++) {
        s32 x = D_00414310[i][0], y = D_00414310[i][1];

        sceVu0ScaleVector(V(ov, 0xC0), V(ov, 0xA0), 0.0f + (f32)x);
        sceVu0ScaleVector(V(ov, 0xD0), V(ov, 0x80), 0.0f + (f32)y);
        sceVu0AddVector(V(ov, 0xB0), V(ov, 0xC0), V(ov, 0xD0));
        sceVu0SubVector(tmp, V(ov, 0xE0), V(ov, 0xB0));
        sceVu0SubVector(rel, V(ov, 0x70), tmp);
        xy[i * 2] = x << 9;
        xy[i * 2 + 1] = (y << 9) - 0x20;
    }
    gl2d_colors(c, AT(ov, 0xF8, u32), 4);
    glr_prim2d(-1, GLR_2D_STRIP, 4, xy, NULL, c, NULL, 0, 0x40);
}
#else
void Overlay_DrawRect(void *ov);
#endif

/* +0xC draw: place the camera vectors (when +0x24 changed, refresh the stamp first), then the
 * packet. Always draws. */
/* 0x002CF8C0 */
s32 Overlay_Draw(void *ov) {
    void *cam;

    if (AT(ov, 0x24, s32) != AT(ov, 0x10, s32)) {
        AT(ov, 0x8, u64) = TexCache_Tex0(0);
    }
    cam = gCamera;
    VCALL(cam, 0x24, void (*)(void *, f32 *))(cam, V(ov, 0x70));
    VCALL(cam, 0xA4, void (*)(void *, f32 *))(cam, V(ov, 0x80));
    VCALL(cam, 0xA0, void (*)(void *, f32 *))(cam, V(ov, 0x90));
    sceVu0OuterProduct(V(ov, 0xA0), V(ov, 0x80), V(ov, 0x90));
    sceVu0ScaleVector(V(ov, 0xC0), V(ov, 0xA0), 0x1.666666p+0f);   /* 1.4 */
    sceVu0ScaleVector(V(ov, 0xD0), V(ov, 0x80), 0x1.0cccccp+0f);   /* 1.05 */
    sceVu0AddVector(V(ov, 0xB0), V(ov, 0xC0), V(ov, 0xD0));
    VCALL(cam, 0x20, void (*)(void *, f32 *))(cam, V(ov, 0xE0));
    sceVu0AddVector(V(ov, 0xE0), V(ov, 0xE0), V(ov, 0xB0));
    Overlay_DrawRect(ov);
    AT(ov, 0x10, s32) = AT(ov, 0x24, s32);
    return 1;
}

/* the screen bloom (vtable Bloom_vtable; its draw Bloom_Draw): colour `rgba`, subtracted when
 * `sub`, drawn in renderer layer `layer`; the renderer's glow pass (+0x58) runs this frame too */
/* 0x0026B180 */
void Bloom_Start(u8 *o, u32 rgba, s32 layer, s32 sub) {
    VObject *r = (VObject *)gRenderer;

    AT(o, 0x8, u32) = rgba;
    AT(o, 0xC, s32) = sub;
    VCALL(r, 0xC, void (*)(VObject *, void *, s32, s32))(r, o, layer, 0);
    VCALL(r, 0x58, void (*)(VObject *))(r);
}

#ifdef HG_NATIVE
/* the bloom's draw: the screen halved (256 x 224), brightened and blurred (8 taps at 1/2, then
 * 8 wider ones at 1/8 added back), an eighth of it added into the renderer's glow buffer, and
 * stretched over the screen tinted by +0x8 (0x80 = 1.0) at its alpha / 2 - added, or subtracted
 * when +0xC. On PC glr does the passes (the original's 0x209 qwords of GS sprites) */
/* 0x002699D0 */
s32 Bloom_Draw(u8 *o) {
    extern void glr_bloom(u32 rgba, s32 subtract);   /* native/platform/glr.c */

    glr_bloom(AT(o, 0x8, u32), AT(o, 0xC, s32) != 0);
    return 1;
}
#endif
