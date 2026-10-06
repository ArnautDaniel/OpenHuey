/* Riccardo: the Pursuer of the later chapters (kind 4, vtable 0x46F6B0; code 0x2D7A60..0x2DC660).
 * His object is a Pursuer with 0x40 bytes more (0x1840; +0x1824 a table of his). He has a rage
 * (mode 2, +0x16B8) at the top threat level, his own reactions to cries, and his own
 * behaviours. See pursuer.h. */
#include "common.h"
#include "pursuer.h"
#include "progress.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "navmesh.h"
#include "actor.h"
#include "ptmf.h"
#include "game.h"
#include "fiona.h"
#include "hewie.h"
#include "model.h"
#include "riccardo.h"
#include "scene_game.h"
#include "heap.h"
#include "vecmath.h"
#include "msl.h"
#include "input.h"
#include "memcard.h"
#include "event.h"
#include "lights.h"
#include "sound.h"
#include "effectmgr.h"
#include "debilitas.h"
#include "libc.h"
#include "effectmgr.h"   /* HitEffect_Spawn */
#include "item.h"
#include "renderer.h"
#include "charaction.h"
#include "gl2d.h"
#include "daniella.h"
#include "debilitas2.h"
#include "lorenzo.h"
#include "system.h"
#include "char_load.h"
#ifdef HG_NATIVE
#include <stdio.h>
#include <stdlib.h>
#include "glr.h"
#endif

extern void *Riccardo_vtable[];

extern u8 D_00414840[];
extern u8 D_00414800[], D_004147C0[];
#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

void *Riccardo_MotionFiles(void);
void Riccardo_DoorOffset(void *self, s32 id, u32 *out);
void Riccardo_ActionOffsets(void *self, s32 id, u32 *out);
f32 Riccardo_AttackRange(void);
f32 Riccardo_ReachHewie(void);
s32 Riccardo_SlowWalkAnim(u8 *p);
void *Riccardo_ModelFiles(void);

void Kind37_BehaviourEnded(Pursuer *p);

s32 Kind37_AttackAnimB(void);
s32 Kind37_AttackAnimA(void);

extern u8 D_00441810[], D_004417D0[];
f32 Kind37_FrightAttack(void);
f32 Kind37_FrightSeen(void);
f32 Kind37_ReachHewie(void);
s32 Kind37_SlowWalkAnim(u8 *p);
void *Kind37_ModelFiles(void);

extern void *D_00470390[];
extern void *BonePoint_vtable[];
void *Part60_ctor(u8 *p);
void *HangPoint_ctor(u8 *p);
void *IK2_ctor(u8 *p);
void *Part50_ctor(u8 *p);
extern void *RiccardoModel_vtable[];
extern void *HangPoint_ctor(u8 *p);
extern void *IK2_ctor(u8 *p);
extern void *Part50_ctor(u8 *p);
extern void *Part60_ctor(u8 *p);
extern void *Kind37_vtable[];
extern u8 pstr_O_RCG_RCG_200_PCK_2[], pstr_O_RCG_RCG_200_PCK[];
extern u8 pstr_O_RCT_RCT_200_PCK_4[], pstr_O_RCT_RCT_200_PCK_3[];
extern u32 D_00441850[];
#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

#define U32(p, off) (*(u32 *)((u8 *)(p) + (off)))

static inline s32 b5_prog_flag8000(void);

extern u8 D_0041A2C0[];
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

void *RiccardoModel_dtor(u8 *m, s32 flags);
void RiccardoModel_Vt2C(u8 *m);
void RiccardoModel_Vt30(u8 *m);
void RiccardoModel_SecondaryMotion(u8 *m);
s32 RiccardoModel_Vt98(u8 *m);
s32 RiccardoModel_Vt9C(u8 *m);
s32 RiccardoModel_Part0(u8 *m);
s32 RiccardoModel_Part1(u8 *m);
s32 RiccardoModel_Part2(u8 *m);
s32 RiccardoModel_Part3(u8 *m);
void RiccardoModel_Parts1450(u8 *m);
void RiccardoModel_PartsRest(u8 *m);
void RiccardoModel_Parts10A0(u8 *m);
void RiccardoModel_PartsBE0(u8 *m);
void RiccardoModel_Springs(u8 *m);
void RiccardoModel_Vt3C(u8 *m);
void RiccardoModel_Frame(u8 *m);
void RiccardoModel_Loaded(u8 *m);

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

static inline __attribute__((always_inline)) void creature_inplay(Pursuer *p);

/* in play: Actor_TeleportRandom(-1) */
static inline __attribute__((always_inline)) void creature_inplay(Pursuer *p) {
    if (Npc_InPlayedRoom(p) != 0) {
        Actor_TeleportRandom(&p->c.a, -1);
    }
}

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

Character *Kind37_dtor(Character *c, s32 flags);
void *Kind37_MotionFiles(void);
void Kind37_DoorOffset(void *self, s32 i, f32 *out);
void Kind37_ActionOffsets(void *self, s32 i, f32 *out);

/* the full base with its two parts, then the shared layout of the 0x1310-byte models: 4 parts
 * (0x50, +0x9A0), 4 members (0x40, +0xAE0), 12 parts (0x60, +0xC20), 5 members (0x70, +0x10E0) */
static inline void model_1310(u8 *m, void **vtbl) {
    u8 *e;

    ModelBase_ctor(m);
    AT(m, 0x0, void **) = HumanModel_vtable;
    IK2_ctor(m + 0x8D0);
    IK2_ctor(m + 0x930);
    AT(m, 0x0, void **) = vtbl;
    func_00100340(m + 0x9A0, HangPoint_ctor, HangPoint_dtor, 0x50, 4);
    for (e = m + 0xAE0; e < m + 0xBE0; e += 0x40) {
        AT(e, 0x30, void **) = BonePoint_vtable;
    }
    AT(m, 0xC14, s32) = 0;
    AT(m, 0xC10, s32) = 0;
    func_00100340(m + 0xC20, Part60_ctor, Part60_dtor, 0x60, 0xC);
    AT(m, 0x10D4, s32) = 0;
    AT(m, 0x10D0, s32) = 0;
    for (e = m + 0x10E0; e < m + 0x1310; e += 0x70) {
        AT(e, 0x30, void **) = D_00470390;
    }
}

/* 0x00172BE0 */
void *Kind37_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x25, arg, Kind37_vtable);
}

/* 0x00173380 */
void *Riccardo_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x4, arg, Riccardo_vtable);
}
/* 0x46F69C: nothing (0) */
/* 0x002D7A60 */
s32 ItemA1_Use(void) {
    return 0;
}

/* vtable +0x8: destructor */
/* 0x002D7A70 */
Pursuer *Riccardo_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = Riccardo_vtable;
        if (p != NULL) {
            Pursuer_DestroyBase(p);
        }
        if ((s16)flags > 0) {
            Actor_Destroy(&p->c.a);
        }
    }
    return p;
}

/* 0x002D7B80 */
void *Riccardo_MotionFiles(void) { return D_00414840; }

/* 0x002D7B90 */
void Riccardo_DoorOffset(void *self, s32 id, u32 *out) {
    u32 z;

    switch (id) {
    case 1: z = 0x40F24DD3; break;
    case 3: z = 0xC0DC710D; break;
    case 0: z = 0xC0AA0903; break;
    case 2: z = 0x40FDFD8B; break;
    default: return;
    }
    out[0] = 0;
    out[1] = 0;
    out[2] = z;
}

/* 0x002D7C30 */
void Riccardo_ActionOffsets(void *self, s32 id, u32 *out) {
    u32 x, z;

    switch (id) {
    case 10: case 11: x = 0xBD1F559B; z = 0x41267A78; break;
    case 12: case 13: x = 0x3F3B295F; z = 0x41540419; break;
    case 14: x = 0xBF92D42C; z = 0xC0215326; break;
    case 15: x = 0xBEA7EF9E; z = 0xC0192546; break;
    default: return;
    }
    out[0] = x;
    out[1] = 0;
    out[2] = z;
}

/* vtable +0x200: done at the door unless it has a side (+0x104), then the Pursuer's */
/* 0x002D7CE0 */
void Riccardo_ExitDone(Pursuer *p) {
    if (p->c.unk104[0] == 0) {
        PURSUER_STEP_DONE(p) = 1;
        return;
    }
    Pursuer_ExitDone(p);
}

/* vtable +0xE4: at a door he breaks (Progress_ExitOpen) while opening or attacking it: use and
   damage it (DoorHold_Take / DoorHold_Shut) and change room through it (vtable +0x28) */
