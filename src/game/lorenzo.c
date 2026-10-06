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

/* a dust cloud (0x720 bytes, vtable 0x46FF20, its quad drawer at +0x610; as room 0x03's prop) */
extern void *D_0046FF20[];

static inline void Dust_Init(void **obj) {
    obj[0] = D_0046FF20;
    obj[0x610 / 4] = D_00469D00;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = D_0046FC30;
}

/* his slam 0x2301: at its key (2), eight grey dust clouds of random size (320..640) at his
   hand (bone 0x32) */
void func_00309680(Pursuer *p) {
    struct {
        f32 pos[4];
        s32 kind, r, g, b, size;
    } dp __attribute__((aligned(16)));
    u8 *mgr;
    VObject *rnd;
    u8 i;

    if (MOTION_ANIM(p) != 0x2301 || !(func_001F4770(p->c.motion, 0, 0, 1) & 0xFF & 2)) {
        return;
    }
    sceVu0CopyVector(dp.pos, func_0017CE80(MOTION_AT(p, 0x810, u8 *), 0x32) + 0xC);
    dp.pos[3] = 1.0f;
    dp.kind = 2;
    dp.b = 0x50;
    dp.g = 0x50;
    dp.r = 0x50;
    mgr = D_0044E578;
    rnd = D_0044E550;
    for (i = 0; i < 8; i++) {
        s32 slot = Effect_New(mgr, 0x720, Dust_Init);

        dp.size = (s32)(320.0f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd)) + 320;
        func_002D6090(mgr, slot, &dp);
    }
}

/* a spark (0x130 bytes, vtable 0x47A350, its part at +0xD0) */
extern void *D_0047A350[];

static inline void Spark_Init(void **obj) {
    obj[0] = D_0047A350;
    obj[0xD0 / 4] = D_00469D00;
    ((s32 *)obj)[0xD4 / 4] = -1;
    obj[0xD0 / 4] = D_0046FC30;
}

extern const f32 D_00423AC0[7][4];

/* his blows 0x1904 / 0xE00 / 0xE04 / 0xE05 (hand, bone 0x32) and 0xE06 (bone 0x28): at the
   key (0x20), seven sparks around the bone (offsets D_00423AC0) of random scale, speed and life */
void func_00309890(Pursuer *p) {
    struct {
        f32 pos[4];
        f32 one, scale, speed;
        s32 life;
    } sp __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    VObject *rnd;
    u8 *mgr;
    s32 bone;
    u8 i;

    switch (MOTION_ANIM(p)) {
    case 0x1904:
    case 0xE05:
    case 0xE04:
    case 0xE00:
        bone = 0x32;
        break;
    case 0xE06:
        bone = 0x28;
        break;
    default:
        return;
    }
    if (!(func_001F4770(p->c.motion, 0, 0, 1) & 0xFF & 0x20)) {
        return;
    }
    sp.one = 1.0f;
    sceVu0CopyVector(at, func_0017CE80(MOTION_AT(p, 0x810, u8 *), bone) + 0xC);
    rnd = D_0044E550;
    mgr = D_0044E578;
    for (i = 0; i < 7; i++) {
        s32 slot;

        sp.scale = 0.0f + 0.5f + 0.5f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);
        sp.speed = 0.0f + 0x1.19999ap+0f + 0x1.99999ap-5f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);   /* 1.1 + 0.05 */
        sp.life = (s32)(6.0f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd)) + 12;
        slot = Effect_New(mgr, 0x130, Spark_Init);
        sceVu0AddVector(sp.pos, at, D_00423AC0[i]);
        func_002D6090(mgr, slot, &sp);
    }
}

/* an animation whose end ends the step */
static inline void Lorenzo2_PlayOut(Pursuer *p) {
    func_00125A10(&p->c);
    if (AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    }
}

/* state: see Lorenzo2_PlayOut */
void func_0030B1E0(Pursuer *p) {
    Lorenzo2_PlayOut(p);
}

extern const PTMF D_00423A38;
void func_0030B540(Pursuer *p);

/* animation 0x1303 in state `st` (D_00423A38: func_0030B540), run at once */
static inline __attribute__((always_inline)) void Lorenzo2_SinkBehindAs(Pursuer *p, const PTMF *st, void (*fn)(Pursuer *)) {
    func_00297B40(p, 0x1303, 0);
    Actor_SetState(&p->c.a, st);
    fn(p);
}
#define Lorenzo2_SinkBehind(p) Lorenzo2_SinkBehindAs(p, &D_00423A38, func_0030B540)

void func_0030B7C0(Pursuer *p) {
    Lorenzo2_SinkBehind(p);
}

/* is his slam 0x2301 at its impact key (0x20) now (active and on screen) */
s32 func_0030BB70(Pursuer *p) {
    if (!p->c.a.active || func_00217510(p) == 0 || MOTION_ANIM(p) != 0x2301) {
        return 0;
    }
    return (func_001F4770(p->c.motion, 0, 0, 1) & 0xFF & 0x20) ? 1 : 0;
}

extern const PTMF D_00423AA8;
void func_0030A210(Pursuer *p);

/* start of his grab: action 0x17 instead when he may not go for his target; finish the walk,
   then animation 0xE01 (with +0x16F7 when gProgress+0x30 bit 0x8000) and func_0030A210 */
void func_0030A4D0(Pursuer *p) {
    if (!(func_00283870(p) & 0xFF)) {
        p->c.unk104[0] = 0;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x17);
        return;
    }
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    func_00297B40(p, 0xE01, 0);
    p->c.unk104[0] = 0;
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        PU(p, 0x16F7, u8) = 1;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_00423AA8);
    func_0030A210(p);
}

/* state: a sweep that hits both: at its key (2) Fiona and Hewie within the reach of attack
   entry 2 (+0x171C +0x48: +0xC reach) and on the mesh are hit (func_00178070 with the entry,
   stunning by its +0x18 chance), once each (+0x1760); its end ends the step */
