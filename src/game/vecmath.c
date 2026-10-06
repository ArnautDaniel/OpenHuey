/* Quaternions (x, y, z, w).
 *
 * (was spline.c) Key splines (the room cameras' paths): +0x0 the current time, +0x4 the keys (32
 * bytes: time, value, ..., one row of n keys per component), +0x8 n, +0xC / +0x10 the first /
 * last time, +0x14 the current segment, +0x18 the components. Values are cubic Bezier segments
 * with the control points value, value + out (+0x14) and next value + in (+0x2C).
 *
 * (was stalker_math.c) Heading helpers the pursuers reach (0x211A30.., 0x2E2BC0..): the game's
 * "heading" is a turn about Y, 0 facing +Z. Generic (other characters use them too); kept here on
 * the stalkers branch.
 *
 * (was random.c) Random number generator (Mersenne Twister; vtable around 0x46AB60, global
 * gRandom).
 */
#include "common.h"
#include "vecmath.h"
#include "msl.h"
#include "fiona.h"
#include "progress.h"
#include "navmesh.h"
#include "globals.h"
#include "actor.h"
#include "ptmf.h"
#include "memcard.h"
#include "hewie.h"
#include "model.h"
#include "pursuer.h"
#include "scene_game.h"
#include "heap.h"
#include "game.h"
#include "charaction.h"
#include "input.h"
#include "gl2d.h"
#include "effects.h"
#include "renderer.h"
#include "sound.h"
#include "creature.h"
#include "effectmgr.h"
#include "libc.h"
#include "effectmgr.h"   /* HitEffect_Spawn */
#include "event.h"
#include "lights.h"
#include "debilitas.h"
#include "sce/libvu0.h"
#ifdef HG_NATIVE
#include <stdio.h>
#include <stdlib.h>
#include "glr.h"
#endif

extern const char *const pstr_kibako, *const pstr_a_koushi;   /* "kibako" (the box), "a_koushi" (the grate) */
void Mtx_ApplyVector(f32 *out, f32 (*m)[4], const f32 *v);

extern void *Random_vtable[];
extern void *D_0046AB80[];
#define DBL_2POW26 0x4190000000000000ULL     /* 67108864.0 */

#define DBL_2POWM53 0x3CA0000000000000ULL    /* 1.0 / 9007199254740992.0 */

/* MT19937 state: mt[624] at +4, next output at +0x9C4, outputs left at +0x9C8. */
#define MT_N 624

#define MT_M 397

#define MT(r) ((u32 *)((u8 *)(r) + 4))

#define MT_NEXT(r) (*(u32 **)((u8 *)(r) + 0x9C4))

#define MT_LEFT(r) (*(s32 *)((u8 *)(r) + 0x9C8))

#define MT_TWIST(u, v) ((((u) & 0x80000000u) | ((v) & 0x7FFFFFFFu)) >> 1 ^ ((v) & 1 ? 0x9908B0DFu : 0))

#define MT_INT32(r) VCALL(r, 0x10, u32 (*)(VObject *))(r)

#define DBL_2POW32 0x41F0000000000000ULL     /* 4294967296.0 */

#define DBL_2POW32M1 0x41EFFFFFFFE00000ULL   /* 4294967295.0 */

#define DBL_HALF 0x3FE0000000000000ULL

u64 Random_Res53(VObject *rng);
VObject *Random_ctor(VObject *rng, s32 seed);
void Random_Seed(VObject *r, u32 seed);
void Random_Refill(VObject *r);
u32 Random_Int32(VObject *r);
u32 Random_Int31(VObject *r);
f32 Random_Real1(VObject *r);
f32 Random_Real2(VObject *r);
f32 Random_Real3(VObject *r);
VObject *Random_dtor(VObject *r, s32 flags);
void *RandomBase_dtor(void *o, s32 flags);

void Spline_SeekTo(u8 *s, f32 *t, s32 *seg, f32 u);
void Spline_Seek(u8 *s, f32 u);
void Spline_Init(u8 *s, s32 dims, s32 n, f32 *keys);
f32 Spline_Eval(u8 *s, s32 comp, f32 u);