/* 0x002D7D20 */
void Riccardo_DoorBreak(Pursuer *p, s32 exit) {
    Progress *pr;

    if (!(Progress_ExitOpen(gProgress, p->c.a.room, exit) & 0xFF)) {
        return;
    }
    switch (PU(p, 0x175C, s32)) {
    case 0xC:
    case 0x29:
    case 0xD:
        pr = gProgress;
        DoorHold_Take(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
        DoorHold_Shut(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
        VCALL(p, 0x28, void (*)(Pursuer *, s32, s32, s32))(p, VCALL(gRooms, 0x28, s32 (*)(VObject *, s32))(gRooms, exit), 0, 0);
        break;
    }
}

/* vtable +0xF0: nothing */
/* 0x002D7E10 */
void Riccardo_ExitArg(Pursuer *p, s32 exit) {
}

/* vtable +0x138: the hit points of an attack entry (`e`: +0 animation, +4 / +8 bones); his
   grab 0xE05 is at Fiona herself */
/* 0x002D8120 */
void Riccardo_BonePositions(Pursuer *p, s32 *e, f32 *a, f32 *b) {
    if (e[0] == 0xE05) {
        sceVu0CopyVector(a, gCharPlayer->a.pos);
        sceVu0CopyVector(b, gCharPlayer->a.pos);
        return;
    }
    sceVu0CopyVector(a, Skel_Bone(MOTION_AT(p, 0x810, u8 *), e[1]) + 0xC);
    if (e[2] >= 0) {
        sceVu0CopyVector(b, Skel_Bone(MOTION_AT(p, 0x810, u8 *), e[2]) + 0xC);
    } else {
        sceVu0CopyVector(b, Skel_Bone(MOTION_AT(p, 0x810, u8 *), e[1]) + 0xC);
    }
}

extern u8 D_00414D50[], D_00414D90[], D_00414DC0[], D_00414DF0[], D_00414E20[], D_00414E40[],
    D_00414E58[], D_00414E68[], D_00414E80[], D_00414EB0[], D_00414EC0[], D_00414EE0[],
    D_00414F10[], D_00414F40[], str_t_7[], D_00414FA0[], str_r_5[];
extern u8 D_00415150[], D_004151C0[], D_00415200[], D_00415250[], D_00415280[], D_004152B0[],
    D_004152D0[], D_00415340[], D_00415360[], D_004153A0[], D_004153B0[], D_004153F0[],
    D_00415420[], D_00415450[], str_t_8[], D_004154E0[], str_r_6[];

/* his attack tables for situations 0..16; the second set when gProgress+0x30 bit 0x8000 */
static u8 *const sAttackTables[2][17] = {
    { D_00414D50, D_00414DC0, D_00414D90, D_00414DF0, D_00414E20, D_00414E40, D_00414E58,
      D_00414E68, D_00414E80, D_00414EB0, D_00414EC0, D_00414EE0, D_00414F10, D_00414F40,
      D_00414FA0, str_r_5, str_t_7 },
    { D_00415150, D_00415200, D_004151C0, D_00415250, D_00415280, D_004152B0, D_004152D0,
      D_00415340, D_00415360, D_004153A0, D_004153B0, D_004153F0, D_00415420, D_00415450,
      D_004154E0, str_r_6, str_t_8 },
};

/* vtable +0x130: the attack table for a situation */
/* 0x002D8210 */
void Riccardo_AttackTable(Pursuer *p, s8 situation) {
    s32 alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;

    PU(p, 0x1718, u8 *) = sAttackTables[alt][(u32)situation < 17 ? situation : 0];
}

extern const PTMF Riccardo_StateBehaviour_ptmf;

/* vtable +0x264: the next behaviour; his own (Riccardo_StateBehaviour) unless at threat level 5, then the
   Pursuer's */
/* 0x002DB7F0 */
void Riccardo_ChaseDecision(Pursuer *p) {
    s32 next;

    if (AT(gProgress, 0x7B8, u8) == 5) {
        Pursuer_ChaseDecision(p);
        return;
    }
    next = PU(p, 0x1758, s32);
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &Riccardo_StateBehaviour_ptmf);
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
    Riccardo_StateBehaviour(p);
}

/* vtable +0x31C: his rage (mode 2, +0x16B8) on, with a roar (sound 0x26), or off */
/* 0x002DB900 */
void Riccardo_SetRage(Pursuer *p, s32 on) {
    if (!on) {
        PU(p, 0x16B8, s32) = 0;
        return;
    }
    PU(p, 0x16B8, s32) = 2;
    Pursuer_Sound(p, 0x26, 7, 0, 0, NULL);
}

/* vtable +0x310 / +0x30C */
/* 0x002DB950 */
s32 Riccardo_AttackAnimB(Pursuer *p) {
    return 0x11;
}

/* 0x002DB960 */
s32 Riccardo_AttackAnimA(Pursuer *p) {
    return 0x10;
}

/* 0x002DB970 */
f32 Riccardo_AttackRange(void) { return 20.0f; }

/* 0x002DB980 */
f32 Riccardo_ReachHewie(void) { return 12.0f; }

/* vtable +0x320: the Pursuer's choice (Pursuer_WalkAnim), but 1 for 0 / 3 unless +0xE0 is set */
/* 0x002DB990 */
s32 Riccardo_WalkAnim(Pursuer *p) {
    s32 r = Pursuer_WalkAnim(p);

    if (p->c.unkE0 == 0 && (r == 3 || r == 0)) {
        r = 1;
    }
    return r;
}

/* vtable +0x324: the Pursuer's slow walk (Pursuer_StanceAnim), 0x200 for 0x204 */
/* 0x002DB9E0 */
s32 Riccardo_StanceAnim(Pursuer *p) {
    s32 r = Pursuer_StanceAnim(p);

    return r == 0x204 ? 0x200 : r;
}

/* 0x002DBA10 */
s32 Riccardo_SlowWalkAnim(u8 *p) {
    f32 d;

    if (F(p, 0xC4, s32) == 1) {
        return 0x203;
    }
    if (p[0x1544] != 0 && F(p, 0x1540, s32) == (s32)gCharPlayer) {
        d = F(p, 0x1588, f32);
        if (d < 100.0f && !(d <= 0.0f)) {
            return 0x206;
        }
    }
    return 0x201;
}

/* vtable +0x30: his frame update. On screen: into his rage at threat level 5 while chasing and
   idle (vtable +0x31C), out of it below; a cry heard from Fiona or Hewie makes him react
   (Riccardo_CryHit); the behaviour step, stance and voice; a snort (sound 0x24) at the key of
   animation 0x600; Riccardo_Impact. Off screen the behaviour step and the off-screen move */
/* 0x002DC070 */
void Riccardo_Update(Pursuer *p) {
    PTMF *st = (PTMF *)((u8 *)p + 0x174C);

    Stalker_ThinkStart(p);
    if (Npc_InPlayedRoom(p) != 0) {
        Pursuer_DoorNear(p);
        if (PU(p, 0x16B8, s32) == 2) {
            if (AT(gProgress, 0x7B8, u8) != 5) {
                VCALL(p, 0x31C, void (*)(Pursuer *, s32))(p, 0);
            }
        } else if (AT(gProgress, 0x7B8, u8) == 5 && PU(p, 0x16C8, u8) == 0 && p->c.moveMode == 0) {
            VCALL(p, 0x31C, void (*)(Pursuer *, s32))(p, 1);
        }
        switch (Pursuer_CryHeard(p) & 0xFF) {
        case 1:
            Riccardo_CryHit(p, gCharPlayer);
            break;
        case 2:
            Riccardo_CryHit(p, gCharPartner);
            break;
        }
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        VCALL(p, 0x110, void (*)(Pursuer *))(p);
        if (p->c.unk14D0 <= 0 || p->c.unk14D0 == 5) {
            Pursuer_AnimSounds(p, -1);
        }
        if (MOTION_ANIM(p) == 0x600 && (Motion_EventFlags(p->c.motion, 0, 0, 1) & 0xFF & 1)) {
            Pursuer_Sound(p, 0x24, 5, 0, 0, NULL);
        }
        Npc_BoneHeight(p);
        Riccardo_Impact(p);
        Pursuer_KeepOnWalkable(p);
    } else {
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        Pursuer_FootstepsThroughWalls(p);
    }
    Stalker_ThinkEnd(p);
}

/* 0x002DC460 */
u8 *Riccardo_ModelFileTable(Pursuer *p) {
    return (F(gProgress, 0x30, u32) & 0x8000) ? pstr_O_RCG_RCG_200_PCK_2 : pstr_O_RCG_RCG_200_PCK;
}

/* 0x002DC4A0 */
void *Riccardo_ModelFiles(void) {
    return (F(gProgress, 0x30, u32) & 0x8000) ? D_00414800 : D_004147C0;
}

extern u8 D_004148A0[], D_004148E0[], D_00414A30[], D_00414A40[], D_00414CD0[], D_00414D00[],
    D_00415060[], D_004150B0[], D_004150D0[], str_Z_6[], D_00415130[], D_004155E0[],
    D_00415630[], D_00415650[], D_0047AC30[];

/* vtable +0xF4: his setup over the Pursuer's (Pursuer_Setup): his tables and stats (different
   when gProgress+0x30 bit 0x8000 is set) */
/* 0x002DC4E0 */
void Riccardo_Setup(Pursuer *p) {
    void *m;

    Pursuer_Setup(p);
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
        PU(p, 0x1748, u8 *) = str_Z_6;
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

/* +0x8: destructor */
/* 0x002F61E0 */
void *RiccardoModel_dtor(u8 *m, s32 flags) {
    if (m != NULL) {
        AT(m, 0x0, void **) = RiccardoModel_vtable;
        func_001002C0(m + 0x1310, Part50_dtor, 0x50, 4);
        func_001002C0(m + 0xC20, Part60_dtor, 0x60, 0xC);
        func_001002C0(m + 0x9A0, HangPoint_dtor, 0x50, 4);
        HumanModel_Destroy(m, flags);
    }
    return m;
}

/* +0x2C / +0x30: part +0xBA's draw flag 2 on / off */
/* 0x002F6340 */
void RiccardoModel_Vt2C(u8 *m) {
    AT(m, 0xBA, u8) |= 2;
}

/* 0x002F6350 */
void RiccardoModel_Vt30(u8 *m) {
    AT(m, 0xBA, u8) &= 0xFD;
}

/* +0xB4: his secondary-motion table */
/* 0x002F6360 */
void RiccardoModel_SecondaryMotion(u8 *m) {
    AT(m, 0x874, u8 *) = D_0041A2C0;
}

/* +0x98 / +0x9C, +0x84 .. +0x90: his mesh parts */
/* 0x002F6370 */
s32 RiccardoModel_Vt98(u8 *m) {
    return 0x31;
}

/* 0x002F6380 */
s32 RiccardoModel_Vt9C(u8 *m) {
    return 0x30;
}

/* 0x002F6390 */
s32 RiccardoModel_Part0(u8 *m) {
    return 3;
}

/* 0x002F63A0 */
s32 RiccardoModel_Part1(u8 *m) {
    return 7;
}

/* 0x002F63B0 */
s32 RiccardoModel_Part2(u8 *m) {
    return 0x1E;
}

/* 0x002F63C0 */
s32 RiccardoModel_Part3(u8 *m) {
    return 0x2C;
}

/* the four parts on +0x1450 (bones 0x26..0x29, two of two) */
/* 0x002F63D0 */
void RiccardoModel_Parts1450(u8 *m) {
    s32 i;

    SpringSet_Clear(m + 0x1450);
    for (i = 0; i < 4; i++) {
        Set_AddLink(m + 0x1450, m + 0x1310 + i * 0x50);
    }
    Set_Init(m + 0x1450, m, 0.0f, 0x1.99999ap-4f /* 0.1 */, 0.0f, 0x1.99999ap-1f /* 0.8 */);
    for (i = 0; i < 4; i++) {
        u8 *n = m + 0x1310 + i * 0x50;

        AT(n, 0x40, f32) = 0x1.570a3ep-1f;   /* 0.67 */
        AT(n, 0x24, s32) = 0x26 + i;
        AT(n, 0x20, u8) = i % 2 == 0;
        AT(n, 0x44, f32) = 0x1.99999ap-2f;   /* 0.4 */
    }
}

/* the twelve 0x60 parts at rest: their length along bone 1's Z axis (the second six the other
   way) from their anchors */
/* 0x002F64F0 */
void RiccardoModel_PartsRest(u8 *m) {
    f32 down[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    u8 *p = m + 0xC20;
    s32 i;

    sceVu0CopyVector(down, Skel_Bone(AT(AT(m, 0x10B4, u8 *), 0x810, u8 *), 1) + 8);
    for (i = 0; i < 12; i++, p += 0x60) {
        AT(p, 0x18, f32) = 0.0f;
        AT(p, 0x14, f32) = 0.0f;
        AT(p, 0x10, f32) = 0.0f;
        if (AT(p, 0x20, u8) != 0) {
            sceVu0CopyVector(at, Skel_Bone(AT(AT(m, 0x10B4, u8 *), 0x810, u8 *), AT(p, 0x24, s32)) + 12);
        } else {
            sceVu0CopyVector(at, AT(p, 0x2C, f32 *));
        }
        if (i < 6) {
            sceVu0ScaleVector(d, down, AT(p, 0x40, f32));
        } else {
            sceVu0ScaleVector(d, down, -AT(p, 0x40, f32));
        }
        sceVu0AddVector((f32 *)p, at, d);
        sceVu0CopyVector((f32 *)(p + 0x50), at);
    }
}

/* the twelve 0x60 parts (bones 0xA..0x15) on +0x10A0 and its five capsules */
/* 0x002F6600 */
void RiccardoModel_Parts10A0(u8 *m) {
    s32 i;

    SpringSet_Clear(m + 0x10A0);
    for (i = 0; i < 12; i++) {
        Set_AddLink(m + 0x10A0, m + 0xC20 + i * 0x60);
    }
    for (i = 0; i < 5; i++) {
        Set_AddCollider(m + 0x10A0, m + 0x10E0 + i * 0x70);
    }
    Set_Init(m + 0x10A0, m, 0.0f, 0x1.99999ap-4f /* 0.1 */, 0.0f, 0x1.99999ap-1f /* 0.8 */);
    for (i = 0; i < 12; i++) {
        u8 *n = m + 0xC20 + i * 0x60;
        s32 j = i % 6;

        AT(n, 0x20, u8) = i % 2 == 0;
        AT(n, 0x24, s32) = 0xA + i;
        AT(n, 0x40, f32) = 0x1.19999ap+0f;   /* 1.1 */
        if (j < 2) {
            AT(n, 0x44, f32) = 0.0f;
            AT(n, 0x48, u8 *) = NULL;
        } else {
            AT(n, 0x44, f32) = 0.5f;
            AT(n, 0x48, u8 *) = m + 0xC20 + (i - j + j % 2) * 0x60;
        }
    }
    Capsule_Set(m + 0x10E0, 2, 6, -0.5f, 0.0f, -0x1.99999ap-4f, 1.0f, -0.5f, 0.0f, 0x1.99999ap-4f);
    Capsule_Set(m + 0x1150, 2, 6, 0.0f, 0.0f, -0x1.99999ap-4f, 1.0f, 0.0f, 0.0f, 0x1.99999ap-4f);
    Capsule_Set(m + 0x11C0, 2, 6, 0.5f, 0.0f, -0x1.99999ap-4f, 1.0f, 0.5f, 0.0f, 0x1.99999ap-4f);
    Capsule_Set(m + 0x1230, 2, 6, 1.0f, 0.0f, -0x1.99999ap-4f, 1.0f, 1.0f, 0.0f, 0x1.99999ap-4f);
    Capsule_Set(m + 0x12A0, 2, 6, 0.0f, -0x1.333334p-2f, -0x1.99999ap-4f, 1.0f, 0.0f, -0x1.333334p-2f, 0x1.99999ap-4f);
}

/* the four parts (bones 0x16..0x19, two of two) on +0xBE0 and its four spheres on bone 2 */
/* 0x002F69C0 */
void RiccardoModel_PartsBE0(u8 *m) {
    s32 i;

    SpringSet_Clear(m + 0xBE0);
    for (i = 0; i < 4; i++) {
        Set_AddLink(m + 0xBE0, m + 0x9A0 + i * 0x50);
    }
    for (i = 0; i < 4; i++) {
        Set_AddCollider(m + 0xBE0, m + 0xAE0 + i * 0x40);
    }
    Set_Init(m + 0xBE0, m, 0.0f, 0.5f, 0.0f, 0x1.99999ap-2f /* 0.4 */);
    for (i = 0; i < 4; i++) {
        u8 *n = m + 0x9A0 + i * 0x50;

        AT(n, 0x40, f32) = 0x1.028f5cp+0f;   /* 1.01 */
        AT(n, 0x24, s32) = 0x16 + i;
        AT(n, 0x20, u8) = i % 2 == 0;
    }
    Sphere_Set(m + 0xAE0, 2, -0x1.99999ap-1f, 0.0f, 0.0f, 1.0f);
    Sphere_Set(m + 0xB20, 2, 0.0f, 0.0f, 0.0f, 1.0f);
    Sphere_Set(m + 0xB60, 2, 0x1.99999ap-1f, 0.0f, 0.0f, 1.0f);
    Sphere_Set(m + 0xBA0, 2, 0x1.99999ap+0f, 0.0f, 0.0f, 1.0f);
}

/* all his springs */
/* 0x002F6BD0 */
void RiccardoModel_Springs(u8 *m) {
    AT(m, 0x850, u8) = 1;
    RiccardoModel_PartsBE0(m);
    RiccardoModel_Parts10A0(m);
    RiccardoModel_Parts1450(m);
}

/* +0x3C: his springs a frame: one step, or after a reset (+0x850) at rest and 30 to settle */
/* 0x002F6C10 */
void RiccardoModel_Vt3C(u8 *m) {
    s32 n = 1;
    s32 i;

    if (AT(m, 0x850, u8) != 0) {
        RiccardoModel_PartsRest(m);
        n = 30;
    }
    SpringSet_Begin(m + 0xBE0);
    SpringSet_Begin(m + 0x10A0);
    SpringSet_Begin(m + 0x1450);
    for (i = 0; i < n; i++) {
        SpringSet_Step(m + 0xBE0);
        SpringSet_Step(m + 0x10A0);
        SpringSet_Step(m + 0x1450);
    }
    SpringSet_Finish(m + 0xBE0);
    SpringSet_Finish(m + 0x10A0);
    SpringSet_Finish(m + 0x1450);
    AT(m, 0x850, u8) = 0;
}

/* +0x10 */
/* 0x002F6CD0 */
void RiccardoModel_Frame(u8 *m) {
    Model_Release(m);
}

/* +0xC: once loaded: the base setup, the part roles, his springs, per-part draw settings */
/* 0x002F6CE0 */
void RiccardoModel_Loaded(u8 *m) {
    HumanModel_Loaded(m);
    AT(m, 0x890, s32) = 2;
    AT(m, 0x894, s32) = 3;
    AT(m, 0x898, s32) = 4;
    AT(m, 0x89C, s32) = 5;
    AT(m, 0x8B8, s32) = 0x20;
    AT(m, 0x8A0, s32) = 6;
    AT(m, 0x8A4, s32) = 7;
    AT(m, 0x8A8, s32) = 8;
    AT(m, 0x8AC, s32) = 9;
    AT(m, 0x8BC, s32) = 0x2E;
    AT(m, 0x8B0, s32) = 0x23;
    AT(m, 0x8B4, s32) = 0x1A;
    AT(m, 0x880, s32) = 8;
    AT(m, 0x860, f32) = 0.0f;
    AT(m, 0x864, f32) = 16.0f;
    AT(m, 0x868, f32) = 0.0f;
    AT(m, 0x854, s32) = 0;
    AT(m, 0x858, s32) = 0;
    RiccardoModel_Springs(m);
    AT(m, 0xC8, u8) = 4;
    AT(m, 0xC9, u8) = 0x40;
    AT(m, 0xCA, u8) = 4;
    AT(m, 0xCB, u8) = 0x40;
    AT(m, 0xCC, u8) = 4;
    AT(m, 0xCD, u8) = 0x40;
    AT(m, 0xBA, u8) = 4;
    AT(m, 0xBB, u8) = 0xC0;
}

/* 0x0034B7B0 */
Character *Kind37_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind37_vtable); }

/* 0x0034B8C0 */
void *Kind37_MotionFiles(void) {
    return D_00441850;
}

/* 0x0034B8D0 */
void Kind37_DoorOffset(void *self, s32 i, f32 *out) {
    switch (i) {
    case 1: out[0] = 0.0f; out[1] = 0.0f; out[2] = 0x1.e49ba6p+2f /* 7.572 */; break;
    case 3: out[0] = 0.0f; out[1] = 0.0f; out[2] = -0x1.b8e21ap+2f /* -6.8888 */; break;
    case 0: out[0] = 0.0f; out[1] = 0.0f; out[2] = -0x1.541206p+2f /* -5.3136 */; break;
    case 2: out[0] = 0.0f; out[1] = 0.0f; out[2] = 0x1.fbfb16p+2f /* 7.9372 */; break;
    }
}

/* 0x0034B970 */
void Kind37_ActionOffsets(void *self, s32 i, f32 *out) {
    switch (i) {
    case 10: case 11: out[0] = -0x1.3eab36p-5f /* -0.0389 */; out[1] = 0.0f; out[2] = 0x1.4cf4fp+3f /* 10.4049 */; break;
    case 12: case 13: out[0] = 0x1.7652bep-1f /* 0.7311 */; out[1] = 0.0f; out[2] = 0x1.a80832p+3f /* 13.251 */; break;
    case 14: out[0] = -0x1.25a858p+0f /* -1.1471 */; out[1] = 0.0f; out[2] = -0x1.42a64cp+1f /* -2.5207 */; break;
    case 15: out[0] = -0x1.4fdf3cp-2f /* -0.328 */; out[1] = 0.0f; out[2] = -0x1.324a8cp+1f /* -2.3929 */; break;
    }
}

extern const PTMF Pursuer_AttackNextStep_ptmf6;

/* the end of his lunge animation: at threat level 5 it leads straight into attack 5;
   otherwise it ends the step */
static inline void Riccardo_LungeEnd(Pursuer *p) {
    Character_RootMoveMasked(&p->c);
    if (AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        if (AT(gProgress, 0x7B8, u8) != 5) {
            PURSUER_STEP_DONE(p) = 1;
            PURSUER_STEP_NEXT(p) = 1;
            return;
        }
        PU(p, 0x1728, s32) = 5;
        Actor_SetState(&p->c.a, &Pursuer_AttackNextStep_ptmf6);
        p->c.moveMode = 8;
        Pursuer_AttackNextStep(p);
    }
}

/* state: the lunge (see Riccardo_LungeEnd) */
/* 0x002D85D0 */
void Riccardo_StateLunge(Pursuer *p) {
    Riccardo_LungeEnd(p);
}

extern const PTMF Riccardo_StateLunge_ptmf;

/* start of the lunge: finish the current walk, then animation 0x1300 in state Riccardo_StateLunge */
/* 0x002D8690 */
void Riccardo_StartLunge(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    Pursuer_PlayAnimIf(p, 0x1300, 0);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &Riccardo_StateLunge_ptmf);
    Riccardo_LungeEnd(p);
}

