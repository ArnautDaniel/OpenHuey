/* The nav mesh (room manager +0x3E0, gNavMesh): the room's walkable triangles (PAC section
 * 0) with their flags (section 16), section 1, and the door regions (pairs of triangles with a
 * door group in their flags). */
#include "common.h"
#include "game.h"
#include "navmesh.h"
#include "globals.h"
#include "memcard.h"
#include "libc.h"
#include "msl.h"

#define MESH_SIZE 0x50

extern void *D_0046AA40[];
void *NavMeshBase_dtor(u8 *o, s32 flags);

/* take the room's meshes (`meshes`: count, then entries from +0x10; their bounds get the
 * float low bit cleared), the per-mesh flag bytes (`flags`, may be NULL: bit 0 -> mesh flag
 * 0x4000, bit 1 -> 0x80000) and section `sec1`. Returns the mesh count (-1: no meshes). */
/* 0x0017CC00 */
s32 NavMeshSet_Take(u8 *set, u8 *meshes, u8 *sec1, u8 *flags) {
    u32 i, k;
    u8 *f;

    if (meshes == NULL) {
        return -1;
    }
    AT(set, 0x8, u32) = AT(meshes, 0, u32);
    AT(set, 0x4, u8 *) = meshes + 0x10;
    AT(set, 0x18, u8 *) = flags;
    for (i = 0; i < AT(set, 0x8, u32); i++) {
        for (k = 0; k < 3; k++) {
            u8 *m = AT(set, 0x4, u8 *) + i * MESH_SIZE + k * 0x10;

            AT(m, 0x0, u32) &= ~1;
            AT(m, 0x8, u32) &= ~1;
        }
    }
    f = AT(set, 0x18, u8 *);
    for (i = 0; i < AT(set, 0x8, u32); i++) {
        u8 *m = AT(set, 0x4, u8 *) + i * MESH_SIZE;

        AT(m, 0x3C, u32) &= 0xDF39FFFF;
        if (f != NULL) {
            if (*f & 1) {
                AT(m, 0x3C, u32) |= 0x4000;
            }
            if (*f & 2) {
                AT(m, 0x3C, u32) |= 0x80000;
            }
            f++;
        }
    }
    if (sec1 != NULL) {
        AT(set, 0x10, u32) = AT(sec1, 0, u32);
        AT(set, 0xC, u8 *) = sec1 + 0x10;
    }
    return AT(set, 0x8, u32);
}

/* destructor (vtable D_0046AA40) */
/* 0x0017CDD0 */
void *NavMeshBase_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AA40;
        gNavMesh = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

#include "sce/libvu0.h"

#define TRI(set, i) (AT(set, 0x4, u8 *) + (i) * MESH_SIZE)
#define LINK(set, g) ((set) + 0x20 + (g) * 0x30)

/* +0x4C find the door regions: triangles whose flags carry a link group (bits 9..13) pair up (the
 * higher one first; up to 5 links at +0x20, 0x30 each). For each side of a link: the edge
 * facing the other triangle (vt+0x20), its midpoint (+0x10 / +0x20) and the heading across
 * it (+0x8 / +0xC). +0x14: the number of links. */
/* 0x0017B660 */
void NavMesh_FindDoorRegions(u8 *set) {
    f32 centre[2][4] __attribute__((aligned(16)));
    f32 edge[4] __attribute__((aligned(16)));
    f32 tmp[4] __attribute__((aligned(16)));
    u32 i;
    s32 g, s, k;

    for (g = 0; g < 5; g++) {
        AT(LINK(set, g), 0x0, s32) = -1;
        AT(LINK(set, g), 0x4, s32) = -1;
    }
    for (i = 0; i < AT(set, 0x8, u32); i++) {
        u8 *t = TRI(set, i);
        u32 grp = AT(t, 0x3C, u32) & 0x3E00;
        u8 *l;

        if (grp == 0) {
            continue;
        }
        l = LINK(set, (grp >> 9) - 1);
        if (AT(l, 0x0, s32) == -1) {
            AT(l, 0x0, s32) = i;
        } else if (AT(t, 0x4, f32) <= AT(TRI(set, AT(l, 0x0, s32)), 0x4, f32)) {
            AT(l, 0x4, s32) = i;
        } else {
            AT(l, 0x4, s32) = AT(l, 0x0, s32);
            AT(l, 0x0, s32) = i;
        }
    }
    for (g = 0; g < 5; g++) {
        u8 *l = LINK(set, g);

        if (AT(l, 0x0, s32) == -1) {
            break;
        }
        for (s = 0; s < 2; s++) {
            f32 *v = (f32 *)TRI(set, AT(l, s * 4, s32));

            for (k = 0; k < 3; k++) {
                centre[s][k] = (v[4 + k] + v[k] + v[8 + k]) / 3.0f;
            }
            centre[s][3] = 1.0f;
        }
        for (s = 0; s < 2; s++) {
            s32 tri = AT(l, s * 4, s32);
            u8 *t = TRI(set, tri);
            u32 e0 = VCALL(set, 0x20, u32 (*)(u8 *, s32, f32 *, f32 *))(set, tri, centre[s], centre[s ^ 1]);
            u32 e1 = e0 < 2 ? e0 + 1 : 0;
            f32 *a = (f32 *)(t + e0 * 16);
            f32 *b = (f32 *)(t + e1 * 16);

            sceVu0SubVector(edge, b, a);
            sceVu0SubVector(tmp, centre[s], a);
            sceVu0OuterProduct(tmp, edge, tmp);
            sceVu0OuterProduct(edge, edge, tmp);
            sceVu0Normalize(tmp, edge);
            AT(l, 0x8 + s * 4, f32) = func_0031C5C0(tmp[0], tmp[2]);
            sceVu0CopyVector(edge, a);
            sceVu0AddVector(edge, edge, b);
            func_0010E640((f32 *)(l + 0x10 + s * 0x10), edge, 0.5f);
            AT(l, 0x1C + s * 0x10, f32) = 1.0f;
        }
    }
    for (g = 0; g < 5; g++) {
        if (AT(LINK(set, g), 0x0, s32) == -1) {
            break;
        }
    }
    AT(set, 0x14, s32) = g;
}

