#ifndef LOADER_H
#define LOADER_H

/* loader.c: what other files call. */
#include "common.h"

typedef struct VObject VObject;

/* loader.c */
extern void Loader_Init(u8 *l);
extern void Loader_RegisterAll(u8 *l);
extern void Loader_Tick(u8 *l);   /* file loader tick */
extern void *Loader_dtor(VObject *l, s32 flags);
extern void Loader_CloseAll(u8 *l);

#endif /* LOADER_H */
