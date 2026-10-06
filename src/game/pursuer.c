/* Pursuer: the stalkers' shared base class, code 0x278490..0x29FF10. See include/pursuer.h.
 *
 * (was pursuer_ai.c) Pursuer methods and helpers, code 0x211C80..0x219460, and the stalker
 * vtables' defaults (0x179600..0x179970). See include/pursuer.h.
 */
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
#include "renderer.h"
#include "charaction.h"
#include "input.h"
#include "gl2d.h"
#include "daniella.h"
#include "debilitas2.h"
#include "hewie.h"
#include "lorenzo.h"
#include "model.h"
#include "fiona.h"
#include "system.h"
#include "scene_game.h"
#include "heap.h"
#include "vecmath.h"
#include "msl.h"
#include "char_load.h"
#include "music.h"
#include "director.h"
#include "doors.h"
#include "room.h"
#include "text.h"
#include "movie.h"
#include "placed.h"
#include "room_map.h"
#include "lights.h"
#include "draw_leaves.h"
#include "libc.h"
#include "sce/eekernel.h"
#include "sce/intc.h"
#include "effectmgr.h"
#include "riccardo.h"
#include "story_chars.h"
#include "tintstalker.h"

extern u8 kPursuerSteps[]; /* table of 28-byte entries */
extern u8 pstr_O_DB0_DB0_200_PCK[];
extern u8 pstr_O_DB0_DB0_200_PCK_2[];
#define FLD(p, off, T) (*(T *)((u8 *)(p) + (off)))

extern void *Kind15_vtable[];
extern void *Kind14_vtable[];
extern void *Debilitas2_vtable[];
extern void *Debilitas_vtable[];

#define U32(p, off) (*(u32 *)((u8 *)(p) + (off)))

static inline s32 b5_prog_flag8000(void);

static inline s32 b5_prog_flag8000(void);

extern void *D_0046D730[];
extern void *EffectBase_vtable[];
extern void *Reflection_vtable[];
extern void *Effect79FF0_vtable[];

extern const char *const pstr_O_DNL_DNL_202_TEX;
static inline s32 b5_prog_flag8000(void);

extern void *Effect726E0_vtable[];   /* the strand (creature.c) */
static inline s32 b5_prog_flag8000(void);

static inline __attribute__((always_inline)) s32 creature_slot_done(Pursuer *p);
static inline __attribute__((always_inline)) void creature_act5(Pursuer *p, const PTMF *st);
static inline __attribute__((always_inline)) void creature_inplay(Pursuer *p);

extern VObject *gSceneGameF29740; /* path planner */
extern u8 D_0047A930[8];   /* 0..7 */
extern s32 Summoner_Offstage(u8 *o);
extern s32 Summoner_InPlay(u8 *o);
extern u8 D_0047AC90[];        /* per noise level: summon chance, hunted chance (percent) */
extern u32 D_00419DC0[];       /* the seconds before the pursuer can be summoned, by kind */
/* the summoner `o` takes the pursuer as it is now (when active: its room +0x8, its state +0,
   kind `kind`, waited 0); then progress slot refresh (Progress_CharDone) */
static inline void summoner_take(u8 *o, u8 kind) {
    u8 *pu = (u8 *)gCharPursuer;

    if (pu == NULL) {
        return;
    }
    if (AT(pu, 0x28, u8)) {
        AT(o, 0x8, s32) = AT(pu, 0x30, s32);
        AT(o, 0x0, u32) = Pursuer_RandomDelay((Pursuer *)pu) & 0x7FFFFFFF;
        AT(o, 0xC, s32) = 0;
        AT(o, 0x11, u8) = kind;
    }
    Progress_CharDone(gProgress, AT(pu, 0x20, u32));
}

/* bring the pursuer in for the summoner `o`: a random room out of the progress's list for the
 * current one (+0x3C) - `near` 0: one not next to it; else one next to it (once per exit
 * leading there) that the rooms allow (+0x88); either way not the current room and with its
 * progress bit clear - then the first of its exits (rooms +0x74) with a route from Fiona's
 * exit point to the exit's point (Character_RouteVia; next-door: route mode 1, the point not 0 / 1;
 * else mode 2); it comes in there with plan `plan` (and +0's summoned bit). 1 if it came */
