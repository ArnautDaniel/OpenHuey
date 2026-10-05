/* Capcom's EE sound library: the link to the sound driver on the IOP (SNDDRV.IRX, RPC server
 * 0x77777777 / 0x77777778; the EE serves 0x77777779 back). src/game/snd_lib.c, used by the
 * sound driver object (src/game/snd_driver.c). Natively the SIF calls below are where the IOP
 * is emulated (native/platform/sif.c). */
#ifndef SNDLIB_H
#define SNDLIB_H

#include "common.h"

/* EE-only bits: interrupts back on after a SIF callback, and the uncached view of EE memory
   that the IOP's DMA writes go through */
#ifdef HG_NATIVE
#define EE_SYNC_EI()
#define UNCACHED(p) ((void *)(p))
#else
#define EE_SYNC_EI() __asm__ volatile("sync\n\tei")
#define UNCACHED(p) ((void *)((u32)(p) | 0x20000000))
#endif

/* sceSifClientData / sceSifServeData / sceSifQueueData: only passed around */
typedef struct SifClient { u8 data[0x28]; } SifClient;

typedef struct SifDma {
    /* 0x0 */ u32 src;
    /* 0x4 */ u32 dest;
    /* 0x8 */ s32 size;
    /* 0xC */ s32 attr;
} SifDma;

/* the PS2 SDK (libkernel / sifrpc / sifdev) */
extern s32 func_002700E8(void *cd, u32 fno, u32 mode, void *send, s32 ssize, void *recv, s32 rsize,
                         void (*end)(void *), void *endParam);   /* sceSifCallRpc */
extern s32 func_002702E8(void *cd);                            /* sceSifCheckStatRpc */
extern s32 func_0026FF08(void *cd, u32 id, s32 mode);          /* sceSifBindRpc */
extern void func_00270328(void *queue, s32 thread);            /* sceSifSetRpcQueue */
extern void func_002703C0(void *sd, u32 id, void *(*func)(u32, void *), void *buf,
                          void *cfunc, void *cbuf, void *queue);   /* sceSifRegisterRpc */
extern void func_002707D8(void *queue);                        /* sceSifRpcLoop */
extern u32 func_002744D8(u32 size);                            /* sceSifAllocIopHeap */
extern s32 func_00274640(u32 addr);                            /* sceSifFreeIopHeap */
extern void func_0026CA98(u32 start, u32 end);                 /* write back the data cache */
extern void func_0026CB18(u32 start, u32 end);                 /* (in an interrupt) */
extern u32 sceSifSetDma(SifDma *d, s32 n);
extern u32 isceSifSetDma(SifDma *d, s32 n);
extern s32 sceSifDmaStat(u32 id);
extern s32 isceSifDmaStat(u32 id);

/* the library */
u32 *func_0021FB70(u32 cmd, void *args);                      /* call the driver */
void *func_0021F9F0(u32 cmd, void *args);                     /* ... without waiting */
s32 func_0021F2D0(u32 cmd, s32 poll);                         /* a transfer still running */
s32 func_0021F840(u32 src, u32 dest, u32 size, s32 attr, s32 inIrq);   /* EE -> IOP DMA */

#endif