void Mtx_Model(f32 (*mtx)[4], const f32 *pos, f32 heading);
void Mtx_FrameKeepZ(f32 (*out)[4], f32 (*b)[4], const f32 *up);
void Mtx_TurnTwo(f32 (*out)[4], f32 (*axes)[4], f32 a, f32 b);

/* wrap an angle into -pi..pi (in place) */
static void wrap_pi(f32 *a) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kTwoPi = {0x40C90FDB}, kNegPi = {0xC0490FDB};
    f32 v = *a;

    while (!(v <= kPi.f)) {
        v -= kTwoPi.f;
    }
    while (v < kNegPi.f) {
        v += kTwoPi.f;
    }
    *a = v;
}

void Mtx_FromEuler(void *mat, f32 *rot, const f32 *trans);

/* float from its bit pattern */
static inline f32 B4_FLT(u32 bits) {
    union { u32 u; f32 f; } c;
    c.u = bits;
    return c.f;
}

/* genrand_res53: uniform double in [0, 1) with 53-bit resolution, from two 32-bit outputs
 * (vtable +0x10). Returned as a double in v0. */
/* 0x001A43D0 */
u64 Random_Res53(VObject *rng) {
    u32 a = VCALL(rng, 0x10, u32 (*)(VObject *))(rng) >> 5;
    u32 b = VCALL(rng, 0x10, u32 (*)(VObject *))(rng) >> 6;
    u64 x = sf_muldf3(DBL_2POW26, sf_floatunsidf(a));

    return sf_muldf3(sf_adddf3(x, sf_floatunsidf(b)), DBL_2POWM53);
}

/* +0x20 genrand_real3: (0, 1) */
/* 0x001A4460 */
f32 Random_Real3(VObject *r) {
    return sf_truncdfsf2(sf_divdf3(sf_adddf3(DBL_HALF, sf_floatunsidf(MT_INT32(r))), DBL_2POW32));
}

/* +0x1C genrand_real2: [0, 1) (RNG01() in the game code) */
/* 0x001A44C0 */
f32 Random_Real2(VObject *r) {
    return sf_truncdfsf2(sf_divdf3(sf_floatunsidf(MT_INT32(r)), DBL_2POW32));
}

/* +0x18 genrand_real1: [0, 1] */
/* 0x001A4510 */
f32 Random_Real1(VObject *r) {
    return sf_truncdfsf2(sf_divdf3(sf_floatunsidf(MT_INT32(r)), DBL_2POW32M1));
}

/* +0x14 genrand_int31 */
/* 0x001A4570 */
u32 Random_Int31(VObject *r) {
    return MT_INT32(r) >> 1;
}

/* +0x10 genrand_int32 */
/* 0x001A45A0 */
u32 Random_Int32(VObject *r) {
    u32 y;

    Random_Refill(r);
    y = *MT_NEXT(r)++;
    y ^= y >> 11;
    y ^= (y << 7) & 0x9D2C5680u;
    y ^= (y << 15) & 0xEFC60000u;
    return y ^ (y >> 18);
}

/* +0xC init_genrand(seed) */
/* 0x001A4630 */
void Random_Seed(VObject *r, u32 seed) {
    u32 *mt = MT(r);
    u32 j;

    mt[0] = seed;
    for (j = 1; j < MT_N; j++) {
        mt[j] = 1812433253u * (mt[j - 1] ^ (mt[j - 1] >> 30)) + j;
    }
    MT_LEFT(r) = 1;
}

/* refill the state once all outputs are used */
/* 0x001A46D0 */
void Random_Refill(VObject *r) {
    u32 *p;
    s32 j;

    if (--MT_LEFT(r) != 0) {
        return;
    }
    p = MT(r);
    MT_LEFT(r) = MT_N;
    MT_NEXT(r) = p;
    for (j = MT_N - MT_M; j != 0; j--, p++) {
        *p = p[MT_M] ^ MT_TWIST(p[0], p[1]);
    }
    for (j = MT_M - 1; j != 0; j--, p++) {
        *p = p[MT_M - MT_N] ^ MT_TWIST(p[0], p[1]);
    }
    *p = p[MT_M - MT_N] ^ MT_TWIST(p[0], MT(r)[0]);
}

