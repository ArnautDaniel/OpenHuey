/* Leaf functions, batch 4 (func_002DCA70..func_002E6040). */
#include "common.h"
#include "ptmf.h"

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

/* float from its bit pattern */
static inline f32 B4_FLT(u32 bits) {
    union { u32 u; f32 f; } c;
    c.u = bits;
    return c.f;
}

/* gFileLoader->vfunc_0xC(name, dest, 0x4000000, 0): start loading a file */
#define LOAD(name, dest) \
    VCALL(gFileLoader, 0xC, s32 (*)(void *, const char *, void *, s32, s32))(gFileLoader, name, dest, 0x4000000, 0)

extern u8 *gProgress;
extern u8 D_0046FB50[], D_00469C60[], D_00469C20[];
extern u8 D_00416A50[];
extern u8 D_00416AF0[];
extern u8 D_00416B90[];
extern u8 D_00416D60[];
extern u8 D_00416DB0[];
extern void *D_00417120[];
extern u8 D_00417170[];
extern void *D_0047AC48[];
extern u8 D_004174B0[];
extern u8 D_004174C0[];
extern u8 D_004175D0[];
extern u8 D_00417690[];
extern void *D_00417800[];
extern u8 D_00417830[];
extern void *D_00417820[];
extern u8 D_00417840[];
extern u8 D_00417850[];
extern u8 D_00417960[];
extern u8 D_00417A20[];

static void b4_clear_dca70(u8 *p) {
    s32 i;

    F(p, 0x38, u32) = 0;
    F(p, 0x3C, u32) = 0;
    F(p, 0x40, u32) = 0;
    F(p, 0x48, u32) = 0;
    F(p, 0x4C, u32) = 0;
    F(p, 0x50, u32) = 0;
    for (i = 0; i < 16; i++) {
        F(p, 0x58 + i * 4, u32) = 0;
    }
    F(p, 0x870, u32) = 0;
}

void func_002DCAE0(u8 *p) {
    b4_clear_dca70(p);
    p[0x30] = 1;
}

/* Moves the two floats at +0x854/+0x858 toward 0 by |step|. */
void func_002DCF10(u8 *p, f32 step) {
    f32 v, a;

    if (step <= 0.0f) step = -step;
    v = F(p, 0x854, f32);
    a = v <= 0.0f ? -v : v;
    if (a <= step) {
        F(p, 0x854, f32) = 0.0f;
    } else if (v <= 0.0f) {
        F(p, 0x854, f32) += step;
    } else {
        F(p, 0x854, f32) -= step;
    }
    v = F(p, 0x858, f32);
    a = v <= 0.0f ? -v : v;
    if (a <= step) {
        F(p, 0x858, f32) = 0.0f;
    } else if (v <= 0.0f) {
        F(p, 0x858, f32) += step;
    } else {
        F(p, 0x858, f32) -= step;
    }
}

/* Moves the floats at +0x854/+0x858 toward (tx, ty) by at most |sx|/|sy|. */
void func_002DD310(u8 *p, f32 tx, f32 ty, f32 sx, f32 sy) {
    f32 v, d;

    if (sx <= 0.0f) sx = -sx;
    v = F(p, 0x854, f32);
    d = tx - v;
    if (d <= 0.0f) d = -d;
    if (d <= sx) {
        F(p, 0x854, f32) = tx;
    } else if (tx <= v) {
        F(p, 0x854, f32) -= sx;
    } else {
        F(p, 0x854, f32) += sx;
    }
    if (sy <= 0.0f) sy = -sy;
    v = F(p, 0x858, f32);
    d = ty - v;
    if (d <= 0.0f) d = -d;
    if (d <= sy) {
        F(p, 0x858, f32) = ty;
    } else if (ty <= v) {
        F(p, 0x858, f32) -= sy;
    } else {
        F(p, 0x858, f32) += sy;
    }
}

f32 func_002DD970(void) { return 1.0f; }

f32 func_002DDAB0(void) { return 0.0f; }

void func_002DE510(u8 *p, s32 a1, u32 v) {
    F(p, 0x154C, s32) = v < 3 ? (s32)v : -1;
}