void func_0030A650(Pursuer *p) {
    func_00125A10(&p->c);
    if (func_001F4770(p->c.motion, 0, 0, 1) & 0xFF & 2) {
        u8 *e = PU(p, 0x171C, u8 *) + 0x48;
        u32 hit = 0;
        f32 d;

        d = func_00124490(&p->c.a, gCharPlayer->a.pos);
        if (d <= AT(e, 0xC, f32) && !(d < 0.0f) &&
            func_00124480(&p->c.a, gCharPlayer->a.pos, p->c.a.navMask) != (u32)-1) {
            hit = (hit | 1) & 0xFF;
        }
        d = func_00124490(&p->c.a, gCharPartner->a.pos);
        if (d <= AT(e, 0xC, f32) && !(d < 0.0f) &&
            func_00124480(&p->c.a, gCharPartner->a.pos, p->c.a.navMask) != (u32)-1) {
            hit = (hit | 2) & 0xFF;
        }
        if (func_00283870(p) != 0 && (hit & ~PU(p, 0x1760, u8))) {
            s16 stun = 100.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550) <= AT(e, 0x18, f32) ? 0x8000 : 0;

            func_00178070(gProgress, *(u8 *)&p->c.a.slot, hit, AT(e, 0x10, u8), AT(e, 0x12, u16), stun, AT(e, 0x14, f32));
            PU(p, 0x1764, s32) = 12;
        }
    }
    if (AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    }
}

/* where he sinks away (0x700 bytes, vtable 0x47A750, its drawer at +0x610) */
extern void *D_0047A750[];

static inline void Sink_Init(void **obj) {
    obj[0] = D_0047A750;
    obj[0x610 / 4] = D_00469D00;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = D_0046FC30;
}

extern const PTMF D_00423A88;
void func_0030A850(Pursuer *p);

/* state: sinking away (animation 0x1304). At its end (Lorenzo2_Sink) he heads under the floor
   toward his target: a point along the path (func_00214890), or where he is when it's out of
   reach; state D_00423A88 (func_0030A850) */
/* the end of a sinking: out of contact (+0x29 / +0x2D), the sink effect where he stood, moved
   to the exit of func_00177AB0 kind 9 if any, 15 frames underground (+0x1624); returns the
   distance (func_00214B90) to his target's point `t` on the mesh */
static inline f32 Lorenzo2_Sink(Pursuer *p, f32 *t) {
    struct {
        f32 pos[4];
        s32 kind;
    } sk __attribute__((aligned(16)));
    u8 *mgr = D_0044E578;
    s32 slot;
    u32 k, tri;

    p->c.a.disabled = 1;
    p->c.a.unk2D = 1;
    slot = Effect_New(mgr, 0x700, Sink_Init);
    sceVu0CopyVector(sk.pos, p->c.a.pos);
    sk.kind = 0;
    func_002D6090(mgr, slot, &sk);
    k = func_00177AB0(gProgress, 9, *(u8 *)&p->c.a.slot) & 0xFF;
    if (k != 0xFF) {
        p->c.a.navTri = VCALL(D_0044E568, 0x34, u32 (*)(VObject *, u32, f32 *))(D_0044E568, k, p->c.a.pos);
    }
    PU(p, 0x1624, s32) = 15;
    sceVu0CopyVector(t, p->target->a.pos);
    tri = func_00216E00(p, p->target->a.navTri, t, t);
    return func_00214B90(p, tri, t);
}

void func_0030AB80(Pursuer *p) {
    f32 t[4] __attribute__((aligned(16)));
    f32 d;

    func_00125A10(&p->c);
    if (!(AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END)) {
        return;
    }
    d = Lorenzo2_Sink(p, t);
    if (d < 0.0f) {
        sceVu0CopyVector(p->c.unk110, p->c.a.pos);
        p->c.unk104[0] = p->c.a.navTri;
    } else {
        u32 out;

        func_00214890(p, &out, p->c.unk110, d);
        p->c.unk104[0] = out;
    }
    Actor_SetState(&p->c.a, &D_00423A88);
    func_0030A850(p);
}

extern const PTMF D_00423A78;

/* closing on his target: when it's out of reach by the mesh (func_001257B0 < 0), back to the
   walk and the step ends; otherwise he sinks away (0x1304, func_0030AB80) */
static inline __attribute__((always_inline)) void Lorenzo2_ApproachAs(Pursuer *p, const PTMF *st, void (*fn)(Pursuer *)) {
    f32 t[4] __attribute__((aligned(16)));
    u32 tri;

    sceVu0CopyVector(t, p->target->a.pos);
    tri = func_00216E00(p, p->target->a.navTri, t, t);
    if (func_001257B0(&p->c, tri, t, -1) < 0.0f) {
        if (PU(p, 0x1788, s32) != 0) {
            func_00297B40(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p), 0);
        }
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
        return;
    }
    func_00297B40(p, 0x1304, 0);
    Actor_SetState(&p->c.a, st);
    fn(p);
}
#define Lorenzo2_Approach(p) Lorenzo2_ApproachAs(p, &D_00423A78, func_0030AB80)

/* state: closing on his target (see Lorenzo2_Approach) */
void func_0030AE00(Pursuer *p) {
    Lorenzo2_Approach(p);
}

/* the burst of his grab (0xFC0 bytes, vtable 0x47A010, four parts at +0xB50 / +0xB88 / +0xBC0 /
   +0xBF8) */
extern void *D_0047A010[];

static inline void Burst_Init(void **obj) {
    s32 k;

    obj[0] = D_0047A010;
    for (k = 0; k < 4; k++) {
        obj[(0xB50 + k * 0x38) / 4] = D_00469D00;
        ((s32 *)obj)[(0xB54 + k * 0x38) / 4] = -1;
        obj[(0xB50 + k * 0x38) / 4] = D_0046FC30;
    }
}

/* state: his grab. Until its aim key (frame 12, key 2) it follows the target (+0x110); at the
   hit key whoever is in reach of attack entry 3 (+0x171C +0x6C; func_002179F0 at the aimed
   point) is hit, stunned by the entry's +0x18 chance, once each (+0x1760); the burst effect at
   the point (also to +0x1770). Its end ends the step */
void func_0030A210(Pursuer *p) {
    func_00125A10(&p->c);
    if (func_001F4770(p->c.motion, 0, 0xC, 1) & 0xFF & 2) {
        sceVu0CopyVector(p->c.unk110, p->target->a.pos);
    } else if (func_001F4770(p->c.motion, 0, 0, 1) & 0xFF & 2) {
        u8 *e = PU(p, 0x171C, u8 *) + 0x6C;
        u32 hit = func_002179F0(p, (s32)(u32)p->c.unk110, AT(e, 0xC, f32)) & 0xFF;
        u8 *mgr;

        if (func_00283870(p) != 0 && (hit & ~PU(p, 0x1760, u8))) {
            s16 stun = 100.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550) <= AT(e, 0x18, f32) ? 0x8000 : 0;

            func_00178070(gProgress, *(u8 *)&p->c.a.slot, hit, AT(e, 0x10, u8), AT(e, 0x12, u16), stun, AT(e, 0x14, f32));
            PU(p, 0x1764, s32) = 12;
        }
        sceVu0CopyVector((f32 *)((u8 *)p + 0x1770), p->c.unk110);
        mgr = D_0044E578;
        func_002D6090(mgr, Effect_New(mgr, 0xFC0, Burst_Init), p->c.unk110);
    }
    if (AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        AT(p->c.unk110, 0x0, s32) = 0;
        AT(p->c.unk110, 0x4, s32) = 0;
        AT(p->c.unk110, 0x8, s32) = 0;
        AT(p->c.unk110, 0xC, s32) = 0;
        if (AT(gProgress, 0x30, u32) & 0x8000) {
            PU(p, 0x16F7, u8) = 0;
        }
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    }
}

