#ifndef EFFECTS_H
#define EFFECTS_H

/* effects.c: what other files call. */
#include "common.h"

/* effects.c */
extern void func_002E56C0(u8 *d);   /* draw a textured quad (corners +0x14, record +0x10) */
extern void func_002CF3A0(u8 *f, s32 kind);   /* jump a fade to its end */
extern u32 func_002D6010(u8 *p);   /* the effects paused */
extern void func_002C86F0(u8 *d, f32 a, f32 from, f32 to, f32 b);   /* depth of field */

#endif /* EFFECTS_H */
