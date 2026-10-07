/* The second Debilitas class (kind 6, vtable 0x470A90; code 0x2FA0B0..0x2FCB28): the same
 * code as Debilitas (debilitas_body.inc) with its own tables, built here under its own names. It
 * leaves out his own behaviour picker and chase (the Pursuer's run instead, +0x16B4 set), gives
 * up only after 80 hits, and has 90 hp (135 when gProgress+0x30 bit 0x8000). See debilitas.c. */
#include "common.h"
#include "pursuer.h"
#include "progress.h"
#include "sce/libvu0.h"

/* Debilitas's names for the shared code -> this class's */
#define Debilitas_Attack0_00 Debilitas2_Attack0_00
#define Debilitas_Attack0_02 Debilitas2_Attack0_02
#define Debilitas_Attack0_01 Debilitas2_Attack0_01
#define Debilitas_Attack0_03 Debilitas2_Attack0_03
#define Debilitas_Attack0_04 Debilitas2_Attack0_04
#define Debilitas_Attack0_05 Debilitas2_Attack0_05
#define Debilitas_Attack0_06 Debilitas2_Attack0_06
#define Debilitas_Attack0_07 Debilitas2_Attack0_07
#define Debilitas_Attack0_08 Debilitas2_Attack0_08
#define Debilitas_Attack0_09 Debilitas2_Attack0_09
#define Debilitas_Attack0_10 Debilitas2_Attack0_10
#define Debilitas_Attack0_11 Debilitas2_Attack0_11
#define Debilitas_Attack0_12 Debilitas2_Attack0_12
#define Debilitas_Attack0_13 Debilitas2_Attack0_13
#define Debilitas_Attack0_16 str_t_11
#define Debilitas_Attack0_14 Debilitas2_Attack0_14
#define Debilitas_Attack0_15 Debilitas2_Attack0_15
#define kDebilitasRoomSpots D_0041BE40
#define Debilitas_Attack1_00 Debilitas2_Attack1_00
#define Debilitas_Attack1_02 Debilitas2_Attack1_02
#define Debilitas_Attack1_01 Debilitas2_Attack1_01
#define Debilitas_Attack1_03 Debilitas2_Attack1_03
#define Debilitas_Attack1_04 Debilitas2_Attack1_04
#define Debilitas_Attack1_05 Debilitas2_Attack1_05
#define Debilitas_Attack1_06 Debilitas2_Attack1_06
#define Debilitas_Attack1_07 Debilitas2_Attack1_07
#define Debilitas_Attack1_08 Debilitas2_Attack1_08
#define Debilitas_Attack1_09 Debilitas2_Attack1_09
#define Debilitas_Attack1_10 Debilitas2_Attack1_10
#define Debilitas_Attack1_11 Debilitas2_Attack1_11
#define Debilitas_Attack1_12 Debilitas2_Attack1_12
#define Debilitas_Attack1_13 Debilitas2_Attack1_13
#define str_t_2 str_t_12
#define Debilitas_Attack1_14 Debilitas2_Attack1_14
#define Debilitas_Attack1_15 Debilitas2_Attack1_15
#define D_003AFF60 D_0041C2D0
#define D_003AFF70 D_0041C2E0
#define D_003AFF80 D_0041C2F0
#define D_003AFF90 D_0041C300
#define Debilitas_StateTurnToFiona_ptmf D_0041C310
#define Debilitas_StateBlow_ptmf D_0041C320
#define Debilitas_StateStun_ptmf D_0041C330
#define Debilitas_StateStun_ptmf2 D_0041C340
#define Debilitas_StateLunge_ptmf D_0041C350
#define Pursuer_AttackNextStep_ptmf Pursuer_AttackNextStep_ptmf7
#define Debilitas_StateGrab_ptmf D_0041C370
#define D_003B0030 D_0041C380
#define Debilitas_vtable Debilitas2_vtable
#define Debilitas_dtor Debilitas2_dtor
#define Debilitas_SlowWalkAnim Debilitas2_SlowWalkAnim
#define Debilitas_AttackTable Debilitas2_AttackTable
#define Debilitas_GivesUp Debilitas2_GivesUp
#define Debilitas_PickDestination Debilitas2_PickDestination
#define Debilitas_StateGrab Debilitas2_StateGrab
#define Debilitas_StartGrab Debilitas2_StartGrab
#define Debilitas_StateLunge Debilitas2_StateLunge
#define Debilitas_StartLunge Debilitas2_StartLunge
#define Debilitas_StateStun Debilitas2_StateStun
#define Debilitas_StateBlow Debilitas2_StateBlow
#define Debilitas_StateTurnToFiona Debilitas2_StateTurnToFiona
#define Debilitas_StartTurnToFiona Debilitas2_StartTurnToFiona
#define Debilitas_DoorAnim Debilitas2_DoorAnim
#define Debilitas_Stairs Debilitas2_Stairs
#define Debilitas_ChaseTarget Debilitas2_ChaseTarget
#define Debilitas_Update Debilitas2_Update
#define Debilitas_CarryOn Debilitas2_CarryOn
#define Debilitas_FreshStart Debilitas2_FreshStart
#define Debilitas_HeadingStep Debilitas2_HeadingStep
#define Debilitas_GoForFiona Debilitas2_GoForFiona
#define Debilitas_HeadFor Debilitas2_HeadFor
#define Debilitas_HeadForFiona Debilitas2_HeadForFiona
#define Debilitas_GoTo Debilitas2_GoTo
#define Debilitas_AttackAnimB Debilitas2_AttackAnimB
#define Debilitas_AttackAnimA Debilitas2_AttackAnimA