extern const PTMF D_00423A98;

/* state: under the floor after sinking (Lorenzo2_Underground). Then he rises at the goal (+0x104 /
   +0x110, 5 short of his target when that close), facing his target, in the sweep 0xE02
   (func_0030A650) with the rising effect */
/* travelling under the floor (+0x1624 frames left) through the mesh toward his goal
   (func_00211B00), out of contact where it fails; 1 while travelling */
static inline s32 Lorenzo2_Underground(Pursuer *p) {
    if (PU(p, 0x1624, s32) <= 0) {
        return 0;
    }
    if (func_00214A90(p, p->c.a.navTri) != 0) {
        p->c.a.navTri = func_00211B00(p, p->c.a.navTri);
        if (p->c.a.navTri != (u32)-1 && !(func_00214A90(p, p->c.a.navTri) & 0xFF)) {
            VCALL(D_0044E570, 0xC, void (*)(void *, u32, f32 *))(D_0044E570, p->c.a.navTri, p->c.a.pos);
        } else {
            func_00124890(&p->c.a, -1);
            p->c.a.disabled = 1;
            p->c.a.unk2D = 1;
        }
    }
    PU(p, 0x1624, s32)--;
    return 1;
}

/* the sweep of the other class (func_003643E0): at its key (2) whoever it touches
   (func_00217920) is hit (func_00178070 with attack entry 2, stunning by its +0x18 chance),
   once each (+0x1760); its end ends the step */
static inline void Lorenzo2b_Sweep(Pursuer *p) {
    func_00125A10(&p->c);
    if (func_001F4770(p->c.motion, 0, 0, 1) & 0xFF & 2) {
        u32 hit = func_00217920(p) & 0xFF;

        if (func_00283870(p) != 0 && (hit & ~PU(p, 0x1760, u8))) {
            u8 *e = PU(p, 0x171C, u8 *) + 0x48;
            s16 stun = 100.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550) <= AT(e, 0x18, f32) ? 0x8000 : 0;

            func_00178070(gProgress, *(u8 *)&p->c.a.slot, hit, AT(e, 0x10, u8), AT(e, 0x12, u16), stun, AT(e, 0x14, f32));
            PU(p, 0x1764, s32) = 12;
        }
    }
    if (AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    }
}

/* the rise into state `state` with its sweep: his (func_0030A650) or the other class's */
static inline void Lorenzo2_Rise(Pursuer *p, const PTMF *state, s32 other) {
    if (Lorenzo2_Underground(p)) {
        return;
    }
    {
        f32 goal[4] __attribute__((aligned(16)));
        f32 v[4] __attribute__((aligned(16)));
        struct {
            f32 pos[4];
            s32 kind;
        } sk __attribute__((aligned(16)));
        u8 *mgr;
        u32 tri;
        f32 d, h;
        s32 slot;

        tri = func_00216E00(p, p->c.unk104[0], p->c.unk110, goal);
        d = func_00214B90(p, tri, goal);
        sceVu0SubVector(v, p->target->a.pos, goal);
        v[3] = 0.0f;
        if (__builtin_sqrtf(sceVu0InnerProduct(v, v)) < 5.0f) {
            d -= 5.0f;
        }
        if (!(d <= 0.0f)) {
            func_001273D0(&p->c, &p->c.a.navTri, p->c.a.pos, d);
        }
        h = func_001244D0(&p->c.a, p->target->a.pos);
        p->c.a.angle[1] = h;
        sceVu0UnitMatrix(p->c.a.rot);
        sceVu0RotMatrixY(p->c.a.rot, p->c.a.rot, h);
        func_00297B40(p, 0xE02, 1);
        p->c.a.disabled = 0;
        p->c.a.unk2D = 0;
        p->c.moveMode = 8;
        p->c.moveSub = 0x1C;
        mgr = D_0044E578;
        p->c.unkE8 = p->target->unkE8;
        p->c.unkEC = p->target->unkEC;
        slot = Effect_New(mgr, 0x700, Sink_Init);
        sceVu0CopyVector(sk.pos, p->c.a.pos);
        sk.kind = 1;
        func_002D6090(mgr, slot, &sk);
        Actor_SetState(&p->c.a, state);
        if (other) {
            Lorenzo2b_Sweep(p);
        } else {
            func_0030A650(p);
        }
    }
}

void func_0030A850(Pursuer *p) {
    Lorenzo2_Rise(p, &D_00423A98, 0);
}

extern const PTMF D_00423A68;

/* start of his stalk from below: when he may go for his target, it's reachable by the mesh and
   there's no exit for him (func_00177AB0 kind 9): finish the walk, then close in (state
   D_00423A68, Lorenzo2_Approach); otherwise action 0x17 */
void func_0030AF20(Pursuer *p) {
    f32 t[4] __attribute__((aligned(16)));
    u32 tri;

    if (!(func_00283870(p) & 0xFF)) {
        p->c.unk104[0] = 0;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x17);
        return;
    }
    sceVu0CopyVector(t, p->target->a.pos);
    tri = func_00216E00(p, p->target->a.navTri, t, t);
    if (func_001257B0(&p->c, tri, t, -1) < 0.0f || (func_00177AB0(gProgress, 9, *(u8 *)&p->c.a.slot) & 0xFF) != 0xFF) {
        p->c.unk104[0] = 0;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x17);
        return;
    }
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_00423A68);
    Lorenzo2_Approach(p);
}

extern const PTMF D_00423A58;

/* state: under the floor, then he rises (0x1305) at his goal (+0x104 / +0x110) facing his
   target, with the rising effect; its end ends the step (as func_0030B1E0) */
