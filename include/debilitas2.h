#ifndef DEBILITAS2_H
#define DEBILITAS2_H

/* debilitas2.c: what other files call. */
#include "common.h"

typedef struct Pursuer Pursuer;

/* debilitas2.c */
extern void Debilitas2_DoorOffset(Pursuer *p, s32 side, f32 *out);
extern void Debilitas2_ActionOffsets(Pursuer *p, s32 kind, f32 *out);
extern void Debilitas2_ExitDone(Pursuer *p);
extern u32 Debilitas2_BlockFlags(Pursuer *p);
extern f32 Debilitas2_SpeedBase(Pursuer *p);
extern f32 Debilitas2_SpeedTop(Pursuer *p);
extern f32 Debilitas2_AttackRange(Pursuer *p);
extern f32 Debilitas2_ReachHewie(Pursuer *p);
extern f32 Debilitas2_AttackAngle(Pursuer *p);
extern f32 Debilitas2_Dist2E8(Pursuer *p);
extern f32 Debilitas2_ReachFiona(Pursuer *p);
extern f32 Debilitas2_LookSwing(Pursuer *p);
extern f32 Debilitas2_LookFrames(Pursuer *p);
extern f32 Debilitas2_TurnRateFast(Pursuer *p);
extern f32 Debilitas2_TurnRate(Pursuer *p);
extern void Debilitas2_SetTimer(Pursuer *p, s32 t);
extern void Debilitas2_Timer10s(Pursuer *p);
extern u8 *Debilitas2_ModelFileTable(Pursuer *p);
extern u8 *Debilitas2_ModelFiles(Pursuer *p);
extern void Debilitas2_Setup(Pursuer *p);

#endif /* DEBILITAS2_H */
