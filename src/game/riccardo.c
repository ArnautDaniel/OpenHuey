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

/* a blow: at its hit key, if he may go for his target and it's within the reach of the blow
   (+0x171C entry +0x104, 0x24 bytes: +0xC reach), it lands (func_00178070 kind 1 with the
   entry's damage); at the animation's end, on to the next (func_002D8AC0) */
static inline void Riccardo_Blow(Pursuer *p) {
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

/* state: a blow (see Riccardo_Blow) */
void func_002D8CB0(Pursuer *p) {
    Riccardo_Blow(p);
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

/* a cry from `who` (Fiona or Hewie): a hit effect on them. In his grab (0x1000, unless its entry
   is kind 6) at a random one of four of their bones (motion vtable +0x84..+0x90); in his blows
   0xE05 / 0x1A01 a large one at their bone of motion vtable +0x94 */
void func_002D7E20(Pursuer *p, Character *who) {
    HitEffectParams hp;
    u8 *wm;
    s32 bone;

    if (PU(p, 0x175C, s32) == 0x1000) {
        if (AT(PU(p, 0x171C, u8 *) + p->c.unk104[0] * 0x24, 0x10, u8) == 6) {
            return;
        }
        hp.kind = who == gCharPlayer ? 0 : 1;
        wm = who->motion;
        switch ((u32)(4.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550))) {
        case 0:
            bone = VCALL(wm, 0x84, s32 (*)(void *))(wm);
            break;
        case 1:
            bone = VCALL(wm, 0x88, s32 (*)(void *))(wm);
            break;
        case 2:
            bone = VCALL(wm, 0x8C, s32 (*)(void *))(wm);
            break;
        default:
            bone = VCALL(wm, 0x90, s32 (*)(void *))(wm);
            break;
        }
        hp.big = 0.0f;
    } else {
        hp.kind = 0;
        if (MOTION_ANIM(p) != 0xE05 && MOTION_ANIM(p) != 0x1A01) {
            return;
        }
        wm = who->motion;
        bone = VCALL(wm, 0x94, s32 (*)(void *))(wm);
        hp.big = 1.0f;
    }
    {
        f32 pos[4] __attribute__((aligned(16)));

        sceVu0CopyVector(pos, func_0017CE80(AT(who->motion, 0x810, u8 *), bone) + 0xC);
        hp.pos[0] = pos[0];
        hp.pos[1] = pos[1];
        hp.pos[2] = pos[2];
        hp.pos[3] = pos[3];
    }
    HitEffect_Spawn(&hp);
}

/* his attack for the distance to `who` (NULL: his target): a 0..100 roll under the distance's
   chance (100 within 10, 50 within 30, 25 within 60, 15 within 90, 10 within 150, never further)
   attacks; on Hewie action 4; on Fiona a second roll picks 6, 4 or 3 by the chances for the
   distance (6 only below threat level 4). 0xFF: none */
s32 func_002D8840(Pursuer *p, Character *who) {
    f32 d, any, c3 = 0.0f, c4 = 0.0f, c6 = 0.0f;
    f32 roll;
    VObject *rnd;

    if (who == NULL) {
        who = p->target;
    }
    d = func_00124490(&p->c.a, who->a.pos);
    any = 10.0f;
    if (d < 10.0f) {
        any = 100.0f;
        c3 = 100.0f;
    } else if (d < 30.0f) {
        any = 50.0f;
        if (AT(gProgress, 0x7B8, u8) < 4) {
            c6 = 50.0f;
        }
        c4 = 100.0f;
    } else if (d < 60.0f) {
        any = 25.0f;
        if (AT(gProgress, 0x7B8, u8) < 4) {
            c6 = 75.0f;
        }
        c4 = 100.0f;
    } else if (d < 90.0f) {
        any = 15.0f;
        if (AT(gProgress, 0x7B8, u8) < 4) {
            c6 = 75.0f;
        }
        c4 = 100.0f;
    } else if (d < 150.0f) {
        if (AT(gProgress, 0x7B8, u8) < 4) {
            c6 = 100.0f;
        } else {
            c4 = 100.0f;
        }
    } else {
        any = 0.0f;
    }
    rnd = D_0044E550;
    if (!(100.0f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd) < any)) {
        return 0xFF;
    }
    if (who != gCharPlayer) {
        return 4;
    }
    roll = 100.0f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);
    if (roll <= c6) {
        return 6;
    }
    if (roll <= c4) {
        return 4;
    }
    if (roll <= c3) {
        return 3;
    }
    return 0xFF;
}

