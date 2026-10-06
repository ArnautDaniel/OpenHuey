/* The nav mesh (room manager +0x3E0, gNavMesh): the room's walkable triangles (PAC section
 * 0) with their flags (section 16), section 1, and the door regions (pairs of triangles with a
 * door group in their flags).
 *
 * (was collision.c) Collision mesh queries.
 *
 * (was pathplan.c) The path planner (SceneGame +0xF29740, gSceneGameF29740, vtable
 * PathPlan_vtable): A over the nav mesh's triangles for the characters (Hewie, the stalkers, the
 * event characters). Four searches (+0x10, 0x10060 bytes each; +0x4 the mask of the ones in use,
 * search 3 the one taken when none is asked for). A search: +0x0 steps taken +0x4 start triangle,
 * +0x8 goal triangle, +0xC the triangle flags that stop it +0x10 the triangle flags that end it
 * (0, 4: as the goal) +0x20 start point, +0x30 goal point +0x40 open list length (s16), +0x42
 * closed list length (s16) +0x44 a node per triangle (0x18 bytes): +0 flags (1 open, 2 closed, 4
 * at the goal, 8), +2 its place in the open list, +4 the node it was reached from, +8 the next in
 * a list, +0xC the cost so far, +0x10 the estimate to the goal, +0x14 A triangle's +0x40 holds
 * the cost of crossing each of its edges. +0xC044 the open list (node pointers, sorted by cost),
 * +0xE044 the closed list +0x10044 the node being expanded, +0x10048 the path's end node +0x1004C
 * the step function (PTMF; it returns nonzero when the search is over) +0x10058 the nearest
 * triangle found when the goal can't be reached, +0x1005C its distance +0x40190 the found path's
 * length (triangles), +0x40198 its triangles (u16, start first) +0x40194 the smoothed path's
 * length, +0x41198 its triangles (u16)
 */
#include "common.h"
#include "game.h"
#include "navmesh.h"
#include "globals.h"
#include "memcard.h"
#include "libc.h"
#include "msl.h"
#include "ptmf.h"
#include "progress.h"
#include "pursuer.h"
#include "effectmgr.h"
#include "renderer.h"
#include "charaction.h"
#include "gl2d.h"
#include "music.h"
#include "camera.h"
#include "heap.h"
#include "char_load.h"
#include "creature.h"
#include "doors.h"
#include "event.h"
#include "gameover.h"
#include "hewie.h"
#include "items.h"
#include "loader.h"
#include "model.h"
#include "movie.h"
#include "fiona.h"
#include "pause.h"
#include "placed.h"
#include "room_map.h"
#include "system.h"
#include "scene.h"
#include "scene_game.h"
#include "scene_title.h"
#include "draw_leaves.h"
#include "sce/eekernel.h"
#include "effects.h"
#include "sound.h"
#include "vecmath.h"
#include "daniella.h"
#include "pad.h"
#include "scene_boot.h"
#include "sce/iop.h"
#include "cri/adx.h"
#include "actor.h"
#include "subscreen.h"
#include "text.h"
#include "input.h"
#include "director.h"
#include "room.h"
#include "lights.h"
#include "sce/intc.h"
#include "sce/libvu0.h"
#ifdef HG_NATIVE
#include "glr.h"
#endif

#define MESH_SIZE 0x50

extern void *D_0046AA40[];
void *NavMeshBase_dtor(u8 *o, s32 flags);

typedef struct { f32 x, y, z, w; } Vec4;

typedef struct { Vec4 v[3]; u32 pad[8]; } Tri;          /* 0x50 bytes */

typedef struct { u32 unk0; Tri *tris; u32 count; } TriMesh;

s32 NavMesh_PointInTri(TriMesh *mesh, u32 index, Vec4 *p);

extern void *PathPlan_vtable[], *D_0046AC00[];
extern VObject *gSceneGameF29740;
extern const u32 PathPlan_StepBreadth_ptmf[];   /* the step functions (PTMFs, 16-byte aligned) by request kind 0..7; 8: at the goal */
#define SEARCH(pl, i) ((u8 *)(pl) + 0x10 + (i) * 0x10060)

#define NODE(s, t) ((s) + 0x44 + (t) * 0x18)

#define NODE_INDEX(s, n) ((u32)((u8 *)(n) - ((s) + 0x44)) / 0x18)

extern void *NavGroups_vtable[];
extern void *NavMesh_vtable[];
extern void *gRoomEventObj;
#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

void *NavGroups_ctor(u8 *p);
void *NavMesh_ctor(u8 *p);

extern void *D_0046DB60[];
/* the nav mesh triangle t's flag word (NULL->flags, as the original, past the end) */
static u32 *tri_flags_word(u32 t) {
    u8 *tri = t < AT(gNavMesh, 0x8, u32) && AT(gNavMesh, 0x4, u8 *) != NULL
                  ? AT(gNavMesh, 0x4, u8 *) + t * 0x50 : NULL;

    return &AT(tri, 0x3C, u32);
}

void NavMesh_DropPending(u8 *p);
s32 NavGroups_SetFlags(u8 *o, s32 clear, u32 g, u32 bits);
s32 NavGroups_SetGroup(u8 *o, u32 g, u32 bits);
s32 NavGroups_ClearGroup(u8 *o, u32 g, u32 bits);
s32 NavGroups_SetTri(u8 *o, u32 t, u32 bits);
s32 NavGroups_ClearTri(u8 *o, u32 t, u32 bits);
s32 NavGroups_Holds(u8 *o, u32 t, u32 g);
void *NavGroupsBase_dtor(void *o, s32 flags);

/* a search's step: (planner->*step)(search) */
static inline s32 search_step(u8 *pl, u8 *s) {
    const PTMF *p = &AT(s, 0x1004C, PTMF);
    char *obj = (char *)pl + p->this_delta;
    s32 (*fn)(void *, u8 *);

    if (p->vtbl_offset < 0) {
        fn = (s32 (*)(void *, u8 *))p->u.func;
    } else {
        char *vtbl = *(char **)(obj + p->u.vptr_offset);
        fn = *(s32 (**)(void *, u8 *))(vtbl + p->vtbl_offset);
    }
    return fn(obj, s);
}

/* a search's step function = entry k of the table */
static inline void set_step(u8 *s, s32 k) {
    AT(s, 0x1004C, u32) = PathPlan_StepBreadth_ptmf[k * 4 + 0];
    AT(s, 0x10050, u32) = PathPlan_StepBreadth_ptmf[k * 4 + 1];
    AT(s, 0x10054, u32) = PathPlan_StepBreadth_ptmf[k * 4 + 2];
}

/* the centre of triangle `t` (nav mesh +0xC) */
static inline void tri_centre(NavMesh *nm, u32 t, f32 *out) {
    VCALL((VObject *)nm, 0xC, void (*)(VObject *, u32, f32 *))((VObject *)nm, t, out);
}

/* triangle `t`'s flags as the original reads them (a bad index reads through NULL) */
static inline u32 tri_flags(NavMesh *nm, u32 t) {
    return NavMesh_TriFlags(nm, t);
}

#define OPEN(s, i) AT(s, 0xC044 + (i) * 4, u8 *)

/* put node `n` at place `i` of the open list (its +2 too) */
#define OPEN_PUT(s, i, n) (OPEN(s, i) = (n), AT(OPEN(s, i), 0x2, s16) = (i))

/* an open node `nb` reached cheaper (its key at `key` lowered): moved up the list to its place */
static inline __attribute__((always_inline)) void open_raise(u8 *s, u8 *nb, s32 key) {
    s32 pos = AT(nb, 0x2, s16);

    if (pos == 1) {
        return;
    }
    if (AT(nb, key, f32) < AT(OPEN(s, 1), key, f32)) {
        for (; pos >= 2; pos--) {
            OPEN_PUT(s, pos, OPEN(s, pos - 1));
        }
    } else {
        while (AT(nb, key, f32) < AT(OPEN(s, pos - 1), key, f32)) {
            OPEN_PUT(s, pos, OPEN(s, pos - 1));
            pos--;
        }
    }
    OPEN_PUT(s, pos, nb);
}

/* the open list (`n`, its head just expanded) after the `k` new nodes, sorted by `sort`, are
 * merged in by `key` (places kept at +2): none - the head goes; else the best replaces the
 * head, found with the goal node as a sentinel past the end (its key set above it) */
static inline __attribute__((always_inline)) void open_merge(u8 *s, u8 **nbs, s32 k, s32 n, s32 sort, s32 key) {
    s32 i, j;

    if (k == 0) {
        for (i = 0; i < n - 1; i++) {
            OPEN_PUT(s, i, OPEN(s, i + 1));
        }
        return;
    }
    for (i = 1; i < k; i++) {
        for (j = 0; j < k - i; j++) {
            if (!(AT(nbs[j], sort, f32) <= AT(nbs[j + 1], sort, f32))) {
                u8 *x = nbs[j];

                nbs[j] = nbs[j + 1];
                nbs[j + 1] = x;
            }
        }
    }
    if (n == 1) {
        for (i = 0; i < k; i++) {
            OPEN_PUT(s, i, nbs[i]);
        }
    } else {
        s32 at = n - 1;
        u8 *g;

        for (i = k - 1; i > 0; i--) {   /* the rest, merged in from the end */
            u8 *x = nbs[i];

            if (AT(x, key, f32) < AT(OPEN(s, 1), key, f32)) {
                for (; at > 0; at--) {
                    OPEN_PUT(s, at + i, OPEN(s, at));
                }
            } else {
                while (AT(x, key, f32) < AT(OPEN(s, at), key, f32)) {
                    OPEN_PUT(s, at + i, OPEN(s, at));
                    at--;
                }
            }
            OPEN_PUT(s, at + i, x);
        }
        g = NODE(s, AT(s, 0x8, s32));
        OPEN(s, n + k - 1) = g;
        AT(g, key, f32) = 1.0f + AT(nbs[0], key, f32);
        for (i = 0; !(AT(nbs[0], key, f32) <= AT(OPEN(s, i + 1), key, f32)); i++) {
            OPEN_PUT(s, i, OPEN(s, i + 1));
        }
        OPEN_PUT(s, i, nbs[0]);
    }
}

/* the walk along the line from `from` (in triangle `t`) to `to` through the nav mesh (+0x20 the
 * edge it leaves by, 3 when it ends inside, 4 off the mesh): the triangle it ends in, or -1 when
 * it leaves the mesh or crosses one with flags of `stop` */