extern const PTMF Riccardo_FlurryBlow_ptmf2;

/* state: after a blow: his blows left (+0x1624) count down; another while he may go for his
   target and it's in reach and in front (within 90 degrees): Fiona seen within 100 (not at
   threat level 5), Hewie heard within 30. Otherwise the step ends */
/* 0x002D8AC0 */
void Riccardo_StateAfterBlow(Pursuer *p) {
    Character *t;
    f32 a;

    PU(p, 0x1624, s32)--;
    if (PU(p, 0x1624, s32) <= 0 || !(Pursuer_MayGoForTarget(p) & 0xFF)) {
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
    if (!(Angle_Wrap(Actor_HeadingTo(&p->c.a, t->a.pos) - p->c.a.angle[1]) <= 0.0f)) {
        a = Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
    } else {
        a = -Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
    }
    if (!(a <= 0x1.921fb6p+0f /* 90 degrees */)) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
        return;
    }
    PU(p, 0x1760, u8) = 0;
    Actor_SetState(&p->c.a, &Riccardo_FlurryBlow_ptmf2);
}

extern const PTMF Riccardo_StateAfterBlow_ptmf3;

/* a blow: at its hit key, if he may go for his target and it's within the reach of the blow
   (+0x171C entry +0x104, 0x24 bytes: +0xC reach), it lands (Relation_Request kind 1 with the
   entry's damage); at the animation's end, on to the next (Riccardo_StateAfterBlow) */
