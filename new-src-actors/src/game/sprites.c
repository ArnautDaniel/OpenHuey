/* Sprites (sprites.h): the original's quad drawer (src/game/effects.c gl_sprites). */
#include "sprites.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../core/files.h"
#include "../data/tex.h"
#include "../render/render.h"

#define CACHE 64
#define DECODED 128

typedef struct Group {
    const uint8_t *bank;
    size_t size;
    int count;
} Group;

typedef struct Decoded {   /* a cache texture on the GPU, in one palette */
    const uint8_t *bank;
    int i, csa;
    GpuTexture tex;
} Decoded;

typedef struct Batch {
    int used;
    int group, id, palette;
    int x, y, cw, ch, tw, th, frames;
    int flags, layer;
    float cx, cy;
    float corner[4][3];
    int uv;                 /* its cell given in texture coordinates */
    float u0, v0, u1, v1;
    int n;
    Sprite s[SPRITES_PER_BATCH];
    GpuMesh gpu;
} Batch;

static Group sGroups[CACHE];   /* by the group's first index */
static Decoded sDecoded[DECODED];
static Batch *sBatch[SPRITE_BATCHES];
static uint8_t *sGameFix;
static MeshVertex sVerts[SPRITES_PER_BATCH * 6];

void sprites_group(int group, const uint8_t *bank, size_t size) {
    int i;

    if (group < 0 || group >= CACHE) {
        return;
    }
    for (i = 0; i < DECODED; i++) {   /* its textures gone from the GPU */
        if (sDecoded[i].tex != 0 && sDecoded[i].bank == sGroups[group].bank) {
            render_texture_free(sDecoded[i].tex);
            memset(&sDecoded[i], 0, sizeof(sDecoded[i]));
        }
    }
    sGroups[group].bank = bank;
    sGroups[group].size = size;
    sGroups[group].count = bank != NULL ? tex_count(bank, size) : 0;
}

void sprites_init(void) {
    size_t size;

    sGameFix = files_read("GAME_FIX.TEX", &size);
    if (sGameFix != NULL) {
        sprites_group(0x10, sGameFix, size);
    }
}

/* cache index group + id: the group holding it (the latest added below it that reaches it) */
static GpuTexture texture(int group, int id, int csa) {
    int idx = group + id, g, i, free_i = -1;
    const Group *gr = NULL;
    uint8_t *rgba;
    const TexEntry *t;

    for (g = idx < CACHE - 1 ? idx : CACHE - 1; g >= 0 && idx < CACHE; g--) {
        if (sGroups[g].bank != NULL && idx - g < sGroups[g].count) {
            gr = &sGroups[g];
            break;
        }
    }
    if (gr == NULL) {
        return 0;
    }
    for (i = 0; i < DECODED; i++) {
        if (sDecoded[i].tex != 0 && sDecoded[i].bank == gr->bank && sDecoded[i].i == idx - g && sDecoded[i].csa == csa) {
            return sDecoded[i].tex;
        }
        if (sDecoded[i].tex == 0 && free_i < 0) {
            free_i = i;
        }
    }
    if (free_i < 0) {
        return 0;
    }
    t = tex_entry(gr->bank, idx - g);
    rgba = t != NULL && t->w > 0 && t->h > 0 && t->w <= 2048 && t->h <= 2048 ? malloc((size_t)t->w * t->h * 4) : NULL;
    if (rgba == NULL || !tex_decode_csa(gr->bank, gr->size, idx - g, csa, rgba)) {
        free(rgba);
        return 0;
    }
    sDecoded[free_i] = (Decoded){gr->bank, idx - g, csa, render_texture(rgba, t->w, t->h)};
    free(rgba);
    return sDecoded[free_i].tex;
}

int sprites_new(void) {
    int b;

    for (b = 0; b < SPRITE_BATCHES; b++) {
        if (sBatch[b] == NULL) {
            sBatch[b] = calloc(1, sizeof(Batch));
        }
        if (sBatch[b] != NULL && !sBatch[b]->used) {
            GpuMesh keep = sBatch[b]->gpu;

            memset(sBatch[b], 0, sizeof(Batch));
            sBatch[b]->gpu = keep;
            sBatch[b]->used = 1;
            sBatch[b]->palette = -1;
            sBatch[b]->frames = 1;
            return b;
        }
    }
    return -1;
}

