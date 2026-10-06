/* Character shadows: shadow volumes of per-bone boxes, counted in renderer layer 6.
 *
 * A model's shadow object (model +0x1D0, vtable D_0046B1C0) is queued in layer 6 once per
 * light that casts it (Shadow_Queue). Its draw (Shadow_Draw) extrudes the bone boxes away
 * from the light and draws their volumes with a counter in the half-size layer-6 buffer: the
 * PS2 does it in the buffer's alpha, read through a palette that adds or takes 1 (lights +0x30),
 * starting at 0x7F; z-pass against the scene's depth copied down to half size. The light's
 * blocker quads' volumes count the other way (Shadow_BlockerVolumes), so a character's shadow doesn't
 * fall where a wall already shades the light. Where the count ends above 0x7F the shadow's
 * colour is written (its opacity from the light's falloff), and layer 6's end blurs the buffer
 * and takes it off the screen. On PC glr keeps the count in a stencil buffer.
 *
 * The shadow object: +0xC the skeleton, +0x8 the bone boxes, +0x110..+0x11C the screen box of
 * this draw (1/16 pixels), +0x120 the box's vertices in the volume (bit 0 on the outline, 1 on
 * a lit face), +0x128 its lit faces, +0x12E its outline edges, +0x140 the vertices extruded,
 * +0x1C0 / +0x240 both projected (ints), +0x2C0 the light's offset, +0x2D0 the bone node,
 * +0x2D4 the colour, +0x2D8 the light, +0x2DC the model's layer, +0x2E0 bones left out.
 *
 * Bone boxes (+0x8): a count, then 0x20-byte entries from +0x10: the bone, the face count (6),
 * then offsets to the faces (0x30 each: 4 vertices, 4 edges, the normal at +0x20), the edges (12
 * x 2 vertices) and the vertices (8). */
#include "common.h"
#include "game.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "navmesh.h"
#include "lights.h"
#include "heap.h"
#include "actor.h"
#include "memcard.h"
#include "ptmf.h"
#include "music.h"
#include "director.h"
#include "doors.h"
#include "fiona.h"
#include "room.h"
#include "text.h"
#include "model.h"
#include "movie.h"
#include "placed.h"
#include "pursuer.h"
#include "vecmath.h"
#include "room_map.h"
#include "scene_game.h"
#include "progress.h"
#include "system.h"
#include "draw_leaves.h"
#include "libc.h"
#include "msl.h"
#include "sce/eekernel.h"
#include "sce/intc.h"
#include "input.h"
#ifdef HG_NATIVE
#include "glr.h"
#endif

extern void *Lights_vtable[], *D_0046B350[];
void Lights_Brightest(VObject *l, s32 *out, u32 tri, const f32 *pos);
/* clip-space point c inside the view volume (|x|, |y|, |z| within w) */
static s32 clip_inside(const f32 *c) {
    return c[0] <= c[3] && !(c[0] < -c[3]) && c[1] <= c[3] && !(c[1] < -c[3]) && c[2] <= c[3] && !(c[2] < -c[3]);
}

/* a VRAM area for an offscreen picture: its slot, and TEX0 for drawing it (64 x 64? psm 0) */
static void vram_area(u8 *o, u32 slotOff, u32 tex0Off, s32 which) {
    VObject *vram = gVram;
    s32 slot = VCALL(vram, 0x14, s32 (*)(VObject *, s32, s32, s32, s32))(vram, 0xFF, 0, 0, 0);

    AT(o, slotOff, s32) = slot;
    if (AT(o, slotOff, s32) != -1) {
        u64 v = VCALL(vram, 0x38, u64 (*)(VObject *, s32))(vram, AT(o, slotOff, s32));

        AT(o, tex0Off, u64) = (v & 0xFFFFFFE000000000ULL) | (0x21B13000 | (6ULL << 32));
        VCALL(gRenderer, 0x94, void (*)(VObject *, s32, s32, s32, s32))(gRenderer, AT(o, slotOff, s32), which, 1, 0);
    }
}

static f32 light_sqrt(f32 x) {
    return __builtin_sqrtf(x);
}

/* light i (12 floats) */
static void light_get(VObject *l, f32 *out, s32 i) {
    VCALL(l, 0x1C, void (*)(f32 *, VObject *, s32))(out, l, i);
}

/* one picked light into slot k */
static void light_slot(f32 (*dir)[4], f32 (*col)[4], f32 *fall, f32 (*lpos)[4], s32 k, const f32 *v, const f32 *pos) {
    f32 at[4] __attribute__((aligned(16)));

    sceVu0SubVector(dir[k], (f32 *)v, pos);
    sceVu0Normalize(dir[k], dir[k]);
    sceVu0CopyVector(at, (f32 *)v);
    if (lpos != NULL) {
        sceVu0CopyVector(lpos[k], at);
    }
    dir[k][3] = -sceVu0InnerProduct(dir[k], at);
    col[k][0] = v[4] * v[7];
    col[k][1] = v[5] * v[7];
    col[k][2] = v[6] * v[7];
    col[k][3] = 0.0f;
    fall[k] = v[9];
}

/* the slots from k on empty: no direction, no colour, falloff 1 */
static void light_empty(f32 (*dir)[4], f32 (*col)[4], f32 *fall, s32 k) {
    for (; k < 3; k++) {
        dir[k][0] = 0.0f;
        dir[k][1] = 0.0f;
        dir[k][2] = 0.0f;
        dir[k][3] = 0.0f;
        sceVu0Normalize(dir[k], dir[k]);
        col[k][0] = 0.0f;
        col[k][1] = 0.0f;
        col[k][2] = 0.0f;
        col[k][3] = 0.0f;
        fall[k] = 1.0f;
    }
}

s32 Doorway_OnScreen(u8 *o, f32 (*q)[4]);
s32 Lights_AddDoorway(u8 *o, f32 (*q)[4]);
void Lights_Reset(u8 *o);
void Lights_RoomStart(u8 *o);
void Lights_ExtraLight(u8 *o, u8 on, const f32 *v, f32 k);
void Lights_ExtraLightI(u8 *o, s32 i, u8 on, const f32 *pos, f32 a, f32 b);
void *Lights_dtor(u8 *o, s32 flags);
void Lights_SetLight(u8 *o, const f32 *light, s32 i);
void Lights_GetLight(f32 *out, u8 *o, s32 i);
u64 Lights_AreaTex0(u8 *o, s32 i);
s32 Lights_ShadowMayFall(u8 *o, u32 tri, f32 *pos);
void Lights_ShadowLights(u8 *o, const f32 *pos, s32 tri, s32 *out);
u8 *Lights_Blockers(u8 *o);
s32 Lights_BlockerCount(u8 *o);
void Lights_Brightest(VObject *l, s32 *out, u32 tri, const f32 *pos);
void Lights_Scripted(u8 *l, f32 (*dir)[4], f32 (*col)[4], f32 *fall, f32 (*lpos)[4]);
void Lights_ForModel(u8 *l, const f32 *pos, s32 tri, f32 (*dir)[4], f32 (*col)[4], f32 *fall, f32 (*lpos)[4]);
void Lights_RestoreLight(u8 *o, s32 i);
void Lights_TakeRoom(VObject *l, u8 *sec);
void Lights_Frame(u8 *o);
void Lights_ReleaseVram(u8 *o);
void Lights_Set944(u8 *o, s32 v);
u8 *Lights_Get14(u8 *o);

