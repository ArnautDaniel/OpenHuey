#ifndef SHADOW_H
#define SHADOW_H

/* shadow.c: what other files call. */
#include "common.h"

/* shadow.c */
extern void func_001F3530(u8 *s, s32 tri, s32 bone, f32 *light, s32 layer);   /* the shadow drawer */
extern void func_00278D60(u8 *o, u32 tri, f32 *pos, f32 *rot);

#endif /* SHADOW_H */
