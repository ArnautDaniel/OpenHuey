/* Pursuer: the stalkers' shared base class, code 0x278490..0x29FF10. See include/pursuer.h. */
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
#include "item.h"
#include "texcache.h"
#include "charaction.h"
#include "input.h"
#include "gl2d.h"
#include "daniella.h"
#include "debilitas2.h"
#include "hewie.h"
#include "lorenzo.h"
#include "model.h"
#include "panic.h"
#include "pursuer_ai.h"
#include "runtime.h"
#include "scene_game_members.h"
#include "skeleton.h"
#include "stalker_math.h"
#include "stalker_models.h"
#include "stalker_progress.h"
#include "msl.h"

extern u8 D_003EC3E0[]; /* table of 28-byte entries */
extern u8 D_003AF1F0[];
extern u8 D_003AF230[];
#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

s32 Character_TimerDown(void *p, s32 n);
void Pursuer_StartActionNext(void *p, u32 id);
void Pursuer_Caught(void *p);
void Pursuer_NoGiveUp(void *p);
f32 Pursuer_FrightSeen(void);
f32 Pursuer_FrightAttack(void);
f32 Pursuer_ThreatAmount(void);
void Pursuer_SetRage(void *p, s32 on);
s32 Pursuer_IsBusy(void *p);

extern void *D_00477790[];
extern void *D_00476C10[];
extern void *D_004767D0[];
extern void *D_004764A0[];
extern void *D_00476120[];
extern void *D_00475DB0[];
extern void *D_00474C10[];
extern void *D_00474130[];
extern void *D_00473CD0[];
extern void *D_00478770[];
extern void *D_004738A0[];
extern void *D_0046A620[];
extern void *D_00474560[];
extern void *D_00474890[];
extern void *D_004734A0[];
extern void *D_00473110[];
extern void *D_00472C30[];
extern void *D_004728A0[];
extern void *D_0046A2F0[];
extern void *D_004723B0[];
extern void *D_00472020[];
extern void *D_0046FF80[];
extern void *D_004718F0[];
extern void *D_00470720[];
extern void *D_00479B20[];
extern void *D_004715C0[];
extern void *D_00471290[];
extern void *D_0046F6B0[];
extern void *D_00478160[];
extern void *D_00477E30[];
extern void *D_00477AE0[];
extern void *D_0046BBB0[];
extern void *D_00474FD0[];
extern void *D_0046F020[];
extern void *D_00470A90[];
extern void *D_00469D10[];
extern u8 D_00419DD0[];
extern u8 D_00419E10[];
extern u8 D_004297C0[];
extern u8 D_00429800[];
extern u8 D_0042C870[];
extern u8 D_0042C8B0[];
void *Kind14_ModelFiles(void);
void *Kind14_MotionFiles(void);
void *Kind21_ModelFiles(void);
void *Kind21_MotionFiles(void);
void *func_002ECC90(void);
void *func_002ECCA0(void);

extern u8 D_00413530[], D_004134F0[], D_00413510[], D_004134D0[];
#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

void *Debilitas3_ModelFiles(void);

extern u8 D_00414820[], D_004147E0[];

extern u8 D_00422380[];
extern u8 D_004223C0[];
#define U32(p, off) (*(u32 *)((u8 *)(p) + (off)))

extern u8 D_00422460[];
extern u8 D_004224A0[];
static inline s32 b5_prog_flag8000(void);

extern u8 D_00429C50[];
void *Kind16_ModelFiles(void);

extern u8 D_00429C90[];
extern u8 D_00429CE0[];
extern u8 D_00429D20[];
extern u8 D_0042A2E0[];
extern u8 D_0042A320[];
void *Kind16_MotionFiles(void);
void *Kind17_ModelFiles(void);
void *Kind17_MotionFiles(void);
static inline s32 b5_prog_flag8000(void);

extern void *D_0046D730[];
extern void *D_0046F580[];
extern void *D_00472F60[];
extern void *D_00479FF0[];
void *func_00316D80(u8 *o, s32 flags);
void *func_0036A4D0(u8 *o, s32 flags);

extern const char *const D_0042C358;
extern const PTMF D_0042C8F0;
extern u8 D_0042E460[];
extern u8 D_0042E4A0[];
static inline s32 b5_prog_flag8000(void);

extern u8 D_0043B6C0[], D_0043B680[];
extern u8 D_0043CDA0[], D_0043CD60[];

extern u8 D_0043DC30[], D_0043DBF0[];

extern u8 D_00441830[], D_004417F0[];

void func_0036A580(void);

extern const PTMF D_00422348;            /* a creature state */
extern void *D_004726E0[];   /* the strand (creature.c) */
extern u8 D_00422360[];
extern u8 D_004223A0[];
extern u8 D_004223E0[];
void *Kind09_MotionFiles(void);
void *Kind09_ModelFiles(void);
static inline s32 b5_prog_flag8000(void);

extern u8 D_00429730[];
extern u8 D_00429770[];
void *Kind13_ModelFiles(void);
void *Kind13_MotionFiles(void);

extern u8 D_0042A0F0[];
extern u8 D_0042A130[];
void *Kind19_ModelFiles(void);
void *Kind19_MotionFiles(void);

extern u8 D_0042C380[];
extern u8 D_0042C3C0[];
extern u8 D_0042C900[];
extern u8 D_0042C940[];
void *Kind24_ModelFiles(void);
void *Kind24_MotionFiles(void);
void *Kind20_ModelFiles(void);
void *Kind20_MotionFiles(void);

extern u8 D_00430820[];
extern u8 D_00430860[];
void *Kind29_ModelFiles(void);
void *Kind29_MotionFiles(void);

extern u32 D_0043B5B0[];
extern u32 D_0043B5F0[];
void *Kind33_ModelFiles(void);
void *Kind33_MotionFiles(void);

extern u32 D_00441850[];
void *Kind37_MotionFiles(void);
void Kind37_DoorOffset(void *self, s32 i, f32 *out);
void Kind37_ActionOffsets(void *self, s32 i, f32 *out);

extern const PTMF D_0042A170;
extern const PTMF D_0042C400;
extern const PTMF D_0042C980;
extern const PTMF D_004308A0;
extern const PTMF D_0043B630;
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
            func_00124E40(&c->a);
        }
    }
    return c;
}

Character *Kind13_dtor(Character *c, s32 flags);
Character *Kind19_dtor(Character *c, s32 flags);
void Kind19_ShowUp(Pursuer *p);
void Kind19_EventState(Pursuer *p);
s32 Kind19_GrabOrder(Pursuer *p);
Character *Kind24_dtor(Character *c, s32 flags);
void Kind24_ShowUp(Pursuer *p);
void Kind24_EventState(Pursuer *p);
s32 Kind24_GrabOrder(Pursuer *p);
Character *Kind20_dtor(Character *c, s32 flags);
void Kind20_ShowUp(Pursuer *p);
void Kind20_EventState(Pursuer *p);
s32 Kind20_GrabOrder(Pursuer *p);
Character *Kind29_dtor(Character *c, s32 flags);
void Kind29_ShowUp(Pursuer *p);
void Kind29_EventState(Pursuer *p);
s32 Kind29_GrabOrder(Pursuer *p);
Character *Kind33_dtor(Character *c, s32 flags);
void Kind33_ShowUp(Pursuer *p);
void Kind33_EventState(Pursuer *p);
s32 Kind33_GrabOrder(Pursuer *p);
Character *Kind37_dtor(Character *c, s32 flags);
static inline __attribute__((always_inline)) s32 creature_slot_done(Pursuer *p);
static inline __attribute__((always_inline)) void creature_act5(Pursuer *p, const PTMF *st);
static inline __attribute__((always_inline)) void creature_inplay(Pursuer *p);

static void strand_init(void **obj) {
    obj[0] = D_004726E0;
    obj[0x40 / 4] = D_00469D00;
    ((s32 *)obj)[0x44 / 4] = -1;
    obj[0x40 / 4] = D_0046FC30;
}

Character *Kind09_dtor(Character *c, s32 flags);
void Kind09_Behaviour25C(Character *c);
void Kind09_BehaviourSearch(Character *c);
void func_0036A530(u8 *o, s32 *m);
void func_0036A7F0(u8 *o);
s32 func_0036A590(u8 *o);

/* in play: func_00124890(-1) */
static inline __attribute__((always_inline)) void creature_inplay(Pursuer *p) {
    if (func_00217510(p) != 0) {
        func_00124890(&p->c.a, -1);
    }
}

/* the action 5 taken (+0x14E8): in play +0x8C, the state st, +0x114 1; the action cleared */
static inline __attribute__((always_inline)) void creature_act5(Pursuer *p, const PTMF *st) {
    if (PU(p, 0x14E8, s32) != 5) {
        return;
    }
    if ((u8)func_00217510(p) != 0) {
        VCALL(p, 0x8C, void (*)(Pursuer *))(p);
        ptmf_set(&PU(p, 0x174C, PTMF), st);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
    }
    PU(p, 0x14E8, s32) = 0;
    PU(p, 0x14EC, s32) = 0;
}

/* its slot's progress entry (func_00177870) 1: SlotCmd_Cancel; -1 */
static inline __attribute__((always_inline)) s32 creature_slot_done(Pursuer *p) {
    Progress *g = gProgress;

    if ((u8)func_00177870(g, *(u8 *)&p->c.a.slot) == 1) {
        SlotCmd_Cancel(g, *(u8 *)&p->c.a.slot);
    }
    return -1;
}

void Kind21_ShowUp(Pursuer *p);
void Kind21_EventState(Pursuer *p);
s32 Kind21_GrabOrder(Pursuer *p);

/* gProgress+0x30 bit 0x8000 selects between two data sets (difficulty/mode flag?) */
static inline s32 b5_prog_flag8000(void) {
    return U32(gProgress, 0x30) & 0x8000;
}

static inline void *b0_RoomCtor(void *p, u32 id, s32 arg, void **vtbl) {
    FLD(p, 0x0, void **) = D_00469C20;
    FLD(p, 0x20, s32) = arg;
    FLD(p, 0x24, s32) = 0x2000000;
    FLD(p, 0x0, void **) = D_00469C60;
    FLD(p, 0x1380, s32) = 0;
    FLD(p, 0x153C, u8) = (u8)id;
    FLD(p, 0x0, void **) = vtbl;
    return p;
}

/* Count down the timer at +0x14C8 by |n|; 1 (and clamp to 0) when it runs out. */
/* 0x00124ED0 */
s32 Character_TimerDown(void *p, s32 n) {
    if (n <= 0) {
        n = -n;
    }
    FLD(p, 0x14C8, s32) -= n;
    if (FLD(p, 0x14C8, s32) > 0) {
        return 0;
    }
    FLD(p, 0x14C8, s32) = 0;
    return 1;
}
/* nothing */
/* 0x00283EE0 */
void Pursuer_AttackTable(Pursuer *p) {
}

/* a character's step (base) */
void func_002992F0(Pursuer *p) {
    func_00125960(&p->c);
}

/* chasing Hewie? */
s32 func_0029A850(Pursuer *p) {
    return p->target == gCharPartner;
}

/* stop the pursuer's sounds (sound driver +0x10) */
/* 0x0029D3E0 */
void Pursuer_StopSounds(Pursuer *p) {
    VCALL(gSound, 0x10, void (*)(VObject *, s32, s32))(gSound, 0, 0x400002);
}

/* clear a 3-word vector (third argument) */
/* 0x0029CE40 */
void Pursuer_ActionOffsets(Pursuer *p, s32 a1, s32 *out) {
    out[0] = 0;
    out[1] = 0;
    out[2] = 0;
}

/* vtable +0x58: deactivate (Character part) */
/* 0x0029E600 */
void Pursuer_Deactivate(Pursuer *p) {
    Character_Deactivate(&p->c);
}

/* vtable +0x24 */
/* 0x0029EAA0 */
void Pursuer_RememberPos(Pursuer *p) {
    Character_RememberPos(&p->c);
}

/* vtable +0x10: nothing */
/* 0x0029EE00 */
void Pursuer_Cleanup(Pursuer *p) {
}

/* vtable +0x1C4 */
/* 0x0028ED20 */
void Pursuer_Door1C4(Pursuer *p) {
    PU(p, 0x16EE, u8) = 1;
    PU(p, 0x16F0, u8) = 1;
    p->c.unk100 = -1;
    p->c.unk104[0] = -1;
    p->c.a.unk2B = 0;
    p->c.a.unk2A = 0;
}

/* ---- batch 2 ---- */

/* nav triangle +0x179C, if valid */
void func_0027E5A0(Pursuer *p, u32 tri) {
    if (tri < AT(gNavMesh, 0x8, u32)) {
        PU(p, 0x179C, u32) = tri;
    }
}

/* clear the step counters */
void func_0029A6D0(Pursuer *p) {
    PU(p, 0x1624, s32) = 0;
    PU(p, 0x1628, s32) = 0;
    PU(p, 0x162C, s32) = 0;
    PU(p, 0x1630, s32) = 0;
    PU(p, 0x1634, s32) = 0;
    PU(p, 0x1638, s32) = 0;
    PU(p, 0x163C, s32) = 0;
    PU(p, 0x1640, s32) = 0;
    PU(p, 0x1650, s32) = 0;
    PU(p, 0x1654, s32) = 0;
    PU(p, 0x1658, s32) = 0;
    PU(p, 0x165C, s32) = 0;
}

/* vtable +0x2C4: timer +0x1660 to 900 frames (15 s), 1 when the progress byte +0x1FBEC1 is set */
/* 0x0027E560 */
void Pursuer_Timer15s(Pursuer *p) {
    PU(p, 0x1660, s32) = AT(gProgress, 0x1FBEC1, u8) != 0 ? 1 : 900;
}

/* vtable +0x324 / +0x320: animation ids by stance (+0xC4) and +0x16B8 */
/* 0x00297AC0 */
s32 Pursuer_StanceAnim(Pursuer *p) {
    if (p->c.a.unkC4 != 1) {
        return PU(p, 0x16B8, s32) == 2 ? 0x204 : 0x200;
    }
    return 0x202;
}

/* 0x00297B00 */
s32 Pursuer_WalkAnim(Pursuer *p) {
    if (p->c.a.unkC4 != 1) {
        return PU(p, 0x16B8, s32) == 2 ? 3 : 0;
    }
    return 2;
}

/* vtable +0x328 */
/* 0x00297A70 */
s32 Pursuer_SlowWalkAnim(Pursuer *p) {
    if (p->c.a.unkC4 != 1) {
        if (PU(p, 0x16B8, s32) != 2) {
            return PU(p, 0x16C8, u8) != 0 ? 0x201 : 0x206;
        }
        return 0x205;
    }
    return 0x203;
}

/* stop when the animation has run out (+0x550 time left) */
void func_00292120(Pursuer *p) {
    if (p->c.unkE0 != 0) {
        s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

        if (!((over ^ 1) & 0xFF)) {
            p->c.unkE1 = 1;
        }
    }
}

/* vtable +0x240: step, count +0x104 down */
/* 0x00289810 */
void Pursuer_StepCount(Pursuer *p) {
    func_00125A10(&p->c);
    p->c.unk104[0]--;
    if (p->c.unk104[0] <= 0) {
        PURSUER_STEP_DONE(p) = 1;
    }
}

/* step until the animation ends */
void func_00289F50(Pursuer *p) {
    func_00125A10(&p->c);
    if (MOTION_KEYS(p) & MOTION_KEY_END) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    }
}

/* vtable +0x10C: in stance 2 playing animation 0x1805 / 0x1806 */
/* 0x0029A870 */
s32 Pursuer_InStance2Anim(Pursuer *p) {
    if (p->c.a.unkC4 == 2) {
        s32 anim = MOTION_ANIM(p);

        if (anim != 0x1806 && anim != 0x1805) {
            return 0;
        }
        return 1;
    }
    return 0;
}

void func_00291C30(Pursuer *p) {
    if ((MOTION_KEYS(p) & MOTION_KEY_END) != 0) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    } else {
        func_00125A10(&p->c);
    }
}

void func_00285AB0(Pursuer *p) {
    if (MOTION_KEYS(p) & 0x400) {
        PU(p, 0x1624, s32)--;
        if (PU(p, 0x1624, s32) <= 0) {
            PURSUER_STEP_DONE(p) = 1;
            PURSUER_STEP_NEXT(p) = 1;
            p->c.unk104[0] = -1;
            PU(p, 0x1628, s32) = 0;
            PU(p, 0x1624, s32) = 0;
        }
    }
    func_00125A10(&p->c);
}

/* vtable +0x4C: disable, animation paused, its message taken down */
/* 0x002990E0 */
void Pursuer_Disable(Pursuer *p) {
    Character_Disable(&p->c);
    MOTION_AT(p, 0x4D8, u8) = 1;
    if (PU(p, 0x1688, s32) != 0) {
        VCALL(gBootMessage, 0x10, void (*)(VObject *, u32, s32, s32))(gBootMessage, p->c.msgSlot, PU(p, 0x1688, s32), 0);
    }
}

/* vtable +0x150 */
/* 0x002801B0 */
void Pursuer_EventOver2(Pursuer *p) {
    f32 pos[4] __attribute__((aligned(16)));

    if (func_00122B50(&p->c.a, pos) != 0) {
        ((void (*)(Character *, s32, f32 *))func_001264C0)(&p->c, 0, pos);
        ((void (*)(Character *, s32, f32 *))func_001264C0)(&p->c, 5, pos);
    }
}

/* vtable +0x50: enable, animation running, its message released */
/* 0x00299080 */
void Pursuer_Enable(Pursuer *p) {
    Character_Enable(&p->c);
    MOTION_AT(p, 0x4D8, u8) = 0;
    if (PU(p, 0x1688, s32) != 0) {
        VCALL(gBootMessage, 0x14, void (*)(VObject *, u32))(gBootMessage, p->c.msgSlot);
        PU(p, 0x1688, s32) = 0;
    }
}

/* vtable +0x54: load the message image (file name at (+0x168C)->+0x1C) */
/* 0x0029EE10 */
s32 Pursuer_LoadMessage(Pursuer *p) {
    char *name = AT(PU(p, 0x168C, u8 *), 0x1C, char *);

    if (*name == 0) {
        return 0;
    }
    VCALL(gFileLoader, 0xC, void (*)(VObject *, char *, s32, u32, s32))(gFileLoader, name, PU(p, 0x1688, s32), p->c.a.flags24 | p->c.a.slot, 0);
    return 1;
}

extern void *D_0046D800[];
extern void *D_00469D00[];

/* destructor of a small object (vtables 0x46D800 -> 0x469D00) */
void **func_00278490(void **obj, s32 flags) {
    if (obj != NULL) {
        *obj = D_0046D800;
        if (obj != NULL) {
            *obj = D_00469D00;
        }
        if ((s16)flags > 0) {
            func_00100490(obj);
        }
    }
    return obj;
}

/* the entry of a {threshold, value} table (8 bytes each) whose threshold +0x1588 reaches */
s32 func_00297290(Pursuer *p, f32 *table, u32 n) {
    u32 i;

    for (i = 0; i < n; i++) {
        s32 below = PU(p, 0x1588, f32) <= table[i * 2];

        if ((below ^ 1) == 0) {
            return ((s32 *)table)[i * 2 + 1];
        }
    }
    return 0;
}

/* run the state, stopping on reaching the triangle +0x104 */
void func_00299300(Pursuer *p) {
    if (ptmf_test(&p->c.a.state)) {
        ptmf_scall(p, &p->c.a.state);
    }
    if ((s32)p->c.a.navTri == p->c.unk104[0]) {
        p->c.unkE1 = 1;
    }
}

/* vtable +0x178: count +0x1624 down while the room's flag +0x17B0 isn't set */
/* 0x0027CE80 */
void Pursuer_WaitRoomFlag(Pursuer *p) {
    if (Progress_CurRoomFlag(gProgress, p->c.a.room, PU(p, 0x17B0, u8)) != 0) {
        PU(p, 0x1624, s32) = 0;
    }
    if (PU(p, 0x1624, s32) > 0) {
        PU(p, 0x1624, s32)--;
    } else {
        PURSUER_STEP_DONE(p) = 1;
    }
}

/* may the pursuer go for its target? */
s32 func_00283870(Pursuer *p) {
    if (p->target->a.unk2D == 1) {
        return 0;
    }
    if ((Progress_TestFlag(gProgress, 0xE) & 0xFF) == 1) {
        return 0;
    }
    return gCharPlayer->moveSub != 0x10;
}

/* vtable +0x1FC: through the door +0x100 once it's open */
/* 0x0028D540 */
void Pursuer_DoorThroughOpen(Pursuer *p) {
    if (VCALL(gDoors, 0x30, s32 (*)(VObject *, u32))(gDoors, (u8)p->c.unk100) != 0) {
        func_00212CA0(p, (u8)p->c.unk100);
        p->c.moveMode = 0;
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
        p->c.a.disabled = 0;
        p->c.unk100 = -1;
    }
}

/* vtable +0x1E0: reset to idle */
/* 0x0028DBD0 */
void Pursuer_DoorIdle(Pursuer *p) {
    PU(p, 0x1624, s32) = 0;
    PURSUER_STEP_DONE(p) = 1;
    PURSUER_STEP_NEXT(p) = 1;
    p->c.unk100 = -1;
    p->c.unk104[0] = -1;
    p->c.a.unk2D = 0;
    p->c.a.unk2B = 0;
    p->c.a.unk2A = 0;
    p->c.a.navMask = VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
    p->c.pathReq->mask = p->c.a.navMask;
    func_002177D0(p);
}

/* vtable +0x180 */
/* 0x002919B0 */
void Pursuer_Move180(Pursuer *p) {
    s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

    if (((over ^ 1) & 0xFF) == 1) {
        func_00125A10(&p->c);
    } else if (p->c.unkE0 != 0) {
        p->c.unkE1 = 1;
    }
}

/* vtable +0x170 */
/* 0x0027D130 */
void Pursuer_Plan170(Pursuer *p) {
    if (func_00217510(p) == 0) {
        if (PU(p, 0x17B4, s32) == 0 && PU(p, 0x1664, s32) == 0 && (func_00212A80(p, 0xFF) & 0xFF) != 0xFF) {
            PURSUER_STEP_DONE(p) = 1;
        }
    } else {
        PU(p, 0x16ED, u8) = 1;
    }
}

/* vtable +0x208 */
/* 0x0028D040 */
void Pursuer_Door208(Pursuer *p) {
    PU(p, 0x1634, s32) = 0;
    p->c.a.unk2B = 0;
    if (VCALL(gRooms, 0x70, s32 (*)(VObject *, s32, u32))(gRooms, p->c.a.room, (u8)p->c.unk100) & 0xFF) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    } else {
        func_002815E0(p, (u8)p->c.unk100);
    }
    p->c.unk100 = -1;
}

/* is `room` (-1: the current one) in the vtable +0x314 list ({room, ...} 8 bytes each, -1 ends)? */
s32 func_0029A8C0(Pursuer *p, s32 room) {
    s32 *e;

    if (room == -1) {
        room = p->c.a.room;
    }
    e = VCALL(p, 0x314, s32 *(*)(Pursuer *))(p);
    for (; *e != -1; e += 2) {
        if (room == *e) {
            return 1;
        }
    }
    return 0;
}

/* vtable +0x28: placement, turned to the animation's heading */
/* 0x0029ED80 */
s32 Pursuer_Place(Pursuer *p, u32 tri, const f32 *heading, f32 *pos) {
    s32 r = Character_Place(&p->c, tri, heading, pos);
    u8 *m;

    MOTION_AT(p, 0x854, s32) = 0;
    MOTION_AT(p, 0x858, f32) = 0.0f;
    PU(p, 0x1574, f32) = func_002E2D00(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
    m = p->c.motion;
    VCALL(m, 0x54, void (*)(void *))(m);
    MOTION_AT(p, 0x850, u8) = 1;
    return r;
}

/* run the state; then, a step done, the next behaviour (+0x114) */
void func_002994B0(Pursuer *p) {
    if (ptmf_test(&p->c.a.state)) {
        ptmf_scall(p, &p->c.a.state);
    }
    if (PURSUER_STEP_DONE(p) == 1) {
        PURSUER_STEP_DONE(p) = 0;
        if (PU(p, 0x175C, s32) != 0) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0);
        }
    }
}

/* vtable +0x2C: room light change */
/* 0x0029ED00 */
void Pursuer_LightChange(Pursuer *p) {
    u8 *m;

    if (p->c.a.disabled == 1) {
        return;
    }
    if (p->c.unkE4 == 1 && p->c.unk152C != 0x11 && p->c.unk152C != 0x14) {
        VCALL(p, 0x80, void (*)(Pursuer *))(p);
    }
    m = p->c.motion;
    VCALL(m, 0x38, void (*)(void *, s32, u32, s32))(m, p->c.unk152C, p->c.a.navTri, 0);
}

/* vtable +0x78: left behind in another room while in behaviour 0x23 */
/* 0x00298FF0 */
void Pursuer_LeftBehind(Pursuer *p) {
    s32 room = p->c.a.room;

    if (room != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) && PU(p, 0x175C, s32) == 0x23) {
        PU(p, 0x1664, s32) = 0;
        VCALL(p, 0x2A8, void (*)(Pursuer *))(p);
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
    }
}

/* release the model files (loader +0x18; the sound bank 7 too for slot 2) */
void func_0029F2C0(Pursuer *p) {
    if (p->c.a.slot != 2) {
        VCALL(gFileLoader, 0x18, void (*)(VObject *, u32))(gFileLoader, p->c.a.flags24 | p->c.a.slot);
        return;
    }
    VCALL(gSound, 0x88, void (*)(VObject *, s32))(gSound, 7);
    VCALL(gFileLoader, 0x18, void (*)(VObject *, u32))(gFileLoader, p->c.a.flags24 | p->c.a.slot);
}

/* vtable +0x18 */
/* 0x0029F350 */
void Pursuer_FilesLoading(Pursuer *p) {
    if (p->c.a.slot != 2) {
        VCALL(gFileLoader, 0x14, void (*)(VObject *, u32))(gFileLoader, p->c.a.flags24 | p->c.a.slot);
        return;
    }
    VCALL(gSound, 0x84, void (*)(VObject *, s32))(gSound, 7);
    VCALL(gFileLoader, 0x14, void (*)(VObject *, u32))(gFileLoader, p->c.a.flags24 | p->c.a.slot);
}

extern const PTMF D_003ED0D0, D_003ED050, D_003ED0E0, D_003ED060, D_003ED530, D_003ED5A0, D_003ECF20,
    D_003ED070, D_003ED0F0, D_003ECF80, D_003ECE80, D_003ED590, D_003ED080, D_003ECF30, D_003ECD30,
    D_003ED210, D_003ECF40, D_003ECCB0, D_003ECC70, D_003ECB90;

void func_0028A100(Pursuer *p) {
    Relation_Request(gProgress, *(u8 *)&p->c.a.slot, 1, 9, 0, 8, 0.0f);
    Actor_SetState(&p->c.a, &D_003ED0D0);
}

void func_0028AA80(Pursuer *p) {
    Relation_Request(gProgress, *(u8 *)&p->c.a.slot, 1, 9, 0, 6, 0.0f);
    Actor_SetState(&p->c.a, &D_003ED050);
}

void func_0028A060(Pursuer *p) {
    if (p->c.state[0] != 7) {
        p->c.state[0] = 0;
        Actor_SetState(&p->c.a, &D_003ED0E0);
        return;
    }
    p->c.state[0] = 0;
    VCALL(p, 0x134, void (*)(Pursuer *))(p);
}

void func_0028A9E0(Pursuer *p) {
    if (p->c.state[0] != 7) {
        p->c.state[0] = 0;
        Actor_SetState(&p->c.a, &D_003ED060);
        return;
    }
    p->c.state[0] = 0;
    VCALL(p, 0x134, void (*)(Pursuer *))(p);
}

/* +0x16E0 plus a random 0..3600 frames */
s32 func_0029CE50(Pursuer *p) {
    f32 r = 3600.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);

    return PU(p, 0x16E0, s32) + (u32)r;
}

/* back on its feet: at least 1 hp, chase state reset */
void func_002860D0(Pursuer *p) {
    if (p->c.hp <= 0) {
        p->c.hp = 1;
    }
    if (gCharPartner->a.unkC4 == 2) {
        func_00297160(p);
    }
    PU(p, 0x1760, u8) = 0;
    p->c.unk104[0] = -1;
    PU(p, 0x1770, s32) = 0;
    PU(p, 0x1774, s32) = 0;
    PU(p, 0x1778, s32) = 0;
    PU(p, 0x1624, s32) = 0;
    PU(p, 0x1628, s32) = 0;
    PU(p, 0x1634, s32) = 0;
    PU(p, 0x1638, s32) = 0;
    PURSUER_STEP_DONE(p) = 1;
    PURSUER_STEP_NEXT(p) = 1;
}

/* vtable +0x20: unload: message, model, sounds */
/* 0x0029EC60 */
void Pursuer_Unload(Pursuer *p) {
    if (p->c.a.unkD0 != 0) {
        VCALL(gBootMessage, 0xC, void (*)(VObject *, u32))(gBootMessage, p->c.msgSlot);
        p->c.a.unkD0 = 0;
    }
    if (p->c.a.unkD1 != 0) {
        u8 *m = p->c.motion;

        VCALL(m, 0x10, void (*)(void *))(m);
        p->c.a.unkD1 = 0;
    }
    VCALL(gSound, 0x10, void (*)(VObject *, s32, s32))(gSound, 0, 0x40F002);
}

/* vtable +0x2A4: behaviour state cleared */
/* 0x0027DFA0 */
void Pursuer_ClearBehaviour(Pursuer *p) {
    PU(p, 0x16F6, u8) = 0;
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED530);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x2A8, void (*)(Pursuer *))(p);
}

/* wait for animation 0x1903 to end, then hold for (+0x1748)[0] frames (90 without) */
void func_0028A660(Pursuer *p) {
    func_00125A10(&p->c);
    if ((MOTION_KEYS(p) & MOTION_KEY_END) != 0 && MOTION_ANIM(p) == 0x1903) {
        s32 *t = PU(p, 0x1748, s32 *);

        PU(p, 0x1760, u8) = 1;
        PU(p, 0x178C, s32) = t != NULL ? *t : 90;
        PURSUER_STEP_NEXT(p) = 1;
        p->c.unk104[0] = -1;
        PURSUER_STEP_DONE(p) = 1;
    }
}

/* vtable +0x108: sound id of the current path node (move 8 / 0x1A..0x1C), -1 none */
/* 0x0029CB40 */
u32 Pursuer_PathNodeSound(Pursuer *p) {
    if (p->c.moveMode == 8) {
        s32 sub = p->c.moveSub;

        if (sub == 0x1A || sub == 0x1B || sub == 0x1C) {
            s32 i = PU(p, 0x1728, s32);

            if (i >= 0) {
                s8 k = PU(p, 0x172C, s8);

                if (k < 4) {
                    s8 n = PU(p, 0x1724, s8 *)[i * 4 + k];

                    return AT(PU(p, 0x171C, u8 *) + n * 0x24, 0x12, u16);
                }
            }
            return -1;
        }
        return -1;
    }
    return -1;
}

/* the positions of bones `bones`[1] and (if >= 0) `bones`[2] */
/* 0x00283A50 */
void Pursuer_BonePositions(Pursuer *p, s32 *bones, f32 *a, f32 *b) {
    sceVu0CopyVector(a, Skel_Bone(MOTION_AT(p, 0x810, u8 *), bones[1]) + 0xC);
    if (bones[2] >= 0) {
        sceVu0CopyVector(b, Skel_Bone(MOTION_AT(p, 0x810, u8 *), bones[2]) + 0xC);
    }
}

/* ---- batch 3 ---- */

extern void *D_003EC8E0;

/* the defaults of every pursuer (radius, height, hp 100, hearing, timers) */
/* 0x0029FB20 */
void Pursuer_Setup(Pursuer *p) {
    p->c.a.radius = 5.0f;
    p->c.a.height = 20.0f;
    p->c.hpMax = 100;
    p->c.hp = p->c.hpMax;
    p->c.hearThreshold = 12;
    PU(p, 0x16B4, u8) = 0;
    PU(p, 0x1714, s32) = 0;
    PU(p, 0x16AC, s32) = 0;
    PU(p, 0x16B0, s32) = 0;
    PU(p, 0x171C, s32) = 0;
    PU(p, 0x1720, s32) = 0;
    PU(p, 0x1724, s32) = 0;
    PU(p, 0x1734, s32) = 0;
    PU(p, 0x1730, s32) = 0;
    PU(p, 0x1740, s32) = 0;
    PU(p, 0x173C, s32) = 0;
    PU(p, 0x1744, void *) = &D_003EC8E0;
    PU(p, 0x1748, s32) = 0;
    PU(p, 0x1580, f32) = 150.0f;          /* sight range */
    PU(p, 0x1584, f32) = 0x1.0c15240000000p+0f /* 1.0471976 */;      /* 60 degrees */
    PU(p, 0x16DC, s32) = 100;
    PU(p, 0x16E8, f32) = 10.0f;
    PU(p, 0x16D4, s32) = 300;             /* frames */
    PU(p, 0x16D8, s32) = 1800;
    PU(p, 0x16D0, s32) = 1800;
    PU(p, 0x16E0, s32) = 9000;
    PU(p, 0x16E4, s32) = 150;
}

/* vtable +0x16C */
/* 0x0027D1B0 */
void Pursuer_Plan16C(Pursuer *p) {
    p->c.moveSub = p->c.unk128 < p->c.unk124 ? 0x16 : 0x17;
    p->c.unk1530 = 0;
    p->c.unk1538 = 0;
    p->c.unk1534 = 0;
    ptmf_set((PTMF *)((u8 *)p + 0x17A0), &D_003ED5A0);
    VCALL(p, 0x170, void (*)(Pursuer *))(p);
}

/* step while facing within 45 degrees of the target */
void func_00289500(Pursuer *p) {
    f32 h = func_001244D0(&p->c.a, p->target->a.pos);
    f32 d = func_002140A0(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));

    if (d <= 0.0f) {
        d = -d;
    }
    if (d < 0x1.921fb60000000p-1f /* 0.7853982 */) {
        func_00125A10(&p->c);
    }
}

/* vtable +0x214 */
/* 0x0028CD70 */
void Pursuer_Door214(Pursuer *p) {
    PU(p, 0x1634, s32) = 0;
    p->c.a.unk2B = 0;
    if (!(VCALL(gRooms, 0x70, s32 (*)(VObject *, s32, u32))(gRooms, p->c.a.room, (u8)p->c.unk100) & 0xFF)) {
        if (p->c.a.unkC4 == 2) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x20);
        }
        func_002815E0(p, (u8)p->c.unk100);
    } else {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x20);
    }
    p->c.unk100 = -1;
}

/* vtable +0x1EC: open the door the pursuer heads for, unless it already is */
/* 0x0028D8A0 */
void Pursuer_ExitOpen(Pursuer *p) {
    func_00214620(p, 0);
    if (!(PursuerGroup_Fields(gProgress, p->c.door, *(u8 *)&p->c.a.slot) & 0xFF & 1)) {
        p->c.unk100 = p->c.door;
        Actor_SetState(&p->c.a, &D_003ECF20);
        VCALL(p, 0x1AC, void (*)(Pursuer *))(p);
    }
}

/* a sound at the pursuer, unless a room object or progress flag 8 holds them */
void func_0029D410(Pursuer *p, s32 sound, s32 a2, s32 a3, s32 a4, void *a5) {
    if (VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(gProgress, 8) == 0) {
        func_00122C20(&p->c.a, sound, a2, a3, a4, a5);
    }
}

/* raise the threat level when someone it could reach is in sight */
void func_002837C0(Pursuer *p, u32 mask) {
    Progress *pr = gProgress;

    if (!(Progress_TestFlag(pr, 0xE) & 0xFF)) {
        u32 seen = func_00217920(p) & 0xFF & (mask & 0xFF);

        if (~PU(p, 0x1760, u8) & seen) {
            Relation_Request(pr, *(u8 *)&p->c.a.slot, seen & 0xFF, 4, 5, 0, 10.0f);
        }
    }
}

void func_0028A930(Pursuer *p) {
    if (p->c.state[0] != 7) {
        p->c.state[0] = 0;
        func_002DDD20(p->c.motion, 0x1900, -1);
        p->c.moveSub = 0x19;
        Actor_SetState(&p->c.a, &D_003ED070);
        return;
    }
    p->c.state[0] = 0;
    VCALL(p, 0x134, void (*)(Pursuer *))(p);
}

void func_00289FA0(Pursuer *p) {
    if (p->c.state[0] != 7) {
        func_002DDD20(p->c.motion, 0x1A01, -1);
        p->c.a.unk2D = 1;
        p->c.moveSub = 0x19;
        Actor_SetState(&p->c.a, &D_003ED0F0);
        return;
    }
    p->c.state[0] = 0;
    VCALL(p, 0x134, void (*)(Pursuer *))(p);
}

/* vtable +0x210: walk to the door */
/* 0x0028CF80 */
void Pursuer_DoorWalk(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    PU(p, 0x1634, f32) = func_00212550(p, (u8)p->c.unk100);
    if (VCALL(gDoors, 0x70, s32 (*)(VObject *, u32))(gDoors, (u8)p->c.unk100) != 0) {
        p->c.a.unk2B = 1;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECF80);
    VCALL(p, 0x218, void (*)(Pursuer *))(p);
}

/* vtable +0x1BC: the door animation over, through */
/* 0x0028EFC0 */
void Pursuer_DoorThrough(Pursuer *p) {
    s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

    if (((over ^ 1) & 0xFF) != 1) {
        VCALL(gDoors, 0xC, void (*)(VObject *, u32, s32, s32, s32))(gDoors, (u8)p->c.unk100, p->c.unk104[0], p->c.a.slot, 1);
        Actor_SetState(&p->c.a, &D_003ECE80);
    }
}

/* load the motion files (names at (+0x168C)->+0x0 / +0x8) */
void func_0029EF80(Pursuer *p, s32 id) {
    char *name;

    if (id == -1) {
        id = p->c.a.flags24 | p->c.a.slot;
    }
    name = AT(PU(p, 0x168C, u8 *), 0x0, char *);
    if (*name != 0) {
        VCALL(gFileLoader, 0xC, void (*)(VObject *, char *, s32, s32, s32))(gFileLoader, name, PU(p, 0x1670, s32), id, 0);
    }
    name = AT(PU(p, 0x168C, u8 *), 0x8, char *);
    if (*name != 0) {
        VCALL(gFileLoader, 0xC, void (*)(VObject *, char *, s32, s32, s32))(gFileLoader, name, PU(p, 0x1674, s32), id, 0);
    }
}

/* vtable +0x160: follow the planned path */
/* 0x0027D810 */
void Pursuer_FollowPlan(Pursuer *p) {
    PURSUER_STEP_NEXT(p) = 1;
    p->c.unk124 = p->c.unk128;
    *(f32 *)&p->c.unk14C4 = (f32)VCALL(gRooms, 0x38, s32 (*)(VObject *, u32, s32))(gRooms, PU(p, 0x138C, u16), p->c.a.room) + 150.0f;
    p->c.unk1530 = 0;
    p->c.unk1538 = 0;
    p->c.unk1534 = 0;
    ptmf_set((PTMF *)((u8 *)p + 0x17A0), &D_003ED590);
    PU(p, 0x1784, s32) = 0;
}

/* animation 0x1901 after the current one, and the threat level up by vtable +0x300 */
void func_0028A860(Pursuer *p) {
    u8 *m = p->c.motion;

    if (MOTION_KEYS(p) & MOTION_KEY_END) {
        func_002DDE20(m, 0x1901, -1);
        Actor_SetState(&p->c.a, &D_003ED080);
        Panic_Fright((u8 *)gProgress + 0x7B8, VCALL(p, 0x300, f32 (*)(Pursuer *))(p));
        return;
    }
    func_00125A10(&p->c);
}

/* vtable +0x1F4: go to the room exit +0x17B0 */
/* 0x0028D7D0 */
void Pursuer_ExitGoTo(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    PU(p, 0x15A4, s32) = VCALL(gRooms, 0x24, s32 (*)(VObject *, u32))(gRooms, PU(p, 0x17B0, u8));
    VCALL(gNavMesh, 0xC, void (*)(void *, s32, f32 *))(gNavMesh, PU(p, 0x15A4, s32), (f32 *)((u8 *)p + 0x15B0));
    VCALL(p, 0xD8, void (*)(Pursuer *))(p);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECF30);
    VCALL(p, 0x198, void (*)(Pursuer *))(p);
}

/* vtable +0x6C: store the pursuer's state in the progress (saved with the game) */
/* 0x0029E9D0 */
void Pursuer_SaveState(Pursuer *p) {
    u8 *s = (u8 *)gProgress + 0x83C;

    AT(gProgress, 0x83C, s32) = p->c.a.room;
    AT(gProgress, 0x844, s32) = p->c.a.unkC4;
    AT(gProgress, 0x848, s32) = PU(p, 0x16B8, s32);
    AT(gProgress, 0x850, s32) = p->c.hp;
    AT(gProgress, 0x854, s32) = p->c.hpMax;
    AT(gProgress, 0x858, s32) = PU(p, 0x1594, s32);
    AT(gProgress, 0x85C, s32) = PU(p, 0x15A4, s32);
    AT(gProgress, 0x868, u8) = p->c.door;
    AT(gProgress, 0x860, s32) = PU(p, 0x16BC, s32);
    AT(s, 0x28, u32) = (u32)PU(p, 0x16C0, f32);
    AT(s, 0x2C, u8) = p->c.door;
    AT(s, 0x2D, u8) = PU(p, 0x16C4, s32);
    AT(s, 0x2E, u8) = PU(p, 0x16C8, u8);
    AT(s, 0x30, s32) = PU(p, 0x1660, s32);
    AT(s, 0x34, s32) = PU(p, 0x1664, s32);
    AT(s, 0x4, u32) = p->c.a.navTri;
    AT(s, 0x10, f32) = p->c.a.angle[1];
}

/* the behaviour ended (+0x1758 next, -1 none): reset, vtable +0x2A0 */
/* 0x002927D0 */
void Pursuer_BehaviourEnded(Pursuer *p) {
    s32 next = PU(p, 0x1758, s32);

    if (next != -1 && next != -2) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, next);
    }
    PU(p, 0x16F6, u8) = 0;
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    PU(p, 0x178C, s32) = 0;
    p->c.unk14D0 = 0;
    func_001F6E10(p->c.motion);
    PU(p, 0x16F8, u8) = 0;
    PU(p, 0x16F7, u8) = 0;
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECD30);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x2A0, void (*)(Pursuer *))(p);
}

/* vtable +0x5C: reset (room entry) */
/* 0x0029E520 */
void Pursuer_Activate(Pursuer *p) {
    p->c.a.navMask = VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
    if (p->c.a.slot == 2) {
        func_002DDE20(p->c.motion, 0, -1);
    }
    PU(p, 0x17B0, u8) = 0xFF;
    PU(p, 0x1664, s32) = 0;
    PU(p, 0x1544, u8) = 0;
    PU(p, 0x1545, u8) = 0;
    PU(p, 0x16C8, u8) = 3;
    VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
    PU(p, 0x16C9, u8) = 0;
    PU(p, 0x16CB, u8) = 0;
    PU(p, 0x16CA, u8) = 0;
    PU(p, 0x16CC, u8) = 0;
    PU(p, 0x179C, s32) = -1;
    PU(p, 0x16F1, u8) = 0;
    PU(p, 0x16F3, u8) = 0;
    PU(p, 0x16F2, u8) = 0;
    PU(p, 0x16F4, u8) = 0;
    if (p->c.hp <= 0) {
        p->c.hp = p->c.hpMax;
    }
    VCALL(p, 0x31C, void (*)(Pursuer *, s32))(p, 0);
    Character_Activate(&p->c);
}

/* the D_0046F020 stalker's +0x5C reset: the base one, and its +0x16DC 0x10 while the countdown
 * runs (progress +0x1FBEC1) */
/* 0x002CF0F0 */
void Debilitas3_Activate(Pursuer *p) {
    Pursuer_Activate(p);
    if (AT(gProgress, 0x1FBEC1, u8) != 0) {
        AT(p, 0x16DC, s32) = 0x10;
    }
}

u8 *func_002CF140(Pursuer *p) {
    return (F(gProgress, 0x30, u32) & 0x8000) ? D_00413530 : D_004134F0;
}

/* 0x002CF180 */
void *Debilitas3_ModelFiles(void) {
    return (F(gProgress, 0x30, u32) & 0x8000) ? D_00413510 : D_004134D0;
}

u8 *func_002DC460(Pursuer *p) {
    return (F(gProgress, 0x30, u32) & 0x8000) ? D_00414820 : D_004147E0;
}

/* Moves the floats at +0x854/+0x858 toward (tx, ty) by at most |sx|/|sy|. */
void func_002DD310(u8 *p, f32 tx, f32 ty, f32 sx, f32 sy) {
    f32 v, d;

    if (sx <= 0.0f) sx = -sx;
    v = F(p, 0x854, f32);
    d = tx - v;
    if (d <= 0.0f) d = -d;
    if (d <= sx) {
        F(p, 0x854, f32) = tx;
    } else if (tx <= v) {
        F(p, 0x854, f32) -= sx;
    } else {
        F(p, 0x854, f32) += sx;
    }
    if (sy <= 0.0f) sy = -sy;
    v = F(p, 0x858, f32);
    d = ty - v;
    if (d <= 0.0f) d = -d;
    if (d <= sy) {
        F(p, 0x858, f32) = ty;
    } else if (ty <= v) {
        F(p, 0x858, f32) -= sy;
    } else {
        F(p, 0x858, f32) += sy;
    }
}

/* the chase towards the target: by path, or straight at it once on its triangle */
/* 0x00290810 */
void Pursuer_ChaseTarget(Pursuer *p) {
    if (PU(p, 0x1590, f32) < 0.0f) {
        VCALL(p, 0xB4, void (*)(Pursuer *, Character *))(p, p->target);
        if (p->c.a.room == p->target->a.room && !(func_00284440(p) & 0xFF)) {
            PU(p, 0x16EF, u8) = 1;
        }
    } else {
        Character *t = p->target;
        u32 tri = t->a.navTri;

        if (func_00124480(&p->c.a, t->a.pos, -1) != tri) {
            s32 n = p->c.unk128;

            if ((n < p->c.unk124) == 1) {
                func_00214620(p, n);
            }
        } else {
            func_002143D0(p, p->target->a.pos);
        }
    }
}

/* vtable +0x60: reset all behaviour state */
/* 0x0029E440 */
void Pursuer_ResetBehaviour(Pursuer *p) {
    if (func_00217510(p) != 0 && p->c.moveMode == 2) {
        func_00213270(p, 0xFF);
    }
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x16F5, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    PU(p, 0x16EC, u8) = 0;
    PU(p, 0x1710, u8) = 0;
    PU(p, 0x16F9, u8) = 0;
    PU(p, 0x16F6, u8) = 0;
    PU(p, 0x16F8, u8) = 0;
    PU(p, 0x16F7, u8) = 0;
    PU(p, 0x15A0, u8) = 1;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1761, u8) = 0;
    PU(p, 0x1760, u8) = 0;
    PU(p, 0x178C, s32) = 0;
    PU(p, 0x1764, s32) = -1;
    p->c.unk14D0 = 0;
    PU(p, 0x17B4, s32) = 0;
    PU(p, 0x1728, s32) = -1;
    PU(p, 0x172C, u8) = 0;
    PU(p, 0x1738, s32) = 0;
    func_0029A6D0(p);
    PU(p, 0x1700, s32) = 0;
    PU(p, 0x1704, s32) = 0;
    PU(p, 0x1708, s32) = 0;
    PU(p, 0x1770, s32) = 0;
    PU(p, 0x1774, s32) = 0;
    PU(p, 0x1778, s32) = 0;
    VCALL(p, 0x7C, void (*)(Pursuer *))(p);
    Character_ResetBehaviour(&p->c);
}

/* give the model the motion banks loaded by character `slot` (its +0x1670 file) */
void func_0029F040(Pursuer *p, u32 slot) {
    u8 *m = p->c.motion;
    u8 *f = PU(gCharacters[slot & 0xFF], 0x1670, u8 *);

    AT(m, 0x4C0, u8 *) = AT(f, 0x4, s32) != 0 ? f + AT(f, 0x4, s32) : NULL;
    AT(m, 0x4D0, u8 *) = AT(f, 0x8, s32) != 0 ? f + AT(f, 0x8, s32) : NULL;
    AT(m, 0x4CC, u8 *) = AT(f, 0xC, s32) != 0 ? f + AT(f, 0xC, s32) : NULL;
    AT(m, 0x4C4, u8 *) = AT(f, 0x10, s32) != 0 ? f + AT(f, 0x10, s32) : NULL;
    m = p->c.motion;
    VCALL(m, 0xC, void (*)(void *))(m);
    p->c.a.unkD1 = 1;
    MOTION_AT(p, 0x24, u8) = slot;
}

/* turn to +0x1634 by +0x1638 while the animation runs; then face it */
void func_00286AA0(Pursuer *p) {
    s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

    if (((over ^ 1) & 0xFF) != 1) {
        f32 h = PU(p, 0x1634, f32);

        p->c.a.angle[1] = h;
        sceVu0UnitMatrix(p->c.a.rot);
        sceVu0RotMatrixY(p->c.a.rot, p->c.a.rot, h);
        PU(p, 0x1634, f32) = 1.0f;
        PU(p, 0x1638, s32) = 0;
        Actor_SetState(&p->c.a, &D_003ED210);
    } else {
        func_00124530(&p->c.a, PU(p, 0x1634, f32), PU(p, 0x1638, f32));
    }
}

/* vtable +0x1F8 .. : through the exit +0x17B0 if it's usable */
/* 0x0028D6E0 */
void Pursuer_ExitDone(Pursuer *p) {
    if (PU(p, 0x16B8, s32) == 2) {
        PURSUER_STEP_DONE(p) = 1;
        return;
    }
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (!(VCALL(gRooms, 0x78, s32 (*)(VObject *, s32, u32))(gRooms, p->c.a.room, PU(p, 0x17B0, u8)) & 0xFF)) {
        PU(p, 0x16EF, u8) = 1;
        PURSUER_STEP_NEXT(p) = 1;
        return;
    }
    p->c.a.disabled = 1;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECF40);
    VCALL(p, 0x1F8, void (*)(Pursuer *))(p);
}

/* behaviour starts: the previous one's follow-up (+0x1758, -1: `dflt`), flags reset,
 * the behaviour's step function (+0x174C) set, then vtable `next` */
#define PURSUER_BEGIN(p, dflt)                                                  \
    do {                                                                        \
        s32 prev_ = PU(p, 0x1758, s32);                                         \
        if (prev_ != -1) {                                                      \
            if (prev_ != -2) {                                                  \
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, prev_);            \
            }                                                                   \
        } else {                                                                \
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, dflt);                 \
        }                                                                       \
    } while (0)

/* vtable +0x288 */
/* 0x00294150 */
void Pursuer_Behaviour288(Pursuer *p) {
    PURSUER_BEGIN(p, 0x14);
    PU(p, 0x16F6, u8) = 1;
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    PU(p, 0x1760, u8) = 0;
    PU(p, 0x162C, s32) = 0;
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECCB0);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x28C, void (*)(Pursuer *))(p);
}

/* vtable +0x278: chase Fiona */
/* 0x002947F0 */
void Pursuer_ChaseFiona(Pursuer *p) {
    PURSUER_BEGIN(p, 4);
    p->target = gCharPlayer;
    PU(p, 0x16F6, u8) = 1;
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECC70);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x27C, void (*)(Pursuer *))(p);
}

/* vtable +0x25C */
/* 0x00296ED0 */
void Pursuer_Behaviour25C(Pursuer *p) {
    PURSUER_BEGIN(p, 8);
    p->target = gCharPlayer;
    PU(p, 0x16F6, u8) = 1;
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECB90);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x260, void (*)(Pursuer *))(p);
}

/* ---- batch 4 ---- */

extern void *D_0046A620[], *D_00474560[], *D_0046D810[], *D_0046C220[], *D_00469C60[], *D_00469C20[];
extern const PTMF D_003ED190, D_003ED1A0, D_003ED0B0, D_003ECE20, D_003ED510, D_003ED520, D_003ECDA0,
    D_003ECF00;

/* vtable +0x40 (via +0x84 first): blocking flags (8 = held at a door), then +0x110 / +0x40 */
/* 0x0029E390 */
void Pursuer_Update(Pursuer *p) {
    VCALL(p, 0x84, void (*)(Pursuer *))(p);
    p->c.a.navMask = p->c.a.unk2B == 1 ? 8 : VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
    p->c.pathReq->mask = p->c.a.navMask;
    if (func_00217510(p) != 0) {
        func_00297C60(p);
        VCALL(p, 0x110, void (*)(Pursuer *))(p);
    }
    VCALL(p, 0x40, void (*)(Pursuer *))(p);
}

/* the target out of reach on the current triangle: search (0x12) or wait at a door (0xB) */
s32 func_00284440(Pursuer *p) {
    if (PU(p, 0x15A4, s32) == -1) {
        return 0;
    }
    if ((func_002138F0(p, PU(p, 0x15A4, s32)) & 0xFF) == 1) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x12);
        return 1;
    }
    if ((func_00213690(p, PU(p, 0x15A4, s32)) & 0xFF) == 1) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0xB);
        return 1;
    }
    if (AT(gNavMesh, 0x14, u32) >= 2 && (func_00212FE0(p, -1) & 0xFF) == 1) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x12);
        return 1;
    }
    return 0;
}

/* a cry heard (gProgress +0x1020 per slot): hold for (+0x1748)[0] frames (90 without), with
 * its sound +0x1764 and reaction animation */
u32 func_0029B4B0(Pursuer *p) {
    Progress *pr = gProgress;
    u32 heard = AT(pr, 0x1020 + *(u8 *)&p->c.a.slot * 0x10, u8) & ~PU(p, 0x1760, u8) & 0xFF;

    if (heard != 0) {
        s32 *t = PU(p, 0x1748, s32 *);
        s32 snd;

        PU(p, 0x178C, s32) = t != NULL ? *t : 90;
        PU(p, 0x1760, u8) |= heard;
        snd = PU(p, 0x1764, s32);
        if (snd >= 0) {
            if (VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(pr, 8) == 0) {
                func_00122C20(&p->c.a, snd, 7, 0, 0, NULL);
            }
            p->c.unk14D0 = 5;
            func_001F6E30(p->c.motion);
        }
    }
    PU(p, 0x1764, s32) = -1;
    return heard;
}

/* the model takes the motion banks of its file +0x1670; the message image +0x1674 */
void func_0029EE70(Pursuer *p) {
    u8 *f = PU(p, 0x1670, u8 *);
    u8 *m = p->c.motion;

    AT(m, 0x4C0, u8 *) = AT(f, 0x4, s32) != 0 ? f + AT(f, 0x4, s32) : NULL;
    AT(m, 0x4D0, u8 *) = AT(f, 0x8, s32) != 0 ? f + AT(f, 0x8, s32) : NULL;
    AT(m, 0x4CC, u8 *) = AT(f, 0xC, s32) != 0 ? f + AT(f, 0xC, s32) : NULL;
    AT(m, 0x4C4, u8 *) = AT(f, 0x10, s32) != 0 ? f + AT(f, 0x10, s32) : NULL;
    if (*AT(PU(p, 0x168C, u8 *), 0x8, char *) != 0) {
        p->c.msgSlot = p->c.a.slot;
        if ((VCALL(gBootMessage, 0x8, s32 (*)(VObject *, u32, s32))(gBootMessage, p->c.msgSlot, PU(p, 0x1674, s32)) & 0xFF) == 1) {
            p->c.a.unkD0 = 1;
        }
        m = p->c.motion;
        VCALL(m, 0xC, void (*)(void *))(m);
        p->c.a.unkD1 = 1;
        MOTION_AT(p, 0x24, u8) = p->c.msgSlot;
    }
}

/* threat: `amount` within 10 units of the target, * 0.75 for each further 10, none past 40 */
/* 0x002982A0 */
void Pursuer_Threat(Pursuer *p, f32 amount) {
    f32 d = PU(p, 0x1588, f32);

    if (d < 0.0f) {
        return;
    }
    if (d < 10.0f) {
        Threat_Raise((u8 *)gProgress + 0x7B8, amount);
    } else if (d < 20.0f) {
        Threat_Raise((u8 *)gProgress + 0x7B8, 0.75f * amount);
    } else if (d < 30.0f) {
        Threat_Raise((u8 *)gProgress + 0x7B8, 0.75f * (0.75f * amount));
    } else if (d < 40.0f) {
        Threat_Raise((u8 *)gProgress + 0x7B8, 0.75f * (0.75f * (0.75f * amount)));
    }
}

/* stance step */
void func_00280090(Pursuer *p) {
    u8 k;

    func_00214ED0(p);
    func_0027FE90(p);
    k = PU(p, 0x16C8, u8);
    switch (k) {
    case 0:
        if (PU(p, 0x1544, u8) == 0) {
            PU(p, 0x16C8, u8) = 2;
            VCALL(p, 0x2C0, void (*)(Pursuer *, u32))(p, k);
            PU(p, 0x16C9, u8) = 3;
            PU(p, 0x16CA, u8) = 4;
        }
        break;
    case 1:
        if (func_00217260(p) != 0) {
            if (PU(p, 0x159C, s32) == -2) {
                PU(p, 0x159C, s32) = -1;
            }
            if (PU(p, 0x16C9, u8) == 2) {
                PU(p, 0x16C9, u8) = 0;
                PU(p, 0x16CB, u8) = 0;
                PU(p, 0x16CA, u8) = 0;
                PU(p, 0x16CC, u8) = 0;
                PU(p, 0x179C, s32) = -1;
                PU(p, 0x16F1, u8) = 0;
                PU(p, 0x16F3, u8) = 0;
                PU(p, 0x16F2, u8) = 0;
                PU(p, 0x16F4, u8) = 0;
            }
            PU(p, 0x16C8, u8) = 2;
        }
        break;
    }
}

/* play animation `anim` (restart only if it ended or doesn't loop); 1 if started */
s32 func_00297B40(Pursuer *p, s32 anim, s32 blend) {
    u8 *m = p->c.motion;

    if (anim == AT(m, 0x55C, s32)) {
        s32 over = AT(m, 0x550, f32) <= 0.0f;

        if ((over ^ 1) & 0xFF) {
            return 0;
        }
        if (((AT(AT(m, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0) != 1) {
            s32 i = func_001F4710(m, anim);
            u16 fl = i != -1 ? AT(AT(m, 0x874, u8 *) + i * 6, 0x4, u16) : 0;

            if (fl & 4) {
                return 0;
            }
        }
        func_002DDED0(p->c.motion, anim, -1);
        return 1;
    }
    if (blend == 0) {
        func_002DDED0(m, anim, -1);
    } else {
        func_002DDE20(m, anim, -1);
    }
    return 1;
}

/* hit reaction by the animation being played (inlined in func_00287950) */
static void Pursuer_HitReact(Pursuer *p) {
    if (p->c.a.unkC4 != 2 && MOTION_ANIM(p) == 0x1709) {
        PU(p, 0x1624, s32) = 0x1806;
    } else {
        PU(p, 0x1624, s32) = MOTION_ANIM(p);
    }
    PU(p, 0x1628, s32) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (PU(p, 0x1624, s32) != 0x1807 && PU(p, 0x1624, s32) != 0x1803) {
        Actor_SetState(&p->c.a, &D_003ED1A0);
        func_00287CD0(p);
    } else {
        Actor_SetState(&p->c.a, &D_003ED190);
        func_00287B50(p);
    }
}

void func_00288030(Pursuer *p) {
    Pursuer_HitReact(p);
}

void func_0028A540(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    p->target = gCharPlayer;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED0B0);
    func_0028A190(p);
}

/* vtable +0x1C8 */
/* 0x0028FC10 */
void Pursuer_Door1C8(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    PU(p, 0x1568, s32) = 0;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECE20);
    VCALL(p, 0x1AC, void (*)(Pursuer *))(p);
}

/* vtable +0x184 */
/* 0x00291880 */
void Pursuer_Move184(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    func_00297300(p, (u8)p->c.unk104[0]);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECDA0);
    VCALL(p, 0x188, void (*)(Pursuer *))(p);
}

/* vtable +0x1F0 */
/* 0x0028DAA0 */
void Pursuer_Exit1F0(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    VCALL(p, 0x128, void (*)(Pursuer *))(p);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECF00);
    VCALL(p, 0x1E8, void (*)(Pursuer *))(p);
}

/* vtable +0x74: the point the attack lands (+0x1770), while attacking */
/* 0x0029CBE0 */
s32 Pursuer_AttackPoint(Pursuer *p, f32 *out) {
    if (PU(p, 0x175C, s32) != 0x23) {
        if (p->c.moveMode == 8) {
            s32 sub = p->c.moveSub;

            if (sub != 0x1A && sub != 0x1B && sub != 0x1C) {
                return 0;
            }
            if (PU(p, 0x1728, s32) >= 0 && !(func_001F4770(p->c.motion, 0, -2, 1) & 0xFF & 2)) {
                return 0;
            }
            sceVu0CopyVector(out, (f32 *)((u8 *)p + 0x1770));
            return 1;
        }
        return 0;
    }
    if (!(func_001F4770(p->c.motion, 0, -2, 1) & 0xFF & 2)) {
        return 0;
    }
    sceVu0CopyVector(out, (f32 *)((u8 *)p + 0x1770));
    return 1;
}

/* vtable +0x290: the idle behaviour (another one while the progress byte +0x1FBEC1 is set) */
/* 0x0027E440 */
void Pursuer_BehaviourIdle(Pursuer *p) {
    PU(p, 0x16F6, u8) = 1;
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    if (AT(gProgress, 0x1FBEC1, u8) != 0) {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED510);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x298, void (*)(Pursuer *))(p);
        return;
    }
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED520);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x294, void (*)(Pursuer *))(p);
}

/* vtable +0x188: once the animation ends, back to the stand animation */
/* 0x00291760 */
void Pursuer_BackToStand(Pursuer *p) {
    if (((MOTION_KEYS(p) & MOTION_KEY_END) != 0) == 1) {
        u8 *m;

        PURSUER_STEP_DONE(p) = 1;
        p->c.unk104[0] = -1;
        m = p->c.motion;
        if (AT(m, 0x55C, s32) == 0) {
            s32 over = AT(m, 0x550, f32) <= 0.0f;

            if (!((over ^ 1) & 0xFF)) {
                if (((AT(AT(m, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0) != 1) {
                    s32 i = func_001F4710(m, 0);
                    u16 fl = i != -1 ? AT(AT(m, 0x874, u8 *) + i * 6, 0x4, u16) : 0;

                    if (fl & 4) {
                        return;
                    }
                }
                func_002DDED0(p->c.motion, 0, -1);
            }
        } else {
            func_002DDED0(m, 0, -1);
        }
    } else {
        func_00125A10(&p->c);
    }
}

/* the chance roll, rising each time it fails: true when 100 * random <= the chance
 * (table +0x1740 by situation: {chance, rise}) plus what built up (+0x16C0) */
s32 func_00297160(Pursuer *p) {
    f32 *t = PU(p, 0x1740, f32 *);

    if (t != NULL) {
        f32 chance, rise;

        if (gCharPartner->a.unkC4 == 2) {
            chance = t[0];
            rise = t[1];
        } else if (PU(p, 0x1760, u8) & 2) {
            chance = t[2];
            rise = t[3];
        } else if (PU(p, 0x1761, u8) != 0) {
            chance = t[6];
            rise = t[7];
        } else {
            chance = t[4];
            rise = t[5];
        }
        if (100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) <= chance + PU(p, 0x16C0, f32)) {
            PU(p, 0x16BC, s32) = 0;
            PU(p, 0x16C0, f32) = 0.0f;
        } else {
            PU(p, 0x16C0, f32) += rise;
            return 0;
        }
    } else {
        PU(p, 0x16BC, s32) = 0;
        PU(p, 0x16C0, f32) = 0.0f;
    }
    PU(p, 0x16F8, u8) = 0;
    return 1;
}

/* vtable +0x48: place the model on the character (or let the model place it) */
/* 0x0029A2A0 */
void Pursuer_PlaceModel(Pursuer *p) {
    u8 *m;

    if (p->c.unkE3 == 0) {
        f32 mat[4][4] __attribute__((aligned(16)));

        sceVu0UnitMatrix(mat);
        m = p->c.motion;
        VCALL(m, 0x28, void (*)(void *, f32 (*)[4]))(m, mat);
    } else {
        m = p->c.motion;
        VCALL(m, 0x40, void (*)(void *, Pursuer *, f32, f32))(m, p, 0.0f, 0.0f);
    }
    if (p->c.a.disabled == 0) {
        func_001F6AF0(p->c.motion);
        if (VCALL(gCutscene, 0x54, s32 (*)(VObject *, s32, s32))(gCutscene, 0, 0) > 0) {
            MOTION_AT(p, 0x850, u8) = 1;
        }
        m = p->c.motion;
        VCALL(m, 0x3C, void (*)(void *))(m);
        p->c.a.room = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
        if (p->c.unkE3 == 0) {
            sceVu0CopyVector(p->c.a.pos, Skel_Bone(MOTION_AT(p, 0x810, u8 *), 0) + 0xC);
        }
        p->c.a.navTri = VCALL(gNavMesh, 0x3C, u32 (*)(void *, f32 *, s32))(gNavMesh, p->c.a.pos, 0);
    }
}

/* destructor of the kind 0x15 class (vtables 0x474560 / 0x46A620 -> 0x46D810 -> 0x46C220 ->
 * Character) */
/* 0x00179970 */
Pursuer *Kind22_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = D_0046A620;
        if (p != NULL) {
            p->c.a.vtbl = D_00474560;
            if (p != NULL) {
                p->c.a.vtbl = D_0046D810;
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
                    p->c.a.vtbl = D_0046C220;
                    VCALL(p, 0x10, void (*)(Pursuer *))(p);
                    if (p != NULL) {
                        p->c.a.vtbl = D_00469C60;
                        if (p != NULL) {
                            p->c.a.vtbl = D_00469C20;
                        }
                    }
                }
            }
        }
        if ((s16)flags > 0) {
            func_00124E40(&p->c.a);
        }
    }
    return p;
}

/* 0x00179A90 */
void *Kind21_ModelFiles(void) {
    return D_0042C870;
}

/* 0x00179AA0 */
void *Kind21_MotionFiles(void) {
    return D_0042C8B0;
}

/* ---- batch 5 ---- */

extern const PTMF D_003ECF50, D_003ECFC0, D_003ECF10, D_003ECF90, D_003ECCD0, D_003ED090, D_003ED0A0,
    D_003ECDB0;
extern void *D_0046D810[];

/* vtable +0x8: destructor (Pursuer 0x46D810 -> NPC 0x46C220 -> Character); the model is
 * freed for slots 3..5 */
/* 0x00172810 */
Pursuer *Pursuer_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        Pursuer_DestroyBase(p);
        if ((s16)flags > 0) {
            func_00124E40(&p->c.a);
        }
    }
    return p;
}

/* 0x00172910 */
void *Kind33_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x21, arg, D_00477790);
}

/* 0x00172960 */
void *Kind32_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x20, arg, D_00476C10);
}

/* 0x001729B0 */
void *Kind31_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x1F, arg, D_004767D0);
}

/* 0x00172A00 */
void *Kind30_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x1E, arg, D_004764A0);
}

/* 0x00172A50 */
void *Kind29_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x1D, arg, D_00476120);
}

/* 0x00172AA0 */
void *Kind28_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x1C, arg, D_00475DB0);
}

/* 0x00172AF0 */
void *Kind26_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x1A, arg, D_00474C10);
}

/* 0x00172B40 */
void *Kind25_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x19, arg, D_00474130);
}

/* 0x00172B90 */
void *Kind24_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x18, arg, D_00473CD0);
}

/* 0x00172BE0 */
void *Kind37_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x25, arg, D_00478770);
}

/* 0x00172C30 */
void *TintStalker_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x17, arg, D_004738A0);
}

/* 0x00172C80 */
void *Kind22_ctor(void *p, u32 id, u32 arg) {
    return b0_RoomCtor(p, id, (u8)arg, D_0046A620);
}

/* vtable +0x1F8: open the door at exit +0x17B0 and step through */
/* 0x0028D5B0 */
void Pursuer_ExitOpenStep(Pursuer *p) {
    VObject *d = gDoors;
    u32 side;

    p->c.unk100 = PU(p, 0x17B0, u8);
    side = ((s8)VCALL(d, 0x18, s32 (*)(VObject *, u32, f32 *))(d, (u8)p->c.unk100, p->c.a.pos) != 0 ? 3 : 1) & 0xFFFF;
    if ((DoorHold_Take(gProgress, p->c.a.room, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) & 0xFF) == 1) {
        p->c.unk100 = -1;
        PU(p, 0x16EF, u8) = 1;
        return;
    }
    p->c.moveMode = 2;
    p->c.moveSub = 0x15;
    func_00212DC0(p, (u8)p->c.unk100);
    VCALL(d, 0xC, void (*)(VObject *, u32, u32, s32, s32))(d, (u8)p->c.unk100, side, p->c.a.slot, 1);
    Actor_SetState(&p->c.a, &D_003ECF50);
}

/* arrive at the goal (+0x15A4 / +0x15B0): snap onto it, or turn to the heading +0x10C */
void func_00299370(Pursuer *p) {
    if (PURSUER_STEP_DONE(p) == 1) {
        s32 tri = PU(p, 0x15A4, s32);

        if ((s32)p->c.a.navTri != tri) {
            f32 d = func_001257B0(&p->c, tri, (f32 *)((u8 *)p + 0x15B0), -1);

            if (d < 0x1.99999a0000000p-4f /* 0.1 */ && !(d < 0.0f)) {
                p->c.a.navTri = PU(p, 0x15A4, s32);
                sceVu0CopyVector(p->c.a.pos, (f32 *)((u8 *)p + 0x15B0));
                return;
            }
            PURSUER_STEP_DONE(p) = 0;
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 4);
        } else if (func_002140A0(p, AT(p, 0x10C, f32), VCALL(p, 0xA0, f32 (*)(Pursuer *))(p)) == 0.0f) {
            PURSUER_STEP_DONE(p) = 0;
            if (PU(p, 0x175C, s32) != 0) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0);
            }
        }
    } else if (ptmf_test(&p->c.a.state)) {
        ptmf_scall(p, &p->c.a.state);
    }
}

/* vtable +0x68: does event `kind` concern the pursuer (character `slot`, door `door`)? */
/* 0x0029CD00 */
s32 Pursuer_EventConcerns(Pursuer *p, u32 kind, s32 slot, u32 door) {
    kind &= 0xFF;
    if (kind != 5) {
        Character *c = gCharacters[slot];

        if (c == NULL || (c->a.active == 0 && c->a.disabled == 1)) {
            return 0;
        }
    } else if (!(VCALL(gDoors, 0x40, s32 (*)(VObject *, u32))(gDoors, door & 0xFF) & 0xFF)) {
        return 0;
    }
    switch (kind) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
    case 8:
    case 11:
        return 1;
    case 6:
        if (p->c.moveMode != 4) {
            return 1;
        }
        return 0;
    case 9:
    case 10:
        if (p->c.moveSub != 9 && p->c.moveSub != 0xA && p->c.moveMode != 3) {
            return 1;
        }
        return 0;
    default:
        return 0;
    }
}

/* walk on to the exit the pursuer heads for */
/* 0x00285360 */
void Pursuer_WalkToExit(Pursuer *p) {
    s32 done = 0;

    if (PU(p, 0x1590, f32) < 0.0f) {
        VObject *rm;
        u32 node;

        if (func_00284440(p) & 0xFF) {
            return;
        }
        rm = gRooms;
        PU(p, 0x17B0, u8) = p->c.door;
        PU(p, 0x15A4, s32) = VCALL(rm, 0x34, s32 (*)(VObject *, u32, f32 *))(rm, PU(p, 0x17B0, u8), (f32 *)((u8 *)p + 0x15B0));
        VCALL(p, 0xD8, void (*)(Pursuer *))(p);
        node = VCALL(rm, 0x10, u32 (*)(VObject *, s32, u32))(rm, p->c.a.room, p->c.door) & 0xFFFF;
        p->c.unk148C[node >> 5] |= 1 << (node & 0x1F);
    }
    if ((p->c.unk128 < p->c.unk124) != 1) {
        if (PU(p, 0x1590, f32) != 0.0f) {
            PU(p, 0x16EF, u8) = 1;
        } else {
            done = 1;
        }
    } else {
        done = func_00214620(p, p->c.unk128) & 0xFF;
    }
    if (done == 1) {
        PURSUER_STEP_DONE(p) = 1;
    }
}

/* turned to the door (+0x1568): open it with the arm (animation 0x700 / 0x704 by +0x104) */
/* 0x0028C420 */
void Pursuer_DoorArmOpen(Pursuer *p) {
    u8 *m;

    if (func_002140A0(p, PU(p, 0x1568, f32), VCALL(p, 0xA0, f32 (*)(Pursuer *))(p)) != 0.0f) {
        return;
    }
    m = p->c.motion;
    if (AT(m, 0x858, f32) == 0.0f) {
        s32 over = AT(m, 0x550, f32) <= 0.0f;

        if (((over ^ 1) & 0xFF) != 1) {
            RoomSlots_Enter(gProgress, (u8)p->c.unk100, *(u8 *)&p->c.a.slot);
            p->c.a.unk2A = 1;
            PU(p, 0x15C0, u8) = 0xFF;
            PU(p, 0x1624, s32) = PU(p, 0x15A4, s32);
            if (p->c.unk104[0] == 1) {
                func_002DDD20(p->c.motion, 0x700, -1);
            } else {
                func_002DDD20(p->c.motion, 0x704, -1);
            }
            Actor_SetState(&p->c.a, &D_003ECFC0);
        }
    }
}

/* how is Fiona doing (for the chase): 0 calm, 1 in move mode 0xB, 2 panicking, 3 out of play
 * or in move 0xA / 0xB, 4 / 5 by the threat stage (gProgress +0x7B8) */
s32 func_00283EF0(Pursuer *p) {
    s32 can;

    if (p->target->a.unk2D == 1 || (Progress_TestFlag(gProgress, 0xE) & 0xFF) == 1 || gCharPlayer->moveSub == 0x10) {
        can = 0;
    } else {
        can = 1 & 0xFF;
    }
    if (can & 0xFF) {
        s32 hiding = 1;

        if (gCharPlayer->moveSub != 0xA && gCharPlayer->moveSub != 0xB) {
            hiding = 0;
        }
        switch (AT(gProgress, 0x7B8, u8)) {
        case 4:
            if (hiding == 0) {
                return 4;
            }
            /* fallthrough */
        case 5:
            return 5;
        default:
            if (hiding == 0) {
                if (gCharPlayer->moveMode == 0xB) {
                    return 1;
                }
                return func_00217560() == 0 ? 0 : 2;
            }
            return 3;
        }
    }
    return 3;
}

/* vtable +0x1E8: through the door the pursuer heads for */
/* 0x0028D950 */
void Pursuer_ExitThrough(Pursuer *p) {
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    VObject *d;
    u32 r, side;

    if (VCALL(gRooms, 0x34, s32 (*)(VObject *, u32, f32 *))(gRooms, p->c.door, a) == -1) {
        PU(p, 0x16EF, u8) = 1;
        return;
    }
    d = gDoors;
    r = VCALL(d, 0x18, s32 (*)(VObject *, u32, f32 *))(d, p->c.door, a) & 0xFF;
    if (r == 0) {
        side = 1;
    } else if (r == 1) {
        side = 3;
    } else {
        PU(p, 0x16EF, u8) = 1;
        return;
    }
    PU(p, 0x15C4, s32) = VCALL(d, 0x14, s32 (*)(VObject *, u32, u32, f32 *, f32 *, s32))(d, p->c.door, side, (f32 *)((u8 *)p + 0x15D0), b, 1);
    if (!(VCALL(p, 0xDC, s32 (*)(Pursuer *))(p) & 0xFF)) {
        PU(p, 0x16EF, u8) = 1;
        return;
    }
    Actor_SetState(&p->c.a, &D_003ECF10);
    VCALL(p, 0x1EC, void (*)(Pursuer *))(p);
}

/* vtable +0x90: reset after an event (stance 4 resolved, Hewie back on his feet: Fiona's
 * relief) */
/* 0x0029A3D0 */
void Pursuer_EventReset(Pursuer *p) {
    if (p->c.moveMode == 2 && (Progress_CurRoomFlag(gProgress, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1) {
        func_00213270(p, 0xFF);
    }
    if (PU(p, 0x16C8, u8) == 4) {
        if (VCALL(p, 0xC0, s32 (*)(Pursuer *))(p) != 0) {
            PU(p, 0x16C8, u8) = 0;
            VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
            PU(p, 0x16C9, u8) = 6;
            PU(p, 0x16CA, u8) = 7;
        } else {
            PU(p, 0x16C8, u8) = 2;
            func_0027E790(p);
            PU(p, 0x16C9, u8) = 2;
            PU(p, 0x16CA, u8) = 3;
        }
    }
    PURSUER_STEP_NEXT(p) = 1;
    PU(p, 0x16F6, u8) = 1;
    p->c.unk100 = -1;
    PU(p, 0x1700, s32) = 0;
    PU(p, 0x1704, s32) = 0;
    PU(p, 0x1708, s32) = 0;
    Character_EventReset(&p->c);
    if (p->c.a.unkC4 == 2) {
        p->c.a.unkC4 = 0;
        p->c.hp = p->c.hpMax;
        Relation_Request(gProgress, *(u8 *)&gCharPlayer->a.slot, 4, 3, 0, 0, 0.0f);
    }
}

/* vtable +0x214: push through the door +0x100 */
/* 0x0028CE20 */
void Pursuer_DoorPushThrough(Pursuer *p) {
    if (VCALL(gRooms, 0x70, s32 (*)(VObject *, s32, u32))(gRooms, p->c.a.room, (u8)p->c.unk100) != 0 &&
        p->c.a.unk2B != 0 && !(PursuerGroup_Fields(gProgress, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) & 0xFF & 8)) {
        p->c.a.unk2B = 0;
    }
    if (VCALL(gDoors, 0x70, s32 (*)(VObject *, u32))(gDoors, (u8)p->c.unk100) != 0 && p->c.a.unk2B != 0) {
        f32 v[4] __attribute__((aligned(16)));

        Heading_Vector(v, PU(p, 0x1634, f32));
        func_0010E640(v, v, 3.0f);
        func_001247E0(&p->c.a, v);
    }
    if ((Progress_CurRoomFlag(gProgress, p->c.a.room, (u8)p->c.unk100) & 0xFF) != 1) {
        Actor_SetState(&p->c.a, &D_003ECF90);
        VCALL(p, 0x214, void (*)(Pursuer *))(p);
    } else {
        func_00125A10(&p->c);
    }
}

/* vtable +0x280: go after Fiona (behaviour 0x1C / 0x27 by +0x16CA) */
/* 0x002934C0 */
void Pursuer_GoAfterFiona(Pursuer *p) {
    s32 prev = PU(p, 0x1758, s32);

    if (prev != -1) {
        if (prev != -2) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, prev);
        }
    } else if (PU(p, 0x16CA, u8) == 4) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x1C);
    } else {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x27);
    }
    p->target = gCharPlayer;
    PU(p, 0x16F6, u8) = 1;
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    if (func_00217260(p) != 0 && !(func_0027CA00(p) & 0xFF)) {
        PURSUER_STEP_NEXT(p) = 1;
        func_0029AF20(p);
        return;
    }
    PU(p, 0x162C, s32) = 0;
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECCD0);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x284, void (*)(Pursuer *))(p);
}

/* repeated taunt: threat up each time the animation ends, 6 times, then animation 0x1904 */
void func_0028A700(Pursuer *p) {
    func_00125A10(&p->c);
    if (MOTION_KEYS(p) & MOTION_KEY_END) {
        Panic_Fright((u8 *)gProgress + 0x7B8, VCALL(p, 0x304, f32 (*)(Pursuer *))(p));
        p->c.unk104[0]++;
        if (p->c.state[0] != 7) {
            if (p->c.unk104[0] >= 6) {
                func_002DDE20(p->c.motion, 0x1904, -1);
                Actor_SetState(&p->c.a, &D_003ED0A0);
                return;
            }
            func_002DDE20(p->c.motion, 0x1901, -1);
        } else {
            p->c.state[0] = 0;
            func_002DDE20(p->c.motion, 0x1903, -1);
            Actor_SetState(&p->c.a, &D_003ED090);
        }
    }
}

/* vtable +0x194: walk to the goal triangle +0x15A4 */
/* 0x00291600 */
void Pursuer_WalkToGoal(Pursuer *p) {
    u32 tri;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    tri = PU(p, 0x15A4, u32);
    if (tri == (u32)-1 || tri >= AT(gNavMesh, 0x8, u32)) {
        VCALL(p, 0xB4, void (*)(Pursuer *, s32))(p, 0);
    }
    if (VCALL(gNavMesh, 0x10, s32 (*)(void *, u32, f32 *))(gNavMesh, PU(p, 0x15A4, u32), (f32 *)((u8 *)p + 0x15B0)) == 4) {
        VCALL(p, 0xB4, void (*)(Pursuer *, s32))(p, 0);
    }
    if (!(VCALL(p, 0xD8, s32 (*)(Pursuer *))(p) & 0xFF)) {
        if (!(func_00284440(p) & 0xFF)) {
            PU(p, 0x16EF, u8) = 1;
        }
        func_00125A10(&p->c);
        return;
    }
    PU(p, 0x1624, s32) = -1;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECDB0);
    VCALL(p, 0x198, void (*)(Pursuer *))(p);
}

/* ---- batch 6 ---- */

extern const PTMF D_003ED360, D_003ED100, D_003ED2A0, D_003ED1F0, D_003ECF70, D_003ECD60, D_003ECB80,
    D_003ECE40;

/* the attack: pick one from the table +0x1718 ({kind, value, chance} by a 0..100 roll) */
/* 0x00283AE0 */
void Pursuer_PickAttack(Pursuer *p) {
    u8 *e;
    s32 kind;
    f32 roll;
    u32 i;

    VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, AT(gProgress, 0x7B8, u8) >= 4 ? 7 : 6);
    roll = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
    for (i = 0;; i = (i + 1) & 0xFF) {
        e = PU(p, 0x1718, u8 *) + (i & 0xFF) * 0xC;
        if (roll <= AT(e, 0x8, f32)) {
            break;
        }
    }
    kind = AT(e, 0x0, s32);
    if (kind == 0x17) {
        p->c.unk104[0] = AT(e, 0x4, s32);
    } else if (kind == 0x13) {
        PU(p, 0x1728, s32) = AT(e, 0x4, s32);
        PU(p, 0x172C, u8) = 0;
    }
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED360);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, kind);
}

/* how much ground the pursuer gains on its target in 5 frames of the animations (the target's
 * stride counted only while it can't see the pursuer: 150 units, 90 degrees either side) */
f32 func_002838E0(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));
    f32 gain = 0.0f;
    u32 i;

    for (i = 0; i < 5; i = (i + 1) & 0xFF) {
        func_001F6370(p->c.motion, v, (f32)i);
        gain += v[2];
        if (!(func_00218300(p, &p->target->a, &p->c.a, p->target->a.angle[1], 150.0f, 0x1.921fb60000000p+0f /* 1.5707964 */) & 0xFF)) {
            func_001F6370(p->target->motion, v, (f32)i);
            gain -= v[2];
        }
    }
    if (AT(gProgress, 0x7B8, u8) == 5 && p->target == gCharPlayer) {
        return 10.0f + gain;
    }
    return gain + VCALL(p, 0x2F0, f32 (*)(Pursuer *))(p);
}

/* keep standing on walkable ground (step off triangles that block the pursuer) */
void func_0029E210(Pursuer *p) {
    if (func_00214A90(p, p->c.a.navTri) != 0 && p->c.a.unk2B == 0 && func_00217510(p) != 0) {
        p->c.a.navTri = func_00211B00(p, p->c.a.navTri);
        if (p->c.a.navTri != (u32)-1 && !(func_00214A90(p, p->c.a.navTri) & 0xFF)) {
            VCALL(gNavMesh, 0xC, void (*)(void *, u32, f32 *))(gNavMesh, p->c.a.navTri, p->c.a.pos);
        } else {
            func_00124890(&p->c.a, -1);
        }
    }
    if (p->c.moveMode == 0) {
        void *nm = gNavMesh;

        if (VCALL(nm, 0x10, s32 (*)(void *, u32, f32 *))(nm, p->c.a.navTri, p->c.a.pos) == 4) {
            if (func_00214A90(p, p->c.a.navTri) & 0xFF) {
                p->c.a.navTri = func_00211B00(p, p->c.a.navTri);
                if (p->c.a.navTri == (u32)-1 || func_00214A90(p, p->c.a.navTri) != 0) {
                    func_00124890(&p->c.a, -1);
                }
            } else {
                VCALL(nm, 0xC, void (*)(void *, u32, f32 *))(nm, p->c.a.navTri, p->c.a.pos);
            }
        }
    }
}

/* hit reaction over: back to the stand animation */
void func_00287B50(Pursuer *p) {
    if (((MOTION_KEYS(p) & MOTION_KEY_END) != 0) == 1) {
        if (p->c.a.unkC4 != 0) {
            p->c.a.unkC4 = 0;
        }
        PURSUER_STEP_NEXT(p) = 1;
        Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
        PU(p, 0x1761, u8) = 0;
        PU(p, 0x1624, s32) = 0;
        PU(p, 0x1628, s32) = 0;
        PU(p, 0x1784, s32) = 0;
        PURSUER_STEP_DONE(p) = 1;
        return;
    }
    PURSUER_STEP_NEXT(p) = 0;
    func_00213B60(p, -1);
    func_00125A10(&p->c);
}

/* vtable +0x...: look around (animation 0x1A00) */
void func_00289DD0(Pursuer *p) {
    u8 *m;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    m = p->c.motion;
    if (AT(m, 0x55C, s32) != 0x1A00) {
        func_002DDD20(m, 0x1A00, -1);
    }
    VCALL(p, 0x12C, void (*)(Pursuer *, f32))(p, VCALL(p, 0x308, f32 (*)(Pursuer *))(p));
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED100);
    if (((MOTION_KEYS(p) & MOTION_KEY_END) != 0) == 1) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    } else {
        func_00125A10(&p->c);
    }
}

/* vtable +0x258: go to the door the pursuer heads for */
/* 0x00284FD0 */
void Pursuer_GoToDoor(Pursuer *p) {
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    VObject *d;
    u32 r, side;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if (VCALL(gRooms, 0x34, s32 (*)(VObject *, u32, f32 *))(gRooms, p->c.door, a) == -1) {
        PU(p, 0x16EF, u8) = 1;
        return;
    }
    d = gDoors;
    r = VCALL(d, 0x18, s32 (*)(VObject *, u32, f32 *))(d, p->c.door, a) & 0xFF;
    if (r == 0) {
        side = 1;
    } else if (r == 1) {
        side = 3;
    } else {
        PU(p, 0x16EF, u8) = 1;
        return;
    }
    PU(p, 0x15A4, s32) = VCALL(d, 0x14, s32 (*)(VObject *, u32, u32, f32 *, f32 *, s32))(d, p->c.door, side, (f32 *)((u8 *)p + 0x15B0), b, 1);
    if (!(VCALL(p, 0xD8, s32 (*)(Pursuer *))(p) & 0xFF)) {
        if (!(func_00284440(p) & 0xFF)) {
            PU(p, 0x16EF, u8) = 1;
        }
        return;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED2A0);
    VCALL(p, 0x198, void (*)(Pursuer *))(p);
}

/* plan where to go by the stance +0x16C8: to Fiona's room, through a random other exit, ... */
s32 func_0027CA00(Pursuer *p) {
    switch (PU(p, 0x16C8, u8)) {
    case 0:
        PU(p, 0x1594, s32) = gCharPlayer->a.room;
        PU(p, 0x1598, s32) = func_002172F0(p, gCharPlayer);
        /* fallthrough */
    case 1:
    case 2:
        return func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) > 0;
    case 3: {
        s32 again = 0;
        VObject *rm;
        u32 exit;

        for (;;) {
            exit = func_00212A80(p, 0xFF) & 0xFF;
            if (exit == 0xFF) {
                return 0;
            }
            if (again != 0) {
                break;
            }
            again = 1;
            if (exit != p->c.door) {
                break;
            }
        }
        rm = gRooms;
        PU(p, 0x1594, s32) = VCALL(rm, 0x18, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, exit);
        PU(p, 0x1598, s32) = -1;
        if (func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) > 0) {
            return 1;
        }
        PU(p, 0x1594, s32) = VCALL(rm, 0x18, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, p->c.door);
        return func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) > 0;
    }
    case 4:
        return VCALL(p, 0xE8, s32 (*)(Pursuer *))(p);
    default:
        return 0;
    }
}

/* vtable +0x1C: the model files loaded: give the model its motion banks, message image, door
 * sounds and +0x4D4 (slot 2; others func_0029EE70) */
/* 0x0029F120 */
void Pursuer_FilesLoaded(Pursuer *p) {
    if (p->c.a.slot == 2) {
        u8 *f = PU(p, 0x1670, u8 *);
        u8 *m = p->c.motion;

        AT(m, 0x4C0, u8 *) = AT(f, 0x4, s32) != 0 ? f + AT(f, 0x4, s32) : NULL;
        AT(m, 0x4D0, u8 *) = AT(f, 0x8, s32) != 0 ? f + AT(f, 0x8, s32) : NULL;
        AT(m, 0x4CC, u8 *) = AT(f, 0xC, s32) != 0 ? f + AT(f, 0xC, s32) : NULL;
        AT(m, 0x4C4, u8 *) = AT(f, 0x10, s32) != 0 ? f + AT(f, 0x10, s32) : NULL;
        if (*AT(PU(p, 0x168C, u8 *), 0x8, char *) != 0) {
            p->c.msgSlot = p->c.a.slot;
            if ((VCALL(gBootMessage, 0x8, s32 (*)(VObject *, u32, s32))(gBootMessage, p->c.msgSlot, PU(p, 0x1674, s32)) & 0xFF) == 1) {
                p->c.a.unkD0 = 1;
            }
            m = p->c.motion;
            VCALL(m, 0xC, void (*)(void *))(m);
            p->c.a.unkD1 = 1;
            MOTION_AT(p, 0x24, u8) = p->c.msgSlot;
        }
        if (*AT(PU(p, 0x168C, u8 *), 0xC, char *) != 0) {
            VCALL(gDoors, 0x4C, void (*)(VObject *, s32))(gDoors, 1);
        }
        if (*AT(PU(p, 0x168C, u8 *), 0x4, char *) != 0) {
            MOTION_AT(p, 0x4D4, s32) = PU(p, 0x1678, s32);
        } else {
            MOTION_AT(p, 0x4D4, s32) = 0;
        }
        return;
    }
    func_0029EE70(p);
}

/* vtable +0x...: the door ahead: stand back if it's in the way, else turn away from it */
void func_002871E0(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));
    Progress *pr;
    u32 st;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    pr = gProgress;
    st = PursuerGroup_Fields(pr, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) & 0xFF;
    VCALL(gDoors, 0x38, void (*)(VObject *, u32, f32 *))(gDoors, (u8)p->c.unk100, v);
    if ((func_00124490(&p->c.a, v) < 9.0f || (st & 0xFF & 9)) &&
        func_00178300(pr, p->c.a.room, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) != 0) {
        PU(p, 0x1634, f32) = func_00212550(p, (u8)p->c.unk100);
        p->c.a.unk2B = 1;
    } else {
        if (p->c.hp <= 0) {
            if (p->c.a.unkC4 == 2) {
                p->c.hp = p->c.hpMax;
                PU(p, 0x1664, s32) = 1;
            } else {
                p->c.hp = 1;
            }
        }
        PU(p, 0x1634, f32) = func_002E2D00(0x1.921fb60000000p+1f /* 3.1415927 */ + func_00212550(p, (u8)p->c.unk100));
        p->c.unk100 = 0xFF;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED1F0);
    func_00286F10(p);
}

/* vtable +0x204: back off from the door +0x100 */
/* 0x0028D0C0 */
void Pursuer_DoorBackOff(Pursuer *p) {
    f32 h;

    if (VCALL(gRooms, 0x70, s32 (*)(VObject *, s32, u32))(gRooms, p->c.a.room, (u8)p->c.unk100) != 0 &&
        p->c.a.unk2B != 0 && !(PursuerGroup_Fields(gProgress, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) & 0xFF & 8)) {
        p->c.a.unk2B = 0;
    }
    if (VCALL(gDoors, 0x70, s32 (*)(VObject *, u32))(gDoors, (u8)p->c.unk100) != 0 && p->c.a.unk2B != 0) {
        f32 v[4] __attribute__((aligned(16)));

        Heading_Vector(v, PU(p, 0x1634, f32));
        func_0010E640(v, v, 3.0f);
        func_001247E0(&p->c.a, v);
    }
    h = func_002E2D00(0x1.921fb60000000p+1f /* 3.1415927 */ + PU(p, 0x1634, f32));
    func_002140A0(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    if ((Progress_CurRoomFlag(gProgress, p->c.a.room, (u8)p->c.unk100) & 0xFF) != 1) {
        Actor_SetState(&p->c.a, &D_003ECF70);
        VCALL(p, 0x208, void (*)(Pursuer *))(p);
    } else {
        func_00125A10(&p->c);
    }
}

/* vtable +0x...: stand (vtable +0x320 animation) */
void func_00292170(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECD60);
    if (p->c.unkE0 != 0) {
        s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

        if (!((over ^ 1) & 0xFF)) {
            p->c.unkE1 = 1;
        }
    }
}

/* vtable +0x260: a door near the pursuer it should go through (not during behaviours 0x10,
 * 0x11, 0x21, 0x22) */
void func_00296FC0(Pursuer *p) {
    s32 b = PU(p, 0x175C, s32);

    if (b != 0x22 && b != 0x11 && b != 0x21 && b != 0x10 && p->c.moveMode != 2) {
        u32 door = func_00212850(p) & 0xFF;

        if (door != 0xFF) {
            p->c.unk100 = door;
            if (p->c.moveSub == 0xA) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x11);
            } else {
                ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECB80);
                PU(p, 0x1758, s32) = -1;
                VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x10);
            }
            if (p->c.a.unkC4 != 2) {
                PU(p, 0x16C8, u8) = 0;
                VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
                PU(p, 0x16C9, u8) = 6;
                PU(p, 0x16CA, u8) = 7;
            }
            if ((VCALL(gDoors, 0x70, s32 (*)(VObject *, u32))(gDoors, (u8)p->c.unk100) & 0xFF) == 1 &&
                (PursuerGroup_Fields(gProgress, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) & 0xFF & 8)) {
                p->c.a.unk2B = 1;
            }
        }
    }
}

/* turn to the heading +0x10C, then wait for the animation (or play the stand animation) */
void func_00299140(Pursuer *p) {
    if (func_002140A0(p, AT(p, 0x10C, f32), VCALL(p, 0xA0, f32 (*)(Pursuer *))(p)) == 0.0f) {
        if (PU(p, 0x1788, s32) == 0) {
            s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

            if (!((over ^ 1) & 0xFF)) {
                AT(p, 0x10C, s32) = 0;
                p->c.unkE1 = 1;
            }
        } else {
            Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
        }
    }
}

/* vtable +0x40: model update: placed by its stance (+0x1694.. by +0x1788) or on the floor */
/* 0x0029EAB0 */
void Pursuer_ModelUpdate(Pursuer *p) {
    s32 on = 0;
    u8 *m;

    if (func_00217510(p) != 0) {
        if (p->c.a.navTri != (u32)-1) {
            on = 1;
            if (p->c.moveMode == 3 || (p->c.unkE0 == 1 && p->c.a.unk2B == 1)) {
                on = 0;
            }
            m = p->c.motion;
            if ((on & 0xFF) == 1) {
                if (PU(p, 0x1788, s32) == 0x201) {
                    VCALL(m, 0x40, void (*)(void *, Pursuer *, f32, f32))(m, p, PU(p, 0x1694, f32), PU(p, 0x169C, f32));
                } else {
                    VCALL(m, 0x40, void (*)(void *, Pursuer *, f32, f32))(m, p, PU(p, 0x1698, f32), PU(p, 0x16A0, f32));
                }
            } else {
                VCALL(m, 0x40, void (*)(void *, Pursuer *, f32, f32))(m, p, 0.0f, 0.0f);
            }
        } else {
            Model_BodyFrames(p->c.motion, p, 0.0f, 0.0f);
        }
    }
    func_00284040(p);
    func_002DCB40(p->c.motion);
    func_002DC960(p->c.motion);
    func_001F6AF0(p->c.motion);
    if (p->c.a.room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) && p->c.a.navTri != (u32)-1) {
        m = p->c.motion;
        VCALL(m, 0x4C, void (*)(void *, s32, Pursuer *))(m, on, p);
    }
    m = p->c.motion;
    VCALL(m, 0x3C, void (*)(void *))(m);
}

/* vtable +0x8C: back to normal (deactivation of an event) */
/* 0x0029A520 */
void Pursuer_BackToNormal(Pursuer *p) {
    if (p->c.moveMode == 2 && (Progress_CurRoomFlag(gProgress, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1) {
        func_00213270(p, 0xFF);
    }
    if (gCharPartner != NULL && gCharPartner->a.active == 1 && gCharPartner->unkE0 == 0 && p->c.moveSub == 9) {
        VCALL(gCharPartner, 0x7C, void (*)(Character *))(gCharPartner);
    }
    if (gCharPlayer->unkE0 == 0 && (u32)(p->c.moveSub - 0x18) < 2) {
        VCALL(gCharPlayer, 0x7C, void (*)(Character *))(gCharPlayer);
    }
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x16F5, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    PU(p, 0x16EC, u8) = 0;
    PU(p, 0x1710, u8) = 0;
    PU(p, 0x16F9, u8) = 0;
    PU(p, 0x16F6, u8) = 0;
    PU(p, 0x16F8, u8) = 0;
    PU(p, 0x16F7, u8) = 0;
    PU(p, 0x15A0, u8) = 1;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1761, u8) = 0;
    PU(p, 0x1760, u8) = 0;
    PU(p, 0x178C, s32) = 0;
    PU(p, 0x1764, s32) = -1;
    p->c.unk14D0 = 0;
    PU(p, 0x17B4, s32) = 0;
    PU(p, 0x1728, s32) = -1;
    PU(p, 0x172C, u8) = 0;
    PU(p, 0x1738, s32) = 0;
    func_0029A6D0(p);
    PU(p, 0x1700, s32) = 0;
    PU(p, 0x1704, s32) = 0;
    PU(p, 0x1708, s32) = 0;
    PU(p, 0x1770, s32) = 0;
    PU(p, 0x1774, s32) = 0;
    PU(p, 0x1778, s32) = 0;
    p->c.unk100 = -1;
    PU(p, 0x1700, s32) = 0;
    PU(p, 0x1704, s32) = 0;
    PU(p, 0x1708, s32) = 0;
    Character_BackToNormal(&p->c);
}

/* the door-opening animation: done (0x703 / 0x707: placed on the other side) or moving */
/* 0x0028BD40 */
void Pursuer_DoorAnim(Pursuer *p) {
    Progress *pr = gProgress;
    u8 *m;

    if (!(Progress_TestFlag(pr, 0xE) & 0xFF)) {
        u32 seen = func_00217920(p) & 0xFF;

        if (~PU(p, 0x1760, u8) & seen) {
            Relation_Request(pr, *(u8 *)&p->c.a.slot, seen & 0xFF, 4, 5, 0, 10.0f);
        }
    }
    m = p->c.motion;
    if (AT(AT(m, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) {
        f32 ofs[4] __attribute__((aligned(16)));

        switch (AT(m, 0x55C, s32)) {
        case 0x703:
            VCALL(p, 0x9C, void (*)(Pursuer *, u32, f32 *))(p, 3, ofs);
            p->c.a.navTri = func_00123710(p, p->c.unk100, 0, ofs, p->c.a.pos);
            break;
        case 0x707:
            VCALL(p, 0x9C, void (*)(Pursuer *, u32, f32 *))(p, 2, ofs);
            p->c.a.navTri = func_00123710(p, p->c.unk100, 1, ofs, p->c.a.pos);
            break;
        }
        RoomSlots_Leave(pr, (u8)p->c.unk100, *(u8 *)&p->c.a.slot);
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
        p->c.unk100 = -1;
        p->c.unk104[0] = -1;
        p->c.a.unk2A = 0;
        return;
    }
    {
        f32 v[4] __attribute__((aligned(16)));

        func_001F6370(m, v, 0.0f);
        func_00125900(&p->c);
        sceVu0ApplyMatrix(v, p->c.a.rot, v);
        sceVu0AddVector(p->c.a.pos, p->c.a.pos, v);
        p->c.a.pos[3] = 1.0f;
    }
}

/* vtable +0x1B0: go through door +0x100 from side +0x104, if it isn't locked to us */
/* 0x0028F840 */
void Pursuer_DoorGoThrough(Pursuer *p) {
    Progress *pr = gProgress;

    if ((Progress_CurRoomFlag(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1 &&
        (PursuerGroup_Fields(pr, (u8)p->c.unk100, 0) & 0xFF & 4)) {
        goto fail;
    }
    switch (p->c.unk104[0]) {
    case 1:
    case 3:
        if (!(func_00178980(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF)) {
            goto fail;
        }
        break;
    case 0:
    case 2:
        if (func_00178980(pr, p->c.a.room, (u8)p->c.unk100) != 0) {
            goto fail;
        }
        break;
    default:
        goto fail;
    }
    if (!(func_00214620(p, p->c.unk128) & 0xFF)) {
        return;
    }
    Actor_SetState(&p->c.a, &D_003ECE40);
    VCALL(p, 0x1B4, void (*)(Pursuer *))(p);
    return;
fail:
    p->c.unk100 = -1;
    p->c.unk104[0] = -1;
    PU(p, 0x1568, s32) = 0;
    PU(p, 0x16EF, u8) = 1;
}

/* ---- batch 7 ---- */

extern const PTMF D_003ECEF0, D_003ED540, D_003ED550, D_003ECE50, D_003ECDF0, D_003ED270, D_003ED250,
    D_003ED1C0, D_003ED1D0, D_003ECEC0, D_003ECE10, D_003ECD90, D_003ED030, D_003ED280, D_003ED290,
    D_003ECED0;

/* add `n` stops to the search route: 60% the room's next point of interest (+0x1738 counts them
 * through), otherwise a random walkable triangle (+0x15DC of the entry: 1) */
void func_0027E5D0(Pursuer *p, s32 n) {
    u32 count = n & 0xFF;

    if ((s32)count > 0) {
        VObject *o = VCALL(gEvents, 0x64, VObject *(*)(VObject *))(gEvents);
        s32 *pts = VCALL(o, 0x38, s32 *(*)(VObject *))(o);
        u32 npts = 0, used = 0, i;

        if (pts != NULL) {
            while (pts[npts & 0xFF] != -1) {
                npts = (npts + 1) & 0xFF;
                if (npts >= 8) {
                    break;
                }
            }
        }
        for (i = 0; i < count; i = (i + 1) & 0xFF) {
            VObject *rnd = gRandom;
            u32 np = npts & 0xFF;

            if (PU(p, 0x1738, u32) >= np) {
                PU(p, 0x1738, u32) = 0;
            }
            if ((s32)(used & 0xFF) < (s32)np && 100.0f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd) <= 60.0f) {
                u32 k = PU(p, 0x1738, u32);

                PU(p, 0x1738, u32) = k + 1;
                func_00218C20(p, pts[k]);
                used = (used + 1) & 0xFF;
                PU(p, 0x15DC + PU(p, 0x1621, u8) * 8, u8) = 0;
            } else {
                func_00218C20(p, func_00214940(p));
                PU(p, 0x15DC + PU(p, 0x1621, u8) * 8, u8) = 1;
            }
        }
    }
}

/* vtable +0x1DC: wait at the door until it opens; make noise if Fiona holds it shut */
/* 0x0028DC40 */
void Pursuer_DoorWait(Pursuer *p) {
    VObject *d = gDoors;

    if (!(VCALL(d, 0x30, s32 (*)(VObject *, u32))(d, (u8)p->c.unk100) & 0xFF)) {
        if (p->c.unk104[0] == 0) {
            f32 a = VCALL(d, 0x64, f32 (*)(VObject *, u32))(d, (u8)p->c.unk100);

            if (a > -80.0f && a < -10.0f &&
                ((VCALL(d, 0x6C, s32 (*)(VObject *, s32, u32, f32 *))(d, 2, (u8)p->c.unk100, gCharPlayer->a.pos) & 0xFF) == 1 ||
                 (PursuerGroup_Fields(gProgress, (u8)p->c.unk100, 0) & 0xFF & 8))) {
                Relation_Request(gProgress, *(u8 *)&p->c.a.slot, 1, 5, 0, *(s16 *)&p->c.unk100, 20.0f);
            }
        }
        func_00125A10(&p->c);
        return;
    }
    if (p->c.moveSub == 0x15) {
        func_00212CA0(p, (u8)p->c.unk100);
    } else {
        func_00212D30(p, (u8)p->c.unk100);
    }
    p->c.moveMode = 0;
    Actor_SetState(&p->c.a, &D_003ECEF0);
    VCALL(p, 0x1E0, void (*)(Pursuer *))(p);
}

/* vtable +0x2A8: give up (unless +0x1664): stand, heal, behaviour by the stance */
/* 0x0027DDB0 */
void Pursuer_GiveUp(Pursuer *p) {
    if (PU(p, 0x1664, s32) == 0) {
        u8 *m = p->c.motion;

        if (AT(m, 0x55C, s32) == 0) {
            Pursuer_PlayAnim(p, 0);
        } else {
            func_002DDE20(m, 0, -1);
        }
        PU(p, 0x1624, s32) = 0;
        if (p->c.a.unkC4 == 2) {
            p->c.a.unkC4 = 0;
        }
        if (p->c.hp <= 0) {
            p->c.hp = p->c.hpMax;
        }
        PU(p, 0x1761, u8) = 0;
        PU(p, 0x1760, u8) = 0;
        if (PU(p, 0x16C8, u8) != 4) {
            PU(p, 0x16F6, u8) = 1;
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED550);
        } else {
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED540);
        }
        PU(p, 0x1758, s32) = -1;
    }
}

/* vtable +0x1B4: at the door, turn to it (within ~60 degrees) or step to its side */
/* 0x0028F650 */
void Pursuer_DoorFace(Pursuer *p) {
    s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

    if (((over ^ 1) & 0xFF) != 1) {
        u32 t = func_00213EC0(p, PU(p, 0x1568, f32), 0x1.0c1524p+0f /* 60 degrees */, 0x1.4f1a6ep+1f /* 150 degrees */) & 0xFF;

        if (t == 0xFF) {
            Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
        } else {
            func_00297300(p, t);
            PU(p, 0x1624, s32) = t & 0xFF;
        }
        func_00297C60(p);
        Actor_SetState(&p->c.a, &D_003ECE50);
        VCALL(p, 0x1B8, void (*)(Pursuer *))(p);
    }
}

/* vtable +0x194..: walk the search route (8 stops at most, then give up the search) */
/* 0x00290620 */
void Pursuer_SearchRoute(Pursuer *p) {
    void *nm = gNavMesh;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    for (;;) {
        if (PU(p, 0x1620, u8) < PU(p, 0x1621, u8)) {
            func_00218E70(p);
        } else {
            PU(p, 0x15A4, s32) = func_00214940(p);
            VCALL(nm, 0xC, void (*)(void *, s32, f32 *))(nm, PU(p, 0x15A4, s32), (f32 *)((u8 *)p + 0x15B0));
            func_00218C20(p, PU(p, 0x15A4, s32));
            PU(p, 0x15DC + PU(p, 0x1621, u8) * 8, u8) = 1;
        }
        if (VCALL(p, 0xD8, s32 (*)(Pursuer *))(p) != 0) {
            break;
        }
        if (func_00284440(p) != 0) {
            return;
        }
        PU(p, 0x1620, u8)++;
        if (PU(p, 0x1620, u8) >= 8) {
            s32 k;

            for (k = 0; k < 8; k++) {
                PU(p, 0x15E0 + k * 8, s32) = -1;
                PU(p, 0x15E4 + k * 8, u8) = 0;
            }
            PU(p, 0x1620, u8) = 0xFF;
            PU(p, 0x1621, u8) = 0xFF;
            PU(p, 0x1794, s32) = 0;
            PU(p, 0x16EF, u8) = 1;
            return;
        }
    }
    if (PU(p, 0x15E4 + PU(p, 0x1620, u8) * 8, u8) != 0) {
        if (PU(p, 0x1798, s32) == 0) {
            PU(p, 0x1798, s32) = 150;
        }
    } else {
        PU(p, 0x1798, s32) = 0;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECDF0);
    VCALL(p, 0x198, void (*)(Pursuer *))(p);
}

/* vtable +0x248: walked there; the stance animation */
/* 0x002854A0 */
void Pursuer_Arrived(Pursuer *p) {
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    Pursuer_PlayAnim(p, VCALL(p, 0x328, s32 (*)(Pursuer *))(p));
    Actor_SetState(&p->c.a, &D_003ED270);
    VCALL(p, 0x24C, void (*)(Pursuer *))(p);
}

/* vtable +0x244: follow the path (+0xE8) to the next exit on it */
/* 0x002858B0 */
void Pursuer_FollowPathExit(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if (VCALL(p, 0xE8, s32 (*)(Pursuer *))(p) & 0xFF) {
        if (func_00217260(p) & 0xFF) {
            if (!(VCALL(p, 0xD8, s32 (*)(Pursuer *))(p) & 0xFF)) {
                if (!(func_00284440(p) & 0xFF)) {
                    PU(p, 0x16EF, u8) = 1;
                }
                return;
            }
        } else {
            VObject *rm = gRooms;

            PU(p, 0x17B0, u8) = VCALL(rm, 0x3C, u32 (*)(VObject *, u32, s32))(rm, PU(p, 0x138C + p->c.unk1388 * 2, u16), p->c.a.room);
            PU(p, 0x15A4, s32) = VCALL(rm, 0x34, s32 (*)(VObject *, u32, f32 *))(rm, PU(p, 0x17B0, u8), (f32 *)((u8 *)p + 0x15B0));
            if (!(VCALL(p, 0xD8, s32 (*)(Pursuer *))(p) & 0xFF)) {
                if (!(func_00284440(p) & 0xFF)) {
                    PU(p, 0x16EF, u8) = 1;
                }
                return;
            }
            if (PU(p, 0x1590, f32) < 20.0f && !(PU(p, 0x1590, f32) < 0.0f)) {
                PURSUER_STEP_DONE(p) = 1;
                return;
            }
        }
        PU(p, 0x1784, s32) = 0;
        Actor_SetState(&p->c.a, &D_003ED250);
        VCALL(p, 0x248, void (*)(Pursuer *))(p);
        return;
    }
    func_0029B190(p);
    VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1D);
}

/* the search while out of sight: in the played room, catch up the route by the time spent away
 * (one stop per 150 frames); away, count the time the route still takes */
void func_0027EEA0(Pursuer *p) {
    if (func_00217510(p) != 0) {
        u32 t = PU(p, 0x17B4, u32);

        if (t != 0) {
            s32 left = PU(p, 0x1621, u8) - PU(p, 0x1620, u8);
            u32 steps = (u32)((f32)t / 150.0f) & 0xFF & 0xFF;

            if (left >= (s32)steps) {
                if ((s32)steps < left && steps != (u32)left) {
                    do {
                        PU(p, 0x1620, u8)++;
                    } while (steps != (u32)(PU(p, 0x1621, u8) - PU(p, 0x1620, u8)));
                }
            } else {
                func_0027E5D0(p, (steps - left) & 0xFF);
            }
            if ((s32)steps > 0 && steps != 0xFF) {
                PU(p, 0x1794, s32) = 1800;
            } else {
                PU(p, 0x1794, s32) = 0;
            }
            PU(p, 0x17B4, s32) = 0;
        } else {
            PU(p, 0x1620, u8) = PU(p, 0x1621, u8);
        }
        if (PU(p, 0x1620, u8) == PU(p, 0x1621, u8) && PU(p, 0x1621, u8) != 0xFF) {
            PU(p, 0x16C8, u8) = 3;
            VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
        }
    } else if (PU(p, 0x17B4, s32) == 0) {
        u8 k = PU(p, 0x16C8, u8);

        if (k == 3 || k == 2) {
            PU(p, 0x17B4, s32) = (PU(p, 0x1621, u8) - PU(p, 0x1620, u8)) * 150;
        }
    }
}

/* vtable +0x...: hurt: by the animation being played (0x1800 / 0x1804 / 0x1709: knocked down) */
void func_00287950(Pursuer *p) {
    s32 a;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    AT(gProgress, 0x874, u8) = 0;
    PU(p, 0x1784, s32) = 0;
    a = MOTION_ANIM(p);
    if (a != 0x1804 && a != 0x1800 && a != 0x1709) {
        Actor_SetState(&p->c.a, &D_003ED1D0);
        Pursuer_HitReact(p);
    } else {
        Actor_SetState(&p->c.a, &D_003ED1C0);
        func_00288150(p);
    }
}

/* vtable +0x1B8: facing the door; open it (side +0x104) if it is ours to open */
/* 0x0028E2D0 */
void Pursuer_DoorFacing(Pursuer *p) {
    if (func_002140A0(p, PU(p, 0x1568, f32), VCALL(p, 0xA0, f32 (*)(Pursuer *))(p)) == 0.0f) {
        Progress *pr = gProgress;

        if ((Progress_CurRoomFlag(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1 &&
            (PursuerGroup_Fields(pr, (u8)p->c.unk100, 0) & 0xFF & 4)) {
            Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
        } else {
            if (func_00178980(pr, p->c.a.room, (u8)p->c.unk100) != 0) {
                PURSUER_STEP_DONE(p) = 1;
                return;
            }
            PURSUER_STEP_NEXT(p) = 0;
            Actor_SetState(&p->c.a, &D_003ECEC0);
        }
    }
}

/* stance by where Fiona is (vtable +0xC0 sees her -> 0 chase; a different side -> 1) */
void func_0027FE90(Pursuer *p) {
    if (AT(gProgress, 0x1FBEC1, u8) != 0) {
        if (PU(p, 0x16C8, u8) == 4 || p->c.a.room != gCharPlayer->a.room) {
            return;
        }
    } else if (PU(p, 0x16C8, u8) == 4 || func_00217510(p) == 0) {
        return;
    }
    PU(p, 0x1574, f32) = func_002E2D00(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
    PU(p, 0x1544, u8) = VCALL(p, 0xC0, s32 (*)(Pursuer *))(p);
    if (PU(p, 0x1544, u8) == 1) {
        VCALL(p, 0xCC, void (*)(Pursuer *))(p);
        PU(p, 0x16C8, u8) = 0;
        VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
    }
    if (PU(p, 0x16C8, u8) == 0) {
        s32 side = func_00217340(p);

        if (side != func_002172F0(p, gCharPlayer)) {
            PU(p, 0x16C8, u8) = 1;
            VCALL(p, 0xB0, void (*)(Pursuer *))(p);
        }
    }
}

/* vtable +0x18C */
/* 0x0028FD30 */
void Pursuer_Move18C(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECE10);
    VCALL(p, 0x190, void (*)(Pursuer *))(p);
}

/* vtable +0x17C */
/* 0x00291A20 */
void Pursuer_Move17C(Pursuer *p) {
    PU(p, 0x16EC, u8) = 1;
    PURSUER_STEP_NEXT(p) = 1;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    if (PU(p, 0x1788, s32) != 0) {
        Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECD90);
    VCALL(p, 0x180, void (*)(Pursuer *))(p);
}

/* walked up: face Fiona (stand animation) */
void func_0028AEC0(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
    p->target = gCharPlayer;
    p->c.unk104[0] = 0;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED030);
    func_0028AB10(p);
}

/* vtable +0x250: to the next exit of the path; look through it first if it's where Fiona was */
/* 0x00285150 */
void Pursuer_NextPathExit(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if (p->c.unk1388 < p->c.unk1384 && func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) != -1) {
        VObject *rm = gRooms;

        PU(p, 0x17B0, u8) = VCALL(rm, 0x3C, u32 (*)(VObject *, u32, s32))(rm, PU(p, 0x138C + p->c.unk1388 * 2, u16), p->c.a.room);
        PU(p, 0x15A4, s32) = VCALL(rm, 0x34, s32 (*)(VObject *, u32, f32 *))(rm, PU(p, 0x17B0, u8), (f32 *)((u8 *)p + 0x15B0));
        if (func_00212190(p, PU(p, 0x17B0, u8)) != 0) {
            u32 t = func_00213EC0(p, func_00212730(p, PU(p, 0x17B0, u8)), 0x1.0c1524p+0f /* 60 degrees */, 0x1.4f1a6ep+1f /* 150 degrees */) & 0xFF;

            if (t == 0xFF) {
                PURSUER_STEP_DONE(p) = 1;
            } else {
                p->c.unk104[0] = t;
                Actor_SetState(&p->c.a, &D_003ED280);
            }
        } else {
            if (!(VCALL(p, 0xD8, s32 (*)(Pursuer *))(p) & 0xFF)) {
                if (!(func_00284440(p) & 0xFF)) {
                    PU(p, 0x16EF, u8) = 1;
                }
                return;
            }
            PU(p, 0x1784, s32) = 0;
            Actor_SetState(&p->c.a, &D_003ED290);
            VCALL(p, 0x198, void (*)(Pursuer *))(p);
        }
    } else {
        PU(p, 0x16EF, u8) = 1;
    }
}

/* vtable +0x1D4 */
/* 0x0028E0C0 */
void Pursuer_Door1E4(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    if (PU(p, 0x1788, s32) != 0x200) {
        Pursuer_PlayAnim(p, VCALL(p, 0x324, s32 (*)(Pursuer *))(p));
    }
    PU(p, 0x1624, s32) = 0;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECED0);
    VCALL(p, 0x1D8, void (*)(Pursuer *))(p);
}

/* ---- batch 8 ---- */

extern const PTMF D_003ED260, D_003ECE30, D_003ED5B0, D_003ECD70, D_003ED130, D_003ED580, D_003ECD80;
extern PTMF D_0045B3A0;   /* followed by the idle move: +0xC kind, +0x10 move mode, +0x14 sub */

/* vtable +0x254: on to the next exit (of the path, or a random one) */
/* 0x002856A0 */
void Pursuer_OnToNextExit(Pursuer *p) {
    VObject *rm;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if (p->c.unk1388 < p->c.unk1384 && func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) != -1) {
        PU(p, 0x17B0, u8) = VCALL(gRooms, 0x3C, u32 (*)(VObject *, u32, s32))(gRooms, PU(p, 0x138C + p->c.unk1388 * 2, u16), p->c.a.room);
    } else {
        PU(p, 0x17B0, u8) = func_00212A80(p, p->c.door);
    }
    PU(p, 0x15A4, s32) = VCALL(gRooms, 0x34, s32 (*)(VObject *, u32, f32 *))(gRooms, PU(p, 0x17B0, u8), (f32 *)((u8 *)p + 0x15B0));
    if (!(VCALL(p, 0xD8, s32 (*)(Pursuer *))(p) & 0xFF)) {
        u32 node;

        if (func_00284440(p) & 0xFF) {
            return;
        }
        rm = gRooms;
        PU(p, 0x17B0, u8) = p->c.door;
        PU(p, 0x15A4, s32) = VCALL(rm, 0x34, s32 (*)(VObject *, u32, f32 *))(rm, PU(p, 0x17B0, u8), (f32 *)((u8 *)p + 0x15B0));
        VCALL(p, 0xD8, s32 (*)(Pursuer *))(p);
        node = VCALL(rm, 0x10, u32 (*)(VObject *, s32, u32))(rm, p->c.a.room, p->c.door) & 0xFFFF;
        p->c.unk148C[node >> 5] |= 1 << (node & 0x1F);
    }
    if (PU(p, 0x1590, f32) < 20.0f && !(PU(p, 0x1590, f32) < 0.0f)) {
        PURSUER_STEP_DONE(p) = 1;
        return;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED260);
    VCALL(p, 0x248, void (*)(Pursuer *))(p);
}

/* vtable +0x1AC: line up at the door +0x100 (side by which way it opens) */
/* 0x0028FA00 */
void Pursuer_DoorLineUp(Pursuer *p) {
    f32 dir[4] __attribute__((aligned(16)));
    VObject *d = gDoors;
    s8 r = VCALL(d, 0x18, s32 (*)(VObject *, u32, f32 *))(d, (u8)p->c.unk100, p->c.a.pos);

    if (r == -1) {
        p->c.unk100 = -1;
        PU(p, 0x16EF, u8) = 1;
        return;
    }
    if ((func_00178980(gProgress, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1) {
        p->c.unk104[0] = (r != 0 ? 3 : 1) & 0xFFFF;
    } else {
        p->c.unk104[0] = (r != 0 ? 2 : 0) & 0xFFFF;
    }
    PU(p, 0x15C4, s32) = VCALL(d, 0x14, s32 (*)(VObject *, u32, s32, f32 *, f32 *, s32))(d, (u8)p->c.unk100, p->c.unk104[0], (f32 *)((u8 *)p + 0x15D0), dir, 1);
    if ((VCALL(p, 0xDC, s32 (*)(Pursuer *))(p) & 0xFF) != 1) {
        p->c.unk100 = -1;
        p->c.unk104[0] = -1;
        PU(p, 0x16EF, u8) = 1;
        return;
    }
    PU(p, 0x1568, f32) = dir[1];
    if (func_00124490(&p->c.a, (f32 *)((u8 *)p + 0x15D0)) < 0x1.99999a0000000p-4f /* 0.1 */) {
        f32 d2 = p->c.a.angle[1] - PU(p, 0x1568, f32);

        if (d2 <= 0.0f) {
            d2 = -d2;
        }
        if (d2 < VCALL(p, 0xA0, f32 (*)(Pursuer *))(p)) {
            sceVu0CopyVector(p->c.a.pos, (f32 *)((u8 *)p + 0x15D0));
            p->c.a.angle[1] = PU(p, 0x1568, f32);
        } else {
            VCALL(p, 0x128, void (*)(Pursuer *))(p);
        }
    } else {
        VCALL(p, 0x128, void (*)(Pursuer *))(p);
    }
    Actor_SetState(&p->c.a, &D_003ECE30);
}

/* vtable +0x...: back onto the walk mesh (to the exit or the way in) */
void func_0027FC70(Pursuer *p) {
    f32 h;

    if (p->c.a.navTri == (u32)-1) {
        if (PU(p, 0x17AC, s32) != 4 && p->c.door != 0xFF) {
            if (*(f32 *)&p->c.unk14C4 <= 80.0f && PU(p, 0x17B0, u8) != 0xFF) {
                p->c.a.navTri = VCALL(gRooms, 0x28, u32 (*)(VObject *, u32))(gRooms, PU(p, 0x17B0, u8));
                h = func_00212730(p, PU(p, 0x17B0, u8));
            } else {
                p->c.a.navTri = VCALL(gRooms, 0x28, u32 (*)(VObject *, u32))(gRooms, p->c.door);
                h = func_002E2D00(0x1.921fb60000000p+1f /* 3.1415927 */ + func_00212730(p, p->c.door));
            }
            VCALL(p, 0x28, s32 (*)(Pursuer *, u32, f32 *, s32))(p, p->c.a.navTri, &h, 0);
        } else {
            func_00124890(&p->c.a, func_00217340(p));
        }
    } else {
        s32 k = PU(p, 0x17AC, s32);

        if ((k == 5 || k == 0 || k == 1 || k == 3) && *(f32 *)&p->c.unk14C4 <= 80.0f && PU(p, 0x17B0, u8) != 0xFF) {
            p->c.a.navTri = VCALL(gRooms, 0x28, u32 (*)(VObject *, u32))(gRooms, PU(p, 0x17B0, u8));
            h = func_00212730(p, PU(p, 0x17B0, u8));
            VCALL(p, 0x28, s32 (*)(Pursuer *, u32, f32 *, s32))(p, p->c.a.navTri, &h, 0);
            return;
        }
        if (VCALL(p, 0x28, s32 (*)(Pursuer *, u32, f32 *, s32))(p, p->c.a.navTri, NULL, 0) == -1) {
            func_00124890(&p->c.a, -1);
        }
    }
}

extern u8 D_003AF270[], D_0041B610[], D_00413570[], D_0042E4E0[], D_003D73D0[], D_00414860[],
    D_00422400[], D_004224E0[], D_00444B30[], D_0041A610[], D_00423B90[], D_00419E30[], D_00429790[],
    D_00429820[], D_00429CB0[], D_00429D40[], D_00429DD0[], D_0042A150[], D_0042C960[], D_0042C8D0[],
    D_0042A360[], D_00441870[], D_0042C3E0[], D_0042C700[], D_0042C9F0[], D_0042F4D0[], D_00430880[],
    D_00430910[], D_004309A0[], D_00430A70[], D_0043B610[], D_00443530[];

/* vtable +0xFC: the motion / message files of character id +0x153C (slot 2: vtable +0xFC) */
/* 0x0029F690 */
u8 *Pursuer_MotionFiles(Pursuer *p) {
    if (p->c.a.slot != 2) {
        switch (p->c.unk153C) {
        case 2: return D_003AF270;
        case 3: case 34: case 35: case 36: return D_003D73D0;
        case 4: return D_00414860;
        case 6: return D_0041B610;
        case 7: return D_00413570;
        case 8: return D_00419E30;
        case 9: return D_00422400;
        case 10: return D_004224E0;
        case 11: return D_0041A610;
        case 12: return D_00423B90;
        case 13: return D_00429790;
        case 14: case 15: return D_00429820;
        case 16: return D_00429CB0;
        case 17: return D_00429D40;
        case 18: return D_00429DD0;
        case 19: return D_0042A150;
        case 20: return D_0042C960;
        case 21: case 22: return D_0042C8D0;
        case 23: return D_0042A360;
        case 24: return D_0042C3E0;
        case 25: return D_0042C700;
        case 26: return D_0042C9F0;
        case 27: return D_0042E4E0;
        case 28: return D_0042F4D0;
        case 29: return D_00430880;
        case 30: return D_00430910;
        case 31: return D_004309A0;
        case 32: return D_00430A70;
        case 33: return D_0043B610;
        case 37: return D_00441870;
        case 38: return D_00443530;
        case 39: return D_00444B30;
        default: return NULL;
        }
    }
    return VCALL(p, 0xFC, u8 *(*)(Pursuer *))(p);
}

/* sidestep around the target at the angle +0x104 (5 degree units), back and forth up to 30
 * units, both ways; done when blocked twice or after 120 frames */
void func_00289050(Pursuer *p) {
    s32 flipped = 0;

    for (;;) {
        f32 v[4] __attribute__((aligned(16)));
        f32 a = (f32)p->c.unk104[0] * 0x1.6571860000000p-4f /* 0.08726647 */;
        f32 r = func_00124490(&p->c.a, p->target->a.pos);
        u32 tri;
        s32 ok, clear;

        sceVu0SubVector(v, p->c.a.pos, p->target->a.pos);
        Vec_TurnY(v, v, a);
        sceVu0Normalize(v, v);
        sceVu0ScaleVector(v, v, r);
        sceVu0AddVector(v, v, p->target->a.pos);
        tri = func_00124480(&p->c.a, v, 0);
        ok = tri != (u32)-1 && func_00214A90(p, tri) == 0;
        clear = func_00122C90(&p->c.a, tri, p->target->a.navTri, v, p->target->a.pos, 0) & 0xFF;
        if (!(ok & 0xFF) || !(clear & 0xFF)) {
            if (!(flipped & 0xFF)) {
                PU(p, 0x1634, f32) = 0.0f;
                flipped = 1;
                p->c.unk104[0] = -p->c.unk104[0];
                continue;
            }
            PURSUER_STEP_DONE(p) = 1;
            return;
        }
        func_002143D0(p, v);
        PU(p, 0x1634, f32) += func_00124490(&p->c.a, p->c.a.prevPos);
        if (PU(p, 0x1634, f32) <= 30.0f) {
            if (PU(p, 0x1784, u32) >= 121) {
                PU(p, 0x1634, f32) = 0.0f;
                p->c.unk104[0] = -1;
                PURSUER_STEP_DONE(p) = 1;
            }
            return;
        }
        PU(p, 0x1634, f32) = 0.0f;
        p->c.unk104[0] = -p->c.unk104[0];
        return;
    }
}

/* vtable +0x174: knock at / try the door: wait 1..3 s, a door sound (table +0x16B0 by chance,
 * else 0x21), noise if it's locked */
/* 0x0027CEF0 */
void Pursuer_KnockAtDoor(Pursuer *p) {
    f32 pos[4] __attribute__((aligned(16)));
    VObject *rnd = gRandom;
    VObject *rm = gRooms;
    s16 sound;
    u32 exit;
    Progress *pr;

    PU(p, 0x1624, s32) = (s32)(30.0f * (2.0f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd))) + 60;
    exit = VCALL(rm, 0x14, u32 (*)(VObject *, s32, u32))(rm, p->c.a.room, PU(p, 0x17B0, u8)) & 0xFF;
    VCALL(rm, 0x30, u32 (*)(VObject *, u32, f32 *))(rm, exit, pos);
    sound = 0x21;
    if (PU(p, 0x16B0, u8 *) != NULL) {
        u8 *t = PU(p, 0x16B0, u8 *);
        f32 roll = 100.0f * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd);
        u32 i;

        for (i = 0;; i = (i + 1) & 0xFF) {
            s32 hit = roll <= AT(t + (i & 0xFF) * 8, 0x4, f32);

            if ((hit ^ 1) == 0) {
                break;
            }
        }
        sound = AT(t + (i & 0xFF) * 8, 0x0, s16);
    }
    if (VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(gProgress, 8) == 0) {
        func_00122C20(&p->c.a, sound, 7, 0, 0, pos);
    }
    pr = gProgress;
    if (PursuerGroup_Fields(pr, exit, 0) & 0xFF & 4) {
        Relation_Request(pr, *(u8 *)&p->c.a.slot, 1, 6, 0, 2, 5.0f);
    }
    ptmf_set((PTMF *)((u8 *)p + 0x17A0), &D_003ED5B0);
    VCALL(p, 0x178, void (*)(Pursuer *))(p);
}

/* vtable +0x...: walk, then play the animation +0x104 (-1: done) */
void func_00291EE0(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (p->c.unk104[0] < 0) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
        return;
    }
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    Pursuer_PlayAnim(p, p->c.unk104[0]);
    p->c.unk104[0] = -1;
    PURSUER_STEP_NEXT(p) = 1;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECD70);
    if (((MOTION_KEYS(p) & MOTION_KEY_END) != 0) == 1) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    } else {
        func_00125A10(&p->c);
    }
}

/* vtable +0x...: pace (stance 0 chasing: 1.0..1.2, searching 0.6, else 1.0; 2.0 with
 * progress flag 9), then the exits along the way */
/* 0x0027D5B0 */
void Pursuer_Pace(Pursuer *p) {
    Progress *pr;

    if (PU(p, 0x1664, s32) != 0 || PU(p, 0x17B4, s32) != 0) {
        return;
    }
    if (PU(p, 0x16C8, u8) == 4) {
        s32 room = p->c.a.room;
        s32 *e = VCALL(p, 0x314, s32 *(*)(Pursuer *))(p);

        for (; *e != -1; e += 2) {
            if (room == *e) {
                return;
            }
        }
    }
    pr = gProgress;
    if (Progress_TestFlag(pr, 9) == 0) {
        switch (PU(p, 0x16C8, u8)) {
        case 0:
            func_001272B0(&p->c, 1.0f + 0x1.99999a0000000p-3f /* 0.2 */ * VCALL(gRandom, 0x18, f32 (*)(VObject *))(gRandom));
            break;
        case 2:
        case 3:
            func_001272B0(&p->c, 0x1.3333340000000p-1f /* 0.6 */);
            break;
        case 1:
        case 4:
            func_001272B0(&p->c, 1.0f);
            break;
        }
    } else {
        func_001272B0(&p->c, 2.0f);
    }
    if (!(p->c.unk128 < p->c.unk124)) {
        s32 room = VCALL(gRooms, 0x18, s32 (*)(VObject *, s32, u32))(gRooms, p->c.a.room, PU(p, 0x17B0, u8));

        if (room == VCALL(pr, 0xC, s32 (*)(Progress *))(pr) && (func_00211E00(p, PU(p, 0x17B0, u8)) & 0xFF) == 5 &&
            PU(p, 0x16C8, u8) != 4) {
            PURSUER_STEP_DONE(p) = 1;
            return;
        }
        if (func_0027F0A0(p, PU(p, 0x17B0, u8)) & 0xFF) {
            func_002815E0(p, PU(p, 0x17B0, u8));
        }
    }
}

/* vtable +0x...: walk, then the 0x404 animation, turning to the target */
void func_002895B0(Pursuer *p) {
    f32 h, d;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    Pursuer_PlayAnim(p, 0x404);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED130);
    h = func_001244D0(&p->c.a, p->target->a.pos);
    d = func_002140A0(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    if (d <= 0.0f) {
        d = -d;
    }
    if (d < 0x1.921fb60000000p-1f /* 0.7853982 */) {
        func_00125A10(&p->c);
    }
}

/* vtable +0x16C: plan the way to the goal room (+0x1594), first exit to +0x17B0, its path
 * length to +0x14C4; none: the idle move */
/* 0x0027D8D0 */
void Pursuer_PlanToGoal(Pursuer *p) {
    PURSUER_STEP_NEXT(p) = 1;
    p->c.unk124 = p->c.unk128;
    if (p->c.unk1388 >= p->c.unk1384) {
        u32 i;

        PU(p, 0x1598, s32) = -1;
        for (i = 0; i < 13; i++) {
            p->c.unk148C[i] = 0;
        }
        switch (PU(p, 0x16C9, u8)) {
        case 1:
        case 3:
        case 4:
        case 5:
            PU(p, 0x16C9, u8) = 0;
            PU(p, 0x16CB, u8) = 0;
            PU(p, 0x16CA, u8) = 0;
            PU(p, 0x16CC, u8) = 0;
            PU(p, 0x179C, s32) = -1;
            PU(p, 0x16F1, u8) = 0;
            PU(p, 0x16F3, u8) = 0;
            PU(p, 0x16F2, u8) = 0;
            PU(p, 0x16F4, u8) = 0;
            break;
        }
        if (!(func_0027CA00(p) & 0xFF)) {
            ptmf_set((PTMF *)((u8 *)p + 0x17A0), &D_0045B3A0);
            PU(p, 0x17AC, s32) = (&AT(&D_0045B3A0, 0xC, s32))[0];
            p->c.moveMode = AT(&D_0045B3A0, 0x10, s32);
            p->c.moveSub = AT(&D_0045B3A0, 0x14, s32);
            p->c.unk1530 = 0;
            p->c.unk1538 = 0;
            p->c.unk1534 = 0;
            if (PU(p, 0x16C8, u8) != 2) {
                PU(p, 0x16F4, u8) = 1;
            }
            return;
        }
    } else if (func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) <= 0) {
        p->c.unk1384 = p->c.unk1388;
        PU(p, 0x16F4, u8) = 1;
        return;
    }
    PU(p, 0x17B0, u8) = VCALL(gRooms, 0x3C, u32 (*)(VObject *, u32, s32))(gRooms, PU(p, 0x138C, u16), p->c.a.room);
    p->c.unk14C0 = PU(p, 0x138C, u16);
    *(f32 *)&p->c.unk14C4 = func_00212060(p, p->c.a.room, PU(p, 0x17B0, u8), p->c.door);
    p->c.unk1530 = 0;
    p->c.unk1538 = 0;
    p->c.unk1534 = 0;
    ptmf_set((PTMF *)((u8 *)p + 0x17A0), &D_003ED580);
    PU(p, 0x1784, s32) = 0;
}

/* vtable +0x...: walk, then play the gesture +0x104 of the table +0x1720 ({anim, wait} 8 bytes) */
void func_00291C80(Pursuer *p) {
    u8 *e;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (p->c.unk104[0] < 0) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
        return;
    }
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    e = PU(p, 0x1720, u8 *) + p->c.unk104[0] * 8;
    Pursuer_PlayAnim(p, AT(e, 0x0, s32));
    if (AT(e, 0x4, u8) != 0) {
        PURSUER_STEP_NEXT(p) = 1;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECD80);
    if (((MOTION_KEYS(p) & MOTION_KEY_END) != 0) == 1) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    } else {
        func_00125A10(&p->c);
    }
}

/* ---- batch 9 ---- */

extern const PTMF D_003ED020, D_003ECE90, D_003ECA00, D_003ECA10, D_003ECA20, D_003ED560, D_003ED570,
    D_003ECC30, D_003ECDE0, D_003ECDD0, D_003ED140;
extern PTMF D_0045B358;   /* followed by the wait move: +0xC kind, +0x10 move mode, +0x14 sub */

/* vtable +0x70: placed into room `room` (triangle `tri`, -1: by its exit / way in), on side
 * `side` (0 / 1; else any usable exit) */
/* 0x0029D180 */
s32 Pursuer_PlaceInRoom(Pursuer *p, s32 room, s32 tri, u32 side) {
    VObject *rm;
    u32 i;

    ((void (*)(Character *))func_00125BA0)(&p->c);   /* the original passes only the character */
    p->c.a.room = room;
    rm = gRooms;
    if (side < 2) {
        for (i = 0; i < 8; i = (i + 1) & 0xFF) {
            if (VCALL(rm, 0x74, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, i) != 0 && (func_00211E00(p, i) & 0xFF) != 3 &&
                side == (u32)VCALL(rm, 0x50, s32 (*)(VObject *, s32, u32, s32))(rm, p->c.a.room, i, 1)) {
                p->c.door = i;
                break;
            }
        }
        if ((i & 0xFF) >= 8) {
            p->c.door = 0;
        }
    } else {
        s32 more;

        i = 0;
        do {
            if (VCALL(rm, 0x74, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, i) != 0 && (func_00211E00(p, i) & 0xFF) != 3) {
                p->c.door = i;
            }
            i = (i + 1) & 0xFF;
            more = i < 8;
        } while (more != 0);
        if (more == 0) {
            p->c.door = 0;
        }
    }
    if (PU(p, 0x16C8, u8) == 4) {
        PU(p, 0x16C8, u8) = 3;
        VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
    }
    VCALL(p, 0x7C, void (*)(Pursuer *))(p);
    if (func_00217510(p) != 0) {
        if (tri != -1) {
            VCALL(p, 0x28, s32 (*)(Pursuer *, s32, f32 *, s32))(p, tri, NULL, 0);
        } else {
            func_00124890(&p->c.a, func_00217340(p));
        }
        func_002DDE20(p->c.motion, 0, -1);
        p->c.a.unk2A = 0;
    } else {
        p->c.a.disabled = 1;
        p->c.a.unk2A = 1;
        p->c.a.unk2B = 0;
        p->c.a.unk2D = 0;
        PU(p, 0x1544, u8) = 0;
        PU(p, 0x1545, u8) = 0;
        p->c.a.navTri = tri;
    }
    p->c.unk1530 = 0;
    p->c.unk1538 = 0;
    p->c.unk1534 = 0;
    func_0027EAF0(p);
    return 0;
}

extern u8 D_00419DF0[], D_00423B50[], D_00429750[], D_004297E0[], D_00429C70[], D_00429D00[],
    D_00429D90[], D_0042A110[], D_0042C920[], D_0042C890[], D_0042C3A0[], D_0042C6C0[], D_0042C9B0[],
    D_0042F490[], D_00430840[], D_004308D0[], D_00430960[], D_00430A30[], D_0043B5D0[], D_004434F0[];

/* vtable +0xF8: the model files of character id +0x153C (slot 2: vtable +0xF8) */
/* 0x0029F8C0 */
u8 *Pursuer_ModelFiles(Pursuer *p) {
    if (p->c.a.slot != 2) {
        switch (p->c.unk153C) {
        case 2: return func_0012BFB0(p);
        case 3: return func_0020D620(p);
        case 4: return func_002DC460(p);
        case 6: return func_002FC8F0(p);
        case 7: return func_002CF140(p);
        case 8: return D_00419DF0;
        case 9: return func_00309410(p);
        case 10: return func_0030C1B0(p);
        case 11: return func_002F9000(p);
        case 12: return D_00423B50;
        case 13: return D_00429750;
        case 14: case 15: return D_004297E0;
        case 16: return D_00429C70;
        case 17: return D_00429D00;
        case 18: return D_00429D90;
        case 19: return D_0042A110;
        case 20: return D_0042C920;
        case 21: case 22: return D_0042C890;
        case 23: return func_00320150(p);
        case 24: return D_0042C3A0;
        case 25: return D_0042C6C0;
        case 26: return D_0042C9B0;
        case 27: return func_00331200(p);
        case 28: return D_0042F490;
        case 29: return D_00430840;
        case 30: return D_004308D0;
        case 31: return D_00430960;
        case 32: return D_00430A30;
        case 33: return D_0043B5D0;
        case 34: return func_00347290(p);
        case 35: return func_00348620(p);
        case 36: return func_003495B0(p);
        case 37: return func_0034D980(p);
        case 38: return D_004434F0;
        case 39: return func_00365D10(p);
        default: return NULL;
        }
    }
    return VCALL(p, 0xF8, u8 *(*)(Pursuer *))(p);
}

/* the attack's active frames: everyone in its reach (entry +0xC) is hit (+0x1760 bits, the
 * hit reported with the entry's kind +0x10, sound +0x12, +0x4 and strength +0x14); after it
 * the next attack step */
void func_0028B0D0(Pursuer *p) {
    u8 *e = PU(p, 0x171C, u8 *) + PU(p, 0x1724, s8 *)[PU(p, 0x1728, s32) * 4 + PU(p, 0x172C, s8)] * 0x24;

    if (func_001F4770(p->c.motion, 0, -1, 1) & 0xFF & 2) {
        Progress *pr = gProgress;

        if (!(Progress_TestFlag(pr, 0xE) & 0xFF)) {
            u32 i;

            for (i = 0; i < 3; i = (i + 1) & 0xFF) {
                u32 s = i & 0xFF;
                Character *c = gCharacters[s];

                if (c != NULL && s != (u32)p->c.a.slot && c->a.active != 0 && func_00124490(&p->c.a, c->a.pos) < AT(e, 0xC, f32)) {
                    PU(p, 0x1760, u8) |= (1 << s) & 0xFF;
                }
            }
            if (PU(p, 0x1760, u8) != 0) {
                Relation_Request(pr, (u8)p->c.a.slot, PU(p, 0x1760, u8), AT(e, 0x10, u8), AT(e, 0x12, u16), AT(e, 0x4, s16), AT(e, 0x14, f32));
            }
        }
    }
    if (((MOTION_KEYS(p) & MOTION_KEY_END) != 0) == 1) {
        if (PU(p, 0x1760, u8) != 0) {
            s32 *t = PU(p, 0x1748, s32 *);

            PU(p, 0x178C, s32) = t != NULL ? t[2] : 30;
        }
        PU(p, 0x172C, s8)++;
        Actor_SetState(&p->c.a, &D_003ED020);
        return;
    }
    if (AT(e, 0x1C, u8) != 0) {
        f32 h = func_001244D0(&p->c.a, p->target->a.pos);

        func_002140A0(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    }
    func_00125A10(&p->c);
}

/* vtable +0x1C0: pushing at the door +0x100 (held shut from Fiona's side: noise) */
/* 0x0028ED50 */
void Pursuer_DoorPush(Pursuer *p) {
    VObject *d;
    u32 tri = p->c.a.navTri;
    u8 *e;
    s32 side;

    e = tri < AT(gNavMesh, 0x8, u32) && AT(gNavMesh, 0x4, u8 *) != NULL ? AT(gNavMesh, 0x4, u8 *) + tri * 0x50 : NULL;
    p->c.a.unk2A = (AT(e, 0x3C, u32) & 0x20000) ? 1 : 0;
    d = gDoors;
    side = VCALL(d, 0x18, s32 (*)(VObject *, u32, f32 *))(d, (u8)p->c.unk100, gCharPlayer->a.pos);
    if (VCALL(d, 0x30, s32 (*)(VObject *, u32))(d, (u8)p->c.unk100) & 0xFF) {
        if (p->c.moveSub == 0x15) {
            func_00212CA0(p, (u8)p->c.unk100);
        } else {
            func_00212D30(p, (u8)p->c.unk100);
        }
        p->c.moveMode = 0;
        Actor_SetState(&p->c.a, &D_003ECE90);
        VCALL(p, 0x1C4, void (*)(Pursuer *))(p);
        return;
    }
    if (side == 1 && (p->c.unk104[0] == 2 || p->c.unk104[0] == 0)) {
        f32 a = VCALL(d, 0x64, f32 (*)(VObject *, u32))(d, (u8)p->c.unk100);

        if (a > -80.0f && a < -10.0f &&
            ((VCALL(d, 0x6C, s32 (*)(VObject *, s32, u32, f32 *))(d, 2, (u8)p->c.unk100, gCharPlayer->a.pos) & 0xFF) == 1 ||
             (PursuerGroup_Fields(gProgress, (u8)p->c.unk100, 0) & 0xFF & 8))) {
            Relation_Request(gProgress, *(u8 *)&p->c.a.slot, 1, 5, 0, *(s16 *)&p->c.unk100, 20.0f);
        }
    }
    func_00125A10(&p->c);
}

/* vtable +0x2BC: chase Fiona in the played room (behaviour 0xC), or the wait move elsewhere */
void func_0029AF20(Pursuer *p) {
    if (p->c.a.unkC4 == 2) {
        p->c.a.unkC4 = 0;
        PU(p, 0x1664, s32) = 0;
    }
    if (func_00217510(p) != 0) {
        if (PURSUER_STEP_NEXT(p) == 1 && p->c.unkE0 == 0) {
            if (PU(p, 0x175C, s32) == 0xC) {
                ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECA00);
                PU(p, 0x1758, s32) = -1;
                VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0xC);
            } else {
                ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECA10);
                PU(p, 0x1758, s32) = -1;
            }
        }
        PU(p, 0x16C8, u8) = 2;
        VCALL(p, 0x2C0, void (*)(Pursuer *))(p);
        PU(p, 0x1594, s32) = p->c.a.room;
        func_0027E790(p);
    } else {
        p->c.unk1388 = p->c.unk1384;
        ptmf_set((PTMF *)((u8 *)p + 0x17A0), &D_0045B358);
        PU(p, 0x17AC, s32) = AT(&D_0045B358, 0xC, s32);
        p->c.moveMode = AT(&D_0045B358, 0x10, s32);
        p->c.moveSub = AT(&D_0045B358, 0x14, s32);
        p->c.unk1530 = 0;
        p->c.unk1538 = 0;
        p->c.unk1534 = 0;
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECA20);
        PU(p, 0x1758, s32) = -1;
        PU(p, 0x16C8, u8) = 1;
        VCALL(p, 0xB0, void (*)(Pursuer *))(p);
        PURSUER_STEP_NEXT(p) = 1;
    }
    PU(p, 0x16C9, u8) = 0;
    PU(p, 0x16CB, u8) = 0;
    PU(p, 0x16CA, u8) = 0;
    PU(p, 0x16CC, u8) = 0;
    PU(p, 0x179C, s32) = -1;
    PU(p, 0x16F1, u8) = 0;
    PU(p, 0x16F3, u8) = 0;
    PU(p, 0x16F2, u8) = 0;
    PU(p, 0x16F4, u8) = 0;
}

/* vtable +0x168: plan the way on (Fiona's room / the search), path length to the next exit */
/* 0x0027DB30 */
void Pursuer_PlanWayOn(Pursuer *p) {
    VObject *rm;
    u32 node;

    PURSUER_STEP_NEXT(p) = 1;
    p->c.unk124 = p->c.unk128;
    if (p->c.unk1388 >= p->c.unk1384 && PU(p, 0x16C8, u8) == 2) {
        u8 k;

        PU(p, 0x1598, s32) = -1;
        k = PU(p, 0x16C9, u8);
        if (k == 2 || k == 4 || k == 1 || k == 5) {
            PU(p, 0x16C9, u8) = 0;
            PU(p, 0x16CB, u8) = 0;
            PU(p, 0x16CA, u8) = 0;
            PU(p, 0x16CC, u8) = 0;
            PU(p, 0x179C, s32) = -1;
            PU(p, 0x16F1, u8) = 0;
            PU(p, 0x16F3, u8) = 0;
            PU(p, 0x16F2, u8) = 0;
            PU(p, 0x16F4, u8) = 0;
        }
    }
    rm = gRooms;
    node = VCALL(rm, 0x10, u32 (*)(VObject *, s32, u32))(rm, p->c.a.room, p->c.door) & 0xFFFF;
    if ((func_0027CA00(p) & 0xFF) != 1) {
        u32 i;

        for (i = 0; i < 13; i++) {
            p->c.unk148C[i] = 0;
        }
        if (func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) == -1) {
            PU(p, 0x17B0, u8) = VCALL(rm, 0x3C, u32 (*)(VObject *, u32, s32))(rm, PU(p, 0x138C, u16), p->c.a.room);
            func_0029AC50(p);
            return;
        }
        *(f32 *)&p->c.unk14C4 = func_00211F70(p, p->c.a.room, node, PU(p, 0x138C, u16));
        PU(p, 0x17B0, u8) = VCALL(rm, 0x3C, u32 (*)(VObject *, u32, s32))(rm, PU(p, 0x138C, u16), p->c.a.room);
        ptmf_set((PTMF *)((u8 *)p + 0x17A0), &D_003ED570);
    } else {
        *(f32 *)&p->c.unk14C4 = func_00211F70(p, p->c.a.room, node, PU(p, 0x138C, u16));
        PU(p, 0x17B0, u8) = VCALL(rm, 0x3C, u32 (*)(VObject *, u32, s32))(rm, PU(p, 0x138C, u16), p->c.a.room);
        ptmf_set((PTMF *)((u8 *)p + 0x17A0), &D_003ED560);
    }
    p->c.unk1530 = 0;
    p->c.unk1538 = 0;
    p->c.unk1534 = 0;
    p->c.unk14C0 = PU(p, 0x138C, u16);
    PU(p, 0x1784, s32) = 0;
}

/* vtable +0x270: go for Hewie (behaviour 1 attack / 3 turn first / 7 approach; 30 s limit) */
/* 0x002953F0 */
void Pursuer_GoForHewie(Pursuer *p) {
    s32 prev;

    p->target = gCharPartner;
    PU(p, 0x158C, f32) = func_00213D40(p, gCharPartner);
    PU(p, 0x16C9, u8) = PU(p, 0x16C9, u8) > 0 ? PU(p, 0x16C9, u8) : 1;
    PU(p, 0x16CA, u8) = PU(p, 0x16CA, u8) >= 2 ? PU(p, 0x16CA, u8) : 2;
    prev = PU(p, 0x1758, s32);
    if (prev != -1) {
        if (prev != -2) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, prev);
        }
    } else if (p->c.a.room == p->target->a.room) {
        if (PU(p, 0x158C, f32) < 0.0f) {
            if (!(func_00284440(p) & 0xFF)) {
                VCALL(p, 0x13C, void (*)(Pursuer *))(p);
                return;
            }
        } else if (PU(p, 0x158C, f32) <= VCALL(p, 0x2F0, f32 (*)(Pursuer *))(p) && (func_002175B0(&p->c.a, &gCharPartner->a) & 0xFF)) {
            u32 t = func_00213FA0(p, p->target->a.pos, 0x1.0c15240000000p+0f /* 1.0471976 */, 0x1.4f1a6ep+1f) & 0xFF;

            if (t == 0xFF) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
            } else {
                p->c.unk104[0] = t;
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 3);
            }
        } else {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 7);
        }
    } else {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 7);
    }
    PU(p, 0x16F6, u8) = 1;
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    PU(p, 0x162C, s32) = 0;
    PU(p, 0x1630, s32) = PU(p, 0x16D8, s32);
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECC30);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x274, void (*)(Pursuer *))(p);
}

/* vtable +0x1A4 / +0x1A0: closing in on the target (stance animation +0x328 / +0x324) */
static void Pursuer_CloseIn(Pursuer *p, s32 skip, u32 slotAnim, const PTMF *next) {
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    VCALL(p, 0xE0, void (*)(Pursuer *, Character *))(p, NULL);
    if (!(PU(p, 0x1590, f32) < 0.0f)) {
        if (PU(p, 0x1788, s32) != skip) {
            Pursuer_PlayAnim(p, VCALL(p, slotAnim, s32 (*)(Pursuer *))(p));
        }
        PU(p, 0x1784, s32) = 0;
        Actor_SetState(&p->c.a, next);
        VCALL(p, 0x1A8, void (*)(Pursuer *))(p);
        return;
    }
    VCALL(p, 0xB4, void (*)(Pursuer *, Character *))(p, p->target);
    if (!(func_00284440(p) & 0xFF)) {
        PU(p, 0x16EF, u8) = 1;
    }
}

/* 0x002908F0 */
void Pursuer_Chase1A4(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    Pursuer_CloseIn(p, 0x201, 0x328, &D_003ECDE0);
}

/* 0x00290B70 */
void Pursuer_Chase1A0(Pursuer *p) {
    PU(p, 0x16EC, u8) = 1;
    PURSUER_STEP_NEXT(p) = 1;
    Pursuer_CloseIn(p, 0x200, 0x324, &D_003ECDD0);
}

/* vtable +0x...: sidestep around the target if there's room (8 units) */
void func_00289280(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if (func_00217FC0(p, 8.0f) & 0xFF) {
        Character *t;

        if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
            return;
        }
        t = p->target;
        p->c.unk104[0] = func_002E2D00(func_002E2D00(func_001244D0(&t->a, p->c.a.pos) - t->a.angle[1])) <= 0.0f ? -1 : 1;
        PU(p, 0x1634, s32) = 0;
        if (PU(p, 0x1788, s32) != 0x200) {
            Pursuer_PlayAnim(p, VCALL(p, 0x324, s32 (*)(Pursuer *))(p));
        }
        PU(p, 0x1784, s32) = 0;
        Actor_SetState(&p->c.a, &D_003ED140);
        func_00289050(p);
        return;
    }
    PURSUER_STEP_DONE(p) = 1;
}

/* ---- batch 10 ---- */

#include "effectmgr.h"

extern const PTMF D_003EC930, D_003EC940, D_003ED350, D_003ED4B0, D_003ED4C0, D_003ED4D0, D_003ED4E0,
    D_003ED4F0, D_003ED500, D_003ECEE0, D_003ECB60, D_003ECB70, D_003ED240;
extern PTMF D_0045B340;   /* followed by a move: +0xC kind, +0x10 move mode, +0x14 sub */
extern void *D_00479FF0[];

/* a move from one of the tables at 0x45B340 / 0x45B358 / 0x45B3A0: step function to +0x17A0, kind
 * +0x17AC, move mode and sub */

/* vtable +0x84: the state block (+0x14E8) of an event: 4 hit, 5 back to normal, 0xC hurt (30),
 * 7 freed */
/* 0x0029C8C0 */
void Pursuer_EventState(Pursuer *p) {
    switch (p->c.state[0]) {
    case 4:
        if (func_00217510(p) != 0) {
            func_0029B8B0(p);
        } else {
            func_0029B5C0(p);
        }
        return;
    case 5:
        if (func_00217510(p) & 0xFF) {
            if (p->c.moveSub != 7) {
                VCALL(p, 0x8C, void (*)(Pursuer *))(p);
                VCALL(p, 0x7C, void (*)(Pursuer *))(p);
            } else {
                s32 a = PU(p, 0x1624, s32);
                s32 b = PU(p, 0x1628, s32);

                PU(p, 0x16ED, u8) = 0;
                PURSUER_STEP_DONE(p) = 0;
                PU(p, 0x16EF, u8) = 0;
                PU(p, 0x16F5, u8) = 0;
                PURSUER_STEP_NEXT(p) = 0;
                PU(p, 0x16EC, u8) = 0;
                PU(p, 0x1710, u8) = 0;
                PU(p, 0x16F9, u8) = 0;
                PU(p, 0x16F6, u8) = 0;
                PU(p, 0x16F8, u8) = 0;
                PU(p, 0x16F7, u8) = 0;
                PU(p, 0x15A0, u8) = 1;
                PU(p, 0x1758, s32) = -1;
                PU(p, 0x1761, u8) = 0;
                PU(p, 0x1760, u8) = 0;
                PU(p, 0x178C, s32) = 0;
                PU(p, 0x1764, s32) = -1;
                p->c.unk14D0 = 0;
                PU(p, 0x17B4, s32) = 0;
                PU(p, 0x1728, s32) = -1;
                PU(p, 0x172C, u8) = 0;
                PU(p, 0x1738, s32) = 0;
                func_0029A6D0(p);
                PU(p, 0x1700, s32) = 0;
                PU(p, 0x1704, s32) = 0;
                PU(p, 0x1708, s32) = 0;
                PU(p, 0x1770, s32) = 0;
                PU(p, 0x1774, s32) = 0;
                PU(p, 0x1778, s32) = 0;
                PU(p, 0x1624, s32) = a;
                PU(p, 0x1628, s32) = b;
                Character_BackToNormal(&p->c);
            }
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003EC930);
            PU(p, 0x1758, s32) = -1;
            p->c.state[0] = 0;
        } else {
            p->c.state[0] = 0;
        }
        p->c.state[1] = 0;
        break;
    case 12:
        if (p->c.state[1] == 30) {
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003EC940);
            PU(p, 0x1758, s32) = -1;
            VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0);
        }
        p->c.state[0] = 0;
        p->c.state[1] = 0;
        break;
    case 7:
        if (p->c.moveMode == 0) {
            p->c.state[0] = 0;
            p->c.state[1] = 0;
        }
        return;
    }
}

/* footsteps heard through the walls: while in another room within 80 units (by path) of Fiona,
 * each time the pursuer has closed in by more than 10, a footstep at its position (left / right
 * by +0x16A4). The loudness was meant to grow from 10 to 15 units of approach, but the step is
 * always taken as 15, so it is always full */
void func_0029D7F0(Pursuer *p) {
    if (func_00217510(p) == 0 && PU(p, 0x17AC, s32) != 4) {
        f32 d;

        PU(p, 0x1538, f32) = PU(p, 0x1534, f32);
        d = func_00126E40(&p->c);
        PU(p, 0x1534, f32) = d;
        if (d <= 80.0f) {
            f32 acc, f;

            if (!(PU(p, 0x1538, f32) <= 0.0f)) {
                PU(p, 0x1530, f32) = PU(p, 0x1530, f32) + (PU(p, 0x1538, f32) - d);
            } else {
                PU(p, 0x1530, f32) = 0.0f;
            }
            acc = PU(p, 0x1530, f32);
            if (!(acc <= 10.0f)) {
                f32 pos[4] __attribute__((aligned(16)));

                if (!(acc <= 10.0f)) {
                    acc = 15.0f;
                }
                f = (acc - 10.0f) / 5.0f;
                PU(p, 0x1530, f32) = PU(p, 0x1530, f32) - acc;
                if (f < 0.0f) {
                    f = 0.0f;
                }
                if (!(f <= 1.0f)) {
                    f = 1.0f;
                }
                if (func_001264D0(&p->c, pos) != 0) {
                    s8 vol = (u8)(u32)(10.0f * f) & 0x7F;
                    s8 vol2 = (u8)(u32)(2.0f * f) & 0x7F;
                    s32 foot = PU(p, 0x16A4, s32) & 1;

                    if (VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(gProgress, 8) == 0) {
                        func_00122C20(&p->c.a, foot, 7, vol, vol2, pos);
                    }
                    PU(p, 0x16A4, s32)++;
                }
            }
        }
    }
}

/* vtable +0x130: pick from the attack table +0x1718 ({kind, value, chance}) */
void func_00283C50(Pursuer *p) {
    f32 roll = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
    u8 *e;
    s32 kind;
    u32 i;

    for (i = 0;; i = (i + 1) & 0xFF) {
        e = PU(p, 0x1718, u8 *) + (i & 0xFF) * 0xC;
        if (roll <= AT(e, 0x8, f32)) {
            break;
        }
    }
    kind = AT(e, 0x0, s32);
    switch (kind) {
    case 0x1:
        return;
    case 0x13:
        PU(p, 0x1728, s32) = AT(e, 0x4, s32);
        PU(p, 0x172C, u8) = 0;
        break;
    case 0x18:
    case 0x19:
        p->c.unk104[0] = AT(e, 0x4, s32);
        /* fallthrough */
    case 0x1A:
        VCALL(p, 0x114, void (*)(Pursuer *, s32, u32))(p, kind, i);
        return;
    case 0x2:
    case 0x17:
        p->c.unk104[0] = AT(e, 0x4, s32);
        if ((PU(p, 0x16C8, u8) != 0 || PU(p, 0x1544, u8) == 0) && p->target == gCharPlayer) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32, u32))(p, kind, i);
            return;
        }
        break;
    case 0x14:
    case 0x15:
        if (!(((s32 (*)(Pursuer *, Character *, u32))func_001235C0)(p, gCharPlayer, i) & 0xFF)) {
            VCALL(p, 0x134, void (*)(Pursuer *))(p);
            return;
        }
        break;
    case 0x9:
    case 0xD:
        VCALL(p, 0x114, void (*)(Pursuer *, s32, u32))(p, kind, i);
        return;
    }
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED350);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, kind);
}

/* vtable +0x154: what next after a move, by the move mode / behaviour / stance */
/* 0x0027F350 */
void Pursuer_AfterMove(Pursuer *p) {
    if (p->c.moveMode == 4) {
        if (p->c.a.unkC4 == 2 || PU(p, 0x175C, s32) == 0x20 || p->c.moveSub == 0xA) {
            if (PU(p, 0x1664, u32) < 60) {
                PU(p, 0x1664, u32) = 120;
            }
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED4B0);
        } else if (PU(p, 0x175C, s32) == 0x23 && PU(p, 0x1664, u32) != 0) {
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED4C0);
        } else if (PU(p, 0x16C8, u8) != 4) {
            if (p->c.moveSub != 0x11) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
            }
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED4E0);
        } else {
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED4D0);
        }
    } else if (PU(p, 0x16C8, u8) == 4) {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED4F0);
    } else {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED500);
    }
    PU(p, 0x1758, s32) = -1;
}

/* vtable +0x8C... : into room `room` by event (`found`: chasing Fiona / searching) */
void func_0029CEE0(Pursuer *p, s32 room, u32 found, s32 plan, s32 side) {
    VCALL(p, 0x64, s32 (*)(Pursuer *, s32, s32, s32))(p, room, -1, side);
    func_002DDE20(p->c.motion, 0, -1);
    if ((found & 0xFF) == 1) {
        if (room == gCharPlayer->a.room) {
            PU(p, 0x16C8, u8) = 2;
            PU(p, 0x1594, s32) = room;
        } else {
            PU(p, 0x16C8, u8) = 1;
            VCALL(p, 0xB0, void (*)(Pursuer *))(p);
            PU(p, 0x16C9, u8) = 2;
            PU(p, 0x16CA, u8) = 3;
        }
    } else {
        PU(p, 0x16C8, u8) = 3;
        VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
        if (plan != 0) {
            VCALL(p, 0xB0, void (*)(Pursuer *))(p);
        } else {
            func_0027CA00(p);
        }
    }
    if (p->c.unkE0 == 1) {
        p->c.unkE0 = 0;
    }
    func_0027EAF0(p);
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x16F5, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    PU(p, 0x16EC, u8) = 0;
    PU(p, 0x1710, u8) = 0;
    PU(p, 0x16F9, u8) = 0;
    PU(p, 0x16F6, u8) = 0;
    PU(p, 0x16F8, u8) = 0;
    PU(p, 0x16F7, u8) = 0;
    PU(p, 0x15A0, u8) = 1;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1761, u8) = 0;
    PU(p, 0x1760, u8) = 0;
    PU(p, 0x178C, s32) = 0;
    PU(p, 0x1764, s32) = -1;
    p->c.unk14D0 = 0;
    PU(p, 0x17B4, s32) = 0;
    PU(p, 0x1728, s32) = -1;
    PU(p, 0x172C, u8) = 0;
    PU(p, 0x1738, s32) = 0;
    func_0029A6D0(p);
    PU(p, 0x1700, s32) = 0;
    PU(p, 0x1704, s32) = 0;
    PU(p, 0x1708, s32) = 0;
    PU(p, 0x1770, s32) = 0;
    PU(p, 0x1774, s32) = 0;
    PU(p, 0x1778, s32) = 0;
    PU(p, 0x1784, s32) = 0;
    PU(p, 0x1780, s32) = 0;
    PU(p, 0x1664, s32) = 0;
    PU(p, 0x1790, s32) = 0;
    PU(p, 0x17B4, s32) = 0;
    PU(p, 0x1794, s32) = 0;
    PU(p, 0x1798, s32) = 0;
    switch (PU(p, 0x16C8, u8)) {
    case 0:
        VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
        break;
    case 1:
    case 2:
        VCALL(p, 0x2C0, void (*)(Pursuer *))(p);
        break;
    case 3:
        VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
        break;
    case 4:
        VCALL(p, 0x2C8, void (*)(Pursuer *, s32))(p, 0);
        break;
    }
    if (plan == 0) {
        p->c.door = VCALL(gRooms, 0x3C, u32 (*)(VObject *, u32, s32))(gRooms, PU(p, 0x138C, u16), p->c.a.room);
    }
}

/* the door it barged through: shove on to a free triangle, then go on (not able: hurt,
 * behaviour 0x20) */
void func_00287380(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));

    func_00125A10(&p->c);
    if (p->c.unk100 == 0xFF) {
        if (MOTION_KEYS(p) & MOTION_KEY_END) {
            Heading_Vector(v, PU(p, 0x1634, f32));
            for (;;) {
                u32 tri = p->c.a.navTri, flags;

                if (tri < AT(gNavMesh, 0x8, u32) && AT(gNavMesh, 0x4, u8 *) != NULL) {
                    flags = AT(AT(gNavMesh, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
                } else {
                    flags = 0;
                }
                if (!(p->c.a.navMask & flags)) {
                    break;
                }
                func_001247E0(&p->c.a, v);
            }
            PURSUER_STEP_DONE(p) = 1;
            PURSUER_STEP_NEXT(p) = 1;
            p->c.a.unk2B = 0;
            p->c.unk100 = -1;
            PU(p, 0x1634, s32) = 0;
        }
    } else {
        VObject *rm = gRooms;

        if (VCALL(rm, 0x70, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, p->c.unk100 & 0xFF) != 0 && p->c.a.unk2B != 0) {
            Progress *pr = gProgress;
            u32 st = PursuerGroup_Fields(pr, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) & 0xFF;

            if (!(st & 8) && ((st ^ (PursuerGroup_Fields(pr, (u8)p->c.unk100, 0) & 0xFF)) & 0x10)) {
                p->c.a.unk2B = 0;
            }
        }
        Heading_Vector(v, PU(p, 0x1634, f32));
        func_0010E640(v, v, 3.0f);
        func_001247E0(&p->c.a, v);
        if (!(Progress_CurRoomFlag(gProgress, p->c.a.room, (u8)p->c.unk100) & 0xFF)) {
            PU(p, 0x1634, s32) = 0;
            p->c.a.unk2B = 0;
            if (!(VCALL(rm, 0x70, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, (u8)p->c.unk100) & 0xFF)) {
                if (p->c.hp > 0) {
                    PU(p, 0x1664, s32) = 150;
                } else {
                    VCALL(p, 0x2CC, void (*)(Pursuer *))(p);
                    PU(p, 0x1790, s32) = 0;
                    p->c.a.unkC4 = 2;
                    VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x20);
                }
                func_002815E0(p, (u8)p->c.unk100);
            } else {
                if (p->c.hp <= 0) {
                    p->c.hp = 1;
                }
                PURSUER_STEP_DONE(p) = 1;
                PURSUER_STEP_NEXT(p) = 1;
            }
            p->c.unk100 = -1;
        }
    }
}

/* vtable +0x14C: an event over: the door left, placed by the side it's on, state cleared */
/* 0x00280210 */
void Pursuer_EventOver(Pursuer *p, s32 exit) {
    if (p->c.moveMode == 2) {
        Progress *pr = gProgress;

        if ((Progress_CurRoomFlag(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1) {
            if (PU(p, 0x175C, s32) == 0xF) {
                func_00212D30(p, (u8)p->c.unk100);
            } else if ((func_00178980(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1) {
                func_00212CA0(p, (u8)p->c.unk100);
            } else {
                func_00212D30(p, (u8)p->c.unk100);
            }
        }
        p->c.moveMode = 0;
    }
    if (p->c.moveMode == 3) {
        f32 ofs[4] __attribute__((aligned(16)));
        f32 y;

        RoomSlots_Leave(gProgress, (u8)p->c.unk100, *(u8 *)&p->c.a.slot);
        y = p->c.a.pos[1];
        if (!(y <= PU(p, 0x15D4, f32)) || (y == PU(p, 0x15D4, f32) && y < PU(p, 0x15B4, f32))) {
            VCALL(p, 0x9C, void (*)(Pursuer *, u32, f32 *))(p, 3, ofs);
            p->c.a.navTri = func_00123710(p, p->c.unk100, 0, ofs, p->c.a.pos);
        } else {
            VCALL(p, 0x9C, void (*)(Pursuer *, u32, f32 *))(p, 2, ofs);
            p->c.a.navTri = func_00123710(p, p->c.unk100, 1, ofs, p->c.a.pos);
        }
    }
    if (p->c.unkE0 == 1) {
        VCALL(p, 0x90, void (*)(Pursuer *))(p);
    }
    PU(p, 0x1624, s32) = 0;
    PU(p, 0x1628, s32) = 0;
    PU(p, 0x162C, s32) = 0;
    PU(p, 0x1630, s32) = 0;
    PU(p, 0x1634, s32) = 0;
    PU(p, 0x1638, s32) = 0;
    PU(p, 0x163C, s32) = 0;
    PU(p, 0x1640, s32) = 0;
    PU(p, 0x1650, s32) = 0;
    PU(p, 0x1654, s32) = 0;
    PU(p, 0x1658, s32) = 0;
    PU(p, 0x165C, s32) = 0;
    p->c.unk104[0] = -1;
    PU(p, 0x1761, u8) = 0;
    PU(p, 0x1760, u8) = 0;
    PU(p, 0x178C, s32) = 0;
    PU(p, 0x1764, s32) = -1;
    p->c.unk14D0 = 0;
    func_001F6E10(p->c.motion);
    PU(p, 0x16F5, u8) = 0;
    PU(p, 0x16F8, u8) = 0;
    PU(p, 0x16F7, u8) = 0;
    PU(p, 0x1794, s32) = 0;
    PU(p, 0x1798, s32) = 0;
    PU(p, 0x16F3, u8) = 0;
    if (func_00217510(p) != 0) {
        p->c.a.disabled = 1;
        p->c.a.unk2A = 1;
        p->c.a.unk2B = 0;
        p->c.a.unk2D = 0;
        PU(p, 0x1544, u8) = 0;
        PU(p, 0x1545, u8) = 0;
        p->c.unk124 = p->c.unk128;
    } else if (VCALL(gRooms, 0x18, s32 (*)(VObject *, s32, s32))(gRooms, gCharPlayer->a.room, exit) == p->c.a.room) {
        p->c.unk14C4 = 0;
        p->c.a.unk2A = 0;
    }
}

/* vtable +0x14: load the files: motions / message (+0x0 / +0x8), slot 2 also its sound banks
 * (+0x10 / +0x14 / +0x18), door sounds (+0xC) and +0x4 */
/* 0x0029F3E0 */
void Pursuer_LoadFiles(Pursuer *p) {
    u8 *f = PU(p, 0x168C, u8 *);
    char *name;

    if (p->c.a.slot != 2) {
        u32 id = p->c.a.flags24 | p->c.a.slot;

        name = AT(f, 0x0, char *);
        if (*name != 0) {
            VCALL(gFileLoader, 0xC, void (*)(VObject *, char *, s32, u32, s32))(gFileLoader, name, PU(p, 0x1670, s32), id, 0);
        }
        name = AT(PU(p, 0x168C, u8 *), 0x8, char *);
        if (*name != 0) {
            VCALL(gFileLoader, 0xC, void (*)(VObject *, char *, s32, u32, s32))(gFileLoader, name, PU(p, 0x1674, s32), id, 0);
        }
        return;
    }
    name = AT(f, 0x10, char *);
    if (*name != 0) {
        VCALL(gSound, 0x80, void (*)(VObject *, char *, s32, s32, s32))(gSound, name, 7, 2, PU(p, 0x167C, s32));
    }
    name = AT(PU(p, 0x168C, u8 *), 0x14, char *);
    if (*name != 0) {
        VCALL(gSound, 0x80, void (*)(VObject *, char *, s32, s32, s32))(gSound, name, 7, 0, PU(p, 0x1680, s32));
    }
    name = AT(PU(p, 0x168C, u8 *), 0x18, char *);
    if (*name != 0) {
        VCALL(gSound, 0x80, void (*)(VObject *, char *, s32, s32, s32))(gSound, name, 7, 3, PU(p, 0x1684, s32));
    }
    name = AT(PU(p, 0x168C, u8 *), 0x0, char *);
    if (*name != 0) {
        VCALL(gFileLoader, 0xC, void (*)(VObject *, char *, s32, u32, s32))(gFileLoader, name, PU(p, 0x1670, s32), p->c.a.flags24 | p->c.a.slot, 0);
    }
    name = AT(PU(p, 0x168C, u8 *), 0x8, char *);
    if (*name != 0) {
        VCALL(gFileLoader, 0xC, void (*)(VObject *, char *, s32, u32, s32))(gFileLoader, name, PU(p, 0x1674, s32), p->c.a.flags24 | p->c.a.slot, 0);
    }
    if (*AT(PU(p, 0x168C, u8 *), 0xC, char *) != 0) {
        VCALL(gFileLoader, 0xC, void (*)(VObject *, char *, s32, u32, s32))(gFileLoader, AT(PU(p, 0x168C, u8 *), 0xC, char *),
                                                                           VCALL(gDoors, 0x48, s32 (*)(VObject *, s32))(gDoors, 1),
                                                                           p->c.a.flags24 | p->c.a.slot, 0);
    }
    name = AT(PU(p, 0x168C, u8 *), 0x4, char *);
    if (*name != 0) {
        VCALL(gFileLoader, 0xC, void (*)(VObject *, char *, s32, u32, s32))(gFileLoader, name, PU(p, 0x1678, s32), p->c.a.flags24 | p->c.a.slot, 0);
    }
}

/* may the pursuer use exit `exit`? (clears its "tried" bit +0x148C when it can) */
s32 func_0027F0A0(Pursuer *p, u32 exit) {
    Progress *pr = gProgress;
    VObject *rm;
    u32 node;

    if ((Progress_CurRoomFlag(pr, p->c.a.room, exit) & 0xFF) == 1) {
        return 0;
    }
    rm = gRooms;
    if (VCALL(rm, 0x18, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, exit) != VCALL(pr, 0xC, s32 (*)(Progress *))(pr)) {
        switch (func_00211CF0(p, exit) & 0xFF) {
        case 1:
            node = VCALL(rm, 0x10, u32 (*)(VObject *, s32, u32))(rm, p->c.a.room, exit) & 0xFFFF;
            p->c.unk148C[node >> 5] &= ~(1 << (node & 0x1F));
            return 1;
        case 2:
            return 0;
        case 0:
            if (PU(p, 0x17AC, s32) != 4) {
                func_0027CB90(p, exit);
            }
            return 0;
        default:
            return 0;
        }
    }
    switch (func_00211E00(p, exit) & 0xFF) {
    case 4:
        node = VCALL(rm, 0x10, u32 (*)(VObject *, s32, u32))(rm, p->c.a.room, exit) & 0xFFFF;
        p->c.unk148C[node >> 5] &= ~(1 << (node & 0x1F));
        return 1;
    case 3:
        if (PU(p, 0x17AC, s32) != 4) {
            func_0027CB90(p, exit);
        }
        /* fallthrough */
    case 2:
        return 0;
    default:
        rm = gRooms;
        node = VCALL(rm, 0x10, u32 (*)(VObject *, s32, u32))(rm, p->c.a.room, exit) & 0xFFFF;
        p->c.unk148C[node >> 5] &= ~(1 << (node & 0x1F));
        p->c.unk100 = VCALL(rm, 0x14, u32 (*)(VObject *, s32, u32))(rm, p->c.a.room, exit) & 0xFF;
        pr = gProgress;
        DoorHold_Take(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
        DoorHold_Release(pr, p->c.a.room, exit);
        return 1;
    }
}

/* vtable +0x1D8: barge through the door +0x100 (animation 0x600..0x603 by side) */
/* 0x0028DE10 */
void Pursuer_DoorBarge(Pursuer *p) {
    f32 dir[4] __attribute__((aligned(16)));
    VObject *d = gDoors;
    s32 r = VCALL(d, 0x18, s32 (*)(VObject *, u32, f32 *))(d, (u8)p->c.unk100, p->c.a.pos);
    Progress *pr;

    if (r == -1) {
        p->c.unk100 = -1;
        PU(p, 0x16EF, u8) = 1;
        return;
    }
    pr = gProgress;
    if ((func_00178980(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1) {
        p->c.unk104[0] = (r != 0 ? 3 : 1) & 0xFFFF;
        p->c.moveSub = 0x15;
    } else {
        p->c.unk104[0] = (r != 0 ? 2 : 0) & 0xFFFF;
        p->c.moveSub = 0x14;
    }
    if (gCharPlayer->moveMode != 0 && (PursuerGroup_Fields(pr, (u8)p->c.unk100, 0) & 0xFF & 0x20)) {
        return;
    }
    p->c.a.unk2B = 1;
    p->c.a.unk2A = 1;
    if (p->c.moveSub == 0x14) {
        func_00212DE0(p, (u8)p->c.unk100);
    } else if (p->c.moveSub == 0x15) {
        func_00212DC0(p, (u8)p->c.unk100);
    }
    p->c.a.navTri = VCALL(d, 0x14, u32 (*)(VObject *, u32, s32, f32 *, f32 *, s32))(d, (u8)p->c.unk100, p->c.unk104[0], p->c.a.pos, dir, 1);
    p->c.a.angle[1] = dir[1];
    sceVu0UnitMatrix(p->c.a.rot);
    sceVu0RotMatrixY(p->c.a.rot, p->c.a.rot, dir[1]);
    switch (p->c.unk104[0]) {
    case 0: func_002DDE20(p->c.motion, 0x600, -1); break;
    case 1: func_002DDE20(p->c.motion, 0x601, -1); break;
    case 2: func_002DDE20(p->c.motion, 0x602, -1); break;
    case 3: func_002DDE20(p->c.motion, 0x603, -1); break;
    }
    VCALL(d, 0xC, void (*)(VObject *, u32, s32, s32, s32))(d, (u8)p->c.unk100, p->c.unk104[0], p->c.a.slot, 1);
    Actor_SetState(&p->c.a, &D_003ECEE0);
}

/* vtable +0x7C: back to the move of the stance (behaviour 1), in the room or off-screen */
/* 0x00298D20 */
void Pursuer_BackToStance(Pursuer *p) {
    VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
    switch (PU(p, 0x16C8, u8)) {
    case 0:
        Pursuer_SetMove(p, &D_0045B340);
        break;
    case 1:
    case 3:
    case 4:
        Pursuer_SetMove(p, &D_0045B358);
        break;
    case 2:
        Pursuer_SetMove(p, &D_0045B3A0);
        break;
    }
    if (p->c.unkE0 == 0) {
        if (func_00217510(p) != 0) {
            VCALL(p, 0x13C, void (*)(Pursuer *))(p);
        } else {
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECB70);
            PU(p, 0x1758, s32) = -1;
        }
        PURSUER_STEP_NEXT(p) = 1;
    } else {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECB60);
        PU(p, 0x1758, s32) = -1;
    }
}

static void Pursuer_EffectInit(void **obj) {
    *obj = D_00479FF0;
}

/* a special animation (0x1006) with an effect (0x479FF0) for +0x104 */
void func_00285B10(Pursuer *p) {
    u8 *m;
    s32 args[2];
    u8 *mgr;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    Pursuer_PlayAnim(p, 0x1006);
    m = p->c.motion;
    if (!(AT(AT(p->c.motion, 0x874, u8 *) + func_001F4710(m, AT(m, 0x55C, s32)) * 6, 0x4, u16) & 1)) {
        PU(p, 0x1624, s32) = 1;
    } else {
        PU(p, 0x1624, s32) = p->c.unk104[0];
    }
    mgr = gEffects;
    PU(p, 0x1628, s32) = Effect_New(mgr, 0x38, Pursuer_EffectInit);
    args[0] = (p->c.unk104[0] - 4) >> 1;
    args[1] = PU(p, 0x1624, s32);
    func_002D6090(mgr, PU(p, 0x1628, s32), args);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED240);
    if (MOTION_KEYS(p) & 0x400) {
        PU(p, 0x1624, s32)--;
        if (PU(p, 0x1624, s32) <= 0) {
            PURSUER_STEP_DONE(p) = 1;
            PURSUER_STEP_NEXT(p) = 1;
            p->c.unk104[0] = -1;
            PU(p, 0x1628, s32) = 0;
            PU(p, 0x1624, s32) = 0;
        }
    }
    func_00125A10(&p->c);
}

/* ---- batch 11 ---- */

extern const PTMF D_003ECA30, D_003ECA40, D_003ECA50, D_003ECF60, D_003EC9B0, D_003ECA60, D_003ECA70,
    D_003ECA80;

/* vtable +0x2C4: start searching (in the played room: the route cleared to one stop) */
void func_0029AC50(Pursuer *p) {
    if (p->c.a.unkC4 == 2) {
        p->c.a.unkC4 = 0;
        PU(p, 0x1664, s32) = 0;
    }
    if (func_00217510(p) != 0) {
        s32 k;

        if (PURSUER_STEP_NEXT(p) == 1 && p->c.unkE0 == 0) {
            if (PU(p, 0x175C, s32) == 0xC) {
                ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECA30);
                PU(p, 0x1758, s32) = -1;
                VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0xC);
            } else {
                ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECA40);
                PU(p, 0x1758, s32) = -1;
            }
        }
        for (k = 0; k < 8; k++) {
            PU(p, 0x15E0 + k * 8, s32) = -1;
            PU(p, 0x15E4 + k * 8, u8) = 0;
        }
        PU(p, 0x1620, u8) = 0xFF;
        PU(p, 0x1621, u8) = 0xFF;
        PU(p, 0x1794, s32) = 0;
        PU(p, 0x1620, u8) = 1;
        PU(p, 0x1621, u8) = 1;
    } else {
        Pursuer_SetMove(p, &D_0045B358);
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECA50);
        PU(p, 0x1758, s32) = -1;
        p->c.unk1384 = p->c.unk1388;
        PURSUER_STEP_NEXT(p) = 1;
        PU(p, 0x17B4, s32) = 0;
    }
    if (PU(p, 0x16C8, u8) == 0) {
        PU(p, 0x16C9, u8) = 0;
        PU(p, 0x16CB, u8) = 0;
        PU(p, 0x16CA, u8) = 0;
        PU(p, 0x16CC, u8) = 0;
        PU(p, 0x179C, s32) = -1;
        PU(p, 0x16F1, u8) = 0;
        PU(p, 0x16F3, u8) = 0;
        PU(p, 0x16F2, u8) = 0;
        PU(p, 0x16F4, u8) = 0;
    }
    p->c.unk1388 = p->c.unk1384;
    PU(p, 0x16C8, u8) = 3;
    VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
}

/* vtable +0xD0: where the target (+0x16CB: 5 Fiona, 1 Hewie, 4 a heard noise +0x14DC / +0x14E0)
 * is, to the triangle +0x179C; the path planned (vtable +0xD8) */
/* 0x002849B0 */
void Pursuer_LocateTarget(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));

    switch (PU(p, 0x16CB, u8)) {
    case 5:
        if (PU(p, 0x16F8, u8) == 0) {
            VCALL(p, 0xB0, void (*)(Pursuer *))(p);
        } else if (func_00217340(p) != func_002172F0(p, gCharPlayer)) {
            PU(p, 0x1598, s32) = func_002172F0(p, gCharPlayer);
        }
        PU(p, 0x179C, s32) = gCharPlayer->a.navTri;
        break;
    case 1:
        if (PU(p, 0x16F8, u8) == 0) {
            VCALL(p, 0xB4, void (*)(Pursuer *, Character *))(p, gCharPartner);
        }
        if (VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) == gCharPartner->a.room) {
            PU(p, 0x179C, s32) = gCharPartner->a.navTri;
        }
        break;
    case 4:
        if (PU(p, 0x16F8, u8) == 0) {
            s32 room = p->c.heard.room;

            if (room != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
                PU(p, 0x1594, s32) = room;
                PU(p, 0x1598, s32) = -1;
            } else {
                VCALL(gNavMesh, 0xC, void (*)(void *, s32, f32 *))(gNavMesh, p->c.heard.tri, v);
                if (func_00217510(p) != 0 && func_00217340(p) != -1 && func_001257B0(&p->c, p->c.heard.tri, v, -1) < 0.0f) {
                    if (func_002138F0(p, p->c.heard.tri) == 0 && func_00213690(p, p->c.heard.tri) == 0) {
                        PU(p, 0x1594, s32) = p->c.heard.room;
                        PU(p, 0x1598, s32) = (func_00217340(p) == 0) & 0xFF;
                    } else {
                        VCALL(p, 0xAC, void (*)(Pursuer *, s32, f32 *, s32))(p, p->c.heard.tri, v, p->c.heard.room);
                    }
                    PU(p, 0x179C, s32) = p->c.heard.tri;
                    return;
                }
                VCALL(p, 0xAC, void (*)(Pursuer *, s32, f32 *, s32))(p, p->c.heard.tri, v, p->c.heard.room);
            }
        }
        if (p->c.heard.room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
            PU(p, 0x179C, s32) = p->c.heard.tri;
        }
        break;
    }
    if (PU(p, 0x16F8, u8) == 0 && func_00217510(p) != 0 && func_00217260(p) != 0) {
        VCALL(p, 0xD8, s32 (*)(Pursuer *))(p);
    }
}

/* the door it was knocked through: as func_00287380, but always on to behaviour 0x20 */
void func_00286F10(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));

    func_00125A10(&p->c);
    if (p->c.unk100 == 0xFF) {
        if (MOTION_KEYS(p) & MOTION_KEY_END) {
            Heading_Vector(v, PU(p, 0x1634, f32));
            for (;;) {
                u32 tri = p->c.a.navTri, flags;

                if (tri < AT(gNavMesh, 0x8, u32) && AT(gNavMesh, 0x4, u8 *) != NULL) {
                    flags = AT(AT(gNavMesh, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
                } else {
                    flags = 0;
                }
                if (!(p->c.a.navMask & flags)) {
                    break;
                }
                func_001247E0(&p->c.a, v);
            }
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x20);
            p->c.a.unk2B = 0;
            p->c.unk100 = -1;
            PU(p, 0x1634, s32) = 0;
        }
    } else {
        VObject *rm = gRooms;

        if (VCALL(rm, 0x70, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, p->c.unk100 & 0xFF) != 0 && p->c.a.unk2B != 0) {
            Progress *pr = gProgress;
            u32 st = PursuerGroup_Fields(pr, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) & 0xFF;

            if (!(st & 8) && ((st ^ (PursuerGroup_Fields(pr, (u8)p->c.unk100, 0) & 0xFF)) & 0x10)) {
                p->c.a.unk2B = 0;
            }
        }
        Heading_Vector(v, PU(p, 0x1634, f32));
        func_0010E640(v, v, 3.0f);
        func_001247E0(&p->c.a, v);
        if (!(Progress_CurRoomFlag(gProgress, p->c.a.room, (u8)p->c.unk100) & 0xFF)) {
            PU(p, 0x1634, s32) = 0;
            p->c.a.unk2B = 0;
            if (!(VCALL(rm, 0x70, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, (u8)p->c.unk100) & 0xFF)) {
                if (p->c.hp > 0) {
                    PU(p, 0x1664, s32) = 150;
                } else {
                    VCALL(p, 0x2CC, void (*)(Pursuer *))(p);
                    PU(p, 0x1790, s32) = 0;
                    p->c.a.unkC4 = 2;
                    VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x20);
                }
                func_002815E0(p, (u8)p->c.unk100);
            } else {
                if (p->c.hp <= 0) {
                    if (p->c.a.unkC4 == 2) {
                        p->c.hp = p->c.hpMax;
                        PU(p, 0x1664, s32) = 1;
                    } else {
                        p->c.hp = 1;
                    }
                }
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x20);
            }
            p->c.unk100 = -1;
        }
    }
}

/* vtable +0x204..: back away from the door +0x100, then the 0x404 animation */
/* 0x0028D260 */
void Pursuer_DoorBackAway(Pursuer *p) {
    VObject *d;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    PU(p, 0x1634, f32) = 0.0f;
    d = gDoors;
    PU(p, 0x1634, f32) = func_002E2D00(func_00212550(p, (u8)p->c.unk100));
    if (VCALL(d, 0x70, s32 (*)(VObject *, u32))(d, (u8)p->c.unk100) != 0) {
        p->c.a.unk2B = 1;
    }
    if ((Pursuer_WalkOn(p) & 0xFF) != 1) {
        Pursuer_PlayAnim(p, 0x404);
        PU(p, 0x1784, s32) = 0;
        Actor_SetState(&p->c.a, &D_003ECF60);
        VCALL(p, 0x20C, void (*)(Pursuer *))(p);
        return;
    }
    if (VCALL(d, 0x70, s32 (*)(VObject *, u32))(d, (u8)p->c.unk100) != 0) {
        f32 v[4] __attribute__((aligned(16)));

        if (VCALL(gRooms, 0x70, s32 (*)(VObject *, s32, u32))(gRooms, p->c.a.room, (u8)p->c.unk100) != 0 &&
            p->c.a.unk2B != 0 && !(PursuerGroup_Fields(gProgress, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) & 0xFF & 8)) {
            p->c.a.unk2B = 0;
        }
        Heading_Vector(v, PU(p, 0x1634, f32));
        func_0010E640(v, v, 3.0f);
        func_001247E0(&p->c.a, v);
    }
}

/* exit `exit` (0xFF: +0x17B0) closed to it: mark it tried, plan again (or the stance's move) */
void func_0027CB90(Pursuer *p, u32 exit) {
    u32 node, i;

    if ((exit & 0xFF) == 0xFF) {
        exit = PU(p, 0x17B0, u8);
    }
    for (i = 0; i < 13; i++) {
        p->c.unk148C[i] = 0;
    }
    node = VCALL(gRooms, 0x10, u32 (*)(VObject *, s32, u32))(gRooms, p->c.a.room, exit) & 0xFFFF;
    p->c.unk148C[node >> 5] |= 1 << (node & 0x1F);
    if (func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) == -1) {
        if (PU(p, 0x16C8, u8) != 0 && PU(p, 0x16C8, u8) != 2) {
            Pursuer_SetMove(p, &D_0045B358);
        } else {
            Pursuer_SetMove(p, &D_0045B3A0);
        }
        return;
    }
    if (PU(p, 0x16C8, u8) != 0 && PU(p, 0x16C8, u8) != 2) {
        Pursuer_SetMove(p, &D_0045B358);
    } else {
        Pursuer_SetMove(p, &D_0045B340);
    }
}

/* where to stand to grab Hewie (grab kinds 10..15; 14 / 15 from the other side): search
 * around him in 10 degree steps for a walkable spot level with the pursuer */
s32 func_00285DE0(Pursuer *p, u32 kind, f32 *heading, f32 *pos) {
    f32 h = func_001244D0(&p->c.a, gCharPartner->a.pos);
    f32 ofs[4] __attribute__((aligned(16)));
    f32 best[4] __attribute__((aligned(16)));
    f32 bestH = 0.0f, bestDy = 0.0f;
    s32 bestTri = -1;
    s32 deg;

    switch (kind & 0xFF) {
    case 14:
    case 15:
        h = func_002E2D00(0x1.921fb60000000p+1f /* 3.1415927 */ + h);
        /* fallthrough */
    case 10:
    case 11:
    case 12:
    case 13:
        break;
    default:
        return -1;
    }
    VCALL(p, 0x2D8, void (*)(Pursuer *, u32, f32 *))(p, kind & 0xFF, ofs);
    for (deg = 0; deg < 181; deg += 10) {
        f32 a = 0x1.921fb60000000p+1f /* 3.1415927 */ * (f32)deg;
        s32 side;

        for (side = 0; side < 2; side++) {
            f32 m[4][4] __attribute__((aligned(16)));
            f32 at[4] __attribute__((aligned(16)));
            f32 r = side != 0 ? func_002E2D00(h + a / 180.0f) : func_002E2D00(h - a / 180.0f);
            f32 dy;
            s32 tri;

            Mtx_AtHeading(m, p->c.a.pos, r);
            func_002E2DD0(at, m, ofs);
            dy = p->c.a.pos[1] - at[1];
            tri = func_00123E20(&p->c.a, at);
            if (dy <= 0.0f) {
                dy = -dy;
            }
            if (tri != -1 && tri == func_00124320(&p->c.a, at, gCharPartner->a.navTri, gCharPartner->a.pos, 0x29020008)) {
                if (dy == 0.0f) {
                    *heading = r;
                    sceVu0CopyVector(pos, at);
                    return tri;
                }
                if (bestTri == -1 || dy < bestDy) {
                    bestTri = tri;
                    bestDy = dy;
                    bestH = r;
                    sceVu0CopyVector(best, at);
                }
            }
            if (deg == 0 || deg == 180) {
                break;
            }
        }
    }
    if (bestTri == -1) {
        return -1;
    }
    *heading = bestH;
    sceVu0CopyVector(pos, best);
    return bestTri;
}

/* a hit while off-screen (state block 4): damage counted (+0x16C4, up to 999), knocked away
 * (5) or held (8), with a cry */
void func_0029B5C0(Pursuer *p) {
    s32 snd = -1;

    if (p->c.a.unkC4 == 2) {
        p->c.state[0] = 0;
        p->c.state[1] = 0;
        return;
    }
    if (PU(p, 0x1664, s32) != 0) {
        p->c.state[0] = 0;
        p->c.state[1] = 0;
        return;
    }
    VCALL(p, 0x94, void (*)(Pursuer *, s32))(p, p->c.state[3]);
    PU(p, 0x16C4, s32) += p->c.state[3];
    if (PU(p, 0x16C4, s32) >= 999) {
        PU(p, 0x16C4, s32) = 999;
    }
    if (p->c.state[1] == 3) {
        VCALL(p, 0x94, void (*)(Pursuer *, s32))(p, p->c.hp);
    }
    if (p->c.state[1] == 5) {
        f32 pos[4] __attribute__((aligned(16)));
        VObject *rm = gRooms;

        VCALL(rm, 0x30, u32 (*)(VObject *, s32, f32 *))(rm, (s8)VCALL(rm, 0x14, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, PU(p, 0x17B0, u8)), pos);
        if (p->c.hp > 0) {
            snd = 0x1D;
            PU(p, 0x1664, s32) = (s32)(30.0f * (3.0f * VCALL(gRandom, 0x18, f32 (*)(VObject *))(gRandom))) + 60;
        } else {
            VCALL(p, 0x2CC, void (*)(Pursuer *))(p);
            PU(p, 0x1790, s32) = 0;
            PU(p, 0x17B4, s32) = 0;
            p->c.a.unkC4 = 2;
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003EC9B0);
            PU(p, 0x1758, s32) = -1;
            p->c.a.navTri = -1;
            func_002DDE20(p->c.motion, 0x1802, -1);
            snd = 0x1B;
        }
        p->c.door = VCALL(rm, 0x14, u32 (*)(VObject *, s32, u32))(rm, VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress), (u8)p->c.state[4]);
        if (snd >= 0 && VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(gProgress, 8) == 0) {
            func_00122C20(&p->c.a, snd, 7, 0, 0, pos);
        }
    } else if (p->c.state[1] == 8) {
        f32 pos[4] __attribute__((aligned(16)));

        if (p->c.hp <= 0) {
            p->c.hp = 1;
        }
        PU(p, 0x1664, s32) += p->c.state[4];
        if (p->c.state[3] > 0 && (func_001264D0(&p->c, pos) & 0xFF) == 1) {
            snd = 0x1D;
        }
        if (snd >= 0 && VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(gProgress, 8) == 0) {
            func_00122C20(&p->c.a, snd, 7, 0, 0, pos);
        }
    }
    p->c.state[0] = 0;
    p->c.state[1] = 0;
}

/* vtable +0x2D0: knocked down (behaviour 0x1F in the room / 0x20 away; animation 0x1806 if it
 * falls where it can, else 0x1802) */
void func_0029A940(Pursuer *p) {
    if (func_00217510(p) != 0) {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECA60);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x1F);
        Actor_SetState(&p->c.a, &D_003ECA70);
    } else {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECA80);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x20);
    }
    if (((s32 (*)(Pursuer *, Character *, u32))func_001235C0)(p, (Character *)p, 0) != 0) {
        Pursuer_PlayAnimBlend(p, 0x1806);
    } else {
        Pursuer_PlayAnimBlend(p, 0x1802);
    }
    VCALL(p, 0x94, void (*)(Pursuer *, s32))(p, p->c.hp);
    p->c.a.unkC4 = 2;
    VCALL(p, 0x2CC, void (*)(Pursuer *))(p);
    PU(p, 0x1790, s32) = 0;
}

/* ---- batch 12 ---- */

extern const PTMF D_003EC9C0, D_003EC9D0, D_003EC9E0, D_003EC9F0, D_003ED1E0, D_003ECE00, D_003EC900;

/* vtable +0x2BC: go for Fiona (stance 0): the route forgotten, behaviour by sight */
void func_0029B190(Pursuer *p) {
    u32 i;
    s32 k;

    PU(p, 0x16C8, u8) = 0;
    VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
    if (p->c.a.unkC4 == 2) {
        p->c.a.unkC4 = 0;
        PU(p, 0x1664, s32) = 0;
    }
    if (func_00217510(p) != 0) {
        if (PURSUER_STEP_NEXT(p) == 1 && p->c.unkE0 == 0) {
            VCALL(p, 0xB0, void (*)(Pursuer *))(p);
            if (PU(p, 0x175C, s32) != 0xC) {
                ptmf_set((PTMF *)((u8 *)p + 0x174C), PU(p, 0x1544, u8) != 0 ? &D_003EC9D0 : &D_003EC9E0);
                PU(p, 0x1758, s32) = -1;
            } else {
                ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003EC9C0);
                PU(p, 0x1758, s32) = -1;
                VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0xC);
            }
        }
    } else {
        if (PU(p, 0x17AC, s32) != 0) {
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003EC9F0);
            PU(p, 0x1758, s32) = -1;
            Pursuer_SetMove(p, &D_0045B340);
        }
        PURSUER_STEP_NEXT(p) = 1;
        PU(p, 0x17B4, s32) = 0;
    }
    for (i = 0; i < 13; i++) {
        p->c.unk148C[i] = 0;
    }
    for (k = 0; k < 8; k++) {
        PU(p, 0x15E0 + k * 8, s32) = -1;
        PU(p, 0x15E4 + k * 8, u8) = 0;
    }
    PU(p, 0x1620, u8) = 0xFF;
    PU(p, 0x1621, u8) = 0xFF;
    PU(p, 0x1794, s32) = 0;
    PU(p, 0x16C9, u8) = 6;
    PU(p, 0x16CA, u8) = 7;
    PU(p, 0x16F6, u8) = 1;
}

/* the head turned towards `pos` (small angles ignored) */
static void Pursuer_LookAt(Pursuer *p, const f32 *pos) {
    f32 pitch, yaw;

    func_002DD110(p->c.motion, pos, &pitch, &yaw);
    if ((yaw <= 0.0f ? -yaw : yaw) < 0x1.99999a0000000p-5f /* 0.05 */) {
        yaw = 0.0f;
    }
    func_002DD310(p->c.motion, 0.0f, yaw, 0.0f, VCALL(p, 0xA4, f32 (*)(Pursuer *))(p));
    PU(p, 0x1574, f32) = func_002E2D00(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
}

/* vtable +0x44: the per-frame think: blocking flags, senses (+0x88), the behaviour step, the
 * stance logic (+0x110 / +0x100), animation sounds, the head turned to who it looks at
 * (+0x100: 0 Fiona, 1 Hewie, else +0x1700), the model (+0x40) */
/* 0x00299F80 */
void Pursuer_Think(Pursuer *p) {
    u8 *m;

    p->c.a.navMask = p->c.a.unk2B != 0 ? 0 : VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
    p->c.pathReq->mask = p->c.a.navMask;
    VCALL(p, 0x88, void (*)(Pursuer *))(p);
    func_00215D80(p);
    if (p->c.unkE0 != 0 || p->c.moveMode == 4) {
        PTMF *st = (PTMF *)((u8 *)p + 0x174C);

        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
    }
    func_00297C60(p);
    VCALL(p, 0x110, void (*)(Pursuer *))(p);
    VCALL(p, 0x100, void (*)(Pursuer *))(p);
    func_0029D4C0(p, -1);
    if (PU(p, 0x16F9, u8) == 1) {
        switch (p->c.unk100) {
        case 0:
            Pursuer_LookAt(p, gCharPlayer->a.pos);
            break;
        case 1:
            Pursuer_LookAt(p, gCharPartner->a.pos);
            break;
        default:
            Pursuer_LookAt(p, (f32 *)((u8 *)p + 0x1700));
            break;
        }
    } else {
        m = p->c.motion;
        VCALL(m, 0x5C, void (*)(void *, f32))(m, VCALL(p, 0xA4, f32 (*)(Pursuer *))(p));
    }
    VCALL(p, 0x40, void (*)(Pursuer *))(p);
}

/* vtable +0x...: hit at the door it opened: knocked through it (0x1004) or back (0x1005) */
void func_00287620(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));
    Progress *pr;
    u32 st;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (PU(p, 0x1788, s32) == 0x1800 || MOTION_ANIM(p) == 0x1709) {
        p->c.moveSub = 0xA;
    }
    pr = gProgress;
    st = PursuerGroup_Fields(pr, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) & 0xFF;
    VCALL(gDoors, 0x38, void (*)(VObject *, u32, f32 *))(gDoors, (u8)p->c.unk100, v);
    if ((func_00124490(&p->c.a, v) < 9.0f || (st & 0xFF & 9)) &&
        func_00178300(pr, p->c.a.room, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) != 0) {
        Pursuer_PlayAnimBlend(p, 0x1004);
        PU(p, 0x1634, f32) = func_00212550(p, (u8)p->c.unk100);
        p->c.a.unk2B = 1;
    } else {
        if (p->c.hp <= 0) {
            p->c.hp = 1;
        }
        Pursuer_PlayAnimBlend(p, 0x1005);
        PU(p, 0x1634, f32) = func_002E2D00(0x1.921fb60000000p+1f /* 3.1415927 */ + func_00212550(p, (u8)p->c.unk100));
        p->c.unk100 = 0xFF;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED1E0);
    func_00287380(p);
}

/* sounds keyed to the animation `anim` (-1: the current one) by the table +0x16AC ({left,
 * right} per animation): footsteps, and when it falls (0x1709 / 0x1800 / 0x1804) a splash on
 * water triangles (with ripples in rooms 7, 0xD1 and 0x106) */
void func_0029D4C0(Pursuer *p, s32 anim) {
    VObject *ro;
    void *nm;
    Progress *pr;
    s32 snd = -1, twice = 0;
    u32 bank = 7;

    if (PU(p, 0x16AC, s16 *) == NULL) {
        return;
    }
    if (anim == -1) {
        anim = MOTION_ANIM(p);
    }
    if (anim & 0x8000) {
        return;
    }
    switch (func_001F4770(p->c.motion, 0, 0, 1) & 0xFF & 0x11) {
    case 0x11:
        twice = 1 & 0xFF;
        /* fallthrough */
    case 0x1:
        snd = PU(p, 0x16AC, s16 *)[func_001F4710(p->c.motion, anim) * 2];
        break;
    case 0x10:
        snd = PU(p, 0x16AC, s16 *)[func_001F4710(p->c.motion, anim) * 2 + 1];
        break;
    }
    nm = gNavMesh;
    ro = gEvents;
    pr = gProgress;
    for (;;) {
        if ((s16)snd >= 0) {
            if ((anim == 0x1804 || anim == 0x1800 || anim == 0x1709) && (u32)((s16)snd - 0x11) < 2) {
                f32 v[4] __attribute__((aligned(16)));
                u8 *m = p->c.motion;
                u32 t = p->c.a.navTri;

                VCALL(m, 0x60, void (*)(void *, f32 *))(m, v);
                for (;;) {
                    u8 *e = t < AT(nm, 0x8, u32) && AT(nm, 0x4, u8 *) != NULL ? AT(nm, 0x4, u8 *) + t * 0x50 : NULL;
                    u32 fl = AT(e, 0x3C, u32);
                    s32 r;

                    if ((fl & 0x02000000) && (fl & 0x8000)) {
                        s32 room = p->c.a.room;

                        bank = 6;
                        snd = 0x1D;
                        if (room == 7 || room == 0xD1 || room == 0x106) {
                            func_00125E10(&p->c, p->c.a.pos, 1);
                        }
                        break;
                    }
                    r = VCALL(nm, 0x20, s32 (*)(void *, u32, f32 *, f32 *))(nm, t, p->c.a.pos, v);
                    if (r == 3 || r == 4) {
                        break;
                    }
                    t = AT(e + r * 4, 0x30, u32);
                    if (t == (u32)-1) {
                        break;
                    }
                }
            }
            if (VCALL(ro, 0x50, s32 (*)(VObject *))(ro) == 0 && Progress_TestFlag(pr, 8) == 0) {
                func_00122C20(&p->c.a, (s16)snd, bank & 0xFF, 0, 0, NULL);
            }
        }
        if (twice == 0) {
            break;
        }
        snd = PU(p, 0x16AC, s16 *)[func_001F4710(p->c.motion, anim) * 2 + 1];
        twice = 0;
    }
}

/* vtable +0x230: walk, then step aside (45 degrees, 8 units) if the way on is blocked */
/* 0x002902E0 */
void Pursuer_WalkAside(Pursuer *p) {
    u8 *m;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    if (PU(p, 0x1788, s32) != 0) {
        Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
    }
    m = p->c.motion;
    VCALL(m, 0x5C, void (*)(void *, f32))(m, VCALL(p, 0xA4, f32 (*)(Pursuer *))(p));
    if (MOTION_AT(p, 0x858, f32) != 0.0f) {
        return;
    }
    if (!(func_00217ED0(p, 0x1.921fb60000000p-1f /* 0.7853982 */, 8.0f) & 0xFF) && !(func_00217ED0(p, -0x1.921fb60000000p-1f /* 0.7853982 */, 8.0f) & 0xFF)) {
        if (!(Pursuer_WalkOn(p) & 0xFF)) {
            PURSUER_STEP_DONE(p) = 1;
        }
        return;
    }
    PU(p, 0x1624, s32) = 0;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECE00);
    VCALL(p, 0x234, void (*)(Pursuer *))(p);
}

/* vtable +0xC: init (NPC init, then all pursuer state, stance 2 with Fiona as the target, the
 * file tables of the kind) */
/* 0x0029FBD0 */
void Pursuer_Reset(Pursuer *p) {
    s32 k;

    NPC_Reset(p);
    PU(p, 0x16C9, u8) = 0;
    PU(p, 0x16CB, u8) = 0;
    PU(p, 0x16CA, u8) = 0;
    PU(p, 0x16CC, u8) = 0;
    PU(p, 0x179C, s32) = -1;
    PU(p, 0x16F1, u8) = 0;
    PU(p, 0x16F3, u8) = 0;
    PU(p, 0x16F2, u8) = 0;
    PU(p, 0x16F4, u8) = 0;
    for (k = 0; k < 8; k++) {
        PU(p, 0x15E0 + k * 8, s32) = -1;
        PU(p, 0x15E4 + k * 8, u8) = 0;
    }
    PU(p, 0x1620, u8) = 0xFF;
    PU(p, 0x1621, u8) = 0xFF;
    PU(p, 0x1794, s32) = 0;
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x16F5, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    PU(p, 0x16EC, u8) = 0;
    PU(p, 0x1710, u8) = 0;
    PU(p, 0x16F9, u8) = 0;
    PU(p, 0x16F6, u8) = 0;
    PU(p, 0x16F8, u8) = 0;
    PU(p, 0x16F7, u8) = 0;
    PU(p, 0x15A0, u8) = 1;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1761, u8) = 0;
    PU(p, 0x1760, u8) = 0;
    PU(p, 0x178C, s32) = 0;
    PU(p, 0x1764, s32) = -1;
    p->c.unk14D0 = 0;
    PU(p, 0x17B4, s32) = 0;
    PU(p, 0x1728, s32) = -1;
    PU(p, 0x172C, u8) = 0;
    PU(p, 0x1738, s32) = 0;
    func_0029A6D0(p);
    PU(p, 0x1700, s32) = 0;
    PU(p, 0x1704, s32) = 0;
    PU(p, 0x1708, s32) = 0;
    PU(p, 0x1770, s32) = 0;
    PU(p, 0x1774, s32) = 0;
    PU(p, 0x1778, s32) = 0;
    PU(p, 0x1784, s32) = 0;
    PU(p, 0x1780, s32) = 0;
    PU(p, 0x1664, s32) = 0;
    PU(p, 0x1790, s32) = 0;
    PU(p, 0x17B4, s32) = 0;
    PU(p, 0x1794, s32) = 0;
    PU(p, 0x1798, s32) = 0;
    switch (PU(p, 0x16C8, u8)) {
    case 0:
        VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
        break;
    case 1:
    case 2:
        VCALL(p, 0x2C0, void (*)(Pursuer *))(p);
        break;
    case 3:
        VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
        break;
    case 4:
        VCALL(p, 0x2C8, void (*)(Pursuer *, s32))(p, 0);
        break;
    }
    PU(p, 0x16A4, s32) = 0;
    PU(p, 0x16BC, s32) = 0;
    PU(p, 0x16C0, s32) = 0;
    PU(p, 0x16C4, s32) = 0;
    PU(p, 0x1788, s32) = -1;
    PU(p, 0x16C8, u8) = 2;
    VCALL(p, 0x2C0, void (*)(Pursuer *))(p);
    p->target = gCharPlayer;
    PU(p, 0x16B8, s32) = 0;
    PU(p, 0x17B0, u8) = 0xFF;
    VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 8);
    Pursuer_SetMove(p, &D_0045B358);
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003EC900);
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1718, s32) = 0;
    PU(p, 0x168C, u8 *) = VCALL(p, 0xF8, u8 *(*)(Pursuer *))(p);
    PU(p, 0x1690, u8 *) = VCALL(p, 0xFC, u8 *(*)(Pursuer *))(p);
    VCALL(p, 0xF4, void (*)(Pursuer *))(p);
}

/* off-screen travel: count down the way to the next exit (+0x14C4) at the stance's pace; at
 * it, through. (Going for Fiona the pace is 1.0..1.2 and then 0.6 more each frame: the original
 * falls through from that case into the next) */
/* 0x0027D260 */
void Pursuer_TravelOffscreen(Pursuer *p) {
    if (PU(p, 0x1664, s32) != 0 || PU(p, 0x17B4, s32) != 0) {
        return;
    }
    if (PU(p, 0x16C8, u8) == 4) {
        s32 room = p->c.a.room;
        s32 *e = VCALL(p, 0x314, s32 *(*)(Pursuer *))(p);

        for (; *e != -1; e += 2) {
            if (room == *e) {
                return;
            }
        }
    }
    if (!(*(f32 *)&p->c.unk14C4 <= 0.0f)) {
        if (Progress_TestFlag(gProgress, 9) == 0) {
            switch (PU(p, 0x16C8, u8)) {
            case 0:
                *(f32 *)&p->c.unk14C4 -= 1.0f + 0x1.99999a0000000p-3f /* 0.2 */ * VCALL(gRandom, 0x18, f32 (*)(VObject *))(gRandom);
                /* fallthrough */
            case 2:
            case 3:
                *(f32 *)&p->c.unk14C4 -= 0x1.3333340000000p-1f /* 0.6 */;
                break;
            case 1:
            case 4:
                *(f32 *)&p->c.unk14C4 -= 1.0f;
                break;
            }
        } else {
            *(f32 *)&p->c.unk14C4 -= 2.0f;
        }
    }
    if (*(f32 *)&p->c.unk14C4 <= 0.0f) {
        VObject *rm = gRooms;

        PU(p, 0x17B0, u8) = VCALL(rm, 0x3C, u32 (*)(VObject *, u32, s32))(rm, PU(p, 0x138C + p->c.unk1388 * 2, u16), p->c.a.room);
        if (PU(p, 0x17B0, u8) < 8) {
            s32 next = VCALL(rm, 0x18, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, PU(p, 0x17B0, u8));

            if (next == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) && (func_00211E00(p, PU(p, 0x17B0, u8)) & 0xFF) == 5 &&
                PU(p, 0x16C8, u8) != 4) {
                PURSUER_STEP_DONE(p) = 1;
                return;
            }
            if (!(func_0027F0A0(p, PU(p, 0x17B0, u8)) & 0xFF)) {
                return;
            }
            func_002815E0(p, PU(p, 0x17B0, u8));
            return;
        }
        Pursuer_SetMove(p, &D_0045B358);
    }
}

/* vtable +0xCC: what to go after: seeing Fiona (5, alert 7) or Hewie (1, 2), hearing Fiona (5, 6),
 * Hewie (1, 1) or another noise (4, 5); a higher alert than now takes over */
/* 0x00284C80 */
s32 Pursuer_PickTarget(Pursuer *p) {
    s32 room, side;

    PU(p, 0x16CB, u8) = 0;
    PU(p, 0x16CC, u8) = 0;
    if (PU(p, 0x1544, u8) == 1) {
        PU(p, 0x16CB, u8) = 5;
        PU(p, 0x16CC, u8) = 7;
    }
    if (PU(p, 0x1545, u8) == 1) {
        PU(p, 0x16CB, u8) = PU(p, 0x16CB, u8) > 0 ? PU(p, 0x16CB, u8) : 1;
        PU(p, 0x16CC, u8) = PU(p, 0x16CC, u8) >= 2 ? PU(p, 0x16CC, u8) : 2;
    }
    switch (p->c.heardSlot) {
    case 0xFF:
        break;
    case 0:
        PU(p, 0x16CB, u8) = 5;
        PU(p, 0x16CC, u8) = PU(p, 0x16CC, u8) >= 6 ? PU(p, 0x16CC, u8) : 6;
        break;
    case 1:
        PU(p, 0x16CB, u8) = PU(p, 0x16CB, u8) > 0 ? PU(p, 0x16CB, u8) : 1;
        PU(p, 0x16CC, u8) = PU(p, 0x16CC, u8) > 0 ? PU(p, 0x16CC, u8) : 1;
        break;
    case 3:
        PU(p, 0x16CB, u8) = PU(p, 0x16CB, u8) >= 4 ? PU(p, 0x16CB, u8) : 4;
        PU(p, 0x16CC, u8) = PU(p, 0x16CC, u8) >= 5 ? PU(p, 0x16CC, u8) : 5;
        break;
    }
    if (PU(p, 0x16CB, u8) == 0 || PU(p, 0x16CB, u8) < PU(p, 0x16C9, u8)) {
        return 0;
    }
    if (func_00217510(p) != 0 && p->c.a.room == p->c.heard.room && PU(p, 0x16CC, u8) == 5 && p->c.moveMode == 8) {
        f32 v[4] __attribute__((aligned(16)));

        VCALL(gNavMesh, 0xC, void (*)(void *, s32, f32 *))(gNavMesh, p->c.heard.tri, v);
        if (func_002187D0(p, p->c.heard.tri, v) != 0) {
            return 0;
        }
    }
    room = p->c.heard.room;
    side = -1;
    switch (PU(p, 0x16CB, u8)) {
    case 5:
        side = func_002172F0(p, gCharPlayer);
        room = gCharPlayer->a.room;
        break;
    case 1:
        side = func_002172F0(p, gCharPartner);
        room = gCharPartner->a.room;
        break;
    }
    if (func_00126F80(&p->c, room, side, -1, -1) >= 0) {
        if (PU(p, 0x16C8, u8) != 0 && PU(p, 0x16C8, u8) != 4) {
            PU(p, 0x16F2, u8) = 1;
            PU(p, 0x16F1, u8) = 1;
        }
        if (PU(p, 0x16CA, u8) < PU(p, 0x16CC, u8)) {
            if (func_00217510(p) != 0 && PU(p, 0x16F8, u8) == 0) {
                PU(p, 0x16F3, u8) = 1;
            }
            PU(p, 0x16CA, u8) = PU(p, 0x16CC, u8);
            PU(p, 0x16C9, u8) = PU(p, 0x16CB, u8);
        }
        VCALL(p, 0xD0, void (*)(Pursuer *))(p);
        return 1;
    }
    func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    PU(p, 0x1544, u8) = 0;
    PU(p, 0x1545, u8) = 0;
    PU(p, 0x1546, u8) = 0;
    return 0;
}

/* ---- batch 13 ---- */

extern const PTMF D_003ED1B0, D_003ED200, D_003ECFA0;

/* vtable +0x110: a grab ordered by the progress (Hewie, kinds 10..15): take position, or
 * refuse (-1) */
/* 0x0029C570 */
s32 Pursuer_GrabOrder(Pursuer *p) {
    Progress *pr = gProgress;
    Progress *pr2;
    u32 who, how, kind;
    Character *c;

    if ((func_00177870(pr, *(u8 *)&p->c.a.slot) & 0xFF) != 1) {
        return -1;
    }
    if (func_00217510(p) == 0) {
        SlotCmd_Cancel(pr, *(u8 *)&p->c.a.slot);
        return -1;
    }
    pr2 = gProgress;
    who = SlotCmd_Target(pr2, *(u8 *)&p->c.a.slot) & 0xFF;
    how = SlotCmd_Kind(pr2, *(u8 *)&p->c.a.slot) & 0xFF;
    kind = SlotCmd_Arg(pr2, *(u8 *)&p->c.a.slot) & 0xFF;
    c = gCharacters[who & 0xFF];
    if (p->c.moveMode == 8 && (u32)(p->c.moveSub - 0x18) < 2) {
        SlotCmd_Cancel(pr, *(u8 *)&p->c.a.slot);
        return -1;
    }
    if (!(((s32 (*)(Pursuer *, Character *, u32))func_001235C0)(p, (Character *)p, 0) & 0xFF)) {
        SlotCmd_Cancel(pr, *(u8 *)&p->c.a.slot);
        return -1;
    }
    if ((how & 0xFF) == 1) {
        if (p->c.moveSub != 9 && p->c.moveSub != 0xA) {
            f32 pos[4] __attribute__((aligned(16)));
            f32 h;
            s32 tri = func_00285DE0(p, kind, &h, pos);

            if (tri != -1 && (func_00123470(p, tri, (s32)pos) & 0xFF) == 1 && (u32)tri == func_00124480(&p->c.a, pos, 0x28020028)) {
                sceVu0CopyVector(c->unk110, pos);
                AT(c, 0x10C, f32) = h;
                c->unk104[1] = tri;
                AT(p, 0x10C, f32) = h;
                SlotCmd_Start(pr, *(u8 *)&p->c.a.slot);
                return 0;
            }
        }
    } else if ((how & 0xFF) == 2 && (kind & 0xFF) == 7 && p->c.moveSub != 9 && p->c.moveSub != 0xA) {
        f32 pos[4] __attribute__((aligned(16)));
        f32 h;
        s32 tri = func_00285DE0(p, 0xF, &h, pos);

        if (tri != -1 && (func_00123470(p, tri, (s32)pos) & 0xFF) == 1 && (u32)tri == func_00124480(&p->c.a, pos, 0x28020028)) {
            sceVu0CopyVector(c->unk110, pos);
            AT(c, 0x10C, f32) = h;
            c->unk104[1] = tri;
            AT(p, 0x10C, f32) = h;
            SlotCmd_Start(pr, *(u8 *)&p->c.a.slot);
            /* the second state block set to {0xC, 30} (the rest of the original's stack copy
             * was never written) */
            p->c.state2[0] = 0xC;
            p->c.state2[1] = 30;
            p->c.state2[2] = 0;
            p->c.state2[3] = 0;
            p->c.state2[4] = 0;
            p->c.state2[5] = 0;
            p->c.state2[6] = 0;
            p->c.state2[7] = 0;
            return 0;
        }
    }
    SlotCmd_Cancel(pr, *(u8 *)&p->c.a.slot);
    return -1;
}

/* vtable +0x2C0: search the room (stance 2) / one room on (stance 3): 2..5 / 1..2 more
 * route stops */
void func_0027E790(Pursuer *p) {
    u32 n;
    s32 k;

    switch (PU(p, 0x16C8, u8)) {
    case 2:
        n = (((u32)(4.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom)) & 0xFF & 0xFF) + 2) & 0xFF;
        break;
    case 3:
        n = (((u32)(2.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom)) & 0xFF & 0xFF) + 1) & 0xFF;
        break;
    default:
        n = 0xFF;
        break;
    }
    if (func_00217510(p) != 0) {
        s32 left;

        while (PU(p, 0x1620, u8) != 0 && PU(p, 0x1621, u8) != 0xFF) {
            func_00218B60(p);
        }
        if (PU(p, 0x16CA, u8) != 2 && PU(p, 0x16CA, u8) != 7 && func_00217260(p) == 0) {
            for (k = 0; k < 8; k++) {
                PU(p, 0x15E0 + k * 8, s32) = -1;
                PU(p, 0x15E4 + k * 8, u8) = 0;
            }
            PU(p, 0x1620, u8) = 0xFF;
            PU(p, 0x1621, u8) = 0xFF;
            PU(p, 0x1794, s32) = 0;
            return;
        }
        left = PU(p, 0x1621, u8) - PU(p, 0x1620, u8);
        if (left < (s32)(n & 0xFF)) {
            if (left >= (s32)(n & 0xFF)) {
                if ((s32)(n & 0xFF) < left && (n & 0xFF) != (u32)left) {
                    do {
                        PU(p, 0x1620, u8)++;
                    } while ((n & 0xFF) != (u32)(PU(p, 0x1621, u8) - PU(p, 0x1620, u8)));
                }
            } else {
                func_0027E5D0(p, ((n & 0xFF) - left) & 0xFF);
            }
            if ((s32)(n & 0xFF) > 0 && (n & 0xFF) != 0xFF) {
                PU(p, 0x1794, s32) = 1800;
            } else {
                PU(p, 0x1794, s32) = 0;
            }
        } else {
            PU(p, 0x1794, s32) = 1800;
        }
    } else if (func_00217260(p) != 0) {
        PU(p, 0x17B4, s32) = (n & 0xFF) * 150;
    } else {
        PU(p, 0x17B4, s32) = 0;
        for (k = 0; k < 8; k++) {
            PU(p, 0x15E0 + k * 8, s32) = -1;
            PU(p, 0x15E4 + k * 8, u8) = 0;
        }
        PU(p, 0x1620, u8) = 0xFF;
        PU(p, 0x1621, u8) = 0xFF;
        PU(p, 0x1794, s32) = 0;
    }
}

/* the hit reaction (animation +0x1624, 0x18xx: stagger in three parts while +0x1664 holds) */
void func_00287CD0(Pursuer *p) {
    s32 a = PU(p, 0x1624, s32);

    if ((a & 0xFF00) == 0x1800) {
        if (PU(p, 0x1664, s32) != 0) {
            s32 phase = PU(p, 0x1628, s32);

            if (phase == 1) {
                Pursuer_PlayAnim(p, a - 1);
                PU(p, 0x1628, s32) = 2;
            } else if ((MOTION_KEYS(p) & MOTION_KEY_END) != 0 && phase == 2) {
                Pursuer_PlayAnimBlend(p, a);
                PU(p, 0x1628, s32) = 0;
            }
            func_00125A10(&p->c);
            return;
        }
        Pursuer_PlayAnim(p, a + 1);
        PU(p, 0x1628, s32) = 0;
        PU(p, 0x1784, s32) = 0;
        Actor_SetState(&p->c.a, &D_003ED1B0);
        return;
    }
    PU(p, 0x1624, s32) = MOTION_ANIM(p);
    PU(p, 0x1628, s32) = 0;
    PURSUER_STEP_NEXT(p) = 0;
}

/* Hewie bites (+0x104: 10..15, where): the reaction animation, and the bite's damage from the
 * table +0x173C added to +0x16BC (up to 1000; see func_00286170); other kinds: Hewie is sent
 * state 7 */
void func_00286B90(Pursuer *p) {
    s32 *t;
    f32 d;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    switch (p->c.unk104[0]) {
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15: {
        static const s32 anims[6] = {0x1700, 0x1700, 0x1703, 0x1703, 0x1706, 0x1708};
        static const s32 slots[6] = {0x20, 0x24, 0x18, 0x1C, 0x28, 0x2C};
        s32 k = p->c.unk104[0] - 10;

        PU(p, 0x1624, s32) = anims[k];
        t = PU(p, 0x173C, s32 *);
        if (t != NULL) {
            PU(p, 0x16BC, u32) += AT(t, slots[k], s32);
            if (PU(p, 0x16BC, u32) >= 1001) {
                PU(p, 0x16BC, u32) = 1000;
            }
        }
        break;
    }
    default:
        if (gCharPartner->state[0] != 7) {
            /* the original copies a stack struct of which it only set [0] and [2] */
            gCharPartner->state[0] = 7;
            gCharPartner->state[1] = 0;
            gCharPartner->state[2] = p->c.unk153C;
            gCharPartner->state[3] = 0;
            gCharPartner->state[4] = 0;
            gCharPartner->state[5] = 0;
            gCharPartner->state[6] = 0;
            gCharPartner->state[7] = 0;
        }
        break;
    }
    func_002DDD20(p->c.motion, PU(p, 0x1624, s32), -1);
    PU(p, 0x1628, s32) = 0;
    PU(p, 0x1634, f32) = AT(p, 0x10C, f32);
    d = AT(p, 0x10C, f32) - p->c.a.angle[1];
    if (d <= 0.0f) {
        d = -d;
    }
    PU(p, 0x1638, f32) = (p->c.unk104[0] != 0xF ? 0.125f : 0x1.5551d60000000p-4f /* 0.08333 */) * d;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED200);
}

/* vtable +0x218: walk to the door +0x100 (side +0x104, -1: either), then line up */
/* 0x0028C9E0 */
void Pursuer_DoorApproach(Pursuer *p) {
    s32 tri = -1;
    f32 pos[4] __attribute__((aligned(16)));
    u32 t = 0xFF;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    if (p->c.unk104[0] != -1) {
        if (!(func_00212F40(p, (u8)p->c.unk100, p->c.unk104[0] != 0) & 0xFF)) {
            PU(p, 0x16EF, u8) = 1;
            return;
        }
    } else {
        p->c.unk104[0] = 0;
        while ((func_00212F40(p, (u8)p->c.unk100, p->c.unk104[0] != 0) & 0xFF) != 1) {
            p->c.unk104[0] = (p->c.unk104[0] == 0) & 0xFF;
            if (p->c.unk104[0] == 0) {
                break;
            }
        }
        if (p->c.unk128 >= p->c.unk124) {
            PU(p, 0x16EF, u8) = 1;
            return;
        }
    }
    PU(p, 0x1568, f32) = VCALL(gNavMesh, 0x58, f32 (*)(void *, s32, s32))(gNavMesh, p->c.unk100, p->c.unk104[0]);
    if (!(PU(p, 0x1590, f32) < 4.0f)) {
        func_00214890(p, (u32 *)&tri, pos, 0x1.99999a0000000p-4f /* 0.1 */);
        if (tri != -1) {
            t = func_00213FA0(p, pos, 0x1.657186p+0f /* 80 degrees */, 0x1.657186p+1f /* 160 degrees */) & 0xFF;
        }
    }
    if ((t & 0xFF) == 0xFF) {
        if (PU(p, 0x1590, f32) < 10.0f) {
            Pursuer_PlayAnim(p, VCALL(p, 0x324, s32 (*)(Pursuer *))(p));
        } else {
            VCALL(p, 0x128, void (*)(Pursuer *))(p);
        }
    } else {
        func_00297300(p, t);
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECFA0);
    VCALL(p, 0x21C, void (*)(Pursuer *))(p);
}

/* vtable +0x...: close in on the planned goal; turn if it's off to the side; at the end
 * the animation +0x1624 and the behaviour again */
/* 0x00290DF0 */
void Pursuer_CloseInGoal(Pursuer *p) {
    f32 pos[4] __attribute__((aligned(16)));
    s32 tri = -1;
    s32 done = 0;

    if (PU(p, 0x1590, f32) < 0.0f) {
        if (!(func_00284440(p) & 0xFF)) {
            PU(p, 0x16EF, u8) = 1;
        }
        return;
    }
    if (!(PU(p, 0x1590, f32) < 4.0f)) {
        func_00214890(p, (u32 *)&tri, pos, 0x1.99999a0000000p-4f /* 0.1 */);
    }
    if (PU(p, 0x1788, s32) != 0x400) {
        s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

        if (!((over ^ 1) & 0xFF) && tri != -1) {
            u32 t = func_00213FA0(p, pos, 0x1.657186p+0f /* 80 degrees */, 0x1.657186p+1f /* 160 degrees */) & 0xFF;

            if (t != 0xFF) {
                func_00297300(p, t);
            }
        }
        if ((p->c.unk128 < p->c.unk124) == 1) {
            if (PU(p, 0x1628, s32) == 0 && PU(p, 0x15A4, u32) == func_00124480(&p->c.a, (f32 *)((u8 *)p + 0x15B0), -1)) {
                VCALL(p, 0xD8, s32 (*)(Pursuer *))(p);
                PU(p, 0x1628, s32) = 1;
            }
            done = func_00214620(p, p->c.unk128) & 0xFF;
        } else if (PU(p, 0x1590, f32) != 0.0f) {
            PU(p, 0x16EF, u8) = 1;
        } else {
            done = 1 & 0xFF;
        }
    } else {
        s32 over;

        func_00125A10(&p->c);
        over = MOTION_AT(p, 0x550, f32) <= 0.0f;
        if (!((over ^ 1) & 0xFF)) {
            f32 d = 1.0f;

            if (!(MOTION_KEYS(p) & MOTION_KEY_END)) {
                if (!(func_002E2D00(func_001244D0(&p->c.a, pos) - p->c.a.angle[1]) <= 0.0f)) {
                    d = func_002E2D00(func_001244D0(&p->c.a, pos) - p->c.a.angle[1]);
                } else {
                    d = -func_002E2D00(func_001244D0(&p->c.a, pos) - p->c.a.angle[1]);
                }
            }
            if ((MOTION_KEYS(p) & MOTION_KEY_END) || d < 0x1.99999ap-4f /* 0.1 */) {
                Pursuer_PlayAnim(p, PU(p, 0x1624, s32));
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, PU(p, 0x175C, s32));
                return;
            }
        }
    }
    if ((done & 0xFF) == 1) {
        PURSUER_STEP_DONE(p) = 1;
        PU(p, 0x1624, s32) = 0;
        PU(p, 0x1628, s32) = 0;
    }
}

/* vtable +0x...: grab Hewie from behind (kind 0xF) if there is a spot: Hewie told where to be
 * held, the state block set to {0xC, 30} (the rest of the original's stack copy was never
 * written) */
s32 func_0029A710(Pursuer *p) {
    f32 pos[4] __attribute__((aligned(16)));
    f32 h;
    s32 tri;

    if (func_00125D80(&p->c) & 0xFF) {
        return 0;
    }
    tri = func_00285DE0(p, 0xF, &h, pos);
    if (tri != -1 && (func_00123470(p, tri, (s32)pos) & 0xFF) == 1 && (u32)tri == func_00124480(&p->c.a, pos, 0x28020028)) {
        sceVu0CopyVector(gCharPartner->unk110, pos);
        AT(gCharPartner, 0x10C, f32) = h;
        gCharPartner->unk104[1] = tri;
        AT(p, 0x10C, f32) = h;
        if (p->c.state[0] != 7) {
            p->c.state[0] = 0xC;
            p->c.state[1] = 30;
            p->c.state[2] = 0;
            p->c.state[3] = 0;
            p->c.state[4] = 0;
            p->c.state[5] = 0;
            p->c.state[6] = 0;
            p->c.state[7] = 0;
        }
        return 1;
    }
    return 0;
}

/* ---- batch 14 ---- */

extern const PTMF D_003ED0C0, D_003ED040, D_003ECC00, D_003ECC10, D_003EC910, D_003EC920;

/* look around: the head swept by a sine over vtable +0x2DC frames (full swing +0x2E0, half
 * at the ends), then back to straight ahead */
/* 0x0028FF30 */
void Pursuer_LookAround(Pursuer *p) {
    s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;
    s32 t;

    if (((over ^ 1) & 0xFF) == 1) {
        func_00125A10(&p->c);
    }
    t = PU(p, 0x1624, s32);
    if ((f32)t < 1.5f * VCALL(p, 0x2DC, f32 (*)(Pursuer *))(p)) {
        f32 ph, amp;

        if (t >= (s32)(VCALL(p, 0x2DC, f32 (*)(Pursuer *))(p) / 2.0f) && PU(p, 0x1624, s32) < (s32)VCALL(p, 0x2DC, f32 (*)(Pursuer *))(p)) {
            ph = (f32)(s32)((f32)PU(p, 0x1624, s32) + VCALL(p, 0x2DC, f32 (*)(Pursuer *))(p) / 4.0f);
            amp = VCALL(p, 0x2E0, f32 (*)(Pursuer *))(p);
        } else {
            ph = (f32)(s32)((f32)PU(p, 0x1624, s32) + VCALL(p, 0x2DC, f32 (*)(Pursuer *))(p) / 4.0f);
            amp = VCALL(p, 0x2E0, f32 (*)(Pursuer *))(p) / 2.0f;
        }
        MOTION_AT(p, 0x858, f32) = func_002E2D00(MOTION_AT(p, 0x858, f32) +
                                                  amp * func_0031C058(func_002E2D00(ph * (0x1.921fb60000000p+2f /* 6.2831855 */ / VCALL(p, 0x2DC, f32 (*)(Pursuer *))(p)))));
        PU(p, 0x1574, f32) = func_002E2D00(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
        PU(p, 0x1624, s32)++;
    } else {
        u8 *m = p->c.motion;

        VCALL(m, 0x5C, void (*)(void *, f32))(m, VCALL(p, 0x2E0, f32 (*)(Pursuer *))(p));
        if (MOTION_AT(p, 0x858, f32) == 0.0f) {
            PU(p, 0x1624, s32) = 0;
            PURSUER_STEP_DONE(p) = 1;
        }
    }
}

/* stops for the search on entering a room (stance 2: 2..5, 3: 1..2) */
static u32 Pursuer_SearchStops(Pursuer *p) {
    switch (PU(p, 0x16C8, u8)) {
    case 2:
        return (((u32)(4.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom)) & 0xFF & 0xFF) + 2) & 0xFF;
    case 3:
        return (((u32)(2.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom)) & 0xFF & 0xFF) + 1) & 0xFF;
    default:
        return 0xFF;
    }
}

/* the search route on coming into a room (in the played one at once; else as time away) */
void func_0027EAF0(Pursuer *p) {
    if (func_00217510(p) != 0) {
        u8 k = PU(p, 0x16C8, u8);

        if (k == 3 || k == 2) {
            u32 n = Pursuer_SearchStops(p) & 0xFF;

            if ((s32)n > 0) {
                s32 left = PU(p, 0x1621, u8) - PU(p, 0x1620, u8);

                if (left >= (s32)n) {
                    if ((s32)n < left && n != (u32)left) {
                        do {
                            PU(p, 0x1620, u8)++;
                        } while (n != (u32)(PU(p, 0x1621, u8) - PU(p, 0x1620, u8)));
                    }
                } else {
                    func_0027E5D0(p, (n - left) & 0xFF);
                }
                PU(p, 0x1794, s32) = ((s32)n > 0 && n != 0xFF) ? 1800 : 0;
            } else {
                PU(p, 0x1621, u8) = 0;
                PU(p, 0x1620, u8) = 0;
            }
        }
        PU(p, 0x17B4, s32) = 0;
    } else {
        u8 k = PU(p, 0x16C8, u8);

        if (k == 3 || k == 2) {
            PU(p, 0x17B4, s32) = (Pursuer_SearchStops(p) & 0xFF) * 150;
        }
    }
}

/* closing in on Fiona: strike when in reach (vtable +0x2E4) and facing her (+0x2EC degrees),
 * else attack choice / chase on (0x16) */
static void Pursuer_CloseOnFiona(Pursuer *p, const PTMF *hit, const PTMF *after, s32 a5) {
    if ((Pursuer_WalkOn(p) & 0xFF) != 1) {
        f32 d;

        if (gCharPlayer->moveMode == 3) {
            VCALL(p, 0x134, void (*)(Pursuer *))(p);
            return;
        }
        d = PU(p, 0x1588, f32);
        if (d < VCALL(p, 0x2E4, f32 (*)(Pursuer *))(p) && !(d < 0.0f)) {
            f32 a;

            if (!(func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]) <= 0.0f)) {
                a = func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
            } else {
                a = -func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
            }
            if (a < 0x1.921fb60000000p+1f /* 3.1415927 */ * VCALL(p, 0x2EC, f32 (*)(Pursuer *))(p) / 180.0f) {
                s32 can;

                if (p->target->a.unk2D == 1 || (Progress_TestFlag(gProgress, 0xE) & 0xFF) == 1 || gCharPlayer->moveSub == 0x10) {
                    can = 0;
                } else {
                    can = 1 & 0xFF;
                }
                if (can != 0) {
                    if (((s32 (*)(Pursuer *, Character *, u32))func_001235C0)(p, gCharPlayer, 0) != 0) {
                        Actor_SetState(&p->c.a, hit);
                        Relation_Request(gProgress, *(u8 *)&p->c.a.slot, 1, 9, 0, a5, 0.0f);
                        Actor_SetState(&p->c.a, after);
                        return;
                    }
                    VCALL(p, 0x134, void (*)(Pursuer *))(p);
                    return;
                }
                VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, 3);
                func_00283C50(p);
                return;
            }
        }
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x16);
        return;
    }
    {
        Character *t = gCharPlayer != NULL ? gCharPlayer : p->target;
        f32 h = func_001244D0(&p->c.a, t->a.pos);

        func_002140A0(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    }
}

void func_0028A190(Pursuer *p) {
    Pursuer_CloseOnFiona(p, &D_003ED0C0, &D_003ED0D0, 8);
}

void func_0028AB10(Pursuer *p) {
    Pursuer_CloseOnFiona(p, &D_003ED040, &D_003ED050, 6);
}

/* vtable +0x264: the chase decision: in reach (the ground gained in 5 frames), on the same
 * floor, facing her and the threat below stage 4: strike (1); else by the stage's table
 * (+0x1730: {chance, ...}): 5 or 6 */
/* 0x002961D0 */
void Pursuer_ChaseDecision(Pursuer *p) {
    Progress *pr;
    u8 *e;

    if (PU(p, 0x16F3, u8) == 1) {
        PU(p, 0x16F3, u8) = 0;
        if (PU(p, 0x1758, s32) != -2) {
            VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1C);
        }
    }
    pr = gProgress;
    e = PU(p, 0x1730, u8 *) + AT(pr, 0x7B8, u8) * 0xC;
    PU(p, 0x1588, f32) = func_00213D40(p, gCharPlayer);
    switch (PU(p, 0x1758, s32)) {
    case -2:
        break;
    case 0x1C:
        VCALL(gRumble, 0x18, void (*)(VObject *, s32, s32, s32))(gRumble, 3, 0x80, 30);
        /* fallthrough */
    default:
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, PU(p, 0x1758, s32));
        break;
    case -1: {
        f32 d = PU(p, 0x1588, f32);
        s32 strike = 0;

        if (d < func_002838E0(p) && !(d < 0.0f) && func_002175B0(&p->c.a, &gCharPlayer->a) != 0) {
            f32 a;

            if (!(func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]) <= 0.0f)) {
                a = func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
            } else {
                a = -func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
            }
            if (a < 0x1.921fb60000000p+1f /* 3.1415927 */ * VCALL(p, 0x2EC, f32 (*)(Pursuer *))(p) / 180.0f && AT(pr, 0x7B8, u8) < 4) {
                strike = 1;
            }
        }
        if (strike) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
        } else if (100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) <= AT(e, 0x0, f32)) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
            PU(p, 0x162C, s32) = AT(e, 0x4, s32);
        } else {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
            PU(p, 0x162C, s32) = AT(e, 0x8, s32);
        }
        break;
    }
    }
    p->target = gCharPlayer;
    PU(p, 0x16F6, u8) = 1;
    PU(p, 0x16ED, u8) = 0;
    PURSUER_STEP_DONE(p) = 0;
    PU(p, 0x16EF, u8) = 0;
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1780, s32) = 0;
    if (AT(pr, 0x1FBEC1, u8) != 0) {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECC00);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x26C, void (*)(Pursuer *))(p);
        return;
    }
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECC10);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x268, void (*)(Pursuer *))(p);
}

/* vtable +0x70 ...: back from the save (see Pursuer_SaveState) */
/* 0x0029E610 */
void Pursuer_LoadState(Pursuer *p) {
    u8 *s = (u8 *)gProgress + 0x83C;

    p->c.a.room = AT(gProgress, 0x83C, s32);
    p->c.a.unkC4 = AT(gProgress, 0x844, s32);
    PU(p, 0x16B8, s32) = AT(gProgress, 0x848, s32);
    p->c.hp = AT(gProgress, 0x850, s32);
    p->c.hpMax = AT(gProgress, 0x854, s32);
    PU(p, 0x1594, s32) = AT(gProgress, 0x858, s32);
    PU(p, 0x15A4, s32) = AT(gProgress, 0x85C, s32);
    PU(p, 0x16BC, s32) = AT(gProgress, 0x860, s32);
    PU(p, 0x16C0, f32) = (f32)AT(gProgress, 0x864, u32);
    p->c.door = AT(s, 0x2C, u8);
    PU(p, 0x16C4, s32) = AT(s, 0x2D, u8);
    PU(p, 0x16C8, u8) = AT(s, 0x2E, u8);
    PU(p, 0x1660, s32) = AT(s, 0x30, s32);
    PU(p, 0x1664, s32) = AT(s, 0x34, s32);
    if (p->c.a.active != 0) {
        if (func_00217510(p) == 0) {
            if (p->c.a.unkC4 == 2) {
                ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003EC910);
                PU(p, 0x1758, s32) = -1;
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x20);
                Pursuer_PlayAnimBlend(p, 0x1806);
            } else {
                ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003EC920);
                PU(p, 0x1758, s32) = -1;
            }
        } else {
            p->c.a.navTri = AT(s, 0x4, u32);
            p->c.a.angle[1] = AT(s, 0x10, f32);
            VCALL(p, 0x13C, void (*)(Pursuer *))(p);
            if (p->c.a.unkC4 == 2) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x20);
                Pursuer_PlayAnimBlend(p, 0x1806);
            }
        }
    }
    if (PU(p, 0x16B8, s32) == 2) {
        VCALL(p, 0x31C, void (*)(Pursuer *, s32))(p, 1);
    }
}

/* ---- batch 15 ---- */

extern const PTMF D_003ED160, D_003ECFF0, D_003ED000, D_003ECEB0, D_003ECEA0;
extern PTMF D_0045B370, D_0045B3B8;   /* more moves (see Pursuer_SetMove) */

/* vtable +0x...: knocked down (0x1709 / 0x1804 falling forward if there's room / 0x1800);
 * out of health: down for good (stance cleared), and the progress counter of knock-outs +0xFBC
 * goes up (to 9999) */
void func_002885B0(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (p->c.unk104[0] == 0x1709) {
        Pursuer_PlayAnimBlend(p, 0x1709);
    } else if (((s32 (*)(Pursuer *, Character *, u32))func_001235C0)(p, (Character *)p, 0) != 0 && func_00217600(p) != 0) {
        Pursuer_PlayAnim(p, 0x1804);
    } else {
        Pursuer_PlayAnim(p, 0x1800);
    }
    p->c.unk104[0] = 0;
    if (p->c.hp <= 0) {
        Progress *pr;
        s16 n;

        VCALL(p, 0x2CC, void (*)(Pursuer *))(p);
        PU(p, 0x1790, s32) = 0;
        pr = gProgress;
        PU(p, 0x16F5, u8) = 0;
        p->c.a.unkC4 = 2;
        AT(pr, 0x874, u8) = 0;
        PU(p, 0x16C9, u8) = 0;
        PU(p, 0x16CB, u8) = 0;
        PU(p, 0x16CA, u8) = 0;
        PU(p, 0x16CC, u8) = 0;
        PU(p, 0x179C, s32) = -1;
        PU(p, 0x16F1, u8) = 0;
        PU(p, 0x16F3, u8) = 0;
        PU(p, 0x16F2, u8) = 0;
        PU(p, 0x16F4, u8) = 0;
        VCALL(p, 0x31C, void (*)(Pursuer *, s32))(p, 0);
        n = AT(pr, 0xFBC, s16) + 1;
        if (n >= 10000) {
            n = 9999;
        }
        AT(pr, 0xFBC, s16) = n;
    }
    PU(p, 0x1628, s32) = 0;
    PU(p, 0x1544, u8) = 0;
    PU(p, 0x1545, u8) = 0;
    VCALL(p, 0x104, void (*)(Pursuer *))(p);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED160);
    func_00288150(p);
}

/* the next step of the attack (+0x1728 attack, +0x172C step; table +0x1724 of step entries
 * of +0x171C, 0x24 bytes): its animation, kind 1/2/4 (move 0x1A..0x1C) or 6 (the hit) */
void func_0028B970(Pursuer *p) {
    u8 *tab;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    tab = PU(p, 0x171C, u8 *);
    if (tab == NULL) {
        PU(p, 0x16EF, u8) = 1;
        PURSUER_STEP_NEXT(p) = 1;
        return;
    }
    if (PU(p, 0x172C, s8) < 4) {
        s8 k = PU(p, 0x1724, s8 *)[PU(p, 0x1728, s32) * 4 + PU(p, 0x172C, s8)];

        if (k != -1 && p->c.a.room == p->target->a.room) {
            s32 go = 1;

            if (AT(tab + k * 0x24, 0x10, u8) != 6) {
                f32 reach = 50.0f;

                if (func_002838E0(p) > 50.0f) {
                    reach = func_002838E0(p);
                }
                go = func_00124490(&p->c.a, p->target->a.pos) < reach;
            }
            if (go) {
                u8 *e = PU(p, 0x171C, u8 *) + PU(p, 0x1724, s8 *)[PU(p, 0x1728, s32) * 4 + PU(p, 0x172C, s8)] * 0x24;

                PU(p, 0x1760, u8) = 0;
                Pursuer_PlayAnim(p, AT(e, 0x0, s32));
                PU(p, 0x16F7, u8) = AT(e, 0x1D, u8);
                PU(p, 0x1784, s32) = 0;
                switch (AT(e, 0x10, u8)) {
                case 6:
                    Actor_SetState(&p->c.a, &D_003ECFF0);
                    func_0028B0D0(p);
                    return;
                case 1:
                    p->c.moveSub = 0x1A;
                    break;
                case 2:
                    p->c.moveSub = 0x1B;
                    break;
                case 4:
                    p->c.moveSub = 0x1C;
                    break;
                }
                p->c.unk104[0] = 0;
                Actor_SetState(&p->c.a, &D_003ED000);
                func_0028B340(p);
                return;
            }
        }
    }
    p->c.unk104[0] = -1;
    PU(p, 0x172C, u8) = 0;
    PU(p, 0x1728, s32) = -1;
    PURSUER_STEP_DONE(p) = 1;
    PU(p, 0x16F7, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
}

/* vtable +0x158: the off-screen move step (+0x17A0) and what follows it by its kind +0x17AC */
/* 0x0027E040 */
void Pursuer_OffscreenStep(Pursuer *p) {
    PTMF *st;

    if (PU(p, 0x16ED, u8) == 1) {
        return;
    }
    st = (PTMF *)((u8 *)p + 0x17A0);
    if (ptmf_test(st)) {
        ptmf_scall(p, st);
    }
    switch (PU(p, 0x17AC, u32)) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (PURSUER_STEP_DONE(p) == 1) {
            Pursuer_SetMove(p, &D_0045B3B8);
            PURSUER_STEP_DONE(p) = 0;
            PU(p, 0x17B4, s32) = 0;
            return;
        }
        if (PU(p, 0x17B4, s32) != 0) {
            Pursuer_SetMove(p, &D_0045B3A0);
        } else if (!(func_00126E40(&p->c) <= 0.0f) &&
                   PU(p, 0x17B0, u8) != (VCALL(gRooms, 0x3C, u32 (*)(VObject *, u32, s32))(gRooms, PU(p, 0x138C, u16), p->c.a.room) & 0xFF)) {
            p->c.unk124 = p->c.unk128;
            Pursuer_SetMove(p, &D_0045B358);
        }
        break;
    case 4:
        if (PURSUER_STEP_DONE(p) == 1) {
            if (p->c.unk128 < p->c.unk124) {
                Pursuer_SetMove(p, &D_0045B370);
            } else {
                Pursuer_SetMove(p, &D_0045B358);
            }
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 5:
        if (PURSUER_STEP_DONE(p) == 1 && PU(p, 0x1664, s32) == 0 && func_0027F0A0(p, PU(p, 0x17B0, u8)) != 0) {
            PURSUER_STEP_DONE(p) = 0;
            func_002815E0(p, PU(p, 0x17B0, u8));
        }
        break;
    }
}

/* the head by +0x1710: 1 at Fiona (if seen), 2 at Hewie (if seen, a bit above him), 3 at the
 * goal point if in view, 5 at +0x1700; else ahead */
void func_00284040(Pursuer *p) {
    f32 pitch, yaw;
    u8 *m;

    switch (PU(p, 0x1710, u8)) {
    case 0:
        return;
    case 1:
        if (PU(p, 0x1544, u8) != 0) {
            Pursuer_LookAt(p, gCharPlayer->a.pos);
            return;
        }
        break;
    case 2:
        if (PU(p, 0x1545, u8) != 0) {
            f32 v[4] __attribute__((aligned(16)));
            f32 sp;

            sceVu0CopyVector(v, gCharPartner->a.pos);
            v[1] += 5.0f;
            func_002DD110(p->c.motion, v, &pitch, &yaw);
            sp = VCALL(p, 0xA4, f32 (*)(Pursuer *))(p);
            func_002DD310(p->c.motion, pitch, yaw, sp, VCALL(p, 0xA4, f32 (*)(Pursuer *))(p));
            PU(p, 0x1574, f32) = func_002E2D00(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
            return;
        }
        break;
    case 3:
        if (PU(p, 0x15A4, u32) == func_00124480(&p->c.a, (f32 *)((u8 *)p + 0x15B0), 0x40080) &&
            func_002181D0(p, p->c.a.pos, (f32 *)((u8 *)p + 0x15B0), PU(p, 0x1574, f32), PU(p, 0x1580, f32), PU(p, 0x1584, f32)) != 0) {
            f32 a;

            if (!(func_002E2D00(func_001244D0(&p->c.a, (f32 *)((u8 *)p + 0x15B0)) - p->c.a.angle[1]) <= 0.0f)) {
                a = func_002E2D00(func_001244D0(&p->c.a, (f32 *)((u8 *)p + 0x15B0)) - p->c.a.angle[1]);
            } else {
                a = -func_002E2D00(func_001244D0(&p->c.a, (f32 *)((u8 *)p + 0x15B0)) - p->c.a.angle[1]);
            }
            if (a < 0x1.0c15240000000p-1f /* 0.5235988 */) {
                Pursuer_LookAt(p, (f32 *)((u8 *)p + 0x15B0));
            }
            return;
        }
        break;
    case 5:
        Pursuer_LookAt(p, (f32 *)((u8 *)p + 0x1700));
        return;
    }
    m = p->c.motion;
    VCALL(m, 0x5C, void (*)(void *, f32))(m, VCALL(p, 0xA4, f32 (*)(Pursuer *))(p));
    PU(p, 0x1574, f32) = func_002E2D00(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
}

/* vtable +0x1D0: walking to the door; turn to the side if needed; at it, the next step */
/* 0x0028E4D0 */
void Pursuer_DoorWalking(Pursuer *p) {
    f32 pos[4] __attribute__((aligned(16)));
    s32 tri;

    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    tri = -1;
    if (!(PU(p, 0x1590, f32) < 4.0f)) {
        func_00214890(p, (u32 *)&tri, pos, 0x1.99999a0000000p-4f /* 0.1 */);
    }
    if (PU(p, 0x1788, s32) != 0x400 && PU(p, 0x1788, s32) != 0) {
        s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

        if (!((over ^ 1) & 0xFF)) {
            f32 d = PU(p, 0x1590, f32);

            if (!(d <= 0.0f) && d < 10.0f) {
                Pursuer_PlayAnim(p, VCALL(p, 0x324, s32 (*)(Pursuer *))(p));
                tri = -1;
            }
            if (tri != -1) {
                u32 t = func_00213FA0(p, pos, 0x1.657186p+0f /* 80 degrees */, 0x1.657186p+1f /* 160 degrees */) & 0xFF;

                if (t != 0xFF) {
                    func_00297300(p, t);
                }
            }
        }
        func_00214620(p, p->c.unk128);
    } else {
        s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

        if (!((over ^ 1) & 0xFF)) {
            s32 turn = 0;

            if (!(MOTION_KEYS(p) & MOTION_KEY_END)) {
                f32 d;

                if (!(func_002E2D00(func_001244D0(&p->c.a, pos) - p->c.a.angle[1]) <= 0.0f)) {
                    d = func_002E2D00(func_001244D0(&p->c.a, pos) - p->c.a.angle[1]);
                } else {
                    d = -func_002E2D00(func_001244D0(&p->c.a, pos) - p->c.a.angle[1]);
                }
                if (d < 0x1.99999a0000000p-4f /* 0.1 */) {
                    turn = 1;
                }
            } else {
                turn = 1;
            }
            if (turn) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0xB);
            }
        }
        func_00125A10(&p->c);
    }
    if (PU(p, 0x1590, f32) == 0.0f) {
        Actor_SetState(&p->c.a, &D_003ECEB0);
        VCALL(p, 0x1D4, void (*)(Pursuer *))(p);
    }
}

/* vtable +0x1CC: walk to the door +0x100 (plan with +0xDC), stance animation near it */
/* 0x0028E8E0 */
void Pursuer_DoorWalkTo(Pursuer *p) {
    f32 pos[4] __attribute__((aligned(16)));
    s32 tri = -1;
    u32 t = 0xFF;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    VCALL(p, 0xDC, s32 (*)(Pursuer *))(p);
    if (!(PU(p, 0x1590, f32) < 4.0f)) {
        func_00214890(p, (u32 *)&tri, pos, 0x1.99999a0000000p-4f /* 0.1 */);
        if (tri != -1) {
            t = func_00213FA0(p, pos, 0x1.657186p+0f, 0x1.657186p+1f) & 0xFF;
        }
    }
    if ((t & 0xFF) != 0xFF) {
        func_00297300(p, t);
    } else {
        Progress *pr = gProgress;

        if ((Progress_CurRoomFlag(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1 &&
            (PursuerGroup_Fields(pr, (u8)p->c.unk100, 0) & 0xFF & 4) && PU(p, 0x1590, f32) < 20.0f) {
            if (PU(p, 0x1788, s32) != 0) {
                Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
            }
        } else if (PU(p, 0x1590, f32) < 10.0f) {
            Pursuer_PlayAnim(p, VCALL(p, 0x324, s32 (*)(Pursuer *))(p));
        } else {
            VCALL(p, 0x128, void (*)(Pursuer *))(p);
        }
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECEA0);
    VCALL(p, 0x1D0, void (*)(Pursuer *))(p);
}

extern const PTMF D_003ED170, D_003ED180, D_003ED2B0, D_003ED2C0, D_003ED2D0, D_003ED2E0, D_003ED2F0,
    D_003ED300, D_003ED310, D_003ED320, D_003ED330, D_003ED340, D_003ECDC0;

/* state: knocked down (anim 0x1709) and getting up again (0x1806/0x1802) */
void func_00288150(Pursuer *p) {
    u8 *m = p->c.motion;
    s32 anim;

    if (((AT(AT(m, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0) != 1) {
        if (AT(m, 0x55C, s32) == 0x1709) {
            void *pr = gProgress;
            u32 c0;

            /* a stalker that was down with no health left gets one back, and a knock-out is
               taken off the counter */
            if ((PursuerGroup_Find(pr, 0x20, p->c.a.slot) & 0xFF) != 0xFF && p->c.hp <= 0) {
                s16 n;

                p->c.hp = 1;
                p->c.a.unkC4 = 0;
                PU(p, 0x1664, s32) = 0;
                n = AT(pr, 0xFBC, s16) - 1;
                AT(pr, 0xFBC, s16) = n >= 0 ? n : 0;
            }
            func_00213B60(p, -1);
            c0 = p->c.a.navMask;
            p->c.a.navMask = c0 | 0x80001;
            func_00125A10(&p->c);
            p->c.a.navMask = c0;
        } else {
            func_00213B60(p, -1);
            func_00125A10(&p->c);
        }
        return;
    }
    if (p->c.a.unkC4 != 2 && AT(m, 0x55C, s32) == 0x1709) {
        PU(p, 0x1784, s32) = 0;
        Actor_SetState(&p->c.a, &D_003ED170);
        if (p->c.a.unkC4 != 2 && MOTION_ANIM(p) == 0x1709) {
            PU(p, 0x1624, s32) = 0x1806;
        } else {
            PU(p, 0x1624, s32) = MOTION_ANIM(p);
        }
        PU(p, 0x1628, s32) = 0;
        PURSUER_STEP_NEXT(p) = 0;
        anim = PU(p, 0x1624, s32);
        if (anim == 0x1807 || anim == 0x1803) {
            Actor_SetState(&p->c.a, &D_003ED190);
            func_00287B50(p);
        } else {
            Actor_SetState(&p->c.a, &D_003ED1A0);
            func_00287CD0(p);
        }
        return;
    }
    switch (AT(m, 0x55C, s32)) {
    case 0x1709:
    case 0x1804:
        Pursuer_PlayAnim(p, 0x1806);
        break;
    case 0x1800:
        Pursuer_PlayAnim(p, 0x1802);
        break;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED180);
}

/* pick the behaviour step (+0x174C) for the current mode +0x16C8 */
/* 0x00284540 */
void Pursuer_PickStep(Pursuer *p) {
    PTMF *step = (PTMF *)((u8 *)p + 0x174C);

    if (AT(gProgress, 0x1FBEC1, u8) != 0) {
        VCALL(p, 0x140, void (*)(Pursuer *))(p);
        return;
    }
    if (p->c.a.unkC4 == 2) {
        ptmf_set(step, &D_003ED2B0);
        PU(p, 0x1758, s32) = -1;
        return;
    }
    switch (PU(p, 0x16C8, u8)) {
    case 0:
        PU(p, 0x1544, u8) = VCALL(p, 0xC0, s32 (*)(Pursuer *))(p);
        if (PU(p, 0x1544, u8) != 1) {
            VCALL(p, 0xB0, void (*)(Pursuer *))(p);
            ptmf_set(step, &D_003ED320);
        } else {
            ptmf_set(step, &D_003ED310);
        }
        break;
    case 1:
        if (PU(p, 0x16C9, u8) == 6) {
            VCALL(p, 0xB0, void (*)(Pursuer *))(p);
        }
        ptmf_set(step, &D_003ED2E0);
        break;
    case 2: {
        u8 k = PU(p, 0x16C9, u8);
        s32 tri;

        if (k != 4 && k != 1 && k != 5) {
            ptmf_set(step, &D_003ED300);
            break;
        }
        ptmf_set(step, &D_003ED2F0);
        PU(p, 0x1758, s32) = -1;
        tri = PU(p, 0x179C, s32);
        if (tri != -1) {
            PU(p, 0x15A4, s32) = tri;
            VCALL(gNavMesh, 0xC, void (*)(void *, s32, f32 *))(gNavMesh, PU(p, 0x15A4, s32), (f32 *)((u8 *)p + 0x15B0));
        }
        return;
    }
    case 3:
        if (PU(p, 0x1620, u8) >= PU(p, 0x1621, u8) && PU(p, 0x1621, u8) != 0xFF) {
            ptmf_set(step, &D_003ED2D0);
        } else {
            ptmf_set(step, &D_003ED2C0);
        }
        break;
    case 4:
        ptmf_set(step, &D_003ED330);
        break;
    default:
        ptmf_set(step, &D_003ED340);
        break;
    }
    PU(p, 0x1758, s32) = -1;
}

/* step back from an obstacle onto the nav mesh, then pick an animation and start moving */
/* 0x00291190 */
void Pursuer_StepBack(Pursuer *p) {
    u32 tri;
    f32 pos[4] __attribute__((aligned(16)));
    u32 dir;
    s32 anim;

    if (Pursuer_WalkOn(p)) {
        return;
    }
    tri = -1;
    dir = 0xFF;
    if (!(PU(p, 0x1590, f32) < 4.0f)) {
        func_00214890(p, &tri, pos, 0x1.99999ap-4f /* 0.1 */);
        if (tri != (u32)-1) {
            dir = func_00213FA0(p, pos, 0x1.657186p+0f /* 80 degrees */, 0x1.657186p+1f /* 160 degrees */) & 0xFF;
        }
    }
    if (p->c.unkE0 != 0) {
        if (p->c.unk104[1] == -1) {
            p->c.unk104[1] = VCALL(p, 0x328, s32 (*)(Pursuer *))(p);
        }
        if ((dir & 0xFF) == 0xFF) {
            Pursuer_PlayAnim(p, p->c.unk104[1]);
        } else {
            func_00297300(p, dir);
        }
        anim = p->c.unk104[1];
    } else if ((dir & 0xFF) == 0xFF) {
        if (p->target != gCharPartner) {
            VCALL(p, 0x128, void (*)(Pursuer *))(p);
        } else {
            Pursuer_PlayAnim(p, VCALL(p, 0x328, s32 (*)(Pursuer *))(p));
        }
        anim = MOTION_ANIM(p);
    } else {
        func_00297300(p, dir);
        if (p->target != gCharPartner && PU(p, 0x16C8, u8) == 3) {
            anim = VCALL(p, 0x324, s32 (*)(Pursuer *))(p);
        } else {
            anim = VCALL(p, 0x328, s32 (*)(Pursuer *))(p);
        }
    }
    PU(p, 0x1624, s32) = anim;
    PU(p, 0x1628, s32) = PU(p, 0x15A4, u32) == func_00124480(&p->c.a, (f32 *)((u8 *)p + 0x15B0), -1) ? 1 : 0;
    Actor_SetState(&p->c.a, &D_003ECDC0);
    VCALL(p, 0x19C, void (*)(Pursuer *))(p);
}

extern const PTMF D_003ED620, D_003ED630, D_003ED640, D_003ED650, D_003ED660, D_003ED670, D_003ED680,
    D_003ED690, D_003ED6A0, D_003ED6B0, D_003ED6C0;

/* as Pursuer_PickStep, another set of steps; mode 0 only looks around in Fiona's room */
/* 0x00278EB0 */
void Pursuer_PickStepAlt(Pursuer *p) {
    PTMF *step = (PTMF *)((u8 *)p + 0x174C);

    if (p->c.a.unkC4 == 2) {
        ptmf_set(step, &D_003ED620);
        PU(p, 0x1758, s32) = -1;
        return;
    }
    switch (PU(p, 0x16C8, u8)) {
    case 0:
        if (p->c.a.room != gCharPlayer->a.room) {
            ptmf_set(step, &D_003ED680);
            break;
        }
        PU(p, 0x1544, u8) = VCALL(p, 0xC0, s32 (*)(Pursuer *))(p);
        if (PU(p, 0x1544, u8) != 1) {
            VCALL(p, 0xB0, void (*)(Pursuer *))(p);
            ptmf_set(step, &D_003ED6A0);
        } else {
            ptmf_set(step, &D_003ED690);
        }
        break;
    case 1:
        if (PU(p, 0x16C9, u8) == 6) {
            VCALL(p, 0xB0, void (*)(Pursuer *))(p);
        }
        ptmf_set(step, &D_003ED650);
        break;
    case 2: {
        u8 k = PU(p, 0x16C9, u8);
        s32 tri;

        if (k != 4 && k != 1 && k != 5) {
            ptmf_set(step, &D_003ED670);
            break;
        }
        ptmf_set(step, &D_003ED660);
        PU(p, 0x1758, s32) = -1;
        tri = PU(p, 0x179C, s32);
        if (tri != -1) {
            PU(p, 0x15A4, s32) = tri;
            VCALL(gNavMesh, 0xC, void (*)(void *, s32, f32 *))(gNavMesh, PU(p, 0x15A4, s32), (f32 *)((u8 *)p + 0x15B0));
        }
        return;
    }
    case 3:
        if (PU(p, 0x1620, u8) >= PU(p, 0x1621, u8) && PU(p, 0x1621, u8) != 0xFF) {
            ptmf_set(step, &D_003ED640);
        } else {
            ptmf_set(step, &D_003ED630);
        }
        break;
    case 4:
        ptmf_set(step, &D_003ED6B0);
        break;
    default:
        ptmf_set(step, &D_003ED6C0);
        break;
    }
    PU(p, 0x1758, s32) = -1;
}

extern const PTMF D_003ECFB0;

/* walk to the spot +0x15D0; once there, stand (move mode 3) and hand over to vtable +0x220 */
/* 0x0028C560 */
void Pursuer_WalkToSpot(Pursuer *p) {
    f32 pos[4] __attribute__((aligned(16))) = { 0.0f, 0.0f, 0.0f, 0.0f };   /* read even when no triangle was found */
    u32 tri;
    u8 *m;

    if (PU(p, 0x1590, f32) == 0.0f) {
        f32 d[4] __attribute__((aligned(16)));
        f32 v[4] __attribute__((aligned(16)));

        sceVu0SubVector(d, p->c.a.pos, (f32 *)((u8 *)p + 0x15D0));
        d[3] = 0.0f;
        sceVu0CopyVector(v, d);
        v[1] = 0.0f;
        if (__builtin_sqrtf(sceVu0InnerProduct(v, v)) <= 0.0f) {
            p->c.moveMode = 3;
            PU(p, 0x1710, u8) = 4;
            PURSUER_STEP_NEXT(p) = 0;
            Actor_SetState(&p->c.a, &D_003ECFB0);
            VCALL(p, 0x220, void (*)(Pursuer *))(p);
            return;
        }
        VCALL(p, 0xDC, void (*)(Pursuer *))(p);
    }
    if (Pursuer_WalkOn(p)) {
        return;
    }
    tri = -1;
    if (!(PU(p, 0x1590, f32) < 4.0f)) {
        func_00214890(p, &tri, pos, 0x1.99999ap-4f /* 0.1 */);
    }
    m = p->c.motion;
    if (PU(p, 0x1788, s32) != 0x400) {
        if (!(AT(m, 0x550, f32) > 0.0f)) {
            f32 left = PU(p, 0x1590, f32);

            /* nearly there: the arrival animation instead of turning */
            if (!(left <= 0.0f) && left < 10.0f) {
                Pursuer_PlayAnim(p, VCALL(p, 0x324, s32 (*)(Pursuer *))(p));
                tri = -1;
            }
            if (tri != (u32)-1) {
                u32 dir = func_00213FA0(p, pos, 0x1.657186p+0f /* 80 degrees */, 0x1.657186p+1f /* 160 degrees */) & 0xFF;

                if (dir != 0xFF) {
                    func_00297300(p, dir);
                }
            }
        }
        func_00214620(p, 0);
        return;
    }
    if (!(AT(m, 0x550, f32) > 0.0f)) {
        if ((AT(AT(m, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x12);
        } else {
            f32 a;

            if (!(func_002E2D00(func_001244D0(&p->c.a, pos) - p->c.a.angle[1]) <= 0.0f)) {
                a = func_002E2D00(func_001244D0(&p->c.a, pos) - p->c.a.angle[1]);
            } else {
                a = -func_002E2D00(func_001244D0(&p->c.a, pos) - p->c.a.angle[1]);
            }
            if (a < 0x1.99999ap-4f /* 0.1 */) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x12);
            }
        }
    }
    func_00125A10(&p->c);
}

extern const PTMF D_003ECD40, D_003ECD50;

/* the frame update: run the state; a step done, either give up (vtable +0x11C), go on to the
   next behaviour, or rest; otherwise, at an animation's hit key, strike at Fiona when in reach */
/* 0x00292310 */
void Pursuer_FrameUpdate(Pursuer *p) {
    if (p->c.a.unkC4 == 2) {
        PU(p, 0x1544, u8) = 0;
        PU(p, 0x1545, u8) = 0;
        PU(p, 0x1546, u8) = 0;
    }
    if (ptmf_test(&p->c.a.state)) {
        ptmf_scall(p, &p->c.a.state);
    }
    if (PURSUER_STEP_DONE(p) == 1) {
        AT(p, 0x2A, u8) = 0;
        AT(p, 0x2B, u8) = 0;
        if (p->c.hp <= 0) {
            p->c.hp = p->c.hpMax;
        }
        if (VCALL(p, 0x11C, s32 (*)(Pursuer *))(p) != 0) {
            PU(p, 0x16C4, s32) = 0;
            if (VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(gProgress, 8) == 0) {
                func_00122C20(&p->c.a, 0x20, 7, 0, 0, NULL);
            }
            p->c.unk1384 = p->c.unk1388;
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECD40);
            PU(p, 0x1758, s32) = -1;
            VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x25);
            PU(p, 0x16C8, u8) = 4;
            VCALL(p, 0x2C8, void (*)(Pursuer *, s32))(p, 0);
        } else if (PU(p, 0x16DC, u32) < PU(p, 0x16BC, u32) && Progress_TestFlag(gProgress, 0xE) == 0 &&
                   PU(p, 0x16C8, u8) != 4 && ((PU(p, 0x1761, u8) & 2) || p->target == gCharPartner)) {
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECD50);
            PU(p, 0x1758, s32) = -1;
            PU(p, 0x16F8, u8) = 1;
        } else {
            func_0027FE90(p);
            VCALL(p, 0x13C, void (*)(Pursuer *))(p);
        }
        PURSUER_STEP_DONE(p) = 0;
        PU(p, 0x1761, u8) = 0;
        if (PU(p, 0x16F5, u8) != 0 && PU(p, 0x16C8, u8) != 4 &&
            VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(gProgress, 8) == 0) {
            func_00122C20(&p->c.a, 0x1F, 7, 0, 0, NULL);
        }
        PU(p, 0x16F5, u8) = 0;
        if (PU(p, 0x16C8, u8) != 4) {
            PU(p, 0x16F6, u8) = 1;
        }
        return;
    }
    if (func_001F4770(p->c.motion, 0, 0, 1) & 0xFF & 2) {
        s32 may;

        /* func_00283870, inline */
        if (p->target->a.unk2D == 1 || (Progress_TestFlag(gProgress, 0xE) & 0xFF) == 1) {
            may = 0;
        } else {
            may = gCharPlayer->moveSub != 0x10;
        }
        if (may && (PU(p, 0x1761, u8) & 1) && p->target == gCharPlayer && PU(p, 0x175C, s32) == 0x1E &&
            PU(p, 0x16C8, u8) != 4) {
            f32 a;

            if (!(func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]) <= 0.0f)) {
                a = func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
            } else {
                a = -func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
            }
            /* in front or behind */
            VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, a < 0x1.921fb6p+0f /* 90 degrees */ ? 8 : 9);
            PU(p, 0x1761, u8) = 0;
            func_00283C50(p);
        }
    }
}

/* off-screen update: run the move step +0x17A0; follow Fiona's room while idle, and choose the
   next move from what the last one (+0x17AC kind) finished with */
/* 0x0027A1E0 */
void Pursuer_OffscreenUpdate(Pursuer *p) {
    if (PU(p, 0x16ED, u8) == 1) {
        return;
    }
    if (ptmf_test((PTMF *)((u8 *)p + 0x17A0))) {
        ptmf_scall(p, (PTMF *)((u8 *)p + 0x17A0));
    }
    if (PU(p, 0x16C8, u8) == 0 && func_00217510(p) == 0) {
        if (PU(p, 0x1594, s32) != gCharPlayer->a.room || func_00217340(p) != func_002172F0(p, gCharPlayer)) {
            p->c.unk124 = p->c.unk128;
            VCALL(p, 0xB0, void (*)(Pursuer *))(p);
            p->c.unk1538 = 0;
            p->c.unk1530 = 0;
            p->c.unk1534 = 0;
            PU(p, 0x17B4, s32) = 0;
        }
        if (p->c.a.room == gCharPlayer->a.room && func_00217340(p) == func_002172F0(p, gCharPlayer)) {
            PU(p, 0x17B4, s32) = 150;
        }
    }
    switch (PU(p, 0x17AC, u32)) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (PURSUER_STEP_DONE(p) == 1) {
            Pursuer_SetMove(p, &D_0045B3B8);
            PURSUER_STEP_DONE(p) = 0;
            PU(p, 0x17B4, s32) = 0;
        } else if (PU(p, 0x17B4, s32) != 0) {
            Pursuer_SetMove(p, &D_0045B3A0);
        } else if (!(func_00126E40(&p->c) <= 0.0f)) {
            u8 want = PU(p, 0x17B0, u8);

            if (want != (VCALL(gRooms, 0x3C, s32 (*)(VObject *, u32, s32))(gRooms, PU(p, 0x138C, u16), p->c.a.room) & 0xFF)) {
                p->c.unk124 = p->c.unk128;
                Pursuer_SetMove(p, &D_0045B358);
            }
        }
        break;
    case 4:
        if (PURSUER_STEP_DONE(p) == 1) {
            Pursuer_SetMove(p, p->c.unk128 < p->c.unk124 ? &D_0045B370 : &D_0045B358);
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 5:
        if (PURSUER_STEP_DONE(p) == 1 && PU(p, 0x1664, s32) == 0 && func_0027F0A0(p, PU(p, 0x17B0, u8)) != 0) {
            PURSUER_STEP_DONE(p) = 0;
            func_002815E0(p, PU(p, 0x17B0, u8));
        }
        break;
    }
}

extern const PTMF D_003ED450, D_003ED460, D_003ED470, D_003ED480, D_003ED490, D_003ED4A0;

/* come into a room through the door +0x14D4: stand at its triangle facing in, pick the next
   behaviour, and react to a locked or barred door */
/* 0x002804B0 */
void Pursuer_EnterRoom(Pursuer *p) {
    VObject *rooms;
    f32 ang;

    if (func_00217510(p) == 0) {
        p->c.a.navTri = -1;
        if (p->c.a.unkC4 != 2) {
            func_0027EAF0(p);
        }
        return;
    }
    {
        VObject *o = VCALL(gEvents, 0x64, VObject *(*)(VObject *))(gEvents);
        s32 *t = VCALL(o, 0x38, s32 *(*)(VObject *))(o);

        if (t != NULL) {
            u8 n = 0;

            /* a random wait over the room's (up to 8) entries */
            while (t[n] != -1 && ++n < 8) {
            }
            PU(p, 0x1738, u32) = (u32)((f32)n * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom));
        }
    }
    rooms = gRooms;
    ang = VCALL(rooms, 0x8C, f32 (*)(VObject *, u32))(rooms, p->c.door);
    if (PU(p, 0x1624, s32) == 1) {
        p->c.a.navTri = VCALL(rooms, 0x28, s32 (*)(VObject *, u32))(rooms, p->c.door);
        VCALL(p, 0x28, void (*)(Pursuer *, s32, f32 *, s32))(p, p->c.a.navTri, &ang, 0);
        PU(p, 0x1624, s32) = 0;
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED450);
        PU(p, 0x1758, s32) = -1;
        PU(p, 0x16F6, u8) = 1;
    } else {
        PTMF *step = (PTMF *)((u8 *)p + 0x174C);

        p->c.a.navTri = VCALL(rooms, 0x24, s32 (*)(VObject *, u32))(rooms, p->c.door);
        ang = func_002E2D00(0x1.921fb6p+1f /* pi */ + ang);
        VCALL(p, 0x28, void (*)(Pursuer *, s32, f32 *, s32))(p, p->c.a.navTri, &ang, 0);
        if (!__ptmf_cmpr(step, &D_003ED460) || !__ptmf_cmpr(step, &D_003ED470) || !__ptmf_cmpr(step, &D_003ED480)) {
            func_00280090(p);
            func_0027EAF0(p);
            VCALL(p, 0x13C, void (*)(Pursuer *))(p);
            PU(p, 0x16F6, u8) = 1;
        } else if (PU(p, 0x16C8, u8) == 4) {
            s32 room;
            s32 *e;
            s32 i;

            ptmf_set(step, &D_003ED490);
            PU(p, 0x1758, s32) = -1;
            room = p->c.a.room;
            e = VCALL(p, 0x314, s32 *(*)(Pursuer *))(p);
            for (i = 0; *e != -1; i++, e += 2) {
                if (room == *e) {
                    func_00214AF0(p);
                    break;
                }
            }
            VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x26);
            PU(p, 0x16F6, u8) = 0;
        } else {
            ptmf_set(step, &D_003ED4A0);
            PU(p, 0x1758, s32) = -1;
            PU(p, 0x16F6, u8) = 1;
        }
    }
    PURSUER_STEP_NEXT(p) = 1;
    if ((VCALL(gDoors, 0x40, s32 (*)(VObject *, u32))(gDoors, p->c.door) & 0xFF) == 1) {
        Progress *pr = gProgress;

        if (func_00178980(pr, p->c.a.room, p->c.door) & 0xFF) {
            u8 k = PU(p, 0x16C9, u8);

            if ((u32)(PU(p, 0x16C8, u8) - 2) < 2 && k != 4 && k != 1 && k != 5) {
                VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x28);
            }
        } else if (!(DoorHold_Take(pr, p->c.a.room, p->c.door, *(u8 *)&p->c.a.slot) & 0xFF)) {
            /* the door won't open for it: give up on it, with a rumble */
            VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0xC);
            p->c.a.unk2D = 1;
            VCALL(gRumble, 0x18, void (*)(VObject *, s32, s32, s32))(gRumble, 3, 0x80, 0xF);
        }
    }
    func_00126270(&p->c);
}

extern const PTMF D_003ECFD0, D_003ECFE0;

/* climb the stairs +0x100, up (+0x104 = 1) or down, one step animation after another (0x701/0x702
   up, 0x705/0x706 down); turn round when the goal +0x15A4 is the other way, and step off at the
   top (0x703) or bottom (0x707) */
/* 0x0028BEF0 */
void Pursuer_Stairs(Pursuer *p) {
    Progress *pr = gProgress;
    f32 end[4] __attribute__((aligned(16)));
    f32 ofs[4] __attribute__((aligned(16)));
    s32 turn;
    s32 anim;

    if (!(Progress_TestFlag(pr, 0xE) & 0xFF)) {
        u32 m = func_00217920(p) & 0xFF;

        if (~PU(p, 0x1760, u8) & m) {
            Relation_Request(pr, *(u8 *)&p->c.a.slot, m & 0xFF, 4, 5, 0, 10.0f);
        }
    }
    if (((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF) {
        return;
    }
    p->c.moveSub = 7;
    if (!((AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0)) {
        f32 v[4] __attribute__((aligned(16)));

        /* mid step: root motion */
        func_001F6370(p->c.motion, v, 0.0f);
        func_00125900(&p->c);
        sceVu0ApplyMatrix(v, (void *)((u8 *)p + 0x60), v);
        sceVu0AddVector(p->c.a.pos, p->c.a.pos, v);
        p->c.a.pos[3] = 1.0f;
        return;
    }
    turn = 0;
    if (PU(p, 0x1624, s32) != PU(p, 0x15A4, s32) && func_00217260(p) != 0) {
        u8 k = PU(p, 0x16C8, u8);

        if (k != 4 && (k == 3 || k == 1 || k == 2 || k == 0)) {
            if (p->c.unk104[0] == (s32)(func_00212E00(p, PU(p, 0x15A4, s32), (f32 *)((u8 *)p + 0x15B0), (u8)p->c.unk100) & 0xFF)) {
                turn = 1;
            } else {
                PU(p, 0x1624, s32) = PU(p, 0x15A4, s32);
            }
        }
    }
    anim = MOTION_ANIM(p);
    if (p->c.unk104[0] == 1) {
        VCALL(gNavMesh, 0x5C, void (*)(void *, s32, s32, f32 *))(gNavMesh, p->c.unk100, 0, end);
        if (end[1] - p->c.a.pos[1] < 20.0f) {
            if (anim == 0x701) {
                func_002DDE20(p->c.motion, 0x703, -1);
            } else {
                func_002DDD20(p->c.motion, 0x703, -1);
            }
            VCALL(p, 0x9C, void (*)(Pursuer *, s32, f32 *))(p, 3, ofs);
            p->c.a.navTri = func_00123710(p, p->c.unk100, 0, ofs, end);
            Actor_SetState(&p->c.a, &D_003ECFD0);
            return;
        }
        if (anim != 0x700 && PU(p, 0x16C8, u8) != 4 && turn) {
            p->c.unk104[0] = 0;
            PU(p, 0x1624, s32) = PU(p, 0x15A4, s32);
            func_002DDE20(p->c.motion, anim == 0x701 ? 0x706 : 0x705, -1);
            return;
        }
    } else {
        VCALL(gNavMesh, 0x5C, void (*)(void *, s32, s32, f32 *))(gNavMesh, p->c.unk100, 1, end);
        if (p->c.a.pos[1] - end[1] < 4.0f) {
            if (anim == 0x706) {
                func_002DDE20(p->c.motion, 0x707, -1);
            } else {
                func_002DDD20(p->c.motion, 0x707, -1);
            }
            Actor_SetState(&p->c.a, &D_003ECFE0);
            return;
        }
        if (anim == 0x704 || PU(p, 0x16C8, u8) == 4) {
            VCALL(p, 0x9C, void (*)(Pursuer *, s32, f32 *))(p, 2, ofs);
            p->c.a.navTri = func_00123710(p, p->c.unk100, 1, ofs, end);
        } else if (turn) {
            p->c.unk104[0] = 1;
            PU(p, 0x1624, s32) = PU(p, 0x15A4, s32);
            func_002DDE20(p->c.motion, anim == 0x705 ? 0x702 : 0x701, -1);
            return;
        }
    }
    /* the next step, alternating legs */
    switch (anim) {
    case 0x700:
    case 0x702:
        func_002DDE20(p->c.motion, 0x701, -1);
        break;
    case 0x701:
        func_002DDE20(p->c.motion, 0x702, -1);
        break;
    case 0x705:
        func_002DDE20(p->c.motion, 0x706, -1);
        break;
    case 0x704:
    case 0x706:
        func_002DDE20(p->c.motion, 0x705, -1);
        break;
    }
}

extern const PTMF D_003ED110, D_003ED120;

/* react to the event +0x16CA once the current animation is over: 7 and 4 play a short reaction
   (0x1601, 0x1602; 4 also clears the event state and switches to searching), 1, 5 and 6 turn
   towards the spot +0x15B0 (or the room's centre) and call out */
/* 0x00289860 */
void Pursuer_ReactToEvent(Pursuer *p) {
    u8 ev;

    PU(p, 0x16EC, u8) = 0;
    ev = PU(p, 0x16CA, u8);
    PURSUER_STEP_NEXT(p) = ev != 4 && ev != 7;
    if (Pursuer_WalkOn(p)) {
        return;
    }
    PU(p, 0x1784, s32) = 0;
    switch (PU(p, 0x16CA, u8)) {
    case 7:
        Pursuer_PlayAnim(p, 0x1601);
        PU(p, 0x1710, u8) = 1;
        break;
    case 4:
        Pursuer_PlayAnim(p, 0x1602);
        PU(p, 0x16C9, u8) = 0;
        PU(p, 0x16CB, u8) = 0;
        PU(p, 0x16CA, u8) = 0;
        PU(p, 0x16CC, u8) = 0;
        PU(p, 0x179C, s32) = -1;
        PU(p, 0x16F1, u8) = 0;
        PU(p, 0x16F3, u8) = 0;
        PU(p, 0x16F2, u8) = 0;
        PU(p, 0x16F4, u8) = 0;
        PU(p, 0x16CA, u8) = 3;
        PU(p, 0x16C9, u8) = 2;
        break;
    case 1:
    case 5:
    case 6: {
        s32 room;

        Pursuer_PlayAnim(p, 0);
        PU(p, 0x1710, u8) = 5;
        room = p->c.a.room;
        if (PU(p, 0x1594, s32) == room) {
            sceVu0CopyVector((f32 *)((u8 *)p + 0x1700), (f32 *)((u8 *)p + 0x15B0));
        } else {
            VObject *rooms = gRooms;

            VCALL(rooms, 0x30, void (*)(VObject *, s32, f32 *))(rooms,
                VCALL(rooms, 0x3C, s32 (*)(VObject *, u32, s32))(rooms, PU(p, 0x138C, u16), room), (f32 *)((u8 *)p + 0x1700));
        }
        p->c.unk104[0] = 90;
        if (VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(gProgress, 8) == 0) {
            func_00122C20(&p->c.a, 0x21, 7, 0, 0, NULL);
        }
        Actor_SetState(&p->c.a, &D_003ED110);
        VCALL(p, 0x240, void (*)(Pursuer *))(p);
        return;
    }
    default:
        PURSUER_STEP_DONE(p) = 1;
        return;
    }
    Actor_SetState(&p->c.a, &D_003ED120);
    if (((AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0) == 1) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    } else {
        func_00125A10(&p->c);
    }
}

extern const PTMF D_003ECC80, D_003ECC90, D_003ECCA0;

/* run the state, then act on how the current action +0x175C went */
/* 0x00294240 */
void Pursuer_BehaviourRun(Pursuer *p) {
    if (ptmf_test(&p->c.a.state)) {
        ptmf_scall(p, &p->c.a.state);
    }
    switch (PU(p, 0x175C, s32)) {
    case 4:
        if (PU(p, 0x16EF, u8) == 1) {
            PU(p, 0x16C8, u8) = 2;
            VCALL(p, 0x2C0, void (*)(Pursuer *, s32))(p, 2);
            PU(p, 0x16C9, u8) = 2;
            PU(p, 0x16CA, u8) = 3;
            func_0027E790(p);
            PU(p, 0x16ED, u8) = 1;
            PU(p, 0x16EF, u8) = 0;
        } else if (PU(p, 0x1590, f32) < 10.0f && !(PU(p, 0x1590, f32) < 0.0f)) {
            PURSUER_STEP_DONE(p) = 0;
            if (PU(p, 0x16C8, u8) == 0) {
                if (AT(gCharPlayer, 0x1AD630, u8) != 0) {
                    PU(p, 0x1660, s32) = 0;
                }
            } else {
                if (PU(p, 0x16C9, u8) != 3) {
                    PU(p, 0x16C9, u8) = 2;
                }
                PU(p, 0x179C, s32) = -1;
                if (PU(p, 0x16CA, u8) != 4) {
                    PU(p, 0x16CA, u8) = 3;
                }
            }
            VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, 0x10);
            func_00283C50(p);
        }
        break;
    case 40:
    case 41: {
        u8 k;

        if (PURSUER_STEP_DONE(p) != 1) {
            break;
        }
        PURSUER_STEP_DONE(p) = 0;
        k = PU(p, 0x16C9, u8);
        if (k == 4 || k == 1 || k == 5) {
            s32 tri = PU(p, 0x179C, s32);

            if (tri != -1) {
                PU(p, 0x15A4, s32) = tri;
                VCALL(gNavMesh, 0xC, void (*)(void *, s32, f32 *))(gNavMesh, PU(p, 0x15A4, s32), (f32 *)((u8 *)p + 0x15B0));
            }
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 4);
        } else {
            VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, PU(p, 0x175C, s32) == 0x28 ? 0xE : 0xF);
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 4);
            func_00283C50(p);
        }
        return;
    }
    case 12:
        if (PURSUER_STEP_DONE(p) == 1) {
            PURSUER_STEP_DONE(p) = 0;
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 4);
        }
        break;
    case 28:
        if (PU(p, 0x1544, u8) != 0) {
            PURSUER_STEP_NEXT(p) = 1;
        }
        /* fall through */
    case 11:
    case 13:
    case 18:
        if (PU(p, 0x16EF, u8) == 1) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 4);
            PU(p, 0x16EF, u8) = 0;
        } else if (PURSUER_STEP_DONE(p) == 1) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 4);
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 2:
    case 9:
    case 23:
        if (PURSUER_STEP_DONE(p) == 1) {
            if (PU(p, 0x16C8, u8) != 0) {
                PU(p, 0x16ED, u8) = 1;
            } else {
                VCALL(p, 0xB4, void (*)(Pursuer *, s32))(p, 0);
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 4);
            }
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 16:
        if (PURSUER_STEP_DONE(p) == 1) {
            PURSUER_STEP_NEXT(p) = 1;
            PU(p, 0x1544, u8) = 1;
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    }
    if (PURSUER_STEP_NEXT(p) != 1) {
        return;
    }
    if (PU(p, 0x1544, u8) == 1) {
        /* Fiona in sight */
        ptmf_set((PTMF *)((u8 *)p + 0x174C), !(func_00217260(p) & 0xFF) ? &D_003ECC80 : &D_003ECC90);
        PU(p, 0x1758, s32) = -1;
    } else if (PU(p, 0x16ED, u8) == 1) {
        VCALL(p, 0x13C, void (*)(Pursuer *))(p);
    } else if (PU(p, 0x16F3, u8) == 1) {
        if (func_00217260(p) & 0xFF) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x1C);
        } else {
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECCA0);
            PU(p, 0x1758, s32) = -1;
            VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1C);
        }
        PU(p, 0x16F3, u8) = 0;
        return;
    }
    if (PU(p, 0x16F3, u8) == 1) {
        VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1C);
        PU(p, 0x16F3, u8) = 0;
    }
}

extern const PTMF D_003ECE60, D_003ECE70;

/* forget the door the pursuer was about to open */
static void Pursuer_DropDoor(Pursuer *p) {
    p->c.unk100 = -1;
    p->c.unk104[0] = -1;
    PU(p, 0x1568, f32) = 0.0f;
    PU(p, 0x16EF, u8) = 1;
}

/* turn to the heading +0x1568, then open the door +0x100 (+0x104 its kind, 0..3) unless it's
   locked against this pursuer */
/* 0x0028F080 */
void Pursuer_DoorOpen(Pursuer *p) {
    Progress *pr;
    s32 kind;

    if (PU(p, 0x1788, s32) == 0x400) {
        f32 d, s;

        if (!(func_002E2D00(PU(p, 0x1568, f32) - p->c.a.angle[1]) <= 0.0f)) {
            d = func_002E2D00(PU(p, 0x1568, f32) - p->c.a.angle[1]);
        } else {
            d = -func_002E2D00(PU(p, 0x1568, f32) - p->c.a.angle[1]);
        }
        if (!(VCALL(p, 0xA0, f32 (*)(Pursuer *))(p) <= 0.0f)) {
            s = VCALL(p, 0xA0, f32 (*)(Pursuer *))(p);
        } else {
            s = -VCALL(p, 0xA0, f32 (*)(Pursuer *))(p);
        }
        if (!(d < s)) {
            if ((AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0) {
                Actor_SetState(&p->c.a, &D_003ECE60);
                VCALL(p, 0x1B4, void (*)(Pursuer *))(p);
            } else {
                func_00125900(&p->c);
            }
            return;
        }
        /* close enough: face it exactly */
        {
            f32 h = PU(p, 0x1568, f32);

            p->c.a.angle[1] = h;
            sceVu0UnitMatrix((void *)((u8 *)p + 0x60));
            sceVu0RotMatrixY((void *)((u8 *)p + 0x60), (void *)((u8 *)p + 0x60), h);
        }
        PU(p, 0x1574, f32) = func_002E2D00(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
        if (((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF) {
            return;
        }
        Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
        func_00297C60(p);
    } else if (func_002140A0(p, PU(p, 0x1568, f32), VCALL(p, 0xA0, f32 (*)(Pursuer *))(p)) != 0.0f) {
        return;
    }
    if (((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF) {
        return;
    }
    kind = p->c.unk104[0];
    pr = gProgress;
    switch (kind) {
    case 0:
    case 2:
        if (func_00178980(pr, p->c.a.room, (u8)p->c.unk100) != 0) {
            Pursuer_DropDoor(p);
            return;
        }
        break;
    case 1:
    case 3:
        if (!(func_00178980(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF)) {
            Pursuer_DropDoor(p);
            return;
        }
        break;
    default:
        Pursuer_DropDoor(p);
        return;
    }
    if ((DoorHold_Take(pr, p->c.a.room, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) & 0xFF) == 1) {
        Pursuer_DropDoor(p);
        return;
    }
    AT(p, 0x2B, u8) = 1;
    PURSUER_STEP_NEXT(p) = 0;
    PU(p, 0x15C0, u8) = 0xFF;
    p->c.moveMode = 2;
    if ((func_00178980(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1) {
        p->c.moveSub = 0x15;
        func_00212DC0(p, (u8)p->c.unk100);
    } else {
        p->c.moveSub = 0x14;
        func_00212DE0(p, (u8)p->c.unk100);
        if (DoorHold_Usable(pr, p->c.a.room, (u8)p->c.unk100) != 0) {
            Progress *g = gProgress;

            DoorHold_Release(g, p->c.a.room, (u8)p->c.unk100);
            DoorHold_Take(g, p->c.a.room, (u8)p->c.unk100, *(u8 *)&p->c.a.slot);
        }
    }
    switch (p->c.unk104[0]) {
    case 0:
        func_002DDD20(p->c.motion, 0x600, -1);
        break;
    case 2:
        func_002DDD20(p->c.motion, 0x602, -1);
        break;
    case 1:
        func_002DDD20(p->c.motion, 0x601, -1);
        break;
    case 3:
        func_002DDD20(p->c.motion, 0x603, -1);
        break;
    }
    PU(p, 0x1568, f32) = 0.0f;
    PU(p, 0x1624, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECE70);
}

/* start an animation unless it's already playing; 1 if it was (re)started */
static s32 Pursuer_StartAnim(Pursuer *p, s32 anim) {
    u8 *m = p->c.motion;

    if (anim == AT(m, 0x55C, s32)) {
        s32 over = AT(m, 0x550, f32) <= 0.0f;

        if ((over ^ 1) & 0xFF) {
            return 0;
        }
        if (((AT(AT(m, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0) != 1) {
            s32 i = func_001F4710(m, anim);
            u16 fl = i != -1 ? AT(AT(m, 0x874, u8 *) + i * 6, 0x4, u16) : 0;

            if (fl & 4) {
                return 0;
            }
        }
        func_002DDED0(p->c.motion, anim, -1);
    } else {
        func_002DDED0(m, anim, -1);
    }
    return 1;
}

/* turn on the spot in direction dir (0..3); 0x405.. instead of 0x400.. in mode +0xC4 1 */
s32 func_00297300(Pursuer *p, u32 dir) {
    s32 base = p->c.a.unkC4 == 1 ? 0x405 : 0x400;

    switch (dir & 0xFF) {
    case 0:
        return Pursuer_StartAnim(p, base);
    case 1:
        return Pursuer_StartAnim(p, base + 1);
    case 2:
        return Pursuer_StartAnim(p, base + 2);
    case 3:
        return Pursuer_StartAnim(p, base + 3);
    }
    return 0;
}

/* the motion group +0x1788 of the current animation (0x400 turning, 0x600 doors, 0x700 stairs,
   0x1700 knocked down, ...); anything unknown goes back to the idle */
void func_00297C60(Pursuer *p) {
    s32 anim = MOTION_ANIM(p);
    s32 g;

    switch (anim) {
    case 0 ... 3:
        g = 0;
        break;
    case 0x200:
    case 0x202:
    case 0x204:
        g = 0x200;
        break;
    case 0x201:
    case 0x203:
    case 0x205:
    case 0x206:
        g = 0x201;
        break;
    case 0x400 ... 0x403:
    case 0x405 ... 0x408:
        g = 0x400;
        break;
    case 0x404:
        g = 0x401;
        break;
    case 0x600 ... 0x603:
        g = 0x600;
        break;
    case 0x700 ... 0x707:
        g = 0x700;
        break;
    case 0xE00 ... 0xE08:
    case 0x2300 ... 0x2304:
        g = 0xE00;
        break;
    case 0x1000 ... 0x1006:
        g = 0x1000;
        break;
    case 0x1300 ... 0x1307:
        g = 0x1300;
        break;
    case 0x1600 ... 0x1602:
        g = 0x1600;
        break;
    case 0x1700 ... 0x1709:
        g = 0x1700;
        break;
    case 0x1800 ... 0x1807:
        g = 0x1800;
        break;
    case 0x1900 ... 0x1905:
    case 0x1A00 ... 0x1A01:
        g = 0x1900;
        break;
    default:
        if ((anim & ~0xFF) == 0x8000 || (anim & ~0xFF) == 0x9000) {
            g = 0x8000;
            break;
        }
        if (anim == -1) {
            return;
        }
        Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
        g = 0;
        break;
    }
    PU(p, 0x1788, s32) = g;
}

/* outside the idle/walk groups, carry a lying-down or getting-up animation on to its next part,
   or go back to the idle */
void func_0027F5F0(Pursuer *p) {
    s32 g = PU(p, 0x1788, s32);

    if (g == 0x201 || g == 0x200 || g == 0) {
        return;
    }
    switch (MOTION_ANIM(p)) {
    case 0x1801:
        Pursuer_PlayAnimBlend(p, 0x1802);
        break;
    case 0x1805:
    case 0x1709:
        Pursuer_PlayAnimBlend(p, 0x1806);
        break;
    case 0x1706:
        Pursuer_PlayAnimBlend(p, 0x1707);
        break;
    case 0x1703:
        Pursuer_PlayAnimBlend(p, 0x1704);
        break;
    case 0x1700:
        Pursuer_PlayAnimBlend(p, 0x1701);
        break;
    case 0x404:
    case 0x1800:
    case 0x1802:
    case 0x1803:
    case 0x1804:
    case 0x1806:
    case 0x1807:
        break;
    default:
        Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
        break;
    }
}

extern const PTMF D_003ED150;

/* state: hit, flinching from direction +0x104 (0..3; bit 1 when the hit only staggers it) */
void func_00288970(Pursuer *p) {
    s32 anim, sub;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if ((PursuerGroup_Find(gProgress, 0x20, p->c.a.slot) & 0xFF) != 0xFF) {
        if (p->c.unk104[0] & 2) {
            anim = 0x1005;
            sub = 0xD;
        } else {
            anim = 0x1002;
            sub = 0xC;
        }
    } else {
        switch (p->c.unk104[0]) {
        case 0:
            anim = 0x1001;
            sub = 0xE;
            break;
        case 1:
            anim = 0x1000;
            sub = 0xE;
            break;
        case 2:
            anim = 0x1004;
            sub = 0xF;
            break;
        case 3:
            anim = 0x1003;
            sub = 0xF;
            break;
        default:
            anim = 0x1002;
            sub = 0xC;
            break;
        }
    }
    Pursuer_PlayAnim(p, anim);
    p->c.moveSub = sub;
    VCALL(p, 0x104, void (*)(Pursuer *))(p);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED150);
    if (((AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0) == 1) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    } else {
        func_00125A10(&p->c);
    }
}

/* Fiona seen again: back to the chase (mode 0) if she's in reach, else follow (mode 1) */
static void Pursuer_Resight(Pursuer *p) {
    if (func_00217260(p) != 0 && func_00217340(p) == func_002172F0(p, gCharPlayer)) {
        PU(p, 0x16C8, u8) = 0;
        VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
        PU(p, 0x16C9, u8) = 6;
    } else {
        PU(p, 0x16C8, u8) = 1;
    }
    Pursuer_ClearRoute(p);
    PU(p, 0x16F2, u8) = 0;
    PU(p, 0x16F4, u8) = 0;
}

/* the mode machine +0x16C8: 0 chasing Fiona, 1 following her trail, 2 heading for a noise or
   sighting, 3 searching, 4 resting; +0x1544 sight, +0x16F2 a new lead, +0x16F4 lead lost */
/* 0x0027A6A0 */
void Pursuer_Modes(Pursuer *p) {
    if (p->c.a.unkC4 == 2) {
        if (PU(p, 0x16C8, u8) != 3) {
            PU(p, 0x16C8, u8) = 3;
            VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
            PU(p, 0x16C9, u8) = 0;
            PU(p, 0x16CB, u8) = 0;
            PU(p, 0x16CA, u8) = 0;
            PU(p, 0x16CC, u8) = 0;
            PU(p, 0x179C, s32) = -1;
            PU(p, 0x16F1, u8) = 0;
            PU(p, 0x16F3, u8) = 0;
            PU(p, 0x16F2, u8) = 0;
            PU(p, 0x16F4, u8) = 0;
            PU(p, 0x16F2, u8) = 0;
            PU(p, 0x16F4, u8) = 0;
        }
        return;
    }
    switch (PU(p, 0x16C8, u8)) {
    case 0:
        if (PU(p, 0x1544, u8) != 0) {
            PU(p, 0x16C9, u8) = 6;
            PU(p, 0x16CA, u8) = 7;
            VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
            PU(p, 0x16F4, u8) = 0;
            return;
        }
        PU(p, 0x16C9, u8) = 5;
        if (p->c.moveMode == 2 || p->c.moveMode == 3) {
            return;
        }
        if (PU(p, 0x1660, s32) != 0) {
            if (PU(p, 0x16F4, u8) != 0) {
                PU(p, 0x16C8, u8) = 3;
                VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
                PU(p, 0x16C9, u8) = 2;
                PU(p, 0x16CA, u8) = 3;
                PU(p, 0x16F4, u8) = 0;
            } else {
                PU(p, 0x1660, s32)--;
            }
        } else if (p->c.a.room == gCharPlayer->a.room) {
            PU(p, 0x16C8, u8) = 2;
            VCALL(p, 0x2C0, void (*)(Pursuer *))(p);
            func_0027EAF0(p);
            PU(p, 0x16C9, u8) = 3;
            PU(p, 0x16CA, u8) = 4;
        } else {
            PU(p, 0x16C8, u8) = 1;
            PU(p, 0x16C9, u8) = 2;
            PU(p, 0x16CA, u8) = 3;
        }
        break;
    case 1:
        if (PU(p, 0x1544, u8) == 1) {
            if (func_00217260(p) != 0 && func_00217340(p) == func_002172F0(p, gCharPlayer)) {
                PU(p, 0x16C8, u8) = 0;
                VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
                PU(p, 0x16C9, u8) = 6;
            }
            PU(p, 0x16F2, u8) = 0;
        } else if (func_00217260(p) != 0) {
            PU(p, 0x16C8, u8) = 2;
        } else if (PU(p, 0x16F4, u8) != 0) {
            PU(p, 0x16C8, u8) = 3;
            VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
            PU(p, 0x16F4, u8) = 0;
        }
        if (PU(p, 0x16F2, u8) != 0) {
            func_0027E790(p);
            PU(p, 0x16F2, u8) = 0;
        }
        break;
    case 2:
        if (PU(p, 0x1544, u8) == 1) {
            Pursuer_Resight(p);
        } else if (PU(p, 0x16F2, u8) != 0) {
            if (!(func_00217260(p) & 0xFF)) {
                PU(p, 0x16C8, u8) = 1;
                func_0027E790(p);
            } else if (!(func_00217510(p) & 0xFF) || PU(p, 0x1620, u8) != 0) {
                func_0027E790(p);
            }
            PU(p, 0x16F2, u8) = 0;
            PU(p, 0x16F4, u8) = 0;
        } else if (PU(p, 0x16F4, u8) != 0 || (PU(p, 0x17B4, s32) == 0 && !(func_00217510(p) & 0xFF))) {
            PU(p, 0x16C8, u8) = 3;
            VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
            PU(p, 0x16F4, u8) = 0;
        }
        break;
    case 3:
        if (PU(p, 0x1544, u8) == 1) {
            Pursuer_Resight(p);
        } else if (PU(p, 0x16F2, u8) != 0) {
            PU(p, 0x16C8, u8) = func_00217260(p) != 0 ? 2 : 1;
            func_0027E790(p);
            PU(p, 0x16F2, u8) = 0;
            PU(p, 0x16F4, u8) = 0;
        } else if (PU(p, 0x16F4, u8) != 0) {
            PU(p, 0x16F4, u8) = 0;
        } else if (!(func_00217510(p) & 0xFF)) {
            if (PU(p, 0x1660, s32) != 0) {
                PU(p, 0x1660, s32)--;
            } else if (!(func_00217460(p, 1) & 0xFF)) {
                /* nothing left here: target Hewie instead (vtable +0xB4) */
                VCALL(p, 0xB4, void (*)(Pursuer *, Character *))(p, gCharPartner);
                PU(p, 0x16C8, u8) = (func_00217260(p) & 0xFF) ? 2 : 1;
                func_0027E790(p);
            }
        }
        break;
    case 4:
        if (PU(p, 0x1660, s32) == 0) {
            u32 i;

            PU(p, 0x16C8, u8) = 0;
            VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
            for (i = 0; i < 13; i++) {
                p->c.unk148C[i] = 0;
            }
            Pursuer_ClearRoute(p);
            PU(p, 0x1594, s32) = gCharPlayer->a.room;
            p->c.unk1384 = p->c.unk1388;
            PU(p, 0x16C9, u8) = 6;
            PU(p, 0x16F6, u8) = 1;
            PU(p, 0x16F2, u8) = 0;
            PU(p, 0x16F4, u8) = 0;
        }
        break;
    }
}

extern const PTMF D_003ED010;

/* state: an attack step in progress (entry e of func_0028B970). At the animation's hit key the
   strike lands where the entry's bone (+0x4) is, unless a wall is in the way; once over, either
   the next step of the attack or a rest of +0x1748's frames before the next one */
void func_0028B340(Pursuer *p) {
    u8 *e = PU(p, 0x171C, u8 *) + PU(p, 0x1724, s8 *)[PU(p, 0x1728, s32) * 4 + PU(p, 0x172C, s8)] * 0x24;
    Progress *pr;

    if (((AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0) == 1) {
        u32 *rest = PU(p, 0x1748, u32 *);

        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
        if (PU(p, 0x1760, u8) != 0) {
            /* it hit */
            PU(p, 0x178C, u32) = rest != NULL ? rest[0] : 90;
        } else if (rest != NULL) {
            PU(p, 0x178C, u32) = PU(p, 0x178C, u32) < rest[1] ? rest[1] : PU(p, 0x178C, u32);
        } else {
            PU(p, 0x178C, u32) = PU(p, 0x178C, u32) < 30 ? 30 : PU(p, 0x178C, u32);
        }
        p->c.unk104[0] = -1;
        PU(p, 0x172C, s8) = 0;
        PU(p, 0x1728, s32) = -1;
        PU(p, 0x1770, s32) = 0;
        PU(p, 0x1774, s32) = 0;
        PU(p, 0x1778, s32) = 0;
        return;
    }
    if (AT(e, 0x1C, u8) != 0) {
        /* keep turning to the target */
        f32 h = func_001244D0(&p->c.a, p->target->a.pos);

        func_002140A0(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    }
    if (p->c.unk14D0 <= 0) {
        func_00125A10(&p->c);
    }
    if (func_001F4770(p->c.motion, 0, -1, 1) & 0xFF & 2) {
        pr = gProgress;
        if (!(Progress_TestFlag(pr, 0xE) & 0xFF)) {
            u32 hit;
            f32 other[4] __attribute__((aligned(16))) = { 0.0f, 0.0f, 0.0f, 0.0f };

            if (!(((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF)) {
                f32 bone[4] __attribute__((aligned(16)));

                sceVu0CopyVector(bone, Skel_Bone(MOTION_AT(p, 0x810, u8 *), AT(e, 0x4, s32)) + 0xC);
                if (PU(p, 0x1788, s32) != 0 && func_00217110(p, bone) != 0) {
                    /* the blow is blocked: back to the idle */
                    Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
                    PU(p, 0x16F7, u8) = 0;
                    if (VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(pr, 8) == 0) {
                        func_00122C20(&p->c.a, 0x2B, 7, 0, 0, NULL);
                    }
                    return;
                }
            }
            if (p->c.unk104[0] != 1) {
                if (p->c.unk104[0] == 2) {
                    PU(p, 0x1760, u8) = 0;
                }
                if (VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(pr, 8) == 0) {
                    func_00122C20(&p->c.a, 0x10, 7, 0, 0, NULL);
                }
                p->c.unk104[0] = 1;
            }
            VCALL(p, 0x138, void (*)(Pursuer *, u8 *, f32 *, f32 *))(p, e, (f32 *)((u8 *)p + 0x1770), other);
            hit = ~PU(p, 0x1760, u8) & (func_002179F0(p, (s32)((u8 *)p + 0x1770), AT(e, 0xC, f32)) & 0xFF) & 0xFF;
            if (AT(e, 0x8, s32) >= 0) {
                hit = (hit | (~PU(p, 0x1760, u8) & (func_002179F0(p, (s32)other, AT(e, 0xC, f32)) & 0xFF) & 0xFF)) & 0xFF;
            }
            if (hit & 0xFF) {
                s16 crit = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) <= AT(e, 0x18, f32) ? 0x8000 : 0;

                Relation_Request(pr, *(u8 *)&p->c.a.slot, hit, AT(e, 0x10, u8), AT(e, 0x12, u16), crit, AT(e, 0x14, f32));
                PU(p, 0x1764, s32) = AT(e, 0x20, s32);
            }
            return;
        }
    }
    if (p->c.unk104[0] != 1) {
        return;
    }
    if (p->target == gCharPlayer && !(PU(p, 0x1760, u8) & 1)) {
        VCALL(p, 0x12C, void (*)(Pursuer *, f32))(p, AT(e, 0x14, f32) / 2.0f);
    }
    if (PU(p, 0x172C, s8) < 4 && PU(p, 0x1724, s8 *)[PU(p, 0x1728, s32) * 4 + PU(p, 0x172C, s8) + 1] != -1 &&
        func_00124490(&p->c.a, p->target->a.pos) < 50.0f) {
        /* still in reach: chain the next step */
        PU(p, 0x172C, s8)++;
        PU(p, 0x1770, s32) = 0;
        PU(p, 0x1774, s32) = 0;
        PU(p, 0x1778, s32) = 0;
        Actor_SetState(&p->c.a, &D_003ED010);
        return;
    }
    p->c.unk104[0] = 2;
    PU(p, 0x16F7, u8) = 0;
}

/* the nav triangle record (0x50 bytes) of tri, if valid */
static u8 *Pursuer_NavTri(u32 tri) {
    if (tri < AT(gNavMesh, 0x8, u32) && AT(gNavMesh, 0x4, u8 *) != NULL) {
        return AT(gNavMesh, 0x4, u8 *) + tri * 0x50;
    }
    return NULL;
}

/* footsteps: when a foot comes down, play the step for the floor under it (the triangle's
   material flags +0x3C; stairs have their own), as loud as the pursuer is fast, and leave a
   noise for the others to hear (0x1C slow, 0x1F fast) */
/* 0x0029DA80 */
void Pursuer_Footsteps(Pursuer *p) {
    f32 left[4] __attribute__((aligned(16)));
    f32 right[4] __attribute__((aligned(16)));
    u32 l, r;
    s32 step;
    s32 g;

    if (AT(p, 0x29, u8) == 1 || p->c.a.navTri == (u32)-1) {
        return;
    }
    step = 0;
    l = func_002DD420(p->c.motion, left, 1, -1.0f, 1.0f) & 0xFF;
    r = func_002DD420(p->c.motion, right, 0, -1.0f, 1.0f) & 0xFF;
    g = PU(p, 0x1788, s32);
    if (g == 0x1600 || g == 0) {
        if ((((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF) == 1) {
            l = func_002DD860(p->c.motion, 1, -1.0f) & 0xFF;
            r = func_002DD860(p->c.motion, 0, -1.0f) & 0xFF;
        } else {
            l = 1;
            r = 1;
        }
    }
    if ((l & 0xFF) == 1 && PU(p, 0x16A8, u8) == 0) {
        step = 1;
    } else if ((r & 0xFF) == 1 && PU(p, 0x16A9, u8) == 0) {
        step = -1;
    }
    PU(p, 0x16A8, u8) = l;
    PU(p, 0x16A9, u8) = r;
    if (step == 0) {
        if (!(((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF)) {
            g = PU(p, 0x1788, s32);
            if (g == 0x1600 || g == 0) {
                PU(p, 0x16A4, s32) = 0;
            }
        }
        return;
    }
    {
        f32 m[4][4] __attribute__((aligned(16)));
        f32 foot[4] __attribute__((aligned(16)));
        f32 ofs[4] __attribute__((aligned(16)));
        f32 end[4] __attribute__((aligned(16)));
        f32 root[4] __attribute__((aligned(16)));
        u32 tri;
        u8 *t;
        s32 flags, snd, level;
        f32 speed;
        u32 vol;

        sceVu0CopyMatrix(m, (void *)((u8 *)p + 0x60));
        sceVu0CopyVector(m[3], p->c.a.pos);
        func_002E2DD0(foot, m, step == 1 ? left : right);
        tri = func_00123E20(&p->c.a, foot);
        t = Pursuer_NavTri(tri != (u32)-1 ? tri : p->c.a.navTri);
        if (p->c.a.room == 7 || p->c.a.room == 0x106) {
            func_00125E10(&p->c, foot, 1);
        }
        flags = -1;
        snd = -1;
        if (p->c.moveSub == 7) {
            /* on the stairs */
            switch (MOTION_ANIM(p)) {
            case 0x700:
                if (!(((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF)) {
                    snd = 10;
                }
                break;
            case 0x701:
            case 0x702:
            case 0x705:
            case 0x706:
                snd = 10;
                break;
            case 0x703:
            case 0x704:
                VCALL(p, 0x9C, void (*)(Pursuer *, s32, f32 *))(p, 3, ofs);
                if (func_00124320(&p->c.a, foot, func_00123710(p, p->c.unk100, 0, ofs, end), end, 0) == (u32)-1) {
                    snd = 10;
                } else if (MOTION_ANIM(p) == 0x703) {
                    tri = func_00124320(&p->c.a, foot,
                        VCALL(gNavMesh, 0x5C, s32 (*)(void *, s32, s32, f32 *))(gNavMesh, p->c.unk100, 0, end), end, 0);
                    if (tri != (u32)-1) {
                        t = Pursuer_NavTri(tri);
                        flags = t != NULL ? AT(t, 0x3C, s32) : (s32)NAV_BAD_TRI_FLAGS;
                    }
                }
                break;
            case 0x707:
                tri = func_00124320(&p->c.a, foot,
                    VCALL(gNavMesh, 0x5C, s32 (*)(void *, s32, s32, f32 *))(gNavMesh, p->c.unk100, 1, end), end, 0);
                if (tri != (u32)-1) {
                    t = Pursuer_NavTri(tri);
                    flags = t != NULL ? AT(t, 0x3C, s32) : (s32)NAV_BAD_TRI_FLAGS;
                }
                break;
            }
        }
        level = 7;
        if (snd == -1) {
            if (flags == -1) {
                flags = t != NULL ? AT(t, 0x3C, s32) : (s32)NAV_BAD_TRI_FLAGS;
            }
            if (flags & 0x2000000) {
                if (flags & 0x8000) {
                    snd = 0x14;
                    level = 6;
                } else {
                    snd = 8;
                }
            } else {
                switch (flags & 0x18000) {
                case 0x18000:
                    snd = 6;
                    break;
                case 0x10000:
                    snd = 4;
                    break;
                case 0x8000:
                    snd = 2;
                    break;
                default:
                    snd = AT(gProgress, 0x1FBEC0, u8) != 0 ? 0x2C : 0;
                    break;
                }
            }
            /* left and right feet alternate */
            snd += PU(p, 0x16A4, s32) & 1;
            PU(p, 0x16A4, s32)++;
        }
        func_001F6370(p->c.motion, root, 0.0f);
        root[2] *= VCALL((VObject *)p->c.motion, 0x44, f32 (*)(void *, Pursuer *))(p->c.motion, p);
        speed = VCALL(p, 0x2F8, f32 (*)(Pursuer *))(p);
        speed -= VCALL(p, 0x2FC, f32 (*)(Pursuer *))(p);
        speed = (root[2] - VCALL(p, 0x2FC, f32 (*)(Pursuer *))(p)) / speed;
        if (speed < 0.0f) {
            speed = 0.0f;
        }
        if (!(speed <= 1.0f)) {
            speed = 1.0f;
        }
        vol = (u32)(2.0f * speed) & 0xFF & 0x7F;
        if (VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(gProgress, 8) == 0) {
            func_00122C20(&p->c.a, snd, level, 0, (s8)vol, NULL);
        }
        func_002A8440((u8 *)gProgress + 0x798, speed < 0.5f ? 0x1C : 0x1F, p->c.a.room, p->c.a.navTri, 0xFFFF);
    }
}

/* Fiona seen again (variant): back to the chase if she's in reach, or if either of them is
   somewhere func_002172F0/func_00217340 rate 2 or more; else follow */
static s32 Pursuer_ResightTest(Pursuer *p) {
    u32 a = func_002172F0(p, gCharPlayer);
    u32 b = func_00217340(p);

    return func_00217260(p) != 0 && (a >= 2 || b >= 2 || a == b);
}

/* the mode machine of Pursuer_Modes for pursuers that search a random number of stops when they
   go somewhere, and give up on the room (vtable +0xB0) rather than turn on Hewie */
/* 0x002983C0 */
void Pursuer_ModesSearching(Pursuer *p) {
    Progress *pr = gProgress;

    if (AT(pr, 0x1FBEC1, u8) != 0) {
        VCALL(p, 0x124, void (*)(Pursuer *))(p);
        return;
    }
    if (p->c.a.unkC4 == 2) {
        if (PU(p, 0x16C8, u8) != 3) {
            PU(p, 0x16C8, u8) = 3;
            VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
            PU(p, 0x16C9, u8) = 0;
            PU(p, 0x16CB, u8) = 0;
            PU(p, 0x16CA, u8) = 0;
            PU(p, 0x16CC, u8) = 0;
            PU(p, 0x179C, s32) = -1;
            PU(p, 0x16F1, u8) = 0;
            PU(p, 0x16F3, u8) = 0;
            PU(p, 0x16F2, u8) = 0;
            PU(p, 0x16F4, u8) = 0;
            PU(p, 0x16F2, u8) = 0;
            PU(p, 0x16F4, u8) = 0;
        }
        return;
    }
    switch (PU(p, 0x16C8, u8)) {
    case 0:
        if (PU(p, 0x1544, u8) != 0) {
            PU(p, 0x16C9, u8) = 6;
            PU(p, 0x16CA, u8) = 7;
            VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
            PU(p, 0x16F4, u8) = 0;
            return;
        }
        PU(p, 0x16C9, u8) = 5;
        if (p->c.moveMode == 2 || p->c.moveMode == 3) {
            return;
        }
        if (PU(p, 0x1660, s32) != 0) {
            if (PU(p, 0x16F4, u8) != 0) {
                PU(p, 0x16C8, u8) = 3;
                VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
                PU(p, 0x16C9, u8) = 2;
                PU(p, 0x16CA, u8) = 3;
                PU(p, 0x16F4, u8) = 0;
            } else {
                PU(p, 0x1660, s32)--;
            }
        } else if (func_00217510(p) == 0) {
            PU(p, 0x16C8, u8) = 1;
            PU(p, 0x16C9, u8) = 2;
            PU(p, 0x16CA, u8) = 3;
        } else {
            u32 n;
            s32 have;

            PU(p, 0x16C8, u8) = 2;
            VCALL(p, 0x2C0, void (*)(Pursuer *))(p);
            /* how many search stops to have left: 2..5 heading for something, 1..2 searching */
            switch (PU(p, 0x16C8, u8)) {
            case 2:
                n = (((u32)(4.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom)) & 0xFF) + 2) & 0xFF;
                break;
            case 3:
                n = (((u32)(2.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom)) & 0xFF) + 1) & 0xFF;
                break;
            default:
                n = 0xFF;
                break;
            }
            have = PU(p, 0x1621, u8) - PU(p, 0x1620, u8);
            if (have < (s32)n) {
                func_0027E5D0(p, (n - have) & 0xFF);
            } else if ((s32)n < have && n != have) {
                do {
                    PU(p, 0x1620, u8)++;
                } while (n != PU(p, 0x1621, u8) - PU(p, 0x1620, u8));
            }
            PU(p, 0x1794, s32) = (s32)n > 0 && n != 0xFF ? 1800 : 0;
            PU(p, 0x16C9, u8) = 3;
            PU(p, 0x16CA, u8) = 4;
        }
        break;
    case 1:
        if (PU(p, 0x1544, u8) == 1) {
            if (Pursuer_ResightTest(p)) {
                PU(p, 0x16C8, u8) = 0;
                VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
                PU(p, 0x16C9, u8) = 6;
            }
            PU(p, 0x16F2, u8) = 0;
        } else if (func_00217260(p) != 0) {
            PU(p, 0x16C8, u8) = 2;
        } else if (PU(p, 0x16F4, u8) != 0) {
            PU(p, 0x16C8, u8) = 3;
            VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
            PU(p, 0x16F4, u8) = 0;
        } else if (func_00217510(p) == 0 && Countdown_Seconds((u8 *)pr + 0x764) == 0) {
            PU(p, 0x16C8, u8) = 3;
            VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
        }
        if (PU(p, 0x16F2, u8) != 0) {
            func_0027E790(p);
            PU(p, 0x16F2, u8) = 0;
        }
        break;
    case 2:
    case 3:
        if (PU(p, 0x1544, u8) == 1) {
            if (Pursuer_ResightTest(p)) {
                PU(p, 0x16C8, u8) = 0;
                VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
                PU(p, 0x16C9, u8) = 6;
            } else {
                PU(p, 0x16C8, u8) = 1;
            }
            Pursuer_ClearRoute(p);
            PU(p, 0x16F2, u8) = 0;
            PU(p, 0x16F4, u8) = 0;
        } else if (PU(p, 0x16C8, u8) == 2) {
            if (PU(p, 0x16F2, u8) != 0) {
                if (!(func_00217260(p) & 0xFF)) {
                    PU(p, 0x16C8, u8) = 1;
                    func_0027E790(p);
                } else if (!(func_00217510(p) & 0xFF) || PU(p, 0x1620, u8) != 0) {
                    func_0027E790(p);
                }
                PU(p, 0x16F2, u8) = 0;
                PU(p, 0x16F4, u8) = 0;
            } else if (PU(p, 0x16F4, u8) != 0 || (PU(p, 0x17B4, s32) == 0 && !(func_00217510(p) & 0xFF))) {
                PU(p, 0x16C8, u8) = 3;
                VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
                PU(p, 0x16F4, u8) = 0;
            }
        } else if (PU(p, 0x16F2, u8) != 0) {
            PU(p, 0x16C8, u8) = func_00217260(p) != 0 ? 2 : 1;
            func_0027E790(p);
            PU(p, 0x16F2, u8) = 0;
            PU(p, 0x16F4, u8) = 0;
        } else if (PU(p, 0x16F4, u8) != 0) {
            PU(p, 0x16F4, u8) = 0;
        } else if (!(func_00217510(p) & 0xFF)) {
            if (PU(p, 0x1660, s32) != 0) {
                PU(p, 0x1660, s32)--;
            } else if (!(func_00217460(p, 0) & 0xFF)) {
                VCALL(p, 0xB0, void (*)(Pursuer *))(p);
                PU(p, 0x16C8, u8) = (func_00217260(p) & 0xFF) ? 2 : 1;
                func_0027E790(p);
            }
        }
        break;
    case 4:
        if (PU(p, 0x1660, s32) == 0) {
            u32 i;

            PU(p, 0x16C8, u8) = 0;
            VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
            for (i = 0; i < 13; i++) {
                p->c.unk148C[i] = 0;
            }
            Pursuer_ClearRoute(p);
            PU(p, 0x1594, s32) = gCharPlayer->a.room;
            p->c.unk1384 = p->c.unk1388;
            PU(p, 0x16C9, u8) = 6;
            PU(p, 0x16F6, u8) = 1;
            PU(p, 0x16F2, u8) = 0;
            PU(p, 0x16F4, u8) = 0;
        }
        break;
    }
}

extern const PTMF D_003ECA90, D_003ECAA0, D_003ECAB0, D_003ECAC0, D_003ECAD0, D_003ECAE0, D_003ECAF0,
    D_003ECB00, D_003ECB10, D_003ECB20, D_003ECB30, D_003ECB40, D_003ECB50;

/* walk (0x200) or run (0x201) to wherever the script asked */
static void Pursuer_ScriptMove(Pursuer *p, s32 group, const PTMF *step) {
    if ((p->c.unk104[1] & 0xFF00) != group) {
        p->c.unk104[1] = group == 0x200 ? VCALL(p, 0x324, s32 (*)(Pursuer *))(p) : VCALL(p, 0x328, s32 (*)(Pursuer *))(p);
    }
    PU(p, 0x1788, s32) = group;
    ptmf_set((PTMF *)((u8 *)p + 0x174C), step);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 4);
}

/* commands from events: a pending nav-mask change (+0x14E8 4), then the script command +0xF4 with
   its arguments +0x100.. (go to a triangle or a point, play animations, look at someone) */
/* 0x00299530 */
void Pursuer_EventCommand(Pursuer *p) {
    PTMF *step = (PTMF *)((u8 *)p + 0x174C);

    if (p->c.state[0] == 4) {
        if (p->c.state[1] != 6 && p->c.moveMode != 3) {
            u32 old = p->c.a.navMask;
            u8 *t;

            p->c.a.navMask = VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
            t = Pursuer_NavTri(p->c.a.navTri);
            if (!(p->c.a.navMask & (t != NULL ? AT(t, 0x3C, u32) : 0))) {
                /* the floor it stands on is fine with the new mask */
                func_0029B8B0(p);
                VCALL(p, 0x90, void (*)(Pursuer *))(p);
                return;
            }
            p->c.a.navMask = old;
        }
        p->c.state[0] = 0;
        p->c.state[1] = 0;
    }
    switch (p->c.unkF4) {
    case 1:
        if (p->c.moveSub == 7) {
            Character_EventReset(&p->c);
            func_0027FE90(p);
            VCALL(p, 0x13C, void (*)(Pursuer *))(p);
            PU(p, 0x1758, s32) = -2;
        } else {
            VCALL(p, 0x90, void (*)(Pursuer *))(p);
            func_0027FE90(p);
            VCALL(p, 0x13C, void (*)(Pursuer *))(p);
        }
        break;
    case 2:
        ptmf_set(step, &D_003ECA90);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
        break;
    case 3:
        ptmf_set(step, &D_003ECAA0);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0xA);
        break;
    case 4:
        ptmf_set(step, &D_003ECAB0);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0xA);
        break;
    case 5:
    case 10:
        VCALL(p, 0xAC, void (*)(Pursuer *, s32, f32 *, s32))(p, p->c.unk104[0], p->c.unk110, -1);
        if (p->c.unkF4 == 5) {
            Pursuer_ScriptMove(p, 0x200, &D_003ECAC0);
        } else {
            Pursuer_ScriptMove(p, 0x201, &D_003ECAD0);
        }
        break;
    case 6:
    case 11:
        PU(p, 0x15A4, s32) = p->c.unk104[0];
        VCALL(gNavMesh, 0xC, void (*)(void *, s32, f32 *))(gNavMesh, PU(p, 0x15A4, s32), (f32 *)((u8 *)p + 0x15B0));
        if (p->c.unkF4 == 6) {
            Pursuer_ScriptMove(p, 0x200, &D_003ECAE0);
        } else {
            Pursuer_ScriptMove(p, 0x201, &D_003ECAF0);
        }
        break;
    case 7:
        func_002DDED0(p->c.motion, p->c.unk104[0], -1);
        p->c.unkE1 = 1;
        ptmf_set(step, &D_003ECB00);
        PU(p, 0x1758, s32) = -1;
        break;
    case 8:
        func_002DDC60(p->c.motion, p->c.unk104[0], p->c.unk104[1], -1);
        p->c.unkE1 = 1;
        ptmf_set(step, &D_003ECB10);
        PU(p, 0x1758, s32) = -1;
        break;
    case 9:
        func_002DDBA0(p->c.motion, p->c.unk104[0], p->c.unk104[1]);
        p->c.unkE1 = 1;
        ptmf_set(step, &D_003ECB20);
        PU(p, 0x1758, s32) = -1;
        break;
    case 16:
        func_002DDC60(p->c.motion, 0, p->c.unk104[1], -1);
        p->c.unkE1 = 1;
        ptmf_set(step, &D_003ECB30);
        PU(p, 0x1758, s32) = -1;
        break;
    case 12:
        if (p->c.unk100 == 0xFF) {
            PU(p, 0x16F9, u8) = 0;
        } else if (gCharacters[p->c.unk100] != NULL) {
            PU(p, 0x16F9, u8) = 1;
        }
        p->c.unkE1 = 1;
        break;
    case 13:
        sceVu0CopyVector((f32 *)((u8 *)p + 0x1700), p->c.unk110);
        PU(p, 0x16F9, u8) = 1;
        p->c.unkE1 = 1;
        break;
    case 14:
        if (gCharacters[p->c.unk100] == NULL) {
            p->c.unkE1 = 1;
            break;
        }
        Pursuer_PlayAnim(p, 0x200);
        *(f32 *)&p->c.unk104[2] = func_001244D0(&p->c.a, gCharacters[p->c.unk100]->a.pos);
        ptmf_set(step, &D_003ECB40);
        PU(p, 0x1758, s32) = -1;
        break;
    case 15:
        Pursuer_PlayAnim(p, 0x200);
        ptmf_set(step, &D_003ECB50);
        PU(p, 0x1758, s32) = -1;
        break;
    }
    if (p->c.unkF4 != 0) {
        p->c.unkF4 = 0;
        PURSUER_STEP_DONE(p) = 0;
    }
}

/* strike with attack entry e at the animation's hit key (see func_0028B340) */
static void Pursuer_Strike(Pursuer *p, u8 *e) {
    f32 other[4] __attribute__((aligned(16))) = { 0.0f, 0.0f, 0.0f, 0.0f };
    u32 hit;

    VCALL(p, 0x138, void (*)(Pursuer *, u8 *, f32 *, f32 *))(p, e, (f32 *)((u8 *)p + 0x1770), other);
    hit = ~PU(p, 0x1760, u8) & (func_002179F0(p, (s32)((u8 *)p + 0x1770), AT(e, 0xC, f32)) & 0xFF) & 0xFF;
    if (AT(e, 0x8, s32) >= 0) {
        hit = (hit | (~PU(p, 0x1760, u8) & (func_002179F0(p, (s32)other, AT(e, 0xC, f32)) & 0xFF) & 0xFF)) & 0xFF;
    }
    if (hit & 0xFF) {
        s16 crit = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) <= AT(e, 0x18, f32) ? 0x8000 : 0;

        Relation_Request(gProgress, *(u8 *)&p->c.a.slot, hit, AT(e, 0x10, u8), AT(e, 0x12, u16), crit, AT(e, 0x14, f32));
        PU(p, 0x1764, s32) = AT(e, 0x20, s32);
    }
}

/* the attack entry (index) the pursuer throws Hewie off with */
static s32 Pursuer_ShakeOffEntry(Pursuer *p) {
    s8 k;

    if (PU(p, 0x1624, s32) == 0x1700) {
        k = VCALL(p, 0x30C, s32 (*)(Pursuer *))(p);
    } else {
        k = VCALL(p, 0x310, s32 (*)(Pursuer *))(p);
    }
    return k;
}

extern const PTMF D_003ED220, D_003ED230;

/* state: Hewie has it by the arm or leg (+0x104 10..15 where, 15 knocked down). Each bite adds the
   part's damage (+0x173C table) to +0x16BC, at most 1000; every hit key it may throw him off,
   the likelier the longer he's held on (+0x1634) and the more damage relative to +0x16DC */
void func_00286170(Pursuer *p) {
    u8 *m;
    u32 keys;
    s32 anim;

    if (p->c.state[0] == 7) {
        if (PU(p, 0x1628, s32) == 0) {
            PU(p, 0x1628, s32) = 2;
        }
        p->c.state[0] = 0;
    }
    m = p->c.motion;
    keys = AT(AT(m, 0x6A4, u8 *), 0x18, u32);
    if (((keys & MOTION_KEY_END) != 0) == 1) {
        if (PU(p, 0x1628, s32) == 1) {
            /* thrown off: the throw animation, and the throw hits Hewie */
            s32 k;
            u8 *e;

            anim = PU(p, 0x1624, s32) + 2;
            if (anim == AT(m, 0x55C, s32)) {
                Actor_SetState(&p->c.a, &D_003ED220);
            } else {
                func_002DDE20(m, anim, -1);
                k = Pursuer_ShakeOffEntry(p);
                e = PU(p, 0x171C, u8 *) + k * 0x24;
                Relation_Request(gProgress, *(u8 *)&p->c.a.slot, 2, 0xB, AT(e, 0x12, u16), 0, 0.0f);
            }
        } else if (PU(p, 0x1628, s32) == 2) {
            /* Hewie let go */
            if (p->c.unk104[0] == 0xF) {
                p->c.unk104[0] = 0x1709;
                PU(p, 0x1628, s32) = 0;
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x1F);
                func_002885B0(p);
            } else {
                anim = PU(p, 0x1624, s32) + 1;
                if (anim == AT(m, 0x55C, s32)) {
                    Actor_SetState(&p->c.a, &D_003ED230);
                } else if (p->c.hp == 0) {
                    VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x1F);
                    func_002885B0(p);
                } else {
                    func_002DDE20(m, anim, -1);
                }
            }
        } else {
            u8 *dmg = PU(p, 0x173C, u8 *);
            static const u8 part[6] = { 0x38, 0x3C, 0x30, 0x34, 0x40, 0x44 };

            if ((u32)(p->c.unk104[0] - 10) < 6 && dmg != NULL) {
                PU(p, 0x16BC, u32) += AT(dmg, part[p->c.unk104[0] - 10], s32);
                if (PU(p, 0x16BC, u32) > 1000) {
                    PU(p, 0x16BC, u32) = 1000;
                }
            }
        }
        PU(p, 0x1634, f32) += 1.0f;
    } else if ((keys & 0x400) && PU(p, 0x1628, s32) == 0 && (u32)(p->c.unk104[0] - 10) < 4 &&
               !(Progress_TestFlag(gProgress, 0xE) & 0xFF)) {
        f32 chance = PU(p, 0x1634, f32) * PU(p, 0x16E8, f32) +
                     25.0f * ((f32)PU(p, 0x16BC, u32) / (f32)PU(p, 0x16DC, u32));

        if (100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) <= chance) {
            u8 slot = PU(p, 0x153C, u8);

            PU(p, 0x1628, s32) = 1;
            if (gCharPartner->state[0] != 7) {
                /* tell Hewie he's been thrown (the rest of the message is never filled in) */
                Character *h = gCharPartner;

                h->state[0] = 7;
                h->state[1] = 0;
                h->state[2] = slot;
                h->state[3] = 0;
                h->state[4] = 0;
                h->state[5] = 0;
                h->state[6] = 0;
                h->state[7] = 0;
            }
        }
    }
    if (p->c.unk104[0] == 0xF && PU(p, 0x1624, s32) + 1 == MOTION_ANIM(p)) {
        if (p->c.hp <= 0) {
            p->c.hp = 1;
        }
    } else if (PU(p, 0x1628, s32) == 1) {
        if (p->c.hp <= 0) {
            p->c.hp = 1;
        }
        if (func_001F4770(p->c.motion, 0, -1, 1) & 0xFF & 2) {
            s32 k = Pursuer_ShakeOffEntry(p);

            if (k > 0) {
                u8 *e = PU(p, 0x171C, u8 *) + k * 0x24;

                if (e != NULL) {
                    Pursuer_Strike(p, e);
                }
            }
        }
    } else if (PU(p, 0x1628, s32) == 2 && PU(p, 0x1624, s32) + 1 == MOTION_ANIM(p) && p->c.hp <= 0) {
        p->c.hp = 1;
    }
    func_00125A10(&p->c);
}

extern const PTMF D_003ECBA0, D_003ECBB0, D_003ECBC0, D_003ECBD0, D_003ECBE0, D_003ECBF0;

/* the next stop of the search route, or the route is done */
static void Pursuer_NextStop(Pursuer *p) {
    if (PU(p, 0x1620, u8) < PU(p, 0x1621, u8) && PU(p, 0x1621, u8) < 8) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 8);
    } else {
        PU(p, 0x1620, u8) = PU(p, 0x1621, u8);
        PU(p, 0x16ED, u8) = 1;
    }
}

/* a stop searched: on to the next one, or the route is finished */
static void Pursuer_StopSearched(Pursuer *p) {
    PU(p, 0x1620, u8)++;
    PU(p, 0x1798, s32) = 0;
    if (PU(p, 0x1620, u8) >= PU(p, 0x1621, u8)) {
        PU(p, 0x16F4, u8) = 1;
        PU(p, 0x1794, s32) = 0;
    }
    VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, 0x10);
    func_00283C50(p);
}

/* a stop given up: on to the next one, or the route is finished */
static s32 Pursuer_StopSkipped(Pursuer *p) {
    PU(p, 0x1620, u8)++;
    PU(p, 0x1798, s32) = 0;
    if (PU(p, 0x1620, u8) < PU(p, 0x1621, u8) && PU(p, 0x1621, u8) < 8) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 8);
        return 0;
    }
    PU(p, 0x1620, u8) = PU(p, 0x1621, u8);
    PU(p, 0x16ED, u8) = 1;
    PU(p, 0x16F4, u8) = 1;
    PU(p, 0x1794, s32) = 0;
    return 1;
}

/* as Pursuer_BehaviourRun, while searching: run the state, act on how the action +0x175C went, then
   pick the next behaviour (chase on sight, go for Hewie when he's done enough damage, ...) */
/* 0x00296580 */
void Pursuer_BehaviourSearch(Pursuer *p) {
    PTMF *step = (PTMF *)((u8 *)p + 0x174C);

    if (ptmf_test(&p->c.a.state)) {
        ptmf_scall(p, &p->c.a.state);
    }
    switch (PU(p, 0x175C, s32)) {
    case 4:
    case 8:
        if (!(((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF)) {
            if (PU(p, 0x1590, f32) < 10.0f && PU(p, 0x1788, s32) == 0x201) {
                Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
            } else if (PU(p, 0x1788, s32) == 0) {
                PURSUER_STEP_DONE(p) = 0;
                Pursuer_StopSearched(p);
            }
        }
        if (PU(p, 0x1798, s32) != 0) {
            /* the time to search a stop ran out */
            PU(p, 0x1798, s32)--;
            if (PU(p, 0x1798, s32) == 0) {
                PURSUER_STEP_DONE(p) = 1;
            }
        }
        if (PU(p, 0x16EF, u8) == 1) {
            Pursuer_StopSkipped(p);
            PU(p, 0x16EF, u8) = 0;
        } else if (PURSUER_STEP_DONE(p) == 1) {
            PURSUER_STEP_DONE(p) = 0;
            Pursuer_StopSearched(p);
        }
        break;
    case 11:
    case 18:
        if (PU(p, 0x16EF, u8) == 1) {
            if (Pursuer_StopSkipped(p)) {
                PURSUER_STEP_NEXT(p) = 1;
            }
            PU(p, 0x16EF, u8) = 0;
        } else if (PURSUER_STEP_DONE(p) == 1) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 8);
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 2:
    case 9:
    case 13:
    case 23:
    case 28:
        if (PURSUER_STEP_DONE(p) == 1) {
            Pursuer_NextStop(p);
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 40:
    case 41:
        if (PURSUER_STEP_DONE(p) == 1) {
            PURSUER_STEP_DONE(p) = 0;
            VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, PU(p, 0x175C, s32) == 0x28 ? 0xE : 0xF);
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 8);
            func_00283C50(p);
            return;
        }
        break;
    case 12:
        if (PURSUER_STEP_DONE(p) == 1) {
            PURSUER_STEP_DONE(p) = 0;
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, (u32)(PU(p, 0x16C8, u8) - 2) < 2 ? 0x29 : 8);
        }
        break;
    case 16:
        if (PURSUER_STEP_DONE(p) == 1) {
            PURSUER_STEP_NEXT(p) = 1;
            PU(p, 0x1544, u8) = 1;
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    }
    if (PURSUER_STEP_NEXT(p) != 1) {
        return;
    }
    if (PU(p, 0x1544, u8) == 1) {
        ptmf_set(step, (func_00217260(p) & 0xFF) ? &D_003ECBB0 : &D_003ECBA0);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1C);
        PU(p, 0x16F3, u8) = 0;
        return;
    }
    if ((PU(p, 0x1545, u8) == 1 || PU(p, 0x1546, u8) == 1) &&
        (PU(p, 0x16DC, u32) < PU(p, 0x16BC, u32)) == 1 && !(Progress_TestFlag(gProgress, 0xE) & 0xFF) &&
        PU(p, 0x16C9, u8) < 3) {
        f32 dy;
        s32 go = 1;

        if (PU(p, 0x158C, f32) < 0.0f) {
            /* only on about the same floor as Hewie */
            if (!(p->c.a.pos[1] - gCharPartner->a.pos[1] <= 0.0f)) {
                dy = p->c.a.pos[1] - gCharPartner->a.pos[1];
            } else {
                dy = -(p->c.a.pos[1] - gCharPartner->a.pos[1]);
            }
            go = dy < 100.0f;
        }
        if (go) {
            /* Hewie has hurt it enough: go after him */
            ptmf_set(step, &D_003ECBC0);
            PU(p, 0x1758, s32) = -1;
            VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1C);
            PU(p, 0x16F3, u8) = 0;
            return;
        }
    }
    if (PU(p, 0x16F3, u8) == 1) {
        ptmf_set(step, (func_00217260(p) & 0xFF) ? &D_003ECBE0 : &D_003ECBD0);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1C);
        PU(p, 0x16F3, u8) = 0;
    } else if (PU(p, 0x16ED, u8) == 1) {
        PU(p, 0x16F4, u8) = 1;
        ptmf_set(step, &D_003ECBF0);
        PU(p, 0x1758, s32) = -1;
        PU(p, 0x16ED, u8) = 0;
    }
}

extern const PTMF D_003ED370, D_003ED380, D_003ED390, D_003ED3A0;

/* go through the door `door` into the next room (arriving by its exit there): forget the room's
   search and attack state, and either come in on screen (func_00217510) or go on off screen */
void func_002815E0(Pursuer *p, u32 door) {
    VObject *rooms;
    s32 room;
    u8 exit;

    if (AT(gProgress, 0x1FBEC1, u8) != 0) {
        func_0027AD80(p, door);
        return;
    }
    p->c.unk1530 = 0;
    rooms = gRooms;
    p->c.unk1538 = 0;
    p->c.unk1534 = 0;
    room = VCALL(rooms, 0x18, s32 (*)(VObject *, s32, u32))(rooms, p->c.a.room, door);
    exit = VCALL(rooms, 0x14, s32 (*)(VObject *, s32, u32))(rooms, p->c.a.room, door);
    Pursuer_ClearRoute(p);
    p->c.unk124 = p->c.unk128;
    PU(p, 0x1761, u8) = 0;
    PU(p, 0x1760, u8) = 0;
    PU(p, 0x178C, s32) = 0;
    PU(p, 0x1764, s32) = -1;
    p->c.unk14D0 = 0;
    func_001F6E10(p->c.motion);
    PU(p, 0x16F5, u8) = 0;
    PU(p, 0x16F8, u8) = 0;
    PU(p, 0x16F7, u8) = 0;
    PU(p, 0x1794, s32) = 0;
    PU(p, 0x1798, s32) = 0;
    PU(p, 0x16F3, u8) = 0;
    if (func_00217510(p) != 0) {
        PTMF *step = (PTMF *)((u8 *)p + 0x174C);

        if (p->c.a.unkC4 == 2) {
            ptmf_set(step, &D_003ED370);
            PU(p, 0x1758, s32) = -1;
            Pursuer_SetMove(p, &D_0045B3A0);
        }
        if (PU(p, 0x175C, s32) == 0x20) {
            func_002DDE20(p->c.motion, 0x1802, -1);
        } else {
            switch (PU(p, 0x16C8, u8)) {
            case 0:
            case 2:
                ptmf_set(step, &D_003ED380);
                PU(p, 0x1758, s32) = -1;
                Pursuer_SetMove(p, &D_0045B340);
                break;
            case 1:
            case 3:
                p->c.unk1388++;
                ptmf_set(step, &D_003ED390);
                PU(p, 0x1758, s32) = -1;
                if (room == PU(p, 0x1594, s32) && p->c.unk1388 >= p->c.unk1384) {
                    Pursuer_SetMove(p, &D_0045B3A0);
                } else {
                    Pursuer_SetMove(p, &D_0045B358);
                }
                break;
            case 4:
                p->c.unk1388++;
                ptmf_set(step, &D_003ED3A0);
                PU(p, 0x1758, s32) = -1;
                Pursuer_SetMove(p, &D_0045B358);
                break;
            }
            if (PU(p, 0x16C9, u8) == 2) {
                PU(p, 0x16C9, u8) = 0;
                PU(p, 0x16CB, u8) = 0;
                PU(p, 0x16CA, u8) = 0;
                PU(p, 0x16CC, u8) = 0;
                PU(p, 0x179C, s32) = -1;
                PU(p, 0x16F1, u8) = 0;
                PU(p, 0x16F3, u8) = 0;
                PU(p, 0x16F2, u8) = 0;
                PU(p, 0x16F4, u8) = 0;
            }
        }
        PU(p, 0x1624, s32) = 0;
        PU(p, 0x1628, s32) = 0;
        PU(p, 0x162C, s32) = 0;
        PU(p, 0x1630, s32) = 0;
        AT(p, 0x29, u8) = 1;
        AT(p, 0x2A, u8) = 1;
        AT(p, 0x2B, u8) = 0;
        p->c.a.unk2D = 0;
        PU(p, 0x1544, u8) = 0;
        PU(p, 0x1545, u8) = 0;
    } else {
        p->c.unk1388++;
        if (room == gCharPlayer->a.room) {
            /* into Fiona's room: it will appear */
            PU(p, 0x16ED, u8) = 1;
            AT(p, 0x2A, u8) = 0;
        } else {
            Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
            switch (PU(p, 0x16C8, u8)) {
            case 0:
            case 2:
                Pursuer_SetMove(p, &D_0045B340);
                break;
            case 1:
            case 3:
                if (room == PU(p, 0x1594, s32) && p->c.unk1388 >= p->c.unk1384) {
                    Pursuer_SetMove(p, &D_0045B3A0);
                } else {
                    Pursuer_SetMove(p, &D_0045B358);
                }
                break;
            case 4:
                Pursuer_SetMove(p, &D_0045B358);
                break;
            }
        }
    }
    p->c.door = exit;   /* the way in, in the new room */
    PU(p, 0x17B0, u8) = p->c.door;
    p->c.a.room = room;
    if (PU(p, 0x16C8, u8) == 1 && func_00217260(p) != 0) {
        PU(p, 0x16C8, u8) = 2;
    }
    VCALL(p, 0x148, void (*)(Pursuer *))(p);
    VCALL(gEvents, 0x2C, void (*)(VObject *, Pursuer *))(gEvents, p);
    if (p->c.state[0] == 5 && p->c.moveMode == 2) {
        func_00213270(p, 0xFF);
    }
}

extern const PTMF D_003ED5C0, D_003ED5D0, D_003ED5E0, D_003ED5F0;

/* func_002815E0 while the progress byte +0x1FBEC1 is set: a chase that keeps to Fiona's room
   (on arriving there it waits 150 frames, +0x17B4) */
void func_0027AD80(Pursuer *p, u32 door) {
    VObject *rooms = gRooms;
    s32 room;
    u8 exit;

    p->c.unk1530 = 0;
    p->c.unk1538 = 0;
    p->c.unk1534 = 0;
    room = VCALL(rooms, 0x18, s32 (*)(VObject *, s32, u32))(rooms, p->c.a.room, door);
    exit = VCALL(rooms, 0x14, s32 (*)(VObject *, s32, u32))(rooms, p->c.a.room, door);
    Pursuer_ClearRoute(p);
    p->c.unk124 = p->c.unk128;
    PU(p, 0x1761, u8) = 0;
    PU(p, 0x1760, u8) = 0;
    PU(p, 0x178C, s32) = 0;
    PU(p, 0x1764, s32) = -1;
    p->c.unk14D0 = 0;
    func_001F6E10(p->c.motion);
    PU(p, 0x16F5, u8) = 0;
    PU(p, 0x16F8, u8) = 0;
    PU(p, 0x16F7, u8) = 0;
    PU(p, 0x1794, s32) = 0;
    PU(p, 0x1798, s32) = 0;
    PU(p, 0x16F3, u8) = 0;
    if (func_00217510(p) != 0) {
        PTMF *step = (PTMF *)((u8 *)p + 0x174C);

        if (p->c.a.unkC4 == 2) {
            ptmf_set(step, &D_003ED5C0);
            PU(p, 0x1758, s32) = -1;
            Pursuer_SetMove(p, &D_0045B3A0);
        }
        if (PU(p, 0x175C, s32) == 0x20) {
            func_002DDE20(p->c.motion, 0x1802, -1);
        } else {
            switch (PU(p, 0x16C8, u8)) {
            case 0:
            case 2:
                ptmf_set(step, &D_003ED5D0);
                PU(p, 0x1758, s32) = -1;
                Pursuer_SetMove(p, &D_0045B3A0);
                PU(p, 0x17B4, s32) = 150;
                break;
            case 1:
            case 3:
                p->c.unk1388++;
                ptmf_set(step, &D_003ED5E0);
                PU(p, 0x1758, s32) = -1;
                if (room == PU(p, 0x1594, s32) && p->c.unk1388 >= p->c.unk1384) {
                    Pursuer_SetMove(p, &D_0045B3A0);
                } else {
                    Pursuer_SetMove(p, &D_0045B358);
                }
                break;
            case 4:
                p->c.unk1388++;
                ptmf_set(step, &D_003ED5F0);
                PU(p, 0x1758, s32) = -1;
                Pursuer_SetMove(p, &D_0045B358);
                break;
            }
            if (PU(p, 0x16C9, u8) == 2) {
                PU(p, 0x16C9, u8) = 0;
                PU(p, 0x16CB, u8) = 0;
                PU(p, 0x16CA, u8) = 0;
                PU(p, 0x16CC, u8) = 0;
                PU(p, 0x179C, s32) = -1;
                PU(p, 0x16F1, u8) = 0;
                PU(p, 0x16F3, u8) = 0;
                PU(p, 0x16F2, u8) = 0;
                PU(p, 0x16F4, u8) = 0;
            }
        }
        PU(p, 0x1624, s32) = 0;
        PU(p, 0x1628, s32) = 0;
        PU(p, 0x162C, s32) = 0;
        PU(p, 0x1630, s32) = 0;
        AT(p, 0x29, u8) = 1;
        AT(p, 0x2A, u8) = 1;
        AT(p, 0x2B, u8) = 0;
        p->c.a.unk2D = 0;
        PU(p, 0x1544, u8) = 0;
        PU(p, 0x1545, u8) = 0;
    } else {
        p->c.unk1388++;
        if (room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
            PU(p, 0x16ED, u8) = 1;
            AT(p, 0x2A, u8) = 0;
        } else {
            Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
            switch (PU(p, 0x16C8, u8)) {
            case 0:
            case 2:
                if (room != gCharPlayer->a.room) {
                    Pursuer_SetMove(p, &D_0045B340);
                } else {
                    Pursuer_SetMove(p, &D_0045B3A0);
                    PU(p, 0x17B4, s32) = 150;
                }
                break;
            case 1:
            case 3:
                if (room == PU(p, 0x1594, s32) && p->c.unk1388 >= p->c.unk1384) {
                    Pursuer_SetMove(p, &D_0045B3A0);
                } else {
                    Pursuer_SetMove(p, &D_0045B358);
                }
                break;
            case 4:
                Pursuer_SetMove(p, &D_0045B358);
                break;
            }
        }
    }
    p->c.door = exit;
    PU(p, 0x17B0, u8) = p->c.door;
    p->c.a.room = room;
    if (PU(p, 0x16C8, u8) == 1 && func_00217260(p) != 0) {
        PU(p, 0x16C8, u8) = 2;
    }
    VCALL(p, 0x148, void (*)(Pursuer *))(p);
    VCALL(gEvents, 0x2C, void (*)(VObject *, Pursuer *))(gEvents, p);
}

/* func_00283870, inline: may the pursuer go for its target? */
static inline s32 Pursuer_MayGo(Pursuer *p) {
    if (p->target->a.unk2D == 1 || (Progress_TestFlag(gProgress, 0xE) & 0xFF) == 1) {
        return 0;
    }
    return gCharPlayer->moveSub != 0x10;
}

/* nothing to the sides or behind within 2 units (func_00217ED0 at 110, -110 and 180 degrees) */
static s32 Pursuer_RoomAround(Pursuer *p) {
    return func_00217ED0(p, 0x1.921fb6p+1f /* 180 degrees */, 2.0f) == 0 &&
           func_00217ED0(p, 0x1.eb7c16p+0f /* 110 degrees */, 2.0f) == 0 &&
           func_00217ED0(p, -0x1.eb7c16p+0f, 2.0f) == 0;
}

/* attack (vtable +0x130: 12 after a hit, 13 after a miss) */
static void Pursuer_AttackAgain(Pursuer *p) {
    VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, PU(p, 0x1760, u8) != 0 ? 0xC : 0xD);
    func_00283C50(p);
}

/* the way round to face the target: 60..150 degrees off, or 0xFF */
static u32 Pursuer_TurnDir(Pursuer *p) {
    return func_00213FA0(p, p->target->a.pos, 0x1.0c1524p+0f /* 60 degrees */, 0x1.4f1a6ep+1f /* 150 degrees */) & 0xFF;
}

extern const PTMF D_003ECCC0;

/* as Pursuer_BehaviourRun, while attacking: run the state, act on how the action +0x175C went (close
   in, turn, back off, strike again), and once done (+0x16ED) choose: another attack if the
   target is right there, go after Hewie if he's hurt it enough, or the next behaviour */
/* 0x00293620 */
void Pursuer_BehaviourAttack(Pursuer *p) {
    s32 act;
    u32 dir;

    if (ptmf_test(&p->c.a.state)) {
        ptmf_scall(p, &p->c.a.state);
    }
    act = PU(p, 0x175C, s32);
    switch (act) {
    case 1:
        if (PU(p, 0x178C, s32) == 0) {
            PU(p, 0x16ED, u8) = 1;
        } else if (PU(p, 0x162C, s32)-- <= 0) {
            Pursuer_AttackAgain(p);
        }
        break;
    case 2:
        if (PURSUER_STEP_DONE(p) == 1) {
            PURSUER_STEP_DONE(p) = 0;
            PU(p, 0x16ED, u8) = 1;
        }
        break;
    case 0x17:
    case 0x1B:
        if (PURSUER_STEP_DONE(p) == 1) {
            PURSUER_STEP_DONE(p) = 0;
            PU(p, 0x16ED, u8) = 1;
            break;
        }
        if (act != 0x1B && AT(PU(p, 0x1720, u8 *) + p->c.unk104[0] * 8, 0x4, u8) == 0) {
            break;
        }
        if ((p->target == gCharPlayer && PU(p, 0x1544, u8) == 0) ||
            (p->target == gCharPartner && PU(p, 0x1545, u8) == 0)) {
            /* lost sight of it */
            PU(p, 0x16ED, u8) = 1;
        } else if (!(PU(p, 0x1590, f32) < 60.0f) || PU(p, 0x1590, f32) < 0.0f) {
            PU(p, 0x16ED, u8) = 1;
        }
        break;
    case 0x1D:
        if (Pursuer_MayGo(p) && p->target->moveMode != 4 && PU(p, 0x178C, s32) == 0) {
            PU(p, 0x16ED, u8) = 1;
        }
        if (!(PU(p, 0x1590, f32) <= 20.0f) || Pursuer_RoomAround(p)) {
            Pursuer_AttackAgain(p);
        }
        break;
    case 0x18:
        if (AT(PU(p, 0x1720, u8 *) + p->c.unk104[0] * 8, 0x4, u8) != 0) {
            dir = Pursuer_TurnDir(p);
            if (dir != 0xFF) {
                p->c.unk104[0] = dir;
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 3);
                PU(p, 0x178C, s32) = 0;
            }
        }
        /* fall through */
    case 3:
    case 0x1A:
        if (PURSUER_STEP_DONE(p) != 1) {
            break;
        }
        PURSUER_STEP_DONE(p) = 0;
        if (Pursuer_MayGo(p) && p->target->moveMode != 4 && PU(p, 0x178C, s32) == 0) {
            PU(p, 0x16ED, u8) = 1;
        } else if (!(PU(p, 0x1590, f32) <= 40.0f) || PU(p, 0x1590, f32) < 0.0f) {
            PU(p, 0x16ED, u8) = 1;
        } else if ((dir = Pursuer_TurnDir(p)) != 0xFF) {
            p->c.unk104[0] = dir;
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 3);
        } else if (!(PU(p, 0x1590, f32) <= 20.0f) || Pursuer_RoomAround(p)) {
            /* step in */
            PU(p, 0x162C, s32) = 30;
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
        } else {
            /* cornered too close: back off */
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x1D);
        }
        break;
    case 0x16:
        if (PURSUER_STEP_DONE(p) != 1) {
            break;
        }
        if ((dir = Pursuer_TurnDir(p)) != 0xFF) {
            p->c.unk104[0] = dir;
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 3);
        } else if (!(PU(p, 0x1590, f32) <= 20.0f)) {
            PU(p, 0x16ED, u8) = 1;
        } else {
            PU(p, 0x178C, s32) = PU(p, 0x1748, s32 *) != NULL ? PU(p, 0x1748, s32 *)[1] : 30;
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x1D);
        }
        PURSUER_STEP_DONE(p) = 0;
        break;
    default:
        if (PU(p, 0x16EF, u8) == 1) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x16);
            PU(p, 0x16EF, u8) = 0;
            break;
        }
        if (PURSUER_STEP_DONE(p) != 1) {
            break;
        }
        PURSUER_STEP_DONE(p) = 0;
        if (p->target == gCharPartner) {
            func_00297160(p);
            if (PU(p, 0x16C9, u8) < 2) {
                PU(p, 0x16C9, u8) = 2;
                PU(p, 0x16CA, u8) = 3;
            }
        }
        if (PU(p, 0x178C, s32) == 0 || !(PU(p, 0x1590, f32) <= 100.0f)) {
            PU(p, 0x16ED, u8) = 1;
        } else if (PU(p, 0x1590, f32) <= 20.0f) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x1D);
        } else {
            Pursuer_AttackAgain(p);
        }
        break;
    }
    if (PURSUER_STEP_NEXT(p) != 1 || PU(p, 0x16ED, u8) != 1) {
        return;
    }
    PU(p, 0x1760, u8) = 0;
    PU(p, 0x178C, s32) = 0;
    PU(p, 0x162C, s32) = 0;
    if (p->target == gCharPlayer) {
        if (PU(p, 0x1544, u8) == 1) {
            f32 d = PU(p, 0x1590, f32);

            if ((d < func_002838E0(p) || d < 10.0f) && !(d <= 0.0f)) {
                f32 a;

                if (!(func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]) <= 0.0f)) {
                    a = func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
                } else {
                    a = -func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
                }
                if (a < 0x1.921fb6p+1f * VCALL(p, 0x2EC, f32 (*)(Pursuer *))(p) / 180.0f &&
                    func_002175B0(&p->c.a, &p->target->a) != 0) {
                    /* Fiona is right there in front: attack at once */
                    VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, (s8)func_00283EF0(p));
                    func_00283C50(p);
                    return;
                }
            }
        }
    } else if (p->target == gCharPartner) {
        if (!(Pursuer_MayGo(p) & 0xFF)) {
            PU(p, 0x16BC, u32) = 0;
            PU(p, 0x16C0, s32) = 0;
            PU(p, 0x16F8, u8) = 0;
        }
        if (PU(p, 0x16DC, u32) < PU(p, 0x16BC, u32) && PU(p, 0x16F3, u8) == 0) {
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECCC0);
            PU(p, 0x1758, s32) = -1;
            return;
        }
    }
    VCALL(p, 0x13C, void (*)(Pursuer *))(p);
    if (PU(p, 0x16F3, u8) != 0) {
        PU(p, 0x16F3, u8) = 0;
        VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1C);
    }
}

extern const PTMF D_003ECC40, D_003ECC50, D_003ECC60;

/* as Pursuer_BehaviourRun, while after Hewie: run the state, follow him from room to room (through
   doors, +0x17B0), act on how the action +0x175C went, and attack when he's in reach */
/* 0x002948E0 */
void Pursuer_BehaviourHewie(Pursuer *p) {
    Progress *pr;
    u32 dir;

    if (PU(p, 0x175C, s32) == 4 && !(((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF) &&
        PU(p, 0x1788, s32) != 0x201) {
        Pursuer_PlayAnim(p, VCALL(p, 0x328, s32 (*)(Pursuer *))(p));
    }
    pr = gProgress;
    if (!(Progress_TestFlag(pr, 0xE) & 0xFF)) {
        u32 m = func_00217920(p) & 0xFF & 1;

        if (~PU(p, 0x1760, u8) & m) {
            Relation_Request(pr, *(u8 *)&p->c.a.slot, m & 0xFF, 4, 5, 0, 10.0f);
        }
    }
    if (ptmf_test(&p->c.a.state)) {
        ptmf_scall(p, &p->c.a.state);
    }
    if (PURSUER_STEP_NEXT(p) == 1 && PU(p, 0x16F3, u8) == 0 && p->c.a.room != gCharPartner->a.room &&
        PU(p, 0x175C, s32) != 0x27 && PU(p, 0x175C, s32) != 0xE) {
        /* Hewie has left the room: follow */
        VCALL(p, 0xB4, void (*)(Pursuer *, Character *))(p, gCharPartner);
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x27);
        PU(p, 0x16EF, u8) = 0;
        PURSUER_STEP_DONE(p) = 0;
    }
    switch (PU(p, 0x175C, s32)) {
    case 0xE: {
        u32 e = PursuerGroup_Fields(pr, PU(p, 0x17B0, u8), *(u8 *)&p->c.a.slot) & 0xFF;

        if (p->c.a.room == gCharPartner->a.room) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 7);
            VCALL(p, 0xB4, void (*)(Pursuer *, Character *))(p, gCharPartner);
            PU(p, 0x16EF, u8) = 0;
            PURSUER_STEP_DONE(p) = 0;
        } else if (PURSUER_STEP_DONE(p) == 1 || (e & 2)) {
            /* through the door after him */
            PU(p, 0x162C, s32) = 0;
            PURSUER_STEP_DONE(p) = 0;
            Pursuer_ClearRoute(p);
            PU(p, 0x16C8, u8) = 1;
            PU(p, 0x16BC, s32) = 0;
            PU(p, 0x16C0, s32) = 0;
            func_002815E0(p, PU(p, 0x17B0, u8));
            return;
        }
        break;
    }
    case 3:
        if (!(PU(p, 0x158C, f32) <= VCALL(p, 0x2F0, f32 (*)(Pursuer *))(p)) || PU(p, 0x158C, f32) < 0.0f) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 7);
        } else if (PURSUER_STEP_DONE(p) == 1) {
            PURSUER_STEP_DONE(p) = 0;
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
        }
        break;
    case 1:
        if ((dir = Pursuer_TurnDir(p)) != 0xFF) {
            p->c.unk104[0] = dir;
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 3);
        } else {
            Character *t = gCharPartner != NULL ? gCharPartner : p->target;
            f32 h = func_001244D0(&p->c.a, t->a.pos);

            func_002140A0(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
            if (!(PU(p, 0x158C, f32) <= VCALL(p, 0x2F0, f32 (*)(Pursuer *))(p)) || PU(p, 0x158C, f32) < 0.0f) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 7);
            }
        }
        break;
    case 0x27:
        if (p->c.a.room == gCharPartner->a.room) {
            /* caught up with him */
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 7);
            VCALL(p, 0xB4, void (*)(Pursuer *, Character *))(p, gCharPartner);
            PU(p, 0x16EF, u8) = 0;
            PURSUER_STEP_DONE(p) = 0;
        } else if (PU(p, 0x16EF, u8) == 1 || !(func_00178980(pr, p->c.a.room, PU(p, 0x17B0, u8)) & 0xFF)) {
            PU(p, 0x16ED, u8) = 1;
            PU(p, 0x16EF, u8) = 0;
            PURSUER_STEP_DONE(p) = 0;
        } else if (PURSUER_STEP_DONE(p) == 1 || func_00212190(p, PU(p, 0x17B0, u8)) != 0) {
            PURSUER_STEP_DONE(p) = 0;
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0xE);
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 7:
        if (PU(p, 0x16EF, u8) == 1) {
            PU(p, 0x16ED, u8) = 1;
            PU(p, 0x16EF, u8) = 0;
        } else if (PU(p, 0x158C, f32) < VCALL(p, 0x2F0, f32 (*)(Pursuer *))(p) && !(PU(p, 0x158C, f32) < 0.0f)) {
            if ((dir = Pursuer_TurnDir(p)) == 0xFF) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
            } else {
                p->c.unk104[0] = dir;
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 3);
            }
        }
        break;
    case 2:
        if (PURSUER_STEP_DONE(p) == 1) {
            PU(p, 0x16ED, u8) = 1;
            PURSUER_STEP_DONE(p) = 0;
        }
        /* fall through */
    case 0x1C:
        if (PU(p, 0x16F3, u8) != 0) {
            PURSUER_STEP_NEXT(p) = 1;
        }
        /* fall through */
    case 0xB:
    case 0x10:
    case 0x12:
        if (PU(p, 0x16EF, u8) == 1) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 7);
            PU(p, 0x16EF, u8) = 0;
        } else if (PURSUER_STEP_DONE(p) == 1) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 7);
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    }
    if (PU(p, 0x1630, s32) > 0) {
        PU(p, 0x1630, s32)--;
    }
    if (PURSUER_STEP_NEXT(p) != 1) {
        return;
    }
    if (p->c.a.room == gCharPartner->a.room && func_00217340(p) != func_002172F0(p, gCharPartner)) {
        VCALL(p, 0x13C, void (*)(Pursuer *))(p);
        PU(p, 0x16ED, u8) = 0;
        PU(p, 0x1630, s32) = 0;
        PU(p, 0x16F8, u8) = 0;
    }
    if (PU(p, 0x16ED, u8) == 1 || (Progress_TestFlag(pr, 0xE) & 0xFF) == 1) {
        VCALL(p, 0x13C, void (*)(Pursuer *))(p);
        PU(p, 0x16ED, u8) = 0;
        PU(p, 0x1630, s32) = 0;
        PU(p, 0x16F8, u8) = 0;
        return;
    }
    if (PU(p, 0x16F3, u8) == 1) {
        const PTMF *s;

        if (func_00217260(p) & 0xFF) {
            s = PU(p, 0x1544, u8) == 1 ? &D_003ECC50 : &D_003ECC60;
        } else {
            s = &D_003ECC40;
        }
        ptmf_set((PTMF *)((u8 *)p + 0x174C), s);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1C);
        PU(p, 0x16F3, u8) = 0;
        PU(p, 0x1630, s32) = 0;
        PU(p, 0x16F8, u8) = 0;
        return;
    }
    if (PU(p, 0x1630, s32) <= 0 && PU(p, 0x16BC, s32) != 0) {
        PU(p, 0x16BC, s32) = 0;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 2);
        p->c.unk104[0] = 0x1602;
        PU(p, 0x1630, s32) = 0;
        return;
    }
    if (PU(p, 0x158C, f32) < VCALL(p, 0x2F4, f32 (*)(Pursuer *))(p) && !(PU(p, 0x158C, f32) < 0.0f)) {
        f32 d = PU(p, 0x158C, f32);

        if (d < func_002838E0(p) || d < 10.0f) {
            f32 a;

            if (!(func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]) <= 0.0f)) {
                a = func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
            } else {
                a = -func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
            }
            if (a < 0x1.921fb6p+1f * VCALL(p, 0x2EC, f32 (*)(Pursuer *))(p) / 180.0f) {
                VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, 0xB);
                func_00283C50(p);
                PU(p, 0x1630, s32) = 0;
            }
        }
    }
}

/* back off (action 5) or hold off (6) for the entry's time, by its chance in percent */
static void Pursuer_HoldOff(Pursuer *p, const u8 *e) {
    if (100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom) <= AT(e, 0x0, f32)) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
        PU(p, 0x162C, s32) = AT(e, 0x4, s32);
    } else {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
        PU(p, 0x162C, s32) = AT(e, 0x8, s32);
    }
}

/* the target moved off its triangle: face it again (and pick up the idle) */
static void Pursuer_Refollow(Pursuer *p) {
    u32 tri = p->target->a.navTri;

    if (func_00124480(&p->c.a, p->target->a.pos, 0) == tri) {
        if (PU(p, 0x1788, s32) != 0x201) {
            Pursuer_PlayAnim(p, VCALL(p, 0x328, s32 (*)(Pursuer *))(p));
        }
        func_002143D0(p, p->target->a.pos);
    }
    PU(p, 0x16EF, u8) = 0;
}

extern const PTMF D_003ECC20;

/* as Pursuer_BehaviourRun for a pursuer that stalks Fiona at a distance: it closes in, then backs or
   holds off for a while (chance and times from +0x1730 by the threat level, gProgress+0x7B8),
   and strikes when she's right in front of it */
/* 0x00295670 */
void Pursuer_BehaviourStalk(Pursuer *p) {
    const u8 *e;
    f32 near;

    if (ptmf_test(&p->c.a.state)) {
        ptmf_scall(p, &p->c.a.state);
    }
    e = PU(p, 0x1730, u8 *) + AT(gProgress, 0x7B8, u8) * 12;
    switch (PU(p, 0x175C, s32)) {
    case 0x1D:
        near = 5.0f + (p->c.a.radius + p->target->a.radius);
        if (!(PU(p, 0x1588, f32) < near) || PU(p, 0x1588, f32) < 0.0f) {
            Pursuer_HoldOff(p, e);
        }
        break;
    case 6:
        if (PU(p, 0x16EF, u8) != 0) {
            Pursuer_Refollow(p);
        }
        if (PU(p, 0x162C, s32) <= 0 && PU(p, 0x1588, f32) < 100.0f) {
            /* waited long enough and close: attack */
            VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, 0xA);
            func_00283C50(p);
            return;
        }
        if (PU(p, 0x1588, f32) <= p->c.a.radius + p->target->a.radius && !(PU(p, 0x1588, f32) < 0.0f)) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x1D);
        }
        break;
    case 5:
        if (PU(p, 0x16EF, u8) != 0) {
            Pursuer_Refollow(p);
        }
        if (PU(p, 0x162C, s32) <= 0 || !(PU(p, 0x1588, f32) <= 100.0f)) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
            PU(p, 0x162C, s32) = AT(e, 0x8, s32);
        } else if (PU(p, 0x1588, f32) <= p->c.a.radius + p->target->a.radius && !(PU(p, 0x1588, f32) < 0.0f)) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x1D);
        }
        break;
    case 0xB:
    case 0x10:
    case 0x12:
    case 0x1C:
        if (PU(p, 0x16EF, u8) == 1) {
            PURSUER_STEP_DONE(p) = 0;
            PU(p, 0x16EF, u8) = 0;
            PURSUER_STEP_NEXT(p) = 1;
            func_0029AF20(p);
        } else if (PURSUER_STEP_DONE(p) == 1) {
            Pursuer_HoldOff(p, e);
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 0x19:
    case 0x1A:
        if ((PU(p, 0x175C, s32) == 0x1A || AT(PU(p, 0x1720, u8 *) + p->c.unk104[0] * 8, 0x4, u8) != 0) &&
            !(PU(p, 0x1588, f32) <= 100.0f)) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
            PU(p, 0x162C, s32) = AT(e, 0x8, s32);
            PURSUER_STEP_DONE(p) = 0;
            PU(p, 0x16EF, u8) = 0;
        }
        if (PURSUER_STEP_DONE(p) == 1 || PU(p, 0x16EF, u8) == 1) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
            PU(p, 0x162C, s32) = AT(e, 0x4, s32);
            PURSUER_STEP_DONE(p) = 0;
            PU(p, 0x16EF, u8) = 0;
        }
        break;
    case 1: {
        Character *t = gCharPlayer != NULL ? gCharPlayer : p->target;
        f32 h = func_001244D0(&p->c.a, t->a.pos);
        f32 a;

        func_002140A0(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
        if (!(PU(p, 0x1588, f32) <= func_002838E0(p))) {
            Pursuer_HoldOff(p, e);
            break;
        }
        if (!(func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]) <= 0.0f)) {
            a = func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
        } else {
            a = -func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
        }
        if (!(a <= 0x1.921fb6p+1f * VCALL(p, 0x2EC, f32 (*)(Pursuer *))(p) / 180.0f)) {
            Pursuer_HoldOff(p, e);
            break;
        }
        if (PU(p, 0x1588, f32) < 0.0f) {
            VCALL(p, 0xB0, void (*)(Pursuer *))(p);
            if (!(func_00284440(p) & 0xFF)) {
                func_0029AF20(p);
                return;
            }
        }
        break;
    }
    }
    if (PU(p, 0x162C, s32) > 0) {
        PU(p, 0x162C, s32)--;
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
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECC20);
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

/* the door +0x17B0 can't be passed: mark its link in the visited set (+0x148C) and plan again
   to the goal room; 0 if there's no way at all */
static s32 Pursuer_Replan(Pursuer *p, VObject *rooms) {
    u32 bit;

    PURSUER_STEP_DONE(p) = 0;
    bit = VCALL(rooms, 0x10, s32 (*)(VObject *, s32, u32))(rooms, p->c.a.room, PU(p, 0x17B0, u8)) & 0xFFFF;
    p->c.unk148C[bit >> 5] |= 1 << (bit & 0x1F);
    return func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
}

extern const PTMF D_003ECCE0, D_003ECCF0, D_003ECD00, D_003ECD10, D_003ECD20;

/* as Pursuer_BehaviourRun while heading through doors on screen: walk to the door (+0x17B0), open it
   (action 0xE), go through (0xF / func_002815E0), or plan round it when it's locked */
/* 0x002928B0 */
void Pursuer_BehaviourDoors(Pursuer *p) {
    u32 ds = 0xFF;
    u32 r;

    if (ptmf_test(&p->c.a.state)) {
        ptmf_scall(p, &p->c.a.state);
    }
    if (PU(p, 0x17B0, u8) != 0xFF) {
        ds = PursuerGroup_Fields(gProgress, PU(p, 0x17B0, u8), *(u8 *)&p->c.a.slot) & 0xFF;
    }
    switch (PU(p, 0x175C, s32)) {
    case 0x27:
        if (PU(p, 0x16EF, u8) == 1) {
            PU(p, 0x16EF, u8) = 0;
            if (func_0027CA00(p) & 0xFF) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x27);
            } else {
                func_0029AF20(p);
            }
            return;
        }
        if (PURSUER_STEP_DONE(p) != 1) {
            break;
        }
        switch (func_00211E00(p, PU(p, 0x17B0, u8)) & 0xFF) {
        case 5:
        case 6:
            p->c.unk100 = PU(p, 0x17B0, u8);
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0xA);
            PURSUER_STEP_DONE(p) = 0;
            break;
        case 4:
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0xE);
            PURSUER_STEP_DONE(p) = 0;
            break;
        case 3:
            if (Pursuer_Replan(p, gRooms) == -1 && !(func_0027CA00(p) & 0xFF)) {
                func_0029AF20(p);
                return;
            }
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x27);
            break;
        }
        break;
    case 4: {
        VObject *rooms;
        f32 door[4] __attribute__((aligned(16)));
        u32 onMesh, a;

        if (PU(p, 0x16EF, u8) == 1) {
            PU(p, 0x16EF, u8) = 0;
            func_0029AF20(p);
            return;
        }
        rooms = gRooms;
        VCALL(rooms, 0x30, void (*)(VObject *, u32, f32 *))(rooms, PU(p, 0x17B0, u8), door);
        onMesh = func_00124480(&p->c.a, door, -1) != (u32)-1;
        a = func_00123C60(&p->c.a, PU(p, 0x17B0, u8), (f32 *)((u8 *)p + 0x15B0)) & 0xFF;
        if (!(ds & 4) && PURSUER_STEP_DONE(p) != 1 && !((onMesh & 0xFF) & a)) {
            break;
        }
        switch (func_00211E00(p, PU(p, 0x17B0, u8)) & 0xFF) {
        case 5:
        case 6:
            if (!(Progress_CurRoomFlag(gProgress, p->c.a.room, PU(p, 0x17B0, u8)) & 0xFF)) {
                p->c.unk100 = PU(p, 0x17B0, u8);
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0xA);
                PURSUER_STEP_DONE(p) = 0;
            }
            break;
        case 4:
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0xE);
            PURSUER_STEP_DONE(p) = 0;
            break;
        case 3:
            if (Pursuer_Replan(p, rooms) == -1 && (func_0027CA00(p) & 0xFF) != 1) {
                func_0029AF20(p);
                return;
            }
            PU(p, 0x17B0, u8) = VCALL(rooms, 0x3C, s32 (*)(VObject *, u32, s32))(rooms, PU(p, 0x138C, u16), p->c.a.room);
            PU(p, 0x15A4, s32) = VCALL(rooms, 0x34, s32 (*)(VObject *, u32, f32 *))(rooms, PU(p, 0x17B0, u8), (f32 *)((u8 *)p + 0x15B0));
            break;
        }
        break;
    }
    case 0xA:
        if (PU(p, 0x16EF, u8) == 1) {
            PU(p, 0x16EF, u8) = 0;
            func_0029AF20(p);
            return;
        }
        if (PURSUER_STEP_DONE(p) == 1) {
            /* at the door: open it (the original leaves 0xE in the register from its case chain) */
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0xE);
            PU(p, 0x162C, s32) = 1;
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 0xE:
        if ((PURSUER_STEP_DONE(p) == 1 || (ds & 2)) && !(ds & 8)) {
            if (VCALL(gRooms, 0x78, s32 (*)(VObject *, s32, u32))(gRooms, p->c.a.room, PU(p, 0x17B0, u8)) != 0) {
                /* the door is open: through it */
                p->c.unk104[0] = PU(p, 0x162C, s32);
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0xF);
            } else {
                PU(p, 0x16ED, u8) = 1;
            }
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 0xF:
        if (PURSUER_STEP_DONE(p) == 1 || PU(p, 0x16EF, u8) == 1) {
            PU(p, 0x16ED, u8) = 1;
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 0x28:
    case 0x29:
        if (PURSUER_STEP_DONE(p) == 1) {
            PURSUER_STEP_DONE(p) = 0;
            VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, PU(p, 0x175C, s32) == 0x28 ? 0xE : 0xF);
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x27);
            func_00283C50(p);
            return;
        }
        break;
    case 0xC:
        if (PURSUER_STEP_DONE(p) == 1) {
            PURSUER_STEP_DONE(p) = 0;
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, (u32)(PU(p, 0x16C8, u8) - 2) < 2 ? 0x29 : 0x27);
        }
        break;
    case 2:
    case 0xB:
    case 0xD:
    case 0x12:
    case 0x17:
    case 0x1C:
        if (PU(p, 0x16EF, u8) == 1) {
            PU(p, 0x16EF, u8) = 0;
            PURSUER_STEP_NEXT(p) = 1;
            func_0029AF20(p);
            return;
        }
        if (PURSUER_STEP_DONE(p) == 1) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x27);
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 0x10:
        if (PURSUER_STEP_DONE(p) == 1) {
            PURSUER_STEP_NEXT(p) = 1;
            PU(p, 0x1544, u8) = 1;
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    }
    if (PURSUER_STEP_NEXT(p) != 1) {
        return;
    }
    if (PU(p, 0x1544, u8) == 1 || PU(p, 0x16C8, u8) == 0) {
        if (func_00217260(p) != 0) {
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECCE0);
            PU(p, 0x1758, s32) = -1;
            if (PU(p, 0x16F3, u8) != 0) {
                VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1C);
                PU(p, 0x16F3, u8) = 0;
            }
            return;
        }
        if (PU(p, 0x16F3, u8) != 0) {
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECCF0);
            PU(p, 0x1758, s32) = -1;
            VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1C);
            PU(p, 0x16F3, u8) = 0;
            return;
        }
    }
    if ((PU(p, 0x1545, u8) == 1 || PU(p, 0x1546, u8) == 1) &&
        (PU(p, 0x16DC, u32) < PU(p, 0x16BC, u32)) == 1 && !(Progress_TestFlag(gProgress, 0xE) & 0xFF) &&
        PU(p, 0x16C9, u8) < 3) {
        f32 dy;
        s32 go = 1;

        if (PU(p, 0x158C, f32) < 0.0f) {
            if (!(p->c.a.pos[1] - gCharPartner->a.pos[1] <= 0.0f)) {
                dy = p->c.a.pos[1] - gCharPartner->a.pos[1];
            } else {
                dy = -(p->c.a.pos[1] - gCharPartner->a.pos[1]);
            }
            go = dy < 100.0f;
        }
        if (go) {
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECD00);
            PU(p, 0x1758, s32) = -1;
            VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1C);
            PU(p, 0x16F3, u8) = 0;
            return;
        }
    }
    if (PU(p, 0x16F3, u8) != 0 || PU(p, 0x16C8, u8) == 2) {
        Progress *pr;

        PU(p, 0x16F3, u8) = 0;
        PU(p, 0x16ED, u8) = 0;
        if (PU(p, 0x175C, s32) == 0xF && (pr = gProgress, !(func_00178980(pr, p->c.a.room, PU(p, 0x17B0, u8)) & 0xFF))) {
            /* in the doorway with the door shut */
            if (!(func_00217260(p) & 0xFF)) {
                u8 want = PU(p, 0x17B0, u8);

                if (want == (VCALL(gRooms, 0x3C, s32 (*)(VObject *, u32, s32))(gRooms,
                                 PU(p, 0x138C + p->c.unk1388 * 2, u16), p->c.a.room) & 0xFF)) {
                    PU(p, 0x162C, s32) = 0;
                    func_002815E0(p, PU(p, 0x17B0, u8));
                    return;
                }
            }
            if (!(DoorHold_Take(pr, p->c.a.room, PU(p, 0x17B0, u8), *(u8 *)&p->c.a.slot) & 0xFF)) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0xC);
                p->c.a.unk2D = 1;
                p->c.unk100 = PU(p, 0x17B0, u8);
            } else {
                PU(p, 0x16C8, u8) = 3;
                VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
                func_002815E0(p, PU(p, 0x17B0, u8));
            }
            return;
        }
        ptmf_set((PTMF *)((u8 *)p + 0x174C), (func_00217260(p) & 0xFF) ? &D_003ECD20 : &D_003ECD10);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1C);
        return;
    }
    if (PU(p, 0x16ED, u8) == 1) {
        PU(p, 0x162C, s32) = 0;
        func_002815E0(p, PU(p, 0x17B0, u8));
        PU(p, 0x16ED, u8) = 0;
    }
}

extern const PTMF D_003ED3B0, D_003ED3C0, D_003ED3D0, D_003ED3E0, D_003ED3F0, D_003ED400, D_003ED410,
    D_003ED420, D_003ED430, D_003ED440;

/* place the pursuer on triangle tri facing its heading (vtable +0x28), or nowhere */
static void Pursuer_PlaceOn(Pursuer *p, s32 tri) {
    if (VCALL(p, 0x28, s32 (*)(Pursuer *, s32, f32 *, f32 *))(p, tri, &p->c.a.angle[1], p->c.a.pos) == -1) {
        func_00124890(&p->c.a, -1);
    }
}

/* show up: once the pursuer is in Fiona's room (func_00217510) stand it at the door it came in
   by and pick what to do, by the behaviour step it was given (+0x174C); while still away, wait
   at the door (+0x1624 1: go through) */
/* 0x002809E0 */
void Pursuer_ShowUp(Pursuer *p) {
    PTMF *step = (PTMF *)((u8 *)p + 0x174C);
    f32 at[4] __attribute__((aligned(16)));

    if ((u32)p->c.a.slot >= 3) {
        VCALL(p, 0x150, void (*)(Pursuer *))(p);
        return;
    }
    if (func_00217510(p) == 0) {
        if (PU(p, 0x1624, s32) == 1) {
            PU(p, 0x16C9, u8) = 6;
            PU(p, 0x16CA, u8) = 7;
            PU(p, 0x17B4, s32) = 0;
            func_002815E0(p, PU(p, 0x17B0, u8));
        } else {
            switch (PU(p, 0x1788, s32)) {
            case 0x2300:
            case 0x1600:
            case 0x1300:
            case 0x1000:
            case 0xE00:
            case 0x700:
            case 0x600:
                Pursuer_PlayAnimBlend(p, 0);
                break;
            }
            if (p->c.a.unkC4 != 2) {
                func_0027EEA0(p);
            }
        }
    } else {
        if (!__ptmf_cmpr(step, &D_003ED3B0) || !__ptmf_cmpr(step, &D_003ED3C0) || !__ptmf_cmpr(step, &D_003ED3D0)) {
            func_0027FC70(p);
            func_00214ED0(p);
            if (PU(p, 0x16C8, u8) == 0) {
                PU(p, 0x17B4, s32) = 0;
            } else {
                func_0027EEA0(p);
            }
            func_0027FE90(p);
            VCALL(p, 0x13C, void (*)(Pursuer *))(p);
            p->c.moveMode = 0;
            p->c.moveSub = 0;
            PU(p, 0x1664, s32) = 0;
            PURSUER_STEP_NEXT(p) = 1;
            PU(p, 0x16F6, u8) = 1;
        } else if (!__ptmf_cmpr(step, &D_003ED3E0) || !__ptmf_cmpr(step, &D_003ED3F0)) {
            ptmf_set(step, &D_003ED400);
            PU(p, 0x1758, s32) = -1;
            if (PU(p, 0x175C, s32) == 0x23) {
                /* came in knocked down or getting up */
                if (p->c.a.unkC4 != 2) {
                    switch (MOTION_ANIM(p)) {
                    case 0x1708:
                        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x20);
                        Pursuer_PlayAnimBlend(p, 0x1806);
                        break;
                    case 0x1706:
                        VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 2);
                        p->c.unk104[0] = 0x1707;
                        break;
                    case 0x1703:
                        VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 2);
                        p->c.unk104[0] = 0x1704;
                        break;
                    case 0x1700:
                        VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 2);
                        p->c.unk104[0] = 0x1701;
                        break;
                    default:
                        p->c.unk104[0] = 0;
                        break;
                    }
                    PU(p, 0x1664, s32) = 0;
                } else if (PU(p, 0x17B4, s32) != 0) {
                    VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1F);
                    Pursuer_PlayAnimBlend(p, ((s32 (*)(Pursuer *, Character *, u32))func_001235C0)(p, (Character *)p, 0) != 0 ? 0x1804 : 0x1800);
                    PU(p, 0x17B4, s32) = 0;
                } else {
                    VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x1F);
                    Actor_SetState(&p->c.a, &D_003ED410);
                    Pursuer_PlayAnimBlend(p, ((s32 (*)(Pursuer *, Character *, u32))func_001235C0)(p, (Character *)p, 0) != 0 ? 0x1806 : 0x1802);
                }
                PU(p, 0x1761, u8) = 1;
                func_0027EEA0(p);
                Pursuer_PlaceOn(p, p->c.a.navTri);
            } else {
                p->c.moveMode = 4;
                p->c.moveSub = 0xA;
                if (PU(p, 0x1664, u32) < 120) {
                    PU(p, 0x1664, s32) = 0;
                }
                if (p->c.a.unkC4 != 2) {
                    PU(p, 0x17B4, s32) = 0;
                }
                if (p->c.a.navTri != (u32)-1) {
                    Pursuer_PlaceOn(p, p->c.a.navTri);
                } else {
                    VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x20);
                    p->c.a.navTri = VCALL(gRooms, 0x28, s32 (*)(VObject *, u32))(gRooms, p->c.door);
                    VCALL(p, 0x28, s32 (*)(Pursuer *, s32, f32 *, f32 *))(p, p->c.a.navTri, NULL, NULL);
                }
            }
            PURSUER_STEP_NEXT(p) = 0;
            PU(p, 0x16F6, u8) = 0;
        } else if (!__ptmf_cmpr(step, &D_003ED420) || !__ptmf_cmpr(step, &D_003ED430)) {
            s32 room;
            s32 *e;
            s32 found = 0;

            ptmf_set(step, &D_003ED440);
            PU(p, 0x1758, s32) = -1;
            room = p->c.a.room;
            for (e = VCALL(p, 0x314, s32 *(*)(Pursuer *))(p); *e != -1; e += 2) {
                if (room == *e) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x26);
            } else if (p->c.a.navTri == (u32)-1) {
                func_00214AF0(p);
                VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x26);
            } else {
                VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 4);
            }
            func_0027FC70(p);
            PURSUER_STEP_NEXT(p) = 1;
            PU(p, 0x16F6, u8) = 0;
        } else {
            /* stand at the door, unless that floor is barred to it */
            u8 *t = Pursuer_NavTri(p->c.a.navTri);
            u32 fl = t != NULL ? AT(t, 0x3C, u32) : 0;

            if ((fl & p->c.a.navMask) || (fl & 0x300000) == 0x300000) {
                p->c.a.navTri = VCALL(gRooms, 0x28, s32 (*)(VObject *, u32))(gRooms, p->c.door);
            }
            if (p->c.a.navTri != (u32)-1) {
                VCALL(p, 0x28, s32 (*)(Pursuer *, s32, f32 *, f32 *))(p, p->c.a.navTri, NULL, NULL);
            } else {
                func_00124890(&p->c.a, -1);
            }
            PURSUER_STEP_NEXT(p) = 1;
        }
        p->c.unk14C4 = 0;
        func_0027F5F0(p);
        func_00126270(&p->c);
    }
    if (func_00122B50(&p->c.a, at) != 0) {
        ((void (*)(Character *, s32, f32 *))func_001264C0)(&p->c, 0, at);
        ((void (*)(Character *, s32, f32 *))func_001264C0)(&p->c, 5, at);
    }
}

extern const PTMF D_003EC950, D_003EC960, D_003EC970, D_003EC980, D_003EC990, D_003EC9A0;

/* the bite table +0x173C damage for a hit from in front/behind (bit 0) and high/low (bit 1) */
static void Pursuer_AddBiteDamage(Pursuer *p, u32 dir) {
    u8 *t = PU(p, 0x173C, u8 *);
    static const u8 off[4] = { 0x0, 0x8, 0x4, 0xC };

    if (t == NULL || (dir & 0xFF) > 3) {
        return;
    }
    PU(p, 0x16BC, u32) += AT(t, off[dir & 0xFF], s32);
    if (PU(p, 0x16BC, u32) > 1000) {
        PU(p, 0x16BC, u32) = 1000;
    }
}

static void Pursuer_ClearMessage(Pursuer *p) {
    p->c.state[0] = 0;
    p->c.state[1] = 0;
}

/* a hit (the message in the state block: [1] kind, [2] who from, 0xFF none; [3] damage, [4] flags,
   0x8000 stuns): take the damage, count Hewie's bites, and react (flinch, fall, die) */
void func_0029B8B0(Pursuer *p) {
    s32 kind = p->c.state[1];
    s32 from;
    f32 at[4] __attribute__((aligned(16)));
    f32 a;
    u32 dir;

    if (p->c.moveSub == 0xA && kind != 5) {
        if (PU(p, 0x1628, s32) == 0) {
            PU(p, 0x1628, s32) = 1;
        }
        Pursuer_ClearMessage(p);
        return;
    }
    if (kind == 6) {
        /* Hewie snapping at it */
        f32 d = PU(p, 0x158C, f32);

        if (d < 100.0f && !(d < 0.0f) && p->c.moveMode != 4 && p->c.moveMode != 7 &&
            PU(p, 0x16C8, u8) != 0 && PU(p, 0x16C8, u8) != 4) {
            u8 *t = PU(p, 0x173C, u8 *);

            if (t != NULL) {
                Progress *pr = gProgress;

                PU(p, 0x16BC, u32) += (Progress_TestFlag(pr, 9) != 0 || Progress_TestFlag(pr, 0xA) != 0) ? AT(t, 0x14, s32) : AT(t, 0x10, s32);
                if (PU(p, 0x16BC, u32) > 1000) {
                    PU(p, 0x16BC, u32) = 1000;
                }
            }
            if (PU(p, 0x16DC, u32) < PU(p, 0x16BC, u32) && !(Progress_TestFlag(gProgress, 0xE) & 0xFF) &&
                PU(p, 0x16C9, u8) < 3 && p->target != gCharPartner && p->c.moveMode == 0) {
                ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003EC950);
                PU(p, 0x1758, s32) = -1;
            }
        }
        Pursuer_ClearMessage(p);
        return;
    }
    VCALL(p, 0x94, void (*)(Pursuer *, s32))(p, p->c.state[3]);
    PU(p, 0x16C4, s32) += p->c.state[3];
    if (PU(p, 0x16C4, s32) >= 999) {
        PU(p, 0x16C4, s32) = 999;
    }
    if (p->c.state[1] == 3) {
        /* a killing blow */
        VCALL(p, 0x94, void (*)(Pursuer *, s32))(p, p->c.hp);
    }
    if (p->c.state[2] != 0xFF) {
        sceVu0CopyVector(at, gCharacters[p->c.state[2]]->a.pos);
    } else {
        sceVu0CopyVector(at, (f32 *)((u8 *)gProgress + p->c.a.slot * 32 + 0x1060));
    }
    if (!(func_002E2D00(func_001244D0(&p->c.a, at) - p->c.a.angle[1]) <= 0.0f)) {
        a = func_002E2D00(func_001244D0(&p->c.a, at) - p->c.a.angle[1]);
    } else {
        a = -func_002E2D00(func_001244D0(&p->c.a, at) - p->c.a.angle[1]);
    }
    dir = a < 0x1.921fb6p+0f /* 90 degrees */ ? 0 : 1;
    kind = p->c.state[1];
    if (kind == 2 || kind == 4) {
        dir |= 2;
    }
    if (p->c.state[2] == 1 && kind != 0xA && kind != 0xB) {
        Pursuer_AddBiteDamage(p, dir);
    }
    if ((MOTION_ANIM(p) & 0xFF00) == 0x700) {
        /* on the stairs it can't fall */
        if (p->c.hp <= 0) {
            p->c.hp = 1;
        }
        Pursuer_ClearMessage(p);
        return;
    }
    if (p->c.moveSub == 9) {
        if (p->c.state[1] == 0xB && (p->c.state[4] & 0x8000)) {
            p->c.a.unkC4 = 1;
            PU(p, 0x1790, s32) = 900;
            PU(p, 0x16F5, u8) = 1;
            if (p->c.state[2] == 1) {
                func_00166150((Hewie *)gCharPartner, (Character *)p, 1);
            }
        }
        {
            s16 snd = p->c.state[2] == 1 ? 0x1D : 0x1C;

            if (VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(gProgress, 8) == 0) {
                func_00122C20(&p->c.a, snd, 7, 0, 0, NULL);
            }
        }
        Pursuer_ClearMessage(p);
        return;
    }
    if (PU(p, 0x16F7, u8) == 1 && p->c.state[1] == 1 && p->c.hp > 0) {
        if (VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(gProgress, 8) == 0) {
            func_00122C20(&p->c.a, 0x1C, 7, 0, 0, NULL);
        }
        Pursuer_ClearMessage(p);
        return;
    }
    from = p->c.state[2];
    if (from != 0xFF) {
        PU(p, 0x1761, u8) |= (1 << from) & 0xFF;
    }
    if (p->c.state[1] == 5) {
        if (p->c.moveSub == 0xA) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x22);
        } else {
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003EC960);
            PU(p, 0x1758, s32) = -1;
            VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x21);
        }
        if (p->c.a.unkC4 != 2) {
            PU(p, 0x16C8, u8) = 0;
            VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
            PU(p, 0x16C9, u8) = 6;
            PU(p, 0x16CA, u8) = 7;
        }
        p->c.unk100 = p->c.state[4];
        Pursuer_ClearMessage(p);
        return;
    }
    if (p->c.moveMode == 2) {
        func_00213270(p, 0xFF);
        PURSUER_STEP_NEXT(p) = 1;
    }
    if (p->c.state[1] == 0xA) {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003EC970);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x23);
        p->c.unk104[0] = p->c.state[4];
        Pursuer_ClearMessage(p);
        return;
    }
    if (p->c.hp <= 0) {
        if ((PursuerGroup_Find(gProgress, 0x20, p->c.a.slot) & 0xFF) != 0xFF) {
            p->c.hp = 1;
        } else {
            /* down */
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003EC980);
            PU(p, 0x1758, s32) = -1;
            VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1F);
            if (p->c.state[2] == 1) {
                func_00166150((Hewie *)gCharPartner, (Character *)p, 3);
            }
        }
    }
    if (p->c.state[1] == 3 && p->c.hp > 0 && p->c.moveMode != 4) {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003EC990);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1E);
        p->c.unk104[0] = 2;
    }
    if (p->c.moveSub == 0x11 && p->c.state[1] == 7 && p->c.unk104[0] > 0) {
        u8 *m = p->c.motion;

        if (AT(AT(m, 0x874, u8 *) + func_001F4710(m, AT(m, 0x55C, s32)) * 6, 0x4, u16) & 1) {
            u8 *mgr = gEffects;
            s32 args[2];

            p->c.unk104[0] = p->c.state[4] & 0x7FFF;
            PU(p, 0x1624, s32) = p->c.state[4] & 0x7FFF;
            func_002D6090(mgr, PU(p, 0x1628, s32), NULL);
            PU(p, 0x1628, s32) = Effect_New(mgr, 0x38, Pursuer_EffectInit);
            args[0] = (p->c.unk104[0] - 4) >> 1;
            args[1] = PU(p, 0x1624, s32);
            func_002D6090(mgr, PU(p, 0x1628, s32), args);
        }
    }
    if (p->c.moveMode == 4) {
        Pursuer_ClearMessage(p);
        return;
    }
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003EC9A0);
    PU(p, 0x1758, s32) = -1;
    if (p->c.state[2] == 0) {
        /* hit by Fiona */
        PU(p, 0x178C, s32) = 0;
        PU(p, 0x1760, u8) = 0;
        if (p->target == gCharPartner) {
            func_00297160(p);
        }
    }
    if (p->c.state[4] & 0x8000) {
        p->c.a.unkC4 = 1;
        PU(p, 0x1790, s32) = 900;
        PU(p, 0x16F5, u8) = 1;
        if (p->c.state[2] == 1) {
            func_00166150((Hewie *)gCharPartner, (Character *)p, 1);
        }
    }
    switch (p->c.state[1]) {
    case 7:
        if (p->target == gCharPartner) {
            PU(p, 0x1761, u8) |= 1;
            func_00297160(p);
        }
        p->c.unk104[0] = p->c.state[4] & 0x7FFF;
        VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x24);
        break;
    case 1:
    case 2:
    case 4:
        p->c.unk104[0] = dir & 0xFF;
        VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1E);
        break;
    }
    Pursuer_ClearMessage(p);
}

extern const PTMF D_003ED600, D_003ED610;

/* Pursuer_BehaviourStalk for a stalker that also follows Fiona from room to room: when she has left the
   room it goes after her through the door (+0x17B0, actions 0x27 walk there, 0xE open it) */
/* 0x00279350 */
void Pursuer_BehaviourFollow(Pursuer *p) {
    Progress *pr;
    const u8 *e;
    f32 near;

    if (ptmf_test(&p->c.a.state)) {
        ptmf_scall(p, &p->c.a.state);
    }
    pr = gProgress;
    e = PU(p, 0x1730, u8 *) + AT(pr, 0x7B8, u8) * 12;
    if (p->c.a.room != gCharPlayer->a.room && PU(p, 0x175C, s32) != 0x27 && PU(p, 0x175C, s32) != 0xE) {
        /* Fiona has left the room: follow */
        VCALL(p, 0xB4, void (*)(Pursuer *, Character *))(p, gCharPlayer);
        if (PURSUER_STEP_NEXT(p) == 1) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x27);
            PU(p, 0x16EF, u8) = 0;
            PURSUER_STEP_DONE(p) = 0;
        }
    }
    switch (PU(p, 0x175C, s32)) {
    case 0xE: {
        u32 ds = PursuerGroup_Fields(pr, PU(p, 0x17B0, u8), *(u8 *)&p->c.a.slot) & 0xFF;

        if (p->c.a.room == gCharPlayer->a.room) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
            VCALL(p, 0xB4, void (*)(Pursuer *, Character *))(p, gCharPlayer);
            PU(p, 0x16EF, u8) = 0;
            PURSUER_STEP_DONE(p) = 0;
        } else if (PURSUER_STEP_DONE(p) == 1 || (ds & 2)) {
            /* through the door after her */
            PU(p, 0x162C, s32) = 0;
            PURSUER_STEP_DONE(p) = 0;
            Pursuer_ClearRoute(p);
            if (PU(p, 0x16C8, u8) == 2) {
                PU(p, 0x16C8, u8) = 1;
            }
            func_002815E0(p, PU(p, 0x17B0, u8));
            return;
        }
        break;
    }
    case 0x27:
        if (p->c.a.room == gCharPlayer->a.room) {
            /* caught up with her */
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
            VCALL(p, 0xB4, void (*)(Pursuer *, Character *))(p, gCharPlayer);
            PU(p, 0x16EF, u8) = 0;
            PURSUER_STEP_DONE(p) = 0;
        } else if (PU(p, 0x16EF, u8) == 1 || !(func_00178980(pr, p->c.a.room, PU(p, 0x17B0, u8)) & 0xFF)) {
            PU(p, 0x16ED, u8) = 1;
            PU(p, 0x16EF, u8) = 0;
            PURSUER_STEP_DONE(p) = 0;
        } else if (PURSUER_STEP_DONE(p) == 1 || func_00212190(p, PU(p, 0x17B0, u8)) != 0) {
            PURSUER_STEP_DONE(p) = 0;
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0xE);
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 0x1D:
        near = 5.0f + (p->c.a.radius + p->target->a.radius);
        if (!(PU(p, 0x1588, f32) < near) || PU(p, 0x1588, f32) < 0.0f) {
            Pursuer_HoldOff(p, e);
        }
        break;
    case 6:
        if (PU(p, 0x16EF, u8) != 0) {
            Pursuer_Refollow(p);
        }
        if (PU(p, 0x162C, s32) <= 0 && PU(p, 0x1588, f32) < 100.0f) {
            VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, 0xA);
            func_00283C50(p);
            return;
        }
        if (PU(p, 0x1588, f32) <= p->c.a.radius + p->target->a.radius && !(PU(p, 0x1588, f32) < 0.0f)) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x1D);
        }
        break;
    case 5:
        if (PU(p, 0x16EF, u8) != 0) {
            Pursuer_Refollow(p);
        }
        if (PU(p, 0x162C, s32) <= 0 || !(PU(p, 0x1588, f32) <= 100.0f)) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
            PU(p, 0x162C, s32) = AT(e, 0x8, s32);
        } else if (PU(p, 0x1588, f32) <= p->c.a.radius + p->target->a.radius && !(PU(p, 0x1588, f32) < 0.0f)) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x1D);
        }
        break;
    case 0xB:
    case 0x10:
    case 0x12:
    case 0x1C:
        if (PU(p, 0x16EF, u8) == 1) {
            PURSUER_STEP_DONE(p) = 0;
            PU(p, 0x16EF, u8) = 0;
            PURSUER_STEP_NEXT(p) = 1;
            func_0029AF20(p);
        } else if (PURSUER_STEP_DONE(p) == 1) {
            Pursuer_HoldOff(p, e);
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 0x19:
    case 0x1A:
        if ((PU(p, 0x175C, s32) == 0x1A || AT(PU(p, 0x1720, u8 *) + p->c.unk104[0] * 8, 0x4, u8) != 0) &&
            !(PU(p, 0x1588, f32) <= 100.0f)) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 6);
            PU(p, 0x162C, s32) = AT(e, 0x8, s32);
            PURSUER_STEP_DONE(p) = 0;
            PU(p, 0x16EF, u8) = 0;
        }
        if (PURSUER_STEP_DONE(p) == 1 || PU(p, 0x16EF, u8) == 1) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 5);
            PU(p, 0x162C, s32) = AT(e, 0x4, s32);
            PURSUER_STEP_DONE(p) = 0;
            PU(p, 0x16EF, u8) = 0;
        }
        break;
    case 1: {
        Character *t = gCharPlayer != NULL ? gCharPlayer : p->target;
        f32 h = func_001244D0(&p->c.a, t->a.pos);
        f32 a;

        func_002140A0(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
        if (!(PU(p, 0x1588, f32) <= func_002838E0(p))) {
            Pursuer_HoldOff(p, e);
            break;
        }
        if (!(func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]) <= 0.0f)) {
            a = func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
        } else {
            a = -func_002E2D00(func_001244D0(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
        }
        if (!(a <= 0x1.921fb6p+1f * VCALL(p, 0x2EC, f32 (*)(Pursuer *))(p) / 180.0f)) {
            Pursuer_HoldOff(p, e);
            break;
        }
        if (PU(p, 0x1588, f32) < 0.0f) {
            VCALL(p, 0xB0, void (*)(Pursuer *))(p);
            if (!(func_00284440(p) & 0xFF)) {
                func_0029AF20(p);
                return;
            }
        }
        break;
    }
    }
    if (PU(p, 0x162C, s32) > 0) {
        PU(p, 0x162C, s32)--;
    }
    if (PURSUER_STEP_NEXT(p) == 1 && PU(p, 0x175C, s32) != 0x12) {
        s32 d = func_002131A0();

        if (d != -1) {
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
    if (p->c.a.room == gCharPlayer->a.room && func_00217340(p) != func_002172F0(p, gCharPlayer)) {
        /* out of reach in the same room: follow her trail */
        PU(p, 0x16C8, u8) = 1;
        VCALL(p, 0xB0, void (*)(Pursuer *))(p);
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED600);
        PU(p, 0x1758, s32) = -1;
        return;
    }
    if (PU(p, 0x1544, u8) == 0) {
        if (p->c.a.room == gCharPlayer->a.room) {
            VCALL(p, 0xB0, void (*)(Pursuer *))(p);
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ED610);
            PU(p, 0x1758, s32) = -1;
            PU(p, 0x162C, s32) = 0;
        }
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

extern PTMF D_0045B388;   /* the move along the stairs */

/* the time Hewie keeps a stalker busy (his "stay" level, +0xF35CC) */
static s32 Pursuer_HewieDelay(Character *h, s32 base) {
    return (AT(h, 0xF35CC, s16) + 1) * 15 + base;
}

/* plan from where the pursuer is to its goal room; 0 when there's no way and it can't wait */
static s32 Pursuer_PlanOn(Pursuer *p) {
    return func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) > 0 || (func_0027CA00(p) & 0xFF);
}

/* a door or stair move still in progress when the pursuer leaves the screen: finish it */
static void Pursuer_FinishCrossing(Pursuer *p) {
    if (p->c.moveMode == 2) {
        Progress *pr = gProgress;

        if ((Progress_CurRoomFlag(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1) {
            /* finish going through the door */
            if (PU(p, 0x175C, s32) == 0xF) {
                func_00212D30(p, (u8)p->c.unk100);
            } else if ((func_00178980(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1) {
                func_00212CA0(p, (u8)p->c.unk100);
            } else {
                func_00212D30(p, (u8)p->c.unk100);
            }
            AT(p, 0x2B, u8) = 0;
            p->c.a.unk2D = 0;
            p->c.a.navMask = VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
            if (func_00214A90(p, p->c.a.navTri) != 0) {
                VObject *doors = gDoors;
                f32 v[4] __attribute__((aligned(16)));
                s8 side = VCALL(doors, 0x18, s32 (*)(VObject *, u32, f32 *))(doors, (u8)p->c.unk100, p->c.a.pos);

                if (side == 1) {
                    side = 2;
                }
                p->c.a.navTri = VCALL(doors, 0x14, s32 (*)(VObject *, u32, s32, f32 *, f32 *, s32))(doors,
                    (u8)p->c.unk100, side, p->c.a.pos, v, 1);
                p->c.a.angle[1] = v[1];
                sceVu0UnitMatrix((void *)((u8 *)p + 0x60));
                sceVu0RotMatrixY((void *)((u8 *)p + 0x60), (void *)((u8 *)p + 0x60), v[1]);
            }
        }
        p->c.moveMode = 0;
    }
    if (p->c.moveMode == 3) {
        /* finish the stairs at the end it is nearer */
        f32 ofs[4] __attribute__((aligned(16)));
        f32 y;

        RoomSlots_Leave(gProgress, (u8)p->c.unk100, *(u8 *)&p->c.a.slot);
        y = p->c.a.pos[1];
        if (!(y <= PU(p, 0x15D4, f32)) || (y == PU(p, 0x15D4, f32) && y < PU(p, 0x15B4, f32))) {
            VCALL(p, 0x9C, void (*)(Pursuer *, s32, f32 *))(p, 3, ofs);
            p->c.a.navTri = func_00123710(p, p->c.unk100, 0, ofs, p->c.a.pos);
        } else {
            VCALL(p, 0x9C, void (*)(Pursuer *, s32, f32 *))(p, 2, ofs);
            p->c.a.navTri = func_00123710(p, p->c.unk100, 1, ofs, p->c.a.pos);
        }
    }
    if (p->c.unkE0 == 1) {
        VCALL(p, 0x90, void (*)(Pursuer *))(p);
    }
}

/* forget the room's action, attack and stagger state */
static void Pursuer_ResetForRoom(Pursuer *p) {
    s32 i;

    if (PU(p, 0x16F4, u8) != 0) {
        PU(p, 0x16C8, u8) = 3;
        VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
        PU(p, 0x16F4, u8) = 0;
    }
    for (i = 0; i < 8; i++) {
        PU(p, 0x1624 + i * 4, s32) = 0;
    }
    for (i = 0; i < 4; i++) {
        PU(p, 0x1650 + i * 4, s32) = 0;
    }
    p->c.unk104[0] = -1;
    PU(p, 0x1761, u8) = 0;
    PU(p, 0x1760, u8) = 0;
    PU(p, 0x178C, s32) = 0;
    PU(p, 0x1764, s32) = -1;
    p->c.unk14D0 = 0;
    func_001F6E10(p->c.motion);
    PU(p, 0x16F5, u8) = 0;
    PU(p, 0x16F8, u8) = 0;
    PU(p, 0x16F7, u8) = 0;
    PU(p, 0x1794, s32) = 0;
    PU(p, 0x1798, s32) = 0;
    PU(p, 0x16F3, u8) = 0;
}

/* off screen and the next room isn't the one it's in: carry on with the plan */
static void Pursuer_LeaveOffScreen(Pursuer *p, s32 next, u32 door) {
    if (next == p->c.a.room) {
        p->c.unk1530 = 0;
        p->c.unk1538 = 0;
        p->c.unk1534 = 0;
        AT(p, 0x2A, u8) = 0;
    } else if (PU(p, 0x16C8, u8) == 3) {
        if (p->c.unk1388 >= p->c.unk1384 && PU(p, 0x17B4, s32) == 0) {
            Pursuer_SetMove(p, &D_0045B358);
        }
    } else if (PU(p, 0x16C8, u8) == 0) {
        VCALL(p, 0xB8, void (*)(Pursuer *, u32))(p, door);
        if (func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) == -1) {
            func_0029AC50(p);
        }
    }
}

/* walk to the exit +0x17B0 of the plan (or along the stairs) */
static void Pursuer_HeadForExit(Pursuer *p, VObject *rooms, u32 door) {
    if (p->c.moveMode == 3) {
        Pursuer_SetMove(p, &D_0045B388);
    } else {
        PU(p, 0x15A4, s32) = VCALL(rooms, 0x34, s32 (*)(VObject *, u32, f32 *))(rooms, PU(p, 0x17B0, u8), (f32 *)((u8 *)p + 0x15B0));
        p->c.a.navMask = VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
        AT(p->c.pathReq, 0x40, u32) = p->c.a.navMask;
        if ((func_00212360(p) & 0xFF) != 1) {
            Pursuer_SetMove(p, &D_0045B388);
        } else {
            if (p->c.moveMode != 4 && PU(p, 0x16C8, u8) == 0) {
                func_00212240(p, door);
            }
            Pursuer_SetMove(p, &D_0045B370);
        }
    }
    p->c.unk14C0 = VCALL(rooms, 0x10, s32 (*)(VObject *, s32, u32))(rooms, p->c.a.room, PU(p, 0x17B0, u8));
}

/* the exit for the plan's next room +0x138C */
static void Pursuer_NextExit(Pursuer *p, VObject *rooms) {
    PU(p, 0x17B0, u8) = VCALL(rooms, 0x3C, s32 (*)(VObject *, u32, s32))(rooms, PU(p, 0x138C, u16), p->c.a.room);
}

/* the shared mode cases (1..4) of picking the next exit; 0 when it stays put instead */
static s32 Pursuer_PickExit(Pursuer *p, VObject *rooms) {
    switch (PU(p, 0x16C8, u8)) {
    case 2:
        if (PU(p, 0x1620, u8) < PU(p, 0x1621, u8)) {
            Pursuer_SetMove(p, &D_0045B3A0);
            return 0;
        }
        if (!(PU(p, 0x1621, u8) < 8)) {
            Pursuer_ClearRoute(p);
            func_0027EAF0(p);
            Pursuer_SetMove(p, &D_0045B3A0);
            return 0;
        }
        break;
    case 1:
        if (!(func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) > 0)) {
            PU(p, 0x16F4, u8) = 1;
            if (!(func_0027CA00(p) & 0xFF)) {
                Pursuer_SetMove(p, &D_0045B3A0);
                return 0;
            }
        }
        Pursuer_NextExit(p, rooms);
        break;
    case 3:
        if (PU(p, 0x1620, u8) < PU(p, 0x1621, u8) || !Pursuer_PlanOn(p)) {
            Pursuer_SetMove(p, &D_0045B3A0);
            return 0;
        }
        Pursuer_NextExit(p, rooms);
        break;
    case 4: {
        s32 room = p->c.a.room;
        s32 *e;

        for (e = VCALL(p, 0x314, s32 *(*)(Pursuer *))(p); *e != -1; e += 2) {
            if (room == *e) {
                Pursuer_SetMove(p, &D_0045B3A0);
                return 0;
            }
        }
        if (!Pursuer_PlanOn(p)) {
            Pursuer_SetMove(p, &D_0045B3A0);
            PU(p, 0x16F4, u8) = 1;
            return 0;
        }
        Pursuer_NextExit(p, rooms);
        break;
    }
    }
    return 1;
}

/* leave the screen through door `door` while the progress byte +0x1FBEC1 is set (see
   Pursuer_LeaveScreen): finish a door or stair move in progress, reset the per-room state, and set
   off-screen moving (or, still in view, walking) to the next exit of its plan */
/* 0x0027B810 */
void Pursuer_LeaveScreenEnding(Pursuer *p, u32 door) {
    Character *h;
    VObject *rooms;

    Pursuer_FinishCrossing(p);
    h = gCharPartner;
    if (AT(h, 0xF3581, u8) != 0) {
        if (p->c.hp > 0) {
            switch (p->c.unk104[0]) {
            case 14:
            case 15:
                PU(p, 0x1664, s32) = Pursuer_HewieDelay(h, 60);
                break;
            case 10:
            case 11:
            case 12:
            case 13:
                PU(p, 0x1664, s32) = Pursuer_HewieDelay(h, 30);
                break;
            default:
                if (PU(p, 0x175C, s32) != 0x1F) {
                    PU(p, 0x1664, s32) = Pursuer_HewieDelay(h, 30);
                }
                break;
            }
        } else if (p->c.a.unkC4 != 2) {
            VCALL(p, 0x2CC, void (*)(Pursuer *))(p);
            PU(p, 0x1790, s32) = 0;
            PU(p, 0x17B4, s32) = Pursuer_HewieDelay(h, 60);
            p->c.a.unkC4 = 2;
        }
    } else if (p->c.moveSub == 0x11) {
        PU(p, 0x1664, s32) = p->c.unk104[0] * 60;
        p->c.unk104[0] = -1;
    }
    Pursuer_ResetForRoom(p);
    rooms = gRooms;
    if (func_00217510(p) == 0) {
        Pursuer_LeaveOffScreen(p, VCALL(rooms, 0x18, s32 (*)(VObject *, s32, u32))(rooms,
            VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress), door), door);
        return;
    }
    p->c.unk1530 = 0;
    p->c.unk1538 = 0;
    p->c.unk1534 = 0;
    VCALL(p, 0xE4, void (*)(Pursuer *, u32))(p, p->c.door);
    VCALL(p, 0x154, void (*)(Pursuer *))(p);
    AT(p, 0x29, u8) = 1;
    AT(p, 0x2A, u8) = 1;
    AT(p, 0x2B, u8) = 0;
    p->c.a.unk2D = 0;
    PU(p, 0x1544, u8) = 0;
    PU(p, 0x1545, u8) = 0;
    if (p->c.a.unkC4 != 2 && p->target == gCharPartner) {
        PU(p, 0x16C9, u8) = 2;
        PU(p, 0x16CA, u8) = 3;
        PU(p, 0x16C8, u8) = 1;
        VCALL(p, 0xBC, void (*)(Pursuer *, u32))(p, door);
        Pursuer_ClearRoute(p);
    } else if (PU(p, 0x16C8, u8) != 0) {
        switch (PU(p, 0x16C9, u8)) {
        case 3:
            PU(p, 0x16C9, u8) = 2;
            PU(p, 0x16CA, u8) = 3;
            break;
        case 1:
            if (func_00217260(p) != 0 || PU(p, 0x1594, s32) == -1) {
                PU(p, 0x16C9, u8) = 2;
                PU(p, 0x16CA, u8) = 3;
                PU(p, 0x16C8, u8) = 1;
                VCALL(p, 0xBC, void (*)(Pursuer *, u32))(p, door);
                Pursuer_ClearRoute(p);
            }
            break;
        case 4:
        case 5:
            if (func_00217260(p) != 0 || PU(p, 0x1594, s32) == -1) {
                PU(p, 0x16C9, u8) = 2;
                PU(p, 0x16CA, u8) = 3;
            }
            break;
        }
    }
    p->c.unk124 = p->c.unk128;
    if (PU(p, 0x16C8, u8) == 0) {
        if ((door & 0xFF) == 0xFF) {
            PU(p, 0x17B4, s32) = 150;
            Pursuer_SetMove(p, &D_0045B3A0);
            PU(p, 0x16F4, u8) = 1;
            return;
        }
        if (!(func_0027CA00(p) & 0xFF)) {
            PU(p, 0x17B4, s32) = 150;
            Pursuer_SetMove(p, &D_0045B3A0);
            if (p->c.a.room != gCharPlayer->a.room) {
                PU(p, 0x16F4, u8) = 1;
            }
            return;
        }
        Pursuer_NextExit(p, rooms);
    } else if (!Pursuer_PickExit(p, rooms)) {
        return;
    }
    Pursuer_HeadForExit(p, rooms, door);
}

/* how long to wait on arriving (+0x1744: {frames, cumulative percent} pairs, ascending) */
static s32 Pursuer_RandomWait(Pursuer *p) {
    u8 *t = PU(p, 0x1744, u8 *);
    f32 r;
    u32 i;

    if (t == NULL) {
        return 0;
    }
    r = 100.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
    for (i = 0;; i = (i + 1) & 0xFF) {
        u8 *e = t + (i & 0xFF) * 8;

        if (!(AT(e, 0x4, f32) <= 100.0f) || AT(e, 0x4, f32) < 0.0f) {
            return 0;
        }
        if (!(AT(e, 0x4, f32) < r)) {
            return AT(e, 0x0, s32);
        }
    }
}

/* leave the screen through door `door` (Pursuer_LeaveScreenEnding while the progress byte +0x1FBEC1 is set):
   finish a crossing, react to Hewie still hanging on, reset the per-room state, and set off for
   the next exit of the plan, after a random wait when it was chasing */
/* 0x00282010 */
void Pursuer_LeaveScreen(Pursuer *p, u32 door) {
    Character *h;
    VObject *rooms;

    if ((u32)p->c.a.slot >= 3) {
        VCALL(p, 0x14C, void (*)(Pursuer *))(p);
        return;
    }
    if (AT(gProgress, 0x1FBEC1, u8) != 0) {
        VCALL(p, 0x144, void (*)(Pursuer *))(p);
        return;
    }
    Pursuer_FinishCrossing(p);
    h = gCharPartner;
    if (AT(h, 0xF3581, u8) != 0) {
        if (p->c.hp > 0) {
            /* Hewie still has it: stay in the bitten pose a while */
            switch (p->c.unk104[0]) {
            case 10:
            case 11:
                PU(p, 0x1664, s32) = Pursuer_HewieDelay(h, 30);
                Pursuer_PlayAnimBlend(p, 0x1700);
                break;
            case 12:
            case 13:
                PU(p, 0x1664, s32) = Pursuer_HewieDelay(h, 30);
                Pursuer_PlayAnimBlend(p, 0x1703);
                break;
            case 14:
                PU(p, 0x1664, s32) = Pursuer_HewieDelay(h, 60);
                Pursuer_PlayAnimBlend(p, 0x1706);
                break;
            case 15:
                PU(p, 0x1664, s32) = Pursuer_HewieDelay(h, 60);
                Pursuer_PlayAnimBlend(p, 0x1708);
                break;
            default:
                if (PU(p, 0x175C, s32) != 0x1F) {
                    PU(p, 0x1664, s32) = Pursuer_HewieDelay(h, 30);
                }
                break;
            }
            func_00297C60(p);
        } else if (p->c.a.unkC4 != 2) {
            VCALL(p, 0x2CC, void (*)(Pursuer *))(p);
            PU(p, 0x1790, s32) = 0;
            PU(p, 0x17B4, s32) = Pursuer_HewieDelay(h, 60);
            p->c.a.unkC4 = 2;
        }
    } else if (p->c.moveSub == 0x11) {
        PU(p, 0x1664, s32) = PU(p, 0x1624, s32) * 30;
        p->c.unk104[0] = -1;
    }
    Pursuer_ResetForRoom(p);
    rooms = gRooms;
    if (func_00217510(p) == 0) {
        Pursuer_LeaveOffScreen(p, VCALL(rooms, 0x18, s32 (*)(VObject *, s32, u32))(rooms, gCharPlayer->a.room, door), door);
        return;
    }
    p->c.unk1530 = 0;
    p->c.unk1538 = 0;
    p->c.unk1534 = 0;
    VCALL(p, 0xE4, void (*)(Pursuer *, u32))(p, p->c.door);
    VCALL(p, 0x154, void (*)(Pursuer *))(p);
    AT(p, 0x29, u8) = 1;
    AT(p, 0x2A, u8) = 1;
    AT(p, 0x2B, u8) = 0;
    p->c.a.unk2D = 0;
    PU(p, 0x1544, u8) = 0;
    PU(p, 0x1545, u8) = 0;
    if (PU(p, 0x16C8, u8) != 0) {
        switch (PU(p, 0x16C9, u8)) {
        case 3:
            PU(p, 0x16C9, u8) = 2;
            PU(p, 0x16CA, u8) = 3;
            break;
        case 5:
            if (func_00217260(p) != 0 || PU(p, 0x1594, s32) == -1) {
                PU(p, 0x16C9, u8) = 2;
                PU(p, 0x16CA, u8) = 3;
                PU(p, 0x16C8, u8) = 1;
                VCALL(p, 0xB8, void (*)(Pursuer *, u32))(p, door);
                Pursuer_ClearRoute(p);
                PU(p, 0x17B4, s32) = Pursuer_RandomWait(p);
            }
            break;
        case 1:
        case 4:
            if (func_00217260(p) != 0 || PU(p, 0x1594, s32) == -1) {
                PU(p, 0x16C9, u8) = 2;
                PU(p, 0x16CA, u8) = 3;
            }
            break;
        }
    }
    p->c.unk124 = p->c.unk128;
    if (PU(p, 0x16C8, u8) == 0) {
        u32 link;

        if ((door & 0xFF) == 0xFF) {
            PU(p, 0x17B4, s32) = 150;
            Pursuer_SetMove(p, &D_0045B3A0);
            PU(p, 0x16F4, u8) = 1;
            return;
        }
        /* chasing: through this door, whatever the plan said about it */
        PU(p, 0x17B0, u8) = door;
        link = VCALL(rooms, 0x10, s32 (*)(VObject *, s32, u32))(rooms, p->c.a.room, door) & 0xFFFF;
        p->c.unk148C[link >> 5] &= ~(1 << (link & 0x1F));
        VCALL(p, 0xB8, void (*)(Pursuer *, u32))(p, door);
        if (!(func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) > 0)) {
            PU(p, 0x17B4, s32) = 150;
            Pursuer_SetMove(p, &D_0045B3A0);
            PU(p, 0x16F4, u8) = 1;
            return;
        }
        if (link != PU(p, 0x138C, u16)) {
            if ((VCALL(p, 0xEC, s32 (*)(Pursuer *, u32))(p, door) & 0xFF) == 1) {
                s32 a = VCALL(rooms, 0x1C, s32 (*)(VObject *, u32, s32))(rooms, link, p->c.a.room);

                if (a != VCALL(rooms, 0x1C, s32 (*)(VObject *, u32, s32))(rooms, PU(p, 0x138C, u16), p->c.a.room)) {
                    PU(p, 0x17B4, s32) = 150;
                    Pursuer_SetMove(p, &D_0045B3A0);
                    PU(p, 0x16F4, u8) = 1;
                    return;
                }
                PU(p, 0x138C, u16) = link;
            } else {
                Pursuer_NextExit(p, rooms);
            }
        }
        PU(p, 0x17B4, s32) = Pursuer_RandomWait(p);
    } else if (!Pursuer_PickExit(p, rooms)) {
        return;
    }
    Pursuer_HeadForExit(p, rooms, door);
}

/* ---- the base functions placed with the stalkers' code (0x127A40..) ---- */

extern u8 D_003EC3E0[];   /* the action table: 0x1C bytes each */

/* vtable +0x114: start action `kind` (0x1000 set: from the pursuer's own table +0x1714): its
   state, action id +0x175C, move mode and sub, sense mode +0x15C0 and look mode +0x1710 */
/* 0x00127A40 */
void Pursuer_StartAction(Pursuer *p, u32 kind) {
    u8 *e;

    if (kind & 0x1000) {
        e = PU(p, 0x1714, u8 *) + (kind & ~0x1000) * 0x1C;
    } else {
        e = D_003EC3E0 + kind * 0x1C;
    }
    Actor_SetState(&p->c.a, (const PTMF *)e);
    PU(p, 0x175C, s32) = AT(e, 0xC, s32);
    p->c.moveMode = AT(e, 0x10, s32);
    p->c.moveSub = AT(e, 0x14, s32);
    PU(p, 0x15C0, u8) = AT(e, 0x18, u8);
    PU(p, 0x1710, u8) = AT(e, 0x19, u8);
    PU(p, 0x15A0, u8) = 1;
}

/* 0x00127B20 */
void Pursuer_StartActionNext(void *p, u32 id) {
    u8 *e;

    FLD(p, 0x1758, u32) = id;
    if (id & 0x1000) {
        e = FLD(p, 0x1714, u8 *) + (id & 0xFFF) * 28;
    } else {
        e = D_003EC3E0 + id * 28;
    }
    FLD(p, 0x175C, s32) = FLD(e, 0xC, s32);
    FLD(p, 0xF8, s32) = FLD(e, 0x10, s32);
    FLD(p, 0xFC, s32) = FLD(e, 0x14, s32);
}

/* vtable +0x190: nothing */
/* 0x00127B80 */
void Pursuer_Nothing190(Pursuer *p) {
}

/* 0x00127B90 */
void Pursuer_Caught(void *p) {
    FLD(p, 0x1660, s32) = FLD(p, 0x16D4, s32);
}

/* 0x00127BA0 */
void Pursuer_NoGiveUp(void *p) {
    FLD(p, 0x1664, s32) = FLD(p, 0x16D0, s32);
}

/* vtable +0x2D0 / +0x2D4 */
/* 0x00127BB0 */
s32 Pursuer_Get2D0(Pursuer *p) {
    return PU(p, 0x16E0, s32);
}

/* 0x00127BC0 */
s32 Pursuer_Get2D4(Pursuer *p) {
    return PU(p, 0x16E4, s32);
}

/* 0x00127BD0 */
f32 Pursuer_FrightSeen(void) {
    return 10.0f;
}

/* 0x00127BE0 */
f32 Pursuer_FrightAttack(void) {
    return 5.0f;
}

/* 0x00127BF0 */
f32 Pursuer_ThreatAmount(void) {
    return 20.0f;
}

/* vtable +0x318 */
/* 0x00127C00 */
s32 Pursuer_Get318(Pursuer *p) {
    return 0;
}

/* 0x00127C10 */
void Pursuer_SetRage(void *p, s32 on) {
    FLD(p, 0x16B8, s32) = on ? 2 : 0;
}

/* 0x00127C30 */
s32 Pursuer_IsBusy(void *p) {
    return FLD(p, 0x20, s32) == 2;
}

u8 *func_0012BFB0(Pursuer *p) {
    if (FLD(gProgress, 0x30, u32) & 0x8000) {
        return D_003AF230;
    }
    return D_003AF1F0;
}

/* ---- the room character of id 8 (vtable 0x46FF80; built by Kind08_ctor): a Pursuer that
 * mostly keeps the Pursuer's own behaviour ---- */

extern void *D_0046FF80[];
extern const PTMF D_00419E50;   /* its behaviour after a reset */

/* vtable +0x8: destructor */
/* 0x002ECB80 */
Pursuer *Kind08_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = D_0046FF80;
        if (p != NULL) {
            Pursuer_DestroyBase(p);
        }
        if ((s16)flags > 0) {
            func_00124E40(&p->c.a);
        }
    }
    return p;
}

void *func_002ECC90(void) {
    return D_00419DD0;
}

void *func_002ECCA0(void) {
    return D_00419E10;
}

/* vtable +0xF4: setup (the Pursuer's) */
void func_002ECCB0(Pursuer *p) {
    Pursuer_Setup(p);
}

/* vtable +0x38: on screen, func_00124890(-1) */
/* 0x002ECCC0 */
void Kind08_ShowUp(Pursuer *p) {
    if (func_00217510(p) != 0) {
        func_00124890(&p->c.a, -1);
    }
}

/* vtable +0x84: a pending reset (state 5): on screen, +0x8C, its behaviour D_00419E50 and the
 * next one cleared (+0x1758), then +0x114(1); the state is cleared either way */
/* 0x002ECD10 */
void Kind08_EventState(Pursuer *p) {
    if (p->c.state[0] != 5) {
        return;
    }
    if (func_00217510(p) & 0xFF) {
        VCALL(p, 0x8C, void (*)(Pursuer *))(p);
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_00419E50);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
    }
    p->c.state[0] = 0;
    p->c.state[1] = 0;
}

/* vtable +0x110: a joint action pending for it is cancelled; -1 */
s32 func_002ECDE0(Pursuer *p) {
    Progress *pr = gProgress;

    if ((func_00177870(pr, *(u8 *)&p->c.a.slot) & 0xFF) == 1) {
        SlotCmd_Cancel(pr, *(u8 *)&p->c.a.slot);
    }
    return -1;
}

/* +0x8 destructor (0x471290 -> Pursuer 0x46D810 -> NPC 0x46C220 -> Character); the model freed
 * for slots 3..5 */
/* 0x00308EC0 */
Character *Kind09_dtor(Character *c, s32 flags) {
    if (c != NULL) {
        c->a.vtbl = D_00471290;
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
            func_00124E40(&c->a);
        }
    }
    return c;
}

/* 0x00308FD0 */
void *Kind09_MotionFiles(void) {
    return D_004223E0;
}

/* a state: +0x114 5, the state D_00422348 at +0x174C, +0x1758 -1, then +0x260 */
/* 0x00308FF0 */
void Kind09_Behaviour25C(Character *c) {
    VCALL(c, 0x114, void (*)(Character *, s32))(c, 5);
    ptmf_set(&AT(c, 0x174C, PTMF), &D_00422348);
    AT(c, 0x1758, s32) = -1;
    VCALL(c, 0x260, void (*)(Character *))(c);
}

/* each frame: its state (+0xA0), then back to +0x114 5 unless already (+0x175C) */
/* 0x00309080 */
void Kind09_BehaviourSearch(Character *c) {
    if (ptmf_test(&AT(c, 0xA0, PTMF))) {
        ptmf_scall(c, &AT(c, 0xA0, PTMF));
    }
    if (AT(c, 0x175C, s32) != 5) {
        VCALL(c, 0x114, void (*)(Character *, s32))(c, 5);
    }
}

/* ---- character kind 14 (vtable D_004723B0, the pursuer base with three of its own) ---- */

extern const PTMF D_00429840;

/* +0x38 a frame: back on the mesh (func_00124890) when func_00217510 says so */
/* 0x00312EA0 */
void Kind14_ShowUp(Pursuer *p) {
    if (func_00217510(p) != 0) {
        func_00124890(&p->c.a, -1);
    }
}

/* +0x84 a request of kind 5 (+0x14E8): when func_00217510 allows it, +0x8C, its state
 * (+0x174C) D_00429840 with no target (+0x1758 -1), +0x114(1); the request cleared either way */
/* 0x00312EF0 */
void Kind14_EventState(Pursuer *p) {
    if (PU(p, 0x14E8, s32) != 5) {
        return;
    }
    if ((func_00217510(p) & 0xFF) != 0) {
        VCALL(p, 0x8C, void (*)(Pursuer *))(p);
        ptmf_set(&PU(p, 0x174C, PTMF), &D_00429840);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
    }
    PU(p, 0x14E8, s32) = 0;
    PU(p, 0x14EC, s32) = 0;
}

/* +0x110 let its progress slot go (SlotCmd_Cancel) if it holds one (func_00177870); -1 */
/* 0x00312FC0 */
s32 Kind14_GrabOrder(Pursuer *p) {
    Progress *pr = gProgress;

    if ((func_00177870(pr, *(u8 *)&p->c.a.slot) & 0xFF) == 1) {
        SlotCmd_Cancel(pr, *(u8 *)&p->c.a.slot);
    }
    return -1;
}

/* ---- character kind 0x10 (vtable D_004728A0, the pursuer base with four of its own) ---- */

extern void *D_004728A0[];
extern const PTMF D_00429CD0;

/* +0x8 destructor */
/* 0x00315B30 */
Pursuer *Kind16_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = D_004728A0;
        Pursuer_DestroyBase(p);
        if ((s16)flags > 0) {
            func_00124E40(&p->c.a);
        }
    }
    return p;
}

/* 0x00315C40 */
void *Kind16_ModelFiles(void) {
    return D_00429C50;
}

/* 0x00315C50 */
void *Kind16_MotionFiles(void) {
    return D_00429C90;
}

/* +0x38 a frame: back on the mesh (func_00124890) when func_00217510 says so */
/* 0x00315C70 */
void Kind16_ShowUp(Pursuer *p) {
    if (func_00217510(p) != 0) {
        func_00124890(&p->c.a, -1);
    }
}

/* +0x84 a request of kind 5 (+0x14E8): when func_00217510 allows it, +0x8C, its state
 * (+0x174C) D_00429CD0 with no target (+0x1758 -1), +0x114(1); the request cleared either way */
/* 0x00315CC0 */
void Kind16_EventState(Pursuer *p) {
    if (PU(p, 0x14E8, s32) != 5) {
        return;
    }
    if ((func_00217510(p) & 0xFF) != 0) {
        VCALL(p, 0x8C, void (*)(Pursuer *))(p);
        ptmf_set(&PU(p, 0x174C, PTMF), &D_00429CD0);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
    }
    PU(p, 0x14E8, s32) = 0;
    PU(p, 0x14EC, s32) = 0;
}

/* +0x110 let its progress slot go (SlotCmd_Cancel) if it holds one (func_00177870); -1 */
/* 0x00315D90 */
s32 Kind16_GrabOrder(Pursuer *p) {
    Progress *pr = gProgress;

    if ((func_00177870(pr, *(u8 *)&p->c.a.slot) & 0xFF) == 1) {
        SlotCmd_Cancel(pr, *(u8 *)&p->c.a.slot);
    }
    return -1;
}

/* ---- character kind 0x11 (vtable D_00472C30, the pursuer base with four of its own) ---- */

extern void *D_00472C30[];
extern const PTMF D_00429D60;

/* +0x8 destructor */
/* 0x00316AB0 */
Pursuer *Kind17_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = D_00472C30;
        Pursuer_DestroyBase(p);
        if ((s16)flags > 0) {
            func_00124E40(&p->c.a);
        }
    }
    return p;
}

/* 0x00316BC0 */
void *Kind17_ModelFiles(void) {
    return D_00429CE0;
}

/* 0x00316BD0 */
void *Kind17_MotionFiles(void) {
    return D_00429D20;
}

/* +0x38 a frame: back on the mesh (func_00124890) when func_00217510 says so */
/* 0x00316BF0 */
void Kind17_ShowUp(Pursuer *p) {
    if (func_00217510(p) != 0) {
        func_00124890(&p->c.a, -1);
    }
}

/* +0x84 a request of kind 5 (+0x14E8): when func_00217510 allows it, +0x8C, its state
 * (+0x174C) D_00429D60 with no target (+0x1758 -1), +0x114(1); the request cleared either way */
/* 0x00316C40 */
void Kind17_EventState(Pursuer *p) {
    if (PU(p, 0x14E8, s32) != 5) {
        return;
    }
    if ((func_00217510(p) & 0xFF) != 0) {
        VCALL(p, 0x8C, void (*)(Pursuer *))(p);
        ptmf_set(&PU(p, 0x174C, PTMF), &D_00429D60);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
    }
    PU(p, 0x14E8, s32) = 0;
    PU(p, 0x14EC, s32) = 0;
}

/* +0x110 let its progress slot go (SlotCmd_Cancel) if it holds one (func_00177870); -1 */
/* 0x00316D10 */
s32 Kind17_GrabOrder(Pursuer *p) {
    Progress *pr = gProgress;

    if ((func_00177870(pr, *(u8 *)&p->c.a.slot) & 0xFF) == 1) {
        SlotCmd_Cancel(pr, *(u8 *)&p->c.a.slot);
    }
    return -1;
}

/* destructor (vtable D_00472F60) */
void *func_00316D80(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00472F60;
        AT(o, 0x0, void **) = D_0046D730;
        if ((s16)flags > 0) {
            func_002672E0(o);
        }
    }
    return o;
}

/* 0x0031E5C0 */
Character *Kind19_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, D_004734A0); }

/* 0x0031E6D0 */
void *Kind19_ModelFiles(void) {
    return D_0042A0F0;
}

/* 0x0031E6E0 */
void *Kind19_MotionFiles(void) {
    return D_0042A130;
}

/* 0x0031E700 */
void Kind19_ShowUp(Pursuer *p) { creature_inplay(p); }

/* 0x0031E750 */
void Kind19_EventState(Pursuer *p) { creature_act5(p, &D_0042A170); }

/* 0x0031E820 */
s32 Kind19_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

u8 *func_00320150(Pursuer *p) {
    return b5_prog_flag8000() ? D_0042A320 : D_0042A2E0;
}

/* 0x00321640 */
Character *Kind24_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, D_00473CD0); }

/* 0x00321750 */
void *Kind24_ModelFiles(void) {
    return D_0042C380;
}

/* 0x00321760 */
void *Kind24_MotionFiles(void) {
    return D_0042C3C0;
}

/* 0x00321780 */
void Kind24_ShowUp(Pursuer *p) { creature_inplay(p); }

/* 0x003217D0 */
void Kind24_EventState(Pursuer *p) { creature_act5(p, &D_0042C400); }

/* 0x003218A0 */
s32 Kind24_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

/* 0x0032C830 */
void Kind21_ShowUp(Pursuer *p) { creature_inplay(p); }

/* 0x0032C880 */
void Kind21_EventState(Pursuer *p) { creature_act5(p, &D_0042C8F0); }

/* 0x0032C950 */
s32 Kind21_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

/* 0x0032C9C0 */
Character *Kind20_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, D_00474890); }

/* 0x0032CAD0 */
void *Kind20_ModelFiles(void) {
    return D_0042C900;
}

/* 0x0032CAE0 */
void *Kind20_MotionFiles(void) {
    return D_0042C940;
}

/* 0x0032CB00 */
void Kind20_ShowUp(Pursuer *p) { creature_inplay(p); }

/* 0x0032CB50 */
void Kind20_EventState(Pursuer *p) { creature_act5(p, &D_0042C980); }

/* 0x0032CC20 */
s32 Kind20_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

u8 *func_00331200(Pursuer *p) {
    return b5_prog_flag8000() ? D_0042E4A0 : D_0042E460;
}

/* 0x0033ABA0 */
Character *Kind29_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, D_00476120); }

/* 0x0033ACB0 */
void *Kind29_ModelFiles(void) {
    return D_00430820;
}

/* 0x0033ACC0 */
void *Kind29_MotionFiles(void) {
    return D_00430860;
}

/* 0x0033ACE0 */
void Kind29_ShowUp(Pursuer *p) { creature_inplay(p); }

/* 0x0033AD30 */
void Kind29_EventState(Pursuer *p) { creature_act5(p, &D_004308A0); }

/* 0x0033AE00 */
s32 Kind29_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

/* 0x00345100 */
Character *Kind33_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, D_00477790); }

/* 0x00345210 */
void *Kind33_ModelFiles(void) {
    return D_0043B5B0;
}

/* 0x00345220 */
void *Kind33_MotionFiles(void) {
    return D_0043B5F0;
}

/* 0x00345240 */
void Kind33_ShowUp(Pursuer *p) { creature_inplay(p); }

/* 0x00345290 */
void Kind33_EventState(Pursuer *p) { creature_act5(p, &D_0043B630); }

/* 0x00345360 */
s32 Kind33_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

u8 *func_00347290(Pursuer *p) {
    return (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) ? D_0043B6C0 : D_0043B680;
}

u8 *func_00348620(Pursuer *p) {
    return (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) ? D_0043CDA0 : D_0043CD60;
}

u8 *func_003495B0(Pursuer *p) {
    return (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) ? D_0043DC30 : D_0043DBF0;
}

/* 0x0034B7B0 */
Character *Kind37_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, D_00478770); }

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

u8 *func_0034D980(Pursuer *p) {
    return (*(u32 *)((u8 *)gProgress + 0x30) & 0x8000) ? D_00441830 : D_004417F0;
}

/* destructor (vtable D_00479FF0) */
void *func_0036A4D0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_00479FF0;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* D_00479FB0 effect message: none sets +0x34 (done); else +0x2C its word 0, and below 2 in word 1
 * +0x30 30 */
void func_0036A530(u8 *o, s32 *m) {
    if (m == NULL) {
        AT(o, 0x34, u8) = 1;
        return;
    }
    AT(o, 0x2C, s32) = m[0];
    if (m[1] < 2) {
        AT(o, 0x30, s32) = 30;
    }
}

void func_0036A580(void) {
}

/* D_00479FB0's update: while the pursuer is in state 4 / action 0x11 (and not stopped, +0x34,
 * nor its delay +0x30 run out) each of its ten timers (+0x4..) counts down; at most one at 0 a
 * frame restarts (10..41) and lets a strand (kind = the pursuer's, +0x2C) drip */
s32 func_0036A590(u8 *o) {
    VObject *rnd;
    u8 *mgr;
    u8 spawned = 0;
    s32 i;

    if (AT(gCharPursuer, 0xF8, s32) != 4 || AT(gCharPursuer, 0xFC, s32) != 0x11 || AT(o, 0x34, u8) == 1) {
        return 0;
    }
    if (AT(o, 0x30, s32) >= 0) {
        AT(o, 0x30, s32) = AT(o, 0x30, s32) - 1;
        if (AT(o, 0x30, s32) <= 0) {
            return 0;
        }
    }
    rnd = gRandom;
    mgr = gEffects;
    for (i = 0; i < 10; i++) {
        s32 *t = &AT(o, 0x4 + i * 4, s32);

        if (*t != 0) {
            (*t)--;
        } else if (!spawned) {
            s32 msg[2];
            s32 slot;

            spawned = 1;
            *t = (VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 0x1F) + 10;
            slot = Effect_New(mgr, 0x220, strand_init);
            msg[0] = AT(gCharPursuer, 0x153C, u8);
            msg[1] = AT(o, 0x2C, s32);
            func_002D6090(mgr, slot, msg);
        }
    }
    return 1;
}

/* its set up: +0x34 0, +0x30 -1, ten random 0..15 from +0x4 */
void func_0036A7F0(u8 *o) {
    VObject *rnd;
    s32 i;

    AT(o, 0x34, u8) = 0;
    AT(o, 0x30, s32) = -1;
    rnd = gRandom;
    for (i = 0; i < 10; i++) {
        AT(o, 0x4 + i * 4, s32) = VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 0xF;
    }
}

/* ---- two more pursuer-kind destructors and two empty methods (2026-10-05) ---- */

extern void *D_00474560[], *D_004723B0[];

/* vtable +0x8 of D_00474560 (kind 0x15's second class): its vtable, then the base's */
/* 0x00172CD0 */
Pursuer *Kind21_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = D_00474560;
        Pursuer_DestroyBase(p);
        if ((s16)flags > 0) {
            func_00124E40(&p->c.a);
        }
    }
    return p;
}

/* 0x00172DE0 */
void *Kind21_ctor(void *p, s32 arg, u32 id) {
    return b0_RoomCtor(p, id, arg, D_00474560);
}

/* 0x00172E20 */
void *Kind20_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x14, arg, D_00474890);
}

/* 0x00172E70 */
void *Kind19_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x13, arg, D_004734A0);
}

/* 0x00172EC0 */
void *Kind18_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x12, arg, D_00473110);
}

/* 0x00172F10 */
void *Kind17_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x11, arg, D_00472C30);
}

/* 0x00172F60 */
void *Kind16_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x10, arg, D_004728A0);
}

/* 0x00172FB0 */
void *Kind15_ctor(void *p, u32 id, u32 arg) {
    return b0_RoomCtor(p, id, (u8)arg, D_0046A2F0);
}

extern void *D_0046A2F0[];

/* vtable +0x8 of D_0046A2F0 (derived from kind 14) */
/* 0x001794C0 */
Pursuer *Kind15_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = D_0046A2F0;
        p->c.a.vtbl = D_004723B0;
        Pursuer_DestroyBase(p);
        if ((s16)flags > 0) {
            func_00124E40(&p->c.a);
        }
    }
    return p;
}

/* 0x001795E0 */
void *Kind14_ModelFiles(void) {
    return D_004297C0;
}

/* 0x001795F0 */
void *Kind14_MotionFiles(void) {
    return D_00429800;
}

/* vtable +0x8 of D_004723B0 (kind 14) */
/* 0x00173000 */
Pursuer *Kind14_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = D_004723B0;
        Pursuer_DestroyBase(p);
        if ((s16)flags > 0) {
            func_00124E40(&p->c.a);
        }
    }
    return p;
}

/* 0x00173110 */
void *Kind14_ctor(void *p, s32 arg, u32 id) {
    return b0_RoomCtor(p, id, arg, D_004723B0);
}

/* 0x00173150 */
void *Kind13_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0xD, arg, D_00472020);
}

/* 0x001731A0 */
void *Kind08_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x8, arg, D_0046FF80);
}

/* 0x001731F0 */
void *Kind12_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0xC, arg, D_004718F0);
}

/* 0x00173240 */
void *Lorenzo_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0xB, arg, D_00470720);
}

/* 0x00173290 */
void *Kind39_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x27, arg, D_00479B20);
}

/* 0x001732E0 */
void *Lorenzo2_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0xA, arg, D_004715C0);
}

/* 0x00173330 */
void *Kind09_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x9, arg, D_00471290);
}

/* 0x00173380 */
void *Riccardo_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x4, arg, D_0046F6B0);
}

/* 0x001733D0 */
void *Kind36_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x24, arg, D_00478160);
}

/* 0x00173420 */
void *Kind35_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x23, arg, D_00477E30);
}

/* 0x00173470 */
void *Kind34_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x22, arg, D_00477AE0);
}

/* 0x001734C0 */
void *Daniella_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x3, arg, D_0046BBB0);
}

/* 0x00173510 */
void *Kind27_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x1B, arg, D_00474FD0);
}

/* 0x00173560 */
void *Debilitas3_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x7, arg, D_0046F020);
}

/* 0x001735B0 */
void *Debilitas2_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x6, arg, D_00470A90);
}

/* 0x00173600 */
void *Debilitas_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x2, arg, D_00469D10);
}

void func_00173650(void) {
}

void func_00173660(void) {
}

/* ---- methods of stalker subclasses D_004712xx / D_004714xx / D_004720xx (room creatures) ---- */

extern const PTMF D_00422338, D_00422420, D_00422430, D_004297B0;

/* a state: D_00422338 at +0x174C, +0x1758 -1, then +0x294 */
/* 0x003090F0 */
void Kind09_BehaviourIdle(Pursuer *p) {
    ptmf_set(&PU(p, 0x174C, PTMF), &D_00422338);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x294, void (*)(Pursuer *))(p);
}

/* 0x00309170 */
void Kind09_OffscreenStep(Pursuer *p) {
}

/* in play: func_00124890(-1), the state D_00422430 (+0x1758 -1) */
/* 0x00309180 */
void Kind09_ShowUp(Pursuer *p) {
    if (func_00217510(p) == 0) {
        return;
    }
    func_00124890(&p->c.a, -1);
    ptmf_set(&PU(p, 0x174C, PTMF), &D_00422430);
    PU(p, 0x1758, s32) = -1;
}

/* the action 5 taken (+0x14E8): in play +0x8C, the state `st`, +0x114 1; the action cleared */
static inline __attribute__((always_inline)) void act5(Pursuer *p, const PTMF *st) {
    if (PU(p, 0x14E8, s32) != 5) {
        return;
    }
    if ((u8)func_00217510(p) != 0) {
        VCALL(p, 0x8C, void (*)(Pursuer *))(p);
        ptmf_set(&PU(p, 0x174C, PTMF), st);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
    }
    PU(p, 0x14E8, s32) = 0;
    PU(p, 0x14EC, s32) = 0;
}

/* 0x00309210 */
void Kind09_EventState(Pursuer *p) {
    act5(p, &D_00422420);
}

/* 0x00311B30 */
void Kind13_EventState(Pursuer *p) {
    act5(p, &D_004297B0);
}

/* its slot's progress entry (func_00177870) 1: SlotCmd_Cancel; -1 */
static inline __attribute__((always_inline)) s32 slot_done(Pursuer *p) {
    Progress *g = gProgress;

    if ((u8)func_00177870(g, *(u8 *)&p->c.a.slot) == 1) {
        SlotCmd_Cancel(g, *(u8 *)&p->c.a.slot);
    }
    return -1;
}

/* 0x003092E0 */
s32 Kind09_GrabOrder(Pursuer *p) {
    return slot_done(p);
}

/* 0x00311C00 */
s32 Kind13_GrabOrder(Pursuer *p) {
    return slot_done(p);
}

/* each frame: +0x84; the nav mask +0xC0 (8 when +0x2B is 1, else +0xA8) onto its model
 * (+0x1380 +0x40); in play func_00297C60, its state (+0x174C) and +0x110; then +0x40 */
/* 0x00309350 */
void Kind09_Update(Pursuer *p) {
    VCALL(p, 0x84, void (*)(Pursuer *))(p);
    if (AT(p, 0x2B, u8) == 1) {
        AT(p, 0xC0, s32) = 8;
    } else {
        AT(p, 0xC0, s32) = VCALL(p, 0xA8, s32 (*)(Pursuer *))(p);
    }
    AT(PU(p, 0x1380, u8 *), 0x40, s32) = AT(p, 0xC0, s32);
    if (func_00217510(p) != 0) {
        func_00297C60(p);
        if (ptmf_test(&PU(p, 0x174C, PTMF))) {
            ptmf_scall(p, &PU(p, 0x174C, PTMF));
        }
        VCALL(p, 0x110, void (*)(Pursuer *))(p);
    }
    VCALL(p, 0x40, void (*)(Pursuer *))(p);
}

u8 *func_00309410(Pursuer *p) {
    return b5_prog_flag8000() ? D_004223C0 : D_00422380;
}

/* 0x00309450 */
void *Kind09_ModelFiles(void) {
    return b5_prog_flag8000() ? D_004223A0 : D_00422360;
}

u8 *func_0030C1B0(Pursuer *p) {
    return b5_prog_flag8000() ? D_004224A0 : D_00422460;
}

/* 0x003119A0 */
Character *Kind13_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, D_00472020); }

/* 0x00311AB0 */
void *Kind13_ModelFiles(void) {
    return D_00429730;
}

/* 0x00311AC0 */
void *Kind13_MotionFiles(void) {
    return D_00429770;
}

/* in play: func_00124890(-1) */
/* 0x00311AE0 */
void Kind13_ShowUp(Pursuer *p) {
    if (func_00217510(p) != 0) {
        func_00124890(&p->c.a, -1);
    }
}
