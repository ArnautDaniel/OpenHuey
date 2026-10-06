#ifndef RENDERER_H
#define RENDERER_H

/* renderer.c: what other files call. */
#include "common.h"

typedef struct VObject VObject;

/* renderer.c */
extern void Renderer_SetupVideo(u8 *r, s32 mode);
extern void Renderer_AllocVram(u8 *r);
extern const u8 *gl2d_image(u32 block);
extern void Renderer_WaitChain(u8 *r);
extern void Renderer_SendFinal(u8 *r);
extern void Renderer_NextClear(u8 *r);
extern void Renderer_EndFrame(u8 *r);
extern s32 Renderer_FillScreen(VObject *r, u32 rgba);

#endif /* RENDERER_H */
