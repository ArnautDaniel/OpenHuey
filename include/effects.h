#ifndef EFFECTS_H
#define EFFECTS_H

/* effects.c: what other files call. */
#include "common.h"

/* effects.c */
extern void Drawer_Submit(u8 *d);   /* draw a textured quad (corners +0x14, record +0x10) */
extern void DepthBand_Queue(u8 *d, f32 a, f32 from, f32 to, f32 b);   /* depth of field */

/* ---- (was props.h) ---- */

/* props.c: what other files call. */

/* props.c */
extern void ModelDraw_Fill(u8 *d, const f32 *pos, const f32 *rot, s32 a, s32 b, s32 layer);

/* effects.c */
extern void **DustMoteSource_Init(void **o);
extern void **SpriteBurst_InitDust(void **o);
extern void **Splash_InitEvent(void **o);
extern void **SpeckSwarm_InitEvent(void **o);
extern void **Fog_Init(void **o);
extern void **ScreenBlend_Init(void **o);
extern void **DepthRange_Init(void **o);
extern void **Butterflies_Init(void **o, u32 n);
extern void *EvEffect7F_Init(void *p);   /* a EvEffect7F_vtable effect */
extern void *EvEffect86_Init(void *p);   /* a EvEffect86_vtable effect */
extern void RoomEffects_Reset(u8 *o);
extern void RoomEffects_delete(void *p);   /* delete (effects' heap) */
extern void EffectMgr_Reset(u8 *o);
extern void EffectMgr_free(void *p);   /* free from the scene heap? */
extern void *RoomEffects_new(u32 size, void *place);   /* placement new */
extern void EffectMgr_RoomReset(u8 *o);
extern void EffectMgr_Remove(u8 *o, s32 slot);
extern void RoomEffects_Release(u8 *fx, s32 n);   /* effect slot n gone */
extern s32 RoomEffects_Send(u8 *o, s32 n, void *arg);
extern void *EffectMgr_new(u32 size, void *place);   /* placement new */
extern s32 EffectMgr_Start(u8 *mgr, s32 slot, void *params);
extern void RoomEffects_Update(u8 *o);
extern void EffectMgr_Update(u8 *mgr);
extern void *RoomEffects_Get(void *effects, s32 k);   /* effect slot n */
extern void RoomEffects_Draw(u8 *o);
extern void EffectMgr_Draw(u8 *mgr);
extern s32 EffectMgr_Query(u8 *o, s32 slot);   /* a slot's effect state (3: ended) */

#endif /* EFFECTS_H */
