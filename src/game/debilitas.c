/* Debilitas: the Pursuer of the first chapters (kind 2, vtable 0x469D10; code 0x1276F0..0x12C1xx).
 * He overrides about thirty of the Pursuer's virtual functions: his attacks, his own way of
 * searching, and his reactions. The object is a plain Pursuer (0x1800 bytes). See pursuer.h.
 * Most of his code is shared with the second Debilitas class (debilitas2.c) and lives in
 * debilitas_body.inc; this file has the rest: his behaviour picker and chase, wandering, the
 * walk choice and his setup.
 *
 * (was debilitas3.c) The third Debilitas class (kind 7, vtable Debilitas3_vtable; code
 * 0x2CD7A0..0x2CF3xx): a Pursuer with Debilitas's model and its own behaviour, used where the
 * countdown runs (its reset gives +0x16DC 0x10 while progress +0x1FBEC1 is set, pursuer.c
 * Debilitas3_Activate). See debilitas.c.
 */
#include "common.h"
#include "pursuer.h"
#include "progress.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "navmesh.h"
#include "actor.h"
#include "ptmf.h"

/* his functions defined further down */

#define DEBILITAS_GIVE_UP_HITS 20
#include "debilitas_body.inc"
#include "debilitas.h"
#include "hewie.h"
#include "model.h"
#include "item.h"
#include "game.h"
#include "heap.h"
#include "items.h"
#include "renderer.h"
#include "sound.h"
#include "vecmath.h"
#include "msl.h"
#include "input.h"
#include "memcard.h"
#include "event.h"
#include "scene_game.h"
#include "lights.h"
#include "effectmgr.h"
#include "fiona.h"
#include "libc.h"
#include "effectmgr.h"   /* HitEffect_Spawn */
#include "effects.h"
#include "placed.h"
#include "daniella.h"
#include "loader.h"
#include "pad.h"
#include "scene.h"
#include "scene_boot.h"
#include "scene_title.h"
#include "system.h"
#include "text.h"
#include "sce/iop.h"
#include "charaction.h"
#include "gl2d.h"
#include "debilitas2.h"
#include "lorenzo.h"
#include "char_load.h"
#ifdef HG_NATIVE
#include <stdio.h>
#include <stdlib.h>
#include "glr.h"
#endif

extern u8 D_003AF250[];
extern u8 D_003AF1D0[];
extern u8 D_003AF210[];
#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

extern u8 D_004137B0[], D_00414220[], D_00414270[], D_00413E10[], D_00413E60[], D_00413E80[], str_Z_5[];
extern u8 D_004139D0[], D_00413A10[], D_00413640[], D_004137A0[], D_0047AC08[];
extern const PTMF D_004135D0;
extern u8 D_0042E440[];
extern u8 D_0042E480[];
extern const PTMF Pursuer_AttackNextStep_ptmf5;
extern const PTMF Debilitas3_StateLunge_ptmf;
extern const PTMF Debilitas3_StateGrab_ptmf;
void Debilitas3_StateBlow(Pursuer *p);
extern const PTMF Debilitas3_StateBlow_ptmf;
extern const PTMF Debilitas3_StateTurnToFiona_ptmf;
extern const PTMF Debilitas3_StateStun_ptmf, Debilitas3_StateStun_ptmf2;
extern const f32 D_00414300[4];
/* ---- the same shapes in other classes, generated from the functions they copy (2026-10-05) ---- */
extern const PTMF Kind27_StateGrab_ptmf;
extern const PTMF Kind27_StateLunge_ptmf;
extern const PTMF Kind27_StateStun_ptmf;
extern const PTMF Kind27_StateStun_ptmf2;
extern const f32 D_0042F1F0[4];
extern u8 D_0042E9D0[], D_0042EA00[], D_0042EA30[], D_0042EA70[], D_0042EAA0[], D_0042EAE0[], D_0042EB10[], D_0042EB40[], D_0042EB70[], D_0042EBB0[], D_0042EBC0[], D_0042EC00[], D_0042EC20[], D_0042EC50[], str_t_15[], D_0042ECC8[], D_0042ECD8[], D_0042EDC0[], D_0042EE10[], D_0042EE60[], D_0042EEB0[], D_0042EEE0[], D_0042EF10[], D_0042EF20[], D_0042EF60[], D_0042EF80[], D_0042EFC8[], D_0042EFE0[], D_0042F020[], D_0042F040[], D_0042F080[], str_t_16[], D_0042F0E8[], D_0042F0F8[];
extern const PTMF Pursuer_AttackNextStep_ptmf8;
void Kind27_StateBlow(Pursuer *p);
extern const PTMF Kind27_StateBlow_ptmf, Kind27_StateTurnToFiona_ptmf;
extern u8 D_0042E720[], D_0042F110[], D_0042F160[], D_0042ED60[], str_Z_9[], D_0042ECF0[], D_0042ED40[];
extern u8 D_0042E940[], D_0042E980[], D_0042E5B0[], D_0042E710[], D_0047ADC0[];
extern const PTMF D_0042E540;
#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

/* Field access by byte offset into objects whose layout is not yet known. */
#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

#define U32(p, off) (*(u32 *)((u8 *)(p) + (off)))

extern u8 D_00413550[];
extern void *Debilitas3_vtable[];
extern void *BonePoint_vtable[];
void *HangPoint_ctor(u8 *p);
void *IK2_ctor(u8 *p);
extern void *DebilitasModel_vtable[];
extern void *HangPoint_ctor(u8 *p);
extern void *IK2_ctor(u8 *p);

extern void *Kind27_vtable[];
/* writes {x, 0, z} */
#define B5_SET3(out, x, z) ((out)[0] = (x), (out)[1] = 0.0f, (out)[2] = (z))

Character *Kind27_dtor(Character *c, s32 flags);
void Kind27_DoorOffset(void *self, s32 id, f32 *out);
void Kind27_ActionOffsets(void *self, s32 id, f32 *out);
void Kind27_ExitDone(u8 *self);
void Kind27_FollowPathExit(void);
void Kind27_Arrived(void);
void Kind27_WalkToExit(void);
void Kind27_OnToNextExit(void);
s32 Kind27_PickDestination(void);
static inline __attribute__((always_inline)) Character *creature_dtor(Character *c, s32 flags, void **vt);

