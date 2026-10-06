/* The creatures (character slots 7..9, 0x1600 bytes; their manager is gCreatures): Character
 * with their own block at +0x1540. Two classes: D_0046FAA0 (here) and D_00474080, both on the
 * creature base D_0046FB50. Their state is kept across rooms in Progress +0x878 (36 bytes per
 * slot, func_002DE840). */
#include "common.h"
#include "game.h"
#include "actor.h"
#include "progress.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "pursuer.h"
#include "creature.h"
#include "effects.h"
#include "hewie.h"
#include "model.h"
#include "pursuer_ai.h"
#include "scene_game_members.h"
#include "skeleton.h"
#include "stalker_math.h"
#include "stalker_models.h"
#include "stalker_progress.h"
#include "msl.h"

extern void *D_0046FAA0[], *D_0046FB50[], *D_00469C60[], *D_00469C20[];

#define CR(c) ((u8 *)(c) + 0x1540)   /* the creature's own block */

/* ---- the creature base (D_0046FB50): defaults ---- */

extern PTMF D_01990D40[];
s32 Room4F_Command(void *self, u32 i, s32 a, s32 b);

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

void func_002DE510(u8 *p, s32 a1, u32 v);
s32 func_002DE830(u8 *p);
void func_002DE840(u8 *p, s32 slot, u32 v);
void *func_002E2220(u8 *p);

/* Field access by byte offset into objects whose layout is not yet known. */
#define S16(p, off) (*(s16 *)((u8 *)(p) + (off)))

#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

#define S64(p, off) (*(s64 *)((u8 *)(p) + (off)))

void StrandSplash_Start(u8 *self);

#define U32(p, off) (*(u32 *)((u8 *)(p) + (off)))

#define F32(p, off) (*(f32 *)((u8 *)(p) + (off)))

void Effect737D0_SetParams(u8 *self, u8 *src);
void Effect737D0_Start(u8 *self);

void Effect737D0_Draw(u8 *o);
void func_0032A890(Character *c);
void func_0032C000(Character *c);

s32 Effect737D0_Update(void);
void func_00325D60(void);
void func_0032A0D0(void);

extern u8 D_0042C6A0[];
extern u8 D_0042C6E0[];
void func_00324790(u8 *self, s32 unused, u32 v);
s32 func_003247C0(u8 *self);
void func_00324C00(u8 *self, s32 slot, u32 b12);
void *Kind25_ModelFiles(void);
void *Kind25_MotionFiles(void);

extern const char *const D_0042C358;
extern void *D_00474130[];
extern const PTMF D_0042C720;
#define B7_W(p, off)  (*(s32 *)((u8 *)(p) + (off)))

#define B7_H(p, off)  (*(s16 *)((u8 *)(p) + (off)))

#define B7_B(p, off)  (*(u8 *)((u8 *)(p) + (off)))

#define B7_D(p, off)  (*(s64 *)((u8 *)(p) + (off)))

void Cr19Bubbles_Start(u8 *p);

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
            Actor_Destroy(&c->a);
        }
    }
    return c;
}

/* in play: Actor_TeleportRandom(-1) */
static inline __attribute__((always_inline)) void creature_inplay(Pursuer *p) {
    if (func_00217510(p) != 0) {
        Actor_TeleportRandom(&p->c.a, -1);
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

/* its slot's progress entry (Progress_HasRelationCmd) 1: SlotCmd_Cancel; -1 */
static inline __attribute__((always_inline)) s32 creature_slot_done(Pursuer *p) {
    Progress *g = gProgress;

    if ((u8)Progress_HasRelationCmd(g, *(u8 *)&p->c.a.slot) == 1) {
        SlotCmd_Cancel(g, *(u8 *)&p->c.a.slot);
    }
    return -1;
}

Character *Kind25_dtor(Character *c, s32 flags);
void Kind25_ShowUp(Pursuer *p);
void Kind25_EventState(Pursuer *p);
s32 Kind25_GrabOrder(Pursuer *p);

void func_002E2260(Character *c) {   /* +0xA8 */
}

s32 func_002E2270(Character *c) {    /* +0x3C */
    return 1;
}

void func_002E2280(Character *c) {   /* +0xA4 */
}

void func_002E2290(Character *c) {   /* +0xA0 */
}

void func_002E22A0(Character *c) {   /* +0x9C */
}

void func_002E22B0(Character *c) {   /* +0x38 */
}

void func_002E22C0(Character *c) {   /* +0x34 */
}

void func_002E22D0(Character *c) {   /* +0x40 */
}

void func_002E22E0(Character *c) {   /* +0x30 */
}

void func_002E22F0(Character *c) {   /* +0x2C */
}

/* +0x10 */
void func_002E2300(Character *c) {
    Actor_Cleanup(&c->a);
}

/* +0xC */
void func_002E2310(Character *c) {
    Character_Reset(c);
}

/* operator delete for objects put in place (nothing to free) */
void func_002E2320(void *p) {
}

/* placement new */
void *func_002E2330(u32 size, void *place) {
    return place;
}

/* ---- the creature (D_0046FAA0) ---- */

/* +0x8 destructor */
Character *func_002DE490(Character *c, s32 flags) {
    if (c != NULL) {
        AT(c, 0x0, void **) = D_0046FAA0;
        if (c != NULL) {
            AT(c, 0x0, void **) = D_0046FB50;
            if (c != NULL) {
                AT(c, 0x0, void **) = D_00469C60;
                if (c != NULL) {
                    AT(c, 0x0, void **) = D_00469C20;
                }
            }
        }
        if ((s16)flags > 0) {
            func_002E2320(c);
        }
    }
    return c;
}

void func_002DE510(u8 *p, s32 a1, u32 v) {
    F(p, 0x154C, s32) = v < 3 ? (s32)v : -1;
}

void func_002E2020(Character *c) {   /* +0x10 */
}

void func_002E2010(Character *c) {   /* +0x1C */
}

void func_002E2000(Character *c) {   /* +0x20 */
}

void func_002DFE00(Character *c) {   /* +0x84 */
}

void func_002DFDF0(Character *c) {   /* +0x88 */
}

void func_002E0510(Character *c) {   /* a state with nothing to do */
}

/* +0x28 put on triangle `tri` (Character_Place), remembering it as the previous one and the
   position (+0x38 / +0x40) */
s32 func_002E19A0(Character *c, u32 tri, const f32 *heading, f32 *pos) {
    s32 r = Character_Place(c, tri, heading, pos);

    c->a.prevNavTri = tri;
    sceVu0CopyVector(c->a.prevPos, c->a.pos);
    return r;
}

/* +0x5C reset (Character_Activate): not hit (+0xB), no target (+0x9 0xFF), mode 2 (+0xC) */
void func_002E0BF0(Character *c) {
    u8 *k = CR(c);

    Character_Activate(c);
    AT(k, 0xB, u8) = 0;
    AT(k, 0x9, u8) = 0xFF;
    AT(k, 0xC, s32) = 2;
}

/* +0x40: copies of its orientation and position into locals (left unused) */
void func_002E1340(Character *c) {
    f32 m[4][4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));

    sceVu0CopyMatrix(m, c->a.rot);
    sceVu0CopyVector(v, c->a.pos);
}

/* per kind (+0x1571): +0x8 a byte, +0x0 a float, +0x6 a short */
typedef struct CreatureKind {
    u8 b;
    u8 pad[3];
    f32 f;
    s16 s;
    u8 pad2[2];
} CreatureKind;

extern const CreatureKind D_004164F0[];

/* its rest time (+0x80) when it isn't out (+0x82): 1800 .. 7200 frames */
static inline void creature_rest(u8 *k) {
    AT(k, 0x80, s16) = ((VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) & 3) + 1) * 1800;
}

/* +0xA0 set up: mode `mode` (+0xC), kind `kind` (+0x31) with its table entry, strength
 * `str` x10 (+0x4); from save slot `slot` (-1: none) when that one is in use (+0x88A): its
 * state (+0x20), strength, out (+0x82, then a rest time), +0x2E, +0x87, +0x9A. Kind 0x24:
 * +0x14 -9. (+0x64 first: the Character's reset, a1..a3 passed on) */
void func_002DE8E0(Character *c, s32 a1, s32 a2, s32 mode, s32 kind, s32 str, s32 slot) {
    u8 *k = CR(c);
    const CreatureKind *t;

    VCALL(c, 0x64, void (*)(Character *, s32, s32, s32))(c, a1, a2, mode);
    AT(k, 0xC, s32) = mode;
    AT(k, 0x82, u8) = 0;
    AT(k, 0x80, s16) = 0;
    AT(k, 0x2A, u8) = 0;
    c->unk14C4 = 0;
    AT(k, 0x2E, u8) = 0;
    AT(k, 0x31, u8) = kind;
    AT(k, 0x4, s16) = (s16)str * 10;
    t = &D_004164F0[AT(k, 0x31, u8)];
    AT(k, 0x8, u8) = t->b;
    AT(k, 0x0, f32) = t->f;
    AT(k, 0x6, s16) = t->s;
    if (slot != -1) {
        u8 *e = (u8 *)gProgress + slot * 36 + 0x878;

        if (AT(e, 0x12, u8) != 0) {
            AT(k, 0x20, s16) = AT(e, 0xC, s16);
            AT(k, 0x4, s16) = AT(e, 0xF, u8) * 10;
            AT(k, 0x82, u8) = AT(e, 0x10, u8);
            AT(k, 0x2E, u8) = AT(e, 0x11, u8);
            AT(k, 0x87, u8) = AT(e, 0x13, u8);
            AT(k, 0x9A, u8) = AT(e, 0x14, u8);
            if (AT(k, 0x82, u8) != 0) {
                creature_rest(k);
            }
        }
    }
    if (AT(k, 0x31, u8) == 0x24) {
        AT(k, 0x14, f32) = -9.0f;
    }
}

/* +0x9C set up to come after Fiona: (+0x64 with mode 2) +0x2A whether someone (slots 2..5) is
 * up in her room; out, with a rest time; a path to her room (Character_Route) and its length
 * through the doors on it (rooms +0x38 by `a1`) into +0x14C4, the last door +0x14C0 */
void func_002DEA80(Character *c, s32 a1, s32 a2) {
    u8 *k = CR(c);
    u32 i;
    s32 n;

    VCALL(c, 0x64, void (*)(Character *, s32, s32, s32))(c, a1, a2, 2);
    AT(k, 0x2A, u8) = 0;
    for (i = 2; i < 6; i = (i + 1) & 0xFF) {
        Character *o = gCharacters[i & 0xFF];

        if (o != NULL && o->a.active == 1 && o->a.disabled == 0 && gCharPlayer->a.room == gCharacters[i & 0xFF]->a.room) {
            AT(k, 0x2A, u8) = 1;
        }
    }
    AT(k, 0x2E, u8) = 0;
    AT(k, 0x82, u8) = 1;
    creature_rest(k);
    if (Character_Route(c, gCharPlayer->a.room, -1, AT(k, 0xC, s32), -1) == -1) {
        return;
    }
    for (n = 0; n < c->unk1384; n++) {
        VObject *rooms = gRooms;
        u16 door = AT(c->unk138C, n * 2, u16);

        AT(&c->unk14C4, 0, f32) += (f32)VCALL(rooms, 0x38, s32 (*)(VObject *, u32, s32))(rooms, door, a1);
        c->unk14C0 = AT(c->unk138C, n * 2, u16);
    }
}

#include "navmesh.h"

extern VObject *gSceneGameF29740;   /* the path planner */

/* go through exit `exit` into the next room (off screen): its room, side (+0xC) and door
 * (+0x9); off the mesh, moving through the door (+0xFC 0x17), out of play; +0x8A.. cleared.
 * -1: no such exit */
s32 func_002DF760(Character *c, s32 exit) {
    u8 *k = CR(c);
    VObject *rooms;
    u32 d;
    s32 i;

    d = VCALL(gRooms, 0x14, u32 (*)(VObject *, s32, s32))(gRooms, c->a.room, exit) & 0xFF;
    if (d == 0xFF) {
        return -1;
    }
    rooms = gRooms;
    c->a.room = VCALL(rooms, 0x18, s32 (*)(VObject *, s32, s32))(rooms, c->a.room, exit);
    AT(k, 0xC, s32) = VCALL(rooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(rooms, c->a.room, d, 0);
    AT(k, 0x9, u8) = d;
    c->a.navTri = NAV_NONE;
    AT(k, 0xB, u8) = 0;
    c->moveSub = 0x17;
    c->a.disabled = 1;
    for (i = 0; i < 8; i++) {
        AT(k, 0x8A + i * 2, s16) = 0;
    }
    return 0;
}

/* plan a path to `goal` on `tri` (not across the room's divider; one request at a time,
 * +0x2B), waiting up to 50 polls for the planner, and start it (`direct`: the straight
 * variant). 0 = on its way, -1 = no. */
static inline __attribute__((always_inline)) s32 creature_path(Character *c, u32 tri, const f32 *goal, s32 direct, u32 busy) {
    u8 *k = CR(c);
    s32 r, i;

    if (NavMesh_AcrossDivider(gNavMesh, tri, c->a.navTri)) {
        return -1;
    }
    if (AT(k, busy, u8) == 0) {
        AT(k, busy, u8) += 1;
        if (Character_PlanPathOpt(c, 0, tri, goal, -1) == -1) {
            AT(k, busy, u8) = 0;
            return -1;
        }
    }
    for (i = 0;;) {
        r = VCALL(gSceneGameF29740, 0x10, s32 (*)(VObject *, s32))(gSceneGameF29740, c->pathId);
        if (r != 0 || ++i >= 50) {
            break;
        }
    }
    if (r > 0) {
        r = direct ? Character_Waypoints(c) : Character_WaypointsCurve(c);
        Character_ReleasePath(c);
        AT(k, busy, u8) = 0;
    }
    if (r >= 0) {
        return -(r == 0);
    }
    Character_ReleasePath(c);
    AT(k, busy, u8) = 0;
    return -1;
}

s32 func_002DF860(Character *c, u32 tri, const f32 *goal, s32 direct) {
    return creature_path(c, tri, goal, direct, 0x2B);
}

/* on the way to `tri`: while not there (its target +0xB0 not on it) the path ahead is looked
 * at (12 steps and 1; unused); a door on the way (+0x88 bit 0) with an exit +0x100 is gone
 * through */
void func_002DF470(Character *c, u32 tri) {
    u8 *k = CR(c);

    if (tri != Actor_TriTo(&c->a, c->a.unkB0, NAV_NONE)) {
        u32 t;
        f32 far[4] __attribute__((aligned(16)));
        f32 near[4] __attribute__((aligned(16)));
        f32 dir[4] __attribute__((aligned(16)));
        f32 fwd[4] __attribute__((aligned(16)));

        Character_WaypointAhead(c, &t, far, 12.0f * AT(k, 0x0, f32));
        Character_WaypointAhead(c, &t, near, AT(k, 0x0, f32));
        sceVu0SubVector(dir, near, c->a.pos);
        *(s32 *)&dir[1] = 0;
        sceVu0Normalize(dir, dir);
        fwd[2] = 1.0f;
        *(s32 *)&fwd[0] = 0;
        *(s32 *)&fwd[1] = 0;
        sceVu0ApplyMatrix(fwd, c->a.rot, fwd);
    }
    if (!(AT(k, 0x88, u16) & 1)) {
        return;
    }
    if ((VCALL(gRooms, 0x10, u32 (*)(VObject *, s32, u32))(gRooms, c->a.room, (u8)c->unk100) & 0xFFFF) != 0xFFFF) {
        AT(k, 0x84, u8) = 0;
        func_002DF760(c, (u8)c->unk100);
    }
}

/* step straight at Fiona (at its speed +0x0) unless she's held (her mode 4) right next to it
 * (+0x24 within 1) */
static s32 creature_close_in(Character *c, u32 t) {
    u8 *k = CR(c);

    if (!(AT(k, 0x24, f32) <= 1.0f) || gCharPlayer->moveMode != 4) {
        f32 dir[4] __attribute__((aligned(16)));

        sceVu0SubVector(dir, gCharPlayer->a.pos, c->a.pos);
        *(s32 *)&dir[1] = 0;
        sceVu0Normalize(dir, dir);
        func_0010E640(dir, dir, AT(k, 0x0, f32));
        Actor_Move(&c->a, dir);
    }
    return t;
}

/* when it can walk straight at Fiona (her triangle reachable; +0x8 set: only from hers), it
 * drops its path and does; on another level (height) it gives up after +0x30 tries while
 * close (+0x24 under 2) unless on her triangle (+0x2F). Her triangle, -1 if not. */
s32 func_002DFF70(Character *c) {
    u8 *k = CR(c);
    u32 t;

    AT(c, 0x15DB, u8) = 0;
    t = Actor_TriTo(&c->a, gCharPlayer->a.pos, c->pathReq->mask);
    if (AT(k, 0x8, u8) != 0) {
        if (t != gCharPlayer->a.navTri) {
            return -1;
        }
        Character_ReleasePath(c);
        AT(k, 0x2B, u8) = 0;
        c->unk124 = c->unk128;
        return creature_close_in(c, t);
    }
    if (t == NAV_NONE) {
        return -1;
    }
    Character_ReleasePath(c);
    AT(k, 0x2B, u8) = 0;
    c->unk124 = c->unk128;
    if (gCharPlayer->a.pos[1] != c->a.pos[1]) {
        if (AT(k, 0x24, f32) < 2.0f) {
            AT(k, 0x22, s16) += 1;
        }
        if (AT(k, 0x22, s16) >= AT(k, 0x30, u8) && c->a.navTri != gCharPlayer->a.navTri) {
            AT(k, 0x22, s16) = 0;
            AT(k, 0x2F, u8) = 1;
            return -1;
        }
    }
    return creature_close_in(c, t);
}

/* placed at a random triangle of its room (only in the room being played) on its level
 * `level` (0 / 1: flag 0x100000 / 0x200000 free; -1 / 2: not both; flag 8 never), away from
 * the room's doors' event spots */
void func_002DE540(Character *c, s32 level) {
    u32 mask, tri;
    s32 room = c->a.room;
    VObject *rnd, *rooms, *ev_mgr;
    NavMesh *nm;
    u32 n;
    u8 ok;

    if (room != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        return;
    }
    switch (level) {
    case 0:
        mask = 0x100000;
        break;
    case 1:
        mask = 0x200000;
        break;
    default:
        mask = 0x300000;
        break;
    }
    rnd = gRandom;
    nm = gNavMesh;
    rooms = gRooms;
    n = nm->numTris;
    ev_mgr = gEvents;
    for (;;) {
        f32 pos[4] __attribute__((aligned(16)));
        u32 flags, d;

        tri = (u32)((f32)n * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd));
        flags = NavMesh_TriFlags(nm, tri);
        if (flags & 8) {
            continue;
        }
        if (level != -1 && level != 2) {
            if (flags & mask) {
                continue;
            }
        } else if (mask == (mask & flags)) {
            continue;
        }
        ok = 1;
        VCALL(nm, 0xC, void (*)(NavMesh *, u32, f32 *))(nm, tri, pos);
        for (d = 0; d < 8; d = (d + 1) & 0xFF) {
            u32 door = VCALL(rooms, 0x48, u32 (*)(VObject *, s32, u32))(rooms, c->a.room, d) & 0xFFFF;

            if (door != 0xFFFF &&
                (VCALL(ev_mgr, 0x10, u32 (*)(VObject *, f32 *, u32, s32))(ev_mgr, pos, door, -1) & 0xFF) == 1) {
                ok = 0;
                break;
            }
        }
        if (ok == 1) {
            break;
        }
    }
    VCALL(c, 0x28, void (*)(Character *, u32, s32, s32))(c, tri, 0, 0);
}

s32 func_002DE830(u8 *p) { return p[0x156E] != 0; }

/* Saves this object's state into a 36-byte slot of the progress block (+0x878). */
void func_002DE840(u8 *p, s32 slot, u32 v) {
    u8 *e = (u8 *)gProgress + slot * 36;

    F(e, 0x878, u32) = F(p, 0x30, u32);
    F(e, 0x87C, u32) = F(p, 0x154C, u32);
    F(e, 0x880, u32) = F(p, 0x34, u32);
    F(e, 0x884, s16) = F(p, 0x1560, s16);
    e[0x886] = p[0x1571];
    e[0x887] = F(p, 0x1544, s16) / 10;
    e[0x888] = p[0x15C2];
    e[0x889] = p[0x156E];
    e[0x88A] = (u8)v;
    e[0x88B] = p[0x15C7];
    e[0x88C] = p[0x15DA];
    F(e, 0x890, u32) = 0;
    F(e, 0x894, u32) = 0;
    F(e, 0x898, u32) = 0;
}

/* the first door of the room (doors +0x40) it may use (+0x15CA[door] bit of its slot), that
 * isn't locked (+0x30 (1, 0)): headed for (+0x85 the door, +0x88 1), its path planned to the
 * door's spot; if it was ahead of the plan it snaps onto the next step (+0x84 1) */
void func_002DF5B0(Character *c) {
    u8 *k = CR(c);
    VObject *doors = gDoors, *rooms = gRooms;
    f32 at[4] __attribute__((aligned(16)));
    f32 p[4] __attribute__((aligned(16)));
    u32 d, t;

    for (d = 0; d < 8; d = (d + 1) & 0xFF) {
        u16 may;

        if ((VCALL(doors, 0x40, u32 (*)(VObject *, u32))(doors, d) & 0xFF) != 1) {
            continue;
        }
        may = (AT(c, 0x15CA + (d & 0xFF) * 2, u16) & ((1 << *(u8 *)&c->a.slot) & 0xFFFF)) != 0;
        if (VCALL(doors, 0x30, u32 (*)(VObject *, u32, s32, s32))(doors, d, 1, 0) & 0xFF) {
            continue;
        }
        if (may & 1) {
            AT(k, 0x85, u8) = d;
            AT(k, 0x88, u16) = may;
            c->a.unk2B = 1;
            if (func_002DF860(c, VCALL(rooms, 0x30, u32 (*)(VObject *, u32, f32 *))(rooms, AT(k, 0x85, u8), at), at, 0) != 0) {
                c->unk124 = c->unk128;
            }
            if (c->unk128 < c->unk124) {
                s32 next = Character_WaypointAhead(c, &t, p, AT(k, 0x0, f32));

                c->a.navTri = t;
                sceVu0CopyVector(c->a.pos, p);
                c->unk128 = next;
            }
            AT(k, 0x84, u8) = 1;
        }
    }
}

/* per frame: how far Fiona is (+0x24, on the floor plane) when in the room being played. Out
 * (+0x82): its rest runs down; done and still elsewhere, it comes back (+0x2E). Else it comes
 * for her (+0x2E) once she is in reach (her triangle seen, within +0x4) - kind 0x24 after 61
 * frames (+0x9D) - or right away with no reach */
void func_002E0520(Character *c) {
    u8 *k = CR(c);
    Progress *p = gProgress;
    s32 room = c->a.room;
    u8 come;

    if (room == VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        f32 d[4] __attribute__((aligned(16)));

        sceVu0SubVector(d, c->a.pos, gCharPlayer->a.pos);
        AT(k, 0x24, f32) = __builtin_sqrtf(d[2] * d[2] + d[0] * d[0]);
    }
    if (AT(k, 0x82, u8) != 0) {
        if (AT(k, 0x80, s16) > 0) {
            AT(k, 0x80, s16) -= 1;
        }
        if (AT(k, 0x80, s16) <= 0) {
            AT(k, 0x80, s16) = 0;
            room = c->a.room;
            if (room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
                AT(k, 0x2E, u8) = 1;
            }
        }
        return;
    }
    if (AT(c, 0x1571, u8) == 0x24) {
        AT(k, 0x9D, u8) += 1;
        come = AT(k, 0x9D, u8) >= 61;
    } else {
        u32 t = gCharPlayer->a.navTri;

        come = Actor_TriTo(&c->a, gCharPlayer->a.pos, NAV_NONE) == t && AT(k, 0x24, f32) <= (f32)AT(k, 0x4, s16);
    }
    if (come == 1) {
        AT(k, 0x2E, u8) = 1;
    }
    if (AT(k, 0x4, s16) == 0) {
        AT(k, 0x2E, u8) = 1;
    }
}

/* in play, a bobbing motion: height offset +0x14 (kept within -10..5) moved by +0x18, itself
 * by +0x1C, which turns over every +0x32 frames (+0x28); now and then (1 in 32) a short burst
 * (+0x34: 5-frame turns at speed 0.08 for 10 frames, then back) */
void func_002DEFA0(Character *c) {
    u8 *k = CR(c);

    if (c->a.disabled) {
        return;
    }
    AT(k, 0x33, u8) = VCALL(gRandom, 0x14, s32 (*)(VObject *))(gRandom) & 0x1F;
    if (AT(k, 0x34, u8) == 0) {
        if (AT(k, 0x33, u8) == 0) {
            AT(k, 0x34, u8) = 1;
            AT(k, 0x38, f32) = AT(k, 0x18, f32);
            AT(k, 0x3C, f32) = AT(k, 0x1C, f32);
            AT(k, 0x36, s8) = AT(k, 0x28, s8);
            AT(k, 0x35, s8) = 0;
            AT(k, 0x28, s8) = 0;
            if (AT(k, 0x1C, f32) < 0.0f) {
                AT(k, 0x1C, f32) = -0x1.47ae14p-4f /* -0.08 */;
            }
            if (!(AT(k, 0x1C, f32) <= 0.0f)) {
                AT(k, 0x1C, f32) = 0x1.47ae14p-4f /* 0.08 */;
            }
            AT(k, 0x32, u8) = 5;
        }
    } else if (AT(k, 0x35, s8)++ == AT(k, 0x32, u8) * 2) {
        AT(k, 0x34, u8) = 0;
        AT(k, 0x18, f32) = AT(k, 0x38, f32);
        AT(k, 0x1C, f32) = AT(k, 0x3C, f32);
        AT(k, 0x28, s8) = AT(k, 0x36, s8);
        AT(k, 0x35, s8) = 0;
        AT(k, 0x32, u8) = 20;
    }
    if (AT(k, 0x28, s8) % AT(k, 0x32, u8) == 0) {
        AT(k, 0x1C, f32) = -AT(k, 0x1C, f32);
        AT(k, 0x28, s8) = 0;
    }
    AT(k, 0x28, s8) += 1;
    if (!(AT(k, 0x14, f32) < 5.0f)) {
        AT(k, 0x14, f32) = 5.0f;
    }
    if (AT(k, 0x14, f32) <= -10.0f) {
        AT(k, 0x14, f32) = -10.0f;
    }
    AT(k, 0x40, f32) += AT(k, 0x1C, f32);
    AT(k, 0x14, f32) += AT(k, 0x18, f32);
    AT(k, 0x18, f32) += AT(k, 0x1C, f32);
}

#include "ptmf.h"
#include "effectmgr.h"

extern void *D_00472370[];
extern const PTMF D_00416790;   /* vanishing */

/* its vanishing effect (0x4A0 bytes, vtable D_00472370; its quad drawer at +0x370) */
static void vanish_init(void **o) {
    o[0] = D_00472370;
    o[0x370 / 4] = D_00469D00;
    ((s32 *)o)[0x374 / 4] = -1;
    o[0x370 / 4] = D_0046FC30;
}

