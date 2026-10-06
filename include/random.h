#ifndef RANDOM_H
#define RANDOM_H

/* random.c: what other files call. */
#include "common.h"

typedef struct VObject VObject;

/* random.c */
extern VObject *func_001A48C0(VObject *rng, s32 seed);   /* random number generator (Game +0x400000) */
extern VObject *func_001A4850(VObject *r, s32 flags);

#endif /* RANDOM_H */