extern u8 pstr_O_DB2_DB2_200_PCK_2[], pstr_O_DB2_DB2_200_PCK[], D_00413510[], D_004134D0[];
extern u8 pstr_O_DB2_DB2_200_PCK_3[];
extern u8 pstr_O_DB2_DB2_200_PCK_4[];
static inline s32 b5_prog_flag8000(void);

extern u8 D_003D89A0[];
void *DebilitasModel_dtor(u8 *m, s32 flags);
void DebilitasModel_Frame(u8 *m);
void DebilitasModel_SecondaryMotion(u8 *m);
s32 DebilitasModel_Part0(u8 *m);
s32 DebilitasModel_Part1(u8 *m);
s32 DebilitasModel_Part2(u8 *m);
s32 DebilitasModel_Part3(u8 *m);
void DebilitasModel_Springs(u8 *m);
void DebilitasModel_Loaded(u8 *m);
void DebilitasModel_Vt3C(u8 *m);

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

void Debilitas3_Activate(Pursuer *p);
void *Debilitas3_ModelFiles(void);
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

Character *Debilitas3_dtor(Character *c, s32 flags);
void *Debilitas3_MotionFiles(void);
void Debilitas3_DoorOffset(void *self, s32 id, u32 *out);
void Debilitas3_ActionOffsets(void *self, s32 id, u32 *out);
void Debilitas3_ExitDone(u8 *p);
void Debilitas3_FollowPathExit(void);
void Debilitas3_Arrived(void);
void Debilitas3_WalkToExit(void);
void Debilitas3_OnToNextExit(void);
s32 Debilitas3_PickDestination(void);

/* gProgress+0x30 bit 0x8000 selects between two data sets (difficulty/mode flag?) */
static inline s32 b5_prog_flag8000(void) {
    return U32(gProgress, 0x30) & 0x8000;
}

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

/* the Kind27_vtable Debilitas's attack tables for each situation 0..16; the second set when gProgress+0x30 bit 0x8000 */
static u8 *const sAttackTablesD[2][17] = {
    { D_0042E9D0, D_0042EA30, D_0042EA00, D_0042EA70, D_0042EAA0, D_0042EAE0, D_0042EB10,
      D_0042EB40, D_0042EB70, D_0042EBB0, D_0042EBC0, D_0042EC00, D_0042EC20, D_0042EC50,
      D_0042ECC8, D_0042ECD8, str_t_15 },
    { D_0042EDC0, D_0042EE60, D_0042EE10, D_0042EEB0, D_0042EEE0, D_0042EF10, D_0042EF20,
      D_0042EF60, D_0042EF80, D_0042EFC8, D_0042EFE0, D_0042F020, D_0042F040, D_0042F080,
      D_0042F0E8, D_0042F0F8, str_t_16 },
};

void Debilitas3_Setup(Pursuer *p);
void Debilitas3_StateNone(void);
void Debilitas3_HeadFor(Pursuer *p, Character *c);
void Debilitas3_HeadForFiona(Pursuer *p);
void Debilitas3_GoTo(Pursuer *p, u32 tri, const f32 *pos, s32 room);
u32 Debilitas3_BlockFlags(void);
s32 Debilitas3_AttackAnimB(void);
s32 Debilitas3_AttackAnimA(void);
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
void Debilitas3_SetTimer(u8 *p, s32 v);
void Debilitas3_Timer10s(u8 *p);
void Debilitas3_StateStun(Pursuer *p);
void Debilitas3_StateLunge(Pursuer *p);
void Debilitas3_StartLunge(Pursuer *p);
void Debilitas3_AttackTable(Pursuer *p, s8 situation);
void Debilitas3_DoorAnim(Pursuer *p);
void Debilitas3_Stairs(Pursuer *p);
void Debilitas3_StateGrab(Pursuer *p);
void Debilitas3_StartGrab(Pursuer *p);
void Debilitas3_StateTurnToFiona(Pursuer *p);
void Debilitas3_StartTurnToFiona(Pursuer *p);
void Debilitas3_StateBlow(Pursuer *p);
void Debilitas3_ChaseTarget(Pursuer *p);
void Debilitas3_Update(Pursuer *p);
void Kind27_AttackTable(Pursuer *p, s8 situation);
void Kind27_StateGrab(Pursuer *p);
void Kind27_StartGrab(Pursuer *p);
void Kind27_StateLunge(Pursuer *p);
void Kind27_StateTurnToFiona(Pursuer *p);
void Kind27_StartTurnToFiona(Pursuer *p);
void Kind27_StartLunge(Pursuer *p);
void Kind27_StateStun(Pursuer *p);
void Kind27_StateBlow(Pursuer *p);
void Kind27_ChaseTarget(Pursuer *p);
void Kind27_Update(Pursuer *p);
u32 Kind27_PathNodeSound(Pursuer *p);
void Kind27_HeadFor(Pursuer *p, Character *c);
void Kind27_HeadForFiona(Pursuer *p);
void Kind27_GoTo(Pursuer *p, u32 tri, const f32 *pos, s32 room);
u32 Kind27_BlockFlags(void);
s32 Kind27_AttackAnimB(void);
s32 Kind27_AttackAnimA(void);
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
void Kind27_Setup(Pursuer *p);
void Kind27_StateNone(void);

/* 0x001278C0 */
void *Debilitas_MotionFiles(void) {
    return D_003AF250;
}

/* 0x001278D0 */
void Debilitas_DoorOffset(void *p, s32 kind, f32 *out) {
    f32 z;

    switch (kind) {
    case 0:
        z = -0x1.8d89380000000p+2f /* 6.2115 */;
        break;
    case 1:
        z = 0x1.4ccccc0000000p+3f /* 10.4 */;
        break;
    case 2:
        z = 0x1.9276c80000000p+2f /* 6.2885 */;
        break;
    case 3:
        z = -0x1.dc2f840000000p+2f /* 7.4404 */;
        break;
    default:
        return;
    }
    out[0] = 0.0f;
    FLD(out, 4, s32) = 0;
    out[2] = z;
}