/* seen: once it has come (+0x2E) and Fiona isn't hidden (+0x2D) or busy (Character_Held) and
 * sees it (Actor_Touching), a noise (kind 0xD) is made, it vanishes in a glow (12 above it, red
 * for kinds 0x12.. but 0x24, else blue) with a sound (0x8B) when not in an event, and its
 * vanishing state starts (+0x37, +0x29 set, the motion +0x18 / +0x1C stopped) */
void func_002DF180(Character *c) {
    u8 *k = CR(c);
    u8 *mgr;
    s32 slot;
    struct {
        f32 pos[4];
        u8 rgba[4];
    } fx __attribute__((aligned(16)));

    if (AT(c, 0x1569, u8) != 0 || AT(k, 0x2E, u8) == 0 || gCharPlayer->a.unk2D == 1 ||
        (Character_Held(gCharPlayer) & 0xFF) || (Actor_Touching(&c->a, &gCharPlayer->a, 0.0f, 0.0f) & 0xFF) != 1) {
        return;
    }
    Progress_Noise(gProgress, c->a.pos, 1, 0xD, 0, 0, 0.0f);
    mgr = gEffects;
    slot = Effect_New(mgr, 0x4A0, vanish_init);
    sceVu0CopyVector(fx.pos, c->a.pos);
    fx.pos[1] = 12.0f + c->a.pos[1] + AT(k, 0x14, f32);
    if (AT(k, 0x31, u8) >= 0x12 && AT(k, 0x31, u8) != 0x24) {
        fx.rgba[0] = 0x80;
        fx.rgba[1] = 0x30;
        fx.rgba[3] = 0x60;
        fx.rgba[2] = 0x30;
    } else {
        fx.rgba[2] = 0x80;
        fx.rgba[0] = 0x30;
        fx.rgba[3] = 0x60;
        fx.rgba[1] = 0x30;
    }
    func_002D6090(mgr, slot, &fx);
    if (gCharPlayer->unkE2 == 0 && c->a.disabled == 0 && VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0) {
        Actor_PlaySound(&c->a, 0x8B, 5, 0, 0, NULL);
    }
    AT(k, 0x37, u8) = 1;
    AT(k, 0x29, u8) = 1;
    AT(k, 0x18, s32) = 0;
    AT(k, 0x1C, s32) = 0;
    ptmf_set(&c->a.state, &D_00416790);
}

/* in the room being played: note the doors (by exit, +0x8A) whose event spot it stands on */
static void creature_at_doors(Character *c, u32 doors) {
    u8 *k = CR(c);
    VObject *rooms = gRooms, *ev_mgr;
    s32 cur = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    u32 i;

    ev_mgr = gEvents;
    for (i = 0; i < 8; i++) {
        u32 door;

        AT(k, doors + i * 2, u16) = 0;
        door = VCALL(rooms, 0x48, u32 (*)(VObject *, s32, u32))(rooms, cur, i & 0xFF) & 0xFFFF;
        if (cur == c->a.room && door != 0xFFFF && c->a.disabled == 0 &&
            VCALL(ev_mgr, 0x10, s32 (*)(VObject *, f32 *, u32, s32))(ev_mgr, c->a.pos, door, -1) != 0) {
            AT(k, doors + i * 2, u16) |= (1 << *(s32 *)&c->a.slot) & 0xFFFF;
        }
    }
}

/* travelling (unless in an event, flag 0x18): off screen the distance to the next door
 * (+0x14C4) runs down by its speed (2/3 of it with company, +0x2A); at a door (+0xB) it walks
 * the path at half speed. On arriving at the door (+0x14C0): if the way through is open
 * (Progress_ExitOpen) and the door isn't shut (Progress_CurRoomFlag), through it - into the room
 * being played at the exit's spot, facing in, noting the doors whose event spot it stands on
 * (+0x8A) - else the next leg's distance; a closed way is marked (+0x148C bit) and the trip
 * ends (+0x86) */
void func_002DFA50(Character *c) {
    u8 *k = CR(c);
    Progress *p = gProgress;
    VObject *rooms;
    u8 arrived = 0;
    u32 exit;

    if (Progress_TestFlag(p, 0x18) == 0) {
        if (AT(k, 0xB, u8) != 1) {
            f32 *left = &AT(&c->unk14C4, 0, f32);

            if (AT(k, 0x2A, u8) == 0) {
                *left = *left - AT(k, 0x0, f32);
            } else {
                *left = *left - AT(k, 0x0, f32) / 1.5f;
            }
            if (*left < 0.0f) {
                *left = 0.0f;
                arrived = 1;
                AT(k, 0x2A, u8) = 0;
            }
        } else {
            Character_FollowWaypoints(c, AT(k, 0x0, f32) / 2.0f);
            if (!(c->unk128 < c->unk124)) {
                arrived = 1;
            }
        }
    }
    if (arrived != 1) {
        return;
    }
    rooms = gRooms;
    exit = VCALL(rooms, 0x3C, u32 (*)(VObject *, u32, s32))(rooms, c->unk14C0, c->a.room) & 0xFF;
    if (!(Progress_ExitOpen(p, c->a.room, exit) & 0xFF)) {
        c->unk148C[c->unk14C0 >> 5] |= 1 << (c->unk14C0 & 0x1F);
        AT(k, 0xB, u8) = 0;
        c->a.navTri = NAV_NONE;
        c->unk1388 = c->unk1384;
        AT(k, 0x86, u8) = 1;
        return;
    }
    AT(k, 0x9, u8) = exit;
    if (exit == 0xFF) {
        AT(k, 0xB, u8) = 0;
        c->a.navTri = NAV_NONE;
        return;
    }
    if ((Progress_CurRoomFlag(p, c->a.room, exit) & 0xFF) == 1) {
        return;
    }
    AT(k, 0xB, u8) = 0;
    func_002DF760(c, exit);
    if (c->a.room == VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        f32 at[4] __attribute__((aligned(16)));
        f32 in[4] __attribute__((aligned(16)));
        f32 d[4] __attribute__((aligned(16)));
        f32 yaw;
        u32 tri;

        tri = VCALL(gRooms, 0x30, u32 (*)(VObject *, u32, f32 *))(gRooms, AT(k, 0x9, u8), at);
        VCALL(gRooms, 0x34, u32 (*)(VObject *, u32, f32 *))(gRooms, AT(k, 0x9, u8), in);
        sceVu0SubVector(d, in, at);
        yaw = func_0031C5C0(d[0], d[2]);
        VCALL(c, 0x28, void (*)(Character *, u32, f32 *, f32 *))(c, tri, &yaw, at);
        creature_at_doors(c, 0x8A);
    } else {
        c->a.navTri = NAV_NONE;
        AT(&c->unk14C4, 0, f32) = (f32)VCALL(rooms, 0x38, s32 (*)(VObject *, u32, s32))(rooms, c->unk14C0, c->a.room);
        c->unk1388 = c->unk1384;
    }
}

/* state: vanishing (D_00416790). Not seen (+0x37 0): once its time (+0x20) reaches the kind's
 * (+0x6) a fade (+0x2C, 8 a frame) and it is gone. Seen: it sinks (+0x1C, 0.1 faster each
 * frame) to 12 below, fades, then leaves a noise (red ones: noise 3 here; others a level
 * 0x80 noise at its place) and is gone (inactive, its path dropped) */
void func_002DFE10(Character *c) {
    u8 *k = CR(c);

    if (AT(c, 0x1577, s8) == 0) {
        if (AT(k, 0x20, s16) < AT(k, 0x6, s16)) {
            return;
        }
        if (AT(k, 0x2C, s16) > 0) {
            AT(k, 0x2C, s16) -= 8;
            return;
        }
        c->a.active = 0;
        Character_ReleasePath(c);
        return;
    }
    if (!(AT(k, 0x14, f32) <= -12.0f)) {
        AT(k, 0x1C, f32) -= 0x1.99999ap-4f /* 0.1 */;
        AT(k, 0x14, f32) += AT(k, 0x1C, f32);
        return;
    }
    AT(k, 0x14, f32) = -12.0f;
    if (AT(k, 0x2C, s16) > 0) {
        AT(k, 0x2C, s16) -= 8;
        return;
    }
    if (AT(k, 0x31, u8) >= 0x12 && AT(k, 0x31, u8) != 0x24) {
        Progress_Noise(gProgress, c->a.pos, 1, 3, 0, 0, 0.0f);
    } else {
        func_002A8440((u8 *)gProgress + 0x7A8, 0x80, c->a.room, c->a.navTri, 0xFFFF);
    }
    c->a.active = 0;
    Character_ReleasePath(c);
}

/* state: to the door it chose (+0x85): its spot asked of the planner; on the way there
 * (func_002DF470) with the door as its exit (+0x100) and its spot as the target (+0xB0) */
void func_002E01C0(Character *c) {
    u8 *k = CR(c);
    VObject *rooms = gRooms;
    f32 at[4] __attribute__((aligned(16)));
    f32 spot[4] __attribute__((aligned(16)));
    u32 tri;

    tri = VCALL(rooms, 0x30, u32 (*)(VObject *, u32, f32 *))(rooms, AT(c, 0x15C5, u8), at);
    if (Character_PlanPathKind(c, 0, tri, at) > 0) {
        VCALL(gSceneGameF29740, 0x14, s32 (*)(VObject *))(gSceneGameF29740);
    }
    if (AT(k, 0x85, u8) == 0xFF) {
        return;
    }
    c->unk100 = AT(k, 0x85, u8);
    tri = VCALL(rooms, 0x30, u32 (*)(VObject *, u32, f32 *))(rooms, AT(k, 0x85, u8), spot);
    sceVu0CopyVector(c->a.unkB0, spot);
    c->unk124 = c->unk128;
    func_002DF470(c, tri);
}

/* state: travelling: at the end of its doors (+0x1388) and not at a door, a new trip to the
 * room being played (Character_Route): its first door (+0x14C0) and that leg's distance
 * (+0x14C4); none: the closed ways it noted (+0x148C) are forgotten */
static inline __attribute__((always_inline)) void creature_route(Character *c, u32 flagOff, u32 idOff) {
    u8 *k = CR(c);
    s32 i;

    if (c->unk1388 < c->unk1384 || AT(k, flagOff, u8) != 0) {
        return;
    }
    if (Character_Route(c, VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress), -1, AT(k, idOff, s32), -1) <= 0) {
        for (i = 0; i < 13; i++) {
            c->unk148C[i] = 0;
        }
        return;
    }
    c->unk14C0 = AT(c->unk138C, 0, u16);
    AT(&c->unk14C4, 0, f32) = (f32)VCALL(gRooms, 0x38, s32 (*)(VObject *, u32, s32))(gRooms, AT(c->unk138C, 0, u16), c->a.room);
}

void func_002E02B0(Character *c) {
    creature_route(c, 0xB, 0xC);
}

/* state: after Fiona (unless she is in mode 3): in her room, unless given up (+0x2F), straight
 * at her when it can (func_002DFF70) - not on a 0x20000 triangle, where it gives up at once
 * (its time +0x20 to the kind's +0x6); otherwise along a path to her triangle */
void func_002E0390(Character *c) {
    u8 *k = CR(c);
    s32 t = -1;
    s32 room;

    AT(c, 0x15C6, u8) = 0;
    if (gCharPlayer->moveMode == 3) {
        return;
    }
    room = c->a.room;
    if (room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) && AT(k, 0x2F, u8) == 0) {
        t = func_002DFF70(c);
        if (t == -1 && (NavMesh_TriFlags(gNavMesh, c->a.navTri) & 0x20000)) {
            AT(k, 0x20, s16) = AT(k, 0x6, s16);
        }
    }
    if (t >= 0) {
        return;
    }
    if (c->unk128 >= c->unk124) {
        if (gCharPlayer->a.navTri == NAV_NONE) {
            return;
        }
        if (func_002DF860(c, gCharPlayer->a.navTri, gCharPlayer->a.pos, 0) != 0) {
            return;
        }
    }
    Character_FollowWaypointsBlocked(c, AT(k, 0x0, f32));
}

/* +0x64 put in room `room` on triangle `tri`, mode `mode` (+0xC): (the Character's +0x64,
 * Character_ToRoom) in the room being played placed there (+0x28; its result) and its doors
 * noted, else just its triangle; 0 */
static inline __attribute__((always_inline)) s32 creature_place(Character *c, s32 room, u32 tri, s32 mode, u32 modeOff,
                                                                  u32 doors) {
    Progress *p;
    s32 r = 0;

    Character_ToRoom(c, room, tri, mode);
    AT(CR(c), modeOff, s32) = mode;
    p = gProgress;
    if (room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        c->a.navTri = tri;
        return r;
    }
    r = VCALL(c, 0x28, s32 (*)(Character *, u32, f32 *, f32 *))(c, tri, NULL, NULL);
    creature_at_doors(c, doors);
    return r;
}

s32 func_002E0C30(Character *c, s32 room, u32 tri, s32 mode) {
    return creature_place(c, room, tri, mode, 0xC, 0x8A);
}

/* back on its triangle in the room being played (or, on one it may not stand on, a random place
   on its level), its doors noted */
static void creature_back_on_mesh(Character *c) {
    u32 tri = c->a.navTri;

    if (NavMesh_TriFlags(gNavMesh, tri) & c->a.navMask) {
        func_002DE540(c, AT(CR(c), 0xC, s32));
    } else {
        VCALL(gNavMesh, 0x14, void (*)(NavMesh *, u32, f32 *))(gNavMesh, tri, c->a.pos);
        VCALL(c, 0x28, s32 (*)(Character *, u32, f32 *, f32 *))(c, c->a.navTri, &c->a.angle[1], c->a.pos);
    }
    creature_at_doors(c, 0x8A);
}

/* +0x38 the room is entered: if it was out (+0x10) and is in the room being played, back on
 * its triangle (or, on one it may not stand on, a random place on its level) with its doors
 * noted; in the room being played it is in play (somewhere random if off the mesh), else not */
void func_002E0DB0(Character *c) {
    u8 *k = CR(c);
    Progress *p = gProgress;
    s32 room;

    if (AT(k, 0x10, s32) != 0) {
        room = c->a.room;
        if (room == VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
            creature_back_on_mesh(c);
        }
    }
    room = c->a.room;
    if (room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        if (c->a.navTri == NAV_NONE) {
            func_002DE540(c, AT(k, 0xC, s32));
        }
        c->a.disabled = 0;
    } else {
        c->a.disabled = 1;
    }
}

/* +0xC set up: the Character's (Character_Reset); size 1.5 x 16, company 1, no blocking flags,
 * path kind 6 blocking 0x40080; no model; a random kind 0..15 (its table entry), strength 0;
 * all its own state cleared (bobbing 0.5 / 0.05, fade 0x80, turns every 20, gives up after
 * 60 or with +0x8 30 tries), then +0x5C */
void func_002E2030(Character *c) {
    u8 *k = CR(c);
    const CreatureKind *t;
    s32 i;

    Character_Reset(c);
    c->a.radius = 1.5f;
    c->a.height = 16.0f;
    c->a.unk2A = 1;
    c->a.navMask = 0;
    c->pathReq->unk4 = 6;
    c->pathReq->mask = 0x40080;
    c->motion = NULL;
    AT(k, 0x31, u8) = VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) & 0xF;
    AT(k, 0x4, s16) = 0;
    t = &D_004164F0[AT(k, 0x31, u8)];
    AT(k, 0x8, u8) = t->b;
    AT(k, 0x0, f32) = t->f;
    AT(k, 0x6, s16) = t->s;
    AT(k, 0x28, u8) = 0;
    AT(k, 0x14, s32) = 0;
    AT(k, 0x18, f32) = 0.5f;
    AT(k, 0x1C, f32) = 0x1.99999ap-5f /* 0.05 */;
    AT(k, 0x9, u8) = 0;
    AT(k, 0xA, u8) = 0;
    AT(k, 0xB, u8) = 0;
    AT(k, 0xC, s32) = 0;
    AT(k, 0x10, s32) = 0;
    AT(k, 0x20, s16) = 0;
    AT(k, 0x22, s16) = 0;
    AT(k, 0x24, s32) = 0;
    AT(k, 0x29, u8) = 0;
    AT(k, 0x2C, s16) = 0x80;
    AT(k, 0x2A, u8) = 0;
    AT(k, 0x2B, u8) = 0;
    AT(k, 0x32, u8) = 20;
    AT(k, 0x2F, u8) = 0;
    AT(k, 0x30, u8) = AT(k, 0x8, u8) == 0 ? 60 : 30;
    AT(k, 0x33, u8) = 0;
    AT(k, 0x34, u8) = 0;
    AT(k, 0x35, u8) = 0;
    AT(k, 0x36, u8) = 0;
    AT(k, 0x37, u8) = 0;
    AT(k, 0x38, s32) = 0;
    AT(k, 0x3C, s32) = 0;
    AT(k, 0x40, s32) = 0;
    for (i = 0x50; i < 0x80; i += 4) {
        AT(k, i, s32) = 0;
    }
    AT(k, 0x83, u8) = 0;
    AT(k, 0x84, u8) = 0;
    AT(k, 0x85, u8) = 0;
    AT(k, 0x88, u16) = 0;
    AT(k, 0x44, s32) = 0;
    AT(k, 0x86, u8) = 0;
    AT(k, 0x9D, u8) = 0;
    for (i = 0; i < 8; i++) {
        AT(k, 0x8A + i * 2, s16) = 0;
    }
    AT(k, 0x87, u8) = 0;
    AT(k, 0x9A, u8) = 0;
    AT(k, 0x9B, u8) = 0;
    AT(k, 0x9C, u8) = 0;
    VCALL(c, 0x5C, void (*)(Character *))(c);
}

/* Inlined destructor chain: resets the vtable to each base class in turn. */
void *func_002E2220(u8 *p) {
    if (p != NULL) {
        *(void *volatile *)p = D_0046FB50;
        *(void *volatile *)p = D_00469C60;
        *(void *volatile *)p = D_00469C20;
    }
    return p;
}

/* it can head for exit `e` of its room: on its side (or it is on both, 2) with a path to the
 * exit's spot (left in `at`) */
