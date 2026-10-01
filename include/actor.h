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

/* Request for the path planner (gSceneGameF29740, vtable +0xC plan(req, 0) -> id, +0x14 length). */
typedef struct PathRequest {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 pad04[8];
    /* 0x0C */ u32 startTri;
    /* 0x10 */ f32 startPos[4] __attribute__((aligned(16)));
    /* 0x20 */ u32 goalTri;
    /* 0x24 */ u8 pad24[0xC];
    /* 0x30 */ f32 goalPos[4] __attribute__((aligned(16)));
    /* 0x40 */ u32 mask;              /* blocking triangle flags */
} PathRequest;

/* Character: Actor with path finding (vtable 0x469C60; Fiona, Hewie and the pursuers derive from it). */
typedef struct Character {
    /* 0x0000 */ Actor a;
    /* 0x00E0 */ u8 padE0[0xF0 - sizeof(Actor)];
    /* 0x00F0 */ void *motion;           /* animation player (root motion) */
    /* 0x00F4 */ u8 padF4[0x120 - 0xF4];
    /* 0x0120 */ s32 pathId;          /* planner result, -1 = none */
    /* 0x0124 */ u8 pad124[0x1380 - 0x124];
    /* 0x1380 */ PathRequest *pathReq;
    /* 0x1384 */ u8 pad1384[0x14D4 - 0x1384];
    /* 0x14D4 */ u8 door;             /* room exit (0..7) the character heads for, 0xFF = none */
    /* 0x14D5 */ u8 pad14D5[0x152C - 0x14D5];
    /* 0x152C */ s32 unk152C;         /* 10 / 15 set by the region fade (vtable +0x80) */
} Character;
_Static_assert(__builtin_offsetof(Character, pathId) == 0x120, "pathId");
_Static_assert(__builtin_offsetof(Character, pathReq) == 0x1380, "pathReq");
_Static_assert(__builtin_offsetof(Character, door) == 0x14D4, "door");

#endif
