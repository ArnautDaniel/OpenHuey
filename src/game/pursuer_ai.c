/* Pursuer methods and helpers, code 0x211C80..0x219460, and the stalker vtables' defaults
 * (0x179600..0x179970). See include/pursuer.h. */
#include "common.h"
#include "pursuer.h"
#include "progress.h"
#include "sce/libvu0.h"
#include "globals.h"


/* ---- defaults shared by the stalker vtables (0x179600..0x179970) ---- */


void func_00179600(Pursuer *p) {
    func_0029FB20(p);
}

/* vtable +0x11C */
s32 func_00179610(Pursuer *p) {
    return 0;
}

/* vtable +0x2AC .. +0x2B8: nothing */
void func_00179710(Pursuer *p) {
}

void func_00179720(Pursuer *p) {
}

void func_00179730(Pursuer *p) {
}

void func_00179740(Pursuer *p) {
}

/* vtable +0x2C0: timer +0x1660 to 600 frames (10 s) */
void func_00179750(Pursuer *p) {
    PU(p, 0x1660, s32) = 600;
}

/* vtable +0x2C8: timer +0x1660 to `frames`, 0 = 900 (15 s) */
void func_00179760(Pursuer *p, s32 frames) {
    PU(p, 0x1660, s32) = frames != 0 ? frames : 900;
}

/* vtable +0xA0 / +0xA4: turn rates, 4 and 8 degrees (in radians) */
f32 func_00179830(Pursuer *p) {
    return 0x1.1df46a0000000p-4f /* 0.06981317 */;
}

f32 func_00179850(Pursuer *p) {
    return 0x1.1df46a0000000p-3f /* 0.13962634 */;
}

/* vtable +0x2DC .. +0x2FC: distances and factors */
f32 func_00179870(Pursuer *p) {
    return 60.0f;
}

f32 func_00179880(Pursuer *p) {
    return 0x1.eb851e0000000p-4f /* 0.12 */;
}

f32 func_001798A0(Pursuer *p) {
    return 20.0f;
}

f32 func_001798B0(Pursuer *p) {
    return 20.0f;
}

f32 func_001798C0(Pursuer *p) {
    return 20.0f;
}

f32 func_001798D0(Pursuer *p) {
    return 16.0f;
}

f32 func_001798E0(Pursuer *p) {
    return 24.0f;
}

f32 func_001798F0(Pursuer *p) {
    return 0x1.6666660000000p+0f /* 1.4 */;
}

f32 func_00179910(Pursuer *p) {
    return 0x1.3333340000000p-1f /* 0.6 */;
}

s32 func_00179930(Pursuer *p) {
    return -1;
}

s32 func_00179940(Pursuer *p) {
    return -1;
}

/* vtable +0x314 */
s32 func_00179950(Pursuer *p) {
    return 0;
}


/* vtable +0xE8 */
void func_00179960(Pursuer *p) {
    func_00212400(p);
}

/* ---- 0x211C80..0x219460 ---- */

/* vtable +0xE4: nothing */
void func_00212540(Pursuer *p) {
}

/* shut / open door `door` (doors vtable +0x1C) */
s32 func_00212DC0(Pursuer *p, s32 door) {
    return VCALL(gDoors, 0x1C, s32 (*)(VObject *, s32, s32, s32))(gDoors, door, 0, 0x60000);
}

s32 func_00212DE0(Pursuer *p, s32 door) {
    return VCALL(gDoors, 0x1C, s32 (*)(VObject *, s32, s32, s32))(gDoors, door, 1, 0x60000);
}

/* the room's side behind the exit the pursuer heads for (rooms vtable +0x50) */
s32 func_00217340(Pursuer *p) {
    return VCALL(gRooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(gRooms, p->c.a.room, p->c.door, 1);
}

/* vtable +0xC8: reacting to a noise */
s32 func_002181C0(Pursuer *p) {
    return p->c.heardSlot != 0xFF;
}

/* vtable +0xA8: blocking nav triangle flags */
u32 func_00219450(Pursuer *p) {
    return 0x2C020028;
}

/* ---- batch 2 ---- */

extern Progress *gProgress;
extern Character *gCharPlayer;    /* Fiona */
extern void *gNavMesh;          /* nav mesh */
extern VObject *gSceneGameF29740; /* path planner */


/* `tri` if the pursuer may stand on it (its blocking flags, vtable +0xA8, against the
   triangle's +0x3C), else the nearest triangle it may (a planner query of kind 7; -1 if none) */
u32 func_00211B00(Pursuer *p, u32 tri) {
    VObject *nav = gNavMesh;
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
void func_00211C80(Pursuer *p, s32 a2) {
    Progress *pr = gProgress;

    func_00178DB0(pr, p->c.a.room, a2, *(u8 *)&p->c.a.slot);
    func_00178C10(pr, p->c.a.room, a2, *(u8 *)&p->c.a.slot);
}

/* door `door` shut, the other side ... (doors +0x20 / +0x1C, progress) */
void func_00212CA0(Pursuer *p, u32 door) {
    VObject *d = gDoors;
    Progress *pr;

    VCALL(d, 0x20, void (*)(VObject *, u32, s32, s32))(d, door, 1, 0x60000);
    VCALL(d, 0x1C, void (*)(VObject *, u32, s32, s32))(d, door, 0, 0x60000);
    pr = gProgress;
    func_00178A90(pr, VCALL(pr, 0xC, s32 (*)(Progress *))(pr), door, 0xFF);
}

void func_00212D30(Pursuer *p, u32 door) {
    VObject *d = gDoors;
    Progress *pr;

    VCALL(d, 0x20, void (*)(VObject *, u32, s32, s32))(d, door, 0, 0x60000);
    VCALL(d, 0x1C, void (*)(VObject *, u32, s32, s32))(d, door, 1, 0x60000);
    pr = gProgress;
    func_00178C10(pr, VCALL(pr, 0xC, s32 (*)(Progress *))(pr), door, 0xFF);
}

/* plan a path to the point beside door `door` (vtable +0x9C offset); 1 if +0xDC agrees */
s32 func_00212F40(Pursuer *p, u32 door, u32 side) {
    f32 ofs[4] __attribute__((aligned(16)));

    VCALL(p, 0x9C, void (*)(Pursuer *, u32, f32 *))(p, side & 0xFF, ofs);
    PU(p, 0x15C4, s32) = func_00123710(p, door & 0xFF, side & 0xFF, ofs, (f32 *)((u8 *)p + 0x15D0));
    if (PU(p, 0x15C4, s32) == -1) {
        return 0;
    }
    return (VCALL(p, 0xDC, s32 (*)(Pursuer *))(p) & 0xFF) == 1;
}

/* plan a path to triangle +0x15A4 / point +0x15B0; its length to +0x1590 (-1: none) */
s32 func_00212360(Pursuer *p) {
    if (func_00127140(&p->c, 0, PU(p, 0x15A4, s32), (f32 *)((u8 *)p + 0x15B0)) <= 0) {
        return 0;
    }
    if (func_001270A0(&p->c) > 0) {
        PU(p, 0x1590, f32) = VCALL(gSceneGameF29740, 0x30, f32 (*)(VObject *, s32))(gSceneGameF29740, p->c.pathId);
        func_00127060(&p->c);
        return 1;
    }
    PU(p, 0x1590, f32) = -1.0f;
    func_00127060(&p->c);
    return 0;
}

/* height of the bone vtable +0x80 of the model above its base (+0x804), at least 3 */
void func_00213E30(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));
    u8 *m = p->c.motion;
    s32 bone = VCALL(m, 0x80, s32 (*)(void *))(m);
    f32 h;

    sceVu0CopyVector(v, func_0017CE80(MOTION_AT(p, 0x810, u8 *), bone) + 0xC);
    h = v[1] - MOTION_AT(p, 0x804, f32);
    if (h < 3.0f) {
        h = 3.0f;
    }
    p->c.a.height = h;
}

