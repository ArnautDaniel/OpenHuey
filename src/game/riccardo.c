/* Riccardo: the Pursuer of the later chapters (kind 4, vtable 0x46F6B0; code 0x2D7A60..0x2DC660).
 * His object is a Pursuer with 0x40 bytes more (0x1840; +0x1824 a table of his). He has a rage
 * (mode 2, +0x16B8) at the top threat level, his own reactions to cries, and his own
 * behaviours. See pursuer.h. */
#include "common.h"
#include "pursuer.h"
#include "progress.h"
#include "sce/libvu0.h"

extern void *D_0046F6B0[];

/* 0x46F69C: nothing (0) */
s32 func_002D7A60(void) {
    return 0;
}

/* vtable +0x8: destructor */
Pursuer *func_002D7A70(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = D_0046F6B0;
        if (p != NULL) {
            Pursuer_DestroyBase(p);
        }
        if ((s16)flags > 0) {
            func_00124E40(&p->c.a);
        }
    }
    return p;
}

/* vtable +0x200: done at the door unless it has a side (+0x104), then the Pursuer's */
void func_002D7CE0(Pursuer *p) {
    if (p->c.unk104[0] == 0) {
        PURSUER_STEP_DONE(p) = 1;
        return;
    }
    func_0028D6E0(p);
}

/* vtable +0xE4: at a door he breaks (func_00178980) while opening or attacking it: use and
   damage it (func_00178DB0 / func_00178A90) and change room through it (vtable +0x28) */
