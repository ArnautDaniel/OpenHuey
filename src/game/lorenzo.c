/* Lorenzo (kind 11, vtable 0x470720; code 0x2F8820..0x2F9210). He runs the Pursuer's behaviour
 * with his own tables, a slow turn (2 degrees a frame), and footsteps only in his move groups.
 * See pursuer.h. */
#include "common.h"
#include "pursuer.h"
#include "progress.h"
#include "sce/libvu0.h"

extern void *D_00470720[];

/* vtable +0x8: destructor */
Pursuer *func_002F8820(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = D_00470720;
        if (p != NULL) {
            Pursuer_DestroyBase(p);
        }
        if ((s16)flags > 0) {
            func_00124E40(&p->c.a);
        }
    }
    return p;
}

/* vtable +0xA0: his turn rate, 2 degrees */
f32 func_002F8940(Pursuer *p) {
    return 0x1.1df46ap-5f;   /* 0.0349066 */
}

/* vtable +0x2D8: the offsets of his actions 10..15 (local) */
void func_002F8960(Pursuer *p, s32 kind, f32 *out) {
    f32 x, z;

    switch (kind) {
    case 10:
    case 11:
        x = -0x1.38ce70p+0f;   /* -1.2219 */
        z = -0x1.fe12d8p+3f;   /* -15.9398 */
        break;
    case 12:
    case 13:
        x = 0x1.32ca58p-2f;    /* 0.2996 */
        z = 0x1.cbd2f2p+3f;    /* 14.3695 */
        break;
    case 14:
        x = -0x1.958106p-2f;   /* -0.3960 */
        z = -0x1.4d14e4p+0f;   /* -1.3011 */
        break;
    case 15:
        x = -0x1.5a0276p-2f;   /* -0.3379 */
        z = -0x1.6978d4p-1f;   /* -0.7060 */
        break;
    default:
        return;
    }
    out[0] = x;
    AT(out, 0x4, s32) = 0;
    out[2] = z;
}

/* vtable +0x200: done at the door at once */
void func_002F8A10(Pursuer *p) {
    PURSUER_STEP_DONE(p) = 1;
}

extern u8 D_0041A870[], D_0041A8D0[], D_0041A930[], D_0041A980[], D_0041A9A0[], D_0041A9F0[],
    D_0041AA20[], D_0041AA70[], D_0041AAC0[], D_0041AAF0[], D_0041AB08[], D_0041AB20[],
    D_0041AB40[], D_0041AB60[], D_0041AB80[], D_0041ABA8[], D_0041ABB8[];
extern u8 D_0041AC50[], D_0041ACB0[], D_0041AD10[], D_0041AD60[], D_0041AD80[], D_0041ADD0[],
    D_0041AE00[], D_0041AE50[], D_0041AEA0[], D_0041AED0[], D_0041AEF0[], D_0041AF10[],
    D_0041AF30[], D_0041AF50[], D_0041AF70[], D_0041AF98[], D_0041AFA8[];

/* his attack tables for situations 0..16; the second set when gProgress+0x30 bit 0x8000 */
static u8 *const sAttackTables[2][17] = {
    { D_0041A870, D_0041A930, D_0041A8D0, D_0041A980, D_0041A9A0, D_0041A9F0, D_0041AA20,
      D_0041AA70, D_0041AAC0, D_0041AAF0, D_0041AB08, D_0041AB20, D_0041AB40, D_0041AB60,
      D_0041ABA8, D_0041ABB8, D_0041AB80 },
    { D_0041AC50, D_0041AD10, D_0041ACB0, D_0041AD60, D_0041AD80, D_0041ADD0, D_0041AE00,
      D_0041AE50, D_0041AEA0, D_0041AED0, D_0041AEF0, D_0041AF10, D_0041AF30, D_0041AF50,
      D_0041AF98, D_0041AFA8, D_0041AF70 },
};

/* vtable +0x130: the attack table for a situation */
void func_002F8A20(Pursuer *p, s8 situation) {
    s32 alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;

    PU(p, 0x1718, u8 *) = sAttackTables[alt][(u32)situation < 17 ? situation : 0];
}

/* vtable +0x2F4 / +0x2F0 */
f32 func_002F8CE0(Pursuer *p) {
    return 18.0f;
}

f32 func_002F8CF0(Pursuer *p) {
    return 12.0f;
}

/* vtable +0x100: the Pursuer's footsteps (func_0029DA80), only in his move groups 0x200 / 0x400 */
void func_002F8D00(Pursuer *p) {
    switch (MOTION_ANIM(p) & 0xFF00) {
    case 0x400:
    case 0x200:
        func_0029DA80(p);
        break;
    }
}

