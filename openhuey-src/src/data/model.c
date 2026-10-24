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
    int *order;          /* the last draw's vertices: the strip vertex each is */
    int norder, caporder;
    int part;            /* the next part's number (skinned, then rigid, then morphing: the
                          * original's draw flags go by it) */
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
    b->norder = 0;
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
            if (b->norder == b->caporder) {
                b->caporder = b->caporder * 2 + 1024;
                b->order = realloc(b->order, (size_t)b->caporder * sizeof(int));
            }
            b->order[b->norder++] = j;
            m->v[m->nv++] = b->strip[j];
        }
    }
    d->count = m->nv - d->first;
    if (d->count == 0) {
        m->nd--;
    }
}

/* the strip as the draw of part b->part (its group: the part's number from 1, so a model shows
 * or hides parts); 1 if it drew anything */
static int part_flush(Builder *b, int texture, int second_pass) {
    int before = b->m->nd, part = b->part++;

    strip_flush(b, texture, second_pass);
    if (b->m->nd == before) {
        return 0;
    }
    if (part < 255) {
        b->m->d[b->m->nd - 1].group = (uint8_t)(part + 1);
    }
    return 1;
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
            b->part++;
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
        part_flush(b, (int)texture, flags & 1);
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
            b->part++;
            continue;
        }
        strip_reset(b, n);
        read_positions(r, b, rec + u32(r, rec + 0x04), n);
        read_uv_normals(r, b, rec + u32(r, rec + 0x08), rec + u32(r, rec + 0x0C), n);
        one_bone(b, n, (int)u32(r, rec + 0x1C));
        for (k = 0; k < n; k++) {
            b->no_tri[k] = u8(r, rec + u32(r, rec + 0x10) + k) != 0;
        }
        part_flush(b, (int)u32(r, rec + 0x14), 0);
    }
}

/* a morphing part's shapes, for the draw just made (record `rec`, the `index`th): each one's
 * vertices less the rest's - the shapes' s16 keys less shape 0's, so the rest plus one is the
 * shape exactly (base + key) / 4096 */
static void read_shapes(Reader *r, Builder *b, size_t rec, uint32_t index, int face) {
    Model *m = b->m;
    const MeshDraw *d = &m->d[m->nd - 1];
    size_t table = rec + u32(r, rec + 0x10);
    int nshapes = (int)u32(r, rec), s, k, j;
    ModelMorph *mo;

    if (nshapes > 64) {
        return;
    }
    m->morphs = realloc(m->morphs, (size_t)(m->nmorphs + 1) * sizeof(ModelMorph));
    mo = &m->morphs[m->nmorphs++];
    mo->first = d->first;
    mo->count = d->count;
    mo->face = face;
    mo->hand = index == 1 ? 0 : 1;
    mo->nshapes = nshapes;
    mo->dpos = calloc((size_t)nshapes * (size_t)d->count, sizeof(Vec3));
    mo->dnrm = calloc((size_t)nshapes * (size_t)d->count, sizeof(Vec3));
    for (s = 1; s < nshapes; s++) {
        size_t e = table + (size_t)s * 8, e0 = table, pos = e + u32(r, e), nrm = e + u32(r, e + 4);
        size_t pos0 = e0 + u32(r, e0), nrm0 = e0 + u32(r, e0 + 4);

        for (k = 0; k < d->count; k++) {
            size_t q = (size_t)b->order[k] * 6;
            float dp[3], dn[3];

            for (j = 0; j < 3; j++) {
                dp[j] = (float)(s16(r, pos + q + j * 2) - s16(r, pos0 + q + j * 2)) * POS_SCALE;
                dn[j] = (float)(s16(r, nrm + q + j * 2) - s16(r, nrm0 + q + j * 2)) * UNIT;
            }
            mo->dpos[s * d->count + k] = vec3(dp[0], dp[1], dp[2]);
            mo->dnrm[s * d->count + k] = vec3(dn[0], dn[1], dn[2]);
        }
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
            b->part++;
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
        if (part_flush(b, (int)u32(r, rec + 0x18), u32(r, rec + 0x20) & 1)) {
            read_shapes(r, b, rec, i, kind != 0);
        }
    }
}