static inline void Riccardo_Blow(Pursuer *p) {
    if ((Motion_EventFlags(p->c.motion, 0, -1, 1) & 0xFF & 2) && Pursuer_MayGoForTarget(p) != 0) {
        u8 *e = PU(p, 0x171C, u8 *) + p->c.unk104[0] * 0x24;

        if (Actor_Distance(&p->c.a, p->target->a.pos) < AT(e, 0xC, f32)) {
            Relation_Request(gProgress, *(u8 *)&p->c.a.slot, 1, AT(e, 0x10, u8), AT(e, 0x12, u16), AT(e, 0x4, s16), AT(e, 0x14, f32));
        }
    }
    if (AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        PU(p, 0x178C, s32) = 0;
        p->c.unk100 = 0;
        Actor_SetState(&p->c.a, &Riccardo_StateAfterBlow_ptmf3);
        Riccardo_StateAfterBlow(p);
    }
    Character_RootMoveMasked(&p->c);
}

/* state: a blow (see Riccardo_Blow) */
/* 0x002D8CB0 */
void Riccardo_StateBlow(Pursuer *p) {
    Riccardo_Blow(p);
}

extern const f32 D_004156C0[6], D_004156E0[6];
extern const PTMF Riccardo_FlurryBlow_ptmf;

/* start of a flurry: how many blows (+0x1624, 1..7) by a roll against his cumulative chances
   (D_004156C0, or D_004156E0 when gProgress+0x30 bit 0x8000); then Riccardo_FlurryBlow. When he may
   not go for his target: action 0x17 instead */
/* 0x002DA4C0 */
void Riccardo_StartFlurry(Pursuer *p) {
    const f32 *chance;
    f32 roll;

    if (!(Pursuer_MayGoForTarget(p) & 0xFF)) {
        PU(p, 0x1624, s32) = 0;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x17);
        p->c.unk104[0] = 0;
        Character_RootMoveMasked(&p->c);
        return;
    }
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    chance = (AT(gProgress, 0x30, u32) & 0x8000) ? D_004156E0 : D_004156C0;
    roll = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
    PU(p, 0x1624, s32) = 0;
    while (PU(p, 0x1624, s32) < 6 && !(roll <= chance[PU(p, 0x1624, s32)])) {
        PU(p, 0x1624, s32)++;
    }
    PU(p, 0x1624, s32)++;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &Riccardo_FlurryBlow_ptmf);
    Riccardo_FlurryBlow(p);
}

/* a cry from `who` (Fiona or Hewie): a hit effect on them. In his grab (0x1000, unless its entry
   is kind 6) at a random one of four of their bones (motion vtable +0x84..+0x90); in his blows
   0xE05 / 0x1A01 a large one at their bone of motion vtable +0x94 */
/* 0x002D7E20 */
void Riccardo_CryHit(Pursuer *p, Character *who) {
    HitEffectParams hp;
    u8 *wm;
    s32 bone;

    if (PU(p, 0x175C, s32) == 0x1000) {
        if (AT(PU(p, 0x171C, u8 *) + p->c.unk104[0] * 0x24, 0x10, u8) == 6) {
            return;
        }
        hp.kind = who == gCharPlayer ? 0 : 1;
        wm = who->motion;
        switch ((u32)(4.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom))) {
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

        sceVu0CopyVector(pos, Skel_Bone(AT(who->motion, 0x810, u8 *), bone) + 0xC);
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
/* 0x002D8840 */
s32 Riccardo_AttackForDistance(Pursuer *p, Character *who) {
    f32 d, any, c3 = 0.0f, c4 = 0.0f, c6 = 0.0f;
    f32 roll;
    VObject *rnd;

    if (who == NULL) {
        who = p->target;
    }
    d = Actor_Distance(&p->c.a, who->a.pos);
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
    rnd = gRandom;
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

extern const PTMF Riccardo_StateBlow_ptmf, Riccardo_StateBlowHewie_ptmf, Riccardo_StateBlowFiona_ptmf;

/* a blow of the flurry: on Fiona, in front of her (Npc_SameFloor) entry 0 or 3, else from behind
   8 or 9 (one blow only); on Hewie entry 1 if he's in front, else give up (action 0x13). A blow
   of kind 6 is struck at once (Riccardo_Blow); others first close in: on Hewie Riccardo_StateBlowHewie, on
   Fiona Riccardo_StateBlowFiona */
/* 0x002DA120 */
void Riccardo_FlurryBlow(Pursuer *p) {
    u8 *e;

    if (p->target != gCharPartner) {
        if (!(Npc_SameFloor(&p->c.a, &p->target->a) & 0xFF)) {
            p->c.unk104[0] = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) <= 50.0f ? 9 : 8;
            PU(p, 0x1624, s32) = 1;
        } else {
            p->c.unk104[0] = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) <= 50.0f ? 0 : 3;
        }
    } else if (Npc_SameFloor(&p->c.a, &p->target->a) & 0xFF) {
        p->c.unk104[0] = 1;
    } else {
        PU(p, 0x1624, s32) = 0;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x13);
        PU(p, 0x1728, s32) = 2;
        PU(p, 0x172C, u8) = 0;
        Character_RootMoveMasked(&p->c);
        return;
    }
    e = PU(p, 0x171C, u8 *) + p->c.unk104[0] * 0x24;
    Pursuer_PlayAnimIf(p, AT(e, 0x0, s32), 0);
    p->c.unk100 = -1;
    if (AT(e, 0x10, u8) == 6) {
        Actor_SetState(&p->c.a, &Riccardo_StateBlow_ptmf);
        Riccardo_Blow(p);
    } else if (p->target == gCharPartner) {
        Actor_SetState(&p->c.a, &Riccardo_StateBlowHewie_ptmf);
        Riccardo_StateBlowHewie(p);
    } else {
        Actor_SetState(&p->c.a, &Riccardo_StateBlowFiona_ptmf);
        Riccardo_StateBlowFiona(p);
    }
}

