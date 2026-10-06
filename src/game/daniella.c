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
#include "heap.h"
#include "msl.h"
#include "input.h"
#include "event.h"
#include "vecmath.h"
#include "scene_game.h"
#include "lights.h"
#include "hewie.h"
#include "sound.h"
#include "effectmgr.h"
#include "debilitas.h"
#include "libc.h"
#include "effectmgr.h"   /* HitEffect_Spawn */
#include "effects.h"
#include "loader.h"
#include <stdint.h>
#include "item.h"
#include "renderer.h"
#include "charaction.h"
#include "gl2d.h"
#include "debilitas2.h"
#include "lorenzo.h"
#include "system.h"
#include "char_load.h"
#ifdef HG_NATIVE
#include <stdio.h>
#include <stdlib.h>
#include "glr.h"
#endif

extern void *Daniella_vtable[];

extern u8 D_003D73B0[];
void *Daniella_MotionFiles(void);

extern u32 D_0043DC50[];
extern u8 pstr_O_DNL_DNL_001_PCK_2[], pstr_O_DNL_DNL_001_PCK[];
extern u8 D_0043C260[], str_Z_11[], D_0043C2D0[], D_0043C210[], str_Z_10[], D_0043C2B0[];
extern u8 D_0043CC20[], D_0043CD08[], D_0043CC90[], D_0043CBD0[], str_Z_12[], D_0043CC70[];
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

extern void *Kind36_vtable[];
void Kind36_FilesLoaded(Pursuer *p);

extern u8 pstr_O_DNL_DNL_001_PCK_4[], pstr_O_DNL_DNL_001_PCK_3[];
f32 Kind36_FrightSeen(void);
f32 Kind36_ReachHewie(void);
void *Kind36_ModelFiles(void);

extern void *D_00470390[];
extern void *BonePoint_vtable[];
extern void *SprungPoint_vtable[];
void *BoneHangPoint_ctor(u8 *p);
void *IK2_ctor(u8 *p);
void *HairPoint_ctor(u8 *p);
void *HangingPart_ctor(u8 *p);
extern void *DaniellaModel_vtable[];
extern void *BoneHangPoint_ctor(u8 *p);
extern void *HairPoint_ctor(u8 *p);
extern void *HangingPart_ctor(u8 *p);
extern void *IK2_ctor(u8 *p);

extern void *Kind34_vtable[];
extern void *Kind35_vtable[];
extern u32 D_0043B6E0[];
extern u32 D_0043CDC0[];
Character *Kind35_dtor(Character *c, s32 flags);
s32 Kind35_Kind(void);
void *Kind35_MotionFiles(void);
Character *Kind34_dtor(Character *c, s32 flags);
s32 Kind34_Kind(void);
void *Kind34_MotionFiles(void);
void Kind34_FilesLoaded(Pursuer *p);
void Kind34_DoorOffset(void *self, s32 i, f32 *out);
void Kind34_ActionOffsets(void *self, s32 i, f32 *out);
static inline __attribute__((always_inline)) Character *creature_dtor(Character *c, s32 flags, void **vt);

extern u8 pstr_O_DNL_DNL_200_PCK_4[], pstr_O_DNL_DNL_200_PCK_3[];
extern u8 pstr_O_DNL_DNL_200_PCK_6[], pstr_O_DNL_DNL_200_PCK_5[];
extern u8 pstr_O_DNL_DNL_200_PCK_8[], pstr_O_DNL_DNL_200_PCK_7[];
#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

#define U32(p, off) (*(u32 *)((u8 *)(p) + (off)))

static inline s32 b5_prog_flag8000(void);

extern void *SpringPartBase_vtable[];
extern u8 D_00419E60[];
static inline void Set_AddLink(u8 *set, u8 *node) {
    if (AT(set, 0x30, u8 *) != NULL && AT(set, 0x34, u8 *) != NULL) {
        AT(AT(set, 0x34, u8 *), 0x28, u8 *) = node;
        AT(node, 0x28, u8 *) = NULL;
        AT(node, 0x2C, u8 *) = AT(set, 0x34, u8 *);
        AT(set, 0x34, u8 *) = node;
    } else {
        AT(set, 0x34, u8 *) = node;
        AT(set, 0x30, u8 *) = node;
        AT(node, 0x2C, u8 *) = NULL;
        AT(node, 0x28, u8 *) = NULL;
    }
}

static inline void Set_AddCollider(u8 *set, u8 *col) {
    AT(col, 0x2C, u8 *) = NULL;
    if (AT(set, 0x18, u8 *) == NULL) {
        AT(set, 0x18, u8 *) = col;
    } else {
        u8 *c = AT(set, 0x18, u8 *);

        while (AT(c, 0x2C, u8 *) != NULL) {
            c = AT(c, 0x2C, u8 *);
        }
        AT(c, 0x2C, u8 *) = col;
    }
}

/* a set's settings: force (x, y, z), damping, its model */
static inline void Set_Init(u8 *set, u8 *m, f32 fx, f32 fy, f32 fz, f32 damp) {
    AT(set, 0x0, f32) = fx;
    AT(set, 0x4, f32) = fy;
    AT(set, 0x8, f32) = fz;
    AT(set, 0x10, f32) = damp;
    AT(set, 0x14, u8 *) = m;
    AT(set, 0x20, u8) = 0;
    AT(set, 0x1C, s32) = 0;
}

/* a set's hanging parts put back under their anchors (a bone when +0x20, else the point at
   +0x2C), `len` along `dir`, at rest */