/* +0xC the centre of triangle `i` (w 1) */
/* 0x0017CB60 */
void NavMesh_TriCentre(NavMesh *nm, u32 i, f32 *out) {
    f32 (*v)[4];

    if (i >= nm->numTris || nm->tris == NULL) {
        return;
    }
    v = nm->tris[i].v;
    out[0] = (v[1][0] + v[0][0] + v[2][0]) / 3.0f;
    out[1] = (v[1][1] + v[0][1] + v[2][1]) / 3.0f;
    out[2] = (v[1][2] + v[0][2] + v[2][2]) / 3.0f;
    out[3] = 1.0f;
}

/* +0x14 put pos onto triangle i's plane (its y from x, z) */
/* 0x0017C9D0 */
void NavMesh_OntoPlane(NavMesh *nm, u32 i, f32 *pos) {
    f32 (*v)[4];
    f32 e1x, e1y, e1z, e2x, e2y, e2z, nx, ny, nz;

    if (i >= nm->numTris || nm->tris == NULL) {
        return;
    }
    v = nm->tris[i].v;
    e2x = v[2][0] - v[0][0];
    e2y = v[2][1] - v[0][1];
    e1y = v[1][1] - v[0][1];
    e1x = v[1][0] - v[0][0];
    e1z = v[1][2] - v[0][2];
    e2z = v[2][2] - v[0][2];
    nx = e1y * e2z - e2y * e1z;
    nz = e1x * e2y - e2x * e1y;
    ny = e1z * e2x - e2z * e1x;
    pos[1] = v[0][1] - ((pos[2] - v[0][2]) * nz + (pos[0] - v[0][0]) * nx) / ny;
}

/* +0x50 whether `pos` is at door region d (within 5 vertically and 20 across of either side's
 * edge midpoint) */
/* 0x0017B540 */
s32 NavMesh_AtDoorRegion(NavMesh *nm, s32 d, const f32 *pos) {
    f32 v[4] __attribute__((aligned(16)));
    s32 s;

    if (d < 0 || (u32)d >= nm->numDoors) {
        return 0;
    }
    for (s = 0; s < 2; s++) {
        f32 dy;

        sceVu0SubVector(v, (f32 *)pos, (f32 *)((u8 *)nm + 0x30 + d * 0x30 + s * 0x10));
        dy = v[1];
        if (dy <= 0.0f) {
            dy = -dy;
        }
        if (dy <= 5.0f && __builtin_sqrtf(v[2] * v[2] + v[0] * v[0]) <= 20.0f) {
            return 1;
        }
    }
    return 0;
}

/* walk from `from` in triangle `tri` towards `to`: crossing edges into the neighbours (vtable
 * +0x20: the edge left by, 3 still inside, 4 lost - then +0x64 finds the triangle under it);
 * at a wall (no neighbour, or one blocked by `mask`) the target slides along it (+0x28) and
 * the walk restarts, at most 4 times, then it is clamped to the wall (+0x24). The end point
 * (`out`, height fixed by +0x14) and its triangle (-1: off the mesh) */
/* 0x0017C050 */
s32 NavMesh_Walk(NavMesh *nm, s32 tri, f32 *out, f32 *from, f32 *to, u32 mask) {
    f32 p[4] __attribute__((aligned(16)));
    f32 q[4] __attribute__((aligned(16)));
    s32 cur = tri, prev;
    u32 slides = 0;

    sceVu0CopyVector(p, to);
    for (;;) {
        s32 edge = VCALL(nm, 0x20, s32 (*)(NavMesh *, s32, f32 *, f32 *))(nm, cur, from, p);
        s32 next;

        if (edge == 4) {
            prev = VCALL(nm, 0x64, s32 (*)(NavMesh *, s32, f32 *, u32))(nm, cur, p, mask);
            if (prev == -1) {
                cur = -1;
                break;
            }
            sceVu0CopyVector(out, p);
            VCALL(nm, 0x14, void (*)(NavMesh *, s32, f32 *))(nm, prev, out);
            return prev;
        }
        if (edge == 3) {
            sceVu0CopyVector(out, p);
            break;
        }
        prev = cur;
        next = nm->tris[cur].adj[edge];
        if (!(next & 0x80000000) && !(mask & nm->tris[next].flags)) {
            cur = next;
            continue;
        }
        if (slides >= 4) {
            VCALL(nm, 0x24, void (*)(NavMesh *, s32, f32 *, f32 *, f32 *))(nm, prev, q, from, p);
            if (VCALL(nm, 0x10, s32 (*)(NavMesh *, s32, f32 *))(nm, prev, q) == 4) {
                cur = -1;
            } else {
                sceVu0CopyVector(out, q);
                cur = prev;
            }
            break;
        }
        VCALL(nm, 0x28, void (*)(NavMesh *, s32, f32 *, f32 *, f32 *))(nm, prev, q, from, p);
        sceVu0CopyVector(p, q);
        cur = tri;
        slides++;
    }
    VCALL(nm, 0x14, void (*)(NavMesh *, s32, f32 *))(nm, cur, out);
    return cur;
}

extern s32 NavTri_ExitEdge(NavTri *t, f32 *from, f32 *to);   /* the edge the segment leaves by */

/* vtable +0x20: which edge of triangle `i` the step from -> to leaves by (3: stays inside),
 * 4 for no such triangle */
