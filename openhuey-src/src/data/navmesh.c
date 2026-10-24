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
        memcpy(t->cost, r + 0x40, 12);
        memcpy(&t->lights, r + 0x4C, 4);
    }
    return 1;
}

void navmesh_take_flags(NavMesh *n, const uint8_t *sec16, size_t size) {
    int i;

    free(n->marks);
    n->marks = NULL;
    n->nmarks = 0;
    if (sec16 != NULL && size > 0) {
        n->marks = malloc(size);
        if (n->marks != NULL) {
            memcpy(n->marks, sec16, size);
            n->nmarks = (int)size;
        }
    }
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
    free(n->marks);
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
        /* (and not onto a floor more than a climb above: the game's y runs down - a balcony over
         * her when the floor ahead is blocked) */
        if (navmesh_find(n, q, climb, &h) >= 0 && navmesh_find(n, ahead, climb, &h2) >= 0
            && h >= p.y - climb && h2 >= p.y - climb) {
            q.y = h;
            out = q;
            m->block = block;
            return out;
        }
    }
    /* no way on nor along: up to the wall straight ahead (Actor_Move stops at it) */
    if (here >= 0 && radius == 0.0f) {
        float len = sqrtf(dx * dx + dz * dz), reach, h;

        if (len > 0.0f) {
            navmesh_walk(n, here, p, vec3(p.x + dx, p.y, p.z + dz), &reach);
            reach -= 0.01f;
            if (reach > 0.0f && reach < len) {
                Vec3 q = vec3(p.x + dx * reach / len, p.y, p.z + dz * reach / len);

                if (navmesh_find(n, q, climb, &h) >= 0 && h >= p.y - climb) {
                    q.y = h;
                    out = q;
                }
            }
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

/* ---- curved paths ---- */

static Vec3 scaled_to(Vec3 v, float len) {   /* v made `len` long (on the level) */
    float l = sqrtf(v.x * v.x + v.z * v.z);

    return l > 0.0f ? vec3(v.x * len / l, 0.0f, v.z * len / l) : v;
}
static float len2(Vec3 v) { return v.x * v.x + v.z * v.z; }

/* a turning point moved `by` away from the corner it turns round (out of the turn), when that
 * is on the floor: the funnel's corners sit on the walls' corners, the original's curve went by
 * the triangles' centres */
static Vec3 off_the_corner(const NavMesh *n, Vec3 prev, Vec3 p, Vec3 next, float by) {
    Vec3 u = vec3_sub(prev, p), w = vec3_sub(next, p), in;
    float lu = sqrtf(len2(u)), lw = sqrtf(len2(w)), h;
    Vec3 q;

    if (lu <= 0.0f || lw <= 0.0f) {
        return p;
    }
    in = vec3(u.x / lu + w.x / lw, 0.0f, u.z / lu + w.z / lw);   /* (into the turn) */
    if (len2(in) < 1e-6f) {
        return p;
    }
    in = scaled_to(in, by);
    q = vec3(p.x - in.x, p.y, p.z - in.z);
    {   /* (only where the way still walks straight from the last point to it and on to the next) */
        int tp = navmesh_find(n, prev, 4.0f, &h), tq = navmesh_find(n, q, 4.0f, &h);

        if (tp < 0 || tq < 0 || navmesh_walk(n, tp, prev, q, NULL) < 0 || navmesh_walk(n, tq, q, next, NULL) < 0) {
            return p;
        }
    }
    return q;
}

int navmesh_curve(const NavMesh *n, int from, Vec3 a, const Vec3 *given, int npts, Vec3 *out, int max) {
    Vec3 pts[64];
    int i, j, k = 0, tri = from;

    if (npts * 8 > max || npts < 2 || from < 0 || npts > 64) {   /* (nothing to round, or too long: as it is) */
        for (i = 0; i < npts && i < max; i++) {
            out[i] = given[i];
        }
        return npts < max ? npts : max;
    }
    for (i = 0; i < npts; i++) {   /* (each from the one before it as moved) */
        pts[i] = i + 1 < npts ? off_the_corner(n, i == 0 ? a : pts[i - 1], given[i], given[i + 1], 2.0f) : given[i];
    }
    for (i = 0; i < npts; i++) {
        Vec3 s = i == 0 ? a : pts[i - 1], e = pts[i];
        Vec3 before = i <= 1 ? a : pts[i - 2], after = i + 1 < npts ? pts[i + 1] : e;
        Vec3 d1 = vec3_sub(e, i == 0 ? s : before), d2 = vec3_sub(after, s);
        float l1 = len2(d1), l2 = len2(d2), l3 = 2.0f * len2(vec3_sub(e, s));
        Vec3 piece[8];
        int tries, ok = 0, t;
        float h;

        if (len2(vec3_sub(e, s)) < 1e-4f) {   /* (the same point twice: nothing to walk) */
            continue;
        }
        if (i > 0) {   /* (the stretch's start found afresh: moved off its corner) */
            int found = navmesh_find(n, s, 4.0f, &h);

            tri = found >= 0 ? found : tri;
        }
        t = tri;

        if (i + 1 == npts) {   /* (the last: toward its end) */
            d2 = vec3_sub(e, s);
            l2 = len2(d2);
        }
        if (l1 < l3) {   /* (the tangents no longer than the stretch allows: as the original) */
            if (l1 < l2) {
                d2 = scaled_to(d2, sqrtf(l1));
            } else {
                d1 = scaled_to(d1, sqrtf(l2));
            }
        } else if (l2 < l3) {
            d1 = scaled_to(d1, sqrtf(l2));
        } else {
            d1 = scaled_to(d1, sqrtf(l3));
            d2 = scaled_to(d2, sqrtf(l3));
        }
        for (tries = 0; tries < 10 && !ok; tries++) {
            Vec3 at = s;

            d1 = vec3(0.5f * d1.x, 0.0f, 0.5f * d1.z);
            d2 = vec3(0.5f * d2.x, 0.0f, 0.5f * d2.z);
            t = tri;
            ok = 1;
            for (j = 1; j <= 8 && ok; j++) {
                float u = (float)j / 8.0f, u2 = u * u, u3 = u2 * u;
                float h00 = 1.0f + 2.0f * u3 - 3.0f * u2, h01 = 3.0f * u2 - 2.0f * u3;
                float h10 = u + u3 - 2.0f * u2, h11 = u3 - u2;
                Vec3 q = j == 8 ? e
                                : vec3(h00 * s.x + h01 * e.x + h10 * d1.x + h11 * d2.x, s.y + u * (e.y - s.y),
                                       h00 * s.z + h01 * e.z + h10 * d1.z + h11 * d2.z);

                t = navmesh_walk(n, t, at, q, NULL);   /* (each step straight over the floor) */
                if (t < 0) {
                    ok = 0;
                    break;
                }
                piece[j - 1] = q;
                at = q;
            }
        }
        if (ok) {
            for (j = 0; j < 8; j++) {
                out[k++] = piece[j];
            }
            tri = t;
        } else {   /* (straight) */
            out[k++] = e;
            t = navmesh_walk(n, tri, s, e, NULL);
            tri = t >= 0 ? t : tri;
        }
    }
    return k;
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

int navmesh_walk_between(const NavMesh *n, int from, Vec3 a, int to, Vec3 b, uint32_t mask) {
    int t = from, came = -1, steps;
    float w[3];

    if (from < 0 || from >= n->ntris || to < 0 || to >= n->ntris) {
        return NAV_WALK_FAIL;
    }
    for (steps = 0; steps < n->ntris + 2; steps++) {
        const NavTri *tr = &n->tris[t];
        float best = -1.0f;
        int e, cross = -1;

        if (tr->flags & mask) {
            return t;
        }
        if (inside(tr, b.x, b.z, w)) {
            return t == to ? NAV_WALK_REACHED : NAV_WALK_STUCK;
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
            return NAV_WALK_FAIL;
        }
        if (tr->next[cross] < 0) {
            return NAV_WALK_OFF;
        }
        came = t;
        t = tr->next[cross];
    }
    return NAV_WALK_FAIL;
}

int navmesh_can_walk_between(const NavMesh *n, int ta, Vec3 a, int tb, Vec3 b, uint32_t mask) {
    uint32_t walk;
    int r, r2;

    if (mask == 0) {
        mask = 0x40080;
    }
    walk = mask | 0x4000;
    r = navmesh_walk_between(n, ta, a, tb, b, walk);
    if (r == NAV_WALK_REACHED) {
        return 1;
    }
    if (r == NAV_WALK_OFF || r == NAV_WALK_FAIL) {
        return 0;
    }
    if (r != NAV_WALK_STUCK && (mask & n->tris[r].flags)) {
        return 0;
    }
    r2 = navmesh_walk_between(n, tb, b, ta, a, walk);
    if (r2 == NAV_WALK_REACHED) {
        return 0;
    }
    if (r2 == NAV_WALK_STUCK) {
        return r != NAV_WALK_STUCK;
    }
    if (r2 == NAV_WALK_OFF || r2 == NAV_WALK_FAIL) {
        return 0;
    }
    return (mask & n->tris[r2].flags) ? 0 : 1;
}

float navmesh_height(const NavMesh *n, int t, float x, float z) {
    const NavTri *tr;
    float w[3];

    if (t < 0 || t >= n->ntris) {
        return 0.0f;
    }
    tr = &n->tris[t];
    inside(tr, x, z, w);   /* (outside it: its plane carried on) */
    return w[0] * tr->v[0].y + w[1] * tr->v[1].y + w[2] * tr->v[2].y;
}

Vec3 navmesh_normal(const NavMesh *n, int t) {
    const NavTri *tr;
    Vec3 a, b, c;
    float l;

    if (t < 0 || t >= n->ntris) {
        return vec3(0, 1, 0);
    }
    tr = &n->tris[t];
    a = vec3(tr->v[1].x - tr->v[0].x, tr->v[1].y - tr->v[0].y, tr->v[1].z - tr->v[0].z);
    b = vec3(tr->v[2].x - tr->v[0].x, tr->v[2].y - tr->v[0].y, tr->v[2].z - tr->v[0].z);
    c = vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
    l = sqrtf(c.x * c.x + c.y * c.y + c.z * c.z);
    if (l <= 0.0f) {
        return vec3(0, 1, 0);
    }
    if (c.y < 0.0f) {
        l = -l;
    }
    return vec3(c.x / l, c.y / l, c.z / l);
}

int navmesh_fly(const NavMesh *n, int from, Vec3 a, Vec3 b, uint32_t mask, Vec3 *out, Vec3 *normal, int *what) {
    int t = from, came = -1, steps;

    *out = b;
    *normal = vec3(0, 1, 0);
    *what = NAV_REACHED;
    if (t < 0 || t >= n->ntris) {
        return -1;
    }
    for (steps = 0; steps < n->ntris + 2; steps++) {
        const NavTri *tr = &n->tris[t];
        float best = -1.0f, w[3];
        int e, cross = -1, next;
        Vec3 c;

        if (inside(tr, b.x, b.z, w)) {
            return t;
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
            return t;   /* (on its edge: as reached) */
        }
        c = vec3(a.x + (b.x - a.x) * best, a.y + (b.y - a.y) * best, a.z + (b.z - a.z) * best);
        next = tr->next[cross];
        if (next >= 0 && (n->tris[next].flags & 0x10000000)) {   /* a hole */
            *out = c;
            *what = NAV_HOLE;
            return next;
        }
        if (next < 0 || (n->tris[next].flags & mask) || navmesh_height(n, next, c.x, c.z) > c.y + 0.5f) {
            Vec3 e0 = tr->v[cross], e1 = tr->v[(cross + 1) % 3], mid;
            float dx = e1.x - e0.x, dz = e1.z - e0.z, l = sqrtf(dx * dx + dz * dz), nx, nz;

            mid = vec3((tr->v[0].x + tr->v[1].x + tr->v[2].x) / 3.0f, 0.0f, (tr->v[0].z + tr->v[1].z + tr->v[2].z) / 3.0f);
            nx = l > 0.0f ? dz / l : 0.0f;
            nz = l > 0.0f ? -dx / l : 0.0f;
            if (nx * (mid.x - e0.x) + nz * (mid.z - e0.z) < 0.0f) {   /* (inward) */
                nx = -nx;
                nz = -nz;
            }
            *out = vec3(c.x + nx * 0.01f, c.y, c.z + nz * 0.01f);
            *normal = vec3(nx, 0.0f, nz);
            *what = NAV_WALL;
            return t;
        }
        came = t;
        t = next;
    }
    return t;
}

int navmesh_crosses(const NavMesh *n, int from, Vec3 a, Vec3 b, uint32_t mask) {
    int t = from, came = -1, steps;

    if (t < 0 || t >= n->ntris) {
        return 0;
    }
    for (steps = 0; steps < n->ntris + 2; steps++) {
        const NavTri *tr = &n->tris[t];
        float best = -1.0f, w[3];
        int e, cross = -1;

        if ((tr->flags & mask) == mask) {
            return 1;
        }
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
        if (cross < 0 || tr->next[cross] < 0) {
            return 0;
        }
        came = t;
        t = tr->next[cross];
    }
    return 0;
}

/* ---- the pushed objects' squares (pairs of triangles of the pushable floor's grid) ---- */

int navmesh_square_step(const NavMesh *n, int *a, int *b, float dx, float dz) {
    int ta = *a, tb = *b, cur, came = -1, crossed = 0, steps, k;
    float len = sqrtf(dx * dx + dz * dz), w[3];
    Vec3 from = vec3(0, 0, 0), to;

    if (ta < 0 || tb < 0 || ta >= n->ntris || tb >= n->ntris || len <= 0.0f) {
        return -1;
    }
    for (k = 0; k < 3; k++) {
        from = vec3_add(from, vec3_add(n->tris[ta].v[k], n->tris[tb].v[k]));
    }
    from = vec3_scale(from, 1.0f / 6.0f);
    to = vec3(from.x + dx / len * 5.0f, from.y, from.z + dz / len * 5.0f);
    cur = inside(&n->tris[ta], from.x, from.z, w) ? ta : tb;
    for (steps = 0; steps < n->ntris + 2; steps++) {
        const NavTri *tr = &n->tris[cur];
        float best = -1.0f;
        int e, cross = -1;

        if (inside(tr, to.x, to.z, w)) {
            return -1;
        }
        for (e = 0; e < 3; e++) {   /* the edge it leaves by */
            float f = seg_cross(from, to, tr->v[e], tr->v[(e + 1) % 3]);

            if (f < 0.0f || f > 1.0f || (came >= 0 && tr->next[e] == came)) {
                continue;
            }
            if (cross < 0 || f > best) {
                best = f;
                cross = e;
            }
        }
        if (cross < 0 || tr->next[cross] < 0) {
            return -1;
        }
        came = cur;
        cur = tr->next[cross];
        if (cur != ta && cur != tb) {
            tb = ta;
            ta = cur;
            if (++crossed == 2) {
                *a = ta;
                *b = tb;
                return 0;
            }
        }
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

/* NavTri_ExitEdgeHit */
int navmesh_exit_edge_hit(const NavMesh *n, int ti, Vec3 from, Vec3 to, Vec3 *hit) {
    const NavTri *t;
    unsigned inside = 0, tested = 0;
    int e;

    if (ti < 0 || ti >= n->ntris) {
        return 4;
    }
    t = &n->tris[ti];
    for (e = 0; e < 3; e++) {
        Vec3 a = t->v[e], b = t->v[e < 2 ? e + 1 : 0];
        float ez = b.z - a.z, ex = b.x - a.x, p, q, dx, dz, den, s, u;

        tested |= 1u << e;
        if (!((to.x - a.x) * ez - (to.z - a.z) * ex < 0.0f)) {
            inside |= 1u << e;
            continue;
        }
        q = (from.z - a.z) * ex;
        p = (from.x - a.x) * ez;
        if (p - q < 0.0f) {
            continue;
        }
        dx = to.x - from.x;
        dz = to.z - from.z;
        den = dx * ez - dz * ex;
        if (den == 0.0f) {
            continue;
        }
        s = (q - p) / den;
        if (s < 0.0f || !(s <= 1.0f)) {
            continue;
        }
        u = fabsf(ex) <= fabsf(ez) ? (from.z + s * dz - a.z) / ez : (from.x + s * dx - a.x) / ex;
        if (u < 0.0f || !(u <= 1.0f)) {
            continue;
        }
        {   /* (the point on the line, its height on the triangle's plane) */
            Vec3 v0 = t->v[0], v1 = t->v[1], v2 = t->v[2], h = vec3_add(from, vec3_scale(vec3_sub(to, from), s));
            float e2x = v2.x - v0.x, e2y = v2.y - v0.y, e1y = v1.y - v0.y, e1x = v1.x - v0.x;
            float e1z = v1.z - v0.z, e2z = v2.z - v0.z, nx = e1y * e2z - e2y * e1z, nz = e1x * e2y - e2x * e1y;
            float ny = e1z * e2x - e2z * e1x;

            h.y = v0.y - ((h.z - v0.z) * nz + (h.x - v0.x) * nx) / ny;
            *hit = h;
        }
        return e;
    }
    return inside == tested ? 3 : 4;
}

/* PathPlan_StepCheapNear's open list: nodes by their cost so far, each knowing its place (open_raise,
 * open_merge - the goal node, here the start, a sentinel past the end) */
typedef struct NearNode {
    uint16_t flags;   /* 1 open, 2 closed, 4 the goal */
    int place;
    float cost;
} NearNode;

static void near_put(int *open, NearNode *node, int i, int t) {
    open[i] = t;
    node[t].place = i;
}

static void near_raise(int *open, NearNode *node, int t) {
    int pos = node[t].place;

    if (pos == 1) {
        return;
    }
    if (node[t].cost < node[open[1]].cost) {
        for (; pos >= 2; pos--) {
            near_put(open, node, pos, open[pos - 1]);
        }
    } else {
        while (node[t].cost < node[open[pos - 1]].cost) {
            near_put(open, node, pos, open[pos - 1]);
            pos--;
        }
    }
    near_put(open, node, pos, t);
}

static void near_merge(int *open, NearNode *node, int *nbs, int k, int n, int goal) {
    int i, j;

    if (k == 0) {
        for (i = 0; i < n - 1; i++) {
            near_put(open, node, i, open[i + 1]);
        }
        return;
    }
    for (i = 1; i < k; i++) {
        for (j = 0; j < k - i; j++) {
            if (!(node[nbs[j]].cost <= node[nbs[j + 1]].cost)) {
                int x = nbs[j];

                nbs[j] = nbs[j + 1];
                nbs[j + 1] = x;
            }
        }
    }
    if (n == 1) {
        for (i = 0; i < k; i++) {
            near_put(open, node, i, nbs[i]);
        }
        return;
    }
    {
        int at = n - 1;

        for (i = k - 1; i > 0; i--) {
            int x = nbs[i];

            if (node[x].cost < node[open[1]].cost) {
                for (; at > 0; at--) {
                    near_put(open, node, at + i, open[at]);
                }
            } else {
                while (node[x].cost < node[open[at]].cost) {
                    near_put(open, node, at + i, open[at]);
                    at--;
                }
            }
            near_put(open, node, at + i, x);
        }
        open[n + k - 1] = goal;
        node[goal].cost = 1.0f + node[nbs[0]].cost;
        for (i = 0; !(node[nbs[0]].cost <= node[open[i + 1]].cost); i++) {
            near_put(open, node, i, open[i + 1]);
        }
        near_put(open, node, i, nbs[0]);
    }
}

int navmesh_nearest_free(const NavMesh *n, int start, uint32_t mask) {
    NearNode *node;
    int *open, nopen = 1, nearest = -1;
    float nearest_d = 0.0f;
    Vec3 from;

    if (start < 0 || start >= n->ntris) {
        return -1;
    }
    if (!(n->tris[start].flags & mask)) {
        return start;
    }
    node = calloc((size_t)n->ntris, sizeof(*node));
    open = calloc((size_t)n->ntris + 2, sizeof(*open));
    if (node == NULL || open == NULL) {
        free(node);
        free(open);
        return -1;
    }
    from = navmesh_center(n, start);
    node[start].flags = 1 | 4;
    near_put(open, node, 0, start);
    while (nopen > 0) {
        int cur = open[0], k = 0, e, nbs[3];
        const NavTri *tri = &n->tris[cur];

        for (e = 0; e < 3; e++) {
            int t = tri->next[e];

            if (t < 0 || t >= n->ntris) {
                continue;
            }
            if (node[t].flags & 3) {   /* (seen: reached cheaper, moved up the list) */
                float g = node[cur].cost + n->tris[t].cost[e];

                if ((node[t].flags & 1) && g < node[t].cost) {
                    node[t].cost = g;
                    near_raise(open, node, t);
                }
                continue;
            }
            if (!(n->tris[t].flags & mask)) {   /* (free: the nearest so far; further than 30 - over) */
                Vec3 c = navmesh_center(n, t);
                float dx = c.x - from.x, dz = c.z - from.z, d = sqrtf(dz * dz + dx * dx);

                if (nearest < 0 || d < nearest_d) {
                    nearest = t;
                    nearest_d = d;
                }
                if (!(d <= 30.0f)) {
                    free(node);
                    free(open);
                    return nearest;
                }
            }
            nbs[k++] = t;
            node[t].flags |= 1;
            node[t].cost = node[cur].cost + n->tris[t].cost[e];
        }
        node[cur].flags ^= 3;
        near_merge(open, node, nbs, k, nopen, start);
        nopen = nopen + k - 1;
    }
    free(node);
    free(open);
    return nearest;
}

int navmesh_free_toward(const NavMesh *n, int t, Vec3 to, uint32_t mask, Vec3 *out) {
    Vec3 c = to, e = to, d;
    float l;

    for (;;) {
        int r, next;

        if (t < 0 || t >= n->ntris) {
            return -1;
        }
        c = navmesh_center(n, t);
        r = navmesh_exit_edge_hit(n, t, c, to, &e);
        if (r == 4) {
            return -1;
        }
        if (r == 3) {
            e = to;
            break;
        }
        next = n->tris[t].next[r];
        if (next >= 0 && next < n->ntris && (n->tris[next].flags & mask)) {
            break;
        }
        t = next;
    }
    d = vec3_sub(c, to);   /* (Heading_Vector of its heading: on the level) */
    d.y = 0.0f;
    l = sqrtf(d.x * d.x + d.z * d.z);
    if (l > 0.0f) {
        e = vec3_add(e, vec3_scale(d, 0.1f / l));
    } else {   /* (Vec_Heading of none: 10) */
        e = vec3_add(e, vec3(0.1f * sinf(10.0f), 0.0f, 0.1f * cosf(10.0f)));
    }
    *out = e;
    return t;
}
