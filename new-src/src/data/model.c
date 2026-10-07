#include "model.h"

#include <stdlib.h>
#include <string.h>

#define POS_SCALE (1.0f / 4096.0f)
#define UNIT (1.0f / 32768.0f)
#define ANGLE (6.28318530718f / 65536.0f)
#define TRANS (1.0f / 256.0f)

/* ---- reading, with every offset checked against the file ---- */

typedef struct Reader {
    const uint8_t *base;
    size_t size;
    int bad;
} Reader;

static const uint8_t *at(Reader *r, size_t off, size_t n) {
    if (off > r->size || n > r->size - off) {
        r->bad = 1;
        return NULL;
    }
    return r->base + off;
}

static uint32_t u32(Reader *r, size_t off) {
    const uint8_t *p = at(r, off, 4);
    uint32_t x = 0;

    if (p != NULL) {
        memcpy(&x, p, 4);
    }
    return x;
}

static int16_t s16(Reader *r, size_t off) {
    const uint8_t *p = at(r, off, 2);
    int16_t x = 0;

    if (p != NULL) {
        memcpy(&x, p, 2);
    }
    return x;
}

static uint16_t u16(Reader *r, size_t off) {
    return (uint16_t)s16(r, off);
}

static uint8_t u8(Reader *r, size_t off) {
    const uint8_t *p = at(r, off, 1);

    return p != NULL ? *p : 0;
}

static float f32(Reader *r, size_t off) {
    const uint8_t *p = at(r, off, 4);
    float x = 0.0f;

    if (p != NULL) {
        memcpy(&x, p, 4);
    }
    return x;
}

/* ---- building ---- */

typedef struct Builder {
    Model *m;
    int capv, capd;
    SkinVertex *strip;   /* one part's vertices, in strip order */
    uint8_t *no_tri;     /* per strip vertex: no triangle ends here */
    int nstrip, capstrip;
} Builder;

static void strip_reset(Builder *b, int n) {
    if (n > b->capstrip) {
        b->capstrip = n;
        b->strip = realloc(b->strip, (size_t)n * sizeof(SkinVertex));
        b->no_tri = realloc(b->no_tri, (size_t)n);
    }
    b->nstrip = n;
    memset(b->strip, 0, (size_t)n * sizeof(SkinVertex));
}

/* the strip as a triangle list, as one draw */
static void strip_flush(Builder *b, int texture, int second_pass) {
    Model *m = b->m;
    MeshDraw *d;
    int k;

    if (m->nd == b->capd) {
        b->capd = b->capd * 2 + 64;
        m->d = realloc(m->d, (size_t)b->capd * sizeof(MeshDraw));
    }
    d = &m->d[m->nd++];
    memset(d, 0, sizeof(*d));
    d->first = m->nv;
    d->texture = texture;
    d->blend = (uint8_t)second_pass;      /* cut-outs: fur, lashes, torn cloth */
    d->solid_tex = (uint8_t)!second_pass;
    d->lit = 1;
    for (k = 2; k < b->nstrip; k++) {
        int j;

        if (b->no_tri[k]) {
            continue;
        }
        for (j = k - 2; j <= k; j++) {
            if (m->nv == b->capv) {
                b->capv = b->capv * 2 + 4096;
                m->v = realloc(m->v, (size_t)b->capv * sizeof(SkinVertex));
            }
            m->v[m->nv++] = b->strip[j];
        }
    }
    d->count = m->nv - d->first;
    if (d->count == 0) {
        m->nd--;
    }
}

/* positions: a 16-byte header (3 x s32 start), then 3 x s16 deltas a vertex */
static void read_positions(Reader *r, Builder *b, size_t off, int n) {
    int32_t p[3];
    int k, j;

    for (j = 0; j < 3; j++) {
        p[j] = (int32_t)u32(r, off + j * 4);
    }
    for (k = 0; k < n; k++) {
        for (j = 0; j < 3; j++) {
            p[j] += s16(r, off + 0x10 + k * 6 + j * 2);
        }
        b->strip[k].pos = vec3(p[0] * POS_SCALE, p[1] * POS_SCALE, p[2] * POS_SCALE);
    }
}