extern const PTMF D_00415680, Riccardo_StateChase_ptmf;

/* his behaviour: Fiona as the target. Out of sight of her, head for her (vtable +0xB0).
   Otherwise the pending action (0x1C rumbles the pad), or: further than 100 hold off (6);
   closer, seen by her and within +0x17C0 (60) come on (1); else back off (5) or hold off (6) by a
   roll against his table +0x1824 (+0x14 chance, +0 / +4 waits). Then his chase (Riccardo_StateChase) */
static inline void Riccardo_Behaviour(Pursuer *p, const PTMF *away, const PTMF *chase,
                                      void (*chaseFn)(Pursuer *), s32 setReach) {
    s32 next;

    p->target = gCharPlayer;
    PU(p, 0x16F6, u8) = 1;
    if (PU(p, 0x1544, u8) == 0) {
        VCALL(p, 0xB0, void (*)(Pursuer *))(p);
        ptmf_set((PTMF *)((u8 *)p + 0x174C), away);
        PU(p, 0x1758, s32) = -1;
        return;
    }
    if (setReach) {
        PU(p, 0x17C0, f32) = 60.0f;
    }
    next = PU(p, 0x1758, s32);
    if (next == -1) {
        f32 roll = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
        Character *t;

        if (!(PU(p, 0x1588, f32) <= 100.0f)) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
            PU(p, 0x162C, s32) = AT(PU(p, 0x1824, u8 *), 0xC, s32);
        } else if (t = p->target,
                   (Eye_ActorSees(p, &t->a, &p->c.a, t->a.angle[1], PU(p, 0x1580, f32), 0x1.921fb6p+1f /* 180 degrees */) & 0xFF) &&
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
            VCALL(gRumble, 0x18, void (*)(VObject *, s32, s32, s32))(gRumble, 3, 0x80, 0x1E);
        }
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, PU(p, 0x1758, s32));
    }
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    PU(p, 0x1630, s32) = AT(PU(p, 0x1824, u8 *), 0x10, s32);
    ptmf_set((PTMF *)((u8 *)p + 0x174C), chase);
    PU(p, 0x1758, s32) = -1;
    chaseFn(p);
}

/* 0x002DB480 */
void Riccardo_StateBehaviour(Pursuer *p) {
    Riccardo_Behaviour(p, &D_00415680, &Riccardo_StateChase_ptmf, Riccardo_StateChase, 1);
}

extern const f32 D_00415660[4];

/* where the blow of his current animation hits the floor (into `out`; 0 if nowhere). His hammer
   swings 0xE00 / 0xE04: straight ahead along the nav mesh to the first edge he can't cross
   (flags 0x80 or no neighbour), stopping at walls (0x4000); his slams 0x2301 / 0x2303 / 0xE06 /
   0x1602: under the hammer head (bone 0x31, offset D_00415660) */
/* 0x002DBA90 */
s32 Riccardo_BlowFloorPoint(Pursuer *p, f32 *out) {
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

        Actor_PointAhead((u8 *)&p->c.a, 1000.0f, ahead);
        nav = gNavMesh;
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
                Vec_Along(ahead, ahead, p->c.a.angle[1], 1000.0f);
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

        sceVu0CopyVector(head, Skel_Bone(MOTION_AT(p, 0x810, u8 *), 0x31) + 0xC);
        off[0] = D_00415660[0];
        off[1] = D_00415660[1];
        off[2] = D_00415660[2];
        off[3] = D_00415660[3];
        sceVu0CopyMatrix(m, (f32 (*)[4])Skel_Bone(MOTION_AT(p, 0x810, u8 *), 0x31));
        Mtx_ApplyVector(out, m, off);
        sceVu0AddVector(out, out, head);
        tri = Actor_TriTo(&p->c.a, out, 0x20008);
        if (tri == (u32)-1) {
            return 0;
        }
        VCALL(gNavMesh, 0x14, void (*)(void *, u32, f32 *))(gNavMesh, tri, out);
        return 1;
    }
}

/* the impact (0x80 bytes, vtable 0x479600) and the debris cloud (0xF70 bytes, vtable 0x47A710)
   of his hammer */
extern void *StrikeMark_vtable[], *DropletFlash_vtable[];

static inline void Impact_Init(void **obj) {
    obj[0] = StrikeMark_vtable;
}

static inline void Debris_Init(void **obj) {
    obj[0] = DropletFlash_vtable;
    obj[0xC10 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0xC14 / 4] = -1;
    obj[0xC10 / 4] = QuadDrawer_vtable;
}

typedef struct {
    s32 rgb[3];    /* 0x80, 0x50, 0x40: brown */
    f32 pos[3];
} DebrisParams;

/* a debris cloud where a blow hits the floor */
static void Riccardo_Debris(const f32 *at) {
    u8 *mgr = gEffects;
    s32 slot = Effect_New(mgr, 0xF70, Debris_Init);
    DebrisParams dp;

    dp.rgb[0] = 0x80;
    dp.rgb[1] = 0x50;
    dp.rgb[2] = 0x40;
    dp.pos[0] = at[0];
    dp.pos[1] = at[1];
    dp.pos[2] = at[2];
    EffectMgr_Start(mgr, slot, &dp);
}

/* at the impact key of his animation (0x20): a jolt (the 0x80 effect, kind 2) and a noise of
   0x40 where he stands (gProgress+0x798); a debris cloud where his slam or swing hits the floor
   (Riccardo_BlowFloorPoint), or for his grab 0x1A01 a hit effect on Fiona */
/* 0x002DBD70 */
void Riccardo_Impact(Pursuer *p) {
    u8 *mgr;
    s32 jolt[4] = { 2, 0, 0, 0 };   /* kind 2 */

    if (!(Motion_EventFlags(p->c.motion, 0, -1, 1) & 0xFF & 0x20)) {
        return;
    }
    mgr = gEffects;
    EffectMgr_Start(mgr, Effect_New(mgr, 0x80, Impact_Init), jolt);
    Noise_Make((u8 *)gProgress + 0x798, 0x40, p->c.a.room, p->c.a.navTri, 0xFFFF);
    switch (MOTION_ANIM(p)) {
    case 0x2303:
    case 0x2301:
    case 0x1602: {
        f32 at[4] __attribute__((aligned(16)));

        if (Riccardo_BlowFloorPoint(p, at) & 0xFF) {
            Riccardo_Debris(at);
        }
        break;
    }
    case 0x1A01:
        Riccardo_CryHit(p, gCharPlayer);
        break;
    }
}

extern const PTMF Riccardo_StateAfterBlow_ptmf2;

/* state: his blow at Hewie. At the hit key, if he may go for him, hears him within 30, and Hewie
   is in front within 30 degrees: unless Fiona stands in the way (within 4 of the line to Hewie,
   and no further), it's Hewie's blow (Riccardo_AttackForDistance: on a hit Relation_Request kind 2 with the entry,
   else debris where it lands); in her way it's hers (6: the heavy entry 8 with its stun and
   debris, 4: a stun three times in four, 3; none: debris only). Until the hit he turns to
   Hewie; at the animation's end, on (Riccardo_StateAfterBlow); now and then a grunt (sound 0x15) */
