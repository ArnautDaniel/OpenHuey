/* Quaternions (x, y, z, w). */
#include "common.h"

extern f32 func_0031C248(f32 x);   /* sinf */
extern f32 func_0031C058(f32 x);   /* cosf */

/* the rotation by `angle` about the unit `axis` */
void func_0025C6F0(f32 *q, const f32 *axis, f32 angle) {
    f32 h = 0.5f * angle;
    f32 s = func_0031C248(h);

    q[0] = axis[0] * s;
    q[1] = axis[1] * s;
    q[2] = axis[2] * s;
    q[3] = func_0031C058(h);
}

/* its 3 x 3 rotation matrix (the translation row is left as it is) */
void func_0025C770(const f32 *q, f32 (*m)[4]) {
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