static void read_uv_normals(Reader *r, Builder *b, size_t uv, size_t nrm, int n) {
    int k;

    for (k = 0; k < n; k++) {
        b->strip[k].s = u16(r, uv + k * 4) * UNIT;
        b->strip[k].t = u16(r, uv + k * 4 + 2) * UNIT;
        b->strip[k].normal = vec3(s16(r, nrm + k * 6) * UNIT, s16(r, nrm + k * 6 + 2) * UNIT, s16(r, nrm + k * 6 + 4) * UNIT);
    }
}

static void one_bone(Builder *b, int n, int bone) {
    int k;

    for (k = 0; k < n; k++) {
        b->strip[k].bones[0] = (uint8_t)bone;
        b->strip[k].weights[0] = 1.0f;
    }
}

/* skinned parts: resource 0 + mesh table; 0x30-byte records, offsets from the record */
static void read_skinned(Reader *r, Builder *b, size_t table) {
    uint32_t nparts = u32(r, table), i;

    for (i = 0; i < nparts && !r->bad; i++) {
        size_t rec = table + 0x10 + i * 0x30;
        int n = (int)u32(r, rec), infl = (int)u32(r, rec + 0x2C), k, j;
        uint32_t texture = u32(r, rec + 0x1C), flags = u32(r, rec + 0x20), npal = u32(r, rec + 0x24);
        size_t pal = rec + u32(r, rec + 0x28), wts = rec + u32(r, rec + 0x10), slots = rec + u32(r, rec + 0x14);
        size_t fl = rec + u32(r, rec + 0x18);

        if (n <= 0 || n > 0x100000 || infl < 1 || infl > 4) {
            continue;
        }
        strip_reset(b, n);
        read_positions(r, b, rec + u32(r, rec + 0x04), n);
        read_uv_normals(r, b, rec + u32(r, rec + 0x08), rec + u32(r, rec + 0x0C), n);
        for (k = 0; k < n; k++) {
            float sum = 0.0f;

            for (j = 0; j < infl; j++) {
                uint32_t slot = u8(r, slots + (size_t)k * infl + j) / 4;

                b->strip[k].bones[j] = slot < npal ? u8(r, pal + slot) : 0;
                b->strip[k].weights[j] = u16(r, wts + ((size_t)k * infl + j) * 2) * UNIT;
                sum += b->strip[k].weights[j];
            }
            if (sum == 0.0f) {
                b->strip[k].weights[0] = 1.0f;
            }
            b->no_tri[k] = u8(r, fl + k) != 0;
        }
        strip_flush(b, (int)texture, flags & 1);
    }
}

/* rigid parts (eyeballs, ...): 0x20-byte records - count, positions, UVs, normals, strip flags,
 * texture, ?, bone */
static void read_rigid(Reader *r, Builder *b, size_t table) {
    uint32_t count = u32(r, table), i;

    for (i = 0; i < count && !r->bad; i++) {
        size_t rec = table + 0x10 + i * 0x20;
        int n = (int)u32(r, rec), k;

        if (n <= 0 || n > 0x100000) {
            continue;
        }
        strip_reset(b, n);
        read_positions(r, b, rec + u32(r, rec + 0x04), n);
        read_uv_normals(r, b, rec + u32(r, rec + 0x08), rec + u32(r, rec + 0x0C), n);
        one_bone(b, n, (int)u32(r, rec + 0x1C));
        for (k = 0; k < n; k++) {
            b->no_tri[k] = u8(r, rec + u32(r, rec + 0x10) + k) != 0;
        }
        strip_flush(b, (int)u32(r, rec + 0x14), 0);
    }
}

