/* Hewie (vtable 0x46A120, 0xF37C0 bytes, SceneGame +0xE35F80 = character slot 1). */
#ifndef HEWIE_H
#define HEWIE_H

#include "actor.h"
#include "common.h"

/* Hewie fields not understood yet, by offset. */
#define HW(h, off, type) (*(type *)((u8 *)(h) + (off)))

typedef struct Hewie {
    /* 0x00000 */ Character c;
    /* 0x01540 */ u8 pad1540[0xF37C0 - 0x1540];   /* +0x1540: his .PCK (model) */
} Hewie;
_Static_assert(sizeof(Hewie) == 0xF37C0, "Hewie size");

#define HEWIE_NAV_MASK 0x29020008
#define HEWIE_ACTION(h) HW(h, 0xF3564, s32)                  /* current action (Hewie_SetAction) */
#define HEWIE_SIDE(h) HW(h, 0xF3668, s32)                    /* side of the room (rooms +0x50) */
#define HEWIE_STATE(h) ((PTMF *)((u8 *)(h) + 0xF35D0))       /* behaviour (pointer to member) */
#define HEWIE_MSG(h) HW(h, 0xF3540, void *)                  /* his message image */
#define HEWIE_MRK(h) ((u8 *)(h) + 0xF1540)                   /* his .MRK data */
#define HEWIE_HP(h) ((h)->c.hp)

typedef struct Character Character;

/* A placement: room, side, triangle (... +0x38 the exit he came by). */
typedef struct HewiePlacement {
    /* 0x00 */ s32 room;
    /* 0x04 */ s32 side;
    /* 0x08 */ u32 tri;
    /* 0x0C */ u8 pad0C[0x2C];
    /* 0x38 */ u8 exit;
} HewiePlacement;