static s32 summon_via(u8 *o, s32 near, s32 plan) {
    Progress *p = gProgress;
    VObject *rooms = gRooms;
    s32 cur = VCALL(p, 0xC, s32 (*)(Progress *))(p);
    s32 *list = msl_malloc(0x104);
    u32 n, m = 0, i, e;
    u8 *pu, *pl;
    s32 from, to = -1, ok = 0;

    if (list == NULL) {
        return 0;
    }
    n = VCALL(p, 0x3C, u32 (*)(Progress *, s32 *, s32))(p, list, cur);
    if (n == 0) {
        msl_free(list);
        return 0;
    }
    for (i = 0; i < n; i++) {
        s32 c = list[i];

        if (c == -1 || c == cur) {
            continue;
        }
        if (!near) {
            for (e = 0; e < 8; e++) {
                if (list[i] == VCALL(rooms, 0x18, s32 (*)(VObject *, s32, u32))(rooms, cur, e & 0xFF)) {
                    break;
                }
            }
            if (e == 8 && (u8)Progress_IsBitClear(p, list[i])) {
                list[m++] = list[i];
            }
        } else if ((u8)Progress_IsBitClear(p, c) && (u8)VCALL(rooms, 0x88, s32 (*)(VObject *, s32, s32))(rooms, list[i], cur)) {
            for (e = 0; e < 8; e++) {
                if (list[i] == VCALL(rooms, 0x18, s32 (*)(VObject *, s32, u32))(rooms, cur, e & 0xFF)) {
                    list[m++] = list[i];
                }
            }
        }
    }
    if (m == 0) {
        msl_free(list);
        return 0;
    }
    i = (u8)(u32)((f32)m * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom));
    pu = (u8 *)gCharPursuer;
    pl = (u8 *)gCharPlayer;
    from = VCALL(rooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(rooms, AT(pl, 0x30, s32), AT(pl, 0x14D4, u8), 1);
    for (e = 0; e < 8; e++) {
        if (VCALL(rooms, 0x74, s32 (*)(VObject *, s32, u32))(rooms, list[i], e & 0xFF) == 0) {
            continue;
        }
        to = VCALL(rooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(rooms, list[i], e & 0xFF, 1);
        if (Character_RouteVia((Character *)pu, AT(pl, 0x30, s32), list[i], AT(pu, 0x20, s32), 1, from, to, near ? 1 : 2) >= 0 &&
            (!near || (to != 0 && to != 1))) {
            ok = 1;
            break;
        }
    }
    if (ok) {
        Pursuer_IntoRoomByEvent((Pursuer *)pu, list[i], (AT(o, 0x0, u32) & 0x80000000) != 0, plan, to);
    }
    msl_free(list);
    return ok;
}

s32 Summoner_Via0(u8 *o);
s32 Summoner_Via1(u8 *o);
s32 Summoner_Via1B(u8 *o);
s32 Summoner_Offstage(u8 *o);
s32 Summoner_InPlay(u8 *o);

/* how far Fiona (+0x1588) and Hewie (+0x158C) are on foot, in the played room (-1 elsewhere) */
static f32 Npc_DistanceTo(Pursuer *p, Character *c, f32 *v) {
    void *nm;
    u32 tri;

    if (p->c.a.room != c->a.room) {
        return -1.0f;
    }
    nm = gNavMesh;
    tri = c->a.navTri;
    if (VCALL(nm, 0x10, s32 (*)(void *, u32, f32 *))(nm, tri, c->a.pos) == 4) {
        VCALL(nm, 0xC, void (*)(void *, u32, f32 *))(nm, tri, v);
        if (tri == Actor_TriTo(&p->c.a, v, p->c.a.navMask)) {
            return Actor_Distance(&p->c.a, c->a.pos);
        }
    }
    return VCALL(p, 0xD4, f32 (*)(Pursuer *, u32, f32 *))(p, tri, c->a.pos);
}

/* the path length still to walk (along the waypoints +0x12C, up to +0x124) */
static f32 Npc_PathLeft(Pursuer *p) {
    return VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(gSceneGameF29740,
        p->c.a.pos, p->c.unk128, p->c.unk124, (u8 *)p + 0x12C);
}

/* can the pursuer go `dist` further along its path: the point reached (unless it's the last
   waypoint itself) is on a triangle its nav mask allows */
static s32 Npc_PathClear(Pursuer *p, f32 dist) {
    f32 v[4] __attribute__((aligned(16)));
    u32 tri = p->c.a.navTri;
    s32 n;
    u32 flags;

    sceVu0CopyVector(v, p->c.a.pos);
    n = VCALL(gSceneGameF29740, 0x20, s32 (*)(VObject *, u32 *, f32 *, s32, s32, void *, f32))(gSceneGameF29740,
            &tri, v, p->c.unk128, p->c.unk124, (u8 *)p + 0x12C, dist);
    if (n == p->c.unk124) {
        f32 dx, dz;

        dx = v[0] - AT(p, 0x124 + n * 12, f32);
        if (dx <= 0.0f) {
            dx = -dx;
        }
        if (!(dx <= 0x1.99999ap-4f /* 0.1 */)) {
            return 0;
        }
        dz = v[2] - AT(p, 0x128 + n * 12, f32);
        if (dz <= 0.0f) {
            dz = -dz;
        }
        if (!(dz <= 0x1.99999ap-4f)) {
            return 0;
        }
    }
    if (tri < AT(gNavMesh, 0x8, u32) && AT(gNavMesh, 0x4, u8 *) != NULL) {
        flags = AT(AT(gNavMesh, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
    } else {
        flags = NAV_BAD_TRI_FLAGS;
    }
    return !(flags & p->c.a.navMask);
}

/* the senses, a frame on Fiona and a frame on Hewie in turn (+0x15A0), by the sense mode +0x15C0:
   0 watching Fiona (sight test vtable +0xE0; Hewie and the path left measured on the other
   frame), 1 the same with Hewie, 2/3 on the move (path blocked: vtable +0xD8/+0xDC);
   1 if the target is in view / reachable. `fionaRoom`: mode 0 only while Fiona is in the room */
static s32 Npc_Senses(Pursuer *p, s32 fionaRoom) {
    f32 v[4] __attribute__((aligned(16)));
    s32 room = p->c.a.room;
    s32 r = 0;

    if (room != VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        PU(p, 0x1590, f32) = -1.0f;
        PU(p, 0x158C, f32) = -1.0f;
        PU(p, 0x1588, f32) = -1.0f;
        return 0;
    }
    switch (PU(p, 0x15C0, u8)) {
    case 0:
        if (PU(p, 0x15A0, u8) != 0) {
            if (!fionaRoom || room == gCharPlayer->a.room) {
                r = VCALL(p, 0xE0, s32 (*)(Pursuer *, Character *))(p, gCharPlayer) & 0xFF;
            } else {
                PU(p, 0x1590, f32) = -1.0f;
                PU(p, 0x1588, f32) = -1.0f;
            }
        } else {
            PU(p, 0x158C, f32) = room == gCharPartner->a.room ? Npc_DistanceTo(p, gCharPartner, v) : -1.0f;
            if (!fionaRoom || p->c.a.room == gCharPlayer->a.room) {
                f32 d = Npc_PathLeft(p);

                PU(p, 0x1590, f32) = d;
                PU(p, 0x1588, f32) = d;
            } else {
                PU(p, 0x1590, f32) = -1.0f;
                PU(p, 0x1588, f32) = -1.0f;
            }
            r = !(PU(p, 0x158C, f32) <= 0.0f);
        }
        break;
    case 1:
        if (PU(p, 0x15A0, u8) != 0) {
            if (room == gCharPartner->a.room) {
                r = VCALL(p, 0xE0, s32 (*)(Pursuer *, Character *))(p, gCharPartner) & 0xFF;
            } else {
                PU(p, 0x1590, f32) = -1.0f;
                PU(p, 0x158C, f32) = -1.0f;
            }
        } else {
            PU(p, 0x1588, f32) = room == gCharPlayer->a.room ? Npc_DistanceTo(p, gCharPlayer, v) : -1.0f;
            if (p->c.a.room == gCharPartner->a.room) {
                f32 d = Npc_PathLeft(p);

                PU(p, 0x1590, f32) = d;
                PU(p, 0x158C, f32) = d;
            } else {
                PU(p, 0x1590, f32) = -1.0f;
                PU(p, 0x158C, f32) = -1.0f;
            }
            r = !(PU(p, 0x1588, f32) <= 0.0f);
        }
        break;
    case 2:
    case 3: {
        f32 vel[4] __attribute__((aligned(16)));
        s32 ok;

        if (PU(p, 0x15A0, u8) != 0) {
            PU(p, 0x1588, f32) = room == gCharPlayer->a.room ? Npc_DistanceTo(p, gCharPlayer, v) : -1.0f;
        } else {
            PU(p, 0x158C, f32) = room == gCharPartner->a.room ? Npc_DistanceTo(p, gCharPartner, v) : -1.0f;
        }
        Motion_RootMovement(p->c.motion, vel, 0.0f);
        vel[2] *= VCALL((VObject *)p->c.motion, 0x44, f32 (*)(void *, Pursuer *))(p->c.motion, p);
        if ((p->c.unk128 < p->c.unk124) == 1) {
            /* still walking: is the way ahead (30 units, then this frame's step) clear? */
            ok = Npc_PathClear(p, 30.0f);
            if (ok) {
                f32 step = __builtin_sqrtf(sceVu0InnerProduct(vel, vel));

                ok = step < 0.0f ? 0 : Npc_PathClear(p, step);
            }
            if (!ok) {
                if (PU(p, 0x15C0, u8) == 3) {
                    r = VCALL(p, 0xD8, s32 (*)(Pursuer *))(p) & 0xFF;
                } else if (PU(p, 0x15C4, s32) != -1) {
                    r = VCALL(p, 0xDC, s32 (*)(Pursuer *))(p) & 0xFF;
                } else {
                    r = 0;
                }
                break;
            }
        }
        {
            f32 d = Npc_PathLeft(p);

            PU(p, 0x1590, f32) = d;
            r = !(d < 0.0f);
        }
        break;
    }
    default:
        if (PU(p, 0x15A0, u8) != 0) {
            PU(p, 0x1588, f32) = room == gCharPlayer->a.room ? Npc_DistanceTo(p, gCharPlayer, v) : -1.0f;
        } else {
            PU(p, 0x158C, f32) = room == gCharPartner->a.room ? Npc_DistanceTo(p, gCharPartner, v) : -1.0f;
        }
        PU(p, 0x1590, f32) = p->target == gCharPlayer ? PU(p, 0x1588, f32) : PU(p, 0x158C, f32);
        break;
    }
    PU(p, 0x15A0, u8) = !(PU(p, 0x15A0, u8) != 0);
    return r;
}

static void strand_init(void **obj) {
    obj[0] = Effect726E0_vtable;
    obj[0x40 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0x44 / 4] = -1;
    obj[0x40 / 4] = QuadDrawer_vtable;
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

/* nothing */
/* 0x00283EE0 */
void Pursuer_AttackTable(Pursuer *p) {
}

/* a character's step (base) */
/* 0x002992F0 */
void Pursuer_StateRootStep(Pursuer *p) {
    Character_RootMove(&p->c);
}

/* chasing Hewie? */
/* 0x0029A850 */
s32 Pursuer_ChasingHewie(Pursuer *p) {
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
/* 0x0027E5A0 */
void Pursuer_SetGoalTri(Pursuer *p, u32 tri) {
    if (tri < AT(gNavMesh, 0x8, u32)) {
        PU(p, 0x179C, u32) = tri;
    }
}

/* clear the step counters */
/* 0x0029A6D0 */
void Pursuer_ClearSteps(Pursuer *p) {
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
/* 0x00292120 */
void Pursuer_StateStopAtEnd(Pursuer *p) {
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
    Character_RootMoveMasked(&p->c);
    p->c.unk104[0]--;
    if (p->c.unk104[0] <= 0) {
        PURSUER_STEP_DONE(p) = 1;
    }
}

/* step until the animation ends */
/* 0x00289F50 */
void Pursuer_StateStepToEnd(Pursuer *p) {
    Character_RootMoveMasked(&p->c);
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

/* 0x00291C30 */
void Pursuer_StateEndStep(Pursuer *p) {
    if ((MOTION_KEYS(p) & MOTION_KEY_END) != 0) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    } else {
        Character_RootMoveMasked(&p->c);
    }
}

/* 0x00285AB0 */
void Pursuer_StateCountKeys(Pursuer *p) {
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
    Character_RootMoveMasked(&p->c);
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

    if (Actor_PosInCurrentRoom(&p->c.a, pos) != 0) {
        ((void (*)(Character *, s32, f32 *))Character_Sound)(&p->c, 0, pos);
        ((void (*)(Character *, s32, f32 *))Character_Sound)(&p->c, 5, pos);
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
extern void *Helper469D00_vtable[];

/* destructor of a small object (vtables 0x46D800 -> 0x469D00) */
/* 0x00278490 */
void **SmallObj46D800_dtor(void **obj, s32 flags) {
    if (obj != NULL) {
        *obj = D_0046D800;
        if (obj != NULL) {
            *obj = Helper469D00_vtable;
        }
        if ((s16)flags > 0) {
            __dl__FPv(obj);
        }
    }
    return obj;
}

/* the entry of a {threshold, value} table (8 bytes each) whose threshold +0x1588 reaches */
/* 0x00297290 */
s32 Pursuer_ThresholdEntry(Pursuer *p, f32 *table, u32 n) {
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
/* 0x00299300 */
void Pursuer_StateRunToTri(Pursuer *p) {
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
/* 0x00283870 */
s32 Pursuer_MayGoForTarget(Pursuer *p) {
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
        Npc_DoorShutOther(p, (u8)p->c.unk100);
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
    Npc_WhoAroundEnding(p);
}

/* vtable +0x180 */
/* 0x002919B0 */
void Pursuer_Move180(Pursuer *p) {
    s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

    if (((over ^ 1) & 0xFF) == 1) {
        Character_RootMoveMasked(&p->c);
    } else if (p->c.unkE0 != 0) {
        p->c.unkE1 = 1;
    }
}

/* vtable +0x170 */
/* 0x0027D130 */
void Pursuer_Plan170(Pursuer *p) {
    if (Npc_InPlayedRoom(p) == 0) {
        if (PU(p, 0x17B4, s32) == 0 && PU(p, 0x1664, s32) == 0 && (Npc_RandomExit(p, 0xFF) & 0xFF) != 0xFF) {
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
        Pursuer_ThroughDoor(p, (u8)p->c.unk100);
    }
    p->c.unk100 = -1;
}

/* is `room` (-1: the current one) in the vtable +0x314 list ({room, ...} 8 bytes each, -1 ends)? */
/* 0x0029A8C0 */
s32 Pursuer_IsOwnRoom(Pursuer *p, s32 room) {
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
    PU(p, 0x1574, f32) = Angle_Wrap(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
    m = p->c.motion;
    VCALL(m, 0x54, void (*)(void *))(m);
    MOTION_AT(p, 0x850, u8) = 1;
    return r;
}

/* run the state; then, a step done, the next behaviour (+0x114) */
/* 0x002994B0 */
void Pursuer_StateRunThenNext(Pursuer *p) {
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
/* 0x0029F2C0 */
void Pursuer_ReleaseModelFiles(Pursuer *p) {
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

extern const PTMF Pursuer_StateCloseA_ptmf, Pursuer_StateCloseB_ptmf, Pursuer_StateGrabStart_ptmf, Pursuer_StateTauntStart_ptmf, D_003ED530, D_003ED5A0, D_003ECF20,
    Pursuer_StateTaunt_ptmf, Pursuer_StateStepToEnd_ptmf, D_003ECF80, D_003ECE80, D_003ED590, Pursuer_StateTauntRepeat_ptmf, D_003ECF30, D_003ECD30,
    Pursuer_StateHewieHolds_ptmf, D_003ECF40, D_003ECCB0, D_003ECC70, D_003ECB90;

/* 0x0028A100 */
void Pursuer_StateRelation1(Pursuer *p) {
    Relation_Request(gProgress, *(u8 *)&p->c.a.slot, 1, 9, 0, 8, 0.0f);
    Actor_SetState(&p->c.a, &Pursuer_StateCloseA_ptmf);
}

/* 0x0028AA80 */
void Pursuer_StateRelation2(Pursuer *p) {
    Relation_Request(gProgress, *(u8 *)&p->c.a.slot, 1, 9, 0, 6, 0.0f);
    Actor_SetState(&p->c.a, &Pursuer_StateCloseB_ptmf);
}

/* 0x0028A060 */
void Pursuer_StateCloseA(Pursuer *p) {
    if (p->c.state[0] != 7) {
        p->c.state[0] = 0;
        Actor_SetState(&p->c.a, &Pursuer_StateGrabStart_ptmf);
        return;
    }
    p->c.state[0] = 0;
    VCALL(p, 0x134, void (*)(Pursuer *))(p);
}

/* 0x0028A9E0 */
void Pursuer_StateCloseB(Pursuer *p) {
    if (p->c.state[0] != 7) {
        p->c.state[0] = 0;
        Actor_SetState(&p->c.a, &Pursuer_StateTauntStart_ptmf);
        return;
    }
    p->c.state[0] = 0;
    VCALL(p, 0x134, void (*)(Pursuer *))(p);
}

/* +0x16E0 plus a random 0..3600 frames */
/* 0x0029CE50 */
s32 Pursuer_RandomDelay(Pursuer *p) {
    f32 r = 3600.0f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);

    return PU(p, 0x16E0, s32) + (u32)r;
}

/* back on its feet: at least 1 hp, chase state reset */
/* 0x002860D0 */
void Pursuer_StateBackOnFeet(Pursuer *p) {
    if (p->c.hp <= 0) {
        p->c.hp = 1;
    }
    if (gCharPartner->a.unkC4 == 2) {
        Pursuer_ChanceRoll(p);
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
/* 0x0028A660 */
void Pursuer_StateTauntHold(Pursuer *p) {
    Character_RootMoveMasked(&p->c);
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
/* 0x00289500 */
void Pursuer_StateStepFacing(Pursuer *p) {
    f32 h = Actor_HeadingTo(&p->c.a, p->target->a.pos);
    f32 d = Npc_TurnToward(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));

    if (d <= 0.0f) {
        d = -d;
    }
    if (d < 0x1.921fb60000000p-1f /* 0.7853982 */) {
        Character_RootMoveMasked(&p->c);
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
        Pursuer_ThroughDoor(p, (u8)p->c.unk100);
    } else {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x20);
    }
    p->c.unk100 = -1;
}

/* vtable +0x1EC: open the door the pursuer heads for, unless it already is */
/* 0x0028D8A0 */
void Pursuer_ExitOpen(Pursuer *p) {
    Npc_WalkPathStride(p, 0);
    if (!(PursuerGroup_Fields(gProgress, p->c.door, *(u8 *)&p->c.a.slot) & 0xFF & 1)) {
        p->c.unk100 = p->c.door;
        Actor_SetState(&p->c.a, &D_003ECF20);
        VCALL(p, 0x1AC, void (*)(Pursuer *))(p);
    }
}

/* a sound at the pursuer, unless a room object or progress flag 8 holds them */
/* 0x0029D410 */
void Pursuer_Sound(Pursuer *p, s32 sound, s32 a2, s32 a3, s32 a4, void *a5) {
    if (VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(gProgress, 8) == 0) {
        Actor_PlaySound(&p->c.a, sound, a2, a3, a4, a5);
    }
}

/* raise the threat level when someone it could reach is in sight */
/* 0x002837C0 */
void Pursuer_RaiseThreat(Pursuer *p, u32 mask) {
    Progress *pr = gProgress;

    if (!(Progress_TestFlag(pr, 0xE) & 0xFF)) {
        u32 seen = Npc_WhoSeen(p) & 0xFF & (mask & 0xFF);

        if (~PU(p, 0x1760, u8) & seen) {
            Relation_Request(pr, *(u8 *)&p->c.a.slot, seen & 0xFF, 4, 5, 0, 10.0f);
        }
    }
}

/* 0x0028A930 */
void Pursuer_StateTauntStart(Pursuer *p) {
    if (p->c.state[0] != 7) {
        p->c.state[0] = 0;
        Motion_PlayOwnBlend(p->c.motion, 0x1900, -1);
        p->c.moveSub = 0x19;
        Actor_SetState(&p->c.a, &Pursuer_StateTaunt_ptmf);
        return;
    }
    p->c.state[0] = 0;
    VCALL(p, 0x134, void (*)(Pursuer *))(p);
}

/* 0x00289FA0 */
void Pursuer_StateGrabStart(Pursuer *p) {
    if (p->c.state[0] != 7) {
        Motion_PlayOwnBlend(p->c.motion, 0x1A01, -1);
        p->c.a.unk2D = 1;
        p->c.moveSub = 0x19;
        Actor_SetState(&p->c.a, &Pursuer_StateStepToEnd_ptmf);
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
    PU(p, 0x1634, f32) = Npc_DoorFacingFromFiona(p, (u8)p->c.unk100);
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
/* 0x0029EF80 */
void Pursuer_LoadMotions(Pursuer *p, s32 id) {
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
/* 0x0028A860 */
void Pursuer_StateTaunt(Pursuer *p) {
    u8 *m = p->c.motion;

    if (MOTION_KEYS(p) & MOTION_KEY_END) {
        Motion_Play(m, 0x1901, -1);
        Actor_SetState(&p->c.a, &Pursuer_StateTauntRepeat_ptmf);
        Panic_Fright((u8 *)gProgress + 0x7B8, VCALL(p, 0x300, f32 (*)(Pursuer *))(p));
        return;
    }
    Character_RootMoveMasked(&p->c);
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
    Motion_Unfreeze(p->c.motion);
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
        Motion_Play(p->c.motion, 0, -1);
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

/* the chase towards the target: by path, or straight at it once on its triangle */
/* 0x00290810 */
void Pursuer_ChaseTarget(Pursuer *p) {
    if (PU(p, 0x1590, f32) < 0.0f) {
        VCALL(p, 0xB4, void (*)(Pursuer *, Character *))(p, p->target);
        if (p->c.a.room == p->target->a.room && !(Pursuer_TargetOutOfReach(p) & 0xFF)) {
            PU(p, 0x16EF, u8) = 1;
        }
    } else {
        Character *t = p->target;
        u32 tri = t->a.navTri;

        if (Actor_TriTo(&p->c.a, t->a.pos, -1) != tri) {
            s32 n = p->c.unk128;

            if ((n < p->c.unk124) == 1) {
                Npc_WalkPathStride(p, n);
            }
        } else {
            Npc_StepToward(p, p->target->a.pos);
        }
    }
}

/* vtable +0x60: reset all behaviour state */
/* 0x0029E440 */
void Pursuer_ResetBehaviour(Pursuer *p) {
    if (Npc_InPlayedRoom(p) != 0 && p->c.moveMode == 2) {
        Npc_LeaveDoor(p, 0xFF);
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
    Pursuer_ClearSteps(p);
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
/* 0x0029F040 */
void Pursuer_GiveMotionBanks(Pursuer *p, u32 slot) {
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
/* 0x00286AA0 */
void Pursuer_StateTurnAnim(Pursuer *p) {
    s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

    if (((over ^ 1) & 0xFF) != 1) {
        f32 h = PU(p, 0x1634, f32);

        p->c.a.angle[1] = h;
        sceVu0UnitMatrix(p->c.a.rot);
        sceVu0RotMatrixY(p->c.a.rot, p->c.a.rot, h);
        PU(p, 0x1634, f32) = 1.0f;
        PU(p, 0x1638, s32) = 0;
        Actor_SetState(&p->c.a, &Pursuer_StateHewieHolds_ptmf);
    } else {
        Actor_TurnToward(&p->c.a, PU(p, 0x1634, f32), PU(p, 0x1638, f32));
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

extern void *Kind22_vtable[], *Kind21_vtable[], *Pursuer_vtable[], *NPC_vtable[], *Character_vtable[], *Actor_vtable[];
extern const PTMF Pursuer_StateHitOver_ptmf, Pursuer_StateHitReaction_ptmf, Pursuer_StateCloseOnFionaA_ptmf, D_003ECE20, D_003ED510, D_003ED520, D_003ECDA0,
    D_003ECF00;

/* vtable +0x40 (via +0x84 first): blocking flags (8 = held at a door), then +0x110 / +0x40 */
/* 0x0029E390 */
void Pursuer_Update(Pursuer *p) {
    VCALL(p, 0x84, void (*)(Pursuer *))(p);
    p->c.a.navMask = p->c.a.unk2B == 1 ? 8 : VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
    p->c.pathReq->mask = p->c.a.navMask;
    if (Npc_InPlayedRoom(p) != 0) {
        Pursuer_MotionGroup(p);
        VCALL(p, 0x110, void (*)(Pursuer *))(p);
    }
    VCALL(p, 0x40, void (*)(Pursuer *))(p);
}

/* the target out of reach on the current triangle: search (0x12) or wait at a door (0xB) */
/* 0x00284440 */
s32 Pursuer_TargetOutOfReach(Pursuer *p) {
    if (PU(p, 0x15A4, s32) == -1) {
        return 0;
    }
    if ((Npc_RoundThroughDoor(p, PU(p, 0x15A4, s32)) & 0xFF) == 1) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x12);
        return 1;
    }
    if ((Npc_ExitsToTri(p, PU(p, 0x15A4, s32)) & 0xFF) == 1) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0xB);
        return 1;
    }
    if (AT(gNavMesh, 0x14, u32) >= 2 && (Npc_FindDoor(p, -1) & 0xFF) == 1) {
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x12);
        return 1;
    }
    return 0;
}

/* a cry heard (gProgress +0x1020 per slot): hold for (+0x1748)[0] frames (90 without), with
 * its sound +0x1764 and reaction animation */
/* 0x0029B4B0 */
u32 Pursuer_CryHeard(Pursuer *p) {
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
                Actor_PlaySound(&p->c.a, snd, 7, 0, 0, NULL);
            }
            p->c.unk14D0 = 5;
            Motion_Freeze(p->c.motion);
        }
    }
    PU(p, 0x1764, s32) = -1;
    return heard;
}

/* the model takes the motion banks of its file +0x1670; the message image +0x1674 */
/* 0x0029EE70 */
void Pursuer_ModelTakeBanks(Pursuer *p) {
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
/* 0x00280090 */
void Pursuer_StanceStep(Pursuer *p) {
    u8 k;

    Npc_Senses2Ending(p);
    Pursuer_StanceByFiona(p);
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
        if (Npc_ReachedRoom(p) != 0) {
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
/* 0x00297B40 */
s32 Pursuer_PlayAnimIf(Pursuer *p, s32 anim, s32 blend) {
    u8 *m = p->c.motion;

    if (anim == AT(m, 0x55C, s32)) {
        s32 over = AT(m, 0x550, f32) <= 0.0f;

        if ((over ^ 1) & 0xFF) {
            return 0;
        }
        if (((AT(AT(m, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0) != 1) {
            s32 i = Motion_AnimIndex(m, anim);
            u16 fl = i != -1 ? AT(AT(m, 0x874, u8 *) + i * 6, 0x4, u16) : 0;

            if (fl & 4) {
                return 0;
            }
        }
        Motion_PlayTable(p->c.motion, anim, -1);
        return 1;
    }
    if (blend == 0) {
        Motion_PlayTable(m, anim, -1);
    } else {
        Motion_Play(m, anim, -1);
    }
    return 1;
}

/* hit reaction by the animation being played (inlined in Pursuer_StateHurt) */
static void Pursuer_HitReact(Pursuer *p) {
    if (p->c.a.unkC4 != 2 && MOTION_ANIM(p) == 0x1709) {
        PU(p, 0x1624, s32) = 0x1806;
    } else {
        PU(p, 0x1624, s32) = MOTION_ANIM(p);
    }
    PU(p, 0x1628, s32) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (PU(p, 0x1624, s32) != 0x1807 && PU(p, 0x1624, s32) != 0x1803) {
        Actor_SetState(&p->c.a, &Pursuer_StateHitReaction_ptmf);
        Pursuer_StateHitReaction(p);
    } else {
        Actor_SetState(&p->c.a, &Pursuer_StateHitOver_ptmf);
        Pursuer_StateHitOver(p);
    }
}

/* 0x00288030 */
void Pursuer_StateHitReact(Pursuer *p) {
    Pursuer_HitReact(p);
}

/* 0x0028A540 */
void Pursuer_StateWalkOn(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    p->target = gCharPlayer;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &Pursuer_StateCloseOnFionaA_ptmf);
    Pursuer_StateCloseOnFionaA(p);
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
    Pursuer_TurnOnSpot(p, (u8)p->c.unk104[0]);
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
            if (PU(p, 0x1728, s32) >= 0 && !(Motion_EventFlags(p->c.motion, 0, -2, 1) & 0xFF & 2)) {
                return 0;
            }
            sceVu0CopyVector(out, (f32 *)((u8 *)p + 0x1770));
            return 1;
        }
        return 0;
    }
    if (!(Motion_EventFlags(p->c.motion, 0, -2, 1) & 0xFF & 2)) {
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
                    s32 i = Motion_AnimIndex(m, 0);
                    u16 fl = i != -1 ? AT(AT(m, 0x874, u8 *) + i * 6, 0x4, u16) : 0;

                    if (fl & 4) {
                        return;
                    }
                }
                Motion_PlayTable(p->c.motion, 0, -1);
            }
        } else {
            Motion_PlayTable(m, 0, -1);
        }
    } else {
        Character_RootMoveMasked(&p->c);
    }
}

/* the chance roll, rising each time it fails: true when 100 * random <= the chance
 * (table +0x1740 by situation: {chance, rise}) plus what built up (+0x16C0) */
/* 0x00297160 */
s32 Pursuer_ChanceRoll(Pursuer *p) {
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
        Motion_Update(p->c.motion);
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

/* ---- batch 5 ---- */

extern const PTMF D_003ECF50, D_003ECFC0, D_003ECF10, D_003ECF90, D_003ECCD0, Pursuer_StateTauntHold_ptmf, Pursuer_StateTauntHold_ptmf2,
    D_003ECDB0;
extern void *Pursuer_vtable[];

/* vtable +0x8: destructor (Pursuer 0x46D810 -> NPC 0x46C220 -> Character); the model is
 * freed for slots 3..5 */
/* 0x00172810 */
Pursuer *Pursuer_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        Pursuer_DestroyBase(p);
        if ((s16)flags > 0) {
            Actor_Destroy(&p->c.a);
        }
    }
    return p;
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
    Npc_DoorShut(p, (u8)p->c.unk100);
    VCALL(d, 0xC, void (*)(VObject *, u32, u32, s32, s32))(d, (u8)p->c.unk100, side, p->c.a.slot, 1);
    Actor_SetState(&p->c.a, &D_003ECF50);
}

/* arrive at the goal (+0x15A4 / +0x15B0): snap onto it, or turn to the heading +0x10C */
/* 0x00299370 */
void Pursuer_StateArrive(Pursuer *p) {
    if (PURSUER_STEP_DONE(p) == 1) {
        s32 tri = PU(p, 0x15A4, s32);

        if ((s32)p->c.a.navTri != tri) {
            f32 d = Character_PathLength(&p->c, tri, (f32 *)((u8 *)p + 0x15B0), -1);

            if (d < 0x1.99999a0000000p-4f /* 0.1 */ && !(d < 0.0f)) {
                p->c.a.navTri = PU(p, 0x15A4, s32);
                sceVu0CopyVector(p->c.a.pos, (f32 *)((u8 *)p + 0x15B0));
                return;
            }
            PURSUER_STEP_DONE(p) = 0;
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 4);
        } else if (Npc_TurnToward(p, AT(p, 0x10C, f32), VCALL(p, 0xA0, f32 (*)(Pursuer *))(p)) == 0.0f) {
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

        if (Pursuer_TargetOutOfReach(p) & 0xFF) {
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
        done = Npc_WalkPathStride(p, p->c.unk128) & 0xFF;
    }
    if (done == 1) {
        PURSUER_STEP_DONE(p) = 1;
    }
}

/* turned to the door (+0x1568): open it with the arm (animation 0x700 / 0x704 by +0x104) */
/* 0x0028C420 */
void Pursuer_DoorArmOpen(Pursuer *p) {
    u8 *m;

    if (Npc_TurnToward(p, PU(p, 0x1568, f32), VCALL(p, 0xA0, f32 (*)(Pursuer *))(p)) != 0.0f) {
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
                Motion_PlayOwnBlend(p->c.motion, 0x700, -1);
            } else {
                Motion_PlayOwnBlend(p->c.motion, 0x704, -1);
            }
            Actor_SetState(&p->c.a, &D_003ECFC0);
        }
    }
}

/* how is Fiona doing (for the chase): 0 calm, 1 in move mode 0xB, 2 panicking, 3 out of play
 * or in move 0xA / 0xB, 4 / 5 by the threat stage (gProgress +0x7B8) */
/* 0x00283EF0 */
s32 Pursuer_FionaState(Pursuer *p) {
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
                return Npc_FionaPanicking() == 0 ? 0 : 2;
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
        Npc_LeaveDoor(p, 0xFF);
    }
    if (PU(p, 0x16C8, u8) == 4) {
        if (VCALL(p, 0xC0, s32 (*)(Pursuer *))(p) != 0) {
            PU(p, 0x16C8, u8) = 0;
            VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
            PU(p, 0x16C9, u8) = 6;
            PU(p, 0x16CA, u8) = 7;
        } else {
            PU(p, 0x16C8, u8) = 2;
            Pursuer_SearchRoom(p);
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
        vu0_ScaleXYZ(v, v, 3.0f);
        Actor_Move(&p->c.a, v);
    }
    if ((Progress_CurRoomFlag(gProgress, p->c.a.room, (u8)p->c.unk100) & 0xFF) != 1) {
        Actor_SetState(&p->c.a, &D_003ECF90);
        VCALL(p, 0x214, void (*)(Pursuer *))(p);
    } else {
        Character_RootMoveMasked(&p->c);
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
    if (Npc_ReachedRoom(p) != 0 && !(Pursuer_PlanWhere(p) & 0xFF)) {
        PURSUER_STEP_NEXT(p) = 1;
        Pursuer_ChaseFionaHere(p);
        return;
    }
    PU(p, 0x162C, s32) = 0;
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECCD0);
    PU(p, 0x1758, s32) = -1;
    VCALL(p, 0x284, void (*)(Pursuer *))(p);
}

/* repeated taunt: threat up each time the animation ends, 6 times, then animation 0x1904 */
/* 0x0028A700 */
void Pursuer_StateTauntRepeat(Pursuer *p) {
    Character_RootMoveMasked(&p->c);
    if (MOTION_KEYS(p) & MOTION_KEY_END) {
        Panic_Fright((u8 *)gProgress + 0x7B8, VCALL(p, 0x304, f32 (*)(Pursuer *))(p));
        p->c.unk104[0]++;
        if (p->c.state[0] != 7) {
            if (p->c.unk104[0] >= 6) {
                Motion_Play(p->c.motion, 0x1904, -1);
                Actor_SetState(&p->c.a, &Pursuer_StateTauntHold_ptmf2);
                return;
            }
            Motion_Play(p->c.motion, 0x1901, -1);
        } else {
            p->c.state[0] = 0;
            Motion_Play(p->c.motion, 0x1903, -1);
            Actor_SetState(&p->c.a, &Pursuer_StateTauntHold_ptmf);
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
        if (!(Pursuer_TargetOutOfReach(p) & 0xFF)) {
            PU(p, 0x16EF, u8) = 1;
        }
        Character_RootMoveMasked(&p->c);
        return;
    }
    PU(p, 0x1624, s32) = -1;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ECDB0);
    VCALL(p, 0x198, void (*)(Pursuer *))(p);
}

/* ---- batch 6 ---- */

extern const PTMF D_003ED360, Pursuer_StateEndStep_ptmf3, D_003ED2A0, Pursuer_StateKnockedThrough_ptmf, D_003ECF70, Pursuer_StateStopAtEnd_ptmf, D_003ECB80,
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
/* 0x002838E0 */
f32 Pursuer_GroundGained(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));
    f32 gain = 0.0f;
    u32 i;

    for (i = 0; i < 5; i = (i + 1) & 0xFF) {
        Motion_RootMovement(p->c.motion, v, (f32)i);
        gain += v[2];
        if (!(Eye_ActorSees(p, &p->target->a, &p->c.a, p->target->a.angle[1], 150.0f, 0x1.921fb60000000p+0f /* 1.5707964 */) & 0xFF)) {
            Motion_RootMovement(p->target->motion, v, (f32)i);
            gain -= v[2];
        }
    }
    if (AT(gProgress, 0x7B8, u8) == 5 && p->target == gCharPlayer) {
        return 10.0f + gain;
    }
    return gain + VCALL(p, 0x2F0, f32 (*)(Pursuer *))(p);
}

/* keep standing on walkable ground (step off triangles that block the pursuer) */
/* 0x0029E210 */
void Pursuer_KeepOnWalkable(Pursuer *p) {
    if (Npc_TriBlocked(p, p->c.a.navTri) != 0 && p->c.a.unk2B == 0 && Npc_InPlayedRoom(p) != 0) {
        p->c.a.navTri = Npc_TriIfStandable(p, p->c.a.navTri);
        if (p->c.a.navTri != (u32)-1 && !(Npc_TriBlocked(p, p->c.a.navTri) & 0xFF)) {
            VCALL(gNavMesh, 0xC, void (*)(void *, u32, f32 *))(gNavMesh, p->c.a.navTri, p->c.a.pos);
        } else {
            Actor_TeleportRandom(&p->c.a, -1);
        }
    }
    if (p->c.moveMode == 0) {
        void *nm = gNavMesh;

        if (VCALL(nm, 0x10, s32 (*)(void *, u32, f32 *))(nm, p->c.a.navTri, p->c.a.pos) == 4) {
            if (Npc_TriBlocked(p, p->c.a.navTri) & 0xFF) {
                p->c.a.navTri = Npc_TriIfStandable(p, p->c.a.navTri);
                if (p->c.a.navTri == (u32)-1 || Npc_TriBlocked(p, p->c.a.navTri) != 0) {
                    Actor_TeleportRandom(&p->c.a, -1);
                }
            } else {
                VCALL(nm, 0xC, void (*)(void *, u32, f32 *))(nm, p->c.a.navTri, p->c.a.pos);
            }
        }
    }
}

/* hit reaction over: back to the stand animation */
/* 0x00287B50 */
void Pursuer_StateHitOver(Pursuer *p) {
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
    Npc_TurnToRootMotion(p, -1);
    Character_RootMoveMasked(&p->c);
}

/* vtable +0x...: look around (animation 0x1A00) */
/* 0x00289DD0 */
void Pursuer_StateLookAround(Pursuer *p) {
    u8 *m;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    m = p->c.motion;
    if (AT(m, 0x55C, s32) != 0x1A00) {
        Motion_PlayOwnBlend(m, 0x1A00, -1);
    }
    VCALL(p, 0x12C, void (*)(Pursuer *, f32))(p, VCALL(p, 0x308, f32 (*)(Pursuer *))(p));
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &Pursuer_StateEndStep_ptmf3);
    if (((MOTION_KEYS(p) & MOTION_KEY_END) != 0) == 1) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    } else {
        Character_RootMoveMasked(&p->c);
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
        if (!(Pursuer_TargetOutOfReach(p) & 0xFF)) {
            PU(p, 0x16EF, u8) = 1;
        }
        return;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &D_003ED2A0);
    VCALL(p, 0x198, void (*)(Pursuer *))(p);
}

/* plan where to go by the stance +0x16C8: to Fiona's room, through a random other exit, ... */
/* 0x0027CA00 */
s32 Pursuer_PlanWhere(Pursuer *p) {
    switch (PU(p, 0x16C8, u8)) {
    case 0:
        PU(p, 0x1594, s32) = gCharPlayer->a.room;
        PU(p, 0x1598, s32) = Npc_CharSideBehind(p, gCharPlayer);
        /* fallthrough */
    case 1:
    case 2:
        return Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) > 0;
    case 3: {
        s32 again = 0;
        VObject *rm;
        u32 exit;

        for (;;) {
            exit = Npc_RandomExit(p, 0xFF) & 0xFF;
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
        if (Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) > 0) {
            return 1;
        }
        PU(p, 0x1594, s32) = VCALL(rm, 0x18, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, p->c.door);
        return Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) > 0;
    }
    case 4:
        return VCALL(p, 0xE8, s32 (*)(Pursuer *))(p);
    default:
        return 0;
    }
}

/* vtable +0x1C: the model files loaded: give the model its motion banks, message image, door
 * sounds and +0x4D4 (slot 2; others Pursuer_ModelTakeBanks) */
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
    Pursuer_ModelTakeBanks(p);
}

/* vtable +0x...: the door ahead: stand back if it's in the way, else turn away from it */
/* 0x002871E0 */
void Pursuer_StateDoorAhead(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));
    Progress *pr;
    u32 st;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    pr = gProgress;
    st = PursuerGroup_Fields(pr, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) & 0xFF;
    VCALL(gDoors, 0x38, void (*)(VObject *, u32, f32 *))(gDoors, (u8)p->c.unk100, v);
    if ((Actor_Distance(&p->c.a, v) < 9.0f || (st & 0xFF & 9)) &&
        Progress_ExitPassable(pr, p->c.a.room, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) != 0) {
        PU(p, 0x1634, f32) = Npc_DoorFacingFromFiona(p, (u8)p->c.unk100);
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
        PU(p, 0x1634, f32) = Angle_Wrap(0x1.921fb60000000p+1f /* 3.1415927 */ + Npc_DoorFacingFromFiona(p, (u8)p->c.unk100));
        p->c.unk100 = 0xFF;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &Pursuer_StateKnockedThrough_ptmf);
    Pursuer_StateKnockedThrough(p);
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
        vu0_ScaleXYZ(v, v, 3.0f);
        Actor_Move(&p->c.a, v);
    }
    h = Angle_Wrap(0x1.921fb60000000p+1f /* 3.1415927 */ + PU(p, 0x1634, f32));
    Npc_TurnToward(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    if ((Progress_CurRoomFlag(gProgress, p->c.a.room, (u8)p->c.unk100) & 0xFF) != 1) {
        Actor_SetState(&p->c.a, &D_003ECF70);
        VCALL(p, 0x208, void (*)(Pursuer *))(p);
    } else {
        Character_RootMoveMasked(&p->c);
    }
}

/* vtable +0x...: stand (vtable +0x320 animation) */
/* 0x00292170 */
void Pursuer_StateStand(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &Pursuer_StateStopAtEnd_ptmf);
    if (p->c.unkE0 != 0) {
        s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

        if (!((over ^ 1) & 0xFF)) {
            p->c.unkE1 = 1;
        }
    }
}

/* vtable +0x260: a door near the pursuer it should go through (not during behaviours 0x10,
 * 0x11, 0x21, 0x22) */
/* 0x00296FC0 */
void Pursuer_DoorNear(Pursuer *p) {
    s32 b = PU(p, 0x175C, s32);

    if (b != 0x22 && b != 0x11 && b != 0x21 && b != 0x10 && p->c.moveMode != 2) {
        u32 door = Npc_DoorOnWay(p) & 0xFF;

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
/* 0x00299140 */
void Pursuer_StateTurnThenWait(Pursuer *p) {
    if (Npc_TurnToward(p, AT(p, 0x10C, f32), VCALL(p, 0xA0, f32 (*)(Pursuer *))(p)) == 0.0f) {
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

    if (Npc_InPlayedRoom(p) != 0) {
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
    Pursuer_HeadLook(p);
    Motion_Hands(p->c.motion);
    Motion_Eyes(p->c.motion);
    Motion_Update(p->c.motion);
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
        Npc_LeaveDoor(p, 0xFF);
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
    Pursuer_ClearSteps(p);
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
        u32 seen = Npc_WhoSeen(p) & 0xFF;

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
            p->c.a.navTri = Actor_DoorFront(p, p->c.unk100, 0, ofs, p->c.a.pos);
            break;
        case 0x707:
            VCALL(p, 0x9C, void (*)(Pursuer *, u32, f32 *))(p, 2, ofs);
            p->c.a.navTri = Actor_DoorFront(p, p->c.unk100, 1, ofs, p->c.a.pos);
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

        Motion_RootMovement(m, v, 0.0f);
        Character_RootTurn(&p->c);
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
        if (!(Progress_ExitOpen(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF)) {
            goto fail;
        }
        break;
    case 0:
    case 2:
        if (Progress_ExitOpen(pr, p->c.a.room, (u8)p->c.unk100) != 0) {
            goto fail;
        }
        break;
    default:
        goto fail;
    }
    if (!(Npc_WalkPathStride(p, p->c.unk128) & 0xFF)) {
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
    Pursuer_StateDownAndUp_ptmf2, Pursuer_StateHitReact_ptmf4, D_003ECEC0, D_003ECE10, D_003ECD90, Pursuer_StateCloseOnFionaB_ptmf, D_003ED280, D_003ED290,
    D_003ECED0;

/* add `n` stops to the search route: 60% the room's next point of interest (+0x1738 counts them
 * through), otherwise a random walkable triangle (+0x15DC of the entry: 1) */
/* 0x0027E5D0 */
void Pursuer_AddSearchStops(Pursuer *p, s32 n) {
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
                Npc_RouteAdd(p, pts[k]);
                used = (used + 1) & 0xFF;
                PU(p, 0x15DC + PU(p, 0x1621, u8) * 8, u8) = 0;
            } else {
                Npc_RouteAdd(p, Npc_RandomTri(p));
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
        Character_RootMoveMasked(&p->c);
        return;
    }
    if (p->c.moveSub == 0x15) {
        Npc_DoorShutOther(p, (u8)p->c.unk100);
    } else {
        Npc_DoorRelease(p, (u8)p->c.unk100);
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
            Motion_Play(m, 0, -1);
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
        u32 t = Npc_TurnWay(p, PU(p, 0x1568, f32), 0x1.0c1524p+0f /* 60 degrees */, 0x1.4f1a6ep+1f /* 150 degrees */) & 0xFF;

        if (t == 0xFF) {
            Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
        } else {
            Pursuer_TurnOnSpot(p, t);
            PU(p, 0x1624, s32) = t & 0xFF;
        }
        Pursuer_MotionGroup(p);
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
            Npc_RouteAim(p);
        } else {
            PU(p, 0x15A4, s32) = Npc_RandomTri(p);
            VCALL(nm, 0xC, void (*)(void *, s32, f32 *))(nm, PU(p, 0x15A4, s32), (f32 *)((u8 *)p + 0x15B0));
            Npc_RouteAdd(p, PU(p, 0x15A4, s32));
            PU(p, 0x15DC + PU(p, 0x1621, u8) * 8, u8) = 1;
        }
        if (VCALL(p, 0xD8, s32 (*)(Pursuer *))(p) != 0) {
            break;
        }
        if (Pursuer_TargetOutOfReach(p) != 0) {
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
        if (Npc_ReachedRoom(p) & 0xFF) {
            if (!(VCALL(p, 0xD8, s32 (*)(Pursuer *))(p) & 0xFF)) {
                if (!(Pursuer_TargetOutOfReach(p) & 0xFF)) {
                    PU(p, 0x16EF, u8) = 1;
                }
                return;
            }
        } else {
            VObject *rm = gRooms;

            PU(p, 0x17B0, u8) = VCALL(rm, 0x3C, u32 (*)(VObject *, u32, s32))(rm, PU(p, 0x138C + p->c.unk1388 * 2, u16), p->c.a.room);
            PU(p, 0x15A4, s32) = VCALL(rm, 0x34, s32 (*)(VObject *, u32, f32 *))(rm, PU(p, 0x17B0, u8), (f32 *)((u8 *)p + 0x15B0));
            if (!(VCALL(p, 0xD8, s32 (*)(Pursuer *))(p) & 0xFF)) {
                if (!(Pursuer_TargetOutOfReach(p) & 0xFF)) {
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
    Pursuer_GoForFionaStance0(p);
    VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1D);
}

/* the search while out of sight: in the played room, catch up the route by the time spent away
 * (one stop per 150 frames); away, count the time the route still takes */
/* 0x0027EEA0 */
void Pursuer_SearchOffscreen(Pursuer *p) {
    if (Npc_InPlayedRoom(p) != 0) {
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
                Pursuer_AddSearchStops(p, (steps - left) & 0xFF);
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
/* 0x00287950 */
void Pursuer_StateHurt(Pursuer *p) {
    s32 a;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    AT(gProgress, 0x874, u8) = 0;
    PU(p, 0x1784, s32) = 0;
    a = MOTION_ANIM(p);
    if (a != 0x1804 && a != 0x1800 && a != 0x1709) {
        Actor_SetState(&p->c.a, &Pursuer_StateHitReact_ptmf4);
        Pursuer_HitReact(p);
    } else {
        Actor_SetState(&p->c.a, &Pursuer_StateDownAndUp_ptmf2);
        Pursuer_StateDownAndUp(p);
    }
}

/* vtable +0x1B8: facing the door; open it (side +0x104) if it is ours to open */
/* 0x0028E2D0 */
void Pursuer_DoorFacing(Pursuer *p) {
    if (Npc_TurnToward(p, PU(p, 0x1568, f32), VCALL(p, 0xA0, f32 (*)(Pursuer *))(p)) == 0.0f) {
        Progress *pr = gProgress;

        if ((Progress_CurRoomFlag(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1 &&
            (PursuerGroup_Fields(pr, (u8)p->c.unk100, 0) & 0xFF & 4)) {
            Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
        } else {
            if (Progress_ExitOpen(pr, p->c.a.room, (u8)p->c.unk100) != 0) {
                PURSUER_STEP_DONE(p) = 1;
                return;
            }
            PURSUER_STEP_NEXT(p) = 0;
            Actor_SetState(&p->c.a, &D_003ECEC0);
        }
    }
}

/* stance by where Fiona is (vtable +0xC0 sees her -> 0 chase; a different side -> 1) */
/* 0x0027FE90 */
void Pursuer_StanceByFiona(Pursuer *p) {
    if (AT(gProgress, 0x1FBEC1, u8) != 0) {
        if (PU(p, 0x16C8, u8) == 4 || p->c.a.room != gCharPlayer->a.room) {
            return;
        }
    } else if (PU(p, 0x16C8, u8) == 4 || Npc_InPlayedRoom(p) == 0) {
        return;
    }
    PU(p, 0x1574, f32) = Angle_Wrap(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
    PU(p, 0x1544, u8) = VCALL(p, 0xC0, s32 (*)(Pursuer *))(p);
    if (PU(p, 0x1544, u8) == 1) {
        VCALL(p, 0xCC, void (*)(Pursuer *))(p);
        PU(p, 0x16C8, u8) = 0;
        VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
    }
    if (PU(p, 0x16C8, u8) == 0) {
        s32 side = Npc_ExitSideBehind(p);

        if (side != Npc_CharSideBehind(p, gCharPlayer)) {
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
/* 0x0028AEC0 */
void Pursuer_StateFaceFiona(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
    p->target = gCharPlayer;
    p->c.unk104[0] = 0;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &Pursuer_StateCloseOnFionaB_ptmf);
    Pursuer_StateCloseOnFionaB(p);
}

/* vtable +0x250: to the next exit of the path; look through it first if it's where Fiona was */
/* 0x00285150 */
void Pursuer_NextPathExit(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if (p->c.unk1388 < p->c.unk1384 && Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) != -1) {
        VObject *rm = gRooms;

        PU(p, 0x17B0, u8) = VCALL(rm, 0x3C, u32 (*)(VObject *, u32, s32))(rm, PU(p, 0x138C + p->c.unk1388 * 2, u16), p->c.a.room);
        PU(p, 0x15A4, s32) = VCALL(rm, 0x34, s32 (*)(VObject *, u32, f32 *))(rm, PU(p, 0x17B0, u8), (f32 *)((u8 *)p + 0x15B0));
        if (Npc_AtSpawn(p, PU(p, 0x17B0, u8)) != 0) {
            u32 t = Npc_TurnWay(p, Npc_ExitHeading(p, PU(p, 0x17B0, u8)), 0x1.0c1524p+0f /* 60 degrees */, 0x1.4f1a6ep+1f /* 150 degrees */) & 0xFF;

            if (t == 0xFF) {
                PURSUER_STEP_DONE(p) = 1;
            } else {
                p->c.unk104[0] = t;
                Actor_SetState(&p->c.a, &D_003ED280);
            }
        } else {
            if (!(VCALL(p, 0xD8, s32 (*)(Pursuer *))(p) & 0xFF)) {
                if (!(Pursuer_TargetOutOfReach(p) & 0xFF)) {
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

extern const PTMF D_003ED260, D_003ECE30, D_003ED5B0, Pursuer_StateEndStep_ptmf, Pursuer_StateStepFacing_ptmf, D_003ED580, Pursuer_StateEndStep_ptmf2;
extern PTMF kPursuerIdleMove;   /* followed by the idle move: +0xC kind, +0x10 move mode, +0x14 sub */

/* vtable +0x254: on to the next exit (of the path, or a random one) */
/* 0x002856A0 */
void Pursuer_OnToNextExit(Pursuer *p) {
    VObject *rm;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if (p->c.unk1388 < p->c.unk1384 && Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) != -1) {
        PU(p, 0x17B0, u8) = VCALL(gRooms, 0x3C, u32 (*)(VObject *, u32, s32))(gRooms, PU(p, 0x138C + p->c.unk1388 * 2, u16), p->c.a.room);
    } else {
        PU(p, 0x17B0, u8) = Npc_RandomExit(p, p->c.door);
    }
    PU(p, 0x15A4, s32) = VCALL(gRooms, 0x34, s32 (*)(VObject *, u32, f32 *))(gRooms, PU(p, 0x17B0, u8), (f32 *)((u8 *)p + 0x15B0));
    if (!(VCALL(p, 0xD8, s32 (*)(Pursuer *))(p) & 0xFF)) {
        u32 node;

        if (Pursuer_TargetOutOfReach(p) & 0xFF) {
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
    if ((Progress_ExitOpen(gProgress, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1) {
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
    if (Actor_Distance(&p->c.a, (f32 *)((u8 *)p + 0x15D0)) < 0x1.99999a0000000p-4f /* 0.1 */) {
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
/* 0x0027FC70 */
void Pursuer_BackOnMesh(Pursuer *p) {
    f32 h;

    if (p->c.a.navTri == (u32)-1) {
        if (PU(p, 0x17AC, s32) != 4 && p->c.door != 0xFF) {
            if (*(f32 *)&p->c.unk14C4 <= 80.0f && PU(p, 0x17B0, u8) != 0xFF) {
                p->c.a.navTri = VCALL(gRooms, 0x28, u32 (*)(VObject *, u32))(gRooms, PU(p, 0x17B0, u8));
                h = Npc_ExitHeading(p, PU(p, 0x17B0, u8));
            } else {
                p->c.a.navTri = VCALL(gRooms, 0x28, u32 (*)(VObject *, u32))(gRooms, p->c.door);
                h = Angle_Wrap(0x1.921fb60000000p+1f /* 3.1415927 */ + Npc_ExitHeading(p, p->c.door));
            }
            VCALL(p, 0x28, s32 (*)(Pursuer *, u32, f32 *, s32))(p, p->c.a.navTri, &h, 0);
        } else {
            Actor_TeleportRandom(&p->c.a, Npc_ExitSideBehind(p));
        }
    } else {
        s32 k = PU(p, 0x17AC, s32);

        if ((k == 5 || k == 0 || k == 1 || k == 3) && *(f32 *)&p->c.unk14C4 <= 80.0f && PU(p, 0x17B0, u8) != 0xFF) {
            p->c.a.navTri = VCALL(gRooms, 0x28, u32 (*)(VObject *, u32))(gRooms, PU(p, 0x17B0, u8));
            h = Npc_ExitHeading(p, PU(p, 0x17B0, u8));
            VCALL(p, 0x28, s32 (*)(Pursuer *, u32, f32 *, s32))(p, p->c.a.navTri, &h, 0);
            return;
        }
        if (VCALL(p, 0x28, s32 (*)(Pursuer *, u32, f32 *, s32))(p, p->c.a.navTri, NULL, 0) == -1) {
            Actor_TeleportRandom(&p->c.a, -1);
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
/* 0x00289050 */
void Pursuer_StateSidestep(Pursuer *p) {
    s32 flipped = 0;

    for (;;) {
        f32 v[4] __attribute__((aligned(16)));
        f32 a = (f32)p->c.unk104[0] * 0x1.6571860000000p-4f /* 0.08726647 */;
        f32 r = Actor_Distance(&p->c.a, p->target->a.pos);
        u32 tri;
        s32 ok, clear;

        sceVu0SubVector(v, p->c.a.pos, p->target->a.pos);
        Vec_TurnY(v, v, a);
        sceVu0Normalize(v, v);
        sceVu0ScaleVector(v, v, r);
        sceVu0AddVector(v, v, p->target->a.pos);
        tri = Actor_TriTo(&p->c.a, v, 0);
        ok = tri != (u32)-1 && Npc_TriBlocked(p, tri) == 0;
        clear = Actor_CanWalkBetween(&p->c.a, tri, p->target->a.navTri, v, p->target->a.pos, 0) & 0xFF;
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
        Npc_StepToward(p, v);
        PU(p, 0x1634, f32) += Actor_Distance(&p->c.a, p->c.a.prevPos);
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
        Actor_PlaySound(&p->c.a, sound, 7, 0, 0, pos);
    }
    pr = gProgress;
    if (PursuerGroup_Fields(pr, exit, 0) & 0xFF & 4) {
        Relation_Request(pr, *(u8 *)&p->c.a.slot, 1, 6, 0, 2, 5.0f);
    }
    ptmf_set((PTMF *)((u8 *)p + 0x17A0), &D_003ED5B0);
    VCALL(p, 0x178, void (*)(Pursuer *))(p);
}

/* vtable +0x...: walk, then play the animation +0x104 (-1: done) */
/* 0x00291EE0 */
void Pursuer_StateWalkThenAnim(Pursuer *p) {
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
    Actor_SetState(&p->c.a, &Pursuer_StateEndStep_ptmf);
    if (((MOTION_KEYS(p) & MOTION_KEY_END) != 0) == 1) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    } else {
        Character_RootMoveMasked(&p->c);
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
            Character_FollowWaypoints(&p->c, 1.0f + 0x1.99999a0000000p-3f /* 0.2 */ * VCALL(gRandom, 0x18, f32 (*)(VObject *))(gRandom));
            break;
        case 2:
        case 3:
            Character_FollowWaypoints(&p->c, 0x1.3333340000000p-1f /* 0.6 */);
            break;
        case 1:
        case 4:
            Character_FollowWaypoints(&p->c, 1.0f);
            break;
        }
    } else {
        Character_FollowWaypoints(&p->c, 2.0f);
    }
    if (!(p->c.unk128 < p->c.unk124)) {
        s32 room = VCALL(gRooms, 0x18, s32 (*)(VObject *, s32, u32))(gRooms, p->c.a.room, PU(p, 0x17B0, u8));

        if (room == VCALL(pr, 0xC, s32 (*)(Progress *))(pr) && (Npc_ExitKind(p, PU(p, 0x17B0, u8)) & 0xFF) == 5 &&
            PU(p, 0x16C8, u8) != 4) {
            PURSUER_STEP_DONE(p) = 1;
            return;
        }
        if (Pursuer_MayUseExit(p, PU(p, 0x17B0, u8)) & 0xFF) {
            Pursuer_ThroughDoor(p, PU(p, 0x17B0, u8));
        }
    }
}

/* vtable +0x...: walk, then the 0x404 animation, turning to the target */
/* 0x002895B0 */
void Pursuer_StateWalkThen404(Pursuer *p) {
    f32 h, d;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
        return;
    }
    Pursuer_PlayAnim(p, 0x404);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &Pursuer_StateStepFacing_ptmf);
    h = Actor_HeadingTo(&p->c.a, p->target->a.pos);
    d = Npc_TurnToward(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    if (d <= 0.0f) {
        d = -d;
    }
    if (d < 0x1.921fb60000000p-1f /* 0.7853982 */) {
        Character_RootMoveMasked(&p->c);
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
        if (!(Pursuer_PlanWhere(p) & 0xFF)) {
            ptmf_set((PTMF *)((u8 *)p + 0x17A0), &kPursuerIdleMove);
            PU(p, 0x17AC, s32) = (&AT(&kPursuerIdleMove, 0xC, s32))[0];
            p->c.moveMode = AT(&kPursuerIdleMove, 0x10, s32);
            p->c.moveSub = AT(&kPursuerIdleMove, 0x14, s32);
            p->c.unk1530 = 0;
            p->c.unk1538 = 0;
            p->c.unk1534 = 0;
            if (PU(p, 0x16C8, u8) != 2) {
                PU(p, 0x16F4, u8) = 1;
            }
            return;
        }
    } else if (Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) <= 0) {
        p->c.unk1384 = p->c.unk1388;
        PU(p, 0x16F4, u8) = 1;
        return;
    }
    PU(p, 0x17B0, u8) = VCALL(gRooms, 0x3C, u32 (*)(VObject *, u32, s32))(gRooms, PU(p, 0x138C, u16), p->c.a.room);
    p->c.unk14C0 = PU(p, 0x138C, u16);
    *(f32 *)&p->c.unk14C4 = Npc_RoomNodeDistance(p, p->c.a.room, PU(p, 0x17B0, u8), p->c.door);
    p->c.unk1530 = 0;
    p->c.unk1538 = 0;
    p->c.unk1534 = 0;
    ptmf_set((PTMF *)((u8 *)p + 0x17A0), &D_003ED580);
    PU(p, 0x1784, s32) = 0;
}

/* vtable +0x...: walk, then play the gesture +0x104 of the table +0x1720 ({anim, wait} 8 bytes) */
/* 0x00291C80 */
void Pursuer_StateWalkGesture(Pursuer *p) {
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
    Actor_SetState(&p->c.a, &Pursuer_StateEndStep_ptmf2);
    if (((MOTION_KEYS(p) & MOTION_KEY_END) != 0) == 1) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    } else {
        Character_RootMoveMasked(&p->c);
    }
}

/* ---- batch 9 ---- */

extern const PTMF Pursuer_AttackNextStep_ptmf4, D_003ECE90, D_003ECA00, D_003ECA10, D_003ECA20, D_003ED560, D_003ED570,
    D_003ECC30, D_003ECDE0, D_003ECDD0, Pursuer_StateSidestep_ptmf;
extern PTMF kPursuerWaitMove;   /* followed by the wait move: +0xC kind, +0x10 move mode, +0x14 sub */

/* vtable +0x70: placed into room `room` (triangle `tri`, -1: by its exit / way in), on side
 * `side` (0 / 1; else any usable exit) */
/* 0x0029D180 */
s32 Pursuer_PlaceInRoom(Pursuer *p, s32 room, s32 tri, u32 side) {
    VObject *rm;
    u32 i;

    ((void (*)(Character *))Character_ToRoom)(&p->c);   /* the original passes only the character */
    p->c.a.room = room;
    rm = gRooms;
    if (side < 2) {
        for (i = 0; i < 8; i = (i + 1) & 0xFF) {
            if (VCALL(rm, 0x74, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, i) != 0 && (Npc_ExitKind(p, i) & 0xFF) != 3 &&
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
            if (VCALL(rm, 0x74, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, i) != 0 && (Npc_ExitKind(p, i) & 0xFF) != 3) {
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
    if (Npc_InPlayedRoom(p) != 0) {
        if (tri != -1) {
            VCALL(p, 0x28, s32 (*)(Pursuer *, s32, f32 *, s32))(p, tri, NULL, 0);
        } else {
            Actor_TeleportRandom(&p->c.a, Npc_ExitSideBehind(p));
        }
        Motion_Play(p->c.motion, 0, -1);
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
    Pursuer_SearchRouteIn(p);
    return 0;
}

extern u8 pstr_O_GLM_GLM_200_PCK[], pstr_O_LRF_LRF_200_PCK[], pstr_O_FIN_FIN_200_PCK_2[], pstr_O_CRW_CRW_200_PCK[], D_00429C70[], pstr_O_SGM_SGM_200_PCK[],
    pstr_O_FIW_FIW_200_PCK_2[], pstr_O_FS0_FS0_200_PCK[], pstr_O_FS1_FS1_200_PCK[], pstr_O_FS2_FS2_200_PCK[], pstr_O_SHT_SHT_200_PCK[], pstr_O_WIR_WIR_200_PCK[], pstr_O_RBT_RBT_200_PCK[],
    pstr_O_HMA_HMA_200_PCK[], pstr_O_DNT_DNT_200_PCK[], pstr_O_LRC_LRC_200_PCK[], pstr_O_LRH_LRH_200_PCK[], pstr_O_FIM_FIM_200_PCK_2[], pstr_O_HMB_HMB_200_PCK[], pstr_O_HND_HND_200_PCK[];

/* vtable +0xF8: the model files of character id +0x153C (slot 2: vtable +0xF8) */
/* 0x0029F8C0 */
u8 *Pursuer_ModelFiles(Pursuer *p) {
    if (p->c.a.slot != 2) {
        switch (p->c.unk153C) {
        case 2: return Debilitas_ModelFileTable(p);
        case 3: return Daniella_ModelFileTable(p);
        case 4: return Riccardo_ModelFileTable(p);
        case 6: return Debilitas2_ModelFileTable(p);
        case 7: return Debilitas3_ModelFileTable(p);
        case 8: return pstr_O_GLM_GLM_200_PCK;
        case 9: return Kind09_ModelFileTable(p);
        case 10: return Lorenzo2_ModelFileTable(p);
        case 11: return Lorenzo_ModelFileTable(p);
        case 12: return pstr_O_LRF_LRF_200_PCK;
        case 13: return pstr_O_FIN_FIN_200_PCK_2;
        case 14: case 15: return pstr_O_CRW_CRW_200_PCK;
        case 16: return D_00429C70;
        case 17: return pstr_O_SGM_SGM_200_PCK;
        case 18: return pstr_O_FIW_FIW_200_PCK_2;
        case 19: return pstr_O_FS0_FS0_200_PCK;
        case 20: return pstr_O_FS1_FS1_200_PCK;
        case 21: case 22: return pstr_O_FS2_FS2_200_PCK;
        case 23: return TintStalker_ModelFileTable(p);
        case 24: return pstr_O_SHT_SHT_200_PCK;
        case 25: return pstr_O_WIR_WIR_200_PCK;
        case 26: return pstr_O_RBT_RBT_200_PCK;
        case 27: return Kind27_ModelFileTable(p);
        case 28: return pstr_O_HMA_HMA_200_PCK;
        case 29: return pstr_O_DNT_DNT_200_PCK;
        case 30: return pstr_O_LRC_LRC_200_PCK;
        case 31: return pstr_O_LRH_LRH_200_PCK;
        case 32: return pstr_O_FIM_FIM_200_PCK_2;
        case 33: return pstr_O_HMB_HMB_200_PCK;
        case 34: return Kind34_ModelFileTable(p);
        case 35: return Kind35_ModelFileTable(p);
        case 36: return Kind36_ModelFileTable(p);
        case 37: return Kind37_ModelFileTable(p);
        case 38: return pstr_O_HND_HND_200_PCK;
        case 39: return Kind39_ModelFileTable(p);
        default: return NULL;
        }
    }
    return VCALL(p, 0xF8, u8 *(*)(Pursuer *))(p);
}

/* the attack's active frames: everyone in its reach (entry +0xC) is hit (+0x1760 bits, the
 * hit reported with the entry's kind +0x10, sound +0x12, +0x4 and strength +0x14); after it
 * the next attack step */
/* 0x0028B0D0 */
void Pursuer_StateAttackActive(Pursuer *p) {
    u8 *e = PU(p, 0x171C, u8 *) + PU(p, 0x1724, s8 *)[PU(p, 0x1728, s32) * 4 + PU(p, 0x172C, s8)] * 0x24;

    if (Motion_EventFlags(p->c.motion, 0, -1, 1) & 0xFF & 2) {
        Progress *pr = gProgress;

        if (!(Progress_TestFlag(pr, 0xE) & 0xFF)) {
            u32 i;

            for (i = 0; i < 3; i = (i + 1) & 0xFF) {
                u32 s = i & 0xFF;
                Character *c = gCharacters[s];

                if (c != NULL && s != (u32)p->c.a.slot && c->a.active != 0 && Actor_Distance(&p->c.a, c->a.pos) < AT(e, 0xC, f32)) {
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
        Actor_SetState(&p->c.a, &Pursuer_AttackNextStep_ptmf4);
        return;
    }
    if (AT(e, 0x1C, u8) != 0) {
        f32 h = Actor_HeadingTo(&p->c.a, p->target->a.pos);

        Npc_TurnToward(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    }
    Character_RootMoveMasked(&p->c);
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
            Npc_DoorShutOther(p, (u8)p->c.unk100);
        } else {
            Npc_DoorRelease(p, (u8)p->c.unk100);
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
    Character_RootMoveMasked(&p->c);
}

/* vtable +0x2BC: chase Fiona in the played room (behaviour 0xC), or the wait move elsewhere */
/* 0x0029AF20 */
void Pursuer_ChaseFionaHere(Pursuer *p) {
    if (p->c.a.unkC4 == 2) {
        p->c.a.unkC4 = 0;
        PU(p, 0x1664, s32) = 0;
    }
    if (Npc_InPlayedRoom(p) != 0) {
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
        Pursuer_SearchRoom(p);
    } else {
        p->c.unk1388 = p->c.unk1384;
        ptmf_set((PTMF *)((u8 *)p + 0x17A0), &kPursuerWaitMove);
        PU(p, 0x17AC, s32) = AT(&kPursuerWaitMove, 0xC, s32);
        p->c.moveMode = AT(&kPursuerWaitMove, 0x10, s32);
        p->c.moveSub = AT(&kPursuerWaitMove, 0x14, s32);
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
    if ((Pursuer_PlanWhere(p) & 0xFF) != 1) {
        u32 i;

        for (i = 0; i < 13; i++) {
            p->c.unk148C[i] = 0;
        }
        if (Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) == -1) {
            PU(p, 0x17B0, u8) = VCALL(rm, 0x3C, u32 (*)(VObject *, u32, s32))(rm, PU(p, 0x138C, u16), p->c.a.room);
            Pursuer_StartSearch(p);
            return;
        }
        *(f32 *)&p->c.unk14C4 = Npc_NodeDistance(p, p->c.a.room, node, PU(p, 0x138C, u16));
        PU(p, 0x17B0, u8) = VCALL(rm, 0x3C, u32 (*)(VObject *, u32, s32))(rm, PU(p, 0x138C, u16), p->c.a.room);
        ptmf_set((PTMF *)((u8 *)p + 0x17A0), &D_003ED570);
    } else {
        *(f32 *)&p->c.unk14C4 = Npc_NodeDistance(p, p->c.a.room, node, PU(p, 0x138C, u16));
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
    PU(p, 0x158C, f32) = Npc_FootDistance(p, gCharPartner);
    PU(p, 0x16C9, u8) = PU(p, 0x16C9, u8) > 0 ? PU(p, 0x16C9, u8) : 1;
    PU(p, 0x16CA, u8) = PU(p, 0x16CA, u8) >= 2 ? PU(p, 0x16CA, u8) : 2;
    prev = PU(p, 0x1758, s32);
    if (prev != -1) {
        if (prev != -2) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, prev);
        }
    } else if (p->c.a.room == p->target->a.room) {
        if (PU(p, 0x158C, f32) < 0.0f) {
            if (!(Pursuer_TargetOutOfReach(p) & 0xFF)) {
                VCALL(p, 0x13C, void (*)(Pursuer *))(p);
                return;
            }
        } else if (PU(p, 0x158C, f32) <= VCALL(p, 0x2F0, f32 (*)(Pursuer *))(p) && (Npc_SameFloor(&p->c.a, &gCharPartner->a) & 0xFF)) {
            u32 t = Npc_TurnWayTo(p, p->target->a.pos, 0x1.0c15240000000p+0f /* 1.0471976 */, 0x1.4f1a6ep+1f) & 0xFF;

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
    if (!(Pursuer_TargetOutOfReach(p) & 0xFF)) {
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
/* 0x00289280 */
void Pursuer_StateSidestepRoom(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 1;
    if (Npc_RoomToSide(p, 8.0f) & 0xFF) {
        Character *t;

        if ((Pursuer_WalkOn(p) & 0xFF) == 1) {
            return;
        }
        t = p->target;
        p->c.unk104[0] = Angle_Wrap(Angle_Wrap(Actor_HeadingTo(&t->a, p->c.a.pos) - t->a.angle[1])) <= 0.0f ? -1 : 1;
        PU(p, 0x1634, s32) = 0;
        if (PU(p, 0x1788, s32) != 0x200) {
            Pursuer_PlayAnim(p, VCALL(p, 0x324, s32 (*)(Pursuer *))(p));
        }
        PU(p, 0x1784, s32) = 0;
        Actor_SetState(&p->c.a, &Pursuer_StateSidestep_ptmf);
        Pursuer_StateSidestep(p);
        return;
    }
    PURSUER_STEP_DONE(p) = 1;
}

/* ---- batch 10 ---- */


extern const PTMF Pursuer_StateRunThenNext_ptmf, D_003EC940, D_003ED350, D_003ED4B0, D_003ED4C0, D_003ED4D0, D_003ED4E0,
    D_003ED4F0, D_003ED500, D_003ECEE0, Pursuer_StateRunThenNext_ptmf5, D_003ECB70, Pursuer_StateCountKeys_ptmf;
extern PTMF kPursuerMove;   /* followed by a move: +0xC kind, +0x10 move mode, +0x14 sub */
extern void *Effect79FF0_vtable[];

/* a move from one of the tables at 0x45B340 / 0x45B358 / 0x45B3A0: step function to +0x17A0, kind
 * +0x17AC, move mode and sub */

/* vtable +0x84: the state block (+0x14E8) of an event: 4 hit, 5 back to normal, 0xC hurt (30),
 * 7 freed */
/* 0x0029C8C0 */
void Pursuer_EventState(Pursuer *p) {
    switch (p->c.state[0]) {
    case 4:
        if (Npc_InPlayedRoom(p) != 0) {
            Pursuer_Hit(p);
        } else {
            Pursuer_HitOffscreen(p);
        }
        return;
    case 5:
        if (Npc_InPlayedRoom(p) & 0xFF) {
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
                Pursuer_ClearSteps(p);
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
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &Pursuer_StateRunThenNext_ptmf);
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
/* 0x0029D7F0 */
void Pursuer_FootstepsThroughWalls(Pursuer *p) {
    if (Npc_InPlayedRoom(p) == 0 && PU(p, 0x17AC, s32) != 4) {
        f32 d;

        PU(p, 0x1538, f32) = PU(p, 0x1534, f32);
        d = Character_PathRemaining2(&p->c);
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
                if (Character_RouteExitPoint(&p->c, pos) != 0) {
                    s8 vol = (u8)(u32)(10.0f * f) & 0x7F;
                    s8 vol2 = (u8)(u32)(2.0f * f) & 0x7F;
                    s32 foot = PU(p, 0x16A4, s32) & 1;

                    if (VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(gProgress, 8) == 0) {
                        Actor_PlaySound(&p->c.a, foot, 7, vol, vol2, pos);
                    }
                    PU(p, 0x16A4, s32)++;
                }
            }
        }
    }
}

/* vtable +0x130: pick from the attack table +0x1718 ({kind, value, chance}) */
/* 0x00283C50 */
void Pursuer_PickFromTable(Pursuer *p) {
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
        if (!(((s32 (*)(Pursuer *, Character *, u32))Actor_TriFreeFor)(p, gCharPlayer, i) & 0xFF)) {
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
/* 0x0029CEE0 */
void Pursuer_IntoRoomByEvent(Pursuer *p, s32 room, u32 found, s32 plan, s32 side) {
    VCALL(p, 0x64, s32 (*)(Pursuer *, s32, s32, s32))(p, room, -1, side);
    Motion_Play(p->c.motion, 0, -1);
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
            Pursuer_PlanWhere(p);
        }
    }
    if (p->c.unkE0 == 1) {
        p->c.unkE0 = 0;
    }
    Pursuer_SearchRouteIn(p);
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
    Pursuer_ClearSteps(p);
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
/* 0x00287380 */
void Pursuer_StateBargedThrough(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));

    Character_RootMoveMasked(&p->c);
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
                Actor_Move(&p->c.a, v);
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
        vu0_ScaleXYZ(v, v, 3.0f);
        Actor_Move(&p->c.a, v);
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
                Pursuer_ThroughDoor(p, (u8)p->c.unk100);
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
                Npc_DoorRelease(p, (u8)p->c.unk100);
            } else if ((Progress_ExitOpen(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1) {
                Npc_DoorShutOther(p, (u8)p->c.unk100);
            } else {
                Npc_DoorRelease(p, (u8)p->c.unk100);
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
            p->c.a.navTri = Actor_DoorFront(p, p->c.unk100, 0, ofs, p->c.a.pos);
        } else {
            VCALL(p, 0x9C, void (*)(Pursuer *, u32, f32 *))(p, 2, ofs);
            p->c.a.navTri = Actor_DoorFront(p, p->c.unk100, 1, ofs, p->c.a.pos);
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
    Motion_Unfreeze(p->c.motion);
    PU(p, 0x16F5, u8) = 0;
    PU(p, 0x16F8, u8) = 0;
    PU(p, 0x16F7, u8) = 0;
    PU(p, 0x1794, s32) = 0;
    PU(p, 0x1798, s32) = 0;
    PU(p, 0x16F3, u8) = 0;
    if (Npc_InPlayedRoom(p) != 0) {
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
/* 0x0027F0A0 */
s32 Pursuer_MayUseExit(Pursuer *p, u32 exit) {
    Progress *pr = gProgress;
    VObject *rm;
    u32 node;

    if ((Progress_CurRoomFlag(pr, p->c.a.room, exit) & 0xFF) == 1) {
        return 0;
    }
    rm = gRooms;
    if (VCALL(rm, 0x18, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, exit) != VCALL(pr, 0xC, s32 (*)(Progress *))(pr)) {
        switch (Npc_ExitWhatToDo(p, exit) & 0xFF) {
        case 1:
            node = VCALL(rm, 0x10, u32 (*)(VObject *, s32, u32))(rm, p->c.a.room, exit) & 0xFFFF;
            p->c.unk148C[node >> 5] &= ~(1 << (node & 0x1F));
            return 1;
        case 2:
            return 0;
        case 0:
            if (PU(p, 0x17AC, s32) != 4) {
                Pursuer_ExitClosed(p, exit);
            }
            return 0;
        default:
            return 0;
        }
    }
    switch (Npc_ExitKind(p, exit) & 0xFF) {
    case 4:
        node = VCALL(rm, 0x10, u32 (*)(VObject *, s32, u32))(rm, p->c.a.room, exit) & 0xFFFF;
        p->c.unk148C[node >> 5] &= ~(1 << (node & 0x1F));
        return 1;
    case 3:
        if (PU(p, 0x17AC, s32) != 4) {
            Pursuer_ExitClosed(p, exit);
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
    if ((Progress_ExitOpen(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1) {
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
        Npc_DoorShut2(p, (u8)p->c.unk100);
    } else if (p->c.moveSub == 0x15) {
        Npc_DoorShut(p, (u8)p->c.unk100);
    }
    p->c.a.navTri = VCALL(d, 0x14, u32 (*)(VObject *, u32, s32, f32 *, f32 *, s32))(d, (u8)p->c.unk100, p->c.unk104[0], p->c.a.pos, dir, 1);
    p->c.a.angle[1] = dir[1];
    sceVu0UnitMatrix(p->c.a.rot);
    sceVu0RotMatrixY(p->c.a.rot, p->c.a.rot, dir[1]);
    switch (p->c.unk104[0]) {
    case 0: Motion_Play(p->c.motion, 0x600, -1); break;
    case 1: Motion_Play(p->c.motion, 0x601, -1); break;
    case 2: Motion_Play(p->c.motion, 0x602, -1); break;
    case 3: Motion_Play(p->c.motion, 0x603, -1); break;
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
        Pursuer_SetMove(p, &kPursuerMove);
        break;
    case 1:
    case 3:
    case 4:
        Pursuer_SetMove(p, &kPursuerWaitMove);
        break;
    case 2:
        Pursuer_SetMove(p, &kPursuerIdleMove);
        break;
    }
    if (p->c.unkE0 == 0) {
        if (Npc_InPlayedRoom(p) != 0) {
            VCALL(p, 0x13C, void (*)(Pursuer *))(p);
        } else {
            ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECB70);
            PU(p, 0x1758, s32) = -1;
        }
        PURSUER_STEP_NEXT(p) = 1;
    } else {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &Pursuer_StateRunThenNext_ptmf5);
        PU(p, 0x1758, s32) = -1;
    }
}

static void Pursuer_EffectInit(void **obj) {
    *obj = Effect79FF0_vtable;
}

/* a special animation (0x1006) with an effect (0x479FF0) for +0x104 */
/* 0x00285B10 */
void Pursuer_StateSpecialAnim(Pursuer *p) {
    u8 *m;
    s32 args[2];
    u8 *mgr;

    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    Pursuer_PlayAnim(p, 0x1006);
    m = p->c.motion;
    if (!(AT(AT(p->c.motion, 0x874, u8 *) + Motion_AnimIndex(m, AT(m, 0x55C, s32)) * 6, 0x4, u16) & 1)) {
        PU(p, 0x1624, s32) = 1;
    } else {
        PU(p, 0x1624, s32) = p->c.unk104[0];
    }
    mgr = gEffects;
    PU(p, 0x1628, s32) = Effect_New(mgr, 0x38, Pursuer_EffectInit);
    args[0] = (p->c.unk104[0] - 4) >> 1;
    args[1] = PU(p, 0x1624, s32);
    EffectMgr_Start(mgr, PU(p, 0x1628, s32), args);
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &Pursuer_StateCountKeys_ptmf);
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
    Character_RootMoveMasked(&p->c);
}

/* ---- batch 11 ---- */

extern const PTMF D_003ECA30, D_003ECA40, D_003ECA50, D_003ECF60, D_003EC9B0, D_003ECA60, Pursuer_StateHitReact_ptmf,
    D_003ECA80;

/* vtable +0x2C4: start searching (in the played room: the route cleared to one stop) */
/* 0x0029AC50 */
void Pursuer_StartSearch(Pursuer *p) {
    if (p->c.a.unkC4 == 2) {
        p->c.a.unkC4 = 0;
        PU(p, 0x1664, s32) = 0;
    }
    if (Npc_InPlayedRoom(p) != 0) {
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
        Pursuer_SetMove(p, &kPursuerWaitMove);
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
        } else if (Npc_ExitSideBehind(p) != Npc_CharSideBehind(p, gCharPlayer)) {
            PU(p, 0x1598, s32) = Npc_CharSideBehind(p, gCharPlayer);
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
                if (Npc_InPlayedRoom(p) != 0 && Npc_ExitSideBehind(p) != -1 && Character_PathLength(&p->c, p->c.heard.tri, v, -1) < 0.0f) {
                    if (Npc_RoundThroughDoor(p, p->c.heard.tri) == 0 && Npc_ExitsToTri(p, p->c.heard.tri) == 0) {
                        PU(p, 0x1594, s32) = p->c.heard.room;
                        PU(p, 0x1598, s32) = (Npc_ExitSideBehind(p) == 0) & 0xFF;
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
    if (PU(p, 0x16F8, u8) == 0 && Npc_InPlayedRoom(p) != 0 && Npc_ReachedRoom(p) != 0) {
        VCALL(p, 0xD8, s32 (*)(Pursuer *))(p);
    }
}

/* the door it was knocked through: as Pursuer_StateBargedThrough, but always on to behaviour 0x20 */
/* 0x00286F10 */
void Pursuer_StateKnockedThrough(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));

    Character_RootMoveMasked(&p->c);
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
                Actor_Move(&p->c.a, v);
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
        vu0_ScaleXYZ(v, v, 3.0f);
        Actor_Move(&p->c.a, v);
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
                Pursuer_ThroughDoor(p, (u8)p->c.unk100);
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
    PU(p, 0x1634, f32) = Angle_Wrap(Npc_DoorFacingFromFiona(p, (u8)p->c.unk100));
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
        vu0_ScaleXYZ(v, v, 3.0f);
        Actor_Move(&p->c.a, v);
    }
}

/* exit `exit` (0xFF: +0x17B0) closed to it: mark it tried, plan again (or the stance's move) */
/* 0x0027CB90 */
void Pursuer_ExitClosed(Pursuer *p, u32 exit) {
    u32 node, i;

    if ((exit & 0xFF) == 0xFF) {
        exit = PU(p, 0x17B0, u8);
    }
    for (i = 0; i < 13; i++) {
        p->c.unk148C[i] = 0;
    }
    node = VCALL(gRooms, 0x10, u32 (*)(VObject *, s32, u32))(gRooms, p->c.a.room, exit) & 0xFFFF;
    p->c.unk148C[node >> 5] |= 1 << (node & 0x1F);
    if (Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) == -1) {
        if (PU(p, 0x16C8, u8) != 0 && PU(p, 0x16C8, u8) != 2) {
            Pursuer_SetMove(p, &kPursuerWaitMove);
        } else {
            Pursuer_SetMove(p, &kPursuerIdleMove);
        }
        return;
    }
    if (PU(p, 0x16C8, u8) != 0 && PU(p, 0x16C8, u8) != 2) {
        Pursuer_SetMove(p, &kPursuerWaitMove);
    } else {
        Pursuer_SetMove(p, &kPursuerMove);
    }
}

/* where to stand to grab Hewie (grab kinds 10..15; 14 / 15 from the other side): search
 * around him in 10 degree steps for a walkable spot level with the pursuer */
/* 0x00285DE0 */
s32 Pursuer_GrabHewieSpot(Pursuer *p, u32 kind, f32 *heading, f32 *pos) {
    f32 h = Actor_HeadingTo(&p->c.a, gCharPartner->a.pos);
    f32 ofs[4] __attribute__((aligned(16)));
    f32 best[4] __attribute__((aligned(16)));
    f32 bestH = 0.0f, bestDy = 0.0f;
    s32 bestTri = -1;
    s32 deg;

    switch (kind & 0xFF) {
    case 14:
    case 15:
        h = Angle_Wrap(0x1.921fb60000000p+1f /* 3.1415927 */ + h);
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
            f32 r = side != 0 ? Angle_Wrap(h + a / 180.0f) : Angle_Wrap(h - a / 180.0f);
            f32 dy;
            s32 tri;

            Mtx_AtHeading(m, p->c.a.pos, r);
            Mtx_ApplyPoint(at, m, ofs);
            dy = p->c.a.pos[1] - at[1];
            tri = Actor_TriOfOnMesh(&p->c.a, at);
            if (dy <= 0.0f) {
                dy = -dy;
            }
            if (tri != -1 && tri == Actor_TriFrom(&p->c.a, at, gCharPartner->a.navTri, gCharPartner->a.pos, 0x29020008)) {
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
/* 0x0029B5C0 */
void Pursuer_HitOffscreen(Pursuer *p) {
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
            Motion_Play(p->c.motion, 0x1802, -1);
            snd = 0x1B;
        }
        p->c.door = VCALL(rm, 0x14, u32 (*)(VObject *, s32, u32))(rm, VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress), (u8)p->c.state[4]);
        if (snd >= 0 && VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(gProgress, 8) == 0) {
            Actor_PlaySound(&p->c.a, snd, 7, 0, 0, pos);
        }
    } else if (p->c.state[1] == 8) {
        f32 pos[4] __attribute__((aligned(16)));

        if (p->c.hp <= 0) {
            p->c.hp = 1;
        }
        PU(p, 0x1664, s32) += p->c.state[4];
        if (p->c.state[3] > 0 && (Character_RouteExitPoint(&p->c, pos) & 0xFF) == 1) {
            snd = 0x1D;
        }
        if (snd >= 0 && VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(gProgress, 8) == 0) {
            Actor_PlaySound(&p->c.a, snd, 7, 0, 0, pos);
        }
    }
    p->c.state[0] = 0;
    p->c.state[1] = 0;
}

/* vtable +0x2D0: knocked down (behaviour 0x1F in the room / 0x20 away; animation 0x1806 if it
 * falls where it can, else 0x1802) */
/* 0x0029A940 */
void Pursuer_KnockedDown(Pursuer *p) {
    if (Npc_InPlayedRoom(p) != 0) {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECA60);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x1F);
        Actor_SetState(&p->c.a, &Pursuer_StateHitReact_ptmf);
    } else {
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECA80);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x20);
    }
    if (((s32 (*)(Pursuer *, Character *, u32))Actor_TriFreeFor)(p, (Character *)p, 0) != 0) {
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

extern const PTMF D_003EC9C0, D_003EC9D0, D_003EC9E0, D_003EC9F0, Pursuer_StateBargedThrough_ptmf, D_003ECE00, D_003EC900;

/* vtable +0x2BC: go for Fiona (stance 0): the route forgotten, behaviour by sight */
/* 0x0029B190 */
void Pursuer_GoForFionaStance0(Pursuer *p) {
    u32 i;
    s32 k;

    PU(p, 0x16C8, u8) = 0;
    VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
    if (p->c.a.unkC4 == 2) {
        p->c.a.unkC4 = 0;
        PU(p, 0x1664, s32) = 0;
    }
    if (Npc_InPlayedRoom(p) != 0) {
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
            Pursuer_SetMove(p, &kPursuerMove);
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

    Motion_LookAt(p->c.motion, pos, &pitch, &yaw);
    if ((yaw <= 0.0f ? -yaw : yaw) < 0x1.99999a0000000p-5f /* 0.05 */) {
        yaw = 0.0f;
    }
    Motion_EaseTilt(p->c.motion, 0.0f, yaw, 0.0f, VCALL(p, 0xA4, f32 (*)(Pursuer *))(p));
    PU(p, 0x1574, f32) = Angle_Wrap(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
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
    Npc_SensesWatching(p);
    if (p->c.unkE0 != 0 || p->c.moveMode == 4) {
        PTMF *st = (PTMF *)((u8 *)p + 0x174C);

        if (ptmf_test(st)) {
            ptmf_scall(p, st);
        }
    }
    Pursuer_MotionGroup(p);
    VCALL(p, 0x110, void (*)(Pursuer *))(p);
    VCALL(p, 0x100, void (*)(Pursuer *))(p);
    Pursuer_AnimSounds(p, -1);
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
/* 0x00287620 */
void Pursuer_StateHitAtDoor(Pursuer *p) {
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
    if ((Actor_Distance(&p->c.a, v) < 9.0f || (st & 0xFF & 9)) &&
        Progress_ExitPassable(pr, p->c.a.room, (u8)p->c.unk100, *(u8 *)&p->c.a.slot) != 0) {
        Pursuer_PlayAnimBlend(p, 0x1004);
        PU(p, 0x1634, f32) = Npc_DoorFacingFromFiona(p, (u8)p->c.unk100);
        p->c.a.unk2B = 1;
    } else {
        if (p->c.hp <= 0) {
            p->c.hp = 1;
        }
        Pursuer_PlayAnimBlend(p, 0x1005);
        PU(p, 0x1634, f32) = Angle_Wrap(0x1.921fb60000000p+1f /* 3.1415927 */ + Npc_DoorFacingFromFiona(p, (u8)p->c.unk100));
        p->c.unk100 = 0xFF;
    }
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &Pursuer_StateBargedThrough_ptmf);
    Pursuer_StateBargedThrough(p);
}

/* sounds keyed to the animation `anim` (-1: the current one) by the table +0x16AC ({left,
 * right} per animation): footsteps, and when it falls (0x1709 / 0x1800 / 0x1804) a splash on
 * water triangles (with ripples in rooms 7, 0xD1 and 0x106) */
/* 0x0029D4C0 */
void Pursuer_AnimSounds(Pursuer *p, s32 anim) {
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
    switch (Motion_EventFlags(p->c.motion, 0, 0, 1) & 0xFF & 0x11) {
    case 0x11:
        twice = 1 & 0xFF;
        /* fallthrough */
    case 0x1:
        snd = PU(p, 0x16AC, s16 *)[Motion_AnimIndex(p->c.motion, anim) * 2];
        break;
    case 0x10:
        snd = PU(p, 0x16AC, s16 *)[Motion_AnimIndex(p->c.motion, anim) * 2 + 1];
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
                            Character_WaterStep(&p->c, p->c.a.pos, 1);
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
                Actor_PlaySound(&p->c.a, (s16)snd, bank & 0xFF, 0, 0, NULL);
            }
        }
        if (twice == 0) {
            break;
        }
        snd = PU(p, 0x16AC, s16 *)[Motion_AnimIndex(p->c.motion, anim) * 2 + 1];
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
    if (!(Npc_TargetSideWalkable(p, 0x1.921fb60000000p-1f /* 0.7853982 */, 8.0f) & 0xFF) && !(Npc_TargetSideWalkable(p, -0x1.921fb60000000p-1f /* 0.7853982 */, 8.0f) & 0xFF)) {
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
    Pursuer_ClearSteps(p);
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
    Pursuer_SetMove(p, &kPursuerWaitMove);
    ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003EC900);
    PU(p, 0x1758, s32) = -1;
    PU(p, 0x1718, s32) = 0;
    PU(p, 0x168C, u8 *) = VCALL(p, 0xF8, u8 *(*)(Pursuer *))(p);
    PU(p, 0x1690, u8 *) = VCALL(p, 0xFC, u8 *(*)(Pursuer *))(p);
    VCALL(p, 0xF4, void (*)(Pursuer *))(p);
}

/* 0x002EB390 */
s32 Summoner_Via0(u8 *o) {
    return summon_via(o, 0, 0);
}

/* 0x002EB730 */
s32 Summoner_Via1(u8 *o) {
    return summon_via(o, 1, 0);
}

/* 0x002EBB00 */
s32 Summoner_Via1B(u8 *o) {
    return summon_via(o, 1, 1);
}

/* each frame with the pursuer in play: the cooldown runs down; its mode (+0x16C8) changed: the
 * wait restarts; Fiona in its room, or next door where it isn't hunting her (progress +0x64
 * not 4 or Pursuer_IsOwnRoom): likewise; else the wait counts up, and after 5 s hunting (+0xC4 2:
 * kind 2) or in mode 4 (kind 1), or with the cooldown over 3 s in mode 3 (kind 0), the
 * summoner takes it back. 1 if it did */
/* 0x002EBED0 */
s32 Summoner_InPlay(u8 *o) {
    u8 *pu = (u8 *)gCharPursuer;
    u8 *pl;
    u8 mode;
    s32 go = 0, kind = 0;

    if (pu == NULL || AT(pu, 0x28, u8) == 0) {
        return 0;
    }
    if (AT(o, 0x4, u32) != 0) {
        AT(o, 0x4, u32)--;
    }
    mode = AT(pu, 0x16C8, u8);
    if (mode != AT(o, 0x10, u8)) {
        AT(o, 0x10, u8) = mode;
        AT(o, 0xC, u32) = 0;
        return 0;
    }
    pl = (u8 *)gCharPlayer;
    if (pl != NULL && AT(pl, 0x28, u8) != 0 && AT(pl, 0x30, s32) != -1) {
        s32 pr = AT(pl, 0x30, s32), ur = AT(pu, 0x30, s32);
        u32 e;

        if (pr == ur) {
            AT(o, 0xC, u32) = 0;
            return 0;
        }
        for (e = 0; e < 8; e++) {
            if (ur == VCALL(gRooms, 0x18, s32 (*)(VObject *, s32, u32))(gRooms, pr, e & 0xFF) &&
                ((u8)VCALL(gProgress, 0x64, s32 (*)(Progress *))(gProgress) != 4 || !Pursuer_IsOwnRoom((Pursuer *)pu, -1))) {
                AT(o, 0xC, u32) = 0;
                return 0;
            }
        }
    }
    if (AT(o, 0xC, u32) + 1 != 0) {
        AT(o, 0xC, u32)++;
    }
    if (AT(pu, 0xC4, s32) == 2 && AT(o, 0xC, u32) >= 151) {
        go = 1;
        kind = 2;
    }
    if (!go && mode == 4 && AT(o, 0xC, u32) >= 151) {
        go = 1;
        kind = 1;
    }
    if (!go && AT(o, 0x4, u32) != 0) {
        return 0;
    }
    if (!go && mode == 3 && AT(o, 0xC, u32) >= 91) {
        go = 1;
        kind = 0;
    }
    if (go) {
        summoner_take(o, kind);
    }
    return go;
}

/* each frame with the pursuer offstage (Progress flag 0: this stage has one): the first time
 * (flag 1) +0 its time away; while flag 2 is clear and that runs, counting down (+0xC counting
 * up); then it tries to come in, by one of the three ways from a random first one onwards
 * (Summoner_Via1B / Summoner_Via1 / Summoner_Via0); having come, the wait restarts, +0x10 its
 * mode, flag 2 off and the cooldown +0x4 its +0x2D4 seconds. 1 if it came */
/* 0x002EC170 */
s32 Summoner_Offstage(u8 *o) {
    u8 *pu = (u8 *)gCharPursuer;
    Progress *p = gProgress;
    u32 k;
    s32 ok = 0;

    if (!Progress_TestFlag(p, 0)) {
        return 0;
    }
    if (!Progress_TestFlag(p, 1)) {
        AT(o, 0x0, u32) = Pursuer_RandomDelay((Pursuer *)pu);
        AT(o, 0xC, u32) = 0;
        Progress_SetFlag(p, 1);
    }
    if (AT(pu, 0x28, u8) != 0) {
        return 0;
    }
    if (!Progress_TestFlag(p, 2) && (AT(o, 0x0, u32) & 0x7FFFFFFF) != 0) {
        AT(o, 0x0, u32)--;
        if (AT(o, 0xC, u32) + 1 != 0) {
            AT(o, 0xC, u32)++;
        }
        return 0;
    }
    k = (u8)(u32)(3.0f * VCALL(gRandom, 0x18, f32 (*)(VObject *))(gRandom));
    do {
        switch (k++) {
        case 0:
            ok = (u8)Summoner_Via1B(o);
            break;
        case 1:
            ok = (u8)Summoner_Via1(o);
            break;
        case 2:
            ok = (u8)Summoner_Via0(o);
            break;
        }
    } while ((k & 0xFF) < 3 && !ok);
    if (ok == 1) {
        AT(o, 0xC, u32) = 0;
        AT(o, 0x10, u8) = AT(pu, 0x16C8, u8);
        Progress_ClearFlag(p, 2);
        AT(o, 0x4, u32) = VCALL((VObject *)pu, 0x2D4, s32 (*)(void *))(pu) * 30;
    }
    return ok;
}

/* 0x002EC3C0 */
void Summoner_LessCooldown(u8 *o, s32 sec) {
    u32 d = sec * 30;

    AT(o, 0x4, u32) = AT(o, 0x4, u32) >= AT(o, 0x4, u32) - d ? AT(o, 0x4, u32) - d : 0;
}

/* the summoner's cooldown (+0x4) to `sec` seconds / less by `sec` (not below 0) */
/* 0x002EC450 */
void Summoner_SetCooldown(u8 *o, s32 sec) {
    AT(o, 0x4, u32) = sec * 30;
}

/* 0x002EC470 */
void Summoner_Take(u8 *o, u8 kind) {
    summoner_take(o, kind);
}

/* a frame's loudest noise `n` (Noise_Make) against the summoner `o` (+0xC frames waited,
 * +0x11 its kind): in the current room only. Its level 0..3 by loudness (one less away from
 * doors with every door of the room shut). With the pursuer waiting offstage (+0xD0 / +0xD1,
 * not active), Progress flag 0 without flag 2, at least 3 s and its kind's wait gone, the
 * level's chance (percent) summons it (o +0 = 0x80000000); otherwise the level's second
 * chance - on the rest - sets condition 6 (hunted), as does mode 1 with any chance */
/* 0x002EC4F0 */
void Summoner_Noise(u8 *o, u8 *n) {
    Progress *p;
    VObject *rooms;
    u8 *pu;
    u8 *t;
    u32 i, d;
    s32 open = 0, lvl, call = 0, roll;

    if (n == NULL) {
        return;
    }
    p = gProgress;
    if (AT(n, 0x4, s32) != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        return;
    }
    rooms = gRooms;
    for (i = 0; i < 8; i++) {
        d = (u16)VCALL(rooms, 0x10, s32 (*)(VObject *, s32, u32))(rooms, AT(n, 0x4, s32), i & 0xFF);
        if (d < 0x190 && (u8)Progress_DoorOpen(p, d) == 1) {
            open = 1;
            break;
        }
    }
    lvl = AT(n, 0x0, u8) >= 0x80 ? 4 : AT(n, 0x0, u8) >= 0x60 ? 3 : AT(n, 0x0, u8) >= 0x40 ? 2
        : AT(n, 0x0, u8) >= 0x20 ? 1 : 0;
    if (AT(n, 0xC, u16) == 0xFFFF && !open && (s8)lvl < 4) {
        lvl = (s8)(lvl - 1);
    }
    if ((s8)lvl >= 4) {
        lvl = 3;
    }
    if ((s8)lvl < 0) {
        lvl = 0;
    }
    t = D_0047AC90 + (s8)lvl * 2;
    pu = (u8 *)gCharPursuer;
    if (pu != NULL && (AT(pu, 0xD0, u8) != 0 || AT(pu, 0xD1, u8) != 0)) {
        if (AT(pu, 0x28, u8) != 0) {
            return;
        }
        roll = 0;
        if (t[0] != 0) {
            if (t[0] == 100 || 100.0f * VCALL(gRandom, 0x18, f32 (*)(VObject *))(gRandom) <= (f32)t[0]) {
                roll = 1;
            }
        }
        call = roll && Progress_TestFlag(p, 0) && !Progress_TestFlag(p, 2);
        if (call) {
            u32 secs = (u16)(AT(o, 0xC, u32) / 30);

            if (secs < 3 || secs < D_00419DC0[AT(o, 0x11, u8)]) {
                call = 0;
            }
        }
    }
    if (call) {
        AT(o, 0x0, u32) = 0x80000000;
        return;
    }
    if (t[1] != 0 &&
        (100.0f - (f32)t[0]) * VCALL(gRandom, 0x18, f32 (*)(VObject *))(gRandom) <= (f32)t[1]) {
        Progress_SetCondBit(p, 6);
        return;
    }
    if ((u8)Progress_GameMode(p) == 1 && t[0] + t[1] != 0) {
        Progress_SetCondBit(p, 6);
    }
}

/* SceneGame +0x7A4 at the start of play in a room: whether the pursuer comes in (the progress
 * +0x28 says the room allows it; then by the stage, +0x64 4) or is placed elsewhere */
/* 0x002EC940 */
void Summoner_RoomStart(u8 *o) {
    u8 *pu = (u8 *)gCharPursuer;
    Progress *p, *q;

    if (pu == NULL || !(AT(pu, 0xD0, u8) != 0 || AT(pu, 0xD1, u8) != 0)) {
        return;
    }
    p = gProgress;
    if (Progress_TestFlag(p, 0x17)) {
        return;
    }
    if (Progress_TestFlag(p, 0x18) && AT(pu, 0x30, s32) != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        return;
    }
    if (AT(p, 0x1FBEC1, u8) != 0) {
        return;
    }
    q = gProgress;
    if (VCALL(q, 0x28, s32 (*)(Progress *, s32, s32))(q, VCALL(q, 0xC, s32 (*)(Progress *))(q), 2)) {
        if (AT(gCharPlayer, 0x30, s32) == AT(pu, 0x30, s32)) {
            return;
        }
        if ((u8)VCALL(p, 0x64, s32 (*)(Progress *))(p) != 4) {
            return;
        }
        if (!Pursuer_IsOwnRoom((Pursuer *)pu, -1)) {
            return;
        }
        summoner_take(o, 1);
        return;
    }
    if (VCALL(gCamDirector, 0x38, s32 (*)(VObject *))(gCamDirector)) {
        return;
    }
    if (!(u8)Summoner_Offstage(o)) {
        Summoner_InPlay(o);
    }
}

/* 0x002ECB50 */
void Summoner_Reset(u8 *p) {
    AT(p, 0xC, s32) = 0;
    AT(p, 0x4, s32) = 0;
    AT(p, 0x0, s32) = 0;
    AT(p, 0x8, s32) = -1;
    AT(p, 0x10, u8) = 0xFF;
    AT(p, 0x11, u8) = 0;
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

            if (next == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress) && (Npc_ExitKind(p, PU(p, 0x17B0, u8)) & 0xFF) == 5 &&
                PU(p, 0x16C8, u8) != 4) {
                PURSUER_STEP_DONE(p) = 1;
                return;
            }
            if (!(Pursuer_MayUseExit(p, PU(p, 0x17B0, u8)) & 0xFF)) {
                return;
            }
            Pursuer_ThroughDoor(p, PU(p, 0x17B0, u8));
            return;
        }
        Pursuer_SetMove(p, &kPursuerWaitMove);
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
    if (Npc_InPlayedRoom(p) != 0 && p->c.a.room == p->c.heard.room && PU(p, 0x16CC, u8) == 5 && p->c.moveMode == 8) {
        f32 v[4] __attribute__((aligned(16)));

        VCALL(gNavMesh, 0xC, void (*)(void *, s32, f32 *))(gNavMesh, p->c.heard.tri, v);
        if (Npc_SeesPoint(p, p->c.heard.tri, v) != 0) {
            return 0;
        }
    }
    room = p->c.heard.room;
    side = -1;
    switch (PU(p, 0x16CB, u8)) {
    case 5:
        side = Npc_CharSideBehind(p, gCharPlayer);
        room = gCharPlayer->a.room;
        break;
    case 1:
        side = Npc_CharSideBehind(p, gCharPartner);
        room = gCharPartner->a.room;
        break;
    }
    if (Character_Route(&p->c, room, side, -1, -1) >= 0) {
        if (PU(p, 0x16C8, u8) != 0 && PU(p, 0x16C8, u8) != 4) {
            PU(p, 0x16F2, u8) = 1;
            PU(p, 0x16F1, u8) = 1;
        }
        if (PU(p, 0x16CA, u8) < PU(p, 0x16CC, u8)) {
            if (Npc_InPlayedRoom(p) != 0 && PU(p, 0x16F8, u8) == 0) {
                PU(p, 0x16F3, u8) = 1;
            }
            PU(p, 0x16CA, u8) = PU(p, 0x16CC, u8);
            PU(p, 0x16C9, u8) = PU(p, 0x16CB, u8);
        }
        VCALL(p, 0xD0, void (*)(Pursuer *))(p);
        return 1;
    }
    Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    PU(p, 0x1544, u8) = 0;
    PU(p, 0x1545, u8) = 0;
    PU(p, 0x1546, u8) = 0;
    return 0;
}

/* ---- batch 13 ---- */

extern const PTMF Pursuer_StateHitOver_ptmf2, Pursuer_StateTurnAnim_ptmf, D_003ECFA0;

/* vtable +0x110: a grab ordered by the progress (Hewie, kinds 10..15): take position, or
 * refuse (-1) */
/* 0x0029C570 */
s32 Pursuer_GrabOrder(Pursuer *p) {
    Progress *pr = gProgress;
    Progress *pr2;
    u32 who, how, kind;
    Character *c;

    if ((Progress_HasRelationCmd(pr, *(u8 *)&p->c.a.slot) & 0xFF) != 1) {
        return -1;
    }
    if (Npc_InPlayedRoom(p) == 0) {
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
    if (!(((s32 (*)(Pursuer *, Character *, u32))Actor_TriFreeFor)(p, (Character *)p, 0) & 0xFF)) {
        SlotCmd_Cancel(pr, *(u8 *)&p->c.a.slot);
        return -1;
    }
    if ((how & 0xFF) == 1) {
        if (p->c.moveSub != 9 && p->c.moveSub != 0xA) {
            f32 pos[4] __attribute__((aligned(16)));
            f32 h;
            s32 tri = Pursuer_GrabHewieSpot(p, kind, &h, pos);

            if (tri != -1 && (Actor_TriFree(p, tri, (s32)pos) & 0xFF) == 1 && (u32)tri == Actor_TriTo(&p->c.a, pos, 0x28020028)) {
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
        s32 tri = Pursuer_GrabHewieSpot(p, 0xF, &h, pos);

        if (tri != -1 && (Actor_TriFree(p, tri, (s32)pos) & 0xFF) == 1 && (u32)tri == Actor_TriTo(&p->c.a, pos, 0x28020028)) {
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
/* 0x0027E790 */
void Pursuer_SearchRoom(Pursuer *p) {
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
    if (Npc_InPlayedRoom(p) != 0) {
        s32 left;

        while (PU(p, 0x1620, u8) != 0 && PU(p, 0x1621, u8) != 0xFF) {
            Npc_RouteDrop(p);
        }
        if (PU(p, 0x16CA, u8) != 2 && PU(p, 0x16CA, u8) != 7 && Npc_ReachedRoom(p) == 0) {
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
                Pursuer_AddSearchStops(p, ((n & 0xFF) - left) & 0xFF);
            }
            if ((s32)(n & 0xFF) > 0 && (n & 0xFF) != 0xFF) {
                PU(p, 0x1794, s32) = 1800;
            } else {
                PU(p, 0x1794, s32) = 0;
            }
        } else {
            PU(p, 0x1794, s32) = 1800;
        }
    } else if (Npc_ReachedRoom(p) != 0) {
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
/* 0x00287CD0 */
void Pursuer_StateHitReaction(Pursuer *p) {
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
            Character_RootMoveMasked(&p->c);
            return;
        }
        Pursuer_PlayAnim(p, a + 1);
        PU(p, 0x1628, s32) = 0;
        PU(p, 0x1784, s32) = 0;
        Actor_SetState(&p->c.a, &Pursuer_StateHitOver_ptmf2);
        return;
    }
    PU(p, 0x1624, s32) = MOTION_ANIM(p);
    PU(p, 0x1628, s32) = 0;
    PURSUER_STEP_NEXT(p) = 0;
}

/* Hewie bites (+0x104: 10..15, where): the reaction animation, and the bite's damage from the
 * table +0x173C added to +0x16BC (up to 1000; see Pursuer_StateHewieHolds); other kinds: Hewie is sent
 * state 7 */
/* 0x00286B90 */
void Pursuer_StateHewieBites(Pursuer *p) {
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
    Motion_PlayOwnBlend(p->c.motion, PU(p, 0x1624, s32), -1);
    PU(p, 0x1628, s32) = 0;
    PU(p, 0x1634, f32) = AT(p, 0x10C, f32);
    d = AT(p, 0x10C, f32) - p->c.a.angle[1];
    if (d <= 0.0f) {
        d = -d;
    }
    PU(p, 0x1638, f32) = (p->c.unk104[0] != 0xF ? 0.125f : 0x1.5551d60000000p-4f /* 0.08333 */) * d;
    PU(p, 0x1784, s32) = 0;
    Actor_SetState(&p->c.a, &Pursuer_StateTurnAnim_ptmf);
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
        if (!(Npc_PlanBesideDoor(p, (u8)p->c.unk100, p->c.unk104[0] != 0) & 0xFF)) {
            PU(p, 0x16EF, u8) = 1;
            return;
        }
    } else {
        p->c.unk104[0] = 0;
        while ((Npc_PlanBesideDoor(p, (u8)p->c.unk100, p->c.unk104[0] != 0) & 0xFF) != 1) {
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
        Npc_StepPath(p, (u32 *)&tri, pos, 0x1.99999a0000000p-4f /* 0.1 */);
        if (tri != -1) {
            t = Npc_TurnWayTo(p, pos, 0x1.657186p+0f /* 80 degrees */, 0x1.657186p+1f /* 160 degrees */) & 0xFF;
        }
    }
    if ((t & 0xFF) == 0xFF) {
        if (PU(p, 0x1590, f32) < 10.0f) {
            Pursuer_PlayAnim(p, VCALL(p, 0x324, s32 (*)(Pursuer *))(p));
        } else {
            VCALL(p, 0x128, void (*)(Pursuer *))(p);
        }
    } else {
        Pursuer_TurnOnSpot(p, t);
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
        if (!(Pursuer_TargetOutOfReach(p) & 0xFF)) {
            PU(p, 0x16EF, u8) = 1;
        }
        return;
    }
    if (!(PU(p, 0x1590, f32) < 4.0f)) {
        Npc_StepPath(p, (u32 *)&tri, pos, 0x1.99999a0000000p-4f /* 0.1 */);
    }
    if (PU(p, 0x1788, s32) != 0x400) {
        s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

        if (!((over ^ 1) & 0xFF) && tri != -1) {
            u32 t = Npc_TurnWayTo(p, pos, 0x1.657186p+0f /* 80 degrees */, 0x1.657186p+1f /* 160 degrees */) & 0xFF;

            if (t != 0xFF) {
                Pursuer_TurnOnSpot(p, t);
            }
        }
        if ((p->c.unk128 < p->c.unk124) == 1) {
            if (PU(p, 0x1628, s32) == 0 && PU(p, 0x15A4, u32) == Actor_TriTo(&p->c.a, (f32 *)((u8 *)p + 0x15B0), -1)) {
                VCALL(p, 0xD8, s32 (*)(Pursuer *))(p);
                PU(p, 0x1628, s32) = 1;
            }
            done = Npc_WalkPathStride(p, p->c.unk128) & 0xFF;
        } else if (PU(p, 0x1590, f32) != 0.0f) {
            PU(p, 0x16EF, u8) = 1;
        } else {
            done = 1 & 0xFF;
        }
    } else {
        s32 over;

        Character_RootMoveMasked(&p->c);
        over = MOTION_AT(p, 0x550, f32) <= 0.0f;
        if (!((over ^ 1) & 0xFF)) {
            f32 d = 1.0f;

            if (!(MOTION_KEYS(p) & MOTION_KEY_END)) {
                if (!(Angle_Wrap(Actor_HeadingTo(&p->c.a, pos) - p->c.a.angle[1]) <= 0.0f)) {
                    d = Angle_Wrap(Actor_HeadingTo(&p->c.a, pos) - p->c.a.angle[1]);
                } else {
                    d = -Angle_Wrap(Actor_HeadingTo(&p->c.a, pos) - p->c.a.angle[1]);
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
/* 0x0029A710 */
s32 Pursuer_GrabHewieBehind(Pursuer *p) {
    f32 pos[4] __attribute__((aligned(16)));
    f32 h;
    s32 tri;

    if (Character_Held(&p->c) & 0xFF) {
        return 0;
    }
    tri = Pursuer_GrabHewieSpot(p, 0xF, &h, pos);
    if (tri != -1 && (Actor_TriFree(p, tri, (s32)pos) & 0xFF) == 1 && (u32)tri == Actor_TriTo(&p->c.a, pos, 0x28020028)) {
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

extern const PTMF Pursuer_StateRelation1_ptmf, Pursuer_StateRelation2_ptmf, D_003ECC00, D_003ECC10, D_003EC910, D_003EC920;

/* look around: the head swept by a sine over vtable +0x2DC frames (full swing +0x2E0, half
 * at the ends), then back to straight ahead */
/* 0x0028FF30 */
void Pursuer_LookAround(Pursuer *p) {
    s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;
    s32 t;

    if (((over ^ 1) & 0xFF) == 1) {
        Character_RootMoveMasked(&p->c);
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
        MOTION_AT(p, 0x858, f32) = Angle_Wrap(MOTION_AT(p, 0x858, f32) +
                                                  amp * msl_cosf(Angle_Wrap(ph * (0x1.921fb60000000p+2f /* 6.2831855 */ / VCALL(p, 0x2DC, f32 (*)(Pursuer *))(p)))));
        PU(p, 0x1574, f32) = Angle_Wrap(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
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
/* 0x0027EAF0 */
void Pursuer_SearchRouteIn(Pursuer *p) {
    if (Npc_InPlayedRoom(p) != 0) {
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
                    Pursuer_AddSearchStops(p, (n - left) & 0xFF);
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

            if (!(Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]) <= 0.0f)) {
                a = Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
            } else {
                a = -Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
            }
            if (a < 0x1.921fb60000000p+1f /* 3.1415927 */ * VCALL(p, 0x2EC, f32 (*)(Pursuer *))(p) / 180.0f) {
                s32 can;

                if (p->target->a.unk2D == 1 || (Progress_TestFlag(gProgress, 0xE) & 0xFF) == 1 || gCharPlayer->moveSub == 0x10) {
                    can = 0;
                } else {
                    can = 1 & 0xFF;
                }
                if (can != 0) {
                    if (((s32 (*)(Pursuer *, Character *, u32))Actor_TriFreeFor)(p, gCharPlayer, 0) != 0) {
                        Actor_SetState(&p->c.a, hit);
                        Relation_Request(gProgress, *(u8 *)&p->c.a.slot, 1, 9, 0, a5, 0.0f);
                        Actor_SetState(&p->c.a, after);
                        return;
                    }
                    VCALL(p, 0x134, void (*)(Pursuer *))(p);
                    return;
                }
                VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, 3);
                Pursuer_PickFromTable(p);
                return;
            }
        }
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x16);
        return;
    }
    {
        Character *t = gCharPlayer != NULL ? gCharPlayer : p->target;
        f32 h = Actor_HeadingTo(&p->c.a, t->a.pos);

        Npc_TurnToward(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    }
}

/* 0x0028A190 */
void Pursuer_StateCloseOnFionaA(Pursuer *p) {
    Pursuer_CloseOnFiona(p, &Pursuer_StateRelation1_ptmf, &Pursuer_StateCloseA_ptmf, 8);
}

/* 0x0028AB10 */
void Pursuer_StateCloseOnFionaB(Pursuer *p) {
    Pursuer_CloseOnFiona(p, &Pursuer_StateRelation2_ptmf, &Pursuer_StateCloseB_ptmf, 6);
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
    PU(p, 0x1588, f32) = Npc_FootDistance(p, gCharPlayer);
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

        if (d < Pursuer_GroundGained(p) && !(d < 0.0f) && Npc_SameFloor(&p->c.a, &gCharPlayer->a) != 0) {
            f32 a;

            if (!(Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]) <= 0.0f)) {
                a = Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
            } else {
                a = -Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
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
        if (Npc_InPlayedRoom(p) == 0) {
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

extern const PTMF Pursuer_StateDownAndUp_ptmf, Pursuer_StateAttackActive_ptmf, Pursuer_StateAttackStep_ptmf, D_003ECEB0, D_003ECEA0;
extern PTMF kPursuerMoveA, kPursuerMoveB;   /* more moves (see Pursuer_SetMove) */

/* vtable +0x...: knocked down (0x1709 / 0x1804 falling forward if there's room / 0x1800);
 * out of health: down for good (stance cleared), and the progress counter of knock-outs +0xFBC
 * goes up (to 9999) */
/* 0x002885B0 */
void Pursuer_StateKnockedDown(Pursuer *p) {
    PU(p, 0x16EC, u8) = 0;
    PURSUER_STEP_NEXT(p) = 0;
    if (p->c.unk104[0] == 0x1709) {
        Pursuer_PlayAnimBlend(p, 0x1709);
    } else if (((s32 (*)(Pursuer *, Character *, u32))Actor_TriFreeFor)(p, (Character *)p, 0) != 0 && Npc_WalkableAhead(p) != 0) {
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
    Actor_SetState(&p->c.a, &Pursuer_StateDownAndUp_ptmf);
    Pursuer_StateDownAndUp(p);
}

/* the next step of the attack (+0x1728 attack, +0x172C step; table +0x1724 of step entries
 * of +0x171C, 0x24 bytes): its animation, kind 1/2/4 (move 0x1A..0x1C) or 6 (the hit) */
/* 0x0028B970 */
void Pursuer_AttackNextStep(Pursuer *p) {
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

                if (Pursuer_GroundGained(p) > 50.0f) {
                    reach = Pursuer_GroundGained(p);
                }
                go = Actor_Distance(&p->c.a, p->target->a.pos) < reach;
            }
            if (go) {
                u8 *e = PU(p, 0x171C, u8 *) + PU(p, 0x1724, s8 *)[PU(p, 0x1728, s32) * 4 + PU(p, 0x172C, s8)] * 0x24;

                PU(p, 0x1760, u8) = 0;
                Pursuer_PlayAnim(p, AT(e, 0x0, s32));
                PU(p, 0x16F7, u8) = AT(e, 0x1D, u8);
                PU(p, 0x1784, s32) = 0;
                switch (AT(e, 0x10, u8)) {
                case 6:
                    Actor_SetState(&p->c.a, &Pursuer_StateAttackActive_ptmf);
                    Pursuer_StateAttackActive(p);
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
                Actor_SetState(&p->c.a, &Pursuer_StateAttackStep_ptmf);
                Pursuer_StateAttackStep(p);
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
            Pursuer_SetMove(p, &kPursuerMoveB);
            PURSUER_STEP_DONE(p) = 0;
            PU(p, 0x17B4, s32) = 0;
            return;
        }
        if (PU(p, 0x17B4, s32) != 0) {
            Pursuer_SetMove(p, &kPursuerIdleMove);
        } else if (!(Character_PathRemaining2(&p->c) <= 0.0f) &&
                   PU(p, 0x17B0, u8) != (VCALL(gRooms, 0x3C, u32 (*)(VObject *, u32, s32))(gRooms, PU(p, 0x138C, u16), p->c.a.room) & 0xFF)) {
            p->c.unk124 = p->c.unk128;
            Pursuer_SetMove(p, &kPursuerWaitMove);
        }
        break;
    case 4:
        if (PURSUER_STEP_DONE(p) == 1) {
            if (p->c.unk128 < p->c.unk124) {
                Pursuer_SetMove(p, &kPursuerMoveA);
            } else {
                Pursuer_SetMove(p, &kPursuerWaitMove);
            }
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 5:
        if (PURSUER_STEP_DONE(p) == 1 && PU(p, 0x1664, s32) == 0 && Pursuer_MayUseExit(p, PU(p, 0x17B0, u8)) != 0) {
            PURSUER_STEP_DONE(p) = 0;
            Pursuer_ThroughDoor(p, PU(p, 0x17B0, u8));
        }
        break;
    }
}

/* the head by +0x1710: 1 at Fiona (if seen), 2 at Hewie (if seen, a bit above him), 3 at the
 * goal point if in view, 5 at +0x1700; else ahead */
/* 0x00284040 */
void Pursuer_HeadLook(Pursuer *p) {
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
            Motion_LookAt(p->c.motion, v, &pitch, &yaw);
            sp = VCALL(p, 0xA4, f32 (*)(Pursuer *))(p);
            Motion_EaseTilt(p->c.motion, pitch, yaw, sp, VCALL(p, 0xA4, f32 (*)(Pursuer *))(p));
            PU(p, 0x1574, f32) = Angle_Wrap(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
            return;
        }
        break;
    case 3:
        if (PU(p, 0x15A4, u32) == Actor_TriTo(&p->c.a, (f32 *)((u8 *)p + 0x15B0), 0x40080) &&
            Eye_CanSee(p, p->c.a.pos, (f32 *)((u8 *)p + 0x15B0), PU(p, 0x1574, f32), PU(p, 0x1580, f32), PU(p, 0x1584, f32)) != 0) {
            f32 a;

            if (!(Angle_Wrap(Actor_HeadingTo(&p->c.a, (f32 *)((u8 *)p + 0x15B0)) - p->c.a.angle[1]) <= 0.0f)) {
                a = Angle_Wrap(Actor_HeadingTo(&p->c.a, (f32 *)((u8 *)p + 0x15B0)) - p->c.a.angle[1]);
            } else {
                a = -Angle_Wrap(Actor_HeadingTo(&p->c.a, (f32 *)((u8 *)p + 0x15B0)) - p->c.a.angle[1]);
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
    PU(p, 0x1574, f32) = Angle_Wrap(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
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
        Npc_StepPath(p, (u32 *)&tri, pos, 0x1.99999a0000000p-4f /* 0.1 */);
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
                u32 t = Npc_TurnWayTo(p, pos, 0x1.657186p+0f /* 80 degrees */, 0x1.657186p+1f /* 160 degrees */) & 0xFF;

                if (t != 0xFF) {
                    Pursuer_TurnOnSpot(p, t);
                }
            }
        }
        Npc_WalkPathStride(p, p->c.unk128);
    } else {
        s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

        if (!((over ^ 1) & 0xFF)) {
            s32 turn = 0;

            if (!(MOTION_KEYS(p) & MOTION_KEY_END)) {
                f32 d;

                if (!(Angle_Wrap(Actor_HeadingTo(&p->c.a, pos) - p->c.a.angle[1]) <= 0.0f)) {
                    d = Angle_Wrap(Actor_HeadingTo(&p->c.a, pos) - p->c.a.angle[1]);
                } else {
                    d = -Angle_Wrap(Actor_HeadingTo(&p->c.a, pos) - p->c.a.angle[1]);
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
        Character_RootMoveMasked(&p->c);
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
        Npc_StepPath(p, (u32 *)&tri, pos, 0x1.99999a0000000p-4f /* 0.1 */);
        if (tri != -1) {
            t = Npc_TurnWayTo(p, pos, 0x1.657186p+0f, 0x1.657186p+1f) & 0xFF;
        }
    }
    if ((t & 0xFF) != 0xFF) {
        Pursuer_TurnOnSpot(p, t);
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

extern const PTMF Pursuer_StateHitReact_ptmf2, Pursuer_StateHitReact_ptmf3, D_003ED2B0, D_003ED2C0, D_003ED2D0, D_003ED2E0, D_003ED2F0,
    D_003ED300, D_003ED310, D_003ED320, D_003ED330, D_003ED340, D_003ECDC0;

/* state: knocked down (anim 0x1709) and getting up again (0x1806/0x1802) */
/* 0x00288150 */
void Pursuer_StateDownAndUp(Pursuer *p) {
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
            Npc_TurnToRootMotion(p, -1);
            c0 = p->c.a.navMask;
            p->c.a.navMask = c0 | 0x80001;
            Character_RootMoveMasked(&p->c);
            p->c.a.navMask = c0;
        } else {
            Npc_TurnToRootMotion(p, -1);
            Character_RootMoveMasked(&p->c);
        }
        return;
    }
    if (p->c.a.unkC4 != 2 && AT(m, 0x55C, s32) == 0x1709) {
        PU(p, 0x1784, s32) = 0;
        Actor_SetState(&p->c.a, &Pursuer_StateHitReact_ptmf2);
        if (p->c.a.unkC4 != 2 && MOTION_ANIM(p) == 0x1709) {
            PU(p, 0x1624, s32) = 0x1806;
        } else {
            PU(p, 0x1624, s32) = MOTION_ANIM(p);
        }
        PU(p, 0x1628, s32) = 0;
        PURSUER_STEP_NEXT(p) = 0;
        anim = PU(p, 0x1624, s32);
        if (anim == 0x1807 || anim == 0x1803) {
            Actor_SetState(&p->c.a, &Pursuer_StateHitOver_ptmf);
            Pursuer_StateHitOver(p);
        } else {
            Actor_SetState(&p->c.a, &Pursuer_StateHitReaction_ptmf);
            Pursuer_StateHitReaction(p);
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
    Actor_SetState(&p->c.a, &Pursuer_StateHitReact_ptmf3);
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
        Npc_StepPath(p, &tri, pos, 0x1.99999ap-4f /* 0.1 */);
        if (tri != (u32)-1) {
            dir = Npc_TurnWayTo(p, pos, 0x1.657186p+0f /* 80 degrees */, 0x1.657186p+1f /* 160 degrees */) & 0xFF;
        }
    }
    if (p->c.unkE0 != 0) {
        if (p->c.unk104[1] == -1) {
            p->c.unk104[1] = VCALL(p, 0x328, s32 (*)(Pursuer *))(p);
        }
        if ((dir & 0xFF) == 0xFF) {
            Pursuer_PlayAnim(p, p->c.unk104[1]);
        } else {
            Pursuer_TurnOnSpot(p, dir);
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
        Pursuer_TurnOnSpot(p, dir);
        if (p->target != gCharPartner && PU(p, 0x16C8, u8) == 3) {
            anim = VCALL(p, 0x324, s32 (*)(Pursuer *))(p);
        } else {
            anim = VCALL(p, 0x328, s32 (*)(Pursuer *))(p);
        }
    }
    PU(p, 0x1624, s32) = anim;
    PU(p, 0x1628, s32) = PU(p, 0x15A4, u32) == Actor_TriTo(&p->c.a, (f32 *)((u8 *)p + 0x15B0), -1) ? 1 : 0;
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
        Npc_StepPath(p, &tri, pos, 0x1.99999ap-4f /* 0.1 */);
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
                u32 dir = Npc_TurnWayTo(p, pos, 0x1.657186p+0f /* 80 degrees */, 0x1.657186p+1f /* 160 degrees */) & 0xFF;

                if (dir != 0xFF) {
                    Pursuer_TurnOnSpot(p, dir);
                }
            }
        }
        Npc_WalkPathStride(p, 0);
        return;
    }
    if (!(AT(m, 0x550, f32) > 0.0f)) {
        if ((AT(AT(m, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0) {
            VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x12);
        } else {
            f32 a;

            if (!(Angle_Wrap(Actor_HeadingTo(&p->c.a, pos) - p->c.a.angle[1]) <= 0.0f)) {
                a = Angle_Wrap(Actor_HeadingTo(&p->c.a, pos) - p->c.a.angle[1]);
            } else {
                a = -Angle_Wrap(Actor_HeadingTo(&p->c.a, pos) - p->c.a.angle[1]);
            }
            if (a < 0x1.99999ap-4f /* 0.1 */) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x12);
            }
        }
    }
    Character_RootMoveMasked(&p->c);
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
                Actor_PlaySound(&p->c.a, 0x20, 7, 0, 0, NULL);
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
            Pursuer_StanceByFiona(p);
            VCALL(p, 0x13C, void (*)(Pursuer *))(p);
        }
        PURSUER_STEP_DONE(p) = 0;
        PU(p, 0x1761, u8) = 0;
        if (PU(p, 0x16F5, u8) != 0 && PU(p, 0x16C8, u8) != 4 &&
            VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(gProgress, 8) == 0) {
            Actor_PlaySound(&p->c.a, 0x1F, 7, 0, 0, NULL);
        }
        PU(p, 0x16F5, u8) = 0;
        if (PU(p, 0x16C8, u8) != 4) {
            PU(p, 0x16F6, u8) = 1;
        }
        return;
    }
    if (Motion_EventFlags(p->c.motion, 0, 0, 1) & 0xFF & 2) {
        s32 may;

        /* Pursuer_MayGoForTarget, inline */
        if (p->target->a.unk2D == 1 || (Progress_TestFlag(gProgress, 0xE) & 0xFF) == 1) {
            may = 0;
        } else {
            may = gCharPlayer->moveSub != 0x10;
        }
        if (may && (PU(p, 0x1761, u8) & 1) && p->target == gCharPlayer && PU(p, 0x175C, s32) == 0x1E &&
            PU(p, 0x16C8, u8) != 4) {
            f32 a;

            if (!(Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]) <= 0.0f)) {
                a = Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
            } else {
                a = -Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
            }
            /* in front or behind */
            VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, a < 0x1.921fb6p+0f /* 90 degrees */ ? 8 : 9);
            PU(p, 0x1761, u8) = 0;
            Pursuer_PickFromTable(p);
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
    if (PU(p, 0x16C8, u8) == 0 && Npc_InPlayedRoom(p) == 0) {
        if (PU(p, 0x1594, s32) != gCharPlayer->a.room || Npc_ExitSideBehind(p) != Npc_CharSideBehind(p, gCharPlayer)) {
            p->c.unk124 = p->c.unk128;
            VCALL(p, 0xB0, void (*)(Pursuer *))(p);
            p->c.unk1538 = 0;
            p->c.unk1530 = 0;
            p->c.unk1534 = 0;
            PU(p, 0x17B4, s32) = 0;
        }
        if (p->c.a.room == gCharPlayer->a.room && Npc_ExitSideBehind(p) == Npc_CharSideBehind(p, gCharPlayer)) {
            PU(p, 0x17B4, s32) = 150;
        }
    }
    switch (PU(p, 0x17AC, u32)) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (PURSUER_STEP_DONE(p) == 1) {
            Pursuer_SetMove(p, &kPursuerMoveB);
            PURSUER_STEP_DONE(p) = 0;
            PU(p, 0x17B4, s32) = 0;
        } else if (PU(p, 0x17B4, s32) != 0) {
            Pursuer_SetMove(p, &kPursuerIdleMove);
        } else if (!(Character_PathRemaining2(&p->c) <= 0.0f)) {
            u8 want = PU(p, 0x17B0, u8);

            if (want != (VCALL(gRooms, 0x3C, s32 (*)(VObject *, u32, s32))(gRooms, PU(p, 0x138C, u16), p->c.a.room) & 0xFF)) {
                p->c.unk124 = p->c.unk128;
                Pursuer_SetMove(p, &kPursuerWaitMove);
            }
        }
        break;
    case 4:
        if (PURSUER_STEP_DONE(p) == 1) {
            Pursuer_SetMove(p, p->c.unk128 < p->c.unk124 ? &kPursuerMoveA : &kPursuerWaitMove);
            PURSUER_STEP_DONE(p) = 0;
        }
        break;
    case 5:
        if (PURSUER_STEP_DONE(p) == 1 && PU(p, 0x1664, s32) == 0 && Pursuer_MayUseExit(p, PU(p, 0x17B0, u8)) != 0) {
            PURSUER_STEP_DONE(p) = 0;
            Pursuer_ThroughDoor(p, PU(p, 0x17B0, u8));
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

    if (Npc_InPlayedRoom(p) == 0) {
        p->c.a.navTri = -1;
        if (p->c.a.unkC4 != 2) {
            Pursuer_SearchRouteIn(p);
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
        ang = Angle_Wrap(0x1.921fb6p+1f /* pi */ + ang);
        VCALL(p, 0x28, void (*)(Pursuer *, s32, f32 *, s32))(p, p->c.a.navTri, &ang, 0);
        if (!__ptmf_cmpr(step, &D_003ED460) || !__ptmf_cmpr(step, &D_003ED470) || !__ptmf_cmpr(step, &D_003ED480)) {
            Pursuer_StanceStep(p);
            Pursuer_SearchRouteIn(p);
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
                    Npc_PlanToRoomObject(p);
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

        if (Progress_ExitOpen(pr, p->c.a.room, p->c.door) & 0xFF) {
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
    Character_MarkObjects(&p->c);
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
        u32 m = Npc_WhoSeen(p) & 0xFF;

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
        Motion_RootMovement(p->c.motion, v, 0.0f);
        Character_RootTurn(&p->c);
        sceVu0ApplyMatrix(v, (void *)((u8 *)p + 0x60), v);
        sceVu0AddVector(p->c.a.pos, p->c.a.pos, v);
        p->c.a.pos[3] = 1.0f;
        return;
    }
    turn = 0;
    if (PU(p, 0x1624, s32) != PU(p, 0x15A4, s32) && Npc_ReachedRoom(p) != 0) {
        u8 k = PU(p, 0x16C8, u8);

        if (k != 4 && (k == 3 || k == 1 || k == 2 || k == 0)) {
            if (p->c.unk104[0] == (s32)(Npc_PlanFromDoor(p, PU(p, 0x15A4, s32), (f32 *)((u8 *)p + 0x15B0), (u8)p->c.unk100) & 0xFF)) {
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
                Motion_Play(p->c.motion, 0x703, -1);
            } else {
                Motion_PlayOwnBlend(p->c.motion, 0x703, -1);
            }
            VCALL(p, 0x9C, void (*)(Pursuer *, s32, f32 *))(p, 3, ofs);
            p->c.a.navTri = Actor_DoorFront(p, p->c.unk100, 0, ofs, end);
            Actor_SetState(&p->c.a, &D_003ECFD0);
            return;
        }
        if (anim != 0x700 && PU(p, 0x16C8, u8) != 4 && turn) {
            p->c.unk104[0] = 0;
            PU(p, 0x1624, s32) = PU(p, 0x15A4, s32);
            Motion_Play(p->c.motion, anim == 0x701 ? 0x706 : 0x705, -1);
            return;
        }
    } else {
        VCALL(gNavMesh, 0x5C, void (*)(void *, s32, s32, f32 *))(gNavMesh, p->c.unk100, 1, end);
        if (p->c.a.pos[1] - end[1] < 4.0f) {
            if (anim == 0x706) {
                Motion_Play(p->c.motion, 0x707, -1);
            } else {
                Motion_PlayOwnBlend(p->c.motion, 0x707, -1);
            }
            Actor_SetState(&p->c.a, &D_003ECFE0);
            return;
        }
        if (anim == 0x704 || PU(p, 0x16C8, u8) == 4) {
            VCALL(p, 0x9C, void (*)(Pursuer *, s32, f32 *))(p, 2, ofs);
            p->c.a.navTri = Actor_DoorFront(p, p->c.unk100, 1, ofs, end);
        } else if (turn) {
            p->c.unk104[0] = 1;
            PU(p, 0x1624, s32) = PU(p, 0x15A4, s32);
            Motion_Play(p->c.motion, anim == 0x705 ? 0x702 : 0x701, -1);
            return;
        }
    }
    /* the next step, alternating legs */
    switch (anim) {
    case 0x700:
    case 0x702:
        Motion_Play(p->c.motion, 0x701, -1);
        break;
    case 0x701:
        Motion_Play(p->c.motion, 0x702, -1);
        break;
    case 0x705:
        Motion_Play(p->c.motion, 0x706, -1);
        break;
    case 0x704:
    case 0x706:
        Motion_Play(p->c.motion, 0x705, -1);
        break;
    }
}

extern const PTMF D_003ED110, Pursuer_StateEndStep_ptmf4;

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
            Actor_PlaySound(&p->c.a, 0x21, 7, 0, 0, NULL);
        }
        Actor_SetState(&p->c.a, &D_003ED110);
        VCALL(p, 0x240, void (*)(Pursuer *))(p);
        return;
    }
    default:
        PURSUER_STEP_DONE(p) = 1;
        return;
    }
    Actor_SetState(&p->c.a, &Pursuer_StateEndStep_ptmf4);
    if (((AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0) == 1) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    } else {
        Character_RootMoveMasked(&p->c);
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
            Pursuer_SearchRoom(p);
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
            Pursuer_PickFromTable(p);
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
            Pursuer_PickFromTable(p);
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
        ptmf_set((PTMF *)((u8 *)p + 0x174C), !(Npc_ReachedRoom(p) & 0xFF) ? &D_003ECC80 : &D_003ECC90);
        PU(p, 0x1758, s32) = -1;
    } else if (PU(p, 0x16ED, u8) == 1) {
        VCALL(p, 0x13C, void (*)(Pursuer *))(p);
    } else if (PU(p, 0x16F3, u8) == 1) {
        if (Npc_ReachedRoom(p) & 0xFF) {
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

        if (!(Angle_Wrap(PU(p, 0x1568, f32) - p->c.a.angle[1]) <= 0.0f)) {
            d = Angle_Wrap(PU(p, 0x1568, f32) - p->c.a.angle[1]);
        } else {
            d = -Angle_Wrap(PU(p, 0x1568, f32) - p->c.a.angle[1]);
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
                Character_RootTurn(&p->c);
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
        PU(p, 0x1574, f32) = Angle_Wrap(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
        if (((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF) {
            return;
        }
        Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
        Pursuer_MotionGroup(p);
    } else if (Npc_TurnToward(p, PU(p, 0x1568, f32), VCALL(p, 0xA0, f32 (*)(Pursuer *))(p)) != 0.0f) {
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
        if (Progress_ExitOpen(pr, p->c.a.room, (u8)p->c.unk100) != 0) {
            Pursuer_DropDoor(p);
            return;
        }
        break;
    case 1:
    case 3:
        if (!(Progress_ExitOpen(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF)) {
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
    if ((Progress_ExitOpen(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1) {
        p->c.moveSub = 0x15;
        Npc_DoorShut(p, (u8)p->c.unk100);
    } else {
        p->c.moveSub = 0x14;
        Npc_DoorShut2(p, (u8)p->c.unk100);
        if (DoorHold_Usable(pr, p->c.a.room, (u8)p->c.unk100) != 0) {
            Progress *g = gProgress;

            DoorHold_Release(g, p->c.a.room, (u8)p->c.unk100);
            DoorHold_Take(g, p->c.a.room, (u8)p->c.unk100, *(u8 *)&p->c.a.slot);
        }
    }
    switch (p->c.unk104[0]) {
    case 0:
        Motion_PlayOwnBlend(p->c.motion, 0x600, -1);
        break;
    case 2:
        Motion_PlayOwnBlend(p->c.motion, 0x602, -1);
        break;
    case 1:
        Motion_PlayOwnBlend(p->c.motion, 0x601, -1);
        break;
    case 3:
        Motion_PlayOwnBlend(p->c.motion, 0x603, -1);
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
            s32 i = Motion_AnimIndex(m, anim);
            u16 fl = i != -1 ? AT(AT(m, 0x874, u8 *) + i * 6, 0x4, u16) : 0;

            if (fl & 4) {
                return 0;
            }
        }
        Motion_PlayTable(p->c.motion, anim, -1);
    } else {
        Motion_PlayTable(m, anim, -1);
    }
    return 1;
}

/* turn on the spot in direction dir (0..3); 0x405.. instead of 0x400.. in mode +0xC4 1 */
/* 0x00297300 */
s32 Pursuer_TurnOnSpot(Pursuer *p, u32 dir) {
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
/* 0x00297C60 */
void Pursuer_MotionGroup(Pursuer *p) {
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
/* 0x0027F5F0 */
void Pursuer_CarryOnLying(Pursuer *p) {
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

extern const PTMF Pursuer_StateEndStep_ptmf5;

/* state: hit, flinching from direction +0x104 (0..3; bit 1 when the hit only staggers it) */
/* 0x00288970 */
void Pursuer_StateFlinch(Pursuer *p) {
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
    Actor_SetState(&p->c.a, &Pursuer_StateEndStep_ptmf5);
    if (((AT(AT(p->c.motion, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0) == 1) {
        PURSUER_STEP_DONE(p) = 1;
        PURSUER_STEP_NEXT(p) = 1;
    } else {
        Character_RootMoveMasked(&p->c);
    }
}

/* Fiona seen again: back to the chase (mode 0) if she's in reach, else follow (mode 1) */
static void Pursuer_Resight(Pursuer *p) {
    if (Npc_ReachedRoom(p) != 0 && Npc_ExitSideBehind(p) == Npc_CharSideBehind(p, gCharPlayer)) {
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
            Pursuer_SearchRouteIn(p);
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
            if (Npc_ReachedRoom(p) != 0 && Npc_ExitSideBehind(p) == Npc_CharSideBehind(p, gCharPlayer)) {
                PU(p, 0x16C8, u8) = 0;
                VCALL(p, 0x2BC, void (*)(Pursuer *))(p);
                PU(p, 0x16C9, u8) = 6;
            }
            PU(p, 0x16F2, u8) = 0;
        } else if (Npc_ReachedRoom(p) != 0) {
            PU(p, 0x16C8, u8) = 2;
        } else if (PU(p, 0x16F4, u8) != 0) {
            PU(p, 0x16C8, u8) = 3;
            VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
            PU(p, 0x16F4, u8) = 0;
        }
        if (PU(p, 0x16F2, u8) != 0) {
            Pursuer_SearchRoom(p);
            PU(p, 0x16F2, u8) = 0;
        }
        break;
    case 2:
        if (PU(p, 0x1544, u8) == 1) {
            Pursuer_Resight(p);
        } else if (PU(p, 0x16F2, u8) != 0) {
            if (!(Npc_ReachedRoom(p) & 0xFF)) {
                PU(p, 0x16C8, u8) = 1;
                Pursuer_SearchRoom(p);
            } else if (!(Npc_InPlayedRoom(p) & 0xFF) || PU(p, 0x1620, u8) != 0) {
                Pursuer_SearchRoom(p);
            }
            PU(p, 0x16F2, u8) = 0;
            PU(p, 0x16F4, u8) = 0;
        } else if (PU(p, 0x16F4, u8) != 0 || (PU(p, 0x17B4, s32) == 0 && !(Npc_InPlayedRoom(p) & 0xFF))) {
            PU(p, 0x16C8, u8) = 3;
            VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
            PU(p, 0x16F4, u8) = 0;
        }
        break;
    case 3:
        if (PU(p, 0x1544, u8) == 1) {
            Pursuer_Resight(p);
        } else if (PU(p, 0x16F2, u8) != 0) {
            PU(p, 0x16C8, u8) = Npc_ReachedRoom(p) != 0 ? 2 : 1;
            Pursuer_SearchRoom(p);
            PU(p, 0x16F2, u8) = 0;
            PU(p, 0x16F4, u8) = 0;
        } else if (PU(p, 0x16F4, u8) != 0) {
            PU(p, 0x16F4, u8) = 0;
        } else if (!(Npc_InPlayedRoom(p) & 0xFF)) {
            if (PU(p, 0x1660, s32) != 0) {
                PU(p, 0x1660, s32)--;
            } else if (!(Npc_NearRoom(p, 1) & 0xFF)) {
                /* nothing left here: target Hewie instead (vtable +0xB4) */
                VCALL(p, 0xB4, void (*)(Pursuer *, Character *))(p, gCharPartner);
                PU(p, 0x16C8, u8) = (Npc_ReachedRoom(p) & 0xFF) ? 2 : 1;
                Pursuer_SearchRoom(p);
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

extern const PTMF Pursuer_AttackNextStep_ptmf3;

/* state: an attack step in progress (entry e of Pursuer_AttackNextStep). At the animation's hit key the
   strike lands where the entry's bone (+0x4) is, unless a wall is in the way; once over, either
   the next step of the attack or a rest of +0x1748's frames before the next one */
/* 0x0028B340 */
void Pursuer_StateAttackStep(Pursuer *p) {
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
        f32 h = Actor_HeadingTo(&p->c.a, p->target->a.pos);

        Npc_TurnToward(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    }
    if (p->c.unk14D0 <= 0) {
        Character_RootMoveMasked(&p->c);
    }
    if (Motion_EventFlags(p->c.motion, 0, -1, 1) & 0xFF & 2) {
        pr = gProgress;
        if (!(Progress_TestFlag(pr, 0xE) & 0xFF)) {
            u32 hit;
            f32 other[4] __attribute__((aligned(16))) = { 0.0f, 0.0f, 0.0f, 0.0f };

            if (!(((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF)) {
                f32 bone[4] __attribute__((aligned(16)));

                sceVu0CopyVector(bone, Skel_Bone(MOTION_AT(p, 0x810, u8 *), AT(e, 0x4, s32)) + 0xC);
                if (PU(p, 0x1788, s32) != 0 && Npc_CanWalkStraight(p, bone) != 0) {
                    /* the blow is blocked: back to the idle */
                    Pursuer_PlayAnim(p, VCALL(p, 0x320, s32 (*)(Pursuer *))(p));
                    PU(p, 0x16F7, u8) = 0;
                    if (VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(pr, 8) == 0) {
                        Actor_PlaySound(&p->c.a, 0x2B, 7, 0, 0, NULL);
                    }
                    return;
                }
            }
            if (p->c.unk104[0] != 1) {
                if (p->c.unk104[0] == 2) {
                    PU(p, 0x1760, u8) = 0;
                }
                if (VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(pr, 8) == 0) {
                    Actor_PlaySound(&p->c.a, 0x10, 7, 0, 0, NULL);
                }
                p->c.unk104[0] = 1;
            }
            VCALL(p, 0x138, void (*)(Pursuer *, u8 *, f32 *, f32 *))(p, e, (f32 *)((u8 *)p + 0x1770), other);
            hit = ~PU(p, 0x1760, u8) & (Npc_WhoReachable(p, (s32)((u8 *)p + 0x1770), AT(e, 0xC, f32)) & 0xFF) & 0xFF;
            if (AT(e, 0x8, s32) >= 0) {
                hit = (hit | (~PU(p, 0x1760, u8) & (Npc_WhoReachable(p, (s32)other, AT(e, 0xC, f32)) & 0xFF) & 0xFF)) & 0xFF;
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
        Actor_Distance(&p->c.a, p->target->a.pos) < 50.0f) {
        /* still in reach: chain the next step */
        PU(p, 0x172C, s8)++;
        PU(p, 0x1770, s32) = 0;
        PU(p, 0x1774, s32) = 0;
        PU(p, 0x1778, s32) = 0;
        Actor_SetState(&p->c.a, &Pursuer_AttackNextStep_ptmf3);
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
    l = Motion_FootPos(p->c.motion, left, 1, -1.0f, 1.0f) & 0xFF;
    r = Motion_FootPos(p->c.motion, right, 0, -1.0f, 1.0f) & 0xFF;
    g = PU(p, 0x1788, s32);
    if (g == 0x1600 || g == 0) {
        if ((((MOTION_AT(p, 0x550, f32) <= 0.0f) ^ 1) & 0xFF) == 1) {
            l = Motion_FootDownPrev(p->c.motion, 1, -1.0f) & 0xFF;
            r = Motion_FootDownPrev(p->c.motion, 0, -1.0f) & 0xFF;
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
        Mtx_ApplyPoint(foot, m, step == 1 ? left : right);
        tri = Actor_TriOfOnMesh(&p->c.a, foot);
        t = Pursuer_NavTri(tri != (u32)-1 ? tri : p->c.a.navTri);
        if (p->c.a.room == 7 || p->c.a.room == 0x106) {
            Character_WaterStep(&p->c, foot, 1);
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
                if (Actor_TriFrom(&p->c.a, foot, Actor_DoorFront(p, p->c.unk100, 0, ofs, end), end, 0) == (u32)-1) {
                    snd = 10;
                } else if (MOTION_ANIM(p) == 0x703) {
                    tri = Actor_TriFrom(&p->c.a, foot,
                        VCALL(gNavMesh, 0x5C, s32 (*)(void *, s32, s32, f32 *))(gNavMesh, p->c.unk100, 0, end), end, 0);
                    if (tri != (u32)-1) {
                        t = Pursuer_NavTri(tri);
                        flags = t != NULL ? AT(t, 0x3C, s32) : (s32)NAV_BAD_TRI_FLAGS;
                    }
                }
                break;
            case 0x707:
                tri = Actor_TriFrom(&p->c.a, foot,
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
        Motion_RootMovement(p->c.motion, root, 0.0f);
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
            Actor_PlaySound(&p->c.a, snd, level, 0, (s8)vol, NULL);
        }
        Noise_Make((u8 *)gProgress + 0x798, speed < 0.5f ? 0x1C : 0x1F, p->c.a.room, p->c.a.navTri, 0xFFFF);
    }
}

/* Fiona seen again (variant): back to the chase if she's in reach, or if either of them is
   somewhere Npc_CharSideBehind/Npc_ExitSideBehind rate 2 or more; else follow */
static s32 Pursuer_ResightTest(Pursuer *p) {
    u32 a = Npc_CharSideBehind(p, gCharPlayer);
    u32 b = Npc_ExitSideBehind(p);

    return Npc_ReachedRoom(p) != 0 && (a >= 2 || b >= 2 || a == b);
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
        } else if (Npc_InPlayedRoom(p) == 0) {
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
                Pursuer_AddSearchStops(p, (n - have) & 0xFF);
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
        } else if (Npc_ReachedRoom(p) != 0) {
            PU(p, 0x16C8, u8) = 2;
        } else if (PU(p, 0x16F4, u8) != 0) {
            PU(p, 0x16C8, u8) = 3;
            VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
            PU(p, 0x16F4, u8) = 0;
        } else if (Npc_InPlayedRoom(p) == 0 && Countdown_Seconds((u8 *)pr + 0x764) == 0) {
            PU(p, 0x16C8, u8) = 3;
            VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
        }
        if (PU(p, 0x16F2, u8) != 0) {
            Pursuer_SearchRoom(p);
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
                if (!(Npc_ReachedRoom(p) & 0xFF)) {
                    PU(p, 0x16C8, u8) = 1;
                    Pursuer_SearchRoom(p);
                } else if (!(Npc_InPlayedRoom(p) & 0xFF) || PU(p, 0x1620, u8) != 0) {
                    Pursuer_SearchRoom(p);
                }
                PU(p, 0x16F2, u8) = 0;
                PU(p, 0x16F4, u8) = 0;
            } else if (PU(p, 0x16F4, u8) != 0 || (PU(p, 0x17B4, s32) == 0 && !(Npc_InPlayedRoom(p) & 0xFF))) {
                PU(p, 0x16C8, u8) = 3;
                VCALL(p, 0x2C4, void (*)(Pursuer *))(p);
                PU(p, 0x16F4, u8) = 0;
            }
        } else if (PU(p, 0x16F2, u8) != 0) {
            PU(p, 0x16C8, u8) = Npc_ReachedRoom(p) != 0 ? 2 : 1;
            Pursuer_SearchRoom(p);
            PU(p, 0x16F2, u8) = 0;
            PU(p, 0x16F4, u8) = 0;
        } else if (PU(p, 0x16F4, u8) != 0) {
            PU(p, 0x16F4, u8) = 0;
        } else if (!(Npc_InPlayedRoom(p) & 0xFF)) {
            if (PU(p, 0x1660, s32) != 0) {
                PU(p, 0x1660, s32)--;
            } else if (!(Npc_NearRoom(p, 0) & 0xFF)) {
                VCALL(p, 0xB0, void (*)(Pursuer *))(p);
                PU(p, 0x16C8, u8) = (Npc_ReachedRoom(p) & 0xFF) ? 2 : 1;
                Pursuer_SearchRoom(p);
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

extern const PTMF Pursuer_StateRunThenNext_ptmf2, Pursuer_StateRunThenNext_ptmf3, Pursuer_StateRunThenNext_ptmf4, Pursuer_StateArrive_ptmf, Pursuer_StateArrive_ptmf2, Pursuer_StateRunToTri_ptmf, Pursuer_StateRunToTri_ptmf2,
    Pursuer_StateRootStep_ptmf, Pursuer_StateRootStep_ptmf2, Pursuer_StateRootStep_ptmf3, Pursuer_StateRootStep_ptmf4, Pursuer_StateTurnThenWait_ptmf, Pursuer_StateTurnThenWait_ptmf2;

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
                Pursuer_Hit(p);
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
            Pursuer_StanceByFiona(p);
            VCALL(p, 0x13C, void (*)(Pursuer *))(p);
            PU(p, 0x1758, s32) = -2;
        } else {
            VCALL(p, 0x90, void (*)(Pursuer *))(p);
            Pursuer_StanceByFiona(p);
            VCALL(p, 0x13C, void (*)(Pursuer *))(p);
        }
        break;
    case 2:
        ptmf_set(step, &Pursuer_StateRunThenNext_ptmf2);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 1);
        break;
    case 3:
        ptmf_set(step, &Pursuer_StateRunThenNext_ptmf3);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0xA);
        break;
    case 4:
        ptmf_set(step, &Pursuer_StateRunThenNext_ptmf4);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0xA);
        break;
    case 5:
    case 10:
        VCALL(p, 0xAC, void (*)(Pursuer *, s32, f32 *, s32))(p, p->c.unk104[0], p->c.unk110, -1);
        if (p->c.unkF4 == 5) {
            Pursuer_ScriptMove(p, 0x200, &Pursuer_StateArrive_ptmf);
        } else {
            Pursuer_ScriptMove(p, 0x201, &Pursuer_StateArrive_ptmf2);
        }
        break;
    case 6:
    case 11:
        PU(p, 0x15A4, s32) = p->c.unk104[0];
        VCALL(gNavMesh, 0xC, void (*)(void *, s32, f32 *))(gNavMesh, PU(p, 0x15A4, s32), (f32 *)((u8 *)p + 0x15B0));
        if (p->c.unkF4 == 6) {
            Pursuer_ScriptMove(p, 0x200, &Pursuer_StateRunToTri_ptmf);
        } else {
            Pursuer_ScriptMove(p, 0x201, &Pursuer_StateRunToTri_ptmf2);
        }
        break;
    case 7:
        Motion_PlayTable(p->c.motion, p->c.unk104[0], -1);
        p->c.unkE1 = 1;
        ptmf_set(step, &Pursuer_StateRootStep_ptmf);
        PU(p, 0x1758, s32) = -1;
        break;
    case 8:
        Motion_PlayBlend(p->c.motion, p->c.unk104[0], p->c.unk104[1], -1);
        p->c.unkE1 = 1;
        ptmf_set(step, &Pursuer_StateRootStep_ptmf2);
        PU(p, 0x1758, s32) = -1;
        break;
    case 9:
        Motion_PlayBlend8(p->c.motion, p->c.unk104[0], p->c.unk104[1]);
        p->c.unkE1 = 1;
        ptmf_set(step, &Pursuer_StateRootStep_ptmf3);
        PU(p, 0x1758, s32) = -1;
        break;
    case 16:
        Motion_PlayBlend(p->c.motion, 0, p->c.unk104[1], -1);
        p->c.unkE1 = 1;
        ptmf_set(step, &Pursuer_StateRootStep_ptmf4);
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
        *(f32 *)&p->c.unk104[2] = Actor_HeadingTo(&p->c.a, gCharacters[p->c.unk100]->a.pos);
        ptmf_set(step, &Pursuer_StateTurnThenWait_ptmf);
        PU(p, 0x1758, s32) = -1;
        break;
    case 15:
        Pursuer_PlayAnim(p, 0x200);
        ptmf_set(step, &Pursuer_StateTurnThenWait_ptmf2);
        PU(p, 0x1758, s32) = -1;
        break;
    }
    if (p->c.unkF4 != 0) {
        p->c.unkF4 = 0;
        PURSUER_STEP_DONE(p) = 0;
    }
}

/* strike with attack entry e at the animation's hit key (see Pursuer_StateAttackStep) */
static void Pursuer_Strike(Pursuer *p, u8 *e) {
    f32 other[4] __attribute__((aligned(16))) = { 0.0f, 0.0f, 0.0f, 0.0f };
    u32 hit;

    VCALL(p, 0x138, void (*)(Pursuer *, u8 *, f32 *, f32 *))(p, e, (f32 *)((u8 *)p + 0x1770), other);
    hit = ~PU(p, 0x1760, u8) & (Npc_WhoReachable(p, (s32)((u8 *)p + 0x1770), AT(e, 0xC, f32)) & 0xFF) & 0xFF;
    if (AT(e, 0x8, s32) >= 0) {
        hit = (hit | (~PU(p, 0x1760, u8) & (Npc_WhoReachable(p, (s32)other, AT(e, 0xC, f32)) & 0xFF) & 0xFF)) & 0xFF;
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

extern const PTMF Pursuer_StateBackOnFeet_ptmf, Pursuer_StateBackOnFeet_ptmf2;

/* state: Hewie has it by the arm or leg (+0x104 10..15 where, 15 knocked down). Each bite adds the
   part's damage (+0x173C table) to +0x16BC, at most 1000; every hit key it may throw him off,
   the likelier the longer he's held on (+0x1634) and the more damage relative to +0x16DC */
/* 0x00286170 */
void Pursuer_StateHewieHolds(Pursuer *p) {
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
                Actor_SetState(&p->c.a, &Pursuer_StateBackOnFeet_ptmf);
            } else {
                Motion_Play(m, anim, -1);
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
                Pursuer_StateKnockedDown(p);
            } else {
                anim = PU(p, 0x1624, s32) + 1;
                if (anim == AT(m, 0x55C, s32)) {
                    Actor_SetState(&p->c.a, &Pursuer_StateBackOnFeet_ptmf2);
                } else if (p->c.hp == 0) {
                    VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x1F);
                    Pursuer_StateKnockedDown(p);
                } else {
                    Motion_Play(m, anim, -1);
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
        if (Motion_EventFlags(p->c.motion, 0, -1, 1) & 0xFF & 2) {
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
    Character_RootMoveMasked(&p->c);
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
    Pursuer_PickFromTable(p);
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
            Pursuer_PickFromTable(p);
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
        ptmf_set(step, (Npc_ReachedRoom(p) & 0xFF) ? &D_003ECBB0 : &D_003ECBA0);
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
        ptmf_set(step, (Npc_ReachedRoom(p) & 0xFF) ? &D_003ECBE0 : &D_003ECBD0);
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
   search and attack state, and either come in on screen (Npc_InPlayedRoom) or go on off screen */
/* 0x002815E0 */
void Pursuer_ThroughDoor(Pursuer *p, u32 door) {
    VObject *rooms;
    s32 room;
    u8 exit;

    if (AT(gProgress, 0x1FBEC1, u8) != 0) {
        Pursuer_ThroughDoorEnding(p, door);
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
    Motion_Unfreeze(p->c.motion);
    PU(p, 0x16F5, u8) = 0;
    PU(p, 0x16F8, u8) = 0;
    PU(p, 0x16F7, u8) = 0;
    PU(p, 0x1794, s32) = 0;
    PU(p, 0x1798, s32) = 0;
    PU(p, 0x16F3, u8) = 0;
    if (Npc_InPlayedRoom(p) != 0) {
        PTMF *step = (PTMF *)((u8 *)p + 0x174C);

        if (p->c.a.unkC4 == 2) {
            ptmf_set(step, &D_003ED370);
            PU(p, 0x1758, s32) = -1;
            Pursuer_SetMove(p, &kPursuerIdleMove);
        }
        if (PU(p, 0x175C, s32) == 0x20) {
            Motion_Play(p->c.motion, 0x1802, -1);
        } else {
            switch (PU(p, 0x16C8, u8)) {
            case 0:
            case 2:
                ptmf_set(step, &D_003ED380);
                PU(p, 0x1758, s32) = -1;
                Pursuer_SetMove(p, &kPursuerMove);
                break;
            case 1:
            case 3:
                p->c.unk1388++;
                ptmf_set(step, &D_003ED390);
                PU(p, 0x1758, s32) = -1;
                if (room == PU(p, 0x1594, s32) && p->c.unk1388 >= p->c.unk1384) {
                    Pursuer_SetMove(p, &kPursuerIdleMove);
                } else {
                    Pursuer_SetMove(p, &kPursuerWaitMove);
                }
                break;
            case 4:
                p->c.unk1388++;
                ptmf_set(step, &D_003ED3A0);
                PU(p, 0x1758, s32) = -1;
                Pursuer_SetMove(p, &kPursuerWaitMove);
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
                Pursuer_SetMove(p, &kPursuerMove);
                break;
            case 1:
            case 3:
                if (room == PU(p, 0x1594, s32) && p->c.unk1388 >= p->c.unk1384) {
                    Pursuer_SetMove(p, &kPursuerIdleMove);
                } else {
                    Pursuer_SetMove(p, &kPursuerWaitMove);
                }
                break;
            case 4:
                Pursuer_SetMove(p, &kPursuerWaitMove);
                break;
            }
        }
    }
    p->c.door = exit;   /* the way in, in the new room */
    PU(p, 0x17B0, u8) = p->c.door;
    p->c.a.room = room;
    if (PU(p, 0x16C8, u8) == 1 && Npc_ReachedRoom(p) != 0) {
        PU(p, 0x16C8, u8) = 2;
    }
    VCALL(p, 0x148, void (*)(Pursuer *))(p);
    VCALL(gEvents, 0x2C, void (*)(VObject *, Pursuer *))(gEvents, p);
    if (p->c.state[0] == 5 && p->c.moveMode == 2) {
        Npc_LeaveDoor(p, 0xFF);
    }
}

extern const PTMF D_003ED5C0, D_003ED5D0, D_003ED5E0, D_003ED5F0;

/* Pursuer_ThroughDoor while the progress byte +0x1FBEC1 is set: a chase that keeps to Fiona's room
   (on arriving there it waits 150 frames, +0x17B4) */
/* 0x0027AD80 */
void Pursuer_ThroughDoorEnding(Pursuer *p, u32 door) {
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
    Motion_Unfreeze(p->c.motion);
    PU(p, 0x16F5, u8) = 0;
    PU(p, 0x16F8, u8) = 0;
    PU(p, 0x16F7, u8) = 0;
    PU(p, 0x1794, s32) = 0;
    PU(p, 0x1798, s32) = 0;
    PU(p, 0x16F3, u8) = 0;
    if (Npc_InPlayedRoom(p) != 0) {
        PTMF *step = (PTMF *)((u8 *)p + 0x174C);

        if (p->c.a.unkC4 == 2) {
            ptmf_set(step, &D_003ED5C0);
            PU(p, 0x1758, s32) = -1;
            Pursuer_SetMove(p, &kPursuerIdleMove);
        }
        if (PU(p, 0x175C, s32) == 0x20) {
            Motion_Play(p->c.motion, 0x1802, -1);
        } else {
            switch (PU(p, 0x16C8, u8)) {
            case 0:
            case 2:
                ptmf_set(step, &D_003ED5D0);
                PU(p, 0x1758, s32) = -1;
                Pursuer_SetMove(p, &kPursuerIdleMove);
                PU(p, 0x17B4, s32) = 150;
                break;
            case 1:
            case 3:
                p->c.unk1388++;
                ptmf_set(step, &D_003ED5E0);
                PU(p, 0x1758, s32) = -1;
                if (room == PU(p, 0x1594, s32) && p->c.unk1388 >= p->c.unk1384) {
                    Pursuer_SetMove(p, &kPursuerIdleMove);
                } else {
                    Pursuer_SetMove(p, &kPursuerWaitMove);
                }
                break;
            case 4:
                p->c.unk1388++;
                ptmf_set(step, &D_003ED5F0);
                PU(p, 0x1758, s32) = -1;
                Pursuer_SetMove(p, &kPursuerWaitMove);
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
                    Pursuer_SetMove(p, &kPursuerMove);
                } else {
                    Pursuer_SetMove(p, &kPursuerIdleMove);
                    PU(p, 0x17B4, s32) = 150;
                }
                break;
            case 1:
            case 3:
                if (room == PU(p, 0x1594, s32) && p->c.unk1388 >= p->c.unk1384) {
                    Pursuer_SetMove(p, &kPursuerIdleMove);
                } else {
                    Pursuer_SetMove(p, &kPursuerWaitMove);
                }
                break;
            case 4:
                Pursuer_SetMove(p, &kPursuerWaitMove);
                break;
            }
        }
    }
    p->c.door = exit;
    PU(p, 0x17B0, u8) = p->c.door;
    p->c.a.room = room;
    if (PU(p, 0x16C8, u8) == 1 && Npc_ReachedRoom(p) != 0) {
        PU(p, 0x16C8, u8) = 2;
    }
    VCALL(p, 0x148, void (*)(Pursuer *))(p);
    VCALL(gEvents, 0x2C, void (*)(VObject *, Pursuer *))(gEvents, p);
}

/* Pursuer_MayGoForTarget, inline: may the pursuer go for its target? */
static inline s32 Pursuer_MayGo(Pursuer *p) {
    if (p->target->a.unk2D == 1 || (Progress_TestFlag(gProgress, 0xE) & 0xFF) == 1) {
        return 0;
    }
    return gCharPlayer->moveSub != 0x10;
}

/* nothing to the sides or behind within 2 units (Npc_TargetSideWalkable at 110, -110 and 180 degrees) */
static s32 Pursuer_RoomAround(Pursuer *p) {
    return Npc_TargetSideWalkable(p, 0x1.921fb6p+1f /* 180 degrees */, 2.0f) == 0 &&
           Npc_TargetSideWalkable(p, 0x1.eb7c16p+0f /* 110 degrees */, 2.0f) == 0 &&
           Npc_TargetSideWalkable(p, -0x1.eb7c16p+0f, 2.0f) == 0;
}

/* attack (vtable +0x130: 12 after a hit, 13 after a miss) */
static void Pursuer_AttackAgain(Pursuer *p) {
    VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, PU(p, 0x1760, u8) != 0 ? 0xC : 0xD);
    Pursuer_PickFromTable(p);
}

/* the way round to face the target: 60..150 degrees off, or 0xFF */
static u32 Pursuer_TurnDir(Pursuer *p) {
    return Npc_TurnWayTo(p, p->target->a.pos, 0x1.0c1524p+0f /* 60 degrees */, 0x1.4f1a6ep+1f /* 150 degrees */) & 0xFF;
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
            Pursuer_ChanceRoll(p);
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

            if ((d < Pursuer_GroundGained(p) || d < 10.0f) && !(d <= 0.0f)) {
                f32 a;

                if (!(Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]) <= 0.0f)) {
                    a = Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
                } else {
                    a = -Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
                }
                if (a < 0x1.921fb6p+1f * VCALL(p, 0x2EC, f32 (*)(Pursuer *))(p) / 180.0f &&
                    Npc_SameFloor(&p->c.a, &p->target->a) != 0) {
                    /* Fiona is right there in front: attack at once */
                    VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, (s8)Pursuer_FionaState(p));
                    Pursuer_PickFromTable(p);
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
        u32 m = Npc_WhoSeen(p) & 0xFF & 1;

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
            Pursuer_ThroughDoor(p, PU(p, 0x17B0, u8));
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
            f32 h = Actor_HeadingTo(&p->c.a, t->a.pos);

            Npc_TurnToward(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
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
        } else if (PU(p, 0x16EF, u8) == 1 || !(Progress_ExitOpen(pr, p->c.a.room, PU(p, 0x17B0, u8)) & 0xFF)) {
            PU(p, 0x16ED, u8) = 1;
            PU(p, 0x16EF, u8) = 0;
            PURSUER_STEP_DONE(p) = 0;
        } else if (PURSUER_STEP_DONE(p) == 1 || Npc_AtSpawn(p, PU(p, 0x17B0, u8)) != 0) {
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
    if (p->c.a.room == gCharPartner->a.room && Npc_ExitSideBehind(p) != Npc_CharSideBehind(p, gCharPartner)) {
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

        if (Npc_ReachedRoom(p) & 0xFF) {
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

        if (d < Pursuer_GroundGained(p) || d < 10.0f) {
            f32 a;

            if (!(Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]) <= 0.0f)) {
                a = Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
            } else {
                a = -Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
            }
            if (a < 0x1.921fb6p+1f * VCALL(p, 0x2EC, f32 (*)(Pursuer *))(p) / 180.0f) {
                VCALL(p, 0x130, void (*)(Pursuer *, s32))(p, 0xB);
                Pursuer_PickFromTable(p);
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

    if (Actor_TriTo(&p->c.a, p->target->a.pos, 0) == tri) {
        if (PU(p, 0x1788, s32) != 0x201) {
            Pursuer_PlayAnim(p, VCALL(p, 0x328, s32 (*)(Pursuer *))(p));
        }
        Npc_StepToward(p, p->target->a.pos);
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
            Pursuer_PickFromTable(p);
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
            Pursuer_ChaseFionaHere(p);
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
        f32 h = Actor_HeadingTo(&p->c.a, t->a.pos);
        f32 a;

        Npc_TurnToward(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
        if (!(PU(p, 0x1588, f32) <= Pursuer_GroundGained(p))) {
            Pursuer_HoldOff(p, e);
            break;
        }
        if (!(Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]) <= 0.0f)) {
            a = Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
        } else {
            a = -Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
        }
        if (!(a <= 0x1.921fb6p+1f * VCALL(p, 0x2EC, f32 (*)(Pursuer *))(p) / 180.0f)) {
            Pursuer_HoldOff(p, e);
            break;
        }
        if (PU(p, 0x1588, f32) < 0.0f) {
            VCALL(p, 0xB0, void (*)(Pursuer *))(p);
            if (!(Pursuer_TargetOutOfReach(p) & 0xFF)) {
                Pursuer_ChaseFionaHere(p);
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
        ptmf_set((PTMF *)((u8 *)p + 0x174C), &D_003ECC20);
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

/* the door +0x17B0 can't be passed: mark its link in the visited set (+0x148C) and plan again
   to the goal room; 0 if there's no way at all */
static s32 Pursuer_Replan(Pursuer *p, VObject *rooms) {
    u32 bit;

    PURSUER_STEP_DONE(p) = 0;
    bit = VCALL(rooms, 0x10, s32 (*)(VObject *, s32, u32))(rooms, p->c.a.room, PU(p, 0x17B0, u8)) & 0xFFFF;
    p->c.unk148C[bit >> 5] |= 1 << (bit & 0x1F);
    return Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
}

extern const PTMF D_003ECCE0, D_003ECCF0, D_003ECD00, D_003ECD10, D_003ECD20;

/* as Pursuer_BehaviourRun while heading through doors on screen: walk to the door (+0x17B0), open it
   (action 0xE), go through (0xF / Pursuer_ThroughDoor), or plan round it when it's locked */
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
            if (Pursuer_PlanWhere(p) & 0xFF) {
                VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x27);
            } else {
                Pursuer_ChaseFionaHere(p);
            }
            return;
        }
        if (PURSUER_STEP_DONE(p) != 1) {
            break;
        }
        switch (Npc_ExitKind(p, PU(p, 0x17B0, u8)) & 0xFF) {
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
            if (Pursuer_Replan(p, gRooms) == -1 && !(Pursuer_PlanWhere(p) & 0xFF)) {
                Pursuer_ChaseFionaHere(p);
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
            Pursuer_ChaseFionaHere(p);
            return;
        }
        rooms = gRooms;
        VCALL(rooms, 0x30, void (*)(VObject *, u32, f32 *))(rooms, PU(p, 0x17B0, u8), door);
        onMesh = Actor_TriTo(&p->c.a, door, -1) != (u32)-1;
        a = Actor_NearerRoom(&p->c.a, PU(p, 0x17B0, u8), (f32 *)((u8 *)p + 0x15B0)) & 0xFF;
        if (!(ds & 4) && PURSUER_STEP_DONE(p) != 1 && !((onMesh & 0xFF) & a)) {
            break;
        }
        switch (Npc_ExitKind(p, PU(p, 0x17B0, u8)) & 0xFF) {
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
            if (Pursuer_Replan(p, rooms) == -1 && (Pursuer_PlanWhere(p) & 0xFF) != 1) {
                Pursuer_ChaseFionaHere(p);
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
            Pursuer_ChaseFionaHere(p);
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
            Pursuer_PickFromTable(p);
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
            Pursuer_ChaseFionaHere(p);
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
        if (Npc_ReachedRoom(p) != 0) {
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
        if (PU(p, 0x175C, s32) == 0xF && (pr = gProgress, !(Progress_ExitOpen(pr, p->c.a.room, PU(p, 0x17B0, u8)) & 0xFF))) {
            /* in the doorway with the door shut */
            if (!(Npc_ReachedRoom(p) & 0xFF)) {
                u8 want = PU(p, 0x17B0, u8);

                if (want == (VCALL(gRooms, 0x3C, s32 (*)(VObject *, u32, s32))(gRooms,
                                 PU(p, 0x138C + p->c.unk1388 * 2, u16), p->c.a.room) & 0xFF)) {
                    PU(p, 0x162C, s32) = 0;
                    Pursuer_ThroughDoor(p, PU(p, 0x17B0, u8));
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
                Pursuer_ThroughDoor(p, PU(p, 0x17B0, u8));
            }
            return;
        }
        ptmf_set((PTMF *)((u8 *)p + 0x174C), (Npc_ReachedRoom(p) & 0xFF) ? &D_003ECD20 : &D_003ECD10);
        PU(p, 0x1758, s32) = -1;
        VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x1C);
        return;
    }
    if (PU(p, 0x16ED, u8) == 1) {
        PU(p, 0x162C, s32) = 0;
        Pursuer_ThroughDoor(p, PU(p, 0x17B0, u8));
        PU(p, 0x16ED, u8) = 0;
    }
}

extern const PTMF D_003ED3B0, D_003ED3C0, D_003ED3D0, D_003ED3E0, D_003ED3F0, D_003ED400, Pursuer_StateHitReact_ptmf5,
    D_003ED420, D_003ED430, D_003ED440;

/* place the pursuer on triangle tri facing its heading (vtable +0x28), or nowhere */
static void Pursuer_PlaceOn(Pursuer *p, s32 tri) {
    if (VCALL(p, 0x28, s32 (*)(Pursuer *, s32, f32 *, f32 *))(p, tri, &p->c.a.angle[1], p->c.a.pos) == -1) {
        Actor_TeleportRandom(&p->c.a, -1);
    }
}

/* show up: once the pursuer is in Fiona's room (Npc_InPlayedRoom) stand it at the door it came in
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
    if (Npc_InPlayedRoom(p) == 0) {
        if (PU(p, 0x1624, s32) == 1) {
            PU(p, 0x16C9, u8) = 6;
            PU(p, 0x16CA, u8) = 7;
            PU(p, 0x17B4, s32) = 0;
            Pursuer_ThroughDoor(p, PU(p, 0x17B0, u8));
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
                Pursuer_SearchOffscreen(p);
            }
        }
    } else {
        if (!__ptmf_cmpr(step, &D_003ED3B0) || !__ptmf_cmpr(step, &D_003ED3C0) || !__ptmf_cmpr(step, &D_003ED3D0)) {
            Pursuer_BackOnMesh(p);
            Npc_Senses2Ending(p);
            if (PU(p, 0x16C8, u8) == 0) {
                PU(p, 0x17B4, s32) = 0;
            } else {
                Pursuer_SearchOffscreen(p);
            }
            Pursuer_StanceByFiona(p);
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
                    Pursuer_PlayAnimBlend(p, ((s32 (*)(Pursuer *, Character *, u32))Actor_TriFreeFor)(p, (Character *)p, 0) != 0 ? 0x1804 : 0x1800);
                    PU(p, 0x17B4, s32) = 0;
                } else {
                    VCALL(p, 0x114, void (*)(Pursuer *, s32))(p, 0x1F);
                    Actor_SetState(&p->c.a, &Pursuer_StateHitReact_ptmf5);
                    Pursuer_PlayAnimBlend(p, ((s32 (*)(Pursuer *, Character *, u32))Actor_TriFreeFor)(p, (Character *)p, 0) != 0 ? 0x1806 : 0x1802);
                }
                PU(p, 0x1761, u8) = 1;
                Pursuer_SearchOffscreen(p);
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
                Npc_PlanToRoomObject(p);
                VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 0x26);
            } else {
                VCALL(p, 0x118, void (*)(Pursuer *, s32))(p, 4);
            }
            Pursuer_BackOnMesh(p);
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
                Actor_TeleportRandom(&p->c.a, -1);
            }
            PURSUER_STEP_NEXT(p) = 1;
        }
        p->c.unk14C4 = 0;
        Pursuer_CarryOnLying(p);
        Character_MarkObjects(&p->c);
    }
    if (Actor_PosInCurrentRoom(&p->c.a, at) != 0) {
        ((void (*)(Character *, s32, f32 *))Character_Sound)(&p->c, 0, at);
        ((void (*)(Character *, s32, f32 *))Character_Sound)(&p->c, 5, at);
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
/* 0x0029B8B0 */
void Pursuer_Hit(Pursuer *p) {
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
    if (!(Angle_Wrap(Actor_HeadingTo(&p->c.a, at) - p->c.a.angle[1]) <= 0.0f)) {
        a = Angle_Wrap(Actor_HeadingTo(&p->c.a, at) - p->c.a.angle[1]);
    } else {
        a = -Angle_Wrap(Actor_HeadingTo(&p->c.a, at) - p->c.a.angle[1]);
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
                Hewie_ChangeFeeling((Hewie *)gCharPartner, (Character *)p, 1);
            }
        }
        {
            s16 snd = p->c.state[2] == 1 ? 0x1D : 0x1C;

            if (VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(gProgress, 8) == 0) {
                Actor_PlaySound(&p->c.a, snd, 7, 0, 0, NULL);
            }
        }
        Pursuer_ClearMessage(p);
        return;
    }
    if (PU(p, 0x16F7, u8) == 1 && p->c.state[1] == 1 && p->c.hp > 0) {
        if (VCALL(gEvents, 0x50, s32 (*)(VObject *))(gEvents) == 0 && Progress_TestFlag(gProgress, 8) == 0) {
            Actor_PlaySound(&p->c.a, 0x1C, 7, 0, 0, NULL);
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
        Npc_LeaveDoor(p, 0xFF);
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
                Hewie_ChangeFeeling((Hewie *)gCharPartner, (Character *)p, 3);
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

        if (AT(AT(m, 0x874, u8 *) + Motion_AnimIndex(m, AT(m, 0x55C, s32)) * 6, 0x4, u16) & 1) {
            u8 *mgr = gEffects;
            s32 args[2];

            p->c.unk104[0] = p->c.state[4] & 0x7FFF;
            PU(p, 0x1624, s32) = p->c.state[4] & 0x7FFF;
            EffectMgr_Start(mgr, PU(p, 0x1628, s32), NULL);
            PU(p, 0x1628, s32) = Effect_New(mgr, 0x38, Pursuer_EffectInit);
            args[0] = (p->c.unk104[0] - 4) >> 1;
            args[1] = PU(p, 0x1624, s32);
            EffectMgr_Start(mgr, PU(p, 0x1628, s32), args);
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
            Pursuer_ChanceRoll(p);
        }
    }
    if (p->c.state[4] & 0x8000) {
        p->c.a.unkC4 = 1;
        PU(p, 0x1790, s32) = 900;
        PU(p, 0x16F5, u8) = 1;
        if (p->c.state[2] == 1) {
            Hewie_ChangeFeeling((Hewie *)gCharPartner, (Character *)p, 1);
        }
    }
    switch (p->c.state[1]) {
    case 7:
        if (p->target == gCharPartner) {
            PU(p, 0x1761, u8) |= 1;
            Pursuer_ChanceRoll(p);
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
            Pursuer_ThroughDoor(p, PU(p, 0x17B0, u8));
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
        } else if (PU(p, 0x16EF, u8) == 1 || !(Progress_ExitOpen(pr, p->c.a.room, PU(p, 0x17B0, u8)) & 0xFF)) {
            PU(p, 0x16ED, u8) = 1;
            PU(p, 0x16EF, u8) = 0;
            PURSUER_STEP_DONE(p) = 0;
        } else if (PURSUER_STEP_DONE(p) == 1 || Npc_AtSpawn(p, PU(p, 0x17B0, u8)) != 0) {
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
            Pursuer_PickFromTable(p);
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
            Pursuer_ChaseFionaHere(p);
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
        f32 h = Actor_HeadingTo(&p->c.a, t->a.pos);
        f32 a;

        Npc_TurnToward(p, h, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
        if (!(PU(p, 0x1588, f32) <= Pursuer_GroundGained(p))) {
            Pursuer_HoldOff(p, e);
            break;
        }
        if (!(Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]) <= 0.0f)) {
            a = Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
        } else {
            a = -Angle_Wrap(Actor_HeadingTo(&p->c.a, p->target->a.pos) - p->c.a.angle[1]);
        }
        if (!(a <= 0x1.921fb6p+1f * VCALL(p, 0x2EC, f32 (*)(Pursuer *))(p) / 180.0f)) {
            Pursuer_HoldOff(p, e);
            break;
        }
        if (PU(p, 0x1588, f32) < 0.0f) {
            VCALL(p, 0xB0, void (*)(Pursuer *))(p);
            if (!(Pursuer_TargetOutOfReach(p) & 0xFF)) {
                Pursuer_ChaseFionaHere(p);
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
        s32 d = Npc_OpenDoorFionaHides();

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
    if (p->c.a.room == gCharPlayer->a.room && Npc_ExitSideBehind(p) != Npc_CharSideBehind(p, gCharPlayer)) {
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

extern PTMF kPursuerStairsMove;   /* the move along the stairs */

/* the time Hewie keeps a stalker busy (his "stay" level, +0xF35CC) */
static s32 Pursuer_HewieDelay(Character *h, s32 base) {
    return (AT(h, 0xF35CC, s16) + 1) * 15 + base;
}

/* plan from where the pursuer is to its goal room; 0 when there's no way and it can't wait */
static s32 Pursuer_PlanOn(Pursuer *p) {
    return Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) > 0 || (Pursuer_PlanWhere(p) & 0xFF);
}

/* a door or stair move still in progress when the pursuer leaves the screen: finish it */
static void Pursuer_FinishCrossing(Pursuer *p) {
    if (p->c.moveMode == 2) {
        Progress *pr = gProgress;

        if ((Progress_CurRoomFlag(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1) {
            /* finish going through the door */
            if (PU(p, 0x175C, s32) == 0xF) {
                Npc_DoorRelease(p, (u8)p->c.unk100);
            } else if ((Progress_ExitOpen(pr, p->c.a.room, (u8)p->c.unk100) & 0xFF) == 1) {
                Npc_DoorShutOther(p, (u8)p->c.unk100);
            } else {
                Npc_DoorRelease(p, (u8)p->c.unk100);
            }
            AT(p, 0x2B, u8) = 0;
            p->c.a.unk2D = 0;
            p->c.a.navMask = VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
            if (Npc_TriBlocked(p, p->c.a.navTri) != 0) {
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
            p->c.a.navTri = Actor_DoorFront(p, p->c.unk100, 0, ofs, p->c.a.pos);
        } else {
            VCALL(p, 0x9C, void (*)(Pursuer *, s32, f32 *))(p, 2, ofs);
            p->c.a.navTri = Actor_DoorFront(p, p->c.unk100, 1, ofs, p->c.a.pos);
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
    Motion_Unfreeze(p->c.motion);
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
            Pursuer_SetMove(p, &kPursuerWaitMove);
        }
    } else if (PU(p, 0x16C8, u8) == 0) {
        VCALL(p, 0xB8, void (*)(Pursuer *, u32))(p, door);
        if (Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) == -1) {
            Pursuer_StartSearch(p);
        }
    }
}

/* walk to the exit +0x17B0 of the plan (or along the stairs) */
static void Pursuer_HeadForExit(Pursuer *p, VObject *rooms, u32 door) {
    if (p->c.moveMode == 3) {
        Pursuer_SetMove(p, &kPursuerStairsMove);
    } else {
        PU(p, 0x15A4, s32) = VCALL(rooms, 0x34, s32 (*)(VObject *, u32, f32 *))(rooms, PU(p, 0x17B0, u8), (f32 *)((u8 *)p + 0x15B0));
        p->c.a.navMask = VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
        AT(p->c.pathReq, 0x40, u32) = p->c.a.navMask;
        if ((Npc_PlanToGoal(p) & 0xFF) != 1) {
            Pursuer_SetMove(p, &kPursuerStairsMove);
        } else {
            if (p->c.moveMode != 4 && PU(p, 0x16C8, u8) == 0) {
                Npc_FionaAtSpawn(p, door);
            }
            Pursuer_SetMove(p, &kPursuerMoveA);
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
            Pursuer_SetMove(p, &kPursuerIdleMove);
            return 0;
        }
        if (!(PU(p, 0x1621, u8) < 8)) {
            Pursuer_ClearRoute(p);
            Pursuer_SearchRouteIn(p);
            Pursuer_SetMove(p, &kPursuerIdleMove);
            return 0;
        }
        break;
    case 1:
        if (!(Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) > 0)) {
            PU(p, 0x16F4, u8) = 1;
            if (!(Pursuer_PlanWhere(p) & 0xFF)) {
                Pursuer_SetMove(p, &kPursuerIdleMove);
                return 0;
            }
        }
        Pursuer_NextExit(p, rooms);
        break;
    case 3:
        if (PU(p, 0x1620, u8) < PU(p, 0x1621, u8) || !Pursuer_PlanOn(p)) {
            Pursuer_SetMove(p, &kPursuerIdleMove);
            return 0;
        }
        Pursuer_NextExit(p, rooms);
        break;
    case 4: {
        s32 room = p->c.a.room;
        s32 *e;

        for (e = VCALL(p, 0x314, s32 *(*)(Pursuer *))(p); *e != -1; e += 2) {
            if (room == *e) {
                Pursuer_SetMove(p, &kPursuerIdleMove);
                return 0;
            }
        }
        if (!Pursuer_PlanOn(p)) {
            Pursuer_SetMove(p, &kPursuerIdleMove);
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
    if (Npc_InPlayedRoom(p) == 0) {
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
            if (Npc_ReachedRoom(p) != 0 || PU(p, 0x1594, s32) == -1) {
                PU(p, 0x16C9, u8) = 2;
                PU(p, 0x16CA, u8) = 3;
                PU(p, 0x16C8, u8) = 1;
                VCALL(p, 0xBC, void (*)(Pursuer *, u32))(p, door);
                Pursuer_ClearRoute(p);
            }
            break;
        case 4:
        case 5:
            if (Npc_ReachedRoom(p) != 0 || PU(p, 0x1594, s32) == -1) {
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
            Pursuer_SetMove(p, &kPursuerIdleMove);
            PU(p, 0x16F4, u8) = 1;
            return;
        }
        if (!(Pursuer_PlanWhere(p) & 0xFF)) {
            PU(p, 0x17B4, s32) = 150;
            Pursuer_SetMove(p, &kPursuerIdleMove);
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
            Pursuer_MotionGroup(p);
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
    if (Npc_InPlayedRoom(p) == 0) {
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
            if (Npc_ReachedRoom(p) != 0 || PU(p, 0x1594, s32) == -1) {
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
            if (Npc_ReachedRoom(p) != 0 || PU(p, 0x1594, s32) == -1) {
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
            Pursuer_SetMove(p, &kPursuerIdleMove);
            PU(p, 0x16F4, u8) = 1;
            return;
        }
        /* chasing: through this door, whatever the plan said about it */
        PU(p, 0x17B0, u8) = door;
        link = VCALL(rooms, 0x10, s32 (*)(VObject *, s32, u32))(rooms, p->c.a.room, door) & 0xFFFF;
        p->c.unk148C[link >> 5] &= ~(1 << (link & 0x1F));
        VCALL(p, 0xB8, void (*)(Pursuer *, u32))(p, door);
        if (!(Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) > 0)) {
            PU(p, 0x17B4, s32) = 150;
            Pursuer_SetMove(p, &kPursuerIdleMove);
            PU(p, 0x16F4, u8) = 1;
            return;
        }
        if (link != PU(p, 0x138C, u16)) {
            if ((VCALL(p, 0xEC, s32 (*)(Pursuer *, u32))(p, door) & 0xFF) == 1) {
                s32 a = VCALL(rooms, 0x1C, s32 (*)(VObject *, u32, s32))(rooms, link, p->c.a.room);

                if (a != VCALL(rooms, 0x1C, s32 (*)(VObject *, u32, s32))(rooms, PU(p, 0x138C, u16), p->c.a.room)) {
                    PU(p, 0x17B4, s32) = 150;
                    Pursuer_SetMove(p, &kPursuerIdleMove);
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

extern u8 kPursuerSteps[];   /* the action table: 0x1C bytes each */

/* vtable +0x114: start action `kind` (0x1000 set: from the pursuer's own table +0x1714): its
   state, action id +0x175C, move mode and sub, sense mode +0x15C0 and look mode +0x1710 */
/* 0x00127A40 */
void Pursuer_StartAction(Pursuer *p, u32 kind) {
    u8 *e;

    if (kind & 0x1000) {
        e = PU(p, 0x1714, u8 *) + (kind & ~0x1000) * 0x1C;
    } else {
        e = kPursuerSteps + kind * 0x1C;
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
        e = kPursuerSteps + id * 28;
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

/* vtable +0xEC: can the pursuer go through exit `exit`: 1 if Npc_ExitKind says 4, 5 or 6,
   2 if it says 2 (passed on as is), else 0 */
/* 0x00127C40 */
s32 NPC_CanUseExit(Pursuer *p, s32 exit) {
    switch (Npc_ExitKind(p, exit) & 0xFF) {
    case 2:
        return 2;
    case 4:
    case 5:
    case 6:
        return 1;
    }
    return 0;
}

/* 0x0012BFB0 */
u8 *Debilitas_ModelFileTable(Pursuer *p) {
    if (FLD(gProgress, 0x30, u32) & 0x8000) {
        return pstr_O_DB0_DB0_200_PCK_2;
    }
    return pstr_O_DB0_DB0_200_PCK;
}

/* Room object constructor: base 0x469C20 -> 0x469C60 -> 0x46D810; id at +0x153C. */
/* 0x00171090 */
void *Pursuer_ctor(u8 *p, u32 id, s32 arg) {
    FLD(p, 0x0, void **) = Actor_vtable;
    FLD(p, 0x20, s32) = arg;
    FLD(p, 0x24, s32) = 0x2000000;
    FLD(p, 0x0, void **) = Character_vtable;
    FLD(p, 0x1380, s32) = 0;
    p[0x153C] = (u8)id;
    FLD(p, 0x0, void **) = Pursuer_vtable;
    return p;
}

/* vtable +0x8: NPC destructor (-> Character) */
/* 0x001710D0 */
Pursuer *NPC_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = NPC_vtable;
        VCALL(p, 0x10, void (*)(Pursuer *))(p);
        if (p != NULL) {
            p->c.a.vtbl = Character_vtable;
            if (p != NULL) {
                p->c.a.vtbl = Actor_vtable;
            }
        }
        if ((s16)flags > 0) {
            Actor_Destroy(&p->c.a);
        }
    }
    return p;
}

/* ---- the room character of id 8 (vtable 0x46FF80; built by Kind08_ctor): a Pursuer that
 * mostly keeps the Pursuer's own behaviour ---- */

/* ---- character kind 14 (vtable Kind14_vtable, the pursuer base with three of its own) ---- */

/* ---- character kind 0x10 (vtable Kind16_vtable, the pursuer base with four of its own) ---- */

/* ---- character kind 0x11 (vtable Kind17_vtable, the pursuer base with four of its own) ---- */

/* destructor (vtable Reflection_vtable) */
/* 0x00316D80 */
void *Obj472F60_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = Reflection_vtable;
        AT(o, 0x0, void **) = D_0046D730;
        if ((s16)flags > 0) {
            RoomEffects_delete(o);
        }
    }
    return o;
}

/* destructor (vtable Effect79FF0_vtable) */
/* 0x0036A4D0 */
void *Effect79FF0_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = Effect79FF0_vtable;
        AT(o, 0x0, void **) = EffectBase_vtable;
        if ((s16)flags > 0) {
            EffectMgr_free(o);
        }
    }
    return o;
}

/* Room37_vtable effect message: none sets +0x34 (done); else +0x2C its word 0, and below 2 in word 1
 * +0x30 30 */
/* 0x0036A530 */
void Effect79FF0_SetParams(u8 *o, s32 *m) {
    if (m == NULL) {
        AT(o, 0x34, u8) = 1;
        return;
    }
    AT(o, 0x2C, s32) = m[0];
    if (m[1] < 2) {
        AT(o, 0x30, s32) = 30;
    }
}

/* 0x0036A580 */
void Effect79FF0_Draw(void) {
}

/* Room37_vtable's update: while the pursuer is in state 4 / action 0x11 (and not stopped, +0x34,
 * nor its delay +0x30 run out) each of its ten timers (+0x4..) counts down; at most one at 0 a
 * frame restarts (10..41) and lets a strand (kind = the pursuer's, +0x2C) drip */
/* 0x0036A590 */
s32 Effect79FF0_Update(u8 *o) {
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
            EffectMgr_Start(mgr, slot, msg);
        }
    }
    return 1;
}

/* its set up: +0x34 0, +0x30 -1, ten random 0..15 from +0x4 */
/* 0x0036A7F0 */
void Effect79FF0_Start(u8 *o) {
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

extern void *Kind21_vtable[], *Kind14_vtable[];

/* 0x00172FB0 */
void *Kind15_ctor(void *p, u32 id, u32 arg) {
    return b0_RoomCtor(p, id, (u8)arg, Kind15_vtable);
}

extern void *Kind15_vtable[];

/* vtable +0x8 of Kind15_vtable (derived from kind 14) */
/* 0x001794C0 */
Pursuer *Kind15_dtor(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = Kind15_vtable;
        p->c.a.vtbl = Kind14_vtable;
        Pursuer_DestroyBase(p);
        if ((s16)flags > 0) {
            Actor_Destroy(&p->c.a);
        }
    }
    return p;
}

/* vtable +0x11C */
/* 0x00179610 */
s32 Pursuer_GivesUp(Pursuer *p) {
    return 0;
}

/* vtable +0x128 (Debilitas: own): the stand animation by +0x16C8 */
/* 0x00179620 */
void Pursuer_StandAnim(Pursuer *p) {
    u8 k = PU(p, 0x16C8, u8);

    switch (k) {
    case 0:
    case 1:
    case 2:
    case 4:
        Pursuer_PlayAnimIf(p, VCALL(p, 0x328, s32 (*)(Pursuer *, u32))(p, k), 0);
        break;
    case 3: {
        u8 j = PU(p, 0x16C9, u8);

        if (j != 0 && j != 2) {
            Pursuer_PlayAnimIf(p, VCALL(p, 0x328, s32 (*)(Pursuer *, u32))(p, k), 0);
        } else {
            Pursuer_PlayAnimIf(p, VCALL(p, 0x324, s32 (*)(Pursuer *, u32))(p, k), 0);
        }
        break;
    }
    }
}

/* vtable +0x2AC .. +0x2B8: nothing */
/* 0x00179710 */
void Pursuer_GoForFiona(Pursuer *p) {
}

/* 0x00179720 */
void Pursuer_HeadingStep(Pursuer *p) {
}

/* 0x00179730 */
void Pursuer_FreshStart(Pursuer *p) {
}

/* 0x00179740 */
void Pursuer_CarryOn(Pursuer *p) {
}

/* vtable +0x2C0: timer +0x1660 to 600 frames (10 s) */
/* 0x00179750 */
void Pursuer_Timer10s(Pursuer *p) {
    PU(p, 0x1660, s32) = 600;
}

/* vtable +0x2C8: timer +0x1660 to `frames`, 0 = 900 (15 s) */
/* 0x00179760 */
void Pursuer_SetTimer(Pursuer *p, s32 frames) {
    PU(p, 0x1660, s32) = frames != 0 ? frames : 900;
}

/* ---- batch 3 ---- */

/* vtable +0x9C: offset of the point beside a door, by side (Lorenzo's wheelchair etc. differ) */
/* 0x00179780 */
void Pursuer_DoorOffset(Pursuer *p, s32 side, f32 *out) {
    switch (side) {
    case 0:
        AT(out, 0x0, u32) = 0;
        AT(out, 0x4, u32) = 0;
        AT(out, 0x8, u32) = 0xC0C00000;   /* -6.0 */
        break;
    case 1:
        AT(out, 0x0, u32) = 0x3F23D70A;   /* 0.64 */
        AT(out, 0x4, u32) = 0;
        AT(out, 0x8, u32) = 0x41266666;   /* 10.4 */
        break;
    case 2:
        AT(out, 0x0, u32) = 0xBED4AF4F;   /* -0.4154 */
        AT(out, 0x4, u32) = 0;
        AT(out, 0x8, u32) = 0x41202F1B;   /* 10.0115 */
        break;
    case 3:
        AT(out, 0x0, u32) = 0x3F23D70A;   /* 0.64 */
        AT(out, 0x4, u32) = 0;
        AT(out, 0x8, u32) = 0xC0F23055;   /* -7.568 */
        break;
    }
}

/* vtable +0xA0 / +0xA4: turn rates, 4 and 8 degrees (in radians) */
/* 0x00179830 */
f32 Pursuer_TurnRate(Pursuer *p) {
    return 0x1.1df46a0000000p-4f /* 0.06981317 */;
}

/* 0x00179850 */
f32 Pursuer_TurnRateFast(Pursuer *p) {
    return 0x1.1df46a0000000p-3f /* 0.13962634 */;
}

/* vtable +0x2DC .. +0x2FC: distances and factors */
/* 0x00179870 */
f32 Pursuer_LookFrames(Pursuer *p) {
    return 60.0f;
}

/* 0x00179880 */
f32 Pursuer_LookSwing(Pursuer *p) {
    return 0x1.eb851e0000000p-4f /* 0.12 */;
}

/* 0x001798A0 */
f32 Pursuer_ReachFiona(Pursuer *p) {
    return 20.0f;
}

/* 0x001798B0 */
f32 Pursuer_Dist2E8(Pursuer *p) {
    return 20.0f;
}

/* 0x001798C0 */
f32 Pursuer_AttackAngle(Pursuer *p) {
    return 20.0f;
}

/* 0x001798D0 */
f32 Pursuer_ReachHewie(Pursuer *p) {
    return 16.0f;
}

/* 0x001798E0 */
f32 Pursuer_AttackRange(Pursuer *p) {
    return 24.0f;
}

/* 0x001798F0 */
f32 Pursuer_SpeedTop(Pursuer *p) {
    return 0x1.6666660000000p+0f /* 1.4 */;
}

/* 0x00179910 */
f32 Pursuer_SpeedBase(Pursuer *p) {
    return 0x1.3333340000000p-1f /* 0.6 */;
}

/* 0x00179930 */
s32 Pursuer_AttackAnimA(Pursuer *p) {
    return -1;
}

/* 0x00179940 */
s32 Pursuer_AttackAnimB(Pursuer *p) {
    return -1;
}

/* vtable +0x314 */
/* 0x00179950 */
s32 Pursuer_RoomSpots(Pursuer *p) {
    return 0;
}

/* vtable +0xE8 */
/* 0x00179960 */
void NPC_PickDestination(Pursuer *p) {
    Npc_HeadRandomRoom(p);
}

/* `tri` if the pursuer may stand on it (its blocking flags, vtable +0xA8, against the
   triangle's +0x3C), else the nearest triangle it may (a planner query of kind 7; -1 if none) */
/* 0x00211B00 */
u32 Npc_TriIfStandable(Pursuer *p, u32 tri) {
    VObject *nav = (VObject *)gNavMesh;
    VObject *planner;
    PathRequest q = { 0 };
    u8 *t = NULL;
    u32 found, mask;

    if (tri < AT(nav, 0x8, u32) && AT(nav, 0x4, u8 *) != NULL) {
        t = AT(nav, 0x4, u8 *) + tri * 0x50;
    }
    mask = VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
    if (!((t != NULL ? AT(t, 0x3C, u32) : 0 /* (the original reads address 0x3C) */) & mask)) {
        return tri;
    }
    q.unk0 = 0;
    q.startTri = tri;
    VCALL(nav, 0xC, void (*)(VObject *, u32, f32 *))(nav, tri, q.startPos);
    q.goalTri = tri;
    sceVu0CopyVector(q.goalPos, q.startPos);
    q.unk4 = 7;
    q.mask = VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
    p->c.pathId = VCALL(gSceneGameF29740, 0xC, s32 (*)(VObject *, PathRequest *, s32))(gSceneGameF29740, &q, 0);
    if (p->c.pathId == -1) {
        return -1;
    }
    planner = gSceneGameF29740;
    VCALL(planner, 0x14, s32 (*)(VObject *))(planner);
    found = VCALL(planner, 0x38, u32 (*)(VObject *, s32))(planner, p->c.pathId);
    VCALL(planner, 0x28, void (*)(VObject *, s32))(planner, p->c.pathId);
    p->c.pathId = -1;
    return found;
}

/* `a2` for the pursuer in its room (progress) */
/* 0x00211C80 */
void NPC_ExitArg(Pursuer *p, s32 a2) {
    Progress *pr = gProgress;

    DoorHold_Take(pr, p->c.a.room, a2, *(u8 *)&p->c.a.slot);
    DoorHold_Open(pr, p->c.a.room, a2, *(u8 *)&p->c.a.slot);
}

/* ---- batch 4 ---- */

/* door / exit `exit`: what to do with it (vtable +0xF0 to go through); 2 / 1 / 0 */
/* 0x00211CF0 */
s32 Npc_ExitWhatToDo(Pursuer *p, s32 exit) {
    Progress *pr;

    switch (Npc_ExitKind(p, exit) & 0xFF) {
    case 2:
        return 2;
    case 6:
        pr = gProgress;
        DoorHold_Take(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
        DoorHold_Release(pr, p->c.a.room, exit);
        /* fallthrough */
    case 4:
        if (!(VCALL(gRooms, 0x78, s32 (*)(VObject *, s32, s32))(gRooms, p->c.a.room, exit) & 0xFF)) {
            return 1;
        }
        /* fallthrough */
    case 5:
        VCALL(p, 0xF0, void (*)(Pursuer *, s32))(p, exit);
        return 1;
    default:
        return 0;
    }
}

/* what is exit `exit` like for the pursuer: 2 its own way in, 3 closed to it, 4 open, 5 / 6
 * it must open it (6: from the other side) */
/* 0x00211E00 */
u32 Npc_ExitKind(Pursuer *p, s32 exit) {
    Progress *pr = gProgress;
    VObject *rm;

    if ((Progress_CurRoomFlag(pr, p->c.a.room, exit) & 0xFF) == 1) {
        return 2;
    }
    if (!(Progress_ExitPassable(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot) & 0xFF)) {
        return 3;
    }
    if ((Progress_ExitUnlocked(pr, p->c.a.room, exit) & 0xFF) == 1) {
        return 3;
    }
    rm = gRooms;
    if (!(VCALL(rm, 0x78, s32 (*)(VObject *, s32, s32))(rm, p->c.a.room, exit) & 0xFF)) {
        return 4;
    }
    if ((Progress_ExitOpen(pr, p->c.a.room, exit) & 0xFF) == 1) {
        return 4;
    }
    if ((DoorHold_Usable(pr, p->c.a.room, exit) & 0xFF) == 1) {
        return (VCALL(rm, 0x4C, s32 (*)(VObject *, s32, s32))(rm, p->c.a.room, exit) & 0xFF) == 1 ? 6 : 3;
    }
    return 5;
}

/* path length from node `a` to node `b` of `room`, through the room's table (-1 none) */
/* 0x00211F70 */
f32 Npc_NodeDistance(Pursuer *p, s32 room, u32 a, u32 b) {
    VObject *rm = gRooms;
    s16 n = VCALL(rm, 0x38, s32 (*)(VObject *, u32, s32))(rm, b, room);
    f32 d;

    if (n == -1) {
        return -1.0f;
    }
    d = n;
    if ((a & 0xFFFF) != (b & 0xFFFF)) {
        n = VCALL(rm, 0x38, s32 (*)(VObject *, u32, s32))(rm, a, room);
        if (n == -1) {
            return -1.0f;
        }
        d += n;
    }
    return d;
}

/* path length between the room nodes of `a` and `b` (rooms +0x10 / +0x38), -1 none */
/* 0x00212060 */
f32 Npc_RoomNodeDistance(Pursuer *p, s32 room, s32 a, s32 b) {
    VObject *rm = gRooms;
    u32 na = VCALL(rm, 0x10, u32 (*)(VObject *, s32, s32))(rm, room, a) & 0xFFFF;
    u32 nb = VCALL(rm, 0x10, u32 (*)(VObject *, s32, s32))(rm, room, b) & 0xFFFF;
    s16 n = VCALL(rm, 0x38, s32 (*)(VObject *, u32, s32))(rm, nb, room);
    f32 d;

    if (n == -1) {
        return -1.0f;
    }
    d = n;
    if ((na & 0xFFFF) != (nb & 0xFFFF)) {
        n = VCALL(rm, 0x38, s32 (*)(VObject *, u32, s32))(rm, na, room);
        if (n == -1) {
            return -1.0f;
        }
        d += n;
    }
    return d;
}

/* is `pos` of room `room` where the room's spawn point is, and reachable? */
/* 0x00212190 */
s32 Npc_AtSpawn(Pursuer *p, s32 room) {
    VObject *rm = gRooms;
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    u32 tri = VCALL(rm, 0x30, u32 (*)(VObject *, s32, f32 *))(rm, room, a);

    VCALL(rm, 0x34, void (*)(VObject *, s32, f32 *))(rm, room, b);
    if (tri != Actor_TriTo(&p->c.a, a, 0)) {
        return 0;
    }
    return (Actor_NearerRoom(&p->c.a, room, b) & 0xFF) != 0;
}

/* coming into room `room`: is Fiona at the spawn point there (+0x1624)? */
/* 0x00212240 */
void Npc_FionaAtSpawn(Pursuer *p, s32 room) {
    VObject *rm = gRooms;
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));

    PU(p, 0x1624, s32) = 0;
    VCALL(rm, 0x30, u32 (*)(VObject *, s32, f32 *))(rm, room, a);
    VCALL(rm, 0x34, void (*)(VObject *, s32, f32 *))(rm, room, b);
    if ((Actor_NearerRoom(&p->c.a, room, b) & 0xFF) == 1) {
        if (Actor_TriTo(&p->c.a, a, -1) != (u32)-1) {
            p->c.unk124 = p->c.unk128;
        }
        if (!(Progress_ExitPassable(gProgress, p->c.a.room, room, *(u8 *)&p->c.a.slot) & 0xFF)) {
            return;
        }
        if ((Actor_NearerRoom(&p->c.a, room, gCharPlayer->a.pos) & 0xFF) == 1) {
            PU(p, 0x1624, s32) = 1;
        }
    }
}

/* plan a path to triangle +0x15A4 / point +0x15B0; its length to +0x1590 (-1: none) */
/* 0x00212360 */
s32 Npc_PlanToGoal(Pursuer *p) {
    if (Character_PlanPathKind(&p->c, 0, PU(p, 0x15A4, s32), (f32 *)((u8 *)p + 0x15B0)) <= 0) {
        return 0;
    }
    if (Character_Waypoints(&p->c) > 0) {
        PU(p, 0x1590, f32) = VCALL(gSceneGameF29740, 0x30, f32 (*)(VObject *, s32))(gSceneGameF29740, p->c.pathId);
        Character_ReleasePath(&p->c);
        return 1;
    }
    PU(p, 0x1590, f32) = -1.0f;
    Character_ReleasePath(&p->c);
    return 0;
}

/* vtable +0xE8: head for a random other room (10 tries to avoid the played one) */
/* 0x00212400 */
s32 Npc_HeadRandomRoom(Pursuer *p) {
    Progress *pr = gProgress;
    u32 tries = 0;

    do {
        PU(p, 0x1594, s32) = VCALL(pr, 0x38, s32 (*)(Progress *, s32))(pr, -1);
        PU(p, 0x1598, s32) = -1;
        if (PU(p, 0x1594, s32) == VCALL(pr, 0xC, s32 (*)(Progress *))(pr)) {
            tries = (tries + 1) & 0xFF;
            if (tries < 10) {
                PU(p, 0x1594, s32) = p->c.a.room;
            }
        }
    } while (PU(p, 0x1594, s32) == p->c.a.room);
    if (PU(p, 0x1594, s32) != -1 && Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) != -1) {
        return 1;
    }
    do {
        PU(p, 0x1594, s32) = VCALL(pr, 0x38, s32 (*)(Progress *, s32))(pr, p->c.a.room);
    } while (PU(p, 0x1594, s32) == p->c.a.room);
    if (PU(p, 0x1594, s32) != -1) {
        return Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) != -1;
    }
    return 0;
}

/* ---- 0x211C80..0x219460 ---- */

/* vtable +0xE4: nothing */
/* 0x00212540 */
void NPC_DoorBreak(Pursuer *p) {
}

/* the facing of door / exit `exit` seen from Fiona (10 if none): while Fiona hides, the first
 * door she can be behind */
/* 0x00212550 */
f32 Npc_DoorFacingFromFiona(Pursuer *p, u32 exit) {
    VObject *d;
    s8 side;
    f32 a;

    if ((exit & 0xFF) >= 8) {
        u32 i, found = 0xFF;

        if (gCharPlayer->moveMode == 2) {
            VObject *dd = gDoors;
            Progress *pr = gProgress;

            for (i = 0; i < 8; i++) {
                if (VCALL(dd, 0x40, s32 (*)(VObject *, u32))(dd, i & 0xFF) != 0 &&
                    Progress_CurRoomFlag(pr, p->c.a.room, i & 0xFF) != 0 && (PursuerGroup_Fields(pr, i & 0xFF, 0) & 0xFF & 4)) {
                    found = i & 0xFF;
                    break;
                }
            }
        }
        exit = found & 0xFF;
        if (exit >= 8) {
            return 10.0f;
        }
    }
    if (!(VCALL(gRooms, 0x78, s32 (*)(VObject *, s32, u32))(gRooms, p->c.a.room, exit) & 0xFF)) {
        return 10.0f;
    }
    d = gDoors;
    side = VCALL(d, 0x18, s32 (*)(VObject *, u32, f32 *))(d, exit, gCharPlayer->a.pos);
    if (side == -1) {
        return 10.0f;
    }
    a = VCALL(d, 0x3C, f32 (*)(VObject *, u32))(d, exit);
    if (side == 1) {
        a = Angle_Wrap(0x1.921fb60000000p+1f /* 3.1415927 */ + a);
    }
    return a;
}

/* heading through exit `exit` (from its inner to its outer point) */
/* 0x00212730 */
f32 Npc_ExitHeading(Pursuer *p, s32 exit) {
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));

    if (!(VCALL(gRooms, 0x78, s32 (*)(VObject *, s32, s32))(gRooms, p->c.a.room, exit) & 0xFF)) {
        VObject *rm = gRooms;
        void *nm = gNavMesh;

        VCALL(nm, 0xC, void (*)(void *, s32, f32 *))(nm, VCALL(rm, 0x24, s32 (*)(VObject *, s32))(rm, exit), a);
        VCALL(nm, 0xC, void (*)(void *, s32, f32 *))(nm, VCALL(rm, 0x28, s32 (*)(VObject *, s32))(rm, exit), b);
    } else {
        VObject *rm = gRooms;

        VCALL(rm, 0x30, u32 (*)(VObject *, s32, f32 *))(rm, exit, a);
        VCALL(rm, 0x34, void (*)(VObject *, s32, f32 *))(rm, exit, b);
    }
    sceVu0SubVector(d, a, b);
    return Vec_Heading(d);
}

/* a door of the played room the pursuer must deal with on its way (0xFF none) */
/* 0x00212850 */
u32 Npc_DoorOnWay(Pursuer *p) {
    VObject *rm = gRooms;
    VObject *d = gDoors;
    Progress *pr = gProgress;
    u32 i;

    for (i = 0; i < 8; i++) {
        u32 st;

        if ((VCALL(rm, 0x10, u32 (*)(VObject *, s32, u32))(rm, p->c.a.room, i & 0xFF) & 0xFFFF) == 0xFFFF) {
            continue;
        }
        if ((VCALL(d, 0x40, s32 (*)(VObject *, u32))(d, i & 0xFF) & 0xFF) != 1 ||
            (Progress_CurRoomFlag(pr, VCALL(pr, 0xC, s32 (*)(Progress *))(pr), i & 0xFF) & 0xFF) != 1) {
            continue;
        }
        st = PursuerGroup_Fields(pr, i & 0xFF, *(u8 *)&p->c.a.slot) & 0xFF;
        if (VCALL(rm, 0x70, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, i & 0xFF) != 0) {
            st &= 0xFF;
            if ((st & 4) && ((st ^ (PursuerGroup_Fields(pr, i & 0xFF, 0) & 0xFF)) & 0x10)) {
                return i & 0xFF;
            }
            if (st & 8) {
                u32 tri = p->c.a.navTri, flags;

                if (tri < AT(gNavMesh, 0x8, u32) && AT(gNavMesh, 0x4, u8 *) != NULL) {
                    flags = AT(AT(gNavMesh, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
                } else {
                    flags = 0;
                }
                if (p->c.a.navMask & flags) {
                    return i & 0xFF;
                }
            }
            continue;
        }
        if ((st & 0xFF & 1) || VCALL(d, 0x10, s32 (*)(VObject *, u32, s32, s32))(d, i & 0xFF, 0, p->c.a.slot) != 0) {
            return i & 0xFF;
        }
    }
    return 0xFF;
}

/* a random exit of the pursuer's side of the room it can use (not `skip`); `skip` if none */
/* 0x00212A80 */
u32 Npc_RandomExit(Pursuer *p, u32 skip) {
    u8 list[8];
    VObject *rm;
    u32 n = 0, i;

    for (i = 0; i < 8; i++) {
        list[i] = D_0047A930[i];
    }
    rm = gRooms;
    for (i = 0; i < 8; i = (i + 1) & 0xFF) {
        if ((i & 0xFF) == (skip & 0xFF)) {
            continue;
        }
        if (p->c.door < 8) {
            s32 mine = VCALL(rm, 0x50, s32 (*)(VObject *, s32, u32, s32))(rm, p->c.a.room, p->c.door, 1);

            if (VCALL(rm, 0x50, s32 (*)(VObject *, s32, u32, s32))(rm, p->c.a.room, i, 1) != mine) {
                continue;
            }
        }
        if (VCALL(rm, 0x74, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, i) != 0 &&
            (s8)VCALL(p, 0xEC, s32 (*)(Pursuer *, u32))(p, i) != 0) {
            list[n & 0xFF] = i;
            n = (n + 1) & 0xFF;
        }
    }
    if ((n & 0xFF) != 0) {
        if ((n & 0xFF) == 1) {
            return list[0];
        }
        return list[(u32)((f32)n * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom))];
    }
    return skip;
}

/* door `door` shut, the other side ... (doors +0x20 / +0x1C, progress) */
/* 0x00212CA0 */
void Npc_DoorShutOther(Pursuer *p, u32 door) {
    VObject *d = gDoors;
    Progress *pr;

    VCALL(d, 0x20, void (*)(VObject *, u32, s32, s32))(d, door, 1, 0x60000);
    VCALL(d, 0x1C, void (*)(VObject *, u32, s32, s32))(d, door, 0, 0x60000);
    pr = gProgress;
    DoorHold_Shut(pr, VCALL(pr, 0xC, s32 (*)(Progress *))(pr), door, 0xFF);
}

/* 0x00212D30 */
void Npc_DoorRelease(Pursuer *p, u32 door) {
    VObject *d = gDoors;
    Progress *pr;

    VCALL(d, 0x20, void (*)(VObject *, u32, s32, s32))(d, door, 0, 0x60000);
    VCALL(d, 0x1C, void (*)(VObject *, u32, s32, s32))(d, door, 1, 0x60000);
    pr = gProgress;
    DoorHold_Open(pr, VCALL(pr, 0xC, s32 (*)(Progress *))(pr), door, 0xFF);
}

/* shut / open door `door` (doors vtable +0x1C) */
/* 0x00212DC0 */
s32 Npc_DoorShut(Pursuer *p, s32 door) {
    return VCALL(gDoors, 0x1C, s32 (*)(VObject *, s32, s32, s32))(gDoors, door, 0, 0x60000);
}

/* 0x00212DE0 */
s32 Npc_DoorShut2(Pursuer *p, s32 door) {
    return VCALL(gDoors, 0x1C, s32 (*)(VObject *, s32, s32, s32))(gDoors, door, 1, 0x60000);
}

/* plan a path to `pos` / triangle `tri` from the point beside door `door` (side 1 first, then 0);
 * the side that works, 0xFF none */
/* 0x00212E00 */
u32 Npc_PlanFromDoor(Pursuer *p, u32 tri, const f32 *pos, u32 door) {
    VObject *pl;
    u32 side = 1;

    p->c.pathReq->unk0 = 0;
    p->c.pathReq->goalTri = tri;
    sceVu0CopyVector(p->c.pathReq->goalPos, pos);
    pl = gSceneGameF29740;
    for (;;) {
        f32 ofs[4] __attribute__((aligned(16)));
        f32 at[4] __attribute__((aligned(16)));
        u32 t;

        VCALL(p, 0x9C, void (*)(Pursuer *, u32, f32 *))(p, side & 0xFF, ofs);
        t = Actor_DoorFront(p, door & 0xFF, side & 0xFF, ofs, at);
        if (t != (u32)-1) {
            p->c.pathReq->startTri = t;
            sceVu0CopyVector(p->c.pathReq->startPos, at);
            p->c.pathId = VCALL(pl, 0xC, s32 (*)(VObject *, PathRequest *, s32))(pl, p->c.pathReq, 0);
            if (p->c.pathId != -1) {
                if (VCALL(pl, 0x14, s32 (*)(VObject *))(pl) >= 0) {
                    Character_ReleasePath(&p->c);
                    return side;
                }
                Character_ReleasePath(&p->c);
            }
        }
        side = (side == 0) & 0xFF;
        if (side != 0) {
            return 0xFF;
        }
    }
}

/* plan a path to the point beside door `door` (vtable +0x9C offset); 1 if +0xDC agrees */
/* 0x00212F40 */
s32 Npc_PlanBesideDoor(Pursuer *p, u32 door, u32 side) {
    f32 ofs[4] __attribute__((aligned(16)));

    VCALL(p, 0x9C, void (*)(Pursuer *, u32, f32 *))(p, side & 0xFF, ofs);
    PU(p, 0x15C4, s32) = Actor_DoorFront(p, door & 0xFF, side & 0xFF, ofs, (f32 *)((u8 *)p + 0x15D0));
    if (PU(p, 0x15C4, s32) == -1) {
        return 0;
    }
    return (VCALL(p, 0xDC, s32 (*)(Pursuer *))(p) & 0xFF) == 1;
}

/* find a door (0..4) to go through, from side `side` (0 / 1, other values: either); door to
 * +0x100, side to +0x104 */
/* 0x00212FE0 */
s32 Npc_FindDoor(Pursuer *p, s32 side) {
    s8 s = side;
    u32 either = (s != 1 && s != 0) ? 1 : 0;
    u32 i;

    if ((either & 0xFF) == 1) {
        s = 0;
    }
    for (i = 0; i < 5; i++) {
        u32 retry = either & 0xFF;
        s32 valid = (s32)i >= 0 && i < AT(gNavMesh, 0x14, u32);

        if ((valid & 0xFF) != 1) {
            continue;
        }
        for (;;) {
            f32 ofs[4] __attribute__((aligned(16)));
            u32 sd = s != 0;
            s32 ok;

            VCALL(p, 0x9C, void (*)(Pursuer *, u32, f32 *))(p, sd, ofs);
            PU(p, 0x15C4, s32) = Actor_DoorFront(p, i & 0xFF, sd & 0xFF, ofs, (f32 *)((u8 *)p + 0x15D0));
            ok = PU(p, 0x15C4, s32) != -1 && (VCALL(p, 0xDC, s32 (*)(Pursuer *))(p) & 0xFF) == 1;
            if ((ok & 0xFF) == 1) {
                p->c.unk100 = i;
                p->c.unk104[0] = s;
                return 1;
            }
            if ((retry & 0xFF) != 1) {
                break;
            }
            retry = 0;
            s ^= 1;
        }
    }
    return 0;
}

/* the first open door (0..4) while Fiona is hiding (move mode 3), -1 none */
/* 0x002131A0 */
s32 Npc_OpenDoorFionaHides(void) {
    if (gCharPlayer->moveMode == 3) {
        Progress *pr = gProgress;
        s32 i;

        for (i = 0; (u32)i < 5; i++) {
            s32 valid = i >= 0 && (u32)i < AT(gNavMesh, 0x14, u32);

            if ((valid & 0xFF) == 1 && (RoomSlots_Bytes(pr, i & 0xFF, 0) & 0xFF & 1)) {
                return i;
            }
        }
    }
    return -1;
}

/* leave door `door` (0xFF: +0x100): shut / open it behind, blocking flags back, and off its
 * triangle if that blocks the pursuer */
/* 0x00213270 */
void Npc_LeaveDoor(Pursuer *p, u32 door) {
    VObject *d;
    u32 tri, flags;

    if ((door & 0xFF) == 0xFF) {
        door = (u8)p->c.unk100;
    }
    d = gDoors;
    if (VCALL(d, 0x28, s32 (*)(VObject *, u32))(d, door) == 0) {
        Progress *pr;

        VCALL(d, 0x20, void (*)(VObject *, u32, s32, s32))(d, door, 0, 0x60000);
        VCALL(d, 0x1C, void (*)(VObject *, u32, s32, s32))(d, door, 1, 0x60000);
        pr = gProgress;
        DoorHold_Open(pr, VCALL(pr, 0xC, s32 (*)(Progress *))(pr), door, 0xFF);
    } else {
        Progress *pr;

        VCALL(d, 0x20, void (*)(VObject *, u32, s32, s32))(d, door, 1, 0x60000);
        VCALL(d, 0x1C, void (*)(VObject *, u32, s32, s32))(d, door, 0, 0x60000);
        pr = gProgress;
        DoorHold_Shut(pr, VCALL(pr, 0xC, s32 (*)(Progress *))(pr), door, 0xFF);
    }
    p->c.a.unk2B = 0;
    p->c.a.unk2D = 0;
    p->c.a.navMask = VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
    tri = p->c.a.navTri;
    if (tri < AT(gNavMesh, 0x8, u32) && AT(gNavMesh, 0x4, u8 *) != NULL) {
        flags = AT(AT(gNavMesh, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
    } else {
        flags = 0;
    }
    if (p->c.a.navMask & flags) {
        f32 dir[4] __attribute__((aligned(16)));
        s8 side = VCALL(d, 0x18, s32 (*)(VObject *, u32, f32 *))(d, door, p->c.a.pos);

        if (side != 0) {
            if (side == 1) {
                side = 2;
            }
        } else {
            side = 0;
        }
        p->c.a.navTri = VCALL(d, 0x14, u32 (*)(VObject *, u32, s32, f32 *, f32 *, s32))(d, door, side, p->c.a.pos, dir, 1);
        p->c.a.angle[1] = dir[1];
        sceVu0UnitMatrix(p->c.a.rot);
        sceVu0RotMatrixY(p->c.a.rot, p->c.a.rot, dir[1]);
    }
    p->c.moveMode = 0;
}

/* plan a path from triangle `tri` / `pos` (-1: where the pursuer is) to door `door`, side 0 then
 * 2; the door's triangle (+0x104 side, +0x1568 heading, +0x110 point), -1 none */
/* 0x002134E0 */
s32 Npc_PlanToDoor(Pursuer *p, u32 door, s32 tri, const f32 *pos) {
    f32 from[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));
    f32 dir[4] __attribute__((aligned(16)));
    VObject *d, *pl;
    s8 side;

    if (tri != -1) {
        sceVu0CopyVector(from, pos);
    } else {
        tri = p->c.a.navTri;
        sceVu0CopyVector(from, p->c.a.pos);
    }
    d = gDoors;
    pl = gSceneGameF29740;
    side = 0;
    p->c.pathReq->unk0 = 0;
    for (;;) {
        s32 t = VCALL(d, 0x14, s32 (*)(VObject *, u32, s32, f32 *, f32 *, s32))(d, door, side, at, dir, 1);

        p->c.pathReq->startTri = tri;
        sceVu0CopyVector(p->c.pathReq->startPos, from);
        p->c.pathReq->goalTri = t;
        sceVu0CopyVector(p->c.pathReq->goalPos, at);
        p->c.pathId = VCALL(pl, 0xC, s32 (*)(VObject *, PathRequest *, s32))(pl, p->c.pathReq, 0);
        if (p->c.pathId != -1 && VCALL(pl, 0x14, s32 (*)(VObject *))(pl) > 0) {
            p->c.unk104[0] = side;
            PU(p, 0x1568, f32) = dir[1];
            sceVu0CopyVector(p->c.unk110, at);
            return t;
        }
        Character_ReleasePath(&p->c);
        side = side != 2 ? 2 : 0;
        if (side == 0) {
            return -1;
        }
    }
}

/* ---- batch 9 ---- */

/* the exits of the room the pursuer could go through to reach triangle `tri`: door
 * +0x100, point beside it +0x15D0 (an exit it must open as a fallback) */
/* 0x00213690 */
s32 Npc_ExitsToTri(Pursuer *p, s32 tri) {
    f32 v[4] __attribute__((aligned(16)));
    VObject *rm;
    Progress *pr;
    u32 fallback = 0xFF;
    u32 i;

    VCALL(gNavMesh, 0xC, void (*)(void *, s32, f32 *))(gNavMesh, tri, v);
    rm = gRooms;
    pr = gProgress;
    for (i = 0; i < 8; i++) {
        s32 t;

        if (!(VCALL(rm, 0x78, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, i & 0xFF) & 0xFF)) {
            continue;
        }
        if ((Progress_ExitOpen(pr, p->c.a.room, i & 0xFF) & 0xFF) == 1 && !(Progress_CurRoomFlag(pr, p->c.a.room, i & 0xFF) & 0xFF)) {
            continue;
        }
        if ((Progress_ExitUnlocked(pr, p->c.a.room, i & 0xFF) & 0xFF) == 1) {
            continue;
        }
        t = Npc_PlanToDoor(p, i & 0xFF, tri, v);
        if (t == -1) {
            if (VCALL(rm, 0x70, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, i & 0xFF) != 0 || (fallback & 0xFF) == 0xFF) {
                if (Npc_PlanToDoor(p, i & 0xFF, -1, NULL) != -1) {
                    fallback = i & 0xFF;
                }
            }
            continue;
        }
        PU(p, 0x15C4, s32) = Npc_PlanToDoor(p, i & 0xFF, -1, NULL);
        if (PU(p, 0x15C4, s32) != -1 && t != PU(p, 0x15C4, s32)) {
            p->c.unk100 = i;
            sceVu0CopyVector((f32 *)((u8 *)p + 0x15D0), p->c.unk110);
            return 1;
        }
        p->c.unk104[0] = -1;
        PU(p, 0x1568, s32) = 0;
        AT(p, 0x110, s32) = 0;
        AT(p, 0x114, s32) = 0;
        AT(p, 0x118, s32) = 0;
        AT(p, 0x11C, s32) = 0;
    }
    if ((fallback & 0xFF) != 0xFF && VCALL(rm, 0x70, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, fallback) != 0) {
        PU(p, 0x15C4, s32) = Npc_PlanToDoor(p, fallback, -1, NULL);
        p->c.unk100 = fallback & 0xFF;
        sceVu0CopyVector((f32 *)((u8 *)p + 0x15D0), p->c.unk110);
        return 1;
    }
    return 0;
}

/* can the pursuer get round to triangle `tri` through one of the doors (0..4), from the side
 * it is on? door +0x100, side +0x104, point +0x15D0 */
/* 0x002138F0 */
s32 Npc_RoundThroughDoor(Pursuer *p, s32 tri) {
    f32 at[4] __attribute__((aligned(16)));
    f32 ofs[4] __attribute__((aligned(16)));
    f32 out[4] __attribute__((aligned(16)));
    void *nm = gNavMesh;
    VObject *pl;
    s32 below, s, other;
    u32 i;

    VCALL(nm, 0xC, void (*)(void *, s32, f32 *))(nm, tri, at);
    below = p->c.a.pos[1] < at[1];
    s = (below & 0xFF) != 0;
    pl = gSceneGameF29740;
    other = (s == 0) & 0xFF;
    for (i = 0; i < 5; i++) {
        s32 valid = (s32)i >= 0 && i < AT(nm, 0x14, u32);
        s32 t, t2;

        if (!(valid & 0xFF)) {
            continue;
        }
        VCALL(p, 0x9C, void (*)(Pursuer *, u32, f32 *))(p, other, ofs);
        t = Actor_DoorFront(p, i, other, ofs, out);
        if (t == -1) {
            continue;
        }
        p->c.pathReq->unk0 = 0;
        p->c.pathReq->startTri = tri;
        sceVu0CopyVector(p->c.pathReq->startPos, at);
        p->c.pathReq->goalTri = t;
        sceVu0CopyVector(p->c.pathReq->goalPos, out);
        p->c.pathId = VCALL(pl, 0xC, s32 (*)(VObject *, PathRequest *, s32))(pl, p->c.pathReq, 0);
        if (p->c.pathId == -1) {
            continue;
        }
        if (VCALL(pl, 0x14, s32 (*)(VObject *))(pl) <= 0) {
            Character_ReleasePath(&p->c);
            continue;
        }
        Character_ReleasePath(&p->c);
        VCALL(p, 0x9C, void (*)(Pursuer *, u32, f32 *))(p, s, ofs);
        t2 = Actor_DoorFront(p, i, s, ofs, out);
        if (t2 == -1) {
            continue;
        }
        if (Character_PlanPathKind(&p->c, 0, t2, out) < 0) {
            Character_ReleasePath(&p->c);
            continue;
        }
        Character_ReleasePath(&p->c);
        PU(p, 0x15C4, s32) = t2;
        sceVu0CopyVector((f32 *)((u8 *)p + 0x15D0), out);
        p->c.unk100 = i;
        p->c.unk104[0] = s;
        return 1;
    }
    return 0;
}

/* turn to the root motion's direction (rotated to the walk mesh slope through triangle +0x34) */
/* 0x00213B60 */
void Npc_TurnToRootMotion(Pursuer *p, u32 mask) {
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    u8 *m;
    u32 tri;
    f32 h;

    if (mask == (u32)-1) {
        mask = p->c.a.navMask;
    }
    m = p->c.motion;
    VCALL(m, 0x60, void (*)(void *, f32 *))(m, a);
    tri = Actor_TriTo(&p->c.a, a, 0);
    h = Actor_HeadingTo(&p->c.a, a);
    if (tri != (u32)-1) {
        f32 r;

        VCALL(gNavMesh, 0x40, void (*)(void *, u32, f32 *, f32 *, f32 *, u32))(gNavMesh, p->c.a.navTri, b, p->c.a.pos, a, mask);
        r = Angle_Wrap(p->c.a.angle[1] + Angle_Wrap(Actor_HeadingTo(&p->c.a, b) - h));
        p->c.a.angle[1] = r;
        sceVu0UnitMatrix(p->c.a.rot);
        sceVu0RotMatrixY(p->c.a.rot, p->c.a.rot, r);
    }
}

/* path length to triangle `tri` / point `pos`, from the nearest walkable triangle if blocked */
/* 0x00213C60 */
f32 NPC_PathLengthTo(Pursuer *p, u32 tri, const f32 *pos) {
    f32 v[4] __attribute__((aligned(16)));
    u32 flags;

    if (tri < AT(gNavMesh, 0x8, u32) && AT(gNavMesh, 0x4, u8 *) != NULL) {
        flags = AT(AT(gNavMesh, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
    } else {
        flags = 0;
    }
    if (p->c.a.navMask & flags) {
        tri = Npc_NearestWalkable(p, tri, pos, v);
    } else {
        sceVu0CopyVector(v, pos);
    }
    if (tri == (u32)-1) {
        return -1.0f;
    }
    return Character_PathLength(&p->c, tri, v, VCALL(p, 0xA8, u32 (*)(Pursuer *))(p));
}

/* how far to character `c` on foot: straight if in sight on its triangle, else by path */
/* 0x00213D40 */
f32 Npc_FootDistance(Pursuer *p, Character *c) {
    f32 v[4] __attribute__((aligned(16)));
    void *nm;
    u32 tri;

    if (p->c.a.room != c->a.room) {
        return -1.0f;
    }
    tri = c->a.navTri;
    nm = gNavMesh;
    if (VCALL(nm, 0x10, s32 (*)(void *, u32, const f32 *))(nm, tri, c->a.pos) == 4) {
        VCALL(nm, 0xC, void (*)(void *, u32, f32 *))(nm, tri, v);
        if (tri == Actor_TriTo(&p->c.a, v, p->c.a.navMask)) {
            return Actor_Distance(&p->c.a, c->a.pos);
        }
    }
    return VCALL(p, 0xD4, f32 (*)(Pursuer *, u32, const f32 *))(p, tri, c->a.pos);
}

/* height of the bone vtable +0x80 of the model above its base (+0x804), at least 3 */
/* 0x00213E30 */
void Npc_BoneHeight(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));
    u8 *m = p->c.motion;
    s32 bone = VCALL(m, 0x80, s32 (*)(void *))(m);
    f32 h;

    sceVu0CopyVector(v, Skel_Bone(MOTION_AT(p, 0x810, u8 *), bone) + 0xC);
    h = v[1] - MOTION_AT(p, 0x804, f32);
    if (h < 3.0f) {
        h = 3.0f;
    }
    p->c.a.height = h;
}

/* which way to turn to face `heading`: 1 left beyond `a`, 0 right beyond -`a`, 0xFF within;
 * bit 1 when more than `b` off (if `b` >= `a`) */
/* 0x00213EC0 */
u32 Npc_TurnWay(Pursuer *p, f32 heading, f32 a, f32 b) {
    f32 d;
    u32 r;

    if (a <= 0.0f) {
        a = -a;
    }
    if (b <= 0.0f) {
        b = -b;
    }
    d = Angle_Wrap(heading - p->c.a.angle[1]);
    if (d <= a) {
        if (!(d < -a)) {
            return 0xFF;
        }
        r = 0;
    } else {
        r = 1;
    }
    if (b < a) {
        return r;
    }
    if (d <= 0.0f) {
        d = -d;
    }
    if (d <= b) {
        return r;
    }
    return (r | 2) & 0xFF;
}

/* which way to turn to face point `pos` (see Npc_TurnWay) */
/* 0x00213FA0 */
u32 Npc_TurnWayTo(Pursuer *p, const f32 *pos, f32 a, f32 b) {
    f32 heading = Actor_HeadingTo(&p->c.a, pos);
    f32 d;
    u32 r;

    if (a <= 0.0f) {
        a = -a;
    }
    if (b <= 0.0f) {
        b = -b;
    }
    d = Angle_Wrap(heading - p->c.a.angle[1]);
    if (d <= a) {
        if (!(d < -a)) {
            return 0xFF;
        }
        r = 0;
    } else {
        r = 1;
    }
    if (!(b < a)) {
        if (d <= 0.0f) {
            d = -d;
        }
        if (!(d <= b)) {
            r = (r | 2) & 0xFF;
        }
    }
    return r & 0xFF;
}

/* turn towards `heading` by at most `step`; the angle left */
/* 0x002140A0 */
f32 Npc_TurnToward(Pursuer *p, f32 heading, f32 step) {
    f32 cur = p->c.a.angle[1];
    f32 d = Angle_Wrap(heading - cur);
    f32 s = step <= 0.0f ? -step : step;
    f32 ad = d <= 0.0f ? -d : d;
    f32 h;

    if (ad <= s) {
        h = heading;
    } else if (!(d < 0.0f)) {
        h = Angle_Wrap(cur + step);
    } else {
        h = Angle_Wrap(cur - step);
    }
    p->c.a.angle[1] = h;
    Mtx_TurnY(p->c.a.rot, h);
    return Angle_Wrap(heading - h);
}

/* turn towards `pos` by `step` (the shorter way, or the animation's turn if past 120 degrees);
 * the angle left */
/* 0x00214190 */
f32 Npc_TurnTowardPos(Pursuer *p, const f32 *pos, f32 step) {
    f32 h = Actor_HeadingTo(&p->c.a, pos);
    f32 d = h - p->c.a.angle[1];
    f32 r;

    if (d <= 0.0f) {
        d = -d;
    }
    if (!(d < step)) {
        f32 h2 = Actor_HeadingTo(&p->c.a, pos);
        f32 a = Angle_Wrap(h2 - p->c.a.angle[1]) <= 0.0f ? -Angle_Wrap(h2 - p->c.a.angle[1])
                                                            : Angle_Wrap(h2 - p->c.a.angle[1]);
        f32 b = Angle_Wrap(h2 - (p->c.a.angle[1] + MOTION_AT(p, 0x858, f32))) <= 0.0f
                    ? -Angle_Wrap(h2 - (p->c.a.angle[1] + MOTION_AT(p, 0x858, f32)))
                    : Angle_Wrap(h2 - (p->c.a.angle[1] + MOTION_AT(p, 0x858, f32)));
        s32 dir;

        if (a <= 0x1.0c15240000000p+1f /* 2.0943952 */ || a <= b) {
            dir = Angle_Wrap(h2 - p->c.a.angle[1]) <= 0.0f ? -1 : 1;
        } else {
            dir = MOTION_AT(p, 0x858, f32) <= 0.0f ? -1 : 1;
        }
        r = Angle_Wrap(p->c.a.angle[1] + (f32)dir * step);
        p->c.a.angle[1] = r;
        sceVu0UnitMatrix(p->c.a.rot);
        sceVu0RotMatrixY(p->c.a.rot, p->c.a.rot, r);
    } else {
        p->c.a.angle[1] = h;
        sceVu0UnitMatrix(p->c.a.rot);
        sceVu0RotMatrixY(p->c.a.rot, p->c.a.rot, h);
    }
    return Angle_Wrap(h - p->c.a.angle[1]);
}

/* step towards `pos`: turn, then walk by the animation's stride (straight if within it) */
/* 0x002143D0 */
s32 Npc_StepToward(Pursuer *p, const f32 *pos) {
    f32 v[4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));
    u8 *mo;
    f32 d, ad;

    Motion_RootMovement(p->c.motion, v, 0.0f);
    mo = p->c.motion;
    v[2] *= VCALL(mo, 0x44, f32 (*)(void *, Pursuer *))(mo, p);
    d = Angle_Wrap(Actor_HeadingTo(&p->c.a, pos) - p->c.a.angle[1]);
    ad = d <= 0.0f ? -d : d;
    if (!(ad < 0x1.921fb60000000p-1f /* 0.7853982 */)) {
        if (d <= 0.0f) {
            d = -d;
        }
        if (d < 0x1.921fb60000000p+0f /* 1.5707964 */ && !(Actor_Distance(&p->c.a, pos) <= 10.0f)) {
            Npc_TurnTowardPos(p, pos, 2.0f * VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
            return Actor_Distance(&p->c.a, pos) < 1.0f;
        }
        Npc_TurnTowardPos(p, pos, 2.0f * VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
        return 0;
    }
    Npc_TurnTowardPos(p, pos, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    if (Actor_Distance(&p->c.a, pos) <= v[2] && Actor_TriTo(&p->c.a, pos, -1) != (u32)-1) {
        sceVu0SubVector(v, pos, p->c.a.pos);
    } else {
        Mtx_TurnY(m, Actor_HeadingTo(&p->c.a, pos));
        Mtx_ApplyVector(v, m, v);
    }
    Actor_Move(&p->c.a, v);
    return Actor_Distance(&p->c.a, pos) < 1.0f;
}

/* walk the planned path one stride (the animation's); 1 at its end */
/* 0x00214620 */
s32 Npc_WalkPathStride(Pursuer *p, s32 unused) {
    f32 v[4] __attribute__((aligned(16)));
    f32 w[4] __attribute__((aligned(16)));
    f32 pos[4] __attribute__((aligned(16)));
    u8 *m;
    f32 step, d;
    s32 next = -1;
    u32 tri;
    s32 last;

    Motion_RootMovement(p->c.motion, v, 0.0f);
    m = p->c.motion;
    step = v[2] * VCALL(m, 0x44, f32 (*)(void *, Pursuer *))(m, p);
    if (!(step < 0.0f)) {
        next = Character_WaypointAhead(&p->c, &tri, pos, step);
    }
    if (next < 0) {
        return 0;
    }
    last = AT(p, 0x120 + p->c.unk124 * 0xC, s32);
    w[0] = AT(p, 0x124 + p->c.unk124 * 0xC, f32);
    w[2] = AT(p, 0x128 + p->c.unk124 * 0xC, f32);
    VCALL(gNavMesh, 0x14, void (*)(void *, s32, f32 *))(gNavMesh, last, w);
    if (sceVu0InnerProduct(p->c.a.pos, pos) == 0.0f && last == (s32)Actor_TriTo(&p->c.a, w, 0x20008)) {
        d = Angle_Wrap(Actor_HeadingTo(&p->c.a, w) - p->c.a.angle[1]);
    } else {
        d = Angle_Wrap(Actor_HeadingTo(&p->c.a, pos) - p->c.a.angle[1]);
    }
    if (d <= 0.0f) {
        d = -d;
    }
    if (!(d < 0x1.921fb60000000p-1f /* 0.7853982 */) &&
        !(VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(gSceneGameF29740, p->c.a.pos, p->c.unk128, p->c.unk124, p->c.unk12C) < 4.0f)) {
        Npc_TurnTowardPos(p, pos, 2.0f * VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    } else {
        Npc_TurnTowardPos(p, pos, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
        p->c.a.navTri = tri;
        sceVu0CopyVector(p->c.a.pos, pos);
        p->c.unk128 = next;
    }
    return p->c.unk128 >= p->c.unk124;
}

/* step along the path; the stride from the animation's root motion if `step` <= 0 */
/* 0x00214890 */
s32 Npc_StepPath(Pursuer *p, u32 *triOut, f32 *posOut, f32 step) {
    if (step <= 0.0f) {
        f32 v[4] __attribute__((aligned(16)));
        u8 *m;

        Motion_RootMovement(p->c.motion, v, 0.0f);
        m = p->c.motion;
        step = v[2] * VCALL(m, 0x44, f32 (*)(void *, Pursuer *))(m, p);
    }
    if (step < 0.0f) {
        return -1;
    }
    return Character_WaypointAhead(&p->c, triOut, posOut, step);
}

/* a random walkable triangle of the played room (not blocked, not flagged 0x100000 without
 * 0x200000... ); -1 if the pursuer is elsewhere */
/* 0x00214940 */
u32 Npc_RandomTri(Pursuer *p) {
    if (p->c.a.room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        VObject *rnd = gRandom;
        s32 last = AT(gNavMesh, 0x8, s32) - 1;
        s32 n = last + 1;

        for (;;) {
            u32 tri;
            u32 flags;

            tri = last > 0 ? (s32)((f32)n * VCALL(rnd, 0x1C, f32 (*)(VObject *))(rnd)) : 0;
            if (tri < AT(gNavMesh, 0x8, u32) && AT(gNavMesh, 0x4, u8 *) != NULL) {
                flags = AT(AT(gNavMesh, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
            } else {
                flags = 0;
            }
            if (flags & p->c.a.navMask) {
                continue;
            }
            if (!(flags & 0x100000) || !(flags & 0x200000)) {
                return tri;
            }
        }
    }
    return -1;
}

/* is nav triangle `tri` blocked for the pursuer? */
/* 0x00214A90 */
s32 Npc_TriBlocked(Pursuer *p, u32 tri) {
    u32 flags;

    if (tri < AT(gNavMesh, 0x8, u32) && AT(gNavMesh, 0x4, u8 *) != NULL) {
        flags = AT(AT(gNavMesh, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
    } else {
        flags = 0;
    }
    return (p->c.a.navMask & flags) != 0;
}

/* plan a path to the room object behind the exit the pursuer heads for */
/* 0x00214AF0 */
s32 Npc_PlanToRoomObject(Pursuer *p) {
    VObject *o = VCALL(gEvents, 0x64, VObject *(*)(VObject *))(gEvents);
    s32 *t = VCALL(o, 0x3C, s32 *(*)(VObject *))(o);

    if (t != NULL) {
        s32 r = t[p->c.door];

        if (r != -1) {
            PU(p, 0x1594, s32) = r;
            PU(p, 0x1598, s32) = -1;
            if (Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) > 0) {
                return 1;
            }
        }
    }
    return 0;
}

/* path length to triangle `tri` / point `pos` (null: the triangle's centre), -1 none */
/* 0x00214B90 */
f32 Npc_PathLength(Pursuer *p, u32 tri, const f32 *pos) {
    f32 v[4] __attribute__((aligned(16)));

    if (pos != NULL) {
        sceVu0CopyVector(v, pos);
    } else {
        VCALL(gNavMesh, 0xC, void (*)(void *, u32, f32 *))(gNavMesh, tri, v);
    }
    if (Character_PlanPathKind(&p->c, 0, tri, v) <= 0) {
        return -1.0f;
    }
    if (Character_WaypointsCurve(&p->c) > 0) {
        f32 d = VCALL(gSceneGameF29740, 0x30, f32 (*)(VObject *, s32))(gSceneGameF29740, p->c.pathId);

        Character_ReleasePath(&p->c);
        return d;
    }
    Character_ReleasePath(&p->c);
    return -1.0f;
}

/* 0x00214C70 */
void Npc_Senses2(Pursuer *p) {
    s32 room = p->c.a.room;

    if (room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        f32 a[4] __attribute__((aligned(16)));
        f32 b[4] __attribute__((aligned(16)));

        PU(p, 0x1588, f32) = room == gCharPlayer->a.room ? Npc_DistanceTo(p, gCharPlayer, a) : -1.0f;
        PU(p, 0x158C, f32) = p->c.a.room == gCharPartner->a.room ? Npc_DistanceTo(p, gCharPartner, b) : -1.0f;
    } else {
        PU(p, 0x158C, f32) = -1.0f;
        PU(p, 0x1588, f32) = -1.0f;
    }
}

/* the same, unless the progress byte +0x1FBEC1 is set (then Npc_Senses2) */
/* 0x00214ED0 */
void Npc_Senses2Ending(Pursuer *p) {
    if (AT(gProgress, 0x1FBEC1, u8) == 0) {
        s32 room = p->c.a.room;

        if (room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
            f32 a[4] __attribute__((aligned(16)));
            f32 b[4] __attribute__((aligned(16)));

            PU(p, 0x1588, f32) = Npc_DistanceTo(p, gCharPlayer, a);
            PU(p, 0x158C, f32) = p->c.a.room == gCharPartner->a.room ? Npc_DistanceTo(p, gCharPartner, b) : -1.0f;
        } else {
            PU(p, 0x158C, f32) = -1.0f;
            PU(p, 0x1588, f32) = -1.0f;
        }
        return;
    }
    Npc_Senses2(p);
}

/* 0x00215130 */
s32 Npc_SensesFiona(Pursuer *p) {
    return Npc_Senses(p, 1);
}

/* the same, Fiona watched wherever she is; Npc_SensesFiona while the progress byte +0x1FBEC1 is set */
/* 0x00215D80 */
s32 Npc_SensesWatching(Pursuer *p) {
    if (AT(gProgress, 0x1FBEC1, u8) != 0) {
        return Npc_SensesFiona(p);
    }
    return Npc_Senses(p, 0);
}

/* ---- batch 7 ---- */

/* path length to character `c` (null: the target): +0x1590, and +0x1588 (Fiona) / +0x158C
 * (Hewie) */
/* 0x00216960 */
s32 NPC_PathLengthChar(Pursuer *p, Character *c) {
    f32 v[4] __attribute__((aligned(16)));
    f32 w[4] __attribute__((aligned(16)));
    void *nm;
    u32 tri;
    f32 len;

    if (c == NULL) {
        c = p->target;
    }
    nm = gNavMesh;
    if (VCALL(nm, 0x10, s32 (*)(void *, u32, f32 *))(nm, c->a.navTri, c->a.pos) == 4) {
        VCALL(nm, 0xC, void (*)(void *, u32, f32 *))(nm, c->a.navTri, v);
    } else {
        sceVu0CopyVector(v, c->a.pos);
    }
    tri = Npc_NearestWalkable(p, c->a.navTri, v, v);
    sceVu0CopyVector(w, v);
    if (Character_PlanPathKind(&p->c, 0, tri, w) <= 0) {
        len = -1.0f;
    } else if (Character_WaypointsCurve(&p->c) > 0) {
        len = VCALL(gSceneGameF29740, 0x30, f32 (*)(VObject *, s32))(gSceneGameF29740, p->c.pathId);
        Character_ReleasePath(&p->c);
    } else {
        Character_ReleasePath(&p->c);
        len = -1.0f;
    }
    PU(p, 0x1590, f32) = len;
    if (c == gCharPlayer) {
        PU(p, 0x1588, f32) = PU(p, 0x1590, f32);
    } else if (c == gCharPartner) {
        PU(p, 0x158C, f32) = PU(p, 0x1590, f32);
    }
    if (PU(p, 0x1590, f32) < 0.0f) {
        return 0;
    }
    return 1;
}

/* ---- batch 6 ---- */

/* path length to the nearest walkable point of triangle +0x15C4 / point +0x15D0 (+0x1590) */
/* 0x00216B20 */
s32 NPC_PathLengthSpot(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));
    f32 w[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    u32 tri = Npc_NearestWalkable(p, PU(p, 0x15C4, u32), (f32 *)((u8 *)p + 0x15D0), v);
    f32 len;

    if (tri == (u32)-1) {
        PU(p, 0x1590, f32) = -1.0f;
        return 0;
    }
    sceVu0CopyVector(w, v);
    if (Character_PlanPathKind(&p->c, 0, tri, w) <= 0) {
        len = -1.0f;
    } else if (Character_WaypointsCurve(&p->c) > 0) {
        len = VCALL(gSceneGameF29740, 0x30, f32 (*)(VObject *, s32))(gSceneGameF29740, p->c.pathId);
        Character_ReleasePath(&p->c);
    } else {
        Character_ReleasePath(&p->c);
        len = -1.0f;
    }
    PU(p, 0x1590, f32) = len;
    if (len < 0.0f) {
        return 0;
    }
    sceVu0SubVector(d, v, (f32 *)((u8 *)p + 0x15D0));
    d[3] = 0.0f;
    PU(p, 0x1590, f32) += __builtin_sqrtf(sceVu0InnerProduct(d, d));
    return 1;
}

/* the same for the goal triangle +0x15A4 / point +0x15B0 */
/* 0x00216C90 */
s32 NPC_PathLengthGoal(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));
    f32 w[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    u32 tri = Npc_NearestWalkable(p, PU(p, 0x15A4, u32), (f32 *)((u8 *)p + 0x15B0), v);
    f32 len;

    if (tri == (u32)-1) {
        PU(p, 0x1590, f32) = -1.0f;
        return 0;
    }
    sceVu0CopyVector(w, v);
    if (Character_PlanPathKind(&p->c, 0, tri, w) <= 0) {
        len = -1.0f;
    } else if (Character_WaypointsCurve(&p->c) > 0) {
        len = VCALL(gSceneGameF29740, 0x30, f32 (*)(VObject *, s32))(gSceneGameF29740, p->c.pathId);
        Character_ReleasePath(&p->c);
    } else {
        Character_ReleasePath(&p->c);
        len = -1.0f;
    }
    PU(p, 0x1590, f32) = len;
    if (len < 0.0f) {
        return 0;
    }
    sceVu0SubVector(d, v, (f32 *)((u8 *)p + 0x15B0));
    d[3] = 0.0f;
    PU(p, 0x1590, f32) += __builtin_sqrtf(sceVu0InnerProduct(d, d));
    return 1;
}

/* ---- batch 11 ---- */

/* the walkable point nearest to triangle `tri` / `pos` for the pursuer: from a door it may
 * block, through the walk mesh to the first free triangle (a bit inside it) */
/* 0x00216E00 */
u32 Npc_NearestWalkable(Pursuer *p, u32 tri, const f32 *pos, f32 *out) {
    void *nm = gNavMesh;
    u32 flags;

    if (tri < AT(nm, 0x8, u32) && AT(nm, 0x4, u8 *) != NULL) {
        flags = AT(AT(nm, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
    } else {
        flags = 0;
    }
    if (p->c.a.navMask & flags) {
        VObject *rm = gRooms;
        VObject *d = gDoors;
        u32 t = tri, i;

        for (i = 0; i < 8; i = (i + 1) & 0xFF) {
            if (VCALL(rm, 0x70, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, i) & 0xFF) {
                continue;
            }
            if (VCALL(d, 0x6C, s32 (*)(VObject *, s32, u32, const f32 *))(d, 0, i, pos) != 0) {
                t = VCALL(rm, 0x28, u32 (*)(VObject *, u32))(rm, i);
                break;
            }
        }
        if (t == tri) {
            t = Npc_TriIfStandable(p, t);
        }
        if (t != (u32)-1 && t < AT(nm, 0x8, u32)) {
            f32 c[4] __attribute__((aligned(16)));
            f32 e[4] __attribute__((aligned(16)));
            f32 d2[4] __attribute__((aligned(16)));
            f32 v[4] __attribute__((aligned(16)));

            for (;;) {
                s32 r;
                u8 *te;
                u32 next, nflags;

                VCALL(nm, 0xC, void (*)(void *, u32, f32 *))(nm, t, c);
                r = VCALL(nm, 0x24, s32 (*)(void *, u32, f32 *, f32 *, const f32 *))(nm, t, e, c, pos);
                if (r == 4) {
                    return -1;
                }
                if (r == 3) {
                    sceVu0CopyVector(e, pos);
                    break;
                }
                te = t < AT(nm, 0x8, u32) && AT(nm, 0x4, u8 *) != NULL ? AT(nm, 0x4, u8 *) + t * 0x50 : NULL;
                next = AT(te + r * 4, 0x30, u32);
                if (next < AT(nm, 0x8, u32) && AT(nm, 0x4, u8 *) != NULL) {
                    nflags = AT(AT(nm, 0x4, u8 *) + next * 0x50, 0x3C, u32);
                } else {
                    nflags = 0;
                }
                if (p->c.a.navMask & nflags) {
                    break;
                }
                t = next;
            }
            sceVu0SubVector(d2, c, pos);
            Heading_Vector(v, Vec_Heading(d2));
            sceVu0ScaleVector(v, v, 0x1.99999a0000000p-4f /* 0.1 */);
            sceVu0AddVector(out, e, v);
            return t;
        }
    }
    sceVu0CopyVector(out, pos);
    return tri;
}

/* can the pursuer walk straight to `pos` (over triangles without flag 0x4000)? */
/* 0x00217110 */
s32 Npc_CanWalkStraight(Pursuer *p, const f32 *pos) {
    u32 tri = Actor_TriTo(&p->c.a, pos, 0);

    if (tri == (u32)-1) {
        void *nm = gNavMesh;
        u32 t = p->c.a.navTri;

        for (;;) {
            u8 *e = t < AT(nm, 0x8, u32) && AT(nm, 0x4, u8 *) != NULL ? AT(nm, 0x4, u8 *) + t * 0x50 : NULL;
            s32 r;

            if (AT(e, 0x3C, u32) & 0x4000) {
                return 0;
            }
            r = VCALL(nm, 0x20, s32 (*)(void *, u32, const f32 *, const f32 *))(nm, t, p->c.a.pos, pos);
            if (r == 3 || r == 4) {
                return 0;
            }
            t = AT(e + r * 4, 0x30, u32);
            if (t == (u32)-1) {
                return 1;
            }
        }
    }
    return (Actor_CanWalkBetween(&p->c.a, p->c.a.navTri, tri, p->c.a.pos, pos, 0) & 0xFF) == 0;
}

/* reached the room +0x1594 (and side +0x1598, -1 any)? */
/* 0x00217260 */
s32 Npc_ReachedRoom(Pursuer *p) {
    s32 room = p->c.a.room;

    if (PU(p, 0x1594, s32) == room) {
        if (PU(p, 0x1598, s32) != -1) {
            s32 side = VCALL(gRooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(gRooms, room, p->c.door, 1);

            if (side != -1) {
                return side == PU(p, 0x1598, s32);
            }
        }
        return 1;
    }
    return 0;
}

/* the side of character `c`'s room behind its exit, -1 without one */
/* 0x002172F0 */
s32 Npc_CharSideBehind(Pursuer *p, Character *c) {
    if (c != NULL) {
        return VCALL(gRooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(gRooms, c->a.room, c->door, 1);
    }
    return -1;
}

/* the room's side behind the exit the pursuer heads for (rooms vtable +0x50) */
/* 0x00217340 */
s32 Npc_ExitSideBehind(Pursuer *p) {
    return VCALL(gRooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(gRooms, p->c.a.room, p->c.door, 1);
}

/* are the pursuer and `c` in the same room but on different sides? */
/* 0x00217370 */
s32 Npc_SameRoomOtherSide(Pursuer *p, Character *c) {
    s32 room = p->c.a.room;

    if (room == c->a.room && p->c.door < 8 && (c->door & 0xFF) < 8) {
        VObject *rm = gRooms;
        s32 a = VCALL(rm, 0x50, s32 (*)(VObject *, s32, u32, s32))(rm, room, p->c.door, 1);
        s32 b = -1;

        if (a != -1) {
            if (c != NULL) {
                b = VCALL(rm, 0x50, s32 (*)(VObject *, s32, u32, s32))(rm, c->a.room, c->door, 1);
            }
            if (b != -1) {
                return b != a;
            }
        }
    }
    return 0;
}

/* is character `slot` in the pursuer's room or a neighbouring one? */
/* 0x00217460 */
s32 Npc_NearRoom(Pursuer *p, s32 slot) {
    s32 room = gCharacters[slot]->a.room;
    VObject *rm;
    u32 i;

    if (room == p->c.a.room) {
        return 1;
    }
    rm = gRooms;
    for (i = 0; i < 8; i = (i + 1) & 0xFF) {
        if (room == VCALL(rm, 0x18, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, i)) {
            return 1;
        }
    }
    return 0;
}

/* in the room being played? */
/* 0x00217510 */
s32 Npc_InPlayedRoom(Pursuer *p) {
    return p->c.a.room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
}

/* is Fiona panicking (fear over 90, or her state 0xE)? */
/* 0x00217560 */
s32 Npc_FionaPanicking(void) {
    s32 r = 1;

    if (AT(gCharPlayer, 0x1AD5F4, f32) <= 90.0f) {
        r = 0;
    }
    if (r == 0) {
        r = AT(gCharPlayer, 0x1AD580, s32) == 0xE;
    }
    return r;
}

/* is `b` on about the same floor as `a` (up to 15 above, 10 below)? */
/* 0x002175B0 */
s32 Npc_SameFloor(Actor *a, Actor *b) {
    s32 r = 0;

    if (b->pos[1] <= a->pos[1] + 15.0f && !(b->pos[1] + 10.0f < a->pos[1])) {
        r = 1;
    }
    return r;
}

/* the walkable triangle one height ahead (along the heading); 0 if none */
/* 0x00217600 */
s32 Npc_WalkableAhead(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));
    u32 tri;

    Heading_Vector(v, p->c.a.angle[1]);
    sceVu0ScaleVector(v, v, p->c.a.height);
    sceVu0AddVector(v, v, p->c.a.pos);
    tri = Actor_TriTo(&p->c.a, v, p->c.a.navMask);
    if (tri == (u32)-1) {
        return 0;
    }
    return Actor_TriFree(p, tri, (s32)v);
}

/* vtable +0x... : who's around (+0x1544 Fiona, +0x1545 Hewie, +0x1546 noise) */
/* 0x00217680 */
void Npc_WhoAround(Pursuer *p) {
    Progress *pr = gProgress;
    s32 room = p->c.a.room;

    if (room != VCALL(pr, 0xC, s32 (*)(Progress *))(pr)) {
        PU(p, 0x1544, u8) = 0;
        PU(p, 0x1545, u8) = 0;
    } else {
        PU(p, 0x1574, f32) = Angle_Wrap(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
        if (p->c.moveMode != 2 && (Npc_DoorOnWay(p) & 0xFF) != 0xFF) {
            PU(p, 0x1544, u8) = 1;
        } else if (p->c.a.room != gCharPlayer->a.room) {
            PU(p, 0x1544, u8) = 0;
        } else {
            PU(p, 0x1544, u8) = VCALL(p, 0xC0, s32 (*)(Pursuer *))(p);
        }
        if (VCALL(pr, 0xC, s32 (*)(Progress *))(pr) != gCharPartner->a.room) {
            PU(p, 0x1545, u8) = 0;
        } else {
            PU(p, 0x1545, u8) = VCALL(p, 0xC4, s32 (*)(Pursuer *))(p);
        }
    }
    PU(p, 0x1546, u8) = VCALL(p, 0xC8, s32 (*)(Pursuer *))(p);
    VCALL(p, 0xCC, void (*)(Pursuer *))(p);
}

/* the same, unless the progress byte +0x1FBEC1 is set (then Npc_WhoAround) */
/* 0x002177D0 */
void Npc_WhoAroundEnding(Pursuer *p) {
    Progress *pr = gProgress;

    if (AT(pr, 0x1FBEC1, u8) == 0) {
        s32 room = p->c.a.room;

        if (room != VCALL(pr, 0xC, s32 (*)(Progress *))(pr)) {
            PU(p, 0x1544, u8) = 0;
            PU(p, 0x1545, u8) = 0;
        } else {
            PU(p, 0x1574, f32) = Angle_Wrap(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
            if (p->c.moveMode != 2 && (Npc_DoorOnWay(p) & 0xFF) != 0xFF) {
                PU(p, 0x1544, u8) = 1;
            } else {
                PU(p, 0x1544, u8) = VCALL(p, 0xC0, s32 (*)(Pursuer *))(p);
            }
            if (VCALL(pr, 0xC, s32 (*)(Progress *))(pr) != gCharPartner->a.room) {
                PU(p, 0x1545, u8) = 0;
            } else {
                PU(p, 0x1545, u8) = VCALL(p, 0xC4, s32 (*)(Pursuer *))(p);
            }
        }
        PU(p, 0x1546, u8) = VCALL(p, 0xC8, s32 (*)(Pursuer *))(p);
        VCALL(p, 0xCC, void (*)(Pursuer *))(p);
        return;
    }
    Npc_WhoAround(p);
}

/* who can the pursuer see (bit per character slot 0..2)? */
/* 0x00217920 */
s32 Npc_WhoSeen(Pursuer *p) {
    s32 seen = 0;
    u32 i;

    for (i = 0; i < 3; i = (i + 1) & 0xFF) {
        Character *c = gCharacters[i & 0xFF];

        if (c != NULL && (i & 0xFF) != (u32)p->c.a.slot && c->a.active != 0 &&
            (Actor_Touching(&p->c.a, &c->a, 1.0f, 0.0f) & 0xFF) == 1) {
            seen = (seen | ((1 << (i & 0xFF)) & 0xFF)) & 0xFF;
        }
    }
    return seen;
}

/* who of the characters can the pursuer reach / see by the progress tables (+0x30 / +0x2C) and is
 * in front of it (within 90 degrees): a bit per slot */
/* 0x002179F0 */
s32 Npc_WhoReachable(Pursuer *p, s32 a1, f32 f) {
    Progress *pr = gProgress;
    s32 bits = 0;
    u32 i;

    for (i = 0; i < 3; i = (i + 1) & 0xFF) {
        u32 s = i & 0xFF;
        Character **c = &gCharacters[s];

        if (*c != NULL && s != (u32)p->c.a.slot && (*c)->a.active != 0 &&
            (VCALL(pr, 0x30, s32 (*)(Progress *, u32, s32, u32, f32))(pr, p->c.a.slot & 0xFF, a1, i, f) & 0xFF) == 1) {
            f32 d = Angle_Wrap(p->c.a.angle[1] - Actor_HeadingTo(&p->c.a, (*c)->a.pos)) <= 0.0f
                        ? -Angle_Wrap(p->c.a.angle[1] - Actor_HeadingTo(&p->c.a, (*c)->a.pos))
                        : Angle_Wrap(p->c.a.angle[1] - Actor_HeadingTo(&p->c.a, (*c)->a.pos));

            if (d < 0x1.921fb60000000p+0f /* 1.5707964 */) {
                bits = (bits | ((1 << s) & 0xFF)) & 0xFF;
            }
        }
    }
    return bits;
}

/* 0x00217B90 */
s32 Npc_WhoReachableBits(Pursuer *p, s32 a1, f32 f) {
    Progress *pr = gProgress;
    s32 bits = 0;
    u32 i;

    for (i = 0; i < 3; i = (i + 1) & 0xFF) {
        u32 s = i & 0xFF;
        Character **c = &gCharacters[s];

        if (*c != NULL && s != (u32)p->c.a.slot && (*c)->a.active != 0 &&
            (VCALL(pr, 0x2C, s32 (*)(Progress *, u32, s32, u32, f32))(pr, p->c.a.slot & 0xFF, a1, i, f) & 0xFF) == 1) {
            f32 d = Angle_Wrap(p->c.a.angle[1] - Actor_HeadingTo(&p->c.a, (*c)->a.pos)) <= 0.0f
                        ? -Angle_Wrap(p->c.a.angle[1] - Actor_HeadingTo(&p->c.a, (*c)->a.pos))
                        : Angle_Wrap(p->c.a.angle[1] - Actor_HeadingTo(&p->c.a, (*c)->a.pos));

            if (d < 0x1.921fb60000000p+0f /* 1.5707964 */) {
                bits = (bits | ((1 << s) & 0xFF)) & 0xFF;
            }
        }
    }
    return bits;
}

/* the triangle `dist` away in direction `heading`: -1 none, -2 blocked, -3 flag 1, -4 at a
 * door, -5 at a room point */
/* 0x00217D30 */
u32 Npc_TriAtDirection(Pursuer *p, f32 heading, f32 dist) {
    f32 v[4] __attribute__((aligned(16)));
    f32 w[4] __attribute__((aligned(16)));
    void *nm;
    u32 tri, flags, i;

    Heading_Vector(v, Angle_Wrap(heading));
    sceVu0ScaleVector(v, v, dist);
    sceVu0AddVector(w, p->c.a.pos, v);
    tri = Actor_TriTo(&p->c.a, w, 0);
    if (tri == (u32)-1) {
        return -1;
    }
    nm = gNavMesh;
    if (tri < AT(nm, 0x8, u32) && AT(nm, 0x4, u8 *) != NULL) {
        flags = AT(AT(nm, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
    } else {
        flags = 0;
    }
    if (flags & p->c.a.navMask) {
        return -2;
    }
    if (flags & 1) {
        return -3;
    }
    for (i = 0; i < 8; i++) {
        if (((VCALL(gDoors, 0x2C, s32 (*)(VObject *, u32, f32 *))(gDoors, i & 0xFF, w) != 0) ^ 1) == 0) {
            return -4;
        }
    }
    for (i = 0; i < 5; i++) {
        if (VCALL(nm, 0x50, s32 (*)(void *, u32, f32 *))(nm, i, w) != 0) {
            return -5;
        }
    }
    return tri;
}

/* is the point `dist` away in direction `angle` from the target's heading walkable? */
/* 0x00217ED0 */
s32 Npc_TargetSideWalkable(Pursuer *p, f32 angle, f32 dist) {
    f32 v[4] __attribute__((aligned(16)));
    f32 w[4] __attribute__((aligned(16)));
    u32 tri;

    Heading_Vector(v, Angle_Wrap(angle + Actor_HeadingTo(&p->c.a, p->target->a.pos)));
    sceVu0ScaleVector(v, v, dist);
    sceVu0AddVector(w, p->c.a.pos, v);
    tri = Actor_TriTo(&p->c.a, w, p->c.a.navMask);
    if (tri != (u32)-1) {
        u32 flags;

        if (tri < AT(gNavMesh, 0x8, u32) && AT(gNavMesh, 0x4, u8 *) != NULL) {
            flags = AT(AT(gNavMesh, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
        } else {
            flags = 0;
        }
        if (!(p->c.a.navMask & flags)) {
            return 1;
        }
    }
    return 0;
}

/* is there room `dist` to the side of the target (90 degrees one way, then the other)? */
/* 0x00217FC0 */
s32 Npc_RoomToSide(Pursuer *p, f32 dist) {
    f32 a = 0x1.921fb60000000p+0f /* 1.5707964 */;

    for (;;) {
        f32 v[4] __attribute__((aligned(16)));
        f32 w[4] __attribute__((aligned(16)));
        u32 tri;
        s32 ok = 0;

        Heading_Vector(v, Angle_Wrap(a + Actor_HeadingTo(&p->c.a, p->target->a.pos)));
        sceVu0ScaleVector(v, v, dist);
        sceVu0AddVector(w, p->c.a.pos, v);
        tri = Actor_TriTo(&p->c.a, w, p->c.a.navMask);
        if (tri != (u32)-1) {
            u32 flags;

            if (tri < AT(gNavMesh, 0x8, u32) && AT(gNavMesh, 0x4, u8 *) != NULL) {
                flags = AT(AT(gNavMesh, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
            } else {
                flags = 0;
            }
            if (!(p->c.a.navMask & flags)) {
                ok = 1;
            }
        }
        if ((ok & 0xFF) == 1) {
            return 1;
        }
        a = -a;
        if (!(a < 0.0f)) {
            return 0;
        }
    }
}

/* probe the 8 directions around Fiona, 20 units out (results to +0x1548) */
/* 0x00218110 */
void Npc_ProbeAroundFiona(Pursuer *p) {
    s32 i;

    for (i = 0; i < 8; i++) {
        f32 h = Actor_HeadingTo(&p->c.a, gCharPlayer->a.pos);

        PU(p, 0x1548 + i * 4, s32) = Npc_TriAtDirection(p, Angle_Wrap(0x1.921fb60000000p+2f /* 6.2831855 */ * (f32)i / 8.0f + h), 20.0f);
    }
}

/* vtable +0xC8: reacting to a noise */
/* 0x002181C0 */
s32 NPC_HearNoise(Pursuer *p) {
    return p->c.heardSlot != 0xFF;
}

/* can an eye at `from` facing `heading` see `to`: within `range` and `half` an angle either side */
/* 0x002181D0 */
s32 Eye_CanSee(Pursuer *p, const f32 *from, const f32 *to, f32 heading, f32 range, f32 half) {
    f32 d[4] __attribute__((aligned(16)));
    f32 dist, dx, dz, a;

    sceVu0SubVector(d, from, to);
    d[3] = 0.0f;
    dist = __builtin_sqrtf(sceVu0InnerProduct(d, d));
    dx = to[0] - from[0];
    dz = to[2] - from[2];
    if (dx == 0.0f && dz == 0.0f) {
        return 0;
    }
    a = msl_atan2f(dx, dz);
    if (!(dist <= range)) {
        return 0;
    }
    a = a - heading;
    if (!((Angle_Wrap(a) <= 0.0f ? -Angle_Wrap(a) : Angle_Wrap(a)) <= half)) {
        return 0;
    }
    return 1;
}

/* the same between two actors */
/* 0x00218300 */
s32 Eye_ActorSees(Pursuer *p, Actor *from, Actor *to, f32 heading, f32 range, f32 half) {
    f32 d[4] __attribute__((aligned(16)));
    f32 dist, dx, dz, a;

    sceVu0SubVector(d, from->pos, to->pos);
    d[3] = 0.0f;
    dist = __builtin_sqrtf(sceVu0InnerProduct(d, d));
    dx = to->pos[0] - from->pos[0];
    dz = to->pos[2] - from->pos[2];
    if (dx == 0.0f && dz == 0.0f) {
        return 0;
    }
    a = msl_atan2f(dx, dz);
    if (!(dist <= range)) {
        return 0;
    }
    a = a - heading;
    if (!((Angle_Wrap(a) <= 0.0f ? -Angle_Wrap(a) : Angle_Wrap(a)) <= half)) {
        return 0;
    }
    return 1;
}

/* ---- batch 13 ---- */

/* can the pursuer see character `c`? in its view (+0x1580 range, +0x1584 angle, heading
 * +0x1574) and in sight of her middle, or else of one of 9 points around the far side of her
 * body (her radius out, 22.5 degrees apart); sight is blocked by triangle flags 0x40080
 * (0x40088 with progress flag 9 or 0xA) */
/* 0x00218430 */
s32 Npc_SeesChar(Pursuer *p, Character *c) {
    Progress *pr = gProgress;
    u32 ctri = c->a.navTri;
    u32 mask;
    void *nm;
    f32 at[4] __attribute__((aligned(16)));
    f32 me[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 half, range, heading, dist, dx, dz, a;
    s32 in = 0;

    if ((Progress_TestFlag(pr, 9) & 0xFF) == 1 || (Progress_TestFlag(pr, 0xA) & 0xFF) == 1) {
        mask = 0x40088;
    } else {
        mask = 0x40080;
    }
    nm = gNavMesh;
    if (VCALL(nm, 0x10, s32 (*)(void *, u32, f32 *))(nm, ctri, c->a.pos) == 4) {
        VCALL(nm, 0xC, void (*)(void *, u32, f32 *))(nm, ctri, at);
    } else {
        sceVu0CopyVector(at, c->a.pos);
    }
    if (VCALL(nm, 0x10, s32 (*)(void *, u32, f32 *))(nm, p->c.a.navTri, p->c.a.pos) == 4) {
        VCALL(nm, 0xC, void (*)(void *, u32, f32 *))(nm, p->c.a.navTri, me);
    } else {
        sceVu0CopyVector(me, p->c.a.pos);
    }
    half = PU(p, 0x1584, f32);
    range = PU(p, 0x1580, f32);
    heading = PU(p, 0x1574, f32);
    sceVu0SubVector(d, p->c.a.pos, c->a.pos);
    d[3] = 0.0f;
    dist = __builtin_sqrtf(sceVu0InnerProduct(d, d));
    dx = c->a.pos[0] - p->c.a.pos[0];
    dz = c->a.pos[2] - p->c.a.pos[2];
    if (!(dx == 0.0f && dz == 0.0f)) {
        a = msl_atan2f(dx, dz);
        if (dist <= range) {
            a = a - heading;
            if ((Angle_Wrap(a) <= 0.0f ? -Angle_Wrap(a) : Angle_Wrap(a)) <= half) {
                in = 1;
            }
        }
    }
    if (!(in & 0xFF)) {
        return 0;
    }
    if ((Actor_CanWalkBetween(&p->c.a, p->c.a.navTri, ctri, me, at, mask) & 0xFF) == 1) {
        return 1;
    }
    {
        f32 dir[4] __attribute__((aligned(16)));
        f32 off[4] __attribute__((aligned(16)));
        f32 pt[4] __attribute__((aligned(16)));
        u32 i;

        sceVu0SubVector(dir, at, p->c.a.pos);
        sceVu0Normalize(dir, dir);
        Vec_TurnY(off, dir, 0x1.921fb60000000p+0f /* 1.5707964 */);
        sceVu0Normalize(off, off);
        sceVu0ScaleVector(off, off, c->a.radius);
        for (i = 0; i < 9; i = (i + 1) & 0xFF) {
            u32 t;

            sceVu0AddVector(pt, at, off);
            t = Actor_TriTo(&p->c.a, pt, 0);
            if (t != (u32)-1 && Actor_WalkMesh(&p->c.a, ctri, t, at, pt, mask) == -1 &&
                (Actor_CanWalkBetween(&p->c.a, p->c.a.navTri, t, me, pt, mask) & 0xFF) == 1) {
                return 1;
            }
            Vec_TurnY(off, off, 0x1.921fb60000000p-2f /* 0.3926991 */);
        }
    }
    return 0;
}

/* can the pursuer see point `pos` (on triangle `tri`): in its view (+0x1580 range, +0x1584
 * angle, heading +0x1574) and nothing in the way? */
/* 0x002187D0 */
s32 Npc_SeesPoint(Pursuer *p, u32 tri, const f32 *pos) {
    f32 half = PU(p, 0x1584, f32);
    f32 range = PU(p, 0x1580, f32);
    f32 heading = PU(p, 0x1574, f32);
    f32 d[4] __attribute__((aligned(16)));
    f32 dist, dx, dz, a;
    s32 in = 0;

    sceVu0SubVector(d, p->c.a.pos, pos);
    d[3] = 0.0f;
    dist = __builtin_sqrtf(sceVu0InnerProduct(d, d));
    dx = pos[0] - p->c.a.pos[0];
    dz = pos[2] - p->c.a.pos[2];
    if (!(dx == 0.0f && dz == 0.0f)) {
        a = msl_atan2f(dx, dz);
        if (dist <= range) {
            a = a - heading;
            if ((Angle_Wrap(a) <= 0.0f ? -Angle_Wrap(a) : Angle_Wrap(a)) <= half) {
                in = 1;
            }
        }
    }
    if (!(in & 0xFF)) {
        return 0;
    }
    return Actor_CanWalkBetween(&p->c.a, p->c.a.navTri, tri, p->c.a.pos, pos, 0);
}

/* is Hewie close enough to be caught (in reach and on the same walkable triangle)? */
/* 0x00218940 */
s32 NPC_HewieInReach(Pursuer *p) {
    Character *h = gCharPartner;

    if (Npc_SeesChar(p, h) != 0) {
        return 1;
    }
    if (!(Progress_TestFlag(gProgress, 0xB) & 0xFF)) {
        f32 d = PU(p, 0x158C, f32);
        s32 near = 0;

        if (d < 20.0f && !(d <= 0.0f)) {
            near = 1;
        }
        if ((p->c.a.unk2B == 0) & 0xFF & (near & 0xFF)) {
            u32 tri = h->a.navTri;

            if (Actor_TriTo(&p->c.a, h->a.pos, 0x40080) == tri) {
                return 1;
            }
        }
    }
    return 0;
}

/* is Fiona within reach to be caught (seen, or 20 units on a walkable line; 10 with progress
 * flag 0xA; never with flag 9 or while her +0x1AD630 is set)? */
/* 0x00218A30 */
s32 NPC_FionaInReach(Pursuer *p) {
    Character *f = gCharPlayer;
    Progress *pr;
    f32 reach, d;
    s32 near;

    if (AT(f, 0x1AD630, u8) != 0) {
        return 0;
    }
    if (Npc_SeesChar(p, f) != 0) {
        return 1;
    }
    pr = gProgress;
    if (Progress_TestFlag(pr, 9) != 0) {
        return 0;
    }
    reach = Progress_TestFlag(pr, 0xA) != 0 ? 10.0f : 20.0f;
    d = PU(p, 0x1588, f32);
    near = 0;
    if (d < reach && !(d <= 0.0f)) {
        near = 1;
    }
    if ((p->c.a.unk2B == 0) & 0xFF & (near & 0xFF)) {
        u32 tri = f->a.navTri;

        if (Actor_TriTo(&p->c.a, f->a.pos, 0x40080) == tri) {
            return 1;
        }
    }
    return 0;
}

/* drop the first route entry; the new current one, -1 none */
/* 0x00218B60 */
s32 Npc_RouteDrop(Pursuer *p) {
    u8 n = PU(p, 0x1621, u8);
    u8 i;

    if (n < PU(p, 0x1620, u8) || n == 0xFF) {
        return -1;
    }
    {
        u32 k;

        for (k = 0; k < PU(p, 0x1621, u8); k++) {
            PU(p, 0x15E0 + k * 8, s32) = PU(p, 0x15E8 + k * 8, s32);
            PU(p, 0x15E4 + k * 8, u8) = PU(p, 0x15EC + k * 8, u8);
        }
    }
    PU(p, 0x1621, u8)--;
    if (PU(p, 0x1620, u8) > 0) {
        PU(p, 0x1620, u8)--;
    }
    i = PU(p, 0x1620, u8);
    n = PU(p, 0x1621, u8);
    if (n >= i && n != 0xFF) {
        return PU(p, 0x15E0 + i * 8, s32);
    }
    return -1;
}

/* add nav triangle `tri` to the route list (+0x15E0, 8 entries of 8 bytes) */
/* 0x00218C20 */
s32 Npc_RouteAdd(Pursuer *p, u32 tri) {
    u8 n;

    if (PU(p, 0x1621, u8) == 0xFF) {
        PU(p, 0x1621, u8) = 0;
        PU(p, 0x1620, u8) = 0;
    }
    n = PU(p, 0x1621, u8);
    if (n < 8 && tri < AT(gNavMesh, 0x8, u32)) {
        PU(p, 0x15E0 + n * 8, u32) = tri;
        PU(p, 0x1621, u8)++;
        return 1;
    }
    return 0;
}

/* head for the room / side next to Hewie's (exit `exit` of his room); else keep the goal */
/* 0x00218C90 */
void NPC_HeadNearHewie(Pursuer *p, u32 exit) {
    Character *h = gCharPartner;
    VObject *rm = gRooms;
    s32 room = VCALL(rm, 0x18, s32 (*)(VObject *, s32, u32))(rm, h->a.room, exit);
    s32 side = VCALL(rm, 0x50, s32 (*)(VObject *, s32, u32, s32))(rm, room, VCALL(rm, 0x14, u32 (*)(VObject *, s32, u32))(rm, h->a.room, exit) & 0xFF, 1);

    if (Character_Route(&p->c, room, side, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = room;
        PU(p, 0x1598, s32) = side;
    } else {
        Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
}

/* the same next to Fiona's room */
/* 0x00218D80 */
void NPC_HeadNearFiona(Pursuer *p, u32 exit) {
    Character *f = gCharPlayer;
    VObject *rm = gRooms;
    s32 room = VCALL(rm, 0x18, s32 (*)(VObject *, s32, u32))(rm, f->a.room, exit);
    s32 side = VCALL(rm, 0x50, s32 (*)(VObject *, s32, u32, s32))(rm, room, VCALL(rm, 0x14, u32 (*)(VObject *, s32, u32))(rm, f->a.room, exit) & 0xFF, 1);

    if (Character_Route(&p->c, room, side, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = room;
        PU(p, 0x1598, s32) = side;
    } else {
        Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
}

/* aim at the current entry of the route list: triangle +0x15A4, its centre to +0x15B0 */
/* 0x00218E70 */
void Npc_RouteAim(Pursuer *p) {
    u8 i = PU(p, 0x1620, u8);

    if (i < PU(p, 0x1621, u8)) {
        PU(p, 0x15A4, s32) = PU(p, 0x15E0 + i * 8, s32);
        VCALL(gNavMesh, 0xC, void (*)(void *, s32, f32 *, Pursuer *))(gNavMesh, PU(p, 0x15E0 + PU(p, 0x1620, u8) * 8, s32), (f32 *)((u8 *)p + 0x15B0), p);
    }
}

/* vtable +0xB4: head for character `c` (null: the target) */
/* 0x00218ED0 */
void NPC_HeadFor(Pursuer *p, Character *c) {
    s32 side = PU(p, 0x1598, s32);
    s32 other = 0;

    if (c == NULL) {
        c = p->target;
    }
    if (p->c.a.room == c->a.room && p->c.door < 8 && (c->door & 0xFF) < 8) {
        VObject *rm = gRooms;
        s32 a = VCALL(rm, 0x50, s32 (*)(VObject *, s32, u32, s32))(rm, p->c.a.room, p->c.door, 1);

        if (a != -1) {
            s32 b = -1;

            if (c != NULL) {
                b = VCALL(rm, 0x50, s32 (*)(VObject *, s32, u32, s32))(rm, c->a.room, c->door, 1);
            }
            if (b != -1 && b != a) {
                other = 1;
            }
        }
    }
    if (other != 0) {
        side = c != NULL ? VCALL(gRooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(gRooms, c->a.room, c->door, 1) : -1;
    } else {
        s32 room = c->a.room;

        if (p->c.a.room == room || PU(p, 0x1594, s32) != room) {
            side = -1;
        }
        if (room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
            if (c->moveMode == 0) {
                sceVu0CopyVector((f32 *)((u8 *)p + 0x15B0), c->a.pos);
            } else {
                VCALL(gNavMesh, 0xC, void (*)(void *, u32, f32 *))(gNavMesh, c->a.navTri, (f32 *)((u8 *)p + 0x15B0));
            }
            PU(p, 0x15A4, s32) = c->a.navTri;
        }
    }
    if (Character_Route(&p->c, c->a.room, side, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = c->a.room;
        PU(p, 0x1598, s32) = side;
    } else {
        Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
}

/* vtable +0xB0: head for Fiona (her room's side; her triangle if she's in a room the pursuer
 * can reach) */
/* 0x00219100 */
void NPC_HeadForFiona(Pursuer *p) {
    Character *f = gCharPlayer;
    s32 side = PU(p, 0x1598, s32);
    s32 other = 0;

    if (p->c.a.room == f->a.room && p->c.door < 8 && (f->door & 0xFF) < 8) {
        VObject *rm = gRooms;
        s32 a = VCALL(rm, 0x50, s32 (*)(VObject *, s32, u32, s32))(rm, p->c.a.room, p->c.door, 1);

        if (a != -1) {
            s32 b = -1;

            if (f != NULL) {
                b = VCALL(rm, 0x50, s32 (*)(VObject *, s32, u32, s32))(rm, f->a.room, f->door, 1);
            }
            if (b != -1 && b != a) {
                other = 1;
            }
        }
    }
    if (other != 0) {
        side = f != NULL ? VCALL(gRooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(gRooms, f->a.room, f->door, 1) : -1;
    } else {
        if (p->c.a.room == f->a.room || PU(p, 0x1594, s32) != f->a.room) {
            side = -1;
        }
        if (f->moveMode == 0) {
            sceVu0CopyVector((f32 *)((u8 *)p + 0x15B0), f->a.pos);
        } else {
            VCALL(gNavMesh, 0xC, void (*)(void *, u32, f32 *))(gNavMesh, f->a.navTri, (f32 *)((u8 *)p + 0x15B0));
        }
        PU(p, 0x15A4, s32) = f->a.navTri;
    }
    if (Character_Route(&p->c, f->a.room, side, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = f->a.room;
        PU(p, 0x1598, s32) = side;
    } else {
        Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
}

/* vtable +0xAC: go to triangle `tri` / point `pos` of room `room` (-1 the played one) */
/* 0x00219310 */
void NPC_GoTo(Pursuer *p, u32 tri, const f32 *pos, s32 room) {
    void *nm;

    if (room == -1) {
        room = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    }
    if (room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress)) {
        PU(p, 0x15A4, u32) = tri;
    }
    nm = gNavMesh;
    if (VCALL(nm, 0x10, s32 (*)(void *, u32, const f32 *))(nm, tri, pos) == 3) {
        sceVu0CopyVector((f32 *)((u8 *)p + 0x15B0), pos);
    } else {
        VCALL(nm, 0xC, void (*)(void *, u32, f32 *))(nm, tri, (f32 *)((u8 *)p + 0x15B0));
    }
    if (Character_Route(&p->c, room, -1, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = room;
        PU(p, 0x1598, s32) = -1;
    } else {
        Character_Route(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
}

/* vtable +0xA8: blocking nav triangle flags */
/* 0x00219450 */
u32 NPC_BlockFlags(Pursuer *p) {
    return 0x2C020028;
}

/* vtable +0xC: NPC init */
/* 0x00219460 */
void NPC_Reset(Pursuer *p) {
    u32 i;

    p->c.a.unkC4 = 0;
    Character_Reset(&p->c);
    p->c.a.navMask = VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
    for (i = 0; i < 13; i++) {
        p->c.unk148C[i] = 0;
    }
    p->c.door = 0xFF;
    p->c.unk14C0 = 0xFFFF;
    p->c.unk14C4 = 0;
    p->c.pathReq->unk4 = 6;
    p->c.pathReq->mask = p->c.a.navMask;
    PU(p, 0x15A4, s32) = -1;
    PU(p, 0x15C4, s32) = -1;
    PU(p, 0x1594, s32) = -1;
    PU(p, 0x1598, s32) = -1;
    PU(p, 0x159C, s32) = -1;
    PU(p, 0x15C0, u8) = 0xFF;
    sceVu0CopyVector((f32 *)((u8 *)p + 0x1570), p->c.a.angle);
    PU(p, 0x1568, s32) = 0;
    PU(p, 0x1544, u8) = 0;
    PU(p, 0x1545, u8) = 0;
    PU(p, 0x1546, u8) = 0;
}

/* 0x001735B0 */
void *Debilitas2_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x6, arg, Debilitas2_vtable);
}

/* 0x00173600 */
void *Debilitas_ctor(void *p, s32 arg) {
    return b0_RoomCtor(p, 0x2, arg, Debilitas_vtable);
}

/* ---- methods of stalker subclasses D_004712xx / D_004714xx / D_004720xx (room creatures) ---- */
