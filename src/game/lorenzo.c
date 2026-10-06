/* Lorenzo (kind 11, vtable 0x470720; code 0x2F8820..0x2F9210). He runs the Pursuer's behaviour
 * with his own tables, a slow turn (2 degrees a frame), and footsteps only in his move groups.
 * See pursuer.h. */
#include "common.h"
#include "pursuer.h"
#include "progress.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "navmesh.h"
#include "actor.h"
#include "ptmf.h"
#include "memcard.h"
#include "game.h"
#include "lorenzo.h"
#include "model.h"
#include "pursuer_ai.h"
#include "scene_game_members.h"
#include "skeleton.h"
#include "stalker_progress.h"

extern void *Lorenzo_vtable[];

extern u8 D_0041A5F0[];
void *Lorenzo_MotionFiles(void);

extern void *EffectBase_vtable[];
extern void *Marker_vtable[];
void *Obj470F90_dtor(u8 *o, s32 flags);

extern u8 D_004224C0[];
/* writes {x, 0, z} */
#define B5_SET3(out, x, z) ((out)[0] = (x), (out)[1] = 0.0f, (out)[2] = (z))

void *Lorenzo2_MotionFiles(void);
void Lorenzo2_ActionOffsets(void *self, s32 id, f32 *out);
void Lorenzo2_ExitDone(u8 *self);

extern u8 D_00422440[];
extern u8 D_00422480[];
extern u8 D_00423020[];
extern u8 D_00423070[];
extern u8 D_004238B0[];
extern u8 D_00423900[];
extern u8 D_00423950[];
extern u8 D_00423970[];
extern u8 D_00423990[];
extern u8 D_004239E0[];
extern u8 D_00423B30[];
extern u8 D_00423B70[];
extern u8 D_019910C8[];
extern u8 D_019910D8[];
/* Field access by byte offset into objects whose layout is not yet known. */
#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

#define U32(p, off) (*(u32 *)((u8 *)(p) + (off)))

#define PTR(p, off) (*(void * *)((u8 *)(p) + (off)))

void Kind39_ExitDone(u8 *o);
f32 Kind39_AttackRange(void);
f32 Kind39_ReachHewie(void);
void Kind39_Activate(Pursuer *p);

s32 Kind39_AttackAnimB(void);
s32 Kind39_AttackAnimA(void);

/* gProgress+0x30 bit 0x8000 selects between two data sets (difficulty/mode flag?) */
static inline s32 b5_prog_flag8000(void) {
    return U32(gProgress, 0x30) & 0x8000;
}

void Lorenzo2_SetRage(u8 *self, s32 alt);
f32 Lorenzo2_ThreatAmount(void);
f32 Lorenzo2_FrightAttack(void);
f32 Lorenzo2_FrightSeen(void);
f32 Lorenzo2_AttackRange(u8 *self);
f32 Lorenzo2_ReachHewie(u8 *self);
void *Lorenzo2_ModelFiles(void);
void *Kind12_ModelFiles(void);
void *Kind12_MotionFiles(void);
void Kind12_ExitDone(u8 *self);
f32 Kind12_AttackRange(void);
f32 Kind12_ReachHewie(void);
s32 Kind12_StopSounds(void *self);
f32 Kind39_ThreatAmount(void);
f32 Kind39_FrightAttack(void);
f32 Kind39_FrightSeen(void);

/* vtable +0x8: destructor */
/* 0x002F8820 */
Pursuer *Lorenzo_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = Lorenzo_vtable;
        if (p != NULL) {
            Pursuer_DestroyBase(p);
        }
        if ((s16)flags > 0) {
            Actor_Destroy(&p->c.a);
        }
    }
    return p;
}

/* 0x002F8930 */
void *Lorenzo_MotionFiles(void) {
    return D_0041A5F0;
}

/* vtable +0xA0: his turn rate, 2 degrees */
/* 0x002F8940 */
f32 Lorenzo_TurnRate(Pursuer *p) {
    return 0x1.1df46ap-5f;   /* 0.0349066 */
}

/* vtable +0x2D8: the offsets of his actions 10..15 (local) */
/* 0x002F8960 */
void Lorenzo_ActionOffsets(Pursuer *p, s32 kind, f32 *out) {
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
/* 0x002F8A10 */
void Lorenzo_ExitDone(Pursuer *p) {
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
/* 0x002F8A20 */
void Lorenzo_AttackTable(Pursuer *p, s8 situation) {
    s32 alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;

    PU(p, 0x1718, u8 *) = sAttackTables[alt][(u32)situation < 17 ? situation : 0];
}

/* vtable +0x2F4 / +0x2F0 */
/* 0x002F8CE0 */
f32 Lorenzo_AttackRange(Pursuer *p) {
    return 18.0f;
}

/* 0x002F8CF0 */
f32 Lorenzo_ReachHewie(Pursuer *p) {
    return 12.0f;
}

/* vtable +0x100: the Pursuer's footsteps (Pursuer_Footsteps), only in his move groups 0x200 / 0x400 */
/* 0x002F8D00 */
void Lorenzo_Footsteps(Pursuer *p) {
    switch (MOTION_ANIM(p) & 0xFF00) {
    case 0x400:
    case 0x200:
        Pursuer_Footsteps(p);
        break;
    }
}

/* vtable +0x30: his frame update: the stalkers' (see Stalker_ThinkStart), on screen with the hits,
   the cries heard, the behaviour step, stance and voice; off screen the behaviour step and the
   off-screen move */
/* 0x002F8D50 */
void Lorenzo_Update(Pursuer *p) {
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
    Stalker_ThinkEnd(p);
}

extern u8 D_0041A570[], D_0041A590[], D_0041A5B0[], D_0041A5D0[];

/* his model files (Pursuer_ModelFiles for kind 11) */
/* 0x002F9000 */
u8 *Lorenzo_ModelFileTable(Pursuer *p) {
    return (AT(gProgress, 0x30, u32) & 0x8000) ? D_0041A5D0 : D_0041A590;
}

/* vtable +0xF8: his model files in slot 2 */
/* 0x002F9040 */
u8 *Lorenzo_ModelFiles(Pursuer *p) {
    return (AT(gProgress, 0x30, u32) & 0x8000) ? D_0041A5B0 : D_0041A570;
}

extern u8 D_0041A630[], D_0041A760[], D_0041A770[], D_0041A830[], D_0041A850[], D_0041ABD0[],
    D_0041AC20[], D_0041AC40[], D_0041AFC0[], D_0041B008[], D_01990F90[], D_0047AC98[];

/* vtable +0xF4: his setup over the Pursuer's (Pursuer_Setup): his tables and stats (different
   when gProgress+0x30 bit 0x8000 is set) */
/* 0x002F9080 */
void Lorenzo_Setup(Pursuer *p) {
    Pursuer_Setup(p);
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

/* destructor (vtable Marker_vtable) */
/* 0x00301250 */
void *Obj470F90_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = Marker_vtable;
        AT(o, 0x0, void **) = EffectBase_vtable;
        if ((s16)flags > 0) {
            EffectMgr_free(o);
        }
    }
    return o;
}

/* ---- the second Lorenzo (kind 10, vtable 0x4715C0; code 0x309490..0x30C3E0): a strong one
   (250 hp, 400 when gProgress+0x30 bit 0x8000) with a mode 2 (+0x16B8) from threat level 1 ---- */

extern void *Lorenzo2_vtable[];

/* vtable +0x8: destructor */
/* 0x00309490 */
Pursuer *Lorenzo2_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = Lorenzo2_vtable;
        if (p != NULL) {
            Pursuer_DestroyBase(p);
        }
        if ((s16)flags > 0) {
            Actor_Destroy(&p->c.a);
        }
    }
    return p;
}

