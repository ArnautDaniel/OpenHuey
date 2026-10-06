/* Actor base class (vtables 0x469C20 -> 0x469C60): position, room, nav mesh movement tests,
 * positional sound. Characters (Fiona, Hewie, pursuers) derive from it. */
#include "common.h"
#include "actor.h"
#include "navmesh.h"
#include "sce/libvu0.h"
#include "globals.h"
#include "progress.h"
#include "ptmf.h"
#include "hewie.h"
#include "model.h"
#include "scene_game_members.h"
#include "snd_place.h"
#include "stalker_progress.h"
#include "msl.h"

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

static void b4_clear_dca70(u8 *p) {
    s32 i;

    F(p, 0x38, u32) = 0;
    F(p, 0x3C, u32) = 0;
    F(p, 0x40, u32) = 0;
    F(p, 0x48, u32) = 0;
    F(p, 0x4C, u32) = 0;
    F(p, 0x50, u32) = 0;
    for (i = 0; i < 16; i++) {
        F(p, 0x58 + i * 4, u32) = 0;
    }
    F(p, 0x870, u32) = 0;
}

void Motion_Disable(u8 *p);

/* Position relative to the current room: pos + origin(own room) - origin(current room).
 * False if either room is unknown. */
/* 0x00122B50 */
s32 Actor_PosInCurrentRoom(Actor *a, f32 *out) {
    VObject *rooms = gRooms;
    sceVu0FVECTOR own, cur;

    if (!(VCALL(rooms, 0x80, u32 (*)(VObject *, s32, f32 *))(rooms, a->room, own) & 0xFF)) {
        return 0;
    }
    if (!(VCALL(rooms, 0x80, u32 (*)(VObject *, s32, f32 *))(
              rooms, VCALL((VObject *)gProgress, 0xC, s32 (*)(VObject *))((VObject *)gProgress), cur) & 0xFF)) {
        return 0;
    }
    sceVu0SubVector(out, own, cur);
    sceVu0AddVector(out, out, a->pos);
    return 1;
}

/* Play a sound at `pos` (default: the actor's position), unless the actor is silent. */
/* 0x00122C20 */
void Actor_PlaySound(Actor *a, s32 id, s32 arg2, s32 arg3, s32 arg4, const f32 *pos) {
    if (a->unk2C == 1) {
        return;
    }
    Sound_PlayBankAt(gSound, id, arg2, pos != NULL ? pos : a->pos, arg3, arg4);
}

#define NAV_REACHED ((u32)-1)
#define NAV_FAIL ((u32)-2)
#define NAV_BLOCKED ((u32)-3)
#define NAV_STUCK ((u32)-4)

/* NavMesh vtable +0x20: where the segment a->b leaves triangle `tri`: 0..2 = across that edge,
 * 3 = b is inside the triangle, 4 = through a corner. */
#define NAV_EDGE_INSIDE 3
#define NAV_EDGE_CORNER 4
static inline s32 NavMesh_Exit(NavMesh *nm, u32 tri, const f32 *a, const f32 *b) {
    return VCALL(nm, 0x20, s32 (*)(NavMesh *, u32, const f32 *, const f32 *))(nm, tri, a, b);
}

/* Index of the corner of `t` that lies at `p` (distance^2 < 0.01), or -1. */
static inline s32 NavTri_CornerAt(NavTri *t, const f32 *p) {
    s32 i;

    for (i = 0; i < 3; i++) {
        sceVu0FVECTOR d;

        sceVu0SubVector(d, t->v[i], p);
        d[3] = 0.0f;
        if (sceVu0InnerProduct(d, d) < 0x1.47ae14p-7f /* 0.01 */) {
            return i;
        }
    }
    return -1;
}

/* The neighbour of `tri` (not blocked by `mask`) that has a corner at `p`, or -1. */
/* 0x00122E80 */
u32 Actor_NeighbourWithCorner(void *self, u32 tri, const f32 *p, u32 mask) {
    NavMesh *nm = gNavMesh;
    sceVu0FVECTOR tmp;
    NavTri *t;
    s32 i;

    if (tri >= nm->numTris) {
        return NAV_NONE;
    }
    VCALL(nm, 0xC, void (*)(NavMesh *, u32, f32 *))(nm, tri, tmp);
    t = NavMesh_Tri(nm, tri);
    for (i = 0; i < 3; i++) {
        u32 n = t->adj[i];

        if (n == NAV_NONE || (mask & NavMesh_TriFlags(nm, n))) {
            continue;
        }
        if (NavTri_CornerAt(NavMesh_Tri(nm, n), p) >= 0) {
            return n;
        }
    }
    return NAV_NONE;
}

/* Walk the nav mesh from triangle `from` at `fromPos` towards `to` at `toPos`, stopping at
 * triangles whose flags match `mask`. Returns the blocking triangle, or NAV_REACHED,
 * NAV_BLOCKED (left the mesh), NAV_STUCK (ended in another triangle), NAV_FAIL. */
/* 0x00123080 */
u32 Actor_WalkMesh(void *self, u32 from, u32 to, const f32 *fromPos, const f32 *toPos, u32 mask) {
    NavMesh *nm = gNavMesh;
    u32 cur, toAlt;

    if (from >= nm->numTris || to >= nm->numTris) {
        return NAV_FAIL;
    }
    /* starting exactly on a corner: the walk may begin in the neighbour sharing it */
    cur = from;
    if (NavTri_CornerAt(NavMesh_Tri(nm, from), fromPos) >= 0
        && NavMesh_Exit(nm, from, fromPos, toPos) == NAV_EDGE_CORNER) {
        cur = Actor_NeighbourWithCorner(self, from, fromPos, mask);
    }
    /* ending on a corner: the neighbour sharing it counts as the destination too */
    toAlt = to;
    if (to < nm->numTris && NavTri_CornerAt(NavMesh_Tri(nm, to), toPos) >= 0) {
        toAlt = Actor_NeighbourWithCorner(self, to, toPos, mask);
    }
    if (cur == NAV_NONE) {
        cur = from;
    }
    for (;;) {
        NavTri *t = NavMesh_Tri(nm, cur);
        s32 e;

        if (t->flags & mask) {
            return cur;
        }
        e = NavMesh_Exit(nm, cur, fromPos, toPos);
        if (e == NAV_EDGE_INSIDE) {
            return (cur == to || cur == toAlt) ? NAV_REACHED : NAV_STUCK;
        }
        if (e == NAV_EDGE_CORNER) {
            if (cur == to) {
                to = toAlt;
            } else if (cur != toAlt) {
                return NAV_FAIL;
            }
            return NavMesh_Exit(nm, to, fromPos, toPos) == NAV_EDGE_INSIDE ? NAV_REACHED : NAV_FAIL;
        }
        cur = t->adj[e];
        if (cur == NAV_NONE) {
            return NAV_BLOCKED;
        }
    }
}

/* Can one walk between the two points in both directions (default blocking mask 0x40080)? */
/* 0x00122C90 */
s32 Actor_CanWalkBetween(void *self, u32 triA, u32 triB, const f32 *posA, const f32 *posB, u32 mask) {
    u32 walk;
    u32 r, r2;

    if (mask == 0) {
        mask = 0x40080;
    }
    walk = mask | 0x4000;
    r = Actor_WalkMesh(self, triA, triB, posA, posB, walk);
    if (r == NAV_REACHED) {
        return 1;
    }
    if (r == NAV_BLOCKED) {
        return 0;
    }
    if (r != NAV_STUCK && (mask & NavMesh_TriFlags(gNavMesh, r))) {
        return 0;
    }
    r2 = Actor_WalkMesh(self, triB, triA, posB, posA, walk);
    if (r2 == NAV_REACHED) {
        return 0;
    }
    if (r2 == NAV_STUCK) {
        return r != NAV_STUCK;
    }
    if (r2 == NAV_BLOCKED) {
        return 0;
    }
    return (mask & NavMesh_TriFlags(gNavMesh, r2)) ? 0 : 1;
}

#define NAV_NO_STAND 0x80001     /* triangle flags where nothing may stand */

/* Is triangle `tri` free: standable, and not claimed by any of gDoors's 8 entries or the
 * nav mesh's extra regions (vtable +0x50)? (The flags read goes through a NULL triangle for an
 * out-of-range index, like the original.) */
/* 0x00123470 */
s32 Actor_TriFree(void *self, u32 tri, s32 arg) {
    NavMesh *nm = gNavMesh;
    VObject *obj;
    u32 n;
    u8 i;

    if (NavMesh_Tri(nm, tri)->flags & NAV_NO_STAND) {
        return 0;
    }
    obj = gDoors;
    for (i = 0; i < 8; i++) {
        if ((VCALL(obj, 0x2C, u32 (*)(VObject *, u32, s32))(obj, i, arg) & 0xFF) == 1) {
            return 0;
        }
    }
    n = nm->numDoors;
    for (i = 0; i < n; i++) {
        if ((VCALL(nm, 0x50, u32 (*)(NavMesh *, u32, s32))(nm, i, arg) & 0xFF) == 1) {
            return 0;
        }
    }
    return 1;
}

