/* A room's mesh as glTF: see roomgltf.h. Written by hand; read with cgltf. */
#include "roomgltf.h"

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gltfio.h"

/* ---- writing ---- */

/* the arrays of a run of vertices: positions, texture coordinates, colours (1.0 = 0x80) */
static void vertices(GltfBin *b, const MeshVertex *v, int n, size_t *at) {
    float *pos = malloc(sizeof(float) * 3 * (size_t)(n + 1)), *st = malloc(sizeof(float) * 2 * (size_t)(n + 1)),
          *col = malloc(sizeof(float) * 4 * (size_t)(n + 1));
    int i, c;

    for (i = 0; i < n; i++) {
        pos[i * 3] = v[i].x;
        pos[i * 3 + 1] = v[i].y;
        pos[i * 3 + 2] = v[i].z;
        st[i * 2] = v[i].s;
        st[i * 2 + 1] = v[i].t;
        for (c = 0; c < 4; c++) {
            col[i * 4 + c] = v[i].rgba[c] / 128.0f;
        }
    }
    at[0] = gltf_put(b, pos, sizeof(float) * 3 * (size_t)n);
    at[1] = gltf_put(b, st, sizeof(float) * 2 * (size_t)n);
    at[2] = gltf_put(b, col, sizeof(float) * 4 * (size_t)n);
    free(pos);
    free(st);
    free(col);
}

/* a run's three accessors (vertices first .. first + count of the views at[]), their indices from
 * *next; the position's bounds as glTF wants them */
static void accessors(GltfText *acc, int *next, const int *views, const MeshVertex *v, int first, int count) {
    float lo[3] = {1e30f, 1e30f, 1e30f}, hi[3] = {-1e30f, -1e30f, -1e30f};
    int i;

    for (i = first; i < first + count; i++) {
        const float p[3] = {v[i].x, v[i].y, v[i].z};
        int c;

        for (c = 0; c < 3; c++) {
            lo[c] = p[c] < lo[c] ? p[c] : lo[c];
            hi[c] = p[c] > hi[c] ? p[c] : hi[c];
        }
    }
    gltf_add(acc, "%s{\"bufferView\":%d,\"byteOffset\":%d,\"componentType\":5126,\"count\":%d,\"type\":\"VEC3\","
             "\"min\":[%.9g,%.9g,%.9g],\"max\":[%.9g,%.9g,%.9g]}",
        *next ? "," : "", views[0], first * 12, count, lo[0], lo[1], lo[2], hi[0], hi[1], hi[2]);
    gltf_add(acc, ",{\"bufferView\":%d,\"byteOffset\":%d,\"componentType\":5126,\"count\":%d,\"type\":\"VEC2\"}", views[1],
        first * 8, count);
    gltf_add(acc, ",{\"bufferView\":%d,\"byteOffset\":%d,\"componentType\":5126,\"count\":%d,\"type\":\"VEC4\"}", views[2],
        first * 16, count);
    *next += 3;
}

static void draw_words(GltfText *o, const MeshDraw *d) {
    gltf_add(o, "%d %d %d %d %d %d %d %d %d", d->part, d->blend, d->additive, d->no_zwrite, d->group, d->solid_tex, d->lit,
        d->mask, d->texture);
}

/* a draw's material: its texture, blended or not (one each, made as met) */
static int material(GltfText *mat, int *mats, int *nmats, const MeshDraw *d) {
    int i, key = d->texture * 2 + (d->blend ? 1 : 0);

    if (d->texture < 0) {
        return -1;
    }
    for (i = 0; i < *nmats && mats[i] != key; i++) {
    }
    if (i == *nmats) {
        mats[(*nmats)++] = key;
        gltf_add(mat, "%s{\"name\":\"texture %d%s\",\"pbrMetallicRoughness\":{\"baseColorTexture\":{\"index\":%d},"
                 "\"metallicFactor\":0,\"roughnessFactor\":1},\"alphaMode\":\"%s\",\"doubleSided\":true}",
            i ? "," : "", d->texture, d->blend ? ", blended" : "", d->texture, d->blend ? "BLEND" : "MASK");
    }
    return i;
}