/* morphing parts (faces, hands; resource 1), in their rest shape: 0x40-byte records - shapes,
 * count, UVs, strip flags, shape table (8-byte entries: positions, normals, from the entry),
 * bone, texture, kind (1 face: u32 flags, bit 15; 0 hands: u8 flags, bit 0), flags, base point */
static void read_morphs(Reader *r, Builder *b, size_t res) {
    uint32_t count = u32(r, res), i;

    for (i = 0; i < count && !r->bad; i++) {
        size_t rec = res + 0x10 + i * 0x40, shape0 = rec + u32(r, rec + 0x10);
        int n = (int)u32(r, rec + 0x04), kind = (int)u32(r, rec + 0x1C), k, j;
        size_t pos = shape0 + u32(r, shape0), nrm = shape0 + u32(r, shape0 + 4), fl = rec + u32(r, rec + 0x0C);
        int32_t base[3];

        if (n <= 0 || n > 0x100000 || u32(r, rec) == 0) {
            continue;
        }
        for (j = 0; j < 3; j++) {
            base[j] = (int32_t)u32(r, rec + 0x30 + j * 4);
        }
        strip_reset(b, n);
        for (k = 0; k < n; k++) {
            SkinVertex *v = &b->strip[k];

            v->pos = vec3((base[0] + s16(r, pos + k * 6)) * POS_SCALE, (base[1] + s16(r, pos + k * 6 + 2)) * POS_SCALE,
                          (base[2] + s16(r, pos + k * 6 + 4)) * POS_SCALE);
            v->normal = vec3(s16(r, nrm + k * 6) * UNIT, s16(r, nrm + k * 6 + 2) * UNIT, s16(r, nrm + k * 6 + 4) * UNIT);
            v->s = u16(r, rec + u32(r, rec + 0x08) + k * 4) * UNIT;
            v->t = u16(r, rec + u32(r, rec + 0x08) + k * 4 + 2) * UNIT;
            b->no_tri[k] = kind != 0 ? (u32(r, fl + k * 4) & 0x8000) != 0 : (u8(r, fl + k) & 1) != 0;
        }
        one_bone(b, n, (int)u32(r, rec + 0x14));
        strip_flush(b, (int)u32(r, rec + 0x18), u32(r, rec + 0x20) & 1);
    }
}

/* draws in the second pass (blended) go after the others */
static void order_draws(Model *m) {
    MeshDraw *sorted = malloc((size_t)m->nd * sizeof(MeshDraw));
    int i, n = 0, pass;

    for (pass = 0; pass < 2; pass++) {
        for (i = 0; i < m->nd; i++) {
            if (m->d[i].blend == pass) {
                sorted[n++] = m->d[i];
            }
        }
    }
    memcpy(m->d, sorted, (size_t)n * sizeof(MeshDraw));
    free(sorted);
}

int model_build(Model *m, uint8_t *pck, size_t size) {
    Reader r = {pck, size, 0};
    Builder b;
    uint32_t count, res0, i;

    memset(m, 0, sizeof(*m));
    memset(&b, 0, sizeof(b));
    m->pck = pck;
    m->size = size;
    b.m = m;
    count = u32(&r, 0);
    if (count == 0 || count > 16) {
        return 0;
    }
    res0 = u32(&r, 4);
    m->nbones = (int)u32(&r, res0);
    if (m->nbones <= 0 || m->nbones > MODEL_MAX_BONES) {
        return 0;
    }
    for (i = 0; i < (uint32_t)m->nbones; i++) {
        size_t rec = res0 + 0x10 + i * 0x70;
        ModelBone *bone = &m->bones[i];
        int j;

        bone->parent = (int32_t)u32(&r, rec);
        for (j = 0; j < 3; j++) {
            bone->rest_rot[j] = f32(&r, rec + 0x10 + j * 4);
            bone->rest_pos[j] = f32(&r, rec + 0x20 + j * 4);
        }
        for (j = 0; j < 16; j++) {   /* row-major, row vectors = column-major, column vectors */
            bone->inv_bind.m[j] = f32(&r, rec + 0x30 + j * 4);
        }
    }
    m->bone_table = at(&r, res0 + u32(&r, res0 + 12), 256);
    read_skinned(&r, &b, res0 + u32(&r, res0 + 4));
    if (u32(&r, res0 + 8) != 0) {
        read_rigid(&r, &b, res0 + u32(&r, res0 + 8));
    }
    if (count > 1 && u32(&r, 8) != 0) {
        read_morphs(&r, &b, u32(&r, 8));
    }
    if (count > 3 && u32(&r, 16) != 0 && u32(&r, 16) < size) {
        m->motions = pck + u32(&r, 16);
        m->motions_size = size - u32(&r, 16);
    }
    free(b.strip);
    free(b.no_tri);
    if (r.bad) {
        model_free(m);
        return 0;
    }
    order_draws(m);
    return 1;
}