/* hewie.c */
extern s32 Hewie_AnimGroup(Hewie *h);
extern s32 Hewie_AdjustAction(Hewie *h, s32 act);
extern void Hewie_Chance(Hewie *h, s32 n);
extern void Hewie_SpendPool(Hewie *h, s32 amount);
extern s32 Hewie_PlanAndGo(Hewie *h, u32 tri, const f32 *pos, s32 direct, s32 keep);
extern void Hewie_SetMode(Hewie *h, s32 mode, s32 time);
extern void Hewie_ChangeFeeling(Hewie *h, Character *other, s32 delta);   /* tell Hewie */
extern void Hewie_AddTrust(Hewie *h, s32 add);   /* his trust */
extern void Hewie_Restart(Hewie *h);   /* Hewie restarted (hewie.c) */
extern void Hewie_MakeSound(Hewie *h, s32 snd);
extern void Hewie_StandAnim(Hewie *h, s32 blend);
extern void Hewie_StatePose2(Hewie *h);
extern void Hewie_StatePose4(Hewie *h);
extern void Hewie_State2318(Hewie *h);
extern void Hewie_State20C8(Hewie *h);
extern void Hewie_State20A8(Hewie *h);
extern void Hewie_State2018(Hewie *h);
extern void Hewie_StateLoudNoise(Hewie *h);
extern void Hewie_SetAnim(Hewie *h, s32 a, s32 anim);
extern void Hewie_StatePlayAnim(Hewie *h);
extern void Hewie_StateToDefault(Hewie *h);
extern void Hewie_State2098(Hewie *h);
extern void Hewie_State22B8(Hewie *h);
extern void Hewie_StateAnimOver(Hewie *h);
extern void Hewie_State2358(Hewie *h);
extern s32 Hewie_FreeForCommand2(Hewie *h);   /* Hewie listening (hewie.c) */
extern void Hewie_State22E8(Hewie *h);
extern void Hewie_StateRootMotion(Hewie *h);
extern void Hewie_State1F88(Hewie *h);
extern s32 Hewie_WithChar2(Hewie *h, Character *other);
extern void Hewie_State1A60(Hewie *h);
extern s32 Hewie_MayBreakOff(Hewie *h, s32 once);
extern void Hewie_State23D8(Hewie *h);
extern void Hewie_State1D88(Hewie *h);
extern void Hewie_State2168(Hewie *h);
extern void Hewie_State1F28(Hewie *h);
extern void Hewie_State2198(Hewie *h);
extern s32 Hewie_CanTakeCommand(Hewie *h);
extern s32 Hewie_FionaReachable(Hewie *h);
extern void Hewie_RandomIdle(Hewie *h);
extern void Hewie_State2388(Hewie *h);
extern void Hewie_StateAfter2B(Hewie *h);
extern void Hewie_StateAfter29(Hewie *h);
extern void Hewie_StateAfter27(Hewie *h);
extern void Hewie_State1DC8(Hewie *h);
extern void Hewie_State2418(Hewie *h);
extern void Hewie_StateBackOnMesh(Hewie *h);
extern s32 Hewie_RandomLevel(Hewie *h);
extern void Hewie_State1E48(Hewie *h);
extern s32 Hewie_FionaCanCommand(Hewie *h);
extern void Hewie_State1D98(Hewie *h);
extern void Hewie_State2138(Hewie *h);
extern void Hewie_SetHealthState(Hewie *h, s32 st);
extern void Hewie_StateTargetPursuer(Hewie *h);
extern void Hewie_StateTurnStart(Hewie *h);
extern void Hewie_IdleAction(Hewie *h);
extern s32 Hewie_FionaNearCommand(Hewie *h);
extern void Hewie_StateRun(Hewie *h);
extern void Hewie_StateRunToFiona(Hewie *h);
extern void Hewie_StateFollowMover(Hewie *h);
extern void Hewie_StateMoveAlong(Hewie *h);
extern void Hewie_RollReaction(Hewie *h);
extern void Hewie_StateTurnWithFiona(Hewie *h);
extern void Hewie_StateSteer(Hewie *h);
extern Character *Hewie_PickTarget(Hewie *h);
extern u8 Hewie_FleeExit(Hewie *h, Character *from, s32 both);
extern void Hewie_WhenIdle(Hewie *h);
extern void Hewie_AfterComingOut(Hewie *h);
extern void Hewie_AfterCall(Hewie *h);
extern void Hewie_After4D(Hewie *h);
extern void Hewie_StateBark(Hewie *h);
extern void Hewie_StateRunUpJump(Hewie *h);
extern void Hewie_StateSteered(Hewie *h);
extern void Hewie_StateKeepNear(Hewie *h);
extern void Hewie_StateKnockedDown(Hewie *h);
extern void Hewie_StateHeadForSpot(Hewie *h);
extern void Hewie_StateSetOff(Hewie *h);
extern void Hewie_StateRunForSpot(Hewie *h);
extern void Hewie_StateWhine(Hewie *h);
extern void Hewie_StatePlanPursuer(Hewie *h);
extern void Hewie_StateFetch(Hewie *h);
extern void Hewie_StateKeepBehind(Hewie *h);
extern void Hewie_StateSlope(Hewie *h);
extern void Hewie_StateCloseIn(Hewie *h);
extern void Hewie_StateOffMesh(Hewie *h);
extern void Hewie_StateStepAwayTarget(Hewie *h);
extern void Hewie_StateStepAwayFiona(Hewie *h);
extern void Hewie_StateWalkOut(Hewie *h);
extern void Hewie_StateSqueeze(Hewie *h);
extern void Hewie_StateToDoor(Hewie *h);
extern void Hewie_StateCloseOnTarget(Hewie *h);
extern void Hewie_StateRunAtPursuer(Hewie *h);
extern void Hewie_StateGoToExit(Hewie *h);
extern void Hewie_StateTricks(Hewie *h);
extern void Hewie_StateComeToCommand(Hewie *h);
extern void Hewie_StateRoam(Hewie *h);
extern void Hewie_StateScramble(Hewie *h);
extern void Hewie_StateFlank(Hewie *h);
extern void Hewie_StateKeepAway(Hewie *h);
extern void Hewie_StateSlideToFiona(Hewie *h);
extern void Hewie_StateBarkAtFiona(Hewie *h);
extern void Hewie_StateBarkAtTarget(Hewie *h);
extern void Hewie_StateWaitForFiona(Hewie *h);
extern void Hewie_StateFaceTarget(Hewie *h);
extern void Hewie_StateFaceScent(Hewie *h);
extern s32 Hewie_PlaceAtPlacement(Hewie *h, HewiePlacement *pl);

/* ---- (was hewie_act.h) ---- */

/* hewie_act.c: what other files call. */

typedef struct Hewie Hewie;

/* hewie_act.c */
extern void Hewie_SetAction(Hewie *h, s32 act, s32 arg);   /* his action */

/* hewie.c */
extern void *DogModel_ctor(u8 *m, u8 kind);
extern void *DogModelB_ctor(u8 *m, u8 kind);
extern void *DogModelA_ctor(u8 *m, u8 kind);

extern void *DogModelArray_ctor(void *p);

#endif
