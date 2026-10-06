#ifndef SCE_SIF_H
#define SCE_SIF_H

/* The SIF (EE <-> IOP link) and its RPC: native/platform/sif.c on PC. */
#include "common.h"
#include "sndlib.h"

extern s32 CreateSema(void *s);
extern s32 CreateThread(void *t);
extern void FlushDCacheRange(u32 start, u32 end);   /* write back the data cache */
extern void func_0026CB18(u32 start, u32 end);   /* (in an interrupt) */
extern s32 Kernel_StartThread(s32 thread, void *arg);   /* StartThread */
extern s32 sceSifBindRpc(void *cd, u32 id, s32 mode);   /* sceSifBindRpc */
extern s32 sceSifCallRpc(void *cd, u32 fno, u32 mode, void *send, s32 ssize, void *recv, s32 rsize, void (*end)(void *), void *endParam);
extern s32 sceSifCheckStatRpc(void *cd);   /* sceSifCheckStatRpc */
extern void sceSifSetRpcQueue(void *queue, s32 thread);   /* sceSifSetRpcQueue */
extern void func_002703C0(void *sd, u32 id, void *(*func)(u32, void *), void *buf, void *cfunc, void *cbuf, void *queue);
extern void sceSifRpcLoop(void *queue);   /* sceSifRpcLoop */
extern u32 sceSifAllocIopHeap(u32 size);   /* sceSifAllocIopHeap */
extern void sceSifFreeIopHeap(u32 addr);   /* free IOP memory */
extern s32 isceSifDmaStat(u32 id);
extern u32 isceSifSetDma(SifDma *d, s32 n);
extern s32 sceSifDmaStat(u32 id);
extern u32 sceSifSetDma(SifDma *d, s32 n);

#endif /* SCE_SIF_H */