void model_free(Model *m) {
    free(m->pck);
    free(m->v);
    free(m->d);
    memset(m, 0, sizeof(*m));
}

/* ---- motions ----
 * Bank: u32 records, u32 ?, u32 ?, u32 id map; records of 0x14 bytes at +0x10 (five part offsets
 * from the record). Id map: u32 count, then (id, record) pairs at +0x10. Part: u32 tracks,
 * u32 frames, u32 track table (from the part); track: s32 code (< 0 special channels; else the
 * bone is bone_table[code + 1]), u32 type (bit 16: one key only), u32 keys (from the track). */

static Reader bank(const Model *m) {
    Reader r = {m->motions, m->motions_size, 0};

    return r;
}

int model_motion_count(const Model *m) {
    Reader r = bank(m);

    return m->motions != NULL ? (int)u32(&r, 0) : 0;
}

int model_motion_id(const Model *m, int index) {
    Reader r = bank(m);
    size_t map = u32(&r, 12);
    uint32_t n = u32(&r, map), i;

    for (i = 0; i < n && !r.bad; i++) {
        if ((int)u32(&r, map + 0x10 + i * 8 + 4) == index) {
            return (int)u32(&r, map + 0x10 + i * 8);
        }
    }
    return index;
}

int model_motion_find(const Model *m, int id) {
    Reader r = bank(m);
    size_t map;
    uint32_t n, i;

    if (m->motions == NULL) {
        return -1;
    }
    map = u32(&r, 12);
    n = u32(&r, map);
    for (i = 0; i < n && !r.bad; i++) {
        if ((int)u32(&r, map + 0x10 + i * 8) == id) {
            return (int)u32(&r, map + 0x10 + i * 8 + 4);
        }
    }
    return -1;
}

int model_motion_frames(const Model *m, int index) {
    Reader r = bank(m);
    size_t rec = 0x10 + (size_t)index * 0x14;
    int p, frames = 1;

    if (index < 0 || index >= model_motion_count(m)) {
        return 1;
    }
    for (p = 0; p < 5; p++) {
        uint32_t off = u32(&r, rec + p * 4);

        if (off != 0 && (int)u32(&r, rec + off + 4) > frames) {
            frames = (int)u32(&r, rec + off + 4);
        }
    }
    return frames;
}

static float lerp_angle(float a, float b, float t) {
    float d = b - a;

    while (d > 3.14159265f) {
        d -= 6.28318531f;
    }
    while (d < -3.14159265f) {
        d += 6.28318531f;
    }
    return a + d * t;
}

