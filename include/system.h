#ifndef SYSTEM_H
#define SYSTEM_H

/* system.c: what other files call. */
#include "common.h"

typedef struct Game Game;
typedef struct VObject VObject;

/* system.c */
extern void *GameBase_ctor(Game *game);   /* base class constructor */
extern void *System_ctor(u8 *s);   /* Game.unk69AC0 */
extern void *Obj46AD88_dtor(u8 *o, s32 flags);
extern void Heap_Setup(VObject *h, void *base, u32 size, void *blocks, s32 count);   /* heap init */
extern void *Renderer_dtor(u8 *o, s32 flags);
extern void *SceneTable_ctor(u8 *t);   /* scene table (+0x400A00) */
extern void *Camera_ctor(VObject *o);   /* +0x14D9B00 */
extern void *SmallPool_ctor(u8 *p);   /* object pool (+0x14D9DD0) */
extern void *BigPool_ctor(u8 *p);   /* object pool (+0x14DC530) */
extern void *Slots_ctor(u8 *o);   /* Game.unk14E8C90 */
extern void Slots_Init(u8 *o);   /* init Game.unk14E8C90 */
extern void *Obj46AE10_dtor(u8 *o, s32 flags);
extern void *Vram_dtor(u8 *o, s32 flags);
extern void *IopBuffers_dtor(u8 *e, s32 flags);
extern void *IopArray_dtor(u8 *o, s32 flags);

#endif /* SYSTEM_H */
