/* Heading helpers the pursuers reach (0x211A30.., 0x2E2BC0..): the game's "heading" is a turn
 * about Y, 0 facing +Z. Generic (other characters use them too); kept here on the stalkers
 * branch. */

#include "common.h"
#include "sce/libvu0.h"

extern f32 func_0031C5C0(f32 x, f32 z);   /* atan2f */

/* the unit vector of heading `angle` */
void func_002E2C10(f32 *out, f32 angle) {
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
void func_002E2CA0(f32 *out, f32 *v, f32 angle) {
    f32 m[4][4] __attribute__((aligned(16)));

    sceVu0UnitMatrix(m);
    sceVu0RotMatrixY(m, m, angle);
    sceVu0ApplyMatrix(out, m, v);
}

/* the heading of `v` (10, out of range, when it has no X/Z) */
f32 func_002E2BC0(const f32 *v) {
    if (v[0] == 0.0f && v[2] == 0.0f) {
        return 10.0f;
    }
    return func_0031C5C0(v[0], v[2]);
}

/* the matrix turned to heading `angle`, at `pos` */
void func_002E3130(f32 (*m)[4], f32 *pos, f32 angle) {
    sceVu0UnitMatrix(m);
    sceVu0RotMatrixY(m, m, angle);
    sceVu0TransMatrix(m, m, pos);
}

/* `from` + `dist` along heading `angle` */
void func_00211A30(f32 *out, f32 *from, f32 angle, f32 dist) {
    f32 d[4] __attribute__((aligned(16)));

    func_002E2C10(d, angle);
    sceVu0ScaleVector(d, d, dist);
    sceVu0AddVector(out, from, d);
}

/* `dist` ahead of the actor (its position +0x10, heading +0x54) */
void func_00211A90(u8 *a, f32 dist, f32 *out) {
    f32 d[4] __attribute__((aligned(16)));

    func_002E2C10(d, AT(a, 0x54, f32));
    sceVu0ScaleVector(d, d, dist);
    sceVu0AddVector(out, (f32 *)(a + 0x10), d);
}
