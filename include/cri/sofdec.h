#ifndef CRI_SOFDEC_H
#define CRI_SOFDEC_H

/* CRI Sofdec (the mwPly movie player): native/platform/sofdec.c on PC. */
#include "common.h"

extern void *mwPly_CreateSofdec(void *cprm);   /* mwPlyCreateSofdec */
extern void mwPlyGetCurFrm(VObject *ply, void *frame);   /* mwPly: get the current frame */
extern void mwPlyRelCurFrm(VObject *ply);   /* mwPly: release the current frame */
extern void mwPlyFinishSfdFx(void);   /* mwPlyFinishSfdFx */
extern s32 func_0023B4B0(VObject *ply);
extern s32 mwPlyGetTime(VObject *ply);   /* mwPly: playing time */
extern void func_0023CA88(VObject *ply, s32 mode);
extern void Sofdec_SetParam(void *sfd, u32 a, u32 b, s32 c);   /* Sofdec */
extern void mwPly_CopyFrame(VObject *ply, u8 **frame, void *dst);   /* the frame copied out */
extern s32 mwPlyCalcWorkCprmSfd(void *cprm);
extern void mwPlyInitSfdFx(void *prm);

extern void *mwPly_CreateSofdec(void *cprm);   /* mwPlyCreateSofdec */
extern void mwPlyGetCurFrm(VObject *ply, void *frame);   /* mwPly: get the current frame */
extern void mwPlyRelCurFrm(VObject *ply);   /* mwPly: release the current frame */
extern void mwPlyFinishSfdFx(void);   /* mwPlyFinishSfdFx */
extern s32 func_0023B4B0(VObject *ply);
extern s32 mwPlyGetTime(VObject *ply);   /* mwPly: playing time */
extern void func_0023CA88(VObject *ply, s32 mode);
extern void Sofdec_SetParam(void *sfd, u32 a, u32 b, s32 c);   /* Sofdec */
extern void mwPly_CopyFrame(VObject *ply, u8 **frame, void *dst);   /* the frame copied out */
extern s32 mwPlyCalcWorkCprmSfd(void *cprm);
extern void mwPlyInitSfdFx(void *prm);

extern void *mwPly_CreateSofdec(void *cprm);   /* mwPlyCreateSofdec */
extern void mwPlyGetCurFrm(VObject *ply, void *frame);   /* mwPly: get the current frame */
extern void mwPlyRelCurFrm(VObject *ply);   /* mwPly: release the current frame */
extern void mwPlyFinishSfdFx(void);   /* mwPlyFinishSfdFx */
extern s32 func_0023B4B0(VObject *ply);
extern s32 mwPlyGetTime(VObject *ply);   /* mwPly: playing time */
extern void func_0023CA88(VObject *ply, s32 mode);
extern void Sofdec_SetParam(void *sfd, u32 a, u32 b, s32 c);   /* Sofdec */
extern void mwPly_CopyFrame(VObject *ply, u8 **frame, void *dst);   /* the frame copied out */
extern s32 mwPlyCalcWorkCprmSfd(void *cprm);
extern void mwPlyInitSfdFx(void *prm);

#endif /* CRI_SOFDEC_H */
