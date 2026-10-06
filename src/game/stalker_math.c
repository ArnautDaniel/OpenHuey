/* Heading helpers the pursuers reach (0x211A30.., 0x2E2BC0..): the game's "heading" is a turn
 * about Y, 0 facing +Z. Generic (other characters use them too); kept here on the stalkers
 * branch. */

#include "common.h"
#include "sce/libvu0.h"
#include "stalker_math.h"
#include "msl.h"

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

/* the heading of `v` (10, out of range, when it has no X/Z) */
/* 0x002E2BC0 */
f32 Vec_Heading(const f32 *v) {
    if (v[0] == 0.0f && v[2] == 0.0f) {
        return 10.0f;
    }
    return func_0031C5C0(v[0], v[2]);
}

/* the matrix turned to heading `angle`, at `pos` */
/* 0x002E3130 */
void Mtx_AtHeading(f32 (*m)[4], f32 *pos, f32 angle) {
    sceVu0UnitMatrix(m);
    sceVu0RotMatrixY(m, m, angle);
    sceVu0TransMatrix(m, m, pos);
}

/* `from` + `dist` along heading `angle` */
/* 0x00211A30 */
void Vec_Along(f32 *out, f32 *from, f32 angle, f32 dist) {
    f32 d[4] __attribute__((aligned(16)));

    Heading_Vector(d, angle);
    sceVu0ScaleVector(d, d, dist);
    sceVu0AddVector(out, from, d);
}

/* `dist` ahead of the actor (its position +0x10, heading +0x54) */
/* 0x00211A90 */
void Actor_PointAhead(u8 *a, f32 dist, f32 *out) {
    f32 d[4] __attribute__((aligned(16)));

    Heading_Vector(d, AT(a, 0x54, f32));
    sceVu0ScaleVector(d, d, dist);
    sceVu0AddVector(out, (f32 *)(a + 0x10), d);
}

/* rotation matrix `m` as axis + angle (q: x, y, z the axis, w the angle) */
/* 0x0025C630 */
void Mtx_ToAxisAngle(f32 *unused, f32 *q, f32 (*m)[4]) {
    q[0] = m[2][1] - m[1][2];
    q[1] = m[0][2] - m[2][0];
    q[2] = m[1][0] - m[0][1];
    sceVu0Normalize(q, q);
    q[3] = 2.0f * func_0031C3C0(0.5f * __builtin_sqrtf(1.0f + (m[2][2] + (m[0][0] + m[1][1]))));
}