/* 0x002D8DF0 */
void Riccardo_StateBlowHewie(Pursuer *p) {
    f32 fiona[4] __attribute__((aligned(16)));
    f32 hewie[4] __attribute__((aligned(16)));

    sceVu0CopyVector(fiona, gCharPlayer->a.pos);
    sceVu0CopyVector(hewie, gCharPartner->a.pos);
    if (Motion_EventFlags(p->c.motion, 0, -1, 1) & 0xFF & 2) {
        f32 a;

        if (Pursuer_MayGoForTarget(p) == 0 || PU(p, 0x1545, u8) != 1 ||
            (Npc_SameFloor(&p->c.a, &gCharPartner->a) & 0xFF) != 1) {
            goto done;
        }
        if (!(Angle_Wrap(Actor_HeadingTo(&p->c.a, hewie) - p->c.a.angle[1]) <= 0.0f)) {
            a = Angle_Wrap(Actor_HeadingTo(&p->c.a, hewie) - p->c.a.angle[1]);
        } else {
            a = -Angle_Wrap(Actor_HeadingTo(&p->c.a, hewie) - p->c.a.angle[1]);
        }
        if (!(a < 0x1.0c1524p-1f /* 30 degrees */) || !(PU(p, 0x158C, f32) < 30.0f)) {
            goto done;
        }
        {
            u8 *e = PU(p, 0x171C, u8 *) + p->c.unk104[0] * 0x24;
            f32 at[4] __attribute__((aligned(16)));
            u32 k;

            if (!(Vec_LineDistance(p->c.a.pos, hewie, fiona) <= 4.0f) ||
                Actor_Distance(&p->c.a, hewie) < Actor_Distance(&p->c.a, fiona)) {
                /* Hewie */
                if ((Riccardo_AttackForDistance(p, gCharPartner) & 0xFF) != 0xFF) {
                    Relation_Request(gProgress, *(u8 *)&p->c.a.slot, 2, AT(e, 0x10, u8), AT(e, 0x12, u16), 0, AT(e, 0x14, f32));
                } else if (Riccardo_BlowFloorPoint(p, at) & 0xFF) {
                    Riccardo_Debris(at);
                }
            } else {
                /* Fiona in the way */
                s32 stun = 0;

                k = Riccardo_AttackForDistance(p, gCharPlayer) & 0xFF;
                switch (k) {
                case 6:
                    p->c.unk104[0] = 8;
                    e = PU(p, 0x171C, u8 *) + 0x120;
                    stun = AT(e, 0x4, s16);
                    /* fallthrough */
                case 0xFF:
                    if (Riccardo_BlowFloorPoint(p, at) & 0xFF) {
                        Riccardo_Debris(at);
                    }
                    break;
                case 4:
                    if (100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) < 75.0f) {
                        stun = -0x8000;
                    }
                    break;
                }
                if (k != 0xFF) {
                    Relation_Request(gProgress, *(u8 *)&p->c.a.slot, 1, k, AT(e, 0x12, u16), stun, AT(e, 0x14, f32));
                }
            }
        }
    done:
        p->c.unk100 = 0;
    }
    if (p->c.unk100 == -1) {
        Character *t = gCharPartner != NULL ? gCharPartner : p->target;
        f32 h = Actor_HeadingTo(&p->c.a, t->a.pos);

        Npc_TurnToward(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    }
    if (AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        PU(p, 0x178C, s32) = 0;
        p->c.unk100 = 0;
        Actor_SetState(&p->c.a, &Riccardo_StateAfterBlow_ptmf2);
        Riccardo_StateAfterBlow(p);
    }
    if ((Motion_EventFlags(p->c.motion, 0, 0, 1) & 0xFF & 1) &&
        100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) < 50.0f) {
        Pursuer_Sound(p, 0x15, 7, 0, 0, NULL);
    }
    Character_RootMoveMasked(&p->c);
}

extern const PTMF Riccardo_StateAfterBlow_ptmf;

/* is `pt` clear of the swing at `at` (not within 4 of the line to it, or no nearer than it) */
static inline s32 Riccardo_OutOfLine(Pursuer *p, const f32 *at, const f32 *pt) {
    return !(Vec_LineDistance(p->c.a.pos, at, pt) <= 4.0f) || Actor_Distance(&p->c.a, at) < Actor_Distance(&p->c.a, pt);
}

/* state: his blow at Fiona. At the hit key he aims (+0x100 bits) if he may go for her, sees her
   in front within 30 degrees: 4 at her, 1 at a room object within 50 of her (gEvents vtable
   +0x68) further than 20 from him, 2 at Hewie likewise. At the key the object takes the blow
   (its point to +0x1770) unless she or Hewie is in the way; Hewie takes it if he's in the way
   (Riccardo_AttackForDistance: a hit of kind 2, else debris); then her: if she's lower than his waist the
   hammer passes over (debris), else Riccardo_AttackForDistance picks the hit (6: the heavy entry with its stun
   and debris, 4: a stun three times in four, 3; none: debris only). Until the key he turns to
   her; at the animation's end, on (Riccardo_StateAfterBlow); now and then a grunt (0x22 / 0x23) */
/* 0x002D9500 */
void Riccardo_StateBlowFiona(Pursuer *p) {
    f32 fiona[4] __attribute__((aligned(16)));
    f32 hewie[4] __attribute__((aligned(16)));
    f32 obj[4] __attribute__((aligned(16)));
    u32 aim;

    sceVu0CopyVector(fiona, gCharPlayer->a.pos);
    sceVu0CopyVector(hewie, gCharPartner->a.pos);
    if (Motion_EventFlags(p->c.motion, 0, -1, 1) & 0xFF & 2) {
        f32 a;

        if (!(Pursuer_MayGoForTarget(p) & 0xFF) || PU(p, 0x1544, u8) == 0 ||
            !(Npc_SameFloor(&p->c.a, &gCharPlayer->a) & 0xFF)) {
            p->c.unk100 = 0;
        } else {
            if (!(Angle_Wrap(Actor_HeadingTo(&p->c.a, fiona) - p->c.a.angle[1]) <= 0.0f)) {
                a = Angle_Wrap(Actor_HeadingTo(&p->c.a, fiona) - p->c.a.angle[1]);
            } else {
                a = -Angle_Wrap(Actor_HeadingTo(&p->c.a, fiona) - p->c.a.angle[1]);
            }
            if (!(a <= 0x1.0c1524p-1f /* 30 degrees */)) {
                p->c.unk100 = 0;
            } else {
                f32 d[4] __attribute__((aligned(16)));

                p->c.unk100 = 4;
                if (VCALL(gEvents, 0x68, s32 (*)(VObject *, f32 *, f32 *))(gEvents, fiona, obj) != 0) {
                    sceVu0SubVector(d, fiona, obj);
                    d[3] = 0.0f;
                    if (__builtin_sqrtf(sceVu0InnerProduct(d, d)) < 50.0f && !(Actor_Distance(&p->c.a, obj) <= 20.0f)) {
                        p->c.unk100 |= 1;
                    }
                }
                sceVu0SubVector(d, fiona, hewie);
                d[3] = 0.0f;
                if (__builtin_sqrtf(sceVu0InnerProduct(d, d)) < 50.0f && !(Actor_Distance(&p->c.a, hewie) <= 20.0f)) {
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
        f32 h = Actor_HeadingTo(&p->c.a, t->a.pos);

        Npc_TurnToward(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    } else {
        u8 *e = PU(p, 0x171C, u8 *) + p->c.unk104[0] * 0x24;
        f32 at[4] __attribute__((aligned(16))) = { 0.0f, 0.0f, 0.0f, 0.0f };

        if (aim & 1) {
            u32 tri = Actor_TriFrom(&p->c.a, obj, gCharPlayer->a.navTri, fiona, 0);

            if (tri == (u32)-1) {
                tri = Actor_TriTo(&p->c.a, obj, 0);
            }
            if (tri != (u32)-1 && Npc_SeesPoint(p, tri, obj) != 0 &&
                p->c.a.pos[1] - obj[1] < 10.0f && !(p->c.a.pos[1] - obj[1] <= -15.0f) &&
                Riccardo_OutOfLine(p, obj, fiona) &&
                (!(Vec_LineDistance(p->c.a.pos, obj, hewie) <= 4.0f) ||
                 Actor_Distance(&p->c.a, obj) < Actor_Distance(&p->c.a, hewie) || PU(p, 0x1545, u8) == 0)) {
                sceVu0CopyVector((f32 *)((u8 *)p + 0x1770), obj);
                p->c.unk100 = 0;
                goto end;
            }
        }
        if ((p->c.unk100 & 2) && PU(p, 0x1545, u8) == 1 &&
            (Npc_SameFloor(&p->c.a, &gCharPartner->a) & 0xFF) == 1 && Riccardo_OutOfLine(p, hewie, fiona)) {
            if ((Riccardo_AttackForDistance(p, gCharPartner) & 0xFF) != 0xFF) {
                Relation_Request(gProgress, *(u8 *)&p->c.a.slot, 2, AT(e, 0x10, u8), AT(e, 0x12, u16), 0, AT(e, 0x14, f32));
            } else if (Riccardo_BlowFloorPoint(p, at) & 0xFF) {
                Riccardo_Debris(at);
            }
            p->c.unk100 = 0;
        } else if (p->c.unk100 & 4) {
            if (Actor_Distance(&p->c.a, fiona) < 20.0f && fiona[1] + gCharPlayer->a.height < p->c.a.pos[1] + 5.0f) {
                /* she's below the swing */
                if (Riccardo_BlowFloorPoint(p, at) & 0xFF) {
                    Riccardo_Debris(at);
                }
            } else {
                u32 k = Riccardo_AttackForDistance(p, gCharPlayer) & 0xFF;
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
                    if (Riccardo_BlowFloorPoint(p, at) & 0xFF) {
                        Riccardo_Debris(at);
                    }
                    break;
                case 4:
                    if (100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) < 75.0f) {
                        stun = -0x8000;
                    }
                    break;
                }
                if (k != 0xFF) {
                    Relation_Request(gProgress, *(u8 *)&p->c.a.slot, 1, k, AT(e, 0x12, u16), stun, AT(e, 0x14, f32));
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
        Actor_SetState(&p->c.a, &Riccardo_StateAfterBlow_ptmf);
        Riccardo_StateAfterBlow(p);
    }
    if ((Motion_EventFlags(p->c.motion, 0, 0, 1) & 0xFF & 1) &&
        100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) < 50.0f) {
        if (p->c.unk104[0] == 0) {
            Pursuer_Sound(p, 0x22, 7, 0, 0, NULL);
        } else if (p->c.unk104[0] == 3) {
            Pursuer_Sound(p, 0x23, 7, 0, 0, NULL);
        }
    }
    Character_RootMoveMasked(&p->c);
}

/* does Fiona (the target) see him: within 180 degrees of her heading and his sight range */
static inline s32 Riccardo_Seen(Pursuer *p) {
    Character *t = p->target;

    return Eye_ActorSees(p, &t->a, &p->c.a, t->a.angle[1], PU(p, 0x1580, f32), 0x1.921fb6p+1f /* 180 degrees */) & 0xFF;
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
static inline s32 Riccardo_Lunge(Pursuer *p, u8 *tbl) {
    u32 chance = Pursuer_ThresholdEntry(p, (f32 *)tbl, 1);
    f32 roll = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);

    if (roll < (f32)chance) {
        VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, 0xA);
        Pursuer_PickFromTable(p);
        return 1;
    }
    return 0;
}

/* a waited-out back-off or hold-off: a lunge by the threat-level chance, else back or hold off
   again. 1 when he lunges */
static s32 Riccardo_StrikeOrWait(Pursuer *p, u8 *tbl) {
    f32 roll;

    if (Riccardo_Lunge(p, tbl)) {
        return 1;
    }
    roll = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
    Riccardo_Seen(p);   /* the original asks, then backs or holds off the same either way */
    Riccardo_BackOrHold(p, roll);
    return 0;
}

extern const PTMF D_004156A0, D_004156B0;

/* his chase (from Riccardo_StateBehaviour), as Debilitas's Debilitas_Chase: he stalks Fiona, closing in (1),
   backing off (5) or holding off (6) for the waits from his table +0x1824, and lunges by the
   threat-level chance (every 60 frames while closing in, and when a wait runs out within 100).
   Seen by her while within +0x17C0 he comes straight on (1) a limited number of times
   (+0x1630, refilled from +0x1824 +0x10) */
static inline __attribute__((always_inline)) void Riccardo_Chase(Pursuer *p, u8 *tbl, const PTMF *closeIn,
                                                                  const PTMF *backOff) {
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

        if ((PU(p, 0x1780, u32) + 1) % 60 == 0 && Riccardo_Lunge(p, tbl)) {
            return;
        }
        if (!((((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF))) {
            u32 dir = Npc_TurnWayTo(p, gCharPlayer->a.pos, 0x1.0c1524p+0f /* 60 degrees */, 0x1.4f1a6ep+1f /* 150 degrees */) & 0xFF;

            if (dir != 0xFF) {
                p->c.unk104[0] = dir;
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 3);
            }
        }
        roll = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
        if (PU(p, 0x1588, f32) < 0.0f) {
            if (Pursuer_TargetOutOfReach(p) & 0xFF) {
                break;
            }
            Pursuer_ChaseFionaHere(p);
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
        if (Riccardo_StrikeOrWait(p, tbl)) {
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
        if (Riccardo_StrikeOrWait(p, tbl)) {
            return;
        }
        break;
    }
    if (PURSUER_STEP_NEXT(p) == 1 && PU(p, 0x175C, s32) != 0x12) {
        s32 d = Npc_OpenDoorFionaHides();

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
        ptmf_set((PTMF *)((u8 *)p + 0x174C), closeIn);
        PU(p, 0x1758, s32) = -1;
        PU(p, 0x162C, s32) = 0;
        return;
    }
    if (AT(gProgress, 0x7B8, u8) == 5) {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), backOff);
        PU(p, 0x1758, s32) = -1;
        PU(p, 0x162C, s32) = 0;
        return;
    }
    near = p->target->moveMode == 3 ? Actor_Distance(&p->c.a, p->target->a.pos) : PU(p, 0x1588, f32);
    if (!(near < VCALL(p, 0x2F4, f32 (*)(Pursuer *))(p)) || near < 0.0f) {
        return;
    }
    if (near < Pursuer_GroundGained(p) || near < 10.0f) {
        f32 a;

        if (!(Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]) <= 0.0f)) {
            a = Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
        } else {
            a = -Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
        }
        if (a < 0x1.921fb6p+1f * VCALL(p, 0x2EC, f32 (*)(Pursuer *))(p) / 180.0f &&
            Npc_SameFloor(&p->c.a, &p->target->a) != 0) {
            VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, (s8)Pursuer_FionaState(p));
            Pursuer_PickFromTable(p);
            PU(p, 0x162C, s32) = 0;
        }
    }
}

