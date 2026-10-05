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