extern const PTMF D_00415708, D_00415718, D_00415728;
void func_002D8DF0(Pursuer *p);
void func_002D9500(Pursuer *p);

/* a blow of the flurry: on Fiona, in front of her (func_002175B0) entry 0 or 3, else from behind
   8 or 9 (one blow only); on Hewie entry 1 if he's in front, else give up (action 0x13). A blow
   of kind 6 is struck at once (Riccardo_Blow); others first close in: on Hewie func_002D8DF0, on
   Fiona func_002D9500 */
void func_002DA120(Pursuer *p) {
    u8 *e;

    if (p->target != gCharPartner) {
        if (!(func_002175B0(&p->c.a, &p->target->a) & 0xFF)) {
            p->c.unk104[0] = 100.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550) <= 50.0f ? 9 : 8;
            PU(p, 0x1624, s32) = 1;
        } else {
            p->c.unk104[0] = 100.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550) <= 50.0f ? 0 : 3;
        }
    } else if (func_002175B0(&p->c.a, &p->target->a) & 0xFF) {
        p->c.unk104[0] = 1;
    } else {
        PU(p, 0x1624, s32) = 0;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x13);
        PU(p, 0x1728, s32) = 2;
        PU(p, 0x172C, u8) = 0;
        func_00125A10(&p->c);
        return;
    }
    e = PU(p, 0x171C, u8 *) + p->c.unk104[0] * 0x24;
    func_00297B40(p, AT(e, 0x0, s32), 0);
    p->c.unk100 = -1;
    if (AT(e, 0x10, u8) == 6) {
        Actor_SetState(&p->c.a, &D_00415708);
        Riccardo_Blow(p);
    } else if (p->target == gCharPartner) {
        Actor_SetState(&p->c.a, &D_00415718);
        func_002D8DF0(p);
    } else {
        Actor_SetState(&p->c.a, &D_00415728);
        func_002D9500(p);
    }
}

extern const PTMF D_00415680, D_00415690;
void func_002DA6B0(Pursuer *p);

/* his behaviour: Fiona as the target. Out of sight of her, head for her (vtable +0xB0).
   Otherwise the pending action (0x1C rumbles the pad), or: further than 100 hold off (6);
   closer, seen by her and within +0x17C0 (60) come on (1); else back off (5) or hold off (6) by a
   roll against his table +0x1824 (+0x14 chance, +0 / +4 waits). Then his chase (func_002DA6B0) */
void func_002DB480(Pursuer *p) {
    s32 next;

    p->target = gCharPlayer;
    PU(p, 0x16F6, u8) = 1;
    if (PU(p, 0x1544, u8) == 0) {
        VCALL(p, 0xB0, void (*)(Pursuer *))(p);
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_00415680);
        PU(p, 0x1758, s32) = -1;
        return;
    }
    PU(p, 0x17C0, f32) = 60.0f;
    next = PU(p, 0x1758, s32);
    if (next == -1) {
        f32 roll = 100.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550);
        Character *t;

        if (!(PU(p, 0x1588, f32) <= 100.0f)) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
            PU(p, 0x162C, s32) = AT(PU(p, 0x1824, u8 *), 0xC, s32);
        } else if (t = p->target,
                   (func_00218300(p, &t->a, &p->c.a, t->a.angle[1], PU(p, 0x1580, f32), 0x1.921fb6p+1f /* 180 degrees */) & 0xFF) &&
                   PU(p, 0x1588, f32) <= PU(p, 0x17C0, f32)) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
            PU(p, 0x162C, s32) = 0;
        } else if (roll <= AT(PU(p, 0x1824, u8 *), 0x14, f32)) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
            PU(p, 0x162C, s32) = AT(PU(p, 0x1824, u8 *), 0x0, s32);
        } else {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
            PU(p, 0x162C, s32) = AT(PU(p, 0x1824, u8 *), 0x4, s32);
        }
    } else if (next != -2) {
        if (next == 0x1C) {
            VCALL(D_0044E7A8, 0x18, void (*)(VObject *, s32, s32, s32))(D_0044E7A8, 3, 0x80, 0x1E);
        }
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, PU(p, 0x1758, s32));
    }
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    PU(p, 0x1630, s32) = AT(PU(p, 0x1824, u8 *), 0x10, s32);
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_00415690);
    PU(p, 0x1758, s32) = -1;
    func_002DA6B0(p);
}

