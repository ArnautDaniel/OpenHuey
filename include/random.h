#ifndef RANDOM_H
#define RANDOM_H

/* random.c: what other files call. */
#include "common.h"

typedef struct VObject VObject;

/* random.c */
extern VObject *Random_ctor(VObject *rng, s32 seed);   /* random number generator (Game +0x400000) */
extern VObject *Random_dtor(VObject *r, s32 flags);

#endif /* RANDOM_H */
