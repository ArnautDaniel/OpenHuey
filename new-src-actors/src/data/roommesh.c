#include "roommesh.h"

#include <stdlib.h>
#include <string.h>

typedef struct Builder {
    RoomMesh *m;
    int capv, capd, capdv, capdyn;
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

/* a batch that moves with the view or animates: its strip as it is, and how it moves */
static void dynamic(Builder *b, int part, const Mat4 *local, int n, uint32_t flags, int32_t extra, int texture,
                    const uint8_t *st, const uint8_t *rgba, const uint8_t *xyz) {
    RoomMesh *m = b->m;
    DynBatch *d;
    int k;

    if (m->ndyn == b->capdyn) {
        b->capdyn = b->capdyn * 2 + 32;
        m->dyn = realloc(m->dyn, (size_t)b->capdyn * sizeof(DynBatch));
    }
    if (m->ndv + n > b->capdv) {
        b->capdv = (m->ndv + n) * 2 + 1024;
        m->dv = realloc(m->dv, (size_t)b->capdv * sizeof(MeshVertex));
    }
    d = &m->dyn[m->ndyn++];
    memset(d, 0, sizeof(*d));
    d->local = *local;
    d->draw.first = m->ndv;
    d->draw.count = n;
    d->draw.texture = texture;
    d->draw.part = (uint8_t)part;
    d->draw.blend = flags & 1;
    d->draw.additive = part == MESH_GLOW && (extra >> 8 & 1);
    d->draw.no_zwrite = part == MESH_GLOW;
    d->draw.group = (uint8_t)(flags >> 24);
    d->draw.mask = part == MESH_BLOOM_MASK;
    if (flags >> 8 & 0xFF) {
        d->parallax = (uint8_t)(1 << (flags >> 8 & 0xF));
        d->billboard = (uint8_t)(1 << (flags >> 12 & 0xF));
    }
    d->no_tri = malloc((size_t)n);
    for (k = 0; k < n; k++) {
        MeshVertex *v = &m->dv[m->ndv++];

        memset(v, 0, sizeof(*v));
        v->x = f32(xyz + k * 16);
        v->y = f32(xyz + k * 16 + 4);
        v->z = f32(xyz + k * 16 + 8);
        v->s = f32(st + k * 8);
        v->t = f32(st + k * 8 + 4);
        memcpy(v->rgba, rgba + k * 4, 4);
        d->no_tri[k] = ((uint32_t)i32(xyz + k * 16 + 12) & 0x8000) != 0;
    }
    if (part == MESH_ANIMATED && n >= 4) {   /* a flip book: its frame is the quad's texture size */
        float lo_u = 1.0f, hi_u = 0.0f, lo_v = 1.0f, hi_v = 0.0f;

        for (k = 0; k < 4; k++) {
            lo_u = fminf(lo_u, f32(st + k * 8));
            hi_u = fmaxf(hi_u, f32(st + k * 8));
            lo_v = fminf(lo_v, f32(st + k * 8 + 4));
            hi_v = fmaxf(hi_v, f32(st + k * 8 + 4));
        }
        d->frames = (int)(flags >> 16 & 0xFF);
        d->frame_ticks = extra & 0xF;
        d->flip_type = (uint8_t)(extra >> 4 & 0xF);
        d->timer = d->frame_ticks;
        d->frame_w = (float)(uint8_t)(uint32_t)(hi_u * 256.0f - lo_u * 256.0f) / 256.0f;
        d->frame_h = (float)(uint8_t)(uint32_t)(hi_v * 256.0f - lo_v * 256.0f) / 256.0f;
        if (d->flip_type >= 4 && d->flip_type <= 6) {   /* glowing: added, no depth written */
            d->draw.blend = 1;
            d->draw.additive = 1;
            d->draw.no_zwrite = 1;
        }
    }
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
    if (part == MESH_ANIMATED || (flags >> 8 & 0xFF) != 0) {   /* it moves: kept apart */
        dynamic(b, part, &local, n, flags, extra, texture, st, rgba, xyz);
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
        d->mask = part == MESH_BLOOM_MASK;
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

int roommesh_build_list(RoomMesh *m, const uint8_t *p, const uint8_t *end, int part) {
    Builder b;

    memset(m, 0, sizeof(*m));
    memset(&b, 0, sizeof(b));
    b.m = m;
    m->lo = vec3(1e30f, 1e30f, 1e30f);
    m->hi = vec3(-1e30f, -1e30f, -1e30f);
    while (p != NULL && p + 16 <= end && i32(p) != -1) {
        p = batch(&b, part, p, end);
    }
    if (p == NULL) {
        roommesh_free(m);
        return 0;
    }
    return 1;
}

void roommesh_free(RoomMesh *m) {
    int i;

    for (i = 0; i < m->ndyn; i++) {
        free(m->dyn[i].no_tri);
    }
    free(m->dyn);
    free(m->dv);
    free(m->v);
    free(m->d);
    memset(m, 0, sizeof(*m));
}

/* ---- the moving batches ---- */

void roommesh_tick(RoomMesh *m) {
    int i;

    for (i = 0; i < m->ndyn; i++) {
        DynBatch *d = &m->dyn[i];

        if (d->frames == 0) {
            continue;
        }
        d->timer = (d->timer - 1) & 0xFF;   /* (a byte, as the game keeps it) */
        if (d->timer != 0) {
            continue;
        }
        d->frame++;
        if (d->frame >= d->frames) {
            d->frame = d->flip_type == 2 ? d->frames - 1 : 0;
        }
        d->timer = d->frame_ticks;
    }
}

static Mat4 basis(Vec3 x, Vec3 y, Vec3 z, Vec3 t) {
    Mat4 r = mat4_identity();

    r.m[0] = x.x; r.m[1] = x.y; r.m[2] = x.z;
    r.m[4] = y.x; r.m[5] = y.y; r.m[6] = y.z;
    r.m[8] = z.x; r.m[9] = z.y; r.m[10] = z.z;
    r.m[12] = t.x; r.m[13] = t.y; r.m[14] = t.z;
    return r;
}

/* where a moving batch is this frame */
static Mat4 placement(const DynBatch *d, Vec3 eye, Vec3 forward) {
    Mat4 m = d->local;
    Vec3 t = vec3(m.m[12], m.m[13], m.m[14]), up = vec3(0, 1, 0);

    if (d->frames > 0 && (d->flip_type == 1 || d->flip_type == 3 || d->flip_type == 4 || d->flip_type == 6)) {
        /* a flip book turned to the eye about its own place (RoomMesh_Billboard) */
        Vec3 z = forward, x, y;
        Mat4 turn;

        if (d->flip_type == 1 || d->flip_type == 6) {
            z = vec3_norm(vec3(z.x, 0.0f, z.z));
        }
        x = vec3_norm(vec3_cross(z, up));
        y = vec3_norm(vec3_cross(x, z));
        turn = basis(x, y, z, vec3(0, 0, 0));
        m.m[12] = m.m[13] = m.m[14] = 0.0f;
        m = mat4_mul(turn, m);
        m.m[12] = t.x;
        m.m[13] = t.y;
        m.m[14] = t.z;
        return m;
    }
    if (d->parallax > 1) {   /* shifted sideways by a part of the eye's position (RoomMesh_PlaceBatch) */
        float k = d->parallax == 2 ? 0.2f : d->parallax == 4 ? 0.25f : d->parallax == 8 ? 0.33f : 0.5f;
        Vec3 side = vec3_norm(vec3_cross(forward, vec3(0, -1, 0)));

        m.m[12] += k * eye.x * side.x;
        m.m[14] += k * eye.z * side.z;
        t = vec3(m.m[12], m.m[13], m.m[14]);
    }
    if (d->billboard == 2 || d->billboard == 4) {   /* facing the eye (mesh_face_eye) */
        Vec3 z = vec3_sub(eye, t), x, y;

        if (d->billboard == 2) {
            z.y = 0.0f;
        }
        z = vec3_norm(z);
        x = vec3_norm(d->billboard == 2 ? vec3_cross(up, z) : vec3_cross(z, vec3(0, -1, 0)));
        y = vec3_norm(vec3_cross(z, x));
        m = basis(x, y, z, t);
    }
    return m;
}

int roommesh_dynamic(const RoomMesh *m, Vec3 eye, Vec3 forward, MeshVertex *out, MeshDraw *draws) {
    int i, nout = 0;

    for (i = 0; i < m->ndyn; i++) {
        const DynBatch *d = &m->dyn[i];
        const MeshVertex *src = &m->dv[d->draw.first];
        Mat4 place = placement(d, eye, forward);
        float du = 0.0f, dv = 0.0f;
        MeshDraw *draw = &draws[i];
        int k, j;

        if (d->frames > 0 && d->draw.count >= 4) {   /* the frame's texture coordinates (RoomMesh_FlipBook) */
            float u[4], hi, lo;
            int whole = 0, past = 0;

            for (k = 0; k < 4; k++) {
                u[k] = src[k].s + (float)d->frame * d->frame_w;
                past |= u[k] > 1.0f;
            }
            du = (float)d->frame * d->frame_w;
            if (past) {
                hi = fmaxf(fmaxf(u[0], u[1]), fmaxf(u[2], u[3]));
                whole = (int)(hi - 1.0f / 16.0f);
                du -= (float)whole;
                lo = fminf(fminf(u[0], u[1]), fminf(u[2], u[3])) - (float)whole;
                if (lo < 0.0f) {
                    du -= lo;
                }
                dv = (float)whole * d->frame_h;
            }
        }
        *draw = d->draw;
        draw->first = nout;
        for (k = 2; k < d->draw.count; k++) {
            if (d->no_tri[k]) {
                continue;
            }
            for (j = k - 2; j <= k; j++) {
                MeshVertex v = src[j];
                Vec3 p = mat4_point(&place, vec3(v.x, v.y, v.z));

                v.x = p.x;
                v.y = p.y;
                v.z = p.z;
                v.s += du;
                v.t += dv;
                out[nout++] = v;
            }
        }
        draw->count = nout - draw->first;
    }
    return m->ndyn;
}
