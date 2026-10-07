#include "navmesh.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

int navmesh_build(NavMesh *n, const uint8_t *sec, size_t size) {
    uint32_t count, i;

    memset(n, 0, sizeof(*n));
    if (sec == NULL || size < 16) {
        return 0;
    }
    memcpy(&count, sec, 4);
    if (count == 0 || 16 + (size_t)count * 0x50 > size) {
        return 0;
    }
    n->tris = calloc(count, sizeof(NavTri));
    n->ntris = (int)count;
    for (i = 0; i < count; i++) {
        const uint8_t *r = sec + 0x10 + i * 0x50;
        NavTri *t = &n->tris[i];
        int k;

        for (k = 0; k < 3; k++) {
            float c[3];

            memcpy(c, r + k * 16, 12);
            t->v[k] = vec3(c[0], c[1], c[2]);
            memcpy(&t->next[k], r + 0x30 + k * 4, 4);
        }
        memcpy(&t->flags, r + 0x3C, 4);
        memcpy(&t->lights, r + 0x4C, 4);
    }
    return 1;
}

void navmesh_take_flags(NavMesh *n, const uint8_t *sec16, size_t size) {
    int i;

    for (i = 0; i < n->ntris; i++) {
        n->tris[i].flags &= 0xDF39FFFFu;
        if (sec16 != NULL && (size_t)i < size) {
            if (sec16[i] & 1) {
                n->tris[i].flags |= 0x4000;
            }
            if (sec16[i] & 2) {
                n->tris[i].flags |= 0x80000;
            }
        }
    }
    navmesh_find_links(n);
}

void navmesh_free(NavMesh *n) {
    free(n->tris);
    memset(n, 0, sizeof(*n));
}

/* barycentric weights of (x, z) in the triangle's ground shadow; 0 if outside (or degenerate) */
static int inside(const NavTri *t, float x, float z, float *w) {
    const Vec3 *a = &t->v[0], *b = &t->v[1], *c = &t->v[2];
    float det = (b->z - c->z) * (a->x - c->x) + (c->x - b->x) * (a->z - c->z);
    const float eps = 1e-4f;

    if (det > -1e-6f && det < 1e-6f) {
        return 0;
    }
    w[0] = ((b->z - c->z) * (x - c->x) + (c->x - b->x) * (z - c->z)) / det;
    w[1] = ((c->z - a->z) * (x - c->x) + (a->x - c->x) * (z - c->z)) / det;
    w[2] = 1.0f - w[0] - w[1];
    return w[0] >= -eps && w[1] >= -eps && w[2] >= -eps;
}

int navmesh_find(const NavMesh *n, Vec3 p, float climb, float *height) {
    int i, best = -1;
    float best_d = 1e30f;

    for (i = 0; i < n->ntris; i++) {
        const NavTri *t = &n->tris[i];
        float w[3], h, d;

        if ((t->flags & n->block) || !inside(t, p.x, p.z, w)) {
            continue;
        }
        h = w[0] * t->v[0].y + w[1] * t->v[1].y + w[2] * t->v[2].y;
        if (h > p.y + climb) {
            continue;   /* too high to step onto */
        }
        d = p.y - h;
        if (d < best_d) {
            best_d = d;
            best = i;
            *height = h;
        }
    }
    return best;
}

