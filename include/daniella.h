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
extern void *Pair_dtor(u8 *o, s32 flags);
extern void *Pair44_dtor(u8 *o, s32 flags);
extern void *Triple_dtor(u8 *o, s32 flags);
extern void *Quad4_dtor(u8 *o, s32 flags);

/* daniella.c */
extern void *DaniellaModel_ctor(u8 *m);
extern void DaniellaModel_BackCapsules(u8 *m, s32 pose);
extern u8 *Kind34_ModelFileTable(Pursuer *p);
extern u8 *Kind35_ModelFileTable(Pursuer *p);
extern u8 *Kind36_ModelFileTable(Pursuer *p);
extern void *Kind36_ctor(void *p, s32 arg);
extern void *Kind35_ctor(void *p, s32 arg);
extern void *Kind34_ctor(void *p, s32 arg);
extern void *Daniella_ctor(void *p, s32 arg);

#endif /* DANIELLA_H */
