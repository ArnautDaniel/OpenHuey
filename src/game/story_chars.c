/* The story characters loaded into character slot 2 for events (the character loader's
 * kinds 8, 9, 13..33 and 38 that aren't Debilitas, Daniella, Riccardo or Lorenzo variants):
 * Pursuers with their own tables, models and a few behaviours of their own. See pursuer.h. */
#include "common.h"
#include "pursuer.h"
#include "game.h"
#include "progress.h"
#include "actor.h"
#include "ptmf.h"
#include "char_load.h"
#include "model.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "creature.h"
#include "effects.h"
#include "hewie.h"
#include "scene_game.h"
#include "heap.h"
#include "vecmath.h"
#include "msl.h"
#include "navmesh.h"
#include "effectmgr.h"
#include "item.h"
#include "items.h"
#include "renderer.h"
#include "sound.h"
#include "input.h"
#include "memcard.h"
#include "event.h"
#include "lights.h"
#include "debilitas.h"
#include "fiona.h"
#include "libc.h"
#include "effectmgr.h"   /* HitEffect_Spawn */
#include "loader.h"
#include <stdint.h>
#include "charaction.h"
#include "gl2d.h"
#include "daniella.h"
#include "debilitas2.h"
#include "lorenzo.h"
#include "system.h"
#include "music.h"
#include "camera.h"
#include "doors.h"
#include "gameover.h"
#include "movie.h"
#include "pause.h"
#include "placed.h"
#include "room_map.h"
#include "scene.h"
#include "scene_title.h"
#include "draw_leaves.h"
#include "sce/eekernel.h"
#include "pad.h"
#include "scene_boot.h"
#include "sce/iop.h"
#include "cri/adx.h"
#include "subscreen.h"
#include "text.h"
#include "gs.h"
#include "director.h"
#include "room.h"
#include "sce/intc.h"
#include "story_chars.h"
#ifdef HG_NATIVE
#include <stdio.h>
#include <stdlib.h>
#include "glr.h"
#endif
extern void *Kind38_vtable[];
#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

extern u8 D_0042C6A0[];
extern u8 D_0042C6E0[];
extern const char *const pstr_O_DNL_DNL_202_TEX;
extern void *Kind25_vtable[];
extern const PTMF Pursuer_StateRunThenNext_ptmf15;
extern char str_ITEM01_ITEM_02E_TEX[]; /* file name */
extern char str_ITEM01_ITEM_02D_TEX[]; /* file name */
s32 Item12_LoadPicture(void *self, void *dest);
s32 Item13_LoadPicture(void *self, void *dest);
extern void *Kind18_vtable[];
extern const PTMF Pursuer_StateRunThenNext_ptmf12;
extern void *Kind26_vtable[];
extern const PTMF Pursuer_StateRunThenNext_ptmf18;
extern void *Kind28_vtable[];
extern const PTMF Pursuer_StateRunThenNext_ptmf19;
extern void *Kind30_vtable[];
extern const PTMF Pursuer_StateRunThenNext_ptmf21;
extern void *Kind31_vtable[];
extern const PTMF Pursuer_StateRunThenNext_ptmf22;
extern const PTMF Pursuer_StateRunThenNext_ptmf25;
extern char str_ITEM00_ITEM_008_TEX[];
s32 ItemClassF430_LoadPicture(void *self, void *dest);
extern u8 pstr_O_FIW_FIW_200_PCK[];
extern u8 D_00429DB0[];
extern u8 D_0042C990[];
extern u8 D_0042C9D0[];
extern u8 D_0042CA20[];
extern u8 D_0042CB60[];
extern u8 D_0042CC00[];
extern u8 D_0042D0C0[];
extern void *D_0042E310[];
extern void *pstr_EV0023[];
extern u8 D_0042E410[];
extern u8 D_0042E4C0[];
extern u8 D_01991600[];
extern u8 D_0042F470[];
extern u8 D_0042F4B0[];
extern u8 D_004308B0[];
extern u8 D_004308F0[];
extern u8 D_00430940[];
extern u8 D_00430980[];
void *Kind28_ModelFiles(void);
void *Kind28_MotionFiles(void);
extern u8 D_004434D0[], D_00443510[];
void *Kind38_ModelFiles(void);
void *Kind38_MotionFiles(void);
void Kind26_ShowUp(Pursuer *p);
void Kind26_EventState(Pursuer *p);
s32 Kind26_GrabOrder(Pursuer *p);
Character *Kind28_dtor(Character *c, s32 flags);
void Kind28_ShowUp(Pursuer *p);
void Kind28_EventState(Pursuer *p);
s32 Kind28_GrabOrder(Pursuer *p);
Character *Kind38_dtor(Character *c, s32 flags);
void Kind38_ShowUp(Pursuer *p);
void Kind38_EventState(Pursuer *p);
s32 Kind38_GrabOrder(Pursuer *p);
typedef s32 (*LoaderLoadFn)(void *loader, const char *name, void *dest, s32 flags, s32 arg);

/* gFileLoader->vfunc_0xC(name, dest, 0x4000000, 0): start loading a file */
#define LOAD_002D1360(name, dest) \
    VCALL(gFileLoader, 0xC, s32 (*)(void *, const char *, void *, s32, s32))(gFileLoader, name, dest, 0x4000000, 0)

/* Field access by byte offset into objects whose layout is not yet known. */
#define S16(p, off) (*(s16 *)((u8 *)(p) + (off)))

#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

#define S64(p, off) (*(s64 *)((u8 *)(p) + (off)))

#define F32(p, off) (*(f32 *)((u8 *)(p) + (off)))

#define PTR(p, off) (*(void * *)((u8 *)(p) + (off)))

#define CHAR_POS(c) ((f32 *)((u8 *)(c) + 0x10))

#define CHAR_ON(c) (*((u8 *)(c) + 0x28))

#define CHAR_ROOM(c) S32(c, 0x30)

#define CUR_ROOM() VCALL(gProgress, 0xC, s32 (*)(void *))(gProgress)

extern void *BonePoint_vtable[];
void *HangPoint_ctor(u8 *p);
void *IK2_ctor(u8 *p);
void *SwayPointB_ctor(u8 *p);
extern u8 D_00424250[];
extern u8 D_00429C10[];
extern u8 D_0042C730[];
extern void *SwayPointA_dtor(void *, s32);
extern void *EventHumanModel_vtable[];
extern void CharModel_Loaded(u8 *m);
extern void *Kind18Model_vtable[];
extern void EventHumanModel_Setup(u8 *m);
extern void *Kind09Model_vtable[];
extern void CharModel_Loaded(u8 *m);
extern void *CharModel_dtor(void *p, s32 flags);
extern void EventHumanModel_Setup(u8 *m);
extern void *HangPoint_ctor(u8 *p);
extern void *IK2_ctor(u8 *p);
extern void *Part_delete(void *e, s32 flags);
extern void *SprungPoint_dtor(void *e, s32 flags);
extern void *SwayPointA_dtor(void *e, s32 flags);
extern void *SwayPointB_ctor(u8 *p);
void Kind09Model_SecondaryMotion(u8 *self);
void Kind14Model_SecondaryMotion(u8 *self);
void Kind33Model_SecondaryMotion(u8 *self);
s32 Kind33Model_Part0(void);
s32 Kind33Model_Part1(void);
s32 Kind33Model_Part2(void);
s32 Kind33Model_Part3(void);
void *Kind18Model_dtor(void *p, s32 flags);
void Kind18Model_ShowParts(void);
void Kind18Model_Loaded(u8 *m);
void Kind18Model_Frame(u8 *m);

extern void *Kind32_vtable[];
extern const PTMF Pursuer_StateRunThenNext_ptmf23;
extern u8 pstr_O_FIM_FIM_200_PCK[];
extern u8 D_00430A50[];
Character *Kind32_dtor(Character *c, s32 flags);
void *Kind32_ModelFiles(void);
void *Kind32_MotionFiles(void);
void Kind32_ShowUp(Pursuer *p);
void Kind32_EventState(Pursuer *p);
s32 Kind32_GrabOrder(Pursuer *p);
static inline __attribute__((always_inline)) s32 creature_slot_done(Pursuer *p);
static inline __attribute__((always_inline)) void creature_act5(Pursuer *p, const PTMF *st);
static inline __attribute__((always_inline)) void creature_inplay(Pursuer *p);
static inline __attribute__((always_inline)) Character *creature_dtor(Character *c, s32 flags, void **vt);

extern void *Kind33_vtable[];
extern void *Kind29_vtable[];
extern void *Kind24_vtable[];
extern void *Kind22_vtable[];
extern void *Kind21_vtable[];
extern void *Kind20_vtable[];
extern void *Kind19_vtable[];
extern void *Kind17_vtable[];
extern void *Kind16_vtable[];
extern void *Kind14_vtable[];
extern void *Kind13_vtable[];
extern void *Kind08_vtable[];
extern void *Kind09_vtable[];
extern u8 D_00419DD0[];
extern u8 D_00419E10[];
extern u8 D_004297C0[];
extern u8 D_00429800[];
extern u8 D_0042C870[];
extern u8 D_0042C8B0[];
extern u8 pstr_O_LRM_LRM_200_PCK[];
extern u8 pstr_O_LRM_LRM_200_PCK_2[];
extern u8 D_00429C50[];
extern u8 D_00429C90[];
extern u8 D_00429CE0[];
extern u8 D_00429D20[];
extern const PTMF Pursuer_StateRunThenNext_ptmf16;
extern const PTMF D_00422348;            /* a creature state */
extern u8 D_00422360[];
extern u8 D_004223A0[];
extern u8 D_004223E0[];
extern u8 pstr_O_FIN_FIN_200_PCK[];
extern u8 D_00429770[];
extern u8 D_0042A0F0[];
extern u8 D_0042A130[];
extern u8 D_0042C380[];
extern u8 D_0042C3C0[];
extern u8 D_0042C900[];
extern u8 D_0042C940[];
extern u8 D_00430820[];
extern u8 D_00430860[];
extern u32 D_0043B5B0[];
extern u32 D_0043B5F0[];
extern const PTMF Pursuer_StateRunThenNext_ptmf13;
extern const PTMF Pursuer_StateRunThenNext_ptmf14;
extern const PTMF Pursuer_StateRunThenNext_ptmf17;
extern const PTMF Pursuer_StateRunThenNext_ptmf20;
extern const PTMF Pursuer_StateRunThenNext_ptmf24;
extern const PTMF Pursuer_StateRunThenNext_ptmf6;   /* its behaviour after a reset */
extern const PTMF Pursuer_StateRunThenNext_ptmf9;
extern const PTMF Pursuer_StateRunThenNext_ptmf10;
extern const PTMF Pursuer_StateRunThenNext_ptmf11;
extern const PTMF D_00422338, Pursuer_StateRunThenNext_ptmf7, D_00422430, Pursuer_StateRunThenNext_ptmf8;
#define U32(p, off) (*(u32 *)((u8 *)(p) + (off)))

static inline s32 b5_prog_flag8000(void);

static inline __attribute__((always_inline)) void creature_inplay(Pursuer *p);

extern void *Kind33Model_vtable[];
void *Kind33Model_ctor(u8 *m);

extern void Mtx_Model(f32 (*mtx)[4], const f32 *pos, f32 heading);
extern void *Kind14Model_vtable[];
void *Kind14Model_dtor(u8 *m, s32 flags);
void Kind14Model_Loaded(u8 *m);
void Kind14Model_Frame(u8 *m);
void Kind14Model_Draw(u8 *m, s32 layer, s32 a, s32 b);
void *Kind09Model_dtor(u8 *m, s32 flags);
s32 Kind09Model_Part0(u8 *m);
s32 Kind09Model_Part1(u8 *m);
s32 Kind09Model_Part2(u8 *m);
s32 Kind09Model_Part3(u8 *m);
void Kind09Model_Vt3C(u8 *m);
void Kind09Model_Loaded(u8 *m);
void *Kind33Model_dtor(u8 *m, s32 flags);
void Kind33Model_BodyFrames(u8 *m, u8 *actor, f32 lift);

void *Kind14Model_ctor(u8 *m);

