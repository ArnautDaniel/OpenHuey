/* Pursuer: the shared base of the stalkers (Debilitas, Daniella, Riccardo, Lorenzo) and the
 * story characters loaded into character slot 2 by CharLoad_Partner (0x28 kinds).
 *
 * Classes: Actor (vtable 0x469C20) -> Character (0x469C60) -> NPC (0x46C220, 64 entries:
 * navigation / doors / vision, code 0x211C80..0x219530, src/game/pursuer_ai.c) -> Pursuer
 * (0x46D810, 204 entries, dtor 0x172810, code 0x278490..0x29FF10, src/game/pursuer.c) -> each
 * kind (e.g. Debilitas 0x469D10, Daniella 0x46BBB0, Riccardo 0x46F6B0, Lorenzo 0x470720),
 * which overrides a few entries (+0x8 dtor, +0x30 update, +0xF4 / +0xF8 model load, ...).
 * The kinds' constructors only store Actor, Character and their own vtable; the middle ones
 * appear in the destructors. Shared defaults of the kinds: 0x179600..0x179970.
 * Objects are 0x1800 bytes (0x1840 for Riccardo and a few others). */
#ifndef PURSUER_H
#define PURSUER_H

#include "actor.h"
#include "progress.h"
#include "sce/libvu0.h"
#include "effectmgr.h"
#include "debilitas.h"
#include "model.h"
#include "common.h"

/* Pursuer fields not understood yet, by offset. */
#define PU(p, off, type) (*(type *)((u8 *)(p) + (off)))

typedef struct Pursuer {
    /* 0x0000 */ Character c;
    /* 0x1540 */ Character *target;   /* the character being chased */
    /* 0x1544 */ u8 pad1544[0x1800 - 0x1544];
} Pursuer;
_Static_assert(sizeof(Pursuer) == 0x1800, "Pursuer size");

/* the animation player at Character +0xF0 */
#define MOTION_AT(p, off, type) (*(type *)((u8 *)(p)->c.motion + (off)))
#define MOTION_ANIM(p) MOTION_AT(p, 0x55C, s32)                         /* current animation id */
#define MOTION_KEYS(p) (*(u32 *)(MOTION_AT(p, 0x6A4, u8 *) + 0x18))     /* key flags of the frame */
#define MOTION_KEY_END 0x20                                             /* the animation ended */

/* the current behaviour step is over (+0x16EE) / start the next (+0x16F0) */
#define PURSUER_STEP_DONE(p) PU(p, 0x16EE, u8)
#define PURSUER_STEP_NEXT(p) PU(p, 0x16F0, u8)

/* the game's characters and managers the pursuer code uses */

extern void *Pursuer_vtable[], *NPC_vtable[], *Character_vtable[], *Actor_vtable[];

/* The flags of an out-of-range nav triangle: the original takes the record pointer as NULL and
 * reads +0x3C anyway, i.e. a word of low kernel memory on the PS2. Kept for the difftest build;
 * natively an invalid triangle has no flags. */
#ifdef HG_NATIVE
#define NAV_BAD_TRI_FLAGS 0u
#else
#define NAV_BAD_TRI_FLAGS (*(volatile u32 *)0x3C)
#endif

/* ---- engine functions used (tools/pursuer/externs.py) ---- */
/* ---- end engine ---- */

/* ---- generated from the definitions (tools: protos.py) ---- */
/* ---- end generated ---- */

