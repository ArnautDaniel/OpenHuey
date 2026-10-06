/* The third Debilitas class (kind 7, vtable Debilitas3_vtable; code 0x2CD7A0..0x2CF3xx): a Pursuer
 * with Debilitas's model and its own behaviour, used where the countdown runs (its reset gives
 * +0x16DC 0x10 while progress +0x1FBEC1 is set, pursuer.c Debilitas3_Activate). See debilitas.c. */
#include "common.h"
#include "pursuer.h"
#include "progress.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "navmesh.h"
#include "actor.h"
#include "ptmf.h"
#include "item.h"
#include "game.h"
#include "model.h"
#include "pursuer_ai.h"
#include "skeleton.h"
#include "stalker_progress.h"

extern u8 D_004137B0[], D_00414220[], D_00414270[], D_00413E10[], D_00413E60[], D_00413E80[], str_Z_5[];
extern u8 D_004139D0[], D_00413A10[], D_00413640[], D_004137A0[], D_0047AC08[];
extern const PTMF D_004135D0;

u32 Debilitas3_BlockFlags(void);
f32 Debilitas3_SpeedBase(void);
f32 Debilitas3_SpeedTop(void);
f32 Debilitas3_AttackRange(void);
f32 Debilitas3_ReachHewie(void);
f32 Debilitas3_AttackAngle(void);
f32 Debilitas3_Dist2E8(void);
f32 Debilitas3_ReachFiona(void);
f32 Debilitas3_LookSwing(void);
f32 Debilitas3_LookFrames(void);
f32 Debilitas3_TurnRateFast(void);
f32 Debilitas3_TurnRate(void);

s32 Kind27_AttackAnimB(void);
s32 Kind27_AttackAnimA(void);
void Kind27_StateNone(void);
s32 Debilitas3_AttackAnimB(void);
s32 Debilitas3_AttackAnimA(void);
void Debilitas3_StateNone(void);

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

void Debilitas3_SetTimer(u8 *p, s32 v);
void Debilitas3_Timer10s(u8 *p);

u32 Kind27_PathNodeSound(Pursuer *p);

extern u8 D_0042E440[];
extern u8 D_0042E480[];
/* Field access by byte offset into objects whose layout is not yet known. */
#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

#define U32(p, off) (*(u32 *)((u8 *)(p) + (off)))

/* gProgress+0x30 bit 0x8000 selects between two data sets (difficulty/mode flag?) */
static inline s32 b5_prog_flag8000(void) {
    return U32(gProgress, 0x30) & 0x8000;
}

u32 Kind27_BlockFlags(void);
f32 Kind27_SpeedBase(void);
f32 Kind27_SpeedTop(void);
f32 Kind27_AttackRange(void);
f32 Kind27_ReachHewie(void);
f32 Kind27_AttackAngle(void);
f32 Kind27_Dist2E8(void);
f32 Kind27_ReachFiona(void);
f32 Kind27_LookSwing(void);
f32 Kind27_LookFrames(void);
f32 Kind27_TurnRateFast(void);
f32 Kind27_TurnRate(void);
void Kind27_SetTimer(u8 *self, s32 t);
void Kind27_Timer10s(u8 *self);
void *Kind27_ModelFiles(void);

/* vtable +0xF4: his setup over the Pursuer's (Pursuer_Setup): his tables and stats (165 hp and
 * +0x16E8 18 when gProgress+0x30 bit 0x8000 is set, else 110 hp and 12) */
/* 0x002CF1C0 */
void Debilitas3_Setup(Pursuer *p) {
    Pursuer_Setup(p);
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        p->c.hpMax = 165;
        PU(p, 0x171C, u8 *) = D_004137B0;
        PU(p, 0x1730, u8 *) = D_00414220;
        PU(p, 0x1740, u8 *) = D_00414270;
        PU(p, 0x173C, u8 *) = D_00413E80;
        PU(p, 0x1748, u8 *) = str_Z_5;
        PU(p, 0x16DC, s32) = 40;              /* Hewie bite tolerance */
        PU(p, 0x16E8, f32) = 18.0f;
    } else {
        p->c.hpMax = 110;
        PU(p, 0x171C, u8 *) = D_004137B0;
        PU(p, 0x1730, u8 *) = D_00413E10;
        PU(p, 0x1740, u8 *) = D_00413E60;
        PU(p, 0x173C, u8 *) = D_00413E80;
        PU(p, 0x1748, u8 *) = str_Z_5;
        PU(p, 0x16DC, s32) = 40;
        PU(p, 0x16E8, f32) = 12.0f;
    }
    PU(p, 0x16D4, s32) = 300;   /* frames */
    PU(p, 0x16D8, s32) = 1800;
    PU(p, 0x16D0, s32) = 1350;
    PU(p, 0x16E0, s32) = 9000;
    PU(p, 0x16E4, s32) = 150;
    p->c.a.radius = 5.0f;
    p->c.a.height = 20.0f;
    p->c.hp = p->c.hpMax;
    p->c.hearThreshold = 0;
    PU(p, 0x16B4, u8) = 2;
    PU(p, 0x1714, const PTMF *) = &D_004135D0;
    PU(p, 0x1720, u8 *) = D_004139D0;
    PU(p, 0x1724, u8 *) = D_00413A10;
    PU(p, 0x16AC, u8 *) = D_00413640;
    PU(p, 0x16B0, u8 *) = D_004137A0;
    PU(p, 0x1734, u8 *) = D_0047AC08;
    PU(p, 0x17EC, s32) = 0;
    PU(p, 0x1694, f32) = 6.0f;
    PU(p, 0x169C, f32) = 1.5f;
    PU(p, 0x1698, f32) = 13.0f;
    PU(p, 0x16A0, s32) = 0;
}