/* one track's values at a frame into the bone's rotation and/or translation */
static void sample_track(Reader *r, size_t track, int frames, float frame, float *rot, float *pos) {
    uint32_t type = u32(r, track + 4), kind = type & 0xFFFF;
    size_t keys = track + u32(r, track + 8);
    int count = (type >> 16 & 1) ? 1 : frames, a, b, j;
    float t, va[6], vb[6];
    size_t stride;

    if (count <= 0) {
        return;
    }
    a = (int)frame % count;
    b = (a + 1) % count;
    t = frame - (float)(int)frame;
    switch (kind) {
    case 0: case 1: stride = 6; break;
    case 2: case 4: stride = 12; break;
    case 7: stride = 24; break;
    default: return;   /* (other kinds: not used by the characters seen so far) */
    }
    for (j = 0; j < (int)(stride == 6 ? 3 : 6); j++) {
        if (kind == 7) {
            va[j] = f32(r, keys + a * stride + j * 4);
            vb[j] = f32(r, keys + b * stride + j * 4);
        } else {
            va[j] = s16(r, keys + a * stride + j * 2);
            vb[j] = s16(r, keys + b * stride + j * 2);
        }
    }
    if (kind == 1) {   /* translation only */
        for (j = 0; j < 3; j++) {
            pos[j] = (va[j] + (vb[j] - va[j]) * t) * TRANS;
        }
        return;
    }
    for (j = 0; j < 3; j++) {
        float s = kind == 7 ? 1.0f : ANGLE;

        rot[j] = lerp_angle(va[j] * s, vb[j] * s, t);
    }
    if (kind != 0) {
        for (j = 0; j < 3; j++) {
            float s = kind == 7 ? 1.0f : TRANS;

            pos[j] = (va[3 + j] + (vb[3 + j] - va[3 + j]) * t) * s;
        }
    }
}

/* the local matrix: rotate about X, then Y, then Z, then translate */
static Mat4 bone_local(const float *rot, const float *pos) {
    float cx = cosf(rot[0]), sx = sinf(rot[0]), cy = cosf(rot[1]), sy = sinf(rot[1]);
    float cz = cosf(rot[2]), sz = sinf(rot[2]);
    Mat4 rx = mat4_identity(), ry = mat4_identity(), rz = mat4_identity(), tr = mat4_identity();

    rx.m[5] = cx;  rx.m[6] = sx;  rx.m[9] = -sx;  rx.m[10] = cx;
    ry.m[0] = cy;  ry.m[2] = -sy; ry.m[8] = sy;   ry.m[10] = cy;
    rz.m[0] = cz;  rz.m[1] = sz;  rz.m[4] = -sz;  rz.m[5] = cz;
    tr.m[12] = pos[0];
    tr.m[13] = pos[1];
    tr.m[14] = pos[2];
    return mat4_mul(tr, mat4_mul(rz, mat4_mul(ry, rx)));
}

int model_root_delta(const Model *m, int index, float frame, float *turn, Vec3 *step) {
    Reader r = bank(m);
    size_t rec = 0x10 + (size_t)index * 0x14;
    int p;

    *turn = 0.0f;
    *step = vec3(0, 0, 0);
    if (index < 0 || index >= model_motion_count(m)) {
        return 0;
    }
    for (p = 0; p < 5; p++) {
        size_t part = rec + u32(&r, rec + p * 4);
        uint32_t ntracks, k;
        int frames;

        if (part == rec) {
            continue;
        }
        ntracks = u32(&r, part);
        frames = (int)u32(&r, part + 4);
        for (k = 0; k < ntracks && !r.bad; k++) {
            size_t track = part + u32(&r, part + 8) + k * 12;
            float rot[3] = {0, 0, 0}, pos[3] = {0, 0, 0};

            if ((int32_t)u32(&r, track) != -1) {
                continue;
            }
            sample_track(&r, track, frames, frame, rot, pos);
            *turn = rot[1];
            *step = vec3(pos[0], pos[1], pos[2]);
            return 1;
        }
    }
    return 0;
}

void model_pose(const Model *m, int index, float frame, Mat4 *skin) {
    model_pose_root(m, index, frame, skin, NULL);
}

