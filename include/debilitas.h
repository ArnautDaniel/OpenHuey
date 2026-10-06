#ifndef DEBILITAS_H
#define DEBILITAS_H

/* debilitas.c, debilitas_body.inc: what other files call. */
#include "common.h"

typedef struct Character Character;
typedef struct Pursuer Pursuer;

/* debilitas.c */
extern void *Model_dtor(void **m, s32 flags);
extern s32 RoomBase_Table3C(Pursuer *p);
extern void Debilitas_StunDown(Pursuer *p);
extern void Debilitas_StateLookWalk(Pursuer *p);
extern void Debilitas_ChaseDecision(Pursuer *p);
extern void Debilitas_StateStartWalk(Pursuer *p);
extern void Debilitas_BehaviourRun(Pursuer *p);
extern void Debilitas_StandAnim(Pursuer *p);
extern void Debilitas_Setup(Pursuer *p);
extern void Debilitas_StartWander(Pursuer *p);
extern void Debilitas_Behaviour(Pursuer *p);
extern void Debilitas_Chase(Pursuer *p);

/* debilitas_body.inc */
extern Pursuer *Debilitas_dtor(Pursuer *p, s32 flags);
extern s32 Debilitas_SlowWalkAnim(Pursuer *p);
extern s32 Debilitas_GivesUp(Pursuer *p);
extern void Debilitas_DoorAnim(Pursuer *p);
extern void Debilitas_Stairs(Pursuer *p);
extern s32 Debilitas_AttackAnimB(Pursuer *p);
extern s32 Debilitas_AttackAnimA(Pursuer *p);
extern void Debilitas_StateStun(Pursuer *p);
extern void Debilitas_FreshStart(Pursuer *p);
extern void Debilitas_StateLunge(Pursuer *p);
extern void Debilitas_StartLunge(Pursuer *p);
extern void Debilitas_StateTurnToFiona(Pursuer *p);
extern void Debilitas_StartTurnToFiona(Pursuer *p);
extern void Debilitas_HeadForFiona(Pursuer *p);
extern void Debilitas_GoTo(Pursuer *p, u32 tri, const f32 *pos, s32 room);
extern void Debilitas_HeadFor(Pursuer *p, Character *c);
extern void Debilitas_StateGrab(Pursuer *p);
extern void Debilitas_StartGrab(Pursuer *p);
extern void Debilitas_GoForFiona(Pursuer *p);
extern s32 Debilitas_PickDestination(Pursuer *p);
extern void Debilitas_CarryOn(Pursuer *p);
extern void Debilitas_AttackTable(Pursuer *p, s8 situation);
extern void Debilitas_ChaseTarget(Pursuer *p);
extern void Debilitas_StateBlow(Pursuer *p);
extern void Debilitas_Update(Pursuer *p);
extern void Debilitas_HeadingStep(Pursuer *p);

#endif /* DEBILITAS_H */