static s32 creature_exit_reachable(Character *c, VObject *rooms, u32 e, f32 *at) {
    s32 side = AT(CR(c), 0xC, s32);

    if (side != VCALL(rooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(rooms, c->a.room, e, 0) && side != 2) {
        return 0;
    }
    return func_002DF860(c, VCALL(rooms, 0x34, u32 (*)(VObject *, u32, f32 *))(rooms, e, at), at, 1) == 0;
}

/* +0x34 Fiona left by exit `exit` (+0xA its door): in the room being played it follows - by
 * that exit if it can (+0xB), else by another (+0x87 0 once one is found) - and, when going by
 * hers and she isn't just beyond it (Actor_NearerRoom), skips its path to the door if already
 * nearer the door than the exit's spot; if she is, it stays out (+0x10 0) */
void func_002E0FE0(Character *c, u32 exit) {
    u8 *k = CR(c);
    VObject *rooms = gRooms;
    f32 at[4] __attribute__((aligned(16)));
    s32 room, i;
    u32 x, e;

    AT(k, 0xA, s8) = VCALL(rooms, 0x14, s32 (*)(VObject *, s32, u32))(rooms, c->a.room, exit);
    room = c->a.room;
    if (room != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        return;
    }
    for (i = 0; i < 13; i++) {
        c->unk148C[i] = 0;
    }
    AT(k, 0xB, u8) = 0;
    AT(k, 0x87, u8) = 1;
    x = exit & 0xFF;
    if (x != 0xFF && creature_exit_reachable(c, rooms, exit, at)) {
        AT(k, 0x87, u8) = 0;
        AT(k, 0xB, u8) = 1;
    }
    if (AT(k, 0x87, u8) == 1) {
        for (e = 0; e < 8; e = (e + 1) & 0xFF) {
            if (e != x && (VCALL(rooms, 0x74, u32 (*)(VObject *, s32, u32))(rooms, c->a.room, e) & 0xFF) == 1 &&
                creature_exit_reachable(c, rooms, e, at)) {
                AT(k, 0x87, u8) = 0;
                break;
            }
        }
    }
    if (AT(k, 0x87, u8) == 1 || AT(k, 0xB, u8) != 1) {
        return;
    }
    if ((Actor_NearerRoom(&c->a, exit, gCharPlayer->a.pos) & 0xFF) == 1) {
        VCALL(rooms, 0x14, s32 (*)(VObject *, s32, u32))(rooms, c->a.room, exit);
        AT(k, 0x10, s32) = 0;
        return;
    }
    if (AT(k, 0xB, u8) == 1) {
        f32 door[4] __attribute__((aligned(16)));
        f32 d0[4] __attribute__((aligned(16)));
        f32 d1[4] __attribute__((aligned(16)));

        rooms = gRooms;
        c->unk14C0 = VCALL(rooms, 0x10, s32 (*)(VObject *, s32, u32))(rooms, c->a.room, exit);
        VCALL(rooms, 0x30, u32 (*)(VObject *, u32, f32 *))(rooms, exit, door);
        sceVu0SubVector(d0, c->a.pos, door);
        sceVu0SubVector(d1, at, door);
        if (sceVu0InnerProduct(d0, d0) <= sceVu0InnerProduct(d1, d1) && Actor_TriTo(&c->a, door, NAV_NONE) != NAV_NONE) {
            c->unk124 = c->unk128;
        }
    }
}

extern const f32 D_004167A0[17];   /* turns: 0, then +-0.39 .. +-3.14 */

/* state: hanging about Fiona while she can't be reached (D_00416700). First its path is
 * dropped (+0x9B 1). Then (on arriving) a spot by her: 10..25 to her front or back, turned by
 * a random step of the last try, on a triangle of the mesh other than its own that isn't
 * blocked (0x04020028) - up to 17 tries (a blocked one sets +0x9B back to 1) - and it walks
 * there; no spot, or the path or the walk ends: +0x9B 1 again */
void func_002DEC30(Character *c) {
    u8 *k = CR(c);
    f32 goal[4] __attribute__((aligned(16)));
    f32 turn = 0.0f;

    if (AT(c, 0x15DB, s8) == 0) {
        AT(k, 0x9B, s8) += 1;
        Character_ReleasePath(c);
        AT(k, 0x2B, u8) = 0;
        c->unk124 = c->unk128;
    }
    if (AT(k, 0x9B, s8) == 1) {
        AT(k, 0x9B, s8) += 1;
        if (c->unk128 >= c->unk124) {
            VObject *rnd = gRandom;
            NavMesh *nm = gNavMesh;
            s32 i;

            for (i = 0; i < 17; i++) {
                f32 m[4][4] __attribute__((aligned(16)));
                f32 off[4] __attribute__((aligned(16)));
                f32 v[4] __attribute__((aligned(16)));
                f32 w[4] __attribute__((aligned(16)));
                s32 sign;
                u32 tri;

                *(s32 *)&v[1] = 0;
                *(s32 *)&v[3] = 0;
                *(s32 *)&v[2] = 0;
                *(s32 *)&v[0] = 0;
                sceVu0CopyVector(w, v);
                sceVu0CopyVector(goal, v);
                sign = i % 2 == 0 ? 1 : -1;
                sceVu0UnitMatrix(m);
                *(s32 *)&off[0] = 0;
                *(s32 *)&off[3] = 0;
                *(s32 *)&off[1] = 0;
                off[2] = (f32)sign * (10.0f + (f32)(u32)(VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 0xF));
                sceVu0ApplyMatrix(v, m, off);
                sceVu0AddVector(v, gCharPlayer->a.pos, v);
                sceVu0RotMatrixY(m, m, turn);
                sceVu0ApplyMatrix(w, m, off);
                sceVu0AddVector(goal, gCharPlayer->a.pos, w);
                turn = D_004167A0[VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 0xF];
                AT(k, 0x44, u32) = Actor_TriOf(&gCharPlayer->a, goal);
                tri = AT(k, 0x44, u32);
                if (tri == NAV_NONE || c->a.navTri == tri) {
                    continue;
                }
                if (!(NavMesh_TriFlags(nm, tri) & 0x04020028)) {
                    break;
                }
                AT(k, 0x9B, s8) = 1;
            }
        }
    }
    if (AT(k, 0x44, u32) == NAV_NONE) {
        AT(k, 0x9B, s8) = 1;
        return;
    }
    if (c->unk128 >= c->unk124 && func_002DF860(c, AT(k, 0x44, u32), goal, 0) != 0) {
        AT(k, 0x9B, s8) = 1;
        return;
    }
    if (Character_FollowWaypointsBlocked(c, AT(k, 0x0, f32)) == 0) {
        AT(k, 0x9B, s8) = 1;
    }
}

extern const PTMF D_00416700, D_00416710, D_00416720, D_00416730, D_00416740, D_00416750, D_00416760,
    D_00416770, D_00416780;   /* its states: about Fiona, idle, after her, idle, at the door, to the
                                 door, idle, travelling, idle */

/* +0x30's thinking: in the room being played (in play) - not come yet: idle (on its triangle's
 * centre the first time); come: its time runs (+0x20), on her level it hasn't given up (+0x2F);
 * when she can't be reached (flag 9, her mode 3) about her (+0xF8 2) unless it did that
 * (+0x9C); she hidden (+0x2D, +0xE0): idle; else for a door (func_002DF5B0) - a closed way
 * (Progress_ExitOpen) ends that (+0x84 2), an opened one when it should open it (+0x88) does
 * (+0x84 0) - by +0x84: 0 after her (from afar, +0x9C, only within 40; else idle with its path
 * dropped), 1 waiting at the door, 2 to the door. Elsewhere: out of play, travelling once it
 * has come, else idle */
void func_002E06E0(Character *c) {
    u8 *k = CR(c);
    Progress *p;
    s32 room;

    if (AT(c, 0x1569, u8) != 0) {
        return;
    }
    func_002E0520(c);
    room = c->a.room;
    p = gProgress;
    if (room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        c->a.disabled = 1;
        AT(k, 0x83, u8) = 0;
        AT(k, 0x2F, u8) = 0;
        if (AT(k, 0x2E, u8) != 0) {
            AT(k, 0x20, s16) += 1;
            ptmf_set(&c->a.state, &D_00416770);
        } else {
            ptmf_set(&c->a.state, &D_00416780);
        }
        return;
    }
    c->a.disabled = 0;
    if (AT(k, 0x2E, u8) == 0) {
        if (c->a.navTri != NAV_NONE && AT(k, 0x9B, s8) == 0) {
            AT(k, 0x9B, s8) += 1;
            VCALL(gNavMesh, 0xC, void (*)(NavMesh *, u32, f32 *))(gNavMesh, c->a.navTri, c->a.pos);
        }
        ptmf_set(&c->a.state, &D_00416760);
        return;
    }
    AT(k, 0x20, s16) += 1;
    if (gCharPlayer->a.pos[1] == c->a.pos[1]) {
        AT(k, 0x2F, u8) = 0;
    }
    if ((Progress_TestFlag(p, 9) != 0 || gCharPlayer->moveMode == 3) && AT(k, 0x9C, s8) == 0) {
        c->moveMode = 2;
        ptmf_set(&c->a.state, &D_00416700);
        return;
    }
    if (gCharPlayer->a.unk2D != 0 && gCharPlayer->unkE0 != 0) {
        c->moveMode = 0;
        ptmf_set(&c->a.state, &D_00416710);
        return;
    }
    c->moveMode = 0;
    if (AT(k, 0x86, u8) == 0) {
        func_002DF5B0(c);
    }
    if (AT(k, 0x84, u8) == 1) {
        if (!(Progress_ExitOpen(p, c->a.room, AT(k, 0x85, u8)) & 0xFF)) {
            AT(k, 0x84, u8) = 2;
            c->a.unk2B = 0;
        } else if ((VCALL(gDoors, 0x30, u32 (*)(VObject *, u32))(gDoors, AT(k, 0x85, u8)) & 0xFF) == 1 &&
                   (AT(k, 0x88, u16) & 1)) {
            AT(k, 0x84, u8) = 0;
        }
    }
    switch (AT(k, 0x84, u8)) {
    case 0:
        if (AT(k, 0x9C, s8) == 0 || AT(k, 0x24, f32) <= 40.0f) {
            ptmf_set(&c->a.state, &D_00416720);
        } else {
            Character_ReleasePath(c);
            AT(k, 0x2B, u8) = 0;
            c->unk124 = c->unk128;
            ptmf_set(&c->a.state, &D_00416730);
        }
        break;
    case 1:
        ptmf_set(&c->a.state, &D_00416740);
        break;
    case 2:
        ptmf_set(&c->a.state, &D_00416750);
        break;
    }
}

extern const PTMF D_004166B0, D_004166C0, D_004166D0, D_004166E0, D_004166F0;

/* +0x30 per frame: its trail (+0x50 / +0x60 / +0x70 in turn: its position 12 above its bob),
 * its doors noted, +0x9C whether Fiona holds item 0x8D (equipment slot 3); thinking
 * (func_002E06E0), bobbing unless seen (+0x37), being seen (outside events, or in her room
 * unless she is busy); then by its state: not come - idle (holding 0x8D: out of play or in
 * place); in play - once its time is up gone with a sound (0x8C, +0x29), else waiting; out of
 * play - when its time is up gone, travelling (out, +0x10 0) or, back in the room being
 * played, back on the mesh. Its behaviour, then +0x40. */
void func_002E1380(Character *c) {
    u8 *k = CR(c);
    Progress *p;
    s32 trail = (AT(k, 0x20, s16) + AT(k, 0x28, s8)) % 3;

    switch (trail) {
    case 2:
        func_0010E5F0((f32 *)(k + 0x70), c->a.pos);
        AT(k, 0x74, f32) += 12.0f + AT(k, 0x14, f32);
        break;
    case 0:
        func_0010E5F0((f32 *)(k + 0x50), c->a.pos);
        AT(k, 0x54, f32) += 12.0f + AT(k, 0x14, f32);
        break;
    case 1:
        func_0010E5F0((f32 *)(k + 0x60), c->a.pos);
        AT(k, 0x64, f32) += 12.0f + AT(k, 0x14, f32);
        break;
    }
    p = gProgress;
    creature_at_doors(c, 0x8A);
    AT(k, 0x9C, s8) = VCALL(gSubScreen, 0x10, s32 (*)(VObject *, s32))(gSubScreen, 3) == 0x8D;
    func_002E06E0(c);
    if (AT(k, 0x37, s8) == 0) {
        func_002DEFA0(c);
    }
    if (Progress_TestFlag(p, 0x18) == 0) {
        func_002DF180(c);
    } else {
        s32 room = c->a.room;

        if (room == VCALL(p, 0xC, s32 (*)(Progress *))(p) && gCharPlayer->unkE0 == 0) {
            func_002DF180(c);
        }
    }
    if (AT(k, 0x2E, u8) == 0) {
        if (AT(k, 0x9C, s8) != 0) {
            ptmf_set(&c->a.state, AT(k, 0x29, u8) != 0 ? &D_004166D0 : &D_004166E0);
        } else {
            ptmf_set(&c->a.state, &D_004166F0);
        }
    } else if (c->a.disabled == 0) {
        if (AT(k, 0x29, u8) != 0) {
            ptmf_set(&c->a.state, &D_004166C0);
        } else if (AT(k, 0x20, s16) >= AT(k, 0x6, s16)) {
            AT(k, 0x29, u8) = 1;
            Actor_PlaySound(&c->a, 0x8C, 5, 0, 0, NULL);
            ptmf_set(&c->a.state, &D_004166B0);
        }
    } else {
        u8 travel = 1;

        if (AT(k, 0x20, s16) >= AT(k, 0x6, s16)) {
            c->a.active = 0;
            Character_ReleasePath(c);
        }
        if (AT(k, 0x10, s32) != 0) {
            s32 room = c->a.room;

            if (room == VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
                creature_back_on_mesh(c);
            }
            travel = 0;
        }
        if (travel) {
            func_002DFA50(c);
        }
    }
    ptmf_scall(c, &c->a.state);
    VCALL(c, 0x40, void (*)(Character *))(c);
}

/* +0x2C draw, once it has come (or, out, when its rest is over): six glows - its core (cell
 * 0x40, white), its colour (red for kinds 0x12.. but 0x24, else blue; size 2.5 + +0x40), three
 * fading ones along its trail (+0x50 / +0x60 / +0x70, smaller each), and its body (cell 0x60,
 * palette 3; vanishing: in its colour like the rest); all additive and see-through when it
 * is vanishing (+0x29). The trail starts at it (+0x83). */
void func_002E19F0(Character *c) {
    u8 *k = CR(c);
    u8 red;
    s32 i;

    if (AT(c, 0x15C2, u8) == 0) {
        if (AT(k, 0x80, s16) != 0) {
            return;
        }
    } else if (AT(k, 0x2E, u8) == 0) {
        return;
    }
    if (AT(k, 0x83, u8) == 0) {
        for (i = 0; i < 3; i++) {
            func_0010E5F0((f32 *)(k + 0x50 + i * 0x10), c->a.pos);
            AT(k, 0x54 + i * 0x10, f32) += 12.0f + AT(k, 0x14, f32);
        }
        AT(k, 0x83, u8) += 1;
    }
    for (i = 0; i < 6; i++) {
        QuadDrawer q __attribute__((aligned(16)));
        QuadRec r __attribute__((aligned(16)));

        red = AT(k, 0x31, u8) >= 0x12 && AT(k, 0x31, u8) != 0x24;
        switch (i) {
        case 0:
            r.rgba[3] = 0x60;
            r.rgba[0] = 0x80;
            r.rgba[1] = 0x80;
            r.rgba[2] = 0x80;
            break;
        case 1:
            if (red) {
                r.rgba[1] = 0;
                r.rgba[0] = 0x80;
                r.rgba[2] = 0;
            } else {
                r.rgba[0] = 0;
                r.rgba[1] = 0;
                r.rgba[2] = 0x80;
            }
            r.rgba[3] = 0x60;
            break;
        case 5:
            r.rgba[0] = 0x80;
            r.rgba[1] = 0x80;
            r.rgba[2] = 0x80;
            r.rgba[3] = 0x80;
            break;
        default:
            if (red) {
                r.rgba[1] = (i + 1) * 0x10;
                r.rgba[2] = (i + 1) * 0x10;
                r.rgba[0] = 0x80 - i * 5;
            } else {
                r.rgba[0] = (i + 1) * 0x10;
                r.rgba[1] = (i + 1) * 0x10;
                r.rgba[2] = 0x80 - i * 5;
            }
            r.rgba[3] = 0x60 - i * 5;
            break;
        }
        if (AT(k, 0x29, u8) == 1 && i != 5) {
            r.rgba[3] = 0;
        }
        if (i >= 2 && i != 5) {
            r.pos[0] = AT(k, 0x30 + i * 0x10, f32);
            r.pos[1] = AT(k, 0x34 + i * 0x10, f32);
            r.pos[2] = AT(k, 0x38 + i * 0x10, f32);
        } else {
            r.pos[0] = c->a.pos[0];
            r.pos[1] = 12.0f + c->a.pos[1] + AT(k, 0x14, f32);
            r.pos[2] = c->a.pos[2];
        }
        r.pos[3] = 1.0f;
        switch (i) {
        case 0:
        case 5:
            r.w = 2.5f;
            r.h = 2.5f;
            break;
        case 1:
            r.w = 2.5f + AT(k, 0x40, f32);
            r.h = 2.5f + AT(k, 0x40, f32);
            break;
        default:
            r.w = 2.5f - 0.5f * (f32)(i - 1);
            r.h = 2.0f - 0x1.99999ap-2f /* 0.4 */ * (f32)(i - 1);
            break;
        }
        *(s32 *)&r.turn = 0;
        r.frame = 0;
        q.a = -1;
        q.tex = (u64)-1;
        q.vtbl = D_0046FC30;
        q.rec = &r;
        *(s32 *)&q.cx = 0;
        *(s32 *)&q.cy = 0;
        q.layer = AT(c, 0x152C, s32) == 0x17 ? 0x17 : 0x19;
        q.count = 1;
        q.cellX = i == 0 ? 0x40 : i == 5 ? 0x60 : 0xA0;
        q.cellY = 0x40;
        q.cellW = 0x20;
        q.cellH = 0x20;
        q.texW = 0x200;
        q.texH = 0x100;
        q.flags = i == 5 ? 0 : -0x40;
        q.frames = 1;
        q.texGroup = 0x10;
        q.texId = 1;
        if (i == 5) {
            if (AT(k, 0x29, u8) == 1) {
                q.flags = -0x40;
                if (red) {
                    r.rgba[0] = 0x80;
                    r.rgba[1] = 0x30;
                    r.rgba[3] = 0x60;
                    r.rgba[2] = 0x30;
                } else {
                    r.rgba[0] = 0x30;
                    r.rgba[1] = 0x30;
                    r.rgba[2] = 0x80;
                    r.rgba[3] = 0x60;
                }
                q.palette = -1;
            } else {
                q.palette = 3;
            }
        } else {
            q.palette = -1;
        }
        func_002E56C0((u8 *)&q);
        q.vtbl = D_00469D00;
    }
}

/* ---- the creature manager (gCreatures; vtable D_0046FC00 at +0x28): a list of 10 at +0x0,
 * its models at +0xF630, its objects' heap at +0xDC40 ---- */

#define CREATURES(m) ((Character **)(m))

/* +0x2C creature `i` (if up): its +0xA8 (a2, with 1) */
void func_002E2360(u8 *m, s32 i, s32 a2) {
    Character *c = CREATURES(m)[i & 0xFF];

    if (c->a.active == 1) {
        VCALL(c, 0xA8, void (*)(Character *, s32, s32, s32))(c, i, a2, c->a.active);
    }
}

/* +0x28 remove creature `i`: its model back (slots 7..9; +0xF630 +0x14, then deleted), it
 * back to the heap (+0xDC40 +0x14) and deleted */
void func_002E23B0(u8 *m, s32 i) {
    u32 n = i & 0xFF;
    Character *c = CREATURES(m)[n];

    if (c == NULL) {
        return;
    }
    if (n >= 7 && n < 10) {
        void **model;

        VCALL(m + 0xF630, 0x14, void (*)(void *, void *))(m + 0xF630, c->motion);
        model = c->motion;
        if (model != NULL) {
            VCALL(model, 0x8, void (*)(void *, s32))(model, 1);
        }
        c->motion = NULL;
    }
    VCALL(m + 0xDC40, 0x14, void (*)(void *, Character *))(m + 0xDC40, c);
    if (c != NULL) {
        VCALL(c, 0x8, void (*)(Character *, s32))(c, 1);
    }
    CREATURES(m)[n] = NULL;
}

/* +0x1C keep them: each one saves itself in its Progress slot (+0xA4: slot, up); an empty slot
 * is cleared */
void func_002E2480(u8 *m) {
    u8 *e = (u8 *)gProgress;
    s32 i;

    for (i = 0; i < 10; i++, e += 36) {
        Character *c = CREATURES(m)[i];

        if (c != NULL) {
            VCALL(c, 0xA4, void (*)(Character *, s32, s32))(c, i, c->a.active == 1);
        } else {
            AT(e, 0x878, s32) = 0;
            AT(e, 0x87C, s32) = 0;
            AT(e, 0x880, s32) = 0;
            AT(e, 0x884, s16) = 0;
            AT(e, 0x886, u8) = 0;
            AT(e, 0x887, u8) = 0;
            AT(e, 0x888, u8) = 0;
            AT(e, 0x889, u8) = 0;
            AT(e, 0x88A, u8) = 0;
            AT(e, 0x88B, u8) = 0;
            AT(e, 0x88C, u8) = 0;
            AT(e, 0x890, s32) = 0;
            AT(e, 0x894, s32) = 0;
            AT(e, 0x898, s32) = 0;
        }
    }
}

/* +0x18 set up creature `i` (if up): its +0xA0 (a1, a2, mode, kind, strength, save slot, x) */
void func_002E2540(u8 *m, s32 a1, s32 a2, s32 mode, s32 i, s32 kind, s32 str, s32 slot, u16 x) {
    Character *c = CREATURES(m)[i & 0xFF];

    if (c != NULL && c->a.active == 1) {
        VCALL(c, 0xA0, void (*)(Character *, s32, s32, s32, s32, s32, s32, u16))(c, a1, a2, mode, kind, str, slot, x);
    }
}

/* +0x14 send creature `i` (if up) after Fiona: its +0x9C (a1, a2, a4) */
void func_002E25A0(u8 *m, s32 a1, s32 a2, s32 i, s32 a4) {
    Character *c = CREATURES(m)[i & 0xFF];

    if (c != NULL && c->a.active == 1) {
        VCALL(c, 0x9C, void (*)(Character *, s32, s32, s32))(c, a1, a2, a4);
    }
}

/* Fiona left by exit `exit`: every creature up hears it (+0x34) */
void func_002E26C0(u8 *m, s32 exit) {
    s32 i;

    for (i = 0; i < 10; i++) {
        Character *c = CREATURES(m)[i];

        if (c != NULL && c->a.active == 1) {
            VCALL(c, 0x34, void (*)(Character *, s32))(c, exit);
        }
    }
}

/* every creature up: its +0x40 (1) */
void func_002E2740(u8 *m) {
    s32 i;

    for (i = 0; i < 10; i++) {
        Character *c = CREATURES(m)[i];

        if (c != NULL && c->a.active == 1) {
            VCALL(c, 0x40, void (*)(Character *, s32))(c, c->a.active);
        }
    }
}

/* a pending message (+0x38681 set, its id +0x38680) shown */
/* (possibly dead code: nothing in the game references it) */
void func_002E27B0(u8 *m) {
    if (AT(m, 0x38681, u8) != 0) {
        VCALL(gBootMessage, 0xC, void (*)(VObject *, u32))(gBootMessage, AT(m, 0x38680, u8));
    }
    AT(m, 0x38681, u8) = 0;
}

/* all of them removed (each one's +0x10 first; the manager's +0x28) */
void func_002E2920(u8 *m) {
    s32 i;

    for (i = 0; i < 10; i++) {
        Character *c = CREATURES(m)[i];

        if (c != NULL) {
            VCALL(c, 0x10, void (*)(Character *))(c);
            VCALL_AT(m, 0x28, 0x28, void (*)(u8 *, u32))(m, i & 0xFF);
        }
    }
}

/* ---- D_00472370 (0x4A0 bytes): a creature's vanishing - a glow (records +0x10 + buffer
 * +0x3A8 * 0x30, frames 0..15 of its animation) and 8 sparks (+0x70 + buffer * 0x180) that
 * burst out, then zig-zag (+0x3BC), slow (+0x430 their drag, doubling) and shrink (+0x450) and
 * fade (+0x470); +0x3B0.. each spark's velocity, +0x490 the frame, +0x494 bits 1 / 2 / 4 the
 * glow done / sparks shrunk / faded (all: it ends). Drawn by the quad drawer at +0x370 ---- */

extern void *D_0046F580[];
/* soft-float doubles as raw bit patterns */

#define VANISH_GLOW(o) ((o) + AT(o, 0x3A8, s32) * 0x30 + 0x10)
#define VANISH_SPARK(o, i) ((o) + AT(o, 0x3A8, s32) * 0x180 + (i) * 0x30 + 0x70)

/* +0x8 destructor (the quad drawer at +0x370 inlined) */
/* 0x00312040 */
u8 *CreatureVanish_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00472370;
    AT(o, 0x370, void **) = D_0046FC30;
    AT(o, 0x370, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* +0xC set up: the drawer (texture group 0x10, additive), the glow 5 across, the sparks 4
 * across with their drag ((0.005 + 0.025 x random) / 2, in doubles), shrink and fade */
/* 0x003128E0 */
void CreatureVanish_Start(u8 *o) {
    static const union { u32 u; f32 f; } k0005 = {0x3BA3D70A}, k002 = {0x3CA3D70A};
    VObject *rng;
    s32 i;

    AT(o, 0x3A8, s32) = 0;
    AT(o, 0x378, s64) = -1;
    AT(o, 0x388, s32) = 0;
    AT(o, 0x38C, s32) = 0;
    AT(o, 0x390, s32) = 0x19;
    AT(o, 0x394, s16) = 1;
    AT(o, 0x396, s16) = 0;
    AT(o, 0x398, s16) = 0x60;
    AT(o, 0x39A, s16) = 0x20;
    AT(o, 0x39C, s16) = 0x20;
    AT(o, 0x39E, s16) = 0x200;
    AT(o, 0x3A0, s16) = 0x100;
    AT(o, 0x3A2, u8) = 0x40;
    AT(o, 0x3A3, u8) = 1;
    AT(o, 0x3A4, u8) = 1;
    AT(o, 0x3A5, u8) = 0x10;
    AT(o, 0x3A6, u8) = 0xFF;
    rng = gRandom;
    AT(VANISH_GLOW(o), 0x20, f32) = 5.0f;
    AT(VANISH_GLOW(o), 0x24, f32) = AT(VANISH_GLOW(o), 0x20, f32);
    AT(VANISH_GLOW(o), 0x28, s32) = 0;
    AT(VANISH_GLOW(o), 0x2C, s32) = 0;
    for (i = 0; i < 8; i++) {
        u8 *p = VANISH_SPARK(o, i);

        AT(p, 0x20, f32) = 4.0f;
        AT(p, 0x24, f32) = AT(p, 0x20, f32);
        AT(p, 0x28, s32) = 0;
        AT(p, 0x2C, s32) = 0;
        AT(o, 0x430 + i * 4, f32) = func_0011F878(func_0011F458(
            func_0011F148(0x3F747AE140000000ULL /* 0.005f */,
                          func_0011F208(0x3F9999999999999AULL /* 0.025 */,
                                        func_0011ED78(VCALL(rng, 0x18, f32 (*)(VObject *))(rng)))),
            0x4000000000000000ULL /* 2.0 */));
        AT(o, 0x450 + i * 4, f32) = k0005.f + k002.f * (f32)(VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 7);
        AT(o, 0x470 + i * 4, f32) = (f32)((VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 3) + 1);
    }
    AT(o, 0x490, s32) = 0;
    AT(o, 0x494, u8) = 0;
}

/* a random sign */
static inline __attribute__((always_inline)) s8 vanish_sign(VObject *rng) {
    return (VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 1) ? -1 : 1;
}

/* +0x18 start: at params' position (+0x0) in its colour (+0x10); each spark a little darker,
 * flung out at random (up to 0.5 a frame each way, upward only), its zig-zag 0.0125..0.05 */
/* 0x003120D0 */
void CreatureVanish_SetParams(u8 *o, const u8 *params) {
    VObject *rng;
    u8 *g = VANISH_GLOW(o);
    s32 i;

    AT(g, 0x0, s32) = params[0x10];
    AT(g, 0x4, s32) = params[0x11];
    AT(g, 0x8, s32) = params[0x12];
    AT(g, 0xC, s32) = params[0x13];
    sceVu0CopyVector((f32 *)(g + 0x10), (f32 *)params);
    rng = gRandom;
    for (i = 0; i < 8; i++) {
        u8 *p = VANISH_SPARK(o, i);
        u32 dark = VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 0x1F;
        s8 sign;
        f32 m;

        AT(p, 0x0, s32) = params[0x10] - dark;
        AT(p, 0x4, s32) = params[0x11] - dark;
        AT(p, 0x8, s32) = params[0x12] - dark;
        AT(p, 0xC, s32) = params[0x13];
        sign = vanish_sign(rng);
        m = 0.25f * (f32)((VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 3) + 1);
        AT(o, 0x3B0 + i * 0x10, f32) = (f32)sign * (VCALL(rng, 0x18, f32 (*)(VObject *))(rng) * m / 2.0f);
        m = 0.25f * (f32)((VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 3) + 1);
        AT(o, 0x3B4 + i * 0x10, f32) = VCALL(rng, 0x18, f32 (*)(VObject *))(rng) * m / 2.0f;
        sign = vanish_sign(rng);
        m = 0.25f * (f32)((VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 3) + 1);
        AT(o, 0x3B8 + i * 0x10, f32) = (f32)sign * (VCALL(rng, 0x18, f32 (*)(VObject *))(rng) * m / 2.0f);
        m = (f32)((VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 3) + 1);
        AT(o, 0x3BC + i * 0x10, f32) = 0x1.99999ap-7f /* 0.0125 */ * (f32)sign * m;
        sceVu0CopyVector((f32 *)(p + 0x10), (f32 *)params);
    }
}

/* +0x14 draw (unless the effects are paused): the glow (cell (0, 0x60), 15 frames), then the
 * sparks (cell (0x60, 0x40)) */
/* 0x00312450 */
void CreatureVanish_Draw(u8 *o) {
    if (func_002D6010(gEffects) != 0) {
        return;
    }
    AT(o, 0x394, s16) = 1;
    AT(o, 0x396, s16) = 0;
    AT(o, 0x398, s16) = 0x60;
    AT(o, 0x3A3, u8) = 0xF;
    AT(o, 0x380, u8 *) = VANISH_GLOW(o);
    func_002E56C0(o + 0x370);
    AT(o, 0x394, s16) = 8;
    AT(o, 0x396, s16) = 0x60;
    AT(o, 0x398, s16) = 0x40;
    AT(o, 0x3A3, u8) = 1;
    AT(o, 0x380, u8 *) = VANISH_SPARK(o, 0);
    func_002E56C0(o + 0x370);
}

/* +0x10 update (0 once all of it is done) */
/* 0x00312510 */
s32 CreatureVanish_Update(u8 *o) {
    VObject *rng;
    u8 *g;
    s32 i, k;

    AT(o, 0x3A8, s32) ^= 1;
    {
        u32 buf = AT(o, 0x3A8, u32);
        u32 *dst = &AT(o, 0x10 + buf * 0x30, u32);
        u32 *src = &AT(o, 0x10 + (buf ^ 1) * 0x30, u32);

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
    }
    g = VANISH_GLOW(o);
    AT(g, 0x2C, s32)++;
    if (AT(g, 0x2C, s32) >= 0xF) {
        AT(g, 0x2C, s32) = 0xF;
        AT(g, 0xC, s32) = 0;
        AT(o, 0x494, u8) |= 1;
    }
    rng = gRandom;
    for (i = 0; i < 8; i++) {
        u32 buf = AT(o, 0x3A8, u32);
        u32 *dst = &AT(o, 0x70 + buf * 0x180 + i * 0x30, u32);
        u32 *src = &AT(o, 0x70 + (buf ^ 1) * 0x180 + i * 0x30, u32);
        f32 *v = &AT(o, 0x3B0 + i * 0x10, f32);
        f32 *drag = &AT(o, 0x430 + i * 4, f32);
        u8 *p;
        f32 w;

        for (k = 0; k < 12; k++) {
            dst[k] = src[k];
        }
        p = VANISH_SPARK(o, i);
        AT(p, 0x2C, s32) = 0;
        if (AT(o, 0x490, s32) < 9) {
            AT(p, 0x10, f32) = AT(p, 0x10, f32) + v[0];
            AT(p, 0x18, f32) = AT(p, 0x18, f32) + v[2];
        } else {
            AT(p, 0x10, f32) = AT(p, 0x10, f32) + v[3];
            AT(p, 0x18, f32) = AT(p, 0x18, f32) + v[3];
            if (AT(o, 0x490, u32) % ((VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 3) + 7) == 0) {
                v[3] = -v[3];
            }
        }
        if (AT(o, 0x490, s32) < 5) {
            AT(p, 0x14, f32) = AT(p, 0x14, f32) + (v[1] - *drag);
            *drag = *drag + *drag;
        } else {
            if (!(v[1] <= *drag)) {
                v[1] = *drag;
                *drag = *drag + *drag;
            }
            if (AT(o, 0x490, s32) % 6 == 0) {
                *drag = *drag + *drag / 12.0f;
            }
            AT(p, 0x14, f32) = AT(p, 0x14, f32) + (v[1] - *drag);
        }
        if (AT(o, 0x490, s32) % 2 == 0) {
            w = AT(p, 0x20, f32) - AT(o, 0x450 + i * 4, f32);
            AT(p, 0x20, f32) = w;
            AT(p, 0x24, f32) = w;
        }
        if (AT(p, 0x20, f32) < 0.0f) {
            AT(p, 0x20, f32) = 0.0f;
            AT(p, 0x24, f32) = 0.0f;
            AT(o, 0x494, u8) |= 2;
        }
        AT(p, 0xC, s32) = (s32)((f32)AT(p, 0xC, s32) - AT(o, 0x470 + i * 4, f32));
        if (AT(p, 0xC, s32) < 0) {
            AT(p, 0xC, s32) = 0;
            AT(o, 0x494, u8) |= 4;
        }
    }
    if (AT(o, 0x494, u8) == 7) {
        return 0;
    }
    AT(o, 0x490, s32)++;
    return 1;
}

/* room 0x4F (D_0040C130): the first of the creatures 0..6 within 4 of (-35.7, -7.5) vanishes
 * there (taken off, its glow - blue for kinds below 0x12, else red - and the sound 0x8B): 1;
 * none, 0 */
s32 func_002B4030(void) {
    static const union { u32 u; f32 f; } kX = {0xC20ECCCD};
    s32 i;

    for (i = 0; i < 7; i++) {
        Character *c = AT(gCreatures, i * 4, Character *);
        u8 *mgr;
        s32 slot;
        f32 dz, dx;
        struct {
            f32 pos[4];
            u8 rgba[4];
        } fx __attribute__((aligned(16)));

        if (c == NULL) {
            continue;
        }
        dz = c->a.pos[2] - -7.5f;
        dx = c->a.pos[0] - kX.f;
        if (!(dz * dz + dx * dx < 16.0f)) {
            continue;
        }
        mgr = gEffects;
        c->a.active = 0;
        slot = Effect_New(mgr, 0x4A0, vanish_init);
        sceVu0CopyVector(fx.pos, c->a.pos);
        fx.pos[1] = 12.0f + c->a.pos[1] + AT(CR(c), 0x14, f32);
        if (AT(CR(c), 0x31, u8) < 0x12) {
            fx.rgba[2] = 0x80;
            fx.rgba[0] = 0x30;
            fx.rgba[3] = 0x60;
            fx.rgba[1] = 0x30;
        } else {
            fx.rgba[0] = 0x80;
            fx.rgba[1] = 0x30;
            fx.rgba[3] = 0x60;
            fx.rgba[2] = 0x30;
        }
        func_002D6090(mgr, slot, &fx);
        Actor_PlaySound(&c->a, 0x8B, 5, 0, 0, NULL);
        return 1;
    }
    return 0;
}

/* (self->*D_01990D40[i])(a, b) */
/* 0x002B4250 */
s32 Room4F_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990D40[i & 0xFF], a, b);
}

/* ---- class D_00472390: a looping sprite (one quad, double-buffered at +0x10 + buffer +0xA8 *
 * 0x30, drawn by the quad drawer at +0x70; 4 frames of 32 x 32 at (0x40, 0) in 512 x 256,
 * layer 0x19), each loop at a new random turn; +0xAC frames per frame, +0xB0 stopped ---- */

