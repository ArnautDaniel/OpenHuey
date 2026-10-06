#ifndef CRI_SOFDEC_H
#define CRI_SOFDEC_H

/* CRI Sofdec (the mwPly movie player): native/platform/sofdec.c on PC. */
#include "common.h"

extern void *func_00238BF0(void *cprm);   /* mwPlyCreateSofdec */
extern void func_00239828(VObject *ply, void *frame);   /* mwPly: get the current frame */
extern void func_0023A180(VObject *ply);   /* mwPly: release the current frame */
extern void func_0023AE40(void);   /* mwPlyFinishSfdFx */
extern s32 func_0023B4B0(VObject *ply);
extern s32 func_0023C480(VObject *ply);   /* mwPly: playing time */
extern void func_0023CA88(VObject *ply, s32 mode);
extern void func_0023E878(void *sfd, u32 a, u32 b, s32 c);   /* Sofdec */
extern void func_002410B0(VObject *ply, u8 **frame, void *dst);   /* the frame copied out */
extern s32 mwPlyCalcWorkCprmSfd(void *cprm);
extern void mwPlyInitSfdFx(void *prm);

extern void *func_00238BF0(void *cprm);   /* mwPlyCreateSofdec */
extern void func_00239828(VObject *ply, void *frame);   /* mwPly: get the current frame */
extern void func_0023A180(VObject *ply);   /* mwPly: release the current frame */
extern void func_0023AE40(void);   /* mwPlyFinishSfdFx */
extern s32 func_0023B4B0(VObject *ply);
extern s32 func_0023C480(VObject *ply);   /* mwPly: playing time */
extern void func_0023CA88(VObject *ply, s32 mode);
extern void func_0023E878(void *sfd, u32 a, u32 b, s32 c);   /* Sofdec */
extern void func_002410B0(VObject *ply, u8 **frame, void *dst);   /* the frame copied out */
extern s32 mwPlyCalcWorkCprmSfd(void *cprm);
extern void mwPlyInitSfdFx(void *prm);

extern void *func_00238BF0(void *cprm);   /* mwPlyCreateSofdec */
extern void func_00239828(VObject *ply, void *frame);   /* mwPly: get the current frame */
extern void func_0023A180(VObject *ply);   /* mwPly: release the current frame */
extern void func_0023AE40(void);   /* mwPlyFinishSfdFx */
extern s32 func_0023B4B0(VObject *ply);
extern s32 func_0023C480(VObject *ply);   /* mwPly: playing time */
extern void func_0023CA88(VObject *ply, s32 mode);
extern void func_0023E878(void *sfd, u32 a, u32 b, s32 c);   /* Sofdec */
extern void func_002410B0(VObject *ply, u8 **frame, void *dst);   /* the frame copied out */
extern s32 mwPlyCalcWorkCprmSfd(void *cprm);
extern void mwPlyInitSfdFx(void *prm);

#endif /* CRI_SOFDEC_H */
