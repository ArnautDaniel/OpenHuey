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
extern void func_002D85D0(Pursuer *p);
extern void func_002D8690(Pursuer *p);
extern void func_002D8AC0(Pursuer *p);
extern void func_002D8CB0(Pursuer *p);
extern void func_002DA4C0(Pursuer *p);
extern void func_002D7E20(Pursuer *p, Character *who);
extern s32 func_002D8840(Pursuer *p, Character *who);
extern void func_002DA120(Pursuer *p);
extern void func_002DB480(Pursuer *p);
extern s32 func_002DBA90(Pursuer *p, f32 *out);
extern void func_002DBD70(Pursuer *p);
extern void func_002D8DF0(Pursuer *p);
extern void func_002D9500(Pursuer *p);
extern void func_002DA6B0(Pursuer *p);

#endif /* RICCARDO_H */