extern void *D_00472390[];

#define LOOP_REC(o) ((o) + AT(o, 0xA8, s32) * 0x30 + 0x10)

/* +0x8 destructor (the quad drawer's inlined) */
/* 0x00312B50 */
u8 *LoopingSprite_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_00472390;
    AT(o, 0x70, void **) = D_0046FC30;
    AT(o, 0x70, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* +0x18 start: at the position given; NULL stops it */
/* 0x00312BE0 */
void LoopingSprite_SetParams(u8 *o, f32 *at) {
    if (at == NULL) {
        AT(o, 0xB0, u8) = 1;
        return;
    }
    sceVu0CopyVector((f32 *)(LOOP_REC(o) + 0x10), at);
}

/* +0x14 draw */
/* 0x00312C30 */
void LoopingSprite_Draw(u8 *o) {
    AT(o, 0x80, u8 *) = LOOP_REC(o);
    func_002E56C0(o + 0x70);
}

/* +0x10 update: the buffers swapped (the record copied over); every +0xAC frames the next
 * frame, after the last (+0xA3) the first again at a new turn. 0 once stopped */
/* 0x00312C60 */
s32 LoopingSprite_Update(u8 *o) {
    static const union { u32 u; f32 f; } k2Pi = {0x40C90FDB};
    u32 *dst, *src;
    u32 i;

    if (AT(o, 0xB0, u8) == 1) {
        return 0;
    }
    AT(o, 0xA8, s32) ^= 1;
    dst = (u32 *)LOOP_REC(o);
    src = (u32 *)(o + (AT(o, 0xA8, s32) ^ 1) * 0x30 + 0x10);
    for (i = 0; i < 12; i++) {
        *dst++ = *src++;
    }
    *(volatile s32 *)(o + 0xAC) -= 1;   /* (stored, then read back) */
    if (AT(o, 0xAC, s32) == 0) {
        u8 *r;

        AT(o, 0xAC, s32) = 1;
        r = LOOP_REC(o);
        AT(r, 0x2C, s32) += 1;
        if (!(AT(r, 0x2C, s32) < AT(o, 0xA3, s8))) {
            AT(r, 0x2C, s32) = 0;
            AT(r, 0x28, f32) = k2Pi.f * (VCALL(gRandom, 0x18, f32 (*)(VObject *))(gRandom) - 0.5f);
        }
    }
    return 1;
}

/* +0xC set up: grey, half-transparent, 1.6 across, a random turn; the drawer's settings */
/* 0x00312D90 */
void LoopingSprite_Start(u8 *o) {
    static const union { u32 u; f32 f; } k2Pi = {0x40C90FDB}, kSize = {0x3FCCCCCD};
    u8 *r;

    AT(o, 0xA8, s32) = 0;
    AT(o, 0xAC, s32) = 1;
    AT(o, 0xB0, u8) = 0;
    AT(o, 0x78, s64) = -1;
    AT(o, 0x84, s32) = 0;
    AT(o, 0x88, s32) = 0;
    AT(o, 0x8C, s32) = 0;
    AT(o, 0x90, s32) = 0x19;
    AT(o, 0x94, s16) = 1;
    AT(o, 0x96, s16) = 0x40;
    AT(o, 0x98, s16) = 0;
    AT(o, 0x9A, s16) = 0x20;
    AT(o, 0x9C, s16) = 0x20;
    AT(o, 0x9E, s16) = 0x200;
    AT(o, 0xA0, s16) = 0x100;
    AT(o, 0xA2, s8) = 0;
    AT(o, 0xA3, s8) = 4;
    AT(o, 0xA4, s8) = 1;
    AT(o, 0xA5, s8) = 0x10;
    AT(o, 0xA6, s8) = -1;
    r = LOOP_REC(o);
    AT(r, 0x0, s32) = 0x80;
    AT(r, 0x4, s32) = 0x80;
    AT(r, 0x8, s32) = 0x80;
    AT(r, 0xC, s32) = 0x40;
    AT(r, 0x20, f32) = kSize.f;
    AT(r, 0x24, f32) = AT(r, 0x20, f32);
    AT(r, 0x28, f32) = k2Pi.f * (VCALL(gRandom, 0x18, f32 (*)(VObject *))(gRandom) - 0.5f);
    AT(r, 0x2C, s32) = 0;
}

/* ---- class D_004726E0 (0x220 bytes): a strand hanging from the stalker in slot 2 (drool /
 * blood): up to 4 segments (0x50 each from +0x80: a matrix, alpha +0xC0, cells +0xC4..+0xCA)
 * grown one from the other down a chain of its bones (+0x214: pairs per stalker kind, +0x210
 * the pair), each drawn as two crossed quads +0x20C long with the quad drawer at +0x40 (record
 * +0x10: the colour); +0x218 the kind of strand, +0x21C stopped ---- */

extern void *D_004726E0[];
extern u32 D_00429850[], D_004298F0[], D_00429990[], D_00429A30[], D_00429AD0[], D_00429B70[];

#define STRAND_SEG(o, k) ((o) + (k) * 0x50)
#define STRAND_BONE(o, i) (Skel_Bone(AT(AT(gCharSlot2, 0xF0, u8 *), 0x810, void *), (i)) + 12)

/* +0x8 destructor (the quad drawer's inlined) */
/* 0x00313030 */
u8 *Effect726E0_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_004726E0;
    AT(o, 0x40, void **) = D_0046FC30;
    AT(o, 0x40, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* +0xC set up */
/* 0x00313FC0 */
void Effect726E0_Start(u8 *o) {
    AT(o, 0x21C, u8) = 0;
}

static inline __attribute__((always_inline)) void strand_cells(u8 *s, VObject *rnd) {
    AT(s, 0xC4, s16) = ((VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 3) << 5) + 0x120;
    AT(s, 0xC6, s16) = (VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 1) * 0x10 + 0x40;
    AT(s, 0xC8, s16) = ((VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 3) << 5) + 0x120;
    AT(s, 0xCA, s16) = (VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 1) * 0x10 + 0x40;
}

/* +0x18 start: arg { the stalker's kind, the strand's kind (1 orange, 2 white, else purple) };
 * the bone chain by kind (none: stopped), a random length and first pair, the first segment
 * from that bone a little off at random, turned 45 degrees (give or take 5) each segment */
/* 0x003130C0 */
void Effect726E0_SetParams(u8 *o, s32 *arg) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    f32 d[4] __attribute__((aligned(16)));
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    u8 *r = o + 0x10;
    VObject *rnd;

    if (arg == NULL) {
        AT(o, 0x21C, u8) = 1;
        return;
    }
    switch (arg[0]) {
    case 2: case 6: case 7: case 0x1B:
        AT(o, 0x214, u32 *) = D_00429850;
        break;
    case 3: case 0x22: case 0x23: case 0x24:
        AT(o, 0x214, u32 *) = D_004298F0;
        break;
    case 4:
        AT(o, 0x214, u32 *) = D_00429990;
        break;
    case 0x17: case 0x25:
        AT(o, 0x214, u32 *) = D_00429A30;
        break;
    case 0xB:
        AT(o, 0x214, u32 *) = D_00429AD0;
        break;
    case 0xA: case 0x27:
        AT(o, 0x214, u32 *) = D_00429B70;
        break;
    default:
        AT(o, 0x214, u32 *) = NULL;
        break;
    }
    if (AT(o, 0x214, u32 *) == NULL) {
        AT(o, 0x21C, u8) = 1;
        return;
    }
    AT(o, 0x218, s32) = arg[1];
    if (AT(o, 0x218, s32) == 2) {
        rnd = gRandom;
        AT(r, 0x0, s32) = 0x80;
        AT(r, 0x4, s32) = 0x80;
        AT(r, 0x8, s32) = 0x80;
        AT(o, 0x20C, f32) = 0.0f + 1.0f + 3.0f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    } else if (AT(o, 0x218, s32) == 1) {
        AT(r, 0x0, s32) = 0x80;
        AT(r, 0x4, s32) = 0x64;
        rnd = gRandom;
        AT(r, 0x8, s32) = 0;
        AT(o, 0x20C, f32) = 1.0f + VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    } else {
        AT(r, 0x0, s32) = 0x40;
        AT(r, 0x4, s32) = 0x20;
        AT(r, 0x8, s32) = 0x80;
        rnd = gRandom;
        AT(o, 0x20C, f32) = 1.0f + VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
    }
    AT(o, 0x210, s32) = (VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 7) * 5;
    rnd = gRandom;
    AT(o, 0xC0, s32) = 0;
    AT(o, 0x110, s32) = 0;
    AT(o, 0x160, s32) = 0;
    AT(o, 0x1B0, s32) = 0;
    d[0] = 2.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    d[1] = 2.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    d[2] = 2.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    d[3] = 0.0f;
    sceVu0CopyVector(a, STRAND_BONE(o, AT(o, 0x214, u32 *)[AT(o, 0x210, s32)]));
    a[3] = 1.0f;
    sceVu0AddVector(at, a, d);
    sceVu0Normalize(d, d);
    AT(o, 0x48, s64) = -1;
    AT(o, 0x50, u8 *) = o + 0x10;
    AT(o, 0x58, s32) = 0;
    AT(o, 0x5C, s32) = 0;
    AT(o, 0x60, s32) = 0x19;
    AT(o, 0x64, s16) = 1;
    AT(o, 0x6A, s16) = 0x20;
    AT(o, 0x6C, s16) = 0x10;
    AT(o, 0x6E, s16) = 0x200;
    AT(o, 0x70, s16) = 0x100;
    AT(o, 0x72, s8) = 0x42;
    AT(o, 0x73, s8) = 1;
    AT(o, 0x74, s8) = 1;
    AT(o, 0x75, s8) = 0x10;
    AT(o, 0x76, s8) = -1;
    AT(r, 0x10, s32) = 0;
    AT(r, 0x14, s32) = 0;
    AT(r, 0x18, s32) = 0;
    AT(r, 0x1C, f32) = 1.0f;
    AT(r, 0x20, f32) = 1.0f;
    AT(r, 0x24, f32) = 1.0f;
    AT(r, 0x28, s32) = 0;
    AT(r, 0x2C, s32) = 0;
    AT(o, 0x204, s32) = 0;
    AT(o, 0x208, s32) = (VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 3) + 1;
    AT(o, 0x200, s32) = 0;
    sceVu0UnitMatrix((f32 (*)[4])(o + 0x1C0));
    sceVu0RotMatrixZ((f32 (*)[4])(o + 0x1C0), (f32 (*)[4])(o + 0x1C0),
                     kPi.f * (0.0f + -45.0f + 10.0f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f)) / 180.0f);
    sceVu0UnitMatrix((f32 (*)[4])(o + 0x80));
    sceVu0CopyVector((f32 *)(o + 0x80), d);
    sceVu0CopyVector(b, STRAND_BONE(o, AT(o, 0x214, u32 *)[AT(o, 0x210, s32) + 1]));
    b[3] = 1.0f;
    sceVu0SubVector((f32 *)(o + 0xA0), b, a);
    sceVu0Normalize((f32 *)(o + 0xA0), (f32 *)(o + 0xA0));
    sceVu0OuterProduct((f32 *)(o + 0xA0), (f32 *)(o + 0xA0), (f32 *)(o + 0x80));
    sceVu0Normalize((f32 *)(o + 0xA0), (f32 *)(o + 0xA0));
    sceVu0OuterProduct((f32 *)(o + 0x90), (f32 *)(o + 0xA0), (f32 *)(o + 0x80));
    sceVu0TransMatrix((f32 (*)[4])(o + 0x80), (f32 (*)[4])(o + 0x80), at);
    AT(o, 0xC0, s32) = (VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 0xF) + 0x50;
    strand_cells(o, rnd);
}

/* +0x14 draw (unless the effects are paused or it stopped): each live segment as two crossed
 * quads in its frame (x along, the second turned a quarter) */
/* 0x003136C0 */
void Effect726E0_Draw(u8 *o) {
    f32 c[4][4] __attribute__((aligned(16)));
    f32 h, len;
    s32 k;

    if (func_002D6010(gEffects) != 0 || AT(o, 0x21C, u8) == 1) {
        return;
    }
    len = AT(o, 0x20C, f32);
    h = 0.5f * len;
    AT(o, 0x54, f32 *) = c[0];
    for (k = 0; k < 4; k++) {
        u8 *s = STRAND_SEG(o, k);
        f32 (*m)[4] = (f32 (*)[4])(s + 0x80);

        if (AT(s, 0xC0, s32) == 0) {
            continue;
        }
        AT(o, 0x1C, s32) = AT(s, 0xC0, s32);
        AT(o, 0x66, s16) = AT(s, 0xC4, s16);
        AT(o, 0x68, s16) = AT(s, 0xC6, s16);
        c[0][0] = 0.0f;
        c[0][3] = 1.0f;
        c[0][1] = 0.0f;
        c[0][2] = h;
        sceVu0ApplyMatrix(c[0], m, c[0]);
        c[1][3] = 1.0f;
        c[1][0] = AT(o, 0x20C, f32);
        c[1][1] = 0.0f;
        c[1][2] = h;
        sceVu0ApplyMatrix(c[1], m, c[1]);
        c[2][2] = -h;
        c[2][1] = 0.0f;
        c[2][0] = 0.0f;
        c[2][3] = 1.0f;
        sceVu0ApplyMatrix(c[2], m, c[2]);
        c[3][3] = 1.0f;
        c[3][0] = AT(o, 0x20C, f32);
        c[3][1] = 0.0f;
        c[3][2] = -h;
        sceVu0ApplyMatrix(c[3], m, c[3]);
        func_002E56C0(o + 0x40);
        AT(o, 0x66, s16) = AT(s, 0xC8, s16);
        AT(o, 0x68, s16) = AT(s, 0xCA, s16);
        c[0][0] = 0.0f;
        c[0][3] = 1.0f;
        c[0][1] = h;
        c[0][2] = 0.0f;
        sceVu0ApplyMatrix(c[0], m, c[0]);
        c[1][3] = 1.0f;
        c[1][0] = AT(o, 0x20C, f32);
        c[1][1] = h;
        c[1][2] = 0.0f;
        sceVu0ApplyMatrix(c[1], m, c[1]);
        c[2][1] = -h;
        c[2][2] = 0.0f;
        c[2][0] = 0.0f;
        c[2][3] = 1.0f;
        sceVu0ApplyMatrix(c[2], m, c[2]);
        c[3][3] = 1.0f;
        c[3][0] = AT(o, 0x20C, f32);
        c[3][1] = -h;
        c[3][2] = 0.0f;
        sceVu0ApplyMatrix(c[3], m, c[3]);
        func_002E56C0(o + 0x40);
    }
}

/* +0x10 update: the next segment grown from the current one (its frame turned by +0x1C0, moved
 * on by the length) once it has shown a frame, every 4 the bone pair moved on (the end of the
 * chain: no new segment; else the segment aimed down the bones); every live segment fades by
 * 8..15 (gone under 17) with new cells; for a kind of strand, after +0x208 frames a drop
 * (D_004727C0) from the newest segment, in its colour by the fade. 0 once it stopped */
/* 0x00313980 */
s32 Effect726E0_Update(u8 *o) {
    VObject *rnd;
    u8 *s;
    s32 cur, k;

    if (AT(o, 0x21C, u8) == 1) {
        return 0;
    }
    AT(o, 0x21C, u8) = 1;
    cur = AT(o, 0x200, s32);
    if (AT(STRAND_SEG(o, cur), 0xC0, s32) > 0) {
        if (AT(o, 0x204, s32) == 0) {
            AT(o, 0x204, s32) += 1;
        } else {
            f32 v[4] __attribute__((aligned(16)));
            f32 (*prev)[4] = (f32 (*)[4])(STRAND_SEG(o, cur) + 0x80);

            AT(o, 0x200, s32) = cur + 1;
            if (!(AT(o, 0x200, s32) < 4)) {
                AT(o, 0x200, s32) = 0;
            }
            sceVu0MulMatrix((f32 (*)[4])(STRAND_SEG(o, AT(o, 0x200, s32)) + 0x80), prev,
                            (f32 (*)[4])(o + 0x1C0));
            sceVu0ScaleVector(v, prev[0], AT(o, 0x20C, f32));
            sceVu0TransMatrix((f32 (*)[4])(STRAND_SEG(o, AT(o, 0x200, s32)) + 0x80),
                              (f32 (*)[4])(STRAND_SEG(o, AT(o, 0x200, s32)) + 0x80), v);
            k = (VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) & 0xF) + 0x70;
            AT(STRAND_SEG(o, AT(o, 0x200, s32)), 0xC0, s32) = k;
            AT(o, 0x204, s32) += 1;
            if (!(AT(o, 0x204, s32) < 4)) {
                u32 *pair;

                AT(o, 0x204, s32) = 0;
                AT(o, 0x210, s32) += 1;
                pair = AT(o, 0x214, u32 *) + AT(o, 0x210, s32);
                if (pair[1] == (u32)-1) {
                    AT(STRAND_SEG(o, AT(o, 0x200, s32)), 0xC0, s32) = 0;
                } else {
                    f32 a[4] __attribute__((aligned(16)));
                    f32 b[4] __attribute__((aligned(16)));

                    sceVu0CopyVector(a, STRAND_BONE(o, pair[0]));
                    sceVu0CopyVector(b, STRAND_BONE(o, AT(o, 0x214, u32 *)[AT(o, 0x210, s32) + 1]));
                    s = STRAND_SEG(o, AT(o, 0x200, s32));
                    sceVu0SubVector((f32 *)(s + 0x80), b, (f32 *)(s + 0xB0));
                    s = STRAND_SEG(o, AT(o, 0x200, s32));
                    sceVu0Normalize((f32 *)(s + 0x80), (f32 *)(s + 0x80));
                    s = STRAND_SEG(o, AT(o, 0x200, s32));
                    sceVu0SubVector((f32 *)(s + 0xA0), b, a);
                    s = STRAND_SEG(o, AT(o, 0x200, s32));
                    sceVu0Normalize((f32 *)(s + 0xA0), (f32 *)(s + 0xA0));
                    s = STRAND_SEG(o, AT(o, 0x200, s32));
                    sceVu0OuterProduct((f32 *)(s + 0xA0), (f32 *)(s + 0xA0), (f32 *)(s + 0x80));
                    s = STRAND_SEG(o, AT(o, 0x200, s32));
                    sceVu0OuterProduct((f32 *)(s + 0x90), (f32 *)(s + 0xA0), (f32 *)(s + 0x80));
                    s = STRAND_SEG(o, AT(o, 0x200, s32));
                    sceVu0Normalize((f32 *)(s + 0x90), (f32 *)(s + 0x90));
                }
            }
        }
    }
    rnd = gRandom;
    for (k = 0; k < 4; k++) {
        s = STRAND_SEG(o, k);
        if (AT(s, 0xC0, s32) <= 0) {
            continue;
        }
        AT(o, 0x21C, u8) = 0;
        AT(s, 0xC0, s32) -= (VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 7) + 8;
        if (AT(s, 0xC0, s32) < 0x11) {
            AT(s, 0xC0, s32) = 0;
        }
        strand_cells(s, rnd);
    }
    if (AT(o, 0x218, s32) != 0) {
        AT(o, 0x208, s32) -= 1;
        if (AT(o, 0x208, s32) == 0) {
            s32 idx = AT(o, 0x200, s32) - 1;
            s32 alpha;

            if (idx < 0) {
                idx = 4;   /* (sic: one past the last) */
            }
            alpha = AT(STRAND_SEG(o, idx), 0xC0, s32);
            if (alpha >= 0x21) {
                u8 *mgr = gEffects;
                s32 slot = Effect_New(mgr, 0xF70, DropSplash_Init);
                s32 p[8] __attribute__((aligned(16)));
                f32 f = (f32)alpha / 128.0f;

                p[0] = (s32)((f32)AT(o, 0x10, s32) * f);
                p[1] = (s32)((f32)AT(o, 0x14, s32) * f);
                p[2] = (s32)((f32)AT(o, 0x18, s32) * f);
                AT(&p[3], 0, f32) = AT(STRAND_SEG(o, AT(o, 0x200, s32)), 0xB0, f32);
                AT(&p[4], 0, f32) = AT(STRAND_SEG(o, AT(o, 0x200, s32)), 0xB4, f32);
                AT(&p[5], 0, f32) = AT(STRAND_SEG(o, AT(o, 0x200, s32)), 0xB8, f32);
                AT(&p[6], 0, f32) = AT(o, 0x218, s32) == 1 ? 1.0f : 2.0f;
                func_002D6090(mgr, slot, p);
            }
        }
    }
    return 1;
}

/* ---- D_004727C0 (0xF68 bytes): the strand's drop splash, 32 droplets in two buffers of quad
 * records (+0x10 + 0x600 x the current one +0xF60), each with a velocity (+0xC60) and a pull
 * against it (+0xDE0, 12 bytes each); the quad drawer at +0xC10, the splash point at +0xC50
 * (w 1.0 until a one-frame flash has been drawn there) ---- */

#define DROP_REC(o, buf, i) ((QuadRec *)((o) + 0x10 + (buf) * 0x600) + (i))
#define DROP_VEL(o, i) ((f32 *)((o) + 0xC60 + (i) * 0xC))
#define DROP_PULL(o, i) ((f32 *)((o) + 0xDE0 + (i) * 0xC))

/* +0x8 destructor (the quad drawer's inlined) */
/* 0x00314260 */
u8 *StrandSplash_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_004727C0;
    AT(o, 0xC10, void **) = D_0046FC30;
    AT(o, 0xC10, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

static s32 clamp_colour(s32 c) {
    c <<= 1;
    if (!(c < 0x100)) {
        c = 0xFF;
    }
    return c;
}

/* +0x18 start (arg: colour 0..127 x3, position, size): every droplet at the point in the
 * colour, a random alpha and size, flung out at random (slower the bigger) and pulled back by
 * 10..30% of its speed */
/* 0x003142F0 */
void StrandSplash_SetParams(u8 *o, s32 *arg) {
    static const union { u32 u; f32 f; } kTenth = {0x3DCCCCCD}, kFifth = {0x3E4CCCCD},
                                         kThreeTenths = {0x3E99999A};
    VObject *rnd;
    s32 r, g, b, i;
    f32 small, big;

    if (arg == NULL) {
        return;
    }
    r = clamp_colour(arg[0]);
    g = clamp_colour(arg[1]);
    b = clamp_colour(arg[2]);
    rnd = gRandom;
    AT(o, 0xC50, f32) = ((f32 *)arg)[3];
    AT(o, 0xC54, f32) = ((f32 *)arg)[4];
    AT(o, 0xC58, f32) = ((f32 *)arg)[5];
    AT(o, 0xC5C, f32) = 1.0f;
    small = kTenth.f * ((f32 *)arg)[6];
    big = 4.0f * ((f32 *)arg)[6];
    for (i = 0; i < 32; i++) {
        QuadRec *q = DROP_REC(o, AT(o, 0xF60, s32), i);
        f32 *v = DROP_VEL(o, i);
        f32 *p = DROP_PULL(o, i);
        f32 speed, k;

        q->rgba[0] = r;
        q->rgba[1] = g;
        q->rgba[2] = b;
        q->rgba[3] = (VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 0x1F) + 0x60;
        sceVu0CopyVector(q->pos, (f32 *)(o + 0xC50));
        q->w = 0.0f + small + kThreeTenths.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        q->h = q->w;
        q->turn = 0.0f;
        q->frame = 0;
        speed = 0.0f + big - 10.0f * q->w;
        v[0] = speed * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        v[1] = speed * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        v[2] = speed * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        k = 0.0f + kTenth.f + kFifth.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        p[0] = -(v[0] * k);
        p[1] = -(v[1] * k);
        p[2] = -(v[2] * k);
    }
}

/* the droplets drawn (unless the effects are paused), and on the first frame a flash (2 x 2) of
 * colour r, g, b at the point */
static inline void drops_draw(u8 *o, s32 cr, s32 cg, s32 cb) {
    if (func_002D6010(gEffects) != 0) {
        return;
    }
    AT(o, 0xC20, QuadRec *) = DROP_REC(o, AT(o, 0xF60, s32), 0);
    func_002E56C0(o + 0xC10);
    if (AT(o, 0xC5C, f32) == 1.0f) {
        QuadDrawer q __attribute__((aligned(16)));
        QuadRec r __attribute__((aligned(16)));

        r.rgba[0] = cr;
        r.rgba[1] = cg;
        r.rgba[2] = cb;
        r.rgba[3] = 0x80;
        sceVu0CopyVector(r.pos, (f32 *)(o + 0xC50));
        r.w = 2.0f;
        r.h = 2.0f;
        r.turn = 0.0f;
        r.frame = 0;
        q.vtbl = D_0046FC30;
        q.a = -1;
        q.tex = -1;
        q.rec = &r;
        q.corners = 0;
        q.cx = 0.0f;
        q.cy = 0.0f;
        q.layer = 0x19;
        q.count = 1;
        q.cellX = 0x40;
        q.cellY = 0x40;
        q.cellW = 0x20;
        q.cellH = 0x20;
        q.texW = 0x200;
        q.texH = 0x100;
        q.flags = 0;
        q.frames = 1;
        q.texId = 1;
        q.texGroup = 0x10;
        q.palette = -1;
        func_002E56C0((u8 *)&q);
        AT(o, 0xC5C, f32) = 0.0f;
        q.vtbl = D_00469D00;
    }
}

/* +0x14 draw: the droplets, a white flash on the first frame */
/* 0x003145A0 */
void StrandSplash_Draw(u8 *o) {
    drops_draw(o, 0xC0, 0xC0, 0xC0);
}

/* +0x10 update: flip the buffers (the new one copied from the old); every droplet still seen
 * flickers, slows by its pull and moves on; it goes out once it has (nearly) stopped rising or
 * falling. 0 once none was left last time */
/* 0x00314700 */
s32 StrandSplash_Update(u8 *o) {
    static const union { u32 u; f32 f; } kHundredth = {0x3C23D70A}, kMinusHundredth = {0xBC23D70A};
    VObject *rnd;
    s32 i;

    if (AT(o, 0xF64, u8) == 1) {
        return 0;
    }
    AT(o, 0xF64, u8) = 1;
    rnd = gRandom;
    AT(o, 0xF60, s32) ^= 1;
    for (i = 0; i < 32; i++) {
        QuadRec *q;
        f32 *v = DROP_VEL(o, i);
        f32 *p = DROP_PULL(o, i);

        *DROP_REC(o, AT(o, 0xF60, s32), i) = *DROP_REC(o, AT(o, 0xF60, s32) ^ 1, i);
        q = DROP_REC(o, AT(o, 0xF60, s32), i);
        if (q->rgba[3] <= 0) {
            continue;
        }
        AT(o, 0xF64, u8) = 0;
        q->rgba[3] = (VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 0x7F) + 1;
        v[0] = v[0] + p[0];
        v[1] = v[1] + p[1];
        if (!(p[1] <= 0.0f)) {
            if (!(v[1] <= kMinusHundredth.f)) {
                q->rgba[3] = 0;
            }
        } else if (v[1] < kHundredth.f) {
            q->rgba[3] = 0;
        }
        v[2] = v[2] + p[2];
        q->pos[0] = q->pos[0] + v[0];
        q->pos[1] = q->pos[1] + v[1];
        q->pos[2] = q->pos[2] + v[2];
    }
    return 1;
}

/* 0x00314910 */
void StrandSplash_Start(u8 *self) {
    S32(self, 0xF60) = 0;
    self[0xF64] = 0;
    S64(self, 0xC18) = -1;
    S32(self, 0xC24) = 0;
    S32(self, 0xC28) = 0;
    S32(self, 0xC2C) = 0;
    S32(self, 0xC30) = 25;
    S16(self, 0xC34) = 0x20;
    S16(self, 0xC36) = 0x6C;
    S16(self, 0xC38) = 0x4C;
    S16(self, 0xC3A) = 8;
    S16(self, 0xC3C) = 8;
    S16(self, 0xC3E) = 0x200;
    S16(self, 0xC40) = 0x100;
    self[0xC42] = 0x40;
    self[0xC43] = 1;
    self[0xC44] = 1;
    self[0xC45] = 0x10;
    self[0xC46] = 0xFF;
}

/* ---- the same shapes in other classes, generated from the functions they copy (2026-10-05) ---- */
extern void *D_004737D0[];
extern void *D_00474080[];

/* (as Effect726E0_dtor)  +0x8 destructor (the quad drawer's inlined) */
/* 0x0031E890 */
u8 *Effect737D0_dtor(u8 *o, s32 flags) {
    if (o == NULL) {
        return o;
    }
    AT(o, 0x0, void **) = D_004737D0;
    AT(o, 0x40, void **) = D_0046FC30;
    AT(o, 0x40, void **) = D_00469D00;
    AT(o, 0x0, void **) = D_0046F580;
    if ((s16)flags > 0) {
        func_002D63B0(o);
    }
    return o;
}

/* 0x0031E920 */
void Effect737D0_SetParams(u8 *self, u8 *src) {
    U32(self, 0x10) = U32(src, 0x0);
    U32(self, 0x14) = U32(src, 0x4);
    U32(self, 0x18) = U32(src, 0x8);
    U32(self, 0x1C) = U32(src, 0xC);
    F32(self, 0x20) = F32(src, 0x10);
    F32(self, 0x24) = F32(src, 0x14);
    F32(self, 0x28) = F32(src, 0x18);
    F32(self, 0x2C) = 1.0f;
    F32(self, 0x30) = F32(src, 0x1C);
    F32(self, 0x34) = F32(src, 0x1C);
    S32(self, 0x38) = 0;
    S32(self, 0x3C) = 0;
}

/* its quad drawer (+0x40) on its one record (+0x10), drawn */
/* 0x0031E980 */
void Effect737D0_Draw(u8 *o) {
    AT(o, 0x50, u8 *) = o + 0x10;
    func_002E56C0(o + 0x40);
}

/* 0x0031E990 */
s32 Effect737D0_Update(void) {
    return 0x1;
}

/* 0x0031E9A0 */
void Effect737D0_Start(u8 *self) {
    S64(self, 0x48) = -1;
    S32(self, 0x58) = 0;
    S32(self, 0x5C) = 0;
    S32(self, 0x60) = 25;
    S16(self, 0x64) = 1;
    S16(self, 0x66) = 0xA0;
    S16(self, 0x68) = 0x40;
    S16(self, 0x6A) = 0x20;
    S16(self, 0x6C) = 0x20;
    S16(self, 0x6E) = 0x200;
    S16(self, 0x70) = 0x100;
    self[0x72] = 0x40;
    self[0x73] = 1;
    self[0x74] = 1;
    self[0x75] = 0x10;
    self[0x76] = 0xFF;
}

/* (class D_0047A710, as StrandSplash_SetParams)  +0x18 start (arg: colour 0..127 x3, position): every
 * droplet at the point in the colour, a random alpha and size (0.2..0.6), flung out at random
 * (slower the bigger, three times as fast upwards) and pulled back by 20..50% of its speed */
/* 0x0037BF10 */
void DropletFlash_SetParams(u8 *o, s32 *arg) {
    static const union { u32 u; f32 f; } kFifth = {0x3E4CCCCD}, kTwoFifths = {0x3ECCCCCD},
                                         kThreeTenths = {0x3E99999A};
    VObject *rnd;
    s32 r, g, b, i;

    if (arg == NULL) {
        return;
    }
    r = clamp_colour(arg[0]);
    g = clamp_colour(arg[1]);
    b = clamp_colour(arg[2]);
    rnd = gRandom;
    AT(o, 0xC50, f32) = ((f32 *)arg)[3];
    AT(o, 0xC54, f32) = ((f32 *)arg)[4];
    AT(o, 0xC58, f32) = ((f32 *)arg)[5];
    AT(o, 0xC5C, f32) = 1.0f;
    for (i = 0; i < 32; i++) {
        QuadRec *q = DROP_REC(o, AT(o, 0xF60, s32), i);
        f32 *v = DROP_VEL(o, i);
        f32 *p = DROP_PULL(o, i);
        f32 speed, k;

        q->rgba[0] = r;
        q->rgba[1] = g;
        q->rgba[2] = b;
        q->rgba[3] = (VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 0x1F) + 0x60;
        sceVu0CopyVector(q->pos, (f32 *)(o + 0xC50));
        q->w = 0.0f + kFifth.f + kTwoFifths.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        q->h = q->w;
        q->turn = 0.0f;
        q->frame = 0;
        speed = 0.0f + 8.0f - 10.0f * q->w;
        v[0] = speed * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        v[1] = 3.0f * speed * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        v[2] = speed * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
        k = 0.0f + kFifth.f + kThreeTenths.f * VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd);
        p[0] = -(v[0] * k);
        p[1] = -(v[1] * k);
        p[2] = -(v[2] * k);
    }
}

