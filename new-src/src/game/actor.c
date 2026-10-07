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
    a->rate = 1.0f;   /* (a motion frame a tick: the game ran its motions at its own 30 fps) */
    a->motion = -1;
    a->prev_motion = -1;
    a->loop = 1;
    a->visible = 1;
    a->shadow_size = 7.0f;
    a->lo = vec3(1e30f, 1e30f, 1e30f);
    a->hi = vec3(-1e30f, -1e30f, -1e30f);
    for (i = 0; i < a->model.nv; i++) {
        a->lo = vec3_min(a->lo, a->model.v[i].pos);
        a->hi = vec3_max(a->hi, a->model.v[i].pos);
    }
    return (int)(a - actors);
}

void actor_free(Actor *a) {
    int i;

    for (i = 0; i < a->ntextures; i++) {
        render_texture_free(a->textures[i]);
    }
    render_mesh_free(&a->gpu);
    render_mesh_free(&a->shadow);
    model_free(&a->model);
    free(a->posed);
    memset(a, 0, sizeof(*a));
}

int actor_motion_done(const Actor *a) {
    return a->motion >= 0 && !a->loop && a->frame >= (float)(model_motion_frames(&a->model, a->motion) - 1);
}

/* a play time on by the speed: looping wraps it, else it stops at the last frame; 1 when it
 * went past the end (anim_step) */
static int step_time(float *t, float speed, int frames, int loop) {
    int wrapped = 0;

    *t += speed;
    if (loop) {
        while (*t < 0.0f) {
            *t += (float)frames;
        }
        while (*t >= (float)frames) {
            *t -= (float)frames;
            wrapped = 1;
        }
        return wrapped;
    }
    if (*t < 0.0f) {
        *t = 0.0f;
    }
    if (*t > (float)(frames - 1)) {
        *t = (float)(frames - 1);
        wrapped = 1;
    }
    return wrapped;
}

void actor_tick(Actor *a) {
    int frames, loop;

    if (!a->used || a->motion < 0 || (a->mflags & 0x40)) {
        return;
    }
    frames = model_motion_frames(&a->model, a->motion);
    loop = a->loop || (a->mflags & 1);
    if (!(a->mflags & 0x10)) {
        if (step_time(&a->frame, a->rate, frames, loop)) {
            a->mflags |= 0x20;
        } else {
            a->mflags &= ~0x20;
        }
        if (a->frame >= (float)(frames - 1)) {
            a->mflags |= 0x400;
        } else {
            a->mflags &= ~0x400;
        }
    }
    if (a->fade > 0.0f) {   /* the motion before goes on while it fades out */
        if (a->prev_motion >= 0) {
            step_time(&a->prev_frame, a->rate, model_motion_frames(&a->model, a->prev_motion), 1);
        }
        a->fade -= 1.0f;
    }
}

void actor_motion_start(Actor *a, int index, int flags, float blend) {
    int frames = model_motion_frames(&a->model, index);

    if (a->motion >= 0 && blend > 0.0f) {
        a->prev_motion = a->motion;
        a->prev_frame = a->frame;
        a->fade = a->fade_len = blend;
    } else {
        a->fade = a->fade_len = 0.0f;
    }
    if ((flags & a->mflags & 2) && a->motion >= 0) {   /* in step: the same phase */
        a->frame = (float)frames * (a->frame / (float)model_motion_frames(&a->model, a->motion));
    } else {
        a->frame = 0.0f;
    }
    a->motion = index;
    a->mflags = (flags & 0xFFFF) | (flags & 8 ? 0x10 : 0);
    a->loop = flags & 1;
}

Vec3 actor_bone(const Actor *a, int b) {
    return b >= 0 && b < a->model.nbones ? a->bones[b] : a->pos;
}

int actor_motion_entry(const Actor *a, int index, int *blend, int *pose, int *flags) {
    const uint8_t *e;

    if (a->table == NULL || index < 0 || index >= a->ntable) {
        *blend = *pose = *flags = 0;
        return 0;
    }
    e = a->table + index * 6;
    *blend = (int16_t)(e[0] | e[1] << 8);
    *pose = e[2];
    *flags = e[4] | e[5] << 8;
    return 1;
}