Vec3 navmesh_move(const NavMesh *n, Vec3 p, float dx, float dz, float climb, float radius) {
    const float tries[3][2] = {{dx, dz}, {dx, 0.0f}, {0.0f, dz}};   /* straight, else slide */
    NavMesh *m = (NavMesh *)n;
    uint32_t block = n->block;
    float h0;
    int i, here;
    Vec3 out = p;

    /* standing on a triangle the mask blocks (put there by a door, a script): it only keeps one
     * from going onto such triangles, not from leaving this one */
    m->block = 0;
    here = navmesh_find(n, p, climb, &h0);
    m->block = here >= 0 && (n->tris[here].flags & block) ? 0 : block;
    for (i = 0; i < 3; i++) {
        float len = sqrtf(tries[i][0] * tries[i][0] + tries[i][1] * tries[i][1]), h, h2;
        Vec3 q = vec3(p.x + tries[i][0], p.y, p.z + tries[i][1]), ahead;

        if (len == 0.0f) {
            continue;
        }
        ahead = vec3(q.x + tries[i][0] / len * radius, p.y, q.z + tries[i][1] / len * radius);
        if (navmesh_find(n, q, climb, &h) >= 0 && navmesh_find(n, ahead, climb, &h2) >= 0) {
            q.y = h;
            out = q;
            break;
        }
    }
    m->block = block;
    return out;
}

Vec3 navmesh_center(const NavMesh *n, int i) {
    const NavTri *t;

    if (i < 0 || i >= n->ntris) {
        return vec3(0, 0, 0);
    }
    t = &n->tris[i];
    return vec3_scale(vec3_add(vec3_add(t->v[0], t->v[1]), t->v[2]), 1.0f / 3.0f);
}

Vec3 navmesh_nearest(const NavMesh *n, Vec3 p) {
    int i;
    float best_d = 1e30f;
    Vec3 best = p;

    for (i = 0; i < n->ntris; i++) {
        const NavTri *t = &n->tris[i];
        Vec3 c = vec3_scale(vec3_add(vec3_add(t->v[0], t->v[1]), t->v[2]), 1.0f / 3.0f);
        Vec3 d = vec3_sub(c, p);
        float dd = vec3_dot(d, d);

        if (dd < best_d) {
            best_d = dd;
            best = c;
        }
    }
    return best;
}

/* ---- paths ---- */

typedef struct Node {
    float g, f;
    int parent, state;   /* state 0 unseen, 1 open, 2 closed */
} Node;

static float cross2(Vec3 o, Vec3 a, Vec3 b) {   /* > 0: b is on the "left" of o->a (x, z) */
    return (a.x - o.x) * (b.z - o.z) - (a.z - o.z) * (b.x - o.x);
}

/* the edge triangle t shares with u, as its two corners: left and right as seen from t's middle
 * (in cross2's sense) */
static int shared_edge(const NavMesh *n, int t, int u, Vec3 *l, Vec3 *r) {
    int e;

    for (e = 0; e < 3; e++) {
        if (n->tris[t].next[e] == u) {
            Vec3 p = n->tris[t].v[e], q = n->tris[t].v[(e + 1) % 3], c = navmesh_center(n, t);
            Vec3 mid = vec3((p.x + q.x) * 0.5f, 0.0f, (p.z + q.z) * 0.5f);

            if (cross2(c, mid, p) > 0.0f) {
                *l = p;
                *r = q;
            } else {
                *l = q;
                *r = p;
            }
            return 1;
        }
    }
    return 0;
}