/* 0x0017C750 */
s32 NavMesh_ExitEdge(NavMesh *nm, u32 i, f32 *from, f32 *to) {
    if (i < nm->numTris && nm->tris != NULL) {
        return NavTri_ExitEdge(&nm->tris[i], from, to);
    }
    return 4;
}

/* which edge of triangle `t` (in x/z) the step from -> to leaves it by: 0..2, 3 if `to` is
 * inside, 4 if it is outside but the step crosses no edge */
/* 0x0017AE80 */
s32 NavTri_ExitEdge(NavTri *t, f32 *from, f32 *to) {
    f32 x2 = to[0], z2 = to[2];
    u32 inside = 0, tested = 0;
    u32 e;

    for (e = 0; e < 3; e++) {
        f32 *a = t->v[e];
        f32 *b = t->v[e < 2 ? e + 1 : 0];
        f32 ax = a[0], az = a[2];
        f32 ez = b[2] - az;
        f32 ex = b[0] - ax;
        f32 x1, z1, p, q, dx, dz, den, s, u;

        tested |= 1 << e;
        if (!((x2 - ax) * ez - (z2 - az) * ex < 0.0f)) {
            inside |= 1 << e;
            continue;
        }
        x1 = from[0];
        z1 = from[2];
        q = (z1 - az) * ex;
        p = (x1 - ax) * ez;
        if (p - q < 0.0f) {
            continue;
        }
        dx = x2 - x1;
        dz = z2 - z1;
        den = dx * ez - dz * ex;
        if (den == 0.0f) {
            continue;
        }
        s = (q - p) / den;
        if (s < 0.0f || !(s <= 1.0f)) {
            continue;
        }
        if ((ex <= 0.0f ? -ex : ex) <= (ez <= 0.0f ? -ez : ez)) {
            u = (z1 + s * dz - az) / ez;
        } else {
            u = (x1 + s * dx - ax) / ex;
        }
        if (u < 0.0f || !(u <= 1.0f)) {
            continue;
        }
        return e;
    }
    return inside == tested ? 3 : 4;
}

extern s32 NavTri_ExitEdgeHit(NavTri *t, f32 *hit, f32 *from, f32 *to);

/* vtable +0x24: as +0x20 (the edge of triangle `i` the step from -> to leaves by), also
 * giving where it crosses (`hit`); 4 for no such triangle */
/* 0x0017C6F0 */
s32 NavMesh_ExitEdgeHit(NavMesh *nm, u32 i, f32 *hit, f32 *from, f32 *to) {
    if (i < nm->numTris && nm->tris != NULL) {
        return NavTri_ExitEdgeHit(&nm->tris[i], hit, from, to);
    }
    return 4;
}

/* as NavTri_ExitEdge (the edge of triangle `t` the step from -> to leaves by; 3 inside, 4
 * outside crossing nothing), also giving the crossing point `hit` with its height on the
 * triangle's plane */
/* 0x0017ABB0 */
s32 NavTri_ExitEdgeHit(NavTri *t, f32 *hit, f32 *from, f32 *to) {
    f32 x2 = to[0], z2 = to[2];
    u32 inside = 0, tested = 0;
    u32 e;

    for (e = 0; e < 3; e++) {
        f32 *a = t->v[e];
        f32 *b = t->v[e < 2 ? e + 1 : 0];
        f32 ax = a[0], az = a[2];
        f32 ez = b[2] - az;
        f32 ex = b[0] - ax;
        f32 x1, z1, p, q, dx, dz, den, s, u;

        tested |= 1 << e;
        if (!((x2 - ax) * ez - (z2 - az) * ex < 0.0f)) {
            inside |= 1 << e;
            continue;
        }
        x1 = from[0];
        z1 = from[2];
        q = (z1 - az) * ex;
        p = (x1 - ax) * ez;
        if (p - q < 0.0f) {
            continue;
        }
        dx = x2 - x1;
        dz = z2 - z1;
        den = dx * ez - dz * ex;
        if (den == 0.0f) {
            continue;
        }
        s = (q - p) / den;
        if (s < 0.0f || !(s <= 1.0f)) {
            continue;
        }
        if ((ex <= 0.0f ? -ex : ex) <= (ez <= 0.0f ? -ez : ez)) {
            u = (z1 + s * dz - az) / ez;
        } else {
            u = (x1 + s * dx - ax) / ex;
        }
        if (u < 0.0f || !(u <= 1.0f)) {
            continue;
        }
        sceVu0SubVector(hit, to, from);
        func_0010E640(hit, hit, s);
        sceVu0AddVector(hit, from, hit);
        {
            f32 *v0 = t->v[0], *v1 = t->v[1], *v2 = t->v[2];
            f32 e2x = v2[0] - v0[0], e2y = v2[1] - v0[1], e1y = v1[1] - v0[1], e1x = v1[0] - v0[0];
            f32 e1z = v1[2] - v0[2], hx = hit[0] - v0[0], e2z = v2[2] - v0[2], hz = hit[2] - v0[2];
            f32 nx = e1y * e2z - e2y * e1z;
            f32 nz = e1x * e2y - e2x * e1y;
            f32 dot = hz * nz + hx * nx;
            f32 ny = e1z * e2x - e2z * e1x;

            hit[1] = v0[1] - dot / ny;
            AT(hit, 0xC, u32) = 0x3F800000;   /* 1.0 */
        }
        return e;
    }
    return inside == tested ? 3 : 4;
}