static inline void Parts_Rest(u8 *p, s32 n, s32 size, const f32 *dir, u8 *owner) {
    f32 at[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    s32 i;

    for (i = 0; i < n; i++, p += size) {
        AT(p, 0x18, f32) = 0.0f;
        AT(p, 0x14, f32) = 0.0f;
        AT(p, 0x10, f32) = 0.0f;
        if (AT(p, 0x20, u8) != 0) {
            sceVu0CopyVector(at, Skel_Bone(AT(owner, 0x810, u8 *), AT(p, 0x24, s32)) + 12);
        } else {
            sceVu0CopyVector(at, AT(p, 0x2C, f32 *));
        }
        sceVu0ScaleVector(d, (f32 *)dir, AT(p, 0x40, f32));
        sceVu0AddVector((f32 *)p, at, d);
        sceVu0CopyVector((f32 *)(p + 0x50), at);
    }
}

void *DaniellaModel_dtor(u8 *m, s32 flags);
void DaniellaModel_Vt2C(u8 *m);
void DaniellaModel_Vt30(u8 *m);
void DaniellaModel_SecondaryMotion(u8 *m);
s32 DaniellaModel_Part0(u8 *m);
s32 DaniellaModel_Part1(u8 *m);
s32 DaniellaModel_Part2(u8 *m);
s32 DaniellaModel_Part3(u8 *m);
void DaniellaModel_Frame(u8 *m);
void DaniellaModel_PartsA60(u8 *m);
void DaniellaModel_HairRest(u8 *m);
void DaniellaModel_Hair(u8 *m);
void DaniellaModel_PartA20(u8 *m);
void DaniellaModel_Hanging(u8 *m);
void DaniellaModel_HangingRest(u8 *m);
void DaniellaModel_Springs(u8 *m);
void DaniellaModel_Vt3C(u8 *m);
void DaniellaModel_Loaded(u8 *m);

/* gProgress+0x30 bit 0x8000 selects between two data sets (difficulty/mode flag?) */
static inline s32 b5_prog_flag8000(void) {
    return U32(gProgress, 0x30) & 0x8000;
}

static inline void *b0_RoomCtor(void *p, u32 id, s32 arg, void **vtbl) {
    FLD(p, 0x0, void **) = Actor_vtable;
    FLD(p, 0x20, s32) = arg;
    FLD(p, 0x24, s32) = 0x2000000;
    FLD(p, 0x0, void **) = Character_vtable;
    FLD(p, 0x1380, s32) = 0;
    FLD(p, 0x153C, u8) = (u8)id;
    FLD(p, 0x0, void **) = vtbl;
    return p;
}

/* destructor: own vtable -> Pursuer 0x46D810 -> NPC 0x46C220 -> Character; the model freed for
 * slots 3..5 */
static inline __attribute__((always_inline)) Character *creature_dtor(Character *c, s32 flags, void **vt) {
    if (c != NULL) {
        c->a.vtbl = vt;
        c->a.vtbl = Pursuer_vtable;
        VCALL(c, 0x10, void (*)(Character *))(c);
        if ((u32)c->a.slot >= 3 && (u32)c->a.slot < 6) {
            void **m = c->motion;

            if (m != NULL) {
                VCALL(m, 0x8, void (*)(void *, s32))(m, 1);
                c->motion = NULL;
            }
        }
        c->a.vtbl = NPC_vtable;
        VCALL(c, 0x10, void (*)(Character *))(c);
        c->a.vtbl = Character_vtable;
        c->a.vtbl = Actor_vtable;
        if ((s16)flags > 0) {
            Actor_Destroy(&c->a);
        }
    }
    return c;
}

Character *Kind36_dtor(Character *c, s32 flags);

/* 0x001733D0 */
void *Kind36_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x24, arg, Kind36_vtable);
}

/* 0x00173420 */
void *Kind35_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x23, arg, Kind35_vtable);
}

/* 0x00173470 */
void *Kind34_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x22, arg, Kind34_vtable);
}

/* 0x001734C0 */
void *Daniella_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x3, arg, Daniella_vtable);
}
/* vtable +0x8: destructor */
/* 0x0020C3A0 */
Pursuer *Daniella_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = Daniella_vtable;
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

extern u8 Daniella_Attack00_00[], Daniella_Attack00_02[], Daniella_Attack00_01[], Daniella_Attack00_03[], Daniella_Attack00_04[], Daniella_Attack00_05[],
    Daniella_Attack00_06[], Daniella_Attack00_07[], Daniella_Attack00_08[], Daniella_Attack00_09[], Daniella_Attack00_10[], Daniella_Attack00_11[],
    Daniella_Attack00_12[], Daniella_Attack00_13[], str_t_3[], Daniella_Attack00_14[], str_r_2[];
extern u8 Daniella_Attack01_00[], Daniella_Attack01_02[], Daniella_Attack01_01[], Daniella_Attack01_03[], Daniella_Attack01_04[], Daniella_Attack01_05[],
    Daniella_Attack01_06[], Daniella_Attack01_07[], Daniella_Attack01_08[], Daniella_Attack01_09[], Daniella_Attack01_10[], Daniella_Attack01_11[],
    Daniella_Attack01_12[], Daniella_Attack01_13[], Daniella_Attack01_16[], Daniella_Attack01_14[], Daniella_Attack01_15[];
extern u8 Daniella_Attack10_00[], Daniella_Attack10_02[], Daniella_Attack10_01[], Daniella_Attack10_03[], Daniella_Attack10_04[], Daniella_Attack10_05[],
    Daniella_Attack10_06[], Daniella_Attack10_07[], Daniella_Attack10_08[], Daniella_Attack10_09[], Daniella_Attack10_10[], Daniella_Attack10_11[],
    Daniella_Attack10_12[], Daniella_Attack10_13[], str_t_4[], str_r_3[], str_r_4[];
extern u8 Daniella_Attack11_00[], Daniella_Attack11_02[], Daniella_Attack11_01[], Daniella_Attack11_03[], Daniella_Attack11_04[], Daniella_Attack11_05[],
    Daniella_Attack11_06[], Daniella_Attack11_07[], Daniella_Attack11_08[], Daniella_Attack11_09[], Daniella_Attack11_10[], Daniella_Attack11_11[],
    Daniella_Attack11_12[], Daniella_Attack11_13[], Daniella_Attack11_16[], Daniella_Attack11_14[], Daniella_Attack11_15[];