/* Same test for an actor's current triangle, using the progress flags for its slot. */
/* 0x001235C0 */
s32 Actor_TriFreeFor(void *self, Actor *a) {
    NavMesh *nm = gNavMesh;
    VObject *prog;
    u32 n;
    u8 i;

    if (NavMesh_Tri(nm, a->navTri)->flags & NAV_NO_STAND) {
        return 0;
    }
    prog = (VObject *)gProgress;
    for (i = 0; i < 8; i++) {
        if (PursuerGroup_Fields((Progress *)prog, i, *(u8 *)&a->slot) & 0x20) {
            return 0;
        }
    }
    n = nm->numDoors;
    for (i = 0; i < n; i++) {
        if (RoomSlots_Bytes((Progress *)prog, i, *(u8 *)&a->slot) & 0x8) {
            return 0;
        }
    }
    return 1;
}

/* Point in front of door `door` on `side` (0/1), 5 units out and shifted by `ofs` in the door's
 * frame; walks the mesh from the door to it. Returns its triangle (point in `out`), or -1. */
/* 0x00123710 */
u32 Actor_DoorFront(void *self, s32 door, s32 side, const f32 *ofs, f32 *out) {
    NavMesh *nm;
    sceVu0FMATRIX m;
    sceVu0FVECTOR d, base, target, cur;
    u32 tri;

    if (!(door >= 0 && (u32)door < gNavMesh->numDoors)) {
        return NAV_NONE;
    }
    if (side < 0 || side >= 2) {
        return NAV_NONE;
    }
    d[0] = -ofs[0];
    *(s32 *)&d[1] = 0;
    if (side == 0) {
        d[2] = 5.0f + ofs[2];
    } else {
        d[2] = -(ofs[2] - 5.0f);
    }
    sceVu0UnitMatrix(m);
    nm = gNavMesh;
    sceVu0RotMatrixY(m, m, VCALL(nm, 0x58, f32 (*)(NavMesh *, s32, s32))(nm, door, side));
    sceVu0ApplyMatrix(d, m, d);
    tri = VCALL(nm, 0x5C, u32 (*)(NavMesh *, s32, s32, f32 *))(nm, door, side, base);
    sceVu0AddVector(target, base, d);
    sceVu0CopyVector(cur, base);
    for (;;) {
        s32 e = NavMesh_Exit(nm, tri, cur, target);

        if (e == NAV_EDGE_INSIDE) {
            sceVu0CopyVector(out, target);
            return tri;
        }
        if (e == NAV_EDGE_CORNER) {
            return NAV_NONE;
        }
        tri = NavMesh_Tri(nm, tri)->adj[e];
        if (tri == NAV_NONE) {
            return NAV_NONE;
        }
        VCALL(nm, 0xC, void (*)(NavMesh *, u32, f32 *))(nm, tri, cur);
    }
}

/* Is the actor facing the same way as door `door`'s `side` (positive dot of the forward axes)? */
/* 0x00123960 */
s32 Actor_FacingDoor(Actor *a, s32 door, s32 side) {
    sceVu0FMATRIX m;
    sceVu0FVECTOR fwd, dir;

    if (!(door >= 0 && (u32)door < gNavMesh->numDoors)) {
        return 0;
    }
    if (side < 0 || side >= 2) {
        return 0;
    }
    *(s32 *)&dir[0] = 0;
    *(s32 *)&dir[1] = 0;
    dir[2] = 1.0f;
    sceVu0ApplyMatrix(fwd, a->rot, dir);
    sceVu0UnitMatrix(m);
    sceVu0RotMatrixY(m, m, VCALL(gNavMesh, 0x58, f32 (*)(NavMesh *, s32, s32))(gNavMesh, door, side));
    sceVu0ApplyMatrix(dir, m, dir);
    return !(sceVu0InnerProduct(fwd, dir) <= 0.0f);
}

/* Free distance from `pos` (in triangle `tri`) along heading `angle`, up to `dist`: walks the
 * mesh until the ray leaves it or enters a triangle blocked by `mask` (-1 = the actor's own
 * mask). Horizontal distance to that edge, `dist` if unobstructed, 0 through a corner. */
/* 0x00123A70 */
f32 Actor_FreeDistance(Actor *a, u32 tri, const f32 *pos, u32 mask, f32 angle, f32 dist) {
    NavMesh *nm;
    sceVu0FMATRIX m;
    sceVu0FVECTOR d, target, hit;

    if (mask == NAV_NONE) {
        mask = a->navMask;
    }
    sceVu0UnitMatrix(m);
    sceVu0RotMatrixY(m, m, angle);
    *(s32 *)&d[0] = 0;
    d[2] = dist;
    *(s32 *)&d[1] = 0;
    sceVu0ApplyMatrix(d, m, d);
    sceVu0AddVector(target, pos, d);
    nm = gNavMesh;
    for (;;) {
        /* +0x24: like +0x20, also returns the exit point */
        s32 e = VCALL(nm, 0x24, s32 (*)(NavMesh *, u32, f32 *, const f32 *, const f32 *))(
            nm, tri, hit, pos, target);

        if (e == NAV_EDGE_INSIDE) {
            return dist;
        }
        if (e == NAV_EDGE_CORNER) {
            return 0.0f;
        }
        tri = NavMesh_Tri(nm, tri)->adj[e];
        if (tri == NAV_NONE || (NavMesh_Tri(nm, tri)->flags & mask)) {
            break;
        }
    }
    sceVu0SubVector(d, hit, pos);
    return __builtin_sqrtf(d[2] * d[2] + d[0] * d[0]);
}

/* Is the actor nearer to room `room`'s origin than `pos` is? */
/* 0x00123C60 */
s32 Actor_NearerRoom(Actor *a, s32 room, const f32 *pos) {
    sceVu0FVECTOR origin, da, dp;
    f32 la;

    VCALL(gRooms, 0x30, void (*)(VObject *, s32, f32 *))(gRooms, room, origin);
    sceVu0SubVector(da, a->pos, origin);
    sceVu0SubVector(dp, pos, origin);
    la = sceVu0InnerProduct(da, da);
    return la < sceVu0InnerProduct(dp, dp);
}

/* vtable +0x3C (base): always true */
/* 0x00123D00 */
s32 Actor_CanAct(Actor *a) {
    return 1;
}

/* vtable +0x34 (base): nothing */
/* 0x00123D10 */
void Actor_LeaveScreen(Actor *a) {
}

/* Triangle containing `p`, found by walking from the actor's triangle toward it; -1 if the
 * walk leaves the mesh or passes a corner. */
/* 0x00123D20 */
u32 Actor_TriOf(Actor *a, const f32 *p) {
    NavMesh *nm = gNavMesh;
    u32 tri = a->navTri;

    for (;;) {
        s32 e = NavMesh_Exit(nm, tri, a->pos, p);

        if (e == NAV_EDGE_INSIDE) {
            return tri;
        }
        if (e == NAV_EDGE_CORNER) {
            return NAV_NONE;
        }
        tri = NavMesh_Tri(nm, tri)->adj[e];
        if (tri == NAV_NONE) {
            return NAV_NONE;
        }
    }
}

/* Same, and put `p` on the mesh: its height from the triangle (vtable +0x14), or the actor's
 * height if it isn't on the mesh. */
/* 0x00123E20 */
u32 Actor_TriOfOnMesh(Actor *a, f32 *p) {
    NavMesh *nm = gNavMesh;
    u32 tri = a->navTri;

    for (;;) {
        s32 e = NavMesh_Exit(nm, tri, a->pos, p);

        if (e == NAV_EDGE_INSIDE) {
            VCALL(nm, 0x14, void (*)(NavMesh *, u32, f32 *))(nm, tri, p);
            return tri;
        }
        if (e == NAV_EDGE_CORNER) {
            tri = NAV_NONE;
            break;
        }
        tri = NavMesh_Tri(nm, tri)->adj[e];
        if (tri == NAV_NONE) {
            break;
        }
    }
    p[1] = a->pos[1];
    return tri;
}

/* vtable +0x30 (base): nothing */
/* 0x00123F50 */
void Actor_Update(Actor *a) {
}

/* vtable +0x2C (base): nothing */
/* 0x00123F60 */
void Actor_LightChange(Actor *a) {
}

/* Triangle containing `target`, walking from triangle `tri` at `from`; -1 if the walk leaves
 * the mesh, passes a corner or enters a triangle blocked by `mask` (-1 = the actor's mask). */
/* 0x00124320 */
u32 Actor_TriFrom(Actor *a, const f32 *target, u32 tri, const f32 *from, u32 mask) {
    NavMesh *nm;

    if (mask == NAV_NONE) {
        mask = a->navMask;
    }
    nm = gNavMesh;
    for (;;) {
        s32 e = NavMesh_Exit(nm, tri, from, target);

        if (e == NAV_EDGE_INSIDE) {
            return tri;
        }
        if (e == NAV_EDGE_CORNER) {
            return NAV_NONE;
        }
        tri = NavMesh_Tri(nm, tri)->adj[e];
        if (tri == NAV_NONE || (NavMesh_Tri(nm, tri)->flags & mask)) {
            return NAV_NONE;
        }
    }
}