/* 0x00127970 */
void Debilitas_ActionOffsets(void *p, s32 kind, f32 *out) {
    f32 x, z;

    switch (kind) {
    case 10:
    case 11:
        x = 0x1.e2f8380000000p-1f /* 0.9433 */;
        z = 0x1.6807600000000p+3f /* 11.2509 */;
        break;
    case 12:
    case 13:
        x = 0x1.1656040000000p+1f /* 2.1745 */;
        z = 0x1.e1573e0000000p+3f /* 15.0419 */;
        break;
    case 14:
        x = -0x1.9c98600000000p+0f /* 1.6117 */;
        z = -0x1.15e00e0000000p+2f /* 4.3418 */;
        break;
    case 15:
        x = 0x1.2a30560000000p-6f /* 0.0182 */;
        z = -0x1.00346e0000000p+0f /* 1.0008 */;
        break;
    default:
        return;
    }
    out[0] = x;
    FLD(out, 4, s32) = 0;
    out[2] = z;
}

/* 0x00127A20 */
void *Debilitas_RoomSpots(void) {
    return kDebilitasRoomSpots;
}

/* 0x00127A30 */
void Debilitas_ExitDone(void *p) {
    FLD(p, 0x16EE, u8) = 1;
}

/* vtable +0xE8 */
/* 0x00128080 */
s32 RoomBase_Table3C(Pursuer *p) {
    return 0;
}

/* count the stun +0x14D0 down; at 0 the motion stops being frozen */
/* 0x00129AF0 */
void Debilitas_StunDown(Pursuer *p) {
    if (p->c.unk14D0 > 0) {
        p->c.unk14D0--;
        if (p->c.unk14D0 <= 0) {
            Motion_Unfreeze(p->c.motion);
        }
    }
}

/* state: walking his path while looking out (+0x1624 counts down while the walk goes on) */
/* 0x00128FC0 */
void Debilitas_StateLookWalk(Pursuer *p) {
    s32 arrived = 0;

    Pursuer_RaiseThreat(p, 0xFF);
    if (PU(p, 0x1590, f32) < 0.0f) {
        PU(p, 0x16EF, u8) = 1;
    }
    if ((p->c.unk128 < p->c.unk124) == 1) {
        arrived = Npc_WalkPathStride(p, p->c.unk128) & 0xFF;
    } else if (PU(p, 0x1590, f32) == 0.0f) {
        arrived = 1;
    } else {
        PU(p, 0x16EF, u8) = 1;
    }
    if (arrived != 1 && PU(p, 0x1624, s32) > 0) {
        PU(p, 0x1624, s32)--;
        return;
    }
    PURSUER_STEP_DONE(p) = 1;
}

extern const PTMF Debilitas_Behaviour_ptmf;

/* vtable +0x264: the next behaviour; his own (Debilitas_Behaviour) unless +0x16B4 is set or at threat
   level 5, then the Pursuer's */
/* 0x0012B990 */
void Debilitas_ChaseDecision(Pursuer *p) {
    s32 next;

    if (PU(p, 0x16B4, u8) != 0 || AT(gProgress, 0x7B8, u8) == 5) {
        Pursuer_ChaseDecision(p);
        return;
    }
    next = PU(p, 0x1758, s32);
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &Debilitas_Behaviour_ptmf);
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
    Debilitas_Behaviour(p);
}

/* 0x0012BE50 */
s32 Debilitas_BlockFlags(void) {
    return 0x2C020068;
}

/* 0x0012BE80 */
f32 Debilitas_SpeedBase(void) {
    return 0x1.3333340000000p-1f /* 0.6 */;
}

/* 0x0012BEA0 */
f32 Debilitas_SpeedTop(void) {
    return 0x1.6666660000000p+0f /* 1.4 */;
}

/* 0x0012BEC0 */
f32 Debilitas_AttackRange(void) {
    return 24.0f;
}

/* 0x0012BED0 */
f32 Debilitas_ReachHewie(void) {
    return 16.0f;
}

/* 0x0012BEE0 */
f32 Debilitas_AttackAngle(void) {
    return 20.0f;
}

/* 0x0012BEF0 */
f32 Debilitas_Dist2E8(void) {
    return 10.0f;
}

/* 0x0012BF00 */
f32 Debilitas_ReachFiona(void) {
    return 20.0f;
}

/* 0x0012BF10 */
f32 Debilitas_LookSwing(void) {
    return 0x1.eb851e0000000p-4f /* 0.12 */;
}

/* 0x0012BF30 */
f32 Debilitas_LookFrames(void) {
    return 60.0f;
}

/* 0x0012BF40 */
f32 Debilitas_TurnRateFast(void) {
    return 0x1.1df46a0000000p-3f /* 0.13962634 */; /* 8 degrees in radians */
}

/* 0x0012BF60 */
f32 Debilitas_TurnRate(void) {
    return 0x1.aceea00000000p-5f /* 0.052359879 */; /* 3 degrees in radians */
}

/* 0x0012BF80 */
void Debilitas_SetTimer(void *p, s32 t) {
    FLD(p, 0x1660, s32) = t != 0 ? t : 900;
}

/* 0x0012BFA0 */
void Debilitas_Timer10s(void *p) {
    FLD(p, 0x1660, s32) = 600;
}

/* 0x0012BFF0 */
void *Debilitas_ModelFiles(void) {
    if (FLD(gProgress, 0x30, u32) & 0x8000) {
        return D_003AF210;
    }
    return D_003AF1D0;
}

extern const PTMF Debilitas_StateLookWalk_ptmf;

/* state: start the walk of Debilitas_StateLookWalk (for 60 frames) once the current animation is over */
/* 0x00129090 */
void Debilitas_StateStartWalk(Pursuer *p) {
    if (Pursuer_WalkOn(p)) {
        return;
    }
    Pursuer_PlayAnimIf(p, VCALL(p, 0x328, s32 (*)(Pursuer *))(p), 0);
    PU(p, 0x1624, s32) = 60;
    Actor_SetState(&p->c.a, &Debilitas_StateLookWalk_ptmf);
    Debilitas_StateLookWalk(p);
}

/* vtable +0x27C: the Pursuer's frame update (Pursuer_BehaviourRun), with his walk: in the idle group,
   back to the idle while +0x16B4 is set, else the slow walk (vtable +0x324) when chasing within
   +0x17E8 of Fiona, the normal one (vtable +0x328) otherwise */