/* is nav triangle `tri` blocked for the pursuer? */
s32 func_00214A90(Pursuer *p, u32 tri) {
    u32 flags;

    if (tri < AT(gNavMesh, 0x8, u32) && AT(gNavMesh, 0x4, u8 *) != NULL) {
        flags = AT(AT(gNavMesh, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
    } else {
        flags = 0;
    }
    return (p->c.a.navMask & flags) != 0;
}

/* plan a path to the room object behind the exit the pursuer heads for */
s32 func_00214AF0(Pursuer *p) {
    VObject *o = VCALL(gEvents, 0x64, VObject *(*)(VObject *))(gEvents);
    s32 *t = VCALL(o, 0x3C, s32 *(*)(VObject *))(o);

    if (t != NULL) {
        s32 r = t[p->c.door];

        if (r != -1) {
            PU(p, 0x1594, s32) = r;
            PU(p, 0x1598, s32) = -1;
            if (func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) > 0) {
                return 1;
            }
        }
    }
    return 0;
}

/* reached the room +0x1594 (and side +0x1598, -1 any)? */
s32 func_00217260(Pursuer *p) {
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
s32 func_002172F0(Pursuer *p, Character *c) {
    if (c != NULL) {
        return VCALL(gRooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(gRooms, c->a.room, c->door, 1);
    }
    return -1;
}

/* in the room being played? */
s32 func_00217510(Pursuer *p) {
    return p->c.a.room == VCALL(gProgress, 0xC, s32 (*)(Progress *))(gProgress);
}

/* is Fiona panicking (fear over 90, or her state 0xE)? */
s32 func_00217560(void) {
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
s32 func_002175B0(Actor *a, Actor *b) {
    s32 r = 0;

    if (b->pos[1] <= a->pos[1] + 15.0f && !(b->pos[1] + 10.0f < a->pos[1])) {
        r = 1;
    }
    return r;
}

/* the walkable triangle one height ahead (along the heading); 0 if none */
s32 func_00217600(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));
    u32 tri;

    func_002E2C10(v, p->c.a.angle[1]);
    sceVu0ScaleVector(v, v, p->c.a.height);
    sceVu0AddVector(v, v, p->c.a.pos);
    tri = func_00124480(&p->c.a, v, p->c.a.navMask);
    if (tri == (u32)-1) {
        return 0;
    }
    return func_00123470(p, tri, v);
}

/* add nav triangle `tri` to the route list (+0x15E0, 8 entries of 8 bytes) */
s32 func_00218C20(Pursuer *p, u32 tri) {
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

/* aim at the current entry of the route list: triangle +0x15A4, its centre to +0x15B0 */
void func_00218E70(Pursuer *p) {
    u8 i = PU(p, 0x1620, u8);

    if (i < PU(p, 0x1621, u8)) {
        PU(p, 0x15A4, s32) = PU(p, 0x15E0 + i * 8, s32);
        VCALL(gNavMesh, 0xC, void (*)(void *, s32, f32 *, Pursuer *))(gNavMesh, PU(p, 0x15E0 + PU(p, 0x1620, u8) * 8, s32), (f32 *)((u8 *)p + 0x15B0), p);
    }
}

/* ---- batch 3 ---- */

extern Character *gCharacters[6];
extern Character *gCharPartner;   /* Hewie */


/* vtable +0x9C: offset of the point beside a door, by side (Lorenzo's wheelchair etc. differ) */
void func_00179780(Pursuer *p, s32 side, f32 *out) {
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

/* vtable +0x128 (Debilitas: own): the stand animation by +0x16C8 */
void func_00179620(Pursuer *p) {
    u8 k = PU(p, 0x16C8, u8);

    switch (k) {
    case 0:
    case 1:
    case 2:
    case 4:
        func_00297B40(p, VCALL(p, 0x328, s32 (*)(Pursuer *, u32))(p, k), 0);
        break;
    case 3: {
        u8 j = PU(p, 0x16C9, u8);

        if (j != 0 && j != 2) {
            func_00297B40(p, VCALL(p, 0x328, s32 (*)(Pursuer *, u32))(p, k), 0);
        } else {
            func_00297B40(p, VCALL(p, 0x324, s32 (*)(Pursuer *, u32))(p, k), 0);
        }
        break;
    }
    }
}

/* path length from node `a` to node `b` of `room`, through the room's table (-1 none) */
f32 func_00211F70(Pursuer *p, s32 room, u32 a, u32 b) {
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

/* is `pos` of room `room` where the room's spawn point is, and reachable? */
s32 func_00212190(Pursuer *p, s32 room) {
    VObject *rm = gRooms;
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    u32 tri = VCALL(rm, 0x30, u32 (*)(VObject *, s32, f32 *))(rm, room, a);

    VCALL(rm, 0x34, void (*)(VObject *, s32, f32 *))(rm, room, b);
    if (tri != func_00124480(&p->c.a, a, 0)) {
        return 0;
    }
    return (func_00123C60(&p->c.a, room, b) & 0xFF) != 0;
}

/* the first open door (0..4) while Fiona is hiding (move mode 3), -1 none */
s32 func_002131A0(void) {
    if (gCharPlayer->moveMode == 3) {
        Progress *pr = gProgress;
        s32 i;

        for (i = 0; (u32)i < 5; i++) {
            s32 valid = i >= 0 && (u32)i < AT(gNavMesh, 0x14, u32);

            if ((valid & 0xFF) == 1 && (func_00177A20(pr, i & 0xFF, 0) & 0xFF & 1)) {
                return i;
            }
        }
    }
    return -1;
}

/* path length to triangle `tri` / point `pos`, from the nearest walkable triangle if blocked */
f32 func_00213C60(Pursuer *p, u32 tri, const f32 *pos) {
    f32 v[4] __attribute__((aligned(16)));
    u32 flags;

    if (tri < AT(gNavMesh, 0x8, u32) && AT(gNavMesh, 0x4, u8 *) != NULL) {
        flags = AT(AT(gNavMesh, 0x4, u8 *) + tri * 0x50, 0x3C, u32);
    } else {
        flags = 0;
    }
    if (p->c.a.navMask & flags) {
        tri = func_00216E00(p, tri, pos, v);
    } else {
        sceVu0CopyVector(v, pos);
    }
    if (tri == (u32)-1) {
        return -1.0f;
    }
    return func_001257B0(&p->c, tri, v, VCALL(p, 0xA8, u32 (*)(Pursuer *))(p));
}

/* how far to character `c` on foot: straight if in sight on its triangle, else by path */
f32 func_00213D40(Pursuer *p, Character *c) {
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
        if (tri == func_00124480(&p->c.a, v, p->c.a.navMask)) {
            return func_00124490(&p->c.a, c->a.pos);
        }
    }
    return VCALL(p, 0xD4, f32 (*)(Pursuer *, u32, const f32 *))(p, tri, c->a.pos);
}

/* which way to turn to face `heading`: 1 left beyond `a`, 0 right beyond -`a`, 0xFF within;
 * bit 1 when more than `b` off (if `b` >= `a`) */
u32 func_00213EC0(Pursuer *p, f32 heading, f32 a, f32 b) {
    f32 d;
    u32 r;

    if (a <= 0.0f) {
        a = -a;
    }
    if (b <= 0.0f) {
        b = -b;
    }
    d = func_002E2D00(heading - p->c.a.angle[1]);
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

/* turn towards `heading` by at most `step`; the angle left */
f32 func_002140A0(Pursuer *p, f32 heading, f32 step) {
    f32 cur = p->c.a.angle[1];
    f32 d = func_002E2D00(heading - cur);
    f32 s = step <= 0.0f ? -step : step;
    f32 ad = d <= 0.0f ? -d : d;
    f32 h;

    if (ad <= s) {
        h = heading;
    } else if (!(d < 0.0f)) {
        h = func_002E2D00(cur + step);
    } else {
        h = func_002E2D00(cur - step);
    }
    p->c.a.angle[1] = h;
    func_002E3190(p->c.a.rot, h);
    return func_002E2D00(heading - h);
}

/* step along the path; the stride from the animation's root motion if `step` <= 0 */
s32 func_00214890(Pursuer *p, u32 *triOut, f32 *posOut, f32 step) {
    if (step <= 0.0f) {
        f32 v[4] __attribute__((aligned(16)));
        u8 *m;

        func_001F6370(p->c.motion, v, 0.0f);
        m = p->c.motion;
        step = v[2] * VCALL(m, 0x44, f32 (*)(void *, Pursuer *))(m, p);
    }
    if (step < 0.0f) {
        return -1;
    }
    return func_001273D0(&p->c, triOut, posOut, step);
}

/* path length to triangle `tri` / point `pos` (null: the triangle's centre), -1 none */
f32 func_00214B90(Pursuer *p, u32 tri, const f32 *pos) {
    f32 v[4] __attribute__((aligned(16)));

    if (pos != NULL) {
        sceVu0CopyVector(v, pos);
    } else {
        VCALL(gNavMesh, 0xC, void (*)(void *, u32, f32 *))(gNavMesh, tri, v);
    }
    if (func_00127140(&p->c, 0, tri, v) <= 0) {
        return -1.0f;
    }
    if (func_001270F0(&p->c) > 0) {
        f32 d = VCALL(gSceneGameF29740, 0x30, f32 (*)(VObject *, s32))(gSceneGameF29740, p->c.pathId);

        func_00127060(&p->c);
        return d;
    }
    func_00127060(&p->c);
    return -1.0f;
}

/* are the pursuer and `c` in the same room but on different sides? */
s32 func_00217370(Pursuer *p, Character *c) {
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
s32 func_00217460(Pursuer *p, s32 slot) {
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

/* who can the pursuer see (bit per character slot 0..2)? */
s32 func_00217920(Pursuer *p) {
    s32 seen = 0;
    u32 i;

    for (i = 0; i < 3; i = (i + 1) & 0xFF) {
        Character *c = gCharacters[i & 0xFF];

        if (c != NULL && (i & 0xFF) != (u32)p->c.a.slot && c->a.active != 0 &&
            (func_001241F0(&p->c.a, &c->a, 1.0f, 0.0f) & 0xFF) == 1) {
            seen = (seen | ((1 << (i & 0xFF)) & 0xFF)) & 0xFF;
        }
    }
    return seen;
}

/* is the point `dist` away in direction `angle` from the target's heading walkable? */
s32 func_00217ED0(Pursuer *p, f32 angle, f32 dist) {
    f32 v[4] __attribute__((aligned(16)));
    f32 w[4] __attribute__((aligned(16)));
    u32 tri;

    func_002E2C10(v, func_002E2D00(angle + func_001244D0(&p->c.a, p->target->a.pos)));
    sceVu0ScaleVector(v, v, dist);
    sceVu0AddVector(w, p->c.a.pos, v);
    tri = func_00124480(&p->c.a, w, p->c.a.navMask);
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

/* probe the 8 directions around Fiona, 20 units out (results to +0x1548) */
void func_00218110(Pursuer *p) {
    s32 i;

    for (i = 0; i < 8; i++) {
        f32 h = func_001244D0(&p->c.a, gCharPlayer->a.pos);

        PU(p, 0x1548 + i * 4, s32) = func_00217D30(p, func_002E2D00(0x1.921fb60000000p+2f /* 6.2831855 */ * (f32)i / 8.0f + h), 20.0f);
    }
}

/* is Hewie close enough to be caught (in reach and on the same walkable triangle)? */
s32 func_00218940(Pursuer *p) {
    Character *h = gCharPartner;

    if (func_00218430(p, h) != 0) {
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

            if (func_00124480(&p->c.a, h->a.pos, 0x40080) == tri) {
                return 1;
            }
        }
    }
    return 0;
}

/* drop the first route entry; the new current one, -1 none */
s32 func_00218B60(Pursuer *p) {
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

/* head for the room / side next to Hewie's (exit `exit` of his room); else keep the goal */
void func_00218C90(Pursuer *p, u32 exit) {
    Character *h = gCharPartner;
    VObject *rm = gRooms;
    s32 room = VCALL(rm, 0x18, s32 (*)(VObject *, s32, u32))(rm, h->a.room, exit);
    s32 side = VCALL(rm, 0x50, s32 (*)(VObject *, s32, u32, s32))(rm, room, VCALL(rm, 0x14, u32 (*)(VObject *, s32, u32))(rm, h->a.room, exit) & 0xFF, 1);

    if (func_00126F80(&p->c, room, side, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = room;
        PU(p, 0x1598, s32) = side;
    } else {
        func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
}

/* the same next to Fiona's room */
void func_00218D80(Pursuer *p, u32 exit) {
    Character *f = gCharPlayer;
    VObject *rm = gRooms;
    s32 room = VCALL(rm, 0x18, s32 (*)(VObject *, s32, u32))(rm, f->a.room, exit);
    s32 side = VCALL(rm, 0x50, s32 (*)(VObject *, s32, u32, s32))(rm, room, VCALL(rm, 0x14, u32 (*)(VObject *, s32, u32))(rm, f->a.room, exit) & 0xFF, 1);

    if (func_00126F80(&p->c, room, side, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = room;
        PU(p, 0x1598, s32) = side;
    } else {
        func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
}

/* ---- batch 4 ---- */


/* door / exit `exit`: what to do with it (vtable +0xF0 to go through); 2 / 1 / 0 */
s32 func_00211CF0(Pursuer *p, s32 exit) {
    Progress *pr;

    switch (func_00211E00(p, exit) & 0xFF) {
    case 2:
        return 2;
    case 6:
        pr = gProgress;
        func_00178DB0(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot);
        func_00178750(pr, p->c.a.room, exit);
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

/* path length between the room nodes of `a` and `b` (rooms +0x10 / +0x38), -1 none */
f32 func_00212060(Pursuer *p, s32 room, s32 a, s32 b) {
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

/* coming into room `room`: is Fiona at the spawn point there (+0x1624)? */
void func_00212240(Pursuer *p, s32 room) {
    VObject *rm = gRooms;
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));

    PU(p, 0x1624, s32) = 0;
    VCALL(rm, 0x30, u32 (*)(VObject *, s32, f32 *))(rm, room, a);
    VCALL(rm, 0x34, void (*)(VObject *, s32, f32 *))(rm, room, b);
    if ((func_00123C60(&p->c.a, room, b) & 0xFF) == 1) {
        if (func_00124480(&p->c.a, a, -1) != (u32)-1) {
            p->c.unk124 = p->c.unk128;
        }
        if (!(func_00178300(gProgress, p->c.a.room, room, *(u8 *)&p->c.a.slot) & 0xFF)) {
            return;
        }
        if ((func_00123C60(&p->c.a, room, gCharPlayer->a.pos) & 0xFF) == 1) {
            PU(p, 0x1624, s32) = 1;
        }
    }
}

/* heading through exit `exit` (from its inner to its outer point) */
f32 func_00212730(Pursuer *p, s32 exit) {
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
    return func_002E2BC0(d);
}

/* turn to the root motion's direction (rotated to the walk mesh slope through triangle +0x34) */
void func_00213B60(Pursuer *p, u32 mask) {
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
    tri = func_00124480(&p->c.a, a, 0);
    h = func_001244D0(&p->c.a, a);
    if (tri != (u32)-1) {
        f32 r;

        VCALL(gNavMesh, 0x40, void (*)(void *, u32, f32 *, f32 *, f32 *, u32))(gNavMesh, p->c.a.navTri, b, p->c.a.pos, a, mask);
        r = func_002E2D00(p->c.a.angle[1] + func_002E2D00(func_001244D0(&p->c.a, b) - h));
        p->c.a.angle[1] = r;
        sceVu0UnitMatrix(p->c.a.rot);
        sceVu0RotMatrixY(p->c.a.rot, p->c.a.rot, r);
    }
}

/* which way to turn to face point `pos` (see func_00213EC0) */
u32 func_00213FA0(Pursuer *p, const f32 *pos, f32 a, f32 b) {
    f32 heading = func_001244D0(&p->c.a, pos);
    f32 d;
    u32 r;

    if (a <= 0.0f) {
        a = -a;
    }
    if (b <= 0.0f) {
        b = -b;
    }
    d = func_002E2D00(heading - p->c.a.angle[1]);
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

/* can an eye at `from` facing `heading` see `to`: within `range` and `half` an angle either side */
s32 func_002181D0(Pursuer *p, const f32 *from, const f32 *to, f32 heading, f32 range, f32 half) {
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
    a = func_0031C5C0(dx, dz);
    if (!(dist <= range)) {
        return 0;
    }
    a = a - heading;
    if (!((func_002E2D00(a) <= 0.0f ? -func_002E2D00(a) : func_002E2D00(a)) <= half)) {
        return 0;
    }
    return 1;
}

/* the same between two actors */
s32 func_00218300(Pursuer *p, Actor *from, Actor *to, f32 heading, f32 range, f32 half) {
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
    a = func_0031C5C0(dx, dz);
    if (!(dist <= range)) {
        return 0;
    }
    a = a - heading;
    if (!((func_002E2D00(a) <= 0.0f ? -func_002E2D00(a) : func_002E2D00(a)) <= half)) {
        return 0;
    }
    return 1;
}

/* is Fiona within reach to be caught (seen, or 20 units on a walkable line; 10 with progress
 * flag 0xA; never with flag 9 or while her +0x1AD630 is set)? */
s32 func_00218A30(Pursuer *p) {
    Character *f = gCharPlayer;
    Progress *pr;
    f32 reach, d;
    s32 near;

    if (AT(f, 0x1AD630, u8) != 0) {
        return 0;
    }
    if (func_00218430(p, f) != 0) {
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

        if (func_00124480(&p->c.a, f->a.pos, 0x40080) == tri) {
            return 1;
        }
    }
    return 0;
}

/* ---- batch 5 ---- */

extern void *D_0046C220[], *D_00469C60[], *D_00469C20[];

/* vtable +0x8: NPC destructor (-> Character) */
Pursuer *func_001710D0(Pursuer *p, s32 flags) {
    if (p != NULL) {
        p->c.a.vtbl = D_0046C220;
        VCALL(p, 0x10, void (*)(Pursuer *))(p);
        if (p != NULL) {
            p->c.a.vtbl = D_00469C60;
            if (p != NULL) {
                p->c.a.vtbl = D_00469C20;
            }
        }
        if ((s16)flags > 0) {
            func_00124E40(&p->c.a);
        }
    }
    return p;
}

/* vtable +0xC: NPC init */
void func_00219460(Pursuer *p) {
    u32 i;

    p->c.a.unkC4 = 0;
    func_00127660(&p->c);
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

/* vtable +0xE8: head for a random other room (10 tries to avoid the played one) */
s32 func_00212400(Pursuer *p) {
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
    if (PU(p, 0x1594, s32) != -1 && func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) != -1) {
        return 1;
    }
    do {
        PU(p, 0x1594, s32) = VCALL(pr, 0x38, s32 (*)(Progress *, s32))(pr, p->c.a.room);
    } while (PU(p, 0x1594, s32) == p->c.a.room);
    if (PU(p, 0x1594, s32) != -1) {
        return func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1) != -1;
    }
    return 0;
}

/* plan a path to `pos` / triangle `tri` from the point beside door `door` (side 1 first, then 0);
 * the side that works, 0xFF none */
u32 func_00212E00(Pursuer *p, u32 tri, const f32 *pos, u32 door) {
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
        t = func_00123710(p, door & 0xFF, side & 0xFF, ofs, at);
        if (t != (u32)-1) {
            p->c.pathReq->startTri = t;
            sceVu0CopyVector(p->c.pathReq->startPos, at);
            p->c.pathId = VCALL(pl, 0xC, s32 (*)(VObject *, PathRequest *, s32))(pl, p->c.pathReq, 0);
            if (p->c.pathId != -1) {
                if (VCALL(pl, 0x14, s32 (*)(VObject *))(pl) >= 0) {
                    func_00127060(&p->c);
                    return side;
                }
                func_00127060(&p->c);
            }
        }
        side = (side == 0) & 0xFF;
        if (side != 0) {
            return 0xFF;
        }
    }
}

/* vtable +0xAC: go to triangle `tri` / point `pos` of room `room` (-1 the played one) */
void func_00219310(Pursuer *p, u32 tri, const f32 *pos, s32 room) {
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
    if (func_00126F80(&p->c, room, -1, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = room;
        PU(p, 0x1598, s32) = -1;
    } else {
        func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
}

/* a random walkable triangle of the played room (not blocked, not flagged 0x100000 without
 * 0x200000... ); -1 if the pursuer is elsewhere */
u32 func_00214940(Pursuer *p) {
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

/* can the pursuer walk straight to `pos` (over triangles without flag 0x4000)? */
s32 func_00217110(Pursuer *p, const f32 *pos) {
    u32 tri = func_00124480(&p->c.a, pos, 0);

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
    return (func_00122C90(&p->c.a, p->c.a.navTri, tri, p->c.a.pos, pos, 0) & 0xFF) == 0;
}

/* vtable +0x... : who's around (+0x1544 Fiona, +0x1545 Hewie, +0x1546 noise) */
void func_00217680(Pursuer *p) {
    Progress *pr = gProgress;
    s32 room = p->c.a.room;

    if (room != VCALL(pr, 0xC, s32 (*)(Progress *))(pr)) {
        PU(p, 0x1544, u8) = 0;
        PU(p, 0x1545, u8) = 0;
    } else {
        PU(p, 0x1574, f32) = func_002E2D00(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
        if (p->c.moveMode != 2 && (func_00212850(p) & 0xFF) != 0xFF) {
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

/* the same, unless the progress byte +0x1FBEC1 is set (then func_00217680) */
void func_002177D0(Pursuer *p) {
    Progress *pr = gProgress;

    if (AT(pr, 0x1FBEC1, u8) == 0) {
        s32 room = p->c.a.room;

        if (room != VCALL(pr, 0xC, s32 (*)(Progress *))(pr)) {
            PU(p, 0x1544, u8) = 0;
            PU(p, 0x1545, u8) = 0;
        } else {
            PU(p, 0x1574, f32) = func_002E2D00(p->c.a.angle[1] + MOTION_AT(p, 0x858, f32));
            if (p->c.moveMode != 2 && (func_00212850(p) & 0xFF) != 0xFF) {
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
    func_00217680(p);
}

/* is there room `dist` to the side of the target (90 degrees one way, then the other)? */
s32 func_00217FC0(Pursuer *p, f32 dist) {
    f32 a = 0x1.921fb60000000p+0f /* 1.5707964 */;

    for (;;) {
        f32 v[4] __attribute__((aligned(16)));
        f32 w[4] __attribute__((aligned(16)));
        u32 tri;
        s32 ok = 0;

        func_002E2C10(v, func_002E2D00(a + func_001244D0(&p->c.a, p->target->a.pos)));
        sceVu0ScaleVector(v, v, dist);
        sceVu0AddVector(w, p->c.a.pos, v);
        tri = func_00124480(&p->c.a, w, p->c.a.navMask);
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

/* ---- batch 6 ---- */


/* path length to the nearest walkable point of triangle +0x15C4 / point +0x15D0 (+0x1590) */
s32 func_00216B20(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));
    f32 w[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    u32 tri = func_00216E00(p, PU(p, 0x15C4, u32), (f32 *)((u8 *)p + 0x15D0), v);
    f32 len;

    if (tri == (u32)-1) {
        PU(p, 0x1590, f32) = -1.0f;
        return 0;
    }
    sceVu0CopyVector(w, v);
    if (func_00127140(&p->c, 0, tri, w) <= 0) {
        len = -1.0f;
    } else if (func_001270F0(&p->c) > 0) {
        len = VCALL(gSceneGameF29740, 0x30, f32 (*)(VObject *, s32))(gSceneGameF29740, p->c.pathId);
        func_00127060(&p->c);
    } else {
        func_00127060(&p->c);
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
s32 func_00216C90(Pursuer *p) {
    f32 v[4] __attribute__((aligned(16)));
    f32 w[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    u32 tri = func_00216E00(p, PU(p, 0x15A4, u32), (f32 *)((u8 *)p + 0x15B0), v);
    f32 len;

    if (tri == (u32)-1) {
        PU(p, 0x1590, f32) = -1.0f;
        return 0;
    }
    sceVu0CopyVector(w, v);
    if (func_00127140(&p->c, 0, tri, w) <= 0) {
        len = -1.0f;
    } else if (func_001270F0(&p->c) > 0) {
        len = VCALL(gSceneGameF29740, 0x30, f32 (*)(VObject *, s32))(gSceneGameF29740, p->c.pathId);
        func_00127060(&p->c);
    } else {
        func_00127060(&p->c);
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

/* can the pursuer see point `pos` (on triangle `tri`): in its view (+0x1580 range, +0x1584
 * angle, heading +0x1574) and nothing in the way? */
s32 func_002187D0(Pursuer *p, u32 tri, const f32 *pos) {
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
        a = func_0031C5C0(dx, dz);
        if (dist <= range) {
            a = a - heading;
            if ((func_002E2D00(a) <= 0.0f ? -func_002E2D00(a) : func_002E2D00(a)) <= half) {
                in = 1;
            }
        }
    }
    if (!(in & 0xFF)) {
        return 0;
    }
    return func_00122C90(&p->c.a, p->c.a.navTri, tri, p->c.a.pos, pos, 0);
}

/* what is exit `exit` like for the pursuer: 2 its own way in, 3 closed to it, 4 open, 5 / 6
 * it must open it (6: from the other side) */
u32 func_00211E00(Pursuer *p, s32 exit) {
    Progress *pr = gProgress;
    VObject *rm;

    if ((Progress_CurRoomFlag(pr, p->c.a.room, exit) & 0xFF) == 1) {
        return 2;
    }
    if (!(func_00178300(pr, p->c.a.room, exit, *(u8 *)&p->c.a.slot) & 0xFF)) {
        return 3;
    }
    if ((func_001785B0(pr, p->c.a.room, exit) & 0xFF) == 1) {
        return 3;
    }
    rm = gRooms;
    if (!(VCALL(rm, 0x78, s32 (*)(VObject *, s32, s32))(rm, p->c.a.room, exit) & 0xFF)) {
        return 4;
    }
    if ((func_00178980(pr, p->c.a.room, exit) & 0xFF) == 1) {
        return 4;
    }
    if ((func_00178840(pr, p->c.a.room, exit) & 0xFF) == 1) {
        return (VCALL(rm, 0x4C, s32 (*)(VObject *, s32, s32))(rm, p->c.a.room, exit) & 0xFF) == 1 ? 6 : 3;
    }
    return 5;
}

/* the triangle `dist` away in direction `heading`: -1 none, -2 blocked, -3 flag 1, -4 at a
 * door, -5 at a room point */
u32 func_00217D30(Pursuer *p, f32 heading, f32 dist) {
    f32 v[4] __attribute__((aligned(16)));
    f32 w[4] __attribute__((aligned(16)));
    void *nm;
    u32 tri, flags, i;

    func_002E2C10(v, func_002E2D00(heading));
    sceVu0ScaleVector(v, v, dist);
    sceVu0AddVector(w, p->c.a.pos, v);
    tri = func_00124480(&p->c.a, w, 0);
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

/* who of the characters can the pursuer reach / see by the progress tables (+0x30 / +0x2C) and is
 * in front of it (within 90 degrees): a bit per slot */
s32 func_002179F0(Pursuer *p, s32 a1, f32 f) {
    Progress *pr = gProgress;
    s32 bits = 0;
    u32 i;

    for (i = 0; i < 3; i = (i + 1) & 0xFF) {
        u32 s = i & 0xFF;
        Character **c = &gCharacters[s];

        if (*c != NULL && s != (u32)p->c.a.slot && (*c)->a.active != 0 &&
            (VCALL(pr, 0x30, s32 (*)(Progress *, u32, s32, u32, f32))(pr, p->c.a.slot & 0xFF, a1, i, f) & 0xFF) == 1) {
            f32 d = func_002E2D00(p->c.a.angle[1] - func_001244D0(&p->c.a, (*c)->a.pos)) <= 0.0f
                        ? -func_002E2D00(p->c.a.angle[1] - func_001244D0(&p->c.a, (*c)->a.pos))
                        : func_002E2D00(p->c.a.angle[1] - func_001244D0(&p->c.a, (*c)->a.pos));

            if (d < 0x1.921fb60000000p+0f /* 1.5707964 */) {
                bits = (bits | ((1 << s) & 0xFF)) & 0xFF;
            }
        }
    }
    return bits;
}

s32 func_00217B90(Pursuer *p, s32 a1, f32 f) {
    Progress *pr = gProgress;
    s32 bits = 0;
    u32 i;

    for (i = 0; i < 3; i = (i + 1) & 0xFF) {
        u32 s = i & 0xFF;
        Character **c = &gCharacters[s];

        if (*c != NULL && s != (u32)p->c.a.slot && (*c)->a.active != 0 &&
            (VCALL(pr, 0x2C, s32 (*)(Progress *, u32, s32, u32, f32))(pr, p->c.a.slot & 0xFF, a1, i, f) & 0xFF) == 1) {
            f32 d = func_002E2D00(p->c.a.angle[1] - func_001244D0(&p->c.a, (*c)->a.pos)) <= 0.0f
                        ? -func_002E2D00(p->c.a.angle[1] - func_001244D0(&p->c.a, (*c)->a.pos))
                        : func_002E2D00(p->c.a.angle[1] - func_001244D0(&p->c.a, (*c)->a.pos));

            if (d < 0x1.921fb60000000p+0f /* 1.5707964 */) {
                bits = (bits | ((1 << s) & 0xFF)) & 0xFF;
            }
        }
    }
    return bits;
}

/* plan a path from triangle `tri` / `pos` (-1: where the pursuer is) to door `door`, side 0 then
 * 2; the door's triangle (+0x104 side, +0x1568 heading, +0x110 point), -1 none */
s32 func_002134E0(Pursuer *p, u32 door, s32 tri, const f32 *pos) {
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
        func_00127060(&p->c);
        side = side != 2 ? 2 : 0;
        if (side == 0) {
            return -1;
        }
    }
}

/* find a door (0..4) to go through, from side `side` (0 / 1, other values: either); door to
 * +0x100, side to +0x104 */
s32 func_00212FE0(Pursuer *p, s32 side) {
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
            PU(p, 0x15C4, s32) = func_00123710(p, i & 0xFF, sd & 0xFF, ofs, (f32 *)((u8 *)p + 0x15D0));
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

/* ---- batch 7 ---- */

/* path length to character `c` (null: the target): +0x1590, and +0x1588 (Fiona) / +0x158C
 * (Hewie) */
s32 func_00216960(Pursuer *p, Character *c) {
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
    tri = func_00216E00(p, c->a.navTri, v, v);
    sceVu0CopyVector(w, v);
    if (func_00127140(&p->c, 0, tri, w) <= 0) {
        len = -1.0f;
    } else if (func_001270F0(&p->c) > 0) {
        len = VCALL(gSceneGameF29740, 0x30, f32 (*)(VObject *, s32))(gSceneGameF29740, p->c.pathId);
        func_00127060(&p->c);
    } else {
        func_00127060(&p->c);
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

/* the facing of door / exit `exit` seen from Fiona (10 if none): while Fiona hides, the first
 * door she can be behind */
f32 func_00212550(Pursuer *p, u32 exit) {
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
                    Progress_CurRoomFlag(pr, p->c.a.room, i & 0xFF) != 0 && (func_00177BF0(pr, i & 0xFF, 0) & 0xFF & 4)) {
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
        a = func_002E2D00(0x1.921fb60000000p+1f /* 3.1415927 */ + a);
    }
    return a;
}

/* vtable +0xB0: head for Fiona (her room's side; her triangle if she's in a room the pursuer
 * can reach) */
void func_00219100(Pursuer *p) {
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
    if (func_00126F80(&p->c, f->a.room, side, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = f->a.room;
        PU(p, 0x1598, s32) = side;
    } else {
        func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
}

/* ---- batch 8 ---- */

extern u8 D_0047A930[8];   /* 0..7 */

/* a random exit of the pursuer's side of the room it can use (not `skip`); `skip` if none */
u32 func_00212A80(Pursuer *p, u32 skip) {
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

/* a door of the played room the pursuer must deal with on its way (0xFF none) */
u32 func_00212850(Pursuer *p) {
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
        st = func_00177BF0(pr, i & 0xFF, *(u8 *)&p->c.a.slot) & 0xFF;
        if (VCALL(rm, 0x70, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, i & 0xFF) != 0) {
            st &= 0xFF;
            if ((st & 4) && ((st ^ (func_00177BF0(pr, i & 0xFF, 0) & 0xFF)) & 0x10)) {
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

/* vtable +0xB4: head for character `c` (null: the target) */
void func_00218ED0(Pursuer *p, Character *c) {
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
    if (func_00126F80(&p->c, c->a.room, side, -1, -1) >= 0) {
        PU(p, 0x1594, s32) = c->a.room;
        PU(p, 0x1598, s32) = side;
    } else {
        func_00126F80(&p->c, PU(p, 0x1594, s32), PU(p, 0x1598, s32), -1, -1);
    }
}

/* turn towards `pos` by `step` (the shorter way, or the animation's turn if past 120 degrees);
 * the angle left */
f32 func_00214190(Pursuer *p, const f32 *pos, f32 step) {
    f32 h = func_001244D0(&p->c.a, pos);
    f32 d = h - p->c.a.angle[1];
    f32 r;

    if (d <= 0.0f) {
        d = -d;
    }
    if (!(d < step)) {
        f32 h2 = func_001244D0(&p->c.a, pos);
        f32 a = func_002E2D00(h2 - p->c.a.angle[1]) <= 0.0f ? -func_002E2D00(h2 - p->c.a.angle[1])
                                                            : func_002E2D00(h2 - p->c.a.angle[1]);
        f32 b = func_002E2D00(h2 - (p->c.a.angle[1] + MOTION_AT(p, 0x858, f32))) <= 0.0f
                    ? -func_002E2D00(h2 - (p->c.a.angle[1] + MOTION_AT(p, 0x858, f32)))
                    : func_002E2D00(h2 - (p->c.a.angle[1] + MOTION_AT(p, 0x858, f32)));
        s32 dir;

        if (a <= 0x1.0c15240000000p+1f /* 2.0943952 */ || a <= b) {
            dir = func_002E2D00(h2 - p->c.a.angle[1]) <= 0.0f ? -1 : 1;
        } else {
            dir = MOTION_AT(p, 0x858, f32) <= 0.0f ? -1 : 1;
        }
        r = func_002E2D00(p->c.a.angle[1] + (f32)dir * step);
        p->c.a.angle[1] = r;
        sceVu0UnitMatrix(p->c.a.rot);
        sceVu0RotMatrixY(p->c.a.rot, p->c.a.rot, r);
    } else {
        p->c.a.angle[1] = h;
        sceVu0UnitMatrix(p->c.a.rot);
        sceVu0RotMatrixY(p->c.a.rot, p->c.a.rot, h);
    }
    return func_002E2D00(h - p->c.a.angle[1]);
}

/* step towards `pos`: turn, then walk by the animation's stride (straight if within it) */
s32 func_002143D0(Pursuer *p, const f32 *pos) {
    f32 v[4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));
    u8 *mo;
    f32 d, ad;

    func_001F6370(p->c.motion, v, 0.0f);
    mo = p->c.motion;
    v[2] *= VCALL(mo, 0x44, f32 (*)(void *, Pursuer *))(mo, p);
    d = func_002E2D00(func_001244D0(&p->c.a, pos) - p->c.a.angle[1]);
    ad = d <= 0.0f ? -d : d;
    if (!(ad < 0x1.921fb60000000p-1f /* 0.7853982 */)) {
        if (d <= 0.0f) {
            d = -d;
        }
        if (d < 0x1.921fb60000000p+0f /* 1.5707964 */ && !(func_00124490(&p->c.a, pos) <= 10.0f)) {
            func_00214190(p, pos, 2.0f * VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
            return func_00124490(&p->c.a, pos) < 1.0f;
        }
        func_00214190(p, pos, 2.0f * VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
        return 0;
    }
    func_00214190(p, pos, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    if (func_00124490(&p->c.a, pos) <= v[2] && func_00124480(&p->c.a, pos, -1) != (u32)-1) {
        sceVu0SubVector(v, pos, p->c.a.pos);
    } else {
        func_002E3190(m, func_001244D0(&p->c.a, pos));
        func_002E2DA0(v, m, v);
    }
    func_001247E0(&p->c.a, v);
    return func_00124490(&p->c.a, pos) < 1.0f;
}

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
        if (tri == func_00124480(&p->c.a, v, p->c.a.navMask)) {
            return func_00124490(&p->c.a, c->a.pos);
        }
    }
    return VCALL(p, 0xD4, f32 (*)(Pursuer *, u32, f32 *))(p, tri, c->a.pos);
}

void func_00214C70(Pursuer *p) {
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

/* the same, unless the progress byte +0x1FBEC1 is set (then func_00214C70) */
void func_00214ED0(Pursuer *p) {
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
    func_00214C70(p);
}

/* ---- batch 9 ---- */

/* the exits of the room the pursuer could go through to reach triangle `tri`: door
 * +0x100, point beside it +0x15D0 (an exit it must open as a fallback) */
s32 func_00213690(Pursuer *p, s32 tri) {
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
        if ((func_00178980(pr, p->c.a.room, i & 0xFF) & 0xFF) == 1 && !(Progress_CurRoomFlag(pr, p->c.a.room, i & 0xFF) & 0xFF)) {
            continue;
        }
        if ((func_001785B0(pr, p->c.a.room, i & 0xFF) & 0xFF) == 1) {
            continue;
        }
        t = func_002134E0(p, i & 0xFF, tri, v);
        if (t == -1) {
            if (VCALL(rm, 0x70, s32 (*)(VObject *, s32, u32))(rm, p->c.a.room, i & 0xFF) != 0 || (fallback & 0xFF) == 0xFF) {
                if (func_002134E0(p, i & 0xFF, -1, NULL) != -1) {
                    fallback = i & 0xFF;
                }
            }
            continue;
        }
        PU(p, 0x15C4, s32) = func_002134E0(p, i & 0xFF, -1, NULL);
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
        PU(p, 0x15C4, s32) = func_002134E0(p, fallback, -1, NULL);
        p->c.unk100 = fallback & 0xFF;
        sceVu0CopyVector((f32 *)((u8 *)p + 0x15D0), p->c.unk110);
        return 1;
    }
    return 0;
}

/* leave door `door` (0xFF: +0x100): shut / open it behind, blocking flags back, and off its
 * triangle if that blocks the pursuer */
void func_00213270(Pursuer *p, u32 door) {
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
        func_00178C10(pr, VCALL(pr, 0xC, s32 (*)(Progress *))(pr), door, 0xFF);
    } else {
        Progress *pr;

        VCALL(d, 0x20, void (*)(VObject *, u32, s32, s32))(d, door, 1, 0x60000);
        VCALL(d, 0x1C, void (*)(VObject *, u32, s32, s32))(d, door, 0, 0x60000);
        pr = gProgress;
        func_00178A90(pr, VCALL(pr, 0xC, s32 (*)(Progress *))(pr), door, 0xFF);
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

/* walk the planned path one stride (the animation's); 1 at its end */
s32 func_00214620(Pursuer *p, s32 unused) {
    f32 v[4] __attribute__((aligned(16)));
    f32 w[4] __attribute__((aligned(16)));
    f32 pos[4] __attribute__((aligned(16)));
    u8 *m;
    f32 step, d;
    s32 next = -1;
    u32 tri;
    s32 last;

    func_001F6370(p->c.motion, v, 0.0f);
    m = p->c.motion;
    step = v[2] * VCALL(m, 0x44, f32 (*)(void *, Pursuer *))(m, p);
    if (!(step < 0.0f)) {
        next = func_001273D0(&p->c, &tri, pos, step);
    }
    if (next < 0) {
        return 0;
    }
    last = AT(p, 0x120 + p->c.unk124 * 0xC, s32);
    w[0] = AT(p, 0x124 + p->c.unk124 * 0xC, f32);
    w[2] = AT(p, 0x128 + p->c.unk124 * 0xC, f32);
    VCALL(gNavMesh, 0x14, void (*)(void *, s32, f32 *))(gNavMesh, last, w);
    if (sceVu0InnerProduct(p->c.a.pos, pos) == 0.0f && last == (s32)func_00124480(&p->c.a, w, 0x20008)) {
        d = func_002E2D00(func_001244D0(&p->c.a, w) - p->c.a.angle[1]);
    } else {
        d = func_002E2D00(func_001244D0(&p->c.a, pos) - p->c.a.angle[1]);
    }
    if (d <= 0.0f) {
        d = -d;
    }
    if (!(d < 0x1.921fb60000000p-1f /* 0.7853982 */) &&
        !(VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(gSceneGameF29740, p->c.a.pos, p->c.unk128, p->c.unk124, p->c.unk12C) < 4.0f)) {
        func_00214190(p, pos, 2.0f * VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
    } else {
        func_00214190(p, pos, VCALL(p, 0xA0, f32 (*)(Pursuer *))(p));
        p->c.a.navTri = tri;
        sceVu0CopyVector(p->c.a.pos, pos);
        p->c.unk128 = next;
    }
    return p->c.unk128 >= p->c.unk124;
}

/* can the pursuer get round to triangle `tri` through one of the doors (0..4), from the side
 * it is on? door +0x100, side +0x104, point +0x15D0 */
s32 func_002138F0(Pursuer *p, s32 tri) {
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
        t = func_00123710(p, i, other, ofs, out);
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
            func_00127060(&p->c);
            continue;
        }
        func_00127060(&p->c);
        VCALL(p, 0x9C, void (*)(Pursuer *, u32, f32 *))(p, s, ofs);
        t2 = func_00123710(p, i, s, ofs, out);
        if (t2 == -1) {
            continue;
        }
        if (func_00127140(&p->c, 0, t2, out) < 0) {
            func_00127060(&p->c);
            continue;
        }
        func_00127060(&p->c);
        PU(p, 0x15C4, s32) = t2;
        sceVu0CopyVector((f32 *)((u8 *)p + 0x15D0), out);
        p->c.unk100 = i;
        p->c.unk104[0] = s;
        return 1;
    }
    return 0;
}

/* ---- batch 11 ---- */

/* the walkable point nearest to triangle `tri` / `pos` for the pursuer: from a door it may
 * block, through the walk mesh to the first free triangle (a bit inside it) */
u32 func_00216E00(Pursuer *p, u32 tri, const f32 *pos, f32 *out) {
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
            t = func_00211B00(p, t);
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
            func_002E2C10(v, func_002E2BC0(d2));
            sceVu0ScaleVector(v, v, 0x1.99999a0000000p-4f /* 0.1 */);
            sceVu0AddVector(out, e, v);
            return t;
        }
    }
    sceVu0CopyVector(out, pos);
    return tri;
}

/* ---- batch 13 ---- */

/* can the pursuer see character `c`? in its view (+0x1580 range, +0x1584 angle, heading
 * +0x1574) and in sight of her middle, or else of one of 9 points around the far side of her
 * body (her radius out, 22.5 degrees apart); sight is blocked by triangle flags 0x40080
 * (0x40088 with progress flag 9 or 0xA) */
s32 func_00218430(Pursuer *p, Character *c) {
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
        a = func_0031C5C0(dx, dz);
        if (dist <= range) {
            a = a - heading;
            if ((func_002E2D00(a) <= 0.0f ? -func_002E2D00(a) : func_002E2D00(a)) <= half) {
                in = 1;
            }
        }
    }
    if (!(in & 0xFF)) {
        return 0;
    }
    if ((func_00122C90(&p->c.a, p->c.a.navTri, ctri, me, at, mask) & 0xFF) == 1) {
        return 1;
    }
    {
        f32 dir[4] __attribute__((aligned(16)));
        f32 off[4] __attribute__((aligned(16)));
        f32 pt[4] __attribute__((aligned(16)));
        u32 i;

        sceVu0SubVector(dir, at, p->c.a.pos);
        sceVu0Normalize(dir, dir);
        func_002E2CA0(off, dir, 0x1.921fb60000000p+0f /* 1.5707964 */);
        sceVu0Normalize(off, off);
        sceVu0ScaleVector(off, off, c->a.radius);
        for (i = 0; i < 9; i = (i + 1) & 0xFF) {
            u32 t;

            sceVu0AddVector(pt, at, off);
            t = func_00124480(&p->c.a, pt, 0);
            if (t != (u32)-1 && func_00123080(&p->c.a, ctri, t, at, pt, mask) == -1 &&
                (func_00122C90(&p->c.a, p->c.a.navTri, t, me, pt, mask) & 0xFF) == 1) {
                return 1;
            }
            func_002E2CA0(off, off, 0x1.921fb60000000p-2f /* 0.3926991 */);
        }
    }
    return 0;
}

/* ---- batch 22 ---- */

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
        func_001F6370(p->c.motion, vel, 0.0f);
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

s32 func_00215130(Pursuer *p) {
    return Npc_Senses(p, 1);
}

/* the same, Fiona watched wherever she is; func_00215130 while the progress byte +0x1FBEC1 is set */
s32 func_00215D80(Pursuer *p) {
    if (AT(gProgress, 0x1FBEC1, u8) != 0) {
        return func_00215130(p);
    }
    return Npc_Senses(p, 0);
}

/* vtable +0xEC: can the pursuer go through exit `exit`: 1 if func_00211E00 says 4, 5 or 6,
   2 if it says 2 (passed on as is), else 0 */
s32 func_00127C40(Pursuer *p, s32 exit) {
    switch (func_00211E00(p, exit) & 0xFF) {
    case 2:
        return 2;
    case 4:
    case 5:
    case 6:
        return 1;
    }
    return 0;
}