/* +0x2C triangle `i`'s unit normal into `n` (w 1), (0, 1, 0) for a bad index */
/* 0x0017C5C0 */
void NavMesh_TriNormal(NavMesh *nm, u32 i, f32 *n) {
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    NavTri *t;

    if (i >= nm->numTris || nm->tris == NULL) {
        n[0] = 0.0f;
        n[1] = 1.0f;
        n[2] = 0.0f;
        n[3] = 1.0f;
        return;
    }
    t = &nm->tris[i];
    n[0] = 0.0f;
    n[1] = 0.0f;
    n[2] = 1.0f;
    n[3] = 1.0f;
    sceVu0SubVector(a, t->v[1], t->v[0]);
    sceVu0SubVector(b, t->v[2], t->v[0]);
    a[3] = 1.0f;
    b[3] = 1.0f;
    sceVu0OuterProduct(n, a, b);
    sceVu0Normalize(n, n);
    n[3] = 1.0f;
}

u32 NavTri_Slide(NavTri *t, f32 *out, const f32 *from, const f32 *to);

/* +0x28 slide: for the step `from` -> `to` in triangle `i`, `out` = `to` pulled back inside
 * over the edge it leaves by (NavTri_Slide); the edge then (3: inside), 4 for a bad index */
/* 0x0017C690 */
u32 NavMesh_SlideIn(NavMesh *nm, u32 i, f32 *out, const f32 *from, const f32 *to) {
    if (i >= nm->numTris || nm->tris == NULL) {
        return 4;
    }
    return NavTri_Slide(&nm->tris[i], out, from, to);
}

/* the step `from` -> `to` in triangle `t`: the edge it leaves by (NavTri_ExitEdge; 3 inside), and
 * then `out` - `to` pulled back over that edge along its inward normal (1.1x the overshoot,
 * 0.05 more each try, up to 8, until it is inside), on the triangle's plane (w 1) - and the edge
 * test again for `from` -> `out` */
/* 0x0017A940 */
u32 NavTri_Slide(NavTri *t, f32 *out, const f32 *from, const f32 *to) {
    static const union { u32 u; f32 f; } k11 = {0x3F8CCCCD}, k005 = {0x3D4CCCCD};
    f32 e[4] __attribute__((aligned(16)));
    f32 c[4] __attribute__((aligned(16)));
    f32 n[4] __attribute__((aligned(16)));
    const f32 *a, *b;
    u32 k;
    s32 i;
    f32 over, s, nx, ny, nz;

    k = NavTri_ExitEdge(t, (f32 *)from, (f32 *)to);
    if (k >= 3) {
        return k;
    }
    a = t->v[k];
    b = t->v[k < 2 ? k + 1 : 0];
    sceVu0SubVector(e, b, a);
    sceVu0SubVector(c, to, a);
    c[3] = 1.0f;
    e[3] = 1.0f;
    c[1] = 0.0f;
    e[1] = 0.0f;
    sceVu0OuterProduct(c, e, c);
    sceVu0OuterProduct(n, e, c);
    sceVu0Normalize(n, n);
    sceVu0SubVector(e, a, to);
    e[3] = 1.0f;
    n[3] = 1.0f;
    e[1] = 0.0f;
    over = sceVu0InnerProduct(e, n);
    s = k11.f;
    for (i = 0; i < 8; i++) {
        func_0010E640(e, n, over * s);
        sceVu0AddVector(out, to, e);
        if (!((out[0] - a[0]) * (b[2] - a[2]) - (out[2] - a[2]) * (b[0] - a[0]) <= 0.0f)) {
            break;
        }
        s = s + k005.f;
    }
    nx = (t->v[1][1] - t->v[0][1]) * (t->v[2][2] - t->v[0][2]) - (t->v[2][1] - t->v[0][1]) * (t->v[1][2] - t->v[0][2]);
    nz = (t->v[1][0] - t->v[0][0]) * (t->v[2][1] - t->v[0][1]) - (t->v[2][0] - t->v[0][0]) * (t->v[1][1] - t->v[0][1]);
    ny = (t->v[1][2] - t->v[0][2]) * (t->v[2][0] - t->v[0][0]) - (t->v[2][2] - t->v[0][2]) * (t->v[1][0] - t->v[0][0]);
    out[1] = t->v[0][1] - ((out[2] - t->v[0][2]) * nz + (out[0] - t->v[0][0]) * nx) / ny;
    out[3] = 1.0f;
    return NavTri_ExitEdge(t, (f32 *)from, out);
}

/* +0x68: the neighbour of triangle t holding point p (not blocked by `mask`; +0x10 == 3:
 * inside), or -1 */
/* 0x0017B060 */
u32 NavMesh_NeighbourAt(NavMesh *nm, u32 t, const f32 *p, u32 mask) {
    s32 k;

    for (k = 0; k < 3; k++) {
        u32 n = nm->tris[t].adj[k];

        if (n & 0x80000000) {
            continue;
        }
        if (nm->tris[n].flags & mask) {
            continue;
        }
        if (VCALL(nm, 0x10, s32 (*)(NavMesh *, u32, const f32 *))(nm, n, p) == 3) {
            return n;
        }
    }
    return NAV_NONE;
}

/* the triangle holding point p (horizontally), searched from triangle t: t itself when it is
 * not blocked by `mask` (+0x10 == 3: inside), then +0x68 around t, around each neighbour of t
 * and around each of their neighbours; -1 if none */
/* 0x0017B160 */
u32 NavMesh_FindFrom(NavMesh *nm, u32 t, const f32 *p, u32 mask) {
    u32 (*near)(NavMesh *, u32, const f32 *, u32) = (u32 (*)(NavMesh *, u32, const f32 *, u32))nm->vtbl[0x68 / 4];
    u32 r;
    s32 k, j;

    if (t >= nm->numTris || nm->tris == NULL) {
        return NAV_NONE;
    }
    if (!(t & 0x80000000) && !(nm->tris[t].flags & mask) &&
        VCALL(nm, 0x10, s32 (*)(NavMesh *, u32, const f32 *))(nm, t, p) == 3) {
        return t;
    }
    r = near(nm, t, p, mask);
    if (r != NAV_NONE) {
        return r;
    }
    for (k = 0; k < 3; k++) {
        u32 n = nm->tris[t].adj[k];

        if (n & 0x80000000) {
            continue;
        }
        r = near(nm, n, p, mask);
        if (r != NAV_NONE) {
            return r;
        }
        for (j = 0; j < 3; j++) {
            u32 m = nm->tris[n].adj[j];

            if (m & 0x80000000) {
                continue;
            }
            r = near(nm, m, p, mask);
            if (r != NAV_NONE) {
                return r;
            }
        }
    }
    return NAV_NONE;
}