/* 0x003095A0 */
void *Lorenzo2_MotionFiles(void) {
    return D_004224C0;
}

/* 0x003095B0 */
void Lorenzo2_ActionOffsets(void *self, s32 id, f32 *out) {
    switch (id) {
    case 10:
    case 11:
        B5_SET3(out, 0x1.2d566c0000000p+0f /* 1.1771 */, 0x1.3a28f60000000p+3f /* 9.8175 */);
        break;
    case 12:
    case 13:
        B5_SET3(out, 0x1.e8587a0000000p-1f /* 0.9538 */, 0x1.abdcc60000000p+3f /* 13.3707 */);
        break;
    case 14:
        B5_SET3(out, -0x1.25a8580000000p+0f /* 1.1471 */, -0x1.42a64c0000000p+1f /* 2.5207 */);
        break;
    case 15:
        B5_SET3(out, -0x1.2f4f0e0000000p-3f /* 0.1481 */, -0x1.fd1eb80000000p+1f /* 3.9775 */);
        break;
    }
}

/* vtable +0x10C: never (0) */
/* 0x00309660 */
s32 Lorenzo2_InStance2Anim(Pursuer *p) {
    return 0;
}

/* 0x00309670 */
void Lorenzo2_ExitDone(u8 *self) {
    self[0x16EE] = 1;
}

/* vtable +0x138: the hit points of an attack entry; his grab 0xE01 is at Fiona herself */
/* 0x00309BA0 */
void Lorenzo2_BonePositions(Pursuer *p, s32 *e, f32 *a, f32 *b) {
    if (e[0] == 0xE01) {
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
/* 0x00309C90 */
void Lorenzo2_AttackTable(Pursuer *p, s8 situation) {
    s32 alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;
    s32 mode2 = PU(p, 0x16B8, s32) == 2;

    /* out of range: the first table without mode 2 */
    PU(p, 0x1718, u8 *) = (u32)situation < 17 ? sAttackTables2[alt][mode2][situation] : sAttackTables2[alt][0][0];
}

/* vtable +0x328: the Pursuer's walk (Pursuer_SlowWalkAnim); with gProgress+0x30 bit 0x8000 always the
   fast one 0x205 unless it's 0x203 */
/* 0x0030BBF0 */
s32 Lorenzo2_SlowWalkAnim(Pursuer *p) {
    s32 r = Pursuer_SlowWalkAnim(p);

    if ((AT(gProgress, 0x30, u32) & 0x8000) && r != 0x203) {
        r = 0x205;
    }
    return r;
}

/* vtable +0x310 / +0x30C */
/* 0x0030BC40 */
s32 Lorenzo2_AttackAnimB(Pursuer *p) {
    return 10;
}

/* 0x0030BC50 */
s32 Lorenzo2_AttackAnimA(Pursuer *p) {
    return 9;
}

/* 0x0030BC60 */
f32 Lorenzo2_ThreatAmount(void) {
    return b5_prog_flag8000() ? 45.0f : 2e+01f;
}

/* 0x0030BCA0 */
f32 Lorenzo2_FrightAttack(void) {
    return b5_prog_flag8000() ? 8.0f : 5.0f;
}

/* 0x0030BCE0 */
f32 Lorenzo2_FrightSeen(void) {
    return b5_prog_flag8000() ? 3e+01f : 1e+01f;
}

/* 0x0030BD20 */
f32 Lorenzo2_AttackRange(u8 *self) {
    if (b5_prog_flag8000()) {
        return S32(self, 0x16B8) == 2 ? 18.0f : 40.0f;
    }
    return S32(self, 0x16B8) == 2 ? 18.0f : 70.0f;
}

/* 0x0030BDA0 */
f32 Lorenzo2_ReachHewie(u8 *self) {
    if (b5_prog_flag8000()) {
        return S32(self, 0x16B8) == 2 ? 12.0f : 30.0f;
    }
    return S32(self, 0x16B8) == 2 ? 12.0f : 60.0f;
}

/* vtable +0x30: his frame update. On screen: back in contact (+0x29 / +0x2D cleared) once in
   action 0x10 or 0x21; mode 2 on from threat level 1 while chasing and idle, off at level 0; the
   cries heard, the behaviour step, Lorenzo2_BlowSparks and Lorenzo2_SlamDust, stance and voice. Off
   screen the behaviour step and the off-screen move */
/* 0x0030BE20 */
void Lorenzo2_Update(Pursuer *p) {
    PTMF *st = (PTMF *)((u8 *)p + 0x174C);

    Stalker_ThinkStart(p);
    if (Npc_InPlayedRoom(p) != 0) {
        Pursuer_DoorNear(p);
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
        Pursuer_CryHeard(p);
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        Lorenzo2_BlowSparks(p);
        Lorenzo2_SlamDust(p);
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

/* 0x0030C1F0 */
void *Lorenzo2_ModelFiles(void) {
    return b5_prog_flag8000() ? D_00422480 : D_00422440;
}

extern u8 D_00422500[], D_00422650[], D_00422690[], D_004226F0[], D_00422880[], D_00422A10[],
    D_00422A40[], D_00423020[], D_004230C0[], D_004230E0[], D_00423128[], D_004238B0[],
    D_00423950[], D_00423990[], D_019910C8[], D_0047AD10[];

/* vtable +0xF4: his setup over the Pursuer's (Pursuer_Setup): his tables and stats (different
   when gProgress+0x30 bit 0x8000 is set) */
/* 0x0030C230 */
void Lorenzo2_Setup(Pursuer *p) {
    Pursuer_Setup(p);
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
    obj[0x610 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = QuadDrawer_vtable;
}

/* his slam 0x2301: at its key (2), eight grey dust clouds of random size (320..640) at his
   hand (bone 0x32) */
/* 0x00309680 */
void Lorenzo2_SlamDust(Pursuer *p) {
    struct {
        f32 pos[4];
        s32 kind, r, g, b, size;
    } dp __attribute__((aligned(16)));
    u8 *mgr;
    VObject *rnd;
    u8 i;

    if (MOTION_ANIM(p) != 0x2301 || !(Motion_EventFlags(p->c.motion, 0, 0, 1) & 0xFF & 2)) {
        return;
    }
    sceVu0CopyVector(dp.pos, Skel_Bone(MOTION_AT(p, 0x810, u8 *), 0x32) + 0xC);
    dp.pos[3] = 1.0f;
    dp.kind = 2;
    dp.b = 0x50;
    dp.g = 0x50;
    dp.r = 0x50;
    mgr = gEffects;
    rnd = gRandom;
    for (i = 0; i < 8; i++) {
        s32 slot = Effect_New(mgr, 0x720, Dust_Init);

        dp.size = (s32)(320.0f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd)) + 320;
        EffectMgr_Start(mgr, slot, &dp);
    }
}

/* a spark (0x130 bytes, vtable 0x47A350, its part at +0xD0) */
extern void *D_0047A350[];

static inline void Spark_Init(void **obj) {
    obj[0] = D_0047A350;
    obj[0xD0 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0xD4 / 4] = -1;
    obj[0xD0 / 4] = QuadDrawer_vtable;
}

extern const f32 D_00423AC0[7][4];

/* his blows 0x1904 / 0xE00 / 0xE04 / 0xE05 (hand, bone 0x32) and 0xE06 (bone 0x28): at the
   key (0x20), seven sparks around the bone (offsets D_00423AC0) of random scale, speed and life */
/* 0x00309890 */
void Lorenzo2_BlowSparks(Pursuer *p) {
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
    if (!(Motion_EventFlags(p->c.motion, 0, 0, 1) & 0xFF & 0x20)) {
        return;
    }
    sp.one = 1.0f;
    sceVu0CopyVector(at, Skel_Bone(MOTION_AT(p, 0x810, u8 *), bone) + 0xC);
    rnd = gRandom;
    mgr = gEffects;
    for (i = 0; i < 7; i++) {
        s32 slot;

        sp.scale = 0.0f + 0.5f + 0.5f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);
        sp.speed = 0.0f + 0x1.19999ap+0f + 0x1.99999ap-5f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);   /* 1.1 + 0.05 */
        sp.life = (s32)(6.0f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd)) + 12;
        slot = Effect_New(mgr, 0x130, Spark_Init);
        sceVu0AddVector(sp.pos, at, D_00423AC0[i]);
        EffectMgr_Start(mgr, slot, &sp);
    }
}

/* an animation whose end ends the step */
static inline void Lorenzo2_PlayOut(Pursuer *p) {
    Character_RootMoveMasked(&p->c);
    if (AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    }
}

/* state: see Lorenzo2_PlayOut */
/* 0x0030B1E0 */
void Lorenzo2_StatePlayOut(Pursuer *p) {
    Lorenzo2_PlayOut(p);
}

extern const PTMF D_00423A38;

/* animation 0x1303 in state `st` (D_00423A38: Lorenzo2_StateSink), run at once */
static inline __attribute__((always_inline)) void Lorenzo2_SinkBehindAs(Pursuer *p, const PTMF *st, void (*fn)(Pursuer *)) {
    Pursuer_PlayAnimIf(p, 0x1303, 0);
    Actor_SetState(&p->c.a, st);
    fn(p);
}
#define Lorenzo2_SinkBehind(p) Lorenzo2_SinkBehindAs(p, &D_00423A38, Lorenzo2_StateSink)

/* 0x0030B7C0 */
void Lorenzo2_StateSinkBehind(Pursuer *p) {
    Lorenzo2_SinkBehind(p);
}

/* is his slam 0x2301 at its impact key (0x20) now (active and on screen) */
/* 0x0030BB70 */
s32 Lorenzo2_SlamImpact(Pursuer *p) {
    if (!p->c.a.active || Npc_InPlayedRoom(p) == 0 || MOTION_ANIM(p) != 0x2301) {
        return 0;
    }
    return (Motion_EventFlags(p->c.motion, 0, 0, 1) & 0xFF & 0x20) ? 1 : 0;
}

extern const PTMF D_00423AA8;

/* start of his grab: action 0x17 instead when he may not go for his target; finish the walk,
   then animation 0xE01 (with +0x16F7 when gProgress+0x30 bit 0x8000) and Lorenzo2_StateGrab */
/* 0x0030A4D0 */
void Lorenzo2_StartGrab(Pursuer *p) {
    if (!(Pursuer_MayGoForTarget(p) & 0xFF)) {
        p->c.unk104[0] = 0;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x17);
        return;
    }
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    Pursuer_PlayAnimIf(p, 0xE01, 0);
    p->c.unk104[0] = 0;
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        PU(p, 0x16F7, u8) = 1;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_00423AA8);
    Lorenzo2_StateGrab(p);
}