/* Move to `target` (now in triangle `tri`), on the mesh surface. */
static inline void Actor_PlaceAt(Actor *a, u32 tri, const f32 *target) {
    a->navTri = tri;
    sceVu0CopyVector(a->pos, target);
    VCALL(gNavMesh, 0x14, void (*)(NavMesh *, u32, f32 *))(gNavMesh, a->navTri, a->pos);
}

/* Push this actor out of `other`'s collision cylinder. If the straight push is blocked, try
 * directions rotated 2, 4, ... 180 degrees either way. 0 on success, -1 if nowhere fits. */
/* 0x00123F70 */
s32 Actor_PushOut(Actor *a, Actor *other) {
    sceVu0FMATRIX rotNeg, rotPos;
    sceVu0FVECTOR dir, target, left, right;
    f32 dist = 0x1.47ae14p-7f /* 0.01 */ + (a->radius + other->radius);
    u32 tri;
    s32 deg;

    sceVu0SubVector(dir, a->pos, other->pos);
    *(s32 *)&dir[1] = 0;
    sceVu0Normalize(dir, dir);
    sceVu0ScaleVector(dir, dir, dist);
    sceVu0AddVector(target, other->pos, dir);
    target[3] = 1.0f;
    tri = Actor_TriFrom(a, target, a->navTri, a->pos, NAV_NONE);
    if (tri != NAV_NONE) {
        Actor_PlaceAt(a, tri, target);
        return 0;
    }
    sceVu0UnitMatrix(rotNeg);
    sceVu0RotMatrixY(rotNeg, rotNeg, -0x1.1df46ap-5f /* -2 deg */);
    sceVu0UnitMatrix(rotPos);
    sceVu0RotMatrixY(rotPos, rotPos, 0x1.1df46ap-5f /* 2 deg */);
    sceVu0CopyVector(left, dir);
    sceVu0CopyVector(right, dir);
    for (deg = 2; deg < 181; deg += 2) {
        sceVu0ApplyMatrix(left, rotNeg, left);
        sceVu0AddVector(target, other->pos, left);
        target[3] = 1.0f;
        tri = Actor_TriFrom(a, target, a->navTri, a->pos, NAV_NONE);
        if (tri != NAV_NONE) {
            Actor_PlaceAt(a, tri, target);
            return 0;
        }
        sceVu0ApplyMatrix(right, rotPos, right);
        sceVu0AddVector(target, other->pos, right);
        target[3] = 1.0f;
        tri = Actor_TriFrom(a, target, a->navTri, a->pos, NAV_NONE);
        if (tri != NAV_NONE) {
            Actor_PlaceAt(a, tri, target);
            return 0;
        }
    }
    return -1;
}

/* Are the two actors touching: both enabled, vertically within the lower one's height
 * (+ `vmargin`) and horizontally within their radii (+ `margin`)? */
/* 0x001241F0 */
s32 Actor_Touching(Actor *a, Actor *b, f32 margin, f32 vmargin) {
    sceVu0FVECTOR d;
    f32 r;

    if (!a->active || a->disabled) {
        return 0;
    }
    if (b == NULL || !b->active || b->disabled) {
        return 0;
    }
    if (a->pos[1] < b->pos[1]) {
        if (!(b->pos[1] - a->pos[1] <= a->height + vmargin)) {
            return 0;
        }
    } else if (!(a->pos[1] - b->pos[1] <= b->height + vmargin)) {
        return 0;
    }
    sceVu0SubVector(d, a->pos, b->pos);
    r = a->radius + b->radius;
    return __builtin_sqrtf(d[2] * d[2] + d[0] * d[0]) <= margin + r;
}

/* Triangle containing `target` reached from the actor's position (see Actor_TriFrom). */
/* 0x00124480 */
u32 Actor_TriTo(Actor *a, const f32 *target, u32 mask) {
    return Actor_TriFrom(a, target, a->navTri, a->pos, mask);
}

/* Distance from the actor to `p`. */
/* 0x00124490 */
f32 Actor_Distance(Actor *a, const f32 *p) {
    sceVu0FVECTOR d;

    sceVu0SubVector(d, p, a->pos);
    return __builtin_sqrtf(sceVu0InnerProduct(d, d));
}

/* Heading from the actor to `p` (its current heading if `p` is right above or below it). */
/* 0x001244D0 */
f32 Actor_HeadingTo(Actor *a, const f32 *p) {
    f32 dx = p[0] - a->pos[0];
    f32 dz = p[2] - a->pos[2];

    if (dx == 0.0f && dz == 0.0f) {
        return a->angle[1];
    }
    return func_0031C5C0(dx, dz);
}

/* NavMesh vtable +0x40: slide from `from` toward `to` within the mesh starting in `tri`;
 * the reachable point goes to `out`, returns its triangle or -1. */
static inline u32 NavMesh_Slide(NavMesh *nm, u32 tri, f32 *out, const f32 *from, const f32 *to, u32 mask) {
    return VCALL(nm, 0x40, u32 (*)(NavMesh *, u32, f32 *, const f32 *, const f32 *, u32))(
        nm, tri, out, from, to, mask);
}

/* Move by `delta` horizontally as far as the mesh allows (any triangle), and by delta y. */
/* 0x00124720 */
void Actor_MoveAny(Actor *a, const f32 *delta) {
    sceVu0FVECTOR to, out;
    u32 tri;

    sceVu0CopyVector(to, a->pos);
    to[0] += delta[0];
    to[2] += delta[2];
    tri = NavMesh_Slide(gNavMesh, a->navTri, out, a->pos, to, 0);
    if (tri != NAV_NONE) {
        a->navTri = tri;
        a->pos[0] = out[0];
        a->pos[2] = out[2];
        a->pos[1] += delta[1];
    }
}

/* Move by `delta` horizontally as far as the actor's blocking mask allows. */
/* 0x001247E0 */
void Actor_Move(Actor *a, const f32 *delta) {
    sceVu0FVECTOR to, out;
    u32 tri;

    sceVu0CopyVector(to, a->pos);
    to[0] += delta[0];
    to[2] += delta[2];
    tri = NavMesh_Slide(gNavMesh, a->navTri, out, a->pos, to, a->navMask);
    if (tri != NAV_NONE) {
        a->navTri = tri;
        sceVu0CopyVector(a->pos, out);
    }
}

#define F_PI 0x1.921fb6p+1f       /* 0x40490FDB */
#define F_2PI 0x1.921fb6p+2f      /* 0x40C90FDB */

/* Angle wrapped into -pi..pi (one turn at most). */
static inline f32 WrapAngle(f32 x) {
    if (!(x <= F_PI)) {
        return x - F_2PI;
    }
    if (x < -F_PI) {
        return x + F_2PI;
    }
    return x;
}

/* Turn toward heading `target` by at most `step`; updates the rotation matrix. Returns how far
 * the heading still is from the target (0 when reached). */
/* 0x00124530 */
f32 Actor_TurnToward(Actor *a, f32 target, f32 step) {
    f32 yaw = a->angle[1];
    f32 d = WrapAngle(target - yaw);
    f32 ad = (d <= 0.0f) ? -d : d;
    f32 rest;

    if (ad <= step) {
        yaw = target;
        rest = 0.0f;
    } else {
        yaw = WrapAngle((d <= 0.0f) ? yaw - step : yaw + step);
        rest = WrapAngle(target - yaw);
    }
    a->angle[1] = yaw;
    sceVu0UnitMatrix(a->rot);
    sceVu0RotMatrixY(a->rot, a->rot, yaw);
    return (rest <= 0.0f) ? -rest : rest;
}

/* Teleport to a random free triangle of the current room's mesh (only if the actor is in the
 * current room). `kind` selects the area flags: 0 -> 0x100000, 1 -> 0x200000, else both;
 * for -1 and 2 the triangle must have them, otherwise it must not. Placed via vtable +0x28. */
/* 0x00124890 */
void Actor_TeleportRandom(Actor *a, s32 kind) {
    NavMesh *nm;
    VObject *rng, *rooms, *objs;
    u32 area, n;

    if (a->room != VCALL((VObject *)gProgress, 0xC, s32 (*)(VObject *))((VObject *)gProgress)) {
        return;
    }
    if (kind == 1) {
        area = 0x200000;
    } else if (kind == 0) {
        area = 0x100000;
    } else {
        area = 0x300000;
    }
    nm = gNavMesh;
    rng = gRandom;
    n = nm->numTris;
    rooms = gRooms;
    objs = gEvents;
    for (;;) {
        sceVu0FVECTOR center;
        u32 tri, flags;
        u8 free, i;

        tri = (u32)((f32)n * VCALL(rng, 0x1C, f32 (*)(VObject *))(rng));
        flags = NavMesh_Tri(nm, tri)->flags;
        if (flags & a->navMask) {
            continue;
        }
        if (kind == -1 || kind == 2) {
            if ((area & flags) != area) {
                tri = NAV_NONE;
            }
        } else if (flags & area) {
            tri = NAV_NONE;
        }
        if (tri == NAV_NONE) {
            continue;
        }
        VCALL(nm, 0xC, void (*)(NavMesh *, u32, f32 *))(nm, tri, center);
        free = 1;
        for (i = 0; i < 8; i++) {
            u32 id = VCALL(rooms, 0x48, u32 (*)(VObject *, s32, u32))(rooms, a->room, i) & 0xFFFF;

            if (id != 0xFFFF
                && (VCALL(objs, 0x10, u32 (*)(VObject *, f32 *, u32, u32))(objs, center, id, a->navTri) & 0xFF) == 1) {
                free = 0;
                break;
            }
        }
        if (free == 1) {
            VCALL(a, 0x28, void (*)(Actor *, u32, s32, s32))(a, tri, 0, 0);
            return;
        }
    }
}