/* gProgress+0x30 bit 0x8000 selects between two data sets (difficulty/mode flag?) */
static inline s32 b5_prog_flag8000(void) {
    return U32(gProgress, 0x30) & 0x8000;
}

/* the action 5 taken (+0x14E8): in play +0x8C, the state `st`, +0x114 1; the action cleared */
static inline __attribute__((always_inline)) void act5(Pursuer *p, const PTMF *st) {
    if (PU(p, 0x14E8, s32) != 5) {
        return;
    }
    if ((u8)Npc_InPlayedRoom(p) != 0) {
        VCALL(p, 0x8C, void (*)(Pursuer *))(p);
        ptmf_set(&PU(p, 0x174C, PTMF), st);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
    }
    PU(p, 0x14E8, s32) = 0;
    PU(p, 0x14EC, s32) = 0;
}

/* its slot's progress entry (Progress_HasRelationCmd) 1: SlotCmd_Cancel; -1 */
static inline __attribute__((always_inline)) s32 slot_done(Pursuer *p) {
    Progress *g = gProgress;

    if ((u8)Progress_HasRelationCmd(g, *(u8 *)&p->c.a.slot) == 1) {
        SlotCmd_Cancel(g, *(u8 *)&p->c.a.slot);
    }
    return -1;
}

void *Kind21_ModelFiles(void);
void *Kind21_MotionFiles(void);
Pursuer *Kind08_dtor(Pursuer *p, s32 flags);
void *Kind08_Table6C(void);
void *Kind08_Table70(void);
void Kind08_Setup(Pursuer *p);
void Kind08_ShowUp(Pursuer *p);
void Kind08_EventState(Pursuer *p);
s32 Kind08_GrabOrder(Pursuer *p);
Character *Kind09_dtor(Character *c, s32 flags);
void *Kind09_MotionFiles(void);
void Kind09_Behaviour25C(Character *c);
void Kind09_BehaviourSearch(Character *c);
void Kind14_ShowUp(Pursuer *p);
void Kind14_EventState(Pursuer *p);
s32 Kind14_GrabOrder(Pursuer *p);
Pursuer *Kind16_dtor(Pursuer *p, s32 flags);
void *Kind16_ModelFiles(void);
void *Kind16_MotionFiles(void);
void Kind16_ShowUp(Pursuer *p);
void Kind16_EventState(Pursuer *p);
s32 Kind16_GrabOrder(Pursuer *p);
Pursuer *Kind17_dtor(Pursuer *p, s32 flags);
void *Kind17_ModelFiles(void);
void *Kind17_MotionFiles(void);
void Kind17_ShowUp(Pursuer *p);
void Kind17_EventState(Pursuer *p);
s32 Kind17_GrabOrder(Pursuer *p);
Character *Kind19_dtor(Character *c, s32 flags);
void *Kind19_ModelFiles(void);
void *Kind19_MotionFiles(void);
void Kind19_ShowUp(Pursuer *p);
void Kind19_EventState(Pursuer *p);
s32 Kind19_GrabOrder(Pursuer *p);
Character *Kind24_dtor(Character *c, s32 flags);
void *Kind24_ModelFiles(void);
void *Kind24_MotionFiles(void);
void Kind24_ShowUp(Pursuer *p);
void Kind24_EventState(Pursuer *p);
s32 Kind24_GrabOrder(Pursuer *p);
void Kind21_ShowUp(Pursuer *p);
void Kind21_EventState(Pursuer *p);
s32 Kind21_GrabOrder(Pursuer *p);
Character *Kind20_dtor(Character *c, s32 flags);
void *Kind20_ModelFiles(void);
void *Kind20_MotionFiles(void);
void Kind20_ShowUp(Pursuer *p);
void Kind20_EventState(Pursuer *p);
s32 Kind20_GrabOrder(Pursuer *p);
Character *Kind29_dtor(Character *c, s32 flags);
void *Kind29_ModelFiles(void);
void *Kind29_MotionFiles(void);
void Kind29_ShowUp(Pursuer *p);
void Kind29_EventState(Pursuer *p);
s32 Kind29_GrabOrder(Pursuer *p);
Character *Kind33_dtor(Character *c, s32 flags);
void *Kind33_ModelFiles(void);
void *Kind33_MotionFiles(void);
void Kind33_ShowUp(Pursuer *p);
void Kind33_EventState(Pursuer *p);
s32 Kind33_GrabOrder(Pursuer *p);
Pursuer *Kind21_dtor(Pursuer *p, s32 flags);
void *Kind14_ModelFiles(void);
void *Kind14_MotionFiles(void);
Pursuer *Kind14_dtor(Pursuer *p, s32 flags);
void Kind09_BehaviourIdle(Pursuer *p);
void Kind09_OffscreenStep(Pursuer *p);
void Kind09_ShowUp(Pursuer *p);
void Kind09_EventState(Pursuer *p);
void Kind13_EventState(Pursuer *p);
s32 Kind09_GrabOrder(Pursuer *p);
s32 Kind13_GrabOrder(Pursuer *p);
void Kind09_Update(Pursuer *p);
void *Kind09_ModelFiles(void);
Character *Kind13_dtor(Character *c, s32 flags);
void *Kind13_ModelFiles(void);
void *Kind13_MotionFiles(void);
void Kind13_ShowUp(Pursuer *p);
static inline __attribute__((always_inline)) Character *creature_dtor(Character *c, s32 flags, void **vt);
static inline __attribute__((always_inline)) void creature_inplay(Pursuer *p);
static inline void *b0_RoomCtor(void *p, u32 id, s32 arg, void **vtbl);
static inline __attribute__((always_inline)) s32 creature_slot_done(Pursuer *p);
static inline __attribute__((always_inline)) void creature_act5(Pursuer *p, const PTMF *st);

static f32 animal_rnd(void) {
    return VCALL(gRandom, 0x20, f32 (*)(void *))(gRandom);
}

Character *Kind30_dtor(Character *c, s32 flags);
void *Kind30_ModelFiles(void);
void *Kind30_MotionFiles(void);
void Kind30_ShowUp(Pursuer *p);
void Kind30_EventState(Pursuer *p);
s32 Kind30_GrabOrder(Pursuer *p);
Character *Kind31_dtor(Character *c, s32 flags);
void *Kind31_ModelFiles(void);
void *Kind31_MotionFiles(void);
void Kind31_ShowUp(Pursuer *p);
void Kind31_EventState(Pursuer *p);
s32 Kind31_GrabOrder(Pursuer *p);
Character *Kind18_dtor(Character *c, s32 flags);
void *Kind18_ModelFiles(void);
void *Kind18_MotionFiles(void);
void Kind18_ShowUp(Pursuer *p);
void Kind18_EventState(Pursuer *p);
s32 Kind18_GrabOrder(Pursuer *p);
Character *Kind26_dtor(Character *c, s32 flags);
void *Kind26_ModelFiles(void);
void *Kind26_MotionFiles(void);
f32 Kind26_NearestDistSq(u8 *self, f32 *out);
static inline __attribute__((always_inline)) void creature_act5(Pursuer *p, const PTMF *st);
static inline __attribute__((always_inline)) void creature_inplay(Pursuer *p);
static inline __attribute__((always_inline)) Character *creature_dtor(Character *c, s32 flags, void **vt);
static inline __attribute__((always_inline)) s32 creature_slot_done(Pursuer *p);

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

/* in play: Actor_TeleportRandom(-1) */
static inline __attribute__((always_inline)) void creature_inplay(Pursuer *p) {
    if (Npc_InPlayedRoom(p) != 0) {
        Actor_TeleportRandom(&p->c.a, -1);
    }
}

/* the action 5 taken (+0x14E8): in play +0x8C, the state st, +0x114 1; the action cleared */
static inline __attribute__((always_inline)) void creature_act5(Pursuer *p, const PTMF *st) {
    if (PU(p, 0x14E8, s32) != 5) {
        return;
    }
    if ((u8)Npc_InPlayedRoom(p) != 0) {
        VCALL(p, 0x8C, void (*)(Pursuer *))(p);
        ptmf_set(&PU(p, 0x174C, PTMF), st);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
    }
    PU(p, 0x14E8, s32) = 0;
    PU(p, 0x14EC, s32) = 0;
}

/* its slot's progress entry (Progress_HasRelationCmd) 1: SlotCmd_Cancel; -1 */
static inline __attribute__((always_inline)) s32 creature_slot_done(Pursuer *p) {
    Progress *g = gProgress;

    if ((u8)Progress_HasRelationCmd(g, *(u8 *)&p->c.a.slot) == 1) {
        SlotCmd_Cancel(g, *(u8 *)&p->c.a.slot);
    }
    return -1;
}

Character *Kind25_dtor(Character *c, s32 flags);
void *Kind25_ModelFiles(void);
void *Kind25_MotionFiles(void);
void Kind25_ShowUp(Pursuer *p);
void Kind25_EventState(Pursuer *p);
s32 Kind25_GrabOrder(Pursuer *p);

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

void *Kind38_ctor(void *p, s32 arg);

/* 0x001727C0 */
void *Kind38_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x26, arg, Kind38_vtable);
}

/* 0x00172910 */
void *Kind33_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x21, arg, Kind33_vtable);
}

/* 0x00172960 */
void *Kind32_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x20, arg, Kind32_vtable);
}

/* 0x001729B0 */
void *Kind31_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x1F, arg, Kind31_vtable);
}

/* 0x00172A00 */
void *Kind30_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x1E, arg, Kind30_vtable);
}

/* 0x00172A50 */
void *Kind29_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x1D, arg, Kind29_vtable);
}

/* 0x00172AA0 */
void *Kind28_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x1C, arg, Kind28_vtable);
}

/* 0x00172AF0 */
void *Kind26_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x1A, arg, Kind26_vtable);
}

/* 0x00172B40 */
void *Kind25_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x19, arg, Kind25_vtable);
}

/* 0x00172B90 */
void *Kind24_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x18, arg, Kind24_vtable);
}

/* 0x00172C80 */
void *Kind22_ctor(void *p, u32 id, u32 arg) {
    return b0_RoomCtor(p, id, (u8)arg, Kind22_vtable);
}

/* vtable +0x8 of Kind21_vtable (kind 0x15's second class): its vtable, then the base's */
/* 0x00172CD0 */
Pursuer *Kind21_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = Kind21_vtable;
        Pursuer_DestroyBase(p);
        if ((s16)flags > 0) {
            Actor_Destroy(&p->c.a);
        }
    }
    return p;
}

/* 0x00172DE0 */
void *Kind21_ctor(void *p, s32 arg, u32 id) {
    return b0_RoomCtor(p, id, arg, Kind21_vtable);
}

/* 0x00172E20 */
void *Kind20_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x14, arg, Kind20_vtable);
}

/* 0x00172E70 */
void *Kind19_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x13, arg, Kind19_vtable);
}

/* 0x00172EC0 */
void *Kind18_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x12, arg, Kind18_vtable);
}

/* 0x00172F10 */
void *Kind17_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x11, arg, Kind17_vtable);
}

/* 0x00172F60 */
void *Kind16_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x10, arg, Kind16_vtable);
}

/* vtable +0x8 of Kind14_vtable (kind 14) */
/* 0x00173000 */
Pursuer *Kind14_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = Kind14_vtable;
        Pursuer_DestroyBase(p);
        if ((s16)flags > 0) {
            Actor_Destroy(&p->c.a);
        }
    }
    return p;
}

/* 0x00173110 */
void *Kind14_ctor(void *p, s32 arg, u32 id) {
    return b0_RoomCtor(p, id, arg, Kind14_vtable);
}

/* 0x00173150 */
void *Kind13_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0xD, arg, Kind13_vtable);
}

/* 0x001731A0 */
void *Kind08_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x8, arg, Kind08_vtable);
}

/* 0x00173330 */
void *Kind09_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x9, arg, Kind09_vtable);
}

/* 0x001795E0 */
void *Kind14_ModelFiles(void) {
    return D_004297C0;
}

/* 0x001795F0 */
void *Kind14_MotionFiles(void) {
    return D_00429800;
}

/* ---- defaults shared by the stalker vtables (0x179600..0x179970) ---- */