/* state: a sweep that hits both: at its key (2) Fiona and Hewie within the reach of attack
   entry 2 (+0x171C +0x48: +0xC reach) and on the mesh are hit (Relation_Request with the entry,
   stunning by its +0x18 chance), once each (+0x1760); its end ends the step */
/* 0x0030A650 */
void Lorenzo2_StateSweep(Pursuer *p) {
    Character_RootMoveMasked(&p->c);
    if (Motion_EventFlags(p->c.motion, 0, 0, 1) & 0xFF & 2) {
        u8 *e = PU(p, 0x171C, u8 *) + 0x48;
        u32 hit = 0;
        f32 d;

        d = Actor_Distance(&p->c.a, gCharPlayer->a.pos);
        if (d <= AT(e, 0xC, f32) && !(d < 0.0f) &&
            Actor_TriTo(&p->c.a, gCharPlayer->a.pos, p->c.a.navMask) != (u32)-1) {
            hit = (hit | 1) & 0xFF;
        }
        d = Actor_Distance(&p->c.a, gCharPartner->a.pos);
        if (d <= AT(e, 0xC, f32) && !(d < 0.0f) &&
            Actor_TriTo(&p->c.a, gCharPartner->a.pos, p->c.a.navMask) != (u32)-1) {
            hit = (hit | 2) & 0xFF;
        }
        if (Pursuer_MayGoForTarget(p) != 0 && (hit & ~PU(p, 0x1760, u8))) {
            s16 stun = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) <= AT(e, 0x18, f32) ? 0x8000 : 0;

            Relation_Request(gProgress, *(u8 *)&p->c.a.slot, hit, AT(e, 0x10, u8), AT(e, 0x12, u16), stun, AT(e, 0x14, f32));
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
    obj[0x610 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = QuadDrawer_vtable;
}

extern const PTMF D_00423A88;

/* state: sinking away (animation 0x1304). At its end (Lorenzo2_Sink) he heads under the floor
   toward his target: a point along the path (Npc_StepPath), or where he is when it's out of
   reach; state D_00423A88 (Lorenzo2_StateRise) */
/* the end of a sinking: out of contact (+0x29 / +0x2D), the sink effect where he stood, moved
   to the exit of PursuerGroup_Find kind 9 if any, 15 frames underground (+0x1624); returns the
   distance (Npc_PathLength) to his target's point `t` on the mesh */
static inline f32 Lorenzo2_Sink(Pursuer *p, f32 *t) {
    struct {
        f32 pos[4];
        s32 kind;
    } sk __attribute__((aligned(16)));
    u8 *mgr = gEffects;
    s32 slot;
    u32 k, tri;

    p->c.a.disabled = 1;
    p->c.a.unk2D = 1;
    slot = Effect_New(mgr, 0x700, Sink_Init);
    sceVu0CopyVector(sk.pos, p->c.a.pos);
    sk.kind = 0;
    EffectMgr_Start(mgr, slot, &sk);
    k = PursuerGroup_Find(gProgress, 9, *(u8 *)&p->c.a.slot) & 0xFF;
    if (k != 0xFF) {
        p->c.a.navTri = VCALL(gRooms, 0x34, u32 (*)(VObject *, u32, f32 *))(gRooms, k, p->c.a.pos);
    }
    PU(p, 0x1624, s32) = 15;
    sceVu0CopyVector(t, p->target->a.pos);
    tri = Npc_NearestWalkable(p, p->target->a.navTri, t, t);
    return Npc_PathLength(p, tri, t);
}

/* 0x0030AB80 */
void Lorenzo2_StateAB80(Pursuer *p) {
    f32 t[4] __attribute__((aligned(16)));
    f32 d;

    Character_RootMoveMasked(&p->c);
    if (!(AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END)) {
        return;
    }
    d = Lorenzo2_Sink(p, t);
    if (d < 0.0f) {
        sceVu0CopyVector(p->c.unk110, p->c.a.pos);
        p->c.unk104[0] = p->c.a.navTri;
    } else {
        u32 out;

        Npc_StepPath(p, &out, p->c.unk110, d);
        p->c.unk104[0] = out;
    }
    Actor_SetState(&p->c.a, &D_00423A88);
    Lorenzo2_StateRise(p);
}

extern const PTMF D_00423A78;

/* closing on his target: when it's out of reach by the mesh (Character_PathLength < 0), back to the
   walk and the step ends; otherwise he sinks away (0x1304, Lorenzo2_StateAB80) */
static inline __attribute__((always_inline)) void Lorenzo2_ApproachAs(Pursuer *p, const PTMF *st, void (*fn)(Pursuer *)) {
    f32 t[4] __attribute__((aligned(16)));
    u32 tri;

    sceVu0CopyVector(t, p->target->a.pos);
    tri = Npc_NearestWalkable(p, p->target->a.navTri, t, t);
    if (Character_PathLength(&p->c, tri, t, -1) < 0.0f) {
        if (PU(p, 0x1788, s32) != 0) {
            Pursuer_PlayAnimIf(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p), 0);
        }
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
        return;
    }
    Pursuer_PlayAnimIf(p, 0x1304, 0);
    Actor_SetState(&p->c.a, st);
    fn(p);
}
#define Lorenzo2_Approach(p) Lorenzo2_ApproachAs(p, &D_00423A78, Lorenzo2_StateAB80)

/* state: closing on his target (see Lorenzo2_Approach) */
/* 0x0030AE00 */
void Lorenzo2_StateApproach(Pursuer *p) {
    Lorenzo2_Approach(p);
}

/* the burst of his grab (0xFC0 bytes, vtable 0x47A010, four parts at +0xB50 / +0xB88 / +0xBC0 /
   +0xBF8) */
extern void *D_0047A010[];

static inline void Burst_Init(void **obj) {
    s32 k;

    obj[0] = D_0047A010;
    for (k = 0; k < 4; k++) {
        obj[(0xB50 + k * 0x38) / 4] = Helper469D00_vtable;
        ((s32 *)obj)[(0xB54 + k * 0x38) / 4] = -1;
        obj[(0xB50 + k * 0x38) / 4] = QuadDrawer_vtable;
    }
}

/* state: his grab. Until its aim key (frame 12, key 2) it follows the target (+0x110); at the
   hit key whoever is in reach of attack entry 3 (+0x171C +0x6C; Npc_WhoReachable at the aimed
   point) is hit, stunned by the entry's +0x18 chance, once each (+0x1760); the burst effect at
   the point (also to +0x1770). Its end ends the step */
/* 0x0030A210 */
void Lorenzo2_StateGrab(Pursuer *p) {
    Character_RootMoveMasked(&p->c);
    if (Motion_EventFlags(p->c.motion, 0, 0xC, 1) & 0xFF & 2) {
        sceVu0CopyVector(p->c.unk110, p->target->a.pos);
    } else if (Motion_EventFlags(p->c.motion, 0, 0, 1) & 0xFF & 2) {
        u8 *e = PU(p, 0x171C, u8 *) + 0x6C;
        u32 hit = Npc_WhoReachable(p, (s32)(u32)p->c.unk110, AT(e, 0xC, f32)) & 0xFF;
        u8 *mgr;

        if (Pursuer_MayGoForTarget(p) != 0 && (hit & ~PU(p, 0x1760, u8))) {
            s16 stun = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) <= AT(e, 0x18, f32) ? 0x8000 : 0;

            Relation_Request(gProgress, *(u8 *)&p->c.a.slot, hit, AT(e, 0x10, u8), AT(e, 0x12, u16), stun, AT(e, 0x14, f32));
            PU(p, 0x1764, s32) = 12;
        }
        sceVu0CopyVector((f32 *)((u8 *)p + 0x1770), p->c.unk110);
        mgr = gEffects;
        EffectMgr_Start(mgr, Effect_New(mgr, 0xFC0, Burst_Init), p->c.unk110);
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
   (Lorenzo2_StateSweep) with the rising effect */
/* travelling under the floor (+0x1624 frames left) through the mesh toward his goal
   (Npc_TriIfStandable), out of contact where it fails; 1 while travelling */
static inline s32 Lorenzo2_Underground(Pursuer *p) {
    if (PU(p, 0x1624, s32) <= 0) {
        return 0;
    }
    if (Npc_TriBlocked(p, p->c.a.navTri) != 0) {
        p->c.a.navTri = Npc_TriIfStandable(p, p->c.a.navTri);
        if (p->c.a.navTri != (u32)-1 && !(Npc_TriBlocked(p, p->c.a.navTri) & 0xFF)) {
            VCALL(gNavMesh, 0xC, void (*)(void *, u32, f32 *))(gNavMesh, p->c.a.navTri, p->c.a.pos);
        } else {
            Actor_TeleportRandom(&p->c.a, -1);
            p->c.a.disabled = 1;
            p->c.a.unk2D = 1;
        }
    }
    PU(p, 0x1624, s32)--;
    return 1;
}

/* the sweep of the other class (Kind39_StateSweep): at its key (2) whoever it touches
   (Npc_WhoSeen) is hit (Relation_Request with attack entry 2, stunning by its +0x18 chance),
   once each (+0x1760); its end ends the step */
static inline void Lorenzo2b_Sweep(Pursuer *p) {
    Character_RootMoveMasked(&p->c);
    if (Motion_EventFlags(p->c.motion, 0, 0, 1) & 0xFF & 2) {
        u32 hit = Npc_WhoSeen(p) & 0xFF;

        if (Pursuer_MayGoForTarget(p) != 0 && (hit & ~PU(p, 0x1760, u8))) {
            u8 *e = PU(p, 0x171C, u8 *) + 0x48;
            s16 stun = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) <= AT(e, 0x18, f32) ? 0x8000 : 0;

            Relation_Request(gProgress, *(u8 *)&p->c.a.slot, hit, AT(e, 0x10, u8), AT(e, 0x12, u16), stun, AT(e, 0x14, f32));
            PU(p, 0x1764, s32) = 12;
        }
    }
    if (AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    }
}

/* the rise into state `state` with its sweep: his (Lorenzo2_StateSweep) or the other class's */
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

        tri = Npc_NearestWalkable(p, p->c.unk104[0], p->c.unk110, goal);
        d = Npc_PathLength(p, tri, goal);
        sceVu0SubVector(v, p->target->a.pos, goal);
        v[3] = 0.0f;
        if (__builtin_sqrtf(sceVu0InnerProduct(v, v)) < 5.0f) {
            d -= 5.0f;
        }
        if (!(d <= 0.0f)) {
            Character_WaypointAhead(&p->c, &p->c.a.navTri, p->c.a.pos, d);
        }
        h = Actor_HeadingTo(&p->c.a, p->target->a.pos);
        p->c.a.angle[1] = h;
        sceVu0UnitMatrix(p->c.a.rot);
        sceVu0RotMatrixY(p->c.a.rot, p->c.a.rot, h);
        Pursuer_PlayAnimIf(p, 0xE02, 1);
        p->c.a.disabled = 0;
        p->c.a.unk2D = 0;
        p->c.moveMode = 8;
        p->c.moveSub = 0x1C;
        mgr = gEffects;
        p->c.unkE8 = p->target->unkE8;
        p->c.unkEC = p->target->unkEC;
        slot = Effect_New(mgr, 0x700, Sink_Init);
        sceVu0CopyVector(sk.pos, p->c.a.pos);
        sk.kind = 1;
        EffectMgr_Start(mgr, slot, &sk);
        Actor_SetState(&p->c.a, state);
        if (other) {
            Lorenzo2b_Sweep(p);
        } else {
            Lorenzo2_StateSweep(p);
        }
    }
}