void func_002D7D20(Pursuer *p, s32 exit) {
    Progress *pr;

    if (!(func_00178980(gProgress, p->c.a.room, exit) & 0xFF)) {
        return;
    }
    switch (PU(p, 0x175C, s32)) {
    case 0xC:
    case 0x29:
    case 0xD:
        pr = gProgress;
        func_00178DB0(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
        func_00178A90(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
        VCALL(p, 0x28, void (*)(Pursuer *, s32, s32, s32))(p, VCALL(D_0044E568, 0x28, s32 (*)(VObject *, s32))(D_0044E568, exit), 0, 0);
        break;
    }
}

/* vtable +0xF0: nothing */
void func_002D7E10(Pursuer *p, s32 exit) {
}

/* vtable +0x138: the hit points of an attack entry (`e`: +0 animation, +4 / +8 bones); his
   grab 0xE05 is at Fiona herself */
void func_002D8120(Pursuer *p, s32 *e, f32 *a, f32 *b) {
    if (e[0] == 0xE05) {
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

extern u8 D_00414D50[], D_00414D90[], D_00414DC0[], D_00414DF0[], D_00414E20[], D_00414E40[],
    D_00414E58[], D_00414E68[], D_00414E80[], D_00414EB0[], D_00414EC0[], D_00414EE0[],
    D_00414F10[], D_00414F40[], D_00414F70[], D_00414FA0[], D_00414FB0[];
extern u8 D_00415150[], D_004151C0[], D_00415200[], D_00415250[], D_00415280[], D_004152B0[],
    D_004152D0[], D_00415340[], D_00415360[], D_004153A0[], D_004153B0[], D_004153F0[],
    D_00415420[], D_00415450[], D_004154B0[], D_004154E0[], D_004154F0[];

/* his attack tables for situations 0..16; the second set when gProgress+0x30 bit 0x8000 */
static u8 *const sAttackTables[2][17] = {
    { D_00414D50, D_00414DC0, D_00414D90, D_00414DF0, D_00414E20, D_00414E40, D_00414E58,
      D_00414E68, D_00414E80, D_00414EB0, D_00414EC0, D_00414EE0, D_00414F10, D_00414F40,
      D_00414FA0, D_00414FB0, D_00414F70 },
    { D_00415150, D_00415200, D_004151C0, D_00415250, D_00415280, D_004152B0, D_004152D0,
      D_00415340, D_00415360, D_004153A0, D_004153B0, D_004153F0, D_00415420, D_00415450,
      D_004154E0, D_004154F0, D_004154B0 },
};

/* vtable +0x130: the attack table for a situation */
void func_002D8210(Pursuer *p, s8 situation) {
    s32 alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;

    PU(p, 0x1718, u8 *) = sAttackTables[alt][(u32)situation < 17 ? situation : 0];
}

extern const PTMF D_00415670;
void func_002DB480(Pursuer *p);

/* vtable +0x264: the next behaviour; his own (func_002DB480) unless at threat level 5, then the
   Pursuer's */
void func_002DB7F0(Pursuer *p) {
    s32 next;

    if (AT(gProgress, 0x7B8, u8) == 5) {
        func_002961D0(p);
        return;
    }
    next = PU(p, 0x1758, s32);
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_00415670);
    PU(p, 0x1758, s32) = -1;
    if (next == -2) {
        PU(p, 0x16F3, u8) = 0;
        PU(p, 0x1758, s32) = -2;
    } else if (PU(p, 0x16F3, u8) == 1) {
        PU(p, 0x16F3, u8) = 0;
        VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1C);
    } else if (next != -1) {
        VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, next);
    }
    func_002DB480(p);
}

/* vtable +0x31C: his rage (mode 2, +0x16B8) on, with a roar (sound 0x26), or off */
void func_002DB900(Pursuer *p, s32 on) {
    if (!on) {
        PU(p, 0x16B8, s32) = 0;
        return;
    }
    PU(p, 0x16B8, s32) = 2;
    func_0029D410(p, 0x26, 7, 0, 0, NULL);
}

/* vtable +0x310 / +0x30C */
s32 func_002DB950(Pursuer *p) {
    return 0x11;
}

s32 func_002DB960(Pursuer *p) {
    return 0x10;
}

/* vtable +0x320: the Pursuer's choice (func_00297B00), but 1 for 0 / 3 unless +0xE0 is set */
s32 func_002DB990(Pursuer *p) {
    s32 r = func_00297B00(p);

    if (p->c.unkE0 == 0 && (r == 3 || r == 0)) {
        r = 1;
    }
    return r;
}

/* vtable +0x324: the Pursuer's slow walk (func_00297AC0), 0x200 for 0x204 */
s32 func_002DB9E0(Pursuer *p) {
    s32 r = func_00297AC0(p);

    return r == 0x204 ? 0x200 : r;
}

void func_002D7E20(Pursuer *p, Character *who);
void func_002DBD70(Pursuer *p);

/* vtable +0x30: his frame update. On screen: into his rage at threat level 5 while chasing and
   idle (vtable +0x31C), out of it below; a cry heard from Fiona or Hewie makes him react
   (func_002D7E20); the behaviour step, stance and voice; a snort (sound 0x24) at the key of
   animation 0x600; func_002DBD70. Off screen the behaviour step and the off-screen move */
void func_002DC070(Pursuer *p) {
    PTMF *st = (PTMF *)((u8 *)p + 0x174C);

    Stalker_ThinkStart(p);
    if (func_00217510(p) != 0) {
        func_00296FC0(p);
        if (PU(p, 0x16B8, s32) == 2) {
            if (AT(gProgress, 0x7B8, u8) != 5) {
                VCALL(p, 0x31C, void (*)(Pursuer *, s32))(p, 0);
            }
        } else if (AT(gProgress, 0x7B8, u8) == 5 && PU(p, 0x16C8, u8) == 0 && p->c.moveMode == 0) {
            VCALL(p, 0x31C, void (*)(Pursuer *, s32))(p, 1);
        }
        switch (func_0029B4B0(p) & 0xFF) {
        case 1:
            func_002D7E20(p, gCharPlayer);
            break;
        case 2:
            func_002D7E20(p, gCharPartner);
            break;
        }
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        VCALL(p, 0x110, void (*)(Pursuer *))(p);
        if (p->c.unk14D0 <= 0 || p->c.unk14D0 == 5) {
            func_0029D4C0(p, -1);
        }
        if (MOTION_ANIM(p) == 0x600 && (func_001F4770(p->c.motion, 0, 0, 1) & 0xFF & 1)) {
            func_0029D410(p, 0x24, 5, 0, 0, NULL);
        }
        func_00213E30(p);
        func_002DBD70(p);
        func_0029E210(p);
    } else {
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        func_0029D7F0(p);
    }
    Stalker_ThinkEnd(p);
}

extern u8 D_004148A0[], D_004148E0[], D_00414A30[], D_00414A40[], D_00414CD0[], D_00414D00[],
    D_00415060[], D_004150B0[], D_004150D0[], D_00415118[], D_00415130[], D_004155E0[],
    D_00415630[], D_00415650[], D_0047AC30[];

/* vtable +0xF4: his setup over the Pursuer's (func_0029FB20): his tables and stats (different
   when gProgress+0x30 bit 0x8000 is set) */
void func_002DC4E0(Pursuer *p) {
    void *m;

    func_0029FB20(p);
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        p->c.hpMax = 125;
        PU(p, 0x171C, u8 *) = D_00414A40;
        PU(p, 0x1730, u8 *) = D_004155E0;
        PU(p, 0x1740, u8 *) = D_00415630;
        PU(p, 0x173C, u8 *) = D_004150D0;
        PU(p, 0x1748, u8 *) = D_00415650;
        PU(p, 0x1824, u8 *) = D_00415130;
        PU(p, 0x16DC, s32) = 50;              /* Hewie bite tolerance */
        PU(p, 0x16E8, f32) = 10.0f;
        PU(p, 0x16D4, s32) = 540;
        PU(p, 0x16D8, s32) = 1800;
        PU(p, 0x16D0, s32) = 900;
        PU(p, 0x16E0, s32) = 9000;
        PU(p, 0x16E4, s32) = 90;
    } else {
        p->c.hpMax = 100;
        PU(p, 0x171C, u8 *) = D_00414A40;
        PU(p, 0x1730, u8 *) = D_00415060;
        PU(p, 0x1740, u8 *) = D_004150B0;
        PU(p, 0x173C, u8 *) = D_004150D0;
        PU(p, 0x1748, u8 *) = D_00415118;
        PU(p, 0x1824, u8 *) = D_00415130;
        PU(p, 0x16DC, s32) = 50;
        PU(p, 0x16E8, f32) = 15.0f;
        PU(p, 0x16D4, s32) = 300;
        PU(p, 0x16D8, s32) = 1800;
        PU(p, 0x16D0, s32) = 1200;
        PU(p, 0x16E0, s32) = 9000;
        PU(p, 0x16E4, s32) = 90;
    }
    p->c.a.radius = 3.0f;
    p->c.a.height = 17.0f;
    p->c.hp = p->c.hpMax;
    p->c.hearThreshold = 12;
    PU(p, 0x1714, u8 *) = D_004148A0;
    PU(p, 0x1720, u8 *) = D_00414CD0;
    PU(p, 0x1724, u8 *) = D_00414D00;
    PU(p, 0x16AC, u8 *) = D_004148E0;
    PU(p, 0x16B0, u8 *) = D_00414A30;
    PU(p, 0x1734, u8 *) = D_0047AC30;
    PU(p, 0x1694, f32) = 8.0f;
    PU(p, 0x169C, f32) = 1.5f;
    PU(p, 0x1698, f32) = 12.0f;
    PU(p, 0x16A0, f32) = 1.5f;
    m = p->c.motion;
    VCALL(m, 0x30, void (*)(void *))(m);
}

