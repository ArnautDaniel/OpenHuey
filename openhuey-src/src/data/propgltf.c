/* A prop: any glTF 2.0 model read into the engine's mesh form (propgltf.h). */
#include "propgltf.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../third_party/cgltf.h"
#include "../third_party/stb_image.h"

#define PROP_MAX_VERTICES (1 << 22)   /* (a prop, not a room: 4 million vertices at most) */

/* a buffer's or image's bytes by its URI: data: (base64) or a file beside the glTF */
static uint8_t *uri_bytes(const cgltf_options *o, const char *uri, size_t want, PropRead read, void *user, size_t *size) {
    if (strncmp(uri, "data:", 5) == 0) {
        const char *comma = strchr(uri, ',');
        void *out = NULL;

        if (comma != NULL && want == 0) {   /* (an image's: its size from the text, less the padding) */
            size_t len = strlen(comma + 1);

            want = len / 4 * 3 - (len >= 1 && comma[len] == '=') - (len >= 2 && comma[len - 1] == '=');
        }
        if (comma == NULL || strstr(uri, ";base64,") == NULL || want == 0 ||
            cgltf_load_buffer_base64(o, want, comma + 1, &out) != cgltf_result_success) {
            return NULL;
        }
        *size = want;
        return out;
    } else {
        char *rel = malloc(strlen(uri) + 1);
        uint8_t *b;

        if (rel == NULL) {
            return NULL;
        }
        strcpy(rel, uri);
        cgltf_decode_uri(rel);   /* ("my%20file.bin") */
        b = read(user, rel, size);
        free(rel);
        return b;
    }
}

static int load_buffers(cgltf_data *g, const cgltf_options *o, PropRead read, void *user) {
    cgltf_size i;

    for (i = 0; i < g->buffers_count; i++) {
        cgltf_buffer *b = &g->buffers[i];
        size_t size = 0;

        if (b->data != NULL) {
            continue;
        }
        if (b->uri == NULL) {   /* (a .glb's own chunk) */
            if (i != 0 || g->bin == NULL || g->bin_size < b->size) {
                return 0;
            }
            b->data = (void *)g->bin;
            b->data_free_method = cgltf_data_free_method_none;
            continue;
        }
        if ((b->data = uri_bytes(o, b->uri, b->size, read, user, &size)) == NULL || size < b->size) {
            fprintf(stderr, "prop: buffer %s missing or short\n", b->uri);
            return 0;
        }
        b->data_free_method = cgltf_data_free_method_memory_free;
    }
    return 1;
}

static int load_images(PropMesh *m, const cgltf_data *g, const cgltf_options *o, PropRead read, void *user) {
    cgltf_size i;

    if (g->images_count == 0) {
        return 1;
    }
    if ((m->images = calloc(g->images_count, sizeof(PropImage))) == NULL) {
        return 0;
    }
    m->nimages = (int)g->images_count;
    for (i = 0; i < g->images_count; i++) {
        const cgltf_image *im = &g->images[i];
        const uint8_t *bytes = NULL;
        uint8_t *owned = NULL;
        size_t size = 0;
        int n;

        if (im->buffer_view != NULL && im->buffer_view->buffer->data != NULL) {
            bytes = (const uint8_t *)im->buffer_view->buffer->data + im->buffer_view->offset;
            size = im->buffer_view->size;
        } else if (im->uri != NULL) {
            owned = uri_bytes(o, im->uri, 0, read, user, &size);
            bytes = owned;
        }
        if (bytes != NULL) {
            m->images[i].rgba = stbi_load_from_memory(bytes, (int)size, &m->images[i].w, &m->images[i].h, &n, 4);
        }
        if (m->images[i].rgba == NULL) {
            fprintf(stderr, "prop: image %d (%s) not read - untextured\n", (int)i, im->uri != NULL ? im->uri : "embedded");
        }
        free(owned);
    }
    return 1;
}

/* the normals' matrix: the inverse transpose of the world matrix's upper 3 x 3 (column-major in,
 * row-major out) */
static void normal_matrix(const float w[16], float n[9]) {
    float a = w[0], b = w[4], c = w[8], d = w[1], e = w[5], f = w[9], g = w[2], h = w[6], k = w[10];
    float det = a * (e * k - f * h) - b * (d * k - f * g) + c * (d * h - e * g);

    if (fabsf(det) < 1e-12f) {
        det = 1.0f;
    }
    /* (the inverse's transpose: the cofactors over the determinant) */
    n[0] = (e * k - f * h) / det;  n[1] = -(d * k - f * g) / det;  n[2] = (d * h - e * g) / det;
    n[3] = -(b * k - c * h) / det; n[4] = (a * k - c * g) / det;   n[5] = -(a * h - b * g) / det;
    n[6] = (b * f - c * e) / det;  n[7] = -(a * f - c * d) / det;  n[8] = (a * e - b * d) / det;
}