#define DEBILITAS_GIVE_UP_HITS 80
#include "debilitas_body.inc"
#include "ptmf.h"
#include "debilitas2.h"

void *Debilitas2_RoomSpots(void);

/* vtable +0x9C: where he stands by a door, by side 0..3 (local offsets; as Debilitas) */
/* 0x002FA1D0 */
void Debilitas2_DoorOffset(Pursuer *p, s32 side, f32 *out) {
    f32 z;

    switch (side) {
    case 0:
        z = -0x1.8d8938p+2f;   /* -6.2115 */
        break;
    case 1:
        z = 0x1.4ccccc0p+3f;   /* 10.4 */
        break;
    case 2:
        z = 0x1.9276c8p+2f;    /* 6.2885 */
        break;
    case 3:
        z = -0x1.dc2f84p+2f;   /* -7.4404 */
        break;
    default:
        return;
    }
    out[0] = 0.0f;
    AT(out, 0x4, s32) = 0;
    out[2] = z;
}

/* vtable +0x2D8: the offsets of his actions 10..15 (local; as Debilitas) */
/* 0x002FA270 */
void Debilitas2_ActionOffsets(Pursuer *p, s32 kind, f32 *out) {
    f32 x, z;

    switch (kind) {
    case 10:
    case 11:
        x = 0x1.e2f838p-1f;    /* 0.9433 */
        z = 0x1.680760p+3f;    /* 11.2509 */
        break;
    case 12:
    case 13:
        x = 0x1.165604p+1f;    /* 2.1745 */
        z = 0x1.e1573ep+3f;    /* 15.0419 */
        break;
    case 14:
        x = -0x1.9c9860p+0f;   /* -1.6117 */
        z = -0x1.15e00ep+2f;   /* -4.3418 */
        break;
    case 15:
        x = 0x1.2a3056p-6f;    /* 0.0182 */
        z = -0x1.00346ep+0f;   /* -1.0008 */
        break;
    default:
        return;
    }
    out[0] = x;
    AT(out, 0x4, s32) = 0;
    out[2] = z;
}

/* 0x002FA320 */
void *Debilitas2_RoomSpots(void) {
    return D_0041BE40;
}

/* vtable +0x200: done at the door at once */
/* 0x002FA330 */
void Debilitas2_ExitDone(Pursuer *p) {
    PURSUER_STEP_DONE(p) = 1;
}

/* vtable +0xA8: his blocking triangle flags */
/* 0x002FC790 */
u32 Debilitas2_BlockFlags(Pursuer *p) {
    return 0x2C020068;
}

/* vtable +0x2FC .. +0x2DC, +0xA4, +0xA0: his tuning (as Debilitas) */
/* 0x002FC7C0 */
f32 Debilitas2_SpeedBase(Pursuer *p) {
    return 0x1.333334p-1f;   /* 0.6 */
}

/* 0x002FC7E0 */
f32 Debilitas2_SpeedTop(Pursuer *p) {
    return 0x1.666666p+0f;   /* 1.4 */
}

/* 0x002FC800 */
f32 Debilitas2_AttackRange(Pursuer *p) {
    return 24.0f;
}

/* 0x002FC810 */
f32 Debilitas2_ReachHewie(Pursuer *p) {
    return 16.0f;
}

/* 0x002FC820 */
f32 Debilitas2_AttackAngle(Pursuer *p) {
    return 20.0f;
}

/* 0x002FC830 */
f32 Debilitas2_Dist2E8(Pursuer *p) {
    return 10.0f;
}

/* 0x002FC840 */
f32 Debilitas2_ReachFiona(Pursuer *p) {
    return 20.0f;
}