/* her attack tables for situations 0..16: [gProgress+0x30 bit 0x8000][mode 2 (+0x16B8)] */
static u8 *const sAttackTables[2][2][17] = {
    {
        { Daniella_Attack00_00, Daniella_Attack00_01, Daniella_Attack00_02, Daniella_Attack00_03, Daniella_Attack00_04, Daniella_Attack00_05, Daniella_Attack00_06,
          Daniella_Attack00_07, Daniella_Attack00_08, Daniella_Attack00_09, Daniella_Attack00_10, Daniella_Attack00_11, Daniella_Attack00_12, Daniella_Attack00_13,
          Daniella_Attack00_14, str_r_2, str_t_3 },
        { Daniella_Attack01_00, Daniella_Attack01_01, Daniella_Attack01_02, Daniella_Attack01_03, Daniella_Attack01_04, Daniella_Attack01_05, Daniella_Attack01_06,
          Daniella_Attack01_07, Daniella_Attack01_08, Daniella_Attack01_09, Daniella_Attack01_10, Daniella_Attack01_11, Daniella_Attack01_12, Daniella_Attack01_13,
          Daniella_Attack01_14, Daniella_Attack01_15, Daniella_Attack01_16 },
    },
    {
        { Daniella_Attack10_00, Daniella_Attack10_01, Daniella_Attack10_02, Daniella_Attack10_03, Daniella_Attack10_04, Daniella_Attack10_05, Daniella_Attack10_06,
          Daniella_Attack10_07, Daniella_Attack10_08, Daniella_Attack10_09, Daniella_Attack10_10, Daniella_Attack10_11, Daniella_Attack10_12, Daniella_Attack10_13,
          str_r_3, str_r_4, str_t_4 },
        { Daniella_Attack11_00, Daniella_Attack11_01, Daniella_Attack11_02, Daniella_Attack11_03, Daniella_Attack11_04, Daniella_Attack11_05, Daniella_Attack11_06,
          Daniella_Attack11_07, Daniella_Attack11_08, Daniella_Attack11_09, Daniella_Attack11_10, Daniella_Attack11_11, Daniella_Attack11_12, Daniella_Attack11_13,
          Daniella_Attack11_14, Daniella_Attack11_15, Daniella_Attack11_16 },
    },
};

/* vtable +0x130: the attack table for a situation */
/* 0x0020CC70 */
void Daniella_AttackTable(Pursuer *p, s8 situation) {
    s32 alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;
    s32 mode2 = PU(p, 0x16B8, s32) == 2;

    PU(p, 0x1718, u8 *) = sAttackTables[alt][mode2][(u32)situation < 17 ? situation : 0];
}

extern u8 D_003D7E60[], D_003D7EB0[], D_003D7F00[], D_003D7F20[], str_Z_3[], str_Z_4[];
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
        PU(p, 0x1748, u8 *) = str_Z_4;
        PU(p, 0x1740, u8 *) = D_003D7F20;
    } else {
        PU(p, 0x16B8, s32) = 0;
        PU(p, 0x1730, u8 *) = D_003D7E60;
        PU(p, 0x1748, u8 *) = str_Z_3;
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
    if (Npc_InPlayedRoom(p) != 0) {
        Pursuer_DoorNear(p);
        if (Pursuer_CryHeard(p) != 0 || (Motion_EventFlags(p->c.motion, 0, -1, 1) & 0xFF & 0x20)) {
            Daniella_BlowEffect(p);
        }
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        VCALL(p, 0x110, void (*)(Pursuer *))(p);
        if (p->c.unk14D0 <= 0 || p->c.unk14D0 == 5) {
            Pursuer_AnimSounds(p, -1);
        }
        Npc_BoneHeight(p);
        Pursuer_KeepOnWalkable(p);
    } else {
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        Pursuer_FootstepsThroughWalls(p);
    }
    Stalker_ThinkEnd(p);
}

extern u8 D_003D7330[], pstr_O_DNL_DNL_200_PCK[], D_003D7370[], pstr_O_DNL_DNL_200_PCK_2[];

/* her model files (Pursuer_ModelFiles for kind 3) */
/* 0x0020D620 */
u8 *Daniella_ModelFileTable(Pursuer *p) {
    return (AT(gProgress, 0x30, u32) & 0x8000) ? pstr_O_DNL_DNL_200_PCK_2 : pstr_O_DNL_DNL_200_PCK;
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
        PU(p, 0x1748, u8 *) = str_Z_3;
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
/* 0x0020D8D0 */
void *Pair_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, s32) = 0;
        AT(o, 0x4, s32) = 0;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* destructor (vtable ?) */
/* 0x0020D920 */
void *Pair44_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x44, s32) = 0;
        AT(o, 0x48, s32) = 0;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* destructor (vtable ?) */
/* 0x0020D970 */
void *Triple_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, s32) = 0;
        AT(o, 0x4, s32) = 0;
        AT(o, 0x8, s32) = 0;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* destructor (vtable ?) */
/* 0x0020D9C0 */
void *Quad4_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x10, s32) = 0;
        AT(o, 0xC, s32) = 0;
        AT(o, 0x4, s32) = 0;
        AT(o, 0x8, s32) = 0;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* +0x8: destructor */
/* 0x002ECFD0 */
void *DaniellaModel_dtor(u8 *m, s32 flags) {
    if (m != NULL) {
        AT(m, 0x0, void **) = DaniellaModel_vtable;
        __destroy_arr(m + 0x14D0, BoneHangPoint_dtor, 0x50, 2);
        AT(m, 0x14A0, void **) = SprungPoint_vtable;
        AT(m, 0x14A0, void **) = SpringPartBase_vtable;
        __destroy_arr(m + 0xDE0, HairPoint_dtor, 0x70, 0xA);
        __destroy_arr(m + 0xAA0, HangingPart_dtor, 0x50, 6);
        HumanModel_Destroy(m, flags);
    }
    return m;
}

/* +0x2C / +0x30: two of her parts' draw flags (+0xBA / +0xD6) and model flag 0x20000 on; off
   again by +0x878 (1 or 2 in +0x880) after the base +0x2C */
/* 0x002ED160 */
void DaniellaModel_Vt2C(u8 *m) {
    AT(m, 0xBA, u8) |= 2;
    AT(m, 0xD6, u8) |= 2;
    AT(m, 0x4B0, u32) |= 0x20000;
}

/* 0x002ED190 */
void DaniellaModel_Vt30(u8 *m) {
    VCALL(m, 0x2C, void (*)(u8 *))(m);
    if (AT(m, 0x878, s32) == 0) {
        AT(m, 0xBA, u8) &= 0xFD;
        AT(m, 0x880, s32) = 1;
    } else {
        AT(m, 0xD6, u8) &= 0xFD;
        AT(m, 0x880, s32) = 2;
    }
    AT(m, 0x4B0, u32) &= ~0x20000;
}

/* +0xB4: her secondary-motion table */
/* 0x002ED210 */
void DaniellaModel_SecondaryMotion(u8 *m) {
    AT(m, 0x874, u8 *) = D_00419E60;
}

/* +0x84 .. +0x90: her mesh parts */
/* 0x002ED220 */
s32 DaniellaModel_Part0(u8 *m) {
    return 3;
}

/* 0x002ED230 */
s32 DaniellaModel_Part1(u8 *m) {
    return 7;
}

/* 0x002ED240 */
s32 DaniellaModel_Part2(u8 *m) {
    return 0x1A;
}