static inline __attribute__((always_inline)) u32 line_walk(NavMesh *nm, u32 t, f32 *from, f32 *to, u32 stop,
                                                           s32 *end) {
    for (;;) {
        s32 e = VCALL((VObject *)nm, 0x20, s32 (*)(VObject *, u32, f32 *, f32 *))((VObject *)nm, t, from, to);

        if (e == 3) {
            *end = 3;
            return t;
        }
        if (e == 4) {
            *end = 4;
            return NAV_NONE;
        }
        t = NavMesh_Tri(nm, t)->adj[e];
        if (t == NAV_NONE || (tri_flags(nm, t) & stop)) {
            *end = 4;
            return NAV_NONE;
        }
    }
}

/* the height of `pos` on triangle `t` (nav mesh +0x14), w 1 */
static inline void set_on(NavMesh *nm, u32 t, f32 *pos) {
    VCALL((VObject *)nm, 0x14, void (*)(VObject *, u32, f32 *))((VObject *)nm, t, pos);
    pos[3] = 1.0f;
}

/* the edge crossing walking the line `from` -> `to` out of triangle `t` (nav mesh +0x24: the
 * edge, its point in `cross`; 3 when `to` is inside, 4 off the mesh) */
static inline s32 edge_cross(NavMesh *nm, u32 t, f32 *cross, f32 *from, f32 *to) {
    return VCALL((VObject *)nm, 0x24, s32 (*)(VObject *, u32, f32 *, f32 *, f32 *))((VObject *)nm, t, cross, from, to);
}

void *PathPlan_dtor(void *o, s32 flags);
void PathPlan_Estimates(void *pl, u8 *s, u8 *req);
s32 PathPlan_StepAtGoal(u8 *pl, u8 *s);
s32 PathPlan_StepBreadth(void *pl, u8 *s);
s32 PathPlan_StepDepth(void *pl, u8 *s);
s32 PathPlan_StepGreedy(void *pl, u8 *s);
s32 PathPlan_StepBounded(void *pl, u8 *s);
s32 PathPlan_StepGreedySorted(void *pl, u8 *s);
s32 PathPlan_StepCheapNear(void *pl, u8 *s);
s32 PathPlan_StepDijkstra(void *pl, u8 *s);
s32 PathPlan_StepAStar(void *pl, u8 *s);
void PathPlan_Trace(u8 *pl, u8 *s);
s32 PathPlan_Smooth(u8 *pl, u8 *s);
f32 PathPlan_PathLength(VObject *pl, s32 id);
f32 PathPlan_PointsLength(void *pl, const f32 *pos, s32 from, s32 to, const u8 *pts);
s32 PathPlan_Advance(void *pl, u32 *tri, f32 *pos, s32 i, s32 n, const u8 *pts, f32 dist);
s32 PathPlan_AdvanceOnMesh(void *pl, u32 *tri, f32 *pos, s32 i, s32 n, const u8 *pts, f32 dist);
s32 PathPlan_Points(u8 *pl, s32 id, u8 *out);
s32 PathPlan_Curve(u8 *pl, s32 id, u8 *out);
void PathPlan_RunSearch0(u8 *pl);
void PathPlan_Step(u8 *pl, s32 id);
s32 PathPlan_Start(u8 *pl, u8 *req, s32 id);
s32 PathPlan_EndTri(u8 *pl, s32 id);
void PathPlan_CutBack(u8 *pl, s32 id, u32 mask);
f32 PathPlan_LastLength(u8 *pl, s32 id);
s32 PathPlan_EndCentre(u8 *pl, s32 id, f32 *centre);
void *PathPlanBase_dtor(u8 *o, s32 flags);

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

/* drop two pending pointers (each with its partner word) */
/* 0x0017CD90 */
void NavMesh_DropPending(u8 *p) {
    if (AT(p, 0x4, s32) != 0) {
        AT(p, 0x4, s32) = 0;
        AT(p, 0x8, s32) = 0;
    }
    if (AT(p, 0xC, s32) != 0) {
        AT(p, 0xC, s32) = 0;
        AT(p, 0x10, s32) = 0;
    }
}

/* destructor (vtable D_0046AA40) */
/* 0x0017CDD0 */
void *NavMeshBase_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AA40;
        gNavMesh = NULL;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* +0x8 destructor */
/* 0x001A4970 */
void *PathPlan_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = PathPlan_vtable;
        if (o != NULL) {
            AT(o, 0x0, void **) = D_0046AC00;
            if (o != NULL) {
                gSceneGameF29740 = NULL;
            }
        }
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* the estimates to the goal (+0x10) of every triangle for the request kinds 3..5: the distance
 * from its centre to the goal point (`req` +0x8: 0 plain, 1 heights x 10, 2 also +1000 on
 * triangles flagged 0x40) */
/* 0x001A49E0 */
void PathPlan_Estimates(void *pl, u8 *s, u8 *req) {
    NavMesh *nm;
    u32 n, t;
    u8 *node;

    if (AT(req, 0x4, s32) != 5 && AT(req, 0x4, s32) != 4 && AT(req, 0x4, s32) != 3) {
        return;
    }
    nm = gNavMesh;
    n = nm->numTris;
    node = s + 0x44;
    switch (AT(req, 0x8, s32)) {
    case 0:
        for (t = 0; t < n; t++, node += 0x18) {
            f32 c[4] __attribute__((aligned(16)));
            f32 d[4] __attribute__((aligned(16)));

            tri_centre(nm, t, c);
            sceVu0SubVector(d, c, (f32 *)(s + 0x30));
            AT(node, 0x10, f32) = __builtin_sqrtf(sceVu0InnerProduct(d, d));
        }
        break;
    case 1:
        for (t = 0; t < n; t++, node += 0x18) {
            f32 c[4] __attribute__((aligned(16)));
            f32 d[4] __attribute__((aligned(16)));

            tri_centre(nm, t, c);
            sceVu0SubVector(d, c, (f32 *)(s + 0x30));
            d[1] *= 10.0f;
            AT(node, 0x10, f32) = __builtin_sqrtf(sceVu0InnerProduct(d, d));
        }
        break;
    case 2: {
        NavTri *tri = (n != 0 && nm->tris != NULL) ? nm->tris : NULL;

        for (t = 0; t < n; t++, node += 0x18, tri++) {
            f32 c[4] __attribute__((aligned(16)));
            f32 d[4] __attribute__((aligned(16)));

            tri_centre(nm, t, c);
            sceVu0SubVector(d, c, (f32 *)(s + 0x30));
            d[1] *= 10.0f;
            AT(node, 0x10, f32) = __builtin_sqrtf(sceVu0InnerProduct(d, d));
            if (tri->flags & 0x40) {
                AT(node, 0x10, f32) = AT(node, 0x10, f32) + 1000.0f;
            }
        }
        break;
    }
    }
}

/* step for a start already at the goal: the path is the start node alone (+0x40198 start,
 * +0x4019A goal triangle) */
/* 0x001A4C60 */
s32 PathPlan_StepAtGoal(u8 *pl, u8 *s) {
    AT(pl, 0x40198, s16) = AT(s, 0x4, s32);
    AT(pl, 0x4019A, s16) = AT(s, 0x8, s32);
    AT(s, 0x10048, u8 *) = NODE(s, AT(s, 0x4, s32));
    AT(AT(s, 0x10048, u8 *), 0x4, void *) = NULL;
    return 2;
}

/* step for kind 7, cheapest first by the cost so far (+0xC) on the sorted list, places kept at
 * +2 (a cheaper way to an open node moves it up), within 30 of the start (blocked triangles
 * explored without the check): the first triangle
 * further (or the list running out) ends it at the nearest one seen (+0x10058) */
/* 0x001A4CC0 */
s32 PathPlan_StepCheapNear(void *pl, u8 *s) {
    u8 *nbs[3];
    NavMesh *nm;
    u8 *cur;
    NavTri *tri;
    s32 n, k = 0, e;

    AT(s, 0x0, s32)++;
    cur = OPEN(s, 0);
    n = AT(s, 0x40, s16);
    nm = gNavMesh;
    tri = NavMesh_Tri(nm, NODE_INDEX(s, cur));
    if (tri == NULL) {
        AT(s, 0x10048, u8 *) = NULL;
        return -1;
    }
    for (e = 0; e < 3; e++) {
        u32 t = tri->adj[e];
        NavTri *nt;   /* (the edge costs are read from the neighbour, at the same edge) */
        u8 *nb;
        f32 g;

        if (t == NAV_NONE) {
            continue;
        }
        nb = NODE(s, t);
        nt = NavMesh_Tri(nm, t);
        if (AT(nb, 0x0, u16) & 3) {
            if (!(AT(nb, 0x0, u16) & 1)) {
                continue;
            }
            g = AT(cur, 0xC, f32) + AT(nt, 0x40 + e * 4, f32);
            if (!(g < AT(nb, 0xC, f32))) {
                continue;
            }
            AT(nb, 0x4, u8 *) = cur;
            AT(nb, 0xC, f32) = g;
            open_raise(s, nb, 0xC);
            continue;
        }
        if (!(tri_flags(nm, t) & AT(s, 0xC, u32))) {
            f32 c[4] __attribute__((aligned(16)));
            f32 d[4] __attribute__((aligned(16)));
            f32 dist;

            tri_centre(nm, t, c);
            sceVu0SubVector(d, c, (f32 *)(s + 0x20));
            dist = __builtin_sqrtf(d[2] * d[2] + d[0] * d[0]);
            if (AT(s, 0x10058, s32) == -1) {
                AT(s, 0x10058, u32) = t;
                AT(s, 0x1005C, f32) = dist;
            } else if (dist < AT(s, 0x1005C, f32)) {
                AT(s, 0x1005C, f32) = dist;
                AT(s, 0x10058, u32) = t;
            }
            if (!(dist <= 30.0f)) {
                if (AT(s, 0x10058, s32) != -1) {
                    AT(s, 0x10048, u8 *) = NODE(s, AT(s, 0x10058, s32));
                    AT(NODE(s, AT(s, 0x10058, s32)), 0x4, u8 *) = NULL;
                    return 1;
                }
                AT(s, 0x10048, u8 *) = NULL;
                return -1;
            }
        }
        nbs[k++] = nb;
        AT(nb, 0x0, u16) |= 1;
        AT(nb, 0x4, u8 *) = cur;
        AT(nb, 0xC, f32) = AT(cur, 0xC, f32) + AT(nt, 0x40 + e * 4, f32);
    }
    AT(cur, 0x0, u16) ^= 3;
    AT(s, 0xE044 + AT(s, 0x42, s16) * 4, u8 *) = cur;
    AT(s, 0x42, s16)++;
    open_merge(s, nbs, k, n, 0xC, 0xC);
    AT(s, 0x40, s16) = n + k - 1;
    if (n + k != 0) {
        return 0;
    }
    if (AT(s, 0x10058, s32) != -1) {
        AT(s, 0x10048, u8 *) = NODE(s, AT(s, 0x10058, s32));
        AT(NODE(s, AT(s, 0x10058, s32)), 0x4, u8 *) = NULL;
        return AT(s, 0x0, s32);
    }
    AT(s, 0x10048, u8 *) = NULL;
    return AT(s, 0x0, s32);
}

