#ifndef SCE_EEKERNEL_H
#define SCE_EEKERNEL_H

/* The EE kernel calls the game makes (caches, threads, semaphores): native/platform/sce.c
 * on PC. */
#include "common.h"

extern void FlushCache(s32 mode);
extern void func_0010BE10(s32 inter, s32 mode, s32 ntsc, s32 ffmd);   /* libgraph: sceGsResetGraph */
extern void func_0010BFB0(void);   /* libgraph: sceGsResetPath */
extern void func_0010C440(void *disp);   /* sceGsPutDispEnv */
extern u32 func_0010D3B8(u32 i);   /* libgraph: parameter / address table entry i (0..9) */
extern void func_0010D3E0(s32 mode);   /* libgraph: reset */
extern void func_0010D6E8(u32 *chan, void *tag);   /* libdma: sceDmaSend */
extern s32 func_0010D988(u32 *chan, s32 mode, s32 timeout);   /* libdma: sceDmaSync */
extern void func_00110878(u8 *out);   /* the date and time (Sony libcdvd clock) */
extern void func_001C8478(void);
extern void func_001C8648(void *p);
extern void func_001CA850(void);
extern void func_001CC5B0(s32 a);
extern void func_001EE798(void);   /* libdbc: init */
extern void sceGsDefDispEnv(void *disp, s32 psm, s32 w, s32 h, s32 dx, s32 dy);
extern void sceGsExecLoadImage(void *lp, const void *src);
extern void sceGsPutDrawEnv(void *giftag);
extern void sceGsSetDefLoadImage(void *lp, s16 dbp, s16 dbw, s16 dpsm, s16 x, s16 y, s16 w, s16 h);
extern void sceGsSyncPath(s32 mode, s32 timeout);

#endif /* SCE_EEKERNEL_H */