/* 0x002ED250 */
s32 DaniellaModel_Part3(u8 *m) {
    return 0x2A;
}

/* her five back capsules (bone 2 to bone 6) by pose: 0 standing, 1 flat, 2 bent (crawling) */
/* 0x002ED260 */
void DaniellaModel_BackCapsules(u8 *m, s32 pose) {
    static const f32 sCaps[3][5][7] = {
        {
            {0.0f, 1.0f, 1.0f, 0x1.99999ap+0f, 0.0f, 1.0f, -1.0f},
            {1.0f, 1.0f, 1.0f, 0x1.b33334p+0f, 1.0f, 1.0f, -1.0f},
            {2.0f, 1.0f, 1.0f, 0x1.ccccccp+0f, 2.0f, 1.0f, -1.0f},
            {3.0f, 1.0f, 1.0f, 0x1.e66666p+0f, 3.0f, 1.0f, -1.0f},
            {4.0f, 1.0f, 1.0f, 2.0f, 4.0f, 1.0f, -1.0f},
        },
        {
            {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f},
            {2.0f, 0.0f, 0.0f, 1.0f, 2.0f, 0.0f, 0.0f},
            {3.0f, 0.0f, 0.0f, 1.0f, 3.0f, 0.0f, 0.0f},
            {4.0f, 0.0f, 0.0f, 1.0f, 4.0f, 0.0f, 0.0f},
        },
        {
            {0.0f, 1.0f, -1.0f, 2.0f, 0.0f, 1.0f, 1.0f},
            {1.0f, 1.0f, -1.0f, 2.0f, 1.0f, 1.0f, 1.0f},
            {2.0f, 1.0f, -1.0f, 2.0f, 2.0f, 1.0f, 1.0f},
            {3.0f, 1.0f, -1.0f, 2.0f, 3.0f, 1.0f, 1.0f},
            {4.0f, 1.0f, -1.0f, 2.0f, 4.0f, 1.0f, 1.0f},
        },
    };
    const f32 (*c)[7] = sCaps[pose == 1 ? 1 : pose == 2 ? 2 : 0];
    s32 k;

    AT(m, 0x1578, s8) = pose;
    for (k = 0; k < 5; k++) {
        Capsule_Set(m + 0x1240 + k * 0x70, 2, 6, c[k][0], c[k][1], c[k][2], c[k][3], c[k][4], c[k][5], c[k][6]);
    }
}

/* the two parts on the set +0xA60 (bones 0x2F, 0x30) */
/* 0x002ED600 */
void DaniellaModel_PartsA60(u8 *m) {
    s32 i;

    SpringSet_Clear(m + 0xA60);
    for (i = 0; i < 2; i++) {
        Set_AddLink(m + 0xA60, m + 0x14D0 + i * 0x50);
    }
    Set_Init(m + 0xA60, m, 0.0f, 0x1.99999ap-4f /* 0.1 */, 0.0f, 0x1.fae148p-1f /* 0.99 */);
    AT(m, 0x1510, f32) = 1.0f;
    AT(m, 0x14F4, s32) = 0x2F;
    AT(m, 0x14F0, u8) = 1;
    AT(m, 0x1560, f32) = 1.0f;
    AT(m, 0x1544, s32) = 0x30;
    AT(m, 0x1540, u8) = 1;
}

/* her hair at rest: each point its length (+0x40) along bone 0's Z axis from its anchor */
/* 0x002ED6E0 */
void DaniellaModel_HairRest(u8 *m) {
    f32 down[4] __attribute__((aligned(16)));

    sceVu0CopyVector(down, Skel_Bone(AT(AT(m, 0x9B4, u8 *), 0x810, u8 *), 0) + 8);
    Parts_Rest(m + 0xDE0, 10, 0x70, down, AT(m, 0x9B4, u8 *));
}

/* her hair: the two strands (bones 0xB..0xF and 0x10..0x14), each point tied to the one beside
   it in the other strand, stiffer at the root; the five capsules */
/* 0x002ED7C0 */
void DaniellaModel_Hair(u8 *m) {
    static const f32 sStiff[5] = {
        0x1.99999ap-1f, 0x1.333334p-1f, 0x1.99999ap-2f, 0x1.99999ap-3f, 0.0f,   /* 0.8 .. 0 */
    };
    s32 i;

    SpringSet_Clear(m + 0x9A0);
    for (i = 0; i < 10; i++) {
        Set_AddLink(m + 0x9A0, m + 0xDE0 + i * 0x70);
    }
    for (i = 0; i < 5; i++) {
        Set_AddCollider(m + 0x9A0, m + 0x1240 + i * 0x70);
    }
    Set_Init(m + 0x9A0, m, 0.0f, 0x1.99999ap-1f /* 0.8 */, 0.0f, 0.5f);
    for (i = 0; i < 10; i++) {
        u8 *n = m + 0xDE0 + i * 0x70;

        AT(n, 0x20, u8) = i % 5 == 0;
        AT(n, 0x24, s32) = 0xB + i;
        AT(n, 0x40, f32) = 0x1.333334p+0f;   /* 1.2 */
        AT(n, 0x44, u8 *) = m + 0xDE0 + (i < 5 ? i + 5 : i - 5) * 0x70;
        AT(n, 0x48, f32) = i < 5 ? -1.0f : 1.0f;
        AT(n, 0x60, f32) = sStiff[i % 5];
    }
    DaniellaModel_BackCapsules(m, 0);
}

/* the one part on the set +0xA20 (bone 0x17) */
/* 0x002EDA90 */
void DaniellaModel_PartA20(u8 *m) {
    SpringSet_Clear(m + 0xA20);
    Set_AddLink(m + 0xA20, m + 0x1470);
    Set_Init(m + 0xA20, m, 0.0f, 0.0f, 0.0f, 0.75f);
    AT(m, 0x14B0, f32) = 0x1.cccccc0p-1f;   /* 0.9 */
    AT(m, 0x1494, s32) = 0x17;
    AT(m, 0x1490, u8) = 1;
    AT(m, 0x14C8, f32) = 1.0f;
    AT(m, 0x14C4, f32) = 1.0f;
    AT(m, 0x14C0, f32) = 1.0f;
}

/* the six hanging parts (bones 0x22..0x27, two strands of three) on the set +0x9E0, with two
   capsules and two spheres */