/* +0x8 destructor */
/* 0x001A4850 */
VObject *Random_dtor(VObject *r, s32 flags) {
    if (r != NULL) {
        r->vtbl = Random_vtable;
        r->vtbl = D_0046AB80;
        gRandom = NULL;
        if ((s16)flags > 0) {
            __dl__FPv(r);
        }
    }
    return r;
}

/* constructor: register, seed (vtable +0xC) */
/* 0x001A48C0 */
VObject *Random_ctor(VObject *rng, s32 seed) {
    rng->vtbl = Random_vtable;
    gRandom = rng;
    VCALL(rng, 0xC, void (*)(VObject *, s32))(rng, seed);
    return rng;
}

/* +0x8 destructor (the global goes) */
/* 0x001A4910 */
void *RandomBase_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AB80;
        if (o != NULL) {
            gRandom = NULL;
        }
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* ---- matrices of the model base used by Lorenzo's ---- */

/* a frame from bone matrix `b` keeping its Z axis (flipped to face `up`'s side) with Y = `up` */
/* 0x001F93E0 */
void Mtx_FrameKeepZ(f32 (*out)[4], f32 (*b)[4], const f32 *up) {
    sceVu0CopyVector(out[2], b[2]);
    out[2][1] = 0.0f;
    if (sceVu0InnerProduct(b[1], up) < 0.0f) {
        sceVu0ScaleVector(out[2], out[2], -1.0f);
    }
    sceVu0CopyVector(out[1], up);
    sceVu0OuterProduct(out[0], out[1], out[2]);
    sceVu0OuterProduct(out[2], out[0], out[1]);
    sceVu0Normalize(out[0], out[0]);
    sceVu0Normalize(out[1], out[1]);
    sceVu0Normalize(out[2], out[2]);
}

/* `out` turned `a` about axes[0] then `b` about axes[1] (its translation cleared) */
/* 0x001F94B0 */
void Mtx_TurnTwo(f32 (*out)[4], f32 (*axes)[4], f32 a, f32 b) {
    f32 q[4] __attribute__((aligned(16)));
    f32 ra[4][4] __attribute__((aligned(16)));
    f32 rb[4][4] __attribute__((aligned(16)));
    f32 r[4][4] __attribute__((aligned(16)));
    f32 t[4] __attribute__((aligned(16)));

    q[3] = 0.0f;
    q[2] = 0.0f;
    q[1] = 0.0f;
    q[0] = 0.0f;
    Quat_FromAxisAngle(q, axes[0], a);
    Quat_ToMatrix(q, ra);
    Quat_FromAxisAngle(q, axes[1], b);
    Quat_ToMatrix(q, rb);
    sceVu0MulMatrix(r, rb, ra);
    sceVu0CopyVector(t, out[3]);   /* (kept by the original, unused) */
    out[3][0] = 0.0f;
    out[3][1] = 0.0f;
    out[3][2] = 0.0f;
    sceVu0MulMatrix(out, r, out);
}
/* the distance of point `c` from the line through `a` and `b` */
/* 0x00211910 */
f32 Vec_LineDistance(const f32 *a, const f32 *b, const f32 *c) {
    f32 d[4] __attribute__((aligned(16)));
    f32 p[4] __attribute__((aligned(16)));
    f32 r[4] __attribute__((aligned(16)));
    f32 l1, l2, s;

    sceVu0SubVector(d, (f32 *)b, (f32 *)a);
    vu0_ScaleXYZ(p, b, -1.0f);
    l1 = ee_sqrtf(sceVu0InnerProduct(d, d));
    l2 = ee_sqrtf(sceVu0InnerProduct(d, d));
    s = sceVu0InnerProduct(p, d);
    s = (s + sceVu0InnerProduct((f32 *)c, d)) / (l2 * l1);
    vu0_ScaleXYZ(p, d, s);
    sceVu0AddVector(p, p, (f32 *)b);
    sceVu0SubVector(r, (f32 *)c, p);
    r[3] = 0.0f;
    return ee_sqrtf(sceVu0InnerProduct(r, r));
}