/* the posed vertices and normals, in room space (the shader lights them) */
static void skin(Actor *a) {
    static Mat4 skin_m[MODEL_MAX_BONES];
    Model *m = &a->model;
    Mat4 place = mat4_identity();
    float c = cosf(a->yaw), s = sinf(a->yaw);
    int i, j;

    if (a->drive != NULL) {   /* a cutscene's motion: placed in the room by its root bone */
        const uint8_t *own = m->motions;
        size_t own_size = m->motions_size;
        Vec3 root = a->pos;
        int root_bone;

        m->motions = a->drive;
        m->motions_size = a->drive_size;
        model_pose_root(m, model_motion_find(m, 0x8000), a->drive_frame, skin_m, &root);
        m->motions = own;
        m->motions_size = own_size;
        for (root_bone = 0; root_bone < m->nbones && m->bones[root_bone].parent >= 0; root_bone++) {
        }
        a->pos = vec3(root.x, root.y - (root_bone < m->nbones ? m->bones[root_bone].rest_pos[1] : 0.0f), root.z);
        c = 1.0f;
        s = 0.0f;
        place.m[0] = place.m[5] = place.m[10] = a->scale;
    } else {
        float x = a->fade_len > 0.0f && a->fade > 0.0f ? a->fade / a->fade_len : 0.0f;   /* (fade_weight) */

        model_pose_blend(m, a->motion, a->frame, a->prev_motion, a->prev_frame, x * x * (3.0f - 2.0f * x), skin_m,
                         NULL);
        place.m[0] = c * a->scale;  place.m[2] = -s * a->scale;
        place.m[5] = a->scale;
        place.m[8] = s * a->scale;  place.m[10] = c * a->scale;
        place.m[12] = a->pos.x;
        place.m[13] = a->pos.y;
        place.m[14] = a->pos.z;
    }
    for (i = 0; i < m->nbones; i++) {   /* the bones' origins, placed */
        a->bones[i] = mat4_point(&place, model_pose_origins()[i]);
    }
    for (i = 0; i < m->nv; i++) {
        const SkinVertex *v = &m->v[i];
        MeshVertex *o = &a->posed[i];
        Vec3 p = vec3(0, 0, 0), n = vec3(0, 0, 0);

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
        o->x = p.x;
        o->y = p.y;
        o->z = p.z;
        o->s = v->s;
        o->t = v->t;
        o->rgba[0] = o->rgba[1] = o->rgba[2] = o->rgba[3] = 0x80;
        o->nx = n.x;
        o->ny = n.y;
        o->nz = n.z;
    }
}

/* a soft dark disc on the floor under the actor, a little above it to stay in front */
static void draw_shadow(Actor *a, const Mat4 *view_proj) {
    static const float kCorner[6][2] = {{-1, -1}, {1, -1}, {1, 1}, {-1, -1}, {1, 1}, {-1, 1}};
    static const uint32_t kAllGroups[8] = {~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u};
    MeshVertex q[6];
    MeshDraw d;
    GpuTexture blob = render_blob_texture();
    float r = a->shadow_size * a->scale;
    int i;

    memset(q, 0, sizeof(q));
    for (i = 0; i < 6; i++) {
        q[i].x = a->pos.x + kCorner[i][0] * r;
        q[i].y = a->pos.y + 0.3f;
        q[i].z = a->pos.z + kCorner[i][1] * r;
        q[i].s = kCorner[i][0] * 0.5f + 0.5f;
        q[i].t = kCorner[i][1] * 0.5f + 0.5f;
        q[i].rgba[3] = 0x50;   /* black, at about 60% */
    }
    memset(&d, 0, sizeof(d));
    d.first = 0;
    d.count = 6;
    d.texture = 0;
    d.blend = 1;
    d.no_zwrite = 1;
    render_mesh_update(&a->shadow, q, 6);
    render_mesh(&a->shadow, view_proj, &d, 1, &blob, 1, kAllGroups);
}

void actor_prepare(Actor *a) {
    if (!a->used || !a->visible || a->model.nv == 0) {
        return;
    }
    skin(a);
    render_mesh_update(&a->gpu, a->posed, a->model.nv);
}

Vec3 actor_center(const Actor *a) {
    return vec3(a->pos.x, a->pos.y + (a->lo.y + a->hi.y) * 0.5f * a->scale, a->pos.z);
}

float actor_radius(const Actor *a) {
    Vec3 size = vec3_sub(a->hi, a->lo);

    return 0.5f * a->scale * fmaxf(size.y, fmaxf(size.x, size.z));
}

void actor_draw(Actor *a, const Mat4 *view_proj) {
    static const uint32_t kAllGroups[8] = {~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u};

    if (!a->used || !a->visible || a->model.nv == 0) {
        return;
    }
    if (gRender.shadows && a->shadow_size > 0.0f) {
        draw_shadow(a, view_proj);
    }
    render_mesh(&a->gpu, view_proj, a->model.d, a->model.nd, a->textures, a->ntextures, kAllGroups);
}