void func_0030B240(Pursuer *p) {
    struct {
        f32 pos[4];
        s32 kind;
    } sk __attribute__((aligned(16)));
    f32 goal[4] __attribute__((aligned(16)));
    u8 *mgr;
    u32 tri;
    f32 d, h;
    s32 slot;

    if (Lorenzo2_Underground(p)) {
        return;
    }
    tri = func_00216E00(p, p->c.unk104[0], p->c.unk110, goal);
    d = func_00214B90(p, tri, goal);
    if (!(d <= 0.0f)) {
        func_001273D0(&p->c, &p->c.a.navTri, p->c.a.pos, d);
    }
    h = func_001244D0(&p->c.a, p->target->a.pos);
    p->c.a.angle[1] = h;
    sceVu0UnitMatrix(p->c.a.rot);
    sceVu0RotMatrixY(p->c.a.rot, p->c.a.rot, h);
    func_00297B40(p, 0x1305, 1);
    mgr = D_0044E578;
    p->c.a.disabled = 0;
    p->c.a.unk2D = 0;
    p->c.unkE8 = p->target->unkE8;
    p->c.unkEC = p->target->unkEC;
    slot = Effect_New(mgr, 0x700, Sink_Init);
    sceVu0CopyVector(sk.pos, p->c.a.pos);
    sk.kind = 1;
    func_002D6090(mgr, slot, &sk);
    Actor_SetState(&p->c.a, &D_00423A58);
    Lorenzo2_PlayOut(p);
}

extern const PTMF D_00423A48;

/* state: sinking to come up by his target (0x1303). At its end (Lorenzo2_Sink) he heads under
   the floor for a point 10 short of his target along the path, or where he is when it's nearer
   than 20; state D_00423A48 (func_0030B240) */
void func_0030B540(Pursuer *p) {
    f32 t[4] __attribute__((aligned(16)));
    f32 d;

    func_00125A10(&p->c);
    if (!(AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END)) {
        return;
    }
    d = Lorenzo2_Sink(p, t);
    if (d < 20.0f) {
        sceVu0CopyVector(p->c.unk110, p->c.a.pos);
        p->c.unk104[0] = p->c.a.navTri;
    } else {
        u32 out;

        func_00214890(p, &out, p->c.unk110, d - 10.0f);
        p->c.unk104[0] = out;
    }
    Actor_SetState(&p->c.a, &D_00423A48);
    func_0030B240(p);
}

extern const PTMF D_00423A28;

/* start of sinking to come up by his target: when it's 30 or more away by the mesh and there's
   no exit for him (func_00177AB0 kind 9), finish the walk and sink (Lorenzo2_SinkBehind).
   Otherwise: out of reach or when he may not go, action 0x17; else his grab (0x13, attack 1) */
void func_0030B840(Pursuer *p) {
    f32 t[4] __attribute__((aligned(16)));
    u32 tri;
    f32 d;

    sceVu0CopyVector(t, p->target->a.pos);
    tri = func_00216E00(p, p->target->a.navTri, t, t);
    d = func_001257B0(&p->c, tri, t, -1);
    if (d < 30.0f || (func_00177AB0(gProgress, 9, *(u8 *)&p->c.a.slot) & 0xFF) != 0xFF) {
        if (d < 0.0f || !(func_00283870(p) & 0xFF)) {
            p->c.unk104[0] = 0;
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x17);
        } else {
            PU(p, 0x1728, s32) = 1;
            PU(p, 0x172C, u8) = 0;
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x13);
        }
        return;
    }
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_00423A28);
    Lorenzo2_SinkBehind(p);
}

/* ---- character kind 12 (vtable D_004718F0): a pursuer with his own tables and update ---- */

extern void *D_004718F0[], *D_0046D810[], *D_0046C220[], *D_00469C60[], *D_00469C20[], *D_00470F90[];
extern u8 D_00423D90[], D_00423DB0[], D_00423DE0[], D_00423DF8[], D_00423E10[], D_00423E38[],
    D_00423E50[], D_00423E70[], D_00423E88[], D_00423E98[], D_00423EB0[], D_00423EC8[],
    D_00423EE0[], D_00423F00[], D_00423F20[], D_00423F38[], D_00423F48[];
extern u8 D_00423FE0[], D_00424010[], D_00424040[], D_00424060[], D_00424080[], D_004240B0[],
    D_004240C0[], D_004240F0[], D_00424108[], D_00424118[], D_00424130[], D_00424158[],
    D_00424170[], D_00424190[], D_004241B0[], D_004241C8[], D_004241D8[];
extern u8 D_004241F0[], D_00423FB0[], D_019910F0[], D_00424238[], D_00423F60[], D_00423FD0[];
extern void func_001267F0(Character *c, s32 n);
extern u8 D_00423CF0[], D_00423D60[], D_00423D80[], D_00423BB0[], D_00423CE0[], D_0047AD18[];

/* vtable +0x8: destructor (0x4718F0 -> Pursuer 0x46D810 -> NPC 0x46C220 -> Character); the
 * model freed for slots 3..5 */
Pursuer *func_0030C3E0(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = D_004718F0;
        p->c.a.vtbl = D_0046D810;
        VCALL(p, 0x10, void (*)(Pursuer *))(p);
        if ((u32)p->c.a.slot >= 3 && (u32)p->c.a.slot < 6) {
            void **m = p->c.motion;

            if (m != NULL) {
                VCALL(m, 0x8, void (*)(void *, s32))(m, 1);
                p->c.motion = NULL;
            }
        }
        p->c.a.vtbl = D_0046C220;
        VCALL(p, 0x10, void (*)(Pursuer *))(p);
        p->c.a.vtbl = D_00469C60;
        p->c.a.vtbl = D_00469C20;
        if ((s16)flags > 0) {
            func_00124E40(&p->c.a);
        }
    }
    return p;
}

static void k12_mark_init(void **obj) {
    obj[0] = D_00470F90;
}

/* the marker effect D_00470F90 over him ({1, 0, 1, its slot}) */
static inline __attribute__((always_inline)) void k12_mark(Pursuer *p) {
    u8 *mgr;
    s32 slot, arg[4] __attribute__((aligned(16)));

    PU(p, 0x17C0, u8) = 1;
    mgr = D_0044E578;
    slot = Effect_New(mgr, 0x20, k12_mark_init);
    arg[0] = 1;
    arg[1] = 0;
    AT(&arg[2], 0, f32) = 1.0f;
    arg[3] = slot;
    func_002D6090(mgr, slot, arg);
}

/* vtable +0x38: func_002809E0, then (when func_00217510 allows) the marker */
void func_0030C520(Pursuer *p) {
    func_002809E0(p);
    if (func_00217510(p) != 0) {
        k12_mark(p);
    } else {
        PU(p, 0x17C0, u8) = 0;
    }
}