extern const PTMF D_00415788;

/* the end of his lunge animation: at threat level 5 it leads straight into attack 5;
   otherwise it ends the step */
static inline void Riccardo_LungeEnd(Pursuer *p) {
    func_00125A10(&p->c);
    if (AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        if (AT(gProgress, 0x7B8, u8) != 5) {
            PURSUER_STEP_DONE(p) = 1;
            PURSUER_STEP_NEXT(p) = 1;
            return;
        }
        PU(p, 0x1728, s32) = 5;
        Actor_SetState(&p->c.a, &D_00415788);
        p->c.moveMode = 8;
        func_0028B970(p);
    }
}

/* state: the lunge (see Riccardo_LungeEnd) */
void func_002D85D0(Pursuer *p) {
    Riccardo_LungeEnd(p);
}

extern const PTMF D_00415778;

/* start of the lunge: finish the current walk, then animation 0x1300 in state func_002D85D0 */
void func_002D8690(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    func_00297B40(p, 0x1300, 0);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_00415778);
    Riccardo_LungeEnd(p);
}

extern const PTMF D_00415768;

/* state: after a blow: his blows left (+0x1624) count down; another while he may go for his
   target and it's in reach and in front (within 90 degrees): Fiona seen within 100 (not at
   threat level 5), Hewie heard within 30. Otherwise the step ends */
