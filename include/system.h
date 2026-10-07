#ifndef SYSTEM_H
#define SYSTEM_H

/* system.c: what other files call. */
#include "common.h"

typedef struct Game Game;
typedef struct VObject VObject;

/* system.c */
extern void *GameBase_ctor(Game *game);   /* base class constructor */
extern void *System_ctor(u8 *s);   /* Game.unk69AC0 */
extern void *Obj46AD88_dtor(u8 *o, s32 flags);
extern void *SceneTable_ctor(u8 *t);   /* scene table (+0x400A00) */
extern void *SmallPool_ctor(u8 *p);   /* object pool (+0x14D9DD0) */
extern void *BigPool_ctor(u8 *p);   /* object pool (+0x14DC530) */
extern void *Slots_ctor(u8 *o);   /* Game.unk14E8C90 */
extern void Slots_Init(u8 *o);   /* init Game.unk14E8C90 */
extern void *SystemBase_dtor(u8 *o, s32 flags);
extern void *IopBuffers_dtor(u8 *e, s32 flags);
extern void *IopArray_dtor(u8 *o, s32 flags);

/* ---- (was runtime.h) ---- */

/* runtime.c: what other files call. */

typedef struct PTMF PTMF;

/* runtime.c */
extern s32 __ptmf_cmpr(const PTMF *a, const PTMF *b);   /* __ptmf_cmpr */

typedef struct Game Game;
typedef struct PTMF PTMF;

/* system.c */
extern void Game_Init(Game *game);
extern void Game_Run(Game *game);
extern void Game_SetState(Game *game, const PTMF *state);
extern void Game_StateMain(Game *game);
extern void Game_StateShutdown(Game *game);
extern void Game_StartNextScene(Game *game);
extern void *QuadEntry_dtor(u8 *o, s32 flags);
extern void *Game_dtor(u8 *g, s32 flags);

#endif /* SYSTEM_H */