/* vtable +0x148: the same over func_002804B0 */
void func_0030C670(Pursuer *p) {
    func_002804B0(p);
    if (func_00217510(p) != 0) {
        k12_mark(p);
    } else {
        PU(p, 0x17C0, u8) = 0;
    }
}

/* his attack tables for situations 0..16; the first set when gProgress+0x30 bit 0x8000 */
static u8 *const sK12Tables[2][17] = {
    { D_00423D90, D_00423DE0, D_00423DB0, D_00423DF8, D_00423E10, D_00423E38, D_00423E50,
      D_00423E70, D_00423E88, D_00423E98, D_00423EB0, D_00423EC8, D_00423EE0, D_00423F00,
      D_00423F38, D_00423F48, D_00423F20 },
    { D_00423FE0, D_00424040, D_00424010, D_00424060, D_00424080, D_004240B0, D_004240C0,
      D_004240F0, D_00424108, D_00424118, D_00424130, D_00424158, D_00424170, D_00424190,
      D_004241C8, D_004241D8, D_004241B0 },
};

/* vtable +0x130: the attack table for a situation (out of range: the first of the plain set) */
void func_0030C7C0(Pursuer *p, s8 situation) {
    s32 alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;

    PU(p, 0x1718, u8 *) = (u32)situation < 17 ? sK12Tables[alt][situation] : D_00423D90;
}

/* vtable +0x84: a request of kind 4 seen (+0x14E8, not yet handled +0x14F0) raises the threat,
 * except in game modes 6 / 7; then func_0029C8C0 and back to full health */
void func_0030CA80(Pursuer *p) {
    if (PU(p, 0x14E8, s32) == 4 && PU(p, 0x14F0, s32) == 0 && func_00283870(p) != 0) {
        Progress *pr = gProgress;
        u8 mode = Progress_GetVar(pr, 0x26) & 0xFF;

        if (mode != 7 && mode != 6) {
            func_00178070(pr, *(u8 *)&p->c.a.slot, 1, 3, 0, 0, 100.0f);
        }
    }
    func_0029C8C0(p);
    p->c.hp = p->c.hpMax;
}

/* vtable +0x110: let his progress slot go if he holds one; -1 */
s32 func_0030CB40(Pursuer *p) {
    Progress *pr = gProgress;

    if ((func_00177870(pr, *(u8 *)&p->c.a.slot) & 0xFF) == 1) {
        func_001777D0(pr, *(u8 *)&p->c.a.slot);
    }
    return -1;
}

/* vtable +0x58: the marker off, then the Pursuer's */
void func_0030CBD0(Pursuer *p) {
    PU(p, 0x17C0, u8) = 0;
    func_0029E600(p);
}

/* vtable +0x30: his frame update - the stalkers', with the senses kept on him while the
 * behaviour is fresh outside cutscenes, and the threat raised when he hits */