/* `from` + `dist` along heading `angle` */
/* 0x00211A30 */
void Vec_Along(f32 *out, f32 *from, f32 angle, f32 dist) {
    f32 d[4] __attribute__((aligned(16)));

    Heading_Vector(d, angle);
    sceVu0ScaleVector(d, d, dist);
    sceVu0AddVector(out, from, d);
}
/* the rotation by `angle` about the unit `axis` */
/* 0x0025C6F0 */
void Quat_FromAxisAngle(f32 *q, const f32 *axis, f32 angle) {
    f32 h = 0.5f * angle;
    f32 s = msl_sinf(h);

    q[0] = axis[0] * s;
    q[1] = axis[1] * s;
    q[2] = axis[2] * s;
    q[3] = msl_cosf(h);
}

/* its 3 x 3 rotation matrix (the translation row is left as it is) */
/* 0x0025C770 */
void Quat_ToMatrix(const f32 *q, f32 (*m)[4]) {
    f32 x = q[0], y = q[1], z = q[2], w = q[3];

    m[0][0] = 1.0f + -2.0f * (y * y + z * z);
    m[0][1] = 2.0f * (x * y - z * w);
    m[0][2] = 2.0f * (x * z + y * w);
    m[1][0] = 2.0f * (x * y + z * w);
    m[1][1] = 1.0f + -2.0f * (x * x + z * z);
    m[1][2] = 2.0f * (y * z - x * w);
    m[2][0] = 2.0f * (x * z - y * w);
    m[2][1] = 2.0f * (y * z + x * w);
    m[2][2] = 1.0f + -2.0f * (x * x + y * y);
    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
    m[3][3] = 1.0f;
}

/* component `comp` at time u (or at the current time if u is outside the keys) */
/* 0x0025F580 */
f32 Spline_Eval(u8 *s, s32 comp, f32 u) {
    f32 t = AT(s, 0x0, f32);
    s32 seg = AT(s, 0x14, s32);
    f32 *p;
    f32 x, y, a, a3, b2, b1;

    Spline_SeekTo(s, &t, &seg, u);
    if (comp < 0 || comp >= AT(s, 0x18, s32)) {
        return 0.0f;
    }
    p = AT(s, 0x4, f32 *) + (seg + comp * AT(s, 0x8, s32)) * 8;
    if (t == p[0]) {
        return p[1];
    }
    if (t <= p[0]) {
        return 0.0f;
    }
    x = (t - p[0]) / (p[8] - p[0]);
    y = 1.0f - x;
    a = 3.0f * y;
    a3 = x * (x * x);
    b2 = x * (a * x);
    b1 = x * (a * y);
    return ((p[1] + p[5]) * b1 + p[1] * (y * (y * y))) + (p[9] + p[11]) * b2 + p[9] * a3;
}

/* move the spline's own time to u */
/* 0x0025F6A0 */
void Spline_Seek(u8 *s, f32 u) {
    Spline_SeekTo(s, (f32 *)s, (s32 *)(s + 0x14), u);
}

/* move to time u (if within the keys): *t = u, *seg = the segment holding it */
/* 0x0025F6B0 */
void Spline_SeekTo(u8 *s, f32 *t, s32 *seg, f32 u) {
    f32 *p;
    s32 k;

    if (u < (f32)AT(s, 0xC, s32) || !(u <= (f32)AT(s, 0x10, s32))) {
        return;
    }
    *t = u;
    k = *seg;
    p = AT(s, 0x4, f32 *) + k * 8;
    for (;;) {
        f32 p0 = p[0];

        if (k + 1 < AT(s, 0x8, s32)) {
            if (!(u < p0) && u < p[8]) {
                return;
            }
        } else if (p0 == u) {
            return;
        }
        if (u < p0) {
            (*seg)--;
            p -= 8;
        }
        if (!(u < p[8])) {
            (*seg)++;
            p += 8;
        }
        k = *seg;
    }
}