/* queue shadow `s` of its model's bone `bone` (on nav triangle `tri`, the light offset `light`,
 * the model's layer `layer`) for each light that casts it there, if the lights allow a shadow
 * at its foot (lights +0x2C) */
/* 0x001F3530 */
void Shadow_Queue(u8 *s, s32 tri, s32 bone, f32 *light, s32 layer) {
    f32 p[4] __attribute__((aligned(16)));
    f32 q[4] __attribute__((aligned(16)));
    s32 idx[3];
    s32 i;

    AT(s, 0x2D0, f32 *) = Skel_Bone(AT(s, 0xC, void *), bone);
    sceVu0CopyVector(p, AT(s, 0x2D0, f32 *) + 12);
    if (gNavMesh != NULL) {
        VCALL(gNavMesh, 0x14, void (*)(VObject *, s32, f32 *))((VObject *)gNavMesh, tri, p);
    } else {
        p[1] = 0.0f;
    }
    sceVu0CopyVector(q, AT(s, 0x2D0, f32 *) + 12);
    q[1] = p[1];
    if ((u8)VCALL(gLights, 0x2C, s32 (*)(VObject *, s32, f32 *))(gLights, tri, q) != 1) {
        return;
    }
    sceVu0CopyVector((f32 *)(s + 0x2C0), light);
    AT(s, 0x2DC, s32) = layer;
    VCALL(gLights, 0x14, void (*)(VObject *, f32 *, s32, s32 *))(gLights, AT(s, 0x2D0, f32 *) + 12, tri, idx);
    for (i = 0; i < 3; i++) {
        if (idx[i] >= 0) {
            AT(s, 0x2D8, s32) = idx[i];
            VCALL(gRenderer, 0xC, void (*)(VObject *, u8 *, s32, s32))(gRenderer, s, 6, 0);
        }
    }
}

/* +0x8: destructor (the global goes) */
/* 0x001F9640 */
void *Lights_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = Lights_vtable;
        AT(o, 0x0, void **) = D_0046B350;
        gLights = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* +0x40 the extra light (+0x950) on or off: on, its vector (+0x960) and strength (+0x970);
   off also turns off the two below */
/* 0x001F96B0 */
void Lights_ExtraLight(u8 *o, u8 on, const f32 *v, f32 k) {
    AT(o, 0x950, u8) = on;
    if (on == 0) {
        AT(o, 0x980, u8) = 0;
        AT(o, 0x9B0, u8) = 0;
    } else {
        sceVu0CopyVector((f32 *)(o + 0x960), (f32 *)v);
        AT(o, 0x970, f32) = k;
    }
}

/* +0x44 extra light `i` (of 2, 0x30 apart from +0x980) on or off: on, its position (+0x990)
   and two parameters (+0x9A0 / +0x9A4) */
/* 0x001F9710 */
void Lights_ExtraLightI(u8 *o, s32 i, u8 on, const f32 *pos, f32 a, f32 b) {
    u8 *l = o + i * 0x30;

    AT(l, 0x980, u8) = on;
    if (on != 0) {
        sceVu0CopyVector((f32 *)(l + 0x990), (f32 *)pos);
        AT(l, 0x9A0, f32) = a;
        AT(l, 0x9A4, f32) = b;
    }
}

/* a doorway quad (its first four points) is wholly on screen */
/* 0x001F9790 */
s32 Doorway_OnScreen(u8 *o, f32 (*q)[4]) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 c[4][4] __attribute__((aligned(16)));
    s32 out = 0;

    VCALL(gCamera, 0x48, void (*)(VObject *, f32 (*)[4]))(gCamera, m);
    sceVu0ApplyMatrix(c[0], m, q[0]);
    sceVu0ApplyMatrix(c[1], m, q[1]);
    sceVu0ApplyMatrix(c[2], m, q[2]);
    sceVu0ApplyMatrix(c[3], m, q[3]);
    if (!clip_inside(c[0]) || !clip_inside(c[1]) || !clip_inside(c[2]) || !clip_inside(c[3])) {
        out = 1;
    }
    return !out;
}

/* the up to 3 lights brightest at `pos` of those reaching nav triangle `tri` (its +0x4C bits):
 * by their luminance (0.3 R + 0.6 G + 0.1 B) x intensity, within a range (v2.x, if any)
 * fading out linearly; their indices into out[0..2] (-1: none) */
/* 0x001F99B0 */
void Lights_Brightest(VObject *l, s32 *out, u32 tri, const f32 *pos) {
    f32 best[3][2];
    u32 mask = 0;
    s32 i, k;
    u8 *tris;

    if (tri < AT(gNavMesh, 0x8, u32) && (tris = AT(gNavMesh, 0x4, u8 *)) != NULL) {
        mask = AT(tris + tri * 0x50, 0x4C, u32);
    }
    best[1][0] = -1.0f;
    best[0][0] = -1.0f;
    best[2][0] = -1.0f;
    for (i = 0; i < 16; i++) {
        f32 v[12] __attribute__((aligned(16)));
        f32 idx, score, r;

        if (!(mask & (1u << i))) {
            continue;
        }
        light_get(l, v, i);
        idx = (f32)i;
        score = (0x1.333334p-1f /* 0.6 */ * v[5] + 0x1.333334p-2f /* 0.3 */ * v[4] + 0x1.99999ap-4f /* 0.1 */ * v[6]) * v[7];
        r = v[8];
        if (!(r <= 0.0f)) {
            f32 dy = pos[1] - v[1], dx = pos[0] - v[0], dz = pos[2] - v[2];
            f32 d2 = dy * dy + dx * dx + dz * dz;

            if (!(d2 < r * r)) {
                score = 0.0f;
            } else {
                score = score * ((r - light_sqrt(d2)) / r);
            }
        }
        if (score <= 0.0f) {
            continue;
        }
        for (k = 0; k < 3; k++) {
            if (best[k][0] == -1.0f) {
                best[k][0] = idx;
                best[k][1] = score;
                break;
            }
            if (!(score <= best[k][1])) {
                f32 ti = best[k][0], ts = best[k][1];

                best[k][0] = idx;
                best[k][1] = score;
                idx = ti;
                score = ts;
            }
        }
    }
    out[0] = (s32)best[0][0];
    out[1] = (s32)best[1][0];
    out[2] = (s32)best[2][0];
}

