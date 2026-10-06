#ifndef STALKER_MATH_H
#define STALKER_MATH_H

/* stalker_math.c: what other files call. */
#include "common.h"

/* stalker_math.c */
extern void func_002E2C10(f32 *out, f32 angle);   /* the unit vector of a heading */
extern void func_002E2CA0(f32 *out, f32 *v, f32 angle);   /* v turned about y */
extern f32 func_002E2BC0(const f32 *v);   /* heading of v */
extern void func_002E3130(f32 (*m)[4], f32 *pos, f32 angle);   /* turned by angle about y, at pos */
extern void func_00211A30(f32 *out, f32 *from, f32 angle, f32 dist);   /* from + dir * dist */
extern void func_00211A90(u8 *a, f32 dist, f32 *out);   /* a point ahead */
extern void func_0025C630(f32 *unused, f32 *q, f32 (*m)[4]);   /* rotation matrix to axis + angle */

#endif /* STALKER_MATH_H */
