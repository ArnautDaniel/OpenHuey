/* Actor: common base of characters and other world objects (vtables 0x469C20 -> 0x469C60).
 * Only the members that are understood so far. */
#ifndef ACTOR_H
#define ACTOR_H

#include "common.h"
#include "game.h"

#define ACTOR_AT(a, off, type) (*(type *)((u8 *)(a) + (off)))

typedef struct Actor {
    /* 0x00 */ void **vtbl;
    /* 0x04 */ u8 pad04[0xC];
    /* 0x10 */ f32 pos[4] __attribute__((aligned(16)));
    /* 0x20 */ s32 slot;          /* character slot (gCharacters), 0 Fiona, 1 Hewie, 2 pursuer */
    /* 0x24 */ u32 flags24;       /* 0x2000000 after construction */
    /* 0x28 */ u8 active;         /* 1 = updated by the gameplay tick */
    /* 0x29 */ u8 disabled;       /* nonzero: ignored by contact tests */
    /* 0x2A */ u8 unk2A;
    /* 0x2B */ u8 unk2B;
    /* 0x2C */ u8 unk2C;          /* 1 = silent (no positional sounds) */
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 pad2E[2];
    /* 0x30 */ s32 room;          /* room the actor is in */
    /* 0x34 */ u32 navTri;        /* current nav mesh triangle */
    /* 0x38 */ u32 prevNavTri;
    /* 0x3C */ u8 pad3C[4];
    /* 0x40 */ f32 prevPos[4] __attribute__((aligned(16)));
    /* 0x50 */ f32 angle[4];      /* rotation; [1] = heading (yaw) */
    /* 0x60 */ f32 rot[4][4] __attribute__((aligned(16)));   /* orientation matrix */
    /* 0xA0 */ u8 padA0[0x10];
    /* 0xB0 */ f32 unkB0[4] __attribute__((aligned(16)));
    /* 0xC0 */ u32 navMask;       /* triangle flags that block this actor */
    /* 0xC4 */ u8 padC4[4];
    /* 0xC8 */ f32 radius;        /* collision cylinder */
    /* 0xCC */ f32 height;
    /* 0xD0 */ u8 unkD0;
    /* 0xD1 */ u8 unkD1;
} Actor;

#endif
