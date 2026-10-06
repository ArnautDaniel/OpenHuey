#ifndef LORENZO_H
#define LORENZO_H

/* lorenzo.c: what other files call. */
#include "common.h"

typedef struct Pursuer Pursuer;

/* lorenzo.c */
extern Pursuer *Lorenzo_dtor(Pursuer *p, s32 flags);
extern f32 Lorenzo_TurnRate(Pursuer *p);
extern void Lorenzo_ActionOffsets(Pursuer *p, s32 kind, f32 *out);
extern void Lorenzo_ExitDone(Pursuer *p);
extern void Lorenzo_AttackTable(Pursuer *p, s8 situation);
extern f32 Lorenzo_AttackRange(Pursuer *p);
extern f32 Lorenzo_ReachHewie(Pursuer *p);
extern void Lorenzo_Footsteps(Pursuer *p);
extern void Lorenzo_Update(Pursuer *p);
extern u8 *Lorenzo_ModelFileTable(Pursuer *p);
extern u8 *Lorenzo_ModelFiles(Pursuer *p);
extern void Lorenzo_Setup(Pursuer *p);
extern Pursuer *Lorenzo2_dtor(Pursuer *p, s32 flags);
extern s32 Lorenzo2_InStance2Anim(Pursuer *p);
extern void Lorenzo2_BonePositions(Pursuer *p, s32 *e, f32 *a, f32 *b);
extern void Lorenzo2_AttackTable(Pursuer *p, s8 situation);
extern s32 Lorenzo2_SlowWalkAnim(Pursuer *p);
extern s32 Lorenzo2_AttackAnimB(Pursuer *p);
extern s32 Lorenzo2_AttackAnimA(Pursuer *p);
extern void Lorenzo2_Update(Pursuer *p);
extern void Lorenzo2_Setup(Pursuer *p);
extern void Lorenzo2_SlamDust(Pursuer *p);
extern void Lorenzo2_BlowSparks(Pursuer *p);
extern void Lorenzo2_StatePlayOut(Pursuer *p);
extern void Lorenzo2_StateSinkBehind(Pursuer *p);
extern s32 Lorenzo2_SlamImpact(Pursuer *p);
extern void Lorenzo2_StartGrab(Pursuer *p);
extern void Lorenzo2_StateSweep(Pursuer *p);
extern void Lorenzo2_StateAB80(Pursuer *p);
extern void Lorenzo2_StateApproach(Pursuer *p);
extern void Lorenzo2_StateGrab(Pursuer *p);
extern void Lorenzo2_StateRise(Pursuer *p);
extern void Lorenzo2_StartStalkBelow(Pursuer *p);
extern void Lorenzo2_StateUnderFloor(Pursuer *p);
extern void Lorenzo2_StateSink(Pursuer *p);
extern void Lorenzo2_StartSink(Pursuer *p);
extern s32 Kind39_SlamImpact(Pursuer *p);   /* (lorenzo.c) his slam at its impact key, second form */
extern u8 *Kind39_ModelFileTable(Pursuer *p);

typedef struct Pursuer Pursuer;

/* lorenzo.c */
extern void *Kind12Model_ctor(u8 *m);
extern void *Lorenzo2Model_ctor(u8 *m);
extern void *LorenzoModel_ctor(u8 *m);
extern void *Kind12_ctor(void *p, s32 arg);
extern void *Lorenzo_ctor(void *p, s32 arg);
extern void *Kind39_ctor(void *p, s32 arg);
extern void *Lorenzo2_ctor(void *p, s32 arg);
extern u8 *Lorenzo2_ModelFileTable(Pursuer *p);

extern void Lorenzo2Model_Hanging(u8 *m);
extern void Lorenzo2Model_Strands(u8 *m);

#endif /* LORENZO_H */
