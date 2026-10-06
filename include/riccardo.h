#ifndef RICCARDO_H
#define RICCARDO_H

/* riccardo.c: what other files call. */
#include "common.h"

typedef struct Character Character;
typedef struct Pursuer Pursuer;

/* riccardo.c */
extern s32 ItemA1_Use(void);
extern Pursuer *Riccardo_dtor(Pursuer *p, s32 flags);
extern void Riccardo_ExitDone(Pursuer *p);
extern void Riccardo_DoorBreak(Pursuer *p, s32 exit);
extern void Riccardo_ExitArg(Pursuer *p, s32 exit);
extern void Riccardo_BonePositions(Pursuer *p, s32 *e, f32 *a, f32 *b);
extern void Riccardo_AttackTable(Pursuer *p, s8 situation);
extern void Riccardo_ChaseDecision(Pursuer *p);
extern void Riccardo_SetRage(Pursuer *p, s32 on);
extern s32 Riccardo_AttackAnimB(Pursuer *p);
extern s32 Riccardo_AttackAnimA(Pursuer *p);
extern s32 Riccardo_WalkAnim(Pursuer *p);
extern s32 Riccardo_StanceAnim(Pursuer *p);
extern void Riccardo_Update(Pursuer *p);
extern void Riccardo_Setup(Pursuer *p);
extern void Riccardo_StateLunge(Pursuer *p);
extern void Riccardo_StartLunge(Pursuer *p);
extern void Riccardo_StateAfterBlow(Pursuer *p);
extern void Riccardo_StateBlow(Pursuer *p);
extern void Riccardo_StartFlurry(Pursuer *p);
extern void Riccardo_CryHit(Pursuer *p, Character *who);
extern s32 Riccardo_AttackForDistance(Pursuer *p, Character *who);
extern void Riccardo_FlurryBlow(Pursuer *p);
extern void Riccardo_StateBehaviour(Pursuer *p);
extern s32 Riccardo_BlowFloorPoint(Pursuer *p, f32 *out);
extern void Riccardo_Impact(Pursuer *p);
extern void Riccardo_StateBlowHewie(Pursuer *p);
extern void Riccardo_StateBlowFiona(Pursuer *p);
extern void Riccardo_StateChase(Pursuer *p);

#endif /* RICCARDO_H */
