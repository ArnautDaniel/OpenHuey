#ifndef SYNTH_H
#define SYNTH_H

/* synth.c: what other files call. */
#include "common.h"

/* synth.c */
extern s32 func_00322970(u8 *o);
extern u8 func_00322CD0(u8 *o, s32 shown);   /* the screen's step: bit 0 up, bit 7 done */
extern void func_00323510(u8 *o, u8 row, u8 v, s32 a);
extern void func_003230B0(u8 *o, u8 k, u8 v);

#endif /* SYNTH_H */
