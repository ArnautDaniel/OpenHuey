#ifndef PROGRESS_H
#define PROGRESS_H

#include "common.h"

/*
 * Game progress: story flags and small counters. This is SceneGame's second base
 * class (at SceneGame+0x40, vtable 0x46A260, 33 virtuals); gProgress points to it.
 * Restored from the resident save buffer when a game starts (see SceneGame_StateEntry).
 * Only the fields used by the accessors below are known.
 */
#define PROGRESS_NUM_FLAGS 46

typedef struct Progress {
    /* 0x000 */ void **vtbl;
    /* 0x004 */ u32 unk4;
    /* 0x008 */ u32 flags[2];       /* PROGRESS_NUM_FLAGS story flags */
    /* 0x010 */ u8 pad10[0x9C - 0x10];
    /* 0x09C */ u8 vars[0x100 - 0x9C]; /* byte variables, indexed by u8 id */
    /* 0x100 */ u32 bits[9];        /* second bit set, see Progress_IsBitClear */
    /* 0x124 */ u32 roomFlags[1];   /* the doors' states, a word per door (bit 0 Progress_CurRoomFlag, bit 3 unlocked, 4..7 lock sides) */
} Progress;
_Static_assert(__builtin_offsetof(Progress, vars) == 0x9C, "vars");
_Static_assert(__builtin_offsetof(Progress, bits) == 0x100, "bits");
_Static_assert(__builtin_offsetof(Progress, roomFlags) == 0x124, "roomFlags");

extern Progress *gProgress;

s32 Progress_TestFlag(Progress *p, u32 id);
void Progress_SetFlag(Progress *p, u32 id);
void Progress_ClearFlag(Progress *p, u32 id);
u32 Progress_GetVar(Progress *p, u32 id);              /* id and result are u8 */
void Progress_SetVar(Progress *p, u32 id, u32 value);   /* u8 id, u8 value */
void Progress_IncVar(Progress *p, u32 id);
s32 Progress_IsBitClear(Progress *p, s32 id);
s32 Progress_CurRoomFlag(Progress *p, s32 room, u32 exit);   /* door at that exit: state bit 0 */

#endif /* PROGRESS_H */
