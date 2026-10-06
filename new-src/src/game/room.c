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

/* the room's look: PAC section 13 - offsets (from the section) at +0x8 the tint (two colours,
 * a mode byte: not 0 = not blurred), +0xC the screen blend (a colour, a mode byte: 1 / 4
 * subtract, 2 / 3 / 4 fixed colours), +0x10 the fog (near and far colours, near and far
 * distances) (src/game/effects.c Tint_SetParams, ScreenBlend_SetParams, Fog_SetParams) */
static void load_look(Room *r) {
    size_t size;
    const uint8_t *sec = pac_section(&r->pac, PAC_EFFECTS, &size);
    RoomLook *l = &gRoomLook;

    memset(l, 0, sizeof(*l));
    if (sec == NULL || size < 0x18) {
        return;
    }
    if (word(sec + 0x8) != 0 && word(sec + 0x8) + 9 <= size) {
        const uint8_t *d = sec + word(sec + 0x8);

        l->has_tint = 1;
        colour(word(d), 128.0f, 256.0f, l->tint_glow);   /* (strength: alpha / 2, of 0x80) */
        colour(word(d + 4), 128.0f, 256.0f, l->tint_contrast);
        l->tint_sharp = d[8] != 0;
    }
    if (word(sec + 0xC) != 0 && word(sec + 0xC) + 5 <= size) {
        const uint8_t *d = sec + word(sec + 0xC);
        uint32_t c = word(d);
        int mode = d[4];

        c = mode == 2 ? 0x80004080u : (mode == 3 || mode == 4) ? 0x40404040u : c;
        l->has_bloom = 1;
        colour(c, 128.0f, 256.0f, l->bloom);
        l->bloom_subtract = mode == 1 || mode == 4;
    }
    if (word(sec + 0x10) != 0 && word(sec + 0x10) + 16 <= size) {
        const uint8_t *d = sec + word(sec + 0x10);
        float range[2];

        l->has_fog = 1;
        colour(word(d), 255.0f, 128.0f, l->fog_near_color);
        colour(word(d + 4), 255.0f, 128.0f, l->fog_far_color);
        memcpy(range, d + 8, 8);
        l->fog_near = range[0];
        l->fog_far = range[1];
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
    load_look(r);
    return 1;
}

void room_free(Room *r) {
    int i;

    for (i = 0; i < r->ntextures; i++) {
        render_texture_free(r->textures[i]);
    }
    render_mesh_free(&r->gpu);
    roommesh_free(&r->mesh);
    navmesh_free(&r->nav);
    free(r->pac.data);
    memset(r, 0, sizeof(*r));
    r->id = -1;
}

void room_draw(const Room *r, const Mat4 *view_proj) {
    if (r->id < 0) {
        return;
    }
    render_mesh(&r->gpu, view_proj, r->mesh.d, r->mesh.nd, r->textures, r->ntextures, r->groups);
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