/* vtable +0x30: his frame update: the stalkers' (see Stalker_ThinkStart), on screen with the hits,
   the cries heard, the behaviour step, stance and voice; off screen the behaviour step and the
   off-screen move */
void func_002F8D50(Pursuer *p) {
    PTMF *st = (PTMF *)((u8 *)p + 0x174C);

    Stalker_ThinkStart(p);
    if (func_00217510(p) != 0) {
        func_00296FC0(p);
        func_0029B4B0(p);
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        VCALL(p, 0x110, void (*)(Pursuer *))(p);
        if (p->c.unk14D0 <= 0 || p->c.unk14D0 == 5) {
            func_0029D4C0(p, -1);
        }
        func_00213E30(p);
        func_0029E210(p);
    } else {
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        func_0029D7F0(p);
    }
    Stalker_ThinkEnd(p);
}

extern u8 D_0041A570[], D_0041A590[], D_0041A5B0[], D_0041A5D0[];

/* his model files (func_0029F8C0 for kind 11) */
u8 *func_002F9000(Pursuer *p) {
    return (AT(gProgress, 0x30, u32) & 0x8000) ? D_0041A5D0 : D_0041A590;
}

/* vtable +0xF8: his model files in slot 2 */
u8 *func_002F9040(Pursuer *p) {
    return (AT(gProgress, 0x30, u32) & 0x8000) ? D_0041A5B0 : D_0041A570;
}

extern u8 D_0041A630[], D_0041A760[], D_0041A770[], D_0041A830[], D_0041A850[], D_0041ABD0[],
    D_0041AC20[], D_0041AC40[], D_0041AFC0[], D_0041B008[], D_01990F90[], D_0047AC98[];

/* vtable +0xF4: his setup over the Pursuer's (func_0029FB20): his tables and stats (different
   when gProgress+0x30 bit 0x8000 is set) */
void func_002F9080(Pursuer *p) {
    func_0029FB20(p);
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        p->c.hpMax = 75;
        PU(p, 0x171C, u8 *) = D_0041A770;
        PU(p, 0x1730, u8 *) = D_0041AFC0;
        PU(p, 0x1740, u8 *) = D_0041AC20;
        PU(p, 0x173C, u8 *) = D_01990F90;
        PU(p, 0x1748, u8 *) = D_0041B008;
        PU(p, 0x16DC, s32) = 100;
        PU(p, 0x16E8, s32) = 0;
        PU(p, 0x16D4, s32) = 450;
        PU(p, 0x16D8, s32) = 0;
        PU(p, 0x16D0, s32) = 360;
        PU(p, 0x16E0, s32) = 3600;
        PU(p, 0x16E4, s32) = 100;
    } else {
        p->c.hpMax = 50;
        PU(p, 0x171C, u8 *) = D_0041A770;
        PU(p, 0x1730, u8 *) = D_0041ABD0;
        PU(p, 0x1740, u8 *) = D_0041AC20;
        PU(p, 0x173C, u8 *) = D_01990F90;
        PU(p, 0x1748, u8 *) = D_0041AC40;
        PU(p, 0x16DC, s32) = 100;
        PU(p, 0x16E8, s32) = 0;
        PU(p, 0x16D4, s32) = 600;
        PU(p, 0x16D8, s32) = 0;
        PU(p, 0x16D0, s32) = 510;
        PU(p, 0x16E0, s32) = 3600;
        PU(p, 0x16E4, s32) = 100;
    }
    p->c.a.radius = 5.0f;
    p->c.a.height = 20.0f;
    p->c.hp = p->c.hpMax;
    p->c.hearThreshold = 12;
    PU(p, 0x1720, u8 *) = D_0041A830;
    PU(p, 0x1724, u8 *) = D_0041A850;
    PU(p, 0x16AC, u8 *) = D_0041A630;
    PU(p, 0x16B0, u8 *) = D_0041A760;
    PU(p, 0x1734, u8 *) = D_0047AC98;
    PU(p, 0x1694, f32) = 8.0f;
    PU(p, 0x169C, f32) = -8.0f;
    PU(p, 0x1698, f32) = 8.0f;
    PU(p, 0x16A0, f32) = -8.0f;
}

/* ---- the second Lorenzo (kind 10, vtable 0x4715C0; code 0x309490..0x30C3E0): a strong one
   (250 hp, 400 when gProgress+0x30 bit 0x8000) with a mode 2 (+0x16B8) from threat level 1 ---- */

extern void *D_004715C0[];

/* vtable +0x8: destructor */
Pursuer *func_00309490(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = D_004715C0;
        if (p != NULL) {
            Pursuer_DestroyBase(p);
        }
        if ((s16)flags > 0) {
            func_00124E40(&p->c.a);
        }
    }
    return p;
}