/* vtable +0x28: place the actor in triangle `tri`, optionally turning to `*heading` and moving
 * to `pos` (which must lie in the triangle; it is put on the surface), else to the triangle's
 * centre. 0, or -1 if the triangle is invalid, blocked or doesn't contain `pos`. */
/* 0x00124B80 */
s32 Actor_Place(Actor *a, u32 tri, const f32 *heading, f32 *pos) {
    NavMesh *nm = gNavMesh;

    a->prevNavTri = tri;
    a->navTri = tri;
    if (tri >= nm->numTris) {
        return -1;
    }
    if (NavMesh_Tri(nm, tri)->flags & a->navMask) {
        return -1;
    }
    if (heading != NULL) {
        f32 yaw = *heading;

        a->angle[1] = yaw;
        sceVu0UnitMatrix(a->rot);
        sceVu0RotMatrixY(a->rot, a->rot, yaw);
    }
    if (pos != NULL) {
        sceVu0CopyVector(a->pos, pos);
        if (VCALL(nm, 0x10, s32 (*)(NavMesh *, u32, const f32 *))(nm, tri, pos) != NAV_EDGE_INSIDE) {
            return -1;
        }
        VCALL(nm, 0x14, void (*)(NavMesh *, u32, f32 *))(nm, tri, pos);
    } else {
        VCALL(nm, 0xC, void (*)(NavMesh *, u32, f32 *))(nm, tri, a->pos);
    }
    sceVu0CopyVector(a->prevPos, a->pos);
    a->disabled = 0;
    a->unk2D = 0;
    a->unk2B = 0;
    return 0;
}

/* vtable +0x24: remember the current position and triangle as the previous ones. */
/* 0x00124D00 */
void Actor_RememberPos(Actor *a) {
    a->prevNavTri = a->navTri;
    func_0010E5F0(a->prevPos, a->pos);
}

/* vtable +0x20 (base): nothing */
/* 0x00124D20 */
void Actor_Unload(Actor *a) {
}

/* vtable +0x1C (base): nothing */
/* 0x00124D30 */
void Actor_FilesLoaded(Actor *a) {
}

/* Has the actor's data file (id flags24 | slot) finished loading? */
/* 0x00124D40 */
s32 Actor_DataLoaded(Actor *a) {
    return VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, a->flags24 | a->slot) == 2;
}

/* vtable +0x18 (base): nothing */
/* 0x00124D80 */
void Actor_FilesLoading(Actor *a) {
}

/* vtable +0x14 (base): nothing */
/* 0x00124D90 */
void Actor_LoadFiles(Actor *a) {
}

/* vtable +0x10 (base): nothing */
/* 0x00124DA0 */
void Actor_Cleanup(Actor *a) {
}

/* vtable +0xC: reset position, orientation and state. */
/* 0x00124DB0 */
void Actor_Reset(Actor *a) {
    a->navTri = NAV_NONE;
    a->pos[2] = 0.0f;
    a->pos[1] = 0.0f;
    a->pos[0] = 0.0f;
    a->pos[3] = 1.0f;
    a->prevPos[3] = 1.0f;
    a->angle[0] = 0.0f;
    a->angle[1] = 0.0f;
    a->angle[2] = 0.0f;
    a->angle[3] = 1.0f;
    a->unkB0[2] = 0.0f;
    a->unkB0[1] = 0.0f;
    a->unkB0[0] = 0.0f;
    a->unkB0[3] = 1.0f;
    sceVu0UnitMatrix(a->rot);
    a->unkD0 = 0;
    a->unkD1 = 0;
    a->active = 0;
    a->disabled = 0;
    a->unk2A = 0;
    a->unk2B = 0;
    a->unk2C = 0;
    a->unk2D = 0;
}

/* 0x00124E40 */
void Actor_Destroy(Actor *a) {
}

/* 0x00124E50 */
void *Actor_new(void *a, void *b) {
    return b;
}

extern void *D_00469C20[];   /* Actor base vtable */
extern void *D_00469C60[];   /* Actor vtable */

/* vtable +0x8 (0x469C60): destructor. Actors live inside their scene, so "delete" does nothing. */
/* 0x00124E60 */
Actor *Character_dtor(Actor *a, s32 flags) {
    if (a != NULL) {
        a->vtbl = D_00469C60;
        a->vtbl = D_00469C20;
        if ((s16)flags > 0) {
            Actor_Destroy(a);
        }
    }
    return a;
}

/* 0x00124EC0 */
s32 Character_LoadMessage(Actor *a) {
    return 0;
}

/* 0x00124F10 */
s32 Character_IsBusy(Actor *a) {
    return 0;
}

/* vtable +0x38 (base): per-frame update - nothing for a plain actor */
/* 0x00120F80 */
void Actor_ShowUp(Actor *a) {
}

extern VObject *gSceneGameF29740;   /* path planner */

/* Room manager exits (8 per room). */
#define Room_ExitId(rooms, room, i) VCALL(rooms, 0x40, u32 (*)(VObject *, s32, u32))(rooms, room, i)
#define Room_ExitFlag(rooms, room, i) VCALL(rooms, 0x74, u32 (*)(VObject *, s32, u32))(rooms, room, i)
#define Room_ExitSide(rooms, room, i) VCALL(rooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(rooms, room, i, 0)
#define Room_ExitPos(rooms, i, out) VCALL(rooms, 0x34, u32 (*)(VObject *, u32, f32 *))(rooms, i, out)

/* Ask the path planner for a path from the character to `goal` in triangle `goalTri`;
 * returns its length (<= 0: no path). */
static inline s32 Character_PlanPath(Character *c, VObject *planner, u32 goalTri, const f32 *goal) {
    c->pathReq->unk0 = 0;
    c->pathReq->startTri = c->a.navTri;
    sceVu0CopyVector(c->pathReq->startPos, c->a.pos);
    c->pathReq->goalTri = goalTri;
    sceVu0CopyVector(c->pathReq->goalPos, goal);
    c->pathId = VCALL(planner, 0xC, s32 (*)(VObject *, PathRequest *, s32))(planner, c->pathReq, 0);
    if (c->pathId == -1) {
        return -1;
    }
    return VCALL(planner, 0x14, s32 (*)(VObject *))(planner);
}

/* Choose the room exit to head for (`door` != 0xFF: just set it). Standing in a marked area
 * (nav flags 0x100000 / 0x200000), take an exit on that side; else keep the current exit if a
 * path leads there, else the first reachable exit, preferring flagged ones (vtable +0x74). */
/* 0x00124F20 */
void Character_ChooseExit(Character *c, u32 door) {
    VObject *rooms, *planner;
    sceVu0FVECTOR goal;
    s32 room, side;
    u32 area, goalTri;
    u8 pick, i;

    if ((door & 0xFF) != 0xFF) {
        c->door = door;
        return;
    }
    room = c->a.room;
    if (room != VCALL((VObject *)gProgress, 0xC, s32 (*)(VObject *))((VObject *)gProgress) || c->a.navTri == NAV_NONE) {
        return;
    }
    area = NavMesh_TriFlags(gNavMesh, c->a.navTri) & 0x300000;   /* (off the mesh: the PS2 reads address 0x3C) */
    side = -1;
    if (area == 0 || area == 0x200000) {
        side = 0;
    } else if (area == 0x100000) {
        side = 1;
    }
    pick = 0xFF;
    if (side != -1) {
        if (c->door != 0xFF && side == Room_ExitSide(gRooms, room, c->door)) {
            return;
        }
        rooms = gRooms;
        for (i = 0; i < 8; i++) {
            if (i != c->door && side == Room_ExitSide(rooms, c->a.room, i)) {
                pick = i;
                if ((Room_ExitFlag(rooms, c->a.room, i) & 0xFF) == 1) {
                    break;
                }
            }
        }
        if (pick != 0xFF) {
            c->door = pick;
            return;
        }
    }
    if (c->a.room == 0x17) {
        return;
    }
    if (c->door != 0xFF) {
        rooms = gRooms;
        /* (the original compares the low byte with 0xFFFF: always true) */
        if ((Room_ExitId(rooms, c->a.room, c->door) & 0xFF) != 0xFFFF) {
            goalTri = Room_ExitPos(rooms, c->door, goal);
            if (goalTri != NAV_NONE && Character_PlanPath(c, gSceneGameF29740, goalTri, goal) > 0) {
                return;
            }
        }
    }
    rooms = gRooms;
    pick = 0xFF;
    planner = gSceneGameF29740;
    for (i = 0; i < 8; i++) {
        if (i != c->door && (Room_ExitId(rooms, c->a.room, i) & 0xFF) != 0xFFFF
            && (Room_ExitFlag(rooms, c->a.room, i) & 0xFF) == 1) {
            pick = i;
            goalTri = Room_ExitPos(rooms, i, goal);
            if (goalTri != NAV_NONE && Character_PlanPath(c, planner, goalTri, goal) > 0) {
                c->door = i;
                return;
            }
        }
    }
    for (i = 0; i < 8; i++) {
        if (i != c->door && (Room_ExitId(rooms, c->a.room, i) & 0xFF) != 0xFFFF
            && !(Room_ExitFlag(rooms, c->a.room, i) & 0xFF)) {
            if (pick == 0xFF) {
                pick = i;
            }
            goalTri = Room_ExitPos(rooms, i, goal);
            if (goalTri != NAV_NONE && Character_PlanPath(c, planner, goalTri, goal) > 0) {
                c->door = i;
                return;
            }
        }
    }
    c->door = pick;
}