/* (class D_0047A710)  +0x14 draw: the droplets, a reddish flash (0x80, 0x50, 0x40) on the first
 * frame */
/* 0x0037C1B0 */
void DropletFlash_Draw(u8 *o) {
    drops_draw(o, 0x80, 0x50, 0x40);
}

/* (as StrandSplash_Update)  +0x10 update: flip the buffers (the new one copied from the old); every droplet still seen
 * flickers, slows by its pull and moves on; it goes out once it has (nearly) stopped rising or
 * falling. 0 once none was left last time */
/* 0x0037C310 */
s32 DropletFlash_Update(u8 *o) {
    static const union { u32 u; f32 f; } kHundredth = {0x3C23D70A}, kMinusHundredth = {0xBC23D70A};
    VObject *rnd;
    s32 i;

    if (AT(o, 0xF64, u8) == 1) {
        return 0;
    }
    AT(o, 0xF64, u8) = 1;
    rnd = gRandom;
    AT(o, 0xF60, s32) ^= 1;
    for (i = 0; i < 32; i++) {
        QuadRec *q;
        f32 *v = DROP_VEL(o, i);
        f32 *p = DROP_PULL(o, i);

        *DROP_REC(o, AT(o, 0xF60, s32), i) = *DROP_REC(o, AT(o, 0xF60, s32) ^ 1, i);
        q = DROP_REC(o, AT(o, 0xF60, s32), i);
        if (q->rgba[3] <= 0) {
            continue;
        }
        AT(o, 0xF64, u8) = 0;
        q->rgba[3] = (VCALL(rnd, 0x10, s32 (*)(VObject *))(rnd) & 0x7F) + 1;
        v[0] = v[0] + p[0];
        v[1] = v[1] + p[1];
        if (!(p[1] <= 0.0f)) {
            if (!(v[1] <= kMinusHundredth.f)) {
                q->rgba[3] = 0;
            }
        } else if (v[1] < kHundredth.f) {
            q->rgba[3] = 0;
        }
        v[2] = v[2] + p[2];
        q->pos[0] = q->pos[0] + v[0];
        q->pos[1] = q->pos[1] + v[1];
        q->pos[2] = q->pos[2] + v[2];
    }
    return 1;
}

/* (as func_002DE490)  +0x8 destructor */
Character *func_00324710(Character *c, s32 flags) {
    if (c != NULL) {
        AT(c, 0x0, void **) = D_00474080;
        if (c != NULL) {
            AT(c, 0x0, void **) = D_0046FB50;
            if (c != NULL) {
                AT(c, 0x0, void **) = D_00469C60;
                if (c != NULL) {
                    AT(c, 0x0, void **) = D_00469C20;
                }
            }
        }
        if ((s16)flags > 0) {
            func_002E2320(c);
        }
    }
    return c;
}

void func_00324790(u8 *self, s32 unused, u32 v) {
    S32(self, 0x1540) = v < 3 ? (s32)v : -1;
}

s32 func_003247C0(u8 *self) {
    return self[0x15AF] != 0;
}

/* (as func_002E19A0)  +0x28 put on triangle `tri` (Character_Place), remembering it as the previous one and the
   position (+0x38 / +0x40) */
s32 func_0032BD40(Character *c, u32 tri, const f32 *heading, f32 *pos) {
    s32 r = Character_Place(c, tri, heading, pos);

    c->a.prevNavTri = tri;
    sceVu0CopyVector(c->a.prevPos, c->a.pos);
    return r;
}

/* ---- the same in the other creature classes, whose own block is laid out differently ---- */

/* (as func_002DF860; its request flag at +0x64) */
s32 func_003255C0(Character *c, u32 tri, const f32 *goal, s32 direct) {
    return creature_path(c, tri, goal, direct, 0x64);
}

/* (as func_002E02B0; +0x61 / +0x0) */
void func_00329360(Character *c) {
    creature_route(c, 0x61, 0x0);
}

/* (as func_002E0C30; the mode at +0x0, the doors at +0x50) */
s32 func_0032A8D0(Character *c, s32 room, u32 tri, s32 mode) {
    return creature_place(c, room, tri, mode, 0x0, 0x50);
}

extern void func_003250D0(Character *c, u32 tri);

/* (as func_002E01C0) the other class's: to the door it chose (+0x66 of its block), its spot
   (+0x30 of its block) as the target */
void func_00329270(Character *c) {
    u8 *k = CR(c);
    VObject *rooms = gRooms;
    f32 at[4] __attribute__((aligned(16)));
    f32 spot[4] __attribute__((aligned(16)));
    u32 tri;

    tri = VCALL(rooms, 0x30, u32 (*)(VObject *, u32, f32 *))(rooms, AT(c, 0x15A6, u8), at);
    if (Character_PlanPathKind(c, 0, tri, at) > 0) {
        VCALL(gSceneGameF29740, 0x14, s32 (*)(VObject *))(gSceneGameF29740);
    }
    if (AT(k, 0x66, u8) == 0xFF) {
        return;
    }
    c->unk100 = AT(k, 0x66, u8);
    tri = VCALL(rooms, 0x30, u32 (*)(VObject *, u32, f32 *))(rooms, AT(k, 0x66, u8), spot);
    sceVu0CopyVector((f32 *)(k + 0x30), spot);
    c->unk124 = c->unk128;
    func_003250D0(c, tri);
}

/* ---- the room creature of kind 0x19 (vtable D_00474130; code 0x3247D0..0x32C240). Its own
   block (CR): +0x0 the side it came in by, +0x4 a speed, +0x30 its target, +0x40 / +0x42
   timers, +0x4C the may-use flag, +0x50 its 8 doors, +0x60.. state bytes (+0x61 the route
   flag, +0x63 the door it went by, +0x64 the planner request, +0x65 snapped, +0x66 the door it
   chose, +0x6B flags, bit 0x80 kept). It walks by its animation's root motion. ---- */

s32 func_00325410(Character *c, s32 exit);

/* the step this frame: the root motion forward (+z), not backwards */
static inline f32 cr19_stride(Character *c) {
    f32 v[4] __attribute__((aligned(16)));

    func_001F6370(c->motion, v, 0.0f);
    *(s32 *)&v[1] = 0;
    if (v[2] < 0.0f) {
        v[2] = 0.0f;
    }
    return v[2];
}

/* reset: stopped (its path dropped), speed 0.7, a wait of 150 / 300 / 450 (+0x42), no target;
   its flags +0x6B kept only with bit 0x80 */
static inline void cr19_settle(Character *c) {
    u8 *k = CR(c);

    AT(k, 0x64, u8) = 0;
    AT(k, 0x60, u8) = 0;
    AT(k, 0x40, s16) = 0;
    AT(k, 0x6C, u8) = 0;
    AT(k, 0x4, u32) = 0x3F333333;   /* 0.7f */
    AT(k, 0x1C, s32) = 0;
    if (!(AT(k, 0x6B, u8) & 0x80)) {
        AT(k, 0x6B, u8) = 0;
    }
    AT(k, 0x42, s16) = (VCALL(gRandom, 0x10, u32 (*)(VObject *))(gRandom) & 0xF) % 3 * 150 + 150;
    AT(k, 0x3C, s32) = 0;
    AT(k, 0x38, s32) = 0;
    AT(k, 0x34, s32) = 0;
    AT(k, 0x30, s32) = 0;
}

/* into state `st` (+0x69): move 0, stopped and settled */
static inline void cr19_enter(Character *c, u8 st) {
    AT(CR(c), 0x69, u8) = st;
    AT(c, 0xF8, s32) = 0;
    Character_ReleasePath(c);
    cr19_settle(c);
}

static inline void cr19_reset(Character *c) {
    cr19_enter(c, 0);
}

/* (as func_002DF470) on the way to `tri`: while not there the path ahead is looked at (one
   stride; unused); a door on the way (+0x4C bit 0) with an exit +0x100 is gone through */
void func_003250D0(Character *c, u32 tri) {
    u8 *k = CR(c);

    if (tri != Actor_TriTo(&c->a, (f32 *)(k + 0x30), NAV_NONE)) {
        u32 t;
        f32 near[4] __attribute__((aligned(16)));
        f32 dir[4] __attribute__((aligned(16)));
        f32 fwd[4] __attribute__((aligned(16)));

        Character_WaypointAhead(c, &t, near, cr19_stride(c));
        sceVu0SubVector(dir, near, c->a.pos);
        *(s32 *)&dir[1] = 0;
        sceVu0Normalize(dir, dir);
        fwd[2] = 1.0f;
        *(s32 *)&fwd[0] = 0;
        *(s32 *)&fwd[1] = 0;
        sceVu0ApplyMatrix(fwd, c->a.rot, fwd);
    }
    if (!(AT(k, 0x4C, u16) & 1)) {
        return;
    }
    if ((VCALL(gRooms, 0x10, u32 (*)(VObject *, s32, u32))(gRooms, c->a.room, (u8)c->unk100) & 0xFFFF) != 0xFFFF) {
        AT(k, 0x65, u8) = 0;
        func_00325410(c, (u8)c->unk100);
    }
}

/* (as func_002DF5B0, its door permissions at +0x1590) head for the first usable door */
void func_00325220(Character *c) {
    u8 *k = CR(c);
    VObject *doors = gDoors, *rooms = gRooms;
    f32 at[4] __attribute__((aligned(16)));
    f32 p[4] __attribute__((aligned(16)));
    u32 d, t;

    for (d = 0; d < 8; d = (d + 1) & 0xFF) {
        u16 may;

        if ((VCALL(doors, 0x40, u32 (*)(VObject *, u32))(doors, d) & 0xFF) != 1) {
            continue;
        }
        may = (AT(c, 0x1590 + (d & 0xFF) * 2, u16) & ((1 << *(u8 *)&c->a.slot) & 0xFFFF)) != 0;
        if (VCALL(doors, 0x30, u32 (*)(VObject *, u32, s32, s32))(doors, d, 1, 0) & 0xFF) {
            continue;
        }
        if (may & 1) {
            AT(k, 0x66, u8) = d;
            AT(k, 0x4C, u16) = may;
            c->a.unk2B = 1;
            if (func_003255C0(c, VCALL(rooms, 0x30, u32 (*)(VObject *, u32, f32 *))(rooms, AT(k, 0x66, u8), at), at, 0) != 0) {
                c->unk124 = c->unk128;
            }
            if (c->unk128 < c->unk124) {
                s32 next = Character_WaypointAhead(c, &t, p, cr19_stride(c));

                c->a.navTri = t;
                sceVu0CopyVector(c->a.pos, p);
                c->unk128 = next;
            }
            AT(k, 0x65, u8) = 1;
        }
    }
}

/* (as func_002DF760) go through exit `exit` into the next room: its room, side (+0x0) and
   door (+0x63); off the mesh, out of play; its doors cleared; then reset - stopped, speed 0.7,
   a wait of 150 / 300 / 450 (+0x42), no target. -1: no such exit */
s32 func_00325410(Character *c, s32 exit) {
    u8 *k = CR(c);
    VObject *rooms;
    u32 d;
    s32 i;

    d = VCALL(gRooms, 0x14, u32 (*)(VObject *, s32, s32))(gRooms, c->a.room, exit) & 0xFF;
    if (d == 0xFF) {
        return -1;
    }
    rooms = gRooms;
    c->a.room = VCALL(rooms, 0x18, s32 (*)(VObject *, s32, s32))(rooms, c->a.room, exit);
    AT(k, 0x0, s32) = VCALL(rooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(rooms, c->a.room, d, 0);
    AT(k, 0x63, u8) = d;
    c->a.navTri = NAV_NONE;
    AT(k, 0x61, u8) = 0;
    c->moveSub = 0x17;
    c->a.disabled = 1;
    for (i = 0; i < 8; i++) {
        AT(k, 0x50 + i * 2, s16) = 0;
    }
    cr19_reset(c);
    return 0;
}

/* (as func_002DEA80) +0x9C set up to come after Fiona: out (+0x6E, +0x6F cleared) with a rest
   time (+0x4A, 1800 .. 7200), a path to her room and its length through its doors */
void func_00324FA0(Character *c, s32 a1, s32 a2) {
    u8 *k = CR(c);
    s32 n;

    VCALL(c, 0x64, void (*)(Character *, s32, s32, s32))(c, a1, a2, 2);
    AT(k, 0x6F, u8) = 0;
    AT(k, 0x6E, u8) = 1;
    AT(k, 0x4A, s16) = ((VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) & 3) + 1) * 1800;
    if (Character_Route(c, gCharPlayer->a.room, -1, AT(k, 0x0, s32), -1) == -1) {
        return;
    }
    for (n = 0; n < c->unk1384; n++) {
        VObject *rooms = gRooms;
        u16 door = AT(c->unk138C, n * 2, u16);

        AT(&c->unk14C4, 0, f32) += (f32)VCALL(rooms, 0x38, s32 (*)(VObject *, u32, s32))(rooms, door, a1);
        c->unk14C0 = AT(c->unk138C, n * 2, u16);
    }
}

/* (as func_002E0DB0) the room entered, by its state +0x8: 2 - reset to 0; 1 - in the room
   being played, back on its triangle (or somewhere on its level) with its doors noted (+0x50).
   1 unless state 1 */
s32 func_00325B60(Character *c) {
    u8 *k = CR(c);
    Progress *p = gProgress;
    u32 tri;

    switch (AT(k, 0x8, s32)) {
    case 2:
        AT(k, 0x8, s32) = 0;
        return 1;
    case 1:
        break;
    default:
        return 1;
    }
    if (c->a.room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        return 0;
    }
    tri = c->a.navTri;
    if (NavMesh_TriFlags(gNavMesh, tri) & c->a.navMask) {
        Actor_TeleportRandom(&c->a, AT(k, 0x0, s32));
    } else {
        VCALL(gNavMesh, 0x14, void (*)(NavMesh *, u32, f32 *))(gNavMesh, tri, c->a.pos);
        VCALL(c, 0x28, s32 (*)(Character *, u32, f32 *, f32 *))(c, c->a.navTri, &c->a.angle[1], c->a.pos);
    }
    creature_at_doors(c, 0x50);
    return 0;
}

void func_00325D60(void) {
}

/* +0x2C its draw light: the first door it may use (+0x1590 by slot; none: layer 0xA). Its
   alpha grows as it stands further inside from the door's event plane (the plane's normal
   along the door's facing, the further of its two sides): 0 at the plane, 0x80 half the
   plane's width in; layer 0xF */
void func_003247D0(Character *c) {
    VObject *rooms, *ev;
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    f32 face[4] __attribute__((aligned(16)));
    f32 p0[4] __attribute__((aligned(16)));
    f32 p1[4] __attribute__((aligned(16)));
    f32 p2[4] __attribute__((aligned(16)));
    f32 e[4] __attribute__((aligned(16)));
    f32 n[4] __attribute__((aligned(16)));
    f32 mid[4] __attribute__((aligned(16)));
    u16 bit = (1 << *(u8 *)&c->a.slot) & 0xFFFF;
    u32 d;
    f32 dot, half;
    u32 col;

    for (d = 0; d < 8; d = (d + 1) & 0xFF) {
        if ((AT(c, 0x1590 + (d & 0xFF) * 2, u16) & bit) != 0) {
            break;
        }
    }
    if ((d & 0xFF) == 8) {
        c->unk152C = 0xA;
        return;
    }
    rooms = gRooms;
    VCALL(rooms, 0x2C, void (*)(VObject *, u32, f32 *))(rooms, d, a);
    VCALL(rooms, 0x30, u32 (*)(VObject *, u32, f32 *))(rooms, d, b);
    sceVu0SubVector(face, b, a);
    *(s32 *)&face[1] = 0;
    sceVu0Normalize(face, face);
    ev = gEvents;
    VCALL(ev, 0x24, void (*)(VObject *, u32, f32 *, f32 *, f32 *))(ev, d, p0, p1, p2);
    sceVu0SubVector(e, p1, p0);
    *(s32 *)&e[1] = 0;
    sceVu0Normalize(n, e);
    dot = sceVu0InnerProduct(face, n);
    if ((dot <= 0.0f ? -dot : dot) < 0x1.6a09e6p-1f /* 0.7071 */) {
        sceVu0SubVector(e, p1, p2);
        *(s32 *)&e[1] = 0;
        sceVu0Normalize(n, e);
        dot = sceVu0InnerProduct(face, n);
    }
    if (dot < 0.0f) {
        sceVu0ScaleVector(n, n, -1.0f);
    }
    half = 0.5f * __builtin_sqrtf(e[2] * e[2] + e[0] * e[0]);
    VCALL(ev, 0x20, void (*)(VObject *, u32, f32 *))(ev, d, mid);
    sceVu0SubVector(e, mid, c->a.pos);
    dot = sceVu0InnerProduct(e, n);
    c->unk152C = 0xF;
    if (dot <= 0.0f) {
        col = 0x808080;
    } else {
        col = (u32)(128.0f * (dot / half));
        if (col > 0x80) {
            col = 0x80;
        }
        col = col << 24 | 0x808080;
    }
    VCALL(gRenderer, 0x70, void (*)(VObject *, u32))(gRenderer, col);
}

/* from save slot `slot` (gProgress +0x878, 0x24 each) when in use (+0x12): its timers,
   strength x10, out and its flags, kind, state, heading (+0x1C), +0x28; a rest time when out */
void func_00324AE0(Character *c, s32 slot) {
    u8 *s = (u8 *)gProgress + slot * 0x24 + 0x878;
    u8 *k = CR(c);
    f32 h;

    if (AT(s, 0x12, u8) == 0) {
        return;
    }
    AT(k, 0x44, s16) = AT(s, 0xC, s16);
    AT(k, 0x48, s16) = AT(s, 0xF, u8) * 10;
    AT(k, 0x6E, u8) = AT(s, 0x10, u8);
    AT(k, 0x6F, u8) = AT(s, 0x11, u8);
    AT(k, 0x70, u8) = AT(s, 0x13, u8);
    AT(k, 0x62, u8) = AT(s, 0x14, u8);
    AT(k, 0x69, u8) = AT(k, 0x62, u8);
    AT(c, 0x14C8, s32) = AT(s, 0x18, s32);
    AT(k, 0x28, f32) = AT(s, 0x20, f32);
    h = AT(s, 0x1C, f32);
    c->a.angle[1] = h;
    sceVu0UnitMatrix(c->a.rot);
    sceVu0RotMatrixY(c->a.rot, c->a.rot, h);
    if (AT(k, 0x6E, u8) != 0) {
        AT(k, 0x4A, s16) = ((VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) & 3) + 1) * 1800;
    }
}

/* Saves this enemy's state into gProgress slot `slot` (36-byte records at +0x878). */
void func_00324C00(u8 *self, s32 slot, u32 b12) {
    u8 *rec = (u8 *)gProgress + 0x878 + slot * 36;

    U32(rec, 0x0) = U32(self, 0x30);
    U32(rec, 0x4) = U32(self, 0x1540);
    U32(rec, 0x8) = U32(self, 0x34);
    S16(rec, 0xC) = S16(self, 0x1584);
    rec[0xE] = self[0x15AD];
    rec[0xF] = (u8)(S16(self, 0x1588) / 10);
    rec[0x10] = self[0x15AE];
    rec[0x11] = self[0x15AF];
    rec[0x12] = self[0x15A9] == 4 ? 0 : (u8)b12;
    rec[0x13] = self[0x15B0];
    rec[0x14] = self[0x15A2];
    U32(rec, 0x18) = U32(self, 0x14C8);
    F32(rec, 0x1C) = F32(self, 0x54);
    F32(rec, 0x20) = F32(self, 0x1568);
}

extern const u8 D_0042C460[];   /* per kind (^ 0x80): s32, s16 */

/* +0xA0 set up: mode `mode` (+0x0), kind `kind` (+0x6D) with its table entry, heading `deg`,
   strength x10 (+0x48); from save slot `slot` when given (a reset if its state was 2). Not out:
   +0x28 = `f` without a slot, state 0xB when +0x28 > 0; kinds 8 / 9 (^ 0x80) block nothing and
   plan as kind 6. Then the Character's reset (+0x64) */
void func_00324CD0(Character *c, s32 a1, s32 a2, s32 mode, u8 kind, s32 str, s32 slot, u32 deg, f32 f) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    u8 *k = CR(c);
    const u8 *e;
    f32 h;
    u32 kk;

    AT(k, 0x0, s32) = mode;
    AT(k, 0x6E, u8) = 0;
    AT(k, 0x4A, s16) = 0;
    AT(c, 0x14C4, s32) = 0;
    AT(k, 0x6F, u8) = 0;
    AT(k, 0x6D, u8) = kind;
    e = D_0042C460 + (AT(k, 0x6D, u8) ^ 0x80) * 8;
    AT(c, 0x14C8, s32) = AT(e, 0x0, s32);
    AT(k, 0x46, s16) = AT(e, 0x4, s16);
    h = func_002E2D00(kPi.f * (f32)deg / 180.0f);
    c->a.angle[1] = h;
    sceVu0UnitMatrix(c->a.rot);
    sceVu0RotMatrixY(c->a.rot, c->a.rot, h);
    AT(k, 0x48, s16) = (s16)str * 10;
    if (slot != -1) {
        func_00324AE0(c, slot);
        if (AT(k, 0x69, u8) == 2) {
            cr19_reset(c);
        }
    }
    if (AT(k, 0x6F, u8) == 0) {
        if (slot == -1) {
            AT(k, 0x28, f32) = f;
        }
        if (!(AT(k, 0x28, f32) <= 0.0f)) {
            AT(k, 0x69, u8) = 0xB;
            AT(k, 0x62, u8) = 0xB;
        }
        kk = AT(k, 0x6D, u8) ^ 0x80;
        if (kk == 9 || kk == 8) {
            c->a.navMask = 0;
            c->pathReq->unk4 = 6;
            c->pathReq->mask = 0x40080;
        }
    }
    VCALL(c, 0x64, void (*)(Character *, s32, s32, s32))(c, a1, a2, mode);
}

