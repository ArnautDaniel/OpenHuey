/* Objects the room scripts spawn into the effect manager (SceneGame +0xF6E200 slots). */
#include "common.h"
#include "game.h"
#include "sce/libvu0.h"

extern VObject *D_0044E550;   /* the random number generator */

extern void *D_00474000[], *D_0046FC30[], *D_00469D00[], *D_0046F580[];
extern void func_002D63B0(void *p);   /* free (the effect manager's heap) */

/* (class D_00474000, room 0x2A) +0x8 destructor (the quad drawer at +0x610 inlined) */
u8 *func_00321910(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00474000;
    AT(o, 0x610, void **) = D_0046FC30;
    AT(o, 0x610, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* (class D_00474000, room 0x2A) +0xC reset: a random delay (0x5A..0x79) and its settings */
void func_00322430(u8 *o) {
    AT(o, 0x8EC, s32) = (VCALL(D_0044E550, 0x10, u32 (*)(VObject *))(D_0044E550) & 0x1F) + 0x5A;
    AT(o, 0x8F0, s32) = 0;
    AT(o, 0x8F4, s32) = 0;
    AT(o, 0x8F8, u8) = 0;
    AT(o, 0x618, s64) = -1;
    AT(o, 0x628, s32) = 0;
    AT(o, 0x62C, s32) = 0;
    AT(o, 0x630, s32) = 0x19;
    AT(o, 0x634, s16) = 1;
    AT(o, 0x636, s16) = 0;
    AT(o, 0x638, s16) = 0xE0;
    AT(o, 0x63A, s16) = 0x20;
    AT(o, 0x63C, s16) = 0x20;
    AT(o, 0x63E, s16) = 0x200;
    AT(o, 0x640, s16) = 0x100;
    AT(o, 0x642, u8) = 2;
    AT(o, 0x643, u8) = 0x10;
    AT(o, 0x644, u8) = 1;
    AT(o, 0x645, u8) = 0x10;
    AT(o, 0x646, u8) = 0xA;
}

extern void func_003219A0(u8 *o, s32 i);

typedef union {
    u32 u;
    f32 f;
} F32Bits;

/* +0x18 start: at its fixed spot by the room's (211.28, 2.125, -220.567), 16 particles spread
 * up to 100 to one side */
void func_00321D20(u8 *o, f32 *params) {
    static const F32Bits kX = {0x435347AE}, kZ = {0xC35C9127}, kSpeed = {0xBDCCCCCD};
    VObject *rng;
    s32 i;

    if (params == NULL) {
        return;
    }
    sceVu0CopyVector((f32 *)(o + 0x650), params);
    AT(o, 0x650, f32) = kX.f;
    AT(o, 0x654, f32) = 2.125f;
    AT(o, 0x658, f32) = kZ.f;
    AT(o, 0x65C, f32) = 1.0f;
    rng = D_0044E550;
    for (i = 0; i < 16; i++) {
        u8 *e;
        f32 r;

        func_003219A0(o, i);
        e = o + AT(o, 0x8F4, s32) * 0x300 + i * 0x30 + 0x10;
        r = VCALL(rng, 0x18, f32 (*)(VObject *))(rng);
        AT(e, 0x10, f32) = (AT(o, 0x650, f32) - 20.0f) + 100.0f * r;
    }
    AT(o, 0x8E0, f32) = 0.0f;
    AT(o, 0x8E4, f32) = 0.0f;
    AT(o, 0x8E8, f32) = kSpeed.f;
}

/* particle i of the current set (+0x10 + set * 0x300, 0x30 each): grey, scattered around the
 * spot, with random spin (+0x820 + i * 12), angles (+0x660 + i * 16) and sizes (+0x760) */
void func_003219A0(u8 *o, s32 i) {
    static const F32Bits k02 = {0x3E4CCCCD}, k005 = {0x3D4CCCCD}, k001 = {0x3C23D70A},
                         kM005 = {0xBD4CCCCD}, kPi = {0x40490FDB};
    VObject *rng = D_0044E550;
    u8 *p = o + AT(o, 0x8F4, s32) * 0x300 + i * 0x30 + 0x10;
    s32 k;

    AT(p, 0x0, s32) = 0x80;
    AT(p, 0x4, s32) = 0x80;
    AT(p, 0x8, s32) = 0x80;
    AT(p, 0xC, s32) = 0x80;
#define RND() VCALL(rng, 0x18, f32 (*)(VObject *))(rng)
    AT(p, 0x10, f32) = (AT(o, 0x650, f32) - 10.0f) + 10.0f * (RND() - 0.5f);
    AT(p, 0x14, f32) = AT(o, 0x654, f32) + RND();
    AT(p, 0x18, f32) = (20.0f + AT(o, 0x658, f32)) + 150.0f * RND();
    AT(p, 0x1C, f32) = 1.0f;
    AT(p, 0x20, f32) = 1.0f;
    AT(p, 0x24, f32) = 1.0f;
    AT(p, 0x28, f32) = 0.0f;
    AT(p, 0x2C, u32) = VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 0xF;
    AT(o, 0x820 + i * 12, f32) = k02.f + k005.f * RND();
    AT(o, 0x824 + i * 12, f32) = k001.f * RND();
    AT(o, 0x828 + i * 12, f32) = kM005.f * RND();
    for (k = 0; k < 3; k++) {
        AT(o, 0x660 + i * 16 + k * 4, f32) = kPi.f * (180.0f * RND()) / 180.0f;
    }
    for (k = 0; k < 3; k++) {
        AT(o, 0x760 + i * 12 + k * 4, f32) = 10.0f + 22.5f * (RND() - 0.5f);
    }
#undef RND
}

/* (class D_00478BC0) +0xC reset: three random angles in -pi..pi */
void func_00350A10(u8 *o) {
    static const F32Bits kPi = {0x40490FDB};
    VObject *rng = D_0044E550;
    s32 k;

    for (k = 0; k < 3; k++) {
        f32 r = VCALL(rng, 0x18, f32 (*)(VObject *))(rng);

        AT(o, 0x4 + k * 4, f32) = kPi.f * (360.0f * (r - 0.5f)) / 180.0f;
    }
}

/* ---- class D_00478BC0 (room 0x2A, 0x14 bytes): a model turning on three axes; +0x4/+0x8/+0xC
 * the angles, +0x10 the model (0x2C or 0x30) ---- */

extern void *D_00478BC0[], *D_0046F580[], *D_00478B70[], *D_00469D00[];
extern void func_002D63B0(void *o);         /* delete from the effect heap */

/* +0x8 destructor */
u8 *func_003507B0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00478BC0;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* +0x10 update: turn */
s32 func_00350910(u8 *o) {
    static const F32Bits kPi = {0x40490FDB}, kMinusPi = {0xC0490FDB}, k2Pi = {0x40C90FDB},
                         kStepX = {0x3C0EFA35}, kStepYZ = {0x3AE4C389};

    AT(o, 0x4, f32) = AT(o, 0x4, f32) + kStepX.f;
    if (!(AT(o, 0x4, f32) <= kPi.f)) {
        AT(o, 0x4, f32) = AT(o, 0x4, f32) - k2Pi.f;
    }
    AT(o, 0x8, f32) = AT(o, 0x8, f32) + kStepYZ.f;
    if (!(AT(o, 0x8, f32) <= kPi.f)) {
        AT(o, 0x8, f32) = AT(o, 0x8, f32) - k2Pi.f;
    }
    AT(o, 0xC, f32) = AT(o, 0xC, f32) - kStepYZ.f;
    if (AT(o, 0xC, f32) < kMinusPi.f) {
        AT(o, 0xC, f32) = AT(o, 0xC, f32) + k2Pi.f;
    }
    return 1;
}

/* a model to draw this frame: position, ..., angles, model, colour. Drawn by D_00478B70's
 * func_0034E9E0 these are a light caustic: n the texture, radius the patch's size, angle[0]
 * the ripple phase, angle[1] / angle[2] its two layers' turns, model the alpha threshold of
 * its glow */
typedef struct ModelDrawParams {
    f32 pos[4];
    s32 n;
    f32 radius;
    f32 angle[3];
    s32 model;
    u32 rgba;
} ModelDrawParams;

/* the temporary draw object func_00350660 fills from the parameters (vtable D_00478B70) */
typedef struct ModelDraw {
    void **vtbl;
    s32 slot;
    u8 pad8[8];
    ModelDrawParams p;
} ModelDraw;

void func_00350660(ModelDraw *d, const ModelDrawParams *p);   /* queue a model draw */

/* +0x14 draw: model +0x10 at its spot (65.64, 6.1, 70.94), turned by its angles */
void func_00350860(u8 *o) {
    static const F32Bits kX = {0x42834704}, kY = {0x40C33333}, kZ = {0x428DE227}, kR = {0x421F3333};
    ModelDraw d __attribute__((aligned(16)));
    ModelDrawParams p __attribute__((aligned(16)));

    p.pos[0] = kX.f;
    p.pos[1] = kY.f;
    p.pos[2] = kZ.f;
    p.pos[3] = 1.0f;
    p.n = 0xF;
    p.radius = kR.f;
    p.angle[0] = AT(o, 0x4, f32);
    p.angle[1] = AT(o, 0x8, f32);
    p.angle[2] = AT(o, 0xC, f32);
    p.model = AT(o, 0x10, s32);
    p.rgba = 0x80808080;
    d.slot = -1;
    d.vtbl = D_00478B70;
    func_00350660(&d, &p);
    d.vtbl = D_00469D00;
}

/* +0x18 start: model 0x30 (params[0] 0) or 0x2C, then a first update */
void func_00350810(VObject *o, const u8 *params) {
    if (params == NULL) {
        return;
    }
    AT(o, 0x10, s32) = params[0] == 0 ? 0x30 : 0x2C;
    VCALL(o, 0x10, s32 (*)(VObject *))(o);
}

extern VObject *D_0044E4F0;   /* the renderer */

/* fill draw object `d` from `p` and queue it with the renderer (+0xC, layer 0x19) */
void func_00350660(ModelDraw *d, const ModelDrawParams *p) {
    d->p.pos[0] = p->pos[0];
    d->p.pos[1] = p->pos[1];
    d->p.pos[2] = p->pos[2];
    d->p.pos[3] = p->pos[3];
    d->p.n = p->n;
    d->p.radius = p->radius;
    d->p.angle[0] = p->angle[0];
    d->p.angle[1] = p->angle[1];
    d->p.angle[2] = p->angle[2];
    d->p.model = p->model;
    d->p.rgba = p->rgba;
    VCALL(D_0044E4F0, 0xC, void (*)(VObject *, ModelDraw *, s32, s32))(D_0044E4F0, d, 0x19, 0);
}

/* ---- class D_00477E10 (room 0x55, 0x18 bytes): a light caustic like D_00478BC0's, over the
 * whole room (texture 3, 160 across) at height +0x14; +0x4/+0x8/+0xC its turns (updated by
 * func_003476A0), +0x10 the glow threshold ---- */

extern void *D_00477E10[];

/* +0x8 destructor */
u8 *func_003474E0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00477E10;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* +0xC reset: three random angles in -pi..pi */
void func_003477A0(u8 *o) {
    static const F32Bits kPi = {0x40490FDB};
    VObject *rng = D_0044E550;
    s32 k;

    for (k = 0; k < 3; k++) {
        f32 r = VCALL(rng, 0x18, f32 (*)(VObject *))(rng);

        AT(o, 0x4 + k * 4, f32) = kPi.f * (360.0f * (r - 0.5f)) / 180.0f;
    }
}

/* +0x14 draw */
void func_003475A0(u8 *o) {
    ModelDraw d __attribute__((aligned(16)));
    ModelDrawParams p __attribute__((aligned(16)));

    p.pos[0] = 0.0f;
    p.pos[1] = AT(o, 0x14, f32);
    p.pos[2] = 0.0f;
    p.pos[3] = 1.0f;
    p.n = 3;
    p.radius = 160.0f;
    p.angle[0] = AT(o, 0x4, f32);
    p.angle[1] = AT(o, 0x8, f32);
    p.angle[2] = AT(o, 0xC, f32);
    p.model = AT(o, 0x10, s32);
    p.rgba = 0x80808080;
    d.slot = -1;
    d.vtbl = D_00478B70;
    func_00350660(&d, &p);
    d.vtbl = D_00469D00;
}

/* +0x18 start: params[0] 0 at height 45.1 (threshold 0x60), else -5.9 (0x58); then a first
 * update */
void func_00347540(VObject *o, const s32 *params) {
    static const F32Bits kHigh = {0x42346666}, kLow = {0xC0BCCCCD};

    if (params == NULL) {
        return;
    }
    if (params[0] == 0) {
        AT(o, 0x14, f32) = kHigh.f;
        AT(o, 0x10, s32) = 0x60;
    } else {
        AT(o, 0x14, f32) = kLow.f;
        AT(o, 0x10, s32) = 0x58;
    }
    VCALL(o, 0x10, s32 (*)(VObject *))(o);
}


extern VObject *D_0044E550;   /* random numbers: +0x10 an integer */

/* +0x10 each frame (returns 0 once every particle has left): 16 particles, double-buffered
 * (+0x10, 0x300 per buffer, 0x30 each), drift by their velocity (+0x820) plus a wind
 * (+0x8E0..) whose phase (+0x8F0) changes at random; they spin (+0x660 by +0x760 degrees),
 * count frames up to +0x643, and respawn once past x = 300 */
s32 func_00321FE0(u8 *o) {
    static const union { u32 u; f32 f; } k0005 = {0x3BA3D70A}, k005 = {0x3D4CCCCD}, kPi = {0x40490FDB},
        kTwoPi = {0x40C90FDB};
    s32 i, k;

    if (AT(o, 0x8F8, u8) == 1) {
        return 0;
    }
    AT(o, 0x8F8, u8) = 1;
    AT(o, 0x8F4, s32) ^= 1;
    if (--AT(o, 0x8EC, s32) == 0) {
        AT(o, 0x8EC, s32) = VCALL(D_0044E550, 0x10, s32 (*)(VObject *))(D_0044E550) & 0x3F;
        AT(o, 0x8F0, s32) = (AT(o, 0x8F0, s32) + 1) & 3;
    }
    if (AT(o, 0x8F0, s32) == 1) {
        AT(o, 0x8E0, f32) = AT(o, 0x8E0, f32) - k0005.f;
        if (AT(o, 0x8E0, f32) < -k005.f) {
            AT(o, 0x8E0, f32) = -k005.f;
        }
        AT(o, 0x8E4, f32) = 0.0f;
    } else if (AT(o, 0x8F0, s32) == 3) {
        AT(o, 0x8E0, f32) = AT(o, 0x8E0, f32) + k0005.f;
        if (!(AT(o, 0x8E0, f32) <= k005.f)) {
            AT(o, 0x8E0, f32) = k005.f;
        }
        AT(o, 0x8E4, u32) = 0xBCF5C28F;   /* -0.03f */
    }
    for (i = 0; i < 16; i++) {
        u32 buf = AT(o, 0x8F4, u32);
        u32 *src = &AT(o, 0x10 + (buf ^ 1) * 0x300 + i * 0x30, u32);
        u32 *dst = &AT(o, 0x10 + buf * 0x300 + i * 0x30, u32);
        u8 *p;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        p = o + 0x10 + AT(o, 0x8F4, s32) * 0x300 + i * 0x30;
        AT(p, 0x10, f32) = AT(p, 0x10, f32) + (AT(o, 0x820 + i * 0xC, f32) + AT(o, 0x8E0, f32));
        AT(p, 0x14, f32) = AT(p, 0x14, f32) + (AT(o, 0x824 + i * 0xC, f32) + AT(o, 0x8E4, f32));
        AT(p, 0x18, f32) = AT(p, 0x18, f32) + (AT(o, 0x828 + i * 0xC, f32) + AT(o, 0x8E8, f32));
        if (!(AT(p, 0x10, f32) < 300.0f)) {
            func_003219A0(o, i);
            continue;
        }
        AT(o, 0x8F8, u8) = 0;
        for (k = 0; k < 3; k++) {
            f32 *a = &AT(o, 0x660 + i * 0x10 + k * 4, f32);

            *a = *a + kPi.f * AT(o, 0x760 + i * 0xC + k * 4, f32) / 180.0f;
            if (!(*a <= kPi.f)) {
                *a = *a - kTwoPi.f;
            } else if (*a < -kPi.f) {
                *a = *a + kTwoPi.f;
            }
        }
        if (++AT(p, 0x2C, s32) >= AT(o, 0x643, s8)) {
            AT(p, 0x2C, s32) = 0;
        }
    }
    return 1;
}


extern void func_002E56C0(u8 *quad);   /* draw a textured quad (corners +0x14, record +0x10) */

/* +0x14 draw: each of the 16 particles is a unit quad in the xz plane turned by its spin
 * (+0x660), drawn by the quad drawer (+0x610) from the current buffer's record */
void func_00321E30(u8 *o) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 c[4][4] __attribute__((aligned(16)));
    s32 i;

    AT(o, 0x624, f32 *) = c[0];
    for (i = 0; i < 16; i++) {
        sceVu0UnitMatrix(m);
        sceVu0RotMatrix(m, m, (f32 *)(o + 0x660 + i * 0x10));
        c[0][0] = 0.5f;  c[0][1] = 0.0f; c[0][2] = -0.5f; c[0][3] = 1.0f;
        sceVu0ApplyMatrix(c[0], m, c[0]);
        c[1][0] = 0.5f;  c[1][1] = 0.0f; c[1][2] = 0.5f;  c[1][3] = 1.0f;
        sceVu0ApplyMatrix(c[1], m, c[1]);
        c[2][0] = -0.5f; c[2][1] = 0.0f; c[2][2] = -0.5f; c[2][3] = 1.0f;
        sceVu0ApplyMatrix(c[2], m, c[2]);
        c[3][0] = -0.5f; c[3][1] = 0.0f; c[3][2] = 0.5f;  c[3][3] = 1.0f;
        sceVu0ApplyMatrix(c[3], m, c[3]);
        AT(o, 0x620, u8 *) = o + 0x10 + AT(o, 0x8F4, s32) * 0x300 + i * 0x30;
        func_002E56C0(o + 0x610);
    }
}


#ifdef HG_NATIVE
extern void glr_strip(const f32 *mvp, s32 n, const f32 *xyzw, const f32 *st, const u8 *rgba, const void *tex,
                      u64 tex0, u32 prim);
extern void glr_caustic_begin(void);
extern void glr_caustic_glow(s32 aref);
extern VObject *D_0044E4B8;   /* the camera */
extern VObject *D_0044E4E8;   /* the texture cache */
extern f32 func_002E2D00(f32 angle);   /* wrap an angle into -pi..pi */
extern f32 func_0031C058(f32 x);       /* cosf */
extern f32 func_0031C248(f32 x);       /* sinf */

#define GLR_PRIM_NOZW 0x20000u
#define GLR_PRIM_FIX(f) (0x80000u | (u32)(f) << 24)   /* blend Cs * f / 128 + Cd */

/* func_0034E9E0, the caustic (vtable D_00478B70 +0xC; the draw object of func_00350660): the
 * frame's alpha cleared, then two 8 x 8 grids of the texture (+0x20) - a square of side +0x24
 * at +0x10 lying flat, turned +0x2C / +0x30 about y - added at 1/8 in colour +0x38, depth
 * tested without depth writes. Their texture coordinates wobble by 0.05 with the phase +0x28
 * (cos along one axis, sin along the other, a quarter turn per cell; the second grid -0.4 of a
 * half turn per cell). Where they leave the frame's alpha at least +0x34, the halved screen is
 * blurred (4 diagonal taps at 1/2) and added back at 1/2 - the caustic's glow. On PC glr does
 * the passes; 0 = nothing linked into the layer. */
s32 func_0034E9E0(u8 *d) {
    VObject *cam = D_0044E4B8;
    const void *tex = VCALL(D_0044E4E8, 0xC, void *(*)(VObject *, s32, s32))(D_0044E4E8, AT(d, 0x20, s32), 0);
    f32 size = AT(d, 0x24, f32), half = 0.5f * size, cell = 0.125f * size, phase = AT(d, 0x28, f32);
    s32 g;

    if (tex == NULL) {
        return 0;
    }
    glr_caustic_begin();
    for (g = 0; g < 2; g++) {
        f32 clip[4][4] __attribute__((aligned(16)));
        f32 m[4][4] __attribute__((aligned(16)));
        s32 i, j, k;

        VCALL(cam, 0x48, void (*)(VObject *, f32 (*)[4]))(cam, clip);
        sceVu0UnitMatrix(m);
        sceVu0RotMatrixY(m, m, AT(d, g == 0 ? 0x2C : 0x30, f32));
        sceVu0TransMatrix(m, m, (f32 *)(d + 0x10));
        sceVu0MulMatrix(clip, clip, m);
        for (i = 0; i < 8; i++) {   /* a strip along each row band */
            f32 xyzw[18][4] __attribute__((aligned(16)));
            f32 st[18][2];
            u8 rgba[18][4];

            for (j = 0; j < 9; j++) {
                for (k = 0; k < 2; k++) {
                    s32 v = j * 2 + k, r = i + k;

                    xyzw[v][0] = (f32)r * cell - half;
                    xyzw[v][1] = 0.0f;
                    xyzw[v][2] = (f32)j * cell - half;
                    AT(&xyzw[v][3], 0, u32) = j == 0 ? 0x8000 : 0;   /* the first column starts the strip */
                    if (g == 0) {
                        st[v][0] = 0.1f + 0.1f * (f32)r
                                   + 0.05f * func_0031C058(func_002E2D00(phase + 0.5f * (3.1415927f * (f32)r)));
                        st[v][1] = 0.1f * (f32)j + 0.05f * func_0031C248(func_002E2D00(phase + 0.5f * (3.1415927f * (f32)j)));
                    } else {
                        st[v][0] = 0.1f * (f32)j + 0.05f * func_0031C058(func_002E2D00(phase + -0.4f * (3.1415927f * (f32)r)));
                        st[v][1] = 0.1f + 0.1f * (f32)r
                                   + 0.05f * func_0031C248(func_002E2D00(phase + -0.4f * (3.1415927f * (f32)j)));
                    }
                    AT(rgba[v], 0, u32) = AT(d, 0x38, u32);
                }
            }
            glr_strip(&clip[0][0], 18, &xyzw[0][0], &st[0][0], &rgba[0][0], tex, 1ULL << 34,
                      0x10 | 0x40 | GLR_PRIM_NOZW | GLR_PRIM_FIX(0x10));
        }
    }
    glr_caustic_glow(AT(d, 0x34, s32));
    return 0;
}
#endif

/* ---- class D_00479400 (room 0x55, 0x6D0 bytes): 16 drops falling from up to 150 over an
 * 80 x 80 square, double-buffered (+0x10 + buffer +0x688 * 0x300, a quad record of 0x30 each)
 * and drawn by the quad drawer at +0x610 (texture group 0x10, cell (0x6C, 0x4C) 8 x 8,
 * additive with glow); +0x648 + i * 4 their fall speed, +0x690 + i * 4 the nav triangle under
 * each (-1 none), +0x68C the next splash sound (0..2). Where a drop lands it splashes (and
 * starts again): on the floor a spray, off the mesh below -6 a splash ring and a spray ---- */

#include "effectmgr.h"

extern void *D_00479400[], *D_00479AE0[], *D_00479AA0[];
extern VObject *D_0044E560;   /* the sound driver */
extern void *D_0044E570;      /* the nav mesh: +0x3C the triangle under a point, +0x14 the floor height in one */
extern void func_002FF650(VObject *snd, u32 id, u32 bank, f32 *pos, s32 vol, s32 pitch);

/* +0x8 destructor (the quad drawer at +0x610 inlined) */
u8 *func_003532A0(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00479400;
    AT(o, 0x610, void **) = D_0046FC30;
    AT(o, 0x610, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

#define DROP(o, i) ((o) + AT(o, 0x688, s32) * 0x300 + (i) * 0x30 + 0x10)

/* drop i anew: faint grey (alpha 0x70), half size, somewhere in the square up to 150 high,
 * turned at random, falling 1.5..2.5 a frame */
void func_00353330(u8 *o, s32 i) {
    static const F32Bits kPi = {0x40490FDB};
    VObject *rng = D_0044E550;
    u8 *p = DROP(o, i);
    VObject *nav;

    AT(p, 0x0, s32) = 0x20;
    AT(p, 0x4, s32) = 0x20;
    AT(p, 0x8, s32) = 0x20;
    AT(p, 0xC, s32) = 0x70;
#define RND() VCALL(rng, 0x18, f32 (*)(VObject *))(rng)
    AT(p, 0x10, f32) = 80.0f * (RND() - 0.5f);
    AT(p, 0x14, f32) = 150.0f * RND();
    AT(p, 0x18, f32) = 80.0f * (RND() - 0.5f);
    AT(p, 0x1C, f32) = 1.0f;
    AT(p, 0x20, f32) = 0.5f;
    AT(p, 0x24, f32) = 0.5f;
    AT(p, 0x28, f32) = kPi.f * (360.0f * (RND() - 0.5f)) / 180.0f;
    AT(p, 0x2C, s32) = 0;
    AT(o, 0x648 + i * 4, f32) = 1.5f + RND();
#undef RND
    nav = (VObject *)D_0044E570;
    AT(o, 0x690 + i * 4, s32) = VCALL(nav, 0x3C, s32 (*)(VObject *, f32 *, s32))(nav, (f32 *)(p + 0x10), 0);
}

/* +0x14 draw: the current buffer's 16 records */
void func_003534F0(u8 *o) {
    AT(o, 0x620, u8 *) = DROP(o, 0);
    func_002E56C0(o + 0x610);
}

/* a splash's parameters (D_00479AE0: pos, colour, size) and a spray's (D_00479AA0) */
typedef struct {
    f32 pos[4];
    u8 rgba[4];
    f32 size;
} SplashParams;

typedef struct {
    f32 pos[4];
    u8 rgba[4];
    s32 n;
    f32 v[10];
} SprayParams;

static void splash_init(void **obj) {
    obj[0] = D_00479AE0;
}

static void spray_init(void **obj) {
    obj[0] = D_00479AA0;
    obj[0x610 / 4] = D_00469D00;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = D_0046FC30;
}

/* +0x10 update: swap buffers, carry each drop over and drop it; a landed one splashes (the
 * first drop also with sound 2..4 of bank 6) and starts again */
s32 func_00353520(u8 *o) {
    VObject *rng = D_0044E550;
    u8 *mgr = D_0044E578;
    VObject *nav;
    VObject *snd;
    s32 i, k;

    AT(o, 0x688, s32) ^= 1;
    nav = (VObject *)D_0044E570;
    snd = D_0044E560;
    for (i = 0; i < 16; i++) {
        u32 buf = AT(o, 0x688, u32);
        u32 *dst = &AT(o, 0x10 + buf * 0x300 + i * 0x30, u32);
        u32 *src = &AT(o, 0x10 + (buf ^ 1) * 0x300 + i * 0x30, u32);
        u8 *p;
        f32 at[4] __attribute__((aligned(16)));
        u8 landed = 0;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        p = DROP(o, i);
        AT(p, 0x14, f32) = AT(p, 0x14, f32) - AT(o, 0x648 + i * 4, f32);
        if (AT(o, 0x690 + i * 4, s32) == -1) {
            if (AT(p, 0x14, f32) < -6.0f) {
                static const F32Bits kWater = {0xC0BCCCCD}, k01 = {0x3DCCCCCD}, k015 = {0x3E19999A},
                                     k02 = {0x3E4CCCCD}, k03 = {0x3E99999A};
                SplashParams s __attribute__((aligned(16)));
                SprayParams r __attribute__((aligned(16)));

                landed = 1;
                sceVu0CopyVector(at, (f32 *)(p + 0x10));
                at[1] = kWater.f;
                sceVu0CopyVector(s.pos, at);
                s.rgba[3] = 0x20;
                s.rgba[0] = 0x40;
                s.rgba[1] = 0x40;
                s.rgba[2] = 0x40;
                s.size = k015.f + k01.f * VCALL(rng, 0x1C, f32 (*)(VObject *))(rng);
                func_002D6090(mgr, Effect_New(mgr, 0x40, splash_init), &s);
                sceVu0CopyVector(r.pos, at);
                r.v[2] = 0.0f;
                r.rgba[0] = 0x20;
                r.rgba[3] = 0x30;
                r.n = 1;
                r.rgba[1] = 0x20;
                r.rgba[2] = 0x20;
                r.v[0] = 0.5f;
                r.v[8] = 0.5f;
                r.v[4] = k03.f;
                r.v[1] = k02.f;
                r.v[5] = k02.f;
                r.v[9] = k01.f;
                r.v[3] = 0.0f;
                r.v[6] = 0.0f;
                r.v[7] = 0.0f;
                func_002D6090(mgr, Effect_New(mgr, 0x720, spray_init), &r);
            }
        } else {
            sceVu0CopyVector(at, (f32 *)(p + 0x10));
            VCALL(nav, 0x14, void (*)(VObject *, s32, f32 *))(nav, AT(o, 0x690 + i * 4, s32), at);
            if (AT(p, 0x14, f32) < at[1]) {
                static const F32Bits k01 = {0x3DCCCCCD}, k02 = {0x3E4CCCCD}, k04 = {0x3ECCCCCD};
                SprayParams r __attribute__((aligned(16)));

                landed = 1;
                sceVu0CopyVector(r.pos, at);
                r.rgba[0] = 0x20;
                r.rgba[1] = 0x20;
                r.rgba[2] = 0x20;
                r.rgba[3] = 0x10;
                r.n = 0x10;
                r.v[0] = k04.f;
                r.v[1] = k04.f;
                r.v[8] = 0.5f;
                r.v[2] = k01.f;
                r.v[3] = k02.f;
                r.v[4] = k01.f;
                r.v[5] = k02.f;
                r.v[7] = k02.f;
                r.v[6] = k01.f;
                r.v[9] = k01.f;
                func_002D6090(mgr, Effect_New(mgr, 0x720, spray_init), &r);
            }
        }
        if (landed == 1) {
            if (i == 0) {
                func_002FF650(snd, AT(o, 0x68C, s32) + 2, 6, (f32 *)(p + 0x10), 0, 0);
                if (AT(o, 0x68C, s32) >= 2) {
                    AT(o, 0x68C, s32) = 0;
                } else {
                    AT(o, 0x68C, s32)++;
                }
            }
            func_00353330(o, i);
        }
    }
    return 1;
}

/* +0xC reset: the quad drawer's settings and 16 new drops */
void func_00353BD0(u8 *o) {
    s32 i;

    AT(o, 0x688, s32) = 0;
    AT(o, 0x68C, s32) = 0;
    AT(o, 0x618, s64) = -1;
    AT(o, 0x624, s32) = 0;
    AT(o, 0x628, s32) = 0;
    AT(o, 0x62C, s32) = 0;
    AT(o, 0x630, s32) = 0x19;
    AT(o, 0x634, s16) = 0x10;
    AT(o, 0x636, s16) = 0x6C;
    AT(o, 0x638, s16) = 0x4C;
    AT(o, 0x63A, s16) = 8;
    AT(o, 0x63C, s16) = 8;
    AT(o, 0x63E, s16) = 0x200;
    AT(o, 0x640, s16) = 0x100;
    AT(o, 0x642, u8) = 0xC0;
    AT(o, 0x643, u8) = 1;
    AT(o, 0x644, u8) = 1;
    AT(o, 0x645, u8) = 0x10;
    AT(o, 0x646, u8) = 0xFF;
    for (i = 0; i < 16; i++) {
        func_00353330(o, i);
    }
}

/* ---- class D_00477AC0 (room 0x66, 0x1BC0 bytes): smoke rising at spot +0x1BB8 / 2 of the
 * table D_0043B640 (x, z pairs) - 64 puffs, double-buffered (+0x10 + buffer +0x1BB0 * 0xC00, a
 * quad record of 0x30 each) and drawn by the quad drawer at +0x1840, with a glow sprite (record
 * +0x1810, drawer +0x1878) whose alpha follows how many puffs show. Per puff its rise speed
 * (+0x18B0 + i * 4), sway phase (+0x19B0) and strength (+0x1AB0, 1 at first); +0x1BBC set
 * (start parameter < 0) it dies down: strengths fall by 0.2 a respawn, the glow (+0x1BB4) by
 * 0.003 ---- */

extern void *D_00477AC0[];
extern f32 D_0043B640[], D_0043B644[];   /* the spots: x, z (read as pairs) */
extern f32 func_0031C058(f32 x);   /* cosf */
extern f32 func_0031C248(f32 x);   /* sinf */

#define SMOKE_PUFF(o, i) ((o) + AT(o, 0x1BB0, s32) * 0xC00 + (i) * 0x30 + 0x10)

/* +0x8 destructor (the quad drawers at +0x1840 and +0x1878 inlined) */
u8 *func_003453D0(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00477AC0;
    AT(o, 0x1878, void **) = D_0046FC30;
    AT(o, 0x1878, void **) = D_00469D00;
    AT(o, 0x1840, void **) = D_0046FC30;
    AT(o, 0x1840, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* puff i anew (`again` 0: the first time, part way up already): white, alpha about 0x40
 * (+-0x20 by strength), size 0.5..8, scattered about the spot (wider across x at spot 4,
 * else along z), rising 0.65 +- 0.15 x strength; dying down, its strength falls and once the
 * glow is out it just stays hidden */
void func_00345490(u8 *o, s32 i, s32 again) {
    static const F32Bits k02 = {0x3E4CCCCD}, k0003 = {0x3B449BA6}, k03 = {0x3E99999A}, k065 = {0x3F266666},
                         kPi = {0x40490FDB}, k2Pi = {0x40C90FDB};
    VObject *rng;
    u8 *p;
    f32 up = 0.0f, f;
    s32 k;

    if (AT(o, 0x1BBC, u8) == 1) {
        f32 *s = &AT(o, 0x1AB0 + i * 4, f32);

        *s = *s - k02.f;
        if (*s < 0.0f) {
            *s = 0.0f;
        }
        AT(o, 0x1BB4, f32) = AT(o, 0x1BB4, f32) - k0003.f;
        if (AT(o, 0x1BB4, f32) < 0.0f) {
            AT(o, 0x1BB4, f32) = 0.0f;
            AT(SMOKE_PUFF(o, i), 0xC, s32) = 0;
            return;
        }
    }
    rng = D_0044E550;
    p = SMOKE_PUFF(o, i);
    AT(p, 0x0, s32) = 0x80;
    AT(p, 0x4, s32) = 0x80;
    AT(p, 0x8, s32) = 0x80;
    AT(p, 0xC, s32) = (s32)(AT(o, 0x1AB0 + i * 4, f32) *
                            (f32)(s32)((VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 0x3F) - 0x20)) + 0x40;
    AT(p, 0x20, f32) = 0.5f + 7.5f * VCALL(rng, 0x18, f32 (*)(VObject *))(rng);
    if (again == 0) {
        up = VCALL(rng, 0x18, f32 (*)(VObject *))(rng);
        AT(p, 0xC, s32) = (s32)((f32)AT(p, 0xC, s32) * (1.0f - up));
        AT(p, 0x20, f32) = AT(p, 0x20, f32) + 1.5f * up;
    }
    rng = D_0044E550;
#define RND(o) VCALL(rng, o, f32 (*)(VObject *))(rng)
    k = AT(o, 0x1BB8, s32);
    f = AT(o, 0x1AB0 + i * 4, f32) * (1.0f + (f32)((k == 4) * 5));
    AT(p, 0x10, f32) = D_0043B640[k] + f * (RND(0x18) - 0.5f);
    AT(p, 0x14, f32) = 30.0f * up - 4.0f;
    k = AT(o, 0x1BB8, s32);
    f = AT(o, 0x1AB0 + i * 4, f32) * (1.0f + (f32)((k != 4) << 4));
    AT(p, 0x18, f32) = D_0043B644[k] + f * (RND(0x18) - 0.5f);
    AT(p, 0x1C, f32) = 1.0f;
    AT(p, 0x24, f32) = AT(p, 0x20, f32);
    AT(p, 0x28, f32) = kPi.f * RND(0x1C) / 180.0f;
    AT(p, 0x2C, s32) = 0;
    AT(o, 0x18B0 + i * 4, f32) = k065.f + (k03.f * AT(o, 0x1AB0 + i * 4, f32)) * (RND(0x18) - 0.5f);
    AT(o, 0x19B0 + i * 4, f32) = k2Pi.f * (RND(0x18) - 0.5f);
#undef RND
}

/* +0x14 draw: the puffs, then the glow */
void func_00345A00(u8 *o) {
    AT(o, 0x1850, u8 *) = SMOKE_PUFF(o, 0);
    func_002E56C0(o + 0x1840);
    AT(o, 0x1888, u8 *) = o + 0x1810;
    func_002E56C0(o + 0x1878);
}

/* +0x10 update (0 once it has died down and no puff shows): swap buffers; each puff grows,
 * turns, sways and rises, and starts again above 50 or (every other frame, fading faster the
 * weaker it is) once faded out; the glow's alpha is the number showing (+0..7) times +0x1BB4 */
s32 func_00345A60(u8 *o) {
    static const F32Bits k001 = {0x3C23D70A}, k02 = {0x3E4CCCCD}, k01 = {0x3DCCCCCD}, kPi = {0x40490FDB},
                         k2Pi = {0x40C90FDB};
    VObject *rng = D_0044E550;
    s32 i, k, n;

    AT(o, 0x1BB0, s32) ^= 1;
    for (i = 0; i < 64; i++) {
        u32 buf = AT(o, 0x1BB0, u32);
        u32 *dst = &AT(o, 0x10 + buf * 0xC00 + i * 0x30, u32);
        u32 *src = &AT(o, 0x10 + (buf ^ 1) * 0xC00 + i * 0x30, u32);
        f32 *ph = &AT(o, 0x19B0 + i * 4, f32);
        u8 *p;
        f32 a;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        p = SMOKE_PUFF(o, i);
        AT(p, 0x20, f32) = AT(p, 0x24, f32) =
            AT(p, 0x20, f32) + (k001.f + k02.f * VCALL(rng, 0x18, f32 (*)(VObject *))(rng));
        AT(p, 0x28, f32) = AT(p, 0x28, f32) + 0.5f * (kPi.f * VCALL(rng, 0x1C, f32 (*)(VObject *))(rng) / 180.0f);
        a = *ph + kPi.f * (10.0f * VCALL(rng, 0x18, f32 (*)(VObject *))(rng)) / 180.0f;
        *ph = a;
        if (!(a <= kPi.f)) {
            *ph = a - k2Pi.f;
        }
        AT(p, 0x10, f32) = AT(p, 0x10, f32) + k01.f * func_0031C248(*ph);
        AT(p, 0x14, f32) = AT(p, 0x14, f32) + AT(o, 0x18B0 + i * 4, f32);
        AT(p, 0x18, f32) = AT(p, 0x18, f32) + k01.f * func_0031C058(*ph);
        if (!(AT(p, 0x14, f32) <= 50.0f)) {
            func_00345490(o, i, 1);
        }
        if (AT(o, 0x1BB0, s32) == 0) {
            f32 w = 1.0f - AT(o, 0x1AB0 + i * 4, f32);
            u32 r = VCALL(rng, 0x10, u32 (*)(VObject *))(rng);

            AT(p, 0xC, s32) = AT(p, 0xC, s32) - ((s32)(20.0f * w * w) + (s32)(r & 3));
            if (AT(p, 0xC, s32) <= 0) {
                func_00345490(o, i, 1);
            }
        }
    }
    n = 0;
    for (i = 0; i < 64; i++) {
        if (AT(SMOKE_PUFF(o, i), 0xC, s32) != 0) {
            n++;
        }
    }
    if (AT(o, 0x1BBC, u8) == 1 && n == 0) {
        return 0;
    }
    AT(o, 0x181C, s32) = n + (VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 7);
    AT(o, 0x181C, s32) = (s32)((f32)AT(o, 0x181C, s32) * AT(o, 0x1BB4, f32));
    return 1;
}

/* +0x18 start: a byte < 0 makes it die down; else spot (byte & 0xF) * 2: 64 puffs and the glow
 * (pinkish white, 20 across, 12 up at the spot; texture group 0x10, cell (0xA0, 0x40)) */
void func_003458A0(u8 *o, const s8 *params) {
    s32 i;

    if (params == NULL) {
        return;
    }
    if (params[0] < 0) {
        AT(o, 0x1BBC, u8) = 1;
        return;
    }
    AT(o, 0x1BB8, s32) = (params[0] & 0xF) * 2;
    for (i = 0; i < 64; i++) {
        func_00345490(o, i, 0);
    }
    AT(o, 0x1880, s64) = -1;
    AT(o, 0x1890, s32) = 0;
    AT(o, 0x1894, s32) = 0;
    AT(o, 0x1898, s32) = 0x19;
    AT(o, 0x189C, s16) = 1;
    AT(o, 0x189E, s16) = 0xA0;
    AT(o, 0x18A0, s16) = 0x40;
    AT(o, 0x18A2, s16) = 0x20;
    AT(o, 0x18A4, s16) = 0x20;
    AT(o, 0x18A6, s16) = 0x200;
    AT(o, 0x18A8, s16) = 0x100;
    AT(o, 0x18AA, u8) = 0x40;
    AT(o, 0x18AB, u8) = 1;
    AT(o, 0x18AC, u8) = 1;
    AT(o, 0x18AD, u8) = 0x10;
    AT(o, 0x18AE, u8) = 0xFF;
    AT(o, 0x1810, s32) = 0x80;
    AT(o, 0x1814, s32) = 0x70;
    AT(o, 0x1818, s32) = 0x70;
    AT(o, 0x181C, s32) = 0x40;
    AT(o, 0x1820, f32) = D_0043B640[AT(o, 0x1BB8, s32)];
    AT(o, 0x1824, f32) = 12.0f;
    AT(o, 0x1828, f32) = D_0043B644[AT(o, 0x1BB8, s32)];
    AT(o, 0x182C, f32) = 1.0f;
    AT(o, 0x1830, f32) = 20.0f;
    AT(o, 0x1834, f32) = AT(o, 0x1830, f32);
    AT(o, 0x1838, s32) = 0;
    AT(o, 0x183C, s32) = 0;
}

/* ---- class D_00479580 (room 0x43, 0x6CF0 bytes): a fire - 128 smoke puffs rising from about
 * (-145.6, 35, 7.3) (+0x10, drawer +0x6040; velocity +0x60E8 + i * 12), 32 flame tongues
 * licking about (-135, 9.5, 11) (+0x3010, drawer +0x6078; per tongue its outward drift
 * +0x66E8, rise +0x68E8 and heading +0x6AE8) and a flickering glow (record +0x6010, drawer
 * +0x60B0); the records double-buffered by +0x6CE8 (0x1800 apart). +0x6CEC set (its start
 * parameter) the fire is out: no smoke or glow, and the flames die away ---- */

extern void *D_00479580[];

#define FIRE_REC(o, base, i) ((o) + AT(o, 0x6CE8, s32) * 0x1800 + (i) * 0x30 + (base))

/* +0x8 destructor (the quad drawers at +0x60B0, +0x6078 and +0x6040 inlined) */
u8 *func_003570C0(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00479580;
    AT(o, 0x60B0, void **) = D_0046FC30;
    AT(o, 0x60B0, void **) = D_00469D00;
    AT(o, 0x6078, void **) = D_0046FC30;
    AT(o, 0x6078, void **) = D_00469D00;
    AT(o, 0x6040, void **) = D_0046FC30;
    AT(o, 0x6040, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* +0xC set up */
void func_00358200(u8 *o) {
    AT(o, 0x6CE8, s32) = 0;
}

/* smoke puff i anew (`again` 0: the first time, part way up already) */
void func_00357630(u8 *o, s32 i, s32 again) {
    static const F32Bits k004 = {0x3D23D70A}, kX = {0xC311999A}, kZ = {0x40E9999A}, k003 = {0x3CF5C28F},
                         k007 = {0x3D8F5C29}, k01 = {0x3DCCCCCD}, kPi = {0x40490FDB};
    VObject *rng = D_0044E550;
    u8 *p = FIRE_REC(o, 0x10, i);
    s32 age = 0;

    AT(p, 0x0, s32) = 0x80;
    AT(p, 0x4, s32) = 0x80;
    AT(p, 0x8, s32) = 0x80;
    AT(p, 0xC, s32) = (VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 0x1F) + 0x20;
    AT(p, 0x20, f32) = 1.5f;
    AT(p, 0x24, f32) = 1.5f;
    if (again == 0) {
        age = (s32)(65.0f * VCALL(rng, 0x18, f32 (*)(VObject *))(rng));
        AT(p, 0xC, s32) = AT(p, 0xC, s32) - (age >> 1);
        if (AT(p, 0xC, s32) < 0) {
            AT(p, 0xC, s32) = 0;
        }
        AT(p, 0x20, f32) = AT(p, 0x24, f32) = 1.5f + k004.f * (f32)age;
    }
    rng = D_0044E550;
#define RND() VCALL(rng, 0x18, f32 (*)(VObject *))(rng)
    AT(p, 0x10, f32) = kX.f + 4.0f * (RND() - 0.5f);
    AT(p, 0x14, f32) = 35.0f + (f32)age;
    AT(p, 0x18, f32) = kZ.f + 4.0f * (RND() - 0.5f);
    AT(p, 0x1C, f32) = 1.0f;
    AT(p, 0x28, f32) = kPi.f * (360.0f * (RND() - 0.5f)) / 180.0f;
    AT(p, 0x2C, s32) = 0;
    AT(o, 0x60E8 + i * 12, f32) = k003.f * (RND() - 0.5f);
    AT(o, 0x60EC + i * 12, f32) = k007.f + k01.f * RND();
    AT(o, 0x60F0 + i * 12, f32) = k003.f * (RND() - 0.5f);
#undef RND
}

/* flame tongue i anew: orange-red, its drift 0.7..0.8, rise 0.1..0.3, heading at random,
 * placed about the fire by its heading */
void func_003571B0(u8 *o, s32 i, s32 again) {
    static const F32Bits k07 = {0x3F333333}, k01 = {0x3DCCCCCD}, k02 = {0x3E4CCCCD}, k001 = {0x3C23D70A},
                         kM0025 = {0xBCCCCCCD}, k005 = {0x3D4CCCCD}, k03 = {0x3E99999A}, kPi = {0x40490FDB};
    VObject *rng = D_0044E550;
    f32 *drift = &AT(o, 0x66E8 + i * 4, f32);
    f32 *rise = &AT(o, 0x68E8 + i * 4, f32);
    f32 *head;
    u8 *p;
    s32 age = 0;
    f32 c, s;

#define RND() VCALL(rng, 0x18, f32 (*)(VObject *))(rng)
    *drift = k07.f + k01.f * RND();
    *rise = k01.f + k02.f * RND();
    AT(o, 0x6AE8 + i * 4, f32) = kPi.f * (360.0f * (RND() - 0.5f)) / 180.0f;
    p = FIRE_REC(o, 0x3010, i);
    AT(p, 0x0, s32) = (VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 0x1F) + 0x80;
    AT(p, 0x4, s32) = 0x40;
    AT(p, 0x8, s32) = 0x10;
    AT(p, 0xC, s32) = (VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 0x3F) + 0x40;
    if (again == 0) {
        age = (s32)RND();
        AT(p, 0xC, s32) = AT(p, 0xC, s32) - (s32)(128.0f * (f32)age);
        if (AT(p, 0xC, s32) < 0) {
            AT(p, 0xC, s32) = 0;
        }
        *drift = *drift + -1.5f * (f32)age;
        if (*drift < k001.f) {
            *drift = k001.f;
        }
        *rise = *rise + kM0025.f * (f32)age;
        if (*rise < k005.f) {
            *rise = k005.f;
        }
    }
    head = &AT(o, 0x6AE8 + i * 4, f32);
    rng = D_0044E550;
    c = func_0031C058(*head);
    AT(p, 0x10, f32) = (-135.0f + (f32)age) + 4.0f * (RND() - 0.5f) + k01.f * c;
    AT(p, 0x14, f32) = (9.5f + (f32)age) + 2.0f * (RND() - 0.5f);
    s = func_0031C248(*head);
    AT(p, 0x18, f32) = 11.0f + 4.0f * (RND() - 0.5f) + k01.f * s;
    AT(p, 0x1C, f32) = 1.0f;
    AT(p, 0x20, f32) = AT(p, 0x24, f32) = k03.f + k02.f * RND();
    AT(p, 0x28, f32) = kPi.f * (360.0f * (RND() - 0.5f)) / 180.0f;
    AT(p, 0x2C, s32) = 0;
#undef RND
}

/* +0x10 update (0 once the fire is out and no flame shows): swap buffers; the smoke grows,
 * turns, drifts and fades (every other frame), starting again above 100 or once faded; the
 * glow flickers (alpha 0x30 / 0x38); each flame fades (and reddens less), wanders (its heading
 * by up to 15 degrees a frame, 5 once out), drifts out by its drift less 0.1 plus |cos| / 10,
 * rises ever slower, and starts again once faded */
s32 func_00357BF0(u8 *o) {
    static const F32Bits k004 = {0x3D23D70A}, kTurn = {0x3D567750}, kPi = {0x40490FDB}, k2Pi = {0x40C90FDB},
                         kM0015 = {0xBC75C28F}, kM0005 = {0xBBA3D70A}, kMinusPi = {0xC0490FDB},
                         kM01 = {0xBDCCCCCD}, k001 = {0x3C23D70A}, k01 = {0x3DCCCCCD}, k005 = {0x3D4CCCCD};
    VObject *rng;
    u8 alive;
    s32 i, k;

    AT(o, 0x6CE8, s32) ^= 1;
    alive = AT(o, 0x6CEC, s32) != 0;
    if (AT(o, 0x6CEC, s32) == 0) {
        rng = D_0044E550;
        for (i = 0; i < 128; i++) {
            u32 buf = AT(o, 0x6CE8, u32);
            u32 *dst = &AT(o, 0x10 + buf * 0x1800 + i * 0x30, u32);
            u32 *src = &AT(o, 0x10 + (buf ^ 1) * 0x1800 + i * 0x30, u32);
            u8 *p;
            f32 t;

            for (k = 0; k < 12; k++) {
                dst[k] = src[k];
            }
            p = FIRE_REC(o, 0x10, i);
            AT(p, 0x20, f32) = AT(p, 0x24, f32) = AT(p, 0x20, f32) + k004.f;
            t = AT(p, 0x28, f32) + kTurn.f;
            AT(p, 0x28, f32) = t;
            if (!(t <= kPi.f)) {
                AT(p, 0x28, f32) = t - k2Pi.f;
            }
            AT(p, 0x10, f32) = AT(p, 0x10, f32) + AT(o, 0x60E8 + i * 12, f32);
            AT(p, 0x14, f32) = AT(p, 0x14, f32) + AT(o, 0x60EC + i * 12, f32);
            AT(p, 0x18, f32) = AT(p, 0x18, f32) + AT(o, 0x60F0 + i * 12, f32);
            if (!(AT(p, 0x14, f32) <= 100.0f)) {
                func_00357630(o, i, 1);
            }
            if (AT(o, 0x6CE8, s32) == 0) {
                AT(p, 0xC, s32) = AT(p, 0xC, s32) - (s32)(VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 3);
            }
            if (AT(p, 0xC, s32) <= 0) {
                func_00357630(o, i, 1);
            }
        }
        AT(o, 0x601C, s32) = AT(o, 0x601C, s32) == 0x30 ? 0x38 : 0x30;
    }
    rng = D_0044E550;
    for (i = 0; i < 32; i++) {
        u32 buf = AT(o, 0x6CE8, u32);
        u32 *dst = &AT(o, 0x3010 + buf * 0x1800 + i * 0x30, u32);
        u32 *src = &AT(o, 0x3010 + (buf ^ 1) * 0x1800 + i * 0x30, u32);
        f32 *drift = &AT(o, 0x66E8 + i * 4, f32);
        f32 *rise = &AT(o, 0x68E8 + i * 4, f32);
        f32 *head = &AT(o, 0x6AE8 + i * 4, f32);
        u8 *p;
        f32 a, d;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        p = FIRE_REC(o, 0x3010, i);
        if (AT(p, 0xC, s32) > 0) {
            AT(p, 0xC, s32) = AT(p, 0xC, s32) - (s32)((VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 3) + 2);
            AT(p, 0x0, s32) = AT(p, 0x0, s32) - (s32)(VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 1);
            if (AT(p, 0x0, s32) < 0) {
                AT(p, 0x0, s32) = 0;
            }
        }
        if (AT(p, 0xC, s32) <= 0) {
            if (AT(o, 0x6CEC, s32) != 0) {
                AT(p, 0xC, s32) = 0;
            } else {
                func_003571B0(o, i, 1);
                alive = 0;
            }
            continue;
        }
        alive = 0;
        if (AT(o, 0x6CEC, s32) == 0) {
            *rise = *rise + kM0015.f;
            *head = *head + kPi.f * (30.0f * (VCALL(rng, 0x18, f32 (*)(VObject *))(rng) - 0.5f)) / 180.0f;
        } else {
            *rise = *rise + kM0005.f;
            *head = *head + kPi.f * (10.0f * (VCALL(rng, 0x18, f32 (*)(VObject *))(rng) - 0.5f)) / 180.0f;
        }
        a = *head;
        if (a < kMinusPi.f) {
            *head = a + k2Pi.f;
        } else if (!(a <= kPi.f)) {
            *head = *head - k2Pi.f;
        }
        *drift = *drift + kM01.f;
        if (*drift < k001.f) {
            *drift = k001.f;
        }
        if (!(k01.f * func_0031C058(*head) <= 0.0f)) {
            d = k01.f * func_0031C058(*head);
        } else {
            d = -(k01.f * func_0031C058(*head));
        }
        AT(p, 0x10, f32) = AT(p, 0x10, f32) + (*drift + d);
        if (*rise < k005.f) {
            *rise = k005.f;
        }
        AT(p, 0x14, f32) = AT(p, 0x14, f32) + *rise;
        AT(p, 0x18, f32) = AT(p, 0x18, f32) + k01.f * func_0031C248(*head);
    }
    return alive != 1;
}

/* +0x14 draw: (while it burns) the smoke and the glow, then the flames */
void func_00357B60(u8 *o) {
    if (AT(o, 0x6CEC, s32) == 0) {
        AT(o, 0x6050, u8 *) = FIRE_REC(o, 0x10, 0);
        func_002E56C0(o + 0x6040);
        AT(o, 0x60C0, u8 *) = o + 0x6010;
        func_002E56C0(o + 0x60B0);
    }
    AT(o, 0x6088, u8 *) = FIRE_REC(o, 0x3010, 0);
    func_002E56C0(o + 0x6078);
}

/* +0x18 start: params[0] is the out flag; lit, the three drawers (texture group 0x10: smoke
 * cell (0x80, 0) 64 x 32 x 0x20, flames (0x6C, 0x4C) 8 x 8 additive, glow (0xA0, 0x40) 32 x 32
 * additive), 128 puffs, 32 tongues, and the glow (orange, 5 across at (-128, 5, 14)) */
void func_00357940(u8 *o, const s32 *params) {
    s32 i;

    if (params == NULL) {
        return;
    }
    AT(o, 0x6CEC, s32) = params[0];
    if (AT(o, 0x6CEC, s32) != 0) {
        return;
    }
    AT(o, 0x6048, s64) = -1;
    AT(o, 0x6054, s32) = 0;
    AT(o, 0x6058, s32) = 0;
    AT(o, 0x605C, s32) = 0;
    AT(o, 0x6060, s32) = 0x19;
    AT(o, 0x6064, s16) = 0x80;
    AT(o, 0x6066, s16) = 0;
    AT(o, 0x6068, s16) = 0x40;
    AT(o, 0x606A, s16) = 0x20;
    AT(o, 0x606C, s16) = 0x20;
    AT(o, 0x606E, s16) = 0x200;
    AT(o, 0x6070, s16) = 0x100;
    AT(o, 0x6072, u8) = 0;
    AT(o, 0x6073, u8) = 1;
    AT(o, 0x6074, u8) = 1;
    AT(o, 0x6075, u8) = 0x10;
    AT(o, 0x6076, u8) = 0xFF;
    for (i = 0; i < 128; i++) {
        func_00357630(o, i, 0);
    }
    AT(o, 0x6080, s64) = -1;
    AT(o, 0x608C, s32) = 0;
    AT(o, 0x6090, s32) = 0;
    AT(o, 0x6094, s32) = 0;
    AT(o, 0x6098, s32) = 0x19;
    AT(o, 0x609C, s16) = 0x20;
    AT(o, 0x609E, s16) = 0x6C;
    AT(o, 0x60A0, s16) = 0x4C;
    AT(o, 0x60A2, s16) = 8;
    AT(o, 0x60A4, s16) = 8;
    AT(o, 0x60A6, s16) = 0x200;
    AT(o, 0x60A8, s16) = 0x100;
    AT(o, 0x60AA, u8) = 0x40;
    AT(o, 0x60AB, u8) = 1;
    AT(o, 0x60AC, u8) = 1;
    AT(o, 0x60AD, u8) = 0x10;
    AT(o, 0x60AE, u8) = 0xFF;
    for (i = 0; i < 32; i++) {
        func_003571B0(o, i, 0);
    }
    AT(o, 0x60B8, s64) = -1;
    AT(o, 0x60C8, s32) = 0;
    AT(o, 0x60CC, s32) = 0;
    AT(o, 0x60D0, s32) = 0x19;
    AT(o, 0x60D4, s16) = 1;
    AT(o, 0x60D6, s16) = 0xA0;
    AT(o, 0x60D8, s16) = 0x40;
    AT(o, 0x60DA, s16) = 0x20;
    AT(o, 0x60DC, s16) = 0x20;
    AT(o, 0x60DE, s16) = 0x200;
    AT(o, 0x60E0, s16) = 0x100;
    AT(o, 0x60E2, u8) = 0x40;
    AT(o, 0x60E3, u8) = 1;
    AT(o, 0x60E4, u8) = 1;
    AT(o, 0x60E5, u8) = 0x10;
    AT(o, 0x60E6, u8) = 0xFF;
    AT(o, 0x6010, s32) = 0x70;
    AT(o, 0x6014, s32) = 0x40;
    AT(o, 0x6018, s32) = 0x40;
    AT(o, 0x601C, s32) = 0x30;
    AT(o, 0x6020, f32) = -128.0f;
    AT(o, 0x6024, f32) = 5.0f;
    AT(o, 0x6028, f32) = 14.0f;
    AT(o, 0x602C, f32) = 1.0f;
    AT(o, 0x6030, f32) = 5.0f;
    AT(o, 0x6034, f32) = 5.0f;
    AT(o, 0x6038, s32) = 0;
    AT(o, 0x603C, s32) = 0;
}