/* 0x002DA6B0 */
void Riccardo_StateChase(Pursuer *p) {
    Riccardo_Chase(p, D_0047AC38, &D_004156A0, &D_004156B0);
}

extern u8 D_0047AF50[];
extern const PTMF D_00442928, D_00442938;

/* (as Riccardo_StateChase) the same chase in the other class, with its lunge table and states */
/* 0x0034BFF0 */
void Kind37_StateChase(Pursuer *p) {
    Riccardo_Chase(p, D_0047AF50, &D_00442928, &D_00442938);
}

/* ---- the same shapes in other classes, generated from the functions they copy (2026-10-05) ---- */

/* (as Riccardo_ExitDone)  vtable +0x200: done at the door unless it has a side (+0x104), then the Pursuer's */
/* 0x0034BA20 */
void Kind37_ExitDone(Pursuer *p) {
    if (p->c.unk104[0] == 0) {
        PURSUER_STEP_DONE(p) = 1;
        return;
    }
    Pursuer_ExitDone(p);
}

/* ---- the same shapes in other classes, generated from the functions they copy (2026-10-05) ---- */
extern const PTMF Kind37_StateBehaviour_ptmf;
extern void Kind37_StateBehaviour(Pursuer *p);

/* (as Riccardo_ChaseDecision)  vtable +0x264: the next behaviour; his own (Kind37_StateBehaviour) unless at threat level 5, then the
   Pursuer's */
/* 0x0034D120 */
void Kind37_ChaseDecision(Pursuer *p) {
    s32 next;

    if (AT(gProgress, 0x7B8, u8) == 5) {
        Pursuer_ChaseDecision(p);
        return;
    }
    next = PU(p, 0x1758, s32);
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &Kind37_StateBehaviour_ptmf);
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
    Kind37_StateBehaviour(p);
}

/* 0x0034D230 */
s32 Kind37_AttackAnimB(void) {
    return 0x11;
}

/* 0x0034D240 */
s32 Kind37_AttackAnimA(void) {
    return 0x10;
}

/* 0x0034D250 */
f32 Kind37_FrightAttack(void) {
    return 7.0f;
}

/* 0x0034D260 */
f32 Kind37_FrightSeen(void) {
    return 2e+01f;
}

/* 0x0034D270 */
f32 Kind37_ReachHewie(void) {
    return 12.0f;
}

/* 0x0034D280 */
s32 Kind37_SlowWalkAnim(u8 *p) {
    f32 d;

    if (*(s32 *)(p + 0xC4) == 1) {
        return 0x203;
    }
    if (*(s32 *)(p + 0x16B8) == 2) {
        return 0x205;
    }
    if (p[0x1544] != 0 && *(s32 *)(p + 0x1540) == (s32)gCharPlayer) {
        d = *(f32 *)(p + 0x1588);
        if (d < 100.0f && !(d <= 0.0f)) {
            return 0x206;
        }
    }
    return 0x201;
}

/* ---- his other class (code 0x34BA20..0x34DBF0): his behaviour with its own tables, a
   breathing tint while on screen (+0x17C4 phase, +0x17C8 hold), layer 0x11, and seen-by-Fiona
   alerts (+0x16C4) ---- */

extern u8 D_00441CF0[], D_00441D20[], D_00441D60[], D_00441DA0[], D_00441DC0[], D_00441E00[],
    D_00441E30[], D_00441E60[], D_00441E80[], D_00441EB0[], D_00441EC0[], D_00441EE0[],
    D_00441F00[], D_00441F20[], str_t_23[], D_00441F48[], str_r_20[], D_00441F70[],
    D_00441FC0[], D_00441FF0[], D_00442020[], D_00442030[], D_00442060[], D_00442080[],
    D_004420B0[], D_004420E0[], D_00442110[], D_00442120[], D_00442130[], D_00442160[],
    D_00442178[], str_t_24[], D_004422D0[], D_00442310[], D_00442350[], D_004423A0[],
    D_004423D0[], D_00442410[], D_00442430[], D_00442480[], D_004424C0[], D_00442500[],
    D_00442510[], D_00442530[], D_00442550[], D_00442580[], str_t_25[], D_004425D0[],
    str_r_21[], D_004425F0[], D_00442630[], D_00442670[], D_004426B8[], D_004426D0[],
    D_00442700[], D_00442720[], D_00442760[], D_00442790[], D_004427D0[], D_004427E0[],
    D_004427F0[], D_00442810[], D_00442830[], str_t_26[];

/* its attack tables: [gProgress+0x30 bit 0x8000][alerted (+0xC4 == 1)] */
static u8 *const sAttackTables2[2][2][17] = {
    {
        { D_00441CF0, D_00441D60, D_00441D20, D_00441DA0, D_00441DC0, D_00441E00, D_00441E30,
          D_00441E60, D_00441E80, D_00441EB0, D_00441EC0, D_00441EE0, D_00441F00, D_00441F20,
          D_00441F48, str_r_20, str_t_23 },
        { D_00441F70, D_00441FF0, D_00441FC0, D_00442020, D_00442030, D_00442060, D_00442080,
          D_004420B0, D_004420E0, D_00442110, D_00442120, D_00442130, D_00442160, D_00442178,
          D_00441F48, str_r_20, str_t_24 },
    },
    {
        { D_004422D0, D_00442350, D_00442310, D_004423A0, D_004423D0, D_00442410, D_00442430,
          D_00442480, D_004424C0, D_00442500, D_00442510, D_00442530, D_00442550, D_00442580,
          D_004425D0, str_r_21, str_t_25 },
        { D_004425F0, D_00442670, D_00442630, D_004426B8, D_004426D0, D_00442700, D_00442720,
          D_00442760, D_00442790, D_004427D0, D_004427E0, D_004427F0, D_00442810, D_00442830,
          D_004425D0, str_r_21, str_t_26 },
    },
};