/* 0x00179600 */
void Kind14_Setup(Pursuer *p) {
    Pursuer_Setup(p);
}

/* destructor of the kind 0x15 class (vtables 0x474560 / 0x46A620 -> 0x46D810 -> 0x46C220 ->
 * Character) */
/* 0x00179970 */
Pursuer *Kind22_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = Kind22_vtable;
        if (p != NULL) {
            p->c.a.vtbl = Kind21_vtable;
            if (p != NULL) {
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
        }
        if ((s16)flags > 0) {
            Actor_Destroy(&p->c.a);
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

/* vtable +0x8: destructor */
/* 0x002ECB80 */
Pursuer *Kind08_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = Kind08_vtable;
        if (p != NULL) {
            Pursuer_DestroyBase(p);
        }
        if ((s16)flags > 0) {
            Actor_Destroy(&p->c.a);
        }
    }
    return p;
}

/* 0x002ECC90 */
void *Kind08_Table6C(void) {
    return D_00419DD0;
}

/* 0x002ECCA0 */
void *Kind08_Table70(void) {
    return D_00419E10;
}

/* vtable +0xF4: setup (the Pursuer's) */
/* 0x002ECCB0 */
void Kind08_Setup(Pursuer *p) {
    Pursuer_Setup(p);
}

/* vtable +0x38: on screen, Actor_TeleportRandom(-1) */
/* 0x002ECCC0 */
void Kind08_ShowUp(Pursuer *p) {
    if (Npc_InPlayedRoom(p) != 0) {
        Actor_TeleportRandom(&p->c.a, -1);
    }
}

/* vtable +0x84: a pending reset (state 5): on screen, +0x8C, its behaviour Pursuer_StateRunThenNext_ptmf6 and the
 * next one cleared (+0x1758), then +0x114(1); the state is cleared either way */
/* 0x002ECD10 */
void Kind08_EventState(Pursuer *p) {
    if (p->c.state[0] != 5) {
        return;
    }
    if (Npc_InPlayedRoom(p) & 0xFF) {
        VCALL(p, 0x8C, void (*)(Pursuer *))(p);
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &Pursuer_StateRunThenNext_ptmf6);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
    }
    p->c.state[0] = 0;
    p->c.state[1] = 0;
}

/* vtable +0x110: a joint action pending for it is cancelled; -1 */
/* 0x002ECDE0 */
s32 Kind08_GrabOrder(Pursuer *p) {
    Progress *pr = gProgress;

    if ((Progress_HasRelationCmd(pr, *(u8 *)&p->c.a.slot) & 0xFF) == 1) {
        SlotCmd_Cancel(pr, *(u8 *)&p->c.a.slot);
    }
    return -1;
}

/* +0x8 destructor (0x471290 -> Pursuer 0x46D810 -> NPC 0x46C220 -> Character); the model freed
 * for slots 3..5 */
/* 0x00308EC0 */
Character *Kind09_dtor(Character *c, s32 flags) {
    if (c != NULL) {
        c->a.vtbl = Kind09_vtable;
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

/* in play: Actor_TeleportRandom(-1), the state D_00422430 (+0x1758 -1) */
/* 0x00309180 */
void Kind09_ShowUp(Pursuer *p) {
    if (Npc_InPlayedRoom(p) == 0) {
        return;
    }
    Actor_TeleportRandom(&p->c.a, -1);
    ptmf_set(&PU(p, 0x174C, PTMF), &D_00422430);
    PU(p, 0x1758, s32) = -1;
}

/* 0x00309210 */
void Kind09_EventState(Pursuer *p) {
    act5(p, &Pursuer_StateRunThenNext_ptmf7);
}

/* 0x003092E0 */
s32 Kind09_GrabOrder(Pursuer *p) {
    return slot_done(p);
}

/* each frame: +0x84; the nav mask +0xC0 (8 when +0x2B is 1, else +0xA8) onto its model
 * (+0x1380 +0x40); in play Pursuer_MotionGroup, its state (+0x174C) and +0x110; then +0x40 */
/* 0x00309350 */
void Kind09_Update(Pursuer *p) {
    VCALL(p, 0x84, void (*)(Pursuer *))(p);
    if (AT(p, 0x2B, u8) == 1) {
        AT(p, 0xC0, s32) = 8;
    } else {
        AT(p, 0xC0, s32) = VCALL(p, 0xA8, s32 (*)(Pursuer *))(p);
    }
    AT(PU(p, 0x1380, u8 *), 0x40, s32) = AT(p, 0xC0, s32);
    if (Npc_InPlayedRoom(p) != 0) {
        Pursuer_MotionGroup(p);
        if (ptmf_test(&PU(p, 0x174C, PTMF))) {
            ptmf_scall(p, &PU(p, 0x174C, PTMF));
        }
        VCALL(p, 0x110, void (*)(Pursuer *))(p);
    }
    VCALL(p, 0x40, void (*)(Pursuer *))(p);
}

/* 0x00309410 */
u8 *Kind09_ModelFileTable(Pursuer *p) {
    return b5_prog_flag8000() ? pstr_O_LRM_LRM_200_PCK_2 : pstr_O_LRM_LRM_200_PCK;
}

/* 0x00309450 */
void *Kind09_ModelFiles(void) {
    return b5_prog_flag8000() ? D_004223A0 : D_00422360;
}

/* +0x8 destructor */
/* 0x0030D150 */
void *Kind09Model_dtor(u8 *m, s32 flags) {
    if (m != NULL) {
        AT(m, 0x0, void **) = Kind09Model_vtable;
        __destroy_arr(m + 0xD40, SwayPointB_dtor, 0x50, 0x18);
        __destroy_arr(m + 0x9A0, HangPoint_dtor, 0x50, 6);
        AT(m, 0x0, void **) = HumanModel_vtable;
        AT(m, 0x988, void **) = IK2_vtable;
        AT(m, 0x928, void **) = IK2_vtable;
        AT(m, 0x0, void **) = Model_vtable;
        AT(m, 0x0, void **) = ModelBase_vtable;
        AT(m, 0x1D0, void **) = Shadow_vtable;
        AT(m, 0x1D0, void **) = Helper469D00_vtable;
        AT(m, 0x10, void **) = ModelDrawer_vtable;
        AT(m, 0x10, void **) = Helper469D00_vtable;
        if ((s16)flags > 0) {
            StalkerModel_delete(m);
        }
    }
    return m;
}

/* 0x0030D2A0 */
void Kind09Model_SecondaryMotion(u8 *self) {
    PTR(self, 0x874) = D_00424250;
}

/* +0x84 .. +0x90: his mesh parts */
/* 0x0030D2B0 */
s32 Kind09Model_Part0(u8 *m) {
    return 3;
}

/* 0x0030D2C0 */
s32 Kind09Model_Part1(u8 *m) {
    return 0x13;
}

/* 0x0030D2D0 */
s32 Kind09Model_Part2(u8 *m) {
    return 0x26;
}

/* 0x0030D2E0 */
s32 Kind09Model_Part3(u8 *m) {
    return 0x37;
}

/* +0x3C settle his springs: 30 steps the first time (+0x850), else one */
/* 0x0030D940 */
void Kind09Model_Vt3C(u8 *m) {
    s32 n = AT(m, 0x850, u8) ? 30 : 1, i;

    SpringSet_Begin(m + 0xD00);
    SpringSet_Begin(m + 0x14C0);
    for (i = 0; i < n; i++) {
        SpringSet_Step(m + 0xD00);
        SpringSet_Step(m + 0x14C0);
    }
    SpringSet_Finish(m + 0xD00);
    SpringSet_Finish(m + 0x14C0);
    AT(m, 0x850, u8) = 0;
}

/* +0xC: once loaded: the base setup, the part roles, the springs, per-part draw settings */
/* 0x0030D9F0 */
void Kind09Model_Loaded(u8 *m) {
    HumanModel_Loaded(m);
    AT(m, 0x890, s32) = 2;
    AT(m, 0x894, s32) = 3;
    AT(m, 0x898, s32) = 4;
    AT(m, 0x89C, s32) = 5;
    AT(m, 0x8B8, s32) = 0x28;
    AT(m, 0x8A0, s32) = 0x12;
    AT(m, 0x8A4, s32) = 0x13;
    AT(m, 0x8A8, s32) = 0x14;
    AT(m, 0x8AC, s32) = 0x15;
    AT(m, 0x8BC, s32) = 0x39;
    AT(m, 0x8B0, s32) = 0x2B;
    AT(m, 0x8B4, s32) = 0x22;
    AT(m, 0x860, f32) = 0.0f;
    AT(m, 0x864, f32) = 16.0f;
    AT(m, 0x868, f32) = 0.0f;
    AT(m, 0x854, f32) = 0.0f;
    AT(m, 0x858, f32) = 0.0f;
    Lorenzo2Model_Hanging(m);
    Lorenzo2Model_Strands(m);
    AT(m, 0x850, u8) = 1;
    AT(m, 0x9C, u8) = 4;
    AT(m, 0x9D, u8) = 0x40;
    AT(m, 0x9E, u8) = 4;
    AT(m, 0x9F, u8) = 0x40;
    AT(m, 0xA0, u8) = 4;
    AT(m, 0xA1, u8) = 0x40;
    AT(m, 0xCA, u8) = 4;
    AT(m, 0xCB, u8) = 0x40;
    AT(m, 0xCC, u8) = 4;
    AT(m, 0xCD, u8) = 0x40;
    AT(m, 0xCE, u8) = 4;
    AT(m, 0xCF, u8) = 0x40;
    AT(m, 0xA2, u8) = 4;
    AT(m, 0xA3, u8) = 0xC0;
    AT(m, 0xC4, u8) = 4;
    AT(m, 0xC5, u8) = 0xC0;
}

/* 0x003119A0 */
Character *Kind13_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind13_vtable); }

/* 0x00311AB0 */
void *Kind13_ModelFiles(void) {
    return pstr_O_FIN_FIN_200_PCK;
}

/* 0x00311AC0 */
void *Kind13_MotionFiles(void) {
    return D_00429770;
}

/* in play: Actor_TeleportRandom(-1) */
/* 0x00311AE0 */
void Kind13_ShowUp(Pursuer *p) {
    if (Npc_InPlayedRoom(p) != 0) {
        Actor_TeleportRandom(&p->c.a, -1);
    }
}

/* 0x00311B30 */
void Kind13_EventState(Pursuer *p) {
    act5(p, &Pursuer_StateRunThenNext_ptmf8);
}

/* 0x00311C00 */
s32 Kind13_GrabOrder(Pursuer *p) {
    return slot_done(p);
}

/* +0x38 a frame: back on the mesh (Actor_TeleportRandom) when Npc_InPlayedRoom says so */
/* 0x00312EA0 */
void Kind14_ShowUp(Pursuer *p) {
    if (Npc_InPlayedRoom(p) != 0) {
        Actor_TeleportRandom(&p->c.a, -1);
    }
}

/* +0x84 a request of kind 5 (+0x14E8): when Npc_InPlayedRoom allows it, +0x8C, its state
 * (+0x174C) Pursuer_StateRunThenNext_ptmf9 with no target (+0x1758 -1), +0x114(1); the request cleared either way */
/* 0x00312EF0 */
void Kind14_EventState(Pursuer *p) {
    if (PU(p, 0x14E8, s32) != 5) {
        return;
    }
    if ((Npc_InPlayedRoom(p) & 0xFF) != 0) {
        VCALL(p, 0x8C, void (*)(Pursuer *))(p);
        ptmf_set(&PU(p, 0x174C, PTMF), &Pursuer_StateRunThenNext_ptmf9);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
    }
    PU(p, 0x14E8, s32) = 0;
    PU(p, 0x14EC, s32) = 0;
}

/* +0x110 let its progress slot go (SlotCmd_Cancel) if it holds one (Progress_HasRelationCmd); -1 */
/* 0x00312FC0 */
s32 Kind14_GrabOrder(Pursuer *p) {
    Progress *pr = gProgress;

    if ((Progress_HasRelationCmd(pr, *(u8 *)&p->c.a.slot) & 0xFF) == 1) {
        SlotCmd_Cancel(pr, *(u8 *)&p->c.a.slot);
    }
    return -1;
}