/* +0xC the triangle under (or over) point p: of those (not blocked by `mask`) holding it
 * horizontally (+0x10 == 3), the one whose height (+0x38) is nearest - at once if within
 * 0.001; -1 if none */
/* 0x0017C2A0 */
u32 NavMesh_FindTri(NavMesh *nm, const f32 *p, u32 mask) {
    static const union { u32 u; f32 f; } kEps = {0x3A83126F};
    f32 (*height)(NavMesh *, u32, const f32 *) = (f32 (*)(NavMesh *, u32, const f32 *))nm->vtbl[0x38 / 4];
    u32 best = NAV_NONE, t;
    f32 bestD = 0.0f, d;

    for (t = 0; t < nm->numTris; t++) {
        if (nm->tris[t].flags & mask) {
            continue;
        }
        if (VCALL(nm, 0x10, s32 (*)(NavMesh *, u32, const f32 *))(nm, t, p) != 3) {
            continue;
        }
        if (!(height(nm, t, p) <= 0.0f)) {
            d = height(nm, t, p);
        } else {
            d = -height(nm, t, p);
        }
        if (d < kEps.f) {
            return t;
        }
        if (best == NAV_NONE || !(bestD <= d)) {
            bestD = d;
            best = t;
        }
    }
    return best;
}

/* +0x38 point p's height over triangle t's plane (along its unit normal, v1 - v0 x v2 - v0);
 * 4 for no such triangle */
/* 0x0017C410 */
f32 NavMesh_HeightOver(NavMesh *nm, u32 t, const f32 *p) {
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    f32 n[4] __attribute__((aligned(16)));
    NavTri *tri;

    if (t >= nm->numTris || nm->tris == NULL) {
        return 4.0f;
    }
    tri = &nm->tris[t];
    sceVu0SubVector(a, tri->v[1], tri->v[0]);
    sceVu0SubVector(b, tri->v[2], tri->v[0]);
    a[3] = 1.0f;
    b[3] = 1.0f;
    sceVu0OuterProduct(n, a, b);
    n[3] = 1.0f;
    sceVu0Normalize(n, n);
    sceVu0SubVector(a, tri->v[0], p);
    a[3] = 1.0f;
    return sceVu0InnerProduct(a, n);
}

extern void *NavMesh_vtable[], *D_0046AA40[];

/* the nav mesh (NavMesh_vtable): its two tables (+0x4 / +0xC, with their counts) let go, then the
 * base (D_0046AA40, clearing gNavMesh) */
/* 0x00179F60 */
void *NavMesh_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = NavMesh_vtable;
        if (AT(o, 0x4, void *) != NULL) {
            AT(o, 0x4, void *) = NULL;
            AT(o, 0x8, s32) = 0;
        }
        if (AT(o, 0xC, void *) != NULL) {
            AT(o, 0xC, void *) = NULL;
            AT(o, 0x10, s32) = 0;
        }
        AT(o, 0x0, void **) = D_0046AA40;
        gNavMesh = NULL;
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* ---- door regions (+0x14 of them, 0x30 each from +0x20: the triangle on each side, the
 * facing on each side, the spot on each side) ---- */

#define NAV_DOOR(nm, i) ((u8 *)(nm) + 0x20 + (i) * 0x30)

static inline s32 nav_door_ok(NavMesh *nm, s32 i) {
    return (i >= 0 && (u32)i < nm->numDoors) ? 1 : 0;
}

/* +0x54 door region i's triangle on side s (0 / 1); -1 if none */
/* 0x0017A010 */
s32 NavMesh_DoorTri(NavMesh *nm, s32 i, s32 s) {
    if (!(nav_door_ok(nm, i) & 0xFF)) {
        return -1;
    }
    if (s < 0 || (u32)s >= 2) {
        return -1;
    }
    return AT(NAV_DOOR(nm, i), s * 4, s32);
}

/* +0x58 its facing on side s; 0 if none */
/* 0x0017A090 */
f32 NavMesh_DoorFacing(NavMesh *nm, s32 i, s32 s) {
    if (!(nav_door_ok(nm, i) & 0xFF)) {
        return 0.0f;
    }
    if (s < 0 || (u32)s >= 2) {
        return 0.0f;
    }
    return AT(NAV_DOOR(nm, i), 0x8 + s * 4, f32);
}

/* +0x5C its spot on side s into out; the triangle, -1 if none */
/* 0x0017A110 */
s32 NavMesh_DoorSpot(NavMesh *nm, s32 i, s32 s, f32 *out) {
    if (!(nav_door_ok(nm, i) & 0xFF)) {
        return -1;
    }
    if (s < 0 || (u32)s >= 2) {
        return -1;
    }
    sceVu0CopyVector(out, (f32 *)(NAV_DOOR(nm, i) + 0x10 + s * 0x10));
    return AT(NAV_DOOR(nm, i), s * 4, s32);
}

extern s32 NavTri_SegmentHit(NavTri *t, f32 *out, f32 *p0, f32 *p1);
extern f32 NavTri_Slope(NavTri *t, f32 *out);

/* +0x34 where segment p0 -> p1 meets triangle i (NavTri_SegmentHit); 4 if there is no such triangle */
/* 0x0017C4F0 */
s32 NavMesh_SegmentHit(NavMesh *nm, u32 i, f32 *out, f32 *p0, f32 *p1) {
    if (i < nm->numTris && nm->tris != NULL) {
        return NavTri_SegmentHit(&nm->tris[i], out, p0, p1);
    }
    return 4;
}

