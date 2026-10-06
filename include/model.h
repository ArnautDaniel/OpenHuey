/* Shared bits of the character model classes (model.c, stalker_models.c). */
#ifndef MODEL_H
#define MODEL_H

#include "common.h"
#include "stalker_models.h"

extern void *D_00469D00[], *D_0046ADA0[], *D_0046B210[], *D_0046F9E0[], *D_0046C160[],
    *D_0046B0D0[], *D_0046B1C0[];

typedef struct Progress Progress;

/* model.c */
extern void *func_002DC6E0(u32 size, void *p);   /* placement new */
extern void *func_0016F4B0(u8 *m);
extern void func_003A10B0(Progress *p, u32 slot);
extern void *func_00208210(u8 *m, u8 kind);
extern void **func_00208070(void **o);
extern void **func_00208090(void **o);
extern void **func_002082A0(void **o);
extern void **func_002082C0(void **o);
extern void **func_00208300(void **o);
extern void **func_00208340(void **o);
extern void **func_002083B0(void **o);
extern void *func_002083D0(u8 *m);
extern void **func_00208EB0(void **o);
extern void **func_00208F10(void **o);
extern void **func_00208F30(void **o, u32 n);
extern void *func_002080D0(u8 *m, u8 kind);
extern void *func_00208180(u8 *m, u8 kind);
extern void *func_00208420(u8 *m, s32 kind);
extern void *func_002084D0(u8 *m, s32 kind);
extern void *func_00208650(u8 *m);
extern void *func_002089F0(u8 *m);
extern void *func_00208C90(u8 *m);
extern void func_003A0F90(Progress *p, u32 slot);
extern void *func_00208EF0(void *p);   /* a D_0046FF00 effect */
extern void *func_00208ED0(void *p);   /* a D_0046FF40 effect */
extern void func_003A1860(Progress *p, u32 slot);
extern void func_003A1720(Progress *p, u32 slot);
extern void func_003A1020(Progress *p, u32 slot);
extern void func_003A1190(Progress *p, u32 slot);
extern void func_003A1220(Progress *p, u32 slot);
extern void func_003A1310(Progress *p, u32 slot);
extern void func_003A1420(Progress *p, u32 slot);
extern void func_003A15A0(Progress *p, u32 slot);
extern void HumanModel_Loaded(u8 *m);
extern void Model_Loaded(u8 *m);
extern void func_001F4910(u8 *m);   /* a model's motion reset */
extern void func_002EE960(u8 *set);
extern void func_002EE690(u8 *col, s32 bone, f32 x, f32 y, f32 z, f32 r);
extern void func_002DE030(u8 *m, s32 anim, u32 flags, s32 variant, f32 blend);   /* blended in */
extern void func_002DDE20(u8 *m, s32 anim, s32 variant);
extern void func_002DDC60(u8 *m, s32 anim, s32 blend, s32 variant);
extern void func_002DDBA0(u8 *m, s32 anim, s32 blend);
extern void func_002DDD20(u8 *m, s32 anim, s32 variant);
extern void func_001F6E30(u8 *m);
extern s32 func_001F4710(u8 *m, s32 anim);
extern void func_001F6E10(u8 *m);
extern s32 func_001F1B90(u8 *g, f32 *stick);
extern void func_001F6240(u8 *m, f32 *out, f32 dt);   /* root motion (variant) */
extern void func_001F6370(u8 *m, f32 *out, f32 dt);   /* root translation */
extern void func_001F36B0(void *track, f32 *out, f32 t);   /* sample a track */
extern f32 func_001F6140(u8 *m, f32 dt);   /* root rotation (yaw delta) */
extern s32 func_001F4770(u8 *m, s32 layer, s32 dt, u32 loop);   /* animation state flags (u8) */
extern s32 func_002DD420(u8 *m, f32 *out, s32 left, f32 dt, f32 zscale);   /* foot on the ground (u8) */
extern void func_002DC710(u8 *m, f32 *p, u8 *a);   /* drop a point onto the floor */
extern void func_002DCB40(u8 *m);
extern void func_002DC960(u8 *m);
extern void func_001F6AF0(u8 *m);
extern void func_001F1250(u8 *ik, void *skel, s32 root, s32 mid, s32 end, f32 len1, f32 len2, f32 bend);
extern void func_001F04B0(u8 *ik, void *skel, s32 root, s32 knee, s32 hock, s32 foot, f32 len1, f32 len2, f32 len3, f32 bend1, f32 bend2, f32 reach);
extern void func_002EE8A0(u8 *s);   /* begin a step */
extern void func_002EE900(u8 *s);   /* one step */
extern void func_002EE840(u8 *s);   /* finish */
extern void Model_Draw(u8 *m, s32 layer, s32 a, s32 b);
extern void func_002E3190(f32 (*m)[4], f32 a);   /* turn about y */
extern void func_002DDED0(u8 *m, s32 anim, s32 variant);   /* play anim blended with another (-1 none) */
extern void func_002DD040(u8 *m, s32 anim);
extern void func_002DD090(u8 *m, s32 frame);   /* its motion at the frame */
extern void func_002DD110(u8 *m, const f32 *target, f32 *pitch, f32 *turn);   /* head angles to a point */
extern u32 func_002DD860(void *motion, s32 foot, f32 ofs);   /* foot planted while standing (u8) */
extern void *func_0016FCD0(u8 *m);
extern void *func_0016FC80(void *e, s32 flags);
extern void *func_001F7E40(void *e, s32 flags);
extern void *func_0016FBB0(void *e, s32 flags);
extern void *func_0016FB00(void *e, s32 flags);
extern void *func_001702F0(void *e, s32 flags);
extern void *func_001709D0(void *e, s32 flags);
extern void *func_00170CB0(void *e, s32 flags);
extern void *func_00170EB0(void *e, s32 flags);
extern void *func_00170F30(void *e, s32 flags);
extern void *func_001700E0(u8 *m, s32 kind);
extern void *func_0038C910(u8 *m);
extern void *func_0038C960(u8 *m);
extern void *func_0038CB60(u8 *m);
extern void *func_0038D4D0(u8 *m);
extern void *func_0038C9E0(u8 *m);
extern void *func_0038CC90(u8 *m);
extern void *func_0038CEE0(u8 *m);
extern void *func_0038D160(u8 *m);
extern void CharLoad_Kind33Model(Progress *p, u32 slot);
extern void CharLoad_Kind23Model(Progress *p, u32 slot);
extern void CharLoad_Kind18Model(Progress *p, u32 slot);
extern void CharLoad_Kind14Model(Progress *p, u32 slot);
extern void CharLoad_Kind13Model(Progress *p, u32 slot);
extern void CharLoad_PlainModel(Progress *p, u32 slot);
extern void CharLoad_Kind12Model(Progress *p, u32 slot);
extern void CharLoad_LorenzoModel(Progress *p, u32 slot);
extern void CharLoad_Lorenzo2Model(Progress *p, u32 slot);
extern void CharLoad_Kind09Model(Progress *p, u32 slot);
extern void CharLoad_RiccardoModel(Progress *p, u32 slot);
extern void CharLoad_DaniellaModel(Progress *p, u32 slot);
extern void CharLoad_DebilitasModel(Progress *p, u32 slot);
extern void func_002DDB30(u8 *m, s32 anim);
extern f32 func_00211910(const f32 *a, const f32 *b, const f32 *c);   /* pt's distance from the line */

/* the human model base's destruction: back down its vtables, then (flags > 0) delete */
static inline void *HumanModel_Destroy(u8 *m, s32 flags) {
    if (m != NULL) {
        AT(m, 0x0, void **) = D_0046C160;
        AT(m, 0x988, void **) = D_0046B0D0;
        AT(m, 0x928, void **) = D_0046B0D0;
        AT(m, 0x0, void **) = D_0046F9E0;
        AT(m, 0x0, void **) = D_0046B210;
        AT(m, 0x1D0, void **) = D_0046B1C0;
        AT(m, 0x1D0, void **) = D_00469D00;
        AT(m, 0x10, void **) = D_0046ADA0;
        AT(m, 0x10, void **) = D_00469D00;
        if ((s16)flags > 0) {
            StalkerModel_delete(m);
        }
    }
    return m;
}

#endif
