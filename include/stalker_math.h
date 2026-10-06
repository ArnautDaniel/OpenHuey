#ifndef STALKER_MATH_H
#define STALKER_MATH_H

/* stalker_math.c: what other files call. */
#include "common.h"

/* stalker_math.c */
extern void Heading_Vector(f32 *out, f32 angle);   /* the unit vector of a heading */
extern void Vec_TurnY(f32 *out, f32 *v, f32 angle);   /* v turned about y */
extern f32 Vec_Heading(const f32 *v);   /* heading of v */
extern void Mtx_AtHeading(f32 (*m)[4], f32 *pos, f32 angle);   /* turned by angle about y, at pos */
extern void Vec_Along(f32 *out, f32 *from, f32 angle, f32 dist);   /* from + dir * dist */
extern void Actor_PointAhead(u8 *a, f32 dist, f32 *out);   /* a point ahead */
extern void Mtx_ToAxisAngle(f32 *unused, f32 *q, f32 (*m)[4]);   /* rotation matrix to axis + angle */

#endif /* STALKER_MATH_H */