/* +0x30 triangle i's slope (NavTri_Slope, the way down into out); none: out (0, 0, 0, 1), 0 */
/* 0x0017C550 */
f32 NavMesh_TriSlope(NavMesh *nm, u32 i, f32 *out) {
    if (i < nm->numTris && nm->tris != NULL) {
        return NavTri_Slope(&nm->tris[i], out);
    }
    out[0] = 0.0f;
    out[1] = 0.0f;
    out[2] = 0.0f;
    out[3] = 1.0f;
    return 0.0f;
}

/* is p over the triangle (seen from above: on the inner side of all three edges)? 3 if so,
 * else 4 */
static inline __attribute__((always_inline)) s32 nav_tri_over(NavTri *t, const f32 *p) {
    s32 n = 0;
    u32 k;

    for (k = 0; k < 3; k++) {
        const f32 *a = t->v[k], *b = t->v[k + 1 < 3 ? k + 1 : 0];

        if (!((p[0] - a[0]) * (b[2] - a[2]) - (p[2] - a[2]) * (b[0] - a[0]) < 0.0f)) {
            n++;
        }
    }
    return n == 3 ? 3 : 4;
}

/* the height of the triangle's plane at (q[0], q[2]) */
static inline __attribute__((always_inline)) f32 nav_tri_height(NavTri *t, const f32 *q) {
    f32 dy1 = t->v[1][1] - t->v[0][1], dx2 = t->v[2][0] - t->v[0][0], dx1 = t->v[1][0] - t->v[0][0];
    f32 dy2 = t->v[2][1] - t->v[0][1], qx = q[0] - t->v[0][0];
    f32 dz1 = t->v[1][2] - t->v[0][2], dz2 = t->v[2][2] - t->v[0][2];
    f32 a = dx1 * dy2, b = dy1 * dz2, nx, nyz, qz, num, den;

    nx = 0.0f + b - dy2 * dz1;
    nyz = 0.0f + a - dx2 * dy1;
    qz = q[2] - t->v[0][2];
    num = 0.0f + qz * nyz + qx * nx;
    den = 0.0f + dz1 * dx2 - dz2 * dx1;
    return t->v[0][1] - num / den;
}

/* where the segment p0 -> p1 meets the triangle's plane (into out): 3 if that is over the
 * triangle, else 4 (also for a zero segment or a crossing outside it). A segment along the
 * plane, or starting on it, is taken where the plane's height matches the far / near end
 * (the height looked up at v0 - p0, as the original does) */
/* 0x0017A1D0 */
s32 NavTri_SegmentHit(NavTri *t, f32 *out, f32 *p0, f32 *p1) {
    f32 e1[4] __attribute__((aligned(16)));
    f32 e2[4] __attribute__((aligned(16)));
    f32 n[4] __attribute__((aligned(16)));
    f32 dn, d0, y;

    if (p0[0] == p1[0] && p0[1] == p1[1] && p0[2] == p1[2]) {
        return 4;
    }
    sceVu0SubVector(e1, t->v[1], t->v[0]);
    sceVu0SubVector(e2, t->v[2], t->v[0]);
    e1[3] = 1.0f;
    e2[3] = 1.0f;
    sceVu0OuterProduct(n, e1, e2);
    n[3] = 1.0f;
    sceVu0Normalize(n, n);
    n[3] = 1.0f;
    sceVu0SubVector(e1, p1, p0);
    e1[3] = 1.0f;
    dn = sceVu0InnerProduct(e1, n);
    sceVu0SubVector(e2, t->v[0], p0);
    e2[3] = 1.0f;
    d0 = sceVu0InnerProduct(e2, n);
    if (dn == 0.0f) {
        y = nav_tri_height(t, e2);
        e2[1] = y;
        if (!(y == p1[1])) {
            return 4;
        }
        sceVu0CopyVector(out, p1);
        return nav_tri_over(t, out);
    }
    if (d0 == 0.0f) {
        y = nav_tri_height(t, e2);
        e2[1] = y;
        if (!(y == p0[1])) {
            return 4;
        }
        sceVu0CopyVector(out, p0);
        return nav_tri_over(t, out);
    }
    {
        f32 s = d0 / dn;

        if (s < 0.0f || !(s <= 1.0f)) {
            return 4;
        }
        func_0010E640(out, e1, s);
        sceVu0AddVector(out, p0, out);
    }
    return nav_tri_over(t, out);
}