static uint8_t to_ps2(float c) {   /* (0x80 = 1.0) */
    float v = c * 128.0f + 0.5f;

    return (uint8_t)(v < 0.0f ? 0.0f : v > 255.0f ? 255.0f : v);
}

typedef struct Out {
    MeshVertex *v;
    int nv, cap;
    MeshDraw *d;
    int nd, dcap;
} Out;

static int grow(Out *o, int more) {
    if (o->nv + more > PROP_MAX_VERTICES) {
        return 0;
    }
    if (o->nv + more > o->cap) {
        int cap = o->cap ? o->cap : 1024;
        MeshVertex *v;

        while (cap < o->nv + more) {
            cap *= 2;
        }
        if ((v = realloc(o->v, (size_t)cap * sizeof(MeshVertex))) == NULL) {
            return 0;
        }
        o->v = v;
        o->cap = cap;
    }
    return 1;
}

static int add_draw(Out *o, MeshDraw d) {
    if (o->nd == o->dcap) {
        int cap = o->dcap ? o->dcap * 2 : 16;
        MeshDraw *n = realloc(o->d, (size_t)cap * sizeof(MeshDraw));

        if (n == NULL) {
            return 0;
        }
        o->d = n;
        o->dcap = cap;
    }
    o->d[o->nd++] = d;
    return 1;
}

static const cgltf_accessor *attribute(const cgltf_primitive *p, cgltf_attribute_type type) {
    cgltf_size i;

    for (i = 0; i < p->attributes_count; i++) {
        if (p->attributes[i].type == type && p->attributes[i].index == 0) {
            return p->attributes[i].data;
        }
    }
    return NULL;
}

static int primitive(Out *o, const cgltf_data *g, const cgltf_primitive *p, const float w[16]) {
    const cgltf_accessor *pos = attribute(p, cgltf_attribute_type_position);
    const cgltf_accessor *nrm = attribute(p, cgltf_attribute_type_normal);
    const cgltf_accessor *uv = attribute(p, cgltf_attribute_type_texcoord);
    const cgltf_accessor *col = attribute(p, cgltf_attribute_type_color);
    const cgltf_material *mat = p->material;
    float base[4] = {1, 1, 1, 1}, nm[9];
    cgltf_size count, i;
    MeshDraw d;
    int first = o->nv;

    if (p->type != cgltf_primitive_type_triangles || pos == NULL) {
        return 1;   /* (points, lines, strips: not a prop's) */
    }
    count = p->indices != NULL ? p->indices->count : pos->count;
    count -= count % 3;
    if (!grow(o, (int)count)) {
        fprintf(stderr, "prop: too many vertices\n");
        return 0;
    }
    memset(&d, 0, sizeof(d));
    d.texture = -1;
    d.part = MESH_SOLID;
    d.lit = 1;
    d.solid_tex = 1;
    if (mat != NULL && mat->has_pbr_metallic_roughness) {
        const cgltf_texture *t = mat->pbr_metallic_roughness.base_color_texture.texture;

        memcpy(base, mat->pbr_metallic_roughness.base_color_factor, sizeof(base));
        if (t != NULL && t->image != NULL) {
            d.texture = (int)(t->image - g->images);
        }
    }
    if (mat != NULL && mat->alpha_mode == cgltf_alpha_mode_blend) {
        d.blend = 1;
        d.solid_tex = 0;
    } else if (mat != NULL && mat->alpha_mode == cgltf_alpha_mode_mask) {
        d.solid_tex = 0;   /* (its texture's alpha cuts it) */
    }
    normal_matrix(w, nm);
    for (i = 0; i < count; i++) {
        cgltf_size k = p->indices != NULL ? cgltf_accessor_read_index(p->indices, i) : i;
        MeshVertex *v = &o->v[o->nv + (int)i];
        float a[4] = {0, 0, 0, 1}, c[4] = {1, 1, 1, 1};

        if (k >= pos->count) {
            fprintf(stderr, "prop: an index past its vertices\n");
            return 0;
        }
        cgltf_accessor_read_float(pos, k, a, 3);
        v->x = w[0] * a[0] + w[4] * a[1] + w[8] * a[2] + w[12];
        v->y = w[1] * a[0] + w[5] * a[1] + w[9] * a[2] + w[13];
        v->z = w[2] * a[0] + w[6] * a[1] + w[10] * a[2] + w[14];
        if (uv != NULL && k < uv->count) {
            cgltf_accessor_read_float(uv, k, a, 2);
            v->s = a[0];
            v->t = a[1];
        }
        if (col != NULL && k < col->count) {
            cgltf_accessor_read_float(col, k, c, cgltf_num_components(col->type) < 4 ? 3 : 4);
        }
        v->rgba[0] = to_ps2(c[0] * base[0]);
        v->rgba[1] = to_ps2(c[1] * base[1]);
        v->rgba[2] = to_ps2(c[2] * base[2]);
        v->rgba[3] = to_ps2(c[3] * base[3]);
        if (nrm != NULL && k < nrm->count) {
            float l;

            cgltf_accessor_read_float(nrm, k, a, 3);
            v->nx = nm[0] * a[0] + nm[1] * a[1] + nm[2] * a[2];   /* (nm row-major) */
            v->ny = nm[3] * a[0] + nm[4] * a[1] + nm[5] * a[2];
            v->nz = nm[6] * a[0] + nm[7] * a[1] + nm[8] * a[2];
            l = sqrtf(v->nx * v->nx + v->ny * v->ny + v->nz * v->nz);
            if (l > 0.0f) {
                v->nx /= l;  v->ny /= l;  v->nz /= l;
            }
        }
    }
    if (nrm == NULL) {   /* (none given: each triangle's own facing) */
        for (i = 0; i + 2 < count; i += 3) {
            MeshVertex *t = &o->v[o->nv + (int)i];
            float ux = t[1].x - t[0].x, uy = t[1].y - t[0].y, uz = t[1].z - t[0].z;
            float vx = t[2].x - t[0].x, vy = t[2].y - t[0].y, vz = t[2].z - t[0].z;
            float nx = uy * vz - uz * vy, ny = uz * vx - ux * vz, nz = ux * vy - uy * vx;
            float l = sqrtf(nx * nx + ny * ny + nz * nz);
            int j;

            if (l > 0.0f) {
                nx /= l;  ny /= l;  nz /= l;
            }
            for (j = 0; j < 3; j++) {
                t[j].nx = nx;  t[j].ny = ny;  t[j].nz = nz;
            }
        }
    }
    o->nv += (int)count;
    d.first = first;
    d.count = (int)count;
    return count == 0 || add_draw(o, d);
}