/* reset: ambient (0, 128, 128, 128), the 16 lights to "none" */
/* 0x001F9C80 */
void Lights_Reset(u8 *o) {
    f32 light[12] __attribute__((aligned(16)));   /* (read with lq) */
    s32 i;

    AT(o, 0x9E0, s32) = 0;
    AT(o, 0x10, s32) = 0;
    AT(o, 0x14, f32) = 128.0f;
    AT(o, 0x18, f32) = 128.0f;
    AT(o, 0x1C, f32) = 128.0f;
    light[3] = 1.0f;
    light[0] = 0.0f;
    light[1] = 0.0f;
    light[2] = 0.0f;
    for (i = 4; i < 12; i++) {
        light[i] = 0.0f;
    }
    for (i = 0; i < 16; i++) {
        VCALL((VObject *)o, 0x20, void (*)(VObject *, f32 *, s32))((VObject *)o, light, i);
    }
}

/* release its two VRAM slots (+0x320 / +0x324, the VRAM manager +0x1C) and mark them -1 */
/* (possibly dead code: nothing in the game references it) */
/* 0x001F9D20 */
void Lights_ReleaseVram(u8 *o) {
    VObject *vram = gVram;

    VCALL(vram, 0x1C, void (*)(VObject *, s32))(vram, AT(o, 0x320, s32));
    AT(o, 0x320, s32) = -1;
    VCALL(vram, 0x1C, void (*)(VObject *, s32))(vram, AT(o, 0x324, s32));
    AT(o, 0x324, s32) = -1;
}

/* start of a room: reset the lights, get the two VRAM areas if not yet held */
/* 0x001F9D90 */
void Lights_RoomStart(u8 *o) {
    Lights_Reset(o);
    if (AT(o, 0x320, s32) == -1) {
        vram_area(o, 0x320, 0x328, 0);
    }
    if (AT(o, 0x324, s32) == -1) {
        vram_area(o, 0x324, 0x330, 1);
    }
    AT(o, 0x944, s32) = 0;
    AT(o, 0x950, u8) = 0;
    AT(o, 0x960, s32) = 0;
    AT(o, 0x964, s32) = 0;
    AT(o, 0x968, s32) = 0;
    AT(o, 0x970, f32) = 1.0f;
    AT(o, 0x990, s32) = 0;
    AT(o, 0x994, s32) = 0;
    AT(o, 0x998, s32) = 0;
    AT(o, 0x9A0, s32) = 0;
    AT(o, 0x9A4, s32) = 0;
    AT(o, 0x9C0, s32) = 0;
    AT(o, 0x9C4, s32) = 0;
    AT(o, 0x9C8, s32) = 0;
    AT(o, 0x9D0, s32) = 0;
    AT(o, 0x9D4, s32) = 0;
}

/* 0x001F9F70 */
void Lights_Set944(u8 *o, s32 v) {
    AT(o, 0x944, s32) = v;
}

/* +0x40 the blocker quads' vector count (over 0x60: reset) */
/* 0x001F9F80 */
s32 Lights_BlockerCount(u8 *o) {
    if (AT(o, 0x940, s32) >= 0x61) {
        AT(o, 0x940, s32) = 0;
    }
    return AT(o, 0x940, s32);
}

/* +0x3C the blocker quads (6 vectors each) */
/* 0x001F9FB0 */
u8 *Lights_Blockers(u8 *o) {
    return o + 0x340;
}

/* +0x38 add a lit doorway for this frame (six vectors: four corners, its facing, its top
 * middle) when it is wholly on screen and there is room (+0x340, 16 of them, +0x940 vectors
 * used) */
/* 0x001F9FC0 */
s32 Lights_AddDoorway(u8 *o, f32 (*q)[4]) {
    s32 i;

    if (!(u8)Doorway_OnScreen(o, q) || AT(o, 0x940, s32) >= 0x60) {
        return 0;
    }
    for (i = 0; i < 6; i++) {
        sceVu0CopyVector((f32 *)(o + 0x340 + (AT(o, 0x940, s32) + i) * 16), q[i]);
    }
    AT(o, 0x940, s32) += 6;
    return 1;
}

/* the lights +0x34 (each frame): clear +0x940 */
/* 0x001FA0D0 */
void Lights_Frame(u8 *o) {
    AT(o, 0x940, s32) = 0;
}

/* the outline of bone box `box` (of the boxes `base`) seen along `l` (box space, scaled to the
 * shadow's length): faces facing along it (n . l >= 0) are lit, an edge of an odd number of lit
 * faces is on the outline; vertices of lit faces and outline edges are extruded by -l */
/* 0x001F2900 */
void Shadow_Outline(u8 *s, u8 *base, u8 *box, f32 *l) {
    u8 *v = base + AT(box, 0x10, s32);
    u8 *e = base + AT(box, 0xC, s32);
    u8 *f = base + AT(box, 0x8, s32);
    s32 i;

    for (i = 0; i < 14; i++) {   /* the vertices' and faces' marks */
        AT(s, 0x120 + i, u8) = 0;
    }
    for (i = 0; i < 12; i++) {
        AT(s, 0x12E + i, u8) = 0;
    }
    for (i = 0; i < AT(box, 0x4, s32); i++, f += 0x30) {
        if (sceVu0InnerProduct(l, (f32 *)(f + 0x20)) < 0.0f) {
            continue;
        }
        AT(s, 0x128 + i, u8) = 1;
        AT(s, 0x12E + AT(f, 0x10, s32), u8) ^= 1;
        AT(s, 0x12E + AT(f, 0x14, s32), u8) ^= 1;
        AT(s, 0x12E + AT(f, 0x18, s32), u8) ^= 1;
        AT(s, 0x12E + AT(f, 0x1C, s32), u8) ^= 1;
        AT(s, 0x120 + AT(f, 0x0, s32), u8) |= 2;
        AT(s, 0x120 + AT(f, 0x4, s32), u8) |= 2;
        AT(s, 0x120 + AT(f, 0x8, s32), u8) |= 2;
        AT(s, 0x120 + AT(f, 0xC, s32), u8) |= 2;
    }
    for (i = 0; i < 12; i++, e += 8) {
        if (AT(s, 0x12E + i, u8) != 0) {
            AT(s, 0x120 + AT(e, 0x0, s32), u8) |= 1;
            AT(s, 0x120 + AT(e, 0x4, s32), u8) |= 1;
        }
    }
    for (i = 0; i < 8; i++, v += 0x10) {
        if (AT(s, 0x120 + i, u8) != 0) {
            sceVu0SubVector((f32 *)(s + 0x140 + i * 0x10), (f32 *)v, l);
        }
    }
}

