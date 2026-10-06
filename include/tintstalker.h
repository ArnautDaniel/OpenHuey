#ifndef TINTSTALKER_H
#define TINTSTALKER_H

#include "common.h"

typedef struct Pursuer Pursuer;

/* tintstalker.c */
extern void *Kind23Model_ctor(u8 *m);
extern void *TintStalker_ctor(void *p, s32 arg);
extern u8 *TintStalker_ModelFileTable(Pursuer *p);

#endif /* TINTSTALKER_H */
