/* The room's placed objects: see placed.h. */
#include "placed.h"

#include <stdlib.h>
#include <string.h>

static int32_t rd32(const uint8_t *p) {
    int32_t x;

    memcpy(&x, p, 4);
    return x;
}

static float rdf(const uint8_t *p) {
    float x;

    memcpy(&x, p, 4);
    return x;
}

static int16_t rd16(const uint8_t *p) { return (int16_t)(p[0] | p[1] << 8); }

/* a strip of n vertices into triangles (flags[i]: no triangle ends at i) */
static void strip_to_mesh(RoomMesh *m, const MeshVertex *v, const uint8_t *no_tri, int n, int texture, int blend,
                          int glow) {
    MeshDraw *d;
    int i, k;

    m->v = malloc((size_t)(n > 2 ? n - 2 : 1) * 3 * sizeof(MeshVertex));
    m->d = calloc(1, sizeof(MeshDraw));
    if (m->v == NULL || m->d == NULL) {
        return;
    }
    for (i = 2; i < n; i++) {
        if (no_tri[i]) {
            continue;
        }
        for (k = i - 2; k <= i; k++) {
            m->v[m->nv++] = v[k];
        }
    }
    d = &m->d[0];
    d->count = m->nv;
    d->texture = texture;
    d->part = glow ? MESH_GLOW : blend ? MESH_SEE_THROUGH : MESH_SOLID;
    d->blend = (uint8_t)(blend || glow);
    d->no_zwrite = (uint8_t)glow;
    m->nd = m->nv > 0;
}

/* a morph's n vertices, frames a and b blended by t (gl_morph: the frame table at the model's
 * +0x70, 8 bytes a frame - +0x4 its s16 positions / 4096, +0x0 its RGBA colours; the lit kind
 * (1) always 0x80); 0 if they fall outside the section */
static int morph_fill(Placed *p, const uint8_t *def, const uint8_t *end, MeshVertex *v, int n, int a, int b, float t) {
    const uint8_t *pa = def + rd32(def + 0x74 + a * 8), *pb = def + rd32(def + 0x74 + b * 8);
    const uint8_t *ca = def + rd32(def + 0x70 + a * 8), *cb = def + rd32(def + 0x70 + b * 8);
    const uint8_t *uv = def + rd32(def + 0x4C);
    float w = 1.0f - t;
    int i, c;

    if (a < 0 || b < 0 || a > 255 || b > 255 || def + 0x78 + (a > b ? a : b) * 8 > end) {
        return 0;
    }
    if (pa + n * 6 > end || pb + n * 6 > end || uv + n * 4 > end ||
        (p->kind == 3 && (ca + n * 4 > end || cb + n * 4 > end))) {
        return 0;
    }
    for (i = 0; i < n; i++) {
        v[i].x = rd16(pa + i * 6) / 4096.0f * w + rd16(pb + i * 6) / 4096.0f * t;
        v[i].y = rd16(pa + i * 6 + 2) / 4096.0f * w + rd16(pb + i * 6 + 2) / 4096.0f * t;
        v[i].z = rd16(pa + i * 6 + 4) / 4096.0f * w + rd16(pb + i * 6 + 4) / 4096.0f * t;
        v[i].s = (uint16_t)rd16(uv + i * 4) / 32768.0f;
        v[i].t = (uint16_t)rd16(uv + i * 4 + 2) / 32768.0f;
        for (c = 0; c < 4; c++) {
            v[i].rgba[c] = p->kind == 3 ? (uint8_t)(int)((float)ca[i * 4 + c] * w + (float)cb[i * 4 + c] * t)
                                        : (uint8_t)(c < 3 ? 0x80 : 0x7F);
        }
    }
    return 1;
}