extern void func_00211A90(Actor *a, f32 dist, f32 *out);                       /* a point ahead */
extern void func_00211A30(f32 *out, const f32 *from, f32 angle, f32 dist);   /* from + dir * dist */
extern const f32 D_00415660[4];

/* where the blow of his current animation hits the floor (into `out`; 0 if nowhere). His hammer
   swings 0xE00 / 0xE04: straight ahead along the nav mesh to the first edge he can't cross
   (flags 0x80 or no neighbour), stopping at walls (0x4000); his slams 0x2301 / 0x2303 / 0xE06 /
   0x1602: under the hammer head (bone 0x31, offset D_00415660) */
s32 func_002DBA90(Pursuer *p, f32 *out) {
    s32 swing;

    switch (MOTION_ANIM(p)) {
    case 0x2303:
    case 0x2301:
    case 0xE06:
    case 0x1602:
        swing = 0;
        break;
    case 0xE04:
    case 0xE00:
        swing = 1;
        break;
    default:
        return 0;
    }
    if (swing) {
        u32 tri = p->c.a.navTri;
        f32 ahead[4] __attribute__((aligned(16)));
        void *nav;

        func_00211A90(&p->c.a, 1000.0f, ahead);
        nav = D_0044E570;
        for (;;) {
            u8 *t = tri < AT(nav, 0x8, u32) && AT(nav, 0x4, u8 *) != NULL ? AT(nav, 0x4, u8 *) + tri * 0x50 : NULL;
            s32 edge;
            u32 next;

            if ((t != NULL ? AT(t, 0x3C, u32) : NAV_BAD_TRI_FLAGS) & 0x4000) {
                return 0;
            }
#ifdef HG_NATIVE
            if (t == NULL) {
                return 0;
            }
#endif
            edge = VCALL(nav, 0x20, s32 (*)(void *, u32, f32 *, f32 *))(nav, tri, p->c.a.pos, ahead);
            if (edge == 3) {
                func_00211A30(ahead, ahead, p->c.a.angle[1], 1000.0f);
                continue;
            }
            if (edge == 4) {
                return 0;
            }
            next = AT(t, 0x30 + edge * 4, u32);
            if (next == (u32)-1 || (AT(t, 0x3C, u32) & 0x80)) {
                VCALL(nav, 0x24, void (*)(void *, u32, f32 *, f32 *, f32 *))(nav, tri, out, p->c.a.pos, ahead);
                out[1] += 15.0f;
                return 1;
            }
            tri = next;
        }
    } else {
        f32 head[4] __attribute__((aligned(16)));
        f32 off[4] __attribute__((aligned(16)));
        f32 m[4][4] __attribute__((aligned(16)));
        u32 tri;

        sceVu0CopyVector(head, func_0017CE80(MOTION_AT(p, 0x810, u8 *), 0x31) + 0xC);
        off[0] = D_00415660[0];
        off[1] = D_00415660[1];
        off[2] = D_00415660[2];
        off[3] = D_00415660[3];
        sceVu0CopyMatrix(m, (f32 (*)[4])func_0017CE80(MOTION_AT(p, 0x810, u8 *), 0x31));
        func_002E2DA0(out, m, off);
        sceVu0AddVector(out, out, head);
        tri = func_00124480(&p->c.a, out, 0x20008);
        if (tri == (u32)-1) {
            return 0;
        }
        VCALL(D_0044E570, 0x14, void (*)(void *, u32, f32 *))(D_0044E570, tri, out);
        return 1;
    }
}

/* the impact (0x80 bytes, vtable 0x479600) and the debris cloud (0xF70 bytes, vtable 0x47A710)
   of his hammer */
extern void *D_00479600[], *D_0047A710[];

static inline void Impact_Init(void **obj) {
    obj[0] = D_00479600;
}

static inline void Debris_Init(void **obj) {
    obj[0] = D_0047A710;
    obj[0xC10 / 4] = D_00469D00;
    ((s32 *)obj)[0xC14 / 4] = -1;
    obj[0xC10 / 4] = D_0046FC30;
}

typedef struct {
    s32 rgb[3];    /* 0x80, 0x50, 0x40: brown */
    f32 pos[3];
} DebrisParams;