/* step for kind 6, greedy best first: the node +0x10044 expanded; its new neighbours get the
 * estimate (heights x 10, +5 on triangles flagged 0x40) and go into the list (by +8) sorted by
 * it; a seen neighbour cheaper to come from becomes the node's parent */
/* 0x001A5750 */
s32 PathPlan_StepGreedy(void *pl, u8 *s) {
    u8 *nbs[3];
    NavMesh *nm;
    u8 *cur, *head;
    NavTri *tri;
    s32 k = 0, e, i;

    AT(s, 0x0, s32)++;
    cur = AT(s, 0x10044, u8 *);
    nm = gNavMesh;
    tri = NavMesh_Tri(nm, NODE_INDEX(s, cur));
    if (tri == NULL) {
        return -1;
    }
    for (e = 0; e < 3; e++) {
        u32 t = tri->adj[e];
        u8 *nb;
        u32 f;

        if (t == NAV_NONE) {
            continue;
        }
        nb = NODE(s, t);
        if (AT(nb, 0x0, u16) & 0xB) {
            if (!(AT(cur, 0xC, f32) <= AT(nb, 0xC, f32) + AT(tri, 0x40 + e * 4, f32))) {
                AT(cur, 0x4, u8 *) = nb;
            }
            continue;
        }
        f = tri_flags(nm, t);
        if (f & AT(s, 0xC, u32)) {
            continue;
        }
        if ((AT(nb, 0x0, u16) & 4) || (f & AT(s, 0x10, u32))) {
            AT(s, 0x10048, u8 *) = nb;
            AT(nb, 0x4, u8 *) = cur;
            return AT(s, 0x0, s32);
        }
        {
            f32 d[4] __attribute__((aligned(16)));
            u32 k2 = NODE_INDEX(s, nb);

            tri_centre(nm, k2, d);
            sceVu0SubVector(d, d, (f32 *)(s + 0x30));
            d[1] *= 10.0f;
            AT(nb, 0x10, f32) = __builtin_sqrtf(sceVu0InnerProduct(d, d));
            if (tri_flags(nm, k2) & 0x40) {
                AT(nb, 0x10, f32) = AT(nb, 0x10, f32) + 5.0f;
            }
        }
        nbs[k++] = nb;
        AT(nb, 0xC, f32) = AT(cur, 0xC, f32) + AT(tri, 0x40 + e * 4, f32);
        AT(nb, 0x4, u8 *) = cur;
        AT(nb, 0x0, u16) |= 1;
    }
    AT(cur, 0x0, u16) ^= 3;
    head = AT(AT(s, 0x10044, u8 *), 0x8, u8 *);
    if (k != 0) {
        if (head == NULL) {
            head = nbs[--k];
            AT(head, 0x8, u8 *) = NULL;
        }
        for (i = 0; i < k; i++) {
            u8 *n = nbs[i];
            u8 *at = head, *prev = NULL;
            f32 h = AT(n, 0x10, f32);

            while (!(h <= AT(at, 0x10, f32)) && AT(at, 0x8, u8 *) != NULL) {
                prev = at;
                at = AT(at, 0x8, u8 *);
            }
            if (h <= AT(at, 0x10, f32)) {   /* before `at` */
                if (prev == NULL) {
                    AT(n, 0x8, u8 *) = at;
                    head = n;
                } else {
                    AT(n, 0x8, u8 *) = at;
                    AT(prev, 0x8, u8 *) = n;
                }
            } else if (prev == NULL || AT(at, 0x8, u8 *) == NULL) {   /* after the last */
                AT(at, 0x8, u8 *) = n;
                AT(n, 0x8, u8 *) = NULL;
            } else {
                AT(n, 0x8, u8 *) = at;
                AT(prev, 0x8, u8 *) = n;
            }
        }
    }
    if (head == NULL) {
        return -AT(s, 0x0, s32);
    }
    AT(s, 0x10044, u8 *) = head;
    return 0;
}

/* step for kind 5, a memory-bounded best first search: the node +0x10044 expanded (flag 1);
 * its new neighbours (flag 0x10, depth +2, f = g + h at +0x14) sorted by f; the best is next
 * and the rest pushed on the stack (+0xC044, +0x40) - or, when even the best is no better than
 * the bound (+0x10048), all of them, and the search backs up: the bound becomes the node,
 * the stack keeps only the nodes better than the bound (the rest go to the deferred list
 * +0xE044, +0x42) and is popped; with it empty, the deepest deferred node's best unexpanded
 * ancestor (by f) comes back */
/* 0x001A5BB0 */
s32 PathPlan_StepBounded(void *pl, u8 *s) {
#ifdef HG_NATIVE
    static u8 *sNew0;   /* (the original reads its new[0] stale from the last step's frame) */
#endif
    u8 *nbs[3];
    NavMesh *nm;
    u8 *cur, *next = NULL;
    NavTri *tri;
    s32 open, deferred, k = 0, e, i, j, depth;

    AT(s, 0x0, s32)++;
    cur = AT(s, 0x10044, u8 *);
    open = AT(s, 0x40, s16);
    deferred = AT(s, 0x42, s16);
    nm = gNavMesh;
    tri = NavMesh_Tri(nm, NODE_INDEX(s, cur));
    AT(cur, 0x0, u16) |= 1;
    depth = AT(cur, 0x2, s16) + 1;
    for (e = 0; e < 3; e++) {
        u32 t = tri->adj[e];
        u8 *nb;

        if (t == NAV_NONE) {
            continue;
        }
        nb = NODE(s, t);
        if (tri_flags(nm, t) & AT(s, 0xC, u32)) {
            continue;
        }
        if (AT(nb, 0x0, u16) & 8) {
            continue;
        }
        if (AT(nb, 0x0, u16) & 0x11) {
            continue;
        }
        AT(nb, 0x0, u16) |= 0x10;
        AT(nb, 0x4, u8 *) = cur;
        AT(nb, 0x2, s16) = depth;
        if ((AT(nb, 0x0, u16) & 4) || (tri_flags(nm, t) & AT(s, 0x10, u32))) {
            AT(s, 0x10048, u8 *) = nb;
            AT(nb, 0x4, u8 *) = cur;
            AT(s, 0x40, s16) = open;
            return AT(s, 0x0, s32);
        }
        nbs[k++] = nb;
        AT(nb, 0xC, f32) = AT(cur, 0xC, f32) + AT(tri, 0x40 + e * 4, f32);
        AT(nb, 0x14, f32) = AT(nb, 0x10, f32) + AT(nb, 0xC, f32);
    }
    for (i = 1; i < k; i++) {
        for (j = i; j < k; j++) {
            if (AT(nbs[j], 0x14, f32) < AT(nbs[j - 1], 0x14, f32)) {
                u8 *x = nbs[j - 1];

                nbs[j - 1] = nbs[j];
                nbs[j] = x;
            }
        }
    }
    if (k != 0) {
        u8 *bound = AT(s, 0x10048, u8 *);

#ifdef HG_NATIVE
        sNew0 = nbs[0];
#endif
        if (bound == NULL || AT(nbs[0], 0x14, f32) < AT(bound, 0x14, f32)) {
            next = nbs[0];
            for (i = k - 1; i > 0; i--) {
                AT(s, 0xC044 + open * 4, u8 *) = nbs[i];
                open++;
            }
        } else {
            for (i = k - 1; i >= 0; i--) {
                AT(s, 0xC044 + open * 4, u8 *) = nbs[i];
                open++;
            }
        }
    }
    if (next == NULL) {
        u8 *bound = AT(s, 0x10048, u8 *);
        s32 kept = 0;
        f32 fb;

#ifdef HG_NATIVE
        if (k == 0) {
            nbs[0] = sNew0;
        }
#endif
        if (bound == NULL || AT(nbs[0], 0x14, f32) < AT(bound, 0x14, f32)) {
            AT(s, 0x10048, u8 *) = cur;
        }
        fb = AT(AT(s, 0x10048, u8 *), 0x14, f32);
        for (i = 0; i < open; i++) {
            u8 *n = AT(s, 0xC044 + i * 4, u8 *);

            if (AT(n, 0x14, f32) < fb) {
                AT(s, 0xC044 + kept * 4, u8 *) = n;
                kept++;
            } else {
                AT(s, 0xE044 + deferred * 4, u8 *) = n;
                deferred++;
            }
        }
        open = kept;
        if (open != 0) {
            next = AT(s, 0xC044 + (open - 1) * 4, u8 *);
            open--;
        }
    }
    if (next == NULL && open == 0) {
        s32 best = 0;
        f32 f;
        u8 *a;

        if (deferred == 0) {
            return -AT(s, 0x0, s32);
        }
        next = AT(s, 0xE044, u8 *);
        for (i = 0; i < deferred; i++) {
            u8 *n = AT(s, 0xE044 + i * 4, u8 *);

            if (best < AT(n, 0x2, s16)) {
                next = n;
                best = AT(n, 0x2, s16);
            }
        }
        while (next != NULL && (AT(next, 0x0, u16) & 1)) {
            next = AT(next, 0x4, u8 *);
        }
        if (next == NULL) {
            return -AT(s, 0x0, s32);
        }
        f = AT(next, 0x14, f32);
        for (a = AT(next, 0x4, u8 *); a != NULL; a = AT(a, 0x4, u8 *)) {
            if (!(AT(a, 0x0, u16) & 1) && !(f <= AT(a, 0x14, f32))) {
                f = AT(a, 0x14, f32);
                next = a;
            }
        }
        j = 0;
        for (i = 0; i < deferred; i++) {
            u8 *n = AT(s, 0xE044 + i * 4, u8 *);

            if (n != next) {
                AT(s, 0xE044 + j * 4, u8 *) = n;
                j++;
            }
        }
        deferred = j;
    }
    AT(s, 0x40, s16) = open;
    AT(s, 0x42, s16) = deferred;
    AT(s, 0x10044, u8 *) = next;
    return 0;
}

