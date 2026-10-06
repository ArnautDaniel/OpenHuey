#ifndef SPLINE_H
#define SPLINE_H

/* spline.c: what other files call. */
#include "common.h"

/* spline.c */
extern void Spline_Seek(u8 *s, f32 u);
extern void Spline_Init(u8 *s, s32 dims, s32 n, f32 *keys);
extern f32 Spline_Eval(u8 *s, s32 comp, f32 u);   /* evaluate component comp at u */

#endif /* SPLINE_H */
