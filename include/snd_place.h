#ifndef SND_PLACE_H
#define SND_PLACE_H

/* snd_place.c: what other files call. */
#include "common.h"

typedef struct VObject VObject;

/* snd_place.c */
extern void Sound_PlayAt(VObject *snd, u32 which, f32 *pos);
extern void Sound_PlayBankAt(VObject *snd, u32 id, u32 bank, f32 *pos, s32 vol, s32 pitch);

#endif /* SND_PLACE_H */