/* 0x0012B860 */
void Debilitas_BehaviourRun(Pursuer *p) {
    Pursuer_BehaviourRun(p);
    if (PU(p, 0x175C, s32) != 4 || (((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF) == 1) {
        return;
    }
    if (PU(p, 0x16B4, u8) == 1) {
        if (PU(p, 0x1788, s32) != 0x201) {
            Pursuer_PlayAnimIf(p, VCALL(p, 0x328, s32 (*)(Pursuer *))(p), 0);
        }
    } else if (PU(p, 0x16C8, u8) == 0 && PU(p, 0x1588, f32) < PU(p, 0x17E8, f32)) {
        if (PU(p, 0x1788, s32) != 0x200) {
            Pursuer_PlayAnimIf(p, VCALL(p, 0x324, s32 (*)(Pursuer *))(p), 0);
        }
    } else if (PU(p, 0x1788, s32) != 0x201) {
        Pursuer_PlayAnimIf(p, VCALL(p, 0x328, s32 (*)(Pursuer *))(p), 0);
    }
}

/* vtable +0x128: the walk for his mode: the slow walk (vtable +0x324) when chasing with the path
   short enough (+0x17E8) and Fiona not hiding (move mode 3), or searching in sub-states 0/2;
   the normal walk (vtable +0x328) otherwise */
/* 0x00128210 */
void Debilitas_StandAnim(Pursuer *p) {
    s32 slow;

    switch (PU(p, 0x16C8, u8)) {
    case 0:
        slow = PU(p, 0x1590, f32) <= PU(p, 0x17E8, f32) && !(PU(p, 0x1588, f32) < 0.0f)
            && gCharPlayer->moveMode != 3;
        break;
    case 1:
    case 2:
    case 4:
        slow = 0;
        break;
    case 3:
        slow = PU(p, 0x16C9, u8) == 0 || PU(p, 0x16C9, u8) == 2;
        break;
    default:
        return;
    }
    if (slow) {
        Pursuer_PlayAnimIf(p, VCALL(p, 0x324, s32 (*)(Pursuer *))(p), 0);
    } else {
        Pursuer_PlayAnimIf(p, VCALL(p, 0x328, s32 (*)(Pursuer *))(p), 0);
    }
}

extern PTMF D_003AF2D0;
extern u8 D_003AF340[], D_003AF4A0[], D_003AF4D0[], D_003AF6F0[], D_003AF730[], D_003AFA90[],
    D_003AFAF0[], D_003AFB10[], D_003AFB58[], str_Z_2[], D_0047A900[];

/* vtable +0xF4: his setup over the Pursuer's (Pursuer_Setup): his tables, and his stats, which
   are different when gProgress+0x30 bit 0x8000 is set */
/* 0x0012C030 */
void Debilitas_Setup(Pursuer *p) {
    s32 alt;

    Pursuer_Setup(p);
    alt = (AT(gProgress, 0x30, u32) & 0x8000) != 0;
    p->c.hpMax = alt ? 110 : 70;
    PU(p, 0x171C, u8 *) = D_003AF4D0;
    PU(p, 0x1730, u8 *) = D_003AFA90;
    PU(p, 0x1740, u8 *) = D_003AFAF0;
    PU(p, 0x173C, u8 *) = D_003AFB10;
    PU(p, 0x1748, u8 *) = D_003AFB58;
    PU(p, 0x17F0, u8 *) = str_Z_2;
    PU(p, 0x16DC, s32) = 20;              /* Hewie bite tolerance */
    PU(p, 0x16E8, f32) = 10.0f;
    PU(p, 0x16D4, s32) = alt ? 540 : 300; /* frames */
    PU(p, 0x16D8, s32) = 1800;
    PU(p, 0x16D0, s32) = alt ? 1350 : 1800;
    PU(p, 0x16E0, s32) = 4500;
    PU(p, 0x16E4, s32) = 90;
    PU(p, 0x17E4, f32) = alt ? 40.0f : 50.0f;
    PU(p, 0x17E8, f32) = 80.0f;           /* slow walk when the path to Fiona is shorter */
    p->c.a.radius = 5.0f;
    p->c.a.height = 20.0f;
    p->c.hp = p->c.hpMax;
    p->c.hearThreshold = 0;
    PU(p, 0x16B4, u8) = 0;
    PU(p, 0x1714, PTMF *) = &D_003AF2D0;
    PU(p, 0x1720, u8 *) = D_003AF6F0;
    PU(p, 0x1724, u8 *) = D_003AF730;
    PU(p, 0x16AC, u8 *) = D_003AF340;
    PU(p, 0x16B0, u8 *) = D_003AF4A0;
    PU(p, 0x1734, u8 *) = D_0047A900;
    PU(p, 0x17EC, s32) = 0;
    PU(p, 0x1694, f32) = 8.0f;
    PU(p, 0x169C, f32) = 1.5f;
    PU(p, 0x1698, f32) = 12.0f;
    PU(p, 0x16A0, f32) = 1.5f;
}

/* 0x00173510 */
void *Kind27_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x1B, arg, Kind27_vtable);
}

/* 0x00173560 */
void *Debilitas3_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x7, arg, Debilitas3_vtable);
}

/* +0x8: destructor */
/* 0x002108F0 */
void *DebilitasModel_dtor(u8 *m, s32 flags) {
    if (m != NULL) {
        AT(m, 0x0, void **) = DebilitasModel_vtable;
        __destroy_arr(m + 0x9A0, HangPoint_dtor, 0x50, 4);
        HumanModel_Destroy(m, flags);
    }
    return m;
}

/* +0xB4: his secondary-motion table */
/* 0x00210A20 */
void DebilitasModel_SecondaryMotion(u8 *m) {
    AT(m, 0x874, u8 *) = D_003D89A0;
}

/* +0x84 .. +0x90: his mesh parts */
/* 0x00210A30 */
s32 DebilitasModel_Part0(u8 *m) {
    return 3;
}

/* 0x00210A40 */
s32 DebilitasModel_Part1(u8 *m) {
    return 7;
}

/* 0x00210A50 */
s32 DebilitasModel_Part2(u8 *m) {
    return 0x12;
}

/* 0x00210A60 */
s32 DebilitasModel_Part3(u8 *m) {
    return 0x1C;
}