/* +0x8: destructor */
/* 0x00313FD0 */
void *Kind14Model_dtor(u8 *m, s32 flags) {
    if (m != NULL) {
        AT(m, 0x0, void **) = Kind14Model_vtable;
        AT(m, 0x0, void **) = Model_vtable;
        AT(m, 0x0, void **) = ModelBase_vtable;
        AT(m, 0x1D0, void **) = Shadow_vtable;
        AT(m, 0x1D0, void **) = Helper469D00_vtable;
        AT(m, 0x10, void **) = ModelDrawer_vtable;
        AT(m, 0x10, void **) = Helper469D00_vtable;
        if ((s16)flags > 0) {
            StalkerModel_delete(m);
        }
    }
    return m;
}

/* 0x003140A0 */
void Kind14Model_SecondaryMotion(u8 *self) {
    PTR(self, 0x874) = D_00429C10;
}

/* +0x38 draw: the form by the animation, the texture cache's layers forgotten */
/* 0x003140B0 */
void Kind14Model_Draw(u8 *m, s32 layer, s32 a, s32 b) {
    static const u32 kForm = 4 | 0x40 | 0x200 | 0x400 | 0x10000 | 0x80000;

    if (AT(m, 0x4DC, s32) == 0x9001 || AT(m, 0x4DC, s32) == 0x9000) {
        AT(m, 0x9E, u8) |= 2;
        AT(m, 0xA0, u8) |= 2;
        AT(m, 0xA2, u8) &= ~2;
        AT(m, 0x4B0, u32) |= kForm;
    } else {
        AT(m, 0x9E, u8) &= ~2;
        AT(m, 0xA0, u8) &= ~2;
        AT(m, 0xA2, u8) |= 2;
        AT(m, 0x4B0, u32) &= ~kForm;
    }
    VCALL(gTexCache, 0x18, void (*)(VObject *))(gTexCache);
    Model_Draw(m, layer, a, b);
}

/* 0x00314240 */
void Kind14Model_Frame(u8 *m) {
    Model_Release(m);
}

/* +0xC / +0x10: the base's */
/* 0x00314250 */
void Kind14Model_Loaded(u8 *m) {
    Model_Loaded(m);
}

/* +0x8 destructor */
/* 0x00315B30 */
Pursuer *Kind16_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = Kind16_vtable;
        Pursuer_DestroyBase(p);
        if ((s16)flags > 0) {
            Actor_Destroy(&p->c.a);
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

/* +0x38 a frame: back on the mesh (Actor_TeleportRandom) when Npc_InPlayedRoom says so */
/* 0x00315C70 */
void Kind16_ShowUp(Pursuer *p) {
    if (Npc_InPlayedRoom(p) != 0) {
        Actor_TeleportRandom(&p->c.a, -1);
    }
}

/* +0x84 a request of kind 5 (+0x14E8): when Npc_InPlayedRoom allows it, +0x8C, its state
 * (+0x174C) Pursuer_StateRunThenNext_ptmf10 with no target (+0x1758 -1), +0x114(1); the request cleared either way */
/* 0x00315CC0 */
void Kind16_EventState(Pursuer *p) {
    if (PU(p, 0x14E8, s32) != 5) {
        return;
    }
    if ((Npc_InPlayedRoom(p) & 0xFF) != 0) {
        VCALL(p, 0x8C, void (*)(Pursuer *))(p);
        ptmf_set(&PU(p, 0x174C, PTMF), &Pursuer_StateRunThenNext_ptmf10);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
    }
    PU(p, 0x14E8, s32) = 0;
    PU(p, 0x14EC, s32) = 0;
}

/* +0x110 let its progress slot go (SlotCmd_Cancel) if it holds one (Progress_HasRelationCmd); -1 */
/* 0x00315D90 */
s32 Kind16_GrabOrder(Pursuer *p) {
    Progress *pr = gProgress;

    if ((Progress_HasRelationCmd(pr, *(u8 *)&p->c.a.slot) & 0xFF) == 1) {
        SlotCmd_Cancel(pr, *(u8 *)&p->c.a.slot);
    }
    return -1;
}

/* +0x8 destructor */
/* 0x00316AB0 */
Pursuer *Kind17_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = Kind17_vtable;
        Pursuer_DestroyBase(p);
        if ((s16)flags > 0) {
            Actor_Destroy(&p->c.a);
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

/* +0x38 a frame: back on the mesh (Actor_TeleportRandom) when Npc_InPlayedRoom says so */
/* 0x00316BF0 */
void Kind17_ShowUp(Pursuer *p) {
    if (Npc_InPlayedRoom(p) != 0) {
        Actor_TeleportRandom(&p->c.a, -1);
    }
}

/* +0x84 a request of kind 5 (+0x14E8): when Npc_InPlayedRoom allows it, +0x8C, its state
 * (+0x174C) Pursuer_StateRunThenNext_ptmf11 with no target (+0x1758 -1), +0x114(1); the request cleared either way */
/* 0x00316C40 */
void Kind17_EventState(Pursuer *p) {
    if (PU(p, 0x14E8, s32) != 5) {
        return;
    }
    if ((Npc_InPlayedRoom(p) & 0xFF) != 0) {
        VCALL(p, 0x8C, void (*)(Pursuer *))(p);
        ptmf_set(&PU(p, 0x174C, PTMF), &Pursuer_StateRunThenNext_ptmf11);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
    }
    PU(p, 0x14E8, s32) = 0;
    PU(p, 0x14EC, s32) = 0;
}

/* +0x110 let its progress slot go (SlotCmd_Cancel) if it holds one (Progress_HasRelationCmd); -1 */
/* 0x00316D10 */
s32 Kind17_GrabOrder(Pursuer *p) {
    Progress *pr = gProgress;

    if ((Progress_HasRelationCmd(pr, *(u8 *)&p->c.a.slot) & 0xFF) == 1) {
        SlotCmd_Cancel(pr, *(u8 *)&p->c.a.slot);
    }
    return -1;
}

/* 0x0031D6F0 */
Character *Kind18_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind18_vtable); }

/* 0x0031D800 */
void *Kind18_ModelFiles(void) {
    return pstr_O_FIW_FIW_200_PCK;
}

/* 0x0031D810 */
void *Kind18_MotionFiles(void) {
    return D_00429DB0;
}

/* 0x0031D830 */
void Kind18_ShowUp(Pursuer *p) { creature_inplay(p); }

/* 0x0031D880 */
void Kind18_EventState(Pursuer *p) { creature_act5(p, &Pursuer_StateRunThenNext_ptmf12); }

/* 0x0031D950 */
s32 Kind18_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

/* 0x0031E5C0 */
Character *Kind19_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind19_vtable); }

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
void Kind19_EventState(Pursuer *p) { creature_act5(p, &Pursuer_StateRunThenNext_ptmf13); }

/* 0x0031E820 */
s32 Kind19_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

/* 0x00321640 */
Character *Kind24_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind24_vtable); }

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
void Kind24_EventState(Pursuer *p) { creature_act5(p, &Pursuer_StateRunThenNext_ptmf14); }

/* 0x003218A0 */
s32 Kind24_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

/* 0x0032C240 */
Character *Kind25_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind25_vtable); }

/* 0x0032C350 */
void *Kind25_ModelFiles(void) {
    return D_0042C6A0;
}

/* 0x0032C360 */
void *Kind25_MotionFiles(void) {
    return D_0042C6E0;
}

/* 0x0032C380 */
void Kind25_ShowUp(Pursuer *p) { creature_inplay(p); }

/* 0x0032C3D0 */
void Kind25_EventState(Pursuer *p) { creature_act5(p, &Pursuer_StateRunThenNext_ptmf15); }

/* 0x0032C4A0 */
s32 Kind25_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

/* (as Kind14Model_dtor)  +0x8: destructor */
/* 0x0032C510 */
void *Kind33Model_dtor(u8 *m, s32 flags) {
    if (m != NULL) {
        AT(m, 0x0, void **) = Kind33Model_vtable;
        AT(m, 0x0, void **) = Model_vtable;
        AT(m, 0x0, void **) = ModelBase_vtable;
        AT(m, 0x1D0, void **) = Shadow_vtable;
        AT(m, 0x1D0, void **) = Helper469D00_vtable;
        AT(m, 0x10, void **) = ModelDrawer_vtable;
        AT(m, 0x10, void **) = Helper469D00_vtable;
        if ((s16)flags > 0) {
            StalkerModel_delete(m);
        }
    }
    return m;
}

/* 0x0032C5E0 */
void Kind33Model_SecondaryMotion(u8 *self) {
    PTR(self, 0x874) = D_0042C730;
}

/* 0x0032C5F0 */
s32 Kind33Model_Part0(void) {
    return 0x3;
}

/* 0x0032C600 */
s32 Kind33Model_Part1(void) {
    return 0x6;
}

/* 0x0032C610 */
s32 Kind33Model_Part2(void) {
    return 0xB;
}

/* 0x0032C620 */
s32 Kind33Model_Part3(void) {
    return 0xE;
}

/* Kind33Model_vtable +0x40: the model matrix from the actor's position raised by `lift`, and heading
   (as Model_BodyFrames) */
/* 0x0032C630 */
void Kind33Model_BodyFrames(u8 *m, u8 *actor, f32 lift) {
    f32 pos[4] __attribute__((aligned(16)));

    sceVu0CopyVector(pos, (f32 *)(actor + 0x10));
    pos[1] += lift;
    Mtx_Model((f32 (*)[4])(m + 0x7D0), pos, AT(actor, 0x54, f32));
}

/* 0x0032C830 */
void Kind21_ShowUp(Pursuer *p) { creature_inplay(p); }

/* 0x0032C880 */
void Kind21_EventState(Pursuer *p) { creature_act5(p, &Pursuer_StateRunThenNext_ptmf16); }

/* 0x0032C950 */
s32 Kind21_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

/* 0x0032C9C0 */
Character *Kind20_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind20_vtable); }

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
void Kind20_EventState(Pursuer *p) { creature_act5(p, &Pursuer_StateRunThenNext_ptmf17); }

/* 0x0032CC20 */
s32 Kind20_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

/* 0x0032CE00 */
Character *Kind26_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind26_vtable); }

/* 0x0032CF10 */
void *Kind26_ModelFiles(void) {
    return D_0042C990;
}

/* 0x0032CF20 */
void *Kind26_MotionFiles(void) {
    return D_0042C9D0;
}

/* the squared distance to the nearest of Fiona, Hewie and the pursuer (those two when active
 * and in the room), whose position goes into out */
/* 0x0032CF40 */
f32 Kind26_NearestDistSq(u8 *self, f32 *out) {
    f32 d[4] __attribute__((aligned(16)));
    f32 pf[4] __attribute__((aligned(16)));
    f32 ph[4] __attribute__((aligned(16)));
    f32 pp[4] __attribute__((aligned(16)));
    f32 df, dh, dp;

    sceVu0SubVector(d, CHAR_POS(self), CHAR_POS(gCharPlayer));
    df = sceVu0InnerProduct(d, d);
    sceVu0CopyVector(pf, CHAR_POS(gCharPlayer));
    if (
#ifdef HG_NATIVE
        gCharPartner != NULL &&   /* (no Hewie: HG_NOPARTNER) */
#endif
        CHAR_ON(gCharPartner) && CHAR_ROOM(gCharPartner) == CUR_ROOM()) {
        sceVu0SubVector(d, CHAR_POS(self), CHAR_POS(gCharPartner));
        dh = sceVu0InnerProduct(d, d);
        sceVu0CopyVector(ph, CHAR_POS(gCharPartner));
    } else {
        dh = 1.0e8f;
    }
    if (gCharPursuer != NULL && CHAR_ON(gCharPursuer) && CHAR_ROOM(gCharPursuer) == CUR_ROOM()) {
        sceVu0SubVector(d, CHAR_POS(self), CHAR_POS(gCharPursuer));
        dp = sceVu0InnerProduct(d, d);
        sceVu0CopyVector(pp, CHAR_POS(gCharPursuer));
    } else {
        dp = 1.0e8f;
    }
    sceVu0CopyVector(out, pf);
    if (!(df <= dh)) {
        sceVu0CopyVector(out, ph);
        df = dh;
    }
    if (!(df <= dp)) {
        sceVu0CopyVector(out, pp);
        df = dp;
    }
    return df;
}