static int node(Out *o, const cgltf_data *g, const cgltf_node *n, int depth) {
    cgltf_size i;

    if (depth > 64) {   /* (a cycle, or nonsense) */
        return 0;
    }
    if (n->mesh != NULL) {
        float w[16];

        cgltf_node_transform_world(n, w);
        for (i = 0; i < n->mesh->primitives_count; i++) {
            if (!primitive(o, g, &n->mesh->primitives[i], w)) {
                return 0;
            }
        }
    }
    for (i = 0; i < n->children_count; i++) {
        if (!node(o, g, n->children[i], depth + 1)) {
            return 0;
        }
    }
    return 1;
}

void propgltf_free(PropMesh *m) {
    int i;

    for (i = 0; i < m->nimages; i++) {
        stbi_image_free(m->images[i].rgba);
    }
    free(m->images);
    free(m->v);
    free(m->d);
    memset(m, 0, sizeof(*m));
}

int propgltf_read(PropMesh *m, const uint8_t *data, size_t size, PropRead read, void *user) {
    cgltf_options opts;
    cgltf_data *g = NULL;
    Out o;
    cgltf_size i;
    int ok = 0;

    memset(m, 0, sizeof(*m));
    memset(&o, 0, sizeof(o));
    memset(&opts, 0, sizeof(opts));
    if (cgltf_parse(&opts, data, size, &g) != cgltf_result_success) {
        fprintf(stderr, "prop: not glTF\n");
        return 0;
    }
    if (!load_buffers(g, &opts, read, user)) {
        goto done;
    }
    if (cgltf_validate(g) != cgltf_result_success) {   /* (every accessor within its buffer) */
        fprintf(stderr, "prop: not valid glTF\n");
        goto done;
    }
    if (!load_images(m, g, &opts, read, user)) {
        goto done;
    }
    if (g->scene != NULL || g->scenes_count > 0) {
        const cgltf_scene *s = g->scene != NULL ? g->scene : &g->scenes[0];

        for (i = 0; i < s->nodes_count; i++) {
            if (!node(&o, g, s->nodes[i], 0)) {
                goto done;
            }
        }
    } else {   /* (no scene: every root node) */
        for (i = 0; i < g->nodes_count; i++) {
            if (g->nodes[i].parent == NULL && !node(&o, g, &g->nodes[i], 0)) {
                goto done;
            }
        }
    }
    if (o.nv == 0) {
        fprintf(stderr, "prop: no triangles\n");
        goto done;
    }
    m->v = o.v;
    m->nv = o.nv;
    m->d = o.d;
    m->nd = o.nd;
    o.v = NULL;
    o.d = NULL;
    m->lo[0] = m->hi[0] = m->v[0].x;
    m->lo[1] = m->hi[1] = m->v[0].y;
    m->lo[2] = m->hi[2] = m->v[0].z;
    for (i = 1; i < (cgltf_size)m->nv; i++) {
        const float p[3] = {m->v[i].x, m->v[i].y, m->v[i].z};
        int k;

        for (k = 0; k < 3; k++) {
            m->lo[k] = p[k] < m->lo[k] ? p[k] : m->lo[k];
            m->hi[k] = p[k] > m->hi[k] ? p[k] : m->hi[k];
        }
    }
    ok = 1;
done:
    free(o.v);
    free(o.d);
    cgltf_free(g);
    if (!ok) {
        propgltf_free(m);
    }
    return ok;
}
