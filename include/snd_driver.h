#ifndef SND_DRIVER_H
#define SND_DRIVER_H

/* snd_driver.c: what other files call. */
#include "common.h"

/* snd_driver.c */
extern void func_00210180(u8 *d);
extern void func_00210230(u8 *d);   /* sound driver tick */
extern void func_002102E0(u8 *d);
extern u8 *func_0020E000(u8 *d, s32 flags);

#endif /* SND_DRIVER_H */