/* a debris cloud where a blow hits the floor */
static void Riccardo_Debris(const f32 *at) {
    u8 *mgr = D_0044E578;
    s32 slot = Effect_New(mgr, 0xF70, Debris_Init);
    DebrisParams dp;

    dp.rgb[0] = 0x80;
    dp.rgb[1] = 0x50;
    dp.rgb[2] = 0x40;
    dp.pos[0] = at[0];
    dp.pos[1] = at[1];
    dp.pos[2] = at[2];
    func_002D6090(mgr, slot, &dp);
}

/* at the impact key of his animation (0x20): a jolt (the 0x80 effect, kind 2) and a noise of
   0x40 where he stands (gProgress+0x798); a debris cloud where his slam or swing hits the floor
   (func_002DBA90), or for his grab 0x1A01 a hit effect on Fiona */
void func_002DBD70(Pursuer *p) {
    u8 *mgr;
    s32 jolt[4] = { 2, 0, 0, 0 };   /* kind 2 */

    if (!(func_001F4770(p->c.motion, 0, -1, 1) & 0xFF & 0x20)) {
        return;
    }
    mgr = D_0044E578;
    func_002D6090(mgr, Effect_New(mgr, 0x80, Impact_Init), jolt);
    func_002A8440((u8 *)gProgress + 0x798, 0x40, p->c.a.room, p->c.a.navTri, 0xFFFF);
    switch (MOTION_ANIM(p)) {
    case 0x2303:
    case 0x2301:
    case 0x1602: {
        f32 at[4] __attribute__((aligned(16)));

        if (func_002DBA90(p, at) & 0xFF) {
            Riccardo_Debris(at);
        }
        break;
    }
    case 0x1A01:
        func_002D7E20(p, gCharPlayer);
        break;
    }
}

extern f32 func_00211910(const f32 *from, const f32 *to, const f32 *pt);   /* pt's distance from the line */
extern const PTMF D_00415748;

/* state: his blow at Hewie. At the hit key, if he may go for him, hears him within 30, and Hewie
   is in front within 30 degrees: unless Fiona stands in the way (within 4 of the line to Hewie,
   and no further), it's Hewie's blow (func_002D8840: on a hit func_00178070 kind 2 with the entry,
   else debris where it lands); in her way it's hers (6: the heavy entry 8 with its stun and
   debris, 4: a stun three times in four, 3; none: debris only). Until the hit he turns to
   Hewie; at the animation's end, on (func_002D8AC0); now and then a grunt (sound 0x15) */
void func_002D8DF0(Pursuer *p) {
    f32 fiona[4] __attribute__((aligned(16)));
    f32 hewie[4] __attribute__((aligned(16)));

    sceVu0CopyVector(fiona, gCharPlayer->a.pos);
    sceVu0CopyVector(hewie, gCharPartner->a.pos);
    if (func_001F4770(p->c.motion, 0, -1, 1) & 0xFF & 2) {
        f32 a;

        if (func_00283870(p) == 0 || PU(p, 0x1545, u8) != 1 ||
            (func_002175B0(&p->c.a, &gCharPartner->a) & 0xFF) != 1) {
            goto done;
        }
        if (!(func_002E2D00(func_001244D0(&p->c.a, hewie) - p->c.a.angle[1]) <= 0.0f)) {
            a = func_002E2D00(func_001244D0(&p->c.a, hewie) - p->c.a.angle[1]);
        } else {
            a = -func_002E2D00(func_001244D0(&p->c.a, hewie) - p->c.a.angle[1]);
        }
        if (!(a < 0x1.0c1524p-1f /* 30 degrees */) || !(PU(p, 0x158C, f32) < 30.0f)) {
            goto done;
        }
        {
            u8 *e = PU(p, 0x171C, u8 *) + p->c.unk104[0] * 0x24;
            f32 at[4] __attribute__((aligned(16)));
            u32 k;

            if (!(func_00211910(p->c.a.pos, hewie, fiona) <= 4.0f) ||
                func_00124490(&p->c.a, hewie) < func_00124490(&p->c.a, fiona)) {
                /* Hewie */
                if ((func_002D8840(p, gCharPartner) & 0xFF) != 0xFF) {
                    func_00178070(gProgress, *(u8 *)&p->c.a.slot, 2, AT(e, 0x10, u8), AT(e, 0x12, u16), 0, AT(e, 0x14, f32));
                } else if (func_002DBA90(p, at) & 0xFF) {
                    Riccardo_Debris(at);
                }
            } else {
                /* Fiona in the way */
                s32 stun = 0;

                k = func_002D8840(p, gCharPlayer) & 0xFF;
                switch (k) {
                case 6:
                    p->c.unk104[0] = 8;
                    e = PU(p, 0x171C, u8 *) + 0x120;
                    stun = AT(e, 0x4, s16);
                    /* fallthrough */
                case 0xFF:
                    if (func_002DBA90(p, at) & 0xFF) {
                        Riccardo_Debris(at);
                    }
                    break;
                case 4:
                    if (100.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550) < 75.0f) {
                        stun = -0x8000;
                    }
                    break;
                }
                if (k != 0xFF) {
                    func_00178070(gProgress, *(u8 *)&p->c.a.slot, 1, k, AT(e, 0x12, u16), stun, AT(e, 0x14, f32));
                }
            }
        }
    done:
        p->c.unk100 = 0;
    }
    if (p->c.unk100 == -1) {
        Character *t = gCharPartner != NULL ? gCharPartner : p->target;
        f32 h = func_001244D0(&p->c.a, t->a.pos);

        func_002140A0(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    }
    if (AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        PU(p, 0x178C, s32) = 0;
        p->c.unk100 = 0;
        Actor_SetState(&p->c.a, &D_00415748);
        func_002D8AC0(p);
    }
    if ((func_001F4770(p->c.motion, 0, 0, 1) & 0xFF & 1) &&
        100.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550) < 50.0f) {
        func_0029D410(p, 0x15, 7, 0, 0, NULL);
    }
    func_00125A10(&p->c);
}

