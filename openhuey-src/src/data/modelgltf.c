/* A character model as glTF: see modelgltf.h. */
#include "modelgltf.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gltfio.h"

/* q = qz * qy * qx (model.c's euler_quat), as glTF writes it: x, y, z, w */
static void rest_quat(const float *e, float *q) {
    float cx = cosf(e[0] * 0.5f), sx = sinf(e[0] * 0.5f), cy = cosf(e[1] * 0.5f), sy = sinf(e[1] * 0.5f);
    float cz = cosf(e[2] * 0.5f), sz = sinf(e[2] * 0.5f);

    q[3] = cz * cy * cx + sz * sy * sx;
    q[0] = cz * cy * sx - sz * sy * cx;
    q[1] = cz * sy * cx + sz * cy * sx;
    q[2] = sz * cy * cx - cz * sy * sx;
}

/* the morphing part draw `d` is, or -1 */
static int morph_of(const Model *m, const MeshDraw *d) {
    int i;

    for (i = 0; i < m->nmorphs; i++) {
        if (m->morphs[i].first == d->first && m->morphs[i].count == d->count) {
            return i;
        }
    }
    return -1;
}

static void draw_words(GltfText *o, const MeshDraw *d) {
    gltf_add(o, "%d %d %d %d %d %d %d %d %d", d->part, d->blend, d->additive, d->no_zwrite, d->group, d->solid_tex,
             d->lit, d->mask, d->texture);
}

/* a morph's vertices less the repeats - the same vertex with the same shapes - onto u (*nu so far),
 * each one's index into them into idx; its distinct vertices' shapes (1..: each less the rest) */
static void morph_distinct(const Model *m, int mo, SkinVertex *u, int *nu, uint32_t *idx, int *count, float **sp,
                           float **sn) {
    const ModelMorph *mm = &m->morphs[mo];
    size_t rec = sizeof(SkinVertex) + (size_t)(mm->nshapes - 1) * 24, at;
    uint8_t *all = malloc(rec * (size_t)mm->count + 1), *uniq = malloc(rec * (size_t)mm->count + 1);
    int n = 0, k, s;

    for (k = 0; k < mm->count; k++) {
        uint8_t *r = all + (size_t)k * rec;

        memcpy(r, &m->v[mm->first + k], sizeof(SkinVertex));
        for (s = 1; s < mm->nshapes; s++) {
            memcpy(r + sizeof(SkinVertex) + (size_t)(s - 1) * 24, &mm->dpos[s * mm->count + k], 12);
            memcpy(r + sizeof(SkinVertex) + (size_t)(s - 1) * 24 + 12, &mm->dnrm[s * mm->count + k], 12);
        }
    }
    gltf_distinct(all, mm->count, rec, uniq, &n, idx);
    *sp = malloc((size_t)(mm->nshapes - 1) * (size_t)n * 12 + 4);
    *sn = malloc((size_t)(mm->nshapes - 1) * (size_t)n * 12 + 4);
    for (k = 0; k < n; k++) {
        const uint8_t *r = uniq + (size_t)k * rec;

        memcpy(&u[*nu + k], r, sizeof(SkinVertex));
        for (s = 1; s < mm->nshapes; s++) {
            at = ((size_t)(s - 1) * (size_t)n + (size_t)k) * 3;
            memcpy(*sp + at, r + sizeof(SkinVertex) + (size_t)(s - 1) * 24, 12);
            memcpy(*sn + at, r + sizeof(SkinVertex) + (size_t)(s - 1) * 24 + 12, 12);
        }
    }
    *nu += n;
    *count = n;
    free(all);
    free(uniq);
}

