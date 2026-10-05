/* The path planner (SceneGame +0xF29740, gSceneGameF29740, vtable D_0046ABB0): A* over the
 * nav mesh's triangles for the characters (Hewie, the stalkers, the event characters).
 *
 * Four searches (+0x10, 0x10060 bytes each; +0x4 the mask of the ones in use, search 3 the one
 * taken when none is asked for). A search:
 *   +0x0      steps taken
 *   +0x4      start triangle, +0x8 goal triangle, +0xC the triangle flags that stop it
 *   +0x10     the triangle flags that end it (0, 4: as the goal)
 *   +0x20     start point, +0x30 goal point
 *   +0x40     open list length (s16), +0x42 closed list length (s16)
 *   +0x44     a node per triangle (0x18 bytes): +0 flags (1 open, 2 closed, 4 at the goal, 8),
 *             +2 its place in the open list, +4 the node it was reached from, +8 the next in a
 *             list, +0xC the cost so far, +0x10 the estimate to the goal, +0x14
 * A triangle's +0x40 holds the cost of crossing each of its edges.
 *   +0xC044   the open list (node pointers, sorted by cost), +0xE044 the closed list
 *   +0x10044  the node being expanded, +0x10048 the path's end node
 *   +0x1004C  the step function (PTMF; it returns nonzero when the search is over)
 *   +0x10058  the nearest triangle found when the goal can't be reached, +0x1005C its distance
 * +0x40194 the last path's length (triangles), +0x41198 its triangles (u16) */
#include "common.h"
#include "game.h"
#include "navmesh.h"
#include "ptmf.h"
#include "sce/libvu0.h"

extern void *D_0046ABB0[], *D_0046AC00[];
extern VObject *gSceneGameF29740;
extern const u32 D_003B2EA8[];   /* the step functions (PTMFs, 16-byte aligned) by request kind 0..7; 8: at the goal */
extern void func_00100490(void *p);                          /* operator delete */
extern void func_0010E640(f32 *out, const f32 *v, f32 s);   /* scale x, y, z */

#define SEARCH(pl, i) ((u8 *)(pl) + 0x10 + (i) * 0x10060)
#define NODE(s, t) ((s) + 0x44 + (t) * 0x18)
#define NODE_INDEX(s, n) ((u32)((u8 *)(n) - ((s) + 0x44)) / 0x18)

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
    AT(s, 0x1004C, u32) = D_003B2EA8[k * 4 + 0];
    AT(s, 0x10050, u32) = D_003B2EA8[k * 4 + 1];
    AT(s, 0x10054, u32) = D_003B2EA8[k * 4 + 2];
}

/* the centre of triangle `t` (nav mesh +0xC) */
static inline void tri_centre(NavMesh *nm, u32 t, f32 *out) {
    VCALL((VObject *)nm, 0xC, void (*)(VObject *, u32, f32 *))((VObject *)nm, t, out);
}

/* +0x8 destructor */
void *func_001A4970(void *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0046ABB0;
        if (o != NULL) {
            AT(o, 0x0, void **) = D_0046AC00;
            if (o != NULL) {
                gSceneGameF29740 = NULL;
            }
        }
        if ((s16)flags > 0) {
            func_00100490(o);
        }
    }
    return o;
}

/* the estimates to the goal (+0x10) of every triangle for the request kinds 3..5: the distance
 * from its centre to the goal point (`req` +0x8: 0 plain, 1 heights x 10, 2 also +1000 on
 * triangles flagged 0x40) */
