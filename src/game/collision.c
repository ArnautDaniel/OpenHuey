/* Collision mesh queries. */
#include "common.h"

typedef struct { f32 x, y, z, w; } Vec4;
typedef struct { Vec4 v[3]; u32 pad[8]; } Tri;          /* 0x50 bytes */
typedef struct { u32 unk0; Tri *tris; u32 count; } TriMesh;

/* 0x0017CA80 */
s32 NavMesh_PointInTri(TriMesh *mesh, u32 index, Vec4 *p) {
    Tri *tri;
    s32 inside = 0;
    u32 i;

    if (index >= mesh->count || mesh->tris == 0) {
        return 4;
    }
    tri = &mesh->tris[index];
    for (i = 0; i < 3; i++) {
        s32 j = (s32)(i + 1) < 3 ? (s32)(i + 1) : 0;
        Vec4 *a = &tri->v[i];
        Vec4 *b = &tri->v[j];
        f32 d = (p->x - a->x) * (b->z - a->z) - (p->z - a->z) * (b->x - a->x);
        if (!(d < 0.0f)) {
            inside++;
        }
    }
    return inside == 3 ? 3 : 4;
}