/* vtable +0x10C: never (0) */
s32 func_00309660(Pursuer *p) {
    return 0;
}

/* vtable +0x138: the hit points of an attack entry; his grab 0xE01 is at Fiona herself */
void func_00309BA0(Pursuer *p, s32 *e, f32 *a, f32 *b) {
    if (e[0] == 0xE01) {
        sceVu0CopyVector(a, gCharPlayer->a.pos);
        sceVu0CopyVector(b, gCharPlayer->a.pos);
        return;
    }
    sceVu0CopyVector(a, func_0017CE80(MOTION_AT(p, 0x810, u8 *), e[1]) + 0xC);
    if (e[2] >= 0) {
        sceVu0CopyVector(b, func_0017CE80(MOTION_AT(p, 0x810, u8 *), e[2]) + 0xC);
    } else {
        sceVu0CopyVector(b, func_0017CE80(MOTION_AT(p, 0x810, u8 *), e[1]) + 0xC);
    }
}

extern u8 D_00422A70[], D_00422AC0[], D_00422B10[], D_00422B50[], D_00422B80[], D_00422BC0[],
    D_00422BE0[], D_00422C30[], D_00422C70[], D_00422CA0[], D_00422CD0[], D_00422D00[],
    D_00422D10[], D_00422D20[], D_00422D40[], D_00422D58[], D_00422D68[];
extern u8 D_00422D80[], D_00422DE0[], D_00422E40[], D_00422EA0[], D_00422ED0[], D_00422F30[],
    D_00422F60[], D_00422F90[], D_00422FC0[], D_00422FD0[], D_00422FE0[], D_00423000[];
extern u8 D_00423140[], D_004231C0[], D_00423230[], D_00423290[], D_004232C0[], D_00423320[],
    D_00423350[], D_004233C0[], D_00423420[], D_00423450[], D_00423480[], D_004234D0[],
    D_004234F0[], D_00423510[], D_00423540[], D_00423558[], D_00423568[];
extern u8 D_00423580[], D_00423600[], D_00423670[], D_004236D0[], D_00423700[], D_00423770[],
    D_004237A0[], D_004237D0[], D_00423820[], D_00423840[], D_00423860[], D_00423890[];

/* his attack tables for situations 0..16: [gProgress+0x30 bit 0x8000][mode 2 (+0x16B8)] */
static u8 *const sAttackTables2[2][2][17] = {
    {
        { D_00422A70, D_00422B10, D_00422AC0, D_00422B50, D_00422B80, D_00422BC0, D_00422BE0,
          D_00422C30, D_00422C70, D_00422CA0, D_00422CD0, D_00422D00, D_00422D10, D_00422D20,
          D_00422D58, D_00422D68, D_00422D40 },
        { D_00422D80, D_00422E40, D_00422DE0, D_00422EA0, D_00422B80, D_00422BC0, D_00422ED0,
          D_00422C30, D_00422F30, D_00422F60, D_00422F90, D_00422FC0, D_00422FD0, D_00422FE0,
          D_00422D58, D_00422D68, D_00423000 },
    },
    {
        { D_00423140, D_00423230, D_004231C0, D_00423290, D_004232C0, D_00423320, D_00423350,
          D_004233C0, D_00423420, D_00423450, D_00423480, D_004234D0, D_004234F0, D_00423510,
          D_00423558, D_00423568, D_00423540 },
        { D_00423580, D_00423670, D_00423600, D_004236D0, D_004232C0, D_00423320, D_00423700,
          D_004233C0, D_00423770, D_004237A0, D_004237D0, D_00423820, D_00423840, D_00423860,
          D_00423558, D_00423568, D_00423890 },
    },
};

/* vtable +0x130: the attack table for a situation */
void func_00309C90(Pursuer *p, s8 situation) {
    s32 alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;
    s32 mode2 = PU(p, 0x16B8, s32) == 2;

    /* out of range: the first table without mode 2 */
    PU(p, 0x1718, u8 *) = (u32)situation < 17 ? sAttackTables2[alt][mode2][situation] : sAttackTables2[alt][0][0];
}

/* vtable +0x328: the Pursuer's walk (func_00297A70); with gProgress+0x30 bit 0x8000 always the
   fast one 0x205 unless it's 0x203 */
s32 func_0030BBF0(Pursuer *p) {
    s32 r = func_00297A70(p);

    if ((AT(gProgress, 0x30, u32) & 0x8000) && r != 0x203) {
        r = 0x205;
    }
    return r;
}

