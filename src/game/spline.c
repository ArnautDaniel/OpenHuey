/* Key splines (the room cameras' paths): +0x0 the current time, +0x4 the keys (32 bytes: time,
 * value, ..., one row of n keys per component), +0x8 n, +0xC / +0x10 the first / last time,
 * +0x14 the current segment, +0x18 the components. Values are cubic Bezier segments with the
 * control points value, value + out (+0x14) and next value + in (+0x2C). */
#include "common.h"
#include "spline.h"

/* move to time u (if within the keys): *t = u, *seg = the segment holding it */
void func_0025F6B0(u8 *s, f32 *t, s32 *seg, f32 u) {
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

/* move the spline's own time to u */
void func_0025F6A0(u8 *s, f32 u) {
    func_0025F6B0(s, (f32 *)s, (s32 *)(s + 0x14), u);
}

/* set up: n keys of `dims` components from `keys` */
void func_0025F7A0(u8 *s, s32 dims, s32 n, f32 *keys) {
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

/* component `comp` at time u (or at the current time if u is outside the keys) */
f32 func_0025F580(u8 *s, s32 comp, f32 u) {
    f32 t = AT(s, 0x0, f32);
    s32 seg = AT(s, 0x14, s32);
    f32 *p;
    f32 x, y, a, a3, b2, b1;

    func_0025F6B0(s, &t, &seg, u);
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
