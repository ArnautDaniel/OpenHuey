/* Actor base class (vtables 0x469C20 -> 0x469C60): position, room, nav mesh movement tests,
 * positional sound. Characters (Fiona, Hewie, pursuers) derive from it. */
#include "common.h"
#include "actor.h"
#include "navmesh.h"
#include "sce/libvu0.h"

extern VObject *D_0044E568;   /* room manager: +0x80 GetRoomOrigin(room, out) -> bool */
extern VObject *D_0044E560;   /* sound manager */
extern VObject *gProgress;    /* +0xC current room */
extern VObject *gFileLoader;  /* +0x28 load state(file id): 2 = done */

extern void func_002FF650(VObject *snd, s32 id, s32 arg2, const f32 *pos, s32 arg4, s32 arg5);

/* Position relative to the current room: pos + origin(own room) - origin(current room).
 * False if either room is unknown. */
s32 func_00122B50(Actor *a, f32 *out) {
    VObject *rooms = D_0044E568;
    sceVu0FVECTOR own, cur;

    if (!(VCALL(rooms, 0x80, u32 (*)(VObject *, s32, f32 *))(rooms, a->room, own) & 0xFF)) {
        return 0;
    }
    if (!(VCALL(rooms, 0x80, u32 (*)(VObject *, s32, f32 *))(
              rooms, VCALL(gProgress, 0xC, s32 (*)(VObject *))(gProgress), cur) & 0xFF)) {
        return 0;
    }
    sceVu0SubVector(out, own, cur);
    sceVu0AddVector(out, out, a->pos);
    return 1;
}

