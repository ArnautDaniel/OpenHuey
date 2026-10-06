#ifndef RENDERER_H
#define RENDERER_H

/* renderer.c: what other files call. */
#include "common.h"

typedef struct VObject VObject;

/* renderer.c */
extern void func_001B83D0(u8 *r, s32 mode);
extern void func_001B8250(u8 *r);
extern const u8 *gl2d_image(u32 block);
extern void func_001B87D0(u8 *r);
extern void func_001B8750(u8 *r);
extern void func_001B86A0(u8 *r);
extern void func_001B85B0(u8 *r);
extern s32 func_001B9000(VObject *r, u32 rgba);

#endif /* RENDERER_H */