/* 0x002EDB50 */
void DaniellaModel_Hanging(u8 *m) {
    static const f32 sStiff[3] = {0x1.99999ap-3f, 0x1.99999ap-4f, 0.0f};   /* 0.2, 0.1, 0 */
    s32 i;

    SpringSet_Clear(m + 0x9E0);
    for (i = 0; i < 6; i++) {
        Set_AddLink(m + 0x9E0, m + 0xAA0 + i * 0x50);
    }
    for (i = 0; i < 2; i++) {
        Set_AddCollider(m + 0x9E0, m + 0xC80 + i * 0x70);
    }
    for (i = 0; i < 2; i++) {
        Set_AddCollider(m + 0x9E0, m + 0xD60 + i * 0x40);
    }
    Set_Init(m + 0x9E0, m, 0.0f, 0x1.333334p-2f /* 0.3 */, 0.0f, 0.5f);
    for (i = 0; i < 6; i++) {
        u8 *n = m + 0xAA0 + i * 0x50;

        AT(n, 0x20, u8) = i % 3 == 0;
        AT(n, 0x24, s32) = 0x22 + i;
        AT(n, 0x40, f32) = 0x1.99999ap-1f;   /* 0.8 */
        AT(n, 0x44, f32) = sStiff[i % 3];
    }
    Capsule_Set(m + 0xC80, 0x19, 0x29, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
    Capsule_Set(m + 0xCF0, 0x17, 0x17, 1.5f, 0.0f, -0.5f, 1.0f, -1.5f, 0.0f, -0.5f);
    Sphere_Set(m + 0xD60, 0x1F, 0.0f, 0.0f, 0.0f, 1.0f);
    Sphere_Set(m + 0xDA0, 0x1F, 0.0f, 1.0f, 0.0f, 1.0f);
}

/* the six hanging parts at rest: each its length (+0x40) along bone 0x1F's Z axis bent by the
   set's force (+0x44 of it), from its anchor */
/* 0x002EDE30 */
void DaniellaModel_HangingRest(u8 *m) {
    f32 at[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    u8 *p = m + 0xAA0;
    s32 i;

    for (i = 0; i < 6; i++, p += 0x50) {
        AT(p, 0x18, f32) = 0.0f;
        AT(p, 0x14, f32) = 0.0f;
        AT(p, 0x10, f32) = 0.0f;
        if (AT(p, 0x20, u8) != 0) {
            sceVu0CopyVector(at, Skel_Bone(AT(AT(m, 0x9F4, u8 *), 0x810, u8 *), AT(p, 0x24, s32)) + 12);
        } else {
            sceVu0CopyVector(at, AT(p, 0x2C, f32 *));
        }
        sceVu0CopyVector(d, Skel_Bone(AT(AT(m, 0x9F4, u8 *), 0x810, u8 *), 0x1F) + 8);
        sceVu0ScaleVector(d, d, AT(p, 0x44, f32));
        sceVu0AddVector(d, d, (f32 *)(m + 0x9E0));
        sceVu0Normalize(d, d);
        sceVu0ScaleVector(d, d, AT(p, 0x40, f32));
        sceVu0AddVector((f32 *)p, at, d);
    }
}

/* all her springs */
/* 0x002EDF30 */
void DaniellaModel_Springs(u8 *m) {
    DaniellaModel_Hanging(m);
    DaniellaModel_PartA20(m);
    DaniellaModel_Hair(m);
    DaniellaModel_PartsA60(m);
    AT(m, 0x850, u8) = 1;
}

/* +0x3C: her springs a frame. While she crawls (0x1800..0x1803) the capsules bend and her hair
   falls forward (the force -0.4 along her facing); after a reset (+0x850) everything back at
   rest and settled (+0x1570 / +0x1574 steps) */
/* 0x002EDF80 */
void DaniellaModel_Vt3C(u8 *m) {
    s32 n = 1, nHair = 1;
    s32 i;

    if ((u32)(AT(m, 0x55C, s32) - 0x1800) < 4) {
        if (AT(m, 0x1578, s8) != 2) {
            DaniellaModel_BackCapsules(m, 2);
            AT(m, 0x9A0, f32) = -0x1.99999ap-2f * AT(m, 0x7F0, f32);
            AT(m, 0x9A8, f32) = -0x1.99999ap-2f * AT(m, 0x7F8, f32);
        }
    } else if (AT(m, 0x1578, s8) == 2) {
        DaniellaModel_BackCapsules(m, 0);
        AT(m, 0x9A0, s32) = 0;
        AT(m, 0x9A8, s32) = 0;
    }
    if (AT(m, 0x850, u8) != 0) {
        DaniellaModel_HairRest(m);
        DaniellaModel_HangingRest(m);
        nHair = AT(m, 0x1574, s32);
        n = AT(m, 0x1570, s32);
    }
    SpringSet_Begin(m + 0x9E0);
    SpringSet_Begin(m + 0xA20);
    SpringSet_Begin(m + 0x9A0);
    SpringSet_Begin(m + 0xA60);
    for (i = 0; i < n; i++) {
        SpringSet_Step(m + 0x9E0);
        SpringSet_Step(m + 0xA20);
        SpringSet_Step(m + 0xA60);
    }
    for (i = 0; i < nHair; i++) {
        SpringSet_Step(m + 0x9A0);
    }
    SpringSet_Finish(m + 0x9E0);
    SpringSet_Finish(m + 0xA20);
    SpringSet_Finish(m + 0x9A0);
    SpringSet_Finish(m + 0xA60);
    AT(m, 0x850, u8) = 0;
}

/* +0x10 */
/* 0x002EE110 */
void DaniellaModel_Frame(u8 *m) {
    Model_Release(m);
}

/* +0xC: once loaded: the base setup, the part roles, per-part draw settings, her springs */
/* 0x002EE120 */
void DaniellaModel_Loaded(u8 *m) {
    HumanModel_Loaded(m);
    AT(m, 0x890, s32) = 2;
    AT(m, 0x894, s32) = 3;
    AT(m, 0x898, s32) = 4;
    AT(m, 0x89C, s32) = 5;
    AT(m, 0x8B8, s32) = 0x1C;
    AT(m, 0x8A0, s32) = 6;
    AT(m, 0x8A4, s32) = 7;
    AT(m, 0x8A8, s32) = 8;
    AT(m, 0x8AC, s32) = 9;
    AT(m, 0x8BC, s32) = 0x2C;
    AT(m, 0x8B0, s32) = 0x1F;
    AT(m, 0x8B4, s32) = 0x15;
    AT(m, 0x860, f32) = 0.0f;
    AT(m, 0x864, f32) = 16.0f;
    AT(m, 0x868, f32) = 0.0f;
    AT(m, 0x854, s32) = 0;
    AT(m, 0x858, s32) = 0;
    {
        static const u8 sParts[][2] = {
            {0x98, 0x40}, {0xCC, 0x40}, {0xDA, 0x40}, {0xDC, 0x40}, {0xDE, 0x40},
            {0xD2, 0xC0}, {0xBA, 0xFF}, {0xD6, 0xFF},
        };
        u32 i;

        for (i = 0; i < sizeof(sParts) / sizeof(sParts[0]); i++) {
            AT(m, sParts[i][0], u8) = 4;
            AT(m, sParts[i][0] + 1, u8) = sParts[i][1];
        }
    }
    AT(m, 0x1570, s32) = 50;
    AT(m, 0x1574, s32) = 50;
    DaniellaModel_Springs(m);
}

/* 0x00345EE0 */
Character *Kind34_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind34_vtable); }

