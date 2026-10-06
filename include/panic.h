#ifndef PANIC_H
#define PANIC_H

/* panic.c: what other files call. */
#include "common.h"

/* panic.c */
extern void Panic_Update(u8 *o);
extern void Panic_SetLevel(u8 *o, s16 n);
extern void Panic_SetStage(u8 *o, u32 stage);
extern void Panic_Pause(u8 *o);
extern void Panic_Fright(u8 *o, f32 amount);   /* a fright (less with a charm on) */
extern void Panic_FrightRaw(u8 *o, f32 amount);

#endif /* PANIC_H */