/* the bones' local turns and places for a motion at a time (the rest pose for index -1) */
static void local_pose(const Model *m, int index, float frame, float (*rot)[3], float (*pos)[3]) {
    int i, p;

    for (i = 0; i < m->nbones; i++) {
        memcpy(rot[i], m->bones[i].rest_rot, sizeof(rot[i]));
        memcpy(pos[i], m->bones[i].rest_pos, sizeof(pos[i]));
    }
    if (index >= 0 && index < model_motion_count(m) && m->bone_table != NULL) {
        Reader r = bank(m);
        size_t rec = 0x10 + (size_t)index * 0x14;

        for (p = 0; p < 5; p++) {
            size_t part = rec + u32(&r, rec + p * 4);
            uint32_t ntracks, k;
            int frames;

            if (part == rec) {
                continue;
            }
            ntracks = u32(&r, part);
            frames = (int)u32(&r, part + 4);
            for (k = 0; k < ntracks && !r.bad; k++) {
                size_t track = part + u32(&r, part + 8) + k * 12;
                int32_t code = (int32_t)u32(&r, track);
                int bone;

                if (code < 0 || code + 1 >= 256) {
                    continue;   /* the root's own movement and other special channels */
                }
                bone = (int8_t)m->bone_table[code + 1];
                if (bone >= 0 && bone < m->nbones) {
                    sample_track(&r, track, frames, frame, rot[bone], pos[bone]);
                }
            }
        }
    }
}

void model_pose_root(const Model *m, int index, float frame, Mat4 *skin, Vec3 *root) {
    model_pose_blend(m, index, frame, -1, 0.0f, 0.0f, skin, root);
}

static Vec3 sOrigins[MODEL_MAX_BONES];   /* the last pose's bone origins (model space) */

const Vec3 *model_pose_origins(void) { return sOrigins; }

void model_pose_blend(const Model *m, int index, float frame, int prev, float prev_frame, float w, Mat4 *skin,
                      Vec3 *root) {
    static float rot[MODEL_MAX_BONES][3], pos[MODEL_MAX_BONES][3], rot2[MODEL_MAX_BONES][3], pos2[MODEL_MAX_BONES][3];
    Mat4 world[MODEL_MAX_BONES];
    int done[MODEL_MAX_BONES], i, pass, j;

    local_pose(m, index, frame, rot, pos);
    if (w > 0.0f) {   /* fading from the motion before: prev x w + this x (1 - w) */
        local_pose(m, prev, prev_frame, rot2, pos2);
        for (i = 0; i < m->nbones; i++) {
            for (j = 0; j < 3; j++) {
                rot[i][j] = lerp_angle(rot[i][j], rot2[i][j], w);
                pos[i][j] += (pos2[i][j] - pos[i][j]) * w;
            }
        }
    }
    /* world matrices, parents first (a few passes in case a parent comes later in the list) */
    memset(done, 0, sizeof(done));
    for (pass = 0; pass < m->nbones; pass++) {
        int progress = 0;

        for (i = 0; i < m->nbones; i++) {
            int parent = m->bones[i].parent;

            if (done[i] || (parent >= 0 && parent < m->nbones && !done[parent])) {
                continue;
            }
            world[i] = bone_local(rot[i], pos[i]);
            if (parent >= 0 && parent < m->nbones) {
                world[i] = mat4_mul(world[parent], world[i]);
            }
            done[i] = progress = 1;
        }
        if (!progress) {
            break;
        }
    }
    for (i = 0; i < m->nbones; i++) {
        skin[i] = done[i] ? mat4_mul(world[i], m->bones[i].inv_bind) : mat4_identity();
        sOrigins[i] = done[i] ? vec3(world[i].m[12], world[i].m[13], world[i].m[14]) : vec3(0.0f, 0.0f, 0.0f);
        if (root != NULL && m->bones[i].parent < 0 && done[i]) {
            *root = vec3(world[i].m[12], world[i].m[13], world[i].m[14]);
            root = NULL;
        }
    }
}