/* vtable +0x130 (as Lorenzo's Lorenzo2_AttackTable, alerted for mode 2) */
/* 0x0034BA60 */
void Kind37_AttackTable(Pursuer *p, s8 situation) {
    s32 alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;
    s32 alert = p->c.a.unkC4 == 1;

    PU(p, 0x1718, u8 *) = (u32)situation < 17 ? sAttackTables2[alt][alert][situation] : sAttackTables2[alt][0][0];
}

/* 0x0034BFE0 */
void Kind37_BehaviourEnded(Pursuer *p) {
    PU(p, 0x17C8, s32) = 0x96;
    Pursuer_BehaviourEnded(p);
}

extern const PTMF D_00442908, Kind37_StateChase_ptmf;

/* its behaviour: his (Riccardo_Behaviour) with its chase; the reach +0x17C0 is set up once */
/* 0x0034CDC0 */
void Kind37_StateBehaviour(Pursuer *p) {
    Riccardo_Behaviour(p, &D_00442908, &Kind37_StateChase_ptmf, Kind37_StateChase, 0);
}

extern u8 D_00442210[], D_004422B0[], D_00442890[], D_004428B0[], D_004428E0[];

/* vtable +0x30: its frame update: the stalkers' think; seen by Fiona enough (+0x16C4 >= 100) it
   is alerted (+0xC4 = 1, its alert tables, a stand-down of 600) and calms again below. The
   tint breathes with the phase +0x17C4 (alpha 80 +- 80), wandering by a random step while not
   held (+0x17C8 counts the hold down while it is free) */
/* 0x0034D320 */
void Kind37_Update(Pursuer *p) {
    PTMF *st = (PTMF *)((u8 *)p + 0x174C);
    VObject *r;
    u32 alpha;

    Stalker_ThinkStart(p);
    if (Npc_InPlayedRoom(p) != 0) {
        Pursuer_DoorNear(p);
        Pursuer_CryHeard(p);
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
    if (PU(p, 0x16C4, s32) < 100) {
        if (p->c.a.unkC4 == 1) {
            p->c.a.unkC4 = 0;
            PU(p, 0x16F5, u8) = 0;
            PU(p, 0x1790, s32) = 0;
        }
    } else {
        PU(p, 0x1790, s32) = 600;
        if (p->c.a.unkC4 == 1 || p->c.a.unkC4 == 2) {
            PU(p, 0x16F5, u8) = 0;
        } else {
            p->c.a.unkC4 = 1;
            if (AT(gProgress, 0x30, u32) & 0x8000) {
                PU(p, 0x1824, u8 *) = D_004428E0;
                PU(p, 0x1748, u8 *) = D_004428B0;
                PU(p, 0x1740, u8 *) = D_00442890;
            } else {
                PU(p, 0x1824, u8 *) = D_004422B0;
                PU(p, 0x1740, u8 *) = D_00442210;
            }
            if (PU(p, 0x175C, s32) != 0x1F) {
                PU(p, 0x16F5, u8) = 1;
            }
        }
    }
    Stalker_ThinkTimers(p);
    alpha = (u8)(u32)(80.0f + 80.0f * func_0031C248(PU(p, 0x17C4, f32)));
    if ((s32)alpha < 80) {
        VCALL(gRenderer, 0x6C, void (*)(VObject *))(gRenderer);
    }
    r = gRenderer;
    VCALL(r, 0x64, void (*)(VObject *, u32, s32))(r, alpha << 24 | 0x808080, 0);
    if (PU(p, 0x17C8, s32) != 0) {
        PU(p, 0x17C4, f32) = 0x1.921fb6p+0f;   /* pi / 2 */
        if (p->c.a.unkC4 != 1 && p->c.a.unkC4 != 2 && p->c.moveSub != 0x11 && p->c.moveSub != 9) {
            PU(p, 0x17C8, s32)--;
        }
    } else {
        f32 rnd = VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
        f32 step = 4.0f * func_0031C058(PU(p, 0x17C4, f32)) + 10.0f * rnd;

        PU(p, 0x17C4, f32) = Angle_Wrap(PU(p, 0x17C4, f32) + 0x1.921fb6p+1f * step / 180.0f);
    }
    VCALL(p, 0x40, void (*)(Pursuer *))(p);
    VCALL(p, 0x100, void (*)(Pursuer *))(p);
}

/* vtable +0x2C: the draw (as TintStalker_Draw, always shown) */
/* 0x0034D840 */
void Kind37_LightChange(Pursuer *p) {
    f32 mtx[4][4] __attribute__((aligned(16)));
    VObject *cam;
    u8 *m;

    if (p->c.a.disabled != 0) {
        return;
    }
    if (p->c.unkE4 == 1 && p->c.unk152C != 0x11) {
        VCALL(p, 0x80, void (*)(Pursuer *))(p);
    }
    if (p->c.unk152C == 0x11) {
        VCALL(gTexCache, 0x18, void (*)(VObject *))(gTexCache);
        cam = gCamera;
        VCALL(cam, 0x4C, void (*)(VObject *, f32 (*)[4]))(cam, mtx);
        VCALL(cam, 0x50, void (*)(VObject *, f32 (*)[4]))(cam, mtx);
        VCALL(cam, 0x58, void (*)(VObject *, f32 (*)[4]))(cam, mtx);
        VCALL(cam, 0x54, void (*)(VObject *, f32 (*)[4]))(cam, mtx);
    }
    m = p->c.motion;
    VCALL(m, 0x38, void (*)(void *, s32, u32, s32))(m, p->c.unk152C, p->c.a.navTri, 0);
    if (p->c.unk152C == 0x11) {
        VCALL(gCamera, 0x14, void (*)(VObject *))(gCamera);
    }
}

/* 0x0034D980 */
u8 *Kind37_ModelFileTable(Pursuer *p) {
    return (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) ? pstr_O_RCT_RCT_200_PCK_4 : pstr_O_RCT_RCT_200_PCK_3;
}

/* 0x0034D9C0 */
void *Kind37_ModelFiles(void) {
    return (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) ? D_00441810 : D_004417D0;
}

extern u8 D_00441890[], D_004419E0[], D_00441C70[], D_00441CA0[], D_004421A0[], D_004421F0[],
    D_00442230[], D_00442278[], D_00442290[], D_00442870[], D_004428C0[], D_0047AF48[];

/* vtable +0xF4: its setup over the Pursuer's (Pursuer_Setup); its reach +0x17C0 is 30, the
   tint off, layer 0x11 */
/* 0x0034DA00 */
void Kind37_Setup(Pursuer *p) {
    void *m;

    Pursuer_Setup(p);
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        p->c.hpMax = 125;
        PU(p, 0x171C, u8 *) = D_004419E0;
        PU(p, 0x1730, u8 *) = D_004421A0;
        PU(p, 0x1740, u8 *) = D_00442870;
        PU(p, 0x173C, u8 *) = D_00442230;
        PU(p, 0x1748, u8 *) = D_00442278;
        PU(p, 0x1824, u8 *) = D_004428C0;
        PU(p, 0x16DC, s32) = 50;              /* Hewie bite tolerance */
        PU(p, 0x16E8, f32) = 10.0f;
        PU(p, 0x16D4, s32) = 360;
        PU(p, 0x16D8, s32) = 900;
        PU(p, 0x16D0, s32) = 150;
        PU(p, 0x16E0, s32) = 5400;
        PU(p, 0x16E4, s32) = 45;
        PU(p, 0x17C0, f32) = 30.0f;
    } else {
        p->c.hpMax = 100;
        PU(p, 0x171C, u8 *) = D_004419E0;
        PU(p, 0x1730, u8 *) = D_004421A0;
        PU(p, 0x1740, u8 *) = D_004421F0;
        PU(p, 0x173C, u8 *) = D_00442230;
        PU(p, 0x1748, u8 *) = D_00442278;
        PU(p, 0x1824, u8 *) = D_00442290;
        PU(p, 0x16DC, s32) = 50;
        PU(p, 0x16E8, f32) = 10.0f;
        PU(p, 0x16D4, s32) = 300;
        PU(p, 0x16D8, s32) = 900;
        PU(p, 0x16D0, s32) = 150;
        PU(p, 0x16E0, s32) = 5400;
        PU(p, 0x16E4, s32) = 45;
        PU(p, 0x17C0, f32) = 30.0f;
    }
    p->c.a.radius = 3.0f;
    p->c.a.height = 17.0f;
    p->c.hp = p->c.hpMax;
    p->c.hearThreshold = 12;
    PU(p, 0x1720, u8 *) = D_00441C70;
    PU(p, 0x1724, u8 *) = D_00441CA0;
    PU(p, 0x16AC, u8 *) = D_00441890;
    PU(p, 0x1734, u8 *) = D_0047AF48;
    PU(p, 0x1694, f32) = 8.0f;
    PU(p, 0x169C, f32) = 1.5f;
    PU(p, 0x1698, f32) = 12.0f;
    PU(p, 0x16A0, f32) = 1.5f;
    m = p->c.motion;
    VCALL(m, 0x2C, void (*)(void *))(m);
    PU(p, 0x17C8, s32) = 0;
    PU(p, 0x17C4, s32) = 0;
    Character_Set152C(&p->c, 0x11);
}

/* the same with 4 more parts (0x50, +0x1310), vtable RiccardoModel_vtable */
/* 0x0038CEE0 */
void *RiccardoModel_ctor(u8 *m) {
    model_1310(m, RiccardoModel_vtable);
    func_00100340(m + 0x1310, Part50_ctor, Part50_dtor, 0x50, 4);
    AT(m, 0x1484, s32) = 0;
    AT(m, 0x1480, s32) = 0;
    return m;
}
