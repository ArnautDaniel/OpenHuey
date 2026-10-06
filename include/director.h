#ifndef DIRECTOR_H
#define DIRECTOR_H

/* director.c: what other files call. */
#include "common.h"

/* director.c */
extern u32 Cutscene_KindSlot(u8 *d, s32 kind);
extern s32 Cutscene_MapId(void *self, u32 id);

#endif /* DIRECTOR_H */