/* 0x0030A850 */
void Lorenzo2_StateRise(Pursuer *p) {
    Lorenzo2_Rise(p, &D_00423A98, 0);
}

extern const PTMF D_00423A68;

/* start of his stalk from below: when he may go for his target, it's reachable by the mesh and
   there's no exit for him (PursuerGroup_Find kind 9): finish the walk, then close in (state
   D_00423A68, Lorenzo2_Approach); otherwise action 0x17 */
/* 0x0030AF20 */
void Lorenzo2_StartStalkBelow(Pursuer *p) {
    f32 t[4] __attribute__((aligned(16)));
    u32 tri;

    if (!(Pursuer_MayGoForTarget(p) & 0xFF)) {
        p->c.unk104[0] = 0;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x17);
        return;
    }
    sceVu0CopyVector(t, p->target->a.pos);
    tri = Npc_NearestWalkable(p, p->target->a.navTri, t, t);
    if (Character_PathLength(&p->c, tri, t, -1) < 0.0f || (PursuerGroup_Find(gProgress, 9, *(u8 *)&p->c.a.slot) & 0xFF) != 0xFF) {
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
   target, with the rising effect; its end ends the step (as Lorenzo2_StatePlayOut) */
/* 0x0030B240 */
void Lorenzo2_StateUnderFloor(Pursuer *p) {
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
    tri = Npc_NearestWalkable(p, p->c.unk104[0], p->c.unk110, goal);
    d = Npc_PathLength(p, tri, goal);
    if (!(d <= 0.0f)) {
        Character_WaypointAhead(&p->c, &p->c.a.navTri, p->c.a.pos, d);
    }
    h = Actor_HeadingTo(&p->c.a, p->target->a.pos);
    p->c.a.angle[1] = h;
    sceVu0UnitMatrix(p->c.a.rot);
    sceVu0RotMatrixY(p->c.a.rot, p->c.a.rot, h);
    Pursuer_PlayAnimIf(p, 0x1305, 1);
    mgr = gEffects;
    p->c.a.disabled = 0;
    p->c.a.unk2D = 0;
    p->c.unkE8 = p->target->unkE8;
    p->c.unkEC = p->target->unkEC;
    slot = Effect_New(mgr, 0x700, Sink_Init);
    sceVu0CopyVector(sk.pos, p->c.a.pos);
    sk.kind = 1;
    EffectMgr_Start(mgr, slot, &sk);
    Actor_SetState(&p->c.a, &D_00423A58);
    Lorenzo2_PlayOut(p);
}

extern const PTMF D_00423A48;

/* state: sinking to come up by his target (0x1303). At its end (Lorenzo2_Sink) he heads under
   the floor for a point 10 short of his target along the path, or where he is when it's nearer
   than 20; state D_00423A48 (Lorenzo2_StateUnderFloor) */
/* 0x0030B540 */
void Lorenzo2_StateSink(Pursuer *p) {
    f32 t[4] __attribute__((aligned(16)));
    f32 d;

    Character_RootMoveMasked(&p->c);
    if (!(AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END)) {
        return;
    }
    d = Lorenzo2_Sink(p, t);
    if (d < 20.0f) {
        sceVu0CopyVector(p->c.unk110, p->c.a.pos);
        p->c.unk104[0] = p->c.a.navTri;
    } else {
        u32 out;

        Npc_StepPath(p, &out, p->c.unk110, d - 10.0f);
        p->c.unk104[0] = out;
    }
    Actor_SetState(&p->c.a, &D_00423A48);
    Lorenzo2_StateUnderFloor(p);
}

extern const PTMF D_00423A28;

/* start of sinking to come up by his target: when it's 30 or more away by the mesh and there's
   no exit for him (PursuerGroup_Find kind 9), finish the walk and sink (Lorenzo2_SinkBehind).
   Otherwise: out of reach or when he may not go, action 0x17; else his grab (0x13, attack 1) */
/* 0x0030B840 */
void Lorenzo2_StartSink(Pursuer *p) {
    f32 t[4] __attribute__((aligned(16)));
    u32 tri;
    f32 d;

    sceVu0CopyVector(t, p->target->a.pos);
    tri = Npc_NearestWalkable(p, p->target->a.navTri, t, t);
    d = Character_PathLength(&p->c, tri, t, -1);
    if (d < 30.0f || (PursuerGroup_Find(gProgress, 9, *(u8 *)&p->c.a.slot) & 0xFF) != 0xFF) {
        if (d < 0.0f || !(Pursuer_MayGoForTarget(p) & 0xFF)) {
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

/* 0x0030BAA0 */
void Lorenzo2_SetRage(u8 *self, s32 alt) {
    if (b5_prog_flag8000()) {
        if (alt) {
            S32(self, 0x16B8) = 2;
            PTR(self, 0x1730) = D_00423900;
            PTR(self, 0x1748) = D_019910D8;
            PTR(self, 0x1740) = D_00423970;
            PTR(self, 0x173C) = D_004239E0;
        } else {
            S32(self, 0x16B8) = 0;
            PTR(self, 0x1730) = D_004238B0;
            PTR(self, 0x1748) = D_019910C8;
            PTR(self, 0x1740) = D_00423950;
            PTR(self, 0x173C) = D_00423990;
        }
    } else if (alt) {
        S32(self, 0x16B8) = 2;
        PTR(self, 0x1730) = D_00423070;
    } else {
        S32(self, 0x16B8) = 0;
        PTR(self, 0x1730) = D_00423020;
    }
}

/* ---- character kind 12 (vtable Kind12_vtable): a pursuer with his own tables and update ---- */

extern void *Kind12_vtable[], *Pursuer_vtable[], *NPC_vtable[], *Character_vtable[], *Actor_vtable[], *Marker_vtable[];
extern u8 D_00423D90[], D_00423DB0[], D_00423DE0[], D_00423DF8[], D_00423E10[], D_00423E38[],
    D_00423E50[], D_00423E70[], D_00423E88[], D_00423E98[], D_00423EB0[], D_00423EC8[],
    D_00423EE0[], D_00423F00[], D_00423F20[], D_00423F38[], D_00423F48[];
extern u8 D_00423FE0[], D_00424010[], D_00424040[], D_00424060[], D_00424080[], D_004240B0[],
    D_004240C0[], D_004240F0[], D_00424108[], D_00424118[], D_00424130[], D_00424158[],
    D_00424170[], D_00424190[], D_004241B0[], D_004241C8[], D_004241D8[];
extern u8 D_004241F0[], D_00423FB0[], D_019910F0[], D_00424238[], D_00423F60[], D_00423FD0[];
extern u8 D_00423CF0[], D_00423D60[], D_00423D80[], D_00423BB0[], D_00423CE0[], D_0047AD18[];

/* vtable +0x8: destructor (0x4718F0 -> Pursuer 0x46D810 -> NPC 0x46C220 -> Character); the
 * model freed for slots 3..5 */
/* 0x0030C3E0 */
Pursuer *Kind12_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = Kind12_vtable;
        p->c.a.vtbl = Pursuer_vtable;
        VCALL(p, 0x10, void (*)(Pursuer *))(p);
        if ((u32)p->c.a.slot >= 3 && (u32)p->c.a.slot < 6) {
            void **m = p->c.motion;

            if (m != NULL) {
                VCALL(m, 0x8, void (*)(void *, s32))(m, 1);
                p->c.motion = NULL;
            }
        }
        p->c.a.vtbl = NPC_vtable;
        VCALL(p, 0x10, void (*)(Pursuer *))(p);
        p->c.a.vtbl = Character_vtable;
        p->c.a.vtbl = Actor_vtable;
        if ((s16)flags > 0) {
            Actor_Destroy(&p->c.a);
        }
    }
    return p;
}

/* 0x0030C4F0 */
void *Kind12_ModelFiles(void) {
    return D_00423B30;
}

/* 0x0030C500 */
void *Kind12_MotionFiles(void) {
    return D_00423B70;
}

/* 0x0030C510 */
void Kind12_ExitDone(u8 *self) {
    self[0x16EE] = 1;
}

static void k12_mark_init(void **obj) {
    obj[0] = Marker_vtable;
}

/* the marker effect Marker_vtable over him ({1, 0, 1, its slot}) */
static inline __attribute__((always_inline)) void k12_mark(Pursuer *p) {
    u8 *mgr;
    s32 slot, arg[4] __attribute__((aligned(16)));

    PU(p, 0x17C0, u8) = 1;
    mgr = gEffects;
    slot = Effect_New(mgr, 0x20, k12_mark_init);
    arg[0] = 1;
    arg[1] = 0;
    AT(&arg[2], 0, f32) = 1.0f;
    arg[3] = slot;
    EffectMgr_Start(mgr, slot, arg);
}

/* vtable +0x38: Pursuer_ShowUp, then (when Npc_InPlayedRoom allows) the marker */
/* 0x0030C520 */
void Kind12_ShowUp(Pursuer *p) {
    Pursuer_ShowUp(p);
    if (Npc_InPlayedRoom(p) != 0) {
        k12_mark(p);
    } else {
        PU(p, 0x17C0, u8) = 0;
    }
}

/* vtable +0x148: the same over Pursuer_EnterRoom */
/* 0x0030C670 */
void Kind12_EnterRoom(Pursuer *p) {
    Pursuer_EnterRoom(p);
    if (Npc_InPlayedRoom(p) != 0) {
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
/* 0x0030C7C0 */
void Kind12_AttackTable(Pursuer *p, s8 situation) {
    s32 alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;

    PU(p, 0x1718, u8 *) = (u32)situation < 17 ? sK12Tables[alt][situation] : D_00423D90;
}

/* vtable +0x84: a request of kind 4 seen (+0x14E8, not yet handled +0x14F0) raises the threat,
 * except in game modes 6 / 7; then Pursuer_EventState and back to full health */
/* 0x0030CA80 */
void Kind12_EventState(Pursuer *p) {
    if (PU(p, 0x14E8, s32) == 4 && PU(p, 0x14F0, s32) == 0 && Pursuer_MayGoForTarget(p) != 0) {
        Progress *pr = gProgress;
        u8 mode = Progress_GetVar(pr, 0x26) & 0xFF;

        if (mode != 7 && mode != 6) {
            Relation_Request(pr, *(u8 *)&p->c.a.slot, 1, 3, 0, 0, 100.0f);
        }
    }
    Pursuer_EventState(p);
    p->c.hp = p->c.hpMax;
}

/* vtable +0x110: let his progress slot go if he holds one; -1 */
/* 0x0030CB40 */
s32 Kind12_GrabOrder(Pursuer *p) {
    Progress *pr = gProgress;

    if ((Progress_HasRelationCmd(pr, *(u8 *)&p->c.a.slot) & 0xFF) == 1) {
        SlotCmd_Cancel(pr, *(u8 *)&p->c.a.slot);
    }
    return -1;
}

/* 0x0030CBB0 */
f32 Kind12_AttackRange(void) {
    return 18.0f;
}

/* 0x0030CBC0 */
f32 Kind12_ReachHewie(void) {
    return 14.0f;
}

/* vtable +0x58: the marker off, then the Pursuer's */
/* 0x0030CBD0 */
void Kind12_Deactivate(Pursuer *p) {
    PU(p, 0x17C0, u8) = 0;
    Pursuer_Deactivate(p);
}

/* vtable +0x30: his frame update - the stalkers', with the senses kept on him while the
 * behaviour is fresh outside cutscenes, and the threat raised when he hits */
/* 0x0030CBE0 */
void Kind12_Update(Pursuer *p) {
    PTMF *st = (PTMF *)((u8 *)p + 0x174C);
    Progress *pr;

    VCALL(p, 0x84, void (*)(Pursuer *))(p);
    p->c.a.navMask = p->c.a.unk2B == 1 ? 8 : VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
    p->c.pathReq->mask = p->c.a.navMask;
    Npc_SensesWatching(p);
    if (PU(p, 0x16F6, u8) == 1) {
        if (AT(gProgress, 0x1FBEC1, u8) == 0) {
            PU(p, 0x1544, u8) = p->c.a.room == gCharPlayer->a.room;
            PU(p, 0x1545, u8) = 0;
            PU(p, 0x1546, u8) = 0;
            PU(p, 0x16C9, u8) = 5;
            PU(p, 0x16CA, u8) = 7;
        } else {
            Npc_WhoAroundEnding(p);
        }
    } else {
        PU(p, 0x1544, u8) = 0;
        PU(p, 0x1545, u8) = 0;
        PU(p, 0x1546, u8) = 0;
        PU(p, 0x16CB, u8) = 0;
        PU(p, 0x16CC, u8) = 0;
    }
    VCALL(p, 0x120, void (*)(Pursuer *))(p);
    Pursuer_MotionGroup(p);
    pr = gProgress;
    if (AT(pr, 0x1FBEC1, u8) == 0) {
        if (PU(p, 0x16C8, u8) != 0) {
            Pursuer_GoForFionaStance0(p);
        }
        VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
    }
    if (Npc_InPlayedRoom(p) != 0) {
        Pursuer_DoorNear(p);
        Pursuer_CryHeard(p);
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        if (Pursuer_MayGoForTarget(p) != 0 && ((Npc_WhoSeen(p) & 0xFF) & 1)) {
            Relation_Request(pr, *(u8 *)&p->c.a.slot, 1, 3, 0, 0, 100.0f);
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

/* 0x0030CF80 */
s32 Kind12_StopSounds(void *self) {
    return VCALL(gSound, 0x10, s32 (*)(void *, s32, s32))(gSound, 0, 0x400000);
}

/* vtable +0xF4: his setup over the Pursuer's: tables (two sets by gProgress+0x30 bit 0x8000),
 * 9999 health, his stats */
/* 0x0030CFA0 */
void Kind12_Setup(Pursuer *p) {
    static const union { u32 u; f32 f; } k02 = {0x3E4CCCCD};

    Pursuer_Setup(p);
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
    Character_Set152C(&p->c, 0x14);
}

/* ---- the same shapes in other classes, generated from the functions they copy (2026-10-05) ---- */
extern const f32 D_00445AF0[7][4];
extern u8 D_00444AF0[];
extern u8 D_00444AB0[];
extern u8 D_00444AD0[];
extern u8 D_00444A90[];

/* (as Lorenzo2_ActionOffsets)  vtable +0x2D8: the point (x, 0, z) for spot `id` 10..15 (others untouched) */
/* 0x00363610 */
void Kind39_ActionOffsets(void *self, s32 id, f32 *out) {
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

/* 0x003636C0 */
void Kind39_ExitDone(u8 *o) {
    AT(o, 0x16EE, u8) = 1;
}

/* (as Lorenzo2_SlamDust)  his slam 0x2301: at its key (2), eight grey dust clouds of random size (320..640) at his
   hand (bone 0x32) */
/* 0x003636D0 */
void Kind39_SlamDust(Pursuer *p) {
    struct {
        f32 pos[4];
        s32 kind, r, g, b, size;
    } dp __attribute__((aligned(16)));
    u8 *mgr;
    VObject *rnd;
    u8 i;

    if (MOTION_ANIM(p) != 0x2301 || !(Motion_EventFlags(p->c.motion, 0, 0, 1) & 0xFF & 2)) {
        return;
    }
    sceVu0CopyVector(dp.pos, Skel_Bone(MOTION_AT(p, 0x810, u8 *), 0x32) + 0xC);
    dp.pos[3] = 1.0f;
    dp.kind = 2;
    dp.b = 0x50;
    dp.g = 0x50;
    dp.r = 0x50;
    mgr = gEffects;
    rnd = gRandom;
    for (i = 0; i < 8; i++) {
        s32 slot = Effect_New(mgr, 0x720, Dust_Init);

        dp.size = (s32)(320.0f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd)) + 320;
        EffectMgr_Start(mgr, slot, &dp);
    }
}

/* (as Lorenzo2_BlowSparks)  his blows 0x1904 / 0xE00 / 0xE04 / 0xE05 (hand, bone 0x32) and 0xE06 (bone 0x28): at the
   key (0x20), seven sparks around the bone (offsets D_00445AF0) of random scale, speed and life */
/* 0x003638E0 */
void Kind39_BlowSparks(Pursuer *p) {
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
    if (!(Motion_EventFlags(p->c.motion, 0, 0, 1) & 0xFF & 0x20)) {
        return;
    }
    sp.one = 1.0f;
    sceVu0CopyVector(at, Skel_Bone(MOTION_AT(p, 0x810, u8 *), bone) + 0xC);
    rnd = gRandom;
    mgr = gEffects;
    for (i = 0; i < 7; i++) {
        s32 slot;

        sp.scale = 0.0f + 0.5f + 0.5f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);
        sp.speed = 0.0f + 0x1.19999ap+0f + 0x1.99999ap-5f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);   /* 1.1 + 0.05 */
        sp.life = (s32)(6.0f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd)) + 12;
        slot = Effect_New(mgr, 0x130, Spark_Init);
        sceVu0AddVector(sp.pos, at, D_00445AF0[i]);
        EffectMgr_Start(mgr, slot, &sp);
    }
}

/* (as Lorenzo2_BonePositions)  vtable +0x138: the hit points of an attack entry; his grab 0xE01 is at Fiona herself */
/* 0x00363BF0 */
void Kind39_BonePositions(Pursuer *p, s32 *e, f32 *a, f32 *b) {
    if (e[0] == 0xE01) {
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

/* (as Lorenzo2_StateGrab)  state: his grab. Until its aim key (frame 12, key 2) it follows the target (+0x110); at the
   hit key whoever is in reach of attack entry 3 (+0x171C +0x6C; Npc_WhoReachable at the aimed
   point) is hit, stunned by the entry's +0x18 chance, once each (+0x1760); the burst effect at
   the point (also to +0x1770). Its end ends the step */
/* 0x00363FA0 */
void Kind39_StateGrab(Pursuer *p) {
    Character_RootMoveMasked(&p->c);
    if (Motion_EventFlags(p->c.motion, 0, 0xC, 1) & 0xFF & 2) {
        sceVu0CopyVector(p->c.unk110, p->target->a.pos);
    } else if (Motion_EventFlags(p->c.motion, 0, 0, 1) & 0xFF & 2) {
        u8 *e = PU(p, 0x171C, u8 *) + 0x6C;
        u32 hit = Npc_WhoReachable(p, (s32)(u32)p->c.unk110, AT(e, 0xC, f32)) & 0xFF;
        u8 *mgr;

        if (Pursuer_MayGoForTarget(p) != 0 && (hit & ~PU(p, 0x1760, u8))) {
            s16 stun = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) <= AT(e, 0x18, f32) ? 0x8000 : 0;

            Relation_Request(gProgress, *(u8 *)&p->c.a.slot, hit, AT(e, 0x10, u8), AT(e, 0x12, u16), stun, AT(e, 0x14, f32));
            PU(p, 0x1764, s32) = 12;
        }
        sceVu0CopyVector((f32 *)((u8 *)p + 0x1770), p->c.unk110);
        mgr = gEffects;
        EffectMgr_Start(mgr, Effect_New(mgr, 0xFC0, Burst_Init), p->c.unk110);
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

/* (as Lorenzo2_StatePlayOut)  state: see Lorenzo2_PlayOut */
/* 0x00364F90 */
void Kind39_StatePlayOut(Pursuer *p) {
    Lorenzo2_PlayOut(p);
}

/* (as Lorenzo2_SlamImpact)  is his slam 0x2301 at its impact key (0x20) now (active and on screen) */
/* 0x00365850 */
s32 Kind39_SlamImpact(Pursuer *p) {
    if (!p->c.a.active || Npc_InPlayedRoom(p) == 0 || MOTION_ANIM(p) != 0x2301) {
        return 0;
    }
    return (Motion_EventFlags(p->c.motion, 0, 0, 1) & 0xFF & 0x20) ? 1 : 0;
}

/* 0x003658D0 */
s32 Kind39_AttackAnimB(void) {
    return 0xA;
}

/* 0x003658E0 */
s32 Kind39_AttackAnimA(void) {
    return 0x9;
}

/* as Lorenzo2_ThreatAmount */
/* 0x003658F0 */
f32 Kind39_ThreatAmount(void) {
    return b5_prog_flag8000() ? 45.0f : 2e+01f;
}

/* as Lorenzo2_FrightAttack */
/* 0x00365930 */
f32 Kind39_FrightAttack(void) {
    return b5_prog_flag8000() ? 8.0f : 5.0f;
}

/* as Lorenzo2_FrightSeen */
/* 0x00365970 */
f32 Kind39_FrightSeen(void) {
    return b5_prog_flag8000() ? 3e+01f : 1e+01f;
}

/* 0x003659B0 */
f32 Kind39_AttackRange(void) {
    return 18.0f;
}

/* 0x003659C0 */
f32 Kind39_ReachHewie(void) {
    return 12.0f;
}

/* (as Lorenzo_ModelFileTable)  his model files (Pursuer_ModelFiles for kind 11) */
/* 0x00365D10 */
u8 *Kind39_ModelFileTable(Pursuer *p) {
    return (AT(gProgress, 0x30, u32) & 0x8000) ? D_00444AF0 : D_00444AB0;
}

/* (as Lorenzo_ModelFileTable)  his model files (Pursuer_ModelFiles for kind 11) */
/* 0x00365D50 */
u8 *Kind39_ModelFiles(Pursuer *p) {
    return (AT(gProgress, 0x30, u32) & 0x8000) ? D_00444AD0 : D_00444A90;
}

/* ---- the same shapes in other classes, generated from the functions they copy (2026-10-05) ---- */
extern const PTMF D_00445AB8;
extern void Kind39_StateRise(Pursuer *p);
extern const PTMF D_00445A88;
extern const PTMF D_00445A78;
extern void Kind39_StateUnderFloor(Pursuer *p);

/* as Lorenzo2_StateAB80 */
/* 0x00364930 */
void Kind39_State4930(Pursuer *p) {
    f32 t[4] __attribute__((aligned(16)));
    f32 d;

    Character_RootMoveMasked(&p->c);
    if (!(AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END)) {
        return;
    }
    d = Lorenzo2_Sink(p, t);
    if (d < 0.0f) {
        sceVu0CopyVector(p->c.unk110, p->c.a.pos);
        p->c.unk104[0] = p->c.a.navTri;
    } else {
        u32 out;

        Npc_StepPath(p, &out, p->c.unk110, d);
        p->c.unk104[0] = out;
    }
    Actor_SetState(&p->c.a, &D_00445AB8);
    Kind39_StateRise(p);
}

/* (as Lorenzo2_StateUnderFloor)  state: under the floor, then he rises (0x1305) at his goal (+0x104 / +0x110) facing his
   target, with the rising effect; its end ends the step (as Lorenzo2_StatePlayOut) */
/* 0x00364FF0 */
void Kind39_StateUnderFloor(Pursuer *p) {
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
    tri = Npc_NearestWalkable(p, p->c.unk104[0], p->c.unk110, goal);
    d = Npc_PathLength(p, tri, goal);
    if (!(d <= 0.0f)) {
        Character_WaypointAhead(&p->c, &p->c.a.navTri, p->c.a.pos, d);
    }
    h = Actor_HeadingTo(&p->c.a, p->target->a.pos);
    p->c.a.angle[1] = h;
    sceVu0UnitMatrix(p->c.a.rot);
    sceVu0RotMatrixY(p->c.a.rot, p->c.a.rot, h);
    Pursuer_PlayAnimIf(p, 0x1305, 1);
    mgr = gEffects;
    p->c.a.disabled = 0;
    p->c.a.unk2D = 0;
    p->c.unkE8 = p->target->unkE8;
    p->c.unkEC = p->target->unkEC;
    slot = Effect_New(mgr, 0x700, Sink_Init);
    sceVu0CopyVector(sk.pos, p->c.a.pos);
    sk.kind = 1;
    EffectMgr_Start(mgr, slot, &sk);
    Actor_SetState(&p->c.a, &D_00445A88);
    Lorenzo2_PlayOut(p);
}

/* (as Lorenzo2_StateSink)  state: sinking to come up by his target (0x1303). At its end (Lorenzo2_Sink) he heads under
   the floor for a point 10 short of his target along the path, or where he is when it's nearer
   than 20; state D_00445A78 (Kind39_StateUnderFloor) */
/* 0x003652F0 */
void Kind39_StateSink(Pursuer *p) {
    f32 t[4] __attribute__((aligned(16)));
    f32 d;

    Character_RootMoveMasked(&p->c);
    if (!(AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END)) {
        return;
    }
    d = Lorenzo2_Sink(p, t);
    if (d < 20.0f) {
        sceVu0CopyVector(p->c.unk110, p->c.a.pos);
        p->c.unk104[0] = p->c.a.navTri;
    } else {
        u32 out;

        Npc_StepPath(p, &out, p->c.unk110, d - 10.0f);
        p->c.unk104[0] = out;
    }
    Actor_SetState(&p->c.a, &D_00445A78);
    Kind39_StateUnderFloor(p);
}

/* ---- the same states in the other class (its states D_00445Axx), sharing Lorenzo's helpers ---- */

extern const PTMF D_00445A58, D_00445A68, D_00445A98, D_00445AA8;
extern void Kind39_State4930(Pursuer *p);
extern void Kind39_StateSink(Pursuer *p);

/* (as Lorenzo2_StateApproach) */
/* 0x00364BB0 */
void Kind39_StateApproach(Pursuer *p) {
    Lorenzo2_ApproachAs(p, &D_00445AA8, Kind39_State4930);
}

/* (as Lorenzo2_StateSinkBehind) */
/* 0x00365570 */
void Kind39_StateSinkBehind(Pursuer *p) {
    Lorenzo2_SinkBehindAs(p, &D_00445A68, Kind39_StateSink);
}

/* (as Lorenzo2_StartStalkBelow) */
/* 0x00364CD0 */
void Kind39_StartStalkBelow(Pursuer *p) {
    f32 t[4] __attribute__((aligned(16)));
    u32 tri;

    if (!(Pursuer_MayGoForTarget(p) & 0xFF)) {
        p->c.unk104[0] = 0;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x17);
        return;
    }
    sceVu0CopyVector(t, p->target->a.pos);
    tri = Npc_NearestWalkable(p, p->target->a.navTri, t, t);
    if (Character_PathLength(&p->c, tri, t, -1) < 0.0f || (PursuerGroup_Find(gProgress, 9, *(u8 *)&p->c.a.slot) & 0xFF) != 0xFF) {
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
    Lorenzo2_ApproachAs(p, &D_00445AA8, Kind39_State4930);
}

/* (as Lorenzo2_StartSink) */
/* 0x003655F0 */
void Kind39_StartSink(Pursuer *p) {
    f32 t[4] __attribute__((aligned(16)));
    u32 tri;
    f32 d;

    sceVu0CopyVector(t, p->target->a.pos);
    tri = Npc_NearestWalkable(p, p->target->a.navTri, t, t);
    d = Character_PathLength(&p->c, tri, t, -1);
    if (d < 30.0f || (PursuerGroup_Find(gProgress, 9, *(u8 *)&p->c.a.slot) & 0xFF) != 0xFF) {
        if (d < 0.0f || !(Pursuer_MayGoForTarget(p) & 0xFF)) {
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
    Lorenzo2_SinkBehindAs(p, &D_00445A68, Kind39_StateSink);
}

extern const PTMF D_00445AD8;
extern void Kind39_StateGrab(Pursuer *p);

/* (as Lorenzo2_StartGrab) the other class's start of the grab */
/* 0x00364260 */
void Kind39_StartGrab(Pursuer *p) {
    if (!(Pursuer_MayGoForTarget(p) & 0xFF)) {
        p->c.unk104[0] = 0;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x17);
        return;
    }
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    Pursuer_PlayAnimIf(p, 0xE01, 0);
    p->c.unk104[0] = 0;
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        PU(p, 0x16F7, u8) = 1;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_00445AD8);
    Kind39_StateGrab(p);
}

extern u8 D_00444ED0[], D_00445990[], D_004459E0[], D_00445A00[], D_00445A48[], D_00444D40[], D_004454E0[],
    D_00445530[], D_00445550[], D_00445598[], D_00444CE0[], D_00445060[], D_00445090[], D_00444B50[],
    D_00444CA0[], D_0047B010[];

/* (as Lorenzo2_Setup) the other class's setup: its tables, and its own stats */
/* 0x00365D90 */
void Kind39_Setup(Pursuer *p) {
    Pursuer_Setup(p);
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

/* ---- the other class (vtable Kind39_vtable; code 0x363CE0..0x365AD0): its own tables, sweep and
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

/* vtable +0x130 (as Kind12_AttackTable) */
/* 0x00363CE0 */
void Kind39_AttackTable(Pursuer *p, s8 situation) {
    s32 alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;

    PU(p, 0x1718, u8 *) = sAttackTables3[alt][(u32)situation < 17 ? situation : 0];
}

/* its sweep state */
/* 0x003643E0 */
void Kind39_StateSweep(Pursuer *p) {
    Lorenzo2b_Sweep(p);
}

extern const PTMF D_00445AC8;

/* its rise from under the floor (as Lorenzo2_StateRise) into the sweep */
/* 0x00364510 */
void Kind39_StateRise(Pursuer *p) {
    Lorenzo2_Rise(p, &D_00445AC8, 1);
}

/* vtable +0x30: its frame update (as Lorenzo2_Update, without his mode 2), with its dust
   (Kind39_BlowSparks / Kind39_SlamDust) */
/* 0x003659D0 */
void Kind39_Update(Pursuer *p) {
    PTMF *st = (PTMF *)((u8 *)p + 0x174C);

    Stalker_ThinkStart(p);
    if (Npc_InPlayedRoom(p) != 0) {
        Pursuer_DoorNear(p);
        if (p->c.a.disabled != 0 && (PU(p, 0x175C, s32) == 0x10 || PU(p, 0x175C, s32) == 0x21)) {
            p->c.a.disabled = 0;
            p->c.a.unk2D = 0;
        }
        Pursuer_CryHeard(p);
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        Kind39_BlowSparks(p);
        Kind39_SlamDust(p);
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

/* Pursuer_Activate, then +0x31C (1) */
/* 0x00365CD0 */
void Kind39_Activate(Pursuer *p) {
    Pursuer_Activate(p);
    VCALL(p, 0x31C, void (*)(Pursuer *, s32))(p, 1);
}
