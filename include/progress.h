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

/* progress.c */
extern s32 Progress_TestFlag(Progress *p, u32 id);
extern void Progress_SetFlag(Progress *p, u32 id);
extern void Progress_ClearFlag(Progress *p, u32 id);
extern u32 Progress_GetVar(Progress *p, u32 id);   /* id and result are u8 */
extern void Progress_SetVar(Progress *p, u32 id, u32 value);   /* u8 id, u8 value */
extern void Progress_IncVar(Progress *p, u32 id);
extern s32 Progress_IsBitClear(Progress *p, s32 id);
extern s32 Progress_CurRoomFlag(Progress *p, s32 room, u32 exit);   /* door at that exit: state bit 0 */
extern void func_001779B0(Progress *p, s32 entry);
extern void func_0016D350(Progress *p, s32 set);
extern void func_00176650(Progress *p);
extern void func_00176550(Progress *p);
extern void *func_001776B0(Progress *p, u8 k);
extern s32 func_001764C0(Progress *p);
extern s32 func_001771A0(Progress *p, u32 i);   /* make active */
extern void func_001765D0(Progress *p);
extern s32 func_00179170(Progress *p, u8 idx);   /* (a u8: callers mask) */
extern void func_001792C0(Progress *p, u8 idx);
extern void func_0016D480(Progress *p, s32 room);
extern s32 func_00176DD0(Progress *p, u32 which, u32 how);   /* pad button held / pressed */
extern s32 func_001770D0(Progress *p, s32 id);   /* the slot of character kind (0xFF) */
extern s32 func_00177620(Progress *p);   /* the game mode */
extern void func_001793A0(Progress *p, u8 slot, s32 a, s32 b);
extern s32 func_00177200(Progress *p, u32 slot);
extern s32 func_00178450(Progress *p, u32 d);   /* unlock door d */
extern s32 func_00178500(Progress *p, u32 d);   /* lock door d */
extern s32 func_00178200(Progress *p, u32 d, u32 side);   /* it opens from that side (u8) */
extern s32 func_00178300(Progress *p, s32 room, u32 exit, u32 side);
extern s32 func_001785B0(Progress *p, s32 room, u32 exit);
extern s32 func_00178610(Progress *p, u32 d);   /* the door is locked (u8) */
extern void func_00178630(Progress *p, u32 d);
extern s32 func_00178A60(Progress *p, u32 d);
extern s32 func_00178A30(Progress *p, u32 d);
extern void func_00175DE0(Progress *p);
extern void func_00175430(Progress *p);
extern void func_001776F0(Progress *p);
extern void func_00173670(Progress *p);
extern void func_00173B60(Progress *p);
extern void func_001739A0(Progress *p);
extern void func_00174920(Progress *p);
extern void func_00176440(Progress *p);
extern void func_001762B0(Progress *p);
extern s32 func_00177870(Progress *p, u32 k);   /* joint action pending (u8) */
extern void func_00176160(Progress *p);
extern void func_00177630(Progress *p, s32 n);   /* set condition bit n */
extern s32 func_00177670(Progress *p, s32 n);   /* condition bit n */
extern s32 func_00177770(Progress *p, s32 id);   /* returns u8 */
extern s32 func_0016D670(Progress *p, u32 id, u32 slot);
extern void func_001780C0(Progress *p, s32 door, s32 kind, s32 on);   /* a door's state */
extern s32 func_001788F0(Progress *p, u32 door);   /* returns u8 */
extern s32 func_00178980(Progress *p, s32 room, s32 exit);   /* the door at that exit is open */
extern u8 func_00177160(Progress *p, u32 slot);   /* active */
extern s32 func_00177260(Progress *p, u32 slot);
extern void func_001772B0(Progress *p, u32 slot);
extern void func_00177300(Progress *p, u32 slot);
extern void func_00177350(Progress *p, u32 slot);
extern s32 func_001773A0(Progress *p, u32 slot, u8 quick);
extern void func_001766D0(Progress *p);
extern void func_00177FA0(Progress *p, const f32 *pos, u32 which, u8 kind, s16 a, s16 b, f32 f);
extern s32 func_00178660(Progress *p);
extern s32 func_00176D80(Progress *p);
extern void func_00176720(Progress *p);
extern s32 func_001768B0(Progress *p, const char *path, u32 kind);
extern void func_0016D050(Progress *p, s32 slot);   /* its cutscene motion buffer */
extern void func_0016CF50(Progress *p, s32 slot);   /* ... given back */
extern void *func_0016CC40(void *o, s32 flags);
extern void func_0016CEC0(Progress *p, const char *name);
extern s32 func_0016CD60(Progress *p, s32 who, s32 arg);
extern void func_0016D2F0(Progress *p, s32 i);
extern void func_00174270(Progress *p);

#endif /* PROGRESS_H */