/* 0x00345FF0 */
s32 Kind34_Kind(void) {
    return 0x22;
}

/* 0x00346000 */
void *Kind34_MotionFiles(void) {
    return D_0043B6E0;
}

/* (a pursuer class) Pursuer_FilesLoaded, then its model's +0x34 (1) */
/* 0x00346010 */
void Kind34_FilesLoaded(Pursuer *p) {
    Pursuer_FilesLoaded(p);
    VCALL(p->c.motion, 0x34, void (*)(void *, s32))(p->c.motion, 1);
}

/* Writes a position {0, 0, z} for index 0..3. */
/* 0x00346050 */
void Kind34_DoorOffset(void *self, s32 i, f32 *out) {
    switch (i) {
    case 1: out[0] = 0.0f; out[1] = 0.0f; out[2] = 0x1.be824p+2f /* 6.9767 */; break;
    case 3: out[0] = 0.0f; out[1] = 0.0f; out[2] = -0x1.905f06p+2f /* -6.2558 */; break;
    case 0: out[0] = 0.0f; out[1] = 0.0f; out[2] = -0x1.bdc432p+2f /* -6.9651 */; break;
    case 2: out[0] = 0.0f; out[1] = 0.0f; out[2] = 0x1.ce0418p+2f /* 7.219 */; break;
    }
}

