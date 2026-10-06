#ifndef SYNTH_H
#define SYNTH_H

/* synth.c: what other files call. */
#include "common.h"

/* synth.c */
extern s32 Synth_CountMaterials(u8 *o);
extern u8 ItemFound_Step(u8 *o, s32 shown);   /* the screen's step: bit 0 up, bit 7 done */
extern void SlotMachine_DrawRow(u8 *o, u8 row, u8 v, s32 a);
extern void SlotMachine_DrawCell(u8 *o, u8 k, u8 v);

#endif /* SYNTH_H */