/* vtable +0x80: screen fade by how far the character is past the boundary of the first region
 * flagged for its slot (progress flag bit 0): alpha 0..128 over half the boundary length. */
/* 0x001254B0 */
void Character_RegionFade(Character *c) {
    VObject *rooms, *objs;
    sceVu0FVECTOR a, b, fwd, p0, p1, p2, e, n, q;
    f32 d, ad, half, t;
    u32 color;
    u8 i;

    for (i = 0; i < 8; i++) {
        if (PursuerGroup_Fields((Progress *)((VObject *)gProgress), i, *(u8 *)&c->a.slot) & 0x1) {
            break;
        }
    }
    if (i == 8) {
        c->unk152C = 10;
        return;
    }
    rooms = gRooms;
    VCALL(rooms, 0x2C, void (*)(VObject *, u32, f32 *))(rooms, i, a);
    VCALL(rooms, 0x30, void (*)(VObject *, u32, f32 *))(rooms, i, b);
    sceVu0SubVector(fwd, b, a);
    *(s32 *)&fwd[1] = 0;
    sceVu0Normalize(fwd, fwd);
    objs = gEvents;
    VCALL(objs, 0x24, void (*)(VObject *, u32, f32 *, f32 *, f32 *))(objs, i, p0, p1, p2);
    sceVu0SubVector(e, p1, p0);
    *(s32 *)&e[1] = 0;
    sceVu0Normalize(n, e);
    d = sceVu0InnerProduct(fwd, n);
    ad = (d <= 0.0f) ? -d : d;
    if (ad < 0x1.6a09e6p-1f /* 0.70710677 */) {
        sceVu0SubVector(e, p1, p2);
        *(s32 *)&e[1] = 0;
        sceVu0Normalize(n, e);
        d = sceVu0InnerProduct(fwd, n);
    }
    if (d < 0.0f) {
        sceVu0ScaleVector(n, n, -1.0f);
    }
    half = 0.5f * __builtin_sqrtf(e[2] * e[2] + e[0] * e[0]);
    VCALL(objs, 0x20, void (*)(VObject *, u32, f32 *))(objs, i, q);
    sceVu0SubVector(e, q, c->a.pos);
    t = sceVu0InnerProduct(e, n);
    c->unk152C = 15;
    if (t <= 0.0f) {
        color = 0x808080;
    } else {
        u32 alpha = (u32)(128.0f * (t / half));

        if (alpha > 0x80) {
            alpha = 0x80;
        }
        color = (alpha << 24) | 0x808080;
    }
    VCALL(gRenderer, 0x70, void (*)(VObject *, u32))(gRenderer, color);
}

/* Path length from the character to `goal` (in triangle `goalTri`), avoiding triangles with
 * `mask` (-1: the request's current mask); -1.0 if there is no path. */
/* 0x001257B0 */
f32 Character_PathLength(Character *c, u32 goalTri, const f32 *goal, u32 mask) {
    VObject *planner;
    u32 saved = c->pathReq->mask;
    s32 n;

    c->pathReq->mask = (mask == NAV_NONE) ? saved : mask;
    c->pathReq->unk0 = 0;
    c->pathReq->startTri = c->a.navTri;
    sceVu0CopyVector(c->pathReq->startPos, c->a.pos);
    c->pathReq->goalTri = goalTri;
    sceVu0CopyVector(c->pathReq->goalPos, goal);
    planner = gSceneGameF29740;
    c->pathId = VCALL(planner, 0xC, s32 (*)(VObject *, PathRequest *, s32))(planner, c->pathReq, 0);
    n = -1;
    if (c->pathId != -1) {
        n = VCALL(planner, 0x14, s32 (*)(VObject *))(planner);
    }
    c->pathReq->mask = saved;
    if (n <= 0) {
        return -1.0f;
    }
    return VCALL(planner, 0x40, f32 (*)(VObject *, s32))(planner, c->pathId);
}

/* 0x001258D0 */
void Character_BackToStance(Character *c) {
}

/* 0x001258E0 */
void Character_LeftBehind(Character *c) {
}

/* 0x001258F0 */
s32 Character_AttackPoint(Character *c) {
    return 0;
}

/* Turn by the animation's root rotation. */
static inline void Character_ApplyRootTurn(Character *c) {
    f32 yaw = Angle_Wrap(c->a.angle[1] + Motion_RootRotation(c->motion, 0.0f));

    c->a.angle[1] = yaw;
    sceVu0UnitMatrix(c->a.rot);
    sceVu0RotMatrixY(c->a.rot, c->a.rot, yaw);
}

/* Apply the animation's root rotation. */
/* 0x00125900 */
void Character_RootTurn(Character *c) {
    Character_ApplyRootTurn(c);
}

/* Apply the animation's root motion; unk2B set: move ignoring the blocking mask. */
/* 0x00125960 */
void Character_RootMove(Character *c) {
    sceVu0FVECTOR d;

    Motion_RootMovement(c->motion, d, 0.0f);
    Character_ApplyRootTurn(c);
    sceVu0ApplyMatrix(d, c->a.rot, d);
    if (!c->a.unk2B) {
        Actor_Move(&c->a, d);
    } else {
        Actor_MoveAny(&c->a, d);
    }
}

/* Apply the animation's root motion (always within the blocking mask). */
/* 0x00125A10 */
void Character_RootMoveMasked(Character *c) {
    sceVu0FVECTOR d;

    Motion_RootMovement(c->motion, d, 0.0f);
    Character_ApplyRootTurn(c);
    sceVu0ApplyMatrix(d, c->a.rot, d);
    Actor_Move(&c->a, d);
}

/* 0x00125AA0 */
void Character_LoadState(Character *c) {
}

/* 0x00125AB0 */
void Character_SaveState(Character *c) {
}

/* 0x00125AC0 */
s32 Character_EventConcerns(Character *c) {
    return 1;
}

/* Drop the current path. */
static inline void Character_CancelPath(Character *c) {
    VCALL(gSceneGameF29740, 0x28, void (*)(VObject *, s32))(gSceneGameF29740, c->pathId);
    c->pathId = -1;
    c->unk128 = 0;
    c->unk124 = 0;
}

/* vtable +0x98: true for characters whose progress entry follows them (see gProgress +0x34) */
#define Character_Tracked(c) ((VCALL(c, 0x98, u32 (*)(Character *))(c) & 0xFF) == 1)

/* vtable +0x28 (Character): place, then drop any path and reset the state blocks. */
/* 0x00125AD0 */
s32 Character_Place(Character *c, u32 tri, const f32 *heading, f32 *pos) {
    s32 r = Actor_Place(&c->a, tri, heading, pos);

    Character_CancelPath(c);
    c->unk14D0 = 0;
    if (c->state[0] != 5) {
        func_002A8410((u8 *)c->state);
    }
    func_002A8410((u8 *)c->state2);
    if (Character_Tracked(c)) {
        VCALL((VObject *)gProgress, 0x34, void (*)(VObject *, u32))((VObject *)gProgress, *(u8 *)&c->a.slot);
    }
    return r;
}

/* Move to room `room` (vtable +0x5C notifies first; the other arguments are passed on). */
/* 0x00125BA0 */
s32 Character_ToRoom(Character *c, s32 room, s32 a2, s32 a3) {
    VCALL(c, 0x5C, void (*)(Character *, s32, s32, s32))(c, room, a2, a3);
    c->a.room = room;
    return 0;
}

/* Forget the path and movement state. */
/* 0x00125BE0 */
void Character_ResetBehaviour(Character *c) {
    s32 i;

    c->unk14C0 = 0xFFFF;
    c->unk14C4 = 0;
    Character_CancelPath(c);
    c->unk1388 = 0;
    c->unk1384 = 0;
    for (i = 0; i < 13; i++) {
        c->unk148C[i] = 0;
    }
    func_002A8410((u8 *)c->state);
    func_002A8410((u8 *)c->state2);
    if (Character_Tracked(c)) {
        VCALL((VObject *)gProgress, 0x34, void (*)(VObject *, u32))((VObject *)gProgress, *(u8 *)&c->a.slot);
    }
}

