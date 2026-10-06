#ifndef SCE_SIF_H
#define SCE_SIF_H

/* The SIF (EE <-> IOP link) and its RPC: native/platform/sif.c on PC. */
#include "common.h"
#include "sndlib.h"

extern s32 CreateSema(void *s);
extern s32 CreateThread(void *t);
extern void func_0026CA98(u32 start, u32 end);   /* write back the data cache */
extern void func_0026CB18(u32 start, u32 end);   /* (in an interrupt) */
extern s32 func_0026D2E0(s32 thread, void *arg);   /* StartThread */
extern s32 func_0026FF08(void *cd, u32 id, s32 mode);   /* sceSifBindRpc */
extern s32 func_002700E8(void *cd, u32 fno, u32 mode, void *send, s32 ssize, void *recv, s32 rsize, void (*end)(void *), void *endParam);
extern s32 func_002702E8(void *cd);   /* sceSifCheckStatRpc */
extern void func_00270328(void *queue, s32 thread);   /* sceSifSetRpcQueue */
extern void func_002703C0(void *sd, u32 id, void *(*func)(u32, void *), void *buf, void *cfunc, void *cbuf, void *queue);
extern void func_002707D8(void *queue);   /* sceSifRpcLoop */
extern u32 func_002744D8(u32 size);   /* sceSifAllocIopHeap */
extern void func_00274640(u32 addr);   /* free IOP memory */
extern s32 isceSifDmaStat(u32 id);
extern u32 isceSifSetDma(SifDma *d, s32 n);
extern s32 sceSifDmaStat(u32 id);
extern u32 sceSifSetDma(SifDma *d, s32 n);

#endif /* SCE_SIF_H */
