#ifndef SYSTEM_H
#define SYSTEM_H

/* system.c: what other files call. */
#include "common.h"

typedef struct Game Game;
typedef struct VObject VObject;

/* system.c */
extern void *func_0020E7F0(Game *game);   /* base class constructor */
extern void *func_0020E340(u8 *s);   /* Game.unk69AC0 */
extern void *func_001BC320(u8 *o, s32 flags);
extern void func_00169260(VObject *h, void *base, u32 size, void *blocks, s32 count);   /* heap init */
extern void *func_001AAE10(u8 *o, s32 flags);
extern void *func_0020E280(u8 *t);   /* scene table (+0x400A00) */
extern void *func_0020E260(VObject *o);   /* +0x14D9B00 */
extern void *func_0020E1A0(u8 *p);   /* object pool (+0x14D9DD0) */
extern void *func_0020E110(u8 *p);   /* object pool (+0x14DC530) */
extern void *func_001F4600(u8 *o);   /* Game.unk14E8C90 */
extern void func_001F44D0(u8 *o);   /* init Game.unk14E8C90 */
extern void *func_001BF220(u8 *o, s32 flags);
extern void *func_001BF880(u8 *o, s32 flags);
extern void *func_001BEC10(u8 *e, s32 flags);
extern void *func_001BECA0(u8 *o, s32 flags);

#endif /* SYSTEM_H */
