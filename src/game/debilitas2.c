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
#define D_003AFFC0 D_0041C310
#define D_003AFFD0 D_0041C320
#define D_003AFFE0 D_0041C330
#define D_003AFFF0 D_0041C340
#define D_003B0000 D_0041C350
#define D_003B0010 D_0041C360
#define D_003B0020 D_0041C370
#define D_003B0030 D_0041C380
#define D_00469D10 D_00470A90
#define func_001276F0 func_002FA0B0
#define func_00127CC0 func_002FA340
#define func_00127D00 func_002FA380
#define func_00127FC0 func_002FA640
#define func_00128090 func_002FA700
#define func_00128390 func_002FA880
#define func_001284C0 func_002FA9B0
#define func_001286F0 func_002FABE0
#define func_001287B0 func_002FACA0
#define func_00128970 func_002FAE60
#define func_00128A20 func_002FAF10
#define func_00128CA0 func_002FB190
#define func_00128DB0 func_002FB2A0
#define func_00129550 func_002FB4B0
#define func_00129560 func_002FB4C0
#define func_00129570 func_002FB4D0
#define func_001297C0 func_002FB720
#define func_00129B30 func_002FBA50
#define func_00129D10 func_002FBC30
#define func_00129DB0 func_002FBCD0
#define func_0012A390 func_002FC2B0
#define func_0012BAA0 func_002FC3E0
#define func_0012BBF0 func_002FC530
#define func_0012BD10 func_002FC650
#define func_0012BE60 func_002FC7A0
#define func_0012BE70 func_002FC7B0

#define DEBILITAS_GIVE_UP_HITS 80
#include "debilitas_body.inc"
#include "ptmf.h"

void *func_002FA320(void);

/* vtable +0x9C: where he stands by a door, by side 0..3 (local offsets; as Debilitas) */
void func_002FA1D0(Pursuer *p, s32 side, f32 *out) {
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
void func_002FA270(Pursuer *p, s32 kind, f32 *out) {
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

void *func_002FA320(void) {
    return D_0041BE40;
}

/* vtable +0x200: done at the door at once */
void func_002FA330(Pursuer *p) {
    PURSUER_STEP_DONE(p) = 1;
}

/* vtable +0xA8: his blocking triangle flags */
u32 func_002FC790(Pursuer *p) {
    return 0x2C020068;
}

/* vtable +0x2FC .. +0x2DC, +0xA4, +0xA0: his tuning (as Debilitas) */
f32 func_002FC7C0(Pursuer *p) {
    return 0x1.333334p-1f;   /* 0.6 */
}

f32 func_002FC7E0(Pursuer *p) {
    return 0x1.666666p+0f;   /* 1.4 */
}

f32 func_002FC800(Pursuer *p) {
    return 24.0f;
}

f32 func_002FC810(Pursuer *p) {
    return 16.0f;
}

f32 func_002FC820(Pursuer *p) {
    return 20.0f;
}

f32 func_002FC830(Pursuer *p) {
    return 10.0f;
}

f32 func_002FC840(Pursuer *p) {
    return 20.0f;
}

f32 func_002FC850(Pursuer *p) {
    return 0x1.eb851ep-4f;   /* 0.12 */
}

f32 func_002FC870(Pursuer *p) {
    return 60.0f;
}

f32 func_002FC880(Pursuer *p) {
    return 0x1.1df46ap-3f;   /* 8 degrees */
}

f32 func_002FC8A0(Pursuer *p) {
    return 0x1.aceea0p-5f;   /* 3 degrees */
}

/* vtable +0x2C8 / +0x2C0: the room wait +0x1660 (900 frames by default / 600) */
void func_002FC8C0(Pursuer *p, s32 t) {
    PU(p, 0x1660, s32) = t != 0 ? t : 900;
}

void func_002FC8E0(Pursuer *p) {
    PU(p, 0x1660, s32) = 600;
}

extern u8 D_0041B570[], D_0041B590[], D_0041B5B0[], D_0041B5D0[];

/* his model files (func_0029F8C0 for kind 6) */
u8 *func_002FC8F0(Pursuer *p) {
    return (AT(gProgress, 0x30, u32) & 0x8000) ? D_0041B5D0 : D_0041B590;
}

/* vtable +0xF8: his model files in slot 2 */
u8 *func_002FC930(Pursuer *p) {
    return (AT(gProgress, 0x30, u32) & 0x8000) ? D_0041B5B0 : D_0041B570;
}

extern u8 D_0041B670[], D_0041B6E0[], D_0041B840[], D_0041B860[], D_0041BA80[], D_0041BAC0[],
    D_0041BDF0[], D_0041BE60[], D_0041BE80[], D_0041BEC8[], D_0041C260[], D_0041C2B0[],
    D_0047ACA8[];

/* vtable +0xF4: his setup over the Pursuer's (func_0029FB20): his tables and stats (different
   when gProgress+0x30 bit 0x8000 is set), with +0x16B4 set: the Pursuer's behaviour */
void func_002FC970(Pursuer *p) {
    func_0029FB20(p);
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
