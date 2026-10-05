/* Daniella: the Pursuer of the middle chapters (kind 3, vtable 0x46BBB0; code 0x20C3A0..0x20D860).
 * She mostly runs the Pursuer's own behaviour; her class overrides her setup and tables, the
 * bone positions of her attacks, and a hit effect at her hand. The object is a plain Pursuer
 * (0x1800 bytes). See pursuer.h. */
#include "common.h"
#include "pursuer.h"
#include "progress.h"
#include "effectmgr.h"
#include "sce/libvu0.h"

extern void *D_0046BBB0[];

/* vtable +0x8: destructor */
Pursuer *func_0020C3A0(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = D_0046BBB0;
        if (p != NULL) {
            Pursuer_DestroyBase(p);
        }
        if ((s16)flags > 0) {
            func_00124E40(&p->c.a);
        }
    }
    return p;
}

/* vtable +0x32C: her character kind */
s32 func_0020C4B0(Pursuer *p) {
    return 3;
}

/* vtable +0x1C: the model files loaded (func_0029F120), then motion vtable +0x34 */
void func_0020C4D0(Pursuer *p) {
    void *m;

    func_0029F120(p);
    m = p->c.motion;
    VCALL(m, 0x34, void (*)(void *, s32))(m, 0);
}

/* vtable +0x9C: where she stands by a door, by side 0..3 (local offsets) */
void func_0020C510(Pursuer *p, s32 side, f32 *out) {
    f32 z;

    switch (side) {
    case 0:
        z = -0x1.bdc432p+2f;   /* -6.9651 */
        break;
    case 1:
        z = 0x1.be8240p+2f;    /* 6.9767 */
        break;
    case 2:
        z = 0x1.ce0418p+2f;    /* 7.2190 */
        break;
    case 3:
        z = -0x1.905f06p+2f;   /* -6.2558 */
        break;
    default:
        return;
    }
    out[0] = 0.0f;
    AT(out, 0x4, s32) = 0;
    out[2] = z;
}

/* vtable +0x2D8: the offsets of her actions 10..15 (local) */
void func_0020C5B0(Pursuer *p, s32 kind, f32 *out) {
    f32 x, z;

    switch (kind) {
    case 10:
    case 11:
        x = 0x1.07c84cp-2f;    /* 0.2576 */
        z = 0x1.567fccp+3f;    /* 10.7031 */
        break;
    case 12:
    case 13:
        x = 0x1.a4a8c2p+0f;    /* 1.6432 */
        z = 0x1.5d182ap+3f;    /* 10.9092 */
        break;
    case 14:
        x = -0x1.5f06f6p-3f;   /* -0.1714 */
        z = -0x1.8f6fd2p+1f;   /* -3.1206 */
        break;
    case 15:
        x = 0x1.9a0276p-2f;    /* 0.4004 */
        z = -0x1.792d78p+1f;   /* -2.9467 */
        break;
    default:
        return;
    }
    out[0] = x;
    AT(out, 0x4, s32) = 0;
    out[2] = z;
}

/* vtable +0xE4: at a door she breaks (func_00178980) while opening or attacking it: mark it
   (vtable +0xF0) and change room through it (vtable +0x28) */
void func_0020C660(Pursuer *p, s32 exit) {
    if (!(func_00178980(gProgress, p->c.a.room, exit) & 0xFF)) {
        return;
    }
    switch (PU(p, 0x175C, s32)) {
    case 0xC:
    case 0x29:
    case 0x28:
    case 0xD:
        VCALL(p, 0xF0, void (*)(Pursuer *, s32))(p, exit);
        VCALL(p, 0x28, void (*)(Pursuer *, s32, s32, s32))(p, VCALL(D_0044E568, 0x28, s32 (*)(VObject *, s32))(D_0044E568, exit), 0, 0);
        break;
    }
}

/* vtable +0xF0: the door `exit` of her room used (func_00178DB0), then damaged: in mode 2
   (+0x16B8) by func_00178C10, otherwise func_00178A90 */
