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
extern void Progress_SetStartEntry(Progress *p, s32 entry);
extern void Progress_LoadSoundSet(Progress *p, s32 set);
extern void Progress_StartChars(Progress *p);
extern void Progress_LoadChars(Progress *p);
extern void *Progress_CharLoadBuffer(Progress *p, u8 k);
extern s32 Progress_AnyLoading(Progress *p);
extern s32 Progress_ActivateChar(Progress *p, u32 i);   /* make active */
extern void Progress_CharsEnterRoom(Progress *p);
extern s32 Progress_CameraFollow(Progress *p, u8 idx);   /* (a u8: callers mask) */
extern void Progress_CameraOn(Progress *p, u8 idx);
extern void Progress_LoadRoomSounds(Progress *p, s32 room);
extern s32 Progress_PadCondition(Progress *p, u32 which, u32 how);   /* pad button held / pressed */
extern s32 Progress_SlotOfId(Progress *p, s32 id);   /* the slot of character kind (0xFF) */
extern s32 Progress_GameMode(Progress *p);   /* the game mode */
extern void Progress_CameraSetup(Progress *p, u8 slot, s32 a, s32 b);
extern s32 Progress_CharDone(Progress *p, u32 slot);
extern s32 Progress_UnlockDoor(Progress *p, u32 d);   /* unlock door d */
extern s32 Progress_LockDoor(Progress *p, u32 d);   /* lock door d */
extern s32 Progress_DoorPassable(Progress *p, u32 d, u32 side);   /* it opens from that side (u8) */
extern s32 Progress_ExitPassable(Progress *p, s32 room, u32 exit, u32 side);
extern s32 Progress_ExitUnlocked(Progress *p, s32 room, u32 exit);
extern s32 Progress_DoorUnlocked(Progress *p, u32 d);   /* the door is locked (u8) */
extern void Progress_DoorSetBit2(Progress *p, u32 d);
extern s32 Progress_DoorSetBit1(Progress *p, u32 d);
extern s32 Progress_DoorClearBit1(Progress *p, u32 d);
extern void Progress_WhoIsWhere(Progress *p);
extern void Progress_CharRequests(Progress *p);
extern void Progress_PursuerRequest(Progress *p);
extern void Progress_RelationChanges(Progress *p);
extern void Progress_ResolveRelations(Progress *p);
extern void Progress_OwnRequests(Progress *p);
extern void Progress_PlayerButtons(Progress *p);
extern void Progress_CharsThink(Progress *p);
extern void Progress_CharsFrame(Progress *p);
extern s32 Progress_HasRelationCmd(Progress *p, u32 k);   /* joint action pending (u8) */
extern void Progress_DrawChars(Progress *p);
extern void Progress_SetCondBit(Progress *p, s32 n);   /* set condition bit n */
extern s32 Progress_CondBit(Progress *p, s32 n);   /* condition bit n */
extern s32 Progress_IsLinked(Progress *p, s32 id);   /* returns u8 */
extern s32 Progress_LoadEventChar(Progress *p, u32 id, u32 slot);
extern void Progress_LockDoorFor(Progress *p, s32 door, s32 kind, s32 on);   /* a door's state */
extern s32 Progress_DoorOpen(Progress *p, u32 door);   /* returns u8 */
extern s32 Progress_ExitOpen(Progress *p, s32 room, s32 exit);   /* the door at that exit is open */
extern u8 Progress_CharActive(Progress *p, u32 slot);   /* active */
extern s32 Progress_CharLoading(Progress *p, u32 slot);
extern void Progress_CharLoad(Progress *p, u32 slot);
extern void Progress_CharStart2(Progress *p, u32 slot);
extern void Progress_CharStart3(Progress *p, u32 slot);
extern s32 Progress_RemoveChar(Progress *p, u32 slot, u8 quick);
extern void Progress_RemoveAll(Progress *p);
extern void Progress_Noise(Progress *p, const f32 *pos, u32 which, u8 kind, s16 a, s16 b, f32 f);
extern s32 Progress_UseDoor(Progress *p);
extern s32 Progress_FionaFlag(Progress *p);
extern void Progress_ResetParts(Progress *p);
extern s32 Progress_PlayMovie(Progress *p, const char *path, u32 kind);
extern void Progress_CutsceneSlot(Progress *p, s32 slot);   /* its cutscene motion buffer */
extern void Progress_CutsceneSlotDone(Progress *p, s32 slot);   /* ... given back */
extern void *Progress73EC80_dtor(void *o, s32 flags);
extern void Progress_LoadSpeech(Progress *p, const char *name);
extern s32 Progress_Speak(Progress *p, s32 who, s32 arg);
extern void Progress_UnloadModel(Progress *p, s32 i);
extern void Progress_CommandButtons(Progress *p);

#endif /* PROGRESS_H */