void func_0030CBE0(Pursuer *p) {
    PTMF *st = (PTMF *)((u8 *)p + 0x174C);
    Progress *pr;

    VCALL(p, 0x84, void (*)(Pursuer *))(p);
    p->c.a.navMask = p->c.a.unk2B == 1 ? 8 : VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
    p->c.pathReq->mask = p->c.a.navMask;
    func_00215D80(p);
    if (PU(p, 0x16F6, u8) == 1) {
        if (AT(gProgress, 0x1FBEC1, u8) == 0) {
            PU(p, 0x1544, u8) = p->c.a.room == gCharPlayer->a.room;
            PU(p, 0x1545, u8) = 0;
            PU(p, 0x1546, u8) = 0;
            PU(p, 0x16C9, u8) = 5;
            PU(p, 0x16CA, u8) = 7;
        } else {
            func_002177D0(p);
        }
    } else {
        PU(p, 0x1544, u8) = 0;
        PU(p, 0x1545, u8) = 0;
        PU(p, 0x1546, u8) = 0;
        PU(p, 0x16CB, u8) = 0;
        PU(p, 0x16CC, u8) = 0;
    }
    VCALL(p, 0x120, void (*)(Pursuer *))(p);
    func_00297C60(p);
    pr = gProgress;
    if (AT(pr, 0x1FBEC1, u8) == 0) {
        if (PU(p, 0x16C8, u8) != 0) {
            func_0029B190(p);
        }
        VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
    }
    if (func_00217510(p) != 0) {
        func_00296FC0(p);
        func_0029B4B0(p);
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        if (func_00283870(p) != 0 && ((func_00217920(p) & 0xFF) & 1)) {
            func_00178070(pr, *(u8 *)&p->c.a.slot, 1, 3, 0, 0, 100.0f);
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

/* vtable +0xF4: his setup over the Pursuer's: tables (two sets by gProgress+0x30 bit 0x8000),
 * 9999 health, his stats */
void func_0030CFA0(Pursuer *p) {
    static const union { u32 u; f32 f; } k02 = {0x3E4CCCCD};

    func_0029FB20(p);
    p->c.hpMax = 9999;
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        PU(p, 0x1730, u8 *) = D_004241F0;
        PU(p, 0x1740, u8 *) = D_00423FB0;
        PU(p, 0x173C, u8 *) = D_019910F0;
        PU(p, 0x1748, u8 *) = D_00424238;
    } else {
        PU(p, 0x1730, u8 *) = D_00423F60;
        PU(p, 0x1740, u8 *) = D_00423FB0;
        PU(p, 0x173C, u8 *) = D_019910F0;
        PU(p, 0x1748, u8 *) = D_00423FD0;
    }
    PU(p, 0x16DC, s32) = 100;
    PU(p, 0x16E8, s32) = 0;
    PU(p, 0x16D4, s32) = 0x7512;
    PU(p, 0x16D8, s32) = 0;
    PU(p, 0x16D0, s32) = 0;
    PU(p, 0x16E0, s32) = 3000;
    PU(p, 0x16E4, s32) = 999;
    p->c.a.radius = 3.5f;
    p->c.a.height = 18.0f;
    p->c.hp = p->c.hpMax;
    p->c.hearThreshold = 12;
    PU(p, 0x171C, u8 *) = D_00423CF0;
    PU(p, 0x1720, u8 *) = D_00423D60;
    PU(p, 0x1724, u8 *) = D_00423D80;
    PU(p, 0x16AC, u8 *) = D_00423BB0;
    PU(p, 0x16B0, u8 *) = D_00423CE0;
    PU(p, 0x1734, u8 *) = D_0047AD18;
    PU(p, 0x1694, f32) = 8.0f;
    PU(p, 0x169C, f32) = -8.0f;
    PU(p, 0x1698, f32) = 8.0f;
    PU(p, 0x16A0, f32) = -8.0f;
    PU(p, 0x17C0, u8) = 1;
    PU(p, 0x17C4, f32) = k02.f;
    p->c.unkE4 = 0;
    func_001267F0(&p->c, 0x14);
}

/* ---- the same shapes in other classes, generated from the functions they copy (2026-10-05) ---- */
extern const f32 D_00445AF0[7][4];
extern u8 D_00444AF0[];
extern u8 D_00444AB0[];
extern u8 D_00444AD0[];
extern u8 D_00444A90[];

/* (as func_003095B0)  vtable +0x2D8: the point (x, 0, z) for spot `id` 10..15 (others untouched) */
void func_00363610(void *self, s32 id, f32 *out) {
    switch (id) {
    case 10:
    case 11:
        out[0] = 0x1.2d566c0000000p+0f;   /* 1.1771 */
        out[1] = 0.0f;
        out[2] = 0x1.3a28f60000000p+3f;   /* 9.8175 */
        break;
    case 12:
    case 13:
        out[0] = 0x1.e8587a0000000p-1f;   /* 0.9538 */
        out[1] = 0.0f;
        out[2] = 0x1.abdcc60000000p+3f;   /* 13.3707 */
        break;
    case 14:
        out[0] = -0x1.25a8580000000p+0f;  /* -1.1471 */
        out[1] = 0.0f;
        out[2] = -0x1.42a64c0000000p+1f;  /* -2.5207 */
        break;
    case 15:
        out[0] = -0x1.2f4f0e0000000p-3f;  /* -0.1481 */
        out[1] = 0.0f;
        out[2] = -0x1.fd1eb80000000p+1f;  /* -3.9775 */
        break;
    }
}

/* (as func_00309680)  his slam 0x2301: at its key (2), eight grey dust clouds of random size (320..640) at his
   hand (bone 0x32) */
void func_003636D0(Pursuer *p) {
    struct {
        f32 pos[4];
        s32 kind, r, g, b, size;
    } dp __attribute__((aligned(16)));
    u8 *mgr;
    VObject *rnd;
    u8 i;

    if (MOTION_ANIM(p) != 0x2301 || !(func_001F4770(p->c.motion, 0, 0, 1) & 0xFF & 2)) {
        return;
    }
    sceVu0CopyVector(dp.pos, func_0017CE80(MOTION_AT(p, 0x810, u8 *), 0x32) + 0xC);
    dp.pos[3] = 1.0f;
    dp.kind = 2;
    dp.b = 0x50;
    dp.g = 0x50;
    dp.r = 0x50;
    mgr = D_0044E578;
    rnd = D_0044E550;
    for (i = 0; i < 8; i++) {
        s32 slot = Effect_New(mgr, 0x720, Dust_Init);

        dp.size = (s32)(320.0f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd)) + 320;
        func_002D6090(mgr, slot, &dp);
    }
}

/* (as func_00309890)  his blows 0x1904 / 0xE00 / 0xE04 / 0xE05 (hand, bone 0x32) and 0xE06 (bone 0x28): at the
   key (0x20), seven sparks around the bone (offsets D_00445AF0) of random scale, speed and life */
void func_003638E0(Pursuer *p) {
    struct {
        f32 pos[4];
        f32 one, scale, speed;
        s32 life;
    } sp __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    VObject *rnd;
    u8 *mgr;
    s32 bone;
    u8 i;

    switch (MOTION_ANIM(p)) {
    case 0x1904:
    case 0xE05:
    case 0xE04:
    case 0xE00:
        bone = 0x32;
        break;
    case 0xE06:
        bone = 0x28;
        break;
    default:
        return;
    }
    if (!(func_001F4770(p->c.motion, 0, 0, 1) & 0xFF & 0x20)) {
        return;
    }
    sp.one = 1.0f;
    sceVu0CopyVector(at, func_0017CE80(MOTION_AT(p, 0x810, u8 *), bone) + 0xC);
    rnd = D_0044E550;
    mgr = D_0044E578;
    for (i = 0; i < 7; i++) {
        s32 slot;

        sp.scale = 0.0f + 0.5f + 0.5f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);
        sp.speed = 0.0f + 0x1.19999ap+0f + 0x1.99999ap-5f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);   /* 1.1 + 0.05 */
        sp.life = (s32)(6.0f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd)) + 12;
        slot = Effect_New(mgr, 0x130, Spark_Init);
        sceVu0AddVector(sp.pos, at, D_00445AF0[i]);
        func_002D6090(mgr, slot, &sp);
    }
}

/* (as func_00309BA0)  vtable +0x138: the hit points of an attack entry; his grab 0xE01 is at Fiona herself */
void func_00363BF0(Pursuer *p, s32 *e, f32 *a, f32 *b) {
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

/* (as func_0030A210)  state: his grab. Until its aim key (frame 12, key 2) it follows the target (+0x110); at the
   hit key whoever is in reach of attack entry 3 (+0x171C +0x6C; func_002179F0 at the aimed
   point) is hit, stunned by the entry's +0x18 chance, once each (+0x1760); the burst effect at
   the point (also to +0x1770). Its end ends the step */
void func_00363FA0(Pursuer *p) {
    func_00125A10(&p->c);
    if (func_001F4770(p->c.motion, 0, 0xC, 1) & 0xFF & 2) {
        sceVu0CopyVector(p->c.unk110, p->target->a.pos);
    } else if (func_001F4770(p->c.motion, 0, 0, 1) & 0xFF & 2) {
        u8 *e = PU(p, 0x171C, u8 *) + 0x6C;
        u32 hit = func_002179F0(p, (s32)(u32)p->c.unk110, AT(e, 0xC, f32)) & 0xFF;
        u8 *mgr;

        if (func_00283870(p) != 0 && (hit & ~PU(p, 0x1760, u8))) {
            s16 stun = 100.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550) <= AT(e, 0x18, f32) ? 0x8000 : 0;

            func_00178070(gProgress, *(u8 *)&p->c.a.slot, hit, AT(e, 0x10, u8), AT(e, 0x12, u16), stun, AT(e, 0x14, f32));
            PU(p, 0x1764, s32) = 12;
        }
        sceVu0CopyVector((f32 *)((u8 *)p + 0x1770), p->c.unk110);
        mgr = D_0044E578;
        func_002D6090(mgr, Effect_New(mgr, 0xFC0, Burst_Init), p->c.unk110);
    }
    if (AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        AT(p->c.unk110, 0x0, s32) = 0;
        AT(p->c.unk110, 0x4, s32) = 0;
        AT(p->c.unk110, 0x8, s32) = 0;
        AT(p->c.unk110, 0xC, s32) = 0;
        if (AT(gProgress, 0x30, u32) & 0x8000) {
            PU(p, 0x16F7, u8) = 0;
        }
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    }
}

