#include "actor.h"

#include "../core/files.h"
#include "../data/tex.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void load_textures(Actor *a) {
    char path[96];
    size_t size;
    uint8_t *bank;
    int n, i;

    snprintf(path, sizeof(path), "%s.TEX", a->name);
    bank = files_read(path, &size);
    n = tex_count(bank, size);
    for (i = 0; i < n && i < ACTOR_MAX_TEXTURES; i++) {
        const TexEntry *t = tex_entry(bank, i);
        uint8_t *rgba = malloc((size_t)t->w * t->h * 4 + 4);

        if (rgba != NULL && t->w > 0 && t->h > 0 && tex_decode(bank, size, i, rgba)) {
            a->textures[i] = render_texture(rgba, t->w, t->h);
        }
        free(rgba);
    }
    a->ntextures = n < ACTOR_MAX_TEXTURES ? n : ACTOR_MAX_TEXTURES;
    free(bank);
}

int actor_load(Actor *actors, const char *name) {
    char path[96];
    size_t size;
    uint8_t *pck;
    int i;
    Actor *a;

    for (i = 0; i < MAX_ACTORS && actors[i].used; i++) {
    }
    if (i == MAX_ACTORS) {
        return -1;
    }
    a = &actors[i];
    snprintf(path, sizeof(path), "%s.PCK", name);
    pck = files_read(path, &size);
    if (pck == NULL) {
        return -1;
    }
    memset(a, 0, sizeof(*a));
    if (!model_build(&a->model, pck, size)) {
        return -1;
    }
    a->used = 1;
    snprintf(a->name, sizeof(a->name), "%s", name);
    load_textures(a);
    a->posed = malloc((size_t)a->model.nv * sizeof(MeshVertex));
    a->scale = 1.0f;
    a->rate = 0.5f;
    a->motion = -1;
    a->loop = 1;
    a->visible = 1;
    return i;
}

void actor_free(Actor *a) {
    int i;

    for (i = 0; i < a->ntextures; i++) {
        render_texture_free(a->textures[i]);
    }
    render_mesh_free(&a->gpu);
    model_free(&a->model);
    free(a->posed);
    memset(a, 0, sizeof(*a));
}

int actor_motion_done(const Actor *a) {
    return a->motion >= 0 && !a->loop && a->frame >= (float)(model_motion_frames(&a->model, a->motion) - 1);
}

void actor_tick(Actor *a) {
    int frames;

    if (!a->used || a->motion < 0) {
        return;
    }
    frames = model_motion_frames(&a->model, a->motion);
    a->frame += a->rate;
    if (a->loop) {
        while (a->frame >= (float)frames) {
            a->frame -= (float)frames;
        }
    } else if (a->frame > (float)(frames - 1)) {
        a->frame = (float)(frames - 1);
    }
}

/* the posed vertices, in room space, lit by one light from above and in front */
static void skin(Actor *a) {
    static Mat4 skin_m[MODEL_MAX_BONES];
    const Model *m = &a->model;
    Mat4 place = mat4_identity(), rot = mat4_identity();
    Vec3 light = vec3_norm(vec3(0.3f, 1.0f, 0.5f));
    float c = cosf(a->yaw), s = sinf(a->yaw);
    int i, j;

    model_pose(m, a->motion, a->frame, skin_m);
    rot.m[0] = c;  rot.m[2] = -s;  rot.m[8] = s;  rot.m[10] = c;
    place = rot;
    for (i = 0; i < 12; i++) {
        place.m[i] *= a->scale;
    }
    place.m[12] = a->pos.x;
    place.m[13] = a->pos.y;
    place.m[14] = a->pos.z;
    for (i = 0; i < m->nv; i++) {
        const SkinVertex *v = &m->v[i];
        MeshVertex *o = &a->posed[i];
        Vec3 p = vec3(0, 0, 0), n = vec3(0, 0, 0);
        float lit;

        for (j = 0; j < 4; j++) {
            const Mat4 *b = &skin_m[v->bones[j] < m->nbones ? v->bones[j] : 0];
            float w = v->weights[j];

            if (w == 0.0f) {
                continue;
            }
            p = vec3_add(p, vec3_scale(mat4_point(b, v->pos), w));
            n = vec3_add(n, vec3_scale(vec3(b->m[0] * v->normal.x + b->m[4] * v->normal.y + b->m[8] * v->normal.z,
                                            b->m[1] * v->normal.x + b->m[5] * v->normal.y + b->m[9] * v->normal.z,
                                            b->m[2] * v->normal.x + b->m[6] * v->normal.y + b->m[10] * v->normal.z),
                                       w));
        }
        p = mat4_point(&place, p);
        n = vec3_norm(vec3(c * n.x + s * n.z, n.y, -s * n.x + c * n.z));
        lit = 0.55f + 0.45f * fmaxf(0.0f, vec3_dot(n, light));
        o->x = p.x;
        o->y = p.y;
        o->z = p.z;
        o->s = v->s;
        o->t = v->t;
        o->rgba[0] = o->rgba[1] = o->rgba[2] = (uint8_t)(0x80 * lit);
        o->rgba[3] = 0x80;
    }
}

void actor_draw(Actor *a, const Mat4 *view_proj) {
    static const uint32_t kAllGroups[8] = {~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u};

    if (!a->used || !a->visible || a->model.nv == 0) {
        return;
    }
    skin(a);
    render_mesh_update(&a->gpu, a->posed, a->model.nv);
    render_mesh(&a->gpu, view_proj, a->model.d, a->model.nd, a->textures, a->ntextures, kAllGroups);
}
