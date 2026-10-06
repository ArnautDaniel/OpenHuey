#ifndef LOADER_H
#define LOADER_H

/* loader.c: what other files call. */
#include "common.h"

typedef struct VObject VObject;

/* loader.c */
extern void Loader_Init(u8 *l);
extern void Loader_RegisterAll(u8 *l);
extern void Loader_Tick(u8 *l);   /* file loader tick */
extern void *Loader_dtor(VObject *l, s32 flags);
extern void Loader_CloseAll(u8 *l);

/* ---- (was loading.h) ---- */

/* loading.c: what other files call. */

/* loading.c */
extern void Loading_DrawFrame(u8 *o, s32 frame);
extern u8 *gl_gfm_part(u8 *part, f32 (*mvp)[4], const u8 *tex, s32 csa);   /* loading.c */

#endif /* LOADER_H */