/* (as func_002DFA50) travelling (unless in an event, flag 0x18): off screen the distance to
   the next door (+0x14C4) runs down by 0.35; at a door (+0x61) it walks the path at 0.35. On
   arriving at the door (+0x14C0): if the way is open (Progress_ExitOpen, Progress_ExitPassable kind 2)
   and the door isn't shut, through it - into the room being played at the exit's spot facing
   in, its doors noted (+0x50) - else the next leg's distance; a closed way is marked (+0x148C
   bit), the trip ends (+0x68), and without the kind-2 way its state +0x8 is 2 */
void func_003257B0(Character *c) {
    u8 *k = CR(c);
    Progress *p = gProgress;
    VObject *rooms;
    u8 arrived = 0;
    u32 exit;

    if (Progress_TestFlag(p, 0x18) == 0) {
        if (AT(k, 0x61, u8) != 1) {
            f32 *left = &AT(&c->unk14C4, 0, f32);

            *left = *left - 0x1.666666p-2f;   /* 0.35 */
            if (*left < 0.0f) {
                *left = 0.0f;
                arrived = 1;
            }
        } else {
            Character_FollowWaypoints(c, 0x1.666666p-2f);
            if (!(c->unk128 < c->unk124)) {
                arrived = 1;
            }
        }
    }
    if (arrived != 1) {
        return;
    }
    rooms = gRooms;
    exit = VCALL(rooms, 0x3C, u32 (*)(VObject *, u32, s32))(rooms, c->unk14C0, c->a.room) & 0xFF;
    AT(k, 0x63, u8) = exit;
    if (exit == 0xFF) {
        AT(k, 0x61, u8) = 0;
        c->a.navTri = NAV_NONE;
        return;
    }
    if ((Progress_CurRoomFlag(p, c->a.room, exit) & 0xFF) == 1) {
        return;
    }
    if (!(Progress_ExitOpen(p, c->a.room, exit) & 0xFF) || !(Progress_ExitPassable(p, c->a.room, exit, 2) & 0xFF)) {
        c->unk148C[c->unk14C0 >> 5] |= 1 << (c->unk14C0 & 0x1F);
        AT(k, 0x61, u8) = 0;
        c->a.navTri = NAV_NONE;
        c->unk1388 = c->unk1384;
        AT(k, 0x68, u8) = 1;
        if (!(Progress_ExitPassable(p, c->a.room, exit, 2) & 0xFF)) {
            AT(k, 0x8, s32) = 2;
        }
        return;
    }
    AT(k, 0x61, u8) = 0;
    func_00325410(c, exit);
    if (c->a.room == VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        f32 at[4] __attribute__((aligned(16)));
        f32 in[4] __attribute__((aligned(16)));
        f32 d[4] __attribute__((aligned(16)));
        f32 yaw;
        u32 tri;

        tri = VCALL(gRooms, 0x30, u32 (*)(VObject *, u32, f32 *))(gRooms, AT(k, 0x63, u8), at);
        VCALL(gRooms, 0x34, u32 (*)(VObject *, u32, f32 *))(gRooms, AT(k, 0x63, u8), in);
        sceVu0SubVector(d, in, at);
        yaw = func_0031C5C0(d[0], d[2]);
        VCALL(c, 0x28, void (*)(Character *, u32, f32 *, f32 *))(c, tri, &yaw, at);
        creature_at_doors(c, 0x50);
    } else {
        c->a.navTri = NAV_NONE;
        AT(&c->unk14C4, 0, f32) = (f32)VCALL(rooms, 0x38, s32 (*)(VObject *, u32, s32))(rooms, c->unk14C0, c->a.room);
        c->unk1388 = c->unk1384;
    }
}

/* +0x84 its action (+0x14E8): action 4 with sub 4 / 2 / 1 (unless already in move 4) settles it
   in state 2 (+0x69 / +0x62), move 4; the action is then cleared */
void func_00325D70(Character *c) {
    u8 *k = CR(c);

    if (AT(c, 0xF8, s32) != 4 && AT(c, 0x14E8, s32) == 4) {
        s32 a = AT(c, 0x14EC, s32);

        if (a == 4 || a == 2 || a == 1) {
            c->unk124 = c->unk128;
            Character_ReleasePath(c);
            AT(k, 0x69, u8) = 2;
            AT(k, 0x62, u8) = 2;
            AT(c, 0xF8, s32) = 4;
            Character_ReleasePath(c);
            cr19_settle(c);
        }
    }
    if (AT(c, 0x14E8, s32) != 0) {
        AT(c, 0x14E8, s32) = 0;
    }
}

extern const f32 D_0042C650[17];   /* turns */
void func_0032B080(Character *c, f32 *pos);

/* a target (+0x30) near Fiona: out to her side (alternately right / left, `dist` + 0..15 away,
   turned by a random table turn after the first), until one is on the mesh off its own
   triangle (17 tries); its triangle (+0x20) */
u32 func_00325EC0(Character *c, f32 dist) {
    u8 *k = CR(c);
    VObject *rnd = gRandom;
    f32 tbl[17];
    f32 m[4][4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 off[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 yaw = 0.0f;
    s32 i, s;

    for (i = 0; i < 17; i++) {
        tbl[i] = D_0042C650[i];
    }
    for (i = 0; i < 17; i++) {
        off[3] = 0.0f;
        off[2] = 0.0f;
        off[1] = 0.0f;
        off[0] = 0.0f;
        sceVu0CopyVector(d, off);
        sceVu0CopyVector((f32 *)(k + 0x30), off);
        s = i % 2 == 0 ? 1 : -1;
        sceVu0UnitMatrix(m);
        v[3] = 0.0f;
        v[1] = 0.0f;
        v[0] = 0.0f;
        v[2] = (f32)s * (dist + (f32)(VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 0xF));
        sceVu0ApplyMatrix(off, m, v);
        sceVu0AddVector(off, gCharPlayer->a.pos, off);
        sceVu0RotMatrixY(m, m, yaw);
        sceVu0ApplyMatrix(d, m, v);
        sceVu0AddVector((f32 *)(k + 0x30), gCharPlayer->a.pos, d);
        func_0032B080(c, (f32 *)(k + 0x30));
        yaw = tbl[VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 0xF];
        AT(k, 0x20, u32) = Actor_TriOf(&gCharPlayer->a, (f32 *)(k + 0x30));
        if (Actor_TriTo(&c->a, (f32 *)(k + 0x30), c->pathReq->mask) != NAV_NONE && c->a.navTri != AT(k, 0x20, u32)) {
            break;
        }
    }
    return AT(k, 0x20, u32);
}

/* how far c must still turn to face heading `h` (both ways the same) */
#define CR19_TURN_LEFT(c, h) \
    (func_002E2D00((h) - (c)->a.angle[1]) <= 0.0f ? -func_002E2D00((h) - (c)->a.angle[1]) : func_002E2D00((h) - (c)->a.angle[1]))

/* its approach, by step +0x6C: 0 stop (`dist` kept, +0x1C); 1 a target near Fiona (once the
   path is done; none: try 1 further); 2 turn to it (within 30 degrees); 3 walk the path there
   (a fresh path when the last is done; none: back to 1) - when Fiona is caught (+0x1AD630)
   and it touches the first character, state 3 unless held (+0x6B bit 0x80) or she is in move
   8; 4 turn to Fiona, and at the end of its animation settle in state 0 or 1 (random), move 2 */
void func_00326130(Character *c, f32 dist) {
    u8 *k = CR(c);
    f32 h;

    switch (AT(k, 0x6C, u8)) {
    case 0:
        AT(k, 0x6C, u8)++;
        Character_ReleasePath(c);
        AT(k, 0x64, u8) = 0;
        c->unk124 = c->unk128;
        AT(k, 0x1C, f32) = dist;
        return;
    case 1:
        AT(k, 0x6C, u8)++;
        if (c->unk128 < c->unk124) {
            return;
        }
        if (func_00325EC0(c, AT(k, 0x1C, f32)) != NAV_NONE) {
            return;
        }
        AT(k, 0x6C, u8) = 1;
        AT(k, 0x1C, f32) = AT(k, 0x1C, f32) + 1.0f;
        return;
    case 2:
        h = Actor_HeadingTo(&c->a, (f32 *)(k + 0x30));
        if (CR19_TURN_LEFT(c, h) < 0x1.0c1524p-1f /* 30 degrees */) {
            AT(k, 0x6C, u8)++;
        } else {
            func_0032B080(c, (f32 *)(k + 0x30));
        }
        return;
    case 3:
        if (AT(k, 0x20, s32) == -1) {
            AT(k, 0x6C, u8) = 1;
            return;
        }
        if (AT(gCharPlayer, 0x1AD630, u8) == 1) {
            Character *o = gCharacters[0];
            f32 d[4] __attribute__((aligned(16)));
            u8 hit = 0;

            sceVu0SubVector(d, o->a.pos, c->a.pos);
            if (__builtin_sqrtf(d[2] * d[2] + d[0] * d[0]) < c->a.radius + o->a.radius) {
                Character_ReleasePath(c);
                AT(k, 0x64, u8) = 0;
                hit = 1;
                c->unk124 = c->unk128;
            }
            if (hit == 1 && AT(k, 0x69, u8) != 3 && !(AT(k, 0x6B, u8) & 0x80) && AT(gCharPlayer, 0xF8, s32) != 8) {
                AT(k, 0x69, u8) = 3;
                AT(c, 0xF8, s32) = 0;
                Character_ReleasePath(c);
                cr19_settle(c);
                return;
            }
        }
        if (!(c->unk128 < c->unk124)) {
            if (func_003255C0(c, AT(k, 0x20, u32), (f32 *)(k + 0x30), 0) != 0) {
                AT(k, 0x6C, u8) = 1;
                return;
            }
        }
        if (Character_FollowWaypointsBlocked(c, cr19_stride(c)) == 0) {
            AT(k, 0x6C, u8)++;
        }
        return;
    case 4:
        h = Actor_HeadingTo(&c->a, gCharPlayer->a.pos);
        if (!(CR19_TURN_LEFT(c, h) < 0x1.0c1524p-1f)) {
            func_0032B080(c, gCharPlayer->a.pos);
            return;
        }
        if ((AT(AT(c->motion, 0x6A4, u8 *), 0x18, u32) & 0x20) != 0) {
            s8 r = VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) & 1;

            if (r == 0) {
                AT(k, 0x69, u8) = 1;
            } else if (r == 1) {
                AT(k, 0x69, u8) = 0;
            }
            AT(c, 0xF8, s32) = 2;
            Character_ReleasePath(c);
            cr19_settle(c);
        }
        return;
    }
}

/* moved this frame by its animation's root motion (turned with it) */
static inline void cr19_root_move(Character *c) {
    f32 v[4] __attribute__((aligned(16)));

    func_001F6370(c->motion, v, 0.0f);
    *(s32 *)&v[1] = 0;
    sceVu0ApplyMatrix(v, c->a.rot, v);
    Actor_Move(&c->a, v);
}

/* state: held off - it moves by its animation, facing Fiona; when she is no longer across the
   room's divider from it, back to state 0. Otherwise it alternates idles 0 and 0x1C00 (+0x6C)
   every 90 frames (+0x40, +0x60 started) */
void func_00326680(Character *c) {
    u8 *k = CR(c);

    cr19_root_move(c);
    func_0032B080(c, gCharPlayer->a.pos);
    AT(k, 0x40, s16)++;
    if (!NavMesh_AcrossDivider(gNavMesh, gCharPlayer->a.navTri, c->a.navTri)) {
        cr19_reset(c);
        return;
    }
    switch (AT(k, 0x6C, u8)) {
    case 0:
        if (AT(k, 0x60, u8) == 0) {
            func_002DDED0(c->motion, 0, -1);
            AT(k, 0x60, u8)++;
        }
        if ((u32)AT(k, 0x40, s16) % 90 == 0) {
            AT(k, 0x40, s16) = 0;
            AT(k, 0x60, u8) = 0;
            AT(k, 0x6C, u8)++;
        }
        break;
    case 1:
        if (AT(k, 0x60, u8) == 0) {
            func_002DDED0(c->motion, 0x1C00, -1);
            AT(k, 0x60, u8)++;
        }
        if ((u32)AT(k, 0x40, s16) % 90 == 0) {
            AT(k, 0x40, s16) = 0;
            AT(k, 0x60, u8) = 0;
            AT(k, 0x6C, u8) = 0;
        }
        break;
    }
}

/* a door of the room it may use (+0x1590 by slot) whose way isn't shut for kind 2 (state bit
   4): state 5 */
void func_00326950(Character *c) {
    VObject *doors = gDoors;
    Progress *p = gProgress;
    u32 d;

    for (d = 0; d < 8; d = (d + 1) & 0xFF) {
        if ((VCALL(doors, 0x40, u32 (*)(VObject *, u32))(doors, d) & 0xFF) != 1) {
            continue;
        }
        if (!((AT(c, 0x1590 + (d & 0xFF) * 2, u16) & ((1 << *(u8 *)&c->a.slot) & 0xFFFF)) != 0)) {
            continue;
        }
        if (PursuerGroup_Fields(p, d, 2) & 0xFF & 4) {
            continue;
        }
        cr19_enter(c, 5);
        return;
    }
}

/* state: Fiona in sight (Actor_CanWalkBetween) - back to state 0; across the room's divider from her
   - state 0x12; else its approach at 10 */
void func_00326AF0(Character *c) {
    if ((Actor_CanWalkBetween(c, c->a.navTri, gCharPlayer->a.navTri, c->a.pos, gCharPlayer->a.pos, 0) & 0xFF) == 1) {
        cr19_reset(c);
        return;
    }
    if (NavMesh_AcrossDivider(gNavMesh, gCharPlayer->a.navTri, c->a.navTri)) {
        cr19_enter(c, 0x12);
        return;
    }
    func_00326130(c, 10.0f);
}

/* by the progress: flag 9 - state 9 in move 2; flag 10, or Fiona free (+0x2D, +0xE0 clear) -
   state 5 */
void func_00326DA0(Character *c) {
    u8 *k = CR(c);
    Progress *p = gProgress;

    if (Progress_TestFlag(p, 9) != 0) {
        AT(k, 0x69, u8) = 9;
        AT(c, 0xF8, s32) = 2;
        Character_ReleasePath(c);
        cr19_settle(c);
        return;
    }
    if ((Progress_TestFlag(p, 0xA) & 0xFF) == 1) {
        cr19_enter(c, 5);
        return;
    }
    if (AT(gCharPlayer, 0x2D, u8) != 0 || AT(gCharPlayer, 0xE0, u8) != 0) {
        return;
    }
    cr19_enter(c, 5);
}

/* turn to Fiona; once within 20 degrees, state 5 */
static inline void cr19_face_fiona(Character *c) {
    f32 h = Actor_HeadingTo(&c->a, gCharPlayer->a.pos);

    if (CR19_TURN_LEFT(c, h) < 0x1.6571860p-2f /* 20 degrees */) {
        cr19_enter(c, 5);
    } else {
        func_0032B080(c, gCharPlayer->a.pos);
    }
}

/* by step +0x6C: 0 walk (out of contact, +0x2D) to its spot - triangle 0xE in room 0x8F, in
   room 0x91 0xD0 / 0x9F for kinds 8 / 9 (^ 0x80) - and on arriving block 0x4020028 again;
   1 turn to Fiona (state 5) */
void func_00327030(Character *c) {
    u8 *k = CR(c);
    f32 at[4] __attribute__((aligned(16)));
    u32 tri = 0;   /* (the original's register was left as it came in other rooms) */
    u32 kk;

    switch (AT(k, 0x6C, u8)) {
    case 0:
        c->a.unk2D = 1;
        if (c->a.room == 0x91) {
            kk = AT(k, 0x6D, u8) ^ 0x80;
            if (kk == 8) {
                tri = 0xD0;
            } else if (kk == 9) {
                tri = 0x9F;
            }
        } else if (c->a.room == 0x8F) {
            tri = 0xE;
        }
        VCALL(gNavMesh, 0xC, void (*)(NavMesh *, u32, f32 *))(gNavMesh, tri, at);
        if (func_003255C0(c, tri, at, 0) != 0) {
            return;
        }
        if (Character_FollowWaypointsBlocked(c, cr19_stride(c)) != 0) {
            return;
        }
        AT(k, 0x6C, u8)++;
        c->a.unk2D = 0;
        c->a.navMask = 0x4020028;
        c->pathReq->mask = c->a.navMask;
        return;
    case 1:
        cr19_face_fiona(c);
        return;
    }
}

/* state: turn to Fiona (then state 5) */
void func_003272E0(Character *c) {
    cr19_face_fiona(c);
}

/* state: with Fiona caught (+0x1AD630) its approach at 20, else state 5 */
void func_00327450(Character *c) {
    if (AT(gCharPlayer, 0x1AD630, u8) == 0) {
        cr19_enter(c, 5);
        return;
    }
    func_00326130(c, 20.0f);
}

/* state: knocked down, by step +0x60: 0 animation 0x1001 while its fall +0x28 runs down
   (+0x2C, 0.085 faster each frame), then a thud (sound 5); 1 after its animation and 32
   frames; 2 animation 0x1800; 3 getting up (a sound at frame 34, moved by its animation), at
   its end back in contact in state 5 */
void func_00327540(Character *c) {
    u8 *k = CR(c);

    switch (AT(k, 0x60, u8)) {
    case 0:
        func_002DDED0(c->motion, 0x1001, -1);
        AT(k, 0x28, f32) = AT(k, 0x28, f32) - AT(k, 0x2C, f32);
        AT(k, 0x2C, f32) = AT(k, 0x2C, f32) + 0x1.5c28f6p-4f;   /* 0.085 */
        if (AT(k, 0x28, f32) <= 0.0f) {
            Actor_PlaySound(&c->a, 5, 5, 0, 0, NULL);
            AT(k, 0x28, s32) = 0;
            AT(k, 0x2C, s32) = 0;
            AT(k, 0x60, u8)++;
        }
        break;
    case 1:
        if ((AT(AT(c->motion, 0x6A4, u8 *), 0x18, u32) & 0x20) != 0) {
            if (++AT(k, 0x40, s16) >= 32) {
                AT(k, 0x60, u8)++;
            }
        }
        break;
    case 2:
        AT(k, 0x40, s16) = 0;
        func_002DDED0(c->motion, 0x1800, -1);
        AT(k, 0x60, u8)++;
        break;
    case 3:
        if (++AT(k, 0x40, s16) == 0x22) {
            Actor_PlaySound(&c->a, 5, 5, 0, 0, NULL);
        }
        cr19_root_move(c);
        if ((AT(AT(c->motion, 0x6A4, u8 *), 0x18, u32) & 0x20) != 0) {
            c->a.unk2D = 0;
            cr19_enter(c, 5);
        }
        break;
    }
}

/* state: Fiona on a triangle it may not stand on - its approach at 5; else back to state 0 */
void func_003277C0(Character *c) {
    if (NavMesh_TriFlags(gNavMesh, gCharPlayer->a.navTri) & 0x4020028) {
        func_00326130(c, 5.0f);
        return;
    }
    cr19_reset(c);
}

/* state: with progress flag 9 its approach at 20, else back to state 0 */
void func_003278F0(Character *c) {
    if (Progress_TestFlag(gProgress, 9) != 0) {
        func_00326130(c, 20.0f);
        return;
    }
    cr19_reset(c);
}

/* (as func_002DFF70) when it can walk straight at `goal` on Fiona's triangle, it drops its
   path and does (by its root motion) unless within 2 (+0x24); on another level it gives up
   (+0x6A) after 30 tries close by (+0xC) unless on her triangle. Her triangle, -1 if not */
s32 func_003279F0(Character *c, f32 *goal) {
    u8 *k = CR(c);
    u32 t = Actor_TriTo(&c->a, goal, c->pathReq->mask);

    if (t != gCharPlayer->a.navTri) {
        return -1;
    }
    Character_ReleasePath(c);
    AT(k, 0x64, u8) = 0;
    c->unk124 = c->unk128;
    if (goal[1] != c->a.pos[1]) {
        if (AT(k, 0x24, f32) < 2.0f) {
            AT(k, 0xC, u32)++;
        }
        if (AT(k, 0xC, u32) >= 30 && c->a.navTri != gCharPlayer->a.navTri) {
            AT(k, 0xC, u32) = 0;
            AT(k, 0x6A, u8) = 1;
            return -1;
        }
    }
    if (!(AT(k, 0x24, f32) <= 2.0f)) {
        f32 d[4] __attribute__((aligned(16)));
        f32 v[4] __attribute__((aligned(16)));

        sceVu0SubVector(d, goal, c->a.pos);
        *(s32 *)&d[1] = 0;
        sceVu0Normalize(d, d);
        func_001F6370(c->motion, v, 0.0f);
        *(s32 *)&v[1] = 0;
        func_0010E640(d, d, v[2]);
        Actor_Move(&c->a, d);
    }
    return t;
}

/* state: leaving - moved by its animation; out of contact it is gone at once; at frame 65 a
   sound (0xA), at its animation's end gone; its fade +0x71 runs down by 2 (to 0 under 10) */
void func_00327B70(Character *c) {
    u8 *k = CR(c);
    u8 f;

    cr19_root_move(c);
    if (c->a.disabled != 0) {
        c->a.active = 0;
        Character_ReleasePath(c);
        return;
    }
    if (++AT(k, 0x40, s16) == 0x41 && AT(k, 0x6C, u8) == 0) {
        AT(k, 0x6C, u8)++;
        Actor_PlaySound(&c->a, 0xA, 5, 0, 0, NULL);
    }
    if ((AT(AT(c->motion, 0x6A4, u8 *), 0x18, u32) & 0x20) != 0) {
        c->a.active = 0;
        Character_ReleasePath(c);
    }
    f = AT(k, 0x71, u8);
    if (f < 10) {
        AT(k, 0x71, u8) = 0;
    } else {
        AT(k, 0x71, u8) = f - 2;
    }
}

/* state: with progress flag 9, or Fiona in move 4 / 3, its approach at 10; else its flags
   cleared and state 5 */
void func_00327CA0(Character *c) {
    u8 *k = CR(c);

    if (Progress_TestFlag(gProgress, 9) != 0 || AT(gCharPlayer, 0xF8, s32) == 4 || AT(gCharPlayer, 0xF8, s32) == 3) {
        func_00326130(c, 10.0f);
        return;
    }
    AT(k, 0x6B, u8) = 0;
    cr19_enter(c, 5);
}

extern const f32 D_0042C610[4][4];   /* where it grabs from, around Fiona */

/* state: its grab of Fiona, by step +0x60. 0: while she can be grabbed (+0x68 0xC), a spot
   around her - by +0x6C in turn: her left / right side by which way it is from her (within 90
   degrees of her facing), the other, behind, in front (D_0042C610) - planned to when on the
   mesh and not on a blocked triangle (0x4020038); it walks (move 8; a noise when it can't),
   and with her held (move 4, sub 0x12) turns to her (a sound, animation 0x1900). 1: at its
   end sounds 9 and 4, animation 0x1901. 2: sounds every 35 / 70 frames; released - animation
   0x1902, the threat up 75, a noise. 3: moved by its animation; at its end out of contact,
   held (+0x6B bit 0x80) in state 4, move 4 */
void func_00327DD0(Character *c) {
    u8 *k = CR(c);
    f32 tbl[4][4] __attribute__((aligned(16)));
    f32 off[4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    s32 i;

    for (i = 0; i < 16; i++) {
        ((u32 *)tbl)[i] = ((const u32 *)D_0042C610)[i];
    }
    switch (AT(k, 0x60, u8)) {
    case 0: {
        f32 h, diff, dx, dz;
        u8 front;
        u32 tri;

        if ((VCALL(gCharPlayer, 0x68, u32 (*)(Character *, s32, s32, s32))(gCharPlayer, 0xC, 0xFF, 0) & 0xFF) != 1) {
            return;
        }
        if (!(c->unk128 < c->unk124)) {
            h = gCharPlayer->a.angle[1];
            dx = c->a.pos[0] - gCharPlayer->a.pos[0];
            dz = c->a.pos[2] - gCharPlayer->a.pos[2];
            if (0.0f == dx && 0.0f == dz) {
                diff = c->a.angle[1] - h;
            } else {
                diff = func_0031C5C0(dx, dz) - h;
            }
            front = (func_002E2D00(diff) <= 0.0f ? -func_002E2D00(diff) : func_002E2D00(diff)) < 0x1.921fb6p+0f;   /* 90 degrees */
            switch (AT(k, 0x6C, u8)) {
            case 0:
                sceVu0CopyVector(off, front ? tbl[1] : tbl[0]);
                AT(k, 0x6C, u8)++;
                break;
            case 1:
                sceVu0CopyVector(off, front ? tbl[0] : tbl[1]);
                AT(k, 0x6C, u8)++;
                break;
            case 2:
                sceVu0CopyVector(off, tbl[2]);
                AT(k, 0x6C, u8)++;
                break;
            case 3:
                sceVu0CopyVector(off, tbl[3]);
                AT(k, 0x6C, u8) = 0;
                break;
            }
            Mtx_AtHeading(m, gCharPlayer->a.pos, h);
            func_002E2DD0(at, m, off);
            tri = Actor_TriOf(&gCharPlayer->a, at);
            if (tri == NAV_NONE) {
                return;
            }
            if (NavMesh_TriFlags(gNavMesh, tri) & 0x4020038) {
                return;
            }
            if (func_003255C0(c, tri, at, 0) != 0) {
                return;
            }
        }
        AT(c, 0xF8, s32) = 8;
        if (Character_FollowWaypointsBlocked(c, cr19_stride(c)) != 0) {
            Progress_Noise(gProgress, c->a.pos, 1, 0xC, 0, 0, 0.0f);
            return;
        }
        if (AT(gCharPlayer, 0xF8, s32) != 4 || AT(gCharPlayer, 0xFC, s32) != 0x12) {
            return;
        }
        AT(c, 0x10C, f32) = Actor_HeadingTo(&c->a, gCharPlayer->a.pos);
        Actor_TurnToward(&c->a, AT(c, 0x10C, f32), 0.0f);
        h = func_002E2D00(AT(c, 0x10C, f32));
        c->a.angle[1] = h;
        sceVu0UnitMatrix(c->a.rot);
        sceVu0RotMatrixY(c->a.rot, c->a.rot, h);
        Actor_PlaySound(&c->a, 8, 5, 0, 0, NULL);
        func_002DDED0(c->motion, 0x1900, -1);
        AT(k, 0x60, u8)++;
        return;
    }
    case 1:
        if ((AT(AT(c->motion, 0x6A4, u8 *), 0x18, u32) & 0x20) == 0) {
            return;
        }
        Actor_PlaySound(&c->a, 9, 5, 0, 0, NULL);
        Actor_PlaySound(&c->a, 4, 5, 0, 0, NULL);
        func_002DDED0(c->motion, 0x1901, -1);
        AT(k, 0x60, u8)++;
        return;
    case 2:
        if (++AT(k, 0x40, s16) % 35 == 0) {
            Actor_PlaySound(&c->a, 9, 5, 0, 0, NULL);
        }
        if (AT(k, 0x40, s16) % 70 == 0) {
            Actor_PlaySound(&c->a, 4, 5, 0, 0, NULL);
        }
        if (AT(gCharPlayer, 0xF8, s32) == 4 && AT(gCharPlayer, 0xFC, s32) == 0x12) {
            return;
        }
        AT(c, 0xF8, s32) = 4;
        func_002DDED0(c->motion, 0x1902, -1);
        Threat_Raise((u8 *)gProgress + 0x7B8, 75.0f);
        func_002A8440((u8 *)gProgress + 0x7A8, 0x80, c->a.room, c->a.navTri, 0xFFFF);
        AT(k, 0x60, u8)++;
        return;
    case 3:
        cr19_root_move(c);
        if ((AT(AT(c->motion, 0x6A4, u8 *), 0x18, u32) & 0x20) == 0) {
            return;
        }
        c->a.unk2D = 1;
        AT(k, 0x69, u8) = 4;
        AT(k, 0x6B, u8) |= 0x80;
        AT(c, 0xF8, s32) = 4;
        Character_ReleasePath(c);
        cr19_settle(c);
        return;
    }
}

/* state: hit, by step +0x60 (moved by its animation except at 1). 0: its reaction (+0x94 with
   the hit's +0x14F4), facing where the hit came from (+0x14F0: 0 Fiona, 1 Hewie, 0xFF the
   heading +0x14FC) - animation 0x1001 from the front (within 90 degrees), 0x1000 from behind -
   and sound 3. 1: out of contact; at its end, with no strength left (+0x14C8) held in state 4,
   else 30 frames. 2: animation 0x1800. 3: getting up (a sound at frame 34), at its end back in
   contact in state 0xD */
void func_003284F0(Character *c) {
    u8 *k = CR(c);
    f32 h = 0.0f;   /* (other hit sources leave the original's register as it was) */

    if (AT(k, 0x60, u8) != 1) {
        cr19_root_move(c);
    }
    switch (AT(k, 0x60, u8)) {
    case 0:
        VCALL(c, 0x94, void (*)(Character *, s32))(c, AT(c, 0x14F4, s32));
        switch (AT(c, 0x14F0, s32)) {
        case 0xFF:
            h = AT(c, 0x14FC, f32);
            break;
        case 1:
            h = Actor_HeadingTo(&c->a, gCharPartner->a.pos);
            break;
        case 0:
            h = Actor_HeadingTo(&c->a, gCharPlayer->a.pos);
            break;
        }
        if (CR19_TURN_LEFT(c, h) < 0x1.921fb6p+0f) {
            func_002DDED0(c->motion, 0x1001, -1);
        } else {
            func_002DDED0(c->motion, 0x1000, -1);
        }
        Actor_PlaySound(&c->a, 3, 5, 0, 0, NULL);
        AT(k, 0x60, u8)++;
        return;
    case 1: {
        f32 z[4] __attribute__((aligned(16)));

        c->a.unk2D = 1;
        if ((AT(AT(c->motion, 0x6A4, u8 *), 0x18, u32) & 0x20) == 0) {
            cr19_root_move(c);
            return;
        }
        z[2] = 0.0f;
        z[1] = 0.0f;
        z[0] = 0.0f;
        Actor_Move(&c->a, z);
        if (AT(c, 0x14C8, s32) <= 0) {
            AT(k, 0x69, u8) = 4;
            AT(k, 0x6B, u8) |= 0x80;
            AT(c, 0xF8, s32) = 4;
            Character_ReleasePath(c);
            cr19_settle(c);
            return;
        }
        if ((u32)AT(k, 0x40, s16)++ < 30) {
            return;
        }
        AT(k, 0x60, u8)++;
        return;
    }
    case 2:
        AT(k, 0x40, s16) = 0;
        func_002DDED0(c->motion, 0x1800, -1);
        AT(k, 0x60, u8)++;
        return;
    case 3:
        if (++AT(k, 0x40, s16) == 0x22) {
            Actor_PlaySound(&c->a, 5, 5, 0, 0, NULL);
        }
        if ((AT(AT(c->motion, 0x6A4, u8 *), 0x18, u32) & 0x20) == 0) {
            return;
        }
        c->a.unk2D = 0;
        cr19_enter(c, 0xD);
        return;
    }
}

/* watching Fiona (moved by its animation, facing her; sound 2 every 44 frames when `sound`),
   deciding every 150 frames (+0x40): with progress flag 9 state 9, with her caught state 0xC,
   out of her sight state 0x10 (all move 2 but the last); in sight, every 90 frames state 5 -
   or with +0x6B bit 0x40 state 7 / `other` by a coin */
static inline void cr19_watch(Character *c, s32 sound, u8 other) {
    u8 *k = CR(c);

    cr19_root_move(c);
    func_0032B080(c, gCharPlayer->a.pos);
    if (sound && AT(k, 0x40, s16) % 44 == 0) {
        Actor_PlaySound(&c->a, 2, 5, 0, 0, NULL);
    }
    AT(k, 0x40, s16)++;
    if (Progress_TestFlag(gProgress, 9) != 0) {
        if ((u32)AT(k, 0x40, s16) % 150 == 0) {
            AT(k, 0x69, u8) = 9;
            AT(c, 0xF8, s32) = 2;
            Character_ReleasePath(c);
            cr19_settle(c);
        }
        return;
    }
    if (AT(gCharPlayer, 0x1AD630, u8) == 1) {
        if ((u32)AT(k, 0x40, s16) % 150 == 0) {
            AT(k, 0x69, u8) = 0xC;
            AT(c, 0xF8, s32) = 2;
            Character_ReleasePath(c);
            cr19_settle(c);
        }
        return;
    }
    if (!(Actor_CanWalkBetween(c, c->a.navTri, gCharPlayer->a.navTri, c->a.pos, gCharPlayer->a.pos, 0) & 0xFF)) {
        if ((u32)AT(k, 0x40, s16) % 150 == 0) {
            cr19_enter(c, 0x10);
        }
        return;
    }
    if ((u32)AT(k, 0x40, s16) % 90 != 0) {
        return;
    }
    if (!(AT(k, 0x6B, u8) & 0x40)) {
        AT(k, 0x69, u8) = 5;
    } else {
        s8 r = VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) & 1;

        if (r == 0) {
            AT(k, 0x69, u8) = 7;
        } else if (r == 1) {
            AT(k, 0x69, u8) = other;
        }
    }
    AT(c, 0xF8, s32) = 0;
    Character_ReleasePath(c);
    cr19_settle(c);
}

void func_00328960(Character *c) {
    cr19_watch(c, 1, 0);
}

void func_00328E00(Character *c) {
    cr19_watch(c, 0, 1);
}

/* +0x38 the room is entered (func_00325B60); in the room being played it is in play (off the
   mesh: somewhere on its level, state 0), else not */
void func_0032AA50(Character *c) {
    u8 *k = CR(c);
    s32 room;

    func_00325B60(c);
    room = c->a.room;
    if (room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        if (c->a.navTri == NAV_NONE) {
            Actor_TeleportRandom(&c->a, AT(k, 0x0, s32));
            AT(k, 0x69, u8) = 0;
            AT(k, 0x62, u8) = 0;
        }
        c->a.disabled = 0;
    } else {
        c->a.disabled = 1;
    }
}

/* +0x40 its model matrix (raised by +0x28 in state 0xB) unless out of contact, then the
   model's update */
void func_0032AE80(Character *c) {
    u8 *k = CR(c);

    if (c->a.disabled == 0) {
        if (AT(k, 0x62, u8) == 0xB) {
            VCALL(c->motion, 0x40, void (*)(void *, Character *, f32, f32))(c->motion, c, AT(k, 0x28, f32), 0.0f);
        } else {
            Model_BodyFrames(c->motion, c, 0.0f, 0.0f);
        }
    }
    func_001F6AF0(c->motion);
}

/* its model released (+0xD1 loaded: the model's +0x10) */
void func_0032BEB0(Character *c) {
    AT(c, 0xD0, u8) = 0;
    if (AT(c, 0xD1, u8) != 0) {
        VCALL(c->motion, 0x10, void (*)(void *))(c->motion);
        AT(c, 0xD1, u8) = 0;
    }
}

/* its model's data from the creature manager's file (+0x24: offsets at +0x4.. to its parts),
   its kind (+0x20) into +0x1528 and the model; the model set up (+0xC) */
void func_0032BF00(Character *c) {
    VObject *mgr = (VObject *)gCreatures;
    u8 *d = VCALL_AT(mgr, 0x28, 0x24, u8 *(*)(VObject *))(mgr);
    u8 *m = c->motion;

    AT(m, 0x4C0, u8 *) = AT(d, 0x4, s32) != 0 ? d + AT(d, 0x4, s32) : NULL;
    AT(m, 0x4D0, u8 *) = AT(d, 0x8, s32) != 0 ? d + AT(d, 0x8, s32) : NULL;
    AT(m, 0x4CC, u8 *) = AT(d, 0xC, s32) != 0 ? d + AT(d, 0xC, s32) : NULL;
    AT(m, 0x4C4, u8 *) = AT(d, 0x10, s32) != 0 ? d + AT(d, 0x10, s32) : NULL;
    AT(c, 0x1528, u8) = VCALL_AT(mgr, 0x28, 0x20, u8 (*)(VObject *))(mgr);
    AT(c, 0xD0, u8) = 1;
    VCALL(c->motion, 0xC, void (*)(void *))(c->motion);
    AT(c, 0xD1, u8) = 1;
    AT(c->motion, 0x24, u8) = AT(c, 0x1528, u8);
}

/* +0x20, then func_002E2300 */
void func_0032C000(Character *c) {
    VCALL(c, 0x20, void (*)(Character *))(c);
    func_002E2300(c);
}

/* turn toward `at` (+0x10C) unless its model is busy (+0x550 > 0): the turn's phase +0x10
   restarts at 34 degrees (+0x18 4), a step of 14 degrees x sin(phase) (the model's +0x1C its
   rate) and its sway +0x14 grows by 14 x that; once the turn is done (Actor_TurnToward 0) the
   phase mirrors past 90 and +0x18 is 8. (At 180 and over - never, as it restarts - it would
   snap to the heading.) */
void func_0032B080(Character *c, f32 *at) {
    static const union { u32 u; f32 f; } k14deg = {0x3E7A35DE}, kPi = {0x40490FDB};
    u8 *k = CR(c);
    f32 s, r, h, ph;

    AT(c, 0x10C, f32) = Actor_HeadingTo(&c->a, at);
    if (!(AT(c->motion, 0x550, f32) <= 0.0f)) {
        return;
    }
    AT(k, 0x10, f32) = 30.0f;
    AT(k, 0x14, s32) = 0;
    AT(k, 0x18, f32) = 4.0f;
    ph = *(volatile f32 *)(k + 0x10) + 4.0f;   /* (reloaded, so computed at run time) */
    AT(k, 0x10, f32) = ph;
    s = func_0031C248(kPi.f * ph / 180.0f);
    AT(AT(c->motion, 0x6A4, u8 *), 0x1C, f32) = s;
    r = Actor_TurnToward(&c->a, AT(c, 0x10C, f32), s * k14deg.f);
    AT(k, 0x14, f32) = 0.0f + AT(k, 0x14, f32) + 14.0f * s;
    if (AT(k, 0x10, f32) < 180.0f) {
        if (0.0f == r) {
            if (AT(k, 0x10, f32) < 90.0f) {
                AT(k, 0x10, f32) = 180.0f - AT(k, 0x10, f32);
            }
            AT(k, 0x18, f32) = 8.0f;
        }
        return;
    }
    h = func_002E2D00(AT(c, 0x10C, f32));
    c->a.angle[1] = h;
    sceVu0UnitMatrix(c->a.rot);
    sceVu0RotMatrixY(c->a.rot, c->a.rot, h);
}

/* 1 when it notices someone: Fiona reachable on her triangle within `fiona` of its sight
   (+0x48); Hewie up in its room, reachable on his, within 30 at `hewie` (>= 0); an action 4 on
   it (not in move 4); or no sight at all */
s32 func_0032A0E0(Character *c, f32 fiona, f32 hewie) {
    u8 *k = CR(c);
    Character *h;

    if (Actor_TriTo(&c->a, gCharPlayer->a.pos, NAV_NONE) == gCharPlayer->a.navTri && fiona <= (f32)AT(k, 0x48, s16)) {
        return 1;
    }
    h = gCharPartner;
    if (h != NULL && h->a.active == 1 && c->a.room == h->a.room && !(hewie < 0.0f) &&
        Actor_TriTo(&c->a, gCharPartner->a.pos, NAV_NONE) == gCharPartner->a.navTri && hewie <= 30.0f) {
        return 1;
    }
    if (AT(c, 0xF8, s32) != 4 && AT(c, 0x14E8, s32) == 4) {
        return 1;
    }
    return AT(k, 0x48, s16) == 0;
}

/* its footsteps while walking (animation 0x200), in play: sounds 6 / 7 as each foot comes
   down (+0x73 / +0x72 last frame's) */
void func_0032AF00(Character *c) {
    u8 *k = CR(c);
    f32 l[4] __attribute__((aligned(16)));
    f32 r[4] __attribute__((aligned(16)));
    u8 left, right;

    if (c->a.disabled == 1 || c->a.navTri == NAV_NONE) {
        return;
    }
    if ((VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) & 0xFF) == 1) {
        return;
    }
    if ((Progress_TestFlag(gProgress, 8) & 0xFF) == 1) {
        return;
    }
    left = func_002DD420(c->motion, l, 1, 0.0f, 1.0f);
    right = func_002DD420(c->motion, r, 0, 0.0f, 1.0f);
    if (AT(c->motion, 0x55C, s32) == 0x200) {
        if (right == 1 && AT(k, 0x73, u8) == 0) {
            Actor_PlaySound(&c->a, 6, 5, 0, 0, NULL);
        }
        if (left == 1 && AT(k, 0x72, u8) == 0) {
            Actor_PlaySound(&c->a, 7, 5, 0, 0, NULL);
        }
    }
    AT(k, 0x72, u8) = left;
    AT(k, 0x73, u8) = right;
}