char *roomgltf_write(const RoomMesh *m, const char *bin_name, uint8_t **bin_out, size_t *bin_size, size_t *len) {
    GltfText o = {0}, acc = {0}, mat = {0}, meshes = {0}, nodes = {0};
    GltfBin b = {0};
    size_t at[3], dat[3], notri = 0, iat = 0;
    MeshVertex *u = malloc(sizeof(MeshVertex) * (size_t)(m->nv + 1));
    uint32_t *idx = malloc(sizeof(uint32_t) * (size_t)(m->nv + 1));
    int *ufirst = malloc(sizeof(int) * (size_t)(m->nd + 1)), *ucount = malloc(sizeof(int) * (size_t)(m->nd + 1)), nu = 0;
    int isize;
    int i, next = 0, nmats = 0, *mats = malloc(sizeof(int) * 2 * (size_t)(m->nd + m->ndyn + 1)), maxtex = -1;
    const int sviews[3] = {0, 1, 2}, dviews[3] = {3, 4, 5};
    uint8_t *flags;

    for (i = 0; i < m->nd; i++) {   /* (each draw indexed: its distinct vertices, a run of u) */
        ufirst[i] = nu;
        gltf_distinct(m->v + m->d[i].first, m->d[i].count, sizeof(MeshVertex), u, &nu, idx + m->d[i].first);
        ucount[i] = nu - ufirst[i];
    }
    for (i = 0, isize = 2; i < m->nd; i++) {   /* (16-bit indices when every draw's fit) */
        isize = ucount[i] > 65535 ? 4 : isize;
    }
    vertices(&b, u, nu, at);
    vertices(&b, m->dv, m->ndv, dat);
    if (isize == 2) {
        uint16_t *small = malloc(sizeof(uint16_t) * (size_t)(m->nv + 1));

        for (i = 0; i < m->nv; i++) {
            small[i] = (uint16_t)idx[i];
        }
        iat = gltf_put(&b, small, sizeof(uint16_t) * (size_t)m->nv);
        free(small);
    } else {
        iat = gltf_put(&b, idx, sizeof(uint32_t) * (size_t)m->nv);
    }
    flags = calloc(1, (size_t)m->ndv + 1);
    for (i = 0; i < m->ndyn; i++) {   /* (each batch's strip breaks, in its run of dv) */
        memcpy(flags + m->dyn[i].draw.first, m->dyn[i].no_tri, (size_t)m->dyn[i].draw.count);
    }
    notri = gltf_put(&b, flags, (size_t)m->ndv);
    free(flags);
    /* the room's still triangles: mesh 0, a primitive a draw */
    gltf_add(&meshes, "{\"name\":\"room\",\"primitives\":[");
    for (i = 0; i < m->nd; i++) {
        const MeshDraw *d = &m->d[i];
        int mt = material(&mat, mats, &nmats, d);

        maxtex = d->texture > maxtex ? d->texture : maxtex;
        gltf_add(&meshes, "%s{\"attributes\":{\"POSITION\":%d,\"TEXCOORD_0\":%d,\"COLOR_0\":%d},\"indices\":%d,\"mode\":4",
            i ? "," : "", next, next + 1, next + 2, next + 3);
        accessors(&acc, &next, sviews, u, ufirst[i], ucount[i]);
        gltf_add(&acc, ",{\"bufferView\":7,\"byteOffset\":%d,\"componentType\":%d,\"count\":%d,\"type\":\"SCALAR\"}",
            d->first * isize, isize == 2 ? 5123 : 5125, d->count);
        next++;
        if (mt >= 0) {
            gltf_add(&meshes, ",\"material\":%d", mt);
        }
        gltf_add(&meshes, ",\"extras\":{\"hga\":\"draw ");
        draw_words(&meshes, d);
        gltf_add(&meshes, "\"}}");
    }
    gltf_add(&meshes, "]}");
    gltf_add(&nodes, "{\"name\":\"room\",\"mesh\":0}");
    /* the moving batches: a node and a mesh each */
    for (i = 0; i < m->ndyn; i++) {
        const DynBatch *y = &m->dyn[i];
        int mt = material(&mat, mats, &nmats, &y->draw), k;

        maxtex = y->draw.texture > maxtex ? y->draw.texture : maxtex;
        gltf_add(&meshes, ",{\"name\":\"batch %d\",\"primitives\":[{\"attributes\":{\"POSITION\":%d,\"TEXCOORD_0\":%d,"
                     "\"COLOR_0\":%d,\"_NO_TRI\":%d},\"mode\":5",
            i, next, next + 1, next + 2, next + 3);
        accessors(&acc, &next, dviews, m->dv, y->draw.first, y->draw.count);
        gltf_add(&acc, ",{\"bufferView\":6,\"byteOffset\":%d,\"componentType\":5121,\"count\":%d,\"type\":\"SCALAR\"}",
            y->draw.first, y->draw.count);
        next++;
        if (mt >= 0) {
            gltf_add(&meshes, ",\"material\":%d", mt);
        }
        gltf_add(&meshes, "}]}");
        gltf_add(&nodes, ",{\"name\":\"batch %d\",\"mesh\":%d,\"matrix\":[", i, i + 1);
        for (k = 0; k < 16; k++) {
            gltf_add(&nodes, "%s%.9g", k ? "," : "", y->local.m[k]);
        }
        gltf_add(&nodes, "],\"extras\":{\"hga\":\"batch ");
        draw_words(&nodes, &y->draw);
        gltf_add(&nodes, " %d %d %d %d %d %d %d %.9g %.9g\"}}", y->parallax, y->billboard, y->flip_type, y->frames,
            y->frame_ticks, y->frame, y->timer, y->frame_w, y->frame_h);
    }
    gltf_add(&o, "{\"asset\":{\"version\":\"2.0\",\"generator\":\"OpenHuey (the game library's room mesh)\"},\"scene\":0,"
            "\"scenes\":[{\"name\":\"room\",\"nodes\":[");
    for (i = 0; i <= m->ndyn; i++) {
        gltf_add(&o, "%s%d", i ? "," : "", i);
    }
    gltf_add(&o, "],\"extras\":{\"hga\":\"room %d %.9g %.9g %.9g %.9g %.9g %.9g\"}}],", ROOMGLTF_FORMAT, m->lo.x, m->lo.y,
        m->lo.z, m->hi.x, m->hi.y, m->hi.z);
    gltf_add(&o, "\"nodes\":[%s],\"meshes\":[%s],", nodes.s, meshes.s);
    if (next > 0) {
        gltf_add(&o, "\"accessors\":[%s],", acc.s);
    }
    if (nmats > 0) {
        gltf_add(&o, "\"materials\":[%s],\"textures\":[", mat.s);
        for (i = 0; i <= maxtex; i++) {
            gltf_add(&o, "%s{\"source\":%d}", i ? "," : "", i);
        }
        gltf_add(&o, "],\"images\":[");
        for (i = 0; i <= maxtex; i++) {
            gltf_add(&o, "%s{\"uri\":\"textures.texbank/%d.png\"}", i ? "," : "", i);
        }
        gltf_add(&o, "],");
    }
    gltf_add(&o, "\"bufferViews\":[");
    {
        const size_t starts[8] = {at[0], at[1], at[2], dat[0], dat[1], dat[2], notri, iat};
        const size_t lens[8] = {(size_t)nu * 12, (size_t)nu * 8, (size_t)nu * 16, (size_t)m->ndv * 12,
                                (size_t)m->ndv * 8, (size_t)m->ndv * 16, (size_t)m->ndv, (size_t)m->nv * (size_t)isize};

        const uint32_t zero = 0;
        size_t empty = gltf_put(&b, &zero, 4);   /* (a view with nothing in it: glTF wants a length all the same) */

        for (i = 0; i < 8; i++) {
            gltf_add(&o, "%s{\"buffer\":0,\"byteOffset\":%zu,\"byteLength\":%zu}", i ? "," : "", lens[i] > 0 ? starts[i] : empty,
                lens[i] > 0 ? lens[i] : 4);
        }
    }
    gltf_add(&o, "],\"buffers\":[{\"uri\":\"%s\",\"byteLength\":%zu}]}\n", bin_name, b.n);
    free(acc.s);
    free(mat.s);
    free(meshes.s);
    free(nodes.s);
    free(mats);
    free(u);
    free(idx);
    free(ufirst);
    free(ucount);
    *bin_out = b.b;
    *bin_size = b.n;
    *len = o.n;
    return o.s;
}

