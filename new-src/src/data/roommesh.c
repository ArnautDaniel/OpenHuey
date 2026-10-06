#include "roommesh.h"

#include <stdlib.h>
#include <string.h>

typedef struct Builder {
    RoomMesh *m;
    int capv, capd;
} Builder;

static void add_vertex(Builder *b, const MeshVertex *v) {
    RoomMesh *m = b->m;

    if (m->nv == b->capv) {
        b->capv = b->capv * 2 + 4096;
        m->v = realloc(m->v, (size_t)b->capv * sizeof(MeshVertex));
    }
    m->v[m->nv++] = *v;
}

static MeshDraw *add_draw(Builder *b) {
    RoomMesh *m = b->m;

    if (m->nd == b->capd) {
        b->capd = b->capd * 2 + 256;
        m->d = realloc(m->d, (size_t)b->capd * sizeof(MeshDraw));
    }
    memset(&m->d[m->nd], 0, sizeof(MeshDraw));
    return &m->d[m->nd++];
}

static int32_t i32(const uint8_t *p) {
    int32_t x;

    memcpy(&x, p, 4);
    return x;
}

static float f32(const uint8_t *p) {
    float x;

    memcpy(&x, p, 4);
    return x;
}

/* one batch at p (inside [sec, end)); returns the next batch, or NULL if malformed */
static const uint8_t *batch(Builder *b, int part, const uint8_t *p, const uint8_t *end) {
    int32_t n = i32(p), texture = i32(p + 4), extra = i32(p + 12);
    uint32_t flags = (uint32_t)i32(p + 8);
    const uint8_t *st, *rgba, *xyz, *next;
    static const int kPadRgba[4] = {0, 8, 0, 8}, kPadXyz[4] = {0, 12, 8, 4};
    Mat4 local;
    int i, c;

    if (n < 0 || n > 0x10000) {
        return NULL;
    }
    for (c = 0; c < 4; c++) {   /* row-major in the file */
        for (i = 0; i < 4; i++) {
            local.m[c * 4 + i] = f32(p + 16 + (i * 4 + c) * 4);
        }
    }
    st = p + 16 + 64;
    rgba = st + n * 8 + kPadRgba[n & 3];
    xyz = rgba + n * 4 + kPadXyz[n & 3];
    next = xyz + n * 16;
    if (next > end) {
        return NULL;
    }
    if (part == MESH_ANIMATED || part == MESH_BLOOM_MASK) {   /* not drawn (yet) */
        return next;
    }
    {
        MeshDraw *d = add_draw(b);

        d->first = b->m->nv;
        d->texture = texture;
        d->part = (uint8_t)part;
        d->blend = flags & 1;
        d->additive = part == MESH_GLOW && (extra >> 8 & 1);
        d->no_zwrite = part == MESH_GLOW;
        d->group = (uint8_t)(flags >> 24);
        for (i = 2; i < n; i++) {
            int k;

            if ((uint32_t)i32(xyz + i * 16 + 12) & 0x8000) {   /* a strip break */
                continue;
            }
            for (k = i - 2; k <= i; k++) {
                MeshVertex v = {0};
                Vec3 pos = mat4_point(&local, vec3(f32(xyz + k * 16), f32(xyz + k * 16 + 4), f32(xyz + k * 16 + 8)));

                v.x = pos.x;
                v.y = pos.y;
                v.z = pos.z;
                v.s = f32(st + k * 8);
                v.t = f32(st + k * 8 + 4);
                memcpy(v.rgba, rgba + k * 4, 4);
                add_vertex(b, &v);
                if (part == MESH_SOLID) {
                    b->m->lo = vec3_min(b->m->lo, pos);
                    b->m->hi = vec3_max(b->m->hi, pos);
                }
            }
        }
        d->count = b->m->nv - d->first;
        if (d->count == 0) {
            b->m->nd--;
        }
    }
    return next;
}

int roommesh_build(RoomMesh *m, const uint8_t *sec, size_t size) {
    Builder b;
    const uint8_t *end = sec + size;
    int part;

    memset(m, 0, sizeof(*m));
    memset(&b, 0, sizeof(b));
    b.m = m;
    m->lo = vec3(1e30f, 1e30f, 1e30f);
    m->hi = vec3(-1e30f, -1e30f, -1e30f);
    if (sec == NULL || size < MESH_PARTS * 4) {
        return 0;
    }
    for (part = 0; part < MESH_PARTS; part++) {
        int32_t off = i32(sec + part * 4);
        const uint8_t *p;

        if (off <= 0 || (size_t)off >= size) {
            continue;
        }
        for (p = sec + off; p + 16 <= end && i32(p) != -1;) {
            p = batch(&b, part, p, end);
            if (p == NULL) {
                roommesh_free(m);
                return 0;
            }
        }
    }
    return 1;
}

void roommesh_free(RoomMesh *m) {
    free(m->v);
    free(m->d);
    memset(m, 0, sizeof(*m));
}