/* turn 0.3 of the way towards (x, z) */
static inline __attribute__((always_inline)) void animal_face(u8 *self, f32 x, f32 z) {
    static const union { u32 u; f32 f; } k03 = {0x3E99999A};
    f32 t[4] __attribute__((aligned(16)));
    f32 a, d;

    t[0] = x;
    t[1] = 0.0f;
    t[2] = z;
    t[3] = 1.0f;
    sceVu0SubVector(t, t, CHAR_POS(self));
    a = Angle_Wrap(Vec_Heading(t));
    d = Angle_Wrap(a - Angle_Wrap(F32(self, 0x54)));
    F32(self, 0x54) = Angle_Wrap(0.0f + F32(self, 0x54) + k03.f * d);
}

#define ANIMAL_CLIP_DONE(self) ((S32(PTR(PTR(self, 0xF0), 0x6A4), 0x18) & 0x20) != 0)

static inline void animal_go(u8 *self, s32 state) {
    S32(self, 0x1624) = state;
    S32(self, 0x1628) = 1;
}

/* a frame of its behaviour: 0 grazing (turned to whoever it watches, 1..2 s), 1 looking up, 4
 * turning home (5 frames), 5 going back to grazing; 2 alert (someone within 30: meant to face
 * away from them, but the original takes the point's y for its z), 3 running off (within 20) to +0x163C / +0x1640 - 1 once there (its sound
 * +0x1630 + 1 played) */
/* 0x0032D430 */
s32 Kind26_Behaviour(u8 *self) {
    static const union { u32 u; f32 f; } k08 = {0x3F4CCCCD};
    f32 who[4] __attribute__((aligned(16)));
    f32 d = Kind26_NearestDistSq(self, who);

    if (S32(self, 0x1624) != 3 && d < 400.0f) {
        animal_go(self, 3);
    }
    if (S32(self, 0x1624) != 3 && S32(self, 0x1624) != 2 && d < 900.0f) {
        animal_go(self, 2);
    }
    switch (S32(self, 0x1624)) {
    case 0:
        if (S32(self, 0x1628) != 0) {
            Motion_PlayWith(PTR(self, 0xF0), 0x9000, 1, -1, 5.0f);
            S32(self, 0x162C) = (s32)(60.0f * (1.0f + animal_rnd()));
            S32(self, 0x1628) = 0;
        }
        animal_face(self, F32(D_01991600, 0), F32(D_01991600, 8));
        S32(self, 0x162C) -= 1;
        if (S32(self, 0x162C) < 0) {
            if (d <= 2500.0f || !(animal_rnd() < k08.f)) {
                S32(self, 0x1624) = 1;
            } else {
                S32(self, 0x1624) = 4;
            }
            S32(self, 0x1628) = 1;
            S32(self, 0x162C) = 0;
        }
        break;
    case 1:
        if (S32(self, 0x1628) != 0) {
            Motion_PlayWith(PTR(self, 0xF0), 0x9001, 1, -1, 5.0f);
            S32(self, 0x1628) = 0;
        }
        if (ANIMAL_CLIP_DONE(self) && animal_rnd() < 0.5f) {
            animal_go(self, 0);
        }
        break;
    case 4:
        if (S32(self, 0x1628) != 0) {
            S32(self, 0x162C) = 5;
            S32(self, 0x1628) = 0;
        }
        animal_face(self, F32(self, 0x1634), F32(self, 0x1638));
        if (--S32(self, 0x162C) < 0) {
            animal_go(self, 5);
        }
        break;
    case 5:
        if (S32(self, 0x1628) != 0) {
            Motion_PlayWith(PTR(self, 0xF0), 0x9002, 1, -1, 5.0f);
            S32(self, 0x1628) = 0;
        }
        if (ANIMAL_CLIP_DONE(self)) {
            animal_go(self, 0);
        }
        break;
    case 2: {
        f32 away[4] __attribute__((aligned(16)));

        if (S32(self, 0x1628) != 0) {
            Motion_PlayWith(PTR(self, 0xF0), 0x9002, 1, -1, 5.0f);
            S32(self, 0x1628) = 0;
            Kind26_NearestDistSq(self, (f32 *)D_01991600);
        }
        sceVu0SubVector(away, CHAR_POS(self), (f32 *)D_01991600);
        sceVu0ScaleVector(away, away, 100.0f);
        sceVu0AddVector(away, away, CHAR_POS(self));
        animal_face(self, away[0], away[1]);   /* (sic: y for z - the game's own slip) */
        if (ANIMAL_CLIP_DONE(self)) {
            animal_go(self, 0);
        }
        break;
    }
    case 3: {
        f32 dz, dx;

        if (S32(self, 0x1628) != 0) {
            Motion_PlayWith(PTR(self, 0xF0), 0x9003, 1, -1, 5.0f);
            S32(self, 0x1628) = 0;
        }
        animal_face(self, F32(self, 0x163C), F32(self, 0x1640));
        dz = F32(self, 0x1640) - F32(self, 0x18);
        dx = F32(self, 0x163C) - F32(self, 0x10);
        if (dz * dz + dx * dx < 1.0f) {
            Actor_PlaySound((Actor *)self, S32(self, 0x1630) + 1, 6, 0, 0, NULL);
            return 1;
        }
        break;
    }
    }
    return 0;
}

/* a frame of it (Kind26_Behaviour) moved by its animation's root motion, a footstep sound
 * (+0x1630, unless -1) as either foot comes down; 1 once it has run off */
static inline __attribute__((always_inline)) s32 animal_step(u8 *self) {
    f32 d[4] __attribute__((aligned(16)));
    void *m;
    u8 done = Kind26_Behaviour(self) & 0xFF;
    u8 l, r;

    Motion_RootMovement(PTR(self, 0xF0), d, 0.0f);
    Character_RootTurn((Character *)self);
    sceVu0ApplyMatrix(d, (f32 (*)[4])(self + 0x60), d);
    sceVu0AddVector(CHAR_POS(self), CHAR_POS(self), d);
    m = PTR(self, 0xF0);
    l = VCALL(m, 0x64, s32 (*)(void *, s32, s32))(m, 1, 0) & 0xFF;
    m = PTR(self, 0xF0);
    r = VCALL(m, 0x64, s32 (*)(void *, s32, s32))(m, 0, 0) & 0xFF;
    if (((l == 1 && self[0x16A8] == 0) || (r == 1 && self[0x16A9] == 0)) && S32(self, 0x1630) != -1) {
        Actor_PlaySound((Actor *)self, S32(self, 0x1630), 6, 0, 0, NULL);
    }
    self[0x16A8] = l;
    self[0x16A9] = r;
    return done;
}

/* the two animals' copies (started by Kind26_MoveTo / Kind26_MoveToB) */
/* 0x0032D150 */
s32 Kind26_MoveDone(u8 *self) {
    return animal_step(self);
}

/* 0x0032D2C0 */
s32 Kind26_MoveDoneB(u8 *self) {
    return animal_step(self);
}

/* 0x0032DC50 */
void *RoomC0_EnterScript(void) {
    return D_0042CA20;
}

/* 0x0032DC60 */
void *RoomC0_CharEnterScript(void) {
    return D_0042CB60;
}

/* 0x0032DC70 */
void *RoomC0_Phase1Script(void) {
    return D_0042CC00;
}

/* 0x0032DC80 */
void *RoomC0_Phase2Script(void) {
    return D_0042D0C0;
}

/* 0x0032DC90 */
void *RoomC0_ActionScript(void *self, s32 i) {
    return D_0042E310[i];
}

/* 0x0032DCB0 */
void *RoomC0_Table38(void) {
    return D_0042E410;
}

/* 0x0032DCC0 */
void *RoomC0_ObjectName(void *self, s32 i) {
    return pstr_EV0023[i];
}

/* 0x0032F4E0 */
void ThingBurst_Start(u8 *self) {
    S32(self, 0xFC8) = 0;
    self[0xFCC] = 0;
    S32(self, 0xFC0) = 0;
    S32(self, 0xFC4) = 0;

    S64(self, 0xC18) = -1;
    S32(self, 0xC24) = 0;
    S32(self, 0xC28) = 0;
    S32(self, 0xC2C) = 0;
    S32(self, 0xC30) = 25;
    S16(self, 0xC34) = 0x10;
    S16(self, 0xC36) = 0x20;
    S16(self, 0xC38) = 0x40;
    S16(self, 0xC3A) = 0x20;
    S16(self, 0xC3C) = 0x20;
    S16(self, 0xC3E) = 0x200;
    S16(self, 0xC40) = 0x100;
    self[0xC42] = 0x40;
    self[0xC43] = 1;
    self[0xC44] = 1;
    self[0xC45] = 0x10;
    self[0xC46] = 0xFF;

    S64(self, 0xC50) = -1;
    S32(self, 0xC5C) = 0;
    S32(self, 0xC60) = 0;
    S32(self, 0xC64) = 0;
    S32(self, 0xC68) = 25;
    S16(self, 0xC6C) = 0x10;
    S16(self, 0xC6E) = 0xE;
    S16(self, 0xC70) = 0x6E;
    S16(self, 0xC72) = 4;
    S16(self, 0xC74) = 4;
    S16(self, 0xC76) = 0x200;
    S16(self, 0xC78) = 0x100;
    self[0xC7A] = 0x40;
    self[0xC7B] = 1;
    self[0xC7C] = 1;
    self[0xC7D] = 0x10;
    self[0xC7E] = 0xFF;
}

/* 0x0032F6C0 */
void *Kind27_MotionFiles(void) {
    return D_0042E4C0;
}

/* 0x0032DA60 */
void Kind26_ShowUp(Pursuer *p) { creature_inplay(p); }

/* 0x0032DAB0 */
void Kind26_EventState(Pursuer *p) { creature_act5(p, &Pursuer_StateRunThenNext_ptmf18); }

/* 0x0032DB80 */
s32 Kind26_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

/* Item26_vtable: at spot 3 of room 0x92: flag 0x18, event 4 */
/* 0x00351B40 */
s32 Item26_Use(void *o) {
    Progress *p = gProgress;

    if (!item_room_spot(p, 0x92, 3)) {
        return 0;
    }
    Progress_SetFlag(p, 0x18);
    item_event(gEvents, 0, 4, gCharPlayer);
    return 4;
}

/* no use here: unless Progress +0x30 bit 0x8000, while item `id` is held, a sound (bank 0xC,
   5); else nothing */
static s32 use_sound_only(s32 need_24_4, s32 id) {
    Progress *p = gProgress;

    if ((AT(p, 0x30, u32) & 0x8000) || (need_24_4 && !(AT(p, 0x24, u32) & 4)) ||
        VCALL(gSubScreen, 0xC, s32 (*)(VObject *, s32))(gSubScreen, id) == 0) {
        return 0;
    }
    VCALL(gSound, 0x14, void (*)(VObject *, s32, s32))(gSound, 0xC, 5);
    return 8;
}

/* ItemA4_vtable */
/* 0x00266370 */
s32 ItemA4_Use(void *o) {
    return use_sound_only(0, 0x24B);
}

/* Item06_vtable: at door 0 of room 6: event 1, flag 0x18; else (with Progress +0x24 bit 4) the
   sound while item 0x232 is held */
/* 0x002EEC00 */
s32 Item06_Use(void *o) {
    Progress *p = gProgress;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 6 && item_door_open(p, 0)) {
        item_event(gEvents, 0, 1, gCharPlayer);
        Progress_SetFlag(p, 0x18);
        return 4;
    }
    return use_sound_only(1, 0x232);
}

/* at spot 0x11 of room 0xF once route 8 is open (Progress_DoorUnlocked): event 0x16; else on the altar */
static s32 use_route8_or_offer(void *o) {
    Progress *p = gProgress;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0xF && Progress_DoorUnlocked(p, 8) != 0 &&
        item_at_spot(gEvents, gCharPlayer, 0x11)) {
        item_event(gEvents, 0, 0x16, gCharPlayer);
        return 4;
    }
    return item_offer(p, o);
}