char *modelgltf_write(const Model *m, const char *bin_name, const char *textures, uint8_t **bin_out, size_t *bin_size,
                      size_t *len) {
    GltfText o = {0}, acc = {0}, prims = {0}, nodes = {0}, mats = {0}, mmesh = {0};
    GltfBin b = {0};
    SkinVertex *u = malloc(sizeof(SkinVertex) * (size_t)(m->nv + 1));
    uint32_t *idx = malloc(sizeof(uint32_t) * (size_t)(m->nv + 1));
    int *ufirst = malloc(sizeof(int) * (size_t)(m->nd + 1)), *ucount = malloc(sizeof(int) * (size_t)(m->nd + 1));
    int nu = 0, i, k, next = 0, maxtex = -1, *matkey = malloc(sizeof(int) * (size_t)(m->nd + 1)), nmats = 0;
    size_t vat[5], iat, mat_at, sat[2], slen[2] = {0, 0};
    int nprims = 0;
    float *pos, *nrm, *st, *w;
    uint8_t *j;

    /* each morph's distinct vertices' shapes (its targets: shape by shape, a VEC3 a vertex) */
    int *mcount = calloc((size_t)m->nmorphs + 1, sizeof(int));
    float **mpos = calloc((size_t)m->nmorphs + 1, sizeof(float *)), **mnrm = calloc((size_t)m->nmorphs + 1, sizeof(float *));

    for (i = 0; i < m->nd; i++) {   /* (each draw indexed: its distinct vertices, a run of u; a morph's
                                     * distinct with their shapes) */
        int mo = morph_of(m, &m->d[i]);

        ufirst[i] = nu;
        if (mo >= 0) {
            morph_distinct(m, mo, u, &nu, idx + m->d[i].first, &mcount[mo], &mpos[mo], &mnrm[mo]);
        } else {
            gltf_distinct(m->v + m->d[i].first, m->d[i].count, sizeof(SkinVertex), u, &nu, idx + m->d[i].first);
        }
        ucount[i] = nu - ufirst[i];
    }
    pos = malloc(sizeof(float) * 3 * (size_t)(nu + 1));
    nrm = malloc(sizeof(float) * 3 * (size_t)(nu + 1));
    st = malloc(sizeof(float) * 2 * (size_t)(nu + 1));
    w = malloc(sizeof(float) * 4 * (size_t)(nu + 1));
    j = malloc(4 * (size_t)(nu + 1));
    for (i = 0; i < nu; i++) {
        memcpy(pos + i * 3, &u[i].pos, 12);
        memcpy(nrm + i * 3, &u[i].normal, 12);
        st[i * 2] = u[i].s;
        st[i * 2 + 1] = u[i].t;
        memcpy(w + i * 4, u[i].weights, 16);
        memcpy(j + i * 4, u[i].bones, 4);
    }
    vat[0] = gltf_put(&b, pos, (size_t)nu * 12);
    vat[1] = gltf_put(&b, nrm, (size_t)nu * 12);
    vat[2] = gltf_put(&b, st, (size_t)nu * 8);
    vat[3] = gltf_put(&b, j, (size_t)nu * 4);
    vat[4] = gltf_put(&b, w, (size_t)nu * 16);
    iat = gltf_put(&b, idx, (size_t)m->nv * 4);
    {
        float *ib = malloc(sizeof(float) * 16 * (size_t)m->nbones);

        for (i = 0; i < m->nbones; i++) {
            memcpy(ib + i * 16, m->bones[i].inv_bind.m, 64);
        }
        mat_at = gltf_put(&b, ib, (size_t)m->nbones * 64);
        free(ib);
    }
    for (i = 0; i < m->nmorphs; i++) {   /* (the morphs' shapes less the rest - but the rest's own - in turn) */
        slen[0] += (size_t)(m->morphs[i].nshapes - 1) * (size_t)mcount[i] * 12;
    }
    slen[1] = slen[0];
    {
        uint8_t *sp = malloc(slen[0] + 4), *sn = malloc(slen[0] + 4);
        size_t at = 0;

        for (i = 0; i < m->nmorphs; i++) {
            size_t n = (size_t)(m->morphs[i].nshapes - 1) * (size_t)mcount[i] * 12;

            memcpy(sp + at, mpos[i], n);
            memcpy(sn + at, mnrm[i], n);
            at += n;
        }
        sat[0] = gltf_put(&b, sp, slen[0]);
        sat[1] = gltf_put(&b, sn, slen[1]);
        free(sp);
        free(sn);
    }
    free(pos);
    free(nrm);
    free(st);
    free(w);
    free(j);
    /* the mesh: a primitive a draw */
    for (i = 0; i < m->nd; i++) {
        const MeshDraw *d = &m->d[i];
        float lo[3] = {1e30f, 1e30f, 1e30f}, hi[3] = {-1e30f, -1e30f, -1e30f};
        int mt = -1, key = d->texture * 2 + (d->blend ? 1 : 0);

        for (k = ufirst[i]; k < ufirst[i] + ucount[i]; k++) {
            const float *p = &u[k].pos.x;
            int c;

            for (c = 0; c < 3; c++) {
                lo[c] = p[c] < lo[c] ? p[c] : lo[c];
                hi[c] = p[c] > hi[c] ? p[c] : hi[c];
            }
        }
        if (d->texture >= 0) {
            for (mt = 0; mt < nmats && matkey[mt] != key; mt++) {
            }
            if (mt == nmats) {
                matkey[nmats++] = key;
                gltf_add(&mats, "%s{\"name\":\"texture %d%s\",\"pbrMetallicRoughness\":{\"baseColorTexture\":{\"index\":%d},"
                                "\"metallicFactor\":0,\"roughnessFactor\":1},\"alphaMode\":\"%s\"}",
                         mt ? "," : "", d->texture, d->blend ? ", blended" : "", d->texture, d->blend ? "BLEND" : "MASK");
            }
            maxtex = d->texture > maxtex ? d->texture : maxtex;
        }
        {   /* (a morph: a mesh of its own - glTF's shapes go by the mesh - its shapes its targets) */
            int mo = morph_of(m, d);
            GltfText *pr = mo >= 0 ? &mmesh : &prims;

            if (mo >= 0) {
                gltf_add(&mmesh, ",{\"name\":\"%s %d\",\"primitives\":[", m->morphs[mo].face ? "face" : "hand", mo);
            }
            gltf_add(pr, "%s{\"attributes\":{\"POSITION\":%d,\"NORMAL\":%d,\"TEXCOORD_0\":%d,\"JOINTS_0\":%d,"
                         "\"WEIGHTS_0\":%d},\"indices\":%d,\"mode\":4",
                     mo < 0 && nprims++ ? "," : "", next, next + 1, next + 2, next + 3, next + 4, next + 5);
            if (mt >= 0) {
                gltf_add(pr, ",\"material\":%d", mt);
            }
            if (mo >= 0 && m->morphs[mo].nshapes > 1) {
                gltf_add(pr, ",\"targets\":[");
                for (k = 1; k < m->morphs[mo].nshapes; k++) {
                    gltf_add(pr, "%s{\"POSITION\":%d,\"NORMAL\":%d}", k > 1 ? "," : "", next + 6 + (k - 1) * 2,
                             next + 7 + (k - 1) * 2);
                }
                gltf_add(pr, "]");
            }
            gltf_add(pr, ",\"extras\":{\"hga\":\"draw ");
            draw_words(pr, d);
            gltf_add(pr, " %d", d->first);   /* (where its vertices go: the draws are in their passes' order) */
            if (mo >= 0) {   /* (and a morph's kind, its hand, its shapes, where its draw goes) */
                gltf_add(pr, " morph %d %d %d %d", m->morphs[mo].face, m->morphs[mo].hand, m->morphs[mo].nshapes, i);
            }
            gltf_add(pr, "\"}}");
            if (mo >= 0) {
                gltf_add(&mmesh, "]");
                if (m->morphs[mo].nshapes > 1) {
                    gltf_add(&mmesh, ",\"weights\":[");
                    for (k = 1; k < m->morphs[mo].nshapes; k++) {
                        gltf_add(&mmesh, "%s0", k > 1 ? "," : "");
                    }
                    gltf_add(&mmesh, "],\"extras\":{\"targetNames\":[");
                    for (k = 1; k < m->morphs[mo].nshapes; k++) {
                        gltf_add(&mmesh, "%s\"shape %d\"", k > 1 ? "," : "", k);
                    }
                    gltf_add(&mmesh, "]}");
                }
                gltf_add(&mmesh, "}");
            }
        }
        gltf_add(&acc, "%s{\"bufferView\":0,\"byteOffset\":%d,\"componentType\":5126,\"count\":%d,\"type\":\"VEC3\","
                       "\"min\":[%.9g,%.9g,%.9g],\"max\":[%.9g,%.9g,%.9g]}",
                 next ? "," : "", ufirst[i] * 12, ucount[i], lo[0], lo[1], lo[2], hi[0], hi[1], hi[2]);
        gltf_add(&acc, ",{\"bufferView\":1,\"byteOffset\":%d,\"componentType\":5126,\"count\":%d,\"type\":\"VEC3\"}",
                 ufirst[i] * 12, ucount[i]);
        gltf_add(&acc, ",{\"bufferView\":2,\"byteOffset\":%d,\"componentType\":5126,\"count\":%d,\"type\":\"VEC2\"}",
                 ufirst[i] * 8, ucount[i]);
        gltf_add(&acc, ",{\"bufferView\":3,\"byteOffset\":%d,\"componentType\":5121,\"count\":%d,\"type\":\"VEC4\"}",
                 ufirst[i] * 4, ucount[i]);
        gltf_add(&acc, ",{\"bufferView\":4,\"byteOffset\":%d,\"componentType\":5126,\"count\":%d,\"type\":\"VEC4\"}",
                 ufirst[i] * 16, ucount[i]);
        gltf_add(&acc, ",{\"bufferView\":5,\"byteOffset\":%d,\"componentType\":5125,\"count\":%d,\"type\":\"SCALAR\"}",
                 m->d[i].first * 4, m->d[i].count);
        next += 6;
        {
            int mo = morph_of(m, d);
            size_t at = 0;

            for (k = 0; k < mo; k++) {
                at += (size_t)(m->morphs[k].nshapes - 1) * (size_t)mcount[k];
            }
            for (k = 1; mo >= 0 && k < m->morphs[mo].nshapes; k++) {   /* (its shapes: where each moves its vertices) */
                const float *dp = mpos[mo] + (size_t)(k - 1) * (size_t)mcount[mo] * 3;
                float slo[3] = {1e30f, 1e30f, 1e30f}, shi[3] = {-1e30f, -1e30f, -1e30f};
                int q, c;

                for (q = 0; q < mcount[mo]; q++) {
                    for (c = 0; c < 3; c++) {
                        slo[c] = dp[q * 3 + c] < slo[c] ? dp[q * 3 + c] : slo[c];
                        shi[c] = dp[q * 3 + c] > shi[c] ? dp[q * 3 + c] : shi[c];
                    }
                }
                gltf_add(&acc, ",{\"bufferView\":7,\"byteOffset\":%zu,\"componentType\":5126,\"count\":%d,\"type\":\"VEC3\","
                               "\"min\":[%.9g,%.9g,%.9g],\"max\":[%.9g,%.9g,%.9g]}",
                         (at + (size_t)(k - 1) * (size_t)mcount[mo]) * 12, mcount[mo], slo[0], slo[1], slo[2], shi[0], shi[1],
                         shi[2]);
                gltf_add(&acc, ",{\"bufferView\":8,\"byteOffset\":%zu,\"componentType\":5126,\"count\":%d,\"type\":\"VEC3\"}",
                         (at + (size_t)(k - 1) * (size_t)mcount[mo]) * 12, mcount[mo]);
                next += 2;
            }
        }
    }
    gltf_add(&acc, "%s{\"bufferView\":6,\"componentType\":5126,\"count\":%d,\"type\":\"MAT4\"}", next ? "," : "",
             m->nbones);
    /* the bones: nodes at rest, each with its children */
    for (i = 0; i < m->nbones; i++) {
        const ModelBone *bn = &m->bones[i];
        float q[4];
        int first = 1;

        rest_quat(bn->rest_rot, q);
        gltf_add(&nodes, "%s{\"name\":\"bone %d\",\"translation\":[%.9g,%.9g,%.9g],\"rotation\":[%.9g,%.9g,%.9g,%.9g]",
                 i ? "," : "", i, bn->rest_pos[0], bn->rest_pos[1], bn->rest_pos[2], q[0], q[1], q[2], q[3]);
        for (k = 0; k < m->nbones; k++) {
            if (m->bones[k].parent == i) {
                gltf_add(&nodes, "%s%d", first ? ",\"children\":[" : ",", k);
                first = 0;
            }
        }
        gltf_add(&nodes, "%s,\"extras\":{\"hga\":\"bone %d %.9g %.9g %.9g %.9g %.9g %.9g\"}}", first ? "" : "]",
                 bn->parent, bn->rest_rot[0], bn->rest_rot[1], bn->rest_rot[2], bn->rest_pos[0], bn->rest_pos[1],
                 bn->rest_pos[2]);
    }
    gltf_add(&nodes, ",{\"name\":\"model\",\"mesh\":0,\"skin\":0}");
    for (i = 0; i < m->nmorphs; i++) {   /* (the faces and hands: meshes 1.., in their draws' order) */
        gltf_add(&nodes, ",{\"name\":\"%s %d\",\"mesh\":%d,\"skin\":0}", m->morphs[i].face ? "face" : "hand", i, i + 1);
    }
    gltf_add(&o, "{\"asset\":{\"version\":\"2.0\",\"generator\":\"OpenHuey (the game library's model)\"},\"scene\":0,"
                 "\"scenes\":[{\"name\":\"model\",\"nodes\":[");
    for (i = 0, k = 0; i < m->nbones; i++) {
        if (m->bones[i].parent < 0 || m->bones[i].parent >= m->nbones) {
            gltf_add(&o, "%s%d", k++ ? "," : "", i);
        }
    }
    gltf_add(&o, "%s%d", k ? "," : "", m->nbones);
    for (i = 0; i < m->nmorphs; i++) {
        gltf_add(&o, ",%d", m->nbones + 1 + i);
    }
    gltf_add(&o, "],\"extras\":{\"hga\":\"model %d %d %d ", MODELGLTF_FORMAT, m->nbones, m->nv);
    for (i = 0; i < 256; i++) {
        gltf_add(&o, "%02x", m->bone_table != NULL ? m->bone_table[i] : 0);
    }
    gltf_add(&o, "%s\"}}],", m->bone_table != NULL ? "" : " -");
    gltf_add(&o, "\"nodes\":[%s],\"meshes\":[{\"name\":\"model\",\"primitives\":[%s]}%s],", nodes.s, prims.s ? prims.s : "",
             mmesh.s ? mmesh.s : "");
    gltf_add(&o, "\"skins\":[{\"inverseBindMatrices\":%d,\"joints\":[", next);
    for (i = 0; i < m->nbones; i++) {
        gltf_add(&o, "%s%d", i ? "," : "", i);
    }
    gltf_add(&o, "]}],\"accessors\":[%s],", acc.s);
    if (nmats > 0) {
        gltf_add(&o, "\"materials\":[%s],\"textures\":[", mats.s);
        for (i = 0; i <= maxtex; i++) {
            gltf_add(&o, "%s{\"source\":%d}", i ? "," : "", i);
        }
        gltf_add(&o, "],\"images\":[");
        for (i = 0; i <= maxtex; i++) {
            gltf_add(&o, "%s{\"uri\":\"%s%d.png\"}", i ? "," : "", textures, i);
        }
        gltf_add(&o, "],");
    }
    {
        const uint32_t zero = 0;
        size_t empty = gltf_put(&b, &zero, 4);
        const size_t starts[9] = {vat[0], vat[1], vat[2], vat[3], vat[4], iat, mat_at, sat[0], sat[1]};
        const size_t lens[9] = {(size_t)nu * 12, (size_t)nu * 12, (size_t)nu * 8, (size_t)nu * 4, (size_t)nu * 16,
                                (size_t)m->nv * 4, (size_t)m->nbones * 64, slen[0], slen[1]};

        gltf_add(&o, "\"bufferViews\":[");
        for (i = 0; i < 9; i++) {
            gltf_add(&o, "%s{\"buffer\":0,\"byteOffset\":%zu,\"byteLength\":%zu}", i ? "," : "", lens[i] ? starts[i] : empty,
                     lens[i] ? lens[i] : 4);
        }
    }
    gltf_add(&o, "],\"buffers\":[{\"uri\":\"%s\",\"byteLength\":%zu}]}\n", bin_name, b.n);
    free(acc.s);
    free(prims.s);
    free(mmesh.s);
    for (i = 0; i < m->nmorphs; i++) {
        free(mpos[i]);
        free(mnrm[i]);
    }
    free(mpos);
    free(mnrm);
    free(mcount);
    free(nodes.s);
    free(mats.s);
    free(u);
    free(idx);
    free(ufirst);
    free(ucount);
    free(matkey);
    *bin_out = b.b;
    *bin_size = b.n;
    *len = o.n;
    return o.s;
}