/* pursuer.c */
extern void Pursuer_AttackTable(Pursuer *p);
extern void Pursuer_StateRootStep(Pursuer *p);
extern s32 Pursuer_ChasingHewie(Pursuer *p);
extern void Pursuer_StopSounds(Pursuer *p);
extern void Pursuer_ActionOffsets(Pursuer *p, s32 a1, s32 *out);
extern void Pursuer_Deactivate(Pursuer *p);
extern void Pursuer_RememberPos(Pursuer *p);
extern void Pursuer_Cleanup(Pursuer *p);
extern void Pursuer_Door1C4(Pursuer *p);
extern void Pursuer_SetGoalTri(Pursuer *p, u32 tri);
extern void Pursuer_ClearSteps(Pursuer *p);
extern void Pursuer_Timer15s(Pursuer *p);
extern s32 Pursuer_StanceAnim(Pursuer *p);
extern s32 Pursuer_WalkAnim(Pursuer *p);
extern s32 Pursuer_SlowWalkAnim(Pursuer *p);
extern void Pursuer_StateStopAtEnd(Pursuer *p);
extern void Pursuer_StepCount(Pursuer *p);
extern void Pursuer_StateStepToEnd(Pursuer *p);
extern s32 Pursuer_InStance2Anim(Pursuer *p);
extern void Pursuer_StateEndStep(Pursuer *p);
extern void Pursuer_StateCountKeys(Pursuer *p);
extern void Pursuer_Disable(Pursuer *p);
extern void Pursuer_EventOver2(Pursuer *p);
extern void Pursuer_Enable(Pursuer *p);
extern s32 Pursuer_LoadMessage(Pursuer *p);
extern void **DoorShadow_dtor(void **obj, s32 flags);
extern s32 Pursuer_ThresholdEntry(Pursuer *p, f32 *table, u32 n);
extern void Pursuer_StateRunToTri(Pursuer *p);
extern void Pursuer_WaitRoomFlag(Pursuer *p);
extern s32 Pursuer_MayGoForTarget(Pursuer *p);
extern void Pursuer_DoorThroughOpen(Pursuer *p);
extern void Pursuer_DoorIdle(Pursuer *p);
extern void Pursuer_Move180(Pursuer *p);
extern void Pursuer_Plan170(Pursuer *p);
extern void Pursuer_Door208(Pursuer *p);
extern s32 Pursuer_IsOwnRoom(Pursuer *p, s32 room);
extern s32 Pursuer_Place(Pursuer *p, u32 tri, const f32 *heading, f32 *pos);
extern void Pursuer_StateRunThenNext(Pursuer *p);
extern void Pursuer_LightChange(Pursuer *p);
extern void Pursuer_LeftBehind(Pursuer *p);
extern void Pursuer_ReleaseModelFiles(Pursuer *p);   /* a character's quick unload (keeps its model) */
extern void Pursuer_FilesLoading(Pursuer *p);
extern void Pursuer_StateRelation1(Pursuer *p);
extern void Pursuer_StateRelation2(Pursuer *p);
extern void Pursuer_StateCloseA(Pursuer *p);
extern void Pursuer_StateCloseB(Pursuer *p);
extern s32 Pursuer_RandomDelay(Pursuer *p);
extern void Pursuer_StateBackOnFeet(Pursuer *p);
extern void Pursuer_Unload(Pursuer *p);
extern void Pursuer_ClearBehaviour(Pursuer *p);
extern void Pursuer_StateTauntHold(Pursuer *p);
extern u32 Pursuer_PathNodeSound(Pursuer *p);
extern void Pursuer_BonePositions(Pursuer *p, s32 *bones, f32 *a, f32 *b);
extern void Pursuer_Setup(Pursuer *p);
extern void Pursuer_Plan16C(Pursuer *p);
extern void Pursuer_StateStepFacing(Pursuer *p);
extern void Pursuer_Door214(Pursuer *p);
extern void Pursuer_ExitOpen(Pursuer *p);
extern void Pursuer_Sound(Pursuer *p, s32 sound, s32 a2, s32 a3, s32 a4, void *a5);
extern void Pursuer_RaiseThreat(Pursuer *p, u32 mask);
extern void Pursuer_StateTauntStart(Pursuer *p);
extern void Pursuer_StateGrabStart(Pursuer *p);
extern void Pursuer_DoorWalk(Pursuer *p);
extern void Pursuer_DoorThrough(Pursuer *p);
extern void Pursuer_LoadMotions(Pursuer *p, s32 id);
extern void Pursuer_FollowPlan(Pursuer *p);
extern void Pursuer_StateTaunt(Pursuer *p);
extern void Pursuer_ExitGoTo(Pursuer *p);
extern void Pursuer_SaveState(Pursuer *p);
extern void Pursuer_BehaviourEnded(Pursuer *p);
extern void Pursuer_Activate(Pursuer *p);
extern void Pursuer_ChaseTarget(Pursuer *p);
extern void Pursuer_ResetBehaviour(Pursuer *p);
extern void Pursuer_GiveMotionBanks(Pursuer *p, u32 slot);
extern void Pursuer_StateTurnAnim(Pursuer *p);
extern void Pursuer_ExitDone(Pursuer *p);
extern void Pursuer_Behaviour288(Pursuer *p);
extern void Pursuer_ChaseFiona(Pursuer *p);
extern void Pursuer_Behaviour25C(Pursuer *p);
extern void Pursuer_Update(Pursuer *p);
extern s32 Pursuer_TargetOutOfReach(Pursuer *p);
extern u32 Pursuer_CryHeard(Pursuer *p);
extern void Pursuer_ModelTakeBanks(Pursuer *p);
extern void Pursuer_Threat(Pursuer *p, f32 amount);
extern void Pursuer_StanceStep(Pursuer *p);
extern s32 Pursuer_PlayAnimIf(Pursuer *p, s32 anim, s32 blend);
extern void Pursuer_StateHitReact(Pursuer *p);
extern void Pursuer_StateWalkOn(Pursuer *p);
extern void Pursuer_Door1C8(Pursuer *p);
extern void Pursuer_Move184(Pursuer *p);
extern void Pursuer_Exit1F0(Pursuer *p);
extern s32 Pursuer_AttackPoint(Pursuer *p, f32 *out);
extern void Pursuer_BehaviourIdle(Pursuer *p);
extern void Pursuer_BackToStand(Pursuer *p);
extern s32 Pursuer_ChanceRoll(Pursuer *p);
extern void Pursuer_PlaceModel(Pursuer *p);
extern Pursuer *Pursuer_dtor(Pursuer *p, s32 flags);
extern void Pursuer_ExitOpenStep(Pursuer *p);
extern void Pursuer_StateArrive(Pursuer *p);
extern s32 Pursuer_EventConcerns(Pursuer *p, u32 kind, s32 slot, u32 door);
extern void Pursuer_WalkToExit(Pursuer *p);
extern void Pursuer_DoorArmOpen(Pursuer *p);
extern s32 Pursuer_FionaState(Pursuer *p);
extern void Pursuer_ExitThrough(Pursuer *p);
extern void Pursuer_EventReset(Pursuer *p);
extern void Pursuer_DoorPushThrough(Pursuer *p);
extern void Pursuer_GoAfterFiona(Pursuer *p);
extern void Pursuer_StateTauntRepeat(Pursuer *p);
extern void Pursuer_WalkToGoal(Pursuer *p);
extern void Pursuer_PickAttack(Pursuer *p);
extern f32 Pursuer_GroundGained(Pursuer *p);
extern void Pursuer_KeepOnWalkable(Pursuer *p);
extern void Pursuer_StateHitOver(Pursuer *p);
extern void Pursuer_StateLookAround(Pursuer *p);
extern void Pursuer_GoToDoor(Pursuer *p);
extern s32 Pursuer_PlanWhere(Pursuer *p);
extern void Pursuer_FilesLoaded(Pursuer *p);
extern void Pursuer_StateDoorAhead(Pursuer *p);
extern void Pursuer_DoorBackOff(Pursuer *p);
extern void Pursuer_StateStand(Pursuer *p);
extern void Pursuer_DoorNear(Pursuer *p);
extern void Pursuer_StateTurnThenWait(Pursuer *p);
extern void Pursuer_ModelUpdate(Pursuer *p);
extern void Pursuer_BackToNormal(Pursuer *p);
extern void Pursuer_DoorAnim(Pursuer *p);
extern void Pursuer_DoorGoThrough(Pursuer *p);
extern void Pursuer_AddSearchStops(Pursuer *p, s32 n);
extern void Pursuer_DoorWait(Pursuer *p);
extern void Pursuer_GiveUp(Pursuer *p);
extern void Pursuer_DoorFace(Pursuer *p);
extern void Pursuer_SearchRoute(Pursuer *p);
extern void Pursuer_Arrived(Pursuer *p);
extern void Pursuer_FollowPathExit(Pursuer *p);
extern void Pursuer_SearchOffscreen(Pursuer *p);
extern void Pursuer_StateHurt(Pursuer *p);
extern void Pursuer_DoorFacing(Pursuer *p);
extern void Pursuer_StanceByFiona(Pursuer *p);
extern void Pursuer_Move18C(Pursuer *p);
extern void Pursuer_Move17C(Pursuer *p);
extern void Pursuer_StateFaceFiona(Pursuer *p);
extern void Pursuer_NextPathExit(Pursuer *p);
extern void Pursuer_Door1E4(Pursuer *p);
extern void Pursuer_OnToNextExit(Pursuer *p);
extern void Pursuer_DoorLineUp(Pursuer *p);
extern void Pursuer_BackOnMesh(Pursuer *p);
extern u8 *Pursuer_MotionFiles(Pursuer *p);
extern void Pursuer_StateSidestep(Pursuer *p);
extern void Pursuer_KnockAtDoor(Pursuer *p);
extern void Pursuer_StateWalkThenAnim(Pursuer *p);
extern void Pursuer_Pace(Pursuer *p);
extern void Pursuer_StateWalkThen404(Pursuer *p);
extern void Pursuer_PlanToGoal(Pursuer *p);
extern void Pursuer_StateWalkGesture(Pursuer *p);
extern s32 Pursuer_PlaceInRoom(Pursuer *p, s32 room, s32 tri, u32 side);
extern u8 *Pursuer_ModelFiles(Pursuer *p);
extern void Pursuer_StateAttackActive(Pursuer *p);
extern void Pursuer_DoorPush(Pursuer *p);
extern void Pursuer_ChaseFionaHere(Pursuer *p);
extern void Pursuer_PlanWayOn(Pursuer *p);
extern void Pursuer_GoForHewie(Pursuer *p);
extern void Pursuer_Chase1A4(Pursuer *p);
extern void Pursuer_Chase1A0(Pursuer *p);
extern void Pursuer_StateSidestepRoom(Pursuer *p);
extern void Pursuer_EventState(Pursuer *p);
extern void Pursuer_FootstepsThroughWalls(Pursuer *p);
extern void Pursuer_PickFromTable(Pursuer *p);
extern void Pursuer_AfterMove(Pursuer *p);
extern void Pursuer_IntoRoomByEvent(Pursuer *p, s32 room, u32 found, s32 plan, s32 side);
extern void Pursuer_StateBargedThrough(Pursuer *p);
extern void Pursuer_EventOver(Pursuer *p, s32 exit);
extern void Pursuer_LoadFiles(Pursuer *p);
extern s32 Pursuer_MayUseExit(Pursuer *p, u32 exit);
extern void Pursuer_DoorBarge(Pursuer *p);
extern void Pursuer_BackToStance(Pursuer *p);
extern void Pursuer_StateSpecialAnim(Pursuer *p);
extern void Pursuer_StartSearch(Pursuer *p);
extern void Pursuer_LocateTarget(Pursuer *p);
extern void Pursuer_StateKnockedThrough(Pursuer *p);
extern void Pursuer_DoorBackAway(Pursuer *p);
extern void Pursuer_ExitClosed(Pursuer *p, u32 exit);
extern s32 Pursuer_GrabHewieSpot(Pursuer *p, u32 kind, f32 *heading, f32 *pos);
extern void Pursuer_HitOffscreen(Pursuer *p);
extern void Pursuer_KnockedDown(Pursuer *p);
extern void Pursuer_GoForFionaStance0(Pursuer *p);
extern void Pursuer_Think(Pursuer *p);
extern void Pursuer_StateHitAtDoor(Pursuer *p);
extern void Pursuer_AnimSounds(Pursuer *p, s32 anim);
extern void Pursuer_WalkAside(Pursuer *p);
extern void Pursuer_Reset(Pursuer *p);
extern void Pursuer_TravelOffscreen(Pursuer *p);
extern s32 Pursuer_PickTarget(Pursuer *p);
extern s32 Pursuer_GrabOrder(Pursuer *p);
extern void Pursuer_SearchRoom(Pursuer *p);
extern void Pursuer_StateHitReaction(Pursuer *p);
extern void Pursuer_StateHewieBites(Pursuer *p);
extern void Pursuer_DoorApproach(Pursuer *p);
extern void Pursuer_CloseInGoal(Pursuer *p);
extern s32 Pursuer_GrabHewieBehind(Pursuer *p);
extern void Pursuer_LookAround(Pursuer *p);
extern void Pursuer_SearchRouteIn(Pursuer *p);
extern void Pursuer_StateCloseOnFionaA(Pursuer *p);
extern void Pursuer_StateCloseOnFionaB(Pursuer *p);
extern void Pursuer_ChaseDecision(Pursuer *p);
extern void Pursuer_LoadState(Pursuer *p);
extern void Pursuer_StateKnockedDown(Pursuer *p);
extern void Pursuer_AttackNextStep(Pursuer *p);
extern void Pursuer_OffscreenStep(Pursuer *p);
extern void Pursuer_HeadLook(Pursuer *p);
extern void Pursuer_DoorWalking(Pursuer *p);
extern void Pursuer_DoorWalkTo(Pursuer *p);
extern void Pursuer_StateDownAndUp(Pursuer *p);
extern void Pursuer_PickStep(Pursuer *p);
extern void Pursuer_StepBack(Pursuer *p);
extern void Pursuer_PickStepAlt(Pursuer *p);
extern void Pursuer_WalkToSpot(Pursuer *p);
extern void Pursuer_FrameUpdate(Pursuer *p);
extern void Pursuer_OffscreenUpdate(Pursuer *p);
extern void Pursuer_EnterRoom(Pursuer *p);
extern void Pursuer_Stairs(Pursuer *p);
extern void Pursuer_ReactToEvent(Pursuer *p);
extern void Pursuer_BehaviourRun(Pursuer *p);
extern void Pursuer_DoorOpen(Pursuer *p);
extern s32 Pursuer_TurnOnSpot(Pursuer *p, u32 dir);
extern void Pursuer_MotionGroup(Pursuer *p);
extern void Pursuer_CarryOnLying(Pursuer *p);
extern void Pursuer_StateFlinch(Pursuer *p);
extern void Pursuer_Modes(Pursuer *p);
extern void Pursuer_StateAttackStep(Pursuer *p);
extern void Pursuer_Footsteps(Pursuer *p);
extern void Pursuer_ModesSearching(Pursuer *p);
extern void Pursuer_EventCommand(Pursuer *p);
extern void Pursuer_StateHewieHolds(Pursuer *p);
extern void Pursuer_BehaviourSearch(Pursuer *p);
extern void Pursuer_ThroughDoor(Pursuer *p, u32 door);
extern void Pursuer_ThroughDoorEnding(Pursuer *p, u32 door);
extern void Pursuer_BehaviourAttack(Pursuer *p);
extern void Pursuer_BehaviourHewie(Pursuer *p);
extern void Pursuer_BehaviourStalk(Pursuer *p);
extern void Pursuer_BehaviourDoors(Pursuer *p);
extern void Pursuer_ShowUp(Pursuer *p);
extern void Pursuer_Hit(Pursuer *p);
extern void Pursuer_BehaviourFollow(Pursuer *p);
extern void Pursuer_LeaveScreenEnding(Pursuer *p, u32 door);
extern void Pursuer_LeaveScreen(Pursuer *p, u32 door);
extern void Pursuer_StartAction(Pursuer *p, u32 kind);
extern void Pursuer_Nothing190(Pursuer *p);
extern s32 Pursuer_Get2D0(Pursuer *p);
extern s32 Pursuer_Get2D4(Pursuer *p);
extern s32 Pursuer_Get318(Pursuer *p);
extern u8 *Debilitas_ModelFileTable(Pursuer *p);
extern void *Kind15_ctor(void *p, u32 id, u32 arg);
extern void *Debilitas2_ctor(void *p, s32 arg);
extern void *Debilitas_ctor(void *p, s32 arg);

