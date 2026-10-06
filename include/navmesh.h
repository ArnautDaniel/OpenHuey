/* Walkable-area triangle mesh used for movement and line-of-walk tests (global D_0044E570). */
#ifndef NAVMESH_H
#define NAVMESH_H

#include "common.h"

typedef struct NavTri {
    /* 0x00 */ f32 v[3][4];      /* corners */
    /* 0x30 */ u32 adj[3];       /* neighbour across each edge, -1 = none */
    /* 0x3C */ u32 flags;        /* blocking attributes, tested against a caller's mask */
    /* 0x40 */ u8 pad40[0x10];
} NavTri;

typedef struct NavMesh {
    /* 0x00 */ void **vtbl;      /* +0xC, +0x20 edge test, +0x50 */
    /* 0x04 */ NavTri *tris;
    /* 0x08 */ u32 numTris;
    /* 0x0C */ u8 pad0C[8];
    /* 0x14 */ u32 numDoors;      /* door regions: +0x50 occupied?(i), +0x58 angle(i, side), +0x5C pos(i, side, out) -> tri */
} NavMesh;

extern NavMesh *D_0044E570;

#define NAV_NONE 0xFFFFFFFFu

#ifdef HG_NATIVE
/* PC: what an out-of-range triangle reads. The original reads the NULL record (PS2 low memory,
 * e.g. its flags at address 0x3C) without a check; that faults on PC, so it gets a blank record
 * instead: no corners, no neighbours, no flags (re-blanked on each use, as callers may write it) */
extern NavTri *NavMesh_NullTri(void);
#endif

/* triangle `i`, or NULL if out of range (PC: a blank record) */
static inline NavTri *NavMesh_Tri(NavMesh *nm, u32 i) {
#ifdef HG_NATIVE
    return (i < nm->numTris && nm->tris != NULL) ? &nm->tris[i] : NavMesh_NullTri();
#else
    return (i < nm->numTris && nm->tris != NULL) ? &nm->tris[i] : NULL;
#endif
}

/* flags of triangle `i` (0 if out of range) */
static inline u32 NavMesh_TriFlags(NavMesh *nm, u32 i) {
    return (i < nm->numTris && nm->tris != NULL) ? nm->tris[i].flags : 0;
}

/* triangles `a` and `b` on the two sides of a room's divider (flags 0x100000 / 0x200000) */
static inline s32 NavMesh_AcrossDivider(NavMesh *nm, u32 a, u32 b) {
    u32 fa = NavMesh_TriFlags(nm, a) & 0x300000, fb = NavMesh_TriFlags(nm, b) & 0x300000;

    return (fa == 0x100000 && fb == 0x200000) || (fa == 0x200000 && fb == 0x100000);
}

#endif
