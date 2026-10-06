/* The pursuer kind 23 class (vtable TintStalker_vtable): a stalker drawn with a screen tint that fades
 * in while it is on screen (+0x17C4 delay, +0x17C8 shown, +0x17CC alpha, +0x17D0 fade state:
 * 1 in, 2 out, 3 hold). Its other methods are in src/leaf (b5_00315C50.c, small_hand.c,
 * tiny_gen.c, creature_gen.c). */
#include "common.h"
#include "pursuer.h"
#include "progress.h"
#include "renderer.h"
#include "globals.h"
#include "game.h"
#include "actor.h"
#include "ptmf.h"
#include "sce/libvu0.h"
#include "navmesh.h"
#include "model.h"
#include "input.h"
#include "memcard.h"
#include "heap.h"
#include "event.h"
#include "vecmath.h"
#include "scene_game.h"
#include "lights.h"
#include "msl.h"
#include "hewie.h"
#include "sound.h"
#include "effectmgr.h"
#include "debilitas.h"
#include "fiona.h"
#include "libc.h"
#include "effectmgr.h"   /* HitEffect_Spawn */
#include "music.h"
#include "cri/adx.h"
#include "item.h"
#include "charaction.h"
#include "gl2d.h"
#include "daniella.h"
#include "debilitas2.h"
#include "lorenzo.h"
#include "system.h"
#include "char_load.h"
#include "tintstalker.h"
#ifdef HG_NATIVE
#include <stdio.h>
#include <stdlib.h>
#include "glr.h"
#endif

void TintStalker_SetDelay(Pursuer *p);

s32 TintStalker_AttackAnimB(void);
s32 TintStalker_AttackAnimA(void);

extern u8 D_0042A2C0[];
extern u8 D_0042A300[];
#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

#define U32(p, off) (*(u32 *)((u8 *)(p) + (off)))

#define F32(p, off) (*(f32 *)((u8 *)(p) + (off)))

#define PTR(p, off) (*(void * *)((u8 *)(p) + (off)))

extern void *D_00470390[];
extern void *BonePoint_vtable[];
void *Part60_ctor(u8 *p);
void *HangPoint_ctor(u8 *p);
void *IK2_ctor(u8 *p);
extern u8 D_0042AED0[];
extern void *Kind23Model_vtable[];
extern void *HangPoint_ctor(u8 *p);
extern void *IK2_ctor(u8 *p);
extern void *Part60_ctor(u8 *p);
extern void *TintStalker_vtable[];
extern u8 D_0042A340[];
/* writes {x, 0, z} */
#define B5_SET3(out, x, z) ((out)[0] = (x), (out)[1] = 0.0f, (out)[2] = (z))

extern u8 pstr_O_RCT_RCT_200_PCK[];
extern u8 pstr_O_RCT_RCT_200_PCK_2[];
#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

static inline s32 b5_prog_flag8000(void);

void Kind23Model_Springs(u8 *m);
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

void Kind23Model_PartsRest(u8 *m);
void Kind23Model_Parts10A0(u8 *m);
void Kind23Model_PartsBE0(u8 *m);
void Kind23Model_Springs(u8 *m);
void *Kind23Model_dtor(u8 *m, s32 flags);
void Kind23Model_Vt3C(u8 *m);
void Kind23Model_Loaded(u8 *m);

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

static inline s32 b5_prog_flag8000(void);

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

Character *TintStalker_dtor(Character *c, s32 flags);
void *TintStalker_MotionFiles(void);
void TintStalker_Disable(Pursuer *p);
void TintStalker_Enable(Pursuer *p);
void TintStalker_DoorOffset(void *self, s32 id, f32 *out);
void TintStalker_ActionOffsets(void *self, s32 id, f32 *out);

/* the full base with its two parts, then the shared layout of the 0x1310-byte models: 4 parts
 * (0x50, +0x9A0), 4 members (0x40, +0xAE0), 12 parts (0x60, +0xC20), 5 members (0x70, +0x10E0) */