extern const PTMF D_00415738;

/* is `pt` clear of the swing at `at` (not within 4 of the line to it, or no nearer than it) */
static inline s32 Riccardo_OutOfLine(Pursuer *p, const f32 *at, const f32 *pt) {
    return !(func_00211910(p->c.a.pos, at, pt) <= 4.0f) || func_00124490(&p->c.a, at) < func_00124490(&p->c.a, pt);
}

/* state: his blow at Fiona. At the hit key he aims (+0x100 bits) if he may go for her, sees her
   in front within 30 degrees: 4 at her, 1 at a room object within 50 of her (D_0044E4D0 vtable
   +0x68) further than 20 from him, 2 at Hewie likewise. At the key the object takes the blow
   (its point to +0x1770) unless she or Hewie is in the way; Hewie takes it if he's in the way
   (func_002D8840: a hit of kind 2, else debris); then her: if she's lower than his waist the
   hammer passes over (debris), else func_002D8840 picks the hit (6: the heavy entry with its stun
   and debris, 4: a stun three times in four, 3; none: debris only). Until the key he turns to
   her; at the animation's end, on (func_002D8AC0); now and then a grunt (0x22 / 0x23) */
void func_002D9500(Pursuer *p) {
    f32 fiona[4] __attribute__((aligned(16)));
    f32 hewie[4] __attribute__((aligned(16)));
    f32 obj[4] __attribute__((aligned(16)));
    u32 aim;

    sceVu0CopyVector(fiona, gCharPlayer->a.pos);
    sceVu0CopyVector(hewie, gCharPartner->a.pos);
    if (func_001F4770(p->c.motion, 0, -1, 1) & 0xFF & 2) {
        f32 a;

        if (!(func_00283870(p) & 0xFF) || PU(p, 0x1544, u8) == 0 ||
            !(func_002175B0(&p->c.a, &gCharPlayer->a) & 0xFF)) {
            p->c.unk100 = 0;
        } else {
            if (!(func_002E2D00(func_001244D0(&p->c.a, fiona) - p->c.a.angle[1]) <= 0.0f)) {
                a = func_002E2D00(func_001244D0(&p->c.a, fiona) - p->c.a.angle[1]);
            } else {
                a = -func_002E2D00(func_001244D0(&p->c.a, fiona) - p->c.a.angle[1]);
            }
            if (!(a <= 0x1.0c1524p-1f /* 30 degrees */)) {
                p->c.unk100 = 0;
            } else {
                f32 d[4] __attribute__((aligned(16)));

                p->c.unk100 = 4;
                if (VCALL(D_0044E4D0, 0x68, s32 (*)(VObject *, f32 *, f32 *))(D_0044E4D0, fiona, obj) != 0) {
                    sceVu0SubVector(d, fiona, obj);
                    d[3] = 0.0f;
                    if (__builtin_sqrtf(sceVu0InnerProduct(d, d)) < 50.0f && !(func_00124490(&p->c.a, obj) <= 20.0f)) {
                        p->c.unk100 |= 1;
                    }
                }
                sceVu0SubVector(d, fiona, hewie);
                d[3] = 0.0f;
                if (__builtin_sqrtf(sceVu0InnerProduct(d, d)) < 50.0f && !(func_00124490(&p->c.a, hewie) <= 20.0f)) {
                    p->c.unk100 |= 2;
                }
            }
        }
    }
    aim = p->c.unk100;
    if (aim == 0) {
        PU(p, 0x1770, s32) = 0;
        PU(p, 0x1774, s32) = 0;
        PU(p, 0x1778, s32) = 0;
    } else if (aim == (u32)-1) {
        Character *t = gCharPlayer != NULL ? gCharPlayer : p->target;
        f32 h = func_001244D0(&p->c.a, t->a.pos);

        func_002140A0(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    } else {
        u8 *e = PU(p, 0x171C, u8 *) + p->c.unk104[0] * 0x24;
        f32 at[4] __attribute__((aligned(16))) = { 0.0f, 0.0f, 0.0f, 0.0f };

        if (aim & 1) {
            u32 tri = func_00124320(&p->c.a, obj, gCharPlayer->a.navTri, fiona, 0);

            if (tri == (u32)-1) {
                tri = func_00124480(&p->c.a, obj, 0);
            }
            if (tri != (u32)-1 && func_002187D0(p, tri, obj) != 0 &&
                p->c.a.pos[1] - obj[1] < 10.0f && !(p->c.a.pos[1] - obj[1] <= -15.0f) &&
                Riccardo_OutOfLine(p, obj, fiona) &&
                (!(func_00211910(p->c.a.pos, obj, hewie) <= 4.0f) ||
                 func_00124490(&p->c.a, obj) < func_00124490(&p->c.a, hewie) || PU(p, 0x1545, u8) == 0)) {
                sceVu0CopyVector((f32 *)((u8 *)p + 0x1770), obj);
                p->c.unk100 = 0;
                goto end;
            }
        }
        if ((p->c.unk100 & 2) && PU(p, 0x1545, u8) == 1 &&
            (func_002175B0(&p->c.a, &gCharPartner->a) & 0xFF) == 1 && Riccardo_OutOfLine(p, hewie, fiona)) {
            if ((func_002D8840(p, gCharPartner) & 0xFF) != 0xFF) {
                func_00178070(gProgress, *(u8 *)&p->c.a.slot, 2, AT(e, 0x10, u8), AT(e, 0x12, u16), 0, AT(e, 0x14, f32));
            } else if (func_002DBA90(p, at) & 0xFF) {
                Riccardo_Debris(at);
            }
            p->c.unk100 = 0;
        } else if (p->c.unk100 & 4) {
            if (func_00124490(&p->c.a, fiona) < 20.0f && fiona[1] + gCharPlayer->a.height < p->c.a.pos[1] + 5.0f) {
                /* she's below the swing */
                if (func_002DBA90(p, at) & 0xFF) {
                    Riccardo_Debris(at);
                }
            } else {
                u32 k = func_002D8840(p, gCharPlayer) & 0xFF;
                s32 stun = 0;

                switch (k) {
                case 6:
                    if (p->c.unk104[0] == 0) {
                        p->c.unk104[0] = 6;
                    } else if (p->c.unk104[0] == 3) {
                        p->c.unk104[0] = 7;
                    }
                    e = PU(p, 0x171C, u8 *) + p->c.unk104[0] * 0x24;
                    stun = AT(e, 0x4, s16);
                    /* fallthrough */
                case 0xFF:
                    if (func_002DBA90(p, at) & 0xFF) {
                        Riccardo_Debris(at);
                    }
                    break;
                case 4:
                    if (100.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550) < 75.0f) {
                        stun = -0x8000;
                    }
                    break;
                }
                if (k != 0xFF) {
                    func_00178070(gProgress, *(u8 *)&p->c.a.slot, 1, k, AT(e, 0x12, u16), stun, AT(e, 0x14, f32));
                }
            }
            p->c.unk100 = 0;
        }
    }
end:
    if (AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        PU(p, 0x178C, s32) = 0;
        p->c.unk100 = 0;
        p->c.unk104[0] = 0;
        Actor_SetState(&p->c.a, &D_00415738);
        func_002D8AC0(p);
    }
    if ((func_001F4770(p->c.motion, 0, 0, 1) & 0xFF & 1) &&
        100.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550) < 50.0f) {
        if (p->c.unk104[0] == 0) {
            func_0029D410(p, 0x22, 7, 0, 0, NULL);
        } else if (p->c.unk104[0] == 3) {
            func_0029D410(p, 0x23, 7, 0, 0, NULL);
        }
    }
    func_00125A10(&p->c);
}