/* Item12_vtable */
/* 0x00264A60 */
s32 Item12_Use(void *o) {
    return use_route8_or_offer(o);
}

/* 0x00264C10 */
s32 Item12_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM01_ITEM_02D_TEX, dest, 0x4000000, 0);
}

/* Item13_vtable */
/* 0x00264CE0 */
s32 Item13_Use(void *o) {
    return use_route8_or_offer(o);
}

/* 0x00264E90 */
s32 Item13_LoadPicture(void *self, void *dest) {
    return VCALL(gFileLoader, 0xC, LoaderLoadFn)(gFileLoader, str_ITEM01_ITEM_02E_TEX, dest, 0x4000000, 0);
}

/* Item07_vtable: at open door 1 of room 0x14: event 4, flag 0x18; else on the altar */
/* 0x00303CD0 */
s32 Item07_Use(void *o) {
    Progress *p = gProgress;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x14 && item_door_open(p, 1)) {
        item_event(gEvents, 0, 4, gCharPlayer);
        Progress_SetFlag(p, 0x18);
        return 4;
    }
    return item_offer(p, o);
}

/* at spot 0xB of room 0xC7: event 3, or 0x10 with Progress +0x2C bit 0x4000 */
static s32 use_room_c7(void) {
    Progress *p = gProgress;

    if (!item_room_spot(p, 0xC7, 0xB)) {
        return 0;
    }
    item_event(gEvents, 0, (AT(p, 0x2C, u32) & 0x4000) ? 0x10 : 3, gCharPlayer);
    return 4;
}

/* Item21_vtable */
/* 0x00351390 */
s32 Item21_Use(void *o) {
    return use_room_c7();
}

/* Item22_vtable */
/* 0x00351550 */
s32 Item22_Use(void *o) {
    return use_room_c7();
}

/* Item25_vtable: at room 0x82's open door 0 with route 0xE2: event 2; at room 0x8C's with route
   0xE6: event 8 */
/* 0x00351920 */
s32 Item25_Use(void *o) {
    Progress *p = gProgress;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x82) {
        return item_door_route(p, 0, 0xE2) ? item_event_flag(p, 2) : 0;
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x8C && item_door_route(p, 0, 0xE6)) {
        return item_event_flag(p, 8);
    }
    return 0;
}

/* (as Item28_Use) at spot 3 of room 0x53: event 0; else on the altar */
/* 0x0031CE10 */
s32 Item0E_Use(void *o) {
    Progress *p = gProgress;

    if (item_room_spot(p, 0x53, 3)) {
        return item_event_flag(p, 0);
    }
    return item_offer(p, o);
}

/* Item28_vtable: at spot 5 of room 0: event 0xF; else on the altar */
/* 0x00351D80 */
s32 Item28_Use(void *o) {
    Progress *p = gProgress;

    if (item_room_spot(p, 0, 5)) {
        return item_event_flag(p, 0xF);
    }
    return item_offer(p, o);
}

/* 0x00351F20 */
Character *Kind38_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind38_vtable); }

/* 0x00352030 */
void *Kind38_ModelFiles(void) { return D_004434D0; }

/* 0x00352040 */
void *Kind38_MotionFiles(void) { return D_00443510; }

/* 0x00352060 */
void Kind38_ShowUp(Pursuer *p) { creature_inplay(p); }

/* 0x003520B0 */
void Kind38_EventState(Pursuer *p) { creature_act5(p, &Pursuer_StateRunThenNext_ptmf25); }

/* 0x00352180 */
s32 Kind38_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

/* Item0D_vtable: in room 0x4B, at open door 2 with route 0x51: event 1; door 3 with route 0x52:
   event 3; else on the altar */
/* 0x0031CB40 */
s32 Item0D_Use(void *o) {
    Progress *p = gProgress;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x4B) {
        if (item_door_route(p, 2, 0x51)) {
            return item_event_flag(p, 1);
        }
        if (item_door_route(p, 3, 0x52)) {
            return item_event_flag(p, 3);
        }
    }
    return item_offer(p, o);
}

/* where one of the five medallions (Item08_vtable ..) is placed: room, spot, the events' +0x30
 * mode (-1: not called) and Fiona's event; room 0x40's only without Progress +0x2C bit 0x20 */
typedef struct ItemSlot {
    u8 room, spot;
    s8 mode;
    u8 ev;
} ItemSlot;

static s32 place_item(void *o, const ItemSlot *t, s32 n) {
    Progress *p = gProgress;
    s32 i;

    for (i = 0; i < n; i++, t++) {
        if (VCALL(p, 0xC, s32 (*)(Progress *))(p) != t->room) {
            continue;
        }
        if (t->room == 0x40 && (AT(p, 0x2C, u32) & 0x20)) {
            continue;
        }
        if (!item_at_spot(gEvents, gCharPlayer, t->spot)) {
            continue;
        }
        if (t->mode >= 0) {
            VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, t->mode, AT(o, 0x4, s32));
        }
        return item_event_flag(p, t->ev);
    }
    return 0;
}

/* 0x00306BC0 */
s32 Item08_Use(void *o) {
    static const ItemSlot t[7] = {
        {0x40, 0x11, 0, 1}, {0x42, 5, 1, 1}, {0x56, 4, 0, 0xA}, {0x57, 0xD, 1, 3},
        {0x69, 0xB, 1, 4},  {0x47, 8, -1, 1}, {0xC0, 0x1B, 1, 0x14},
    };

    return place_item(o, t, 7);
}

/* 0x003071D0 */
s32 Item09_Use(void *o) {
    static const ItemSlot t[7] = {
        {0x40, 0x11, 0, 1}, {0x42, 5, 1, 1},   {0x47, 8, 0, 2}, {0x56, 4, 0, 0xA},
        {0x69, 0xB, 1, 4},  {0x57, 0xD, -1, 0}, {0xC0, 0x1B, 1, 0x14},
    };

    return place_item(o, t, 7);
}

/* 0x003077E0 */
s32 Item0A_Use(void *o) {
    static const ItemSlot t[7] = {
        {0x40, 0x11, 0, 1}, {0x42, 5, 1, 1},   {0x47, 8, 0, 2}, {0x56, 4, 0, 0xA},
        {0x57, 0xD, 1, 3},  {0x69, 0xB, -1, 0}, {0xC0, 0x1B, 1, 0x14},
    };

    return place_item(o, t, 7);
}

/* 0x00307DF0 */
s32 Item0B_Use(void *o) {
    static const ItemSlot t[7] = {
        {0x47, 8, 0, 2},    {0x56, 4, 0, 0xA}, {0x57, 0xD, 1, 3}, {0x69, 0xB, 1, 4},
        {0x40, 0x11, 0, 0}, {0x42, 5, -1, 0},  {0xC0, 0x1B, 1, 0x14},
    };

    return place_item(o, t, 7);
}

/* 0x00308400 */
s32 Item0C_Use(void *o) {
    static const ItemSlot t[7] = {
        {0x42, 5, 1, 1},   {0x47, 8, 0, 2},   {0x56, 4, 0, 0xA},    {0x57, 0xD, 1, 3},
        {0x69, 0xB, 1, 4}, {0x40, 0x11, 0, 0}, {0xC0, 0x1B, 1, 0x14},
    };

    return place_item(o, t, 7);
}

/* the three statues' pedestals (room 0x54, spots 0xD / 0x11 / 0x14): a pedestal takes the
 * statue while neither of its two Progress +0x28 bits is set; else on the altar */
typedef struct ItemPedestal {
    u32 taken;   /* +0x28 bits that block it */
    u8 spot, ev;
} ItemPedestal;

static s32 place_statue(void *o, const ItemPedestal *t) {
    Progress *p = gProgress;
    s32 i;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x54) {
        for (i = 0; i < 3; i++, t++) {
            if (!(AT(p, 0x28, u32) & t->taken) && item_at_spot(gEvents, gCharPlayer, t->spot)) {
                return item_event_flag(p, t->ev);
            }
        }
    }
    return item_offer(p, o);
}

/* Item15_vtable */
/* 0x00338FA0 */
s32 Item15_Use(void *o) {
    static const ItemPedestal t[3] = {{0x180, 0xD, 0}, {0xC00, 0x11, 3}, {0x6000, 0x14, 6}};

    return place_statue(o, t);
}

/* Item16_vtable */
/* 0x00339350 */
s32 Item16_Use(void *o) {
    static const ItemPedestal t[3] = {{0xA00, 0x11, 4}, {0x140, 0xD, 1}, {0x5000, 0x14, 7}};

    return place_statue(o, t);
}

/* Item17_vtable */
/* 0x00339700 */
s32 Item17_Use(void *o) {
    static const ItemPedestal t[3] = {{0x3000, 0x14, 8}, {0xC0, 0xD, 2}, {0x600, 0x11, 5}};

    return place_statue(o, t);
}

/* 0x00339A10 */
Character *Kind28_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind28_vtable); }

/* 0x00339B20 */
void *Kind28_ModelFiles(void) {
    return D_0042F470;
}

/* 0x00339B30 */
void *Kind28_MotionFiles(void) {
    return D_0042F4B0;
}

/* 0x00339B50 */
void Kind28_ShowUp(Pursuer *p) { creature_inplay(p); }

/* 0x00339BA0 */
void Kind28_EventState(Pursuer *p) { creature_act5(p, &Pursuer_StateRunThenNext_ptmf19); }

/* 0x00339C70 */
s32 Kind28_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

/* Item10_vtable: the medallions' slots without the altar */
/* 0x0031D1B0 */
s32 Item10_Use(void *o) {
    static const ItemSlot t[5] = {
        {0x42, 5, 1, 1}, {0x47, 8, 0, 2}, {0x57, 0xD, 1, 3}, {0x69, 0xB, 1, 4}, {0x56, 4, -1, 9},
    };

    return place_item(o, t, 5);
}

/* Item1D_vtable: in room 0x52 with Hewie at hand (+0x40), up and within 20, Fiona not busy
 * (+0xE8): event 0xB; else on the altar; else the sound while item 0x239 is held, or (with
 * Progress +0x30 bit 0x8000) while Hewie is in the room being played and item 0x27F is held */
/* 0x003445D0 */
s32 Item1D_Use(void *o) {
    Progress *p = gProgress;
    VObject *ev_mgr = gEvents;
    s32 r;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x52) {
        u8 *h = (u8 *)gCharPartner;

        if (VCALL(o, 0x40, s32 (*)(void *))(o) != 0 && AT(h, 0xC4, s32) != 2 && AT(gCharPlayer, 0xE8, s32) == 0 &&
            Actor_Distance((Actor *)gCharPlayer, (f32 *)(h + 0x10)) < 20.0f) {
            item_event(ev_mgr, 0, 0xB, gCharPlayer);
            return 4;
        }
    }
    r = item_offer(p, o);
    if (r != 0) {
        return r;
    }
    if (!(AT(p, 0x30, u32) & 0x8000)) {
        return use_sound_only(0, 0x239);
    }
    {
        s32 room = AT(gCharPartner, 0x30, s32);

        if (room == VCALL(p, 0xC, s32 (*)(Progress *))(p) &&
            VCALL(gSubScreen, 0xC, s32 (*)(VObject *, s32))(gSubScreen, 0x27F) != 0) {
            VCALL(gSound, 0x14, void (*)(VObject *, s32, s32))(gSound, 0xC, 5);
            return 8;
        }
    }
    return 0;
}

/* free the file loader's 0x4000000 area if it is in state 2 */
/* 0x002600C0 */
void Loader_FreePictureArea(void) {
    VObject *ld = gFileLoader;

    if (VCALL(ld, 0x28, s32 (*)(VObject *, s32))(ld, 0x4000000) == 2) {
        VCALL(ld, 0x14, void (*)(VObject *, s32))(ld, 0x4000000);
    }
}

/* an item's +0x18 set from a message { kind (0xFF: none), sub, byte 2, pad, word }: +0x4 0 when
 * unset (+0x5 0xFF), +0x7 the kind, +0x5 the sub (0xFF without a kind), +0x6, +0xC; +0x8 0 */