int navmesh_path(const NavMesh *n, int from, Vec3 a, int to, Vec3 b, Vec3 *out, int max) {
    Node *nodes;
    int *open, nopen = 0, i, k, cur = -1, *chain, nchain = 0, npts = 0;
    Vec3 apex, left, right;
    int li = 0, ri = 0, ai = 0;
    Vec3 *pl, *pr;

    if (from < 0 || to < 0 || from >= n->ntris || to >= n->ntris || max < 1) {
        return 0;
    }
    if (from == to) {
        out[0] = b;
        return 1;
    }
    nodes = calloc((size_t)n->ntris, sizeof(Node));
    open = malloc((size_t)n->ntris * sizeof(int));
    if (nodes == NULL || open == NULL) {
        free(nodes);
        free(open);
        return 0;
    }
    nodes[from].state = 1;
    nodes[from].parent = -1;
    nodes[from].f = vec3_len(vec3_sub(b, a));
    open[nopen++] = from;
    while (nopen > 0) {
        int best = 0;
        Vec3 c;

        for (i = 1; i < nopen; i++) {
            if (nodes[open[i]].f < nodes[open[best]].f) {
                best = i;
            }
        }
        cur = open[best];
        open[best] = open[--nopen];
        nodes[cur].state = 2;
        if (cur == to) {
            break;
        }
        c = cur == from ? a : navmesh_center(n, cur);
        for (k = 0; k < 3; k++) {
            int u = n->tris[cur].next[k];
            Vec3 cu;
            float g;

            if (u < 0 || u >= n->ntris || nodes[u].state == 2 || (n->tris[u].flags & n->block)) {
                continue;
            }
            cu = u == to ? b : navmesh_center(n, u);
            g = nodes[cur].g + vec3_len(vec3_sub(cu, c));
            if (nodes[u].state == 0 || g < nodes[u].g) {
                nodes[u].g = g;
                nodes[u].f = g + vec3_len(vec3_sub(b, cu));
                nodes[u].parent = cur;
                if (nodes[u].state == 0) {
                    open[nopen++] = u;
                }
                nodes[u].state = 1;
            }
        }
        cur = -1;
    }
    free(open);
    if (cur != to) {
        free(nodes);
        return 0;
    }
    /* the chain of triangles, start to end */
    for (k = to; k >= 0; k = nodes[k].parent) {
        nchain++;
    }
    chain = malloc((size_t)nchain * sizeof(int));
    pl = malloc((size_t)(nchain + 1) * sizeof(Vec3));
    pr = malloc((size_t)(nchain + 1) * sizeof(Vec3));
    if (chain == NULL || pl == NULL || pr == NULL) {
        free(chain);
        free(pl);
        free(pr);
        free(nodes);
        return 0;
    }
    i = nchain;
    for (k = to; k >= 0; k = nodes[k].parent) {
        chain[--i] = k;
    }
    free(nodes);
    /* the portals: each shared edge, then the end point as a closed one */
    for (i = 0; i + 1 < nchain; i++) {
        shared_edge(n, chain[i], chain[i + 1], &pl[i], &pr[i]);
    }
    pl[nchain - 1] = pr[nchain - 1] = b;
    /* the funnel (simple stupid funnel algorithm) */
    apex = left = right = a;
    for (i = 0; i < nchain && npts < max; i++) {
        Vec3 l = pl[i], r = pr[i];

        if (cross2(apex, right, r) >= 0.0f) {   /* the right side narrows */
            if (vec3_len(vec3_sub(apex, right)) < 1e-4f || cross2(apex, left, r) < 0.0f) {
                right = r;
                ri = i;
            } else {   /* it crosses the left: the left corner is a turning point */
                out[npts++] = left;
                apex = right = left;
                ai = ri = li;
                i = ai;
                continue;
            }
        }
        if (cross2(apex, left, l) <= 0.0f) {   /* the left side narrows */
            if (vec3_len(vec3_sub(apex, left)) < 1e-4f || cross2(apex, right, l) > 0.0f) {
                left = l;
                li = i;
            } else {
                out[npts++] = right;
                apex = left = right;
                ai = li = ri;
                i = ai;
                continue;
            }
        }
    }
    if (npts < max && (npts == 0 || vec3_len(vec3_sub(out[npts - 1], b)) > 1e-4f)) {
        out[npts++] = b;
    }
    (void)ai;
    free(chain);
    free(pl);
    free(pr);
    return npts;
}

/* ---- walking straight ---- */

/* where segment p-q (x, z) crosses segment a-b: its fraction along p-q, -1 if it doesn't */
static float seg_cross(Vec3 p, Vec3 q, Vec3 a, Vec3 b) {
    float rx = q.x - p.x, rz = q.z - p.z, sx = b.x - a.x, sz = b.z - a.z;
    float den = rx * sz - rz * sx, t, u;

    if (fabsf(den) < 1e-9f) {
        return -1.0f;
    }
    t = ((a.x - p.x) * sz - (a.z - p.z) * sx) / den;
    u = ((a.x - p.x) * rz - (a.z - p.z) * rx) / den;
    return t >= 0.0f && u >= -1e-4f && u <= 1.0f + 1e-4f ? t : -1.0f;
}