/* Tracked character in a special state (4/5), or flagged by progress for its slot. */
/* 0x00125D80 */
s32 Character_Held(Character *c) {
    if (Character_Tracked(c)) {
        s32 s = c->state[0];

        if (s == 4 || s == 5) {
            return 1;
        }
        if ((Progress_IsLinked((Progress *)((VObject *)gProgress), *(u8 *)&c->a.slot) & 0xFF) == 1) {
            return 1;
        }
    }
    return 0;
}

extern void *D_0046FC30[];
extern void *D_00469D00[];
extern void *D_0046F580[];

/* Destructors of small helper objects (vtables 0x46FC30 -> 0x469D00, 0x469D00, 0x46F580). */
/* 0x00126170 */
void **Helper46FC30_dtor(void **obj, s32 flags) {
    if (obj != NULL) {
        *obj = D_0046FC30;
        *obj = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(obj);
        }
    }
    return obj;
}

/* 0x001261D0 */
void **Helper469D00_dtor(void **obj, s32 flags) {
    if (obj != NULL) {
        *obj = D_00469D00;
        if ((s16)flags > 0) {
            func_00100490(obj);
        }
    }
    return obj;
}

/* 0x00126220 */
void **EffectBase_dtor(void **obj, s32 flags) {
    if (obj != NULL) {
        *obj = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(obj);
        }
    }
    return obj;
}

/* Mark in the progress data which of the room's 8 objects the character stands on
 * (byte 1 of gProgress +0xFD0 + 6*i gets bit `slot`). */
/* 0x00126270 */
void Character_MarkObjects(Character *c) {
    u8 *prog = (u8 *)gProgress;
    VObject *rooms = gRooms;
    VObject *objs = gEvents;
    u32 i;

    for (i = 0; i < 8; i++, prog += 6) {
        u8 *entry = prog + 0xFD0;
        u32 id = VCALL(rooms, 0x48, u32 (*)(VObject *, s32, u32))(rooms, c->a.room, i & 0xFF) & 0xFFFF;

        if (id != 0xFFFF
            && VCALL(objs, 0x10, s32 (*)(VObject *, f32 *, u32, u32))(objs, c->a.pos, id, c->a.navTri) != 0) {
            entry[1] |= (u8)(1 << c->a.slot);
        }
    }
}

/* Full stop: forget path and movement state, clear the flags. */
/* 0x00126360 */
void Character_EventReset(Character *c) {
    s32 i;

    c->unk14C0 = 0xFFFF;
    c->unk14C4 = 0;
    Character_CancelPath(c);
    c->unk1388 = 0;
    c->unk1384 = 0;
    for (i = 0; i < 13; i++) {
        c->unk148C[i] = 0;
    }
    func_002A8410((u8 *)c->state);
    func_002A8410((u8 *)c->state2);
    if (Character_Tracked(c)) {
        VCALL((VObject *)gProgress, 0x34, void (*)(VObject *, u32))((VObject *)gProgress, *(u8 *)&c->a.slot);
    }
    c->unkE0 = 0;
    c->unkE1 = 0;
    c->a.unk2C = 0;
    c->a.unk2D = 0;
    c->a.unk2B = 0;
    c->unkE4 = 1;
}

/* 0x00126450 */
void Character_BackToNormal(Character *c) {
    c->unk14D0 = 0;
    Motion_Unfreeze(c->motion);
    c->unkE0 = 1;
    c->unkE1 = 0;
    c->unkF4 = 0;
    VCALL((VObject *)gProgress, 0x34, void (*)(VObject *, u32))((VObject *)gProgress, *(u8 *)&c->a.slot);
}

/* 0x001264B0 */
void Character_EventCommand(Character *c) {
}

/* Forward to the sound manager. */
/* 0x001264C0 */
void Character_Sound(Character *c, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5) {
    ((void (*)(VObject *, s32, s32, s32, s32, s32))Sound_PlayAt)(gSound, a1, a2, a3, a4, a5);
}

/* 0x001267F0 */
void Character_Set152C(Character *c, s32 v) {
    c->unk152C = v;
}

/* 0x00126800 */
s32 Character_Get152C(Character *c) {
    return c->unk152C;
}

/* Halt: forget path and movement state, stop the animation (motion vtable +0x68). */
/* 0x00126810 */
void Character_Enable(Character *c) {
    s32 i;

    c->unk14C0 = 0xFFFF;
    c->unk14C4 = 0;
    Character_CancelPath(c);
    c->unk1388 = 0;
    c->unk1384 = 0;
    for (i = 0; i < 13; i++) {
        c->unk148C[i] = 0;
    }
    func_002A8410((u8 *)c->state);
    func_002A8410((u8 *)c->state2);
    if (Character_Tracked(c)) {
        VCALL((VObject *)gProgress, 0x34, void (*)(VObject *, u32))((VObject *)gProgress, *(u8 *)&c->a.slot);
    }
    c->unkE2 = 0;
    c->unkE3 = 0;
    c->unkE4 = 1;
    c->a.unk2A = 0;
    c->unkE1 = 0;
    c->unkF4 = 0;
    VCALL(c->motion, 0x68, void (*)(void *))(c->motion);
}

/* Disable the character (vtable +0x60 first). */
/* 0x00126910 */
void Character_Disable(Character *c) {
    s32 s;

    VCALL(c, 0x60, void (*)(Character *))(c);
    c->a.disabled = 1;
    c->unkE0 = 1;
    c->unkE2 = 1;
    c->unkE3 = 0;
    c->unkE4 = 0;
    s = c->unk152C;
    if (s == 0xF || s == 0x1C || s == 0x1A) {
        c->unk152C = 0xA;
    }
    c->a.unk2A = 1;
    Motion_Disable(c->motion);
}

/* 0x001269A0 */
void Character_PlaceModel(Character *c) {
}

/* 0x001269B0 */
void Character_Think(Character *c) {
}

static inline f32 Character_PathRemaining(Character *c) {
    if (c->moveMode == 6) {
        if (c->moveSub == 0x16) {
            return VCALL(gSceneGameF29740, 0x3C, f32 (*)(VObject *, f32 *, s32, s32, void *))(
                gSceneGameF29740, c->a.pos, c->unk128, c->unk124, c->unk12C);
        }
        if (c->moveSub == 0x17) {
            return *(f32 *)&c->unk14C4;
        }
    }
    return 0.0f;
}

/* Remaining distance of the current path move (0 if not following one). */
/* 0x00126E40 */
f32 Character_PathRemaining2(Character *c) {
    return Character_PathRemaining(c);
}

/* Forward to gRoutePlanner +0xC (the route planner) with the character's buffers at +0x148C and
   +0x138C after the first three arguments. */
/* 0x00126F30 */
s32 Character_RouteVia(Character *c, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7) {
    return VCALL(gRoutePlanner, 0xC, s32 (*)(VObject *, s32, s32, s32, void *, void *, s32, s32, s32, s32))(
        gRoutePlanner, a1, a2, a3, c->unk148C, c->unk138C, a4, a5, a6, a7);
}

/* Route request to gRoutePlanner +0xC toward `target`; side -1: the side of the current exit
 * (room manager +0x50 with 1), if any. Result kept in unk1384. */
/* 0x00126F80 */
s32 Character_Route(Character *c, s32 target, s32 unused2, s32 side, s32 unused4) {
    s32 useExit = 0;

    c->unk1388 = 0;
    if (side == -1) {
        useExit = 1;
        if (c->door != 0xFF) {
            side = VCALL(gRooms, 0x50, s32 (*)(VObject *, s32, u32, s32))(gRooms, c->a.room, c->door, 1);
        }
    }
    c->unk1384 = VCALL(gRoutePlanner, 0xC, s32 (*)(VObject *, s32, s32, s32, void *, void *, s32, s32))(
        gRoutePlanner, c->a.room, target, c->a.slot, c->unk148C, c->unk138C, useExit, side);
    return c->unk1384;
}

/* Release the current path (keeps the waypoint state). */
/* 0x00127060 */
void Character_ReleasePath(Character *c) {
    VCALL(gSceneGameF29740, 0x28, void (*)(VObject *, s32))(gSceneGameF29740, c->pathId);
    c->pathId = -1;
}

/* Fetch the path's waypoints into unk12C (planner +0x1C); returns their count. */
/* 0x001270A0 */
s32 Character_Waypoints(Character *c) {
    c->unk128 = 0;
    c->unk124 = VCALL(gSceneGameF29740, 0x1C, s32 (*)(VObject *, s32, void *))(gSceneGameF29740, c->pathId, c->unk12C);
    return c->unk124;
}

/* Same with planner +0x18. */
/* 0x001270F0 */
s32 Character_WaypointsCurve(Character *c) {
    c->unk128 = 0;
    c->unk124 = VCALL(gSceneGameF29740, 0x18, s32 (*)(VObject *, s32, void *))(gSceneGameF29740, c->pathId, c->unk12C);
    return c->unk124;
}

