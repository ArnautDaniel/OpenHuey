#ifndef SND_PLACE_H
#define SND_PLACE_H

/* snd_place.c: what other files call. */
#include "common.h"

typedef struct VObject VObject;

/* snd_place.c */
extern void func_002FF600(VObject *snd, u32 which, f32 *pos);
extern void func_002FF650(VObject *snd, u32 id, u32 bank, f32 *pos, s32 vol, s32 pitch);

#endif /* SND_PLACE_H */