/* 0x002FC850 */
f32 Debilitas2_LookSwing(Pursuer *p) {
    return 0x1.eb851ep-4f;   /* 0.12 */
}

/* 0x002FC870 */
f32 Debilitas2_LookFrames(Pursuer *p) {
    return 60.0f;
}

/* 0x002FC880 */
f32 Debilitas2_TurnRateFast(Pursuer *p) {
    return 0x1.1df46ap-3f;   /* 8 degrees */
}

/* 0x002FC8A0 */
f32 Debilitas2_TurnRate(Pursuer *p) {
    return 0x1.aceea0p-5f;   /* 3 degrees */
}

/* vtable +0x2C8 / +0x2C0: the room wait +0x1660 (900 frames by default / 600) */
/* 0x002FC8C0 */
void Debilitas2_SetTimer(Pursuer *p, s32 t) {
    PU(p, 0x1660, s32) = t != 0 ? t : 900;
}

/* 0x002FC8E0 */
void Debilitas2_Timer10s(Pursuer *p) {
    PU(p, 0x1660, s32) = 600;
}

extern u8 D_0041B570[], pstr_O_DB0_DB0_200_PCK_3[], D_0041B5B0[], pstr_O_DB0_DB0_200_PCK_4[];

/* his model files (Pursuer_ModelFiles for kind 6) */
/* 0x002FC8F0 */
u8 *Debilitas2_ModelFileTable(Pursuer *p) {
    return (AT(gProgress, 0x30, u32) & 0x8000) ? pstr_O_DB0_DB0_200_PCK_4 : pstr_O_DB0_DB0_200_PCK_3;
}

/* vtable +0xF8: his model files in slot 2 */
/* 0x002FC930 */
u8 *Debilitas2_ModelFiles(Pursuer *p) {
    return (AT(gProgress, 0x30, u32) & 0x8000) ? D_0041B5B0 : D_0041B570;
}

extern u8 Debilitas2_Actions[], D_0041B6E0[], D_0041B840[], D_0041B860[], D_0041BA80[], D_0041BAC0[],
    D_0041BDF0[], D_0041BE60[], D_0041BE80[], str_Z_7[], D_0041C260[], D_0041C2B0[],
    D_0047ACA8[];

/* vtable +0xF4: his setup over the Pursuer's (Pursuer_Setup): his tables and stats (different
   when gProgress+0x30 bit 0x8000 is set), with +0x16B4 set: the Pursuer's behaviour */
/* 0x002FC970 */
void Debilitas2_Setup(Pursuer *p) {
    Pursuer_Setup(p);
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        p->c.hpMax = 135;
        PU(p, 0x1730, u8 *) = D_0041C260;
        PU(p, 0x1740, u8 *) = D_0041C2B0;
        PU(p, 0x16E8, f32) = 15.0f;
    } else {
        p->c.hpMax = 90;
        PU(p, 0x1730, u8 *) = D_0041BDF0;
        PU(p, 0x1740, u8 *) = D_0041BE60;
        PU(p, 0x16E8, f32) = 10.0f;
    }
    PU(p, 0x171C, u8 *) = D_0041B860;
    PU(p, 0x173C, u8 *) = D_0041BE80;
    PU(p, 0x1748, u8 *) = str_Z_7;
    PU(p, 0x16DC, s32) = 30;              /* Hewie bite tolerance */
    PU(p, 0x16D4, s32) = 300;
    PU(p, 0x16D8, s32) = 1800;
    PU(p, 0x16D0, s32) = 1800;
    PU(p, 0x16E0, s32) = 10800;
    PU(p, 0x16E4, s32) = 150;
    p->c.a.radius = 5.0f;
    p->c.a.height = 20.0f;
    p->c.hp = p->c.hpMax;
    p->c.hearThreshold = 0;
    PU(p, 0x16B4, u8) = 1;
    PU(p, 0x1714, u8 *) = Debilitas2_Actions;
    PU(p, 0x1720, u8 *) = D_0041BA80;
    PU(p, 0x1724, u8 *) = D_0041BAC0;
    PU(p, 0x16AC, u8 *) = D_0041B6E0;
    PU(p, 0x16B0, u8 *) = D_0041B840;
    PU(p, 0x1734, u8 *) = D_0047ACA8;
    PU(p, 0x17EC, s32) = 0;
    PU(p, 0x1694, f32) = 8.0f;
    PU(p, 0x169C, f32) = 1.5f;
    PU(p, 0x1698, f32) = 12.0f;
    PU(p, 0x16A0, f32) = 1.5f;
}