/* does Fiona (the target) see him: within 180 degrees of her heading and his sight range */
static inline s32 Riccardo_Seen(Pursuer *p) {
    Character *t = p->target;

    return func_00218300(p, &t->a, &p->c.a, t->a.angle[1], PU(p, 0x1580, f32), 0x1.921fb6p+1f /* 180 degrees */) & 0xFF;
}

/* back off (5) or hold off (6) by a roll against his table +0x1824 (+0x14 chance; +0 / +4 waits) */
static void Riccardo_BackOrHold(Pursuer *p, f32 roll) {
    if (roll <= AT(PU(p, 0x1824, u8 *), 0x14, f32)) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
        PU(p, 0x162C, s32) = AT(PU(p, 0x1824, u8 *), 0x0, s32);
    } else {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
        PU(p, 0x162C, s32) = AT(PU(p, 0x1824, u8 *), 0x4, s32);
    }
}

extern u8 D_0047AC38[];

/* the threat-level chance of a lunge (attack table 0xA); 1 when he lunges */
static inline s32 Riccardo_Lunge(Pursuer *p) {
    u32 chance = func_00297290(p, (f32 *)D_0047AC38, 1);
    f32 roll = 100.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550);

    if (roll < (f32)chance) {
        VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, 0xA);
        func_00283C50(p);
        return 1;
    }
    return 0;
}

