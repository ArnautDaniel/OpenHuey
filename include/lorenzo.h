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
extern u8 *func_002F9000(Pursuer *p);
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
extern void func_00309680(Pursuer *p);
extern void func_00309890(Pursuer *p);
extern void func_0030B1E0(Pursuer *p);
extern void func_0030B7C0(Pursuer *p);
extern s32 func_0030BB70(Pursuer *p);
extern void func_0030A4D0(Pursuer *p);
extern void func_0030A650(Pursuer *p);
extern void func_0030AB80(Pursuer *p);
extern void func_0030AE00(Pursuer *p);
extern void func_0030A210(Pursuer *p);
extern void func_0030A850(Pursuer *p);
extern void func_0030AF20(Pursuer *p);
extern void func_0030B240(Pursuer *p);
extern void func_0030B540(Pursuer *p);
extern void func_0030B840(Pursuer *p);
extern s32 func_00365850(Pursuer *p);   /* (lorenzo.c) his slam at its impact key, second form */
extern u8 *func_00365D10(Pursuer *p);

#endif /* LORENZO_H */