static Batch *batch(int b) {
    return b >= 0 && b < SPRITE_BATCHES && sBatch[b] != NULL && sBatch[b]->used ? sBatch[b] : NULL;
}

void sprites_free(int b) {
    Batch *x = batch(b);

    if (x != NULL) {
        x->used = 0;
        x->n = 0;
    }
}

void sprites_free_all(void) {
    int b;

    for (b = 0; b < SPRITE_BATCHES; b++) {
        sprites_free(b);
    }
}

void sprites_texture(int b, int group, int id, int palette) {
    Batch *x = batch(b);

    if (x != NULL) {
        x->group = group;
        x->id = id;
        x->palette = palette;
    }
}

void sprites_cells(int b, int cx, int cy, int cw, int ch, int tw, int th, int frames) {
    Batch *x = batch(b);

    if (x != NULL) {
        x->x = cx;
        x->y = cy;
        x->cw = cw;
        x->ch = ch;
        x->tw = tw;
        x->th = th;
        x->frames = frames < 1 ? 1 : frames;
    }
}

void sprites_flags(int b, int flags, int layer) {
    Batch *x = batch(b);

    if (x != NULL) {
        x->flags = flags;
        x->layer = layer;
    }
}

void sprites_offset(int b, float cx, float cy) {
    Batch *x = batch(b);

    if (x != NULL) {
        x->cx = cx;
        x->cy = cy;
    }
}

void sprites_corner(int b, int k, float cx, float cy, float cz) {
    Batch *x = batch(b);

    if (x != NULL && k >= 0 && k < 4) {
        x->corner[k][0] = cx;
        x->corner[k][1] = cy;
        x->corner[k][2] = cz;
    }
}

void sprites_uv(int b, float u0, float v0, float u1, float v1) {
    Batch *x = batch(b);

    if (x != NULL) {
        x->uv = 1;
        x->u0 = u0;
        x->v0 = v0;
        x->u1 = u1;
        x->v1 = v1;
    }
}

Sprite *sprites_records(int b, int n) {
    Batch *x = batch(b);

    if (x == NULL) {
        return NULL;
    }
    if (n >= 0) {
        x->n = n > SPRITES_PER_BATCH ? SPRITES_PER_BATCH : n;
    }
    return x->s;
}

/* frame f's cell: along rows from the first cell, wrapping at the sheet's width */
static void cell(const Batch *x, int f, float *u0, float *v0, float *u1, float *v1) {
    int cx = x->x, cy = x->y, k;

    if (x->cw > 0 && x->tw > 0) {
        for (k = 0; k < f; k++) {
            cx += x->cw;
            if (cx + x->cw > x->tw) {
                cx = 0;
                cy += x->ch;
            }
        }
    }
    *u0 = x->tw > 0 ? (float)cx / x->tw : 0.0f;
    *v0 = x->th > 0 ? (float)cy / x->th : 0.0f;
    *u1 = x->tw > 0 ? (float)(cx + x->cw) / x->tw : 1.0f;
    *v1 = x->th > 0 ? (float)(cy + x->ch) / x->th : 1.0f;
}

static Vec3 rot_y_quarter(Vec3 v) {   /* sceVu0RotMatrixY by pi / 2 */
    return vec3(v.z, v.y, -v.x);
}

