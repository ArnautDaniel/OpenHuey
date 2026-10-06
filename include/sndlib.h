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
#define UNCACHED(p) ((void *)((u32)(p) | UNCACHED_BIT))
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

/* the library */

#endif
