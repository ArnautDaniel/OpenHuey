#ifndef SCENE_GAME_H
#define SCENE_GAME_H

/* scene_game.c: what other files call. */
#include "common.h"

typedef struct Scene Scene;

/* scene_game.c */
extern Scene *SceneGame_ctor(Scene *g);   /* mode 3: gameplay (16 MB) */
extern void *RoomBase_dtor(void *o, s32 flags);
extern void *EventPoint_dtor(u8 *o, s32 flags);
extern void *Kind33Model_ctor(u8 *m);

/* ---- (was unsorted.h) ---- */

/* unsorted.c: what other files call. */

/* unsorted.c */
extern void AvoidPrompt_Load(u8 *o);
extern void AvoidPrompt_Update(u8 *o);

/* ---- (was scene_game_members.h) ---- */

/* scene_game_members.c: what other files call. */

/* scene_game_members.c */
extern void *Base_ctorNoop(void *p);
extern void Record20_Clear(u8 *p);   /* reset a state block */
extern void RoomSlotBytes_Clear(u8 *p);
extern void Triple_Set(u8 *p, s32 a, s32 b, s32 c);
extern void PathPlanHolder_NewRoom(u8 *o);
extern void Bytes4_Clear(u8 *p);   /* four bytes cleared */
extern void StatusTimers_Frame(u8 *o);
extern void Slots_Reset2(u8 *o);   /* shut down Game.unk14E8C90 */
extern void *NoVtable_dtor(void *o, s32 flags);
extern void Clear_C700(u8 *o);

/* scene_game.c */
extern u32 SceneGame_GetByte19034(u8 *p);   /* the effects paused */
extern void *AvoidPromptBase_dtor(u8 *o, s32 flags);

extern void *Elem_ctorNoop(void *p);

#endif /* SCENE_GAME_H */
