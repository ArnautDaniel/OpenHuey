#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "ptmf.h"

/*
 * The game object (gGame, ~21 MB in BSS at 0x00487A00 in the original).
 * Only fields seen in decompiled code are named; offsets are checked below.
 * It is a C++ object with a vtable at +0.
 */

/* Something with a vtable at +0 whose type isn't known yet. */
typedef struct VObject {
    void **vtbl;
} VObject;

/*
 * Base of the top-level scenes / game modes the Game owns (Game.scenes[]). Each mode
 * is a large object allocated from Game.sceneHeap: see Game_StartNextScene.
 * Vtable (Metrowerks layout): +0x8 destructor, +0xC Update(s32), +0x10 entry state (pure
 * virtual; the initial state is a virtual PTMF to it), +0x14 OnSoftReset.
 */
typedef struct Scene {
    /* 0x00 */ void **vtbl;
    /* 0x04 */ PTMF state;
    /* 0x10 */ u8 slot;       /* index in Game.scenes[] */
    /* 0x11 */ s8 request;    /* SCENE_REQ_*, consumed by Scene_Update */
    /* 0x12 */ s8 status;     /* SCENE_STATUS_*; FINISHED scenes are deleted by Game_StateMain */
    /* 0x13 */ s8 waitFrames; /* for SCENE_REQ_WAIT */
} Scene;

enum {
    SCENE_REQ_NONE = 0,
    SCENE_REQ_FINISH = 1,
    SCENE_REQ_RUN = 2,
    SCENE_REQ_RESUME = 3,
    SCENE_REQ_WAIT = 4,
};
enum {
    SCENE_STATUS_IDLE = 0,
    SCENE_STATUS_FINISHED = 1,
    SCENE_STATUS_2 = 2, /* treated like FINISHED */
    SCENE_STATUS_RUNNING = 3,
    SCENE_STATUS_WAITING = 4,
};

#define GAME_NUM_SCENES 4

typedef struct Game {
    /* 0x0000000 */ void **vtbl;
    /* 0x0000004 */ s32 nextMode;        /* scene to create when none are left (1 at boot, 2 after soft reset) */
    /* 0x0000008 */ s32 mode;            /* mode of the current scene */
    /* 0x000000C */ u8 softResetEnabled; /* Select+Start resets (in gameplay modes) */
    /* 0x000000D */ u8 padD[3];
    /* 0x0000010 */ s32 modeParam;       /* passed on to the next mode */
    /* 0x0000014 */ u8 pad14[0x20 - 0x14];
    /* 0x0000020 */ u8 unk20[0x69AC0 - 0x20];            /* sub-object, type unknown */
    /* 0x0069AC0 */ VObject unk69AC0;                    /* sub-object with vtable */
    /* 0x0069AC4 */ u8 pad69AC4[0x4009CC - 0x69AC4];
    /* 0x04009CC */ PTMF state;                          /* current state: (this->*state)() each frame */
    /* 0x04009D8 */ u8 pad4009D8[0x400A00 - 0x4009D8];
    /* 0x0400A00 */ void **scenesVtbl;                   /* the scene table is an object of its own */
    /* 0x0400A04 */ Scene *scenes[GAME_NUM_SCENES];
    /* 0x0400A14 */ u8 pad400A14[0x14D9A40 - 0x400A14];
    /* 0x14D9A40 */ VObject sceneHeap;                   /* +0x10 alloc(size), +0x14 free(ptr) */
    /* 0x14D9A44 */ u8 pad14D9A44[0x14E8C90 - 0x14D9A44];
    /* 0x14E8C90 */ u8 unk14E8C90[0x14E8FBC - 0x14E8C90]; /* sub-object, type unknown */
    /* 0x14E8FBC */ u32 resetHoldFrames;                 /* frames Select+Start has been held */
} Game;

#define GAME_OFFSET_CHECK(field, off) _Static_assert(__builtin_offsetof(Game, field) == (off), #field)
GAME_OFFSET_CHECK(softResetEnabled, 0xC);
GAME_OFFSET_CHECK(modeParam, 0x10);
GAME_OFFSET_CHECK(unk20, 0x20);
GAME_OFFSET_CHECK(unk69AC0, 0x69AC0);
GAME_OFFSET_CHECK(state, 0x4009CC);
GAME_OFFSET_CHECK(scenes, 0x400A04);
GAME_OFFSET_CHECK(sceneHeap, 0x14D9A40);
GAME_OFFSET_CHECK(unk14E8C90, 0x14E8C90);
GAME_OFFSET_CHECK(resetHoldFrames, 0x14E8FBC);

extern Game gGame;

typedef struct PTMF PTMF;

/* game.c */

#endif /* GAME_H */