void func_002D8AC0(Pursuer *p) {
    Character *t;
    f32 a;

    PU(p, 0x1624, s32)--;
    if (PU(p, 0x1624, s32) <= 0 || !(func_00283870(p) & 0xFF)) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
        return;
    }
    t = p->target;
    if (t != gCharPlayer) {
        if (PU(p, 0x1545, u8) == 0 || !(PU(p, 0x158C, f32) <= 30.0f)) {
            PURSUER_STEP_DONE(p) = 1;
            PURSUER_STEP_NEXT(p) = 1;
            return;
        }
    } else if (PU(p, 0x1544, u8) == 0 || !(PU(p, 0x1588, f32) <= 100.0f) || AT(gProgress, 0x7B8, u8) == 5) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
        return;
    }
    if (!(func_002E2D00(func_001244D0(&p->c.a, t->a.pos) - p->c.a.angle[1]) <= 0.0f)) {
        a = func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
    } else {
        a = -func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
    }
    if (!(a <= 0x1.921fb6p+0f /* 90 degrees */)) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
        return;
    }
    PU(p, 0x1760, u8) = 0;
    Actor_SetState(&p->c.a, &D_00415768);
}

extern const PTMF D_00415758;

/* state: a blow: at its hit key, if he may go for his target and it's within the reach of the
   blow (+0x171C entry +0x104, 0x24 bytes: +0xC reach), it lands (func_00178070 kind 1 with the
   entry's damage); at the animation's end, on to the next (func_002D8AC0) */
void func_002D8CB0(Pursuer *p) {
    if ((func_001F4770(p->c.motion, 0, -1, 1) & 0xFF & 2) && func_00283870(p) != 0) {
        u8 *e = PU(p, 0x171C, u8 *) + p->c.unk104[0] * 0x24;

        if (func_00124490(&p->c.a, p->target->a.pos) < AT(e, 0xC, f32)) {
            func_00178070(gProgress, *(u8 *)&p->c.a.slot, 1, AT(e, 0x10, u8), AT(e, 0x12, u16), AT(e, 0x4, s16), AT(e, 0x14, f32));
        }
    }
    if (AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        PU(p, 0x178C, s32) = 0;
        p->c.unk100 = 0;
        Actor_SetState(&p->c.a, &D_00415758);
        func_002D8AC0(p);
    }
    func_00125A10(&p->c);
}

extern const f32 D_004156C0[6], D_004156E0[6];
extern const PTMF D_004156F8;
void func_002DA120(Pursuer *p);

/* start of a flurry: how many blows (+0x1624, 1..7) by a roll against his cumulative chances
   (D_004156C0, or D_004156E0 when gProgress+0x30 bit 0x8000); then func_002DA120. When he may
   not go for his target: action 0x17 instead */
void func_002DA4C0(Pursuer *p) {
    const f32 *chance;
    f32 roll;

    if (!(func_00283870(p) & 0xFF)) {
        PU(p, 0x1624, s32) = 0;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x17);
        p->c.unk104[0] = 0;
        func_00125A10(&p->c);
        return;
    }
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    chance = (AT(gProgress, 0x30, u32) & 0x8000) ? D_004156E0 : D_004156C0;
    roll = 100.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550);
    PU(p, 0x1624, s32) = 0;
    while (PU(p, 0x1624, s32) < 6 && !(roll <= chance[PU(p, 0x1624, s32)])) {
        PU(p, 0x1624, s32)++;
    }
    PU(p, 0x1624, s32)++;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_004156F8);
    func_002DA120(p);
}
