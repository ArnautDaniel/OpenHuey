/* Daniella: the Pursuer of the middle chapters (kind 3, vtable 0x46BBB0; code 0x20C3A0..0x20D860).
 * She mostly runs the Pursuer's own behaviour; her class overrides her setup and tables, the
 * bone positions of her attacks, and a hit effect at her hand. The object is a plain Pursuer
 * (0x1800 bytes). See pursuer.h. */
#include "common.h"
#include "pursuer.h"
#include "progress.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "actor.h"
#include "ptmf.h"
#include "memcard.h"
#include "navmesh.h"
#include "game.h"
#include "daniella.h"
#include "fiona.h"
#include "model.h"
#include "pursuer_ai.h"
#include "skeleton.h"
#include "stalker_progress.h"
#include "msl.h"

extern void *D_0046BBB0[];

extern u8 D_003D73B0[];
void *Daniella_MotionFiles(void);

extern u32 D_0043DC50[];
extern u8 D_0043B6A0[], D_0043B660[];
extern u8 D_0043C260[], D_0043C348[], D_0043C2D0[], D_0043C210[], D_0043C338[], D_0043C2B0[];
extern u8 D_0043CC20[], D_0043CD08[], D_0043CC90[], D_0043CBD0[], D_0043CCF8[], D_0043CC70[];
extern u8 D_0043CD80[], D_0043CD40[];
void Kind34_SetRage(u8 *p, s32 alt);
f32 Kind34_ThreatAmount(void);
f32 Kind34_FrightSeen(void);
f32 Kind34_ReachHewie(void);
void *Kind34_ModelFiles(void);
void Kind35_DoorOffset(void *self, s32 i, f32 *out);
void Kind35_ActionOffsets(void *self, s32 i, f32 *out);
f32 Kind35_ThreatAmount(void);
f32 Kind35_FrightSeen(void);
f32 Kind35_ReachHewie(void);
void *Kind35_ModelFiles(void);
void *Kind36_MotionFiles(void);
void Kind36_DoorOffset(void *self, s32 i, f32 *out);
void Kind36_ActionOffsets(void *self, s32 i, f32 *out);
f32 Kind36_ThreatAmount(void);

s32 Kind34_AttackAnimB(void);
s32 Kind34_AttackAnimA(void);
s32 Kind35_AttackAnimB(void);
s32 Kind35_AttackAnimA(void);
s32 Kind36_Kind(void);
s32 Kind36_AttackAnimB(void);
s32 Kind36_AttackAnimA(void);

extern void *D_00478160[];
void Kind36_FilesLoaded(Pursuer *p);

extern u8 D_0043DC10[], D_0043DBD0[];
f32 Kind36_FrightSeen(void);
f32 Kind36_ReachHewie(void);
void *Kind36_ModelFiles(void);

/* destructor: own vtable -> Pursuer 0x46D810 -> NPC 0x46C220 -> Character; the model freed for
 * slots 3..5 */
static inline __attribute__((always_inline)) Character *creature_dtor(Character *c, s32 flags, void **vt) {
    if (c != NULL) {
        c->a.vtbl = vt;
        c->a.vtbl = D_0046D810;
        VCALL(c, 0x10, void (*)(Character *))(c);
        if ((u32)c->a.slot >= 3 && (u32)c->a.slot < 6) {
            void **m = c->motion;

            if (m != NULL) {
                VCALL(m, 0x8, void (*)(void *, s32))(m, 1);
                c->motion = NULL;
            }
        }
        c->a.vtbl = D_0046C220;
        VCALL(c, 0x10, void (*)(Character *))(c);
        c->a.vtbl = D_00469C60;
        c->a.vtbl = D_00469C20;
        if ((s16)flags > 0) {
            Actor_Destroy(&c->a);
        }
    }
    return c;
}

Character *Kind36_dtor(Character *c, s32 flags);

/* vtable +0x8: destructor */
/* 0x0020C3A0 */
Pursuer *Daniella_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = D_0046BBB0;
        if (p != NULL) {
            Pursuer_DestroyBase(p);
        }
        if ((s16)flags > 0) {
            Actor_Destroy(&p->c.a);
        }
    }
    return p;
}

/* vtable +0x32C: her character kind */
/* 0x0020C4B0 */
s32 Daniella_Kind(Pursuer *p) {
    return 3;
}

/* 0x0020C4C0 */
void *Daniella_MotionFiles(void) {
    return D_003D73B0;
}

/* vtable +0x1C: the model files loaded (Pursuer_FilesLoaded), then motion vtable +0x34 */
/* 0x0020C4D0 */
void Daniella_FilesLoaded(Pursuer *p) {
    void *m;

    Pursuer_FilesLoaded(p);
    m = p->c.motion;
    VCALL(m, 0x34, void (*)(void *, s32))(m, 0);
}