/* (as func_0030B1E0)  state: see Lorenzo2_PlayOut */
void func_00364F90(Pursuer *p) {
    Lorenzo2_PlayOut(p);
}

/* (as func_0030BB70)  is his slam 0x2301 at its impact key (0x20) now (active and on screen) */
s32 func_00365850(Pursuer *p) {
    if (!p->c.a.active || func_00217510(p) == 0 || MOTION_ANIM(p) != 0x2301) {
        return 0;
    }
    return (func_001F4770(p->c.motion, 0, 0, 1) & 0xFF & 0x20) ? 1 : 0;
}

/* (as func_002F9000)  his model files (func_0029F8C0 for kind 11) */
u8 *func_00365D10(Pursuer *p) {
    return (AT(gProgress, 0x30, u32) & 0x8000) ? D_00444AF0 : D_00444AB0;
}

/* (as func_002F9000)  his model files (func_0029F8C0 for kind 11) */
u8 *func_00365D50(Pursuer *p) {
    return (AT(gProgress, 0x30, u32) & 0x8000) ? D_00444AD0 : D_00444A90;
}

/* ---- the same shapes in other classes, generated from the functions they copy (2026-10-05) ---- */
extern const PTMF D_00445AB8;
extern void func_00364510(Pursuer *p);
extern const PTMF D_00445A88;
extern const PTMF D_00445A78;
extern void func_00364FF0(Pursuer *p);

/* as func_0030AB80 */
void func_00364930(Pursuer *p) {
    f32 t[4] __attribute__((aligned(16)));
    f32 d;

    func_00125A10(&p->c);
    if (!(AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END)) {
        return;
    }
    d = Lorenzo2_Sink(p, t);
    if (d < 0.0f) {
        sceVu0CopyVector(p->c.unk110, p->c.a.pos);
        p->c.unk104[0] = p->c.a.navTri;
    } else {
        u32 out;

        func_00214890(p, &out, p->c.unk110, d);
        p->c.unk104[0] = out;
    }
    Actor_SetState(&p->c.a, &D_00445AB8);
    func_00364510(p);
}

/* (as func_0030B240)  state: under the floor, then he rises (0x1305) at his goal (+0x104 / +0x110) facing his
   target, with the rising effect; its end ends the step (as func_0030B1E0) */
void func_00364FF0(Pursuer *p) {
    struct {
        f32 pos[4];
        s32 kind;
    } sk __attribute__((aligned(16)));
    f32 goal[4] __attribute__((aligned(16)));
    u8 *mgr;
    u32 tri;
    f32 d, h;
    s32 slot;

    if (Lorenzo2_Underground(p)) {
        return;
    }
    tri = func_00216E00(p, p->c.unk104[0], p->c.unk110, goal);
    d = func_00214B90(p, tri, goal);
    if (!(d <= 0.0f)) {
        func_001273D0(&p->c, &p->c.a.navTri, p->c.a.pos, d);
    }
    h = func_001244D0(&p->c.a, p->target->a.pos);
    p->c.a.angle[1] = h;
    sceVu0UnitMatrix(p->c.a.rot);
    sceVu0RotMatrixY(p->c.a.rot, p->c.a.rot, h);
    func_00297B40(p, 0x1305, 1);
    mgr = D_0044E578;
    p->c.a.disabled = 0;
    p->c.a.unk2D = 0;
    p->c.unkE8 = p->target->unkE8;
    p->c.unkEC = p->target->unkEC;
    slot = Effect_New(mgr, 0x700, Sink_Init);
    sceVu0CopyVector(sk.pos, p->c.a.pos);
    sk.kind = 1;
    func_002D6090(mgr, slot, &sk);
    Actor_SetState(&p->c.a, &D_00445A88);
    Lorenzo2_PlayOut(p);
}

/* (as func_0030B540)  state: sinking to come up by his target (0x1303). At its end (Lorenzo2_Sink) he heads under
   the floor for a point 10 short of his target along the path, or where he is when it's nearer
   than 20; state D_00445A78 (func_00364FF0) */
void func_003652F0(Pursuer *p) {
    f32 t[4] __attribute__((aligned(16)));
    f32 d;

    func_00125A10(&p->c);
    if (!(AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END)) {
        return;
    }
    d = Lorenzo2_Sink(p, t);
    if (d < 20.0f) {
        sceVu0CopyVector(p->c.unk110, p->c.a.pos);
        p->c.unk104[0] = p->c.a.navTri;
    } else {
        u32 out;

        func_00214890(p, &out, p->c.unk110, d - 10.0f);
        p->c.unk104[0] = out;
    }
    Actor_SetState(&p->c.a, &D_00445A78);
    func_00364FF0(p);
}

/* ---- the same states in the other class (its states D_00445Axx), sharing Lorenzo's helpers ---- */

extern const PTMF D_00445A58, D_00445A68, D_00445A98, D_00445AA8;
extern void func_00364930(Pursuer *p);
extern void func_003652F0(Pursuer *p);

/* (as func_0030AE00) */
void func_00364BB0(Pursuer *p) {
    Lorenzo2_ApproachAs(p, &D_00445AA8, func_00364930);
}

/* (as func_0030B7C0) */
void func_00365570(Pursuer *p) {
    Lorenzo2_SinkBehindAs(p, &D_00445A68, func_003652F0);
}

/* (as func_0030AF20) */
void func_00364CD0(Pursuer *p) {
    f32 t[4] __attribute__((aligned(16)));
    u32 tri;

    if (!(func_00283870(p) & 0xFF)) {
        p->c.unk104[0] = 0;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x17);
        return;
    }
    sceVu0CopyVector(t, p->target->a.pos);
    tri = func_00216E00(p, p->target->a.navTri, t, t);
    if (func_001257B0(&p->c, tri, t, -1) < 0.0f || (func_00177AB0(gProgress, 9, *(u8 *)&p->c.a.slot) & 0xFF) != 0xFF) {
        p->c.unk104[0] = 0;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x17);
        return;
    }
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_00445A98);
    Lorenzo2_ApproachAs(p, &D_00445AA8, func_00364930);
}