/* +0x2C draw, once out (+0x6E: and its trip begun, +0x6F) or not resting (+0x4A): lit
   (+0xE4: its light +0x80 unless layer 0x17), else on layer 0xF faded by +0x71 (0x80 at most;
   other layers become 0xA); the texture cache's +0x18, its model */
void func_0032BD90(Character *c) {
    u8 *k = CR(c);

    if (AT(k, 0x6E, u8) == 0) {
        if (AT(k, 0x4A, s16) != 0) {
            return;
        }
    } else if (AT(k, 0x6F, u8) == 0) {
        return;
    }
    if (c->unkE4 == 1) {
        if (c->unk152C != 0x17) {
            VCALL(c, 0x80, void (*)(Character *))(c);
        }
    } else if (c->unk152C == 0xF) {
        u32 a = AT(k, 0x71, u8);

        if (a > 0x80) {
            a = 0x80;
        }
        VCALL(gRenderer, 0x70, void (*)(VObject *, u32))(gRenderer, a << 24 | 0x808080);
    } else {
        c->unk152C = 0xA;
    }
    VCALL(gTexCache, 0x18, void (*)(VObject *))(gTexCache);
    VCALL(c->motion, 0x38, void (*)(void *, s32, u32, s32))(c->motion, c->unk152C, c->a.navTri, 0);
}

void func_0032A550(Character *c);
void func_0032B210(Character *c, u8 st);

/* +0x30 its frame: doors noted (+0x50), func_0032A550; out on a trip (+0x6F): out of contact
   it travels (unless held, +0x6B bit 0x80, and when the room entry allows), in play turned by
   its animation with +0x84, footsteps and its state's choice (func_0032B210 with +0x62); then
   its state (+0xA0) and the model (+0x40) */
void func_0032BB50(Character *c) {
    u8 *k = CR(c);

    creature_at_doors(c, 0x50);
    func_0032A550(c);
    if (AT(k, 0x6F, u8) != 0) {
        if (c->a.disabled != 0) {
            if (!(AT(k, 0x6B, u8) & 0x80) && func_00325B60(c) != 0) {
                func_003257B0(c);
            }
        } else {
            f32 h = func_002E2D00(c->a.angle[1] + func_001F6140(c->motion, 0.0f));

            c->a.angle[1] = h;
            sceVu0UnitMatrix(c->a.rot);
            sceVu0RotMatrixY(c->a.rot, c->a.rot, h);
            VCALL(c, 0x84, void (*)(Character *))(c);
            func_0032AF00(c);
            func_0032B210(c, AT(k, 0x62, u8));
        }
    }
    ptmf_scall(c, &c->a.state);
    VCALL(c, 0x40, void (*)(Character *))(c);
}

/* per frame, in the room being played: how far Fiona (+0x24) and Hewie are (across the floor).
   Not out (+0x6E): resting (+0x28 0) it notices them (func_0032A0E0) - a sound, out on its
   trip (+0x6F) in state 0xD (0xE for kinds 8 / 9); else (still settling) out of contact until
   the trip, which starts once it notices Fiona. Out: its rest +0x4A runs down, and done
   elsewhere it starts its trip */
void func_0032A260(Character *c) {
    u8 *k = CR(c);
    Progress *p = gProgress;
    f32 hd = 0.0f;

    if (c->a.room == VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        f32 d[4] __attribute__((aligned(16)));
        Character *h;

        sceVu0SubVector(d, c->a.pos, gCharPlayer->a.pos);
        AT(k, 0x24, f32) = __builtin_sqrtf(d[2] * d[2] + d[0] * d[0]);
        h = gCharPartner;
        if (h != NULL && gCharPartner->a.active == 1 && c->a.room == gCharPartner->a.room) {
            f32 e[4] __attribute__((aligned(16)));

            sceVu0SubVector(e, c->a.pos, gCharPartner->a.pos);
            hd = __builtin_sqrtf(e[2] * e[2] + e[0] * e[0]);
        }
        if (AT(k, 0x6E, u8) == 0) {
            if (AT(k, 0x28, f32) == 0.0f) {
                if ((func_0032A0E0(c, AT(k, 0x24, f32), hd) & 0xFF) == 1 && AT(k, 0x6F, u8) == 0) {
                    u32 kk;

                    Actor_PlaySound(&c->a, 1, 5, 0, 0, NULL);
                    AT(k, 0x6F, u8) = 1;
                    AT(k, 0x69, u8) = 0xD;
                    kk = AT(k, 0x6D, u8) ^ 0x80;
                    if (kk == 9 || kk == 8) {
                        AT(k, 0x69, u8) = 0xE;
                    }
                    AT(c, 0xF8, s32) = 0;
                    Character_ReleasePath(c);
                    cr19_settle(c);
                }
            } else {
                if (AT(k, 0x6F, u8) == 0) {
                    c->a.unk2D = 1;
                }
                if ((func_0032A0E0(c, AT(k, 0x24, f32), -1.0f) & 0xFF) == 1) {
                    AT(k, 0x6F, u8) = 1;
                }
            }
        }
    }
    if (AT(k, 0x6E, u8) == 0) {
        return;
    }
    if (AT(k, 0x4A, s16) > 0) {
        AT(k, 0x4A, s16)--;
    }
    if (AT(k, 0x4A, s16) > 0) {
        return;
    }
    AT(k, 0x4A, s16) = 0;
    if (c->a.room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        AT(k, 0x6F, u8) = 1;
    }
}

extern const PTMF D_0042C5E0, D_0042C5F0, D_0042C600;

/* its states by the frame: on its trip (+0x6F) its time out +0x44 grows to +0x46; in the room
   being played in contact, a door chosen when not done (+0x68); a chosen door (+0x65 1) that
   isn't open (state 2, +0x2B cleared) or is now passed (+0x4C) cleared; a different height
   from Fiona keeps +0x6A; then by the door: none - its state +0x69 into +0x62, chosen - 8,
   shut - 6. Elsewhere out of contact, timed out it is held (+0x6B bit 0x80): its state
   D_0042C5E0, held D_0042C5F0. Not on its trip: placed on its triangle once in the room being
   played, state D_0042C600 */
void func_0032A550(Character *c) {
    u8 *k = CR(c);
    Progress *p;

    func_0032A260(c);
    if (AT(k, 0x6F, u8) == 0) {
        p = gProgress;
        if (c->a.room == VCALL(p, 0xC, s32 (*)(Progress *))(p) && c->a.navTri != NAV_NONE && AT(k, 0x6C, u8) == 0) {
            AT(k, 0x6C, u8)++;
            VCALL(gNavMesh, 0xC, void (*)(NavMesh *, u32, f32 *))(gNavMesh, c->a.navTri, c->a.pos);
        }
        Actor_SetState(&c->a, &D_0042C600);
        return;
    }
    AT(k, 0x44, s16)++;
    if (!(AT(k, 0x44, s16) < AT(k, 0x46, s16))) {
        AT(k, 0x44, s16) = AT(k, 0x46, s16);
    }
    p = gProgress;
    if (c->a.room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        c->a.disabled = 1;
        if (!(AT(k, 0x44, s16) < AT(k, 0x46, s16))) {
            AT(k, 0x6B, u8) |= 0x80;
        }
        if (!(AT(k, 0x6B, u8) & 0x80)) {
            Actor_SetState(&c->a, &D_0042C5E0);
        } else {
            Actor_SetState(&c->a, &D_0042C5F0);
        }
        return;
    }
    c->a.disabled = 0;
    if (AT(k, 0x68, u8) == 0) {
        func_00325220(c);
    }
    if (AT(k, 0x65, u8) == 1) {
        if (!(Progress_ExitOpen(p, c->a.room, AT(k, 0x66, u8)) & 0xFF)) {
            AT(k, 0x65, u8) = 2;
            c->a.unk2B = 0;
        } else if ((VCALL(gDoors, 0x30, u32 (*)(VObject *, u32))(gDoors, AT(k, 0x66, u8)) & 0xFF) == 1 &&
                   (AT(k, 0x4C, u16) & 1)) {
            AT(k, 0x65, u8) = 0;
        }
    }
    if (c->a.room != VCALL(p, 0xC, s32 (*)(Progress *))(p) || gCharPlayer->a.pos[1] == c->a.pos[1]) {
        AT(k, 0x6A, u8) = 0;
    }
    switch (AT(k, 0x65, u8)) {
    case 0:
        if (AT(k, 0x69, u8) != AT(k, 0x62, u8)) {
            AT(k, 0x62, u8) = AT(k, 0x69, u8);
        }
        break;
    case 1:
        AT(k, 0x69, u8) = 8;
        AT(k, 0x62, u8) = 8;
        break;
    case 2:
        AT(k, 0x69, u8) = 6;
        AT(k, 0x62, u8) = 6;
        break;
    }
}

/* (a creature class) Character_Activate, its own block +0x61 0, +0x63 0xFF, mode 2 */
void func_0032A890(Character *c) {
    u8 *k = (u8 *)c + 0x1540;

    Character_Activate(c);
    AT(k, 0x61, u8) = 0;
    AT(k, 0x63, u8) = 0xFF;
    AT(k, 0x0, s32) = 2;
}

extern void func_002E2310(Character *c);

/* head for exit `exit` (+0x67 its door): in the room being played, its closed ways forgotten;
   a path to the exit's spot (+0x34; 0xFF: none) or else the first open door's (+0x70 0 once
   one is planned, +0x61 1 for the exit's own). With the exit seen from Fiona's spot
   (Actor_NearerRoom) the door is noted and its state 0. Without a plan, straight to the exit's
   spot when that works. On its way (+0x61) it keeps the next door (+0x14C0) and drops back
   onto its plan when that is no further from the door than it is */
