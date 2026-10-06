/* The loading screen: a turning emblem (from GAME_FIX.GFM) and the "Now Loading" box, drawn
 * while a room loads. The emblem is a static model drawn through VU1. */
#include "common.h"
#include "game.h"
#include "ptmf.h"
#include "gs.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "hewie.h"
#include "loading.h"
#include "overlay.h"
#ifdef HG_NATIVE
#include "glr.h"
#endif
#ifdef HG_NATIVE
#include <stdint.h>
#include <stdlib.h>
#endif

extern void *Helper469D00_vtable[];
#include "task.h"

extern void *Bloom_vtable[];

/* the loading screen, frame `frame`: the camera at a fixed spot, the emblem (`o`, a model object)
 * turning (its rotation from the frame count) drawn in layers 1 and 0x26, the screen dimmed, and
 * the box for the "Now Loading" message (0x808B) */
/* 0x0033E2A0 */
void Loading_DrawFrame(u8 *o, s32 frame) {
    VObject *tc = gTexCache, *cam = gCamera, *r;
    u8 dim[0x10] __attribute__((aligned(16)));
    Task t;
    s32 box[13];
    s32 w, x;
    f32 a;

    VCALL(tc, 0x18, void (*)(VObject *))(tc);
    VCALL(cam, 0x1C, void (*)(VObject *, f32, f32, f32))(cam, 0x1.0e9ed6p+6f, 0x1.0948dep+5f, 0x1.12a69ap+6f);
    VCALL(cam, 0x5C, void (*)(VObject *, f32))(cam, 0x1.0c1524p-1f);   /* 30 degrees */
    VCALL(cam, 0x28, void (*)(VObject *, f32, f32, f32))(cam, -0x1.6a315cp+4f, 0x1.cacac0p+3f, 0x1.a37d6cp+2f);
    VCALL(cam, 0x14, void (*)(VObject *))(cam);

    AT(o, 0x10, f32) = 0.0f;
    AT(o, 0x14, f32) = 0.0f;
    AT(o, 0x18, f32) = 0.0f;
    AT(o, 0x1C, f32) = 1.0f;
    AT(o, 0x20, f32) = 0.0f;
    AT(o, 0x24, f32) = 0.0f;
    AT(o, 0x28, f32) = 0.0f;
    AT(o, 0x2C, f32) = 0.0f;
    AT(o, 0x30, f32) = Angle_Wrap((f32)frame * 0x1.45b3d6p-6f);
    AT(o, 0x34, f32) = 0.0f;
    AT(o, 0x38, f32) = 0.0f;
    AT(o, 0x3C, f32) = 0.0f;
    AT(o, 0x40, f32) = Angle_Wrap((f32)frame * 0x1.750728p-6f);
    AT(o, 0x44, f32) = 0.0f;
    AT(o, 0x48, f32) = 0.0f;
    AT(o, 0x4C, f32) = 0.0f;
    a = (f32)frame * 0x1.b43958p-6f;
    AT(o, 0x50, f32) = Angle_Wrap(a);
    AT(o, 0x54, f32) = 0.0f;
    AT(o, 0x58, f32) = 0.0f;
    AT(o, 0x5C, f32) = 0.0f;
    AT(o, 0x60, f32) = 0.0f;
    AT(o, 0x64, f32) = Angle_Wrap(a);
    AT(o, 0x68, f32) = 0.0f;
    AT(o, 0x6C, f32) = 0.0f;
    AT(o, 0x70, s32) = 0;
    r = gRenderer;
    VCALL(r, 0xC, void (*)(VObject *, void *, s32, s32))(r, o, 1, 0);
    AT(o, 0x70, s32) = 1;
    VCALL(r, 0xC, void (*)(VObject *, void *, s32, s32))(r, o, 0x26, 0);

    AT(dim, 0x0, void **) = Bloom_vtable;
    AT(dim, 0x4, s32) = -1;
    Bloom_Start(dim, 0x5A623C32, 0x28, 0);
    VCALL(tc, 0x18, void (*)(VObject *))(tc);

    Task_Construct(&t);
    w = Task_MessageWidth(&t, 0x808B, 0x10);
    x = 0x1B0;
    if (w >= 0x69) {
        x -= (w - 0x68) >> 1;
    }
    x -= w >> 1;
    box[5] = 0;
    box[2] = w + 0x28;
    box[0] = x - 0x14;
    box[3] = 0x20;
    box[1] = 0x198;
    box[7] = 0x20;
    box[4] = 0xFF;
    box[6] = 0x90;
    box[8] = 0x80808080;
    box[9] = 2;
    box[10] = 0x10;
    box[11] = 0x30;
    box[12] = 6;
    VCALL(r, 0x80, void (*)(VObject *, s32 *))(r, box);
    if (t.child != NULL) {
        Task_dtor(t.child, 1);
        t.child = NULL;
    }
    AT(dim, 0x0, void **) = Helper469D00_vtable;
}

extern u8 D_003AC3F0[];       /* DMA chain: the static model microprogram (MPG) */

#define PI 0x1.921fb6p+1f
#define TWO_PI 0x1.921fb6p+2f
#define DMA_ADDR(p) ((u64)((u32)(p) & 0x0FFFFFFF) << 32)

/* an angle into -pi..pi */
static inline f32 wrap_angle(f32 a) {
    a = a - TWO_PI * (f32)(s32)(a / TWO_PI);
    if (!(a <= PI)) {
        a -= TWO_PI;
    }
    return a;
}

#ifdef HG_NATIVE
#include "gl2d.h"

