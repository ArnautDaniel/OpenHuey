#ifndef SCE_INTC_H
#define SCE_INTC_H

/* Interrupt handlers (vblank): native/platform/intc.c on PC. */
#include "common.h"

extern s32 RemoveIntcHandler(s32 cause, s32 id);
extern s32 AddIntcHandler(s32 cause, s32 (*handler)(s32), s32 next);   /* AddIntcHandler */
extern void DisableIntc(s32 cause);   /* DisableIntc */
extern s32 EnableIntc(s32 cause);   /* EnableIntc */
extern void hg_hw_write32(u32 addr, u32 value);
extern void hg_wait_flag(volatile u8 *flag);

#endif /* SCE_INTC_H */