static inline void model_1310(u8 *m, void **vtbl) {
    u8 *e;

    ModelBase_ctor(m);
    AT(m, 0x0, void **) = HumanModel_vtable;
    IK2_ctor(m + 0x8D0);
    IK2_ctor(m + 0x930);
    AT(m, 0x0, void **) = vtbl;
    __construct_array(m + 0x9A0, HangPoint_ctor, HangPoint_dtor, 0x50, 4);
    for (e = m + 0xAE0; e < m + 0xBE0; e += 0x40) {
        AT(e, 0x30, void **) = BonePoint_vtable;
    }
    AT(m, 0xC14, s32) = 0;
    AT(m, 0xC10, s32) = 0;
    __construct_array(m + 0xC20, Part60_ctor, Part60_dtor, 0x60, 0xC);
    AT(m, 0x10D4, s32) = 0;
    AT(m, 0x10D0, s32) = 0;
    for (e = m + 0x10E0; e < m + 0x1310; e += 0x70) {
        AT(e, 0x30, void **) = D_00470390;
    }
}

void Kind23Model_Vt2C(u8 *self);
void Kind23Model_Vt30(void);
void Kind23Model_SecondaryMotion(u8 *self);
s32 Kind23Model_Vt98(void);
s32 Kind23Model_Vt9C(void);
s32 Kind23Model_Part0(void);
s32 Kind23Model_Part1(void);
s32 Kind23Model_Part2(void);
s32 Kind23Model_Part3(void);

/* gProgress+0x30 bit 0x8000 selects between two data sets (difficulty/mode flag?) */
static inline s32 b5_prog_flag8000(void) {
    return U32(gProgress, 0x30) & 0x8000;
}

f32 TintStalker_FrightAttack(void);
f32 TintStalker_FrightSeen(void);
f32 TintStalker_ReachHewie(void);
s32 TintStalker_SlowWalkAnim(u8 *self);
void *TintStalker_Table(void);

/* 0x00172C30 */
void *TintStalker_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x17, arg, TintStalker_vtable);
}
/* 0x0031F110 */
Character *TintStalker_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, TintStalker_vtable); }

/* 0x0031F220 */
void *TintStalker_MotionFiles(void) {
    return D_0042A340;
}

/* (pursuer classes) their Pursuer_Disable / Pursuer_Enable with +0x17C8 on / off */
/* 0x0031F230 */
void TintStalker_Disable(Pursuer *p) {
    Pursuer_Disable(p);
    PU(p, 0x17C8, u8) = 1;
}

/* 0x0031F260 */
void TintStalker_Enable(Pursuer *p) {
    Pursuer_Enable(p);
    PU(p, 0x17C8, u8) = 0;
}

/* 0x0031F290 */
void TintStalker_DoorOffset(void *self, s32 id, f32 *out) {
    switch (id) {
    case 1:
        B5_SET3(out, 0.0f, 0x1.e49ba60000000p+2f /* 7.572 */);
        break;
    case 3:
        B5_SET3(out, 0.0f, -0x1.b8e21a0000000p+2f /* 6.8888 */);
        break;
    case 0:
        B5_SET3(out, 0.0f, -0x1.5412060000000p+2f /* 5.3136 */);
        break;
    case 2:
        B5_SET3(out, 0.0f, 0x1.fbfb160000000p+2f /* 7.9372 */);
        break;
    }
}

/* 0x0031F330 */
void TintStalker_ActionOffsets(void *self, s32 id, f32 *out) {
    switch (id) {
    case 10:
    case 11:
        B5_SET3(out, -0x1.3eab360000000p-5f /* 0.0389 */, 0x1.4cf4f00000000p+3f /* 10.4049 */);
        break;
    case 12:
    case 13:
        B5_SET3(out, 0x1.7652be0000000p-1f /* 0.7311 */, 0x1.a808320000000p+3f /* 13.251 */);
        break;
    case 14:
        B5_SET3(out, -0x1.25a8580000000p+0f /* 1.1471 */, -0x1.42a64c0000000p+1f /* 2.5207 */);
        break;
    case 15:
        B5_SET3(out, -0x1.4fdf3c0000000p-2f /* 0.328 */, -0x1.324a8c0000000p+1f /* 2.3929 */);
        break;
    }
}
/* vtable +0xE4: at a door it breaks (Progress_ExitOpen), outside the ending (gProgress+0x1FBEC1),
   while opening or attacking it: use and damage it and change room through it (vtable +0x28) */
