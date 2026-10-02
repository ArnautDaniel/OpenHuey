/* The nav mesh (room manager +0x3E0, D_0044E570): the room's walkable triangles (PAC section
 * 0) with their flags (section 16), section 1, and the door regions (pairs of triangles with a
 * door group in their flags). */
#include "common.h"
#include "game.h"
#include "navmesh.h"

#define MESH_SIZE 0x50

/* take the room's meshes (`meshes`: count, then entries from +0x10; their bounds get the
 * float low bit cleared), the per-mesh flag bytes (`flags`, may be NULL: bit 0 -> mesh flag
 * 0x4000, bit 1 -> 0x80000) and section `sec1`. Returns the mesh count (-1: no meshes). */
s32 func_0017CC00(u8 *set, u8 *meshes, u8 *sec1, u8 *flags) {
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

#include "sce/libvu0.h"

extern void func_0010E640(f32 *out, const f32 *v, f32 s);   /* libvu0: scale x, y, z */
extern f32 func_0031C5C0(f32 x, f32 z);                      /* heading of (x, z) */

#define TRI(set, i) (AT(set, 0x4, u8 *) + (i) * MESH_SIZE)
#define LINK(set, g) ((set) + 0x20 + (g) * 0x30)

/* +0x4C find the door regions: triangles whose flags carry a link group (bits 9..13) pair up (the
 * higher one first; up to 5 links at +0x20, 0x30 each). For each side of a link: the edge
 * facing the other triangle (vt+0x20), its midpoint (+0x10 / +0x20) and the heading across
 * it (+0x8 / +0xC). +0x14: the number of links. */
void func_0017B660(u8 *set) {
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
void func_0017CB60(NavMesh *nm, u32 i, f32 *out) {
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
void func_0017C9D0(NavMesh *nm, u32 i, f32 *pos) {
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
s32 func_0017B540(NavMesh *nm, s32 d, const f32 *pos) {
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
s32 func_0017C050(NavMesh *nm, s32 tri, f32 *out, f32 *from, f32 *to, u32 mask) {
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


extern s32 func_0017AE80(NavTri *t, f32 *from, f32 *to);   /* the edge the segment leaves by */

/* vtable +0x20: which edge of triangle `i` the step from -> to leaves by (3: stays inside),
 * 4 for no such triangle */
s32 func_0017C750(NavMesh *nm, u32 i, f32 *from, f32 *to) {
    if (i < nm->numTris && nm->tris != NULL) {
        return func_0017AE80(&nm->tris[i], from, to);
    }
    return 4;
}


/* which edge of triangle `t` (in x/z) the step from -> to leaves it by: 0..2, 3 if `to` is
 * inside, 4 if it is outside but the step crosses no edge */
s32 func_0017AE80(NavTri *t, f32 *from, f32 *to) {
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


extern s32 func_0017ABB0(NavTri *t, f32 *hit, f32 *from, f32 *to);

/* vtable +0x24: as +0x20 (the edge of triangle `i` the step from -> to leaves by), also
 * giving where it crosses (`hit`); 4 for no such triangle */
s32 func_0017C6F0(NavMesh *nm, u32 i, f32 *hit, f32 *from, f32 *to) {
    if (i < nm->numTris && nm->tris != NULL) {
        return func_0017ABB0(&nm->tris[i], hit, from, to);
    }
    return 4;
}

/* as func_0017AE80 (the edge of triangle `t` the step from -> to leaves by; 3 inside, 4
 * outside crossing nothing), also giving the crossing point `hit` with its height on the
 * triangle's plane */
s32 func_0017ABB0(NavTri *t, f32 *hit, f32 *from, f32 *to) {
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
void func_0017C5C0(NavMesh *nm, u32 i, f32 *n) {
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
