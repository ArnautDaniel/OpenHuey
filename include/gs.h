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

/* VIF codes (the upper 64 bits of a DMA tag quadword carry two of them) */
#define VIF_NOP 0x00000000
#define VIF_DIRECT(qwc) (0x50000000 | (qwc))   /* pass qwc quadwords to the GIF (PATH2) */

/* GIF tag: low 64 bits; the high 64 bits are the register list */
#define GIF_TAG(nloop, eop, flg, nreg) \
    ((u64)(nloop) | ((u64)(eop) << 15) | ((u64)(flg) << 58) | ((u64)(nreg) << 60))
#define GIF_PACKED 0
#define GIF_REGLIST 1
#define GIF_IMAGE 2
#define GIF_REG_AD 0xE   /* register list entry: address + data */
/* register list entries (REGLIST / PACKED descriptors) */
#define GIF_PRIM 0x0
#define GIF_RGBAQ 0x1
#define GIF_UV 0x3
#define GIF_XYZ2 0x5
#define GIF_TEX0_1 0x6
#define GIF_CLAMP_1 0x8
#define GIF_REGS(...) GIF_REGS_(__VA_ARGS__)
#define GIF_REGS_(a, b, c, d, e, f, g, h) \
    ((u64)(a) | (u64)(b) << 4 | (u64)(c) << 8 | (u64)(d) << 12 | (u64)(e) << 16 | (u64)(f) << 20 | \
     (u64)(g) << 24 | (u64)(h) << 28)

/* GS registers (A+D addresses) */
#define GS_PRIM 0x00
#define GS_RGBAQ 0x01
#define GS_XYZ2 0x05
#define GS_CLAMP_1 0x08
#define GS_TEX1_1 0x14
#define GS_XYOFFSET_1 0x18
#define GS_TEXA 0x3B
#define GS_SCISSOR_1 0x40
#define GS_ALPHA_1 0x42
#define GS_PABE 0x49
#define GS_TEXFLUSH 0x3F
#define GS_FBA_1 0x4A
#define GS_FRAME_1 0x4C
#define GS_ZBUF_1 0x4E
#define GS_TEST_1 0x47
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
