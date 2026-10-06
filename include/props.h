#ifndef PROPS_H
#define PROPS_H

/* props.c: what other files call. */
#include "common.h"

/* props.c */
extern void *SynthBase_new(u32 size, void *mem);   /* placement new */
extern void ModelDraw_Fill(u8 *d, const f32 *pos, const f32 *rot, s32 a, s32 b, s32 layer);

#endif /* PROPS_H */
