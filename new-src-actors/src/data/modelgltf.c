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

static void draw_words(GltfText *o, const MeshDraw *d) {
    gltf_add(o, "%d %d %d %d %d %d %d %d %d", d->part, d->blend, d->additive, d->no_zwrite, d->group, d->solid_tex,
             d->lit, d->mask, d->texture);
}

char *modelgltf_write(const Model *m, const char *bin_name, const char *textures, uint8_t **bin_out, size_t *bin_size,
                      size_t *len) {
    GltfText o = {0}, acc = {0}, prims = {0}, nodes = {0}, mats = {0};
    GltfBin b = {0};
    SkinVertex *u = malloc(sizeof(SkinVertex) * (size_t)(m->nv + 1));
    uint32_t *idx = malloc(sizeof(uint32_t) * (size_t)(m->nv + 1));
    int *ufirst = malloc(sizeof(int) * (size_t)(m->nd + 1)), *ucount = malloc(sizeof(int) * (size_t)(m->nd + 1));
    int nu = 0, i, k, next = 0, maxtex = -1, *matkey = malloc(sizeof(int) * (size_t)(m->nd + 1)), nmats = 0;
    size_t vat[5], iat, mat_at;
    float *pos, *nrm, *st, *w;
    uint8_t *j;

    for (i = 0; i < m->nd; i++) {   /* (each draw indexed: its distinct vertices, a run of u) */
        ufirst[i] = nu;
        gltf_distinct(m->v + m->d[i].first, m->d[i].count, sizeof(SkinVertex), u, &nu, idx + m->d[i].first);
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
        gltf_add(&prims, "%s{\"attributes\":{\"POSITION\":%d,\"NORMAL\":%d,\"TEXCOORD_0\":%d,\"JOINTS_0\":%d,"
                         "\"WEIGHTS_0\":%d},\"indices\":%d,\"mode\":4",
                 i ? "," : "", next, next + 1, next + 2, next + 3, next + 4, next + 5);
        if (mt >= 0) {
            gltf_add(&prims, ",\"material\":%d", mt);
        }
        gltf_add(&prims, ",\"extras\":{\"hga\":\"draw ");
        draw_words(&prims, d);
        gltf_add(&prims, " %d\"}}", d->first);   /* (where its vertices go: the draws are in their passes' order) */
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
    gltf_add(&o, "{\"asset\":{\"version\":\"2.0\",\"generator\":\"hga (the game library's model)\"},\"scene\":0,"
                 "\"scenes\":[{\"name\":\"model\",\"nodes\":[");
    for (i = 0, k = 0; i < m->nbones; i++) {
        if (m->bones[i].parent < 0 || m->bones[i].parent >= m->nbones) {
            gltf_add(&o, "%s%d", k++ ? "," : "", i);
        }
    }
    gltf_add(&o, "%s%d],\"extras\":{\"hga\":\"model %d %d %d ", k ? "," : "", m->nbones, MODELGLTF_FORMAT, m->nbones, m->nv);
    for (i = 0; i < 256; i++) {
        gltf_add(&o, "%02x", m->bone_table != NULL ? m->bone_table[i] : 0);
    }
    gltf_add(&o, "%s\"}}],", m->bone_table != NULL ? "" : " -");
    gltf_add(&o, "\"nodes\":[%s],\"meshes\":[{\"name\":\"model\",\"primitives\":[%s]}],", nodes.s, prims.s ? prims.s : "");
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
        const size_t starts[7] = {vat[0], vat[1], vat[2], vat[3], vat[4], iat, mat_at};
        const size_t lens[7] = {(size_t)nu * 12, (size_t)nu * 12, (size_t)nu * 8, (size_t)nu * 4, (size_t)nu * 16,
                                (size_t)m->nv * 4, (size_t)m->nbones * 64};

        gltf_add(&o, "\"bufferViews\":[");
        for (i = 0; i < 7; i++) {
            gltf_add(&o, "%s{\"buffer\":0,\"byteOffset\":%zu,\"byteLength\":%zu}", i ? "," : "", lens[i] ? starts[i] : empty,
                     lens[i] ? lens[i] : 4);
        }
    }
    gltf_add(&o, "],\"buffers\":[{\"uri\":\"%s\",\"byteLength\":%zu}]}\n", bin_name, b.n);
    free(acc.s);
    free(prims.s);
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
        g->buffers[0].size > bin_size || g->scenes_count < 1 || g->meshes_count != 1 || g->skins_count != 1) {
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
        format != MODELGLTF_FORMAT || nv < 0 || nbones <= 0 || nbones > MODEL_MAX_BONES || (size_t)nbones > g->nodes_count) {
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
    for (p = 0; p < g->meshes[0].primitives_count; p++) {
        const cgltf_primitive *pr = &g->meshes[0].primitives[p];
        const cgltf_accessor *ap = gltf_attribute(pr, "POSITION"), *an = gltf_attribute(pr, "NORMAL"),
                             *at = gltf_attribute(pr, "TEXCOORD_0"), *aj = gltf_attribute(pr, "JOINTS_0"),
                             *aw = gltf_attribute(pr, "WEIGHTS_0");
        const uint8_t *bp, *bn, *bt, *bj, *bw;
        MeshDraw d;

        memset(&d, 0, sizeof(d));
        if (!draw_from(gltf_hga(&pr->extras, "draw"), &d) || ap == NULL || an == NULL || at == NULL || aj == NULL ||
            aw == NULL || pr->indices == NULL || aj->component_type != cgltf_component_type_r_8u ||
            (bp = gltf_bytes(ap, 12)) == NULL || (bn = gltf_bytes(an, 12)) == NULL || (bt = gltf_bytes(at, 8)) == NULL ||
            (bj = gltf_bytes(aj, 4)) == NULL || (bw = gltf_bytes(aw, 16)) == NULL) {
            goto done;
        }
        d.count = (int)pr->indices->count;
        if (d.first < 0 || d.first + d.count > nv) {
            goto done;
        }
        for (k = 0; k < pr->indices->count; k++) {
            size_t e = cgltf_accessor_read_index(pr->indices, k);
            SkinVertex *v = &m->v[d.first + (int)k];

            if (e >= ap->count || e >= an->count || e >= at->count || e >= aj->count || e >= aw->count) {
                goto done;
            }
            memset(v, 0, sizeof(*v));
            memcpy(&v->pos, bp + e * 12, 12);
            memcpy(&v->normal, bn + e * 12, 12);
            memcpy(&v->s, bt + e * 8, 4);
            memcpy(&v->t, bt + e * 8 + 4, 4);
            memcpy(v->bones, bj + e * 4, 4);
            memcpy(v->weights, bw + e * 16, 16);
        }
        m->d = realloc(m->d, sizeof(MeshDraw) * (size_t)(m->nd + 1));
        m->d[m->nd++] = d;
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
