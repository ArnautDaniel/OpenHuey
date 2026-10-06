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