int navmesh_wall(const NavMesh *n, int from, Vec3 a, Vec3 b, float *yaw) {
    int t = from, came = -1, steps;
    float w[3];

    if (t < 0 || t >= n->ntris) {
        return 0;
    }
    for (steps = 0; steps < n->ntris + 2; steps++) {
        const NavTri *tr = &n->tris[t];
        float best = -1.0f;
        int e, cross = -1;

        if (inside(tr, b.x, b.z, w)) {
            return 0;
        }
        for (e = 0; e < 3; e++) {
            float f = seg_cross(a, b, tr->v[e], tr->v[(e + 1) % 3]);

            if (f < 0.0f || f > 1.0f || (came >= 0 && tr->next[e] == came)) {
                continue;
            }
            if (cross < 0 || f > best) {
                best = f;
                cross = e;
            }
        }
        if (cross < 0) {
            return 0;
        }
        if (tr->next[cross] < 0 || (n->tris[tr->next[cross]].flags & n->block)) {
            Vec3 p0 = tr->v[cross], p1 = tr->v[(cross + 1) % 3];

            *yaw = atan2f(p1.x - p0.x, p1.z - p0.z);
            return 1;
        }
        came = t;
        t = tr->next[cross];
    }
    return 0;
}

int navmesh_walk(const NavMesh *n, int from, Vec3 a, Vec3 b, float *reach) {
    float len = sqrtf((b.x - a.x) * (b.x - a.x) + (b.z - a.z) * (b.z - a.z)), w[3], done = 0.0f;
    int t = from, came = -1, steps;

    if (reach != NULL) {
        *reach = 0.0f;
    }
    if (t < 0 || t >= n->ntris) {
        return -1;
    }
    for (steps = 0; steps < n->ntris + 2; steps++) {
        const NavTri *tr = &n->tris[t];
        float best = -1.0f;
        int e, cross = -1;

        if (inside(tr, b.x, b.z, w)) {
            if (reach != NULL) {
                *reach = len;
            }
            return t;
        }
        for (e = 0; e < 3; e++) {   /* the edge it leaves by: the farthest crossing ahead */
            float f = seg_cross(a, b, tr->v[e], tr->v[(e + 1) % 3]);

            if (f < 0.0f || f > 1.0f || (came >= 0 && tr->next[e] == came)) {
                continue;
            }
            if (cross < 0 || f > best) {
                best = f;
                cross = e;
            }
        }
        if (cross < 0) {
            return -1;
        }
        done = best * len;
        if (tr->next[cross] < 0 || (n->tris[tr->next[cross]].flags & n->block)) {
            if (reach != NULL) {
                *reach = done;
            }
            return -1;
        }
        came = t;
        t = tr->next[cross];
    }
    return -1;
}

/* ---- links (ladders) ---- */