/* a morph shown again by its frames and blend: the strip's vertices refilled, the triangles redone */
static void morph_update(Placed *p, int a, int b, float t) {
    int n = rd32(p->def + 0x40), i, k, j = 0;
    const uint8_t *strip = p->def + rd32(p->def + 0x50);
    MeshVertex *v;

    p->morph_a = a;
    p->morph_b = b;
    p->morph_t = t;
    if (n <= 0 || n > 0x10000 || p->mesh.v == NULL || strip + n > p->def_end) {
        return;
    }
    v = malloc((size_t)n * sizeof(MeshVertex));
    if (v == NULL) {
        return;
    }
    if (morph_fill(p, p->def, p->def_end, v, n, a, b, t)) {
        for (i = 2; i < n; i++) {   /* (as strip_to_mesh) */
            if (strip[i] & 1) {
                continue;
            }
            for (k = i - 2; k <= i && j < p->mesh.nv; k++) {
                p->mesh.v[j++] = v[k];
            }
        }
        render_mesh_update(&p->gpu, p->mesh.v, p->mesh.nv);
    }
    p->morph_a = a;   /* (tried: not again till they change) */
    p->morph_b = b;
    p->morph_t = t;
    free(v);
}

/* the model of definition `def` (end: the section's end) as triangles in model space */
static void build_model(Placed *p, const uint8_t *def, const uint8_t *end) {
    int n = rd32(def + 0x40), tex = rd32(def + 0x44), i;
    int blend = (p->name[0] == 'a' || p->name[0] == 'A') && p->name[1] == '_';
    int glow = (p->name[0] == 'g' || p->name[0] == 'G') && p->name[1] == '_';
    MeshVertex *v;
    uint8_t *no_tri;

    if (n <= 0 || n > 0x10000) {
        return;
    }
    v = calloc((size_t)n, sizeof(MeshVertex));
    no_tri = calloc((size_t)n, 1);
    if (v == NULL || no_tri == NULL) {
        free(v);
        free(no_tri);
        return;
    }
    p->scale = 1.0f;
    switch (p->kind) {
    case 2: {   /* the room's batch layout */
        const uint8_t *st = def + rd32(def + 0x4C), *rgba = def + rd32(def + 0x50), *xyz = def + rd32(def + 0x54);

        if (xyz + n * 16 > end || st + n * 8 > end || rgba + n * 4 > end) {
            break;
        }
        for (i = 0; i < n; i++) {
            v[i].x = rdf(xyz + i * 16);
            v[i].y = rdf(xyz + i * 16 + 4);
            v[i].z = rdf(xyz + i * 16 + 8);
            no_tri[i] = (rd32(xyz + i * 16 + 12) & 0x8000) != 0;
            v[i].s = rdf(st + i * 8);
            v[i].t = rdf(st + i * 8 + 4);
            memcpy(v[i].rgba, rgba + i * 4, 4);
        }
        break;
    }
    case 0: {   /* rigid: s16 steps from the start */
        const uint8_t *d = def + rd32(def + 0x54), *uv = def + rd32(def + 0x4C), *strip = def + rd32(def + 0x58);
        int32_t x = rd32(def + 0x60), y = rd32(def + 0x64), z = rd32(def + 0x68);

        if (d + n * 6 > end || uv + n * 4 > end || strip + n > end) {
            break;
        }
        for (i = 0; i < n; i++) {
            x += rd16(d + i * 6);
            y += rd16(d + i * 6 + 2);
            z += rd16(d + i * 6 + 4);
            v[i].x = x / 4096.0f;
            v[i].y = y / 4096.0f;
            v[i].z = z / 4096.0f;
            no_tri[i] = strip[i] & 1;
            v[i].s = (uint16_t)rd16(uv + i * 4) / 32768.0f;
            v[i].t = (uint16_t)rd16(uv + i * 4 + 2) / 32768.0f;
            v[i].rgba[0] = v[i].rgba[1] = v[i].rgba[2] = v[i].rgba[3] = 0x80;   /* (lit on VU1 in the original) */
        }
        p->scale = 32.0f;
        break;
    }
    default: {   /* 1 / 3: a morph, its first frame (others: placed_tick, by its frames and blend) */
        const uint8_t *strip = def + rd32(def + 0x50);

        if (strip + n > end || !morph_fill(p, def, end, v, n, 0, 0, 0.0f)) {
            break;
        }
        for (i = 0; i < n; i++) {
            no_tri[i] = strip[i] & 1;
        }
        p->scale = 32.0f;
        break;
    }
    }
    strip_to_mesh(&p->mesh, v, no_tri, n, tex, blend, glow);
    if (p->mesh.nv > 0 && (p->kind == 1 || p->kind == 3)) {
        render_mesh_update(&p->gpu, p->mesh.v, p->mesh.nv);   /* (a morph: refilled as its frames change) */
    } else if (p->mesh.nv > 0) {
        render_mesh_upload(&p->gpu, p->mesh.v, p->mesh.nv);
    }
    free(v);
    free(no_tri);
}

