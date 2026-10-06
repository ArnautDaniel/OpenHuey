/* Quaternions (x, y, z, w). */
#include "common.h"
#include "quat.h"
#include "msl.h"

/* the rotation by `angle` about the unit `axis` */
/* 0x0025C6F0 */
void Quat_FromAxisAngle(f32 *q, const f32 *axis, f32 angle) {
    f32 h = 0.5f * angle;
    f32 s = func_0031C248(h);

    q[0] = axis[0] * s;
    q[1] = axis[1] * s;
    q[2] = axis[2] * s;
    q[3] = func_0031C058(h);
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

#include "sce/libvu0.h"

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
    axis[3] = 2.0f * func_0031C3C0(0.5f * __builtin_sqrtf(1.0f + (r[2][2] + (r[0][0] + r[1][1]))));
    h = 0.5f * (axis[3] * t);
    s = func_0031C248(h);
    q[0] = axis[0] * s;
    q[1] = axis[1] * s;
    q[2] = axis[2] * s;
    q[3] = func_0031C058(h);
    Quat_ToMatrix(q, r);
    sceVu0MulMatrix(out, r, ra);
    sceVu0InterVector(p, tb, ta, t);
    sceVu0TransMatrix(out, out, p);
}