/* the triangle's slope: the sine of its steepness, and (out) the way down, scaled by it */
/* 0x0017A6D0 */
f32 NavTri_Slope(NavTri *t, f32 *out) {
    f32 c[4] __attribute__((aligned(16)));
    f32 a[4] __attribute__((aligned(16)));
    f32 b[4] __attribute__((aligned(16)));
    f32 n[4] __attribute__((aligned(16)));
    f32 s[4] __attribute__((aligned(16)));
    f32 k = 0.0f;

    out[0] = 0.0f;
    out[1] = 0.0f;
    out[2] = 0.0f;
    out[3] = 1.0f;
    c[0] = (t->v[2][0] + (t->v[1][0] + t->v[0][0])) / 3.0f;
    c[1] = (t->v[2][1] + (t->v[1][1] + t->v[0][1])) / 3.0f;
    c[2] = (t->v[2][2] + (t->v[1][2] + t->v[0][2])) / 3.0f;
    c[3] = 1.0f;
    sceVu0SubVector(a, t->v[1], t->v[0]);
    sceVu0SubVector(b, t->v[2], t->v[0]);
    a[3] = 1.0f;
    b[3] = 1.0f;
    sceVu0OuterProduct(n, a, b);
    sceVu0Normalize(n, n);
    sceVu0AddVector(a, c, n);
    a[3] = 1.0f;
    a[1] = 1.0f;
    a[0] = 0.0f;
    a[2] = 0.0f;
    sceVu0OuterProduct(s, a, n);
    if (__builtin_sqrtf(s[1] * s[1] + s[0] * s[0] + s[2] * s[2]) != 0.0f) {
        sceVu0Normalize(s, s);
        sceVu0AddVector(a, c, s);
        a[3] = 1.0f;
        sceVu0OuterProduct(out, s, n);
        if (__builtin_sqrtf(out[1] * out[1] + out[0] * out[0] + out[2] * out[2]) != 0.0f) {
            f32 d;

            sceVu0Normalize(out, out);
            sceVu0AddVector(a, c, out);
            a[3] = 1.0f;
            a[0] = out[0];
            a[1] = 0.0f;
            a[2] = out[2];
            a[3] = 1.0f;
            sceVu0Normalize(a, a);
            d = sceVu0InnerProduct(a, out);
            k = __builtin_sqrtf(0.0f + 1.0f - d * d);
        }
    }
    func_0010E640(out, out, k);
    return k;
}

/* the same height without the zero term (mula / msub: a -0 product stays -0) */
static inline __attribute__((always_inline)) f32 nav_plane_y(NavTri *t, const f32 *q) {
    f32 dx2 = t->v[2][0] - t->v[0][0], dy2 = t->v[2][1] - t->v[0][1], dy1 = t->v[1][1] - t->v[0][1];
    f32 dx1 = t->v[1][0] - t->v[0][0], dz1 = t->v[1][2] - t->v[0][2], qx = q[0] - t->v[0][0];
    f32 dz2 = t->v[2][2] - t->v[0][2], nx, nyz, qz, num, den;

    nx = dy1 * dz2 - dy2 * dz1;
    qz = q[2] - t->v[0][2];
    nyz = dx1 * dy2 - dx2 * dy1;
    num = qz * nyz + qx * nx;
    den = dz1 * dx2 - dz2 * dx1;
    return t->v[0][1] - num / den;
}

/* +0x1C put p on triangle i (p[1]): its highest corner if it is a step / ledge (flags & 3),
 * else its plane's height at p */
/* 0x0017C7B0 */
void NavMesh_PutOnHigh(NavMesh *nm, u32 i, f32 *p) {
    NavTri *t;

    if (!(i < nm->numTris) || nm->tris == NULL) {
        return;
    }
    t = &nm->tris[i];
    if (t->flags & 3) {
        f32 y = t->v[0][1] < t->v[1][1] ? t->v[1][1] : t->v[0][1];

        p[1] = y < t->v[2][1] ? t->v[2][1] : y;
        return;
    }
    p[1] = nav_plane_y(t, p);
}

/* +0x18 the same with its lowest corner */
/* 0x0017C8C0 */
void NavMesh_PutOnLow(NavMesh *nm, u32 i, f32 *p) {
    NavTri *t;

    if (!(i < nm->numTris) || nm->tris == NULL) {
        return;
    }
    t = &nm->tris[i];
    if (t->flags & 3) {
        f32 y = t->v[0][1] <= t->v[1][1] ? t->v[0][1] : t->v[1][1];

        p[1] = y <= t->v[2][1] ? y : t->v[2][1];
        return;
    }
    p[1] = nav_plane_y(t, p);
}

/* +0x60 which side (0 / 1) of door region i p is at: within 5 in height and 12 across of that
 * side's spot, with a straight walk over the mesh from triangle tri to it (+0x20 step by step
 * across edges) ending on the spot's triangle; -1 if neither */
/* 0x0017B380 */
s32 NavMesh_DoorSide(NavMesh *nm, s32 i, u32 tri, f32 *p) {
    u8 *door;
    s32 s;

    if (i < 0 || !((u32)i < nm->numDoors)) {
        return -1;
    }
    door = NAV_DOOR(nm, i);
    for (s = 0; s < 2; s++) {
        f32 d[4] __attribute__((aligned(16)));
        f32 at[4] __attribute__((aligned(16)));
        f32 *spot = (f32 *)(door + 0x10 + s * 0x10);
        f32 dy;
        u32 t;

        sceVu0SubVector(d, p, spot);
        dy = d[1] <= 0.0f ? -d[1] : d[1];
        if (!(dy <= 5.0f) || !(__builtin_sqrtf(d[2] * d[2] + d[0] * d[0]) <= 12.0f)) {
            continue;
        }
        sceVu0CopyVector(at, p);
        t = tri;
        for (;;) {
            s32 e = VCALL(nm, 0x20, s32 (*)(NavMesh *, u32, f32 *, f32 *))(nm, t, at, spot);

            if (e == 3) {
                if (t == AT(door, s * 4, u32)) {
                    return s;
                }
                break;
            }
            if (e == 4) {
                break;
            }
            t = nm->tris[t].adj[e];
        }
    }
    return -1;
}

/* the mesh's outer edges (+0xC, +0x10 of them, 16 bytes each: the triangle, -, its two corners) */
#define NAV_EDGE(nm, i) ((u32 *)(AT(nm, 0xC, u8 *) + (i) * 0x10))

/* +0x48 the outer edge the step p -> q goes out through (seen from above: p inside it, q
 * outside, the crossing within both); of several, the one whose triangle's height under the
 * crossing is closest below p. -1 if none */
