#include "room.h"

#include "../core/files.h"
#include "../data/tex.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int room_exists(int id) {
    char path[64];

    if (id < 0 || id > 0xFFF) {
        return 0;
    }
    pac_room_path(id, path, sizeof(path));
    return files_exist(path);
}

static void load_textures(Room *r) {
    size_t size;
    const uint8_t *bank = pac_section(&r->pac, PAC_TEXTURES, &size);
    int n = tex_count(bank, size), i;

    for (i = 0; i < n && i < ROOM_MAX_TEXTURES; i++) {
        const TexEntry *t = tex_entry(bank, i);
        uint8_t *rgba = malloc((size_t)t->w * t->h * 4 + 4);

        if (rgba != NULL && t->w > 0 && t->h > 0 && t->w <= 2048 && t->h <= 2048 && tex_decode(bank, size, i, rgba)) {
            r->textures[i] = render_texture(rgba, t->w, t->h);
        } else {
            fprintf(stderr, "room %03X: texture %d: format %02X not decoded\n", r->id, i, t->psm);
        }
        free(rgba);
    }
    r->ntextures = n < ROOM_MAX_TEXTURES ? n : ROOM_MAX_TEXTURES;
}

/* a PS2 colour word (little-endian R, G, B, A) */
static void colour(uint32_t c, float scale, float alpha_scale, float *out) {
    out[0] = (float)(c & 0xFF) / scale;
    out[1] = (float)((c >> 8) & 0xFF) / scale;
    out[2] = (float)((c >> 16) & 0xFF) / scale;
    out[3] = (float)(c >> 24) / alpha_scale;
}

static uint32_t word(const uint8_t *p) {
    uint32_t x;

    memcpy(&x, p, 4);
    return x;
}

/* the room's look: PAC section 13 - offsets (from the section) at +0x14 the depth of field
 * (four view distances), +0x8 the tint (two colours,
 * a mode byte: not 0 = not blurred), +0xC the screen blend (a colour, a mode byte: 1 / 4
 * subtract, 2 / 3 / 4 fixed colours), +0x10 the fog (near and far colours, near and far
 * distances) (src/game/effects.c Tint_SetParams, ScreenBlend_SetParams, Fog_SetParams) */
static void load_look(Room *r);

void room_reset_look(Room *r) {
    if (r->id >= 0) {
        load_look(r);
    }
}

/* one of the look's effects from its parameters (the original's room effect slots 0x1C depth of
 * field, 0x1D fog, 0x1E screen blend, 0x1F tint: Tint_SetParams, ScreenBlend_SetParams,
 * Fog_SetParams, DepthRange_SetParams); NULL: that effect removed */
void room_look_set(int slot, const uint8_t *d, size_t n) {
    RoomLook *l = &gRoomLook;

    switch (slot) {
    case 0x1F:
        l->has_tint = d != NULL && n >= 9;
        if (l->has_tint) {
            colour(word(d), 128.0f, 256.0f, l->tint_glow);   /* (strength: alpha / 2, of 0x80) */
            colour(word(d + 4), 128.0f, 256.0f, l->tint_contrast);
            l->tint_sharp = d[8] != 0;
        }
        break;
    case 0x1E:
        l->has_bloom = d != NULL && n >= 5;
        if (l->has_bloom) {
            uint32_t c = word(d);
            int mode = d[4];

            c = mode == 2 ? 0x80004080u : (mode == 3 || mode == 4) ? 0x40404040u : c;
            colour(c, 128.0f, 256.0f, l->bloom);
            l->bloom_subtract = mode == 1 || mode == 4;
            l->bloom_mode = mode;
        }
        break;
    case 0x1D:
        l->has_fog = d != NULL && n >= 16;
        if (l->has_fog) {
            float range[2];

            colour(word(d), 255.0f, 128.0f, l->fog_near_color);
            colour(word(d + 4), 255.0f, 128.0f, l->fog_far_color);
            memcpy(range, d + 8, 8);
            l->fog_near = range[0];
            l->fog_far = range[1];
        }
        break;
    case 0x1C:
        l->has_dof = d != NULL && n >= 16;
        if (l->has_dof) {
            memcpy(l->dof, d, 16);
        }
        break;
    }
}

static void load_look(Room *r) {
    size_t size;
    const uint8_t *sec = pac_section(&r->pac, PAC_EFFECTS, &size);

    memset(&gRoomLook, 0, sizeof(gRoomLook));
    if (sec == NULL || size < 0x18) {
        return;
    }
    if (word(sec + 0x8) != 0 && word(sec + 0x8) + 9 <= size) {
        room_look_set(0x1F, sec + word(sec + 0x8), 9);
    }
    if (word(sec + 0xC) != 0 && word(sec + 0xC) + 5 <= size) {
        room_look_set(0x1E, sec + word(sec + 0xC), 5);
    }
    if (word(sec + 0x10) != 0 && word(sec + 0x10) + 16 <= size) {
        room_look_set(0x1D, sec + word(sec + 0x10), 16);
    }
    if (word(sec + 0x14) != 0 && word(sec + 0x14) + 16 <= size) {   /* (DepthRange_SetParams) */
        room_look_set(0x1C, sec + word(sec + 0x14), 16);
    }
}