s32 func_002DE830(u8 *p) { return p[0x156E] != 0; }

/* Saves this object's state into a 36-byte slot of the progress block (+0x878). */
void func_002DE840(u8 *p, s32 slot, u32 v) {
    u8 *e = gProgress + slot * 36;

    F(e, 0x878, u32) = F(p, 0x30, u32);
    F(e, 0x87C, u32) = F(p, 0x154C, u32);
    F(e, 0x880, u32) = F(p, 0x34, u32);
    F(e, 0x884, s16) = F(p, 0x1560, s16);
    e[0x886] = p[0x1571];
    e[0x887] = F(p, 0x1544, s16) / 10;
    e[0x888] = p[0x15C2];
    e[0x889] = p[0x156E];
    e[0x88A] = (u8)v;
    e[0x88B] = p[0x15C7];
    e[0x88C] = p[0x15DA];
    F(e, 0x890, u32) = 0;
    F(e, 0x894, u32) = 0;
    F(e, 0x898, u32) = 0;
}

/* Inlined destructor chain: resets the vtable to each base class in turn. */
void *func_002E2220(u8 *p) {
    if (p != NULL) {
        *(void *volatile *)p = D_0046FB50;
        *(void *volatile *)p = D_00469C60;
        *(void *volatile *)p = D_00469C20;
    }
    return p;
}

u32 func_002E2340(u8 *p) { return p[0x38680]; }

void *func_002E2350(u8 *p) { return p + 0xF680; }

u32 func_002E25F0(u8 *p, u32 i) { return ((u32 *)p)[(u8)i]; }

/* tail call: member at +0xF630, virtual slot 0x10, with 0x890 */
s32 func_002E2610(u8 *p, s32 a1, s32 a2, s32 a3) {
    u8 *m = p + 0xF630;
    return VCALL(m, 0x10, s32 (*)(void *, s32, s32, s32))(m, 0x890, a2, a3);
}

/* tail call: member at +0xDC40, virtual slot 0x10 */
s32 func_002E2630(u8 *p, s32 a1, s32 a2, s32 a3) {
    u8 *m = p + 0xDC40;
    return VCALL(m, 0x10, s32 (*)(void *, s32, s32, s32))(m, a1, a2, a3);
}

/* Wraps an angle into [-pi, pi]. */
f32 func_002E2D00(f32 a) {
    const f32 pi = B4_FLT(0x40490FDB);
    const f32 twopi = B4_FLT(0x40C90FDB);

    while (!(a <= pi)) {
        a -= twopi;
    }
    while (a < -pi) {
        a += twopi;
    }
    return a;
}

void func_002E34D0(u8 *p) {
    p[0x5] = 0xFF;
    p[0x4] = 0xFF;
    F(p, 0x8, u32) = 0;
    F(p, 0x10, f32) = 1.0f;
    F(p, 0xC, u32) = 0;
    F(p, 0x14, u32) = 0;
}

void *func_002E57A0(void) { return D_00416A50; }

void *func_002E57B0(void) { return D_00416AF0; }

void *func_002E57C0(void) { return D_00416B90; }

void *func_002E57D0(void) { return D_00416D60; }

void *func_002E57E0(void) { return D_00416DB0; }

void *func_002E5800(void *self, s32 i) { return D_00417120[i]; }

void *func_002E5820(void) { return D_00417170; }

void *func_002E5830(void *self, s32 i) { return D_0047AC48[i]; }

void *func_002E5BA0(void) { return D_004174B0; }

void *func_002E5BB0(void) { return D_004174C0; }

void *func_002E5BC0(void) { return D_004175D0; }

void *func_002E5BD0(void) { return D_00417690; }

void *func_002E5BE0(void *self, s32 i) { return D_00417800[i]; }

void *func_002E5C00(void) { return D_00417830; }

void *func_002E5C10(void *self, s32 i) { return D_00417820[i]; }

void *func_002E6010(void) { return D_00417840; }

void *func_002E6020(void) { return D_00417850; }

void *func_002E6030(void) { return D_00417960; }

void *func_002E6040(void) { return D_00417A20; }
