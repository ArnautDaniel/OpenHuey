/* Props: a mod's own models (props.h). */
#include "props.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../core/files.h"
#include "../data/propgltf.h"

static Prop sProps[MAX_PROPS];

/* a file beside the glTF: its folder + the URI */
static uint8_t *read_beside(void *user, const char *rel, size_t *size) {
    const char *gltf = user, *slash = strrchr(gltf, '/');
    char path[256];
    int dir = slash != NULL ? (int)(slash - gltf) + 1 : 0;

    if (strstr(rel, "..") != NULL || rel[0] == '/') {   /* (beside it or below: never out of the mod) */
        return NULL;
    }
    snprintf(path, sizeof(path), "%.*s%s", dir, gltf, rel);
    return files_read(path, size);
}

int props_load(const char *path) {
    PropMesh m;
    uint8_t *data;
    size_t size;
    Prop *p;
    int i, k;

    for (i = 0; i < MAX_PROPS && sProps[i].used; i++) {
    }
    if (i == MAX_PROPS) {
        fprintf(stderr, "prop: no room for %s (%d at most)\n", path, MAX_PROPS);
        return -1;
    }
    if ((data = files_read(path, &size)) == NULL) {
        fprintf(stderr, "prop: no file %s\n", path);
        return -1;
    }
    k = propgltf_read(&m, data, size, read_beside, (void *)path);
    free(data);
    if (!k) {
        fprintf(stderr, "prop: %s not read\n", path);
        return -1;
    }
    p = &sProps[i];
    memset(p, 0, sizeof(*p));
    p->used = 1;
    p->room = -1;
    p->scale = 1.0f;
    snprintf(p->path, sizeof(p->path), "%s", path);
    render_mesh_upload(&p->gpu, m.v, m.nv);
    p->d = m.d;
    p->nd = m.nd;
    m.d = NULL;
    if (m.nimages > 0 && (p->tex = calloc((size_t)m.nimages, sizeof(GpuTexture))) != NULL) {
        p->ntex = m.nimages;
        for (k = 0; k < m.nimages; k++) {
            if (m.images[k].rgba != NULL) {
                p->tex[k] = render_texture(m.images[k].rgba, m.images[k].w, m.images[k].h);
            }
        }
    }
    p->lo = vec3(m.lo[0], m.lo[1], m.lo[2]);
    p->hi = vec3(m.hi[0], m.hi[1], m.hi[2]);
    propgltf_free(&m);
    return i;
}

void props_free(int i) {
    Prop *p = props_get(i);
    int k;

    if (p == NULL) {
        return;
    }
    render_mesh_free(&p->gpu);
    for (k = 0; k < p->ntex; k++) {
        render_texture_free(p->tex[k]);
    }
    free(p->tex);
    free(p->d);
    memset(p, 0, sizeof(*p));
}

void props_clear(void) {
    int i;

    for (i = 0; i < MAX_PROPS; i++) {
        props_free(i);
    }
}

Prop *props_get(int i) {
    return i >= 0 && i < MAX_PROPS && sProps[i].used ? &sProps[i] : NULL;
}

/* scaled, turned about X, then Y, then Z, then placed (as placed.c's room objects) */
Mat4 props_matrix(const Prop *p) {
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

Vec3 props_center(const Prop *p) {
    Mat4 m = props_matrix(p);
    Vec3 c = vec3_scale(vec3_add(p->lo, p->hi), 0.5f);

    return vec3(m.m[0] * c.x + m.m[4] * c.y + m.m[8] * c.z + m.m[12], m.m[1] * c.x + m.m[5] * c.y + m.m[9] * c.z + m.m[13],
                m.m[2] * c.x + m.m[6] * c.y + m.m[10] * c.z + m.m[14]);
}