/* step for kind 4, A*: by f = g + h (+0x14) on the sorted list, places kept at +2 in the open
 * list and in the closed list (+0xE044, +0x42); a closed node reached cheaper is opened again */
/* 0x001A6260 */
s32 PathPlan_StepAStar(void *pl, u8 *s) {
    u8 *nbs[3];
    NavMesh *nm;
    u8 *cur;
    NavTri *tri;
    s32 n, c, k = 0, e, i;

    AT(s, 0x0, s32)++;
    cur = OPEN(s, 0);
    n = AT(s, 0x40, s16);
    c = AT(s, 0x42, s16);
    nm = gNavMesh;
    tri = NavMesh_Tri(nm, NODE_INDEX(s, cur));
    if (tri == NULL) {
        return -1;
    }
    for (e = 0; e < 3; e++) {
        u32 t = tri->adj[e];
        u8 *nb;
        u32 f;
        f32 g;

        if (t == NAV_NONE) {
            continue;
        }
        nb = NODE(s, t);
        f = tri_flags(nm, t);
        if (f & AT(s, 0xC, u32)) {
            continue;
        }
        if (AT(nb, 0x0, u16) & 8) {
            continue;
        }
        if (AT(nb, 0x0, u16) & 3) {
            if (AT(nb, 0x0, u16) & 1) {
                g = AT(cur, 0xC, f32) + AT(tri, 0x40 + e * 4, f32);
                if (!(g < AT(nb, 0xC, f32))) {
                    continue;
                }
                AT(nb, 0x4, u8 *) = cur;
                AT(nb, 0xC, f32) = g;
                AT(nb, 0x14, f32) = AT(nb, 0x10, f32) + g;
                open_raise(s, nb, 0x14);
            } else if (AT(nb, 0x0, u16) & 2) {
                s32 pos;

                g = AT(cur, 0xC, f32) + AT(tri, 0x40 + e * 4, f32);
                if (!(g < AT(nb, 0xC, f32))) {
                    continue;
                }
                c--;
                nbs[k] = nb;
                AT(nb, 0x0, u16) ^= 3;
                AT(nb, 0x4, u8 *) = cur;
                AT(nb, 0xC, f32) = g;
                AT(nb, 0x14, f32) = AT(nb, 0x10, f32) + g;
                pos = AT(nb, 0x2, s16);
                k++;
                for (i = pos; i < c; i++) {
                    AT(s, 0xE044 + i * 4, u8 *) = AT(s, 0xE044 + (i + 1) * 4, u8 *);
                    AT(AT(s, 0xE044 + i * 4, u8 *), 0x2, s16) = i;
                }
            }
            continue;
        }
        if ((AT(nb, 0x0, u16) & 4) || (f & AT(s, 0x10, u32))) {
            AT(s, 0x10048, u8 *) = nb;
            AT(nb, 0x4, u8 *) = cur;
            AT(nb, 0xC, f32) = AT(cur, 0xC, f32) + AT(tri, 0x40 + e * 4, f32);
            AT(nb, 0x14, f32) = AT(nb, 0x10, f32) + AT(nb, 0xC, f32);
            AT(s, 0x40, s16) = n;
            AT(s, 0x42, s16) = c;
            return AT(s, 0x0, s32);
        }
        nbs[k++] = nb;
        AT(nb, 0x0, u16) |= 1;
        AT(nb, 0x4, u8 *) = cur;
        AT(nb, 0xC, f32) = AT(cur, 0xC, f32) + AT(tri, 0x40 + e * 4, f32);
        AT(nb, 0x14, f32) = AT(nb, 0x10, f32) + AT(nb, 0xC, f32);
    }
    AT(cur, 0x0, u16) ^= 3;
    AT(s, 0xE044 + c * 4, u8 *) = cur;
    AT(AT(s, 0xE044 + c * 4, u8 *), 0x2, s16) = c;
    open_merge(s, nbs, k, n, 0xC, 0x14);
    AT(s, 0x40, s16) = n + k - 1;
    AT(s, 0x42, s16) = c + 1;
    if (n + k != 0) {
        return 0;
    }
    return -AT(s, 0x0, s32);
}

/* step for kind 3, greedy best first on the sorted list (+0xC044, the head expanded): its new
 * neighbours, sorted by the estimate (+0x10), are merged in; the best of them replaces the head,
 * found with the goal node as a sentinel past the end (its estimate set above it) */
/* 0x001A6D70 */
s32 PathPlan_StepGreedySorted(void *pl, u8 *s) {
    u8 *nbs[3];
    NavMesh *nm;
    u8 *cur;
    NavTri *tri;
    s32 n, k = 0, e, i, j;

    AT(s, 0x0, s32)++;
    cur = OPEN(s, 0);
    n = AT(s, 0x40, s16);
    nm = gNavMesh;
    tri = NavMesh_Tri(nm, NODE_INDEX(s, cur));
    if (tri == NULL) {
        return -1;
    }
    for (e = 0; e < 3; e++) {
        u32 t = tri->adj[e];
        u8 *nb;
        u32 f;

        if (t == NAV_NONE) {
            continue;
        }
        nb = NODE(s, t);
        if (AT(nb, 0x0, u16) & 0xB) {
            continue;
        }
        f = tri_flags(nm, t);
        if (f & AT(s, 0xC, u32)) {
            continue;
        }
        if ((AT(nb, 0x0, u16) & 4) || (f & AT(s, 0x10, u32))) {
            AT(s, 0x10048, u8 *) = nb;
            AT(nb, 0x4, u8 *) = cur;
            AT(s, 0x40, s16) = n;
            return AT(s, 0x0, s32);
        }
        nbs[k++] = nb;
        AT(nb, 0x0, u16) |= 1;
        AT(nb, 0x4, u8 *) = cur;
    }
    AT(cur, 0x0, u16) ^= 3;
    AT(s, 0xE044 + AT(s, 0x42, s16) * 4, u8 *) = cur;
    AT(s, 0x42, s16)++;
    if (k == 0) {
        for (i = 0; i < n - 1; i++) {
            OPEN(s, i) = OPEN(s, i + 1);
        }
    } else {
        for (i = 1; i < k; i++) {
            for (j = 0; j < k - i; j++) {
                if (!(AT(nbs[j], 0x10, f32) <= AT(nbs[j + 1], 0x10, f32))) {
                    u8 *x = nbs[j];

                    nbs[j] = nbs[j + 1];
                    nbs[j + 1] = x;
                }
            }
        }
        if (n == 1) {
            for (i = 0; i < k; i++) {
                OPEN(s, i) = nbs[i];
            }
        } else {
            s32 at = n - 1;
            u8 *g;

            for (i = k - 1; i > 0; i--) {   /* the rest, merged in from the end */
                u8 *x = nbs[i];

                if (AT(x, 0x10, f32) < AT(OPEN(s, 1), 0x10, f32)) {
                    for (; at > 0; at--) {
                        OPEN(s, at + i) = OPEN(s, at);
                    }
                } else {
                    while (AT(x, 0x10, f32) < AT(OPEN(s, at), 0x10, f32)) {
                        OPEN(s, at + i) = OPEN(s, at);
                        at--;
                    }
                }
                OPEN(s, at + i) = x;
            }
            g = NODE(s, AT(s, 0x8, s32));
            OPEN(s, n + k - 1) = g;
            AT(g, 0x10, f32) = 1.0f + AT(nbs[0], 0x10, f32);
            for (i = 0; !(AT(nbs[0], 0x10, f32) <= AT(OPEN(s, i + 1), 0x10, f32)); i++) {
                OPEN(s, i) = OPEN(s, i + 1);
            }
            OPEN(s, i) = nbs[0];
        }
    }
    n = n + k - 1;
    AT(s, 0x40, s16) = n;
    if (n != 0) {
        return 0;
    }
    return -AT(s, 0x0, s32);
}

/* step for kind 2, cheapest first (Dijkstra) by the cost so far (+0xC) on the sorted list,
 * places kept at +2 */
/* 0x001A7320 */
s32 PathPlan_StepDijkstra(void *pl, u8 *s) {
    u8 *nbs[3];
    NavMesh *nm;
    u8 *cur;
    NavTri *tri;
    s32 n, k = 0, e;

    AT(s, 0x0, s32)++;
    cur = OPEN(s, 0);
    n = AT(s, 0x40, s16);
    nm = gNavMesh;
    tri = NavMesh_Tri(nm, NODE_INDEX(s, cur));
    if (tri == NULL) {
        return -1;
    }
    for (e = 0; e < 3; e++) {
        u32 t = tri->adj[e];
        u8 *nb;
        u32 f;

        if (t == NAV_NONE) {
            continue;
        }
        nb = NODE(s, t);
        f = tri_flags(nm, t);
        if (f & AT(s, 0xC, u32)) {
            continue;
        }
        if (AT(nb, 0x0, u16) & 8) {
            continue;
        }
        if (AT(nb, 0x0, u16) & 3) {
            f32 g;

            if (!(AT(nb, 0x0, u16) & 1)) {
                continue;
            }
            g = AT(cur, 0xC, f32) + AT(tri, 0x40 + e * 4, f32);
            if (!(g < AT(nb, 0xC, f32))) {
                continue;
            }
            AT(nb, 0x4, u8 *) = cur;
            AT(nb, 0xC, f32) = g;
            open_raise(s, nb, 0xC);
            continue;
        }
        if ((AT(nb, 0x0, u16) & 4) || (f & AT(s, 0x10, u32))) {
            AT(s, 0x10048, u8 *) = nb;
            AT(nb, 0x4, u8 *) = cur;
            AT(nb, 0xC, f32) = AT(cur, 0xC, f32) + AT(tri, 0x40 + e * 4, f32);
            AT(s, 0x40, s16) = n;
            return AT(s, 0x0, s32);
        }
        nbs[k++] = nb;
        AT(nb, 0x0, u16) |= 1;
        AT(nb, 0x4, u8 *) = cur;
        AT(nb, 0xC, f32) = AT(cur, 0xC, f32) + AT(tri, 0x40 + e * 4, f32);
    }
    AT(cur, 0x0, u16) ^= 3;
    AT(s, 0xE044 + AT(s, 0x42, s16) * 4, u8 *) = cur;
    AT(s, 0x42, s16)++;
    open_merge(s, nbs, k, n, 0xC, 0xC);
    AT(s, 0x40, s16) = n + k - 1;
    if (n + k != 0) {
        return 0;
    }
    return -AT(s, 0x0, s32);
}

