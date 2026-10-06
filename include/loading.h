#ifndef LOADING_H
#define LOADING_H

/* loading.c: what other files call. */
#include "common.h"

/* loading.c */
extern void Loading_DrawFrame(u8 *o, s32 frame);
extern u8 *gl_gfm_part(u8 *part, f32 (*mvp)[4], const u8 *tex, s32 csa);   /* loading.c */

#endif /* LOADING_H */