void func_001A49E0(void *pl, u8 *s, u8 *req) {
    NavMesh *nm;
    u32 n, t;
    u8 *node;

    if (AT(req, 0x4, s32) != 5 && AT(req, 0x4, s32) != 4 && AT(req, 0x4, s32) != 3) {
        return;
    }
    nm = D_0044E570;
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
s32 func_001A4C60(u8 *pl, u8 *s) {
    AT(pl, 0x40198, s16) = AT(s, 0x4, s32);
    AT(pl, 0x4019A, s16) = AT(s, 0x8, s32);
    AT(s, 0x10048, u8 *) = NODE(s, AT(s, 0x4, s32));
    AT(AT(s, 0x10048, u8 *), 0x4, void *) = NULL;
    return 2;
}

/* triangle `t`'s flags as the original reads them (a bad index reads through NULL) */
static inline u32 tri_flags(NavMesh *nm, u32 t) {
    return NavMesh_TriFlags(nm, t);
}

/* step for kind 0, breadth first: the queue's head (+0xC044) expanded, its new neighbours (not
 * stopped by +0xC) queued; one at the goal (flag 4, or triangle flags of +0x10) ends it. 0 to
 * go on, the steps when found, -1 / -steps when it can't be reached */
s32 func_001A8040(void *pl, u8 *s) {
    NavMesh *nm;
    u8 *cur;
    NavTri *tri;
    s32 n, e, i;

    AT(s, 0x0, s32)++;
    cur = AT(s, 0xC044, u8 *);
    n = AT(s, 0x40, s16);
    nm = D_0044E570;
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

/* step for kind 1, depth first: as kind 0, but the head's new neighbours take its place at
 * the front of the list */
s32 func_001A7C30(void *pl, u8 *s) {
    u8 *nbs[3];
    NavMesh *nm;
    u8 *cur;
    NavTri *tri;
    s32 n, k = 0, e, i;

    AT(s, 0x0, s32)++;
    cur = AT(s, 0xC044, u8 *);
    n = AT(s, 0x40, s16);
    nm = D_0044E570;
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

/* step for kind 6, greedy best first: the node +0x10044 expanded; its new neighbours get the
 * estimate (heights x 10, +5 on triangles flagged 0x40) and go into the list (by +8) sorted by
 * it; a seen neighbour cheaper to come from becomes the node's parent */
s32 func_001A5750(void *pl, u8 *s) {
    u8 *nbs[3];
    NavMesh *nm;
    u8 *cur, *head;
    NavTri *tri;
    s32 k = 0, e, i;

    AT(s, 0x0, s32)++;
    cur = AT(s, 0x10044, u8 *);
    nm = D_0044E570;
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
s32 func_001A5BB0(void *pl, u8 *s) {
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
    nm = D_0044E570;
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

#define OPEN(s, i) AT(s, 0xC044 + (i) * 4, u8 *)

/* step for kind 3, greedy best first on the sorted list (+0xC044, the head expanded): its new
 * neighbours, sorted by the estimate (+0x10), are merged in; the best of them replaces the head,
 * found with the goal node as a sentinel past the end (its estimate set above it) */
s32 func_001A6D70(void *pl, u8 *s) {
    u8 *nbs[3];
    NavMesh *nm;
    u8 *cur;
    NavTri *tri;
    s32 n, k = 0, e, i, j;

    AT(s, 0x0, s32)++;
    cur = OPEN(s, 0);
    n = AT(s, 0x40, s16);
    nm = D_0044E570;
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

/* put node `n` at place `i` of the open list (its +2 too) */
#define OPEN_PUT(s, i, n) (OPEN(s, i) = (n), AT(OPEN(s, i), 0x2, s16) = (i))

/* an open node `nb` reached cheaper (its cost +0xC lowered): moved up the list to its place */
static inline __attribute__((always_inline)) void open_raise(u8 *s, u8 *nb) {
    s32 pos = AT(nb, 0x2, s16);

    if (pos == 1) {
        return;
    }
    if (AT(nb, 0xC, f32) < AT(OPEN(s, 1), 0xC, f32)) {
        for (; pos >= 2; pos--) {
            OPEN_PUT(s, pos, OPEN(s, pos - 1));
        }
    } else {
        while (AT(nb, 0xC, f32) < AT(OPEN(s, pos - 1), 0xC, f32)) {
            OPEN_PUT(s, pos, OPEN(s, pos - 1));
            pos--;
        }
    }
    OPEN_PUT(s, pos, nb);
}

/* the open list (`n`, its head just expanded) after the `k` new nodes, sorted by the cost +0xC,
 * merged in (places kept at +2): none - the head goes; else the best replaces the head, found
 * with the goal node as a sentinel past the end (its cost set above it) */
static inline __attribute__((always_inline)) void open_merge(u8 *s, u8 **nbs, s32 k, s32 n) {
    s32 i, j;

    if (k == 0) {
        for (i = 0; i < n - 1; i++) {
            OPEN_PUT(s, i, OPEN(s, i + 1));
        }
        return;
    }
    for (i = 1; i < k; i++) {
        for (j = 0; j < k - i; j++) {
            if (!(AT(nbs[j], 0xC, f32) <= AT(nbs[j + 1], 0xC, f32))) {
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

            if (AT(x, 0xC, f32) < AT(OPEN(s, 1), 0xC, f32)) {
                for (; at > 0; at--) {
                    OPEN_PUT(s, at + i, OPEN(s, at));
                }
            } else {
                while (AT(x, 0xC, f32) < AT(OPEN(s, at), 0xC, f32)) {
                    OPEN_PUT(s, at + i, OPEN(s, at));
                    at--;
                }
            }
            OPEN_PUT(s, at + i, x);
        }
        g = NODE(s, AT(s, 0x8, s32));
        OPEN(s, n + k - 1) = g;
        AT(g, 0xC, f32) = 1.0f + AT(nbs[0], 0xC, f32);
        for (i = 0; !(AT(nbs[0], 0xC, f32) <= AT(OPEN(s, i + 1), 0xC, f32)); i++) {
            OPEN_PUT(s, i, OPEN(s, i + 1));
        }
        OPEN_PUT(s, i, nbs[0]);
    }
}

/* step for kind 7, cheapest first by the cost so far (+0xC) on the sorted list, places kept at
 * +2 (a cheaper way to an open node moves it up), within 30 of the start (blocked triangles
 * explored without the check): the first triangle
 * further (or the list running out) ends it at the nearest one seen (+0x10058) */
s32 func_001A4CC0(void *pl, u8 *s) {
    u8 *nbs[3];
    NavMesh *nm;
    u8 *cur;
    NavTri *tri;
    s32 n, k = 0, e;

    AT(s, 0x0, s32)++;
    cur = OPEN(s, 0);
    n = AT(s, 0x40, s16);
    nm = D_0044E570;
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
            open_raise(s, nb);
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
    open_merge(s, nbs, k, n);
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

/* step for kind 2, cheapest first (Dijkstra) by the cost so far (+0xC) on the sorted list,
 * places kept at +2 */
s32 func_001A7320(void *pl, u8 *s) {
    u8 *nbs[3];
    NavMesh *nm;
    u8 *cur;
    NavTri *tri;
    s32 n, k = 0, e;

    AT(s, 0x0, s32)++;
    cur = OPEN(s, 0);
    n = AT(s, 0x40, s16);
    nm = D_0044E570;
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
            open_raise(s, nb);
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
    open_merge(s, nbs, k, n);
    AT(s, 0x40, s16) = n + k - 1;
    if (n + k != 0) {
        return 0;
    }
    return -AT(s, 0x0, s32);
}

/* +0x40 the length of search `id`'s path (-1 when it has none) */
f32 func_001A8300(VObject *pl, s32 id) {
    extern s32 func_001A95F0(VObject *pl, u8 *s);

    if (id == -1) {
        return -1.0f;
    }
    if (func_001A95F0(pl, SEARCH(pl, id)) == 0) {
        return VCALL(pl, 0x30, f32 (*)(VObject *, s32))(pl, id);
    }
    return -1.0f;
}

/* +0x3C the length (x / z) of path points from + 1 .. to - 1, from `pos` (points 0xC bytes:
 * triangle, x, z) */
f32 func_001A8380(void *pl, const f32 *pos, s32 from, s32 to, const u8 *pts) {
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
s32 func_001A8400(void *pl, u32 *tri, f32 *pos, s32 i, s32 n, const u8 *pts, f32 dist) {
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
    func_0010E640(d, d, dist);
    sceVu0AddVector(p, p, d);
    *tri = cur;
    pos[0] = p[0];
    pos[2] = p[2];
    pos[3] = 1.0f;
    return i - 1;
}

/* +0x14 run search 0 to its end */
void func_001A9F90(u8 *pl) {
    u8 *s = SEARCH(pl, 0);

    while (search_step(pl, s) == 0) {
    }
}

/* +0x10 a step of search `id` */
void func_001A9FF0(u8 *pl, s32 id) {
    if (id == -1) {
        return;
    }
    search_step(pl, SEARCH(pl, id));
}

/* +0xC start a search for request `req` (+0x0 kind of end, +0x4 kind 0..7, +0x8 estimate
 * kind, +0xC start triangle, +0x10 start point, +0x20 goal triangle, +0x30 goal point, +0x40
 * stop flags) in search `id` (-1: search 3 if free); the search, or -1 (a point off its
 * triangle, or none free) */
s32 func_001AA040(u8 *pl, u8 *req, s32 id) {
    NavMesh *nm;
    u8 *s, *start;
    u32 n, t;

    if (AT(req, 0xC, s32) == -1 || AT(req, 0x20, s32) == -1) {
        return -1;
    }
    if (AT(req, 0x4, s32) < 0 || AT(req, 0x4, s32) >= 8) {
        return -1;
    }
    nm = D_0044E570;
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
    func_001A49E0(pl, s, req);
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
s32 func_001AA720(u8 *pl, s32 id) {
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
void func_001AA790(u8 *pl, s32 id, u32 mask) {
    NavMesh *nm = D_0044E570;
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
f32 func_001AA910(u8 *pl, s32 id) {
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
        tri_centre(D_0044E570, AT(pl, 0x41198 + i * 2, u16), c);
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
s32 func_001AAAE0(u8 *pl, s32 id, f32 *centre) {
    if (id != -1) {
        u8 *s = SEARCH(pl, id);

        if (AT(s, 0x10048, u8 *) != NULL) {
            u32 t = NODE_INDEX(s, AT(s, 0x10048, u8 *));

            tri_centre(D_0044E570, t, centre);
            return t;
        }
    }
    return -1;
}