/* (as func_0030B840) */
void func_003655F0(Pursuer *p) {
    f32 t[4] __attribute__((aligned(16)));
    u32 tri;
    f32 d;

    sceVu0CopyVector(t, p->target->a.pos);
    tri = func_00216E00(p, p->target->a.navTri, t, t);
    d = func_001257B0(&p->c, tri, t, -1);
    if (d < 30.0f || (func_00177AB0(gProgress, 9, *(u8 *)&p->c.a.slot) & 0xFF) != 0xFF) {
        if (d < 0.0f || !(func_00283870(p) & 0xFF)) {
            p->c.unk104[0] = 0;
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x17);
        } else {
            PU(p, 0x1728, s32) = 1;
            PU(p, 0x172C, u8) = 0;
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x13);
        }
        return;
    }
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_00445A58);
    Lorenzo2_SinkBehindAs(p, &D_00445A68, func_003652F0);
}

extern const PTMF D_00445AD8;
extern void func_00363FA0(Pursuer *p);

/* (as func_0030A4D0) the other class's start of the grab */
void func_00364260(Pursuer *p) {
    if (!(func_00283870(p) & 0xFF)) {
        p->c.unk104[0] = 0;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x17);
        return;
    }
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    func_00297B40(p, 0xE01, 0);
    p->c.unk104[0] = 0;
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        PU(p, 0x16F7, u8) = 1;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_00445AD8);
    func_00363FA0(p);
}

extern u8 D_00444ED0[], D_00445990[], D_004459E0[], D_00445A00[], D_00445A48[], D_00444D40[], D_004454E0[],
    D_00445530[], D_00445550[], D_00445598[], D_00444CE0[], D_00445060[], D_00445090[], D_00444B50[],
    D_00444CA0[], D_0047B010[];

/* (as func_0030C230) the other class's setup: its tables, and its own stats */
void func_00365D90(Pursuer *p) {
    func_0029FB20(p);
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        p->c.hpMax = 300;
        PU(p, 0x171C, u8 *) = D_00444ED0;
        PU(p, 0x1730, u8 *) = D_00445990;
        PU(p, 0x1740, u8 *) = D_004459E0;
        PU(p, 0x173C, u8 *) = D_00445A00;
        PU(p, 0x1748, u8 *) = D_00445A48;
        PU(p, 0x16DC, s32) = 75;
        PU(p, 0x16E8, f32) = 65.0f;
        PU(p, 0x16D4, s32) = 900;
        PU(p, 0x16D8, s32) = 360;
    } else {
        p->c.hpMax = 250;
        PU(p, 0x171C, u8 *) = D_00444D40;
        PU(p, 0x1730, u8 *) = D_004454E0;
        PU(p, 0x1740, u8 *) = D_00445530;
        PU(p, 0x173C, u8 *) = D_00445550;
        PU(p, 0x1748, u8 *) = D_00445598;
        PU(p, 0x16DC, s32) = 40;
        PU(p, 0x16E8, f32) = 30.0f;
        PU(p, 0x16D4, s32) = 900;
        PU(p, 0x16D8, s32) = 300;
    }
    PU(p, 0x16D0, s32) = 0;
    PU(p, 0x16E0, s32) = 9000;
    PU(p, 0x16E4, s32) = 150;
    p->c.a.radius = 4.0f;
    p->c.a.height = 18.0f;
    p->c.hp = p->c.hpMax;
    p->c.hearThreshold = 12;
    PU(p, 0x1714, u8 *) = D_00444CE0;
    PU(p, 0x1720, u8 *) = D_00445060;
    PU(p, 0x1724, u8 *) = D_00445090;
    PU(p, 0x16AC, u8 *) = D_00444B50;
    PU(p, 0x16B0, u8 *) = D_00444CA0;
    PU(p, 0x1734, u8 *) = D_0047B010;
    PU(p, 0x1694, f32) = 8.0f;
    PU(p, 0x169C, f32) = -8.0f;
    PU(p, 0x1698, f32) = 8.0f;
    PU(p, 0x16A0, f32) = -8.0f;
}

/* ---- the other class (vtable D_00479B20; code 0x363CE0..0x365AD0): its own tables, sweep and
   frame update ---- */

extern u8 D_004450C0[], D_00445140[], D_004451A0[], D_00445200[], D_00445230[], D_00445290[],
    D_004452C0[], D_00445330[], D_00445390[], D_004453C0[], D_004453E0[], D_00445430[],
    D_00445450[], D_00445470[], D_004454A0[], D_004454B8[], D_004454C8[], D_004455B0[],
    D_00445630[], D_00445690[], D_004456F0[], D_00445720[], D_00445780[], D_004457B0[],
    D_00445820[], D_00445870[], D_004458A0[], D_004458C0[], D_00445900[], D_00445918[],
    D_00445930[], D_00445950[], D_00445968[], D_00445978[];

/* its attack tables; the second set when gProgress+0x30 bit 0x8000 */
static u8 *const sAttackTables3[2][17] = {
    { D_004450C0, D_004451A0, D_00445140, D_00445200, D_00445230, D_00445290, D_004452C0,
      D_00445330, D_00445390, D_004453C0, D_004453E0, D_00445430, D_00445450, D_00445470,
      D_004454B8, D_004454C8, D_004454A0 },
    { D_004455B0, D_00445690, D_00445630, D_004456F0, D_00445720, D_00445780, D_004457B0,
      D_00445820, D_00445870, D_004458A0, D_004458C0, D_00445900, D_00445918, D_00445930,
      D_00445968, D_00445978, D_00445950 },
};

/* vtable +0x130 (as func_0030C7C0) */
void func_00363CE0(Pursuer *p, s8 situation) {
    s32 alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;

    PU(p, 0x1718, u8 *) = sAttackTables3[alt][(u32)situation < 17 ? situation : 0];
}

/* its sweep state */
void func_003643E0(Pursuer *p) {
    Lorenzo2b_Sweep(p);
}

extern const PTMF D_00445AC8;

/* its rise from under the floor (as func_0030A850) into the sweep */
void func_00364510(Pursuer *p) {
    Lorenzo2_Rise(p, &D_00445AC8, 1);
}

/* vtable +0x30: its frame update (as func_0030BE20, without his mode 2), with its dust
   (func_003638E0 / func_003636D0) */
void func_003659D0(Pursuer *p) {
    PTMF *st = (PTMF *)((u8 *)p + 0x174C);

    Stalker_ThinkStart(p);
    if (func_00217510(p) != 0) {
        func_00296FC0(p);
        if (p->c.a.disabled != 0 && (PU(p, 0x175C, s32) == 0x10 || PU(p, 0x175C, s32) == 0x21)) {
            p->c.a.disabled = 0;
            p->c.a.unk2D = 0;
        }
        func_0029B4B0(p);
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        func_003638E0(p);
        func_003636D0(p);
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
