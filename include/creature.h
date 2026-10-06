#ifndef CREATURE_H
#define CREATURE_H

/* creature.c: what other files call. */
#include "common.h"

/* creature.c */
extern void *Creature_new(u32 size, void *place);   /* placement new */
extern void Creatures_FionaLeft(u8 *m, s32 exit);
extern void Creatures_Call40(u8 *m);
extern void Creatures_ShowMessage(u8 *m);
extern void Creatures_RemoveAll(u8 *m);

/* creature.c */
extern void *Creatures_dtor(u8 *o, s32 flags);
extern void Creatures_LoadModel(u8 *o, const void *unused);
extern void Creatures_HookMessages(u8 *o);
extern void Creatures_EnterRoom(u8 *o);
extern void Creatures_Update(u8 *o);
extern void Creatures_Draw(u8 *o);

#endif /* CREATURE_H */