/* step for kind 1, depth first: as kind 0, but the head's new neighbours take its place at
 * the front of the list */
/* 0x001A7C30 */
s32 PathPlan_StepDepth(void *pl, u8 *s) {
    u8 *nbs[3];
    NavMesh *nm;
    u8 *cur;
    NavTri *tri;
    s32 n, k = 0, e, i;

    AT(s, 0x0, s32)++;
    cur = AT(s, 0xC044, u8 *);
    n = AT(s, 0x40, s16);
    nm = gNavMesh;
    tri = NavMesh_Tri(nm, NODE_INDEX(s, cur));
    if (tri == NULL) {
        return -1;
    }
    for (e = 0; e < 3; e++) {
        u32 t = tri->adj[e];
        u8 *nb;
        u32 f;

        if (t == NAV_NONE) {
            continue;
        }
        nb = NODE(s, t);
        if (AT(nb, 0x0, u16) & 0xB) {
            continue;
        }
        f = tri_flags(nm, t);
        if (f & AT(s, 0xC, u32)) {
            continue;
        }
        if ((AT(nb, 0x0, u16) & 4) || (f & AT(s, 0x10, u32))) {
            AT(s, 0x10048, u8 *) = nb;
            AT(nb, 0x4, u8 *) = cur;
            AT(s, 0x40, s16) = n;
            return AT(s, 0x0, s32);
        }
        nbs[k++] = nb;
        AT(nb, 0x0, u16) |= 1;
        AT(nb, 0x4, u8 *) = cur;
    }
    AT(cur, 0x0, u16) ^= 3;
    AT(s, 0xE044 + AT(s, 0x42, s16) * 4, u8 *) = cur;
    AT(s, 0x42, s16)++;
    if (k == 1) {
        AT(s, 0xC044, u8 *) = nbs[0];
    } else if (k == 0) {
        for (i = 0; i < n - 1; i++) {
            AT(s, 0xC044 + i * 4, u8 *) = AT(s, 0xC044 + (i + 1) * 4, u8 *);
        }
    } else {
        for (i = n - 1; i > 0; i--) {
            AT(s, 0xC044 + (i + k - 1) * 4, u8 *) = AT(s, 0xC044 + i * 4, u8 *);
        }
        for (i = 0; i < k; i++) {
            AT(s, 0xC044 + i * 4, u8 *) = nbs[i];
        }
    }
    n = n + k - 1;
    AT(s, 0x40, s16) = n;
    if (n != 0) {
        return 0;
    }
    return -AT(s, 0x0, s32);
}

/* step for kind 0, breadth first: the queue's head (+0xC044) expanded, its new neighbours (not
 * stopped by +0xC) queued; one at the goal (flag 4, or triangle flags of +0x10) ends it. 0 to
 * go on, the steps when found, -1 / -steps when it can't be reached */
/* 0x001A8040 */
s32 PathPlan_StepBreadth(void *pl, u8 *s) {
    NavMesh *nm;
    u8 *cur;
    NavTri *tri;
    s32 n, e, i;

    AT(s, 0x0, s32)++;
    cur = AT(s, 0xC044, u8 *);
    n = AT(s, 0x40, s16);
    nm = gNavMesh;
    tri = NavMesh_Tri(nm, NODE_INDEX(s, cur));
    if (tri == NULL) {
        return -1;
    }
    for (e = 0; e < 3; e++) {
        u32 t = tri->adj[e];
        u8 *nb;
        u32 f;

        if (t == NAV_NONE) {
            continue;
        }
        nb = NODE(s, t);
        if (AT(nb, 0x0, u16) & 0xB) {
            continue;
        }
        f = tri_flags(nm, t);
        if (f & AT(s, 0xC, u32)) {
            continue;
        }
        if ((AT(nb, 0x0, u16) & 4) || (f & AT(s, 0x10, u32))) {
            AT(s, 0x10048, u8 *) = nb;
            AT(nb, 0x4, u8 *) = cur;
            AT(s, 0x40, s16) = n;
            return AT(s, 0x0, s32);
        }
        AT(s, 0xC044 + n * 4, u8 *) = nb;
        n++;
        AT(nb, 0x0, u16) |= 1;
        AT(nb, 0x4, u8 *) = cur;
    }
    AT(cur, 0x0, u16) ^= 3;
    AT(s, 0xE044 + AT(s, 0x42, s16) * 4, u8 *) = cur;
    AT(s, 0x42, s16)++;
    for (i = 0; i < n - 1; i++) {
        AT(s, 0xC044 + i * 4, u8 *) = AT(s, 0xC044 + (i + 1) * 4, u8 *);
    }
    AT(s, 0x40, s16) = n - 1;
    if (n - 1 != 0) {
        return 0;
    }
    return -AT(s, 0x0, s32);
}

/* +0x40 the length of search `id`'s path (-1 when it has none) */
/* 0x001A8300 */
f32 PathPlan_PathLength(VObject *pl, s32 id) {
    if (id == -1) {
        return -1.0f;
    }
    if (PathPlan_Smooth((u8 *)pl, SEARCH(pl, id)) == 0) {
        return VCALL(pl, 0x30, f32 (*)(VObject *, s32))(pl, id);
    }
    return -1.0f;
}

/* +0x3C the length (x / z) of path points from + 1 .. to - 1, from `pos` (points 0xC bytes:
 * triangle, x, z) */
/* 0x001A8380 */
f32 PathPlan_PointsLength(void *pl, const f32 *pos, s32 from, s32 to, const u8 *pts) {
    f32 x = pos[0], z = pos[2], len = 0.0f;
    const u8 *p = pts + (from + 1) * 0xC;
    s32 i;

    for (i = from + 1; i < to; i++, p += 0xC) {
        f32 dx = x - AT(p, 0x4, f32);
        f32 dz = z - AT(p, 0x8, f32);

        x = AT(p, 0x4, f32);
        z = AT(p, 0x8, f32);
        len += __builtin_sqrtf(dz * dz + dx * dx);
    }
    return len;
}

/* +0x24 go `dist` along the path (`n` points, 0xC each: triangle, x, z) from point `i` at
 * (`*tri`, `pos`): the triangle and x / z reached; the point passed last (`n` at its end) */