/* 0x0035BC30 */
void ObjectGlow_SetParams(u8 *o, const u8 *m) {
    if (m == NULL) {
        return;
    }
    if (o[5] == 0xFF) {
        o[4] = 0;
    }
    o[7] = m[0];
    if (o[7] == 0xFF) {
        o[5] = 0xFF;
    } else {
        o[5] = m[1];
    }
    o[6] = m[2];
    AT(o, 0xC, s32) = AT(m, 0x4, s32);
    AT(o, 0x8, s32) = 0;
}

extern void Sound_PlayBankAt(VObject *snd, u32 id, u32 bank, f32 *pos, s32 vol, s32 pitch);

/* the room-object glow (vtable ObjectGlow_vtable, made by RoomC5_Cmd00) +0x10 update: 0 while unset
 * (+0x5 0xFF). On (+0x7) it fades in (+0x4 up to 0x40 by 2), off it fades out; at 0 it is unset,
 * frees its script variable (+0x6, events +0x30) and plays sound 6 at the object (+0xC, +0x20)
 * unless the camera director's +0x38 is set - all 0. Its phase +0x8 runs 0..45 by 2 a frame */
/* 0x0035C8C0 */
s32 ObjectGlow_Update(u8 *o) {
    f32 t;

    if (o[5] == 0xFF) {
        return 0;
    }
    if (o[7] != 0) {
        if (o[4] != 0x40) {
            o[4] += 2;
        }
    } else {
        o[4] -= 2;
        if (o[4] == 0) {
            o[5] = 0xFF;
            VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, o[6], -1);
            if (VCALL(gCamDirector, 0x38, s32 (*)(VObject *))(gCamDirector) == 0) {
                Sound_PlayBankAt(gSound, 2, 6, (f32 *)(AT(o, 0xC, u8 *) + 0x20), 0, 0);
            }
            return 0;
        }
    }
    t = AT(o, 0x8, f32) + 2.0f;
    AT(o, 0x8, f32) = t;
    if (!(t < 45.0f)) {
        AT(o, 0x8, f32) = t - 45.0f;
    }
    return 1;
}

extern const char D_0045D818[], D_0045D820[], D_0045D828[], D_0045D838[], D_0045D840[], D_0045D848[],
    D_0045D850[], D_0045D858[], D_0045D860[], D_0045D870[], D_0045D878[], D_0045D880[];

/* the word item `it` carries (vtable +0x20) is `w` (the first 8 letters) */
static inline s32 item_named(VObject *it, const char *w) {
    const char *n = VCALL(it, 0x20, const char *(*)(VObject *))(it);
    const char *q = w;
    u32 i;

    for (i = 0; i < 8; i++, q++) {
        if (n[i] != *q) {
            return 0;
        }
        if (n[i] == 0) {
            break;
        }
    }
    return 1;
}

/* the elements' flag `bit` (Progress +0x1C bits) set */
static inline void item_progress_bit(Progress *p, s32 bit) {
    AT(p, 0x1C + (bit >> 5) * 4, u32) |= 1 << (bit & 0x1F);
}

/* +0x3C use (the word plate, vtable ItemClassF430_vtable): what Fiona does with the word it carries, room
 * by room (6: an event started) -
 *  0x23, spot 3: event 5
 *  0x24, spot 6, until progress +0x1C bit 27: "EMETH" (bit 26, event 0xA) / "METH" (bit 25,
 *        event 0xB) / "SALTATIO" (event 0xE), each with flag 0x18; anything else event 5
 *  0x26, spot 0xA, until +0x7C bit 12: "MAGNUS" event 0xB (flag 0x18), else 0xC
 *  0x0F, route 8 taken, spot 0x11: "REST" event 0x14 (flag 0x18), else 0x17
 *  0x49, spot 2: event 1
 *  0x66: spot 0x14 event 9; spot 0x16 "SALTATIO" event 0x11, else the word to the events (+0x30
 *        0 / 1) and event 0xA
 *  0x4F, until +0x24 bit 16: the elements "MERCURY" / "SULFUR" / "SALT" (flags 0x4D..0x4F, done:
 *        +0x24 bits 13..15). With two done and Hewie with her (or at spot 0xA): event 5, flag
 *        0x18; else spot 0xA event 0xA (4); spots 8 / 9 (until +0x24 bits 17 / 18) events 6 / 7.
 *        The word given: its flag and the events' +0x5C, else +0x60
 *  0x27 with +0x30 bit 14, spot 3: "ALCHYMIA" / "ADAMAS" / "POWDER" / "MORGAN" (not yet used:
 *        +0x88 bits 8..11) to the events (+0x30 1: 0x88 / 0x8C / 0x83 / 0x89) and event 8, else 9
 * elsewhere 0 */
/* 0x002D27E0 */
s32 ItemClassF430_Use(VObject *it) {
    Progress *p = gProgress;
    VObject *ev = gEvents;

    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x23 && item_at_spot(ev, gCharPlayer, 3)) {
        item_event(ev, 0, 5, gCharPlayer);
        return 6;
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x24 && !(AT(p, 0x1C, u32) & 0x8000000) &&
        item_at_spot(ev, gCharPlayer, 6)) {
        if (item_named(it, D_0045D818)) {
            AT(p, 0x1C, u32) |= 0x4000000;
            item_event(ev, 0, 0xA, gCharPlayer);
            Progress_SetFlag(p, 0x18);
        } else if (item_named(it, D_0045D820)) {
            AT(p, 0x1C, u32) |= 0x2000000;
            item_event(ev, 0, 0xB, gCharPlayer);
            Progress_SetFlag(p, 0x18);
        } else if (item_named(it, D_0045D828)) {
            item_event(ev, 0, 0xE, gCharPlayer);
            Progress_SetFlag(p, 0x18);
        } else {
            item_event(ev, 0, 5, gCharPlayer);
        }
        return 6;
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x26 && !(AT(p, 0x7C, u32) & 0x1000) &&
        item_at_spot(ev, gCharPlayer, 0xA)) {
        if (item_named(it, D_0045D838)) {
            item_event(ev, 0, 0xB, gCharPlayer);
            Progress_SetFlag(p, 0x18);
        } else {
            item_event(ev, 0, 0xC, gCharPlayer);
        }
        return 6;
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0xF && Progress_DoorUnlocked(p, 8) != 0 &&
        item_at_spot(ev, gCharPlayer, 0x11)) {
        if (item_named(it, D_0045D840)) {
            item_event(ev, 0, 0x14, gCharPlayer);
            Progress_SetFlag(p, 0x18);
        } else {
            item_event(ev, 0, 0x17, gCharPlayer);
        }
        return 6;
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x49 && item_at_spot(ev, gCharPlayer, 2)) {
        item_event(ev, 0, 1, gCharPlayer);
        return 6;
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x66) {
        if (item_at_spot(ev, gCharPlayer, 0x14)) {
            item_event(ev, 0, 9, gCharPlayer);
            return 6;
        }
        if (item_at_spot(ev, gCharPlayer, 0x16)) {
            if (item_named(it, D_0045D828)) {
                item_event(ev, 0, 0x11, gCharPlayer);
            } else {
                const s32 *w = VCALL(it, 0x20, const s32 *(*)(VObject *))(it);

                VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 0, w[0]);
                VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, w[1]);
                item_event(ev, 0, 0xA, gCharPlayer);
            }
            return 6;
        }
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x4F && !(AT(p, 0x24, u32) & 0x10000)) {
        s32 given = 0, done = 0, bit = -1;

        if (AT(p, 0x24, u32) & 0x2000) {
            done = 1;
        } else if (item_named(it, D_0045D848)) {
            given = 1;
            bit = 0x4D;
        }
        if (AT(p, 0x24, u32) & 0x4000) {
            done++;
        } else if (item_named(it, D_0045D850)) {
            given = 1;
            bit = 0x4E;
        }
        if (AT(p, 0x24, u32) & 0x8000) {
            done++;
        } else if (item_named(it, D_0045D858)) {
            given = 1;
            bit = 0x4F;
        }
        if (done == 2) {
            u8 *h = (u8 *)gCharPartner;

            if (h != NULL && AT(h, 0x28, u8) == 1 && AT(h, 0x30, s32) == VCALL(p, 0xC, s32 (*)(Progress *))(p) &&
                AT(h, 0xC4, s32) != 2 &&
                (item_at_spot(ev, gCharPlayer, 0xA) || Actor_Distance((Actor *)gCharPlayer, (f32 *)(h + 0x10)) < 20.0f)) {
                item_event(ev, 0, 5, gCharPlayer);
                Progress_SetFlag(p, 0x18);
                if (!given) {
                    VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 0);
                } else {
                    item_progress_bit(p, bit);
                    VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 0);
                }
                return 6;
            }
            if (item_at_spot(ev, gCharPlayer, 0xA)) {
                item_event(ev, 0, 0xA, gCharPlayer);
                return 4;
            }
        } else if (item_at_spot(ev, gCharPlayer, 8) && !(AT(p, 0x24, u32) & 0x20000)) {
            item_event(ev, 0, 6, gCharPlayer);
            if (!given) {
                VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 0);
            } else {
                item_progress_bit(p, bit);
                Progress_SetFlag(p, 0x18);
                VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 0);
            }
            return 6;
        } else if (item_at_spot(ev, gCharPlayer, 9) && !(AT(p, 0x24, u32) & 0x40000)) {
            item_event(ev, 0, 7, gCharPlayer);
            if (!given) {
                VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 0);
            } else {
                item_progress_bit(p, bit);
                Progress_SetFlag(p, 0x18);
                VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 0);
            }
            return 6;
        }
    }
    if (VCALL(p, 0xC, s32 (*)(Progress *))(p) == 0x27 && (AT(p, 0x30, u32) & 0x4000) &&
        item_at_spot(ev, gCharPlayer, 3)) {
        if (item_named(it, D_0045D860) && !(AT(p, 0x88, u32) & 0x100)) {
            VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, 0x88);
            item_event(ev, 0, 8, gCharPlayer);
        } else if (item_named(it, D_0045D870) && !(AT(p, 0x88, u32) & 0x200)) {
            VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, 0x8C);
            item_event(ev, 0, 8, gCharPlayer);
        } else if (item_named(it, D_0045D878) && !(AT(p, 0x88, u32) & 0x400)) {
            VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, 0x83);
            item_event(ev, 0, 8, gCharPlayer);
        } else if (item_named(it, D_0045D880) && !(AT(p, 0x88, u32) & 0x800)) {
            VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, 0x89);
            item_event(ev, 0, 8, gCharPlayer);
        } else {
            item_event(ev, 0, 9, gCharPlayer);
        }
        return 6;
    }
    return 0;
}

/* 0x002D3A20 */
s32 ItemClassF430_LoadPicture(void *self, void *dest) { return LOAD_002D1360(str_ITEM00_ITEM_008_TEX, dest); }

#ifdef HG_NATIVE

extern void *Bloom_vtable[], *Helper469D00_vtable[];
extern f32 msl_cosf(f32 x);   /* cosf */
extern f32 msl_sinf(f32 x);   /* sinf */

/* is clip-space point `p` (camera +0x48) inside the view */
static s32 glow_in_view(f32 (*clip)[4], const f32 *pt) {
    f32 v[4] __attribute__((aligned(16)));

    sceVu0ApplyMatrix(v, clip, (f32 *)pt);
    return v[0] <= v[3] && !(v[0] < -v[3]) && v[1] <= v[3] && !(v[1] < -v[3]) && v[2] <= v[3] && !(v[2] < -v[3]);
}

/* the glow +0x14 draw (while set): around its room object (+0xC, at floor height -1)
 * - a disc of radius r (by its kind +0x5) fading out from the middle (alpha +0x4), added into
 *   layer 0x26 (the bloom's mask); only when all of it is in view (kind 0: the disc left out
 *   then, and the screen also brightened, Bloom_Start 0x40808080 in layer 0x28)
 * - eight faint cyan strips standing 40 high at radius r + 5, turning with its phase (+0x8),
 *   added in layer 2, when all in view */