/* vtable +0x9C: where she stands by a door, by side 0..3 (local offsets) */
/* 0x0020C510 */
void Daniella_DoorOffset(Pursuer *p, s32 side, f32 *out) {
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
/* 0x0020C5B0 */
void Daniella_ActionOffsets(Pursuer *p, s32 kind, f32 *out) {
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

/* vtable +0xE4: at a door she breaks (Progress_ExitOpen) while opening or attacking it: mark it
   (vtable +0xF0) and change room through it (vtable +0x28) */
/* 0x0020C660 */
void Daniella_DoorBreak(Pursuer *p, s32 exit) {
    if (!(Progress_ExitOpen(gProgress, p->c.a.room, exit) & 0xFF)) {
        return;
    }
    switch (PU(p, 0x175C, s32)) {
    case 0xC:
    case 0x29:
    case 0x28:
    case 0xD:
        VCALL(p, 0xF0, void (*)(Pursuer *, s32))(p, exit);
        VCALL(p, 0x28, void (*)(Pursuer *, s32, s32, s32))(p, VCALL(gRooms, 0x28, s32 (*)(VObject *, s32))(gRooms, exit), 0, 0);
        break;
    }
}

/* vtable +0xF0: the door `exit` of her room used (DoorHold_Take), then damaged: in mode 2
   (+0x16B8) by DoorHold_Open, otherwise DoorHold_Shut */
/* 0x0020C730 */
void Daniella_ExitArg(Pursuer *p, s32 exit) {
    Progress *pr = gProgress;

    DoorHold_Take(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
    if (PU(p, 0x16B8, s32) == 2) {
        DoorHold_Open(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
    } else {
        DoorHold_Shut(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
    }
}

extern const f32 D_003D8910[4];

/* the effect at her blow: for her grabs of Fiona (0x1904 / 0x1A01) at Fiona's bone, for her
   strikes (0xE00..0xE07) at a point along her hand (bone 0x2D; 1.5 out, 3.5 for 0xE03 / 0xE05);
   larger for 0xE04 / 0xE05 and the grabs */
/* 0x0020C7C0 */
void Daniella_BlowEffect(Pursuer *p) {
    HitEffectParams hp;
    f32 pos[4] __attribute__((aligned(16)));
    f32 reach = 1.5f;
    s32 atHand = 1;

    switch (MOTION_ANIM(p)) {
    case 0x1904:
        atHand = 0;
        sceVu0CopyVector(pos, Skel_Bone(AT(gCharPlayer->motion, 0x810, u8 *), 0x17) + 0xC);
        hp.big = 1.0f;
        break;
    case 0x1A01: {
        u8 *fm = gCharPlayer->motion;
        s32 bone;

        atHand = 0;
        bone = VCALL(fm, 0x80, s32 (*)(void *))(fm);
        sceVu0CopyVector(pos, Skel_Bone(AT(gCharPlayer->motion, 0x810, u8 *), bone) + 0xC);
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
        sceVu0CopyMatrix(m, (f32 (*)[4])Skel_Bone(MOTION_AT(p, 0x810, u8 *), 0x2D));
        Mtx_ApplyVector(pos, m, off);
        sceVu0CopyVector(hand, Skel_Bone(MOTION_AT(p, 0x810, u8 *), 0x2D) + 0xC);
        sceVu0AddVector(pos, pos, hand);
    }
    hp.kind = 0xFE;
    hp.pos[0] = pos[0];
    hp.pos[1] = pos[1];
    hp.pos[2] = pos[2];
    hp.pos[3] = pos[3];
    HitEffect_Spawn(&hp);
}

extern const f32 D_003D8900[4];

/* vtable +0x138: the hit points of an attack entry (`e`: +0 animation, +4 / +8 bones). Her
   strikes 0xE00, 0xE02..0xE08 reach along her hand (bone 0x2D, offset `reach`) from bone
   +4, and back to the hand; others the two bones (the first twice when +8 is none) */
static inline void Daniella_HitPoints(Pursuer *p, s32 *e, f32 *a, f32 *b, const f32 *reach) {
    if ((u32)(e[0] - 0xE00) < 9 && e[0] != 0xE01) {
        f32 off[4] __attribute__((aligned(16)));
        f32 m[4][4] __attribute__((aligned(16)));
        f32 t[4] __attribute__((aligned(16)));

        off[0] = reach[0];
        off[1] = reach[1];
        off[2] = reach[2];
        off[3] = reach[3];
        sceVu0CopyMatrix(m, (f32 (*)[4])Skel_Bone(MOTION_AT(p, 0x810, u8 *), 0x2D));
        Mtx_ApplyVector(a, m, off);
        sceVu0CopyVector(t, Skel_Bone(MOTION_AT(p, 0x810, u8 *), e[1]) + 0xC);
        sceVu0AddVector(a, a, t);
        sceVu0CopyVector(b, Skel_Bone(MOTION_AT(p, 0x810, u8 *), 0x2D) + 0xC);
        return;
    }
    sceVu0CopyVector(a, Skel_Bone(MOTION_AT(p, 0x810, u8 *), e[1]) + 0xC);
    if (e[2] >= 0) {
        sceVu0CopyVector(b, Skel_Bone(MOTION_AT(p, 0x810, u8 *), e[2]) + 0xC);
    } else {
        sceVu0CopyVector(b, Skel_Bone(MOTION_AT(p, 0x810, u8 *), e[1]) + 0xC);
    }
}

/* 0x0020CAE0 */
void Daniella_BonePositions(Pursuer *p, s32 *e, f32 *a, f32 *b) {
    Daniella_HitPoints(p, e, a, b, D_003D8900);
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
/* 0x0020CC70 */
void Daniella_AttackTable(Pursuer *p, s8 situation) {
    s32 alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;
    s32 mode2 = PU(p, 0x16B8, s32) == 2;

    PU(p, 0x1718, u8 *) = sAttackTables[alt][mode2][(u32)situation < 17 ? situation : 0];
}

extern u8 D_003D7E60[], D_003D7EB0[], D_003D7F00[], D_003D7F20[], D_003D7F88[], D_003D7F98[];
extern u8 D_003D87B0[], D_003D8800[], D_003D8850[], D_003D8870[], D_003D88D8[], D_003D88E8[];

/* vtable +0x31C: mode 2 (+0x16B8) on or off, with its hold-off (+0x1730), +0x1740 and cry
   (+0x1748) tables */
/* 0x0020D1F0 */
void Daniella_SetRage(Pursuer *p, s32 on) {
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
/* 0x0020D2E0 */
s32 Daniella_AttackAnimB(Pursuer *p) {
    return 12;
}

/* 0x0020D2F0 */
s32 Daniella_AttackAnimA(Pursuer *p) {
    return 11;
}

/* vtable +0x308 / +0x300 / +0x2F0 */
/* 0x0020D300 */
f32 Daniella_ThreatAmount(Pursuer *p) {
    return 15.0f;
}

/* 0x0020D310 */
f32 Daniella_FrightSeen(Pursuer *p) {
    return 5.0f;
}

/* 0x0020D320 */
f32 Daniella_ReachHewie(Pursuer *p) {
    return 12.0f;
}

/* vtable +0x30: her frame update: as Debilitas's (Debilitas_Update), but without his growl and
   senses step; on screen the hit effect (Daniella_BlowEffect) when a cry is heard or her animation
   reaches its effect key (0x20) */
/* 0x0020D330 */
void Daniella_Update(Pursuer *p) {
    PTMF *st = (PTMF *)((u8 *)p + 0x174C);

    Stalker_ThinkStart(p);
    if (func_00217510(p) != 0) {
        func_00296FC0(p);
        if (func_0029B4B0(p) != 0 || (Motion_EventFlags(p->c.motion, 0, -1, 1) & 0xFF & 0x20)) {
            Daniella_BlowEffect(p);
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

/* her model files (Pursuer_ModelFiles for kind 3) */
/* 0x0020D620 */
u8 *Daniella_ModelFileTable(Pursuer *p) {
    return (AT(gProgress, 0x30, u32) & 0x8000) ? D_003D7390 : D_003D7350;
}

/* vtable +0xF8: her model files in slot 2 */
/* 0x0020D660 */
u8 *Daniella_ModelFiles(Pursuer *p) {
    return (AT(gProgress, 0x30, u32) & 0x8000) ? D_003D7370 : D_003D7330;
}

extern u8 D_003D73F0[], D_003D7530[], D_003D7550[], D_003D7730[], D_003D7760[], D_003D7F40[],
    D_003D8890[], D_0047A928[];

/* vtable +0xF4: her setup over the Pursuer's (Pursuer_Setup): her tables and stats (different
   when gProgress+0x30 bit 0x8000 is set) */
/* 0x0020D6A0 */
void Daniella_Setup(Pursuer *p) {
    void *m;

    Pursuer_Setup(p);
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

/* destructor (vtable ?) */
void *func_0020D8D0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, s32) = 0;
        AT(o, 0x4, s32) = 0;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable ?) */
void *func_0020D920(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x44, s32) = 0;
        AT(o, 0x48, s32) = 0;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable ?) */
void *func_0020D970(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, s32) = 0;
        AT(o, 0x4, s32) = 0;
        AT(o, 0x8, s32) = 0;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* destructor (vtable ?) */
void *func_0020D9C0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x10, s32) = 0;
        AT(o, 0xC, s32) = 0;
        AT(o, 0x4, s32) = 0;
        AT(o, 0x8, s32) = 0;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* ---- the same shapes in other classes, generated from the functions they copy (2026-10-05) ---- */
extern void Kind34_BlowEffect(Pursuer *p);
extern void Kind35_BlowEffect(Pursuer *p);
extern void Kind36_BlowEffect(Pursuer *p);

/* (as Daniella_DoorBreak)  vtable +0xE4: at a door she breaks (Progress_ExitOpen) while opening or attacking it: mark it
   (vtable +0xF0) and change room through it (vtable +0x28) */
/* 0x003461A0 */
void Kind34_DoorBreak(Pursuer *p, s32 exit) {
    if (!(Progress_ExitOpen(gProgress, p->c.a.room, exit) & 0xFF)) {
        return;
    }
    switch (PU(p, 0x175C, s32)) {
    case 0xC:
    case 0x29:
    case 0x28:
    case 0xD:
        VCALL(p, 0xF0, void (*)(Pursuer *, s32))(p, exit);
        VCALL(p, 0x28, void (*)(Pursuer *, s32, s32, s32))(p, VCALL(gRooms, 0x28, s32 (*)(VObject *, s32))(gRooms, exit), 0, 0);
        break;
    }
}

/* (as Daniella_Update)  vtable +0x30: her frame update: as Debilitas's (Debilitas_Update), but without his growl and
   senses step; on screen the hit effect (Kind34_BlowEffect) when a cry is heard or her animation
   reaches its effect key (0x20) */
/* 0x00346F50 */
void Kind34_Update(Pursuer *p) {
    PTMF *st = (PTMF *)((u8 *)p + 0x174C);

    Stalker_ThinkStart(p);
    if (func_00217510(p) != 0) {
        func_00296FC0(p);
        if (func_0029B4B0(p) != 0 || (Motion_EventFlags(p->c.motion, 0, -1, 1) & 0xFF & 0x20)) {
            Kind34_BlowEffect(p);
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

/* (as Daniella_FilesLoaded)  vtable +0x1C: the model files loaded (Pursuer_FilesLoaded), then motion vtable +0x34 */
/* 0x003479F0 */
void Kind35_FilesLoaded(Pursuer *p) {
    void *m;

    Pursuer_FilesLoaded(p);
    m = p->c.motion;
    VCALL(m, 0x34, void (*)(void *, s32))(m, 0);
}

/* 0x00347A30 */
void Kind35_DoorOffset(void *self, s32 i, f32 *out) {
    switch (i) {
    case 1: out[0] = 0.0f; out[1] = 0.0f; out[2] = 0x1.be824p+2f /* 6.9767 */; break;
    case 3: out[0] = 0.0f; out[1] = 0.0f; out[2] = -0x1.905f06p+2f /* -6.2558 */; break;
    case 0: out[0] = 0.0f; out[1] = 0.0f; out[2] = -0x1.bdc432p+2f /* -6.9651 */; break;
    case 2: out[0] = 0.0f; out[1] = 0.0f; out[2] = 0x1.ce0418p+2f /* 7.219 */; break;
    }
}

/* 0x00347AD0 */
void Kind35_ActionOffsets(void *self, s32 i, f32 *out) {
    switch (i) {
    case 10: case 11: out[0] = 0x1.07c84cp-2f /* 0.2576 */; out[1] = 0.0f; out[2] = 0x1.567fccp+3f /* 10.7031 */; break;
    case 12: case 13: out[0] = 0x1.a4a8c2p+0f /* 1.6432 */; out[1] = 0.0f; out[2] = 0x1.5d182ap+3f /* 10.9092 */; break;
    case 14: out[0] = -0x1.5f06f6p-3f /* -0.1714 */; out[1] = 0.0f; out[2] = -0x1.8f6fd2p+1f /* -3.1206 */; break;
    case 15: out[0] = 0x1.9a0276p-2f /* 0.4004 */; out[1] = 0.0f; out[2] = -0x1.792d78p+1f /* -2.9467 */; break;
    }
}

/* (as Daniella_Update)  vtable +0x30: her frame update: as Debilitas's (Debilitas_Update), but without his growl and
   senses step; on screen the hit effect (Kind35_BlowEffect) when a cry is heard or her animation
   reaches its effect key (0x20) */
/* 0x00348330 */
void Kind35_Update(Pursuer *p) {
    PTMF *st = (PTMF *)((u8 *)p + 0x174C);

    Stalker_ThinkStart(p);
    if (func_00217510(p) != 0) {
        func_00296FC0(p);
        if (func_0029B4B0(p) != 0 || (Motion_EventFlags(p->c.motion, 0, -1, 1) & 0xFF & 0x20)) {
            Kind35_BlowEffect(p);
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

/* 0x00348660 */
void *Kind35_ModelFiles(void) {
    return (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) ? D_0043CD80 : D_0043CD40;
}

/* (as Daniella_Update)  vtable +0x30: her frame update: as Debilitas's (Debilitas_Update), but without his growl and
   senses step; on screen the hit effect (Kind36_BlowEffect) when a cry is heard or her animation
   reaches its effect key (0x20) */
/* 0x003492C0 */
void Kind36_Update(Pursuer *p) {
    PTMF *st = (PTMF *)((u8 *)p + 0x174C);

    Stalker_ThinkStart(p);
    if (func_00217510(p) != 0) {
        func_00296FC0(p);
        if (func_0029B4B0(p) != 0 || (Motion_EventFlags(p->c.motion, 0, -1, 1) & 0xFF & 0x20)) {
            Kind36_BlowEffect(p);
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

/* 0x003495F0 */
void *Kind36_ModelFiles(void) {
    return (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) ? D_0043DC10 : D_0043DBD0;
}

/* ---- the same shapes in other classes, generated from the functions they copy (2026-10-05) ---- */
extern const f32 D_0043CD30[4];
extern const f32 D_0043DBC0[4];
extern const f32 D_0043EA70[4];

/* (as Daniella_BlowEffect)  the effect at her blow: for her grabs of Fiona (0x1904 / 0x1A01) at Fiona's bone, for her
   strikes (0xE00..0xE07) at a point along her hand (bone 0x2D; 1.5 out, 3.5 for 0xE03 / 0xE05);
   larger for 0xE04 / 0xE05 and the grabs */
/* 0x00346330 */
void Kind34_BlowEffect(Pursuer *p) {
    HitEffectParams hp;
    f32 pos[4] __attribute__((aligned(16)));
    f32 reach = 1.5f;
    s32 atHand = 1;

    switch (MOTION_ANIM(p)) {
    case 0x1904:
        atHand = 0;
        sceVu0CopyVector(pos, Skel_Bone(AT(gCharPlayer->motion, 0x810, u8 *), 0x17) + 0xC);
        hp.big = 1.0f;
        break;
    case 0x1A01: {
        u8 *fm = gCharPlayer->motion;
        s32 bone;

        atHand = 0;
        bone = VCALL(fm, 0x80, s32 (*)(void *))(fm);
        sceVu0CopyVector(pos, Skel_Bone(AT(gCharPlayer->motion, 0x810, u8 *), bone) + 0xC);
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

        off[0] = D_0043CD30[0];
        off[1] = D_0043CD30[1];
        off[2] = reach;
        off[3] = D_0043CD30[3];
        sceVu0CopyMatrix(m, (f32 (*)[4])Skel_Bone(MOTION_AT(p, 0x810, u8 *), 0x2D));
        Mtx_ApplyVector(pos, m, off);
        sceVu0CopyVector(hand, Skel_Bone(MOTION_AT(p, 0x810, u8 *), 0x2D) + 0xC);
        sceVu0AddVector(pos, pos, hand);
    }
    hp.kind = 0xFE;
    hp.pos[0] = pos[0];
    hp.pos[1] = pos[1];
    hp.pos[2] = pos[2];
    hp.pos[3] = pos[3];
    HitEffect_Spawn(&hp);
}

/* (as Daniella_BlowEffect)  the effect at her blow: for her grabs of Fiona (0x1904 / 0x1A01) at Fiona's bone, for her
   strikes (0xE00..0xE07) at a point along her hand (bone 0x2D; 1.5 out, 3.5 for 0xE03 / 0xE05);
   larger for 0xE04 / 0xE05 and the grabs */
/* 0x00347B80 */
void Kind35_BlowEffect(Pursuer *p) {
    HitEffectParams hp;
    f32 pos[4] __attribute__((aligned(16)));
    f32 reach = 1.5f;
    s32 atHand = 1;

    switch (MOTION_ANIM(p)) {
    case 0x1904:
        atHand = 0;
        sceVu0CopyVector(pos, Skel_Bone(AT(gCharPlayer->motion, 0x810, u8 *), 0x17) + 0xC);
        hp.big = 1.0f;
        break;
    case 0x1A01: {
        u8 *fm = gCharPlayer->motion;
        s32 bone;

        atHand = 0;
        bone = VCALL(fm, 0x80, s32 (*)(void *))(fm);
        sceVu0CopyVector(pos, Skel_Bone(AT(gCharPlayer->motion, 0x810, u8 *), bone) + 0xC);
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

        off[0] = D_0043DBC0[0];
        off[1] = D_0043DBC0[1];
        off[2] = reach;
        off[3] = D_0043DBC0[3];
        sceVu0CopyMatrix(m, (f32 (*)[4])Skel_Bone(MOTION_AT(p, 0x810, u8 *), 0x2D));
        Mtx_ApplyVector(pos, m, off);
        sceVu0CopyVector(hand, Skel_Bone(MOTION_AT(p, 0x810, u8 *), 0x2D) + 0xC);
        sceVu0AddVector(pos, pos, hand);
    }
    hp.kind = 0xFE;
    hp.pos[0] = pos[0];
    hp.pos[1] = pos[1];
    hp.pos[2] = pos[2];
    hp.pos[3] = pos[3];
    HitEffect_Spawn(&hp);
}

/* (as Daniella_BlowEffect)  the effect at her blow: for her grabs of Fiona (0x1904 / 0x1A01) at Fiona's bone, for her
   strikes (0xE00..0xE07) at a point along her hand (bone 0x2D; 1.5 out, 3.5 for 0xE03 / 0xE05);
   larger for 0xE04 / 0xE05 and the grabs */
/* 0x00348B10 */
void Kind36_BlowEffect(Pursuer *p) {
    HitEffectParams hp;
    f32 pos[4] __attribute__((aligned(16)));
    f32 reach = 1.5f;
    s32 atHand = 1;

    switch (MOTION_ANIM(p)) {
    case 0x1904:
        atHand = 0;
        sceVu0CopyVector(pos, Skel_Bone(AT(gCharPlayer->motion, 0x810, u8 *), 0x17) + 0xC);
        hp.big = 1.0f;
        break;
    case 0x1A01: {
        u8 *fm = gCharPlayer->motion;
        s32 bone;

        atHand = 0;
        bone = VCALL(fm, 0x80, s32 (*)(void *))(fm);
        sceVu0CopyVector(pos, Skel_Bone(AT(gCharPlayer->motion, 0x810, u8 *), bone) + 0xC);
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

        off[0] = D_0043EA70[0];
        off[1] = D_0043EA70[1];
        off[2] = reach;
        off[3] = D_0043EA70[3];
        sceVu0CopyMatrix(m, (f32 (*)[4])Skel_Bone(MOTION_AT(p, 0x810, u8 *), 0x2D));
        Mtx_ApplyVector(pos, m, off);
        sceVu0CopyVector(hand, Skel_Bone(MOTION_AT(p, 0x810, u8 *), 0x2D) + 0xC);
        sceVu0AddVector(pos, pos, hand);
    }
    hp.kind = 0xFE;
    hp.pos[0] = pos[0];
    hp.pos[1] = pos[1];
    hp.pos[2] = pos[2];
    hp.pos[3] = pos[3];
    HitEffect_Spawn(&hp);
}

/* ---- her other classes (vtables near 0x477B00 / 0x477E50 / 0x478180, code 0x346000..0x349630):
   her own methods with their own tables and stats ---- */

extern const f32 D_0043CD20[4], D_0043DBB0[4], D_0043EA60[4];

/* the first: vtable +0xF0: the door `exit` used, then damaged: in mode 2, in the ending
   (gProgress+0x1FBEC1) or with gProgress+0x30 bit 0x8000 by DoorHold_Open, otherwise
   DoorHold_Shut */
/* 0x00346270 */
void Kind34_ExitArg(Pursuer *p, s32 exit) {
    Progress *pr = gProgress;

    DoorHold_Take(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
    if (PU(p, 0x16B8, s32) == 2 || AT(pr, 0x1FBEC1, u8) != 0 || (AT(pr, 0x30, u32) & 0x8000)) {
        DoorHold_Open(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
    } else {
        DoorHold_Shut(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
    }
}

/* vtable +0x138 of the three */
/* 0x00346640 */
void Kind34_BonePositions(Pursuer *p, s32 *e, f32 *a, f32 *b) {
    Daniella_HitPoints(p, e, a, b, D_0043CD20);
}

/* 0x00347E90 */
void Kind35_BonePositions(Pursuer *p, s32 *e, f32 *a, f32 *b) {
    Daniella_HitPoints(p, e, a, b, D_0043DBB0);
}

/* 0x00348E20 */
void Kind36_BonePositions(Pursuer *p, s32 *e, f32 *a, f32 *b) {
    Daniella_HitPoints(p, e, a, b, D_0043EA60);
}

extern u8 D_0043BAC0[], D_0043BB30[], D_0043BBA0[], D_0043BC00[], D_0043BC30[], D_0043BC90[],
    D_0043BCD0[], D_0043BD30[], D_0043BD90[], D_0043BDC0[], D_0043BDF0[], D_0043BE40[],
    D_0043BE60[], D_0043BE90[], D_0043BEC0[], D_0043BEF0[], D_0043BF00[], D_0043BF10[],
    D_0043BF50[], D_0043BF90[], D_0043BFC0[], D_0043BFF0[], D_0043C030[], D_0043C070[],
    D_0043C0B0[], D_0043C0F0[], D_0043C110[], D_0043C130[], D_0043C158[], D_0043C170[],
    D_0043C1A0[], D_0043C1C0[], D_0043C1F0[], D_0043C200[], D_0043C360[], D_0043C3D0[],
    D_0043C440[], D_0043C4A0[], D_0043C4D0[], D_0043C530[], D_0043C560[], D_0043C5D0[],
    D_0043C630[], D_0043C660[], D_0043C690[], D_0043C6E0[], D_0043C710[], D_0043C740[],
    D_0043C770[], D_0043C7A0[], D_0043C7B0[], D_0043C7D0[], D_0043C830[], D_0043C890[],
    D_0043C8F0[], D_0043C920[], D_0043C980[], D_0043C9D0[], D_0043CA30[], D_0043CA90[],
    D_0043CAC0[], D_0043CAE0[], D_0043CB10[], D_0043CB40[], D_0043CB68[], D_0043CB80[],
    D_0043CBB0[], D_0043CBC0[];

/* the first's attack tables: [gProgress+0x30 bit 0x8000][mode 2 (+0x16B8)] */
static u8 *const sAttackTables1[2][2][17] = {
    {
        { D_0043BAC0, D_0043BBA0, D_0043BB30, D_0043BC00, D_0043BC30, D_0043BC90, D_0043BCD0,
          D_0043BD30, D_0043BD90, D_0043BDC0, D_0043BDF0, D_0043BE40, D_0043BE60, D_0043BE90,
          D_0043BEF0, D_0043BF00, D_0043BEC0 },
        { D_0043BF10, D_0043BF90, D_0043BF50, D_0043BFC0, D_0043BFF0, D_0043C030, D_0043C070,
          D_0043C0B0, D_0043C0F0, D_0043C110, D_0043C130, D_0043C158, D_0043C170, D_0043C1A0,
          D_0043C1F0, D_0043C200, D_0043C1C0 },
    },
    {
        { D_0043C360, D_0043C440, D_0043C3D0, D_0043C4A0, D_0043C4D0, D_0043C530, D_0043C560,
          D_0043C5D0, D_0043C630, D_0043C660, D_0043C690, D_0043C6E0, D_0043C710, D_0043C740,
          D_0043C7A0, D_0043C7B0, D_0043C770 },
        { D_0043C7D0, D_0043C890, D_0043C830, D_0043C8F0, D_0043C920, D_0043C980, D_0043C9D0,
          D_0043CA30, D_0043CA90, D_0043CAC0, D_0043CAE0, D_0043CB10, D_0043CB40, D_0043CB68,
          D_0043CBB0, D_0043CBC0, D_0043CB80 },
    },
};

/* the first: vtable +0x130: the attack table for a situation; in the ending situations 15 and
   14 have their own */
/* 0x003467D0 */
void Kind34_AttackTable(Pursuer *p, s8 situation) {
    s32 alt, mode2;

    if (AT(gProgress, 0x1FBEC1, u8) != 0) {
        if (situation == 15) {
            PU(p, 0x1718, u8 *) = D_0043C200;
            return;
        }
        if (situation == 14) {
            PU(p, 0x1718, u8 *) = D_0043C1F0;
            return;
        }
    }
    alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;
    mode2 = PU(p, 0x16B8, s32) == 2;
    PU(p, 0x1718, u8 *) = sAttackTables1[alt][mode2][(u32)situation < 17 ? situation : 0];
}

/* the first: vtable +0x200: done at the door in the ending or with gProgress+0x30 bit 0x8000,
   else the Pursuer's */
/* 0x00346DB0 */
void Kind34_ExitDone(Pursuer *p) {
    Progress *pr = gProgress;

    if (AT(pr, 0x1FBEC1, u8) != 0 || (AT(pr, 0x30, u32) & 0x8000)) {
        PURSUER_STEP_DONE(p) = 1;
        return;
    }
    Pursuer_ExitDone(p);
}

/* Picks one of four table sets depending on story flag 0x8000 (gProgress+0x30) and `alt`. */
/* 0x00346E10 */
void Kind34_SetRage(u8 *p, s32 alt) {
    if (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) {
        if (alt) {
            *(s32 *)(p + 0x16B8) = 2;
            *(void **)(p + 0x1730) = D_0043CC20;
            *(void **)(p + 0x1748) = D_0043CD08;
            *(void **)(p + 0x1740) = D_0043CC90;
        } else {
            *(s32 *)(p + 0x16B8) = 0;
            *(void **)(p + 0x1730) = D_0043CBD0;
            *(void **)(p + 0x1748) = D_0043CCF8;
            *(void **)(p + 0x1740) = D_0043CC70;
        }
    } else {
        if (alt) {
            *(s32 *)(p + 0x16B8) = 2;
            *(void **)(p + 0x1730) = D_0043C260;
            *(void **)(p + 0x1748) = D_0043C348;
            *(void **)(p + 0x1740) = D_0043C2D0;
        } else {
            *(s32 *)(p + 0x16B8) = 0;
            *(void **)(p + 0x1730) = D_0043C210;
            *(void **)(p + 0x1748) = D_0043C338;
            *(void **)(p + 0x1740) = D_0043C2B0;
        }
    }
}

/* 0x00346F00 */
s32 Kind34_AttackAnimB(void) {
    return 0xC;
}

/* 0x00346F10 */
s32 Kind34_AttackAnimA(void) {
    return 0xB;
}

/* 0x00346F20 */
f32 Kind34_ThreatAmount(void) {
    return 15.0f;
}

/* 0x00346F30 */
f32 Kind34_FrightSeen(void) {
    return 5.0f;
}

/* 0x00346F40 */
f32 Kind34_ReachHewie(void) {
    return 12.0f;
}

/* the first: vtable +0x5C: the Pursuer's reset; in the ending the Hewie bite tolerance is 80 */
/* 0x00347240 */
void Kind34_Activate(Pursuer *p) {
    Pursuer_Activate(p);
    if (AT(gProgress, 0x1FBEC1, u8) != 0) {
        PU(p, 0x16DC, s32) = 80;
    }
}

/* 0x003472D0 */
void *Kind34_ModelFiles(void) {
    return (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) ? D_0043B6A0 : D_0043B660;
}

extern u8 D_0043B700[], D_0043B840[], D_0043B860[], D_0043BA40[], D_0043BA70[], D_0043C210[],
    D_0043C2B0[], D_0043C2F0[], D_0043C338[], D_0043CBD0[], D_0043CC70[], D_0043CCB0[],
    D_0043CCF8[], D_0047AED8[];

/* the first: vtable +0xF4: setup over the Pursuer's (Pursuer_Setup) */
/* 0x00347310 */
void Kind34_Setup(Pursuer *p) {
    void *m;

    Pursuer_Setup(p);
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        p->c.hpMax = 200;
        PU(p, 0x171C, u8 *) = D_0043B860;
        PU(p, 0x1730, u8 *) = D_0043CBD0;
        PU(p, 0x1740, u8 *) = D_0043CC70;
        PU(p, 0x173C, u8 *) = D_0043CCB0;
        PU(p, 0x1748, u8 *) = D_0043CCF8;
        PU(p, 0x16DC, s32) = 65;              /* Hewie bite tolerance */
        PU(p, 0x16E8, f32) = 50.0f;
        PU(p, 0x16D4, s32) = 360;
        PU(p, 0x16D8, s32) = 6000;
        PU(p, 0x16D0, s32) = 1350;
        PU(p, 0x16E0, s32) = 6000;
        PU(p, 0x16E4, s32) = 160;
    } else {
        p->c.hpMax = 120;
        PU(p, 0x171C, u8 *) = D_0043B860;
        PU(p, 0x1730, u8 *) = D_0043C210;
        PU(p, 0x1740, u8 *) = D_0043C2B0;
        PU(p, 0x173C, u8 *) = D_0043C2F0;
        PU(p, 0x1748, u8 *) = D_0043C338;
        PU(p, 0x16DC, s32) = 200;
        PU(p, 0x16E8, f32) = 25.0f;
        PU(p, 0x16D4, s32) = 300;
        PU(p, 0x16D8, s32) = 6000;
        PU(p, 0x16D0, s32) = 900;
        PU(p, 0x16E0, s32) = 4500;
        PU(p, 0x16E4, s32) = 180;
    }
    p->c.a.radius = 3.0f;
    p->c.a.height = 17.0f;
    p->c.hp = p->c.hpMax;
    p->c.hearThreshold = 12;
    PU(p, 0x171C, u8 *) = D_0043B860;
    PU(p, 0x1720, u8 *) = D_0043BA40;
    PU(p, 0x1724, u8 *) = D_0043BA70;
    PU(p, 0x16AC, u8 *) = D_0043B700;
    PU(p, 0x16B0, u8 *) = D_0043B840;
    PU(p, 0x1734, u8 *) = D_0047AED8;
    PU(p, 0x1694, f32) = 8.0f;
    PU(p, 0x169C, f32) = 1.5f;
    PU(p, 0x1698, f32) = 12.0f;
    PU(p, 0x16A0, f32) = 1.5f;
    m = p->c.motion;
    VCALL(m, 0x34, void (*)(void *, s32))(m, 1);
}

extern u8 D_0043D180[], D_0043D200[], D_0043D270[], D_0043D2D0[], D_0043D310[], D_0043D370[],
    D_0043D3C0[], D_0043D420[], D_0043D480[], D_0043D4B0[], D_0043D4D0[], D_0043D510[],
    D_0043D530[], D_0043D560[], D_0043D590[], D_0043D5C0[], D_0043D5D0[], D_0043D6B0[],
    D_0043D710[], D_0043D770[], D_0043D7E0[], D_0043D820[], D_0043D880[], D_0043D8D0[],
    D_0043D930[], D_0043D990[], D_0043D9C0[], D_0043D9E0[], D_0043DA10[], D_0043DA30[],
    D_0043DA60[], D_0043DA90[], D_0043DAC0[], D_0043DAD0[];

/* the second's attack tables; the second set when gProgress+0x30 bit 0x8000 */
static u8 *const sAttackTables2[2][17] = {
    { D_0043D180, D_0043D270, D_0043D200, D_0043D2D0, D_0043D310, D_0043D370, D_0043D3C0,
      D_0043D420, D_0043D480, D_0043D4B0, D_0043D4D0, D_0043D510, D_0043D530, D_0043D560,
      D_0043D5C0, D_0043D5D0, D_0043D590 },
    { D_0043D6B0, D_0043D770, D_0043D710, D_0043D7E0, D_0043D820, D_0043D880, D_0043D8D0,
      D_0043D930, D_0043D990, D_0043D9C0, D_0043D9E0, D_0043DA10, D_0043DA30, D_0043DA60,
      D_0043DAC0, D_0043DAD0, D_0043DA90 },
};

/* the second: vtable +0x130 */
/* 0x00348020 */
void Kind35_AttackTable(Pursuer *p, s8 situation) {
    s32 alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;

    PU(p, 0x1718, u8 *) = sAttackTables2[alt][(u32)situation < 17 ? situation : 0];
}

/* 0x003482E0 */
s32 Kind35_AttackAnimB(void) {
    return 0xC;
}

/* 0x003482F0 */
s32 Kind35_AttackAnimA(void) {
    return 0xB;
}

/* 0x00348300 */
f32 Kind35_ThreatAmount(void) {
    return 15.0f;
}

/* 0x00348310 */
f32 Kind35_FrightSeen(void) {
    return 5.0f;
}

/* 0x00348320 */
f32 Kind35_ReachHewie(void) {
    return 12.0f;
}

extern u8 D_0043CDE0[], D_0043CF20[], D_0043D100[], D_0043D130[], D_0043D5E0[], D_0043D630[],
    D_0043D650[], D_0043D698[], D_0043DAE0[], D_0043DB30[], D_0043DB50[], D_0043DB98[],
    D_0047AEE0[];

/* the second: vtable +0xF4: setup over the Pursuer's */
/* 0x003486A0 */
void Kind35_Setup(Pursuer *p) {
    Pursuer_Setup(p);
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        p->c.hpMax = 80;
        PU(p, 0x171C, u8 *) = D_0043CF20;
        PU(p, 0x1730, u8 *) = D_0043DAE0;
        PU(p, 0x1740, u8 *) = D_0043DB30;
        PU(p, 0x173C, u8 *) = D_0043DB50;
        PU(p, 0x1748, u8 *) = D_0043DB98;
        PU(p, 0x16DC, s32) = 75;              /* Hewie bite tolerance */
        PU(p, 0x16E8, f32) = 10.0f;
        PU(p, 0x16D4, s32) = 600;
        PU(p, 0x16D8, s32) = 600;
        PU(p, 0x16D0, s32) = 360;
        PU(p, 0x16E0, s32) = 6000;
        PU(p, 0x16E4, s32) = 100;
    } else {
        p->c.hpMax = 65;
        PU(p, 0x171C, u8 *) = D_0043CF20;
        PU(p, 0x1730, u8 *) = D_0043D5E0;
        PU(p, 0x1740, u8 *) = D_0043D630;
        PU(p, 0x173C, u8 *) = D_0043D650;
        PU(p, 0x1748, u8 *) = D_0043D698;
        PU(p, 0x16DC, s32) = 120;
        PU(p, 0x16E8, f32) = 15.0f;
        PU(p, 0x16D4, s32) = 600;
        PU(p, 0x16D8, s32) = 3000;
        PU(p, 0x16D0, s32) = 450;
        PU(p, 0x16E0, s32) = 3600;
        PU(p, 0x16E4, s32) = 140;
    }
    p->c.a.radius = 3.0f;
    p->c.a.height = 17.0f;
    p->c.hp = p->c.hpMax;
    p->c.hearThreshold = 12;
    PU(p, 0x171C, u8 *) = D_0043CF20;
    PU(p, 0x1720, u8 *) = D_0043D100;
    PU(p, 0x1724, u8 *) = D_0043D130;
    PU(p, 0x16AC, u8 *) = D_0043CDE0;
    PU(p, 0x1734, u8 *) = D_0047AEE0;
    PU(p, 0x1694, f32) = 8.0f;
    PU(p, 0x169C, f32) = 1.5f;
    PU(p, 0x1698, f32) = 12.0f;
    PU(p, 0x16A0, f32) = 1.5f;
}

/* 0x00348850 */
Character *Kind36_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, D_00478160); }

/* 0x00348960 */
s32 Kind36_Kind(void) {
    return 0x24;
}

/* 0x00348970 */
void *Kind36_MotionFiles(void) {
    return D_0043DC50;
}

/* (as Kind34_FilesLoaded) */
/* 0x00348980 */
void Kind36_FilesLoaded(Pursuer *p) {
    Pursuer_FilesLoaded(p);
    VCALL(p->c.motion, 0x34, void (*)(void *, s32))(p->c.motion, 1);
}

/* 0x003489C0 */
void Kind36_DoorOffset(void *self, s32 i, f32 *out) {
    switch (i) {
    case 1: out[0] = 0.0f; out[1] = 0.0f; out[2] = 0x1.be824p+2f /* 6.9767 */; break;
    case 3: out[0] = 0.0f; out[1] = 0.0f; out[2] = -0x1.905f06p+2f /* -6.2558 */; break;
    case 0: out[0] = 0.0f; out[1] = 0.0f; out[2] = -0x1.bdc432p+2f /* -6.9651 */; break;
    case 2: out[0] = 0.0f; out[1] = 0.0f; out[2] = 0x1.ce0418p+2f /* 7.219 */; break;
    }
}

/* 0x00348A60 */
void Kind36_ActionOffsets(void *self, s32 i, f32 *out) {
    switch (i) {
    case 10: case 11: out[0] = 0x1.07c84cp-2f /* 0.2576 */; out[1] = 0.0f; out[2] = 0x1.567fccp+3f /* 10.7031 */; break;
    case 12: case 13: out[0] = 0x1.a4a8c2p+0f /* 1.6432 */; out[1] = 0.0f; out[2] = 0x1.5d182ap+3f /* 10.9092 */; break;
    case 14: out[0] = -0x1.5f06f6p-3f /* -0.1714 */; out[1] = 0.0f; out[2] = -0x1.8f6fd2p+1f /* -3.1206 */; break;
    case 15: out[0] = 0x1.9a0276p-2f /* 0.4004 */; out[1] = 0.0f; out[2] = -0x1.792d78p+1f /* -2.9467 */; break;
    }
}

extern u8 D_0043E010[], D_0043E090[], D_0043E100[], D_0043E160[], D_0043E1A0[], D_0043E200[],
    D_0043E250[], D_0043E2B0[], D_0043E310[], D_0043E340[], D_0043E360[], D_0043E3A0[],
    D_0043E3C0[], D_0043E3F0[], D_0043E420[], D_0043E450[], D_0043E460[], D_0043E540[],
    D_0043E5C0[], D_0043E630[], D_0043E690[], D_0043E6D0[], D_0043E730[], D_0043E770[],
    D_0043E7D0[], D_0043E830[], D_0043E860[], D_0043E880[], D_0043E8B0[], D_0043E8E0[],
    D_0043E910[], D_0043E940[], D_0043E970[], D_0043E980[];

/* the third's attack tables */
static u8 *const sAttackTables3[2][17] = {
    { D_0043E010, D_0043E100, D_0043E090, D_0043E160, D_0043E1A0, D_0043E200, D_0043E250,
      D_0043E2B0, D_0043E310, D_0043E340, D_0043E360, D_0043E3A0, D_0043E3C0, D_0043E3F0,
      D_0043E450, D_0043E460, D_0043E420 },
    { D_0043E540, D_0043E630, D_0043E5C0, D_0043E690, D_0043E6D0, D_0043E730, D_0043E770,
      D_0043E7D0, D_0043E830, D_0043E860, D_0043E880, D_0043E8B0, D_0043E8E0, D_0043E910,
      D_0043E970, D_0043E980, D_0043E940 },
};

/* the third: vtable +0x130 */
/* 0x00348FB0 */
void Kind36_AttackTable(Pursuer *p, s8 situation) {
    s32 alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;

    PU(p, 0x1718, u8 *) = sAttackTables3[alt][(u32)situation < 17 ? situation : 0];
}

/* 0x00349270 */
s32 Kind36_AttackAnimB(void) {
    return 0xC;
}

/* 0x00349280 */
s32 Kind36_AttackAnimA(void) {
    return 0xB;
}

/* 0x00349290 */
f32 Kind36_ThreatAmount(void) {
    return 15.0f;
}

/* 0x003492A0 */
f32 Kind36_FrightSeen(void) {
    return 5.0f;
}

/* 0x003492B0 */
f32 Kind36_ReachHewie(void) {
    return 12.0f;
}

extern u8 D_0043DC70[], D_0043DDB0[], D_0043DF90[], D_0043DFC0[], D_0043E470[], D_0043E4C0[],
    D_0043E4E0[], D_0043E528[], D_0043E990[], D_0043E9E0[], D_0043EA00[], D_0043EA48[],
    D_0047AEE8[];

/* the third: vtable +0xF4: setup over the Pursuer's */
/* 0x00349630 */
void Kind36_Setup(Pursuer *p) {
    Pursuer_Setup(p);
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        p->c.hpMax = 80;
        PU(p, 0x171C, u8 *) = D_0043DDB0;
        PU(p, 0x1730, u8 *) = D_0043E990;
        PU(p, 0x1740, u8 *) = D_0043E9E0;
        PU(p, 0x173C, u8 *) = D_0043EA00;
        PU(p, 0x1748, u8 *) = D_0043EA48;
        PU(p, 0x16DC, s32) = 65;              /* Hewie bite tolerance */
        PU(p, 0x16E8, f32) = 10.0f;
        PU(p, 0x16D4, s32) = 600;
        PU(p, 0x16D8, s32) = 600;
        PU(p, 0x16D0, s32) = 360;
        PU(p, 0x16E0, s32) = 6000;
        PU(p, 0x16E4, s32) = 160;
    } else {
        p->c.hpMax = 65;
        PU(p, 0x171C, u8 *) = D_0043DDB0;
        PU(p, 0x1730, u8 *) = D_0043E470;
        PU(p, 0x1740, u8 *) = D_0043E4C0;
        PU(p, 0x173C, u8 *) = D_0043E4E0;
        PU(p, 0x1748, u8 *) = D_0043E528;
        PU(p, 0x16DC, s32) = 120;
        PU(p, 0x16E8, f32) = 25.0f;
        PU(p, 0x16D4, s32) = 300;
        PU(p, 0x16D8, s32) = 3000;
        PU(p, 0x16D0, s32) = 900;
        PU(p, 0x16E0, s32) = 4500;
        PU(p, 0x16E4, s32) = 180;
    }
    p->c.a.radius = 3.0f;
    p->c.a.height = 17.0f;
    p->c.hp = p->c.hpMax;
    p->c.hearThreshold = 12;
    PU(p, 0x171C, u8 *) = D_0043DDB0;
    PU(p, 0x1720, u8 *) = D_0043DF90;
    PU(p, 0x1724, u8 *) = D_0043DFC0;
    PU(p, 0x16AC, u8 *) = D_0043DC70;
    PU(p, 0x1734, u8 *) = D_0047AEE8;
    PU(p, 0x1694, f32) = 8.0f;
    PU(p, 0x169C, f32) = 1.5f;
    PU(p, 0x1698, f32) = 12.0f;
    PU(p, 0x16A0, f32) = 1.5f;
}