/* pursuer.c */
extern void Summoner_Reset(u8 *p);
extern void Summoner_Take(u8 *o, u8 kind);
extern void Summoner_SetCooldown(u8 *o, s32 sec);
extern void Summoner_LessCooldown(u8 *o, s32 sec);
extern void Summoner_RoomStart(u8 *o);
extern void Summoner_Noise(u8 *o, u8 *n);

/* The Pursuer destructor's body down to the Actor (each stalker's destructor sets its own vtable
 * and runs this inline): vtable +0x10 cleanup at each level, the model freed for slots 3..5. */
static inline void Pursuer_DestroyBase(Pursuer *p) {
    p->c.a.vtbl = Pursuer_vtable;
    VCALL(p, 0x10, void (*)(Pursuer *))(p);
    if ((u32)p->c.a.slot >= 3 && (u32)p->c.a.slot < 6) {
        void **m = p->c.motion;

        if (m != NULL) {
            if (m != NULL) {
                VCALL(m, 0x8, void (*)(void *, s32))(m, 1);
            }
            p->c.motion = NULL;
        }
    }
    if (p != NULL) {
        p->c.a.vtbl = NPC_vtable;
        VCALL(p, 0x10, void (*)(Pursuer *))(p);
        if (p != NULL) {
            p->c.a.vtbl = Character_vtable;
            if (p != NULL) {
                p->c.a.vtbl = Actor_vtable;
            }
        }
    }
}