/* ---- reading ---- */

/* a primitive's vertices onto the end of v (*n of them so far), its indices followed if it has them */
static int read_vertices(const cgltf_primitive *p, MeshVertex **v, int *n, int *first, int *count) {
    const cgltf_accessor *ap = gltf_attribute(p, "POSITION"), *at = gltf_attribute(p, "TEXCOORD_0"), *ac = gltf_attribute(p, "COLOR_0");
    const uint8_t *pp, *pt, *pc;
    size_t i, k, total;

    if (ap == NULL || at == NULL || ac == NULL || ap->count != at->count || ap->count != ac->count ||
        ap->type != cgltf_type_vec3 || at->type != cgltf_type_vec2 || ac->type != cgltf_type_vec4 ||
        ap->component_type != cgltf_component_type_r_32f || at->component_type != cgltf_component_type_r_32f ||
        ac->component_type != cgltf_component_type_r_32f || (pp = gltf_bytes(ap, 12)) == NULL ||
        (pt = gltf_bytes(at, 8)) == NULL || (pc = gltf_bytes(ac, 16)) == NULL) {
        return 0;
    }
    total = p->indices != NULL ? p->indices->count : ap->count;
    *v = realloc(*v, sizeof(MeshVertex) * (*n + total + 1));
    *first = *n;
    *count = (int)total;
    for (i = 0; i < total; i++) {
        MeshVertex *o = &(*v)[(*n)++];
        float f[4];
        size_t e = i;

        if (p->indices != NULL && ((e = cgltf_accessor_read_index(p->indices, i)) >= ap->count)) {
            return 0;
        }

        memset(o, 0, sizeof(*o));
        memcpy(f, pp + e * 12, 12);
        o->x = f[0];
        o->y = f[1];
        o->z = f[2];
        memcpy(f, pt + e * 8, 8);
        o->s = f[0];
        o->t = f[1];
        memcpy(f, pc + e * 16, 16);
        for (k = 0; k < 4; k++) {   /* (1.0: the PS2's 0x80) */
            long c = lrintf(f[k] * 128.0f);

            o->rgba[k] = (uint8_t)(c < 0 ? 0 : c > 255 ? 255 : c);
        }
    }
    return 1;
}

