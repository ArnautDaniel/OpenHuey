#ifndef PS2HW_H
#define PS2HW_H

#include "common.h"
#include "sce/intc.h"

/* Direct writes to PS2 hardware registers (timers, DMA, GS...). On the PS2 they are the plain
 * stores the game did; the PC build routes them to the platform layer (native/platform). */
#ifdef HG_NATIVE
#define HW_WRITE32(addr, value) hg_hw_write32((addr), (value))
#else
#define HW_WRITE32(addr, value) (*(volatile u32 *)(addr) = (value))
#endif

/* Clear an interrupt-set flag and wait until the interrupt sets it again (the vblank handlers
 * set gVblankStartSeen / gVblankEndSeen). On PC the wait advances the platform's vblank clock. */
#ifdef HG_NATIVE
#define VSYNC_WAIT(flag) hg_wait_flag(&(flag))
#else
#define VSYNC_WAIT(flag)                  \
    do {                                  \
        (flag) = 0;                       \
        while (!*(volatile u8 *)&(flag)) { \
        }                                 \
    } while (0)
#endif

#endif