/* project vertex `v` (the box's) with `scr` into `out` (screen ints, 1/16 pixels; z whole) and
 * grow the shadow's screen box; 0 if it is out of view (`clip`) */
static s32 shadow_project(u8 *s, f32 (*scr)[4], f32 (*clip)[4], f32 *v, s32 *out) {
    f32 t[4] __attribute__((aligned(16)));

    sceVu0ApplyMatrix(t, clip, v);
    if (!(t[0] <= t[3]) || t[0] < -t[3] || !(t[1] <= t[3]) || t[1] < -t[3] || !(t[2] <= t[3]) || t[2] < -t[3]) {
        return 0;
    }
    sceVu0ApplyMatrix(t, scr, v);
    t[3] = 1.0f / t[3];
    t[0] *= t[3];
    t[1] *= t[3];
    t[2] *= t[3];
    sceVu0FTOI4Vector(out, t);
    out[2] = out[2] / 16;
    if (out[0] < AT(s, 0x110, s32)) {
        AT(s, 0x110, s32) = out[0];
    }
    if (out[1] < AT(s, 0x114, s32)) {
        AT(s, 0x114, s32) = out[1];
    }
    if (AT(s, 0x118, s32) < out[0]) {
        AT(s, 0x118, s32) = out[0];
    }
    if (AT(s, 0x11C, s32) < out[1]) {
        AT(s, 0x11C, s32) = out[1];
    }
    return 1;
}

/* project the volume's vertices (box vertices `v`, then the extruded ones) with `scr` (the
 * half-size screen matrix of the bone) into +0x1C0 / +0x240; 0 if one is out of view (`clip`) */
/* 0x001F2560 */
s32 Shadow_Project(u8 *s, f32 (*scr)[4], f32 (*clip)[4], f32 *v) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (AT(s, 0x120 + i, u8) != 0 && !shadow_project(s, scr, clip, v + i * 4, (s32 *)(s + 0x1C0 + i * 0x10))) {
            return 0;
        }
    }
    for (i = 0; i < 8; i++) {
        if (AT(s, 0x120 + i, u8) != 0 &&
            !shadow_project(s, scr, clip, (f32 *)(s + 0x140 + i * 0x10), (s32 *)(s + 0x240 + i * 0x10))) {
            return 0;
        }
    }
    return 1;
}

#ifdef HG_NATIVE

/* a projected point (1/16 pixels around 2048, Z) in clip space: as the camera's clip matrix
 * makes it (Z from zMin .. zMax to -1 .. 1) */
static void shadow_ndc(const s32 *p, f32 *out) {
    f32 zMin = AT(gCamera, 0x18, f32), zMax = AT(gCamera, 0x1C, f32);

    out[0] = ((f32)p[0] / 16.0f - 2048.0f) / 2047.0f;
    out[1] = ((f32)p[1] / 16.0f - 2048.0f) / 2047.0f;
    out[2] = (2.0f * (f32)p[2] - zMax - zMin) / (zMax - zMin);
}

/* lights +0x30 (Lights_ShadowQuad): one quad of a shadow volume (a strip of 4 projected points),
 * counting +1 when its screen winding is negative (`flip` the other way); a degenerate one
 * counts nothing. 0: no room for it (never, on PC) */
/* 0x001FA0E0 */
s32 Lights_ShadowQuad(VObject *l, s32 *p0, s32 *p1, s32 *p2, s32 *p3, s32 flip) {
    s32 cross = (p1[0] - p0[0]) * (p2[1] - p0[1]) - (p1[1] - p0[1]) * (p2[0] - p0[0]);
    f32 xyz[4][3];

    (void)l;
    if (cross == 0) {
        return 1;
    }
    shadow_ndc(p0, xyz[0]);
    shadow_ndc(p1, xyz[1]);
    shadow_ndc(p2, xyz[2]);
    shadow_ndc(p3, xyz[3]);
    glr_shadow_quad(&xyz[0][0], ((cross < 0 ? flip : flip ^ 1)) == 0);
    return 1;
}

/* the volumes of the light's blocker quads (lights +0x3C, 0x60 each: corners, the normal +0x40,
 * the middle +0x50; +0x40 the count of their vectors) facing away from it, extruded from the
 * light (5 above it) by its length and again by 1/5 of it, counting the other way. 0 (no
 * shadow) if one is partly out of view */
/* 0x001F2060 */
s32 Shadow_BlockerVolumes(u8 *s) {
    VObject *lights = gLights, *cam = gCamera;
    s32 n = VCALL(lights, 0x40, s32 (*)(VObject *))(lights);
    f32 scr[4][4] __attribute__((aligned(16)));
    f32 clip[4][4] __attribute__((aligned(16)));
    f32 rec[12] __attribute__((aligned(16)));
    f32 lp[4] __attribute__((aligned(16)));
    f32 len;
    s32 pass, k;

    if (n == 0) {
        return 1;
    }
    VCALL(cam, 0x4C, void (*)(VObject *, f32 (*)[4]))(cam, scr);
    VCALL(cam, 0x58, void (*)(VObject *, f32 (*)[4]))(cam, clip);
    VCALL(lights, 0x1C, void (*)(f32 *, VObject *, s32))(rec, lights, AT(s, 0x2D8, s32));
    lp[0] = rec[0];
    lp[1] = rec[1] + 5.0f;
    lp[2] = rec[2];
    lp[3] = rec[3];
    len = rec[11];
    for (pass = 0; pass < 2; pass++, len = 0.2f * rec[11]) {
        u8 *q = VCALL(lights, 0x3C, u8 *(*)(VObject *))(lights);

        for (k = 0; k < n; k += 6, q += 0x60) {
            f32 d[4] __attribute__((aligned(16)));
            f32 e[4][4] __attribute__((aligned(16)));
            f32 pt[8][4] __attribute__((aligned(16)));
            s32 sc[8][4];
            s32 i;

            sceVu0SubVector(d, lp, (f32 *)(q + 0x50));
            sceVu0Normalize(d, d);
            func_0010E640(d, d, len);
            if (!(sceVu0InnerProduct(d, (f32 *)(q + 0x40)) < 0.0f)) {
                continue;
            }
            for (i = 0; i < 4; i++) {
                sceVu0SubVector(e[i], lp, (f32 *)(q + i * 0x10));
                sceVu0Normalize(e[i], e[i]);
                func_0010E640(e[i], e[i], len);
            }
            for (i = 0; i < 8; i++) {
                f32 t[4] __attribute__((aligned(16)));

                if (i >= 4) {
                    sceVu0SubVector(pt[i], (f32 *)(q + (i - 4) * 0x10), e[i - 4]);
                } else {
                    sceVu0CopyVector(pt[i], (f32 *)(q + i * 0x10));
                }
                sceVu0ApplyMatrix(t, clip, pt[i]);
                if (!(t[0] <= t[3]) || t[0] < -t[3] || !(t[1] <= t[3]) || t[1] < -t[3] || !(t[2] <= t[3]) ||
                    t[2] < -t[3]) {
                    return 0;
                }
            }
            for (i = 0; i < 8; i++) {
                f32 t[4] __attribute__((aligned(16)));

                sceVu0ApplyMatrix(t, scr, pt[i]);
                t[3] = 1.0f / t[3];
                t[0] *= t[3];
                t[1] *= t[3];
                t[2] *= t[3];
                sceVu0FTOI4Vector(sc[i], t);
                sc[i][2] = sc[i][2] / 16;
            }
            if (!Lights_ShadowQuad(lights, sc[0], sc[1], sc[4], sc[5], 1) || !Lights_ShadowQuad(lights, sc[1], sc[3], sc[5], sc[7], 1) ||
                !Lights_ShadowQuad(lights, sc[3], sc[2], sc[7], sc[6], 1) || !Lights_ShadowQuad(lights, sc[2], sc[0], sc[6], sc[4], 1) ||
                !Lights_ShadowQuad(lights, sc[4], sc[5], sc[6], sc[7], 1)) {
                return 0;
            }
        }
    }
    return 1;
}

