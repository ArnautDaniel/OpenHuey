/* Actor: common base of characters and other world objects (vtables 0x469C20 -> 0x469C60).
 * Only the members that are understood so far. */
#ifndef ACTOR_H
#define ACTOR_H

#include "common.h"
#include "game.h"
#include "ptmf.h"

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
    /* 0xA0 */ PTMF state;         /* behaviour state (pointer-to-member) */
    /* 0xAC */ u8 padAC[4];
    /* 0xB0 */ f32 unkB0[4] __attribute__((aligned(16)));
    /* 0xC0 */ u32 navMask;       /* triangle flags that block this actor */
    /* 0xC4 */ s32 unkC4;
    /* 0xC8 */ f32 radius;        /* collision cylinder */
    /* 0xCC */ f32 height;
    /* 0xD0 */ u8 unkD0;
    /* 0xD1 */ u8 unkD1;
} Actor;

_Static_assert(sizeof(Actor) == 0xE0, "Actor size");

/* Request for the path planner (gSceneGameF29740, vtable +0xC plan(req, 0) -> id, +0x14 length). */
typedef struct PathRequest {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u8 pad08[4];
    /* 0x0C */ u32 startTri;
    /* 0x10 */ f32 startPos[4] __attribute__((aligned(16)));
    /* 0x20 */ u32 goalTri;
    /* 0x24 */ u8 pad24[0xC];
    /* 0x30 */ f32 goalPos[4] __attribute__((aligned(16)));
    /* 0x40 */ u32 mask;              /* blocking triangle flags */
} PathRequest;

/* A noise made by character slot i (gProgress +0x778 + 0x10 * i). */
typedef struct NoiseEvent {
    /* 0x0 */ u8 level;               /* loudness */
    /* 0x1 */ u8 pad1[3];
    /* 0x4 */ s32 room;
    /* 0x8 */ s32 tri;                /* nav triangle, -1 = unknown */
    /* 0xC */ u16 exitId;             /* door it came through, 0xFFFF = none */
    /* 0xE */ u8 padE[2];
} NoiseEvent;

/* Character: Actor with path finding (vtable 0x469C60; Fiona, Hewie and the pursuers derive from it). */
typedef struct Character {
    /* 0x0000 */ Actor a;
    /* 0x00E0 */ u8 unkE0;
    /* 0x00E1 */ u8 unkE1;
    /* 0x00E2 */ u8 unkE2;
    /* 0x00E3 */ u8 unkE3;
    /* 0x00E4 */ u8 unkE4;
    /* 0x00E5 */ u8 padE5[3];
    /* 0x00E8 */ s32 unkE8;
    /* 0x00EC */ s32 unkEC;
    /* 0x00F0 */ void *motion;           /* animation player (root motion) */
    /* 0x00F4 */ s32 unkF4;
    /* 0x00F8 */ s32 moveMode;        /* 6 = following a path */
    /* 0x00FC */ s32 moveSub;         /* with moveMode 6: 0x16 planner path, 0x17 direct */
    /* 0x0100 */ s32 unk100;
    /* 0x0104 */ s32 unk104[3];
    /* 0x0110 */ f32 unk110[4] __attribute__((aligned(16)));
    /* 0x0120 */ s32 pathId;          /* planner result, -1 = none */
    /* 0x0124 */ s32 unk124;
    /* 0x0128 */ s32 unk128;
    /* 0x012C */ u8 unk12C[0x1330 - 0x12C];  /* waypoints */
    /* 0x1330 */ PathRequest req;         /* pathReq normally points here */
    /* 0x1380 */ PathRequest *pathReq;
    /* 0x1384 */ s32 unk1384;
    /* 0x1388 */ s32 unk1388;
    /* 0x138C */ u8 unk138C[0x148C - 0x138C];
    /* 0x148C */ u32 unk148C[13];
    /* 0x14C0 */ u16 unk14C0;
    /* 0x14C2 */ u8 pad14C2[2];
    /* 0x14C4 */ s32 unk14C4;         /* (read as f32 for moveSub 0x17) */
    /* 0x14C8 */ u8 pad14C8[8];
    /* 0x14D0 */ s32 unk14D0;
    /* 0x14D4 */ u8 door;             /* room exit (0..7) the character heads for, 0xFF = none */
    /* 0x14D5 */ u8 heardSlot;        /* noise event the character reacts to (0..3), 0xFF = none */
    /* 0x14D6 */ u8 pad14D6[2];
    /* 0x14D8 */ NoiseEvent heard;     /* copy of that event */
    /* 0x14E8 */ s32 state[8];        /* state block (func_002A8410 resets it); [0] 4/5 = special */
    /* 0x1508 */ s32 state2[8];
    /* 0x1528 */ u8 msgSlot;          /* message display slot (gBootMessage) */
    /* 0x1529 */ u8 pad1529;
    /* 0x152A */ u16 hearThreshold;   /* events must be louder than this */
    /* 0x152C */ s32 unk152C;         /* 10 / 15 set by the region fade (vtable +0x80) */
    /* 0x1530 */ s32 unk1530;
    /* 0x1534 */ s32 unk1534;
    /* 0x1538 */ s32 unk1538;
    /* 0x153C */ u8 unk153C;          /* 0 Fiona, 1 Hewie (set by SceneGame_ctor) */
} Character;
_Static_assert(__builtin_offsetof(Character, pathId) == 0x120, "pathId");
_Static_assert(__builtin_offsetof(Character, pathReq) == 0x1380, "pathReq");
_Static_assert(__builtin_offsetof(Character, door) == 0x14D4, "door");
_Static_assert(__builtin_offsetof(Character, motion) == 0xF0, "motion");
_Static_assert(__builtin_offsetof(Character, req) == 0x1330, "req");
_Static_assert(sizeof(PathRequest) == 0x50, "PathRequest size");
_Static_assert(__builtin_offsetof(Character, unk148C) == 0x148C, "unk148C");
_Static_assert(__builtin_offsetof(Character, state) == 0x14E8, "state");
_Static_assert(__builtin_offsetof(Character, state2) == 0x1508, "state2");
_Static_assert(__builtin_offsetof(Character, unk152C) == 0x152C, "unk152C");
_Static_assert(__builtin_offsetof(Character, heard) == 0x14D8, "heard");
_Static_assert(__builtin_offsetof(Character, hearThreshold) == 0x152A, "hearThreshold");

#endif