/* ---- helpers shared by the pursuer files ---- */

extern s32 Npc_WalkPathStride(Pursuer *p, s32 unused);
/* keep walking until the animation (+0x550) is over; returns 1 while walking */
static inline s32 Pursuer_WalkOn(Pursuer *p) {
    s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

    if (((over ^ 1) & 0xFF) == 1) {
        if (PU(p, 0x15C0, u8) != 0xFF) {
            if (p->c.unk128 < p->c.unk124) {
                Npc_WalkPathStride(p, p->c.unk128);
            }
        } else {
            Character_RootMoveMasked(&p->c);
        }
        return 1;
    }
    return 0;
}

/* play `anim` unless it is already playing (or ended and loops) */
static inline void Pursuer_PlayAnim(Pursuer *p, s32 anim) {
    u8 *m = p->c.motion;

    if (anim == AT(m, 0x55C, s32)) {
        s32 over = AT(m, 0x550, f32) <= 0.0f;

        if ((over ^ 1) & 0xFF) {
            return;
        }
        if (((AT(AT(m, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0) != 1) {
            s32 i = Motion_AnimIndex(m, anim);
            u16 fl = i != -1 ? AT(AT(m, 0x874, u8 *) + i * 6, 0x4, u16) : 0;

            if (fl & 4) {
                return;
            }
        }
        Motion_PlayTable(p->c.motion, anim, -1);
    } else {
        Motion_PlayTable(m, anim, -1);
    }
}

/* play `anim`: restarted like Pursuer_PlayAnim if it is the current one, else blended in */
static inline void Pursuer_PlayAnimBlend(Pursuer *p, s32 anim) {
    u8 *m = p->c.motion;

    if (AT(m, 0x55C, s32) == anim) {
        Pursuer_PlayAnim(p, anim);
    } else {
        Motion_Play(m, anim, -1);
    }
}

static inline void Pursuer_SetMove(Pursuer *p, PTMF *m) {
    ptmf_set((PTMF *)((u8 *)p + 0x17A0), m);
    PU(p, 0x17AC, s32) = AT(m, 0xC, s32);
    p->c.moveMode = AT(m, 0x10, s32);
    p->c.moveSub = AT(m, 0x14, s32);
    p->c.unk1530 = 0;
    p->c.unk1538 = 0;
    p->c.unk1534 = 0;
}

/* forget the search route (see Pursuer_AddSearchStops) */
static inline void Pursuer_ClearRoute(Pursuer *p) {
    s32 k;

    for (k = 0; k < 8; k++) {
        PU(p, 0x15E0 + k * 8, s32) = -1;
        PU(p, 0x15E4 + k * 8, u8) = 0;
    }
    PU(p, 0x1620, u8) = 0xFF;
    PU(p, 0x1621, u8) = 0xFF;
    PU(p, 0x1794, s32) = 0;
}

/* ---- the stalkers' frame update (vtable +0x30), shared by their own versions ---- */

extern s32 Npc_SensesWatching(Pursuer *p);
extern void Npc_WhoAroundEnding(Pursuer *p);
/* the start: blocking flags, senses (Npc_SensesWatching; while the behaviour is fresh, +0x16F6, they
   are recomputed, else forgotten), vtable +0x120, the threat */
static inline void Stalker_ThinkStart(Pursuer *p) {
    VCALL(p, 0x84, void (*)(Pursuer *))(p);
    p->c.a.navMask = p->c.a.unk2B == 1 ? 8 : VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
    p->c.pathReq->mask = p->c.a.navMask;
    Npc_SensesWatching(p);
    if (PU(p, 0x16F6, u8) == 1) {
        Npc_WhoAroundEnding(p);
    } else {
        PU(p, 0x1544, u8) = 0;
        PU(p, 0x1545, u8) = 0;
        PU(p, 0x1546, u8) = 0;
        PU(p, 0x16CB, u8) = 0;
        PU(p, 0x16CC, u8) = 0;
    }
    VCALL(p, 0x120, void (*)(Pursuer *))(p);
    Pursuer_MotionGroup(p);
}

/* the end: frames in state/behaviour (+0x1784, +0x1780), the room wait +0x1664 / the route rest
   +0x17B4, the stand-down +0x1790, the cry hold +0x178C, the stun, the route growing back
   +0x1794 (Stalker_ThinkTimers); then the model (+0x40) and the stance (+0x100) */
static inline void Stalker_ThinkTimers(Pursuer *p) {
    if (PU(p, 0x1784, s32) != -1) {
        PU(p, 0x1784, s32)++;
    }
    if (PU(p, 0x1780, s32) != -1) {
        PU(p, 0x1780, s32)++;
    }
    if (PU(p, 0x1664, s32) != 0) {
        PU(p, 0x1664, s32)--;
    } else if (PU(p, 0x17B4, s32) != 0) {
        PU(p, 0x17B4, s32)--;
        if (PU(p, 0x17B4, s32) == 0) {
            PU(p, 0x1620, u8) = PU(p, 0x1621, u8);
        }
    }
    if (PU(p, 0x1790, s32) != 0 && p->c.moveSub != 9) {
        PU(p, 0x1790, s32)--;
        if (PU(p, 0x1790, s32) == 0) {
            p->c.a.unkC4 = 0;
            PU(p, 0x16F5, u8) = 0;
        }
    }
    if (PU(p, 0x178C, s32) != 0) {
        PU(p, 0x178C, s32)--;
        if (PU(p, 0x178C, s32) == 0) {
            PU(p, 0x1760, u8) = 0;
        }
    }
    Debilitas_StunDown(p);
    if (PU(p, 0x1794, s32) != 0) {
        PU(p, 0x1794, s32)--;
        if (PU(p, 0x1794, s32) == 0 && PU(p, 0x1620, u8) < PU(p, 0x1621, u8)) {
            PU(p, 0x1621, u8) = PU(p, 0x1620, u8) + 1;
        }
    }
}

static inline void Stalker_ThinkEnd(Pursuer *p) {
    Stalker_ThinkTimers(p);
    VCALL(p, 0x40, void (*)(Pursuer *))(p);
    VCALL(p, 0x100, void (*)(Pursuer *))(p);
}

/* ---- (was pursuer_ai.h) ---- */

/* pursuer_ai.c: what other files call. */

typedef struct Actor Actor;
typedef struct Character Character;
typedef struct Pursuer Pursuer;

/* pursuer_ai.c */
extern s32 Pursuer_GivesUp(Pursuer *p);
extern void Pursuer_GoForFiona(Pursuer *p);
extern void Pursuer_HeadingStep(Pursuer *p);
extern void Pursuer_FreshStart(Pursuer *p);
extern void Pursuer_CarryOn(Pursuer *p);
extern void Pursuer_Timer10s(Pursuer *p);
extern void Pursuer_SetTimer(Pursuer *p, s32 frames);
extern f32 Pursuer_TurnRate(Pursuer *p);
extern f32 Pursuer_TurnRateFast(Pursuer *p);
extern f32 Pursuer_LookFrames(Pursuer *p);
extern f32 Pursuer_LookSwing(Pursuer *p);
extern f32 Pursuer_ReachFiona(Pursuer *p);
extern f32 Pursuer_Dist2E8(Pursuer *p);
extern f32 Pursuer_AttackAngle(Pursuer *p);
extern f32 Pursuer_ReachHewie(Pursuer *p);
extern f32 Pursuer_AttackRange(Pursuer *p);
extern f32 Pursuer_SpeedTop(Pursuer *p);
extern f32 Pursuer_SpeedBase(Pursuer *p);
extern s32 Pursuer_AttackAnimA(Pursuer *p);
extern s32 Pursuer_AttackAnimB(Pursuer *p);
extern s32 Pursuer_RoomSpots(Pursuer *p);
extern void NPC_PickDestination(Pursuer *p);
extern void NPC_DoorBreak(Pursuer *p);
extern s32 Npc_DoorShut(Pursuer *p, s32 door);
extern s32 Npc_DoorShut2(Pursuer *p, s32 door);
extern s32 Npc_ExitSideBehind(Pursuer *p);
extern s32 NPC_HearNoise(Pursuer *p);
extern u32 NPC_BlockFlags(Pursuer *p);
extern u32 Npc_TriIfStandable(Pursuer *p, u32 tri);
extern void NPC_ExitArg(Pursuer *p, s32 a2);
extern void Npc_DoorShutOther(Pursuer *p, u32 door);
extern void Npc_DoorRelease(Pursuer *p, u32 door);
extern s32 Npc_PlanBesideDoor(Pursuer *p, u32 door, u32 side);
extern s32 Npc_PlanToGoal(Pursuer *p);
extern void Npc_BoneHeight(Pursuer *p);
extern s32 Npc_TriBlocked(Pursuer *p, u32 tri);
extern s32 Npc_PlanToRoomObject(Pursuer *p);
extern s32 Npc_ReachedRoom(Pursuer *p);
extern s32 Npc_CharSideBehind(Pursuer *p, Character *c);
extern s32 Npc_InPlayedRoom(Pursuer *p);
extern s32 Npc_FionaPanicking(void);
extern s32 Npc_SameFloor(Actor *a, Actor *b);
extern s32 Npc_WalkableAhead(Pursuer *p);
extern s32 Npc_RouteAdd(Pursuer *p, u32 tri);
extern void Npc_RouteAim(Pursuer *p);
extern void Pursuer_DoorOffset(Pursuer *p, s32 side, f32 *out);
extern void Pursuer_StandAnim(Pursuer *p);
extern f32 Npc_NodeDistance(Pursuer *p, s32 room, u32 a, u32 b);
extern s32 Npc_AtSpawn(Pursuer *p, s32 room);
extern s32 Npc_OpenDoorFionaHides(void);
extern f32 NPC_PathLengthTo(Pursuer *p, u32 tri, const f32 *pos);
extern f32 Npc_FootDistance(Pursuer *p, Character *c);
extern u32 Npc_TurnWay(Pursuer *p, f32 heading, f32 a, f32 b);
extern f32 Npc_TurnToward(Pursuer *p, f32 heading, f32 step);
extern s32 Npc_StepPath(Pursuer *p, u32 *triOut, f32 *posOut, f32 step);
extern f32 Npc_PathLength(Pursuer *p, u32 tri, const f32 *pos);
extern s32 Npc_SameRoomOtherSide(Pursuer *p, Character *c);
extern s32 Npc_NearRoom(Pursuer *p, s32 slot);
extern s32 Npc_WhoSeen(Pursuer *p);
extern s32 Npc_TargetSideWalkable(Pursuer *p, f32 angle, f32 dist);
extern void Npc_ProbeAroundFiona(Pursuer *p);
extern s32 NPC_HewieInReach(Pursuer *p);
extern s32 Npc_RouteDrop(Pursuer *p);
extern void NPC_HeadNearHewie(Pursuer *p, u32 exit);
extern void NPC_HeadNearFiona(Pursuer *p, u32 exit);
extern s32 Npc_ExitWhatToDo(Pursuer *p, s32 exit);
extern f32 Npc_RoomNodeDistance(Pursuer *p, s32 room, s32 a, s32 b);
extern void Npc_FionaAtSpawn(Pursuer *p, s32 room);
extern f32 Npc_ExitHeading(Pursuer *p, s32 exit);
extern void Npc_TurnToRootMotion(Pursuer *p, u32 mask);
extern u32 Npc_TurnWayTo(Pursuer *p, const f32 *pos, f32 a, f32 b);
extern s32 Eye_CanSee(Pursuer *p, const f32 *from, const f32 *to, f32 heading, f32 range, f32 half);
extern s32 Eye_ActorSees(Pursuer *p, Actor *from, Actor *to, f32 heading, f32 range, f32 half);
extern s32 NPC_FionaInReach(Pursuer *p);
extern Pursuer *NPC_dtor(Pursuer *p, s32 flags);
extern void NPC_Reset(Pursuer *p);
extern s32 Npc_HeadRandomRoom(Pursuer *p);
extern u32 Npc_PlanFromDoor(Pursuer *p, u32 tri, const f32 *pos, u32 door);
extern void NPC_GoTo(Pursuer *p, u32 tri, const f32 *pos, s32 room);
extern u32 Npc_RandomTri(Pursuer *p);
extern s32 Npc_CanWalkStraight(Pursuer *p, const f32 *pos);
extern void Npc_WhoAround(Pursuer *p);
extern s32 Npc_RoomToSide(Pursuer *p, f32 dist);
extern s32 NPC_PathLengthSpot(Pursuer *p);
extern s32 NPC_PathLengthGoal(Pursuer *p);
extern s32 Npc_SeesPoint(Pursuer *p, u32 tri, const f32 *pos);
extern u32 Npc_ExitKind(Pursuer *p, s32 exit);
extern u32 Npc_TriAtDirection(Pursuer *p, f32 heading, f32 dist);
extern s32 Npc_WhoReachable(Pursuer *p, s32 a1, f32 f);
extern s32 Npc_WhoReachableBits(Pursuer *p, s32 a1, f32 f);
extern s32 Npc_PlanToDoor(Pursuer *p, u32 door, s32 tri, const f32 *pos);
extern s32 Npc_FindDoor(Pursuer *p, s32 side);
extern s32 NPC_PathLengthChar(Pursuer *p, Character *c);
extern f32 Npc_DoorFacingFromFiona(Pursuer *p, u32 exit);
extern void NPC_HeadForFiona(Pursuer *p);
extern u32 Npc_RandomExit(Pursuer *p, u32 skip);
extern u32 Npc_DoorOnWay(Pursuer *p);
extern void NPC_HeadFor(Pursuer *p, Character *c);
extern f32 Npc_TurnTowardPos(Pursuer *p, const f32 *pos, f32 step);
extern s32 Npc_StepToward(Pursuer *p, const f32 *pos);
extern void Npc_Senses2(Pursuer *p);
extern void Npc_Senses2Ending(Pursuer *p);
extern s32 Npc_ExitsToTri(Pursuer *p, s32 tri);
extern void Npc_LeaveDoor(Pursuer *p, u32 door);
extern s32 Npc_RoundThroughDoor(Pursuer *p, s32 tri);
extern u32 Npc_NearestWalkable(Pursuer *p, u32 tri, const f32 *pos, f32 *out);
extern s32 Npc_SeesChar(Pursuer *p, Character *c);
extern s32 Npc_SensesFiona(Pursuer *p);
extern s32 NPC_CanUseExit(Pursuer *p, s32 exit);

#endif
