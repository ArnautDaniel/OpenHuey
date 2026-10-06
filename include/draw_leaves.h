#ifndef DRAW_LEAVES_H
#define DRAW_LEAVES_H

/* Game draw leaves that only built PS2 VU1 / GS packets: on PC their OpenGL
 * versions are in native/platform/glr_hooks.c. */
#include "common.h"

extern s32 func_001AAE80(u8 *r);
extern s32 func_001AB3F0(u8 *r);
extern s32 func_001AB960(u8 *r);
extern s32 func_001AC0D0(u8 *r);
extern s32 func_001AF3B0(u8 *r);
extern s32 func_001B0D40(u8 *r);
extern s32 func_001B1370(u8 *r);
extern s32 func_001B18E0(u8 *r);
extern s32 func_001B2160(u8 *r);
extern s32 func_001B4330(u8 *r);
extern s32 func_001B4F30(u8 *r);   /* layer 6 setup (u8: 0 = skip the packet) */
extern void func_0021C840(void *o, s32, s32);
extern void func_0021D290(void *ov);
extern void func_0021D8F0(void *ov, s32 limit, s32 amount);   /* the panic tint (palette 5 below `limit`) */
extern void func_0021E1B0(void *ov);

#endif /* DRAW_LEAVES_H */
