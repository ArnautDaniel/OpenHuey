/* Sony libvu0 vector/matrix routines (VU0 macro mode). Platform code: on PC these become
 * plain C. Vectors are 16-byte aligned f32[4]. */
#ifndef SCE_LIBVU0_H
#define SCE_LIBVU0_H

#include "common.h"

typedef f32 sceVu0FVECTOR[4] __attribute__((aligned(16)));
typedef f32 sceVu0FMATRIX[4][4] __attribute__((aligned(16)));

void sceVu0ApplyMatrix(f32 *out, sceVu0FMATRIX m, const f32 *v);
void sceVu0MulMatrix(sceVu0FMATRIX out, sceVu0FMATRIX a, sceVu0FMATRIX b);
void sceVu0InversMatrix(sceVu0FMATRIX out, sceVu0FMATRIX m);
void sceVu0TransposeMatrix(sceVu0FMATRIX m0, sceVu0FMATRIX m1);
void sceVu0ViewScreenMatrix(sceVu0FMATRIX m, f32 scrz, f32 ax, f32 ay, f32 cx, f32 cy, f32 zmin, f32 zmax, f32 nearz, f32 farz);
void sceVu0OuterProduct(f32 *out, const f32 *a, const f32 *b);
f32 sceVu0InnerProduct(const f32 *a, const f32 *b);
void sceVu0Normalize(f32 *out, const f32 *v);
void sceVu0CameraMatrix(sceVu0FMATRIX m, const f32 *p, const f32 *zd, const f32 *yd);
void sceVu0AddVector(f32 *out, const f32 *a, const f32 *b);
void sceVu0SubVector(f32 *out, const f32 *a, const f32 *b);
void sceVu0MulVector(f32 *out, const f32 *a, const f32 *b);
void sceVu0ScaleVector(f32 *out, const f32 *v, f32 s);
void sceVu0InterVector(f32 *out, const f32 *a, const f32 *b, f32 t);   /* a * t + b * (1 - t) */
void sceVu0CopyVector(f32 *out, const f32 *v);
void sceVu0CopyMatrix(sceVu0FMATRIX out, sceVu0FMATRIX m);
void sceVu0UnitMatrix(sceVu0FMATRIX m);
void sceVu0RotMatrixX(sceVu0FMATRIX out, sceVu0FMATRIX m, f32 angle);
void sceVu0RotMatrixY(sceVu0FMATRIX out, sceVu0FMATRIX m, f32 angle);
void sceVu0RotMatrixZ(sceVu0FMATRIX out, sceVu0FMATRIX m, f32 angle);
void sceVu0RotMatrix(sceVu0FMATRIX out, sceVu0FMATRIX m, const f32 *rot);   /* x, then y, then z */
void sceVu0TransMatrix(sceVu0FMATRIX out, sceVu0FMATRIX m, const f32 *t);

extern void vu0_CopyXYZ(f32 *out, const f32 *v);   /* libvu0: copy x, y, z */
extern void vu0_LerpXYZ(f32 *out, const f32 *a, const f32 *b, f32 t);   /* xyz lerp, w of a */
extern void vu0_ScaleXYZ(f32 *out, const f32 *v, f32 s);   /* libvu0: scale x, y, z */
extern void sceVu0FTOI4Vector(s32 *out, const f32 *in);

#endif
