#ifndef SND_DRIVER_H
#define SND_DRIVER_H

/* snd_driver.c: what other files call. */
#include "common.h"

/* snd_driver.c */
extern void SndDriver_FreeIop(u8 *d);
extern void SndDriver_Frame(u8 *d);   /* sound driver tick */
extern void SndDriver_Start(u8 *d);
extern u8 *SndDriver_dtor(u8 *d, s32 flags);

#endif /* SND_DRIVER_H */