void placed_load(PlacedSet *s, const uint8_t *defs, size_t defs_size, const uint8_t *anims, size_t anims_size) {
    int counts[4], total, i, k;

    placed_free(s);
    s->anims = anims;
    s->anims_size = anims != NULL ? anims_size : 0;
    if (defs == NULL || defs_size < 0x20) {
        return;
    }
    for (k = 0; k < 4; k++) {
        counts[k] = rd32(defs + k * 4);
    }
    total = counts[0] + counts[1] + counts[2] + counts[3];
    for (i = 0; i < total && s->n < PLACED_MAX && 0x20 + (size_t)(i + 1) * 4 <= defs_size; i++) {
        uint32_t o = (uint32_t)rd32(defs + 0x20 + i * 4);
        const uint8_t *def = defs + o;
        Placed *p = &s->p[s->n];

        if (o + 0x70 > defs_size) {
            continue;
        }
        memset(p, 0, sizeof(*p));
        p->shown = 1;   /* (drawn until a script hides it: the original's +0 starts 0) */
        memcpy(p->name, def, 15);
        p->kind = i < counts[0] ? 0 : i < counts[0] + counts[1] ? 1 : i < counts[0] + counts[1] + counts[2] ? 2 : 3;
        p->def_rot = p->rot = vec3(rdf(def + 0x10), rdf(def + 0x14), rdf(def + 0x18));
        p->def_pos = p->pos = vec3(rdf(def + 0x20), rdf(def + 0x24), rdf(def + 0x28));
        p->def = def;
        p->def_end = defs + defs_size;
        build_model(p, def, defs + defs_size);
        s->n++;
    }
}

void placed_free(PlacedSet *s) {
    int i;

    for (i = 0; i < s->n; i++) {
        render_mesh_free(&s->p[i].gpu);
        roommesh_free(&s->p[i].mesh);
    }
    memset(s, 0, sizeof(*s));
}

uint32_t *placed_word(Placed *p, int off) {
    if (off < 0 || off >= 0x80 || (off & 3)) {
        return NULL;
    }
    if (off >= 0x10 && off < 0x1C) {
        return (uint32_t *)&(&p->rot.x)[(off - 0x10) / 4];
    }
    if (off >= 0x20 && off < 0x2C) {
        return (uint32_t *)&(&p->pos.x)[(off - 0x20) / 4];
    }
    return &p->words[off / 4];
}

Placed *placed_named(PlacedSet *s, const char *name) {
    int i;

    for (i = 0; i < s->n; i++) {
        if (strncmp(s->p[i].name, name, sizeof(s->p[i].name)) == 0) {
            return &s->p[i];
        }
    }
    return NULL;
}

