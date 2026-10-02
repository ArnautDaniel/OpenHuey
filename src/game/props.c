/* Objects the room scripts spawn into the effect manager (SceneGame +0xF6E200 slots). */
#include "common.h"
#include "game.h"
#include "sce/libvu0.h"

extern VObject *D_0044E550;   /* the random number generator */

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

/* a model to draw this frame: position, ..., angles, model, colour */
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
