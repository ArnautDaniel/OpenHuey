#ifndef DEBILITAS_H
#define DEBILITAS_H

/* debilitas.c, debilitas_body.inc: what other files call. */
#include "common.h"

typedef struct Character Character;
typedef struct Pursuer Pursuer;

/* debilitas.c */
extern void *Model_dtor(void **m, s32 flags);
extern s32 func_00128080(Pursuer *p);
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
extern Pursuer *func_001276F0(Pursuer *p, s32 flags);
extern s32 func_00127CC0(Pursuer *p);
extern s32 func_00127FC0(Pursuer *p);
extern void func_00129550(Pursuer *p);
extern void func_00129560(Pursuer *p);
extern s32 func_0012BE60(Pursuer *p);
extern s32 func_0012BE70(Pursuer *p);
extern void func_00128970(Pursuer *p);
extern void func_00129D10(Pursuer *p);
extern void func_001286F0(Pursuer *p);
extern void func_001287B0(Pursuer *p);
extern void func_00128CA0(Pursuer *p);
extern void func_00128DB0(Pursuer *p);
extern void func_0012BBF0(Pursuer *p);
extern void func_0012BD10(Pursuer *p, u32 tri, const f32 *pos, s32 room);
extern void func_0012BAA0(Pursuer *p, Character *c);
extern void func_00128390(Pursuer *p);
extern void func_001284C0(Pursuer *p);
extern void func_0012A390(Pursuer *p);
extern s32 func_00128090(Pursuer *p);
extern void func_00129B30(Pursuer *p);
extern void func_00127D00(Pursuer *p, s8 situation);
extern void func_00129570(Pursuer *p);
extern void func_00128A20(Pursuer *p);
extern void func_001297C0(Pursuer *p);
extern void func_00129DB0(Pursuer *p);

#endif /* DEBILITAS_H */