/* his four hanging points (+0x9A0, 0x50 each, bones 10..13, the last two hanging from the first)
   on the spring system +0xAE0, and its two collision spheres on bone 2 */
/* 0x00210A70 */
void DebilitasModel_Springs(u8 *m) {
    s32 i;

    SpringSet_Clear(m + 0xAE0);
    for (i = 0; i < 4; i++) {
        u8 *node = m + 0x9A0 + i * 0x50;

        if (AT(m, 0xB10, u8 *) != NULL && AT(m, 0xB14, u8 *) != NULL) {
            AT(AT(m, 0xB14, u8 *), 0x28, u8 *) = node;
            AT(node, 0x28, u8 *) = NULL;
            AT(node, 0x2C, u8 *) = AT(m, 0xB14, u8 *);
            AT(m, 0xB14, u8 *) = node;
        } else {
            AT(m, 0xB14, u8 *) = node;
            AT(m, 0xB10, u8 *) = node;
            AT(node, 0x2C, u8 *) = NULL;
            AT(node, 0x28, u8 *) = NULL;
        }
    }
    for (i = 0; i < 2; i++) {
        u8 *col = m + 0xB20 + i * 0x40;

        AT(col, 0x2C, u8 *) = NULL;
        if (AT(m, 0xAF8, u8 *) == NULL) {
            AT(m, 0xAF8, u8 *) = col;
        } else {
            u8 *c = AT(m, 0xAF8, u8 *);

            while (AT(c, 0x2C, u8 *) != NULL) {
                c = AT(c, 0x2C, u8 *);
            }
            AT(c, 0x2C, u8 *) = col;
        }
    }
    AT(m, 0xAE0, f32) = 0.0f;
    AT(m, 0xAE4, u32) = 0x3ECCCCCD;   /* 0.4f (ee-gcc rounds the literal) */
    AT(m, 0xAE8, f32) = 0.0f;
    AT(m, 0xAF0, u32) = 0x3F7D70A4;   /* 0.99f */
    AT(m, 0xAF4, u8 *) = m;
    AT(m, 0xB00, u8) = 0;
    AT(m, 0xAFC, s32) = 0;
    AT(m, 0x9E0, f32) = 1.25f;
    AT(m, 0x9C4, s32) = 10;
    AT(m, 0x9C0, u8) = 1;
    AT(m, 0xA30, f32) = 1.25f;
    AT(m, 0xA14, s32) = 11;
    AT(m, 0xA10, u8) = 0;
    AT(m, 0xA80, f32) = 1.25f;
    AT(m, 0xA64, s32) = 12;
    AT(m, 0xA60, u8) = 0;
    AT(m, 0xAD0, f32) = 1.25f;
    AT(m, 0xAB4, s32) = 13;
    AT(m, 0xAB0, u8) = 0;
    AT(m, 0xA6C, u8 *) = m + 0x9A0;
    AT(m, 0xABC, u8 *) = m + 0x9A0;
    Sphere_Set(m + 0xB20, 2, 0.0f, 0.0f, 0.0f, 0x1.ccccccp+0f);   /* 1.8 */
    Sphere_Set(m + 0xB60, 2, 1.0f, 0.0f, 0.0f, 0x1.ccccccp+0f);
    AT(m, 0x850, u8) = 1;
}

/* +0x3C: the springs a frame: one step, or 30 to settle after a reset (+0x850) */
/* 0x00210C50 */
void DebilitasModel_Vt3C(u8 *m) {
    s32 n = AT(m, 0x850, u8) != 0 ? 30 : 1;
    s32 i;

    SpringSet_Begin(m + 0xAE0);
    for (i = 0; i < n; i++) {
        SpringSet_Step(m + 0xAE0);
    }
    SpringSet_Finish(m + 0xAE0);
    AT(m, 0x850, u8) = 0;
}

/* +0x10 */
/* 0x00210CD0 */
void DebilitasModel_Frame(u8 *m) {
    HumanModel_Frame(m);
}

/* +0xC: once loaded: the base setup, the part roles, the springs, per-part draw settings */
/* 0x00210CE0 */
void DebilitasModel_Loaded(u8 *m) {
    HumanModel_Loaded(m);
    AT(m, 0x890, s32) = 2;
    AT(m, 0x894, s32) = 3;
    AT(m, 0x898, s32) = 4;
    AT(m, 0x89C, s32) = 5;
    AT(m, 0x8B8, s32) = 0x14;
    AT(m, 0x8A0, s32) = 6;
    AT(m, 0x8A4, s32) = 7;
    AT(m, 0x8A8, s32) = 8;
    AT(m, 0x8AC, s32) = 9;
    AT(m, 0x8BC, s32) = 0x1E;
    AT(m, 0x8B0, s32) = 0x16;
    AT(m, 0x8B4, s32) = 0xF;
    AT(m, 0x860, f32) = 0.0f;
    AT(m, 0x864, f32) = 16.0f;
    AT(m, 0x868, f32) = 0.0f;
    AT(m, 0x854, s32) = 0;
    AT(m, 0x858, s32) = 0;
    DebilitasModel_Springs(m);
    {
        static const u8 sLoose[] = {0x9C, 0x9E, 0xA0, 0xA2, 0xC2, 0xC4, 0xC6};
        static const u8 sStiff[] = {0xB4, 0xBC, 0xBE, 0xC0};
        u32 i;

        for (i = 0; i < sizeof(sLoose); i++) {
            AT(m, sLoose[i], u8) = 4;
            AT(m, sLoose[i] + 1, u8) = 0x40;
        }
        for (i = 0; i < sizeof(sStiff); i++) {
            AT(m, sStiff[i], u8) = 4;
            AT(m, sStiff[i] + 1, u8) = 0x80;
        }
    }
}

/* 0x002CD4E0 */
Character *Debilitas3_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Debilitas3_vtable); }

/* 0x002CD5F0 */
void *Debilitas3_MotionFiles(void) { return D_00413550; }

/* 0x002CD600 */
void Debilitas3_DoorOffset(void *self, s32 id, u32 *out) {
    u32 z;

    switch (id) {
    case 1: z = 0x41266666; break;
    case 3: z = 0xC0EE17C2; break;
    case 0: z = 0xC0C6C49C; break;
    case 2: z = 0x40C93B64; break;
    default: return;
    }
    out[0] = 0;
    out[1] = 0;
    out[2] = z;
}

