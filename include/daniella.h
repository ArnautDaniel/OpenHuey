#ifndef DANIELLA_H
#define DANIELLA_H

/* daniella.c: what other files call. */
#include "common.h"

typedef struct Pursuer Pursuer;

/* daniella.c */
extern Pursuer *Daniella_dtor(Pursuer *p, s32 flags);
extern s32 Daniella_Kind(Pursuer *p);
extern void Daniella_FilesLoaded(Pursuer *p);
extern void Daniella_DoorOffset(Pursuer *p, s32 side, f32 *out);
extern void Daniella_ActionOffsets(Pursuer *p, s32 kind, f32 *out);
extern void Daniella_DoorBreak(Pursuer *p, s32 exit);
extern void Daniella_ExitArg(Pursuer *p, s32 exit);
extern void Daniella_BlowEffect(Pursuer *p);
extern void Daniella_BonePositions(Pursuer *p, s32 *e, f32 *a, f32 *b);
extern void Daniella_AttackTable(Pursuer *p, s8 situation);
extern void Daniella_SetRage(Pursuer *p, s32 on);
extern s32 Daniella_AttackAnimB(Pursuer *p);
extern s32 Daniella_AttackAnimA(Pursuer *p);
extern f32 Daniella_ThreatAmount(Pursuer *p);
extern f32 Daniella_FrightSeen(Pursuer *p);
extern f32 Daniella_ReachHewie(Pursuer *p);
extern void Daniella_Update(Pursuer *p);
extern u8 *Daniella_ModelFileTable(Pursuer *p);
extern u8 *Daniella_ModelFiles(Pursuer *p);
extern void Daniella_Setup(Pursuer *p);
extern void *func_0020D8D0(u8 *o, s32 flags);
extern void *func_0020D920(u8 *o, s32 flags);
extern void *func_0020D970(u8 *o, s32 flags);
extern void *func_0020D9C0(u8 *o, s32 flags);

#endif /* DANIELLA_H */
