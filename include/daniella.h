#ifndef DANIELLA_H
#define DANIELLA_H

/* daniella.c: what other files call. */
#include "common.h"

typedef struct Pursuer Pursuer;

/* daniella.c */
extern Pursuer *func_0020C3A0(Pursuer *p, s32 flags);
extern s32 func_0020C4B0(Pursuer *p);
extern void func_0020C4D0(Pursuer *p);
extern void func_0020C510(Pursuer *p, s32 side, f32 *out);
extern void func_0020C5B0(Pursuer *p, s32 kind, f32 *out);
extern void func_0020C660(Pursuer *p, s32 exit);
extern void func_0020C730(Pursuer *p, s32 exit);
extern void func_0020C7C0(Pursuer *p);
extern void func_0020CAE0(Pursuer *p, s32 *e, f32 *a, f32 *b);
extern void func_0020CC70(Pursuer *p, s8 situation);
extern void func_0020D1F0(Pursuer *p, s32 on);
extern s32 func_0020D2E0(Pursuer *p);
extern s32 func_0020D2F0(Pursuer *p);
extern f32 func_0020D300(Pursuer *p);
extern f32 func_0020D310(Pursuer *p);
extern f32 func_0020D320(Pursuer *p);
extern void func_0020D330(Pursuer *p);
extern u8 *func_0020D620(Pursuer *p);
extern u8 *func_0020D660(Pursuer *p);
extern void func_0020D6A0(Pursuer *p);
extern void *func_0020D8D0(u8 *o, s32 flags);
extern void *func_0020D920(u8 *o, s32 flags);
extern void *func_0020D970(u8 *o, s32 flags);
extern void *func_0020D9C0(u8 *o, s32 flags);

#endif /* DANIELLA_H */
