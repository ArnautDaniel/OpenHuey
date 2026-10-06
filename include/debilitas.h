#ifndef DEBILITAS_H
#define DEBILITAS_H

/* debilitas.c, debilitas_body.inc: what other files call. */
#include "common.h"

typedef struct Character Character;
typedef struct Pursuer Pursuer;

/* debilitas.c */
extern void *Model_dtor(void **m, s32 flags);
extern s32 RoomBase_Table3C(Pursuer *p);
extern void func_00129AF0(Pursuer *p);
extern void func_00128FC0(Pursuer *p);
extern void Debilitas_ChaseDecision(Pursuer *p);
extern void func_00129090(Pursuer *p);
extern void Debilitas_BehaviourRun(Pursuer *p);
extern void Debilitas_StandAnim(Pursuer *p);
extern void Debilitas_Setup(Pursuer *p);
extern void func_001291C0(Pursuer *p);
extern void func_0012B490(Pursuer *p);
extern void func_0012A4C0(Pursuer *p);

/* debilitas_body.inc */
extern Pursuer *Debilitas_dtor(Pursuer *p, s32 flags);
extern s32 Debilitas_SlowWalkAnim(Pursuer *p);
extern s32 Debilitas_GivesUp(Pursuer *p);
extern void Debilitas_DoorAnim(Pursuer *p);
extern void Debilitas_Stairs(Pursuer *p);
extern s32 Debilitas_AttackAnimB(Pursuer *p);
extern s32 Debilitas_AttackAnimA(Pursuer *p);
extern void func_00128970(Pursuer *p);
extern void Debilitas_FreshStart(Pursuer *p);
extern void func_001286F0(Pursuer *p);
extern void func_001287B0(Pursuer *p);
extern void func_00128CA0(Pursuer *p);
extern void func_00128DB0(Pursuer *p);
extern void Debilitas_HeadForFiona(Pursuer *p);
extern void Debilitas_GoTo(Pursuer *p, u32 tri, const f32 *pos, s32 room);
extern void Debilitas_HeadFor(Pursuer *p, Character *c);
extern void func_00128390(Pursuer *p);
extern void func_001284C0(Pursuer *p);
extern void Debilitas_GoForFiona(Pursuer *p);
extern s32 Debilitas_PickDestination(Pursuer *p);
extern void Debilitas_CarryOn(Pursuer *p);
extern void Debilitas_AttackTable(Pursuer *p, s8 situation);
extern void Debilitas_ChaseTarget(Pursuer *p);
extern void func_00128A20(Pursuer *p);
extern void Debilitas_Update(Pursuer *p);
extern void Debilitas_HeadingStep(Pursuer *p);

#endif /* DEBILITAS_H */