/* the shadow's draw (+0xC, layer 6): its opacity from the light (record +0x1C: position, +0x1C
 * strength, +0x24 / +0x28 falloff, +0x2C length; out of range or under 2: none), scaled in the
 * tinted layers; then the bone boxes' volumes and the blockers', and the colour where they
 * count. 0: nothing drawn */
/* 0x001F2B80 */
s32 Shadow_Draw(u8 *s) {
    VObject *lights = gLights, *cam = gCamera;
    f32 rec[12] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 scr[4][4] __attribute__((aligned(16)));
    f32 clip[4][4] __attribute__((aligned(16)));
    f32 fall = 1.0f, f;
    u8 *base;
    u32 a;
    s32 h;

    VCALL(lights, 0x1C, void (*)(f32 *, VObject *, s32))(rec, lights, AT(s, 0x2D8, s32));
    if (rec[11] < 1.0f) {
        return 0;
    }
    sceVu0AddVector(d, AT(s, 0x2D0, f32 *) + 12, (f32 *)(s + 0x2C0));
    d[3] = 0.0f;
    d[0] = rec[0] - d[0];
    d[1] = rec[1] - d[1];
    d[2] = rec[2] - d[2];
    if (rec[10] != 0.0f) {
        f32 r = rec[10] - (d[1] * d[1] + d[0] * d[0] + d[2] * d[2]);

        if (r <= 0.0f) {
            return 0;
        }
        r = rec[9] * (r * rec[9]);
        r = r * r;
        fall = r * r;
    }
    f = 64.0f * fall * rec[7];
    a = f >= 2147483648.0f ? (u32)(s32)(f - 2147483648.0f) | 0x80000000u : (u32)(s32)f;
    if (a < 2) {
        return 0;
    }
    if (a > 0x80) {
        a = 0x80;
    }
    switch (AT(s, 0x2DC, s32)) {   /* the tinted layers dim it as they dim the screen */
    case 0x11:
        a = a * (VCALL(gRenderer, 0x68, u32 (*)(VObject *))(gRenderer) >> 25) >> 7;
        break;
    case 0x0F:
    case 0x23:
    case 0x1A: {
        u32 t = VCALL(gRenderer, 0x74, u32 (*)(VObject *))(gRenderer) >> 24;

        if (t < 0x80) {
            a = a * t >> 7;
        }
        break;
    }
    }
    AT(s, 0x2D4, u32) = a << 24 | 0x808080;
    glr_shadow_begin();
    AT(s, 0x110, s32) = 0xFFF0;
    AT(s, 0x114, s32) = 0xFFF0;
    AT(s, 0x118, s32) = 0;
    AT(s, 0x11C, s32) = 0;
    VCALL(cam, 0x4C, void (*)(VObject *, f32 (*)[4]))(cam, scr);
    VCALL(cam, 0x58, void (*)(VObject *, f32 (*)[4]))(cam, clip);
    base = AT(s, 0x8, u8 *);
    if (base == NULL) {
        glr_shadow_cancel();
        return 0;
    }
    for (h = 0; h < AT(base, 0x0, s32); h++) {
        u8 *box = base + 0x10 + h * 0x20;
        u8 *f = base + AT(box, 0x8, s32);
        u8 *edges = base + AT(box, 0xC, s32);
        f32 b[4][4] __attribute__((aligned(16)));
        f32 bs[4][4] __attribute__((aligned(16)));
        f32 bc[4][4] __attribute__((aligned(16)));
        f32 inv[4][4] __attribute__((aligned(16)));
        f32 l[4] __attribute__((aligned(16)));
        s32 i, k;

        if (AT(s, 0x2E0, u32) & (1u << h)) {
            continue;
        }
        sceVu0CopyMatrix(b, (f32 (*)[4])Skel_Bone(AT(s, 0xC, void *), AT(box, 0x0, s32)));
        sceVu0MulMatrix(bs, scr, b);
        sceVu0MulMatrix(bc, clip, b);
        sceVu0InversMatrix(inv, b);
        sceVu0Normalize(l, d);
        sceVu0ApplyMatrix(l, inv, l);
        func_0010E640(l, l, rec[11]);
        Shadow_Outline(s, base, box, l);
        if (Shadow_Project(s, bs, bc, (f32 *)(base + AT(box, 0x10, s32))) == 0) {
            glr_shadow_cancel();
            return 0;
        }
        for (i = 0; i < 6; i++, f += 0x30) {
            s32 *v = (s32 *)f;

            if (AT(s, 0x128 + i, u8) == 0) {
                continue;
            }
            if (!Lights_ShadowQuad(lights, (s32 *)(s + 0x1C0 + v[0] * 0x10), (s32 *)(s + 0x1C0 + v[1] * 0x10),
                               (s32 *)(s + 0x1C0 + v[3] * 0x10), (s32 *)(s + 0x1C0 + v[2] * 0x10), 0) ||
                !Lights_ShadowQuad(lights, (s32 *)(s + 0x240 + v[1] * 0x10), (s32 *)(s + 0x240 + v[0] * 0x10),
                               (s32 *)(s + 0x240 + v[2] * 0x10), (s32 *)(s + 0x240 + v[3] * 0x10), 0)) {
                glr_shadow_cancel();
            return 0;
            }
            for (k = 0; k < 4; k++) {   /* the face's outline edges: their sides */
                s32 en = v[4 + k], p, q;

                if (AT(s, 0x12E + en, u8) == 0) {
                    continue;
                }
                p = AT(edges + en * 8, 0x0, s32);
                q = AT(edges + en * 8, 0x4, s32);
                if (p == v[k]) {   /* along the face's winding */
                    s32 t = p;

                    p = q;
                    q = t;
                }
                if (!Lights_ShadowQuad(lights, (s32 *)(s + 0x1C0 + p * 0x10), (s32 *)(s + 0x1C0 + q * 0x10),
                                   (s32 *)(s + 0x240 + p * 0x10), (s32 *)(s + 0x240 + q * 0x10), 0)) {
                    glr_shadow_cancel();
            return 0;
                }
            }
        }
    }
    if (!(u8)Shadow_BlockerVolumes(s) || AT(s, 0x110, s32) >= AT(s, 0x118, s32)) {
        glr_shadow_cancel();
        return 0;
    }
    glr_shadow_fill((f32)AT(s, 0x110, s32) / 16.0f, (f32)AT(s, 0x114, s32) / 16.0f, (f32)AT(s, 0x118, s32) / 16.0f,
                    (f32)AT(s, 0x11C, s32) / 16.0f, AT(s, 0x2D4, u32));
    return 1;
}
#endif