static inline void Character_FillRequest(Character *c, s32 kind, u32 goalTri, const f32 *goal) {
    c->pathReq->unk0 = kind;
    c->pathReq->startTri = c->a.navTri;
    sceVu0CopyVector(c->pathReq->startPos, c->a.pos);
    c->pathReq->goalTri = goalTri;
    sceVu0CopyVector(c->pathReq->goalPos, goal);
}

/* Plan a path of request kind `kind` to `goal`; returns its length (-1: none). */
/* 0x00127140 */
s32 Character_PlanPathKind(Character *c, s32 kind, u32 goalTri, const f32 *goal) {
    VObject *planner;

    Character_FillRequest(c, kind, goalTri, goal);
    planner = gSceneGameF29740;
    c->pathId = VCALL(planner, 0xC, s32 (*)(VObject *, PathRequest *, s32))(planner, c->pathReq, 0);
    if (c->pathId == -1) {
        return -1;
    }
    return VCALL(planner, 0x14, s32 (*)(VObject *))(planner);
}

/* Plan a path with planner option `opt`; returns the path id. */
/* 0x00127200 */
s32 Character_PlanPathOpt(Character *c, s32 kind, u32 goalTri, const f32 *goal, s32 opt) {
    Character_FillRequest(c, kind, goalTri, goal);
    c->pathId = VCALL(gSceneGameF29740, 0xC, s32 (*)(VObject *, PathRequest *, s32))(gSceneGameF29740, c->pathReq, opt);
    return c->pathId;
}

/* Advance along the fetched waypoints by `speed` (planner +0x24), facing the direction moved.
 * -1 while waypoints remain, 0 at the end (or if there were none). */
/* 0x001272B0 */
s32 Character_FollowWaypoints(Character *c, f32 speed) {
    sceVu0FVECTOR p;
    u32 tri;
    f32 dx, dz;

    if (!(c->unk128 < c->unk124)) {
        return 0;
    }
    tri = c->a.navTri;
    sceVu0CopyVector(p, c->a.pos);
    c->unk128 = VCALL(gSceneGameF29740, 0x24, s32 (*)(VObject *, u32 *, f32 *, s32, s32, void *, f32))(
        gSceneGameF29740, &tri, p, c->unk128, c->unk124, c->unk12C, speed);
    dx = p[0] - c->a.pos[0];
    dz = p[2] - c->a.pos[2];
    if (!(dx == 0.0f && dz == 0.0f)) {
        f32 yaw = func_0031C5C0(dx, dz);

        c->a.angle[1] = yaw;
        sceVu0UnitMatrix(c->a.rot);
        sceVu0RotMatrixY(c->a.rot, c->a.rot, yaw);
    }
    c->a.navTri = tri;
    sceVu0CopyVector(c->a.pos, p);
    return (c->unk128 < c->unk124) ? -1 : 0;
}

/* Where `step` along the waypoints would lead (planner +0x20), without moving; the current
 * position if that point is blocked for the character. */
/* 0x001273D0 */
s32 Character_WaypointAhead(Character *c, u32 *triOut, f32 *posOut, f32 step) {
    s32 r;

    *triOut = c->a.navTri;
    sceVu0CopyVector(posOut, c->a.pos);
    if (!(c->unk128 < c->unk124)) {
        return c->unk124;
    }
    r = VCALL(gSceneGameF29740, 0x20, s32 (*)(VObject *, u32 *, f32 *, s32, s32, void *, f32))(
        gSceneGameF29740, triOut, posOut, c->unk128, c->unk124, c->unk12C, step);
    if (NavMesh_Tri(gNavMesh, *triOut)->flags & c->a.navMask) {
        *triOut = c->a.navTri;
        sceVu0CopyVector(posOut, c->a.pos);
        return c->unk124;
    }
    return r;
}

/* Like Character_FollowWaypoints with planner +0x20, but a step into a triangle blocked for the character
 * ends the path instead. */
/* 0x001274E0 */
s32 Character_FollowWaypointsBlocked(Character *c, f32 speed) {
    sceVu0FVECTOR p;
    u32 tri;
    f32 dx, dz;

    if (!(c->unk128 < c->unk124)) {
        return 0;
    }
    tri = c->a.navTri;
    sceVu0CopyVector(p, c->a.pos);
    c->unk128 = VCALL(gSceneGameF29740, 0x20, s32 (*)(VObject *, u32 *, f32 *, s32, s32, void *, f32))(
        gSceneGameF29740, &tri, p, c->unk128, c->unk124, c->unk12C, speed);
    dx = p[0] - c->a.pos[0];
    dz = p[2] - c->a.pos[2];
    if (!(dx == 0.0f && dz == 0.0f)) {
        f32 yaw = func_0031C5C0(dx, dz);

        c->a.angle[1] = yaw;
        sceVu0UnitMatrix(c->a.rot);
        sceVu0RotMatrixY(c->a.rot, c->a.rot, yaw);
    }
    if (NavMesh_Tri(gNavMesh, tri)->flags & c->a.navMask) {
        c->unk128 = c->unk124;
        return 0;
    }
    c->a.navTri = tri;
    sceVu0CopyVector(c->a.pos, p);
    return -(c->unk128 < c->unk124);
}

/* vtable +0x24 (Character): as the base. */
/* 0x00127650 */
void Character_RememberPos(Character *c) {
    Actor_RememberPos(&c->a);
}

/* vtable +0xC (Character): reset. */
/* 0x00127660 */
void Character_Reset(Character *c) {
    Actor_Reset(&c->a);
    c->pathReq = &c->req;
    c->pathId = -1;
    c->moveMode = 0;
    c->unk128 = 0;
    c->unk124 = 0;
    func_002A8410((u8 *)c->state);
    func_002A8410((u8 *)c->state2);
    c->unkE0 = 0;
    c->unkE1 = 0;
    c->unkE2 = 0;
    c->unkE3 = 0;
    c->unkE4 = 1;
    c->unkE8 = 0;
    c->unkEC = -1;
    c->unk152C = 10;
    c->unk1530 = 0;
    c->unk1538 = 0;
    c->unk1534 = 0;
    c->heardSlot = 0xFF;
}

/* 0x002DCAE0 */
void Motion_Disable(u8 *p) {
    b4_clear_dca70(p);
    p[0x30] = 1;
}

#include "effectmgr.h"

extern void *D_00479AE0[];   /* ripple effect vtable */
extern void *D_00479AA0[];   /* splash particle effect vtable */

typedef struct RippleParams {
    f32 pos[4] __attribute__((aligned(16)));
    u8 rgba[4];
    f32 size;
} RippleParams;

typedef struct SplashParams {
    f32 pos[4] __attribute__((aligned(16)));
    u8 rgba[4];
    s32 count;
    f32 v[10];
} SplashParams;

static inline void Ripple_Init(void **obj) {
    obj[0] = D_00479AE0;
}

static inline void Splash_Init(void **obj) {
    obj[0] = D_00479AA0;
    obj[0x610 / 4] = D_00469D00;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = D_0046FC30;
}

/* Footstep in water: if `pos` (reached from the character) is on a water triangle (flags
 * 0x2008000), put it on the surface and spawn a ripple and a splash (`big`: larger). */
/* 0x00125E10 */
void Character_WaterStep(Character *c, f32 *pos, s32 big) {
    NavMesh *nm;
    RippleParams rp;
    SplashParams sp;
    u8 *mgr;
    u32 tri, flags;

    tri = Actor_TriTo(&c->a, pos, 0);
    if (tri == NAV_NONE) {
        return;
    }
    nm = gNavMesh;
    flags = NavMesh_Tri(nm, tri)->flags;
    if ((flags & 0x2008000) != 0x2008000) {
        return;
    }
    VCALL(nm, 0x14, void (*)(NavMesh *, u32, f32 *))(nm, tri, pos);

    sceVu0CopyVector(rp.pos, pos);
    rp.rgba[0] = 0x30;
    rp.rgba[1] = 0x30;
    rp.rgba[2] = 0x30;
    rp.rgba[3] = 0x30;
    rp.pos[1] = rp.pos[1] + 0x1.99999ap-4f /* 0.1 */;
    rp.size = big ? 0x1.99999ap-3f /* 0.2 */ : 0x1.333334p-3f /* 0.15 */;
    mgr = gEffects;
    func_002D6090(mgr, Effect_New(mgr, 0x40, Ripple_Init), &rp);

    sceVu0CopyVector(sp.pos, pos);
    sp.rgba[3] = 0x40;
    sp.rgba[0] = 0x20;
    sp.count = 16;
    sp.rgba[1] = 0x20;
    sp.rgba[2] = 0x20;
    sp.v[2] = 0x1.99999ap-4f;   /* 0.1 */
    sp.v[8] = 0x1.99999ap-4f;
    sp.v[3] = 0x1.99999ap-3f;   /* 0.2 */
    sp.v[9] = 0x1.99999ap-4f;
    if (!big) {
        sp.v[0] = 0x1.99999ap-3f;
        sp.v[1] = 0x1.99999ap-3f;
        sp.v[4] = 0x1.99999ap-3f;
        sp.v[6] = 0x1.99999ap-3f;
        sp.v[5] = 0x1.99999ap-4f;
        sp.v[7] = 0x1.99999ap-4f;
    } else {
        sp.v[0] = 0.25f;
        sp.v[1] = 0.25f;
        sp.v[4] = 0.25f;
        sp.v[6] = 0.25f;
        sp.v[5] = 0x1.333334p-3f;   /* 0.15 */
        sp.v[7] = 0x1.333334p-3f;
    }
    func_002D6090(mgr, Effect_New(mgr, 0x720, Splash_Init), &sp);
}