/* 0x0031F3E0 */
void TintStalker_BreakDoor(Pursuer *p, s32 exit) {
    Progress *pr = gProgress;

    if (!(Progress_ExitOpen(pr, p->c.a.room, exit) & 0xFF) || AT(pr, 0x1FBEC1, u8) != 0) {
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

/* vtable +0xF0: in the ending, use the door and close it behind (DoorHold_Open) */
/* 0x0031F4E0 */
void TintStalker_EndingDoor(Pursuer *p, s32 exit) {
    Progress *pr = gProgress;

    if (AT(pr, 0x1FBEC1, u8) != 0) {
        DoorHold_Take(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
        DoorHold_Open(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
    }
}

/* the tint fade: in ramps the alpha down by 4 from 130 to 112, out ramps it to 0 and hides it;
   the renderer's tint (+0x64) is grey with that alpha, and off (+0x6C) while hidden */
/* 0x0031F560 */
void TintStalker_Fade(Pursuer *p) {
    VObject *r;

    switch (PU(p, 0x17D0, s8)) {
    case 1:
        if (PU(p, 0x17CC, f32) < 116.0f) {
            PU(p, 0x17CC, f32) = 112.0f;
            PU(p, 0x17D0, s8) = 0;
        } else {
            PU(p, 0x17CC, f32) -= 4.0f;
        }
        break;
    case 3:
        PU(p, 0x17CC, f32) = 130.0f;
        break;
    case 2:
        PU(p, 0x17CC, f32) -= 4.0f;
        if (PU(p, 0x17CC, f32) <= 0.0f) {
            PU(p, 0x17CC, f32) = 0.0f;
            PU(p, 0x17D0, s8) = 0;
            PU(p, 0x17C8, u8) = 0;
        }
        break;
    }
    r = gRenderer;
    VCALL(r, 0x64, void (*)(VObject *, u32, s32))(r, (u8)(u32)PU(p, 0x17CC, f32) << 24 | 0x808080, 0);
    if (!PU(p, 0x17C8, u8)) {
        VCALL(r, 0x6C, void (*)(VObject *))(r);
    }
}

/* show the tint: held (3) while it is in state 2 (+0xC4), else fading in */
static inline void k23_show(Pursuer *p) {
    if (Npc_InPlayedRoom(p) != 0) {
        PU(p, 0x17C8, u8) = 1;
        PU(p, 0x17D0, s8) = p->c.a.unkC4 == 2 ? 3 : 1;
    }
}

/* the tint per frame: once the delay +0x17C4 has run out (it counts only while free: not in
   states 1 / 2, not moving 0x11 / 9) it shows on screen; hidden ones fade out off screen */
/* 0x0031F6E0 */
void TintStalker_Tint(Pursuer *p) {
    if (PU(p, 0x17C4, s32) != 0) {
        if (Npc_InPlayedRoom(p) != 0) {
            if (!PU(p, 0x17C8, u8) || (p->c.a.unkC4 == 2 && PU(p, 0x17D0, s8) != 3)) {
                k23_show(p);
            } else if (p->c.a.unkC4 != 2 && PU(p, 0x17D0, s8) == 3) {
                k23_show(p);
            }
        }
        if (p->c.a.unkC4 != 1 && p->c.a.unkC4 != 2 && p->c.moveSub != 0x11 && p->c.moveSub != 9) {
            PU(p, 0x17C4, s32)--;
        }
    }
    if (PU(p, 0x17C4, s32) == 0 && Npc_InPlayedRoom(p) != 0 && PU(p, 0x17C8, u8) == 1 &&
        PU(p, 0x17D0, s8) != 2 && Npc_InPlayedRoom(p) != 0) {
        PU(p, 0x17D0, s8) = 2;
    }
    TintStalker_Fade(p);
}

extern u8 D_0042A7F0[], D_0042A830[], D_0042A860[], D_0042A8A0[], D_0042A8D0[], D_0042A900[],
    D_0042A930[], D_0042A960[], D_0042A990[], D_0042A9C0[], D_0042A9D0[], D_0042AA00[],
    D_0042AA20[], D_0042AA50[], str_t_13[], D_0042AAA0[], str_r_8[], D_0042AAC0[];
extern u8 D_0042ABA0[], D_0042ABE0[], D_0042AC20[], D_0042AC70[], D_0042ACA0[], D_0042ACD0[],
    D_0042ACF0[], D_0042AD40[], D_0042AD80[], D_0042ADC0[], D_0042ADD0[], D_0042AE00[],
    D_0042AE20[], D_0042AE50[], str_t_14[], D_0042AEA8[], str_r_9[];

/* its attack tables for situations 0..16; the second set when gProgress+0x30 bit 0x8000 */
static u8 *const sAttackTables[2][17] = {
    { D_0042A7F0, D_0042A860, D_0042A830, D_0042A8A0, D_0042A8D0, D_0042A900, D_0042A930,
      D_0042A960, D_0042A990, D_0042A9C0, D_0042A9D0, D_0042AA00, D_0042AA20, D_0042AA50,
      D_0042AAA0, str_r_8, str_t_13 },
    { D_0042ABA0, D_0042AC20, D_0042ABE0, D_0042AC70, D_0042ACA0, D_0042ACD0, D_0042ACF0,
      D_0042AD40, D_0042AD80, D_0042ADC0, D_0042ADD0, D_0042AE00, D_0042AE20, D_0042AE50,
      D_0042AEA8, str_r_9, str_t_14 },
};

/* vtable +0x130: the attack table for a situation; in the ending situation 15 has its own */
/* 0x0031F880 */
void TintStalker_AttackTable(Pursuer *p, s8 situation) {
    s32 alt;

    if (AT(gProgress, 0x1FBEC1, u8) != 0 && situation == 15) {
        PU(p, 0x1718, u8 *) = D_0042AAC0;
        return;
    }
    alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;
    PU(p, 0x1718, u8 *) = sAttackTables[alt][(u32)situation < 17 ? situation : 0];
}

/* vtable +0x200: done at the door in the ending or without a side (+0x104), else the
   Pursuer's */
/* 0x0031FB80 */
void TintStalker_DoorDone(Pursuer *p) {
    if (AT(gProgress, 0x1FBEC1, u8) != 0 || p->c.unk104[0] == 0) {
        PURSUER_STEP_DONE(p) = 1;
        return;
    }
    Pursuer_ExitDone(p);
}

/* +0x17C4 / +0x17C8 = 150, then Pursuer_BehaviourEnded */
/* 0x0031FBE0 */
void TintStalker_SetDelay(Pursuer *p) {
    PU(p, 0x17C4, s32) = 0x96;
    Pursuer_BehaviourEnded(p);
}

/* 0x0031FBF0 */
s32 TintStalker_AttackAnimB(void) {
    return 0x11;
}

/* 0x0031FC00 */
s32 TintStalker_AttackAnimA(void) {
    return 0x10;
}

/* 0x0031FC10 */
f32 TintStalker_FrightAttack(void) {
    return 1e+01f;
}

/* 0x0031FC20 */
f32 TintStalker_FrightSeen(void) {
    return 3e+01f;
}

/* 0x0031FC30 */
f32 TintStalker_ReachHewie(void) {
    return 12.0f;
}

/* 0x0031FC40 */
s32 TintStalker_SlowWalkAnim(u8 *self) {
    f32 d;

    if (S32(self, 0xC4) == 1) {
        return 0x203;
    }
    if (S32(self, 0x16B8) == 2) {
        return 0x205;
    }
    if (self[0x1544] != 0 && PTR(self, 0x1540) == gCharPlayer) {
        d = F32(self, 0x1588);
        if (d < 100.0f && !(d <= 0.0f)) {
            return 0x206;
        }
    }
    return 0x201;
}

/* vtable +0x30: its frame update: the stalkers' (Stalker_ThinkStart / Stalker_ThinkEnd) with
   the tint (TintStalker_Tint) before the model and stance */
/* 0x0031FCE0 */
void TintStalker_Update(Pursuer *p) {
    PTMF *st = (PTMF *)((u8 *)p + 0x174C);

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
    Stalker_ThinkTimers(p);
    TintStalker_Tint(p);
    VCALL(p, 0x40, void (*)(Pursuer *))(p);
    VCALL(p, 0x100, void (*)(Pursuer *))(p);
}

/* vtable +0x5C: the Pursuer's reset with the tint off; in the ending the Hewie bite tolerance
   is 20 */
/* 0x0031FFA0 */
void TintStalker_Reset(Pursuer *p) {
    Pursuer_Activate(p);
    PU(p, 0x17C4, s32) = 0;
    PU(p, 0x17C8, u8) = 0;
    PU(p, 0x17CC, f32) = 0.0f;
    if (AT(gProgress, 0x1FBEC1, u8) != 0) {
        PU(p, 0x16DC, s32) = 20;
    }
}

/* vtable +0x2C: the draw while the tint shows; on layer 0x11 between the texture cache's
   +0x18 and the camera's matrices (+0x4C / +0x50 / +0x58 / +0x54 through one matrix) and its
   +0x14 after */
/* 0x00320000 */
void TintStalker_Draw(Pursuer *p) {
    f32 mtx[4][4] __attribute__((aligned(16)));
    VObject *cam;
    u8 *m;

    if (p->c.a.disabled != 0 || PU(p, 0x17C8, u8) != 1) {
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

/* 0x00320150 */
u8 *TintStalker_ModelFileTable(Pursuer *p) {
    return b5_prog_flag8000() ? pstr_O_RCT_RCT_200_PCK_2 : pstr_O_RCT_RCT_200_PCK;
}

/* 0x00320190 */
void *TintStalker_Table(void) {
    return b5_prog_flag8000() ? D_0042A300 : D_0042A2C0;
}

extern u8 D_0042A380[], D_0042A4D0[], D_0042A4E0[], D_0042A770[], D_0042A7A0[], D_0042AAD0[],
    D_0042AB20[], D_0042AB40[], D_0042AB88[], D_0047AD70[];

/* vtable +0xF4: its setup over the Pursuer's (Pursuer_Setup): tables and stats (more health
   and a closer reach when gProgress+0x30 bit 0x8000 is set), the tint off, layer 0x11 */
/* 0x003201D0 */
void TintStalker_Setup(Pursuer *p) {
    void *m;

    Pursuer_Setup(p);
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        p->c.hpMax = 125;
        PU(p, 0x171C, u8 *) = D_0042A4E0;
        PU(p, 0x1730, u8 *) = D_0042AAD0;
        PU(p, 0x1740, u8 *) = D_0042AB20;
        PU(p, 0x173C, u8 *) = D_0042AB40;
        PU(p, 0x1748, u8 *) = D_0042AB88;
        PU(p, 0x16DC, s32) = 50;              /* Hewie bite tolerance */
        PU(p, 0x16E8, f32) = 10.0f;
        PU(p, 0x16D4, s32) = 300;
        PU(p, 0x16D8, s32) = 1800;
        PU(p, 0x16D0, s32) = 1200;
        PU(p, 0x16E0, s32) = 5400;
        PU(p, 0x16E4, s32) = 45;
    } else {
        p->c.hpMax = 100;
        PU(p, 0x171C, u8 *) = D_0042A4E0;
        PU(p, 0x1730, u8 *) = D_0042AAD0;
        PU(p, 0x1740, u8 *) = D_0042AB20;
        PU(p, 0x173C, u8 *) = D_0042AB40;
        PU(p, 0x1748, u8 *) = D_0042AB88;
        PU(p, 0x16DC, s32) = 50;
        PU(p, 0x16E8, f32) = 15.0f;
        PU(p, 0x16D4, s32) = 300;
        PU(p, 0x16D8, s32) = 1800;
        PU(p, 0x16D0, s32) = 1200;
        PU(p, 0x16E0, s32) = 5400;
        PU(p, 0x16E4, s32) = 45;
    }
    p->c.a.radius = 3.0f;
    p->c.a.height = 17.0f;
    p->c.hp = p->c.hpMax;
    p->c.hearThreshold = 12;
    PU(p, 0x1720, u8 *) = D_0042A770;
    PU(p, 0x1724, u8 *) = D_0042A7A0;
    PU(p, 0x16AC, u8 *) = D_0042A380;
    PU(p, 0x16B0, u8 *) = D_0042A4D0;
    PU(p, 0x1734, u8 *) = D_0047AD70;
    PU(p, 0x1694, f32) = 8.0f;
    PU(p, 0x169C, f32) = 1.5f;
    PU(p, 0x1698, f32) = 12.0f;
    PU(p, 0x16A0, f32) = 1.5f;
    m = p->c.motion;
    VCALL(m, 0x2C, void (*)(void *))(m);
    PU(p, 0x17C4, s32) = 0;
    PU(p, 0x17C8, u8) = 0;
    PU(p, 0x17CC, f32) = 0.0f;
    Character_Set152C(&p->c, 0x11);
}

/* (as Kind09Model_dtor) another model's destructor: twelve 0x60 nodes at +0xC20, four 0x50 at
 * +0x9A0 */
/* 0x003203B0 */
void *Kind23Model_dtor(u8 *m, s32 flags) {
    if (m != NULL) {
        AT(m, 0x0, void **) = Kind23Model_vtable;
        __destroy_arr(m + 0xC20, Part60_dtor, 0x60, 0xC);
        __destroy_arr(m + 0x9A0, HangPoint_dtor, 0x50, 4);
        AT(m, 0x0, void **) = HumanModel_vtable;
        AT(m, 0x988, void **) = IK2_vtable;
        AT(m, 0x928, void **) = IK2_vtable;
        AT(m, 0x0, void **) = Model_vtable;
        AT(m, 0x0, void **) = ModelBase_vtable;
        AT(m, 0x1D0, void **) = D_0046B1C0;
        AT(m, 0x1D0, void **) = Helper469D00_vtable;
        AT(m, 0x10, void **) = D_0046ADA0;
        AT(m, 0x10, void **) = Helper469D00_vtable;
        if ((s16)flags > 0) {
            StalkerModel_delete(m);
        }
    }
    return m;
}

/* 0x00320500 */
void Kind23Model_Vt2C(u8 *self) {
    self[0xB6] |= 2;
}

/* 0x00320510 */
void Kind23Model_Vt30(void) {
}

/* 0x00320520 */
void Kind23Model_SecondaryMotion(u8 *self) {
    PTR(self, 0x874) = D_0042AED0;
}

/* 0x00320530 */
s32 Kind23Model_Vt98(void) {
    return 0x2D;
}

/* 0x00320540 */
s32 Kind23Model_Vt9C(void) {
    return 0x2C;
}

/* 0x00320550 */
s32 Kind23Model_Part0(void) {
    return 0x3;
}

/* 0x00320560 */
s32 Kind23Model_Part1(void) {
    return 0x7;
}

/* 0x00320570 */
s32 Kind23Model_Part2(void) {
    return 0x1E;
}

/* 0x00320580 */
s32 Kind23Model_Part3(void) {
    return 0x28;
}

/* (as RiccardoModel_PartsRest)  the twelve 0x60 parts at rest: their length along bone 1's Z axis (the second six the other
   way) from their anchors */
/* 0x00320590 */
void Kind23Model_PartsRest(u8 *m) {
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

/* (as RiccardoModel_Parts10A0)  the twelve 0x60 parts (bones 0xA..0x15) on +0x10A0 and its five capsules */
/* 0x003206A0 */
void Kind23Model_Parts10A0(u8 *m) {
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

/* (as RiccardoModel_PartsBE0)  the four parts (bones 0x16..0x19, two of two) on +0xBE0 and its four spheres on bone 2 */
/* 0x00320A60 */
void Kind23Model_PartsBE0(u8 *m) {
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

/* a model's +0x850 on, then Kind23Model_PartsBE0 and Kind23Model_Parts10A0 */
/* 0x00320C70 */
void Kind23Model_Springs(u8 *m) {
    AT(m, 0x850, u8) = 1;
    Kind23Model_PartsBE0(m);
    Kind23Model_Parts10A0(m);
}

/* ---- the rest of the model class at 0x320590 (copies of func_002F6xxx) and the one with
   vtable Kind33Model_vtable ---- */

/* +0x3C: the springs a frame (as RiccardoModel_Vt3C, two sets) */
/* 0x00320CB0 */
void Kind23Model_Vt3C(u8 *m) {
    s32 n = 1;
    s32 i;

    if (AT(m, 0x850, u8) != 0) {
        Kind23Model_PartsRest(m);
        n = 30;
    }
    SpringSet_Begin(m + 0xBE0);
    SpringSet_Begin(m + 0x10A0);
    for (i = 0; i < n; i++) {
        SpringSet_Step(m + 0xBE0);
        SpringSet_Step(m + 0x10A0);
    }
    SpringSet_Finish(m + 0xBE0);
    SpringSet_Finish(m + 0x10A0);
    AT(m, 0x850, u8) = 0;
}

/* +0xC: once loaded (as RiccardoModel_Loaded) */
/* 0x00320D70 */
void Kind23Model_Loaded(u8 *m) {
    static const u8 sParts[] = {0x98, 0x9A, 0xC4, 0xC6, 0xC8};
    u32 i;

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
    AT(m, 0x8BC, s32) = 0x2A;
    AT(m, 0x8B0, s32) = 0x23;
    AT(m, 0x8B4, s32) = 0x1A;
    AT(m, 0x860, f32) = 0.0f;
    AT(m, 0x864, f32) = 16.0f;
    AT(m, 0x868, f32) = 0.0f;
    AT(m, 0x854, s32) = 0;
    AT(m, 0x858, s32) = 0;
    Kind23Model_Springs(m);
    for (i = 0; i < sizeof(sParts); i++) {
        AT(m, sParts[i], u8) = 4;
        AT(m, sParts[i] + 1, u8) = 0x40;
    }
}

/* the gallery's 0x1310-byte model, vtable Kind23Model_vtable */
/* 0x0038CC90 */
void *Kind23Model_ctor(u8 *m) {
    model_1310(m, Kind23Model_vtable);
    return m;
}