void func_0020C730(Pursuer *p, s32 exit) {
    Progress *pr = gProgress;

    func_00178DB0(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
    if (PU(p, 0x16B8, s32) == 2) {
        func_00178C10(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
    } else {
        func_00178A90(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
    }
}

/* the effect she leaves at a hit (0xE60 bytes; vtable 0x470F30, its part at +0xC10) */
extern void *D_00470F30[], *D_00469D00[], *D_0046FC30[];

static inline void DaniellaHit_Init(void **obj) {
    obj[0] = D_00470F30;
    obj[0xC10 / 4] = D_00469D00;
    ((s32 *)obj)[0xC14 / 4] = -1;
    obj[0xC10 / 4] = D_0046FC30;
}

typedef struct {
    f32 pos[4];
    u32 kind;      /* 0xFE */
    f32 big;       /* 1.0 or 0 */
} DaniellaHitParams;

extern const f32 D_003D8910[4];

/* the effect at her blow: for her grabs of Fiona (0x1904 / 0x1A01) at Fiona's bone, for her
   strikes (0xE00..0xE07) at a point along her hand (bone 0x2D; 1.5 out, 3.5 for 0xE03 / 0xE05);
   larger for 0xE04 / 0xE05 and the grabs */
void func_0020C7C0(Pursuer *p) {
    DaniellaHitParams hp;
    f32 pos[4] __attribute__((aligned(16)));
    f32 reach = 1.5f;
    s32 atHand = 1;
    u8 *mgr;

    switch (MOTION_ANIM(p)) {
    case 0x1904:
        atHand = 0;
        sceVu0CopyVector(pos, func_0017CE80(AT(gCharPlayer->motion, 0x810, u8 *), 0x17) + 0xC);
        hp.big = 1.0f;
        break;
    case 0x1A01: {
        u8 *fm = gCharPlayer->motion;
        s32 bone;

        atHand = 0;
        bone = VCALL(fm, 0x80, s32 (*)(void *))(fm);
        sceVu0CopyVector(pos, func_0017CE80(AT(gCharPlayer->motion, 0x810, u8 *), bone) + 0xC);
        hp.big = 1.0f;
        break;
    }
    case 0xE05:
        reach = 3.5f;
        /* fallthrough */
    case 0xE04:
        hp.big = 1.0f;
        break;
    case 0xE03:
        reach = 3.5f;
        /* fallthrough */
    case 0xE07:
    case 0xE06:
    case 0xE02:
    case 0xE00:
        hp.big = 0.0f;
        break;
    default:
        return;
    }
    if (atHand) {
        f32 off[4] __attribute__((aligned(16)));
        f32 m[4][4] __attribute__((aligned(16)));
        f32 hand[4] __attribute__((aligned(16)));

        off[0] = D_003D8910[0];
        off[1] = D_003D8910[1];
        off[2] = reach;
        off[3] = D_003D8910[3];
        sceVu0CopyMatrix(m, (f32 (*)[4])func_0017CE80(MOTION_AT(p, 0x810, u8 *), 0x2D));
        func_002E2DA0(pos, m, off);
        sceVu0CopyVector(hand, func_0017CE80(MOTION_AT(p, 0x810, u8 *), 0x2D) + 0xC);
        sceVu0AddVector(pos, pos, hand);
    }
    hp.kind = 0xFE;
    hp.pos[0] = pos[0];
    hp.pos[1] = pos[1];
    hp.pos[2] = pos[2];
    hp.pos[3] = pos[3];
    mgr = D_0044E578;
    func_002D6090(mgr, Effect_New(mgr, 0xE60, DaniellaHit_Init), &hp);
}

extern const f32 D_003D8900[4];

/* vtable +0x138: the hit points of an attack entry (`e`: +0 animation, +4 / +8 bones). Her
   strikes 0xE00, 0xE02..0xE08 reach along her hand (bone 0x2D, offset D_003D8900) from bone
   +4, and back to the hand; others the two bones (the first twice when +8 is none) */
void func_0020CAE0(Pursuer *p, s32 *e, f32 *a, f32 *b) {
    if ((u32)(e[0] - 0xE00) < 9 && e[0] != 0xE01) {
        f32 off[4] __attribute__((aligned(16)));
        f32 m[4][4] __attribute__((aligned(16)));
        f32 t[4] __attribute__((aligned(16)));

        off[0] = D_003D8900[0];
        off[1] = D_003D8900[1];
        off[2] = D_003D8900[2];
        off[3] = D_003D8900[3];
        sceVu0CopyMatrix(m, (f32 (*)[4])func_0017CE80(MOTION_AT(p, 0x810, u8 *), 0x2D));
        func_002E2DA0(a, m, off);
        sceVu0CopyVector(t, func_0017CE80(MOTION_AT(p, 0x810, u8 *), e[1]) + 0xC);
        sceVu0AddVector(a, a, t);
        sceVu0CopyVector(b, func_0017CE80(MOTION_AT(p, 0x810, u8 *), 0x2D) + 0xC);
        return;
    }
    sceVu0CopyVector(a, func_0017CE80(MOTION_AT(p, 0x810, u8 *), e[1]) + 0xC);
    if (e[2] >= 0) {
        sceVu0CopyVector(b, func_0017CE80(MOTION_AT(p, 0x810, u8 *), e[2]) + 0xC);
    } else {
        sceVu0CopyVector(b, func_0017CE80(MOTION_AT(p, 0x810, u8 *), e[1]) + 0xC);
    }
}

extern u8 D_003D77B0[], D_003D7810[], D_003D7880[], D_003D78B0[], D_003D78E0[], D_003D7940[],
    D_003D7970[], D_003D79C0[], D_003D7A20[], D_003D7A50[], D_003D7A70[], D_003D7AA0[],
    D_003D7AC0[], D_003D7AF0[], D_003D7B10[], D_003D7B40[], D_003D7B50[];
extern u8 D_003D7B60[], D_003D7BA0[], D_003D7BE0[], D_003D7C10[], D_003D7C40[], D_003D7C80[],
    D_003D7CC0[], D_003D7D00[], D_003D7D40[], D_003D7D60[], D_003D7D80[], D_003D7DA8[],
    D_003D7DC0[], D_003D7DF0[], D_003D7E10[], D_003D7E40[], D_003D7E50[];
extern u8 D_003D7FB0[], D_003D8020[], D_003D8080[], D_003D80D0[], D_003D8100[], D_003D8160[],
    D_003D8190[], D_003D81F0[], D_003D8250[], D_003D8280[], D_003D82A0[], D_003D82D0[],
    D_003D8300[], D_003D8330[], D_003D8360[], D_003D8390[], D_003D83A0[];
extern u8 D_003D83B0[], D_003D8410[], D_003D8470[], D_003D84D0[], D_003D8500[], D_003D8560[],
    D_003D85B0[], D_003D8610[], D_003D8670[], D_003D86A0[], D_003D86C0[], D_003D86F0[],
    D_003D8720[], D_003D8748[], D_003D8760[], D_003D8790[], D_003D87A0[];

/* her attack tables for situations 0..16: [gProgress+0x30 bit 0x8000][mode 2 (+0x16B8)] */
static u8 *const sAttackTables[2][2][17] = {
    {
        { D_003D77B0, D_003D7880, D_003D7810, D_003D78B0, D_003D78E0, D_003D7940, D_003D7970,
          D_003D79C0, D_003D7A20, D_003D7A50, D_003D7A70, D_003D7AA0, D_003D7AC0, D_003D7AF0,
          D_003D7B40, D_003D7B50, D_003D7B10 },
        { D_003D7B60, D_003D7BE0, D_003D7BA0, D_003D7C10, D_003D7C40, D_003D7C80, D_003D7CC0,
          D_003D7D00, D_003D7D40, D_003D7D60, D_003D7D80, D_003D7DA8, D_003D7DC0, D_003D7DF0,
          D_003D7E40, D_003D7E50, D_003D7E10 },
    },
    {
        { D_003D7FB0, D_003D8080, D_003D8020, D_003D80D0, D_003D8100, D_003D8160, D_003D8190,
          D_003D81F0, D_003D8250, D_003D8280, D_003D82A0, D_003D82D0, D_003D8300, D_003D8330,
          D_003D8390, D_003D83A0, D_003D8360 },
        { D_003D83B0, D_003D8470, D_003D8410, D_003D84D0, D_003D8500, D_003D8560, D_003D85B0,
          D_003D8610, D_003D8670, D_003D86A0, D_003D86C0, D_003D86F0, D_003D8720, D_003D8748,
          D_003D8790, D_003D87A0, D_003D8760 },
    },
};

/* vtable +0x130: the attack table for a situation */
void func_0020CC70(Pursuer *p, s8 situation) {
    s32 alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;
    s32 mode2 = PU(p, 0x16B8, s32) == 2;

    PU(p, 0x1718, u8 *) = sAttackTables[alt][mode2][(u32)situation < 17 ? situation : 0];
}

extern u8 D_003D7E60[], D_003D7EB0[], D_003D7F00[], D_003D7F20[], D_003D7F88[], D_003D7F98[];
extern u8 D_003D87B0[], D_003D8800[], D_003D8850[], D_003D8870[], D_003D88D8[], D_003D88E8[];

/* vtable +0x31C: mode 2 (+0x16B8) on or off, with its hold-off (+0x1730), +0x1740 and cry
   (+0x1748) tables */
void func_0020D1F0(Pursuer *p, s32 on) {
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        if (on) {
            PU(p, 0x16B8, s32) = 2;
            PU(p, 0x1730, u8 *) = D_003D8800;
            PU(p, 0x1748, u8 *) = D_003D88E8;
            PU(p, 0x1740, u8 *) = D_003D8870;
        } else {
            PU(p, 0x16B8, s32) = 0;
            PU(p, 0x1730, u8 *) = D_003D87B0;
            PU(p, 0x1748, u8 *) = D_003D88D8;
            PU(p, 0x1740, u8 *) = D_003D8850;
        }
    } else if (on) {
        PU(p, 0x16B8, s32) = 2;
        PU(p, 0x1730, u8 *) = D_003D7EB0;
        PU(p, 0x1748, u8 *) = D_003D7F98;
        PU(p, 0x1740, u8 *) = D_003D7F20;
    } else {
        PU(p, 0x16B8, s32) = 0;
        PU(p, 0x1730, u8 *) = D_003D7E60;
        PU(p, 0x1748, u8 *) = D_003D7F88;
        PU(p, 0x1740, u8 *) = D_003D7F00;
    }
}

/* vtable +0x310 / +0x30C */
s32 func_0020D2E0(Pursuer *p) {
    return 12;
}

s32 func_0020D2F0(Pursuer *p) {
    return 11;
}

/* vtable +0x308 / +0x300 / +0x2F0 */
f32 func_0020D300(Pursuer *p) {
    return 15.0f;
}

f32 func_0020D310(Pursuer *p) {
    return 5.0f;
}

f32 func_0020D320(Pursuer *p) {
    return 12.0f;
}

/* vtable +0x30: her frame update: as Debilitas's (func_001297C0), but without his growl and
   senses step; on screen the hit effect (func_0020C7C0) when a cry is heard or her animation
   reaches its effect key (0x20) */
void func_0020D330(Pursuer *p) {
    PTMF *st = (PTMF *)((u8 *)p + 0x174C);

    Stalker_ThinkStart(p);
    if (func_00217510(p) != 0) {
        func_00296FC0(p);
        if (func_0029B4B0(p) != 0 || (func_001F4770(p->c.motion, 0, -1, 1) & 0xFF & 0x20)) {
            func_0020C7C0(p);
        }
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

extern u8 D_003D7330[], D_003D7350[], D_003D7370[], D_003D7390[];

/* her model files (func_0029F8C0 for kind 3) */
u8 *func_0020D620(Pursuer *p) {
    return (AT(gProgress, 0x30, u32) & 0x8000) ? D_003D7390 : D_003D7350;
}

/* vtable +0xF8: her model files in slot 2 */
u8 *func_0020D660(Pursuer *p) {
    return (AT(gProgress, 0x30, u32) & 0x8000) ? D_003D7370 : D_003D7330;
}

extern u8 D_003D73F0[], D_003D7530[], D_003D7550[], D_003D7730[], D_003D7760[], D_003D7F40[],
    D_003D8890[], D_0047A928[];

/* vtable +0xF4: her setup over the Pursuer's (func_0029FB20): her tables and stats (different
   when gProgress+0x30 bit 0x8000 is set) */
void func_0020D6A0(Pursuer *p) {
    void *m;

    func_0029FB20(p);
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        p->c.hpMax = 100;
        PU(p, 0x171C, u8 *) = D_003D7550;
        PU(p, 0x1730, u8 *) = D_003D87B0;
        PU(p, 0x1740, u8 *) = D_003D8850;
        PU(p, 0x173C, u8 *) = D_003D8890;
        PU(p, 0x1748, u8 *) = D_003D88D8;
        PU(p, 0x16DC, s32) = 80;              /* Hewie bite tolerance */
        PU(p, 0x16E8, f32) = 25.0f;
        PU(p, 0x16D4, s32) = 450;
        PU(p, 0x16D8, s32) = 6000;
        PU(p, 0x16D0, s32) = 300;
        PU(p, 0x16E0, s32) = 6000;
        PU(p, 0x16E4, s32) = 100;
    } else {
        p->c.hpMax = 120;
        PU(p, 0x171C, u8 *) = D_003D7550;
        PU(p, 0x1730, u8 *) = D_003D7E60;
        PU(p, 0x1740, u8 *) = D_003D7F00;
        PU(p, 0x173C, u8 *) = D_003D7F40;
        PU(p, 0x1748, u8 *) = D_003D7F88;
        PU(p, 0x16DC, s32) = 200;
        PU(p, 0x16E8, f32) = 10.0f;
        PU(p, 0x16D4, s32) = 450;
        PU(p, 0x16D8, s32) = 6000;
        PU(p, 0x16D0, s32) = 750;
        PU(p, 0x16E0, s32) = 3600;
        PU(p, 0x16E4, s32) = 140;
    }
    p->c.a.radius = 3.0f;
    p->c.a.height = 17.0f;
    p->c.hp = p->c.hpMax;
    p->c.hearThreshold = 12;
    PU(p, 0x1720, u8 *) = D_003D7730;
    PU(p, 0x1724, u8 *) = D_003D7760;
    PU(p, 0x16AC, u8 *) = D_003D73F0;
    PU(p, 0x16B0, u8 *) = D_003D7530;
    PU(p, 0x1734, u8 *) = D_0047A928;
    PU(p, 0x1694, f32) = 8.0f;
    PU(p, 0x169C, f32) = 1.5f;
    PU(p, 0x1698, f32) = 12.0f;
    PU(p, 0x16A0, f32) = 1.5f;
    m = p->c.motion;
    VCALL(m, 0x34, void (*)(void *, s32))(m, 0);
}