/* Writes a position {x, 0, z} for index 10..15. */
/* 0x003460F0 */
void Kind34_ActionOffsets(void *self, s32 i, f32 *out) {
    switch (i) {
    case 10: case 11: out[0] = 0x1.07c84cp-2f /* 0.2576 */; out[1] = 0.0f; out[2] = 0x1.567fccp+3f /* 10.7031 */; break;
    case 12: case 13: out[0] = 0x1.a4a8c2p+0f /* 1.6432 */; out[1] = 0.0f; out[2] = 0x1.5d182ap+3f /* 10.9092 */; break;
    case 14: out[0] = -0x1.5f06f6p-3f /* -0.1714 */; out[1] = 0.0f; out[2] = -0x1.8f6fd2p+1f /* -3.1206 */; break;
    case 15: out[0] = 0x1.9a0276p-2f /* 0.4004 */; out[1] = 0.0f; out[2] = -0x1.792d78p+1f /* -2.9467 */; break;
    }
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
    if (Npc_InPlayedRoom(p) != 0) {
        Pursuer_DoorNear(p);
        if (Pursuer_CryHeard(p) != 0 || (Motion_EventFlags(p->c.motion, 0, -1, 1) & 0xFF & 0x20)) {
            Kind34_BlowEffect(p);
        }
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        VCALL(p, 0x110, void (*)(Pursuer *))(p);
        if (p->c.unk14D0 <= 0 || p->c.unk14D0 == 5) {
            Pursuer_AnimSounds(p, -1);
        }
        Npc_BoneHeight(p);
        Pursuer_KeepOnWalkable(p);
    } else {
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        Pursuer_FootstepsThroughWalls(p);
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
    if (Npc_InPlayedRoom(p) != 0) {
        Pursuer_DoorNear(p);
        if (Pursuer_CryHeard(p) != 0 || (Motion_EventFlags(p->c.motion, 0, -1, 1) & 0xFF & 0x20)) {
            Kind35_BlowEffect(p);
        }
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        VCALL(p, 0x110, void (*)(Pursuer *))(p);
        if (p->c.unk14D0 <= 0 || p->c.unk14D0 == 5) {
            Pursuer_AnimSounds(p, -1);
        }
        Npc_BoneHeight(p);
        Pursuer_KeepOnWalkable(p);
    } else {
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        Pursuer_FootstepsThroughWalls(p);
    }
    Stalker_ThinkEnd(p);
}

/* 0x00348620 */
u8 *Kind35_ModelFileTable(Pursuer *p) {
    return (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) ? pstr_O_DNL_DNL_200_PCK_6 : pstr_O_DNL_DNL_200_PCK_5;
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
    if (Npc_InPlayedRoom(p) != 0) {
        Pursuer_DoorNear(p);
        if (Pursuer_CryHeard(p) != 0 || (Motion_EventFlags(p->c.motion, 0, -1, 1) & 0xFF & 0x20)) {
            Kind36_BlowEffect(p);
        }
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        VCALL(p, 0x110, void (*)(Pursuer *))(p);
        if (p->c.unk14D0 <= 0 || p->c.unk14D0 == 5) {
            Pursuer_AnimSounds(p, -1);
        }
        Npc_BoneHeight(p);
        Pursuer_KeepOnWalkable(p);
    } else {
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        Pursuer_FootstepsThroughWalls(p);
    }
    Stalker_ThinkEnd(p);
}

/* 0x003495B0 */
u8 *Kind36_ModelFileTable(Pursuer *p) {
    return (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) ? pstr_O_DNL_DNL_200_PCK_8 : pstr_O_DNL_DNL_200_PCK_7;
}

/* 0x003495F0 */
void *Kind36_ModelFiles(void) {
    return (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) ? pstr_O_DNL_DNL_001_PCK_4 : pstr_O_DNL_DNL_001_PCK_3;
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

extern u8 Kind34_Attack00_00[], Kind34_Attack00_02[], Kind34_Attack00_01[], Kind34_Attack00_03[], Kind34_Attack00_04[], Kind34_Attack00_05[],
    Kind34_Attack00_06[], Kind34_Attack00_07[], Kind34_Attack00_08[], Kind34_Attack00_09[], Kind34_Attack00_10[], Kind34_Attack00_11[],
    Kind34_Attack00_12[], Kind34_Attack00_13[], str_t_17[], str_r_10[], str_r_11[], Kind34_Attack01_00[],
    Kind34_Attack01_02[], Kind34_Attack01_01[], Kind34_Attack01_03[], Kind34_Attack01_04[], Kind34_Attack01_05[], Kind34_Attack01_06[],
    Kind34_Attack01_07[], Kind34_Attack01_08[], Kind34_Attack01_09[], Kind34_Attack01_10[], Kind34_Attack01_11[], Kind34_Attack01_12[],
    Kind34_Attack01_13[], Kind34_Attack01_16[], Kind34_Attack01_14[], Kind34_Attack01_15[], Kind34_Attack10_00[], Kind34_Attack10_02[],
    Kind34_Attack10_01[], Kind34_Attack10_03[], Kind34_Attack10_04[], Kind34_Attack10_05[], Kind34_Attack10_06[], Kind34_Attack10_07[],
    Kind34_Attack10_08[], Kind34_Attack10_09[], Kind34_Attack10_10[], Kind34_Attack10_11[], Kind34_Attack10_12[], Kind34_Attack10_13[],
    str_t_18[], Kind34_Attack10_14[], Kind34_Attack10_15[], Kind34_Attack11_00[], Kind34_Attack11_02[], Kind34_Attack11_01[],
    Kind34_Attack11_03[], Kind34_Attack11_04[], Kind34_Attack11_05[], Kind34_Attack11_06[], Kind34_Attack11_07[], Kind34_Attack11_08[],
    Kind34_Attack11_09[], Kind34_Attack11_10[], Kind34_Attack11_11[], Kind34_Attack11_12[], Kind34_Attack11_13[], Kind34_Attack11_16[],
    Kind34_Attack11_14[], Kind34_Attack11_15[];

/* the first's attack tables: [gProgress+0x30 bit 0x8000][mode 2 (+0x16B8)] */
static u8 *const sAttackTables1[2][2][17] = {
    {
        { Kind34_Attack00_00, Kind34_Attack00_01, Kind34_Attack00_02, Kind34_Attack00_03, Kind34_Attack00_04, Kind34_Attack00_05, Kind34_Attack00_06,
          Kind34_Attack00_07, Kind34_Attack00_08, Kind34_Attack00_09, Kind34_Attack00_10, Kind34_Attack00_11, Kind34_Attack00_12, Kind34_Attack00_13,
          str_r_10, str_r_11, str_t_17 },
        { Kind34_Attack01_00, Kind34_Attack01_01, Kind34_Attack01_02, Kind34_Attack01_03, Kind34_Attack01_04, Kind34_Attack01_05, Kind34_Attack01_06,
          Kind34_Attack01_07, Kind34_Attack01_08, Kind34_Attack01_09, Kind34_Attack01_10, Kind34_Attack01_11, Kind34_Attack01_12, Kind34_Attack01_13,
          Kind34_Attack01_14, Kind34_Attack01_15, Kind34_Attack01_16 },
    },
    {
        { Kind34_Attack10_00, Kind34_Attack10_01, Kind34_Attack10_02, Kind34_Attack10_03, Kind34_Attack10_04, Kind34_Attack10_05, Kind34_Attack10_06,
          Kind34_Attack10_07, Kind34_Attack10_08, Kind34_Attack10_09, Kind34_Attack10_10, Kind34_Attack10_11, Kind34_Attack10_12, Kind34_Attack10_13,
          Kind34_Attack10_14, Kind34_Attack10_15, str_t_18 },
        { Kind34_Attack11_00, Kind34_Attack11_01, Kind34_Attack11_02, Kind34_Attack11_03, Kind34_Attack11_04, Kind34_Attack11_05, Kind34_Attack11_06,
          Kind34_Attack11_07, Kind34_Attack11_08, Kind34_Attack11_09, Kind34_Attack11_10, Kind34_Attack11_11, Kind34_Attack11_12, Kind34_Attack11_13,
          Kind34_Attack11_14, Kind34_Attack11_15, Kind34_Attack11_16 },
    },
};

/* the first: vtable +0x130: the attack table for a situation; in the ending situations 15 and
   14 have their own */
/* 0x003467D0 */
void Kind34_AttackTable(Pursuer *p, s8 situation) {
    s32 alt, mode2;

    if (AT(gProgress, 0x1FBEC1, u8) != 0) {
        if (situation == 15) {
            PU(p, 0x1718, u8 *) = Kind34_Attack01_15;
            return;
        }
        if (situation == 14) {
            PU(p, 0x1718, u8 *) = Kind34_Attack01_14;
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
            *(void **)(p + 0x1748) = str_Z_12;
            *(void **)(p + 0x1740) = D_0043CC70;
        }
    } else {
        if (alt) {
            *(s32 *)(p + 0x16B8) = 2;
            *(void **)(p + 0x1730) = D_0043C260;
            *(void **)(p + 0x1748) = str_Z_11;
            *(void **)(p + 0x1740) = D_0043C2D0;
        } else {
            *(s32 *)(p + 0x16B8) = 0;
            *(void **)(p + 0x1730) = D_0043C210;
            *(void **)(p + 0x1748) = str_Z_10;
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

/* 0x00347290 */
u8 *Kind34_ModelFileTable(Pursuer *p) {
    return (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) ? pstr_O_DNL_DNL_200_PCK_4 : pstr_O_DNL_DNL_200_PCK_3;
}

/* 0x003472D0 */
void *Kind34_ModelFiles(void) {
    return (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) ? pstr_O_DNL_DNL_001_PCK_2 : pstr_O_DNL_DNL_001_PCK;
}

extern u8 D_0043B700[], D_0043B840[], D_0043B860[], D_0043BA40[], D_0043BA70[], D_0043C210[],
    D_0043C2B0[], D_0043C2F0[], str_Z_10[], D_0043CBD0[], D_0043CC70[], D_0043CCB0[],
    str_Z_12[], D_0047AED8[];

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
        PU(p, 0x1748, u8 *) = str_Z_12;
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
        PU(p, 0x1748, u8 *) = str_Z_10;
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

/* 0x003478C0 */
Character *Kind35_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind35_vtable); }

/* 0x003479D0 */
s32 Kind35_Kind(void) {
    return 0x23;
}

/* 0x003479E0 */
void *Kind35_MotionFiles(void) {
    return D_0043CDC0;
}

extern u8 Kind35_Attack0_00[], Kind35_Attack0_02[], Kind35_Attack0_01[], Kind35_Attack0_03[], Kind35_Attack0_04[], Kind35_Attack0_05[],
    Kind35_Attack0_06[], Kind35_Attack0_07[], Kind35_Attack0_08[], Kind35_Attack0_09[], Kind35_Attack0_10[], Kind35_Attack0_11[],
    Kind35_Attack0_12[], Kind35_Attack0_13[], str_t_19[], str_r_12[], str_r_13[], Kind35_Attack1_00[],
    Kind35_Attack1_02[], Kind35_Attack1_01[], Kind35_Attack1_03[], Kind35_Attack1_04[], Kind35_Attack1_05[], Kind35_Attack1_06[],
    Kind35_Attack1_07[], Kind35_Attack1_08[], Kind35_Attack1_09[], Kind35_Attack1_10[], Kind35_Attack1_11[], Kind35_Attack1_12[],
    Kind35_Attack1_13[], str_t_20[], str_r_14[], str_r_15[];

/* the second's attack tables; the second set when gProgress+0x30 bit 0x8000 */
static u8 *const sAttackTables2[2][17] = {
    { Kind35_Attack0_00, Kind35_Attack0_01, Kind35_Attack0_02, Kind35_Attack0_03, Kind35_Attack0_04, Kind35_Attack0_05, Kind35_Attack0_06,
      Kind35_Attack0_07, Kind35_Attack0_08, Kind35_Attack0_09, Kind35_Attack0_10, Kind35_Attack0_11, Kind35_Attack0_12, Kind35_Attack0_13,
      str_r_12, str_r_13, str_t_19 },
    { Kind35_Attack1_00, Kind35_Attack1_01, Kind35_Attack1_02, Kind35_Attack1_03, Kind35_Attack1_04, Kind35_Attack1_05, Kind35_Attack1_06,
      Kind35_Attack1_07, Kind35_Attack1_08, Kind35_Attack1_09, Kind35_Attack1_10, Kind35_Attack1_11, Kind35_Attack1_12, Kind35_Attack1_13,
      str_r_14, str_r_15, str_t_20 },
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
Character *Kind36_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind36_vtable); }

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

extern u8 Kind36_Attack0_00[], Kind36_Attack0_02[], Kind36_Attack0_01[], Kind36_Attack0_03[], Kind36_Attack0_04[], Kind36_Attack0_05[],
    Kind36_Attack0_06[], Kind36_Attack0_07[], Kind36_Attack0_08[], Kind36_Attack0_09[], Kind36_Attack0_10[], Kind36_Attack0_11[],
    Kind36_Attack0_12[], Kind36_Attack0_13[], str_t_21[], str_r_16[], str_r_17[], Kind36_Attack1_00[],
    Kind36_Attack1_02[], Kind36_Attack1_01[], Kind36_Attack1_03[], Kind36_Attack1_04[], Kind36_Attack1_05[], Kind36_Attack1_06[],
    Kind36_Attack1_07[], Kind36_Attack1_08[], Kind36_Attack1_09[], Kind36_Attack1_10[], Kind36_Attack1_11[], Kind36_Attack1_12[],
    Kind36_Attack1_13[], str_t_22[], str_r_18[], str_r_19[];

/* the third's attack tables */
static u8 *const sAttackTables3[2][17] = {
    { Kind36_Attack0_00, Kind36_Attack0_01, Kind36_Attack0_02, Kind36_Attack0_03, Kind36_Attack0_04, Kind36_Attack0_05, Kind36_Attack0_06,
      Kind36_Attack0_07, Kind36_Attack0_08, Kind36_Attack0_09, Kind36_Attack0_10, Kind36_Attack0_11, Kind36_Attack0_12, Kind36_Attack0_13,
      str_r_16, str_r_17, str_t_21 },
    { Kind36_Attack1_00, Kind36_Attack1_01, Kind36_Attack1_02, Kind36_Attack1_03, Kind36_Attack1_04, Kind36_Attack1_05, Kind36_Attack1_06,
      Kind36_Attack1_07, Kind36_Attack1_08, Kind36_Attack1_09, Kind36_Attack1_10, Kind36_Attack1_11, Kind36_Attack1_12, Kind36_Attack1_13,
      str_r_18, str_r_19, str_t_22 },
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
    D_0043E4E0[], str_x_2[], D_0043E990[], D_0043E9E0[], D_0043EA00[], str_x_3[],
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
        PU(p, 0x1748, u8 *) = str_x_3;
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
        PU(p, 0x1748, u8 *) = str_x_2;
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

/* a model on the full base with its two parts, vtable DaniellaModel_vtable: 4 members (0x40, +0x9A0,
 * their +0x30 / +0x34 cleared), 6 parts (0x50, +0xAA0), 2 + 2 members (0x70 at +0xC80, 0x40 at
 * +0xD60), 10 parts (0x70, +0xDE0), 5 members (0x70, +0x1240), one (+0x1470, vtable at +0x30)
 * and 2 parts (0x50, +0x14D0) */
/* 0x0038D160 */
void *DaniellaModel_ctor(u8 *m) {
    u8 *e;

    ModelBase_ctor(m);
    AT(m, 0x0, void **) = HumanModel_vtable;
    IK2_ctor(m + 0x8D0);
    IK2_ctor(m + 0x930);
    AT(m, 0x0, void **) = DaniellaModel_vtable;
    for (e = m + 0x9A0; e < m + 0xAA0; e += 0x40) {
        AT(e, 0x34, s32) = 0;
        AT(e, 0x30, s32) = 0;
    }
    __construct_array(m + 0xAA0, HangingPart_ctor, HangingPart_dtor, 0x50, 6);
    for (e = m + 0xC80; e < m + 0xD60; e += 0x70) {
        AT(e, 0x30, void **) = D_00470390;
    }
    for (e = m + 0xD60; e < m + 0xDE0; e += 0x40) {
        AT(e, 0x30, void **) = BonePoint_vtable;
    }
    __construct_array(m + 0xDE0, HairPoint_ctor, HairPoint_dtor, 0x70, 0xA);
    for (e = m + 0x1240; e < m + 0x1470; e += 0x70) {
        AT(e, 0x30, void **) = D_00470390;
    }
    AT(m, 0x14A0, void **) = SprungPoint_vtable;
    __construct_array(m + 0x14D0, BoneHangPoint_ctor, BoneHangPoint_dtor, 0x50, 2);
    return m;
}