/* One part of a GAME_FIX.GFM model with OpenGL, as the static model microprogram (D_003AC3F0)
 * draws it: `nv` vertices of UVs (V2-16, / 32768), colours (V4-8, 0x80 = 1.0), positions (V3-16
 * running differences from the part's start, +0x20; / 16) and strip flags (S-8: set, the
 * triangle ending there is skipped), one textured, blended triangle strip; `mvp` the camera's
 * +0x48 by the part's world matrix, `tex` its .TEX entry with palette `csa`. Returns the end of
 * its flags (the next part follows, 16-byte aligned). */
u8 *gl_gfm_part(u8 *part, f32 (*mvp)[4], const u8 *tex, s32 csa) {
    s32 nv = AT(part, 0x0, s32), i;
    const u16 *uv = (const u16 *)(part + AT(part, 0x10, s32));
    const u8 *col = part + AT(part, 0x14, s32);
    const s16 *pos = (const s16 *)(part + AT(part, 0x18, s32));
    u8 *flags = part + AT(part, 0x1C, s32);
    s32 row[3];
    f32 *xyzw, *st;

    if (nv <= 0) {
        return flags;
    }
    row[0] = AT(part, 0x20, s32);
    row[1] = AT(part, 0x24, s32);
    row[2] = AT(part, 0x28, s32);
    xyzw = malloc((u32)nv * 16);
    st = malloc((u32)nv * 8);
    for (i = 0; i < nv; i++) {
        u32 fl = flags[i] ? 0x8000 : 0;

        row[0] += pos[i * 3];
        row[1] += pos[i * 3 + 1];
        row[2] += pos[i * 3 + 2];
        xyzw[i * 4] = (f32)row[0] / 16.0f;
        xyzw[i * 4 + 1] = (f32)row[1] / 16.0f;
        xyzw[i * 4 + 2] = (f32)row[2] / 16.0f;
        AT(&xyzw[i * 4 + 3], 0, u32) = fl;
        st[i * 2] = (f32)uv[i * 2] / 32768.0f;
        st[i * 2 + 1] = (f32)uv[i * 2 + 1] / 32768.0f;
    }
    glr_strip((const f32 *)mvp, nv, xyzw, st, col, tex, 1ull << 34 | (u64)(csa & 0x1F) << 56, 0x10 | 0x40);
    free(xyzw);
    free(st);
    return flags + nv;
}

/* Draw the loading emblem `o` (renderer object: +0x10 position, +0x20 rotation, +0x30.. the
 * parts' extra rotations, +0x70 1: only the last part): GAME_FIX.GFM's first model, texture 2
 * of group 0x10, its parts chained, each turned and moved relative to the previous (palette:
 * the part's +0x4). */
/* 0x0033D9F0 */
s32 LoadingEmblem_Draw(u8 *o) {
    VObject *tc = gTexCache, *cam;
    f32 world[4][4] __attribute__((aligned(16)));
    f32 view[4][4] __attribute__((aligned(16)));
    f32 rel[4][4] __attribute__((aligned(16)));
    f32 extra[4][4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));
    f32 trans[4] __attribute__((aligned(16)));
    f32 rot[4] __attribute__((aligned(16)));
    s32 count, n;
    u8 *texh, *model, *part;

    if (VCALL(tc, 0x8, s32 (*)(VObject *, s32, s32))(tc, 2, 0x10) == -1) {
        return 0;
    }
    texh = VCALL(tc, 0xC, u8 *(*)(VObject *, s32, s32))(tc, 2, 0x10);
    model = VCALL(gGamePtr, 0x1C, u8 *(*)(VObject *))((VObject *)gGamePtr);
    if (AT(model, 0x0, s32) <= 0) {
        return 1;
    }
    part = model + AT(model, 0x0, s32);
    n = 0;
    count = AT(part, 0x8, s32);
    if (count == 0) {
        return 1;
    }
    cam = gCamera;
    do {
        u8 *next;

        count--;
        if (n == 0) {
            sceVu0CopyVector(rot, (f32 *)(o + 0x20));
            sceVu0UnitMatrix(m);
            sceVu0RotMatrix(m, m, rot);
            sceVu0CopyVector(trans, (f32 *)(o + 0x10));
        } else {
            sceVu0AddVector(rot, rot, (f32 *)(part + 0x30));
            rot[0] = wrap_angle(rot[0]);
            rot[1] = wrap_angle(rot[1]);
            rot[2] = wrap_angle(rot[2]);
            sceVu0UnitMatrix(rel);
            sceVu0RotMatrix(rel, rel, rot);
            sceVu0UnitMatrix(extra);
            if (n > 0) {
                sceVu0RotMatrix(extra, extra, (f32 *)(o + 0x30 + (n - 1) * 0x10));
            }
            sceVu0MulMatrix(rel, rel, extra);
            sceVu0MulMatrix(m, m, rel);
            sceVu0AddVector(trans, trans, (f32 *)(part + 0x40));
        }
        n++;
        if (AT(o, 0x70, s32) != 0 && count != 0) {
            next = part + AT(part, 0x1C, s32) + AT(part, 0x0, s32);   /* (only the last part drawn) */
        } else {
            sceVu0TransMatrix(world, m, trans);
            VCALL(cam, 0x48, void (*)(VObject *, f32 (*)[4]))(cam, view);
            sceVu0MulMatrix(view, view, world);
            next = gl_gfm_part(part, view, texh, AT(part, 0x4, s32));
        }
        part = (u8 *)(((uintptr_t)next + 15) & ~(uintptr_t)15);
    } while (count != 0);
    return 1;
}
#endif