/* set up: n keys of `dims` components from `keys` */
/* 0x0025F7A0 */
void Spline_Init(u8 *s, s32 dims, s32 n, f32 *keys) {
    AT(s, 0x0, s32) = 0;
    AT(s, 0x4, s32) = 0;
    AT(s, 0x8, s32) = 0;
    AT(s, 0xC, s32) = 0;
    AT(s, 0x10, s32) = 0;
    AT(s, 0x14, s32) = 0;
    AT(s, 0x18, s32) = 0;
    AT(s, 0x8, s32) = n;
    AT(s, 0x4, f32 *) = keys;
    AT(s, 0xC, s32) = (s32)keys[0];
    AT(s, 0x10, s32) = (s32)keys[n * 8 - 8];
    AT(s, 0x0, f32) = (f32)AT(s, 0xC, s32);
    AT(s, 0x18, s32) = dims;
}

/* the heading of `v` (10, out of range, when it has no X/Z) */
/* 0x002E2BC0 */
f32 Vec_Heading(const f32 *v) {
    if (v[0] == 0.0f && v[2] == 0.0f) {
        return 10.0f;
    }
    return msl_atan2f(v[0], v[2]);
}

/* the unit vector of heading `angle` */
/* 0x002E2C10 */
void Heading_Vector(f32 *out, f32 angle) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 fwd[4] __attribute__((aligned(16)));

    fwd[0] = 0.0f;
    fwd[1] = 0.0f;
    fwd[2] = 1.0f;
    fwd[3] = 0.0f;
    sceVu0UnitMatrix(m);
    sceVu0RotMatrixY(m, m, angle);
    sceVu0ApplyMatrix(out, m, fwd);
}

/* `v` turned by `angle` about Y */
/* 0x002E2CA0 */
void Vec_TurnY(f32 *out, f32 *v, f32 angle) {
    f32 m[4][4] __attribute__((aligned(16)));

    sceVu0UnitMatrix(m);
    sceVu0RotMatrixY(m, m, angle);
    sceVu0ApplyMatrix(out, m, v);
}

