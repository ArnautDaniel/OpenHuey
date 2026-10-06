#ifndef PLACED_H
#define PLACED_H

/* placed.c: what other files call. */
#include "common.h"
#include "common.h"

/* Fixed-size pool: +0x4 base, +0xC element size, +0x10 count, +0x14 in-use flags. */
typedef struct B0_Pool {
    u32 unk0;
    u8 *base;
    u32 unk8;
    u32 elemSize;
    u32 count;
    u8 *used;
} B0_Pool;

/* placed.c */
extern void *PlacedThings_Destroy(u8 *m, s32 flags);
extern void Thing_Place(u8 *o, u32 tri, f32 *pos, f32 *rot, f32 *front);

/* placed.c */
extern void *PlacedThings_dtor(u8 *o, s32 flags);
extern void PlacedThings_Update(u8 *o);
extern void PlacedThings_Draw(u8 *o);

#endif /* PLACED_H */