/* 0x0017B9B0 */
s32 NavMesh_OuterEdge(NavMesh *nm, f32 *p, f32 *q) {
    s32 best = -1;
    u32 *cand = func_00114FA8(AT(nm, 0x10, u32) * 4);
    u32 n = 0, i;
    f32 gap;

    for (i = 0; i < AT(nm, 0x10, u32); i++) {
        u32 *e = NAV_EDGE(nm, i);
        NavTri *t = &nm->tris[e[0]];
        f32 ax = t->v[e[2]][0], az = t->v[e[2]][2];
        f32 ez = t->v[e[3]][2] - az, ex = t->v[e[3]][0] - ax;
        f32 m = (p[0] - ax) * ez, l = (p[2] - az) * ex;
        f32 dx, dz, den, s, u;

        if (m - l < 0.0f) {
            continue;
        }
        if (!(0.0f + (q[0] - ax) * ez - (q[2] - az) * ex < 0.0f)) {
            continue;
        }
        dx = q[0] - p[0];
        dz = q[2] - p[2];
        den = 0.0f + dx * ez - dz * ex;
        if (den == 0.0f) {
            continue;
        }
        s = (l - m) / den;
        if (s < 0.0f || !(s <= 1.0f)) {
            continue;
        }
        if ((ex <= 0.0f ? -ex : ex) <= (ez <= 0.0f ? -ez : ez)) {
            u = (0.0f + p[2] + s * dz - az) / ez;
        } else {
            u = (0.0f + p[0] + s * dx - ax) / ex;
        }
        if (u < 0.0f || !(u <= 1.0f)) {
            continue;
        }
        cand[n++] = i;
    }
    gap = -1.0f;
    for (i = 0; i < n; i++) {
        f32 hit[4] __attribute__((aligned(16)));
        NavTri *t = &nm->tris[NAV_EDGE(nm, cand[i])[0]];
        f32 y;

        NavTri_ExitEdgeHit(t, hit, p, q);
        y = nav_plane_y(t, hit);
        hit[1] = y;
        if (p[1] < y) {
            continue;
        }
        if (gap == -1.0f || !(gap <= p[1] - y)) {
            best = cand[i];
            gap = p[1] - y;
        }
    }
    func_00114FD0(cand);
    return best;
}

/* +0x44 follow the segment from -> to over the mesh from triangle tri, edge by edge (+0x20),
 * not into triangles with any of `mask`'s flags or off the mesh:
 *   it meets a triangle's surface (+0x34): the point into out, that triangle's normal (+0x2C)
 *     into normal; the triangle | 0x80000000
 *   it ends inside a triangle: out = to, its normal; the triangle
 *   it leaves the mesh: +0x64 (tri, to, mask) - if that finds one, out = to and its result;
 *     else -1
 *   it hits a wall (a blocked edge): where on the edge's wall (the edge and 100 below it) into
 *     out, the wall's normal; the last triangle | 0x40000000 */
/* 0x0017BD60 */
s32 NavMesh_Follow(NavMesh *nm, u32 tri, f32 *out, f32 *from, f32 *to, f32 *normal, u32 mask) {
    f32 end[4] __attribute__((aligned(16)));
    f32 hit[4] __attribute__((aligned(16)));

    sceVu0CopyVector(end, to);
    for (;;) {
        s32 e;
        u32 prev, next;

        if (VCALL(nm, 0x34, s32 (*)(NavMesh *, u32, f32 *, f32 *, f32 *))(nm, tri, hit, from, end) == 3) {
            sceVu0CopyVector(out, hit);
            VCALL(nm, 0x2C, void (*)(NavMesh *, u32, f32 *))(nm, tri, normal);
            return tri | 0x80000000;
        }
        e = VCALL(nm, 0x20, s32 (*)(NavMesh *, u32, f32 *, f32 *))(nm, tri, from, end);
        if (e == 4) {
            s32 r = VCALL(nm, 0x64, s32 (*)(NavMesh *, u32, f32 *, u32))(nm, tri, end, mask);

            if (r == -1) {
                return -1;
            }
            sceVu0CopyVector(out, end);
            return r;
        }
        if (e == 3) {
            sceVu0CopyVector(out, end);
            VCALL(nm, 0x2C, void (*)(NavMesh *, u32, f32 *))(nm, tri, normal);
            return tri;
        }
        prev = tri;
        next = nm->tris[tri].adj[e];
        tri = next;
        if (!(next & 0x80000000) && !(mask & nm->tris[next].flags)) {
            continue;
        }
        {
            NavTri wall;
            NavTri *t = prev < nm->numTris && nm->tris != NULL ? &nm->tris[prev] : NULL;
            u32 k = e + 1 < 3 ? e + 1 : 0;
            f32 a[4] __attribute__((aligned(16)));
            f32 b[4] __attribute__((aligned(16)));

            sceVu0CopyVector(wall.v[0], t->v[e]);
            sceVu0CopyVector(wall.v[1], t->v[k]);
            sceVu0CopyVector(wall.v[2], t->v[k]);
            wall.v[2][1] = wall.v[2][1] - 100.0f;
            NavTri_SegmentHit(&wall, out, from, end);
            normal[0] = 0.0f;
            normal[1] = 0.0f;
            normal[2] = 1.0f;
            normal[3] = 1.0f;
            sceVu0SubVector(a, wall.v[1], wall.v[0]);
            sceVu0SubVector(b, wall.v[2], wall.v[0]);
            a[3] = 1.0f;
            b[3] = 1.0f;
            sceVu0OuterProduct(normal, a, b);
            sceVu0Normalize(normal, normal);
            normal[3] = 1.0f;
            return prev | 0x40000000;
        }
    }
}

#ifdef HG_NATIVE
#include <string.h>

/* (see NavMesh_Tri) the blank record an out-of-range triangle reads on PC */
NavTri *NavMesh_NullTri(void) {
    static NavTri blank;

    memset(&blank, 0, sizeof(blank));
    blank.adj[0] = NAV_NONE;
    blank.adj[1] = NAV_NONE;
    blank.adj[2] = NAV_NONE;
    return &blank;
}
#endif