/* Wraps an angle into [-pi, pi]. */
/* 0x002E2D00 */
f32 Angle_Wrap(f32 a) {
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

/* out.xyz = m's 3x3 part x v. (VU0 macro code: its w is whatever the VU register last held;
   0 here - the callers only use x, y, z) */
/* 0x002E2DA0 */
void Mtx_ApplyVector(f32 *out, f32 (*m)[4], const f32 *v) {
    f32 x = v[0], y = v[1], z = v[2];
    s32 i;

    for (i = 0; i < 3; i++) {
        out[i] = m[0][i] * x + m[1][i] * y + m[2][i] * z;
    }
    *(s32 *)&out[3] = 0;
}

/* out.xyz = m x v (v as a point: plus m's translation row; w 0 as above) */
/* 0x002E2DD0 */
void Mtx_ApplyPoint(f32 *out, f32 (*m)[4], const f32 *v) {
    f32 x = v[0], y = v[1], z = v[2];
    s32 i;

    for (i = 0; i < 3; i++) {
        out[i] = m[0][i] * x + m[1][i] * y + m[2][i] * z + m[3][i];
    }
    *(s32 *)&out[3] = 0;
}

/* a bone matrix from Euler angles `rot` (X, then Y, then Z; wrapped into -pi..pi in place) and
 * the position `trans` */
/* 0x002E2E00 */
void Mtx_FromEuler(void *mat, f32 *rot, const f32 *trans) {
    f32 (*m)[4] = mat;

    wrap_pi(&rot[0]);
    wrap_pi(&rot[1]);
    wrap_pi(&rot[2]);
    sceVu0UnitMatrix(m);
    sceVu0RotMatrixX(m, m, rot[0]);
    sceVu0RotMatrixY(m, m, rot[1]);
    sceVu0RotMatrixZ(m, m, rot[2]);
    sceVu0TransMatrix(m, m, (f32 *)trans);
}

/* a model matrix: the heading (wrapped to -pi..pi) about Y, at `pos` */
/* 0x002E3040 */
void Mtx_Model(f32 (*mtx)[4], const f32 *pos, f32 heading) {
    if (!(heading <= 0x1.921fb6p+1f)) {
        do {
            heading -= 0x1.921fb6p+2f;
        } while (!(heading <= 0x1.921fb6p+1f));
    }
    if (heading < -0x1.921fb6p+1f) {
        do {
            heading += 0x1.921fb6p+2f;
        } while (heading < -0x1.921fb6p+1f);
    }
    sceVu0UnitMatrix(mtx);
    sceVu0RotMatrixY(mtx, mtx, heading);
    sceVu0TransMatrix(mtx, mtx, pos);
}

/* the matrix turned to heading `angle`, at `pos` */
/* 0x002E3130 */
void Mtx_AtHeading(f32 (*m)[4], f32 *pos, f32 angle) {
    sceVu0UnitMatrix(m);
    sceVu0RotMatrixY(m, m, angle);
    sceVu0TransMatrix(m, m, pos);
}

/* `m` = a turn of `a` about Y */
/* 0x002E3190 */
void Mtx_TurnY(f32 (*m)[4], f32 a) {
    sceVu0UnitMatrix(m);
    sceVu0RotMatrixY(m, m, a);
}


/* blend bone matrix `a` towards `b` by `t` into `out`: the rotation from a to b as an axis and
 * angle, turned by t of it (the quaternion left in `q`), the positions mixed */
/* 0x0025C440 */
void Bone_BlendMatrix(f32 *q, f32 (*out)[4], f32 (*a)[4], f32 (*b)[4], f32 t) {
    f32 axis[4] __attribute__((aligned(16)));
    f32 tb[4] __attribute__((aligned(16)));
    f32 ta[4] __attribute__((aligned(16)));
    f32 p[4] __attribute__((aligned(16)));
    f32 r[4][4] __attribute__((aligned(16)));
    f32 at[4][4] __attribute__((aligned(16)));
    f32 ra[4][4] __attribute__((aligned(16)));
    f32 h, s;

    sceVu0CopyVector(ta, a[3]);
    sceVu0CopyVector(tb, b[3]);
    sceVu0CopyMatrix(ra, a);
    ra[3][0] = 0.0f;
    ra[3][1] = 0.0f;
    ra[3][2] = 0.0f;
    sceVu0TransposeMatrix(at, ra);
    sceVu0CopyMatrix(r, b);
    r[3][0] = 0.0f;
    r[3][1] = 0.0f;
    r[3][2] = 0.0f;
    sceVu0MulMatrix(r, r, at);
    axis[0] = r[2][1] - r[1][2];
    axis[1] = r[0][2] - r[2][0];
    axis[2] = r[1][0] - r[0][1];
    sceVu0Normalize(axis, axis);
    axis[3] = 2.0f * msl_acosf(0.5f * __builtin_sqrtf(1.0f + (r[2][2] + (r[0][0] + r[1][1]))));
    h = 0.5f * (axis[3] * t);
    s = msl_sinf(h);
    q[0] = axis[0] * s;
    q[1] = axis[1] * s;
    q[2] = axis[2] * s;
    q[3] = msl_cosf(h);
    Quat_ToMatrix(q, r);
    sceVu0MulMatrix(out, r, ra);
    sceVu0InterVector(p, tb, ta, t);
    sceVu0TransMatrix(out, out, p);
}

/* rotation matrix `m` as axis + angle (q: x, y, z the axis, w the angle) */
/* 0x0025C630 */
void Mtx_ToAxisAngle(f32 *unused, f32 *q, f32 (*m)[4]) {
    q[0] = m[2][1] - m[1][2];
    q[1] = m[0][2] - m[2][0];
    q[2] = m[1][0] - m[0][1];
    sceVu0Normalize(q, q);
    q[3] = 2.0f * msl_acosf(0.5f * __builtin_sqrtf(1.0f + (m[2][2] + (m[0][0] + m[1][1]))));
}