/* 0x002CD6A0 */
void Debilitas3_ActionOffsets(void *self, s32 id, u32 *out) {
    u32 x, z;

    switch (id) {
    case 10: case 11: x = 0x3F717C1C; z = 0x413403B0; break;
    case 12: case 13: x = 0x400B2B02; z = 0x4170AB9F; break;
    case 14: x = 0xBFCE4C30; z = 0xC08AF007; break;
    case 15: x = 0x3C95182B; z = 0xBF801A37; break;
    default: return;
    }
    out[0] = x;
    out[1] = 0;
    out[2] = z;
}

/* 0x002CD750 */
void Debilitas3_ExitDone(u8 *p) { p[0x16EE] = 1; }

/* 0x002CD760 */
void Debilitas3_FollowPathExit(void) {
}

/* 0x002CD770 */
void Debilitas3_Arrived(void) {
}

/* 0x002CD780 */
void Debilitas3_WalkToExit(void) {
}

/* 0x002CD790 */
void Debilitas3_OnToNextExit(void) {
}

/* 0x002CD7A0 */
s32 Debilitas3_PickDestination(void) {
    return 0;
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

/* state: the grab (see Debilitas3_Grab) */
/* 0x002CDA70 */
void Debilitas3_StateGrab(Pursuer *p) {
    Debilitas3_Grab(p);
}

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

/* state: the lunge animation (see Debilitas3_LungeEnd) */
/* 0x002CDDD0 */
void Debilitas3_StateLunge(Pursuer *p) {
    Debilitas3_LungeEnd(p, &Pursuer_AttackNextStep_ptmf5);
}

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

/* state: turning to Fiona (see Debilitas3_TurnToFiona) */
/* 0x002CE380 */
void Debilitas3_StateTurnToFiona(Pursuer *p) {
    Debilitas3_TurnToFiona(p, &Debilitas3_StateBlow_ptmf, Debilitas3_StateBlow);
}

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

/* vtable +0x228 / +0x224: the Pursuer's */
/* 0x002CE6A0 */
void Debilitas3_DoorAnim(Pursuer *p) {
    Pursuer_DoorAnim(p);
}

/* 0x002CE6B0 */
void Debilitas3_Stairs(Pursuer *p) {
    Pursuer_Stairs(p);
}

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

/* the Debilitas3_vtable stalker's +0x5C reset: the base one, and its +0x16DC 0x10 while the countdown
 * runs (progress +0x1FBEC1) */
/* 0x002CF0F0 */
void Debilitas3_Activate(Pursuer *p) {
    Pursuer_Activate(p);
    if (AT(gProgress, 0x1FBEC1, u8) != 0) {
        AT(p, 0x16DC, s32) = 0x10;
    }
}

/* 0x002CF140 */
u8 *Debilitas3_ModelFileTable(Pursuer *p) {
    return (F(gProgress, 0x30, u32) & 0x8000) ? pstr_O_DB2_DB2_200_PCK_2 : pstr_O_DB2_DB2_200_PCK;
}

/* 0x002CF180 */
void *Debilitas3_ModelFiles(void) {
    return (F(gProgress, 0x30, u32) & 0x8000) ? D_00413510 : D_004134D0;
}

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

/* 0x0032F5B0 */
Character *Kind27_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind27_vtable); }

/* 0x0032F6D0 */
void Kind27_DoorOffset(void *self, s32 id, f32 *out) {
    switch (id) {
    case 1:
        B5_SET3(out, 0.0f, 0x1.4ccccc0000000p+3f /* 10.4 */);
        break;
    case 3:
        B5_SET3(out, 0.0f, -0x1.dc2f840000000p+2f /* 7.4404 */);
        break;
    case 0:
        B5_SET3(out, 0.0f, -0x1.8d89380000000p+2f /* 6.2115 */);
        break;
    case 2:
        B5_SET3(out, 0.0f, 0x1.9276c80000000p+2f /* 6.2885 */);
        break;
    }
}

/* 0x0032F770 */
void Kind27_ActionOffsets(void *self, s32 id, f32 *out) {
    switch (id) {
    case 10:
    case 11:
        B5_SET3(out, 0x1.e2f8380000000p-1f /* 0.9433 */, 0x1.6807600000000p+3f /* 11.2509 */);
        break;
    case 12:
    case 13:
        B5_SET3(out, 0x1.1656040000000p+1f /* 2.1745 */, 0x1.e1573e0000000p+3f /* 15.0419 */);
        break;
    case 14:
        B5_SET3(out, -0x1.9c98600000000p+0f /* 1.6117 */, -0x1.15e00e0000000p+2f /* 4.3418 */);
        break;
    case 15:
        B5_SET3(out, 0x1.2a30560000000p-6f /* 0.0182 */, -0x1.00346e0000000p+0f /* 1.0008 */);
        break;
    }
}

/* 0x0032F820 */
void Kind27_ExitDone(u8 *self) {
    self[0x16EE] = 1;
}

/* 0x0032F830 */
void Kind27_FollowPathExit(void) {
}

/* 0x0032F840 */
void Kind27_Arrived(void) {
}

/* 0x0032F850 */
void Kind27_WalkToExit(void) {
}

/* 0x0032F860 */
void Kind27_OnToNextExit(void) {
}

/* 0x0032F870 */
s32 Kind27_PickDestination(void) {
    return 0;
}

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

