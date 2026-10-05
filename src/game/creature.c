/* The creatures (character slots 7..9, 0x1600 bytes; their manager is D_0044F258): Character
 * with their own block at +0x1540. Two classes: D_0046FAA0 (here) and D_00474080, both on the
 * creature base D_0046FB50. Their state is kept across rooms in Progress +0x878 (36 bytes per
 * slot, func_002DE840). */
#include "common.h"
#include "game.h"
#include "actor.h"
#include "progress.h"
#include "sce/libvu0.h"

extern void *D_0046FAA0[], *D_0046FB50[], *D_00469C60[], *D_00469C20[];
extern void func_00124DA0(Actor *a);
extern void func_00127660(Character *c);
extern s32 func_00125AD0(Character *c, u32 tri, const f32 *heading, f32 *pos);
extern void func_00125CC0(Character *c);

#define CR(c) ((u8 *)(c) + 0x1540)   /* the creature's own block */

/* ---- the creature base (D_0046FB50): defaults ---- */

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
    func_00124DA0(&c->a);
}

/* +0xC */
void func_002E2310(Character *c) {
    func_00127660(c);
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

/* +0x28 put on triangle `tri` (func_00125AD0), remembering it as the previous one and the
   position (+0x38 / +0x40) */
s32 func_002E19A0(Character *c, u32 tri, const f32 *heading, f32 *pos) {
    s32 r = func_00125AD0(c, tri, heading, pos);

    c->a.prevNavTri = tri;
    sceVu0CopyVector(c->a.prevPos, c->a.pos);
    return r;
}

/* +0x5C reset (func_00125CC0): not hit (+0xB), no target (+0x9 0xFF), mode 2 (+0xC) */
void func_002E0BF0(Character *c) {
    u8 *k = CR(c);

    func_00125CC0(c);
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

extern Character *gCharacters[];
extern Character *gCharPlayer;
extern VObject *D_0044E550;   /* random numbers: +0x10 an integer */
extern VObject *D_0044E568;   /* the rooms */
extern s32 func_00126F80(Character *c, s32 target, s32 unused2, s32 side, s32 unused4);

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
    AT(k, 0x80, s16) = ((VCALL(D_0044E550, 0x10, s32 (*)(VObject *))(D_0044E550) & 3) + 1) * 1800;
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
 * up in her room; out, with a rest time; a path to her room (func_00126F80) and its length
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
    if (func_00126F80(c, gCharPlayer->a.room, -1, AT(k, 0xC, s32), -1) == -1) {
        return;
    }
    for (n = 0; n < c->unk1384; n++) {
        VObject *rooms = D_0044E568;
        u16 door = AT(c->unk138C, n * 2, u16);

        AT(&c->unk14C4, 0, f32) += (f32)VCALL(rooms, 0x38, s32 (*)(VObject *, u32, s32))(rooms, door, a1);
        c->unk14C0 = AT(c->unk138C, n * 2, u16);
    }
}

#include "navmesh.h"

extern VObject *gSceneGameF29740;   /* the path planner */
extern s32 func_00127200(Character *c, s32 kind, u32 goalTri, const f32 *goal, s32 opt);
extern void func_00127060(Character *c);
extern s32 func_001270A0(Character *c);
extern s32 func_001270F0(Character *c);

/* go through exit `exit` into the next room (off screen): its room, side (+0xC) and door
 * (+0x9); off the mesh, moving through the door (+0xFC 0x17), out of play; +0x8A.. cleared.
 * -1: no such exit */
s32 func_002DF760(Character *c, s32 exit) {
    u8 *k = CR(c);
    VObject *rooms;
    u32 d;
    s32 i;

    d = VCALL(D_0044E568, 0x14, u32 (*)(VObject *, s32, s32))(D_0044E568, c->a.room, exit) & 0xFF;
    if (d == 0xFF) {
        return -1;
    }
    rooms = D_0044E568;
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
s32 func_002DF860(Character *c, u32 tri, const f32 *goal, s32 direct) {
    u8 *k = CR(c);
    s32 r, i;

    if (NavMesh_AcrossDivider(D_0044E570, tri, c->a.navTri)) {
        return -1;
    }
    if (AT(k, 0x2B, u8) == 0) {
        AT(k, 0x2B, u8) += 1;
        if (func_00127200(c, 0, tri, goal, -1) == -1) {
            AT(k, 0x2B, u8) = 0;
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
        r = direct ? func_001270A0(c) : func_001270F0(c);
        func_00127060(c);
        AT(k, 0x2B, u8) = 0;
    }
    if (r >= 0) {
        return -(r == 0);
    }
    func_00127060(c);
    AT(k, 0x2B, u8) = 0;
    return -1;
}

extern u32 func_00124480(Actor *a, const f32 *p, u32 mask);
extern s32 func_001273D0(Character *c, u32 *triOut, f32 *posOut, f32 step);
extern void func_0010E640(f32 *out, const f32 *v, f32 s);   /* libvu0: scale x, y, z */
extern void func_001247E0(Actor *a, const f32 *delta);

/* on the way to `tri`: while not there (its target +0xB0 not on it) the path ahead is looked
 * at (12 steps and 1; unused); a door on the way (+0x88 bit 0) with an exit +0x100 is gone
 * through */
void func_002DF470(Character *c, u32 tri) {
    u8 *k = CR(c);

    if (tri != func_00124480(&c->a, c->a.unkB0, NAV_NONE)) {
        u32 t;
        f32 far[4] __attribute__((aligned(16)));
        f32 near[4] __attribute__((aligned(16)));
        f32 dir[4] __attribute__((aligned(16)));
        f32 fwd[4] __attribute__((aligned(16)));

        func_001273D0(c, &t, far, 12.0f * AT(k, 0x0, f32));
        func_001273D0(c, &t, near, AT(k, 0x0, f32));
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
    if ((VCALL(D_0044E568, 0x10, u32 (*)(VObject *, s32, u32))(D_0044E568, c->a.room, (u8)c->unk100) & 0xFFFF) != 0xFFFF) {
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
        func_001247E0(&c->a, dir);
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
    t = func_00124480(&c->a, gCharPlayer->a.pos, c->pathReq->mask);
    if (AT(k, 0x8, u8) != 0) {
        if (t != gCharPlayer->a.navTri) {
            return -1;
        }
        func_00127060(c);
        AT(k, 0x2B, u8) = 0;
        c->unk124 = c->unk128;
        return creature_close_in(c, t);
    }
    if (t == NAV_NONE) {
        return -1;
    }
    func_00127060(c);
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

extern VObject *D_0044E4D0;   /* the events: +0x10 (pos, spot, -1) a position at event spot */
extern VObject *D_0044E558;   /* the doors */

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
    rnd = D_0044E550;
    nm = D_0044E570;
    rooms = D_0044E568;
    n = nm->numTris;
    ev_mgr = D_0044E4D0;
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

/* the first door of the room (doors +0x40) it may use (+0x15CA[door] bit of its slot), that
 * isn't locked (+0x30 (1, 0)): headed for (+0x85 the door, +0x88 1), its path planned to the
 * door's spot; if it was ahead of the plan it snaps onto the next step (+0x84 1) */
void func_002DF5B0(Character *c) {
    u8 *k = CR(c);
    VObject *doors = D_0044E558, *rooms = D_0044E568;
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
                s32 next = func_001273D0(c, &t, p, AT(k, 0x0, f32));

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

        come = func_00124480(&c->a, gCharPlayer->a.pos, NAV_NONE) == t && AT(k, 0x24, f32) <= (f32)AT(k, 0x4, s16);
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
    AT(k, 0x33, u8) = VCALL(D_0044E550, 0x14, s32 (*)(VObject *))(D_0044E550) & 0x1F;
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

extern s32 func_00125D80(Character *c);
extern s32 func_001241F0(Actor *a, Actor *b, f32 x, f32 y);
extern void func_00177FA0(Progress *p, const f32 *pos, u32 which, u8 kind, s16 a, s16 b, f32 f);
extern void func_00122C20(Actor *a, s32 id, s32 arg2, s32 arg3, s32 arg4, const f32 *pos);
extern void *D_00472370[];
extern const PTMF D_00416790;   /* vanishing */

/* its vanishing effect (0x4A0 bytes, vtable D_00472370; its quad drawer at +0x370) */
static void vanish_init(void **o) {
    o[0] = D_00472370;
    o[0x370 / 4] = D_00469D00;
    ((s32 *)o)[0x374 / 4] = -1;
    o[0x370 / 4] = D_0046FC30;
}

/* seen: once it has come (+0x2E) and Fiona isn't hidden (+0x2D) or busy (func_00125D80) and
 * sees it (func_001241F0), a noise (kind 0xD) is made, it vanishes in a glow (12 above it, red
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
        (func_00125D80(gCharPlayer) & 0xFF) || (func_001241F0(&c->a, &gCharPlayer->a, 0.0f, 0.0f) & 0xFF) != 1) {
        return;
    }
    func_00177FA0(gProgress, c->a.pos, 1, 0xD, 0, 0, 0.0f);
    mgr = D_0044E578;
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
    if (gCharPlayer->unkE2 == 0 && c->a.disabled == 0 && VCALL(D_0044E4D0, 0x50, s32 (*)(VObject *))(D_0044E4D0) == 0) {
        func_00122C20(&c->a, 0x8B, 5, 0, 0, NULL);
    }
    AT(k, 0x37, u8) = 1;
    AT(k, 0x29, u8) = 1;
    AT(k, 0x18, s32) = 0;
    AT(k, 0x1C, s32) = 0;
    ptmf_set(&c->a.state, &D_00416790);
}

extern s32 func_00178980(Progress *p, s32 room, s32 exit);   /* u8: the way through is open */
extern s32 Progress_CurRoomFlag(Progress *p, s32 room, u32 exit);
extern s32 func_001272B0(Character *c, f32 speed);
extern f32 func_0031C5C0(f32 x, f32 z);   /* heading of (x, z) */

/* in the room being played: note the doors (by exit, +0x8A) whose event spot it stands on */
static void creature_at_doors(Character *c) {
    u8 *k = CR(c);
    VObject *rooms = D_0044E568, *ev_mgr;
    s32 cur = VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
    u32 i;

    ev_mgr = D_0044E4D0;
    for (i = 0; i < 8; i++) {
        u32 door;

        AT(k, 0x8A + i * 2, u16) = 0;
        door = VCALL(rooms, 0x48, u32 (*)(VObject *, s32, u32))(rooms, cur, i & 0xFF) & 0xFFFF;
        if (cur == c->a.room && door != 0xFFFF && c->a.disabled == 0 &&
            VCALL(ev_mgr, 0x10, s32 (*)(VObject *, f32 *, u32, s32))(ev_mgr, c->a.pos, door, -1) != 0) {
            AT(k, 0x8A + i * 2, u16) |= (1 << *(s32 *)&c->a.slot) & 0xFFFF;
        }
    }
}

/* travelling (unless in an event, flag 0x18): off screen the distance to the next door
 * (+0x14C4) runs down by its speed (2/3 of it with company, +0x2A); at a door (+0xB) it walks
 * the path at half speed. On arriving at the door (+0x14C0): if the way through is open
 * (func_00178980) and the door isn't shut (Progress_CurRoomFlag), through it - into the room
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
            func_001272B0(c, AT(k, 0x0, f32) / 2.0f);
            if (!(c->unk128 < c->unk124)) {
                arrived = 1;
            }
        }
    }
    if (arrived != 1) {
        return;
    }
    rooms = D_0044E568;
    exit = VCALL(rooms, 0x3C, u32 (*)(VObject *, u32, s32))(rooms, c->unk14C0, c->a.room) & 0xFF;
    if (!(func_00178980(p, c->a.room, exit) & 0xFF)) {
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

        tri = VCALL(D_0044E568, 0x30, u32 (*)(VObject *, u32, f32 *))(D_0044E568, AT(k, 0x9, u8), at);
        VCALL(D_0044E568, 0x34, u32 (*)(VObject *, u32, f32 *))(D_0044E568, AT(k, 0x9, u8), in);
        sceVu0SubVector(d, in, at);
        yaw = func_0031C5C0(d[0], d[2]);
        VCALL(c, 0x28, void (*)(Character *, u32, f32 *, f32 *))(c, tri, &yaw, at);
        creature_at_doors(c);
    } else {
        c->a.navTri = NAV_NONE;
        AT(&c->unk14C4, 0, f32) = (f32)VCALL(rooms, 0x38, s32 (*)(VObject *, u32, s32))(rooms, c->unk14C0, c->a.room);
        c->unk1388 = c->unk1384;
    }
}

extern void func_002A8440(void *list, s32 kind, s32 room, u32 tri, s32 a4);

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
        func_00127060(c);
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
        func_00177FA0(gProgress, c->a.pos, 1, 3, 0, 0, 0.0f);
    } else {
        func_002A8440((u8 *)gProgress + 0x7A8, 0x80, c->a.room, c->a.navTri, 0xFFFF);
    }
    c->a.active = 0;
    func_00127060(c);
}

extern s32 func_00127140(Character *c, s32 kind, u32 goalTri, const f32 *goal);

/* state: to the door it chose (+0x85): its spot asked of the planner; on the way there
 * (func_002DF470) with the door as its exit (+0x100) and its spot as the target (+0xB0) */
void func_002E01C0(Character *c) {
    u8 *k = CR(c);
    VObject *rooms = D_0044E568;
    f32 at[4] __attribute__((aligned(16)));
    f32 spot[4] __attribute__((aligned(16)));
    u32 tri;

    tri = VCALL(rooms, 0x30, u32 (*)(VObject *, u32, f32 *))(rooms, AT(c, 0x15C5, u8), at);
    if (func_00127140(c, 0, tri, at) > 0) {
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
 * room being played (func_00126F80): its first door (+0x14C0) and that leg's distance
 * (+0x14C4); none: the closed ways it noted (+0x148C) are forgotten */
void func_002E02B0(Character *c) {
    u8 *k = CR(c);
    s32 i;

    if (c->unk1388 < c->unk1384 || AT(k, 0xB, u8) != 0) {
        return;
    }
    if (func_00126F80(c, VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress), -1, AT(k, 0xC, s32), -1) <= 0) {
        for (i = 0; i < 13; i++) {
            c->unk148C[i] = 0;
        }
        return;
    }
    c->unk14C0 = AT(c->unk138C, 0, u16);
    AT(&c->unk14C4, 0, f32) = (f32)VCALL(D_0044E568, 0x38, s32 (*)(VObject *, u32, s32))(D_0044E568, AT(c->unk138C, 0, u16), c->a.room);
}

extern s32 func_001274E0(Character *c, f32 speed);

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
        if (t == -1 && (NavMesh_TriFlags(D_0044E570, c->a.navTri) & 0x20000)) {
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
    func_001274E0(c, AT(k, 0x0, f32));
}

extern s32 func_00125BA0(Character *c, s32 room, s32 a2, s32 a3);

/* +0x64 put in room `room` on triangle `tri`, mode `mode` (+0xC): (the Character's +0x64,
 * func_00125BA0) in the room being played placed there (+0x28; its result) and its doors
 * noted, else just its triangle; 0 */
s32 func_002E0C30(Character *c, s32 room, u32 tri, s32 mode) {
    Progress *p;
    s32 r = 0;

    func_00125BA0(c, room, tri, mode);
    AT(CR(c), 0xC, s32) = mode;
    p = gProgress;
    if (room != VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        c->a.navTri = tri;
        return r;
    }
    r = VCALL(c, 0x28, s32 (*)(Character *, u32, f32 *, f32 *))(c, tri, NULL, NULL);
    creature_at_doors(c);
    return r;
}

/* back on its triangle in the room being played (or, on one it may not stand on, a random place
   on its level), its doors noted */
static void creature_back_on_mesh(Character *c) {
    u32 tri = c->a.navTri;

    if (NavMesh_TriFlags(D_0044E570, tri) & c->a.navMask) {
        func_002DE540(c, AT(CR(c), 0xC, s32));
    } else {
        VCALL(D_0044E570, 0x14, void (*)(NavMesh *, u32, f32 *))(D_0044E570, tri, c->a.pos);
        VCALL(c, 0x28, s32 (*)(Character *, u32, f32 *, f32 *))(c, c->a.navTri, &c->a.angle[1], c->a.pos);
    }
    creature_at_doors(c);
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

/* +0xC set up: the Character's (func_00127660); size 1.5 x 16, company 1, no blocking flags,
 * path kind 6 blocking 0x40080; no model; a random kind 0..15 (its table entry), strength 0;
 * all its own state cleared (bobbing 0.5 / 0.05, fade 0x80, turns every 20, gives up after
 * 60 or with +0x8 30 tries), then +0x5C */
void func_002E2030(Character *c) {
    u8 *k = CR(c);
    const CreatureKind *t;
    s32 i;

    func_00127660(c);
    c->a.radius = 1.5f;
    c->a.height = 16.0f;
    c->a.unk2A = 1;
    c->a.navMask = 0;
    c->pathReq->unk4 = 6;
    c->pathReq->mask = 0x40080;
    c->motion = NULL;
    AT(k, 0x31, u8) = VCALL(D_0044E550, 0x10, s32 (*)(VObject *))(D_0044E550) & 0xF;
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

extern s32 func_00123C60(Actor *a, s32 room, const f32 *pos);

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
 * hers and she isn't just beyond it (func_00123C60), skips its path to the door if already
 * nearer the door than the exit's spot; if she is, it stays out (+0x10 0) */
void func_002E0FE0(Character *c, u32 exit) {
    u8 *k = CR(c);
    VObject *rooms = D_0044E568;
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
    if ((func_00123C60(&c->a, exit, gCharPlayer->a.pos) & 0xFF) == 1) {
        VCALL(rooms, 0x14, s32 (*)(VObject *, s32, u32))(rooms, c->a.room, exit);
        AT(k, 0x10, s32) = 0;
        return;
    }
    if (AT(k, 0xB, u8) == 1) {
        f32 door[4] __attribute__((aligned(16)));
        f32 d0[4] __attribute__((aligned(16)));
        f32 d1[4] __attribute__((aligned(16)));

        rooms = D_0044E568;
        c->unk14C0 = VCALL(rooms, 0x10, s32 (*)(VObject *, s32, u32))(rooms, c->a.room, exit);
        VCALL(rooms, 0x30, u32 (*)(VObject *, u32, f32 *))(rooms, exit, door);
        sceVu0SubVector(d0, c->a.pos, door);
        sceVu0SubVector(d1, at, door);
        if (sceVu0InnerProduct(d0, d0) <= sceVu0InnerProduct(d1, d1) && func_00124480(&c->a, door, NAV_NONE) != NAV_NONE) {
            c->unk124 = c->unk128;
        }
    }
}

extern u32 func_00123D20(Actor *a, const f32 *p);
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
        func_00127060(c);
        AT(k, 0x2B, u8) = 0;
        c->unk124 = c->unk128;
    }
    if (AT(k, 0x9B, s8) == 1) {
        AT(k, 0x9B, s8) += 1;
        if (c->unk128 >= c->unk124) {
            VObject *rnd = D_0044E550;
            NavMesh *nm = D_0044E570;
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
                AT(k, 0x44, u32) = func_00123D20(&gCharPlayer->a, goal);
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
    if (func_001274E0(c, AT(k, 0x0, f32)) == 0) {
        AT(k, 0x9B, s8) = 1;
    }
}

extern s32 Progress_TestFlag(Progress *p, u32 id);
extern const PTMF D_00416700, D_00416710, D_00416720, D_00416730, D_00416740, D_00416750, D_00416760,
    D_00416770, D_00416780;   /* its states: about Fiona, idle, after her, idle, at the door, to the
                                 door, idle, travelling, idle */

/* +0x30's thinking: in the room being played (in play) - not come yet: idle (on its triangle's
 * centre the first time); come: its time runs (+0x20), on her level it hasn't given up (+0x2F);
 * when she can't be reached (flag 9, her mode 3) about her (+0xF8 2) unless it did that
 * (+0x9C); she hidden (+0x2D, +0xE0): idle; else for a door (func_002DF5B0) - a closed way
 * (func_00178980) ends that (+0x84 2), an opened one when it should open it (+0x88) does
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
            VCALL(D_0044E570, 0xC, void (*)(NavMesh *, u32, f32 *))(D_0044E570, c->a.navTri, c->a.pos);
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
        if (!(func_00178980(p, c->a.room, AT(k, 0x85, u8)) & 0xFF)) {
            AT(k, 0x84, u8) = 2;
            c->a.unk2B = 0;
        } else if ((VCALL(D_0044E558, 0x30, u32 (*)(VObject *, u32))(D_0044E558, AT(k, 0x85, u8)) & 0xFF) == 1 &&
                   (AT(k, 0x88, u16) & 1)) {
            AT(k, 0x84, u8) = 0;
        }
    }
    switch (AT(k, 0x84, u8)) {
    case 0:
        if (AT(k, 0x9C, s8) == 0 || AT(k, 0x24, f32) <= 40.0f) {
            ptmf_set(&c->a.state, &D_00416720);
        } else {
            func_00127060(c);
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

extern void func_0010E5F0(f32 *dst, const f32 *src);   /* libvu0: copy x, y, z */
extern VObject *D_0044E988;                            /* the items */
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
    creature_at_doors(c);
    AT(k, 0x9C, s8) = VCALL(D_0044E988, 0x10, s32 (*)(VObject *, s32))(D_0044E988, 3) == 0x8D;
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
            func_00122C20(&c->a, 0x8C, 5, 0, 0, NULL);
            ptmf_set(&c->a.state, &D_004166B0);
        }
    } else {
        u8 travel = 1;

        if (AT(k, 0x20, s16) >= AT(k, 0x6, s16)) {
            c->a.active = 0;
            func_00127060(c);
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

extern void func_002E56C0(void *drawer);   /* hand a quad drawer to the renderer */

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
        func_002E56C0(&q);
        q.vtbl = D_00469D00;
    }
}

/* ---- the creature manager (D_0044F258; vtable D_0046FC00 at +0x28): a list of 10 at +0x0,
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

extern VObject *gBootMessage;

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
extern void func_002D63B0(void *o);   /* free (the effects' heap) */
extern u32 func_002D6010(u8 *mgr);   /* the effects paused */
/* soft-float doubles as raw bit patterns */
extern u64 func_0011ED78(f32 x);             /* (double)x */
extern u64 func_0011F208(u64 a, u64 b);      /* a * b */
extern u64 func_0011F148(u64 a, u64 b);      /* a + b */
extern u64 func_0011F458(u64 a, u64 b);      /* a / b */
extern f32 func_0011F878(u64 a);             /* (float)a */

#define VANISH_GLOW(o) ((o) + AT(o, 0x3A8, s32) * 0x30 + 0x10)
#define VANISH_SPARK(o, i) ((o) + AT(o, 0x3A8, s32) * 0x180 + (i) * 0x30 + 0x70)

/* +0x8 destructor (the quad drawer at +0x370 inlined) */
u8 *func_00312040(u8 *o, s32 flags) {
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
void func_003128E0(u8 *o) {
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
    rng = D_0044E550;
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
void func_003120D0(u8 *o, const u8 *params) {
    VObject *rng;
    u8 *g = VANISH_GLOW(o);
    s32 i;

    AT(g, 0x0, s32) = params[0x10];
    AT(g, 0x4, s32) = params[0x11];
    AT(g, 0x8, s32) = params[0x12];
    AT(g, 0xC, s32) = params[0x13];
    sceVu0CopyVector((f32 *)(g + 0x10), (f32 *)params);
    rng = D_0044E550;
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
void func_00312450(u8 *o) {
    if (func_002D6010(D_0044E578) != 0) {
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
s32 func_00312510(u8 *o) {
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
    rng = D_0044E550;
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

extern u8 *D_0044F258;   /* the creatures: 10 slots */

/* room 0x4F (D_0040C130): the first of the creatures 0..6 within 4 of (-35.7, -7.5) vanishes
 * there (taken off, its glow - blue for kinds below 0x12, else red - and the sound 0x8B): 1;
 * none, 0 */
s32 func_002B4030(void) {
    static const union { u32 u; f32 f; } kX = {0xC20ECCCD};
    s32 i;

    for (i = 0; i < 7; i++) {
        Character *c = AT(D_0044F258, i * 4, Character *);
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
        mgr = D_0044E578;
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
        func_00122C20(&c->a, 0x8B, 5, 0, 0, NULL);
        return 1;
    }
    return 0;
}