void navmesh_find_links(NavMesh *n) {
    int i, g, sd;

    for (g = 0; g < 5; g++) {
        n->links[g].tri[0] = n->links[g].tri[1] = -1;
    }
    for (i = 0; i < n->ntris; i++) {
        uint32_t grp = n->tris[i].flags & 0x3E00;
        NavLink *l;

        if (grp == 0 || (grp >> 9) > 5) {
            continue;
        }
        l = &n->links[(grp >> 9) - 1];
        if (l->tri[0] == -1) {
            l->tri[0] = i;
        } else if (n->tris[i].v[0].y <= n->tris[l->tri[0]].v[0].y) {
            l->tri[1] = i;
        } else {
            l->tri[1] = l->tri[0];
            l->tri[0] = i;
        }
    }
    for (g = 0; g < 5; g++) {
        NavLink *l = &n->links[g];
        Vec3 c[2];

        if (l->tri[0] == -1 || l->tri[1] == -1) {
            break;
        }
        for (sd = 0; sd < 2; sd++) {
            c[sd] = navmesh_center(n, l->tri[sd]);
        }
        for (sd = 0; sd < 2; sd++) {   /* the edge the way to the other crosses (vt+0x20) */
            const NavTri *t = &n->tris[l->tri[sd]];
            float best = -1.0f, ex, ez, px, pz, len;
            int e, cross = 0;
            Vec3 a, b;

            for (e = 0; e < 3; e++) {
                float fr = seg_cross(c[sd], c[sd ^ 1], t->v[e], t->v[(e + 1) % 3]);

                if (fr >= 0.0f && (best < 0.0f || fr < best)) {
                    best = fr;
                    cross = e;
                }
            }
            a = t->v[cross];
            b = t->v[(cross + 1) % 3];
            ex = b.x - a.x;
            ez = b.z - a.z;
            px = -ez;   /* across the edge, away from its own middle */
            pz = ex;
            if (px * (c[sd].x - a.x) + pz * (c[sd].z - a.z) > 0.0f) {
                px = -px;
                pz = -pz;
            }
            len = sqrtf(px * px + pz * pz);
            l->yaw[sd] = len > 0.0f ? atan2f(px / len, pz / len) : 0.0f;
            l->spot[sd] = vec3((a.x + b.x) * 0.5f, (a.y + b.y) * 0.5f, (a.z + b.z) * 0.5f);
        }
    }
    for (g = 0; g < 5 && n->links[g].tri[0] != -1 && n->links[g].tri[1] != -1; g++) {
    }
    n->nlinks = g;
}

int navmesh_link_at(const NavMesh *n, int i, Vec3 p) {
    int sd;

    if (i < 0 || i >= n->nlinks) {
        return 0;
    }
    for (sd = 0; sd < 2; sd++) {
        Vec3 s = n->links[i].spot[sd];
        float dx = p.x - s.x, dz = p.z - s.z;

        if (fabsf(p.y - s.y) <= 5.0f && sqrtf(dx * dx + dz * dz) <= 20.0f) {
            return 1;
        }
    }
    return 0;
}

int navmesh_link_side(const NavMesh *n, int i, int tri, Vec3 p) {
    int sd;

    if (i < 0 || i >= n->nlinks) {
        return -1;
    }
    for (sd = 0; sd < 2; sd++) {
        Vec3 s = n->links[i].spot[sd];
        float dx = p.x - s.x, dz = p.z - s.z;
        NavMesh m = *n;

        if (!(fabsf(p.y - s.y) <= 5.0f) || !(sqrtf(dx * dx + dz * dz) <= 12.0f)) {
            continue;
        }
        m.block = 0;
        if (navmesh_walk(&m, tri, p, s, NULL) == n->links[i].tri[sd]) {
            return sd;
        }
    }
    return -1;
}

int navmesh_link_front(const NavMesh *n, int i, int side, float ox, float oz, Vec3 *out) {
    float dx, dz, yaw, sn, cs;
    Vec3 base, target;
    NavMesh m;
    int t;

    if (i < 0 || i >= n->nlinks || side < 0 || side > 1) {
        return -1;
    }
    dx = -ox;
    dz = side == 0 ? 5.0f + oz : -(oz - 5.0f);
    yaw = n->links[i].yaw[side];
    sn = sinf(yaw);
    cs = cosf(yaw);
    base = n->links[i].spot[side];
    target = vec3(base.x + cs * dx + sn * dz, base.y, base.z - sn * dx + cs * dz);
    m = *n;
    m.block = 0;
    t = navmesh_walk(&m, n->links[i].tri[side], base, target, NULL);
    if (t < 0) {
        return -1;
    }
    {
        float h;
        int u = navmesh_find(&m, target, 1000.0f, &h);

        target.y = u == t ? h : base.y;
    }
    *out = target;
    return t;
}
