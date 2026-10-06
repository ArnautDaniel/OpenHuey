/* The second Debilitas class (kind 6, vtable 0x470A90; code 0x2FA0B0..0x2FCB28): the same
 * code as Debilitas (debilitas_body.inc) with its own tables, built here under its own names. It
 * leaves out his own behaviour picker and chase (the Pursuer's run instead, +0x16B4 set), gives
 * up only after 80 hits, and has 90 hp (135 when gProgress+0x30 bit 0x8000). See debilitas.c. */
#include "common.h"
#include "pursuer.h"
#include "progress.h"
#include "sce/libvu0.h"

/* Debilitas's names for the shared code -> this class's */
#define D_003AF780 D_0041BB10
#define D_003AF7C0 D_0041BB70
#define D_003AF800 D_0041BBC0
#define D_003AF840 D_0041BC00
#define D_003AF880 D_0041BC30
#define D_003AF8A0 D_0041BC50
#define D_003AF8C0 D_0041BC68
#define D_003AF900 D_0041BC80
#define D_003AF930 D_0041BCA0
#define D_003AF978 D_0041BCE0
#define D_003AF990 D_0041BCF0
#define D_003AF9D0 D_0041BD30
#define D_003AF9E0 D_0041BD40
#define D_003AFA10 D_0041BD70
#define D_003AFA40 D_0041BDA0
#define D_003AFA70 D_0041BDD0
#define D_003AFA80 D_0041BDE0
#define D_003AFAE0 D_0041BE40
#define D_003AFBA0 D_0041BEE0
#define D_003AFBF0 D_0041BF40
#define D_003AFC40 D_0041BF90
#define D_003AFC80 D_0041BFF0
#define D_003AFCB0 D_0041C020
#define D_003AFCF0 D_0041C050
#define D_003AFD10 D_0041C070
#define D_003AFD50 D_0041C0B0
#define D_003AFD80 D_0041C0E0
#define D_003AFDC0 D_0041C120
#define D_003AFDD0 D_0041C130
#define D_003AFE00 D_0041C170
#define D_003AFE10 D_0041C180
#define D_003AFE50 D_0041C1C0
#define D_003AFEA0 D_0041C210
#define D_003AFED0 D_0041C238
#define D_003AFEE0 D_0041C248
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
#define Debilitas_dtor func_002FA0B0
#define Debilitas_SlowWalkAnim func_002FA340
#define Debilitas_AttackTable func_002FA380
#define Debilitas_GivesUp func_002FA640
#define Debilitas_PickDestination func_002FA700
#define Debilitas_StateGrab func_002FA880
#define Debilitas_StartGrab func_002FA9B0
#define Debilitas_StateLunge func_002FABE0
#define Debilitas_StartLunge func_002FACA0
#define Debilitas_StateStun func_002FAE60
#define Debilitas_StateBlow func_002FAF10
#define Debilitas_StateTurnToFiona func_002FB190
#define Debilitas_StartTurnToFiona func_002FB2A0
#define Debilitas_DoorAnim func_002FB4B0
#define Debilitas_Stairs func_002FB4C0
#define Debilitas_ChaseTarget func_002FB4D0
#define Debilitas_Update func_002FB720
#define Debilitas_CarryOn func_002FBA50
#define Debilitas_FreshStart func_002FBC30
#define Debilitas_HeadingStep func_002FBCD0
#define Debilitas_GoForFiona func_002FC2B0
#define Debilitas_HeadFor func_002FC3E0
#define Debilitas_HeadForFiona func_002FC530
#define Debilitas_GoTo func_002FC650
#define Debilitas_AttackAnimB func_002FC7A0
#define Debilitas_AttackAnimA func_002FC7B0

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

extern u8 D_0041B570[], D_0041B590[], D_0041B5B0[], D_0041B5D0[];

/* his model files (Pursuer_ModelFiles for kind 6) */
/* 0x002FC8F0 */
u8 *Debilitas2_ModelFileTable(Pursuer *p) {
    return (AT(gProgress, 0x30, u32) & 0x8000) ? D_0041B5D0 : D_0041B590;
}

/* vtable +0xF8: his model files in slot 2 */
/* 0x002FC930 */
u8 *Debilitas2_ModelFiles(Pursuer *p) {
    return (AT(gProgress, 0x30, u32) & 0x8000) ? D_0041B5B0 : D_0041B570;
}

extern u8 D_0041B670[], D_0041B6E0[], D_0041B840[], D_0041B860[], D_0041BA80[], D_0041BAC0[],
    D_0041BDF0[], D_0041BE60[], D_0041BE80[], D_0041BEC8[], D_0041C260[], D_0041C2B0[],
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
    PU(p, 0x1748, u8 *) = D_0041BEC8;
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
    PU(p, 0x1714, u8 *) = D_0041B670;
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
