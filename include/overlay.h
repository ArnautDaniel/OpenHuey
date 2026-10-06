#ifndef OVERLAY_H
#define OVERLAY_H

/* overlay.c: what other files call. */
#include "common.h"

/* overlay.c */
extern void Overlay_SetColor(void *ov, u32 rgba);   /* an overlay's colour */
extern void Bloom_Start(u8 *o, u32 rgba, s32 layer, s32 sub);

#endif /* OVERLAY_H */