/* ---- reading ---- */

static int draw_from(const char *w, MeshDraw *d) {
    int v[10], i;

    if (w == NULL || sscanf(w, " %d %d %d %d %d %d %d %d %d %d", &v[0], &v[1], &v[2], &v[3], &v[4], &v[5], &v[6], &v[7],
                            &v[8], &v[9]) != 10) {
        return 0;
    }
    d->first = v[9];
    for (i = 0; i < 8; i++) {
        (&d->part)[i] = (uint8_t)v[i];   /* (part, blend, additive, no_zwrite, group, solid_tex, lit, mask) */
    }
    d->texture = v[8];
    return 1;
}

/* a primitive's draw and its vertices into the model's (its words say where they go) */
static int read_draw(Model *m, const cgltf_primitive *pr, MeshDraw *out) {
    const cgltf_accessor *ap = gltf_attribute(pr, "POSITION"), *an = gltf_attribute(pr, "NORMAL"),
                         *at = gltf_attribute(pr, "TEXCOORD_0"), *aj = gltf_attribute(pr, "JOINTS_0"),
                         *aw = gltf_attribute(pr, "WEIGHTS_0");
    const uint8_t *bp, *bn, *bt, *bj, *bw;
    MeshDraw d;
    size_t k;

    memset(&d, 0, sizeof(d));
    if (!draw_from(gltf_hga(&pr->extras, "draw"), &d) || ap == NULL || an == NULL || at == NULL || aj == NULL ||
        aw == NULL || pr->indices == NULL || aj->component_type != cgltf_component_type_r_8u ||
        (bp = gltf_bytes(ap, 12)) == NULL || (bn = gltf_bytes(an, 12)) == NULL || (bt = gltf_bytes(at, 8)) == NULL ||
        (bj = gltf_bytes(aj, 4)) == NULL || (bw = gltf_bytes(aw, 16)) == NULL) {
        return 0;
    }
    d.count = (int)pr->indices->count;
    if (d.first < 0 || d.count <= 0 || d.first + d.count > m->nv) {
        return 0;
    }
    for (k = 0; k < pr->indices->count; k++) {
        size_t e = cgltf_accessor_read_index(pr->indices, k);
        SkinVertex *v = &m->v[d.first + (int)k];

        if (e >= ap->count || e >= an->count || e >= at->count || e >= aj->count || e >= aw->count) {
            return 0;
        }
        memset(v, 0, sizeof(*v));
        memcpy(&v->pos, bp + e * 12, 12);
        memcpy(&v->normal, bn + e * 12, 12);
        memcpy(&v->s, bt + e * 8, 4);
        memcpy(&v->t, bt + e * 8 + 4, 4);
        memcpy(v->bones, bj + e * 4, 4);
        memcpy(v->weights, bw + e * 16, 16);
    }
    *out = d;
    return 1;
}

