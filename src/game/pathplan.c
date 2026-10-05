/* The path planner (SceneGame +0xF29740, gSceneGameF29740, vtable D_0046ABB0): A* over the
 * nav mesh's triangles for the characters (Hewie, the stalkers, the event characters).
 *
 * Four searches (+0x10, 0x10060 bytes each; +0x4 the mask of the ones in use, search 3 the one
 * taken when none is asked for). A search:
 *   +0x0      steps taken
 *   +0x4      start triangle, +0x8 goal triangle, +0xC the triangle flags that stop it
 *   +0x10     how it ends (0, 4)
 *   +0x20     start point, +0x30 goal point
 *   +0x40     open list length (s16), +0x42 closed list length (s16)
 *   +0x44     a node per triangle (0x18 bytes): +0 flags (1 open, 2 closed, 4 at the goal, 8),
 *             +2 its place in the open list, +4 the node it was reached from, +8, +0xC the cost
 *             so far, +0x10 the estimate to the goal, +0x14
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
