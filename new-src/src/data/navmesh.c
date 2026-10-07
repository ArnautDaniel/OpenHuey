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