static int draw_batch(Batch *x, const Mat4 *view_proj, const Mat4 *view) {
    Vec3 right, down, fwd;
    GpuTexture tex = texture(x->group, x->id, x->palette < 0 ? 0 : x->palette & 0x1F);
    MeshDraw d;
    int i, k, nv = 0;

    if (tex == 0 || x->n == 0) {
        return 0;
    }
    /* the camera's basis: x across the screen, y down it (the PS2's screen is y-down), z along
     * the view; upright ones keep y vertical */
    right = vec3(view->m[0], view->m[4], view->m[8]);
    down = vec3(-view->m[1], -view->m[5], -view->m[9]);
    fwd = vec3(-view->m[2], -view->m[6], -view->m[10]);
    if (x->flags & SPRITE_UPRIGHT) {
        fwd.y = 0.0f;
        fwd = vec3_norm(fwd);
        down = vec3(0.0f, -1.0f, 0.0f);
        right = vec3_norm(vec3_cross(down, fwd));
    }
    if (x->flags & SPRITE_QUARTER) {
        right = rot_y_quarter(right);
        down = rot_y_quarter(down);
    }
    for (i = 0; i < x->n; i++) {
        const Sprite *s = &x->s[i];
        float u0, v0, u1, v1, c = cosf(s->rot), sn = sinf(s->rot);
        int f = s->frame >= 0 && s->frame < x->frames ? s->frame : 0;
        Vec3 q[4];
        float st[4][2];
        static const int kTri[6] = {0, 1, 2, 2, 1, 3};

        if (x->uv) {
            u0 = x->u0;
            v0 = x->v0;
            u1 = x->u1;
            v1 = x->v1;
        } else {
            cell(x, f, &u0, &v0, &u1, &v1);
        }
        for (k = 0; k < 4; k++) {
            float lx, ly, lz = 0.0f, rx, ry;

            if (x->flags & SPRITE_CORNERS) {
                lx = x->corner[k][0];
                ly = x->corner[k][1];
                lz = x->corner[k][2];
            } else {
                lx = (k & 1 ? 1.0f : -1.0f) + x->cx;
                ly = (k & 2 ? 1.0f : -1.0f) + x->cy;
            }
            lx *= s->w;
            ly *= s->h;
            rx = c * lx - sn * ly;   /* (sceVu0RotMatrixZ) */
            ry = sn * lx + c * ly;
            if (s->own) {
                q[k] = vec3(s->c[k][0], s->c[k][1], s->c[k][2]);
            } else if (x->flags & SPRITE_CORNERS) {   /* not turned to the camera */
                q[k] = vec3(s->x + rx, s->y + ry, s->z + lz);
            } else {
                q[k] = vec3_add(vec3(s->x, s->y, s->z), vec3_add(vec3_scale(right, rx), vec3_scale(down, ry)));
            }
            st[k][0] = k & 1 ? u1 : u0;
            st[k][1] = k & 2 ? v1 : v0;
        }
        for (k = 0; k < 6; k++) {
            MeshVertex *v = &sVerts[nv++];
            int j = kTri[k];

            memset(v, 0, sizeof(*v));
            v->x = q[j].x;
            v->y = q[j].y;
            v->z = q[j].z;
            v->s = st[j][0];
            v->t = st[j][1];
            memcpy(v->rgba, s->rgba, 4);
        }
    }
    render_mesh_update(&x->gpu, sVerts, nv);   /* (replaceable storage: rebuilt each frame) */
    memset(&d, 0, sizeof(d));
    d.first = 0;
    d.count = nv;
    d.texture = 0;
    d.part = MESH_SEE_THROUGH;
    d.blend = !(x->flags & SPRITE_ADD);
    d.additive = (x->flags & SPRITE_ADD) != 0;
    d.no_zwrite = 1;
    if (x->flags & SPRITE_OPAQUE) {
        d.blend = d.additive = 0;
        d.no_zwrite = 0;
    }
    {
        static const uint32_t kAll[8] = {~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u};

        render_mesh(&x->gpu, view_proj, &d, 1, &tex, 1, kAll);
    }
    return 1;
}

static int by_layer(const void *a, const void *b) {
    const Batch *x = *(Batch *const *)a, *y = *(Batch *const *)b;

    return x->layer - y->layer;
}

void sprites_draw(const Mat4 *view_proj, const Mat4 *view) {
    Batch *order[SPRITE_BATCHES];
    int b, n = 0;

    for (b = 0; b < SPRITE_BATCHES; b++) {
        if (batch(b) != NULL && sBatch[b]->n > 0) {
            order[n++] = sBatch[b];
        }
    }
    qsort(order, (size_t)n, sizeof(order[0]), by_layer);
    for (b = 0; b < n; b++) {
        draw_batch(order[b], view_proj, view);
    }
}