void func_0032AAF0(Character *c, u32 exit) {
    u8 *k = CR(c);
    VObject *rooms = gRooms;
    Progress *p;
    f32 at[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    f32 door[4] __attribute__((aligned(16)));
    u32 e, d;
    s32 i;

    AT(k, 0x67, u8) = VCALL(rooms, 0x14, u32 (*)(VObject *, s32, u32))(rooms, c->a.room, exit);
    p = gProgress;
    if (c->a.room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        return;
    }
    for (i = 0; i < 13; i++) {
        c->unk148C[i] = 0;
    }
    AT(k, 0x61, u8) = 0;
    AT(k, 0x70, u8) = 1;
    e = exit & 0xFF;
    if (e != 0xFF) {
        u32 tri = VCALL(rooms, 0x34, u32 (*)(VObject *, u32, f32 *))(rooms, exit, at);

        if (func_003255C0(c, tri, at, 1) == 0) {
            AT(k, 0x70, u8) = 0;
            AT(k, 0x61, u8) = 1;
        }
    }
    if (AT(k, 0x70, u8) == 1) {
        for (d = 0; d < 8; d = (d + 1) & 0xFF) {
            u32 tri;

            if ((VCALL(rooms, 0x74, u32 (*)(VObject *, s32, u32))(rooms, c->a.room, d) & 0xFF) != 1) {
                continue;
            }
            if ((Progress_ExitOpen(p, c->a.room, d) & 0xFF) != 1) {
                continue;
            }
            tri = VCALL(rooms, 0x34, u32 (*)(VObject *, u32, f32 *))(rooms, d, at);
            if (func_003255C0(c, tri, at, 1) == 0) {
                AT(k, 0x70, u8) = 0;
                break;
            }
        }
    }
    if (AT(k, 0x70, u8) == 1) {
        return;
    }
    if (e != 0xFF && (Actor_NearerRoom(&c->a, exit, gCharPlayer->a.pos) & 0xFF) == 1) {
        VCALL(rooms, 0x14, u32 (*)(VObject *, s32, u32))(rooms, c->a.room, exit);
        AT(k, 0x8, s32) = 0;
        return;
    }
    if (AT(k, 0x61, u8) == 0) {
        u32 tri = VCALL(rooms, 0x34, u32 (*)(VObject *, u32, f32 *))(rooms, exit, b);

        if (Character_PlanPathKind(c, 0, tri, b) > 0) {
            sceVu0CopyVector(at, b);
            if (Character_Waypoints(c) > 0) {
                AT(k, 0x61, u8) = 1;
            }
        }
        Character_ReleasePath(c);
    }
    if (AT(k, 0x61, u8) != 1) {
        return;
    }
    rooms = gRooms;
    c->unk14C0 = VCALL(rooms, 0x10, u32 (*)(VObject *, s32, u32))(rooms, c->a.room, exit);
    VCALL(rooms, 0x30, u32 (*)(VObject *, u32, f32 *))(rooms, exit, door);
    {
        f32 d1[4] __attribute__((aligned(16)));
        f32 d2[4] __attribute__((aligned(16)));
        f32 n1, n2;

        sceVu0SubVector(d1, c->a.pos, door);
        sceVu0SubVector(d2, at, door);
        n1 = sceVu0InnerProduct(d1, d1);
        n2 = sceVu0InnerProduct(d2, d2);
        if (!(n1 <= n2)) {
            return;
        }
    }
    if (Actor_TriTo(&c->a, door, c->pathReq->mask) == NAV_NONE) {
        return;
    }
    c->unk124 = c->unk128;
}

/* +0xC set up: the creature base's (func_002E2310); size 3 x 6, company, in contact, blocking
   0x4020028 (path kind 6); a random kind 0x80..0x82 with its table entry; all its own state
   cleared (speed 0.7, a wait of 150 / 300 / 450); then +0x1C, +0x5C, idle 0 */
void func_0032C040(Character *c) {
    u8 *k = CR(c);
    VObject *rnd;
    const u8 *e;
    s32 i;

    func_002E2310(c);
    c->a.radius = 3.0f;
    c->a.height = 6.0f;
    c->a.unk2A = 1;
    c->a.unk2D = 0;
    c->a.navMask = 0x4020028;
    rnd = gRandom;
    c->pathReq->unk4 = 6;
    c->pathReq->mask = c->a.navMask;
    AT(k, 0x6D, u8) = (VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 0xF) % 3 | 0x80;
    e = D_0042C460 + (AT(k, 0x6D, u8) ^ 0x80) * 8;
    AT(c, 0x14C8, s32) = AT(e, 0x0, s32);
    AT(k, 0x46, s16) = AT(e, 0x4, s16);
    AT(k, 0x0, s32) = 0;
    AT(k, 0x4, u32) = 0x3F333333;   /* 0.7f */
    AT(k, 0x8, s32) = 0;
    AT(k, 0xC, s32) = 0;
    AT(k, 0x10, s32) = 0;
    AT(k, 0x14, s32) = 0;
    AT(k, 0x18, s32) = 0;
    AT(k, 0x1C, s32) = 0;
    AT(k, 0x20, s32) = 0;
    AT(k, 0x24, s32) = 0;
    AT(k, 0x30, s32) = 0;
    AT(k, 0x34, s32) = 0;
    AT(k, 0x38, s32) = 0;
    AT(k, 0x3C, s32) = 0;
    AT(k, 0x40, s16) = 0;
    AT(k, 0x42, s16) = (VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 0xF) % 3 * 150 + 150;
    AT(k, 0x44, s16) = 0;
    AT(k, 0x48, s16) = 0;
    AT(k, 0x61, u8) = 0;
    AT(k, 0x62, u8) = 0;
    AT(k, 0x63, u8) = 0;
    AT(k, 0x64, u8) = 0;
    AT(k, 0x65, u8) = 0;
    AT(k, 0x66, u8) = 0;
    AT(k, 0x4C, u16) = 0;
    AT(k, 0x67, u8) = 0;
    AT(k, 0x68, u8) = 0;
    AT(k, 0x69, u8) = 0;
    AT(k, 0x60, u8) = 0;
    AT(k, 0x6A, u8) = 0;
    AT(k, 0x6B, u8) = 0;
    AT(k, 0x6C, u8) = 0;
    AT(k, 0x70, u8) = 0;
    AT(k, 0x71, u8) = 0;
    AT(k, 0x28, s32) = 0;
    AT(k, 0x2C, s32) = 0;
    for (i = 0; i < 8; i++) {
        AT(k, 0x50 + i * 2, s16) = 0;
    }
    VCALL(c, 0x1C, void (*)(Character *))(c);
    VCALL(c, 0x5C, void (*)(Character *))(c);
    func_002DDED0(c->motion, 0, -1);
    AT(k, 0x69, u8) = 0;
}

/* 0x0032C240 */
Character *Kind25_dtor(Character *c, s32 flags) { return creature_dtor(c, flags, D_00474130); }

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
void Kind25_EventState(Pursuer *p) { creature_act5(p, &D_0042C720); }

/* 0x0032C4A0 */
s32 Kind25_GrabOrder(Pursuer *p) { return creature_slot_done(p); }

extern const PTMF D_0042C4B0, D_0042C4C0, D_0042C4D0, D_0042C4E0, D_0042C4F0, D_0042C500, D_0042C510,
    D_0042C520, D_0042C530, D_0042C540, D_0042C550, D_0042C560, D_0042C570, D_0042C580, D_0042C590,
    D_0042C5A0, D_0042C5B0, D_0042C5C0, D_0042C5D0;
extern void *D_004795E0[];

/* the effect it leaves when it goes (0x2920 bytes, D_004795E0, two quad drawers at +0x2410) */
static inline void Cr19Gone_Init(void **obj) {
    obj[0] = D_004795E0;
    obj[0x2410 / 4] = D_00469D00;
    ((s32 *)obj)[0x2414 / 4] = -1;
    obj[0x2410 / 4] = D_0046FC30;
    obj[0x2448 / 4] = D_00469D00;
    ((s32 *)obj)[0x244C / 4] = -1;
    obj[0x2448 / 4] = D_0046FC30;
}

/* on the first frame of a state (+0x60 0): animation `anim` */
static inline void cr19_first(Character *c, s32 anim) {
    u8 *k = CR(c);

    if (AT(k, 0x60, u8) == 0) {
        func_002DDED0(c->motion, anim, -1);
        AT(k, 0x60, u8)++;
    }
}

/* its state for the choice `st` (+0x62): the state function (+0xA0), with the state's first
   animation. State 4 (leaving): out of contact; on its first frame animation 0x1801, faded
   from 0xFF on layer 0xF unless already lit there, and its going effect at it */
void func_0032B210(Character *c, u8 st) {
    u8 *k = CR(c);

    switch (st) {
    case 0:
        cr19_first(c, 0);
        Actor_SetState(&c->a, &D_0042C4B0);
        break;
    case 1:
        cr19_first(c, 0x1C00);
        Actor_SetState(&c->a, &D_0042C4C0);
        break;
    case 2:
        Actor_SetState(&c->a, &D_0042C4D0);
        break;
    case 3:
        Actor_SetState(&c->a, &D_0042C4E0);
        break;
    case 4:
        c->a.unk2D = 1;
        if (AT(k, 0x60, u8) == 0) {
            u8 *mgr;
            struct {
                f32 pos[3];
                f32 size;
                u32 tri;
            } sp;
            s32 slot;

            func_002DDED0(c->motion, 0x1801, -1);
            AT(k, 0x60, u8)++;
            if (c->unkE4 != 1 || c->unk152C != 0xF) {
                c->unkE4 = 0;
                c->unk152C = 0xF;
                AT(k, 0x71, u8) = 0xFF;
            }
            mgr = gEffects;
            slot = Effect_New(mgr, 0x2920, Cr19Gone_Init);
            sp.pos[0] = c->a.pos[0];
            sp.pos[1] = c->a.pos[1];
            sp.pos[2] = c->a.pos[2];
            sp.size = 1.0f;
            sp.tri = c->a.navTri;
            func_002D6090(mgr, slot, &sp);
        }
        Actor_SetState(&c->a, &D_0042C4F0);
        break;
    case 5:
        cr19_first(c, 0x200);
        Actor_SetState(&c->a, &D_0042C500);
        break;
    case 6:
        cr19_first(c, 0x200);
        Actor_SetState(&c->a, &D_0042C510);
        break;
    case 7:
        cr19_first(c, 0x200);
        Actor_SetState(&c->a, &D_0042C520);
        break;
    case 9:
        cr19_first(c, 0x200);
        Actor_SetState(&c->a, &D_0042C530);
        break;
    case 10:
        cr19_first(c, 0x200);
        Actor_SetState(&c->a, &D_0042C540);
        break;
    case 11:
        Actor_SetState(&c->a, &D_0042C550);
        break;
    case 12:
        cr19_first(c, 0x200);
        Actor_SetState(&c->a, &D_0042C560);
        break;
    case 13:
        cr19_first(c, 0x200);
        Actor_SetState(&c->a, &D_0042C570);
        break;
    case 14:
        cr19_first(c, 0x200);
        Actor_SetState(&c->a, &D_0042C580);
        break;
    case 15:
        cr19_first(c, 0);
        Actor_SetState(&c->a, &D_0042C590);
        break;
    case 16:
        cr19_first(c, 0x200);
        Actor_SetState(&c->a, &D_0042C5A0);
        break;
    case 17:
        cr19_first(c, 0);
        Actor_SetState(&c->a, &D_0042C5B0);
        break;
    case 18:
        Actor_SetState(&c->a, &D_0042C5C0);
        break;
    default:
        Actor_SetState(&c->a, &D_0042C5D0);
        break;
    }
}

/* into state `st` in move `move` */
static inline void cr19_enter_move(Character *c, u8 st, s32 move) {
    AT(CR(c), 0x69, u8) = st;
    AT(c, 0xF8, s32) = move;
    Character_ReleasePath(c);
    cr19_settle(c);
}

/* state: after Fiona. Progress flag 9 - state 9 (move 2); Fiona caught - 0xC (move 2); Fiona out
   of contact or busy (+0x2D / +0xE0) without flag 10 - 0xF; a door it may use shut for kind 2
   - 0x11; across the room's divider from her - 0x12. Else facing her: her in move 4 / 3 -
   state 7 (+0x6B bit 0x40). It walks straight at her when it can (func_003279F0, unless given
   up, +0x6A); during an event (flag 0x18) elsewhere or with her busy - 9. Within 10 and on her
   triangle (not in state 3, held, or her move 8) it grabs (state 3). Without a straight way it
   walks its path to her (re-planned when done; her triangle blocked - 0xA; unplannable and out
   of sight - 0x10). At 40..80 from her, every +0x42 frames it settles in state 0 or 1 */
void func_00329440(Character *c) {
    u8 *k = CR(c);
    Progress *p = gProgress;
    VObject *doors;
    s32 t;
    u32 d;

    if (Progress_TestFlag(p, 9) != 0) {
        cr19_enter_move(c, 9, 2);
        return;
    }
    if (AT(gCharPlayer, 0x1AD630, u8) == 1) {
        cr19_enter_move(c, 0xC, 2);
        return;
    }
    if (AT(gCharPlayer, 0x2D, u8) == 1 || AT(gCharPlayer, 0xE0, u8) == 1) {
        if (!(Progress_TestFlag(p, 0xA) & 0xFF)) {
            cr19_enter(c, 0xF);
            return;
        }
    }
    doors = gDoors;
    for (d = 0; d < 8; d = (d + 1) & 0xFF) {
        if ((VCALL(doors, 0x40, u32 (*)(VObject *, u32))(doors, d) & 0xFF) != 1) {
            continue;
        }
        if (!((AT(c, 0x1590 + (d & 0xFF) * 2, u16) & ((1 << *(u8 *)&c->a.slot) & 0xFFFF)) != 0)) {
            continue;
        }
        if (!(PursuerGroup_Fields(p, d, 2) & 0xFF & 4)) {
            continue;
        }
        cr19_enter(c, 0x11);
        return;
    }
    if (NavMesh_AcrossDivider(gNavMesh, gCharPlayer->a.navTri, c->a.navTri)) {
        cr19_enter(c, 0x12);
        return;
    }
    AT(k, 0x68, u8) = 0;
    func_0032B080(c, gCharPlayer->a.pos);
    if (AT(gCharPlayer, 0xF8, s32) == 4 || AT(gCharPlayer, 0xF8, s32) == 3) {
        AT(k, 0x6B, u8) |= 0x40;
        cr19_enter(c, 7);
        return;
    }
    AT(k, 0x40, s16)++;
    t = -1;
    if (AT(k, 0x6A, u8) == 0) {
        t = func_003279F0(c, gCharPlayer->a.pos);
    }
    if (Progress_TestFlag(p, 0x18) != 0) {
        if (c->a.room != VCALL(p, 0xC, s32 (*)(Progress *))(p) || AT(gCharPlayer, 0xE0, u8) != 0) {
            cr19_enter_move(c, 9, 2);
            return;
        }
    }
    if (t != -1 && AT(k, 0x24, f32) <= 10.0f && AT(k, 0x69, u8) != 3 && !(AT(k, 0x6B, u8) & 0x80) &&
        AT(gCharPlayer, 0xF8, s32) != 8) {
        if (t == (s32)gCharPlayer->a.navTri) {
            cr19_enter(c, 3);
        }
        return;
    }
    if (t < 0) {
        if (!(c->unk128 < c->unk124)) {
            if (gCharPlayer->a.navTri == NAV_NONE) {
                return;
            }
            if (NavMesh_TriFlags(gNavMesh, gCharPlayer->a.navTri) & 0x4020028) {
                cr19_enter(c, 0xA);
                return;
            }
            if (func_003255C0(c, gCharPlayer->a.navTri, gCharPlayer->a.pos, 0) != 0) {
                if (!(Actor_CanWalkBetween(c, c->a.navTri, gCharPlayer->a.navTri, c->a.pos, gCharPlayer->a.pos, 0) & 0xFF)) {
                    cr19_enter(c, 0x10);
                }
                return;
            }
        }
        Character_FollowWaypointsBlocked(c, cr19_stride(c));
    }
    if (AT(k, 0x24, f32) < 40.0f || !(AT(k, 0x24, f32) <= 80.0f)) {
        return;
    }
    if (AT(k, 0x40, s16) % AT(k, 0x42, s16) != 0) {
        return;
    }
    {
        s8 r = VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) & 1;

        if (r == 0) {
            AT(k, 0x69, u8) = 0;
        } else if (r == 1) {
            AT(k, 0x69, u8) = 1;
        }
    }
    AT(c, 0xF8, s32) = 0;
    Character_ReleasePath(c);
    cr19_settle(c);
}

void func_0032A0D0(void) {
}

/* ---- D_004795E0 (0x2920 bytes, Cr19Gone_Init): what the kind-0x19 creature leaves when it
 * goes - at a point (+0x2480) on nav triangle +0x2918, 64 bubbles welling up (records +0x10 +
 * 0xC00 x the current one +0x2910, drawer +0x2410, velocities +0x2490) and 32 drops thrown up
 * (records +0x1810 + 0x600 x the current one, drawer +0x2448, velocities +0x2790), +0x2914
 * its frames; at frame 100 a splat (D_00472BF0) on the spot ---- */

extern void *D_00472BF0[];

#define GONE_BUBBLE(o, buf, i) ((QuadRec *)((o) + 0x10 + (buf) * 0xC00) + (i))
#define GONE_DROP(o, buf, i) ((QuadRec *)((o) + 0x1810 + (buf) * 0x600) + (i))
#define GONE_RND() VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd)

/* drop `i` thrown up from within 2 of the spot (lower the later, frame `t`), 2.5..3 big,
 * flying outward and up, faster the later */
void func_003592E0(u8 *o, s32 i, s32 t) {
    static const union { u32 u; f32 f; } k4 = {0x40800000}, k001 = {0x3C23D70A}, k360 = {0x43B40000},
                                         kPi = {0x40490FDB};   /* multiplied first */
    VObject *rnd = gRandom;
    QuadRec *r = GONE_DROP(o, AT(o, 0x2910, s32), i);
    f32 dx, dz, *v = (f32 *)(o + 0x2790 + i * 0xC);

    dx = k4.f * (GONE_RND() - 0.5f);
    dz = k4.f * (GONE_RND() - 0.5f);
    r->rgba[0] = 0x40;
    r->rgba[1] = 0x10;
    r->rgba[2] = 8;
    r->rgba[3] = 1;
    r->pos[0] = AT(o, 0x2480, f32) + dx;
    r->pos[1] = (1.0f + AT(o, 0x2484, f32)) - (k001.f * (f32)t) * GONE_RND();
    r->pos[2] = AT(o, 0x2488, f32) + dz;
    r->pos[3] = 1.0f;
    r->w = 2.5f + 0.5f * GONE_RND();
    r->h = r->w;
    r->turn = kPi.f * (k360.f * (GONE_RND() - 0.5f)) / 180.0f;
    r->frame = 0;
    v[0] = (k001.f * -dx) * GONE_RND();
    v[1] = 0x1.47ae14p-7f + 0x1.0624dep-11f * (f32)t;   /* 0.01 + 0.0005 t */
    v[2] = (k001.f * -dz) * GONE_RND();
}

/* bubble `i` (re)started within 3 x its size of the spot, two sizes up, drifting out and
 * rising 0.03..0.06 */
void func_00359570(u8 *o, s32 i) {
    static const union { u32 u; f32 f; } k6 = {0x40C00000}, k003 = {0x3CF5C28F}, k360 = {0x43B40000},
                                         kPi = {0x40490FDB};   /* multiplied first */
    VObject *rnd = gRandom;
    QuadRec *r = GONE_BUBBLE(o, AT(o, 0x2910, s32), i);
    f32 s = r->w, dx, dz, *v = (f32 *)(o + 0x2490 + i * 0xC);

    dx = (k6.f * s) * (GONE_RND() - 0.5f);
    dz = (k6.f * r->w) * (GONE_RND() - 0.5f);
    r->rgba[0] = 0x60;
    r->rgba[1] = 0x30;
    r->rgba[2] = 0x28;
    r->rgba[3] = 1;
    r->pos[0] = AT(o, 0x2480, f32) + dx;
    r->pos[1] = AT(o, 0x2484, f32) + 2.0f * s;
    r->pos[2] = AT(o, 0x2488, f32) + dz;
    r->pos[3] = 1.0f;
    r->turn = kPi.f * (k360.f * (GONE_RND() - 0.5f)) / 180.0f;
    r->frame = 0;
    v[0] = (k003.f * dx) * GONE_RND();
    v[1] = 0x1.eb851ep-6f + 0x1.eb851ep-6f * GONE_RND();   /* 0.03 + 0.03 x */
    v[2] = (k003.f * dz) * GONE_RND();
}

/* +0x18 start: arg { the spot, +0x10 its triangle (-1: nothing) }; every bubble hidden, 0.1 ..
 * 0.4 big, every drop hidden at 0.5 */
/* 0x003597B0 */
void Cr19Bubbles_SetParams(u8 *o, u8 *arg) {
    static const union { u32 u; f32 f; } k01 = {0x3DCCCCCD};   /* multiplied first */
    VObject *rnd;
    s32 i;

    if (arg == NULL) {
        return;
    }
    sceVu0CopyVector((f32 *)(o + 0x2480), (f32 *)arg);
    AT(o, 0x2918, s32) = AT(arg, 0x10, s32);
    if (AT(o, 0x2918, s32) == -1) {
        return;
    }
    rnd = gRandom;
    for (i = 0; i < 64; i++) {
        QuadRec *r = GONE_BUBBLE(o, AT(o, 0x2910, s32), i);
        f32 *v = (f32 *)(o + 0x2490 + i * 0xC);

        r->w = k01.f * (f32)((VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 3) + 1);
        r->h = r->w;
        r->rgba[0] = 0;
        r->rgba[1] = 0;
        r->rgba[2] = 0;
        r->rgba[3] = 0;
        r->pos[0] = 0.0f;
        r->pos[1] = 0.0f;
        r->pos[2] = 0.0f;
        r->pos[3] = 1.0f;
        r->turn = 0.0f;
        r->frame = 0;
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[2] = 0.0f;
    }
    for (i = 0; i < 32; i++) {
        QuadRec *r = GONE_DROP(o, AT(o, 0x2910, s32), i);
        f32 *v = (f32 *)(o + 0x2790 + i * 0xC);

        r->rgba[0] = 0;
        r->rgba[1] = 0;
        r->rgba[2] = 0;
        r->rgba[3] = 0;
        r->pos[0] = 0.0f;
        r->pos[1] = 0.0f;
        r->pos[2] = 0.0f;
        r->pos[3] = 1.0f;
        r->w = 0.5f;
        r->h = r->w;
        r->turn = 0.0f;
        r->frame = 0;
        v[0] = 0.0f;
        v[1] = 0.0f;
        v[2] = 0.0f;
    }
}

/* +0x14 draw (not while the effects are paused): the bubbles, then the drops */
/* 0x00359980 */
void Cr19Bubbles_Draw(u8 *o) {
    if (func_002D6010(gEffects) == 0) {
        AT(o, 0x2420, QuadRec *) = GONE_BUBBLE(o, AT(o, 0x2910, s32), 0);
        func_002E56C0(o + 0x2410);
        AT(o, 0x2458, QuadRec *) = GONE_DROP(o, AT(o, 0x2910, s32), 0);
        func_002E56C0(o + 0x2448);
    }
}

static void gone_splat_init(void **obj) {
    obj[0] = D_00472BF0;
    obj[0x70 / 4] = D_00469D00;
    ((s32 *)obj)[0x74 / 4] = -1;
    obj[0x70 / 4] = D_0046FC30;
}

/* +0x10 update: flip the buffers, count the frame (at 100 the splat); each bubble showing
 * carried over, fading in (to over 0x20, then marked) or out by 1..4, slowing as it rises, kept
 * above the spot by its size; until frame 127 up to 4 hidden bubbles a frame restarted, growing
 * by 0.05..0.1 (to 0.7) for 90 frames, then shrinking (out under 0.3); each drop showing fading
 * in / out by 1..2, shrinking (out at 0), turning and flying; until frame 150 one hidden drop a
 * frame thrown. 0 once nothing shows */
/* 0x00359A00 */
s32 Cr19Bubbles_Update(u8 *o) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};   /* multiplied first */
    VObject *rnd;
    u8 done = 1;
    s32 i, k, n;

    if (AT(o, 0x2918, s32) == -1) {
        return 0;
    }
    AT(o, 0x2910, s32) ^= 1;
    AT(o, 0x2914, s32)++;
    if (AT(o, 0x2914, s32) == 100) {
        struct {
            f32 pos[4];
            s32 tri;
            u8 one;
        } sp __attribute__((aligned(16)));
        u8 *mgr = gEffects;
        s32 slot = Effect_New(mgr, 0x140, gone_splat_init);

        sp.pos[0] = AT(o, 0x2480, f32);
        sp.pos[1] = AT(o, 0x2484, f32);
        sp.pos[2] = AT(o, 0x2488, f32);
        sp.pos[3] = 1.0f;
        sp.tri = AT(o, 0x2918, s32);
        sp.one = 1;
        func_002D6090(mgr, slot, &sp);
    }
    rnd = gRandom;
    for (i = 0; i < 64; i++) {
        QuadRec *r;
        f32 *v = (f32 *)(o + 0x2490 + i * 0xC);

        for (k = 0; k < 12; k++) {
            ((u32 *)GONE_BUBBLE(o, AT(o, 0x2910, s32), i))[k] = ((u32 *)GONE_BUBBLE(o, AT(o, 0x2910, s32) ^ 1, i))[k];
        }
        r = GONE_BUBBLE(o, AT(o, 0x2910, s32), i);
        if (r->rgba[3] == 0) {
            continue;
        }
        if (r->rgba[0] == 0x60) {
            r->rgba[3] = r->rgba[3] + ((VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 3) + 1);
            if (r->rgba[3] >= 0x21) {
                r->rgba[0]--;
            }
        } else {
            r->rgba[3] = r->rgba[3] - ((VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 3) + 1);
        }
        if (r->rgba[3] <= 0) {
            r->rgba[3] = 0;
            continue;
        }
        v[1] = v[1] - 0x1.47ae14p-9f;   /* 0.0025 */
        r->pos[0] = r->pos[0] + v[0];
        r->pos[1] = r->pos[1] + v[1];
        done = 0;
        if (r->pos[1] < AT(o, 0x2484, f32) + r->w) {
            r->pos[1] = AT(o, 0x2484, f32) + r->w;
        }
        r->pos[2] = r->pos[2] + v[2];
    }
    if (AT(o, 0x2914, s32) < 0x7F) {
        n = 0;
        for (i = 0; i < 64; i++) {
            QuadRec *r = GONE_BUBBLE(o, AT(o, 0x2910, s32), i);

            if (r->rgba[3] != 0) {
                continue;
            }
            if ((u32)AT(o, 0x2914, s32) < 90) {
                r->w = r->w + (0x1.99999ap-5f + 0x1.99999ap-5f * GONE_RND());   /* 0.05 + 0.05 x */
                if (!(r->w <= 0x1.666666p-1f)) {
                    r->w = 0x1.666666p-1f;   /* 0.7 */
                }
            } else {
                r->w = r->w - (0x1.99999ap-5f + 0x1.99999ap-5f * GONE_RND());
                if (r->w <= 0x1.333334p-2f) {   /* 0.3 */
                    r->rgba[3] = 0;
                    r->w = 0.0f;
                    r->h = 0.0f;
                    continue;
                }
            }
            r->h = r->w;
            func_00359570(o, i);
            n++;
            done = 0;
            if (n >= 4) {
                break;
            }
        }
    }
    for (i = 0; i < 32; i++) {
        QuadRec *r;
        f32 *v = (f32 *)(o + 0x2790 + i * 0xC);

        for (k = 0; k < 12; k++) {
            ((u32 *)GONE_DROP(o, AT(o, 0x2910, s32), i))[k] = ((u32 *)GONE_DROP(o, AT(o, 0x2910, s32) ^ 1, i))[k];
        }
        r = GONE_DROP(o, AT(o, 0x2910, s32), i);
        if (r->rgba[3] == 0) {
            continue;
        }
        done = 0;
        if (r->rgba[0] == 0x40) {
            r->rgba[3] = r->rgba[3] + ((VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 1) + 1);
            if (r->rgba[3] >= 0x21) {
                r->rgba[0]--;
            }
        } else {
            r->rgba[3] = r->rgba[3] - ((VCALL(rnd, 0x10, u32 (*)(VObject *))(rnd) & 1) + 1);
            if (r->rgba[3] < 0) {
                r->rgba[3] = 0;
            }
        }
        if (r->rgba[3] <= 0) {
            r->rgba[3] = 0;
            continue;
        }
        r->w = r->w - 0x1.47ae14p-7f * GONE_RND();   /* 0.01 */
        if (r->w < 0.0f) {
            r->rgba[3] = 0;
            r->w = 0.0f;
        }
        r->h = r->w;
        r->turn = r->turn + kPi.f * GONE_RND() / 180.0f;
        if (!(r->turn <= kPi.f)) {
            r->turn = r->turn - 0x1.921fb6p+2f;
        }
        r->pos[0] = r->pos[0] + v[0];
        r->pos[1] = r->pos[1] + v[1];
        r->pos[2] = r->pos[2] + v[2];
    }
    for (i = 0; i < 32; i++) {
        if (GONE_DROP(o, AT(o, 0x2910, s32), i)->rgba[3] == 0 && (u32)AT(o, 0x2914, s32) < 150) {
            func_003592E0(o, i, AT(o, 0x2914, s32));
            done = 0;
            break;
        }
    }
    return done == 1 ? 0 : 1;
}

/* Initialises two render setting blocks at +0x2418 and +0x2450. */
/* 0x0035A170 */
void Cr19Bubbles_Start(u8 *p) {
    B7_W(p, 0x2910) = 0;
    B7_W(p, 0x2914) = 0;
    B7_D(p, 0x2418) = -1;
    B7_W(p, 0x2424) = 0;
    B7_W(p, 0x2428) = 0;
    B7_W(p, 0x242C) = 0;
    B7_W(p, 0x2430) = 0x19;
    B7_H(p, 0x2434) = 0x40;
    B7_H(p, 0x2436) = 0x1A0;
    B7_H(p, 0x2438) = 0x40;
    B7_H(p, 0x243A) = 0x20;
    B7_H(p, 0x243C) = 0x20;
    B7_H(p, 0x243E) = 0x200;
    B7_H(p, 0x2440) = 0x100;
    B7_B(p, 0x2442) = 0;
    B7_B(p, 0x2443) = 1;
    B7_B(p, 0x2444) = 1;
    B7_B(p, 0x2445) = 0x10;
    B7_B(p, 0x2446) = 2;
    B7_D(p, 0x2450) = -1;
    B7_W(p, 0x245C) = 0;
    B7_W(p, 0x2460) = 0;
    B7_W(p, 0x2464) = 0;
    B7_W(p, 0x2468) = 0x19;
    B7_H(p, 0x246C) = 0x20;
    B7_H(p, 0x246E) = 0;
    B7_H(p, 0x2470) = 0x40;
    B7_H(p, 0x2472) = 0x20;
    B7_H(p, 0x2474) = 0x20;
    B7_H(p, 0x2476) = 0x200;
    B7_H(p, 0x2478) = 0x100;
    B7_B(p, 0x247A) = 0;
    B7_B(p, 0x247B) = 1;
    B7_B(p, 0x247C) = 1;
    B7_B(p, 0x247D) = 0x10;
    B7_B(p, 0x247E) = 0xFF;
}
