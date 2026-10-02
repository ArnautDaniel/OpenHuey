#ifndef GS_H
#define GS_H

/* PS2 graphics packets as the game builds them: DMA tags (source chain), GIF tags and GS
 * registers. The PC build reads the same packets (native/platform). */
#include "common.h"

/* DMA source chain tag (low 64 bits of a quadword) */
#define DMA_TAG(id, qwc, addr) (((u64)(addr) << 32) | ((u64)(id) << 28) | (u64)(qwc))
#define DMA_REFE 0
#define DMA_CNT 1
#define DMA_NEXT 2
#define DMA_REF 3
#define DMA_END 7

/* GIF tag: low 64 bits; the high 64 bits are the register list */
#define GIF_TAG(nloop, eop, flg, nreg) \
    ((u64)(nloop) | ((u64)(eop) << 15) | ((u64)(flg) << 58) | ((u64)(nreg) << 60))
#define GIF_PACKED 0
#define GIF_REGLIST 1
#define GIF_IMAGE 2
#define GIF_REG_AD 0xE   /* register list entry: address + data */

/* GS registers (A+D addresses) */
#define GS_BITBLTBUF 0x50
#define GS_TRXPOS 0x51
#define GS_TRXREG 0x52
#define GS_TRXDIR 0x53

/* BITBLTBUF destination half: base (in 64-word blocks), width (in 64 pixels), pixel format */
#define GS_BITBLT_DST(dbp, dbw, dpsm) \
    (((u64)(dbp) << 32) | ((u64)(dbw) << 48) | ((u64)(dpsm) << 56))
#define GS_PSMCT32 0x00
#define GS_PSMT8 0x13

#endif