/* PAC section 4: a count, the ambient colour, then 0x30 bytes a light (src/game/lights.c
 * Lights_TakeRoom) */
static void load_lights(Room *r) {
    size_t size;
    const uint8_t *sec = pac_section(&r->pac, PAC_LIGHTS, &size);
    int32_t n;
    int i;

    r->nlights = 0;
    r->ambient = vec3(16, 16, 16);
    if (sec == NULL || size < 16) {
        return;
    }
    memcpy(&n, sec, 4);
    memcpy(&r->ambient, sec + 4, 12);
    for (i = 0; i < n && i < ROOM_MAX_LIGHTS && 16 + (size_t)(i + 1) * 48 <= size; i++) {
        float v[12];

        memcpy(v, sec + 16 + i * 48, sizeof(v));
        r->lights[i].pos = vec3(v[0], v[1], v[2]);
        r->lights[i].color = vec3(v[4], v[5], v[6]);
        r->lights[i].intensity = v[7];
        r->lights[i].range = v[8];
    }
    r->nlights = i;
}

int room_lights_at(const Room *r, Vec3 pos, int out[3]) {
    float best[3] = {-1, -1, -1}, h;
    int tri = navmesh_find(&r->nav, pos, 10.0f, &h), i, k, n = 0;
    uint32_t mask = tri >= 0 ? r->nav.tris[tri].lights : (r->nlights > 0 ? 1u : 0u);

    out[0] = out[1] = out[2] = -1;
    for (i = 0; i < r->nlights; i++) {
        const RoomLight *l = &r->lights[i];
        float score = (0.3f * l->color.x + 0.6f * l->color.y + 0.1f * l->color.z) * l->intensity;
        int idx = i;

        if (!(mask >> i & 1)) {
            continue;
        }
        if (l->range > 0.0f) {
            float d = vec3_len(vec3_sub(pos, l->pos));

            score = d < l->range ? score * (l->range - d) / l->range : 0.0f;
        }
        if (score <= 0.0f) {
            continue;
        }
        for (k = 0; k < 3; k++) {   /* kept in order, brightest first */
            if (score > best[k]) {
                float ts = best[k];
                int ti = out[k];

                best[k] = score;
                out[k] = idx;
                score = ts;
                idx = ti;
                if (idx < 0) {
                    break;
                }
            }
        }
    }
    for (k = 0; k < 3; k++) {
        n += out[k] >= 0;
    }
    return n;
}

/* the doors: section 7 (8 offsets by exit, 0 none: an entry is its id, a point +0x4, another
 * +0x10, its turn +0x1C) and section 8 (8 offsets; the nth present door's model at its offset
 * + n x 0x10 - Doors_TakeSection8) */
static float rdf(const uint8_t *p) {
    float f;

    memcpy(&f, p, 4);
    return f;
}

static void load_doors(Room *r) {
    size_t s7size, s8size;
    const uint8_t *s7 = pac_section(&r->pac, PAC_DOORS, &s7size), *s8 = pac_section(&r->pac, PAC_DOORS2, &s8size);
    int i, n = 0;

    if (s7 == NULL || s7size < 32) {
        return;
    }
    for (i = 0; i < ROOM_DOORS; i++) {
        RoomDoor *d = &r->doors[i];
        uint32_t o, m;

        memcpy(&o, s7 + i * 4, 4);
        if (o == 0 || o + 0x28 > s7size) {
            continue;
        }
        d->pos = vec3(rdf(s7 + o + 4), rdf(s7 + o + 8), rdf(s7 + o + 12));
        d->sides = s7 + o + 0x28;
        d->rot = vec3(rdf(s7 + o + 0x1C), rdf(s7 + o + 0x20), rdf(s7 + o + 0x24));
        if (s8 != NULL && (size_t)(n + 1) * 4 <= s8size) {
            memcpy(&m, s8 + n * 4, 4);
            if (m != 0xFFFFFFFFu && m + (uint32_t)n * 0x10 < s8size &&
                roommesh_build_list(&d->mesh, s8 + m + n * 0x10, s8 + s8size, MESH_SOLID) && d->mesh.nv > 0) {
                render_mesh_upload(&d->gpu, d->mesh.v, d->mesh.nv);
                d->present = 1;
            }
        }
        n++;
    }
}

