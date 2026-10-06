#ifndef SPLINE_H
#define SPLINE_H

/* spline.c: what other files call. */
#include "common.h"

/* spline.c */
extern void func_0025F6A0(u8 *s, f32 u);
extern void func_0025F7A0(u8 *s, s32 dims, s32 n, f32 *keys);
extern f32 func_0025F580(u8 *s, s32 comp, f32 u);   /* evaluate component comp at u */

#endif /* SPLINE_H */
