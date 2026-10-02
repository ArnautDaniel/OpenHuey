/* The room's mesh set (room manager +0x3E0): the meshes of the current room (PAC section 0,
 * 0x50-byte entries) with their per-mesh flags (section 16) and section 1. */
#include "common.h"
#include "game.h"

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

/* +0x4C find the links: triangles whose flags carry a link group (bits 9..13) pair up (the
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
