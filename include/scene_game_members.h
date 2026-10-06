#ifndef SCENE_GAME_MEMBERS_H
#define SCENE_GAME_MEMBERS_H

/* scene_game_members.c: what other files call. */
#include "common.h"

/* scene_game_members.c */
extern void *Base_ctorNoop(void *p);
extern void Door_ReleaseRequest(u8 *p);
extern void Record20_Clear(u8 *p);   /* reset a state block */
extern void Summoner_Reset(u8 *p);
extern void *Obstacles_ctor(u8 *p);
extern void *PlacedObjects_ctor(u8 *p);
extern void *Doors_ctor(u8 *p);
extern void RoomSlotBytes_Clear(u8 *p);
extern void Progress_SubReset(u8 *p);   /* a fresh save's progress */
extern void RoomMgr_Clear(u8 *m);
extern void *DimMessage_ctor(u8 *p);
extern void RoomMgr_ctor(u8 *m);
extern void Progress_Reset(u8 *prog);
extern void Triple_Set(u8 *p, s32 a, s32 b, s32 c);
extern void SlotCmds_Reset(u8 *e);
extern void Relations_Reset(u8 *e);
extern void Lights_RoomStart(u8 *o);
extern void Creatures_LoadModel(u8 *o, const void *unused);
extern void RoomMgr_LoadRoom(u8 *rm, u32 room, s32 slot);
extern void PathPlanHolder_NewRoom(u8 *o);
extern void RoomEffects_Reset(u8 *o);
extern void RoomEffects_delete(void *p);   /* delete (effects' heap) */
extern void EffectMgr_Reset(u8 *o);
extern void EffectMgr_free(void *p);   /* free from the scene heap? */
extern s32 RoomMgr_SlotLoading(u8 *rm, s32 slot);   /* room slot still loading */
extern void Creatures_HookMessages(u8 *o);
extern void RoomMgr_MakeCurrent(u8 *rm, s32 slot);
extern void PlacedObject_ToDef(u8 *o);
extern void PlacedObject_StartAnim(u8 *o, s32 id);
extern void *RoomEffects_new(u32 size, void *place);   /* placement new */
extern void EffectMgr_RoomReset(u8 *o);
extern void EffectMgr_Remove(u8 *o, s32 slot);
extern void Map_FindRoom(u8 *m, s32 room);
extern void Map_TurnTo(u8 *m, s8 page);
extern void Map_PageFrame(u8 *m);   /* the map, each frame */
extern void Map_BackToPlayer(u8 *m);
extern void Progress_CopyState(const u8 *s, u8 *d);   /* copies saved flags into Progress */
extern void Bytes4_Clear(u8 *p);   /* four bytes cleared */
extern void *NavGroups_dtor(void *o, s32 flags);
extern void RoomEffects_Release(u8 *fx, s32 n);   /* effect slot n gone */
extern s32 RoomEffects_Send(u8 *o, s32 n, void *arg);
extern void *EffectMgr_new(u32 size, void *place);   /* placement new */
extern s32 EffectMgr_Start(u8 *mgr, s32 slot, void *params);
extern void Creatures_EnterRoom(u8 *o);
extern void StatusTimers_Frame(u8 *o);
extern void Summoner_Take(u8 *o, u8 kind);
extern void Summoner_SetCooldown(u8 *o, s32 sec);
extern void Summoner_LessCooldown(u8 *o, s32 sec);
extern void Summoner_RoomStart(u8 *o);
extern void CharRequest_Clear(u8 *r);
extern void OwnRequest_Clear(u8 *r);
extern void RoomMgr_Draw(u8 *rm, s32 slot);
extern void Creatures_Update(u8 *o);
extern void RoomMgr_Leave(u8 *rm);
extern void RoomMgr_Update(u8 *rm);
extern void PlayTime_Tick(u8 *t);
extern void PlacedThings_Update(u8 *o);
extern void RoomEffects_Update(u8 *o);
extern void EffectMgr_Update(u8 *mgr);
extern void *RoomEffects_Get(void *effects, s32 k);   /* effect slot n */
extern void Creatures_Draw(u8 *o);
extern void PlacedThings_Draw(u8 *o);
extern void RoomEffects_Draw(u8 *o);
extern void EffectMgr_Draw(u8 *mgr);
extern void ScreenFade_Level(u8 *fade, f32 t);   /* the screen fade's level, 0..1 */
extern void ScreenFade_Frame(u8 *fade, s32 mode);
extern void Noise_Make(u8 *n, s32 loud, s32 room, s32 tri, s32 door);   /* make a noise */
extern void Summoner_Noise(u8 *o, u8 *n);
extern s32 RoomMgr_SlotLoaded(u8 *o, s32 k);
extern void Slots_Reset2(u8 *o);   /* shut down Game.unk14E8C90 */
extern void Lights_ReleaseVram(u8 *o);
extern void *NoVtable_dtor(void *o, s32 flags);
extern void Renderer_Call5C(void);
extern void RoomMgr_LoadSlot(u8 *o, s32 k);
extern void Clear_C700(u8 *o);
extern void MovieLib_Shutdown(u8 *o);
extern void *MovieSys_dtor(u8 *o, s32 flags);
extern s32 EffectMgr_Query(u8 *o, s32 slot);   /* a slot's effect state (3: ended) */

#endif /* SCENE_GAME_MEMBERS_H */
