/* Shared bits of the character model classes (model.c, stalker_models.c). */
#ifndef MODEL_H
#define MODEL_H

#include "common.h"
#include "stalker_models.h"

extern void *Helper469D00_vtable[], *D_0046ADA0[], *ModelBase_vtable[], *Model_vtable[], *HumanModel_vtable[],
    *IK2_vtable[], *D_0046B1C0[];

typedef struct Progress Progress;

/* model.c */
extern void *Model_new(u32 size, void *p);   /* placement new */
extern void *ModelBase_ctor(u8 *m);
extern void CharLoad_PartnerModel(Progress *p, u32 slot);
extern void *DogModel_ctor(u8 *m, u8 kind);
extern void **DustMoteSource_Init(void **o);
extern void **SpriteBurst_InitDust(void **o);
extern void **Effect71000_Init(void **o);
extern void **Splash_InitEvent(void **o);
extern void **SpeckSwarm_InitEvent(void **o);
extern void **Fog_Init(void **o);
extern void **ScreenBlend_Init(void **o);
extern void *Costume8Model_ctor(u8 *m);
extern void **Effect6FF60_Init(void **o);
extern void **DepthRange_Init(void **o);
extern void **Butterflies_Init(void **o, u32 n);
extern void *DogModelB_ctor(u8 *m, u8 kind);
extern void *DogModelA_ctor(u8 *m, u8 kind);
extern void *Costume7Model_ctor(u8 *m, s32 kind);
extern void *Costume6Model_ctor(u8 *m, s32 kind);
extern void *Costume3Model_ctor(u8 *m);
extern void *Costume2Model_ctor(u8 *m);
extern void *FionaModel_ctor(u8 *m);
extern void CharLoad_DogModelB(Progress *p, u32 slot);
extern void *EvEffect7F_Init(void *p);   /* a EvEffect7F_vtable effect */
extern void *EvEffect86_Init(void *p);   /* a EvEffect86_vtable effect */
extern void CharLoad_FionaModel(Progress *p, u32 slot);
extern void CharLoad_FionaClothes(Progress *p, u32 slot);
extern void CharLoad_DogModelA(Progress *p, u32 slot);
extern void CharLoad_Costume8(Progress *p, u32 slot);
extern void CharLoad_Costume7(Progress *p, u32 slot);
extern void CharLoad_Costume6(Progress *p, u32 slot);
extern void CharLoad_Costume3(Progress *p, u32 slot);
extern void CharLoad_Costume2(Progress *p, u32 slot);
extern void HumanModel_Loaded(u8 *m);
extern void Model_Loaded(u8 *m);
extern void Model_ResetMotion(u8 *m);   /* a model's motion reset */
extern void SpringSet_Clear(u8 *set);
extern void Sphere_Set(u8 *col, s32 bone, f32 x, f32 y, f32 z, f32 r);
extern void Motion_PlayWith(u8 *m, s32 anim, u32 flags, s32 variant, f32 blend);   /* blended in */
extern void Motion_Play(u8 *m, s32 anim, s32 variant);
extern void Motion_PlayBlend(u8 *m, s32 anim, s32 blend, s32 variant);
extern void Motion_PlayBlend8(u8 *m, s32 anim, s32 blend);
extern void Motion_PlayOwnBlend(u8 *m, s32 anim, s32 variant);
extern void Motion_Freeze(u8 *m);
extern s32 Motion_AnimIndex(u8 *m, s32 anim);
extern void Motion_Unfreeze(u8 *m);
extern s32 Gesture_Update(u8 *g, f32 *stick);
extern void Motion_RootTranslation(u8 *m, f32 *out, f32 dt);   /* root motion (variant) */
extern void Motion_RootMovement(u8 *m, f32 *out, f32 dt);   /* root translation */
extern void Track_Sample(void *track, f32 *out, f32 t);   /* sample a track */
extern f32 Motion_RootRotation(u8 *m, f32 dt);   /* root rotation (yaw delta) */
extern s32 Motion_EventFlags(u8 *m, s32 layer, s32 dt, u32 loop);   /* animation state flags (u8) */
extern s32 Motion_FootPos(u8 *m, f32 *out, s32 left, f32 dt, f32 zscale);   /* foot on the ground (u8) */
extern void Motion_OntoFloor(u8 *m, f32 *p, u8 *a);   /* drop a point onto the floor */
extern void Motion_Hands(u8 *m);
extern void Motion_Eyes(u8 *m);
extern void Motion_Update(u8 *m);
extern void IK2_Setup(u8 *ik, void *skel, s32 root, s32 mid, s32 end, f32 len1, f32 len2, f32 bend);
extern void IK3_Setup(u8 *ik, void *skel, s32 root, s32 knee, s32 hock, s32 foot, f32 len1, f32 len2, f32 len3, f32 bend1, f32 bend2, f32 reach);
extern void SpringSet_Begin(u8 *s);   /* begin a step */
extern void SpringSet_Step(u8 *s);   /* one step */
extern void SpringSet_Finish(u8 *s);   /* finish */
extern void Model_Draw(u8 *m, s32 layer, s32 a, s32 b);
extern void Mtx_TurnY(f32 (*m)[4], f32 a);   /* turn about y */
extern void Motion_PlayTable(u8 *m, s32 anim, s32 variant);   /* play anim blended with another (-1 none) */
extern void Motion_PlayDriven(u8 *m, s32 anim);
extern void Motion_CutsceneTime(u8 *m, s32 frame);   /* its motion at the frame */
extern void Motion_LookAt(u8 *m, const f32 *target, f32 *pitch, f32 *turn);   /* head angles to a point */
extern u32 Motion_FootDownPrev(void *motion, s32 foot, f32 ofs);   /* foot planted while standing (u8) */
extern void *HumanModel_BaseCtor(u8 *m);
extern void *IK2_Destroy(void *e, s32 flags);
extern void *IK3_Destroy(void *e, s32 flags);
extern void *HangPoint_dtor(void *e, s32 flags);
extern void *Part60_dtor(void *e, s32 flags);
extern void *BoneHangPoint_dtor(void *e, s32 flags);
extern void *SwayPointB_dtor(void *e, s32 flags);
extern void *Part50_dtor(void *e, s32 flags);
extern void *HairPoint_dtor(void *e, s32 flags);
extern void *HangingPart_dtor(void *e, s32 flags);
extern void *EventHumanModel_ctor(u8 *m, s32 kind);
extern void *Kind12Model_ctor(u8 *m);
extern void *Lorenzo2Model_ctor(u8 *m);
extern void *LorenzoModel_ctor(u8 *m);
extern void *DebilitasModel_ctor(u8 *m);
extern void *Kind09Model_ctor(u8 *m);
extern void *Kind23Model_ctor(u8 *m);
extern void *RiccardoModel_ctor(u8 *m);
extern void *DaniellaModel_ctor(u8 *m);
extern void CharLoad_Kind33Model(Progress *p, u32 slot);
extern void CharLoad_Kind23Model(Progress *p, u32 slot);
extern void CharLoad_Kind18Model(Progress *p, u32 slot);
extern void CharLoad_Kind14Model(Progress *p, u32 slot);
extern void CharLoad_Kind13Model(Progress *p, u32 slot);
extern void CharLoad_PlainModel(Progress *p, u32 slot);
extern void CharLoad_Kind12Model(Progress *p, u32 slot);
extern void CharLoad_LorenzoModel(Progress *p, u32 slot);
extern void CharLoad_Lorenzo2Model(Progress *p, u32 slot);
extern void CharLoad_Kind09Model(Progress *p, u32 slot);
extern void CharLoad_RiccardoModel(Progress *p, u32 slot);
extern void CharLoad_DaniellaModel(Progress *p, u32 slot);
extern void CharLoad_DebilitasModel(Progress *p, u32 slot);
extern void Motion_PlayTableNoCheck(u8 *m, s32 anim);
extern f32 Vec_LineDistance(const f32 *a, const f32 *b, const f32 *c);   /* pt's distance from the line */

/* the human model base's destruction: back down its vtables, then (flags > 0) delete */
static inline void *HumanModel_Destroy(u8 *m, s32 flags) {
    if (m != NULL) {
        AT(m, 0x0, void **) = HumanModel_vtable;
        AT(m, 0x988, void **) = IK2_vtable;
        AT(m, 0x928, void **) = IK2_vtable;
        AT(m, 0x0, void **) = Model_vtable;
        AT(m, 0x0, void **) = ModelBase_vtable;
        AT(m, 0x1D0, void **) = D_0046B1C0;
        AT(m, 0x1D0, void **) = Helper469D00_vtable;
        AT(m, 0x10, void **) = D_0046ADA0;
        AT(m, 0x10, void **) = Helper469D00_vtable;
        if ((s16)flags > 0) {
            StalkerModel_delete(m);
        }
    }
    return m;
}

#endif
