#ifndef EFFECTS_H
#define EFFECTS_H

/* effects.c: what other files call. */
#include "common.h"

/* effects.c */
extern void Drawer_Submit(u8 *d);   /* draw a textured quad (corners +0x14, record +0x10) */
extern void ScreenFade_Step(u8 *f, s32 kind);   /* jump a fade to its end */
extern u32 SceneGame_GetByte19034(u8 *p);   /* the effects paused */
extern void DepthBand_Queue(u8 *d, f32 a, f32 from, f32 to, f32 b);   /* depth of field */

#endif /* EFFECTS_H */