void room_door_swing(Room *r, int exit, float degrees, int at_once) {
    if (exit >= 0 && exit < ROOM_DOORS) {
        r->doors[exit].target = degrees;
        if (at_once) {
            r->doors[exit].swing = degrees;
        }
    }
}

/* flags set (or cleared) on one side's triangles (Doors_Passage) */
static void door_side_flags(Room *r, const RoomDoor *d, int side, uint32_t flags, int set) {
    const uint8_t *end = r->pac.data + r->pac.size, *t = d->sides;
    uint32_t n, k, tri;

    if (t == NULL || t + 4 > end) {
        return;
    }
    memcpy(&n, t, 4);
    t += 4;
    if (side != 0) {
        t += (size_t)n * 4;
        if (t + 4 > end) {
            return;
        }
        memcpy(&n, t, 4);
        t += 4;
    }
    for (k = 0; k < n && t + 4 <= end; k++, t += 4) {
        memcpy(&tri, t, 4);
        if (tri < (uint32_t)r->nav.ntris) {
            if (set) {
                r->nav.tris[tri].flags |= flags;
            } else {
                r->nav.tris[tri].flags &= ~flags;
            }
        }
    }
}

void room_door_passage(Room *r, int exit, int open, uint32_t locks) {
    const RoomDoor *d;

    if (exit < 0 || exit >= ROOM_DOORS || !r->doors[exit].present) {
        return;
    }
    d = &r->doors[exit];
    door_side_flags(r, d, open ? 0 : 1, 0x60000, 0);
    door_side_flags(r, d, open ? 1 : 0, 0x60000, 1);
    door_side_flags(r, d, 0, 0x5000000, 0);
    if (locks != 0) {
        door_side_flags(r, d, 0, locks, 1);
    }
}

void room_door_angle(Room *r, int exit, float radians) {
    if (exit >= 0 && exit < ROOM_DOORS) {
        r->doors[exit].swing = r->doors[exit].target = (radians - r->doors[exit].rot.y) * 180.0f / 3.14159265f;
    }
}

int room_load(Room *r, int id) {
    char path[64];
    size_t size;
    const uint8_t *sec;

    room_free(r);
    pac_room_path(id, path, sizeof(path));
    r->pac.data = files_read(path, &r->pac.size);
    if (r->pac.data == NULL) {
        fprintf(stderr, "room: can't read %s\n", path);
        return 0;
    }
    r->id = id;
    sec = pac_section(&r->pac, PAC_MESH, &size);
    if (sec != NULL && !roommesh_build(&r->mesh, sec, size)) {
        fprintf(stderr, "room %03X: the mesh section is malformed\n", id);
    }
    if (r->mesh.nv > 0) {
        render_mesh_upload(&r->gpu, r->mesh.v, r->mesh.nv);
    }
    load_textures(r);
    sec = pac_section(&r->pac, PAC_NAV, &size);
    navmesh_build(&r->nav, sec, size);
    sec = pac_section(&r->pac, PAC_NAV3, &size);
    navmesh_take_flags(&r->nav, sec, sec != NULL ? size : 0);
    load_look(r);
    load_lights(r);
    load_doors(r);
    {
        size_t asize = 0;
        const uint8_t *anims = pac_section(&r->pac, PAC_PLACED2, &asize);

        sec = pac_section(&r->pac, PAC_PLACED, &size);
        placed_load(&r->placed, sec, sec != NULL ? size : 0, anims, asize);
    }
    if (r->mesh.ndyn > 0) {
        r->moving_v = malloc((size_t)r->mesh.ndv * 3 * sizeof(MeshVertex));
        r->moving_d = malloc((size_t)r->mesh.ndyn * sizeof(MeshDraw));
    }
    return 1;
}

void room_free(Room *r) {
    int i;

    for (i = 0; i < r->ntextures; i++) {
        render_texture_free(r->textures[i]);
    }
    render_mesh_free(&r->gpu);
    render_mesh_free(&r->moving);
    for (i = 0; i < ROOM_DOORS; i++) {
        render_mesh_free(&r->doors[i].gpu);
        roommesh_free(&r->doors[i].mesh);
    }
    placed_free(&r->placed);
    free(r->moving_v);
    free(r->moving_d);
    roommesh_free(&r->mesh);
    navmesh_free(&r->nav);
    free(r->pac.data);
    memset(r, 0, sizeof(*r));
    r->id = -1;
}

void room_tick(Room *r) {
    int i;

    roommesh_tick(&r->mesh);
    placed_tick(&r->placed);
    for (i = 0; i < ROOM_DOORS; i++) {   /* (a quarter turn in about 20 frames) */
        RoomDoor *d = &r->doors[i];
        float step = 4.5f;

        d->swing = fabsf(d->target - d->swing) <= step ? d->target : d->swing + (d->target > d->swing ? step : -step);
    }
}