static int draw_at(const Model *m, int first) {   /* (the draw whose vertices start there) */
    int i;

    for (i = 0; i < m->nd && m->d[i].first != first; i++) {
    }
    return i;
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

/* the morphing parts in their draws' order (as the library's glTF has them) */
static void order_morphs(Model *m) {
    int i, j;

    for (i = 1; i < m->nmorphs; i++) {
        for (j = i; j > 0 && draw_at(m, m->morphs[j].first) < draw_at(m, m->morphs[j - 1].first); j--) {
            ModelMorph t = m->morphs[j];

            m->morphs[j] = m->morphs[j - 1];
            m->morphs[j - 1] = t;
        }
    }
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
    free(b.order);
    if (r.bad) {
        model_free(m);
        return 0;
    }
    order_draws(m);
    order_morphs(m);
    return 1;
}

void model_free(Model *m) {
    int i;

    for (i = 0; i < m->nmorphs; i++) {
        free(m->morphs[i].dpos);
        free(m->morphs[i].dnrm);
    }
    free(m->morphs);
    free(m->pck);
    free(m->v);
    free(m->d);
    memset(m, 0, sizeof(*m));
}

/* the shapes a morphing part is drawn from, and how much of each (func_001BD650): the face the
 * rest, the mouth (1) and the eyelids (7, 8) - the rest what the others leave, a face of 8 shapes
 * or fewer only its rest (func_001BC8D0); a hand its pose's two shapes, (1 - w) of the first and
 * w of the second (func_001BD280), shape 0 if they aren't its */
static int morph_shapes(const ModelMorph *mo, const ModelFace *f, int sh[16], float w[16]) {
    if (mo->face && f->all) {   /* (func_001BCD30: each shape by its weight, the rest what is left) */
        int k, n = 0;

        for (k = 1; k < 16 && k < mo->nshapes; k++) {
            sh[n] = k;
            w[n] = f->w[k];
            n++;
        }
        return n;
    }
    if (mo->face) {   /* (func_001BC8D0: the mouth and the eyelids; with no eyelids, the rest alone) */
        if (mo->nshapes <= 8) {
            return 0;
        }
        sh[0] = 1;  w[0] = f->w[1];
        sh[1] = 7;  w[1] = f->w[7];
        sh[2] = 8;  w[2] = f->w[8];
        return 3;
    }
    sh[0] = f->hand[mo->hand].a;
    sh[1] = f->hand[mo->hand].b;
    if (sh[0] < 0 || sh[0] >= mo->nshapes || sh[1] < 0 || sh[1] >= mo->nshapes) {
        return 0;
    }
    w[1] = f->hand[mo->hand].w;
    w[0] = 1.0f - w[1];
    return 2;
}

void model_morph(const Model *m, const ModelFace *f, SkinVertex *v) {
    int i, k, j;

    for (i = 0; i < m->nmorphs; i++) {
        const ModelMorph *mo = &m->morphs[i];
        int sh[16], n;
        float w[16];

        n = morph_shapes(mo, f, sh, w);
        for (k = 0; k < mo->count; k++) {   /* (the rest, and each shape's way from it by its share) */
            const SkinVertex *rest = &m->v[mo->first + k];
            Vec3 p = rest->pos, nn = rest->normal;

            for (j = 0; j < n; j++) {
                p = vec3_add(p, vec3_scale(mo->dpos[sh[j] * mo->count + k], w[j]));
                nn = vec3_add(nn, vec3_scale(mo->dnrm[sh[j] * mo->count + k], w[j]));
            }
            v[mo->first + k].pos = p;
            v[mo->first + k].normal = nn;
        }
    }
}

/* ---- motions ----
 * Bank: u32 records, u32 ?, u32 ?, u32 id map; records of 0x14 bytes at +0x10 (five part offsets
 * from the record). Id map: u32 count, then (id, record) pairs at +0x10. Part: u32 tracks,
 * u32 frames, u32 track table (from the part); track: s32 code (< 0 special channels; else the
 * bone is bone_table[code + 1]), u32 type (bit 16: one key only), u32 keys (from the track).
 * A motion is numbered by its place in the id map (Motion_AnimIndex), not by its record: some
 * ids share a record (Debilitas's $201 and $206, the turns $400.. and $405..), and the tables
 * keyed by motion - its sounds, events, blend - have a row for each id. */

/* motion `index`'s bank (its own, or from MODEL_SECOND the second) and its place in the id map */
static int bank_of(const Model *m, int index, Reader *r, int *at) {
    const uint8_t *b = m->motions;
    size_t size = m->motions_size;

    if (index >= MODEL_SECOND) {
        b = m->motions2;
        size = m->motions2_size;
        index -= MODEL_SECOND;
    }
    if (b == NULL || index < 0) {
        return 0;
    }
    *r = (Reader){b, size, 0};
    *at = index;
    return (uint32_t)index < u32(r, u32(r, 12));
}

static Reader bank(const Model *m) {
    Reader r = {m->motions, m->motions_size, 0};

    return r;
}

/* motion `index`'s bank and its record there; 0 if there is no such motion */
static int motion_at(const Model *m, int index, Reader *r, size_t *rec) {
    int at;
    uint32_t record;

    if (!bank_of(m, index, r, &at)) {
        return 0;
    }
    record = u32(r, u32(r, 12) + 0x10 + (size_t)at * 8 + 4);
    if (record >= u32(r, 0)) {
        return 0;
    }
    *rec = 0x10 + (size_t)record * 0x14;
    return 1;
}

int model_motion_count(const Model *m) {
    Reader r = bank(m);

    return m->motions != NULL ? (int)u32(&r, u32(&r, 12)) : 0;
}

int model_motion_id(const Model *m, int index) {
    Reader r;
    int at;

    if (!bank_of(m, index, &r, &at)) {
        return index;
    }
    return (int)u32(&r, u32(&r, 12) + 0x10 + (size_t)at * 8);
}

int model_motion_pos(const Model *m, int index) {   /* (motions are numbered so already) */
    return m->motions != NULL && index >= 0 && index < model_motion_count(m) ? index : -1;
}

/* in a bank's id map: its place (the last match, as Motion_AnimEntry), -1 none */
static int find_in(const uint8_t *b, size_t size, int id) {
    Reader r = {b, size, 0};
    size_t map;
    uint32_t n, i;
    int found = -1;

    if (b == NULL) {
        return -1;
    }
    map = u32(&r, 12);
    n = u32(&r, map);
    for (i = 0; i < n && !r.bad; i++) {
        if ((int)u32(&r, map + 0x10 + i * 8) == id) {
            found = (int)i;
        }
    }
    return found;
}

int model_motion_find(const Model *m, int id) {
    int i = find_in(m->motions, m->motions_size, id);

    if (i < 0) {
        i = find_in(m->motions2, m->motions2_size, id);
        if (i >= 0) {
            i += MODEL_SECOND;
        }
    }
    return i;
}

int model_motion_frames(const Model *m, int index) {
    Reader r;
    size_t rec;
    int p, frames = 1;

    if (!motion_at(m, index, &r, &rec)) {
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


/* a bone's rotation (angles about X, then Y, then Z: bone_local's order) faded `w` of the way to
 * another, as a rotation - the shortest turn between the two - and back to angles: angle by
 * angle, two poses whose angles lie far apart (though the poses don't) swing a limb right round
 * on the way (her arm over her head, fading from what she held into her idle) */
static void euler_quat(const float *e, float *q) {
    float cx = cosf(e[0] * 0.5f), sx = sinf(e[0] * 0.5f), cy = cosf(e[1] * 0.5f), sy = sinf(e[1] * 0.5f);
    float cz = cosf(e[2] * 0.5f), sz = sinf(e[2] * 0.5f);

    /* q = qz * qy * qx (w, x, y, z) */
    q[0] = cz * cy * cx + sz * sy * sx;
    q[1] = cz * cy * sx - sz * sy * cx;
    q[2] = cz * sy * cx + sz * cy * sx;
    q[3] = sz * cy * cx - cz * sy * sx;
}
static void quat_euler(const float *q, float *e) {
    float w = q[0], x = q[1], y = q[2], z = q[3];
    float r20 = 2.0f * (x * z - w * y), r21 = 2.0f * (y * z + w * x), r22 = 1.0f - 2.0f * (x * x + y * y);
    float r10 = 2.0f * (x * y + w * z), r00 = 1.0f - 2.0f * (y * y + z * z);

    e[1] = asinf(fmaxf(-1.0f, fminf(1.0f, -r20)));
    e[0] = atan2f(r21, r22);
    e[2] = atan2f(r10, r00);
}
static void blend_rotation(float *a, const float *b, float w) {
    float qa[4], qb[4], q[4], d, len;
    int k;

    if (w <= 0.0f) {
        return;
    }
    if (w >= 1.0f) {
        a[0] = b[0];
        a[1] = b[1];
        a[2] = b[2];
        return;
    }
    euler_quat(a, qa);
    euler_quat(b, qb);
    d = qa[0] * qb[0] + qa[1] * qb[1] + qa[2] * qb[2] + qa[3] * qb[3];
    if (d < 0.0f) {   /* (the shorter way round) */
        for (k = 0; k < 4; k++) {
            qb[k] = -qb[k];
        }
        d = -d;
    }
    if (d > 0.9995f) {   /* (nearly the same: straight across) */
        for (k = 0; k < 4; k++) {
            q[k] = qa[k] + (qb[k] - qa[k]) * w;
        }
    } else {
        float th = acosf(d), s = sinf(th), ka = sinf((1.0f - w) * th) / s, kb = sinf(w * th) / s;

        for (k = 0; k < 4; k++) {
            q[k] = qa[k] * ka + qb[k] * kb;
        }
    }
    len = sqrtf(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
    for (k = 0; k < 4; k++) {
        q[k] /= len;
    }
    quat_euler(q, a);
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
    Reader r;
    size_t rec;
    int p;

    *turn = 0.0f;
    *step = vec3(0, 0, 0);
    if (!motion_at(m, index, &r, &rec)) {
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

static int track_read(const Model *m, int index, int code, float frame, float out[3], uint32_t *kind_out);

int model_track_raw(const Model *m, int index, int code, float frame, float out[3]) {
    return track_read(m, index, code, frame, out, NULL);
}

/* as model_track_raw, its values as Track_Sample gives them: a place track (format 1) in room
 * units of the model (its s16 keys / 256) */
int model_track(const Model *m, int index, int code, float frame, float out[3]) {
    uint32_t kind = 0;
    int has = track_read(m, index, code, frame, out, &kind), j;

    if (has && kind == 1) {
        for (j = 0; j < 3; j++) {
            out[j] *= 0.00390625f;
        }
    }
    return has;
}

static int track_read(const Model *m, int index, int code, float frame, float out[3], uint32_t *kind_out) {
    Reader r;
    size_t rec;
    int p;

    out[0] = out[1] = out[2] = 0.0f;
    if (!motion_at(m, index, &r, &rec)) {
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
            size_t track = part + u32(&r, part + 8) + k * 12, keys;
            uint32_t type, kind;
            int count, a, b2, j;
            float t;

            if ((int32_t)u32(&r, track) != code) {
                continue;
            }
            type = u32(&r, track + 4);
            kind = type & 0xFFFF;
            if (kind_out != NULL) {
                *kind_out = kind;
            }
            keys = track + u32(&r, track + 8);
            count = (type >> 16 & 1) ? 1 : frames;
            if (count <= 0) {
                return 0;
            }
            a = (int)frame % count;
            b2 = (a + 1) % count;
            t = frame - (float)(int)frame;
            for (j = 0; j < 3; j++) {
                float va, vb;

                if (kind == 8 || kind == 9) {
                    va = f32(&r, keys + a * 12 + j * 4);
                    vb = f32(&r, keys + b2 * 12 + j * 4);
                } else {
                    va = s16(&r, keys + a * 6 + j * 2);
                    vb = s16(&r, keys + b2 * 6 + j * 2);
                    if (kind == 5 && j == 2) {
                        va /= 32768.0f;
                        vb /= 32768.0f;
                    }
                }
                out[j] = va + (vb - va) * t;
            }
            return 1;
        }
    }
    return 0;
}

/* Track_Sample's constants, to the bit: 2 pi / 65536, its unit (~1 / 32767), pi, 2 pi */
static float bits_f(uint32_t u) {
    float f;

    memcpy(&f, &u, 4);
    return f;
}
#define TRK_ROT bits_f(0x38C90FDBu)
#define TRK_UNIT bits_f(0x38000100u)
#define TRK_PI bits_f(0x40490FDBu)
#define TRK_TWO_PI bits_f(0x40C90FDBu)

/* a key's values as Track_Sample reads them (format `kind`): which of out it set */
static int key_values(Reader *r, size_t keys, uint32_t kind, int k, float out[8]) {
    int j;

    switch (kind) {
    case 0: case 1:
        for (j = 0; j < 3; j++) {
            out[j] = (float)s16(r, keys + (size_t)k * 6 + j * 2) * (kind == 0 ? TRK_ROT : 0.00390625f);
        }
        return 0x07;
    case 3:
        for (j = 0; j < 4; j++) {
            out[j] = (float)s16(r, keys + (size_t)k * 8 + j * 2) * TRK_UNIT;
        }
        return 0x0F;
    case 2: case 4:
        for (j = 0; j < 3; j++) {
            out[j] = (float)s16(r, keys + (size_t)k * 12 + j * 2) * TRK_ROT;
            out[4 + j] = (float)s16(r, keys + (size_t)k * 12 + 6 + j * 2) * 0.00390625f;
        }
        return 0x77;
    case 5: {   /* (x and y the s16s as they are - their bits, as an int: the original's) */
        int32_t x = s16(r, keys + (size_t)k * 6), y = s16(r, keys + (size_t)k * 6 + 2);

        memcpy(&out[0], &x, 4);
        memcpy(&out[1], &y, 4);
        out[2] = (float)s16(r, keys + (size_t)k * 6 + 4) * TRK_UNIT;
        return 0x07;
    }
    case 6:
        for (j = 0; j < 8; j++) {
            out[j] = f32(r, keys + (size_t)k * 32 + j * 4);
        }
        return 0xFF;
    case 7:
        for (j = 0; j < 3; j++) {
            out[j] = f32(r, keys + (size_t)k * 24 + j * 4);
            out[4 + j] = f32(r, keys + (size_t)k * 24 + 12 + j * 4);
        }
        return 0x77;
    case 8: case 9:
        for (j = 0; j < 3; j++) {
            out[j] = f32(r, keys + (size_t)k * 12 + j * 4);
        }
        return 0x07;
    }
    return 0;
}

int model_track_sample(const Model *m, int index, int code, float frame, float out[8]) {
    Reader r;
    size_t rec;
    int p;

    if (!motion_at(m, index, &r, &rec)) {
        return 0;
    }
    for (p = 0; p < 5; p++) {
        size_t part = rec + u32(&r, rec + p * 4);
        uint32_t ntracks, k;

        if (part == rec) {
            continue;
        }
        ntracks = u32(&r, part);
        for (k = 0; k < ntracks && !r.bad; k++) {
            size_t track = part + u32(&r, part + 8) + k * 12, keys;
            uint32_t type, kind;
            int fixed, count, a, b, mask, j;
            float base, frac, nx[8];

            if ((int32_t)u32(&r, track) != code) {
                continue;
            }
            type = u32(&r, track + 4);
            kind = type & 0xFFFF;
            fixed = (type >> 16) & 1;
            keys = track + u32(&r, track + 8);
            count = (int)u32(&r, part + 4);   /* (a key a frame: the part's frames) */
            base = fixed ? 0.0f : (float)(int)frame;
            a = (int)base;
            if (a < 0 || (!fixed && a >= count)) {
                return 0;
            }
            mask = key_values(&r, keys, kind, a, out);
            if (frame == base || fixed || !(kind == 0 || kind == 1 || kind == 2 || kind == 4 || kind == 7)) {
                return r.bad ? 0 : mask;   /* (the others stepped: Track_Sample blends only these) */
            }
            frac = frame - base;
            b = a + 1 < count ? a + 1 : 0;
            key_values(&r, keys, kind, b, nx);
            for (j = 0; j < 8; j++) {
                if (mask & (1 << j)) {
                    float n = nx[j];

                    if ((kind == 0 || kind == 2 || kind == 4) && j < 3) {   /* (angles the short way round) */
                        float d = n - out[j];

                        if (!((d <= 0.0f ? -d : d) <= TRK_PI)) {
                            n += d <= 0.0f ? TRK_TWO_PI : -TRK_TWO_PI;
                        }
                    }
                    out[j] = out[j] + frac * (n - out[j]);
                }
            }
            return r.bad ? 0 : mask;
        }
    }
    return 0;
}

void model_pose(const Model *m, int index, float frame, Mat4 *skin) {
    model_pose_root(m, index, frame, skin, NULL);
}

/* the bones' local turns and places for a motion at a time (the rest pose for index -1) */
int model_motion_parts(const Model *m, int index) {
    Reader r;
    size_t rec;
    int p, mask = 0;

    if (!motion_at(m, index, &r, &rec)) {
        return 0;
    }
    for (p = 0; p < 5; p++) {
        if (u32(&r, rec + p * 4) != 0) {
            mask |= 1 << p;
        }
    }
    return mask;
}

/* the parts `mask` of a motion at a time over the bones' values (rest: reset first) */
static void local_pose_parts(const Model *m, int index, float frame, float (*rot)[3], float (*pos)[3], int mask,
                             int rest) {
    int i, p;

    if (rest) {
        for (i = 0; i < m->nbones; i++) {
            memcpy(rot[i], m->bones[i].rest_rot, sizeof(rot[i]));
            memcpy(pos[i], m->bones[i].rest_pos, sizeof(pos[i]));
        }
    }
    Reader r;
    size_t rec;

    if (m->bone_table != NULL && motion_at(m, index, &r, &rec)) {

        for (p = 0; p < 5; p++) {
            size_t part = rec + u32(&r, rec + p * 4);
            uint32_t ntracks, k;
            int frames;

            if (part == rec || !(mask & (1 << p))) {
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
static Vec3 sAxes[MODEL_MAX_BONES][3];   /* and their axes */

const Vec3 *model_pose_origins(void) { return sOrigins; }
const Vec3 (*model_pose_axes(void))[3] { return (const Vec3 (*)[3])sAxes; }

static ModelTurn sTurns[8];
static int sNTurns;
static ModelPoseHook sHook;
static void *sHookUser;
static ModelBoneAdjust sAdjust;
static void *sAdjustUser;

void model_pose_adjust(ModelBoneAdjust adjust, void *user) {
    sAdjust = adjust;
    sAdjustUser = user;
}

void model_pose_hook(ModelPoseHook hook, void *user) {
    sHook = hook;
    sHookUser = user;
}

void model_pose_turns(const ModelTurn *t, int n) {
    sNTurns = n < 8 ? n : 8;
    if (sNTurns > 0) {   /* (none: t may be NULL) */
        memcpy(sTurns, t, (size_t)sNTurns * sizeof(*t));
    }
}

/* bone i's world matrix turned about its own place: by `pitch` about model x, then `yaw` about
 * model y (its children follow) */
static void turn_bone(Mat4 *w, float pitch, float yaw) {
    float cp = cosf(pitch), sp = sinf(pitch), cy = cosf(yaw), sy = sinf(yaw);
    Mat4 r = mat4_identity(), out = *w;
    int c;

    /* r = Ry(yaw) * Rx(pitch), column-major */
    r.m[0] = cy;   r.m[1] = 0.0f; r.m[2] = -sy;
    r.m[4] = sy * sp; r.m[5] = cp; r.m[6] = cy * sp;
    r.m[8] = sy * cp; r.m[9] = -sp; r.m[10] = cy * cp;
    for (c = 0; c < 3; c++) {
        const float *v = &w->m[c * 4];

        out.m[c * 4 + 0] = r.m[0] * v[0] + r.m[4] * v[1] + r.m[8] * v[2];
        out.m[c * 4 + 1] = r.m[1] * v[0] + r.m[5] * v[1] + r.m[9] * v[2];
        out.m[c * 4 + 2] = r.m[2] * v[0] + r.m[6] * v[1] + r.m[10] * v[2];
    }
    *w = out;
}

void model_pose_blend(const Model *m, int index, float frame, int prev, float prev_frame, float w, Mat4 *skin,
                      Vec3 *root) {
    ModelLayer l = {index, frame, prev, prev_frame, w, 0x1F, -1, 0.0f, 0.0f, -1, 0.0f, 0.0f};

    model_pose_layers(m, &l, 1, skin, root);
}

void model_pose_layers(const Model *m, const ModelLayer *layers, int nlayers, Mat4 *skin, Vec3 *root) {
    static float rot[MODEL_MAX_BONES][3], pos[MODEL_MAX_BONES][3], rot2[MODEL_MAX_BONES][3], pos2[MODEL_MAX_BONES][3];
    static float rot1[MODEL_MAX_BONES][3], pos1[MODEL_MAX_BONES][3];
    static float rot3[MODEL_MAX_BONES][3], pos3[MODEL_MAX_BONES][3];
    Mat4 world[MODEL_MAX_BONES];
    int done[MODEL_MAX_BONES], i, pass, j, k;

    for (i = 0; i < m->nbones; i++) {
        memcpy(rot[i], m->bones[i].rest_rot, sizeof(rot[i]));
        memcpy(pos[i], m->bones[i].rest_pos, sizeof(pos[i]));
    }
    for (k = 0; k < nlayers; k++) {   /* each layer's parts over the bones (later layers win) */
        const ModelLayer *l = &layers[k];

        if (l->index < 0 || l->mask == 0) {
            continue;
        }
        memcpy(rot1, rot, sizeof(float) * 3 * (size_t)m->nbones);
        memcpy(pos1, pos, sizeof(float) * 3 * (size_t)m->nbones);
        local_pose_parts(m, l->index, l->frame, rot1, pos1, l->mask, 0);
        /* (motions and their variants faded as rotations: blend_rotation) */
        if (l->var_w > 0.0f && l->var >= 0) {   /* its variant: this x (1 - share) + variant x share */
            memcpy(rot3, rot, sizeof(float) * 3 * (size_t)m->nbones);
            memcpy(pos3, pos, sizeof(float) * 3 * (size_t)m->nbones);
            local_pose_parts(m, l->var, l->var_frame, rot3, pos3, l->mask, 0);
            for (i = 0; i < m->nbones; i++) {
                blend_rotation(rot1[i], rot3[i], l->var_w);
                for (j = 0; j < 3; j++) {
                    pos1[i][j] += (pos3[i][j] - pos1[i][j]) * l->var_w;
                }
            }
        }
        if (l->w > 0.0f && l->prev >= 0) {   /* fading from the motion before: prev x w + this x (1 - w) */
            memcpy(rot2, rot, sizeof(float) * 3 * (size_t)m->nbones);
            memcpy(pos2, pos, sizeof(float) * 3 * (size_t)m->nbones);
            local_pose_parts(m, l->prev, l->prev_frame, rot2, pos2, l->mask, 0);
            if (l->prev_var_w > 0.0f && l->prev_var >= 0) {
                memcpy(rot3, rot, sizeof(float) * 3 * (size_t)m->nbones);
                memcpy(pos3, pos, sizeof(float) * 3 * (size_t)m->nbones);
                local_pose_parts(m, l->prev_var, l->prev_var_frame, rot3, pos3, l->mask, 0);
                for (i = 0; i < m->nbones; i++) {
                    blend_rotation(rot2[i], rot3[i], l->prev_var_w);
                    for (j = 0; j < 3; j++) {
                        pos2[i][j] += (pos3[i][j] - pos2[i][j]) * l->prev_var_w;
                    }
                }
            }
            for (i = 0; i < m->nbones; i++) {
                blend_rotation(rot1[i], rot2[i], l->w);
                for (j = 0; j < 3; j++) {
                    pos1[i][j] += (pos2[i][j] - pos1[i][j]) * l->w;
                }
            }
        }
        memcpy(rot, rot1, sizeof(float) * 3 * (size_t)m->nbones);
        memcpy(pos, pos1, sizeof(float) * 3 * (size_t)m->nbones);
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
            for (j = 0; j < sNTurns; j++) {
                if (sTurns[j].bone == i) {
                    turn_bone(&world[i], sTurns[j].pitch, sTurns[j].yaw);
                }
            }
            if (sAdjust != NULL) {
                sAdjust(i, world, sAdjustUser);
            }
            done[i] = progress = 1;
        }
        if (!progress) {
            break;
        }
    }
    if (sHook != NULL) {
        sHook(world, m->nbones, sHookUser);
    }
    for (i = 0; i < m->nbones; i++) {
        skin[i] = done[i] ? mat4_mul(world[i], m->bones[i].inv_bind) : mat4_identity();
        sOrigins[i] = done[i] ? vec3(world[i].m[12], world[i].m[13], world[i].m[14]) : vec3(0.0f, 0.0f, 0.0f);
        sAxes[i][0] = vec3(world[i].m[0], world[i].m[1], world[i].m[2]);
        sAxes[i][1] = vec3(world[i].m[4], world[i].m[5], world[i].m[6]);
        sAxes[i][2] = vec3(world[i].m[8], world[i].m[9], world[i].m[10]);
        if (root != NULL && m->bones[i].parent < 0 && done[i]) {
            *root = vec3(world[i].m[12], world[i].m[13], world[i].m[14]);
            root = NULL;
        }
    }
}