/* vtable +0x310 / +0x30C */
s32 func_0030BC40(Pursuer *p) {
    return 10;
}

s32 func_0030BC50(Pursuer *p) {
    return 9;
}

void func_00309890(Pursuer *p);
void func_00309680(Pursuer *p);

/* vtable +0x30: his frame update. On screen: back in contact (+0x29 / +0x2D cleared) once in
   action 0x10 or 0x21; mode 2 on from threat level 1 while chasing and idle, off at level 0; the
   cries heard, the behaviour step, func_00309890 and func_00309680, stance and voice. Off
   screen the behaviour step and the off-screen move */
void func_0030BE20(Pursuer *p) {
    PTMF *st = (PTMF *)((u8 *)p + 0x174C);

    Stalker_ThinkStart(p);
    if (func_00217510(p) != 0) {
        func_00296FC0(p);
        if (p->c.a.disabled != 0 && (PU(p, 0x175C, s32) == 0x10 || PU(p, 0x175C, s32) == 0x21)) {
            p->c.a.disabled = 0;
            p->c.a.unk2D = 0;
        }
        if (PU(p, 0x16B8, s32) == 2) {
            if (AT(gProgress, 0x7B8, u8) == 0) {
                VCALL(p, 0x31C, void (*)(Pursuer *, s32))(p, 0);
            }
        } else if (AT(gProgress, 0x7B8, u8) != 0 && PU(p, 0x16C8, u8) == 0 && p->c.moveMode == 0) {
            VCALL(p, 0x31C, void (*)(Pursuer *, s32))(p, 1);
        }
        func_0029B4B0(p);
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        func_00309890(p);
        func_00309680(p);
        VCALL(p, 0x110, void (*)(Pursuer *))(p);
        if (p->c.unk14D0 <= 0 || p->c.unk14D0 == 5) {
            func_0029D4C0(p, -1);
        }
        func_00213E30(p);
        func_0029E210(p);
    } else {
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        func_0029D7F0(p);
    }
    Stalker_ThinkEnd(p);
}

extern u8 D_00422500[], D_00422650[], D_00422690[], D_004226F0[], D_00422880[], D_00422A10[],
    D_00422A40[], D_00423020[], D_004230C0[], D_004230E0[], D_00423128[], D_004238B0[],
    D_00423950[], D_00423990[], D_019910C8[], D_0047AD10[];

/* vtable +0xF4: his setup over the Pursuer's (func_0029FB20): his tables and stats (different
   when gProgress+0x30 bit 0x8000 is set) */
void func_0030C230(Pursuer *p) {
    func_0029FB20(p);
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        p->c.hpMax = 400;
        PU(p, 0x171C, u8 *) = D_00422880;
        PU(p, 0x1730, u8 *) = D_004238B0;
        PU(p, 0x1740, u8 *) = D_00423950;
        PU(p, 0x173C, u8 *) = D_00423990;
        PU(p, 0x1748, u8 *) = D_019910C8;
        PU(p, 0x16DC, s32) = 100;
        PU(p, 0x16E8, f32) = 25.0f;
        PU(p, 0x16D4, s32) = 450;
    } else {
        p->c.hpMax = 250;
        PU(p, 0x171C, u8 *) = D_004226F0;
        PU(p, 0x1730, u8 *) = D_00423020;
        PU(p, 0x1740, u8 *) = D_004230C0;
        PU(p, 0x173C, u8 *) = D_004230E0;
        PU(p, 0x1748, u8 *) = D_00423128;
        PU(p, 0x16DC, s32) = 100;
        PU(p, 0x16E8, f32) = 50.0f;
        PU(p, 0x16D4, s32) = 900;
    }
    PU(p, 0x16D8, s32) = 600;
    PU(p, 0x16D0, s32) = 0;
    PU(p, 0x16E0, s32) = 9000;
    PU(p, 0x16E4, s32) = 150;
    p->c.a.radius = 4.0f;
    p->c.a.height = 18.0f;
    p->c.hp = p->c.hpMax;
    p->c.hearThreshold = 12;
    PU(p, 0x1714, u8 *) = D_00422690;
    PU(p, 0x1720, u8 *) = D_00422A10;
    PU(p, 0x1724, u8 *) = D_00422A40;
    PU(p, 0x16AC, u8 *) = D_00422500;
    PU(p, 0x16B0, u8 *) = D_00422650;
    PU(p, 0x1734, u8 *) = D_0047AD10;
    PU(p, 0x1694, f32) = 8.0f;
    PU(p, 0x169C, f32) = -8.0f;
    PU(p, 0x1698, f32) = 8.0f;
    PU(p, 0x16A0, f32) = -8.0f;
}
