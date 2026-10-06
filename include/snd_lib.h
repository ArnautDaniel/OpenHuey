#ifndef SND_LIB_H
#define SND_LIB_H

/* snd_lib.c: what other files call. */
#include "common.h"

/* snd_lib.c */
extern s32 func_0021F3D0(u8 *t, s32 size);
extern void func_0021F4A0(s32 bank, u8 *t, s32 size, u16 *out);
extern s32 func_0021F840(u32 src, u32 dest, u32 size, s32 attr, s32 inIrq);   /* EE -> IOP DMA */
extern s32 func_0021F2D0(u32 cmd, s32 poll);   /* a transfer still running */
extern void *func_0021F9F0(u32 cmd, void *args);   /* ... without waiting */
extern u32 *func_0021FB70(u32 cmd, void *args);   /* call the driver */
extern void func_00220150(s32 prio, s32 stack);
extern void func_00220210(void);
extern void func_00220270(void);
extern void func_00220340(void);
extern s8 func_00220440(char *out, u8 *p);

#endif /* SND_LIB_H */
