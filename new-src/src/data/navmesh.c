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

        if (!inside(t, p.x, p.z, w)) {
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
    int i;

    for (i = 0; i < 3; i++) {
        float len = sqrtf(tries[i][0] * tries[i][0] + tries[i][1] * tries[i][1]), h, h2;
        Vec3 q = vec3(p.x + tries[i][0], p.y, p.z + tries[i][1]), ahead;

        if (len == 0.0f) {
            continue;
        }
        ahead = vec3(q.x + tries[i][0] / len * radius, p.y, q.z + tries[i][1] / len * radius);
        if (navmesh_find(n, q, climb, &h) >= 0 && navmesh_find(n, ahead, climb, &h2) >= 0) {
            q.y = h;
            return q;
        }
    }
    return p;
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