static int draw_from(const char *w, MeshDraw *d) {
    int part, blend, add_, nz, group, solid, lit, mask, tex, used = 0;

    if (w == NULL || sscanf(w, " %d %d %d %d %d %d %d %d %d%n", &part, &blend, &add_, &nz, &group, &solid, &lit, &mask,
                            &tex, &used) != 9) {
        return 0;
    }
    d->part = (uint8_t)part;
    d->blend = (uint8_t)blend;
    d->additive = (uint8_t)add_;
    d->no_zwrite = (uint8_t)nz;
    d->group = (uint8_t)group;
    d->solid_tex = (uint8_t)solid;
    d->lit = (uint8_t)lit;
    d->mask = (uint8_t)mask;
    d->texture = tex;
    return used;
}

int roomgltf_read(RoomMesh *m, const char *json, size_t len, const uint8_t *bin, size_t bin_size) {
    cgltf_options opts;
    cgltf_data *g = NULL;
    const char *w;
    size_t i;
    int format = 0, ok = 0, capd = 0, capdyn = 0;

    memset(m, 0, sizeof(*m));
    memset(&opts, 0, sizeof(opts));
    if (cgltf_parse(&opts, json, len, &g) != cgltf_result_success || g->buffers_count != 1 ||
        g->buffers[0].size > bin_size || g->scenes_count < 1 || g->nodes_count < 1) {
        cgltf_free(g);
        return 0;
    }
    g->buffers[0].data = (void *)bin;   /* (ours: not freed by cgltf) */
    g->buffers[0].data_free_method = cgltf_data_free_method_none;
    if ((w = gltf_hga(&g->scenes[0].extras, "room")) == NULL ||
        sscanf(w, " %d %f %f %f %f %f %f", &format, &m->lo.x, &m->lo.y, &m->lo.z, &m->hi.x, &m->hi.y, &m->hi.z) != 7 ||
        format != ROOMGLTF_FORMAT) {
        goto done;
    }
    for (i = 0; i < g->nodes_count; i++) {
        const cgltf_node *n = &g->nodes[i];
        const cgltf_primitive *p;
        size_t k;

        if (n->mesh == NULL) {
            continue;
        }
        if (i == 0) {   /* the still triangles: a draw each */
            for (k = 0; k < n->mesh->primitives_count; k++) {
                MeshDraw d;

                memset(&d, 0, sizeof(d));
                p = &n->mesh->primitives[k];
                if (draw_from(gltf_hga(&p->extras, "draw"), &d) == 0 || !read_vertices(p, &m->v, &m->nv, &d.first, &d.count)) {
                    goto done;
                }
                if (m->nd == capd) {
                    capd = capd * 2 + 64;
                    m->d = realloc(m->d, sizeof(MeshDraw) * (size_t)capd);
                }
                m->d[m->nd++] = d;
            }
            continue;
        }
        {   /* a moving batch */
            DynBatch y;
            const cgltf_accessor *an;
            const uint8_t *pn;
            int used, par, bb, ft;

            memset(&y, 0, sizeof(y));
            p = &n->mesh->primitives[0];
            w = gltf_hga(&n->extras, "batch");
            if (n->mesh->primitives_count != 1 || !n->has_matrix || (used = draw_from(w, &y.draw)) == 0 ||
                sscanf(w + used, " %d %d %d %d %d %d %d %f %f", &par, &bb, &ft, &y.frames, &y.frame_ticks, &y.frame,
                       &y.timer, &y.frame_w, &y.frame_h) != 9 ||
                !read_vertices(p, &m->dv, &m->ndv, &y.draw.first, &y.draw.count) ||
                (an = gltf_attribute(p, "_NO_TRI")) == NULL || an->count != (size_t)y.draw.count ||
                an->component_type != cgltf_component_type_r_8u || (pn = gltf_bytes(an, 1)) == NULL) {
                goto done;
            }
            y.parallax = (uint8_t)par;
            y.billboard = (uint8_t)bb;
            y.flip_type = (uint8_t)ft;
            memcpy(y.local.m, n->matrix, sizeof(y.local.m));
            y.no_tri = malloc((size_t)y.draw.count + 1);
            memcpy(y.no_tri, pn, (size_t)y.draw.count);
            if (m->ndyn == capdyn) {
                capdyn = capdyn * 2 + 16;
                m->dyn = realloc(m->dyn, sizeof(DynBatch) * (size_t)capdyn);
            }
            m->dyn[m->ndyn++] = y;
        }
    }
    ok = 1;
done:
    cgltf_free(g);
    if (!ok) {
        roommesh_free(m);
    }
    return ok;
}
