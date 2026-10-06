#ifndef VECMATH_H
#define VECMATH_H

/* quat.c: what other files call. */
#include "common.h"

/* quat.c */
extern void Quat_FromAxisAngle(f32 *q, const f32 *axis, f32 angle);   /* rotation about an axis */
extern void Quat_ToMatrix(const f32 *q, f32 (*m)[4]);   /* its matrix */
extern void Bone_BlendMatrix(f32 *q, f32 (*out)[4], f32 (*a)[4], f32 (*b)[4], f32 t);   /* blend two bones */

/* ---- (was spline.h) ---- */

/* spline.c: what other files call. */

/* spline.c */
extern void Spline_Seek(u8 *s, f32 u);
extern void Spline_Init(u8 *s, s32 dims, s32 n, f32 *keys);
extern f32 Spline_Eval(u8 *s, s32 comp, f32 u);   /* evaluate component comp at u */

/* ---- (was stalker_math.h) ---- */

/* stalker_math.c: what other files call. */

/* stalker_math.c */
extern void Heading_Vector(f32 *out, f32 angle);   /* the unit vector of a heading */
extern void Vec_TurnY(f32 *out, f32 *v, f32 angle);   /* v turned about y */
extern f32 Vec_Heading(const f32 *v);   /* heading of v */
extern void Mtx_AtHeading(f32 (*m)[4], f32 *pos, f32 angle);   /* turned by angle about y, at pos */
extern void Vec_Along(f32 *out, f32 *from, f32 angle, f32 dist);   /* from + dir * dist */
extern void Mtx_ToAxisAngle(f32 *unused, f32 *q, f32 (*m)[4]);   /* rotation matrix to axis + angle */

/* ---- (was random.h) ---- */

/* random.c: what other files call. */

typedef struct VObject VObject;

/* random.c */
extern VObject *Random_ctor(VObject *rng, s32 seed);   /* random number generator (Game +0x400000) */
extern VObject *Random_dtor(VObject *r, s32 flags);

/* vecmath.c */
extern void Mtx_ApplyVector(f32 *out, f32 (*m)[4], const f32 *v);   /* m * v */
extern f32 Angle_Wrap(f32 a);   /* angle wrapped to -pi..pi */
extern void Mtx_ApplyPoint(f32 *out, f32 (*m)[4], const f32 *v);
extern void Mtx_TurnY(f32 (*m)[4], f32 a);   /* turn about y */
extern f32 Vec_LineDistance(const f32 *a, const f32 *b, const f32 *c);   /* pt's distance from the line */

extern void Mtx_FrameKeepZ(f32 (*out)[4], f32 (*b)[4], const f32 *up);
extern void Mtx_TurnTwo(f32 (*out)[4], f32 (*axes)[4], f32 a, f32 b);

#endif /* VECMATH_H */