/* 0x0035BCA0 */
void ObjectGlow_Draw(u8 *o) {
    static const union { u32 u; f32 f; } kRadius[8] = {
        {0x409A3D71}, {0x40800000}, {0x407F5C29}, {0x409D70A4}, {0x408D1EB8}, {0x407C28F6}, {0x405E147B}, {0x411A3D71},
    };
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    f32 clip[4][4] __attribute__((aligned(16)));
    f32 pt[18][4] __attribute__((aligned(16)));
    u8 *obj = AT(o, 0xC, u8 *);
    f32 c[4], r, r2;
    s32 i, clipped = 0;

    if (o[5] == 0xFF) {
        return;
    }
    c[0] = AT(obj, 0x20, f32);
    c[1] = -1.0f;
    c[2] = AT(obj, 0x28, f32);
    c[3] = 1.0f;
    r = o[5] < 8 ? kRadius[o[5]].f : 0.0f;
    VCALL(gCamera, 0x48, void (*)(VObject *, f32 (*)[4]))(gCamera, clip);

    /* the disc: the middle, then 16 points round it */
    sceVu0CopyVector(pt[0], c);
    for (i = 0; i < 16; i++) {
        f32 a = kPi.f * (22.5f * (f32)i) / 180.0f;

        pt[i + 1][0] = c[0] + r * msl_sinf(a);
        pt[i + 1][1] = c[1];
        pt[i + 1][2] = c[2] - r * msl_cosf(a);
        pt[i + 1][3] = 1.0f;
    }
    for (i = 0; i < 17; i++) {
        if (!glow_in_view(clip, pt[i])) {
            if (o[5] != 0) {
                return;
            }
            clipped = 1;
        }
    }
    if (o[5] == 0) {
        u8 drawer[0x20] __attribute__((aligned(16)));

        AT(drawer, 0x0, void **) = Bloom_vtable;
        AT(drawer, 0x4, s32) = -1;
        Bloom_Start(drawer, 0x40808080, 0x28, 0);
        AT(drawer, 0x0, void **) = Helper469D00_vtable;
    }
    if (!clipped) {
        f32 tri[3][4];
        f32 st[3][2] = {{0}};
        u8 col[3][4] = {{0x80, 0x80, 0x80, 0}, {0}, {0}};

        col[0][3] = o[4];
        glr_layer(0x26);
        for (i = 0; i < 16; i++) {   /* the fan, closing on its first rim point */
            sceVu0CopyVector(tri[0], pt[0]);
            sceVu0CopyVector(tri[1], pt[i + 1]);
            sceVu0CopyVector(tri[2], pt[(i + 1) % 16 + 1]);
            AT(&tri[0][3], 0, u32) = AT(&tri[1][3], 0, u32) = AT(&tri[2][3], 0, u32) = 0;
            glr_strip((const f32 *)clip, 3, &tri[0][0], &st[0][0], &col[0][0], NULL, 0,
                      0x40 | GLR_PRIM_ADD | GLR_PRIM_NOZW);
        }
        glr_layer(-1);
    }

    /* the strips: the middle at the bottom and 40 up, then 8 pairs round it */
    r2 = 5.0f + r;
    sceVu0CopyVector(pt[0], c);
    sceVu0CopyVector(pt[1], c);
    pt[1][1] += 40.0f;
    for (i = 0; i < 8; i++) {
        f32 a = kPi.f * (45.0f * (f32)i + AT(o, 0x8, f32)) / 180.0f;

        sceVu0CopyVector(pt[2 + i * 2], c);
        pt[2 + i * 2][0] = c[0] + r2 * msl_sinf(a);
        pt[2 + i * 2][2] = c[2] - r2 * msl_cosf(a);
        sceVu0CopyVector(pt[3 + i * 2], pt[2 + i * 2]);
        pt[3 + i * 2][1] += 40.0f;
    }
    for (i = 0; i < 18; i++) {
        if (!glow_in_view(clip, pt[i])) {
            return;
        }
    }
    glr_layer(2);
    for (i = 0; i < 8; i++) {
        static const u8 kCol[4][4] = {{0x00, 0x20, 0x20, 0x10}, {0, 0, 0x40, 0}, {0, 0, 0x20, 0}, {0, 0, 0x10, 0}};
        f32 q[4][4];
        f32 st[4][2] = {{0}};
        s32 k;

        sceVu0CopyVector(q[0], pt[0]);
        sceVu0CopyVector(q[1], pt[2 + i * 2]);
        sceVu0CopyVector(q[2], pt[1]);
        sceVu0CopyVector(q[3], pt[3 + i * 2]);
        for (k = 0; k < 4; k++) {
            AT(&q[k][3], 0, u32) = 0;
        }
        glr_strip((const f32 *)clip, 4, &q[0][0], &st[0][0], &kCol[0][0], NULL, 0, 0x40 | GLR_PRIM_ADD | GLR_PRIM_NOZW);
    }
    glr_layer(-1);
}
#endif

/* progress +0x84 bits 22..26 as bits 0..4 */
/* 0x00303F00 */
u8 Progress_MapsHeld(void) {
    u32 b = AT(gProgress, 0x84, u32);
    u8 v = 0;

    if (b & 0x400000) {
        v |= 1;
    }
    if (b & 0x800000) {
        v |= 2;
    }
    if (b & 0x1000000) {
        v |= 4;
    }
    if (b & 0x2000000) {
        v |= 8;
    }
    if (b & 0x4000000) {
        v |= 0x10;
    }
    return v;
}

/* 0x0033ABA0 */
Character *Kind29_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind29_vtable); }

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
void Kind29_EventState(Pursuer *p) { creature_act5(p, &Pursuer_StateRunThenNext_ptmf20); }

/* 0x0033AE00 */
s32 Kind29_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

/* 0x0033AF20 */
Character *Kind30_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind30_vtable); }

/* 0x0033B030 */
void *Kind30_ModelFiles(void) {
    return D_004308B0;
}

/* 0x0033B040 */
void *Kind30_MotionFiles(void) {
    return D_004308F0;
}

/* 0x0033B060 */
void Kind30_ShowUp(Pursuer *p) { creature_inplay(p); }

/* 0x0033B0B0 */
void Kind30_EventState(Pursuer *p) { creature_act5(p, &Pursuer_StateRunThenNext_ptmf21); }

/* 0x0033B180 */
s32 Kind30_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

/* 0x0033B1F0 */
Character *Kind31_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind31_vtable); }

/* 0x0033B300 */
void *Kind31_ModelFiles(void) {
    return D_00430940;
}

/* 0x0033B310 */
void *Kind31_MotionFiles(void) {
    return D_00430980;
}

/* 0x0033B330 */
void Kind31_ShowUp(Pursuer *p) { creature_inplay(p); }

/* 0x0033B380 */
void Kind31_EventState(Pursuer *p) { creature_act5(p, &Pursuer_StateRunThenNext_ptmf22); }

/* 0x0033B450 */
s32 Kind31_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

/* 0x0033D6C0 */
Character *Kind32_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind32_vtable); }

/* 0x0033D7D0 */
void *Kind32_ModelFiles(void) {
    return pstr_O_FIM_FIM_200_PCK;
}

/* 0x0033D7E0 */
void *Kind32_MotionFiles(void) {
    return D_00430A50;
}

/* 0x0033D800 */
void Kind32_ShowUp(Pursuer *p) { creature_inplay(p); }

/* 0x0033D850 */
void Kind32_EventState(Pursuer *p) { creature_act5(p, &Pursuer_StateRunThenNext_ptmf23); }

/* 0x0033D920 */
s32 Kind32_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

/* +0x8 destructor */
/* 0x0033E620 */
void *Kind18Model_dtor(void *p, s32 flags) {
    u8 *m = p;

    if (m != NULL) {
        AT(m, 0x0, void **) = Kind18Model_vtable;
        if (m != NULL) {
            AT(m, 0x0, void **) = EventHumanModel_vtable;
            Part_delete(m + 0x1230, -1);
            BoneHangPoint_dtor(m + 0x11E0, -1);
            Part_delete(m + 0x11A0, -1);
            SprungPoint_dtor(m + 0x1140, -1);
            Part_delete(m + 0x1100, -1);
            __destroy_arr(m + 0xE40, HangPoint_dtor, 0x50, 4);
            Part_delete(m + 0xE00, -1);
            BoneHangPoint_dtor(m + 0xDB0, -1);
            Part_delete(m + 0xD70, -1);
            __destroy_arr(m + 0x9B0, SwayPointA_dtor, 0x50, 0xC);
            CharModel_dtor(m, 0);
        }
        if ((s16)flags > 0) {
            StalkerModel_delete(m);
        }
    }
    return m;
}

/* 0x0033E730 */
void Kind18Model_ShowParts(void) {
}

/* +0x10 */
/* 0x0033E740 */
void Kind18Model_Frame(u8 *m) {
    Model_Release(m);
}

/* +0xC loaded (as EventHumanModel_Loaded): the base setup, the parts' roles, her own setup, per-part draw
 * settings for parts 0xA0..0xA8, 0xAE..0xB4 and 0xD2..0xD6 */
/* 0x0033E750 */
void Kind18Model_Loaded(u8 *m) {
    static const u8 sParts[] = {0xA0, 0xA2, 0xA4, 0xA6, 0xA8, 0xAE, 0xB0, 0xB2, 0xB4, 0xD2, 0xD4, 0xD6};
    s32 i;

    CharModel_Loaded(m);
    AT(m, 0x890, s32) = 2;
    AT(m, 0x894, s32) = 3;
    AT(m, 0x898, s32) = 4;
    AT(m, 0x89C, s32) = 5;
    AT(m, 0x8B8, s32) = 0x1E;
    AT(m, 0x8A0, s32) = 6;
    AT(m, 0x8A4, s32) = 7;
    AT(m, 0x8A8, s32) = 8;
    AT(m, 0x8AC, s32) = 9;
    AT(m, 0x8BC, s32) = 0x2E;
    AT(m, 0x8B0, s32) = 0x21;
    AT(m, 0x8B4, s32) = 0x17;
    EventHumanModel_Setup(m);
    for (i = 0; i < 12; i++) {
        AT(m, sParts[i], u8) = 4;
        AT(m, sParts[i] + 1, u8) = 0x40;
    }
}

/* 0x00345100 */
Character *Kind33_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, Kind33_vtable); }

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
void Kind33_EventState(Pursuer *p) { creature_act5(p, &Pursuer_StateRunThenNext_ptmf24); }

/* 0x00345360 */
s32 Kind33_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

/* model classes Kind14Model_vtable / Kind33Model_vtable over the plain one (HumanModel_BaseCtor) */
/* 0x0038C890 */
void *Kind14Model_ctor(u8 *m) {
    HumanModel_BaseCtor(m);
    AT(m, 0x0, void **) = Kind14Model_vtable;
    return m;
}

/* 0x0038C8D0 */
void *Kind33Model_ctor(u8 *m) {
    HumanModel_BaseCtor(m);
    AT(m, 0x0, void **) = Kind33Model_vtable;
    return m;
}

/* a model on the full base with its two parts, 6 parts (0x50, +0x9A0), 6 members (0x40,
 * +0xB80) and 24 more parts (0x50, +0xD40), vtable Kind09Model_vtable */
/* 0x0038C9E0 */
void *Kind09Model_ctor(u8 *m) {
    u8 *e;

    ModelBase_ctor(m);
    AT(m, 0x0, void **) = HumanModel_vtable;
    IK2_ctor(m + 0x8D0);
    IK2_ctor(m + 0x930);
    AT(m, 0x0, void **) = Kind09Model_vtable;
    __construct_array(m + 0x9A0, HangPoint_ctor, HangPoint_dtor, 0x50, 6);
    for (e = m + 0xB80; e < m + 0xD00; e += 0x40) {
        AT(e, 0x30, void **) = BonePoint_vtable;
    }
    AT(m, 0xD34, s32) = 0;
    AT(m, 0xD30, s32) = 0;
    __construct_array(m + 0xD40, SwayPointB_ctor, SwayPointB_dtor, 0x50, 0x18);
    AT(m, 0x14F4, s32) = 0;
    AT(m, 0x14F0, s32) = 0;
    return m;
}