/* Room the current route move leads to (0xFFFF: none). */
static inline u32 Character_RouteRoom(Character *c) {
    if (c->moveMode == 6) {
        if (c->moveSub == 0x16) {
            return c->unk14C0;
        }
        if (c->moveSub == 0x17) {
            if ((c->unk1388 < c->unk1384) == 1) {
                return ((u16 *)c->unk138C)[c->unk1388];
            }
            return 0xFFFF;
        }
        return 0xFFFF;
    }
    return 0xFFFF;
}

/* If the character's route leads into a neighbouring room through one of this room's exits,
 * `out` (optional) = a point past that exit at the remaining distance + 100 (150 if progress
 * says so for the room). True if so. */
/* 0x001264D0 */
s32 Character_RouteExitPoint(Character *c, f32 *out) {
    VObject *rooms, *prog;
    sceVu0FVECTOR a, b;
    u32 next = Character_RouteRoom(c) & 0xFFFF;
    s32 cur, exit;
    u32 i;
    u8 found;
    f32 dist;

    if (next >= 400) {
        return 0;
    }
    rooms = gRooms;
    if (rooms == NULL || gDoors == NULL) {
        return 0;
    }
    prog = (VObject *)gProgress;
    cur = VCALL(prog, 0xC, s32 (*)(VObject *))(prog);
    exit = VCALL(rooms, 0x3C, u32 (*)(VObject *, u32, s32))(rooms, next, cur) & 0xFF;
    if (exit == 0xFF) {
        return 0;
    }
    found = 0;
    for (i = 0; i < 8; i++) {
        if (next == (VCALL(rooms, 0x10, u32 (*)(VObject *, s32, u32))(rooms, cur, i & 0xFF) & 0xFFFF)) {
            found = 1;
            break;
        }
    }
    if (!found) {
        return 0;
    }
    dist = Character_PathRemaining(c);
    if (dist <= 0.0f) {
        return 0;
    }
    if (out != NULL) {
        rooms = gRooms;
        VCALL(rooms, 0x30, void (*)(VObject *, s32, f32 *))(rooms, exit, a);
        VCALL(rooms, 0x34, void (*)(VObject *, s32, f32 *))(rooms, exit, b);
        sceVu0SubVector(out, a, b);
        out[3] = 1.0f;
        sceVu0Normalize(out, out);
        sceVu0ScaleVector(out, out, dist + ((Progress_DoorOpen((Progress *)prog, next) & 0xFF) ? 100.0f : 150.0f));
        sceVu0AddVector(out, out, a);
        out[3] = 1.0f;
    }
    return 1;
}

#define NOISE_EVENTS(prog) ((NoiseEvent *)((u8 *)(prog) + 0x778))
#define NOISE_LEVEL_SETTING(prog) (*((u8 *)(prog) + 0x1114))

/* Hearing: pick the first noise event (of the other characters' slots) still louder than the
 * character's threshold after attenuation - by distance in the current room, or by the number
 * of rooms between (32 loudness per room hop; quiet sounds only through rooms progress marks).
 * The chosen event is copied to `heard` (heardSlot 0xFF: none). */
/* 0x001269C0 */
void Character_Hearing(Character *c) {
    VObject *prog = (VObject *)gProgress;
    VObject *rooms = gRooms;
    NavMesh *nm = gNavMesh;
    VObject *doors = gDoors;
    VObject *router = gRoutePlanner;
    NoiseEvent *ev = NOISE_EVENTS(prog);
    s32 i;
    u32 exit = 0;

    for (i = 0; i < 4; i++, ev++) {
        s16 loud, base;
        u8 heard;

        if (i == c->a.slot) {
            continue;
        }
        loud = ev->level;
        if (loud < 0x80) {
            switch (NOISE_LEVEL_SETTING(prog)) {
            case 1:
                loud = loud - 0x1F;
                break;
            case 2:
                loud = loud - 0x3F;
                break;
            case 3:
                loud = loud - 0x5F;
                break;
            }
        }
        base = loud;
        if (base <= 0) {
            continue;
        }
        heard = 0;
        if (c->a.room == ev->room) {
            heard = 1;
        } else if (ev->exitId != 0xFFFF) {
            exit = VCALL(rooms, 0x3C, u32 (*)(VObject *, u32, s32))(rooms, ev->exitId, ev->room) & 0xFF;
            if (exit == 0xFF || c->a.room == VCALL(rooms, 0x18, s32 (*)(VObject *, s32, u32))(rooms, ev->room, exit)) {
                heard = 1;
            }
        }
        if (heard == 1) {
            s32 room = c->a.room;

            if (room == VCALL(prog, 0xC, s32 (*)(VObject *))(prog)) {
                sceVu0FVECTOR src, d;
                f32 dist, dy;

                if (ev->tri != -1) {
                    VCALL(nm, 0xC, void (*)(NavMesh *, s32, f32 *))(nm, ev->tri, src);
                } else if (ev->exitId != 0xFFFF) {
                    exit = VCALL(rooms, 0x3C, u32 (*)(VObject *, u32, s32))(rooms, ev->exitId, room) & 0xFF;
                    if (exit != 0xFF) {
                        VCALL(doors, 0x34, void (*)(VObject *, u32, f32 *))(doors, exit, src);
                    } else {
                        sceVu0CopyVector(src, c->a.pos);
                    }
                }
                sceVu0SubVector(d, src, c->a.pos);
                dist = __builtin_sqrtf(d[2] * d[2] + d[0] * d[0]);
                dy = (d[1] <= 0.0f) ? -d[1] : d[1];
                loud = loud - (s16)(s32)(0x1.99999ap-4f /* 0.1 */ * ((0.0f + dist) + 3.0f * dy));
            }
        } else {
            struct {
                u16 before;       /* the original reads route[-1] for an empty route */
                u16 route[8];
            } r;
            s32 n = VCALL(router, 0xC, s32 (*)(VObject *, s32, s32, s32, void *, u16 *, s32, s32, s32, s32))(
                router, c->a.room, ev->room, 0xFF, NULL, r.route, 1, -1, -1, 2);

            if (ev->exitId != 0xFFFF && n > 0
                && (exit & 0xFF) == (VCALL(rooms, 0x3C, u32 (*)(VObject *, u32, s32))(rooms, r.route[n - 1], ev->room) & 0xFF)) {
                n--;
            }
            if (n == -1) {
                if (base < 0x60) {
                    loud = 0;
                }
            } else if (base < (n << 5)) {
                loud = 0;
            } else if (base < 0x41 && !(Progress_DoorOpen((Progress *)prog, r.route[n - 1]) & 0xFF)) {
                loud = 0;
            }
        }
        if ((s32)c->hearThreshold < loud) {
            NoiseEvent *e = &NOISE_EVENTS((VObject *)gProgress)[i];

            c->heardSlot = i;
            c->heard.level = e->level;
            c->heard.room = e->room;
            c->heard.tri = e->tri;
            c->heard.exitId = e->exitId;
            return;
        }
    }
    c->heardSlot = 0xFF;
}

/* Activate the character (state reset), then vtable +0x60. Shares its tail with Character_Deactivate
 * in the original. */
/* 0x00125CC0 */
void Character_Activate(Character *c) {
    c->a.active = 1;
    c->a.unkC4 = 0;
    c->unkE0 = 0;
    c->unkE2 = 0;
    c->unkE3 = 0;
    c->unkE4 = 1;
    c->a.disabled = 1;
    c->unkE1 = 0;
    c->a.unk2B = 0;
    c->a.unk2D = 0;
    c->a.unk2A = 0;
    c->moveMode = 0;
    c->unk100 = -1;
    c->unk104[0] = 0;
    c->unk104[1] = 0;
    c->unk104[2] = 0;
    c->unk110[0] = 0.0f;
    c->unk110[1] = 0.0f;
    c->unk110[2] = 0.0f;
    c->unk110[3] = 1.0f;
    c->door = 0xFF;
    c->heardSlot = 0xFF;
    VCALL(c, 0x60, void (*)(Character *))(c);
}

/* Deactivate: vtable +0x60, then off the mesh. */
/* 0x00125D40 */
void Character_Deactivate(Character *c) {
    VCALL(c, 0x60, void (*)(Character *))(c);
    c->a.active = 0;
    c->a.navTri = NAV_NONE;
}

/* the actor pool's placement new and delete (the pool frees blocks itself) */
/* 0x00121370 */
void *ActorPool_new(u32 size, void *place) {
    return place;
}

/* 0x00121360 */
void ActorPool_delete(void *p) {
}