/* state: the lunge animation (as Debilitas3_StateLunge) */
/* 0x0032FEA0 */
void Kind27_StateLunge(Pursuer *p) {
    Debilitas3_LungeEnd(p, &Pursuer_AttackNextStep_ptmf8);
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

/* 0x00331200 */
u8 *Kind27_ModelFileTable(Pursuer *p) {
    return b5_prog_flag8000() ? pstr_O_DB2_DB2_200_PCK_4 : pstr_O_DB2_DB2_200_PCK_3;
}

/* 0x00331240 */
void *Kind27_ModelFiles(void) {
    return b5_prog_flag8000() ? D_0042E480 : D_0042E440;
}

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

/* a model on the full base with its two parts (+0x8D0 / +0x930), 4 parts (0x50, +0x9A0) and
 * 2 members (0x40, +0xB20), vtable DebilitasModel_vtable */
/* 0x0038D4D0 */
void *DebilitasModel_ctor(u8 *m) {
    u8 *e;

    ModelBase_ctor(m);
    AT(m, 0x0, void **) = HumanModel_vtable;
    IK2_ctor(m + 0x8D0);
    IK2_ctor(m + 0x930);
    AT(m, 0x0, void **) = DebilitasModel_vtable;
    __construct_array(m + 0x9A0, HangPoint_ctor, HangPoint_dtor, 0x50, 4);
    AT(m, 0xB14, s32) = 0;
    AT(m, 0xB10, s32) = 0;
    for (e = m + 0xB20; e < m + 0xBA0; e += 0x40) {
        AT(e, 0x30, void **) = BonePoint_vtable;
    }
    return m;
}

/* a nav triangle's flags (+0x3C), 0 for none */
static inline u32 Debilitas_TriFlags(u32 tri) {
    u8 *tris = AT(gNavMesh, 0x4, u8 *);

    return tri < AT(gNavMesh, 0x8, u32) && tris != NULL ? AT(tris + tri * 0x50, 0x3C, u32) : 0;
}

extern const PTMF Debilitas_StateStartWalk_ptmf;

/* start of a wander: try as many random nav triangles as there are for one he may walk on, on
   the same floor level as his (flags 0x300000; not both), at least 20 units away; then walk
   there (state Debilitas_StateLookWalk, at most 60 frames). None: give up (+0x16EF) */
/* 0x001291C0 */
void Debilitas_StartWander(Pursuer *p) {
    void *nav = gNavMesh;
    u32 n;
    u32 i;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    n = AT(nav, 0x8, u32);
    for (i = 0; i < n; i++) {
        f32 pos[4] __attribute__((aligned(16)));
        u32 tri = 0;
        u32 flags;

        if ((s32)(n - 1) > 0) {
            tri = (s32)((f32)(s32)n * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom));
        }
        flags = Debilitas_TriFlags(tri);
        if ((flags & p->c.a.navMask) != 0) {
            continue;
        }
        if ((Debilitas_TriFlags(p->c.a.navTri) & 0x300000) != (flags & 0x300000)) {
            continue;
        }
        if ((flags & 0x100000) && (flags & 0x200000)) {
            continue;
        }
        VCALL(nav, 0xC, void (*)(void *, u32, f32 *))(nav, tri, pos);
        if (Npc_PathLength(p, tri, pos) < 20.0f) {
            continue;
        }
        PU(p, 0x15A4, u32) = tri;
        VCALL(nav, 0xC, void (*)(void *, u32, f32 *))(nav, PU(p, 0x15A4, u32), (f32 *)((u8 *)p + 0x15B0));
        VCALL(p, 0xD8, s32 (*)(Pursuer *))(p);
        break;
    }
    if (i >= n) {
        PU(p, 0x16EF, u8) = 1;
        PURSUER_STEP_NEXT(p) = 1;
        return;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &Debilitas_StateStartWalk_ptmf);
    if (Pursuer_WalkOn(p)) {
        return;
    }
    Pursuer_PlayAnimIf(p, VCALL(p, 0x328, s32 (*)(Pursuer *))(p), 0);
    PU(p, 0x1624, s32) = 60;
    Actor_SetState(&p->c.a, &Debilitas_StateLookWalk_ptmf);
    Debilitas_StateLookWalk(p);
}

extern const PTMF D_003AFF00, D_003AFF10, Debilitas_Chase_ptmf;

/* his behaviour: Fiona as the target. Out of sight of her, head for her (vtable +0xB0). Otherwise
   the pending action (0x1C rumbles the pad), or by a 0..100 roll against his table +0x17F0:
   whether she faces him (within 90 degrees of her heading, his sight range) and whether he's
   within +0x17E4 of her picks action 1 (close and seen: no wait), or 5 / 6 with the wait +0x162C
   from the table. Then his chase step (Debilitas_Chase). */
/* 0x0012B490 */
void Debilitas_Behaviour(Pursuer *p) {
    s32 next;

    if (PU(p, 0x16B4, u8) == 1) {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003AFF00);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x264, void (*)(Pursuer *))(p);
        return;
    }
    p->target = gCharPlayer;
    PU(p, 0x16F6, u8) = 1;
    if (PU(p, 0x1544, u8) == 0) {
        VCALL(p, 0xB0, void (*)(Pursuer *))(p);
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003AFF10);
        PU(p, 0x1758, s32) = -1;
        return;
    }
    next = PU(p, 0x1758, s32);
    if (next == -1) {
        f32 roll = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
        Character *t = p->target;
        u8 *tbl;

        if (!(Eye_ActorSees(p, &t->a, &p->c.a, t->a.angle[1], PU(p, 0x1580, f32), 0x1.921fb6p+0f /* 90 degrees */) & 0xFF)) {
            if (!(PU(p, 0x1588, f32) <= PU(p, 0x17E4, f32))) {
                if (roll <= AT(PU(p, 0x17F0, u8 *), 0x24, f32)) {
                    VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
                    PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), 0x0, s32);
                } else {
                    VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
                    PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), 0x4, s32);
                }
            } else if (roll <= (f32)AT(PU(p, 0x17F0, u8 *), 0x8, u32)) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
                PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), 0x8, s32);
            } else {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
                PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), 0xC, s32);
            }
        } else if (!(PU(p, 0x1588, f32) <= PU(p, 0x17E4, f32))) {
            tbl = PU(p, 0x17F0, u8 *);
            if (roll <= AT(tbl, 0x2C, f32)) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
                PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), 0x10, s32);
            } else {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
                PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), 0x14, s32);
            }
        } else {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
            PU(p, 0x162C, s32) = 0;
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
    PU(p, 0x1630, s32) = AT(PU(p, 0x17F0, u8 *), 0x20, s32);
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &Debilitas_Chase_ptmf);
    PU(p, 0x1758, s32) = -1;
    Debilitas_Chase(p);
}