/* +0x2C a shadow may fall at `pos` on nav triangle `tri`: none of the triangle's lights (+0x4C
 * bits) is switched off for shadows (+0x944), and `pos` lies in front of the nav mesh's plane
 * (+0x2C, by the triangle) as seen from the camera */
/* 0x001FA430 */
s32 Lights_ShadowMayFall(u8 *o, u32 tri, f32 *pos) {
    f32 n[4] __attribute__((aligned(16)));
    f32 eye[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    u32 bits = 0;
    u8 *tris;

    if (tri == (u32)-1) {
        return 0;
    }
    if (tri < AT(gNavMesh, 0x8, u32) && (tris = AT(gNavMesh, 0x4, u8 *)) != NULL) {
        bits = AT(tris + tri * 0x50, 0x4C, u32);
    }
    if (bits & AT(o, 0x944, u32)) {
        return 0;
    }
    VCALL((VObject *)gNavMesh, 0x2C, void (*)(VObject *, u32, f32 *))((VObject *)gNavMesh, tri, n);
    VCALL(gCamera, 0x20, void (*)(VObject *, f32 *))(gCamera, eye);
    sceVu0SubVector(d, pos, eye);
    return !(sceVu0InnerProduct(n, d) < 0.0f) ^ 1;
}

/* 0x001FA520 */
u8 *Lights_Get14(u8 *o) {
    return o + 0x14;
}

/* +0x24 light i back to the room's own (its table +0x9E0, if any) */
/* 0x001FA530 */
void Lights_RestoreLight(u8 *o, s32 i) {
    f32 l[12] __attribute__((aligned(16)));
    const f32 *src;
    s32 k;

    if (AT(o, 0x9E0, u8 *) == NULL) {
        return;
    }
    src = (const f32 *)(AT(o, 0x9E0, u8 *) + 0x10 + i * 0x30);
    for (k = 0; k < 12; k++) {
        l[k] = src[k];
    }
    VCALL((VObject *)o, 0x20, void (*)(VObject *, f32 *, s32))((VObject *)o, l, i);
}

/* +0x20 set light `i` (3 vectors) */
/* 0x001FA5E0 */
void Lights_SetLight(u8 *o, const f32 *light, s32 i) {
    f32 *d = (f32 *)(o + 0x20 + i * 0x30);
    s32 k;

    for (k = 0; k < 12; k++) {
        d[k] = light[k];
    }
}

/* +0x1C light i (12 floats) into `out` (the light is returned by value: the caller's buffer
 * comes first) */
/* 0x001FA680 */
void Lights_GetLight(f32 *out, u8 *o, s32 i) {
    const f32 *l = (const f32 *)(o + 0x20 + i * 0x30);
    s32 k;

    for (k = 0; k < 12; k++) {
        out[k] = l[k];
    }
}

/* +0x18 VRAM area i's TEX0 (+0x328) */
/* 0x001FA6B0 */
u64 Lights_AreaTex0(u8 *o, s32 i) {
    return AT(o, 0x328 + i * 8, u64);
}

/* +0x14 the up to 3 lights casting shadows at `pos` on nav triangle `tri` into out[0..2]
 * (none: count 0, the rest -1) */
/* 0x001FA6C0 */
void Lights_ShadowLights(u8 *o, const f32 *pos, s32 tri, s32 *out) {
    if (AT(o, 0x10, s32) > 0 && tri != -1) {
        Lights_Brightest((VObject *)o, out, tri, pos);
    } else {
        out[0] = 0;
        out[1] = -1;
        out[2] = -1;
    }
}

/* the scripted lighting on top (+0x950): the lights' colours scaled (+0x970), the ambient
 * raised (+0x960), and up to two lights from the camera's side (+0x980, 0x30 apart: on, colour
 * +0x10, turned +0x20 up and +0x24 about) into slots 2 and 1 with no falloff */
/* 0x001FA710 */
void Lights_Scripted(u8 *l, f32 (*dir)[4], f32 (*col)[4], f32 *fall, f32 (*lpos)[4]) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 q[4] __attribute__((aligned(16)));
    f32 up[4] __attribute__((aligned(16)));
    f32 axis[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 tgt[4] __attribute__((aligned(16)));
    f32 eye[4] __attribute__((aligned(16)));
    VObject *cam;
    s32 i, slot = 2;

    q[3] = 0.0f;
    q[2] = 0.0f;
    q[1] = 0.0f;
    q[0] = 0.0f;
    if (AT(l, 0x950, u8) == 0) {
        return;
    }
    cam = gCamera;
    for (i = 0; i < 3; i++) {
        col[i][0] = col[i][0] * AT(l, 0x970, f32);
        col[i][1] = col[i][1] * AT(l, 0x970, f32);
        col[i][2] = col[i][2] * AT(l, 0x970, f32);
    }
    col[3][0] = col[3][0] + AT(l, 0x960, f32);
    col[3][1] = col[3][1] + AT(l, 0x964, f32);
    col[3][2] = col[3][2] + AT(l, 0x968, f32);
    VCALL(cam, 0x20, void (*)(VObject *, f32 *))(cam, eye);
    VCALL(cam, 0x2C, void (*)(VObject *, f32 *))(cam, tgt);
    for (i = 0; i < 2; i++) {
        u8 *x = l + 0x980 + i * 0x30;

        if (AT(x, 0x0, u8) == 0) {
            continue;
        }
        sceVu0SubVector(d, eye, tgt);
        sceVu0Normalize(d, d);
        up[1] = 1.0f;
        up[2] = 0.0f;
        up[0] = 0.0f;
        sceVu0OuterProduct(axis, up, d);
        sceVu0Normalize(axis, axis);
        Quat_FromAxisAngle(q, axis, AT(x, 0x20, f32));
        Quat_ToMatrix(q, m);
        sceVu0ApplyMatrix(d, m, d);
        Quat_FromAxisAngle(q, up, AT(x, 0x24, f32));
        Quat_ToMatrix(q, m);
        sceVu0ApplyMatrix(d, m, d);
        dir[0][slot] = d[0];
        dir[1][slot] = d[1];
        dir[2][slot] = d[2];
        col[slot][0] = AT(x, 0x10, f32);
        col[slot][1] = AT(x, 0x14, f32);
        col[slot][2] = AT(x, 0x18, f32);
        if (lpos != NULL) {
            sceVu0CopyVector(lpos[slot], tgt);
            sceVu0AddVector(lpos[slot], lpos[slot], d);
        }
        fall[slot] = 0.0f;
        slot--;
    }
}

/* +0x10 the light set for a model at `pos` on nav triangle `tri` (none: the room's light 0
 * alone; no position: just a default ambient) into dir / col / fall (and the lights'
 * positions into lpos, if given) */
/* 0x001FAA00 */
void Lights_ForModel(u8 *l, const f32 *pos, s32 tri, f32 (*dir)[4], f32 (*col)[4], f32 *fall, f32 (*lpos)[4]) {
    f32 v[12] __attribute__((aligned(16)));
    s32 idx[4];
    s32 k = 0;

    if (AT(l, 0x10, s32) > 0 && tri != -1) {
        Lights_Brightest((VObject *)l, idx, tri, pos);
        sceVu0UnitMatrix(dir);
        for (k = 0; k < 3 && idx[k] != -1; k++) {
            VCALL((VObject *)l, 0x1C, void (*)(f32 *, VObject *, s32))(v, (VObject *)l, idx[k]);
            light_slot(dir, col, fall, lpos, k, v, pos);
        }
    } else if (pos != NULL) {
        sceVu0UnitMatrix(dir);
        light_get((VObject *)gLights, v, 0);
        light_slot(dir, col, fall, lpos, 0, v, pos);
        k = 1;
    } else {
        for (k = 0; k < 3; k++) {
            dir[k][0] = 0.0f;
            dir[k][1] = 0.0f;
            dir[k][2] = 0.0f;
            dir[k][3] = 0.0f;
            col[k][0] = 0.0f;
            col[k][1] = 0.0f;
            col[k][2] = 0.0f;
            col[k][3] = 0.0f;
            fall[k] = 1.0f;
        }
        AT(col[3], 0x0, u32) = 0x42980000;   /* 76 */
        AT(col[3], 0x4, u32) = 0x42640000;   /* 57 */
        AT(col[3], 0x8, u32) = 0x41400000;   /* 12 */
        col[3][3] = 1.0f;
        fall[3] = 0.0f;
        return;
    }
    light_empty(dir, col, fall, k);
    sceVu0TransposeMatrix(dir, dir);
    col[3][0] = AT(l, 0x14, f32);
    col[3][1] = AT(l, 0x18, f32);
    col[3][2] = AT(l, 0x1C, f32);
    col[3][3] = 1.0f;
    fall[3] = 0.0f;
    Lights_Scripted(l, dir, col, fall, lpos);
}

/* the lights +0xC: take the room's lights (PAC section 4: count, ambient colour, then 0x30
 * bytes per light, set through +0x20); no section: reset */
/* 0x001FAF70 */
void Lights_TakeRoom(VObject *l, u8 *sec) {
    f32 light[12];
    s32 i, k;
    u8 *p;

    if (sec == NULL) {
        Lights_Reset((u8 *)l);
        return;
    }
    AT(l, 0x9E0, u8 *) = sec;
    AT(l, 0x10, s32) = AT(sec, 0x0, s32);
    AT(l, 0x14, f32) = AT(sec, 0x4, f32);
    AT(l, 0x18, f32) = AT(sec, 0x8, f32);
    AT(l, 0x1C, f32) = AT(sec, 0xC, f32);
    p = sec + 0x10;
    for (i = 0; i < AT(l, 0x10, s32); i++) {
        for (k = 0; k < 12; k++) {
            light[k] = AT(p, k * 4, f32);
        }
        p += 0x30;
        VCALL(l, 0x20, void (*)(VObject *, f32 *, s32))(l, light, i);
    }
}

/* ---- a door's shadow (doors +0x190, class D_0046D800): the light through a doorway, as the
 * door's lit face swept away from the light into a box, its four sides drawn in layer 6's
 * buffer shading from the door's colour (+0x54: 0x808080 and the strength) to nothing at the
 * far end. +0x10 the door's matrix, +0x50 its lit face (0 or 6: four corners from +0x50 in
 * D_003EC080, its normal at +4), +0x60 the light (5 above it), +0x70 how far the box goes ---- */

extern f32 D_003EC080[][4], D_003EC0C0[4], D_003EC0D0[4], D_003EC130[4];

/* the shadow from light `l`: 1 if there is one (not out of the light's reach), with its
 * strength (the light's, its falloff at the doorway, the angle it meets the door, the scene's
 * brightness) */
/* 0x002786D0 */
s32 DoorShadow_FromLight(u8 *o, s32 l) {
    VObject *lights = gLights;
    f32 rec[12] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 n[4] __attribute__((aligned(16)));
    f32 power, reach, range, len, fall = 1.0f, c, k, *amb;
    s32 a;

    VCALL(lights, 0x1C, void (*)(f32 *, VObject *, s32))(rec, lights, l);
    power = rec[7];
    reach = rec[9];
    range = rec[10];
    len = rec[11];
    AT(o, 0x60, f32) = rec[0];
    AT(o, 0x64, f32) = 5.0f + rec[1];
    AT(o, 0x68, f32) = rec[2];
    AT(o, 0x6C, f32) = rec[3];
    sceVu0ApplyMatrix(d, (f32 (*)[4])(o + 0x10), D_003EC0D0);
    sceVu0SubVector(d, (f32 *)(o + 0x60), d);
    sceVu0ApplyMatrix(n, (f32 (*)[4])(o + 0x10), D_003EC0C0);
    if (sceVu0InnerProduct(d, n) < 0.0f) {
        AT(o, 0x50, s32) = 0;
    } else {
        AT(o, 0x50, s32) = 6;
        sceVu0ApplyMatrix(d, (f32 (*)[4])(o + 0x10), D_003EC130);
        sceVu0SubVector(d, (f32 *)(o + 0x60), d);
    }
    if (range != 0.0f) {
        f32 d2 = 0.0f + (0.0f + d[1] * d[1] + d[0] * d[0]) + d[2] * d[2];
        f32 r = range - d2;

        if (r < 0.0f) {
            return 0;
        }
        r = r * reach;
        r = reach * r;
        r = r * r;
        fall = r * r;
    }
    sceVu0Normalize(d, d);
    sceVu0ApplyMatrix(n, (f32 (*)[4])(o + 0x10), D_003EC080[AT(o, 0x50, s32) + 4]);
    sceVu0Normalize(n, n);
    c = -sceVu0InnerProduct(d, n);
    k = 4.0f * (c * c);
    amb = VCALL(lights, 0x28, f32 *(*)(VObject *))(lights);
    a = (s32)(k * (fall * power) * (64.0f + (amb[2] + (amb[0] + amb[1])) / 3.0f));
    AT(o, 0x54, s32) = a;
    if (AT(o, 0x54, s32) <= 0) {
        return 0;
    }
    if (AT(o, 0x54, s32) >= 0x81) {
        AT(o, 0x54, s32) = 0x80;
    }
    AT(o, 0x54, u32) = (u32)AT(o, 0x54, s32) << 24 | 0x808080;
    AT(o, 0x70, f32) = 0.75f * len;
    return 1;
}

/* the door's shadow from each of the (up to 3) lights reaching `pos` on nav triangle `tri`
 * (lights +0x2C: a shadow may fall there; +0x14 the lights of the spot), queued in renderer
 * layer 6 when DoorShadow_FromLight finds one. (The original makes the +0x2C call without setting its
 * arguments: they are this function's own tri and pos, still in their registers.) */
/* 0x00278D60 */
void DoorShadow_Queue(u8 *o, u32 tri, f32 *pos, f32 *rot) {
    VObject *lights = gLights;
    s32 l[3];
    s32 i;

    if ((VCALL(lights, 0x2C, s32 (*)(VObject *, u32, f32 *))(lights, tri, pos) & 0xFF) != 1) {
        return;
    }
    VCALL(lights, 0x14, void (*)(VObject *, f32 *, u32, s32 *))(lights, pos, tri, l);
    for (i = 0; i < 3; i++) {
        VObject *r = gRenderer;

        if (l[i] < 0) {
            continue;
        }
        sceVu0UnitMatrix((f32 (*)[4])(o + 0x10));
        sceVu0RotMatrix((f32 (*)[4])(o + 0x10), (f32 (*)[4])(o + 0x10), rot);
        sceVu0TransMatrix((f32 (*)[4])(o + 0x10), (f32 (*)[4])(o + 0x10), pos);
        if ((DoorShadow_FromLight(o, l[i]) & 0xFF) == 1) {
            VCALL(r, 0xC, void (*)(VObject *, u8 *, s32, s32))(r, o, 6, 0);
        }
    }
}

/* the eight corners: the lit face, then each swept away from the light by +0x70; 0 if one is
 * out of view */
static inline __attribute__((always_inline)) s32 door_shadow_corners(u8 *o, VObject *cam, f32 (*p)[4]) {
    f32 clip[4][4] __attribute__((aligned(16)));
    s32 i;

    VCALL(cam, 0x58, void (*)(VObject *, f32 (*)[4]))(cam, clip);
    for (i = 0; i < 8; i++) {
        f32 v[4] __attribute__((aligned(16)));

        if (i < 4) {
            sceVu0ApplyMatrix(p[i], (f32 (*)[4])(o + 0x10), D_003EC080[AT(o, 0x50, s32) + i]);
        } else {
            sceVu0SubVector(p[i], (f32 *)(o + 0x60), p[i - 4]);
            sceVu0Normalize(p[i], p[i]);
            func_0010E640(p[i], p[i], AT(o, 0x70, f32));
            sceVu0SubVector(p[i], p[i - 4], p[i]);
        }
        sceVu0ApplyMatrix(v, clip, p[i]);
        if (!(v[0] <= v[3]) || v[0] < -v[3] || !(v[1] <= v[3]) || v[1] < -v[3] || !(v[2] <= v[3]) || v[2] < -v[3]) {
            return 0;
        }
    }
    return 1;
}

/* the corners on the half-size screen (1/16 pixels, Z) */
static inline __attribute__((always_inline)) void door_shadow_project(VObject *cam, f32 (*p)[4], s32 (*s)[4]) {
    f32 m[4][4] __attribute__((aligned(16))) = {{0}};   /* (an out buffer; cleared so it starts as the original's) */
    s32 i;

    VCALL(cam, 0x4C, void (*)(VObject *, f32 (*)[4]))(cam, m);
    for (i = 0; i < 8; i++) {
        f32 v[4] __attribute__((aligned(16)));
        f32 q;

        sceVu0ApplyMatrix(v, m, p[i]);
        q = 1.0f / v[3];
        v[0] = v[0] * q;
        v[1] = v[1] * q;
        v[3] = q;
        v[2] = v[2] * q;
        sceVu0FTOI4Vector(s[i], v);
        s[i][2] = s[i][2] / 16;
    }
}

#ifdef HG_NATIVE

/* PC: one side of the box, as the PS2 draws it, a gouraud strip into layer 6's buffer */
static void door_shadow_side(u8 *o, s32 *p0, s32 *p1, s32 *p2, s32 *p3) {
    static const f32 kIdentity[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    s32 *p[4] = {p0, p1, p2, p3};
    f32 xyzw[4][4], st[4][2] = {{0}};
    u8 rgba[4][4];
    s32 k;

    if ((p1[0] - p0[0]) * (p2[1] - p0[1]) - (p1[1] - p0[1]) * (p2[0] - p0[0]) >= 0) {
        return;
    }
    for (k = 0; k < 4; k++) {
        u32 c = k < 2 ? AT(o, 0x54, u32) : 0;
        u32 flags = k < 2 ? 0x8000 : 0;

        shadow_ndc(p[k], xyzw[k]);
        AT(&xyzw[k][3], 0, u32) = flags;
        rgba[k][0] = (u8)c;
        rgba[k][1] = (u8)(c >> 8);
        rgba[k][2] = (u8)(c >> 16);
        rgba[k][3] = (u8)(c >> 24);
    }
    glr_strip(kIdentity, 4, &xyzw[0][0], &st[0][0], &rgba[0][0], NULL, 0, 0x8 | 0x40);
}

/* +0xC draw (layer 6): the box's sides into the shadow buffer (page 0x180, 256 x 224) as
 * gouraud strips with OpenGL; 0 if it is partly out of view */
/* 0x002789B0 */
s32 DoorShadow_Draw(u8 *o) {
    VObject *cam = gCamera;
    f32 p[8][4] __attribute__((aligned(16)));
    s32 s[8][4] __attribute__((aligned(16)));

    if (!door_shadow_corners(o, cam, p)) {
        return 0;
    }
    door_shadow_project(cam, p, s);
    door_shadow_side(o, s[0], s[1], s[4], s[5]);
    door_shadow_side(o, s[1], s[3], s[5], s[7]);
    door_shadow_side(o, s[3], s[2], s[7], s[6]);
    door_shadow_side(o, s[2], s[0], s[6], s[4]);
    return 1;
}
#endif