/* a face's or a hand's mesh: its draw where its words say (among `total`), its shapes */
static int read_morph(Model *m, const cgltf_primitive *pr, int total) {
    const char *w = gltf_hga(&pr->extras, "draw"), *mw = w != NULL ? strstr(w, " morph ") : NULL;
    int face, hand, nshapes, at, s;
    ModelMorph *mo;

    if (mw == NULL || sscanf(mw, " morph %d %d %d %d", &face, &hand, &nshapes, &at) != 4 || at < 0 || at >= total ||
        m->d[at].count > 0 || nshapes < 1 || nshapes > 64 || (int)pr->targets_count != nshapes - 1 ||
        !read_draw(m, pr, &m->d[at])) {
        return 0;
    }
    m->morphs = realloc(m->morphs, (size_t)(m->nmorphs + 1) * sizeof(ModelMorph));
    mo = &m->morphs[m->nmorphs++];
    mo->first = m->d[at].first;
    mo->count = m->d[at].count;
    mo->face = face;
    mo->hand = hand;
    mo->nshapes = nshapes;
    mo->dpos = calloc((size_t)nshapes * (size_t)mo->count, sizeof(Vec3));
    mo->dnrm = calloc((size_t)nshapes * (size_t)mo->count, sizeof(Vec3));
    for (s = 1; s < nshapes; s++) {
        const cgltf_morph_target *t = &pr->targets[s - 1];
        const cgltf_accessor *tp = NULL, *tn = NULL;
        const uint8_t *bp, *bn;
        size_t a, k;

        for (a = 0; a < t->attributes_count; a++) {
            if (t->attributes[a].type == cgltf_attribute_type_position) {
                tp = t->attributes[a].data;
            } else if (t->attributes[a].type == cgltf_attribute_type_normal) {
                tn = t->attributes[a].data;
            }
        }
        if (tp == NULL || tn == NULL || (bp = gltf_bytes(tp, 12)) == NULL || (bn = gltf_bytes(tn, 12)) == NULL) {
            return 0;
        }
        for (k = 0; k < pr->indices->count; k++) {   /* (by the indices: a vertex's shapes are its own) */
            size_t e = cgltf_accessor_read_index(pr->indices, k);

            if (e >= tp->count || e >= tn->count) {
                return 0;
            }
            memcpy(&mo->dpos[s * mo->count + (int)k], bp + e * 12, 12);
            memcpy(&mo->dnrm[s * mo->count + (int)k], bn + e * 12, 12);
        }
    }
    return 1;
}

