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
    /* 0x14 */ u32 unk14;        /* count of something iterated with vtable +0x50 */
} NavMesh;

extern NavMesh *D_0044E570;

#define NAV_NONE 0xFFFFFFFFu

/* triangle `i`, or NULL if out of range */
static inline NavTri *NavMesh_Tri(NavMesh *nm, u32 i) {
    return (i < nm->numTris && nm->tris != NULL) ? &nm->tris[i] : NULL;
}

/* flags of triangle `i` (0 if out of range) */
static inline u32 NavMesh_TriFlags(NavMesh *nm, u32 i) {
    return (i < nm->numTris && nm->tris != NULL) ? nm->tris[i].flags : 0;
}

#endif