/* Play a sound at `pos` (default: the actor's position), unless the actor is silent. */
void func_00122C20(Actor *a, s32 id, s32 arg2, s32 arg3, s32 arg4, const f32 *pos) {
    if (a->unk2C == 1) {
        return;
    }
    func_002FF650(D_0044E560, id, arg2, pos != NULL ? pos : a->pos, arg3, arg4);
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
u32 func_00122E80(void *self, u32 tri, const f32 *p, u32 mask) {
    NavMesh *nm = D_0044E570;
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
u32 func_00123080(void *self, u32 from, u32 to, const f32 *fromPos, const f32 *toPos, u32 mask) {
    NavMesh *nm = D_0044E570;
    u32 cur, toAlt;

    if (from >= nm->numTris || to >= nm->numTris) {
        return NAV_FAIL;
    }
    /* starting exactly on a corner: the walk may begin in the neighbour sharing it */
    cur = from;
    if (NavTri_CornerAt(NavMesh_Tri(nm, from), fromPos) >= 0
        && NavMesh_Exit(nm, from, fromPos, toPos) == NAV_EDGE_CORNER) {
        cur = func_00122E80(self, from, fromPos, mask);
    }
    /* ending on a corner: the neighbour sharing it counts as the destination too */
    toAlt = to;
    if (to < nm->numTris && NavTri_CornerAt(NavMesh_Tri(nm, to), toPos) >= 0) {
        toAlt = func_00122E80(self, to, toPos, mask);
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
s32 func_00122C90(void *self, u32 triA, u32 triB, const f32 *posA, const f32 *posB, u32 mask) {
    u32 walk;
    u32 r, r2;

    if (mask == 0) {
        mask = 0x40080;
    }
    walk = mask | 0x4000;
    r = func_00123080(self, triA, triB, posA, posB, walk);
    if (r == NAV_REACHED) {
        return 1;
    }
    if (r == NAV_BLOCKED) {
        return 0;
    }
    if (r != NAV_STUCK && (mask & NavMesh_TriFlags(D_0044E570, r))) {
        return 0;
    }
    r2 = func_00123080(self, triB, triA, posB, posA, walk);
    if (r2 == NAV_REACHED) {
        return 0;
    }
    if (r2 == NAV_STUCK) {
        return r != NAV_STUCK;
    }
    if (r2 == NAV_BLOCKED) {
        return 0;
    }
    return (mask & NavMesh_TriFlags(D_0044E570, r2)) ? 0 : 1;
}

#define NAV_NO_STAND 0x80001     /* triangle flags where nothing may stand */

extern VObject *D_0044E558;      /* +0x2C(i, arg) -> bool, 8 entries */
extern u32 func_00177BF0(VObject *prog, u32 i, u32 slot);   /* returns u8 flags */
extern u32 func_00177A20(VObject *prog, u32 i, u32 slot);   /* returns u8 flags */

/* Is triangle `tri` free: standable, and not claimed by any of D_0044E558's 8 entries or the
 * nav mesh's extra regions (vtable +0x50)? (The flags read goes through a NULL triangle for an
 * out-of-range index, like the original.) */
s32 func_00123470(void *self, u32 tri, s32 arg) {
    NavMesh *nm = D_0044E570;
    VObject *obj;
    u32 n;
    u8 i;

    if (NavMesh_Tri(nm, tri)->flags & NAV_NO_STAND) {
        return 0;
    }
    obj = D_0044E558;
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
s32 func_001235C0(void *self, Actor *a) {
    NavMesh *nm = D_0044E570;
    VObject *prog;
    u32 n;
    u8 i;

    if (NavMesh_Tri(nm, a->navTri)->flags & NAV_NO_STAND) {
        return 0;
    }
    prog = gProgress;
    for (i = 0; i < 8; i++) {
        if (func_00177BF0(prog, i, *(u8 *)&a->slot) & 0x20) {
            return 0;
        }
    }
    n = nm->numDoors;
    for (i = 0; i < n; i++) {
        if (func_00177A20(prog, i, *(u8 *)&a->slot) & 0x8) {
            return 0;
        }
    }
    return 1;
}

/* Point in front of door `door` on `side` (0/1), 5 units out and shifted by `ofs` in the door's
 * frame; walks the mesh from the door to it. Returns its triangle (point in `out`), or -1. */
u32 func_00123710(void *self, s32 door, s32 side, const f32 *ofs, f32 *out) {
    NavMesh *nm;
    sceVu0FMATRIX m;
    sceVu0FVECTOR d, base, target, cur;
    u32 tri;

    if (!(door >= 0 && (u32)door < D_0044E570->numDoors)) {
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
    nm = D_0044E570;
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
s32 func_00123960(Actor *a, s32 door, s32 side) {
    sceVu0FMATRIX m;
    sceVu0FVECTOR fwd, dir;

    if (!(door >= 0 && (u32)door < D_0044E570->numDoors)) {
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
    sceVu0RotMatrixY(m, m, VCALL(D_0044E570, 0x58, f32 (*)(NavMesh *, s32, s32))(D_0044E570, door, side));
    sceVu0ApplyMatrix(dir, m, dir);
    return !(sceVu0InnerProduct(fwd, dir) <= 0.0f);
}

/* Free distance from `pos` (in triangle `tri`) along heading `angle`, up to `dist`: walks the
 * mesh until the ray leaves it or enters a triangle blocked by `mask` (-1 = the actor's own
 * mask). Horizontal distance to that edge, `dist` if unobstructed, 0 through a corner. */
f32 func_00123A70(Actor *a, u32 tri, const f32 *pos, u32 mask, f32 angle, f32 dist) {
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
    nm = D_0044E570;
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
s32 func_00123C60(Actor *a, s32 room, const f32 *pos) {
    sceVu0FVECTOR origin, da, dp;
    f32 la;

    VCALL(D_0044E568, 0x30, void (*)(VObject *, s32, f32 *))(D_0044E568, room, origin);
    sceVu0SubVector(da, a->pos, origin);
    sceVu0SubVector(dp, pos, origin);
    la = sceVu0InnerProduct(da, da);
    return la < sceVu0InnerProduct(dp, dp);
}

/* vtable +0x3C (base): always true */
s32 func_00123D00(Actor *a) {
    return 1;
}

/* vtable +0x34 (base): nothing */
void func_00123D10(Actor *a) {
}

/* Triangle containing `p`, found by walking from the actor's triangle toward it; -1 if the
 * walk leaves the mesh or passes a corner. */
u32 func_00123D20(Actor *a, const f32 *p) {
    NavMesh *nm = D_0044E570;
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
u32 func_00123E20(Actor *a, f32 *p) {
    NavMesh *nm = D_0044E570;
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
void func_00123F50(Actor *a) {
}

/* vtable +0x2C (base): nothing */
void func_00123F60(Actor *a) {
}

/* Triangle containing `target`, walking from triangle `tri` at `from`; -1 if the walk leaves
 * the mesh, passes a corner or enters a triangle blocked by `mask` (-1 = the actor's mask). */
u32 func_00124320(Actor *a, const f32 *target, u32 tri, const f32 *from, u32 mask) {
    NavMesh *nm;

    if (mask == NAV_NONE) {
        mask = a->navMask;
    }
    nm = D_0044E570;
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
    VCALL(D_0044E570, 0x14, void (*)(NavMesh *, u32, f32 *))(D_0044E570, a->navTri, a->pos);
}

/* Push this actor out of `other`'s collision cylinder. If the straight push is blocked, try
 * directions rotated 2, 4, ... 180 degrees either way. 0 on success, -1 if nowhere fits. */
s32 func_00123F70(Actor *a, Actor *other) {
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
    tri = func_00124320(a, target, a->navTri, a->pos, NAV_NONE);
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
        tri = func_00124320(a, target, a->navTri, a->pos, NAV_NONE);
        if (tri != NAV_NONE) {
            Actor_PlaceAt(a, tri, target);
            return 0;
        }
        sceVu0ApplyMatrix(right, rotPos, right);
        sceVu0AddVector(target, other->pos, right);
        target[3] = 1.0f;
        tri = func_00124320(a, target, a->navTri, a->pos, NAV_NONE);
        if (tri != NAV_NONE) {
            Actor_PlaceAt(a, tri, target);
            return 0;
        }
    }
    return -1;
}

/* Are the two actors touching: both enabled, vertically within the lower one's height
 * (+ `vmargin`) and horizontally within their radii (+ `margin`)? */
s32 func_001241F0(Actor *a, Actor *b, f32 margin, f32 vmargin) {
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

extern f32 func_0031C5C0(f32 x, f32 z);   /* float math library: heading of (x, z), atan2-like */

/* Triangle containing `target` reached from the actor's position (see func_00124320). */
u32 func_00124480(Actor *a, const f32 *target, u32 mask) {
    return func_00124320(a, target, a->navTri, a->pos, mask);
}

/* Distance from the actor to `p`. */
f32 func_00124490(Actor *a, const f32 *p) {
    sceVu0FVECTOR d;

    sceVu0SubVector(d, p, a->pos);
    return __builtin_sqrtf(sceVu0InnerProduct(d, d));
}

/* Heading from the actor to `p` (its current heading if `p` is right above or below it). */
f32 func_001244D0(Actor *a, const f32 *p) {
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
void func_00124720(Actor *a, const f32 *delta) {
    sceVu0FVECTOR to, out;
    u32 tri;

    sceVu0CopyVector(to, a->pos);
    to[0] += delta[0];
    to[2] += delta[2];
    tri = NavMesh_Slide(D_0044E570, a->navTri, out, a->pos, to, 0);
    if (tri != NAV_NONE) {
        a->navTri = tri;
        a->pos[0] = out[0];
        a->pos[2] = out[2];
        a->pos[1] += delta[1];
    }
}

/* Move by `delta` horizontally as far as the actor's blocking mask allows. */
void func_001247E0(Actor *a, const f32 *delta) {
    sceVu0FVECTOR to, out;
    u32 tri;

    sceVu0CopyVector(to, a->pos);
    to[0] += delta[0];
    to[2] += delta[2];
    tri = NavMesh_Slide(D_0044E570, a->navTri, out, a->pos, to, a->navMask);
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
f32 func_00124530(Actor *a, f32 target, f32 step) {
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

extern VObject *D_0044E550;      /* random numbers: +0x18 / +0x1C -> f32 in 0..1 */
extern VObject *D_0044E4D0;      /* +0x10(point, id, tri) -> bool: point taken by room object `id` */

/* Teleport to a random free triangle of the current room's mesh (only if the actor is in the
 * current room). `kind` selects the area flags: 0 -> 0x100000, 1 -> 0x200000, else both;
 * for -1 and 2 the triangle must have them, otherwise it must not. Placed via vtable +0x28. */
void func_00124890(Actor *a, s32 kind) {
    NavMesh *nm;
    VObject *rng, *rooms, *objs;
    u32 area, n;

    if (a->room != VCALL(gProgress, 0xC, s32 (*)(VObject *))(gProgress)) {
        return;
    }
    if (kind == 1) {
        area = 0x200000;
    } else if (kind == 0) {
        area = 0x100000;
    } else {
        area = 0x300000;
    }
    nm = D_0044E570;
    rng = D_0044E550;
    n = nm->numTris;
    rooms = D_0044E568;
    objs = D_0044E4D0;
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

extern void func_0010E5F0(f32 *out, const f32 *v);   /* libvu0: copy x, y, z */

/* vtable +0x28: place the actor in triangle `tri`, optionally turning to `*heading` and moving
 * to `pos` (which must lie in the triangle; it is put on the surface), else to the triangle's
 * centre. 0, or -1 if the triangle is invalid, blocked or doesn't contain `pos`. */
s32 func_00124B80(Actor *a, u32 tri, const f32 *heading, f32 *pos) {
    NavMesh *nm = D_0044E570;

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
void func_00124D00(Actor *a) {
    a->prevNavTri = a->navTri;
    func_0010E5F0(a->prevPos, a->pos);
}

/* vtable +0x20 (base): nothing */
void func_00124D20(Actor *a) {
}

/* vtable +0x1C (base): nothing */
void func_00124D30(Actor *a) {
}

/* Has the actor's data file (id flags24 | slot) finished loading? */
s32 func_00124D40(Actor *a) {
    return VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, a->flags24 | a->slot) == 2;
}

/* vtable +0x18 (base): nothing */
void func_00124D80(Actor *a) {
}

/* vtable +0x14 (base): nothing */
void func_00124D90(Actor *a) {
}

/* vtable +0x10 (base): nothing */
void func_00124DA0(Actor *a) {
}

/* vtable +0xC: reset position, orientation and state. */
void func_00124DB0(Actor *a) {
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

void func_00124E40(Actor *a) {
}

void *func_00124E50(void *a, void *b) {
    return b;
}

extern void *D_00469C20[];   /* Actor base vtable */
extern void *D_00469C60[];   /* Actor vtable */

/* vtable +0x8 (0x469C60): destructor. Actors live inside their scene, so "delete" does nothing. */
Actor *func_00124E60(Actor *a, s32 flags) {
    if (a != NULL) {
        a->vtbl = D_00469C60;
        a->vtbl = D_00469C20;
        if ((s16)flags > 0) {
            func_00124E40(a);
        }
    }
    return a;
}

s32 func_00124EC0(Actor *a) {
    return 0;
}

s32 func_00124F10(Actor *a) {
    return 0;
}

/* vtable +0x38 (base): per-frame update - nothing for a plain actor */
void func_00120F80(Actor *a) {
}
