#ifndef QUAT_H
#define QUAT_H

/* quat.c: what other files call. */
#include "common.h"

/* quat.c */
extern void func_0025C6F0(f32 *q, const f32 *axis, f32 angle);   /* rotation about an axis */
extern void func_0025C770(const f32 *q, f32 (*m)[4]);   /* its matrix */
extern void func_0025C440(f32 *q, f32 (*out)[4], f32 (*a)[4], f32 (*b)[4], f32 t);   /* blend two bones */

#endif /* QUAT_H */
