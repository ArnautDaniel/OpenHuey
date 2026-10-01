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

/* Objects in Game.actors[]. */
typedef struct Actor {
    /* 0x00 */ void **vtbl;
    /* 0x04 */ PTMF state;
    /* 0x10 */ u8 unk10[2];
    /* 0x12 */ s8 unk12; /* 1 = finished: deleted at the next update */
} Actor;

#define GAME_NUM_ACTORS 4

typedef struct Game {
    /* 0x0000000 */ void **vtbl;
    /* 0x0000004 */ s32 unk4; /* mode? 1 after init, 2 after soft reset */
    /* 0x0000008 */ s32 unk8;
    /* 0x000000C */ u8 unkC; /* soft reset (Select+Start) enabled */
    /* 0x000000D */ u8 padD[3];
    /* 0x0000010 */ s32 unk10;
    /* 0x0000014 */ u8 pad14[0x20 - 0x14];
    /* 0x0000020 */ u8 unk20[0x69AC0 - 0x20];            /* sub-object, type unknown */
    /* 0x0069AC0 */ VObject unk69AC0;                    /* sub-object with vtable */
    /* 0x0069AC4 */ u8 pad69AC4[0x4009CC - 0x69AC4];
    /* 0x04009CC */ PTMF state;                          /* current state: (this->*state)() each frame */
    /* 0x04009D8 */ u8 pad4009D8[0x400A04 - 0x4009D8];
    /* 0x0400A04 */ Actor *actors[GAME_NUM_ACTORS];
    /* 0x0400A14 */ u8 pad400A14[0x14D9A40 - 0x400A14];
    /* 0x14D9A40 */ VObject unk14D9A40;                  /* actor manager? receives finished actors */
    /* 0x14D9A44 */ u8 pad14D9A44[0x14E8C90 - 0x14D9A44];
    /* 0x14E8C90 */ u8 unk14E8C90[0x14E8FBC - 0x14E8C90]; /* sub-object, type unknown */
    /* 0x14E8FBC */ u32 resetHoldFrames;                 /* frames Select+Start has been held */
} Game;

#define GAME_OFFSET_CHECK(field, off) _Static_assert(__builtin_offsetof(Game, field) == (off), #field)
GAME_OFFSET_CHECK(unkC, 0xC);
GAME_OFFSET_CHECK(unk10, 0x10);
GAME_OFFSET_CHECK(unk20, 0x20);
GAME_OFFSET_CHECK(unk69AC0, 0x69AC0);
GAME_OFFSET_CHECK(state, 0x4009CC);
GAME_OFFSET_CHECK(actors, 0x400A04);
GAME_OFFSET_CHECK(unk14D9A40, 0x14D9A40);
GAME_OFFSET_CHECK(unk14E8C90, 0x14E8C90);
GAME_OFFSET_CHECK(resetHoldFrames, 0x14E8FBC);

extern Game gGame;

void Game_Run(Game *game);
void Game_Init(Game *game);
void Game_SetState(Game *game, const PTMF *state);
void Game_StateMain(Game *game);
void Game_StateShutdown(Game *game);

#endif /* GAME_H */