void placed_anim(PlacedSet *s, Placed *p, int id, int loop) {
    uint32_t n, j;

    p->loop = loop;
    p->keys = NULL;
    p->frames = 0;
    p->frame = 0;
    if (s->anims == NULL || s->anims_size < 4) {
        return;
    }
    n = (uint32_t)rd32(s->anims);
    for (j = 0; j < n && 4 + (size_t)(j + 1) * 4 <= s->anims_size; j++) {   /* PlacedObjects_RecordEntry */
        uint32_t o = (uint32_t)rd32(s->anims + 4 + j * 4);
        const uint8_t *e = s->anims + o, *a;

        if (o + 0x18 > s->anims_size || strncmp((const char *)e, p->name, 16) != 0) {
            continue;
        }
        if (id < 0 || (uint32_t)id >= (uint32_t)rd32(e + 0x10)) {
            return;
        }
        a = e + rd32(e + 0x14) + id * 0x10;
        if (a + 0xC > s->anims + s->anims_size) {
            return;
        }
        p->frames = rd32(a);
        p->format = rd32(a + 4) & 0xFFFF;
        p->keys = a + rd32(a + 8);
        if (p->keys + (size_t)p->frames * 12 > s->anims + s->anims_size) {
            p->keys = NULL;
        }
        return;
    }
}

void placed_tick(PlacedSet *s) {
    int i;

    for (i = 0; i < s->n; i++) {
        Placed *p = &s->p[i];
        Vec3 v;

        if (p->kind == 1 || p->kind == 3) {   /* (a morph: the frames the rooms' commands set) */
            int a = (int)p->words[0x74 / 4], b = (int)p->words[0x78 / 4];
            float t;

            memcpy(&t, &p->words[0x7C / 4], 4);
            if (a != p->morph_a || b != p->morph_b || t != p->morph_t) {
                morph_update(p, a, b, t);
            }
        }

        if (p->keys == NULL || p->frames <= 0) {
            continue;
        }
        if (p->format == 8 || p->format == 9) {   /* (Track_Sample: xyz floats, a key a frame) */
            const uint8_t *k = p->keys + (p->frame < p->frames ? p->frame : p->frames - 1) * 12;

            v = vec3(rdf(k), rdf(k + 4), rdf(k + 8));
            if (p->format == 9) {
                p->pos = v;
            } else {
                p->rot = v;
            }
        }
        if (++p->frame >= p->frames) {
            if (p->loop) {
                p->frame = 0;
            } else {
                p->keys = NULL;
                p->frames = 0;
            }
        }
    }
}

/* scaled, turned about X, then Y, then Z, then placed (obj_mvp) */
static Mat4 placed_matrix(const Placed *p) {
    float cx = cosf(p->rot.x), sx = sinf(p->rot.x), cy = cosf(p->rot.y), sy = sinf(p->rot.y);
    float cz = cosf(p->rot.z), sz = sinf(p->rot.z);
    Mat4 rx = mat4_identity(), ry = mat4_identity(), rz = mat4_identity(), t = mat4_identity(), sc = mat4_identity();

    rx.m[5] = cx;  rx.m[6] = sx;  rx.m[9] = -sx;  rx.m[10] = cx;
    ry.m[0] = cy;  ry.m[2] = -sy; ry.m[8] = sy;   ry.m[10] = cy;
    rz.m[0] = cz;  rz.m[1] = sz;  rz.m[4] = -sz;  rz.m[5] = cz;
    sc.m[0] = sc.m[5] = sc.m[10] = p->scale;
    t.m[12] = p->pos.x;
    t.m[13] = p->pos.y;
    t.m[14] = p->pos.z;
    return mat4_mul(t, mat4_mul(rz, mat4_mul(ry, mat4_mul(rx, sc))));
}

void placed_draw(PlacedSet *s, const Mat4 *view_proj, const GpuTexture *textures, int ntextures) {
    static const uint32_t kAll[8] = {~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u};
    int i;

    for (i = 0; i < s->n; i++) {
        Placed *p = &s->p[i];

        if (p->shown && p->mesh.nd > 0) {
            Mat4 mvp = mat4_mul(*view_proj, placed_matrix(p));

            render_mesh(&p->gpu, &mvp, p->mesh.d, p->mesh.nd, textures, ntextures, kAll);
        }
    }
}
