#ifndef PS2HW_H
#define PS2HW_H

#include "common.h"

/* Direct writes to PS2 hardware registers (timers, DMA, GS...). On the PS2 they are the plain
 * stores the game did; the PC build routes them to the platform layer (native/platform). */
#ifdef HG_NATIVE
void hg_hw_write32(u32 addr, u32 value);
#define HW_WRITE32(addr, value) hg_hw_write32((addr), (value))
#else
#define HW_WRITE32(addr, value) (*(volatile u32 *)(addr) = (value))
#endif

#endif