/* 0x001A8400 */
s32 PathPlan_Advance(void *pl, u32 *tri, f32 *pos, s32 i, s32 n, const u8 *pts, f32 dist) {
    f32 p[4] __attribute__((aligned(16)));
    f32 next[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    const u8 *e;
    u32 cur, nt;
    s32 k;

    if (dist < 0.0f) {
        return i;
    }
    cur = *tri;
    sceVu0CopyVector(p, pos);
    i++;
    k = i;
    e = pts + i * 0xC;
    nt = AT(e, 0x0, u32);
    next[0] = AT(e, 0x4, f32);
    next[2] = AT(e, 0x8, f32);
    for (;;) {
        f32 len;

        if (i != k) {
            e += 0xC;
            nt = AT(e, 0x0, u32);
            k = i;
            next[0] = AT(e, 0x4, f32);
            next[2] = AT(e, 0x8, f32);
        }
        sceVu0SubVector(d, next, p);
        d[1] = 0.0f;
        len = __builtin_sqrtf(sceVu0InnerProduct(d, d));
        if (!(len <= dist)) {
            break;
        }
        if (i == n - 1) {
            *tri = cur;
            pos[0] = p[0];
            pos[2] = p[2];
            pos[3] = 1.0f;
            return n;
        }
        dist -= len;
        cur = nt;
        sceVu0CopyVector(p, next);
        i++;
    }
    sceVu0Normalize(d, d);
    vu0_ScaleXYZ(d, d, dist);
    sceVu0AddVector(p, p, d);
    *tri = cur;
    pos[0] = p[0];
    pos[2] = p[2];
    pos[3] = 1.0f;
    return i - 1;
}

/* +0x20 as +0x24, following the nav mesh: the triangle reached is found by walking the line
 * (or, off it, from the last point's triangle on), and the point is put on it */
/* 0x001A85E0 */
s32 PathPlan_AdvanceOnMesh(void *pl, u32 *tri, f32 *pos, s32 i, s32 n, const u8 *pts, f32 dist) {
    f32 p[4] __attribute__((aligned(16)));
    f32 next[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    f32 q[4] __attribute__((aligned(16)));
    const u8 *e;
    NavMesh *nm;
    u32 cur, t;

    if (dist < 0.0f) {
        return i;
    }
    cur = *tri;
    p[0] = pos[0];
    p[2] = pos[2];
    e = pts + i * 0xC + 0xC;
    for (;;) {
        f32 len;

        next[0] = AT(e, 0x4, f32);
        i++;
        next[2] = AT(e, 0x8, f32);
        sceVu0SubVector(d, next, p);
        d[1] = 0.0f;
        len = __builtin_sqrtf(sceVu0InnerProduct(d, d));
        if (!(len <= dist)) {
            break;
        }
        if (i == n - 1) {
            nm = gNavMesh;
            *tri = AT(e, 0x0, u32);
            pos[0] = AT(e, 0x4, f32);
            pos[2] = AT(e, 0x8, f32);
            set_on(nm, AT(e, 0x0, u32), pos);
            return n;
        }
        cur = AT(e, 0x0, u32);
        dist -= len;
        sceVu0CopyVector(p, next);
        e += 0xC;
    }
    sceVu0Normalize(d, d);
    vu0_ScaleXYZ(d, d, dist);
    sceVu0AddVector(q, p, d);
    nm = gNavMesh;
    for (t = cur;;) {
        s32 r = VCALL((VObject *)nm, 0x20, s32 (*)(VObject *, u32, f32 *, f32 *))((VObject *)nm, t, p, q);

        if (r == 3) {
            *tri = t;
            pos[0] = q[0];
            pos[2] = q[2];
            set_on(nm, t, pos);
            return i - 1;
        }
        if (r == 4) {
            break;
        }
        t = NavMesh_Tri(nm, t)->adj[r];
        if (t == NAV_NONE) {
            break;
        }
    }
    for (t = cur;;) {
        s32 r;

        if (VCALL((VObject *)nm, 0x10, s32 (*)(VObject *, u32, f32 *))((VObject *)nm, t, q) == 3) {
            *tri = t;
            pos[0] = q[0];
            pos[2] = q[2];
            set_on(nm, t, pos);
            return i - 1;
        }
        r = VCALL((VObject *)nm, 0x20, s32 (*)(VObject *, u32, f32 *, f32 *))((VObject *)nm, t, p, next);
        if ((u32)(r - 3) < 2) {
            return n;
        }
        t = NavMesh_Tri(nm, t)->adj[r];
        if (t == NAV_NONE) {
            return n;
        }
    }
}

/* +0x1C search `id`'s path as points (0xC each: triangle, x, z) in `out`: the start, then the
 * edge crossings on the way to each smoothed triangle's centre (a doubled one: the middle of
 * the edge on to the next, and its centre), and the goal; their count (-1: no path) */
/* 0x001A8960 */
s32 PathPlan_Points(u8 *pl, s32 id, u8 *out) {
    f32 cross[4] __attribute__((aligned(16)));
    f32 from[4] __attribute__((aligned(16)));
    f32 to[4] __attribute__((aligned(16)));
    f32 last[4] __attribute__((aligned(16)));
    u8 *s = SEARCH(pl, id);
    NavMesh *nm;
    u16 *p;
    u32 t;
    s32 i = 1, at = 1, count = 1, r;

    if (PathPlan_Smooth(pl, s) != 0) {
        return -1;
    }
    t = AT(s, 0x4, u32);
    p = &AT(pl, 0x4119A, u16);
    AT(out, 0x0, u32) = t;
    from[0] = last[0] = AT(out, 0x4, f32) = AT(s, 0x20, f32);
    from[2] = last[2] = AT(out, 0x8, f32) = AT(s, 0x28, f32);
    if (AT(pl, 0x40194, s32) == 2) {
        to[0] = AT(s, 0x30, f32);
        to[2] = AT(s, 0x38, f32);
    } else {
        tri_centre(gNavMesh, p[0], to);
    }
    nm = gNavMesh;
    r = edge_cross(nm, t, cross, from, to);
    for (;;) {
        if (i != at) {   /* on to the next target */
            if (i == AT(pl, 0x40194, s32) - 1) {
                to[0] = AT(s, 0x30, f32);
                to[2] = AT(s, 0x38, f32);
            } else if (p[0] == p[1]) {   /* a doubled triangle */
                NavTri *tri = NavMesh_Tri(nm, p[1]);
                u32 next;

                p += 2;
                next = p[0];
                for (r = 0; r < 3; r++) {
                    if (tri->adj[r] == next) {
                        s32 e2 = r + 1 < 3 ? r + 1 : 0;

                        AT(out, 0xC, u32) = next;
                        AT(out, 0x10, f32) = 0.5f * (tri->v[r][0] + tri->v[e2][0]);
                        AT(out, 0x14, f32) = 0.5f * (tri->v[r][2] + tri->v[e2][2]);
                        tri_centre(nm, p[0], from);
                        out += 0x18;
                        count += 2;
                        AT(out, 0x0, u32) = p[0];
                        AT(out, 0x4, f32) = from[0];
                        AT(out, 0x8, f32) = from[2];
                        break;
                    }
                }
                t = p[0];
                i += 2;
                continue;
            } else {
                p++;
                tri_centre(nm, p[0], to);
            }
            r = edge_cross(nm, t, cross, from, to);
            if (r == 4) {
                return count;
            }
            at = i;
        }
        if (r == 3) {   /* the target's triangle reached */
            if (i == AT(pl, 0x40194, s32) - 1) {
                AT(out, 0xC, u32) = AT(s, 0x8, u32);
                count++;
                AT(out, 0x10, f32) = to[0];
                AT(out, 0x14, f32) = to[2];
                return count;
            }
            sceVu0CopyVector(last, to);
            sceVu0CopyVector(from, to);
            i++;
        } else {   /* over edge r into the next triangle */
            t = NavMesh_Tri(nm, t)->adj[r];
            sceVu0CopyVector(last, cross);
            r = edge_cross(nm, t, cross, from, to);
            if (r == 4) {
                return count;
            }
        }
        if (i == at) {
            out += 0xC;
            AT(out, 0x0, u32) = t;
            count++;
            AT(out, 0x4, f32) = last[0];
            AT(out, 0x8, f32) = last[2];
        }
    }
}

/* +0x18 search `id`'s path as a smooth curve of points (0xC each: triangle, x, z) in `out`:
 * from each point through the smoothed triangles' centres (a doubled one: the middle of the edge
 * on) by cubic Hermite pieces of 8 steps, their tangents halved until every step stays on the
 * walkable mesh (else, after 10 tries, a straight step); the count (-1: no path) */
/* 0x001A8DC0 */
s32 PathPlan_Curve(u8 *pl, s32 id, u8 *out) {
    f32 a[4] __attribute__((aligned(16)));   /* the piece's start */
    f32 b[4] __attribute__((aligned(16)));   /* its end */
    f32 c[4] __attribute__((aligned(16)));   /* the next centre */
    f32 a0[4] __attribute__((aligned(16)));  /* the last start */
    f32 d1[4] __attribute__((aligned(16)));
    f32 d2[4] __attribute__((aligned(16)));
    f32 d3[4] __attribute__((aligned(16)));
    f32 q[4] __attribute__((aligned(16)));
    f32 r[4] __attribute__((aligned(16)));
    u8 *s = SEARCH(pl, id);
    NavMesh *nm;
    u16 *p;
    s32 m, i = 0, k = 0;

    if (PathPlan_Smooth(pl, s) != 0) {
        return -1;
    }
    m = AT(pl, 0x40194, s32);
    if (m == 2) {
        AT(out, 0x0, u32) = AT(s, 0x4, u32);
        AT(out, 0x4, f32) = AT(s, 0x20, f32);
        AT(out, 0x8, f32) = AT(s, 0x28, f32);
        AT(out, 0xC, u32) = AT(s, 0x8, u32);
        AT(out, 0x10, f32) = AT(s, 0x30, f32);
        AT(out, 0x14, f32) = AT(s, 0x38, f32);
        return 2;
    }
    p = &AT(pl, 0x41198, u16);
    sceVu0CopyVector(a, (f32 *)(s + 0x20));
    nm = gNavMesh;
    tri_centre(nm, p[1], b);
    tri_centre(nm, p[2], c);
    sceVu0SubVector(d1, b, a);
    sceVu0SubVector(d2, c, a);
    AT(out, 0x4, f32) = a[0];
    AT(out, 0x8, f32) = a[2];
    while (i < m - 1) {
        u32 to = p[1], t;
        f32 l1, l2, l3;
        s32 tries, n;
        u8 *start, *pt;

        if (p[0] == p[1]) {   /* a doubled triangle: the middle of its edge on */
            if (i < m - 2) {
                u32 nxt = p[2];
                NavTri *tri = NavMesh_Tri(nm, p[0]);
                s32 e;

                for (e = 0; e < 3; e++) {
                    if (tri->adj[e] == nxt) {
                        s32 e2 = e < 2 ? e + 1 : 0;

                        b[0] = 0.5f * (tri->v[e][0] + tri->v[e2][0]);
                        b[2] = 0.5f * (tri->v[e][2] + tri->v[e2][2]);
                        if (VCALL((VObject *)nm, 0x10, s32 (*)(VObject *, u32, f32 *))((VObject *)nm, p[0], b) != 3) {
                            to = nxt;
                        }
                        break;
                    }
                }
            }
            sceVu0SubVector(d1, b, a);
        }
        sceVu0SubVector(d3, b, a);
        l1 = sceVu0InnerProduct(d1, d1);
        l2 = sceVu0InnerProduct(d2, d2);
        l3 = sceVu0InnerProduct(d3, d3);
        l3 = l3 + l3;
        if (l1 < l3) {
            if (l1 < l2) {
                sceVu0Normalize(d2, d2);
                vu0_ScaleXYZ(d2, d2, __builtin_sqrtf(l1));
            } else {
                sceVu0Normalize(d1, d1);
                vu0_ScaleXYZ(d1, d1, __builtin_sqrtf(l2));
            }
        } else if (l2 < l3) {
            sceVu0Normalize(d1, d1);
            vu0_ScaleXYZ(d1, d1, __builtin_sqrtf(l2));
        } else {
            sceVu0Normalize(d1, d1);
            vu0_ScaleXYZ(d1, d1, __builtin_sqrtf(l3));
            sceVu0Normalize(d2, d2);
            vu0_ScaleXYZ(d2, d2, __builtin_sqrtf(l3));
        }
        tries = 10;
        start = out + k * 0xC;
        for (;;) {
            s32 j, steps;

            pt = start + 0xC;
            if (tries <= 0) {   /* straight on */
                if (i == m - 2) {
                    AT(pt, 0x4, f32) = AT(s, 0x30, f32);
                    AT(pt, 0x8, f32) = AT(s, 0x38, f32);
                } else {
                    AT(pt, 0x4, f32) = b[0];
                    AT(pt, 0x8, f32) = b[2];
                }
                t = to;
                n = 1;
                break;
            }
            tries--;
            vu0_ScaleXYZ(d1, d1, 0.5f);
            vu0_ScaleXYZ(d2, d2, 0.5f);
            for (j = 1; j < 8; j++, pt += 0xC) {
                f32 u = (f32)j / 8.0f;
                f32 u2 = u * u;
                f32 u3 = u * u2;
                f32 t3 = 3.0f * u2;
                f32 h00 = 1.0f + (2.0f * u3 - t3);
                f32 h01 = t3 + -2.0f * u3;
                f32 h10 = u + (u3 - 2.0f * u2);
                f32 h11 = u3 - u2;

                AT(pt, 0x4, f32) = h01 * b[0] + h00 * a[0] + h10 * d1[0] + h11 * d2[0];
                AT(pt, 0x8, f32) = h01 * b[2] + h00 * a[2] + h10 * d1[2] + h11 * d2[2];
            }
            AT(pt, 0x4, f32) = b[0];
            AT(pt, 0x8, f32) = b[2];
            t = p[0];   /* each step walked through the mesh */
            sceVu0CopyVector(q, a);
            pt = start;
            r[0] = AT(start, 0x4, f32);
            r[2] = AT(start, 0x8, f32);
            for (steps = 0;;) {
                s32 e = VCALL((VObject *)nm, 0x20, s32 (*)(VObject *, u32, f32 *, f32 *))((VObject *)nm, t, q, r);

                if (e == 3) {
                    if (steps < 8) {
                        AT(pt, 0x0, u32) = t;
                        sceVu0CopyVector(q, r);
                        pt += 0xC;
                        r[0] = AT(pt, 0x4, f32);
                        r[2] = AT(pt, 0x8, f32);
                    }
                } else if (e == 4) {
                    t = NAV_NONE;
                } else {
                    t = NavMesh_Tri(nm, t)->adj[e];
                    if (t == NAV_NONE) {
                        break;
                    }
                    if (!(tri_flags(nm, t) & AT(s, 0xC, u32))) {
                        continue;
                    }
                    t = NAV_NONE;
                }
                if (t == NAV_NONE || ++steps >= 9) {
                    break;
                }
            }
            if (steps == 9) {
                n = 8;
                break;
            }
        }
        AT(pt, 0x0, u32) = t;
        p++;
        k += n;
        sceVu0CopyVector(a0, a);
        sceVu0CopyVector(a, b);
        i++;
        if (!(i < m - 1)) {
            break;
        }
        if (i == m - 2) {
            sceVu0CopyVector(b, (f32 *)(s + 0x30));
            sceVu0SubVector(d1, b, a0);
            sceVu0SubVector(d2, b, a);
        } else {
            sceVu0CopyVector(b, c);
            sceVu0SubVector(d1, b, a0);
            tri_centre(nm, p[2], c);
            sceVu0SubVector(d2, c, a);
        }
    }
    return k + 1;
}

/* the found path smoothed (+0x41198, +0x40194, at most 0x30): from each kept triangle a straight
 * line to the goal ends it; else the farthest path triangle in sight (tried 5 at a time, then
 * one at a time) is the next kept one. 0, or -1 (no path, or too long) */
/* 0x001A95F0 */
s32 PathPlan_Smooth(u8 *pl, u8 *s) {
    NavMesh *nm;
    f32 from[4] __attribute__((aligned(16)));
    f32 to[4] __attribute__((aligned(16)));
    u32 goal, cur, prev = NAV_NONE;
    s32 n, k = 0, i = 0, seen = 0, j, end;

    PathPlan_Trace(pl, s);
    n = AT(pl, 0x40190, s32);
    if (n == 0) {
        return -1;
    }
    goal = AT(s, 0x8, u32);
    AT(pl, 0x40194, s32) = 0;
    sceVu0CopyVector(from, (f32 *)(s + 0x20));
    nm = gNavMesh;
    for (;;) {
        u32 t;

        cur = AT(pl, 0x40198 + i * 2, u16);
        AT(pl, 0x41198 + k * 2, u16) = cur;
        if (++k >= 0x30) {
            AT(pl, 0x40194, s32) = k;
            return -1;
        }
        if (prev == cur) {   /* no way on: the next path triangle */
            i++;
            cur = AT(pl, 0x40198 + i * 2, u16);
            AT(pl, 0x41198 + k * 2, u16) = cur;
            if (++k >= 0x30) {
                AT(pl, 0x40194, s32) = k;
                return -1;
            }
        }
        if (prev != NAV_NONE) {
            tri_centre(nm, cur, from);
        }
        sceVu0CopyVector(to, (f32 *)(s + 0x30));
        t = line_walk(nm, cur, from, to, AT(s, 0xC, u32), &end);
        if (end == 3) {
            s32 found = 0;

            if (t == goal) {
                found = 1;
            } else if (VCALL((VObject *)nm, 0x10, s32 (*)(VObject *, u32, f32 *))((VObject *)nm, t, to) == 3) {
                NavTri *tri = NavMesh_Tri(nm, t);

                for (j = 0; j < 3; j++) {
                    if (tri->adj[j] == goal) {
                        found = 1;
                        break;
                    }
                }
            }
            if (found == 1) {
                AT(s, 0x8, u32) = t;
                AT(pl, 0x41198 + k * 2, u16) = t;
                AT(pl, 0x40194, s32) = k + 1;
                return 0;
            }
        }
        for (j = i + 5; j < n - 1; j += 5) {   /* 5 at a time */
            u32 at = AT(pl, 0x40198 + j * 2, u16);

            tri_centre(nm, at, to);
            t = line_walk(nm, cur, from, to, AT(s, 0xC, u32), &end);
            if (end == 3 && t == at) {
                seen = j;
            } else {
                i = seen;
                j = n;
            }
        }
        for (j = i + 1; j < n - 1; j++) {   /* then one at a time */
            u32 at = AT(pl, 0x40198 + j * 2, u16);

            tri_centre(nm, at, to);
            t = line_walk(nm, cur, from, to, AT(s, 0xC, u32), &end);
            if (end == 3 && t == at) {
                seen = j;
            } else {
                i = seen;
                j = n;
            }
        }
        if (j == n - 1) {
            i = seen;
        }
        prev = cur;
    }
}

/* the found path's triangles (+0x40198, start first, +0x40190 their count) from the end node
 * back; the goal becomes the end's triangle (its centre) if it isn't */
/* 0x001A9D20 */
void PathPlan_Trace(u8 *pl, u8 *s) {
    u8 *n = AT(s, 0x10048, u8 *);
    s32 count = 0, i;
    u32 t;

    for (; n != NULL; n = AT(n, 0x4, u8 *)) {
        count++;
    }
    n = AT(s, 0x10048, u8 *);
    for (i = count - 1; i >= 0; i--) {
        AT(pl, 0x40198 + i * 2, u16) = NODE_INDEX(s, n);
        n = AT(n, 0x4, u8 *);
    }
    AT(pl, 0x40190, s32) = count;
    t = NODE_INDEX(s, AT(s, 0x10048, u8 *));
    if (t != AT(s, 0x8, u32)) {
        AT(s, 0x8, u32) = t;
        tri_centre(gNavMesh, t, (f32 *)(s + 0x30));
    }
}

/* +0x14 run search 0 to its end */
/* 0x001A9F90 */
void PathPlan_RunSearch0(u8 *pl) {
    u8 *s = SEARCH(pl, 0);

    while (search_step(pl, s) == 0) {
    }
}

/* +0x10 a step of search `id` */
/* 0x001A9FF0 */
void PathPlan_Step(u8 *pl, s32 id) {
    if (id == -1) {
        return;
    }
    search_step(pl, SEARCH(pl, id));
}

/* +0xC start a search for request `req` (+0x0 kind of end, +0x4 kind 0..7, +0x8 estimate
 * kind, +0xC start triangle, +0x10 start point, +0x20 goal triangle, +0x30 goal point, +0x40
 * stop flags) in search `id` (-1: search 3 if free); the search, or -1 (a point off its
 * triangle, or none free) */
/* 0x001AA040 */
s32 PathPlan_Start(u8 *pl, u8 *req, s32 id) {
    NavMesh *nm;
    u8 *s, *start;
    u32 n, t;

    if (AT(req, 0xC, s32) == -1 || AT(req, 0x20, s32) == -1) {
        return -1;
    }
    if (AT(req, 0x4, s32) < 0 || AT(req, 0x4, s32) >= 8) {
        return -1;
    }
    nm = gNavMesh;
    if (VCALL((VObject *)nm, 0x10, s32 (*)(VObject *, s32, f32 *))((VObject *)nm, AT(req, 0xC, s32),
                                                                   (f32 *)(req + 0x10)) == 4) {
        return -1;
    }
    if (VCALL((VObject *)nm, 0x10, s32 (*)(VObject *, s32, f32 *))((VObject *)nm, AT(req, 0x20, s32),
                                                                   (f32 *)(req + 0x30)) == 4) {
        return -1;
    }
    if (id == -1) {
        for (id = 3; id < 4; id++) {
            if (!(AT(pl, 0x4, u32) & (1 << id))) {
                AT(pl, 0x4, u32) |= 1 << id;
                break;
            }
        }
        if (id >= 4) {
            return -1;
        }
    }
    s = SEARCH(pl, id);
    AT(s, 0x4, s32) = AT(req, 0xC, s32);
    sceVu0CopyVector((f32 *)(s + 0x20), (f32 *)(req + 0x10));
    AT(s, 0x8, s32) = AT(req, 0x20, s32);
    sceVu0CopyVector((f32 *)(s + 0x30), (f32 *)(req + 0x30));
    n = nm->numTris;
    for (t = 0; t < n; t++) {
        AT(NODE(s, t), 0x0, u16) = 0;
    }
    AT(s, 0x10048, u8 *) = NULL;
    AT(s, 0x0, s32) = 0;
    AT(s, 0x40, s16) = 0;
    AT(s, 0x42, s16) = 0;
    AT(s, 0xC, s32) = AT(req, 0x40, s32);
    if (AT(req, 0x0, s32) == 1) {
        AT(s, 0x10, s32) = 4;
        AT(NODE(s, AT(s, 0x4, s32)), 0x0, u16) |= 8;
        AT(NODE(s, AT(s, 0x8, s32)), 0x0, u16) |= 8;
    } else if (AT(req, 0x0, s32) == 0) {
        AT(s, 0x10, s32) = 0;
        AT(NODE(s, AT(s, 0x8, s32)), 0x0, u16) |= 4;
    }
    PathPlan_Estimates(pl, s, req);
    start = NODE(s, AT(s, 0x4, s32));
    if (AT(req, 0x4, s32) != 7 && (AT(start, 0x0, u16) & 4)) {
        if (AT(s, 0x4, s32) != AT(s, 0x8, s32)) {
            AT(s, 0x8, s32) = AT(s, 0x4, s32);
            tri_centre(nm, AT(s, 0x8, s32), (f32 *)(s + 0x30));
        }
        set_step(s, 8);
        return id;
    }
    AT(s, 0x10058, s32) = -1;
    switch (AT(req, 0x4, s32)) {
    case 5:
        AT(s, 0x10044, u8 *) = start;
        AT(AT(s, 0x10044, u8 *), 0x4, void *) = NULL;
        AT(AT(s, 0x10044, u8 *), 0x0, u16) |= 1;
        AT(AT(s, 0x10044, u8 *), 0xC, s32) = 0;
        AT(AT(s, 0x10044, u8 *), 0x2, s16) = 0;
        AT(AT(s, 0x10044, u8 *), 0x14, f32) = AT(start, 0x10, f32);
        AT(s, 0x10048, u8 *) = NULL;
        break;
    case 6: {
        f32 d[4] __attribute__((aligned(16)));

        tri_centre(nm, NODE_INDEX(s, start), d);
        sceVu0SubVector(d, d, (f32 *)(s + 0x30));
        d[1] *= 10.0f;
        AT(start, 0x4, void *) = NULL;
        AT(start, 0x10, f32) = __builtin_sqrtf(sceVu0InnerProduct(d, d));
        AT(start, 0x0, u16) |= 1;
        AT(start, 0x8, s32) = 0;
        AT(s, 0x10044, u8 *) = start;
        AT(AT(s, 0x10044, u8 *), 0xC, s32) = 0;
        AT(s, 0x40, s16)++;
        break;
    }
    default:
        AT(s, 0xC044, u8 *) = start;
        AT(AT(s, 0xC044, u8 *), 0x4, void *) = NULL;
        AT(AT(s, 0xC044, u8 *), 0x0, u16) |= 1;
        AT(AT(s, 0xC044, u8 *), 0xC, s32) = 0;
        AT(AT(s, 0xC044, u8 *), 0x14, s32) = 0;
        AT(s, 0x10048, u8 *) = AT(s, 0xC044, u8 *);
        AT(s, 0x40, s16)++;
        break;
    }
    set_step(s, AT(req, 0x4, s32));
    return id;
}

/* +0x38 the triangle of search `id`'s path end (-1: none) */
/* 0x001AA720 */
s32 PathPlan_EndTri(u8 *pl, s32 id) {
    if (id != -1) {
        u8 *s = SEARCH(pl, id);

        if (AT(s, 0x10048, u8 *) != NULL) {
            return NODE_INDEX(s, AT(s, 0x10048, u8 *));
        }
    }
    return -1;
}

/* +0x34 cut search `id`'s path back past the triangles with flags `mask` at its end: the goal
 * becomes the first one left (its centre) */
/* 0x001AA790 */
void PathPlan_CutBack(u8 *pl, s32 id, u32 mask) {
    NavMesh *nm = gNavMesh;
    u8 *s, *end, *n;
    u32 t;

    if (id == -1) {
        return;
    }
    s = SEARCH(pl, id);
    end = AT(s, 0x10048, u8 *);
    t = NODE_INDEX(s, end);
    if (!(NavMesh_TriFlags(nm, t) & mask)) {
        return;
    }
    n = AT(end, 0x4, u8 *);
    while (n != NULL) {
        t = NODE_INDEX(s, n);
        if (!(NavMesh_TriFlags(nm, t) & mask)) {
            break;
        }
        n = AT(n, 0x4, u8 *);
    }
    {
        f32 c[4] __attribute__((aligned(16)));

        AT(s, 0x10048, u8 *) = n;
        tri_centre(nm, t, c);
        AT(s, 0x8, u32) = t;
        AT(s, 0x30, f32) = c[0];
        AT(s, 0x38, f32) = c[2];
    }
}

/* +0x30 the length (x / z) of search `id`'s last path: start point, the centres of its
 * triangles, goal point */
/* 0x001AA910 */
f32 PathPlan_LastLength(u8 *pl, s32 id) {
    f32 len = 0.0f;
    f32 p[4] __attribute__((aligned(16)));
    f32 c[4] __attribute__((aligned(16)));
    f32 d[4] __attribute__((aligned(16)));
    u8 *s;
    s32 i;

    if (id == -1) {
        return len;
    }
    s = SEARCH(pl, id);
    sceVu0CopyVector(p, (f32 *)(s + 0x20));
    for (i = 1; i < AT(pl, 0x40194, s32) - 1; i++) {
        tri_centre(gNavMesh, AT(pl, 0x41198 + i * 2, u16), c);
        d[1] = 0.0f;
        d[0] = p[0] - c[0];
        d[2] = p[2] - c[2];
        len += __builtin_sqrtf(sceVu0InnerProduct(d, d));
        sceVu0CopyVector(p, c);
    }
    sceVu0CopyVector(c, (f32 *)(s + 0x30));
    d[1] = 0.0f;
    d[0] = p[0] - c[0];
    d[2] = p[2] - c[2];
    len += __builtin_sqrtf(sceVu0InnerProduct(d, d));
    return len;
}

/* +0x2C the triangle of search `id`'s path end and its centre (-1: none) */
/* 0x001AAAE0 */
s32 PathPlan_EndCentre(u8 *pl, s32 id, f32 *centre) {
    if (id != -1) {
        u8 *s = SEARCH(pl, id);

        if (AT(s, 0x10048, u8 *) != NULL) {
            u32 t = NODE_INDEX(s, AT(s, 0x10048, u8 *));

            tri_centre(gNavMesh, t, centre);
            return t;
        }
    }
    return -1;
}

/* destructor (vtable D_0046AC00) */
/* 0x001AABD0 */
void *PathPlanBase_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046AC00;
        gSceneGameF29740 = NULL;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* +0x8 destructor */
/* 0x002A8520 */
void *NavGroups_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = NavGroups_vtable;
        AT(o, 0x0, void **) = D_0046DB60;
        gRoomEventObj = NULL;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* +0xC set flag bits on group g */
/* 0x002A8590 */
s32 NavGroups_SetGroup(u8 *o, u32 g, u32 bits) {
    return NavGroups_SetFlags(o, 0, g, bits);
}

/* +0x10 clear them */
/* 0x002A85B0 */
s32 NavGroups_ClearGroup(u8 *o, u32 g, u32 bits) {
    return NavGroups_SetFlags(o, 1, g, bits);
}

/* +0x18 set flag bits on triangle t (-1: none, 0 done) */
/* 0x002A85D0 */
s32 NavGroups_SetTri(u8 *o, u32 t, u32 bits) {
    if (t == (u32)-1 || t >= AT(gNavMesh, 0x8, u32)) {
        return -1;
    }
    *tri_flags_word(t) |= bits;
    return 0;
}

/* +0x1C clear them */
/* 0x002A8640 */
s32 NavGroups_ClearTri(u8 *o, u32 t, u32 bits) {
    if (t == (u32)-1 || t >= AT(gNavMesh, 0x8, u32)) {
        return -1;
    }
    *tri_flags_word(t) &= ~bits;
    return 0;
}

/* +0x14 whether group g holds triangle t */
/* 0x002A86B0 */
s32 NavGroups_Holds(u8 *o, u32 t, u32 g) {
    u8 *sec, *grp;
    u32 n, i;

    if (g >= AT(o, 0x8, u32)) {
        return 0;
    }
    sec = AT(o, 0x4, u8 *);
    grp = sec + AT(sec, 0x4 + g * 4, u32);
    n = AT(grp, 0, u32);
    for (i = 0; i < n; i++) {
        if (AT(grp, 4 + i * 4, u32) == t) {
            return 1;
        }
    }
    return 0;
}

/* ---- room manager +0x9360 (gRoomEventObj): the room's triangle groups (PAC section 14: count,
 * then offsets of {n, triangle indices}) whose nav mesh flags scripts switch ---- */

/* set (`clear` 0) or clear (1) flag bits `bits` on the triangles of group `g` (-1: no group) */
/* 0x002A8730 */
s32 NavGroups_SetFlags(u8 *o, s32 clear, u32 g, u32 bits) {
    u8 *sec = AT(o, 0x4, u8 *);
    u8 *grp;
    u32 n, i;

    if (sec == NULL || g >= AT(o, 0x8, u32)) {
        return -1;
    }
    grp = sec + AT(sec, 0x4 + g * 4, u32);
    n = AT(grp, 0, u32);
    if (clear == 1) {
        for (i = 0; i < n; i++) {
            u32 t = AT(grp, 4 + i * 4, u32);
            u8 *tri = t < AT(gNavMesh, 0x8, u32) && AT(gNavMesh, 0x4, u8 *) != NULL
                          ? AT(gNavMesh, 0x4, u8 *) + t * 0x50 : NULL;

            AT(tri, 0x3C, u32) &= ~bits;
        }
    } else if (clear == 0) {
        for (i = 0; i < n; i++) {
            u32 t = AT(grp, 4 + i * 4, u32);
            u8 *tri = t < AT(gNavMesh, 0x8, u32) && AT(gNavMesh, 0x4, u8 *) != NULL
                          ? AT(gNavMesh, 0x4, u8 *) + t * 0x50 : NULL;

            AT(tri, 0x3C, u32) |= bits;
        }
    }
    return 0;
}

/* the base's destructor */
/* 0x002A88D0 */
void *NavGroupsBase_dtor(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046DB60;
        gRoomEventObj = NULL;
        if ((s16)flags > 0) {
            __dl__FPv(o);
        }
    }
    return o;
}

/* 0x002D1130 */
void *NavGroups_ctor(u8 *p) {
    gRoomEventObj = p;
    F(p, 0x0, void *) = NavGroups_vtable;
    F(p, 0x4, u32) = 0;
    F(p, 0x8, u32) = 0;
    return p;
}

/* 0x002D12E0 */
void *NavMesh_ctor(u8 *p) {
    s32 i;

    gNavMesh = (NavMesh *)p;
    F(p, 0x0, void *) = NavMesh_vtable;
    for (i = 0x4; i <= 0x18; i += 4) {
        F(p, i, u32) = 0;
    }
    return p;
}


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
            AT(l, 0x8 + s * 4, f32) = msl_atan2f(tmp[0], tmp[2]);
            sceVu0CopyVector(edge, a);
            sceVu0AddVector(edge, edge, b);
            vu0_ScaleXYZ((f32 *)(l + 0x10 + s * 0x10), edge, 0.5f);
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
        vu0_ScaleXYZ(hit, hit, s);
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
        vu0_ScaleXYZ(e, n, over * s);
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
            __dl__FPv(o);
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
        vu0_ScaleXYZ(out, e1, s);
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
    vu0_ScaleXYZ(out, out, k);
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
    u32 *cand = msl_malloc(AT(nm, 0x10, u32) * 4);
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
    msl_free(cand);
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
