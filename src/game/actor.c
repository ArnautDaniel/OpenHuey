/* Actor base class (vtables 0x469C20 -> 0x469C60): position, room, nav mesh movement tests,
 * positional sound. Characters (Fiona, Hewie, pursuers) derive from it. */
#include "common.h"
#include "actor.h"
#include "navmesh.h"
#include "sce/libvu0.h"

extern VObject *D_0044E568;   /* room manager: +0x80 GetRoomOrigin(room, out) -> bool */
extern VObject *D_0044E560;   /* sound manager */
extern VObject *gProgress;    /* +0xC current room */

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
