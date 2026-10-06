/* 2D drawing on PC: the game's sprites, boxes and text through the OpenGL renderer
 * (native/platform/glr.h, glr_prim2d) instead of GS packets. Coordinates are the game's
 * 512 x 448 screen pixels as its GS XYZ2 values / 16 (offset 0x700 / 0x720 taken off). */
#ifndef GL2D_H
#define GL2D_H

#ifdef HG_NATIVE
#include "../native/platform/glr.h"

/* GS blend modes (ALPHA_1) as glr prim bits: 0x44 (Cs - Cd) * As + Cd, 0x48 Cs * As + Cd,
 * 0x42 Cd - Cs * As, 0x64 (Cs - Cd) * FIX + Cd, 0x68 Cs * FIX + Cd, 0x62 Cd - Cs * FIX; anything
 * else drawn as 0x44 */
static inline u32 gl2d_blend(u64 alpha) {
    u32 fix = (u32)(alpha >> 32) & 0xFF;

    switch ((u32)alpha & 0xFF) {
    case 0x48:
        return 0x40 | GLR_PRIM_ADD;
    case 0x42:
        return 0x40 | GLR_PRIM_ADD | GLR_PRIM_SUB;
    case 0x64:
        return 0x40 | GLR_PRIM_FIX(fix) | GLR_PRIM_LERP;
    case 0x68:
        return 0x40 | GLR_PRIM_FIX(fix);
    case 0x62:
        return 0x40 | GLR_PRIM_FIX(fix) | GLR_PRIM_SUB;
    }
    return 0x40;
}

/* an image the game sent to VRAM block `block` (renderer +0x48 / +0x4C), as a .TEX-style entry
 * for gl2d_sprite / glr_prim2d (NULL: none) */
const u8 *gl2d_image(u32 block);

/* `n` copies of colour `rgba` (0x80 = 1.0) */
static inline void gl2d_colors(u8 *c, u32 rgba, s32 n) {
    s32 k;

    for (k = 0; k < n; k++) {
        c[k * 4 + 0] = rgba;
        c[k * 4 + 1] = rgba >> 8;
        c[k * 4 + 2] = rgba >> 16;
        c[k * 4 + 3] = rgba >> 24;
    }
}

/* an axis-aligned sprite: x0, y0 .. x1, y1 showing texels u0, v0 .. u1, v1 of .TEX entry `tex`
 * (NULL: untextured; its w x h at +4 / +6) with palette `csa` */
static inline void gl2d_sprite(s32 layer, f32 x0, f32 y0, f32 x1, f32 y1, const u8 *tex, f32 u0, f32 v0, f32 u1,
                               f32 v1, u32 rgba, s32 csa, u32 prim) {
    f32 xy[8], st[8];
    u8 c[16];

    xy[0] = x0; xy[1] = y0;
    xy[2] = x1; xy[3] = y0;
    xy[4] = x0; xy[5] = y1;
    xy[6] = x1; xy[7] = y1;
    if (tex != NULL) {
        f32 tw = *(const u16 *)(tex + 4), th = *(const u16 *)(tex + 6);

        st[0] = u0 / tw; st[1] = v0 / th;
        st[2] = u1 / tw; st[3] = v0 / th;
        st[4] = u0 / tw; st[5] = v1 / th;
        st[6] = u1 / tw; st[7] = v1 / th;
    }
    gl2d_colors(c, rgba, 4);
    glr_prim2d(layer, GLR_2D_STRIP, 4, xy, st, c, tex, csa, prim);
}
#endif

#endif