/* door i's model to room space: turned (its rest turn plus the swing about the vertical), then
 * placed (sceVu0RotMatrix, sceVu0TransMatrix) */
static Mat4 door_matrix(const RoomDoor *d) {
    float y = d->rot.y + d->swing * 3.14159265f / 180.0f;
    float cx = cosf(d->rot.x), sx = sinf(d->rot.x), cy = cosf(y), sy = sinf(y), cz = cosf(d->rot.z), sz = sinf(d->rot.z);
    Mat4 rx = mat4_identity(), ry = mat4_identity(), rz = mat4_identity(), t = mat4_identity();

    rx.m[5] = cx;  rx.m[6] = sx;  rx.m[9] = -sx;  rx.m[10] = cx;
    ry.m[0] = cy;  ry.m[2] = -sy; ry.m[8] = sy;   ry.m[10] = cy;
    rz.m[0] = cz;  rz.m[1] = sz;  rz.m[4] = -sz;  rz.m[5] = cz;
    t.m[12] = d->pos.x;
    t.m[13] = d->pos.y;
    t.m[14] = d->pos.z;
    return mat4_mul(t, mat4_mul(rz, mat4_mul(ry, rx)));
}

/* the draws [first, end) of parts lo..hi (the mesh keeps them in part order) */
static void draw_parts(Room *r, const Mat4 *vp, int lo, int hi) {
    int first = 0, end;

    while (first < r->mesh.nd && r->mesh.d[first].part < lo) {
        first++;
    }
    for (end = first; end < r->mesh.nd && r->mesh.d[end].part <= hi; end++) {
    }
    if (end > first) {
        render_mesh(&r->gpu, vp, r->mesh.d + first, end - first, r->textures, r->ntextures, r->groups);
    }
}

void room_draw(Room *r, const Mat4 *view_proj, Vec3 eye, Vec3 forward) {
    static const uint32_t kAll[8] = {~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u};
    int i;

    if (r->id < 0) {
        return;
    }
    draw_parts(r, view_proj, MESH_SOLID, MESH_SEE_THROUGH);
    for (i = 0; i < ROOM_DOORS; i++) {
        if (r->doors[i].present) {
            Mat4 mvp = mat4_mul(*view_proj, door_matrix(&r->doors[i]));

            render_mesh(&r->doors[i].gpu, &mvp, r->doors[i].mesh.d, r->doors[i].mesh.nd, r->textures, r->ntextures, kAll);
        }
    }
    placed_draw(&r->placed, view_proj, r->textures, r->ntextures);
    if (r->mesh.ndyn > 0) {
        int n = roommesh_dynamic(&r->mesh, eye, forward, r->moving_v, r->moving_d), nv = 0, i;

        for (i = 0; i < n; i++) {
            nv = r->moving_d[i].first + r->moving_d[i].count > nv ? r->moving_d[i].first + r->moving_d[i].count : nv;
        }
        render_mesh_update(&r->moving, r->moving_v, nv > 0 ? nv : 1);
        render_mesh(&r->moving, view_proj, r->moving_d, n, r->textures, r->ntextures, r->groups);
    }
    draw_parts(r, view_proj, MESH_PART3, MESH_GLOW);
    draw_parts(r, view_proj, MESH_BLOOM_MASK, MESH_BLOOM_MASK);
}

int room_floor_below(const Room *r, float x, float y, float z, float *out) {
    int i, k, found = 0;
    float best = -1e30f;

    for (i = 0; i < r->mesh.nd; i++) {
        const MeshDraw *d = &r->mesh.d[i];

        if (d->part != MESH_SOLID) {
            continue;
        }
        for (k = d->first; k + 2 < d->first + d->count; k += 3) {
            const MeshVertex *a = &r->mesh.v[k], *b = &r->mesh.v[k + 1], *c = &r->mesh.v[k + 2];
            /* (x, z) inside the triangle's shadow on the ground plane: barycentric coordinates */
            float det = (b->z - c->z) * (a->x - c->x) + (c->x - b->x) * (a->z - c->z);
            float l1, l2, l3, h;

            if (fabsf(det) < 1e-6f) {
                continue;   /* a wall */
            }
            l1 = ((b->z - c->z) * (x - c->x) + (c->x - b->x) * (z - c->z)) / det;
            l2 = ((c->z - a->z) * (x - c->x) + (a->x - c->x) * (z - c->z)) / det;
            l3 = 1.0f - l1 - l2;
            if (l1 < 0.0f || l2 < 0.0f || l3 < 0.0f) {
                continue;
            }
            h = l1 * a->y + l2 * b->y + l3 * c->y;
            if (h <= y && h > best) {
                best = h;
                found = 1;
            }
        }
    }
    if (found) {
        *out = best;
    }
    return found;
}