/* does Fiona (the target) face him: within 90 degrees of her heading and his sight range */
static inline s32 Debilitas_Seen(Pursuer *p) {
    Character *t = p->target;

    return Eye_ActorSees(p, &t->a, &p->c.a, t->a.angle[1], PU(p, 0x1580, f32), 0x1.921fb6p+0f /* 90 degrees */) & 0xFF;
}

/* back off (action 5) or hold off (6) by his table +0x17F0 entry `i` (0 unseen and far, 1 unseen
   and close, 2 seen): chance in percent at +0x24 + 4i, waits at +8i / +8i+4 */
static void Debilitas_BackOrHold(Pursuer *p, f32 roll, s32 i) {
    if (roll <= AT(PU(p, 0x17F0, u8 *), 0x24 + i * 4, f32)) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
        PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), i * 8, s32);
    } else {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
        PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), i * 8 + 4, s32);
    }
}

extern f32 D_003AF4B0[];

/* a waited-out back-off or hold-off: strike by the chance for the threat level (Pursuer_ThresholdEntry of
   D_003AF4B0), else back or hold off again. 1 when he strikes */
static s32 Debilitas_StrikeOrWait(Pursuer *p) {
    u32 chance = Pursuer_ThresholdEntry(p, D_003AF4B0, 3);
    f32 roll;

    if (100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) < (f32)chance) {
        VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, 0xA);
        Pursuer_PickFromTable(p);
        return 1;
    }
    roll = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
    if (!Debilitas_Seen(p)) {
        Debilitas_BackOrHold(p, roll, PU(p, 0x1588, f32) <= PU(p, 0x17E4, f32) ? 1 : 0);
    } else {
        Debilitas_BackOrHold(p, roll, 2);
    }
    return 0;
}

extern const PTMF D_003AFF30, D_003AFF40, D_003AFF50;

/* his chase (from Debilitas_Behaviour): as the Pursuer's Pursuer_BehaviourStalk he stalks Fiona, closing in
   (1), backing off (5) or holding off (6) for the waits from his table +0x17F0, and now and then
   lunging (attack table 0xA) by the threat-level chance. Seen by her while close, he comes
   straight on (1) a limited number of times (+0x1630). Right up against her with no room
   around, he steps in (0x1D); close and with her standing still, grabs (0x1000) */
/* 0x0012A4C0 */
void Debilitas_Chase(Pursuer *p) {
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
            PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), 0x1C, s32);
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
    case 0x1000:
        PU(p, 0x1544, u8) = 0;
        if (PU(p, 0x16EF, u8) == 1) {
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003AFF30);
            PU(p, 0x1758, s32) = -1;
            PU(p, 0x16EF, u8) = 0;
        } else if (PURSUER_STEP_DONE(p) == 1) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 0x1D:
        if (!(PU(p, 0x1588, f32) <= PU(p, 0x17E4, f32)) ||
            (Npc_TargetSideWalkable(p, 0x1.921fb6p+1f /* 180 degrees */, 2.0f) == 0 &&
             Npc_TargetSideWalkable(p, 0x1.eb7c16p+0f /* 110 degrees */, 2.0f) == 0 &&
             Npc_TargetSideWalkable(p, -0x1.eb7c16p+0f, 2.0f) == 0)) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
        }
        if (PU(p, 0x1588, f32) < 10.0f && !(PU(p, 0x1588, f32) <= 0.0f) && gCharPlayer->moveMode == 0 &&
            gCharPlayer->moveSub != 0 && (PursuerGroup_Find(gProgress, 4, 0) & 0xFF) == 0xFF) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x1000);
            PU(p, 0x162C, s32) = 0;
        }
        break;
    case 1: {
        f32 roll;

        /* every 90 frames, a lunge by the threat-level chance */
        if ((PU(p, 0x1780, u32) + 1) % 90 == 0) {
            u32 chance = Pursuer_ThresholdEntry(p, D_003AF4B0, 3);

            if (100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) <= (f32)chance) {
                VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, 0xA);
                Pursuer_PickFromTable(p);
                return;
            }
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
        if (!Debilitas_Seen(p)) {
            Debilitas_BackOrHold(p, roll, 1);
        } else if (!(PU(p, 0x1588, f32) <= PU(p, 0x17E4, f32) + 20.0f)) {
            Debilitas_BackOrHold(p, roll, 2);
        } else if (PU(p, 0x1630, s32) > 0) {
            PU(p, 0x1630, s32)--;
        } else {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
            PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), 0x18, s32);
        }
        break;
    }
    case 6:
        if (PU(p, 0x16EF, u8) != 0) {
            PU(p, 0x16EF, u8) = 0;
        }
        if (PU(p, 0x1588, f32) <= PU(p, 0x17E4, f32) && Debilitas_Seen(p) == 1) {
            if (PU(p, 0x1630, s32) > 0) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
                PU(p, 0x1630, s32) = AT(PU(p, 0x17F0, u8 *), 0x20, s32);
                PU(p, 0x162C, s32) = 0;
            } else {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
                PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), 0x18, s32);
            }
            break;
        }
        if (PU(p, 0x162C, s32) > 0 || !(PU(p, 0x1588, f32) < 100.0f)) {
            PU(p, 0x162C, s32)--;
            break;
        }
        if (Debilitas_StrikeOrWait(p)) {
            return;
        }
        break;
    case 5:
        if (PU(p, 0x16EF, u8) != 0) {
            PU(p, 0x16EF, u8) = 0;
        }
        if (PU(p, 0x1588, f32) <= PU(p, 0x17E4, f32) && Debilitas_Seen(p) == 1 && PU(p, 0x1630, s32) > 0) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
            PU(p, 0x1630, s32) = AT(PU(p, 0x17F0, u8 *), 0x20, s32);
            PU(p, 0x162C, s32) = 0;
            break;
        }
        if (PU(p, 0x162C, s32) > 0) {
            if (PU(p, 0x1588, f32) <= 100.0f) {
                PU(p, 0x162C, s32)--;
            } else {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
                PU(p, 0x162C, s32) = AT(PU(p, 0x17F0, u8 *), 0x1C, s32);
            }
            break;
        }
        if (Debilitas_StrikeOrWait(p)) {
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
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003AFF40);
        PU(p, 0x1758, s32) = -1;
        PU(p, 0x162C, s32) = 0;
        return;
    }
    if (AT(gProgress, 0x7B8, u8) == 5) {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003AFF50);
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