/* 0x002CF380 */
void Debilitas3_StateNone(void) {
}

/* vtable +0xB4: head for character `c` (NULL: the target) - Debilitas's Debilitas_HeadFor without
 * the resting check */
/* 0x002CEC40 */
void Debilitas3_HeadFor(Pursuer *p, Character *c) {
    s32 side;

    if (c == NULL) {
        c = p->target;
    }
    side = PU(p, 0x1598, s32);
    if (Npc_SameRoomOtherSide(p, c) != 0) {
        side = Npc_CharSideBehind(p, c);
    } else {
        s32 room = c->a.room;

        if (p->c.a.room == room || PU(p, 0x1594, s32) != room) {
            side = -1;
        }
        if (room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
            PU(p, 0x15A4, s32) = Npc_NearestWalkable(p, c->a.navTri, c->a.pos, (f32 *)((u8 *)p + 0x15B0));
        }
    }
    if (Character_Route(&p->c, c->a.room, side, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = c->a.room;
        PU(p, 0x1598, s32) = side;
    } else {
        Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
}

/* vtable +0xB0: head for Fiona - Debilitas's Debilitas_HeadForFiona without the resting check */
/* 0x002CED60 */
void Debilitas3_HeadForFiona(Pursuer *p) {
    Character *f = gCharPlayer;
    s32 side = PU(p, 0x1598, s32);

    if (Npc_SameRoomOtherSide(p, f) != 0) {
        side = Npc_CharSideBehind(p, f);
    } else {
        if (p->c.a.room == f->a.room || PU(p, 0x1594, s32) != f->a.room) {
            side = -1;
        }
        PU(p, 0x15A4, s32) = Npc_NearestWalkable(p, f->a.navTri, f->a.pos, (f32 *)((u8 *)p + 0x15B0));
    }
    if (Character_Route(&p->c, f->a.room, side, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = f->a.room;
        PU(p, 0x1598, s32) = side;
    } else {
        Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
}

/* vtable +0xAC: head for triangle `tri` at `pos` in `room` (-1 the current one) - as Debilitas's
 * Debilitas_GoTo */
/* 0x002CEE50 */
void Debilitas3_GoTo(Pursuer *p, u32 tri, const f32 *pos, s32 room) {
    void *nm;

    if (room == -1) {
        room = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    }
    if (room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        PU(p, 0x15A4, u32) = Npc_NearestWalkable(p, tri, pos, (f32 *)((u8 *)p + 0x15B0));
    }
    nm = gNavMesh;
    if (VCALL(nm, 0x10, s32 (*)(void *, u32, const f32 *))(nm, PU(p, 0x15A4, u32), (f32 *)((u8 *)p + 0x15B0)) != 3) {
        VCALL(nm, 0xC, void (*)(void *, u32, f32 *))(nm, PU(p, 0x15A4, u32), (f32 *)((u8 *)p + 0x15B0));
    }
    if (Character_Route(&p->c, room, -1, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = room;
        PU(p, 0x1598, s32) = -1;
    } else {
        Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
}

/* 0x002CEF90 */
u32 Debilitas3_BlockFlags(void) { return 0x2C020068; }

/* 0x002CEFA0 */
s32 Debilitas3_AttackAnimB(void) {
    return 0xE;
}

/* 0x002CEFB0 */
s32 Debilitas3_AttackAnimA(void) {
    return 0xD;
}

/* 0x002CEFC0 */
f32 Debilitas3_SpeedBase(void) { return 0x1.3333340000000p-1f /* 0.6 */; }

/* 0x002CEFE0 */
f32 Debilitas3_SpeedTop(void) { return 0x1.6666660000000p+0f /* 1.4 */; }

/* 0x002CF000 */
f32 Debilitas3_AttackRange(void) { return 24.0f; }

/* 0x002CF010 */
f32 Debilitas3_ReachHewie(void) { return 16.0f; }

/* 0x002CF020 */
f32 Debilitas3_AttackAngle(void) { return 20.0f; }

/* 0x002CF030 */
f32 Debilitas3_Dist2E8(void) { return 10.0f; }

/* 0x002CF040 */
f32 Debilitas3_ReachFiona(void) { return 20.0f; }

/* 0x002CF050 */
f32 Debilitas3_LookSwing(void) { return 0x1.eb851e0000000p-4f /* 0.12 */; }

/* 0x002CF070 */
f32 Debilitas3_LookFrames(void) { return 60.0f; }

/* 0x002CF080 */
f32 Debilitas3_TurnRateFast(void) { union { u32 u; f32 f; } c = { 0x3E0EFA35 }; return c.f; }

/* 0x002CF0A0 */
f32 Debilitas3_TurnRate(void) { union { u32 u; f32 f; } c = { 0x3D567750 }; return c.f; }

/* 0x002CF0C0 */
void Debilitas3_SetTimer(u8 *p, s32 v) { F(p, 0x1660, s32) = v != 0 ? v : 900; }

/* 0x002CF0E0 */
void Debilitas3_Timer10s(u8 *p) { F(p, 0x1660, s32) = 600; }

/* a stun: the flinch (0x1004) unless already reeling, and frozen while it lasts - as Debilitas's
 * Debilitas_StateStun */
/* 0x002CE050 */
void Debilitas3_StateStun(Pursuer *p) {
    if (PU(p, 0x1788, s32) != 0x1000 && p->c.unk14D0 <= 0) {
        Pursuer_PlayAnimIf(p, 0x1004, 1);
    }
    if (((AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0)) {
        PU(p, 0x1770, s32) = 0;
        PU(p, 0x1774, s32) = 0;
        PU(p, 0x1778, s32) = 0;
        PU(p, 0x16F7, u8) = 0;
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    } else if (p->c.unk14D0 <= 0) {
        Character_RootMoveMasked(&p->c);
    }
}

extern const PTMF Pursuer_AttackNextStep_ptmf5;

/* the end of his lunge animation: at threat level 5 (gProgress+0x7B8) it leads straight into
 * attack 8 (state `attack`); otherwise it ends the step */
static inline void Debilitas3_LungeEnd(Pursuer *p, const PTMF *attack) {
    Character_RootMoveMasked(&p->c);
    if (AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        if (AT(gProgress, 0x7B8, u8) != 5) {
            PURSUER_STEP_DONE(p) = 1;
            PURSUER_STEP_NEXT(p) = 1;
            return;
        }
        PU(p, 0x1728, s32) = 8;
        Actor_SetState(&p->c.a, attack);
        p->c.moveMode = 8;
        Pursuer_AttackNextStep(p);
    }
}

/* state: the lunge animation (see Debilitas3_LungeEnd) */
/* 0x002CDDD0 */
void Debilitas3_StateLunge(Pursuer *p) {
    Debilitas3_LungeEnd(p, &Pursuer_AttackNextStep_ptmf5);
}

extern const PTMF Debilitas3_StateLunge_ptmf;

/* start of the lunge: finish the current walk, then animation 0x1306 in state Debilitas3_StateLunge */
/* 0x002CDE90 */
void Debilitas3_StartLunge(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    Pursuer_PlayAnimIf(p, 0x1306, 0);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &Debilitas3_StateLunge_ptmf);
    Debilitas3_LungeEnd(p, &Pursuer_AttackNextStep_ptmf5);
}

extern u8 D_00413A60[], D_00413AB0[], D_00413AF0[], D_00413B50[], D_00413B90[], D_00413BD0[],
    D_00413BF0[], D_00413C30[], D_00413C70[], D_00413CB0[], D_00413CC0[], D_00413D10[],
    D_00413D30[], D_00413D70[], str_t_5[], D_00413DE8[], D_00413DF8[];
extern u8 D_00413EE0[], D_00413F40[], D_00413F90[], D_00413FE0[], D_00414010[], D_00414040[],
    D_00414050[], D_00414090[], D_004140B0[], D_004140F0[], D_00414100[], D_00414140[],
    D_00414160[], D_004141A0[], str_t_6[], D_004141F8[], D_00414208[];

/* his attack tables for each situation 0..16; the second set when gProgress+0x30 bit 0x8000 */
static u8 *const sAttackTables3[2][17] = {
    { D_00413A60, D_00413AF0, D_00413AB0, D_00413B50, D_00413B90, D_00413BD0, D_00413BF0,
      D_00413C30, D_00413C70, D_00413CB0, D_00413CC0, D_00413D10, D_00413D30, D_00413D70,
      D_00413DE8, D_00413DF8, str_t_5 },
    { D_00413EE0, D_00413F90, D_00413F40, D_00413FE0, D_00414010, D_00414040, D_00414050,
      D_00414090, D_004140B0, D_004140F0, D_00414100, D_00414140, D_00414160, D_004141A0,
      D_004141F8, D_00414208, str_t_6 },
};

/* vtable +0x130: the attack table for a situation */
/* 0x002CD7B0 */
void Debilitas3_AttackTable(Pursuer *p, s8 situation) {
    s32 alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;

    PU(p, 0x1718, u8 *) = (u32)situation < 17 ? sAttackTables3[alt][situation] : sAttackTables3[alt][0];
}

/* vtable +0x228 / +0x224: the Pursuer's */
/* 0x002CE6A0 */
void Debilitas3_DoorAnim(Pursuer *p) {
    Pursuer_DoorAnim(p);
}

/* 0x002CE6B0 */
void Debilitas3_Stairs(Pursuer *p) {
    Pursuer_Stairs(p);
}

/* a grab at Fiona: at the animation's hit key, once, if she's within reach (gProgress vtable
 * +0x2C), it lands (Relation_Request kind 1); over when the animation ends, she's out of sight, or
 * 60 units away - as Debilitas's */
static inline void Debilitas3_Grab(Pursuer *p) {
    Character_RootMoveMasked(&p->c);
    if ((Motion_EventFlags(p->c.motion, 0, 0, 1) & 0xFF & 2) && !(PU(p, 0x1760, u8) & 1)) {
        Progress *pr = gProgress;

        if (VCALL(pr, 0x2C, s32 (*)(Progress *, u32, s32, s32, f32))(pr, *(u8 *)&p->c.a.slot, 0x1E, 0, 5.0f) != 0) {
            Relation_Request(pr, *(u8 *)&p->c.a.slot, 1, 6, 0, 3, 10.0f);
            PU(p, 0x1760, u8) |= 1;
        }
    }
    if ((AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) || PU(p, 0x1544, u8) == 0 ||
        !(PU(p, 0x1590, f32) < 60.0f) || PU(p, 0x1590, f32) < 0.0f) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    }
}

/* state: the grab (see Debilitas3_Grab) */
/* 0x002CDA70 */
void Debilitas3_StateGrab(Pursuer *p) {
    Debilitas3_Grab(p);
}

extern const PTMF Debilitas3_StateGrab_ptmf;

/* start of the grab: finish the current walk, then animation 0xE06 in state Debilitas3_StateGrab */
/* 0x002CDBA0 */
void Debilitas3_StartGrab(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    Pursuer_PlayAnimIf(p, 0xE06, 0);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &Debilitas3_StateGrab_ptmf);
    Debilitas3_Grab(p);
}

void Debilitas3_StateBlow(Pursuer *p);
extern const PTMF Debilitas3_StateBlow_ptmf;

/* turning to Fiona; when the animation ends, the blow (state `st`, run at once: `blow`) */
static inline void Debilitas3_TurnToFiona(Pursuer *p, const PTMF *st, void (*blow)(Pursuer *)) {
    if (AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        Pursuer_PlayAnimIf(p, 0x205, 0);
        p->c.moveSub = 0x1C;
        Actor_SetState(&p->c.a, st);
        blow(p);
        return;
    }
    Character_RootMoveMasked(&p->c);
    {
        Character *t = gCharPlayer != NULL ? gCharPlayer : p->target;
        f32 h = Actor_HeadingTo(&p->c.a, t->a.pos);

        Npc_TurnToward(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    }
}

/* state: turning to Fiona (see Debilitas3_TurnToFiona) */
/* 0x002CE380 */
void Debilitas3_StateTurnToFiona(Pursuer *p) {
    Debilitas3_TurnToFiona(p, &Debilitas3_StateBlow_ptmf, Debilitas3_StateBlow);
}

extern const PTMF Debilitas3_StateTurnToFiona_ptmf;

/* start of the turn to Fiona: finish the current walk, then animation 0x1304 in state
 * Debilitas3_StateTurnToFiona */
/* 0x002CE490 */
void Debilitas3_StartTurnToFiona(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    Pursuer_PlayAnimIf(p, 0x1304, 0);
    PU(p, 0x16F7, u8) = 1;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &Debilitas3_StateTurnToFiona_ptmf);
    Debilitas3_TurnToFiona(p, &Debilitas3_StateBlow_ptmf, Debilitas3_StateBlow);
}

extern const PTMF Debilitas3_StateStun_ptmf, Debilitas3_StateStun_ptmf2;

/* state: his blow, after the turn to Fiona - as Debilitas's Debilitas_StateBlow: the hit point is the
 * bone of the attack entry (+0x171C, +0x94); off the nav mesh it misses (flinch, state
 * Debilitas3_StateStun_ptmf), otherwise on reaching Fiona or Hewie it lands, then the flinch and state Debilitas3_StateStun_ptmf2 */
/* 0x002CE100 */
void Debilitas3_StateBlow(Pursuer *p) {
    u8 *e = PU(p, 0x171C, u8 *) + 0x90;
    f32 pos[4] __attribute__((aligned(16)));
    u32 hit;

    sceVu0CopyVector(pos, Skel_Bone(MOTION_AT(p, 0x810, u8 *), AT(e, 0x4, s32)) + 0xC);
    if (Actor_TriTo(&p->c.a, pos, p->c.a.navMask & ~0x40) == (u32)-1) {
        Pursuer_PlayAnimIf(p, 0x1004, (((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF) != 0);
        p->c.moveSub = 0;
        Actor_SetState(&p->c.a, &Debilitas3_StateStun_ptmf);
        return;
    }
    sceVu0CopyVector((f32 *)((u8 *)p + 0x1770), pos);
    hit = Npc_WhoReachableBits(p, AT(e, 0x4, s32), AT(e, 0xC, f32)) & 0xFF & ~PU(p, 0x1760, u8);
    hit = (hit | (Npc_WhoSeen(p) & 0xFF & ~PU(p, 0x1760, u8) & 0xFF)) & 0xFF;
    if (hit == 0) {
        Character_RootMoveMasked(&p->c);
        return;
    }
    {
        f32 roll = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
        s16 stun = roll < AT(e, 0x18, f32) ? 0x8000 : 0;

        Relation_Request(gProgress, *(u8 *)&p->c.a.slot, hit, AT(e, 0x10, u8), AT(e, 0x12, u16), stun, AT(e, 0x14, f32));
    }
    PU(p, 0x1764, s32) = 12;
    if ((((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF) == 0) {
        Pursuer_PlayAnimIf(p, 0x1004, 0);
    }
    p->c.moveSub = 0;
    Actor_SetState(&p->c.a, &Debilitas3_StateStun_ptmf2);
}

extern const f32 D_00414300[4];

/* vtable +0x1A8: the chase towards the target - as Debilitas's Debilitas_ChaseTarget: on the path he cuts
 * straight for the target once he's no more than 10 units further from it than the path's end */
/* 0x002CE6C0 */
void Debilitas3_ChaseTarget(Pursuer *p) {
    Character *t;

    if (PU(p, 0x1590, f32) < 0.0f && p->c.a.room == p->target->a.room) {
        VCALL(p, 0xB4, void (*)(Pursuer *, Character *))(p, p->target);
        if (p->c.a.room == p->target->a.room || !(Pursuer_TargetOutOfReach(p) & 0xFF)) {
            PU(p, 0x16EF, u8) = 1;
        }
        return;
    }
    if ((p->c.unk128 < p->c.unk124) != 1) {
        return;
    }
    t = p->target;
    if (Npc_TriBlocked(p, t->a.navTri) == 0) {
        u32 tri = p->target->a.navTri;

        if (Actor_TriTo(&p->c.a, p->target->a.pos, -1) == tri) {
            Npc_StepToward(p, p->target->a.pos);
            return;
        }
    } else {
        s32 n = p->c.unk124;
        f32 end[4] __attribute__((aligned(16)));
        f32 d[4] __attribute__((aligned(16)));
        u32 tri;
        f32 left;

        end[0] = D_00414300[0];
        end[1] = D_00414300[1];
        end[2] = D_00414300[2];
        end[3] = D_00414300[3];
        end[0] = AT(p, 0x124 + n * 0xC, f32);
        end[2] = AT(p, 0x128 + p->c.unk124 * 0xC, f32);
        tri = Actor_TriTo(&p->c.a, end, p->c.a.navMask);
        if (tri == AT(p, 0x120 + p->c.unk124 * 0xC, u32)) {
            VCALL(gNavMesh, 0x14, void (*)(void *, u32, f32 *))(gNavMesh, tri, end);
            sceVu0SubVector(d, p->target->a.pos, end);
            d[3] = 0.0f;
            left = __builtin_sqrtf(sceVu0InnerProduct(d, d));
            if (Actor_Distance(&p->c.a, p->target->a.pos) - left <= 10.0f) {
                Npc_StepToward(p, p->target->a.pos);
                return;
            }
        }
    }
    Npc_WalkPathStride(p, p->c.unk128);
}

/* vtable +0x30: his frame update - as Debilitas's Debilitas_Update (full think on screen, with a
 * growl every 90 idle frames; off screen the behaviour step and the off-screen move) */
/* 0x002CE910 */
void Debilitas3_Update(Pursuer *p) {
    PTMF *st = (PTMF *)((u8 *)p + 0x174C);

    Stalker_ThinkStart(p);
    if (Npc_InPlayedRoom(p) != 0) {
        Pursuer_DoorNear(p);
        Npc_ProbeAroundFiona(p);
        Pursuer_CryHeard(p);
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        PU(p, 0x17EC, u32)++;
        if (PU(p, 0x17EC, u32) >= 90) {
            PU(p, 0x17EC, u32) = 0;
            if (p->c.moveMode == 0 && PU(p, 0x1788, s32) != 0x1300 && PU(p, 0x1788, s32) != 0x1600) {
                Pursuer_Sound(p, 0x2A, 7, 0, 0, NULL);
            }
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

/* ---- the same shapes in other classes, generated from the functions they copy (2026-10-05) ---- */
extern const PTMF Kind27_StateGrab_ptmf;
extern const PTMF Kind27_StateLunge_ptmf;
extern const PTMF Kind27_StateStun_ptmf;
extern const PTMF Kind27_StateStun_ptmf2;
extern const f32 D_0042F1F0[4];

extern u8 D_0042E9D0[], D_0042EA00[], D_0042EA30[], D_0042EA70[], D_0042EAA0[], D_0042EAE0[], D_0042EB10[],
    D_0042EB40[], D_0042EB70[], D_0042EBB0[], D_0042EBC0[], D_0042EC00[], D_0042EC20[], D_0042EC50[],
    str_t_15[], D_0042ECC8[], D_0042ECD8[], D_0042EDC0[], D_0042EE10[], D_0042EE60[], D_0042EEB0[],
    D_0042EEE0[], D_0042EF10[], D_0042EF20[], D_0042EF60[], D_0042EF80[], D_0042EFC8[], D_0042EFE0[],
    D_0042F020[], D_0042F040[], D_0042F080[], str_t_16[], D_0042F0E8[], D_0042F0F8[], Kind27_vtable[];

/* the Kind27_vtable Debilitas's attack tables for each situation 0..16; the second set when gProgress+0x30 bit 0x8000 */
static u8 *const sAttackTablesD[2][17] = {
    { D_0042E9D0, D_0042EA30, D_0042EA00, D_0042EA70, D_0042EAA0, D_0042EAE0, D_0042EB10,
      D_0042EB40, D_0042EB70, D_0042EBB0, D_0042EBC0, D_0042EC00, D_0042EC20, D_0042EC50,
      D_0042ECC8, D_0042ECD8, str_t_15 },
    { D_0042EDC0, D_0042EE60, D_0042EE10, D_0042EEB0, D_0042EEE0, D_0042EF10, D_0042EF20,
      D_0042EF60, D_0042EF80, D_0042EFC8, D_0042EFE0, D_0042F020, D_0042F040, D_0042F080,
      D_0042F0E8, D_0042F0F8, str_t_16 },
};

/* vtable +0x130: the attack table for a situation (as Debilitas3_AttackTable) */
/* 0x0032F880 */
void Kind27_AttackTable(Pursuer *p, s8 situation) {
    s32 alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;

    PU(p, 0x1718, u8 *) = (u32)situation < 17 ? sAttackTablesD[alt][situation] : sAttackTablesD[alt][0];
}

/* (as Debilitas3_StateGrab)  state: the grab (see Debilitas3_Grab) */
/* 0x0032FB40 */
void Kind27_StateGrab(Pursuer *p) {
    Debilitas3_Grab(p);
}

/* (as Debilitas3_StartGrab)  start of the grab: finish the current walk, then animation 0xE06 in state Debilitas3_StateGrab */
/* 0x0032FC70 */
void Kind27_StartGrab(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    Pursuer_PlayAnimIf(p, 0xE06, 0);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &Kind27_StateGrab_ptmf);
    Debilitas3_Grab(p);
}

extern const PTMF Pursuer_AttackNextStep_ptmf8;

/* state: the lunge animation (as Debilitas3_StateLunge) */
/* 0x0032FEA0 */
void Kind27_StateLunge(Pursuer *p) {
    Debilitas3_LungeEnd(p, &Pursuer_AttackNextStep_ptmf8);
}

void Kind27_StateBlow(Pursuer *p);
extern const PTMF Kind27_StateBlow_ptmf, Kind27_StateTurnToFiona_ptmf;

/* state: turning to Fiona (as Debilitas3_StateTurnToFiona) */
/* 0x00330450 */
void Kind27_StateTurnToFiona(Pursuer *p) {
    Debilitas3_TurnToFiona(p, &Kind27_StateBlow_ptmf, Kind27_StateBlow);
}

/* start of the turn to Fiona (as Debilitas3_StartTurnToFiona) */
/* 0x00330560 */
void Kind27_StartTurnToFiona(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    Pursuer_PlayAnimIf(p, 0x1304, 0);
    PU(p, 0x16F7, u8) = 1;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &Kind27_StateTurnToFiona_ptmf);
    Debilitas3_TurnToFiona(p, &Kind27_StateBlow_ptmf, Kind27_StateBlow);
}

/* (as Debilitas3_StartLunge)  start of the lunge: finish the current walk, then animation 0x1306 in state Debilitas3_StateLunge */
/* 0x0032FF60 */
void Kind27_StartLunge(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    Pursuer_PlayAnimIf(p, 0x1306, 0);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &Kind27_StateLunge_ptmf);
    Debilitas3_LungeEnd(p, &Pursuer_AttackNextStep_ptmf8);
}

/* (as Debilitas3_StateStun)  a stun: the flinch (0x1004) unless already reeling, and frozen while it lasts - as Debilitas's
 * Debilitas_StateStun */
/* 0x00330120 */
void Kind27_StateStun(Pursuer *p) {
    if (PU(p, 0x1788, s32) != 0x1000 && p->c.unk14D0 <= 0) {
        Pursuer_PlayAnimIf(p, 0x1004, 1);
    }
    if (((AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0)) {
        PU(p, 0x1770, s32) = 0;
        PU(p, 0x1774, s32) = 0;
        PU(p, 0x1778, s32) = 0;
        PU(p, 0x16F7, u8) = 0;
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    } else if (p->c.unk14D0 <= 0) {
        Character_RootMoveMasked(&p->c);
    }
}

/* (as Debilitas3_StateBlow)  state: his blow, after the turn to Fiona - as Debilitas's Debilitas_StateBlow: the hit point is the
 * bone of the attack entry (+0x171C, +0x94); off the nav mesh it misses (flinch, state
 * Kind27_StateStun_ptmf), otherwise on reaching Fiona or Hewie it lands, then the flinch and state Kind27_StateStun_ptmf2 */
/* 0x003301D0 */
void Kind27_StateBlow(Pursuer *p) {
    u8 *e = PU(p, 0x171C, u8 *) + 0x90;
    f32 pos[4] __attribute__((aligned(16)));
    u32 hit;

    sceVu0CopyVector(pos, Skel_Bone(MOTION_AT(p, 0x810, u8 *), AT(e, 0x4, s32)) + 0xC);
    if (Actor_TriTo(&p->c.a, pos, p->c.a.navMask & ~0x40) == (u32)-1) {
        Pursuer_PlayAnimIf(p, 0x1004, (((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF) != 0);
        p->c.moveSub = 0;
        Actor_SetState(&p->c.a, &Kind27_StateStun_ptmf);
        return;
    }
    sceVu0CopyVector((f32 *)((u8 *)p + 0x1770), pos);
    hit = Npc_WhoReachableBits(p, AT(e, 0x4, s32), AT(e, 0xC, f32)) & 0xFF & ~PU(p, 0x1760, u8);
    hit = (hit | (Npc_WhoSeen(p) & 0xFF & ~PU(p, 0x1760, u8) & 0xFF)) & 0xFF;
    if (hit == 0) {
        Character_RootMoveMasked(&p->c);
        return;
    }
    {
        f32 roll = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
        s16 stun = roll < AT(e, 0x18, f32) ? 0x8000 : 0;

        Relation_Request(gProgress, *(u8 *)&p->c.a.slot, hit, AT(e, 0x10, u8), AT(e, 0x12, u16), stun, AT(e, 0x14, f32));
    }
    PU(p, 0x1764, s32) = 12;
    if ((((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF) == 0) {
        Pursuer_PlayAnimIf(p, 0x1004, 0);
    }
    p->c.moveSub = 0;
    Actor_SetState(&p->c.a, &Kind27_StateStun_ptmf2);
}

/* (as Debilitas3_ChaseTarget)  vtable +0x1A8: the chase towards the target - as Debilitas's Debilitas_ChaseTarget: on the path he cuts
 * straight for the target once he's no more than 10 units further from it than the path's end */
/* 0x00330790 */
void Kind27_ChaseTarget(Pursuer *p) {
    Character *t;

    if (PU(p, 0x1590, f32) < 0.0f && p->c.a.room == p->target->a.room) {
        VCALL(p, 0xB4, void (*)(Pursuer *, Character *))(p, p->target);
        if (p->c.a.room == p->target->a.room || !(Pursuer_TargetOutOfReach(p) & 0xFF)) {
            PU(p, 0x16EF, u8) = 1;
        }
        return;
    }
    if ((p->c.unk128 < p->c.unk124) != 1) {
        return;
    }
    t = p->target;
    if (Npc_TriBlocked(p, t->a.navTri) == 0) {
        u32 tri = p->target->a.navTri;

        if (Actor_TriTo(&p->c.a, p->target->a.pos, -1) == tri) {
            Npc_StepToward(p, p->target->a.pos);
            return;
        }
    } else {
        s32 n = p->c.unk124;
        f32 end[4] __attribute__((aligned(16)));
        f32 d[4] __attribute__((aligned(16)));
        u32 tri;
        f32 left;

        end[0] = D_0042F1F0[0];
        end[1] = D_0042F1F0[1];
        end[2] = D_0042F1F0[2];
        end[3] = D_0042F1F0[3];
        end[0] = AT(p, 0x124 + n * 0xC, f32);
        end[2] = AT(p, 0x128 + p->c.unk124 * 0xC, f32);
        tri = Actor_TriTo(&p->c.a, end, p->c.a.navMask);
        if (tri == AT(p, 0x120 + p->c.unk124 * 0xC, u32)) {
            VCALL(gNavMesh, 0x14, void (*)(void *, u32, f32 *))(gNavMesh, tri, end);
            sceVu0SubVector(d, p->target->a.pos, end);
            d[3] = 0.0f;
            left = __builtin_sqrtf(sceVu0InnerProduct(d, d));
            if (Actor_Distance(&p->c.a, p->target->a.pos) - left <= 10.0f) {
                Npc_StepToward(p, p->target->a.pos);
                return;
            }
        }
    }
    Npc_WalkPathStride(p, p->c.unk128);
}

/* (as Debilitas3_Update)  vtable +0x30: his frame update - as Debilitas's Debilitas_Update (full think on screen, with a
 * growl every 90 idle frames; off screen the behaviour step and the off-screen move) */
/* 0x003309E0 */
void Kind27_Update(Pursuer *p) {
    PTMF *st = (PTMF *)((u8 *)p + 0x174C);

    Stalker_ThinkStart(p);
    if (Npc_InPlayedRoom(p) != 0) {
        Pursuer_DoorNear(p);
        Npc_ProbeAroundFiona(p);
        Pursuer_CryHeard(p);
        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
        PU(p, 0x17EC, u32)++;
        if (PU(p, 0x17EC, u32) >= 90) {
            PU(p, 0x17EC, u32) = 0;
            if (p->c.moveMode == 0 && PU(p, 0x1788, s32) != 0x1300 && PU(p, 0x1788, s32) != 0x1600) {
                Pursuer_Sound(p, 0x2A, 7, 0, 0, NULL);
            }
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

/* (a pursuer class) its threat: 50 in action 0x1001, else the pursuers' Pursuer_PathNodeSound */
/* 0x00330D10 */
u32 Kind27_PathNodeSound(Pursuer *p) {
    if (PU(p, 0x175C, s32) == 0x1001) {
        return 0x32;
    }
    return Pursuer_PathNodeSound(p);
}

/* (as Debilitas3_HeadFor)  vtable +0xB4: head for character `c` (NULL: the target) - Debilitas's Debilitas_HeadFor without
 * the resting check */
/* 0x00330D50 */
void Kind27_HeadFor(Pursuer *p, Character *c) {
    s32 side;

    if (c == NULL) {
        c = p->target;
    }
    side = PU(p, 0x1598, s32);
    if (Npc_SameRoomOtherSide(p, c) != 0) {
        side = Npc_CharSideBehind(p, c);
    } else {
        s32 room = c->a.room;

        if (p->c.a.room == room || PU(p, 0x1594, s32) != room) {
            side = -1;
        }
        if (room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
            PU(p, 0x15A4, s32) = Npc_NearestWalkable(p, c->a.navTri, c->a.pos, (f32 *)((u8 *)p + 0x15B0));
        }
    }
    if (Character_Route(&p->c, c->a.room, side, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = c->a.room;
        PU(p, 0x1598, s32) = side;
    } else {
        Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
}

/* (as Debilitas3_HeadForFiona)  vtable +0xB0: head for Fiona - Debilitas's Debilitas_HeadForFiona without the resting check */
/* 0x00330E70 */
void Kind27_HeadForFiona(Pursuer *p) {
    Character *f = gCharPlayer;
    s32 side = PU(p, 0x1598, s32);

    if (Npc_SameRoomOtherSide(p, f) != 0) {
        side = Npc_CharSideBehind(p, f);
    } else {
        if (p->c.a.room == f->a.room || PU(p, 0x1594, s32) != f->a.room) {
            side = -1;
        }
        PU(p, 0x15A4, s32) = Npc_NearestWalkable(p, f->a.navTri, f->a.pos, (f32 *)((u8 *)p + 0x15B0));
    }
    if (Character_Route(&p->c, f->a.room, side, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = f->a.room;
        PU(p, 0x1598, s32) = side;
    } else {
        Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
}

/* (as Debilitas3_GoTo)  vtable +0xAC: head for triangle `tri` at `pos` in `room` (-1 the current one) - as Debilitas's
 * Debilitas_GoTo */
/* 0x00330F60 */
void Kind27_GoTo(Pursuer *p, u32 tri, const f32 *pos, s32 room) {
    void *nm;

    if (room == -1) {
        room = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    }
    if (room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        PU(p, 0x15A4, u32) = Npc_NearestWalkable(p, tri, pos, (f32 *)((u8 *)p + 0x15B0));
    }
    nm = gNavMesh;
    if (VCALL(nm, 0x10, s32 (*)(void *, u32, const f32 *))(nm, PU(p, 0x15A4, u32), (f32 *)((u8 *)p + 0x15B0)) != 3) {
        VCALL(nm, 0xC, void (*)(void *, u32, f32 *))(nm, PU(p, 0x15A4, u32), (f32 *)((u8 *)p + 0x15B0));
    }
    if (Character_Route(&p->c, room, -1, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = room;
        PU(p, 0x1598, s32) = -1;
    } else {
        Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
}

/* 0x003310A0 */
u32 Kind27_BlockFlags(void) {
    return 0x2C020068;
}

/* 0x003310B0 */
s32 Kind27_AttackAnimB(void) {
    return 0xE;
}

/* 0x003310C0 */
s32 Kind27_AttackAnimA(void) {
    return 0xD;
}

/* 0x003310D0 */
f32 Kind27_SpeedBase(void) {
    return 0x1.3333340000000p-1f /* 0.6 */;
}

/* 0x003310F0 */
f32 Kind27_SpeedTop(void) {
    return 0x1.6666660000000p+0f /* 1.4 */;
}

/* 0x00331110 */
f32 Kind27_AttackRange(void) {
    return 24.0f;
}

/* 0x00331120 */
f32 Kind27_ReachHewie(void) {
    return 16.0f;
}

/* 0x00331130 */
f32 Kind27_AttackAngle(void) {
    return 2e+01f;
}

/* 0x00331140 */
f32 Kind27_Dist2E8(void) {
    return 1e+01f;
}

/* 0x00331150 */
f32 Kind27_ReachFiona(void) {
    return 2e+01f;
}

/* 0x00331160 */
f32 Kind27_LookSwing(void) {
    return 0x1.eb851e0000000p-4f /* 0.12 */;
}

/* 0x00331180 */
f32 Kind27_LookFrames(void) {
    return 6e+01f;
}

/* 0x00331190 */
f32 Kind27_TurnRateFast(void) {
    return 0x1.1df46a0000000p-3f /* 0.13962634 */;
}

/* 0x003311B0 */
f32 Kind27_TurnRate(void) {
    return 0x1.aceea00000000p-5f /* 0.05235988 */;
}

/* 0x003311D0 */
void Kind27_SetTimer(u8 *self, s32 t) {
    S32(self, 0x1660) = t != 0 ? t : 900;
}

/* 0x003311F0 */
void Kind27_Timer10s(u8 *self) {
    S32(self, 0x1660) = 600;
}

/* 0x00331240 */
void *Kind27_ModelFiles(void) {
    return b5_prog_flag8000() ? D_0042E480 : D_0042E440;
}

extern u8 D_0042E720[], D_0042F110[], D_0042F160[], D_0042ED60[], str_Z_9[], D_0042ECF0[], D_0042ED40[];
extern u8 D_0042E940[], D_0042E980[], D_0042E5B0[], D_0042E710[], D_0047ADC0[];
extern const PTMF D_0042E540;

/* (as Debilitas3_Setup) the Kind27_vtable Debilitas's vtable +0xF4: its setup over the Pursuer's -
 * its tables and stats (165 hp and +0x16E8 18 when gProgress+0x30 bit 0x8000 is set, else 110
 * hp and 12; Hewie bite tolerance 30) */
/* 0x00331280 */
void Kind27_Setup(Pursuer *p) {
    Pursuer_Setup(p);
    if (AT(gProgress, 0x30, u32) & 0x8000) {
        p->c.hpMax = 165;
        PU(p, 0x171C, u8 *) = D_0042E720;
        PU(p, 0x1730, u8 *) = D_0042F110;
        PU(p, 0x1740, u8 *) = D_0042F160;
        PU(p, 0x173C, u8 *) = D_0042ED60;
        PU(p, 0x1748, u8 *) = str_Z_9;
        PU(p, 0x16DC, s32) = 30;
        PU(p, 0x16E8, f32) = 18.0f;
    } else {
        p->c.hpMax = 110;
        PU(p, 0x171C, u8 *) = D_0042E720;
        PU(p, 0x1730, u8 *) = D_0042ECF0;
        PU(p, 0x1740, u8 *) = D_0042ED40;
        PU(p, 0x173C, u8 *) = D_0042ED60;
        PU(p, 0x1748, u8 *) = str_Z_9;
        PU(p, 0x16DC, s32) = 30;
        PU(p, 0x16E8, f32) = 12.0f;
    }
    PU(p, 0x16D4, s32) = 300;
    PU(p, 0x16D8, s32) = 1800;
    PU(p, 0x16D0, s32) = 1350;
    PU(p, 0x16E0, s32) = 9000;
    PU(p, 0x16E4, s32) = 150;
    p->c.a.radius = 5.0f;
    p->c.a.height = 20.0f;
    p->c.hp = p->c.hpMax;
    p->c.hearThreshold = 0;
    PU(p, 0x16B4, u8) = 3;
    PU(p, 0x1714, const PTMF *) = &D_0042E540;
    PU(p, 0x1720, u8 *) = D_0042E940;
    PU(p, 0x1724, u8 *) = D_0042E980;
    PU(p, 0x16AC, u8 *) = D_0042E5B0;
    PU(p, 0x16B0, u8 *) = D_0042E710;
    PU(p, 0x1734, u8 *) = D_0047ADC0;
    PU(p, 0x17EC, s32) = 0;
    PU(p, 0x1694, f32) = 6.0f;
    PU(p, 0x169C, f32) = 1.5f;
    PU(p, 0x1698, f32) = 13.0f;
    PU(p, 0x16A0, s32) = 0;
}

/* 0x00331440 */
void Kind27_StateNone(void) {
}