int modelgltf_read(Model *m, const char *json, size_t len, const uint8_t *bin, size_t bin_size, uint8_t *motions,
                   size_t motions_size) {
    cgltf_options opts;
    cgltf_data *g = NULL;
    const char *w;
    int format = 0, nbones = 0, nv = 0, ok = 0, i, used = 0;
    size_t k, p;
    uint8_t *own;

    memset(m, 0, sizeof(*m));
    memset(&opts, 0, sizeof(opts));
    /* the model's own bytes: the bone table, then the motion bank (as model_build points into its .PCK) */
    own = malloc(256 + motions_size + 1);
    if (own == NULL || cgltf_parse(&opts, json, len, &g) != cgltf_result_success || g->buffers_count != 1 ||
        g->buffers[0].size > bin_size || g->scenes_count < 1 || g->meshes_count < 1 || g->skins_count != 1) {
        free(own);
        free(motions);
        cgltf_free(g);
        return 0;
    }
    g->buffers[0].data = (void *)bin;
    g->buffers[0].data_free_method = cgltf_data_free_method_none;
    if (motions != NULL) {
        memcpy(own + 256, motions, motions_size);
    }
    free(motions);
    m->pck = own;
    m->size = 256 + motions_size;
    if ((w = gltf_hga(&g->scenes[0].extras, "model")) == NULL || sscanf(w, " %d %d %d %n", &format, &nbones, &nv, &used) != 3 ||
        format < 1 || format > MODELGLTF_FORMAT || nv < 0 || nbones <= 0 || nbones > MODEL_MAX_BONES || (size_t)nbones > g->nodes_count) {
        goto done;
    }
    w += used;
    if (strncmp(w, "-", 1) != 0) {
        for (i = 0; i < 256; i++) {
            unsigned x;

            if (sscanf(w + i * 2, "%2x", &x) != 1) {
                goto done;
            }
            own[i] = (uint8_t)x;
        }
        m->bone_table = own;
    }
    m->nbones = nbones;
    for (i = 0; i < nbones; i++) {
        ModelBone *bn = &m->bones[i];

        if ((w = gltf_hga(&g->nodes[i].extras, "bone")) == NULL ||
            sscanf(w, " %d %f %f %f %f %f %f", &bn->parent, &bn->rest_rot[0], &bn->rest_rot[1], &bn->rest_rot[2],
                   &bn->rest_pos[0], &bn->rest_pos[1], &bn->rest_pos[2]) != 7) {
            goto done;
        }
    }
    {
        const cgltf_accessor *ib = g->skins[0].inverse_bind_matrices;
        const uint8_t *b;

        if (ib == NULL || ib->count != (size_t)nbones || ib->type != cgltf_type_mat4 || (b = gltf_bytes(ib, 64)) == NULL) {
            goto done;
        }
        for (i = 0; i < nbones; i++) {
            memcpy(m->bones[i].inv_bind.m, b + i * 64, 64);
        }
    }
    m->v = calloc((size_t)nv + 1, sizeof(SkinVertex));
    m->nv = nv;
    {   /* the body's draws in turn (mesh 0); the faces' and hands' (meshes 1.., format 2) where
         * their words say they go among them */
        int total = 0, body = 0;

        for (k = 0; k < g->meshes_count; k++) {
            total += (int)g->meshes[k].primitives_count;
        }
        m->d = calloc((size_t)total + 1, sizeof(MeshDraw));
        m->nd = total;
        for (k = 1; k < g->meshes_count; k++) {
            if (g->meshes[k].primitives_count != 1 || !read_morph(m, &g->meshes[k].primitives[0], total)) {
                goto done;
            }
        }
        for (p = 0; p < g->meshes[0].primitives_count; p++) {
            while (body < total && m->d[body].count > 0) {   /* (the places the morphs took) */
                body++;
            }
            if (body >= total || !read_draw(m, &g->meshes[0].primitives[p], &m->d[body])) {
                goto done;
            }
        }
    }
    if (motions_size > 0) {
        m->motions = own + 256;
        m->motions_size = motions_size;
    }
    ok = 1;
done:
    cgltf_free(g);
    if (!ok) {
        model_free(m);
    }
    return ok;
}