/* a waited-out back-off or hold-off: a lunge by the threat-level chance, else back or hold off
   again. 1 when he lunges */
static s32 Riccardo_StrikeOrWait(Pursuer *p) {
    f32 roll;

    if (Riccardo_Lunge(p)) {
        return 1;
    }
    roll = 100.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550);
    Riccardo_Seen(p);   /* the original asks, then backs or holds off the same either way */
    Riccardo_BackOrHold(p, roll);
    return 0;
}

extern const PTMF D_004156A0, D_004156B0;

/* his chase (from func_002DB480), as Debilitas's func_0012A4C0: he stalks Fiona, closing in (1),
   backing off (5) or holding off (6) for the waits from his table +0x1824, and lunges by the
   threat-level chance (every 60 frames while closing in, and when a wait runs out within 100).
   Seen by her while within +0x17C0 he comes straight on (1) a limited number of times
   (+0x1630, refilled from +0x1824 +0x10) */
void func_002DA6B0(Pursuer *p) {
    f32 near;

    if (ptmf_test(&p->c.a.state)) {
        ptmf_scall(p, &p->c.a.state);
    }
    switch (PU(p, 0x175C, s32)) {
    case 0x1A:
    case 0x19:
        if ((PU(p, 0x175C, s32) == 0x1A || AT(PU(p, 0x1720, u8 *) + p->c.unk104[0] * 8, 0x4, u8) != 0) &&
            !(PU(p, 0x1588, f32) <= 100.0f)) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
            PU(p, 0x162C, s32) = AT(PU(p, 0x1824, u8 *), 0xC, s32);
            PURSUER_STEP_DONE(p) = 0;
            PU(p, 0x16EF, u8) = 0;
        }
        if (PURSUER_STEP_DONE(p) == 1 || PU(p, 0x16EF, u8) == 1) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
            PURSUER_STEP_DONE(p) = 0;
            PU(p, 0x16EF, u8) = 0;
        }
        break;
    case 0x10:
    case 0x12:
    case 0xB:
    case 3:
    case 0x1C:
        if (PU(p, 0x16EF, u8) == 1) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
            PU(p, 0x16EF, u8) = 0;
            PURSUER_STEP_DONE(p) = 0;
        } else if (PURSUER_STEP_DONE(p) == 1) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 1: {
        f32 roll;

        if ((PU(p, 0x1780, u32) + 1) % 60 == 0 && Riccardo_Lunge(p)) {
            return;
        }
        if (!((((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF))) {
            u32 dir = func_00213FA0(p, gCharPlayer->a.pos, 0x1.0c1524p+0f /* 60 degrees */, 0x1.4f1a6ep+1f /* 150 degrees */) & 0xFF;

            if (dir != 0xFF) {
                p->c.unk104[0] = dir;
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 3);
            }
        }
        roll = 100.0f * VCALL(D_0044E550, 0x1C, f32 (*)(VObject *))(D_0044E550);
        if (PU(p, 0x1588, f32) < 0.0f) {
            if (func_00284440(p) & 0xFF) {
                break;
            }
            func_0029AF20(p);
            return;
        }
        if (!Riccardo_Seen(p) || !(PU(p, 0x1588, f32) <= PU(p, 0x17C0, f32) + 20.0f)) {
            Riccardo_BackOrHold(p, roll);
        } else if (PU(p, 0x1630, s32) > 0) {
            PU(p, 0x1630, s32)--;
        } else {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
            PU(p, 0x162C, s32) = AT(PU(p, 0x1824, u8 *), 0x8, s32);
        }
        break;
    }
    case 6:
        if (PU(p, 0x16EF, u8) != 0) {
            PU(p, 0x16EF, u8) = 0;
        }
        if (PU(p, 0x1588, f32) <= PU(p, 0x17C0, f32) && Riccardo_Seen(p) == 1) {
            if (PU(p, 0x1630, s32) > 0) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
                PU(p, 0x1630, s32) = AT(PU(p, 0x1824, u8 *), 0x10, s32);
                PU(p, 0x162C, s32) = 0;
            } else {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
                PU(p, 0x162C, s32) = AT(PU(p, 0x1824, u8 *), 0x8, s32);
            }
            break;
        }
        if (PU(p, 0x162C, s32) > 0 || !(PU(p, 0x1588, f32) < 100.0f)) {
            PU(p, 0x162C, s32)--;
            break;
        }
        if (Riccardo_StrikeOrWait(p)) {
            return;
        }
        break;
    case 5:
        if (PU(p, 0x16EF, u8) != 0) {
            PU(p, 0x16EF, u8) = 0;
        }
        if (PU(p, 0x1588, f32) <= PU(p, 0x17C0, f32) && Riccardo_Seen(p) == 1 && PU(p, 0x1630, s32) > 0) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
            PU(p, 0x1630, s32) = AT(PU(p, 0x1824, u8 *), 0x10, s32);
            PU(p, 0x162C, s32) = 0;
            break;
        }
        if (PU(p, 0x162C, s32) > 0) {
            if (PU(p, 0x1588, f32) <= 100.0f) {
                PU(p, 0x162C, s32)--;
            } else {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
                PU(p, 0x162C, s32) = AT(PU(p, 0x1824, u8 *), 0xC, s32);
            }
            break;
        }
        if (Riccardo_StrikeOrWait(p)) {
            return;
        }
        break;
    }
    if (PURSUER_STEP_NEXT(p) == 1 && PU(p, 0x175C, s32) != 0x12) {
        s32 d = func_002131A0();

        if (d != -1) {
            /* Fiona is hiding: go to the door */
            p->c.unk100 = d;
            p->c.unk104[0] = -1;
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x12);
        }
    }
    if (p->c.moveSub == 6 && gCharPlayer->moveMode != 3 && !(PU(p, 0x1588, f32) <= 0.0f)) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
    }
    if (PURSUER_STEP_NEXT(p) != 1) {
        return;
    }
    if (PU(p, 0x1544, u8) == 0) {
        VCALL(p, 0xB0, void (*)(Pursuer *))(p);
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_004156A0);
        PU(p, 0x1758, s32) = -1;
        PU(p, 0x162C, s32) = 0;
        return;
    }
    if (AT(gProgress, 0x7B8, u8) == 5) {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_004156B0);
        PU(p, 0x1758, s32) = -1;
        PU(p, 0x162C, s32) = 0;
        return;
    }
    near = p->target->moveMode == 3 ? func_00124490(&p->c.a, p->target->a.pos) : PU(p, 0x1588, f32);
    if (!(near < VCALL(p, 0x2F4, f32 (*)(Pursuer *))(p)) || near < 0.0f) {
        return;
    }
    if (near < func_002838E0(p) || near < 10.0f) {
        f32 a;

        if (!(func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]) <= 0.0f)) {
            a = func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
        } else {
            a = -func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
        }
        if (a < 0x1.921fb6p+1f * VCALL(p, 0x2EC, f32 (*)(Pursuer *))(p) / 180.0f &&
            func_002175B0(&p->c.a, &p->target->a) != 0) {
            VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, (s8)func_00283EF0(p));
            func_00283C50(p);
            PU(p, 0x162C, s32) = 0;
        }
    }
}
