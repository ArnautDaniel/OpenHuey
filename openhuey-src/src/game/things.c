/* Things lying about: see things.h. The motion is the original's (src/game/placed.c: Ball_Flying /
 * ball_fly, Thing01_Flying, ball_roll, the dropping states); the models are GAME_FIX.GFM's, drawn as
 * FixModel_Draw does (texture 2 of GAME_FIX, the part's palette plus the kind's). */
#include "things.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../core/files.h"
#include "../render/render.h"
#include "engine.h"
#include "sprites.h"

#define TRI_HOLE 0x10000000u
#define THING_MASK 0x20020008u   /* (the floor they keep off: as the characters') */

typedef struct Thing {
    int used, kind, room, tri, state;
    int spent;             /* burst or gone: kept a frame for its owner to see why, then freed */
    int age, steps, events, turn, fading;
    float alpha;
    Vec3 at, v, was;
} Thing;

static Thing sThings[THINGS_MAX];

/* ---- the kinds ---- */

static float gravity_of(int kind) { return kind == 1 || kind == 4 || kind == 6 ? -0.1f : -0.2f; }
static int bursts(int kind) { return kind == 1 || kind == 4 || kind == 6; }   /* the powders: break on what they hit */
static float keep_of(int kind) {   /* how much of its speed a bounce keeps */
    switch (kind) {
    case 3: case 10: return 0.2f;
    case 9: return 0.4f;
    default: return 0.7f;
    }
}
static int bounce_sound(int kind) { return kind != 9 && kind != 10; }

/* ---- small things ---- */

static uint32_t sSeed = 0x6A09E667u;
static int irnd(int n) {
    sSeed = sSeed * 1103515245u + 12345u;
    return (int)((sSeed >> 8) & 0x7FFF) % n;
}
static float dot3(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static float len3(Vec3 a) { return sqrtf(dot3(a, a)); }
static Vec3 scale3(Vec3 a, float k) { return vec3(a.x * k, a.y * k, a.z * k); }
static NavMesh *nav(void) { return &gEngine.room.nav; }
static Thing *get(int i) { return i >= 0 && i < THINGS_MAX && sThings[i].used ? &sThings[i] : NULL; }
static float floor_at(int tri, Vec3 p) { return navmesh_height(nav(), tri, p.x, p.z); }

/* ---- the things ---- */

int things_new(int kind, int room, int tri, Vec3 at, Vec3 v) {
    int i;

    if (kind < 0 || kind > 10) {
        return -1;
    }
    for (i = 0; i < THINGS_MAX; i++) {
        Thing *t = &sThings[i];

        if (!t->used) {
            float h;

            memset(t, 0, sizeof(*t));
            t->used = 1;
            t->kind = kind;
            t->room = room;
            t->tri = tri >= 0 ? tri : navmesh_find(nav(), at, 1000.0f, &h);
            t->at = t->was = at;
            t->v = v;
            t->alpha = 1.0f;
            t->turn = irnd(256);
            return i;
        }
    }
    return -1;
}
void things_free(int i) {
    if (get(i) != NULL) {
        sThings[i].used = 0;
    }
}
void things_clear(void) { memset(sThings, 0, sizeof(sThings)); }
int things_used(int i) { return get(i) != NULL; }
int things_spent(int i) { return get(i) ? sThings[i].spent : 0; }
int things_kind(int i) { return get(i) ? sThings[i].kind : -1; }
int things_room(int i) { return get(i) ? sThings[i].room : -1; }
int things_tri(int i) { return get(i) ? sThings[i].tri : -1; }
int things_state(int i) { return get(i) ? sThings[i].state : -1; }
int things_age(int i) { return get(i) ? sThings[i].age : 0; }
void things_set_age(int i, int age) {
    if (get(i)) {
        sThings[i].age = age;
    }
}
int things_turn(int i) { return get(i) ? sThings[i].turn : 0; }
Vec3 *things_at(int i) { return get(i) ? &sThings[i].at : NULL; }
Vec3 *things_v(int i) { return get(i) ? &sThings[i].v : NULL; }
void things_place(int i, int room, int tri, Vec3 at) {
    Thing *t = get(i);

    if (t == NULL) {
        return;
    }
    t->room = room;
    t->tri = tri;
    t->at = t->was = at;
    t->v = vec3(0, 0, 0);
    t->state = THING_ROLLING;
}
void things_kick(int i, Vec3 dv) {   /* thing_kick_up */
    Thing *t = get(i);

    if (t == NULL) {
        return;
    }
    t->v = vec3_add(t->v, dv);
    t->at.y += 0.1f;
    t->state = THING_FLYING;
}
void things_fade(int i) {
    if (get(i)) {
        sThings[i].fading = 1;
    }
}
int things_events(int i) { return get(i) ? sThings[i].events : 0; }

/* ---- its motion ---- */

static void drop_through(Thing *t) {   /* ball_drop */
    t->state = THING_DROPPING;
    t->steps = 0;
}

/* a bounce at `at` off a surface of normal n: the velocity mirrored, `keep` of it kept; `to` the
 * rest of the step from `at` (0: nothing left) - ball_bounce */
static int bounce(Thing *t, Vec3 *to, Vec3 at, Vec3 n) {
    float speed = len3(t->v), all, left, k;
    Vec3 v;

    if (speed <= 0.0f) {
        return 0;
    }
    v = scale3(t->v, 1.0f / speed);
    v = vec3_sub(v, scale3(n, 2.0f * dot3(n, v)));
    t->v = scale3(v, keep_of(t->kind) * speed);
    all = len3(vec3_sub(t->at, *to));
    left = len3(vec3_sub(t->at, at));
    if (all <= 0.0f) {
        return 0;
    }
    k = 1.0f - left / all;
    if (k <= 0.0f) {
        return 0;
    }
    *to = vec3_add(at, scale3(t->v, k));
    return 1;
}

static void fly(Thing *t) {   /* ball_fly / Thing01_Flying */
    int again, bounced = 0;

    t->v.y += gravity_of(t->kind);
    if (bursts(t->kind)) {   /* (the powders: slowed sideways, and whatever they hit breaks them) */
        Vec3 out, n, to;
        int what, tri;

        t->v.x *= 0.95f;
        t->v.z *= 0.95f;
        to = vec3_add(t->at, t->v);
        tri = navmesh_fly(nav(), t->tri, t->at, to, THING_MASK, &out, &n, &what);
        if (tri < 0) {
            t->events |= THING_BURST;
            t->spent = 1;
            return;
        }
        if (what == NAV_REACHED) {
            float fl = floor_at(tri, out);

            if (out.y < fl) {
                t->at = vec3(out.x, fl, out.z);
                t->events |= THING_BURST;
                t->spent = 1;
                return;
            }
            t->tri = tri;
            t->at = out;
            return;
        }
        if (what == NAV_HOLE) {
            t->tri = tri;
            t->at = out;
            drop_through(t);
            return;
        }
        t->at = out;
        t->events |= THING_BURST;
        t->spent = 1;
        return;
    }
    for (again = 0; again < 2; again++) {
        Vec3 to = vec3_add(t->at, t->v), from = t->at, out, n;
        int tri = t->tri, what, k;

        for (k = 0; k < 8; k++) {
            int r = navmesh_fly(nav(), tri, from, to, THING_MASK, &out, &n, &what);

            if (r < 0) {
                break;
            }
            if (what == NAV_REACHED) {
                float fl = floor_at(r, out);

                if (out.y < fl) {
                    t->state = THING_ROLLING;
                    out.y = fl;
                }
                t->tri = r;
                t->at = out;
                if (bounced && bounce_sound(t->kind)) {
                    t->events |= THING_BOUNCED;
                }
                return;
            }
            if (what == NAV_HOLE) {
                t->tri = r;
                t->at = out;
                drop_through(t);
                return;
            }
            if (!bounce(t, &to, out, n)) {
                t->at = vec3(t->at.x, floor_at(t->tri, t->at), t->at.z);
                t->state = THING_ROLLING;
                return;
            }
            if (fabsf(dot3(n, t->v)) < 0.5f * len3(t->v)) {   /* a glancing hit: down to the floor */
                out.y = floor_at(r, out);
                t->state = THING_ROLLING;
                if (t->kind == 3) {
                    t->events |= THING_PUFF;
                }
            }
            bounced = 1;
            if (t->state == THING_ROLLING) {
                t->tri = r;
                t->at = out;
                if (bounce_sound(t->kind)) {
                    t->events |= THING_BOUNCED;
                }
                return;
            }
            tri = r;
            from = out;
        }
        t->v = scale3(t->v, t->kind == 3 ? -0.2f : -0.8f);   /* (once more the other way) */
    }
    t->at.y = floor_at(t->tri, t->at);   /* (no way: on the floor) */
    t->state = THING_ROLLING;
}

static void roll(Thing *t) {   /* ball_roll */
    Vec3 p, n, fr;
    float c, flat, k, h, sp;
    int tri;

    p = navmesh_move(nav(), t->at, t->v.x, t->v.z, 3.0f, 0.0f);
    tri = navmesh_find(nav(), p, 3.0f, &h);
    if (tri >= 0) {
        t->tri = tri;
        p.y = h;
    }
    t->at = p;
    if (t->tri >= 0 && (nav()->tris[t->tri].flags & TRI_HOLE)) {
        drop_through(t);
        return;
    }
    t->v = vec3_sub(t->at, t->was);
    /* NavTri_Slope: c the sine of its steepness (0 level), n the way down the slope scaled by it
     * (the up-facing normal's (0, -1, 0) left in its plane) */
    n = navmesh_normal(nav(), t->tri);
    flat = n.y;
    c = sqrtf(fmaxf(0.0f, 1.0f - flat * flat));
    n = vec3(n.x * n.y, n.y * n.y - 1.0f, n.z * n.y);
    sp = len3(t->v);
    fr = sp > 0.0f ? scale3(t->v, 0.03f * flat / sp) : vec3(0, 0, 0);   /* (friction: most on the level) */
    if (len3(fr) <= sp) {
        t->v = vec3_sub(t->v, fr);
    } else {
        t->v = vec3(0, 0, 0);
    }
    k = fabsf(c * gravity_of(t->kind));
    t->v = vec3_add(t->v, scale3(n, k));
    t->v.y = 0.0f;   /* (it keeps to the floor) */
    if (len3(t->v) < 0.005f) {
        t->v = vec3(0, 0, 0);
    }
}

static void drop(Thing *t) {   /* the dropping state: through for 31 frames, then gone */
    t->v.y += gravity_of(t->kind);
    t->at = vec3_add(t->at, t->v);
    if (++t->steps >= 31) {
        t->events |= THING_GONE;
        t->spent = 1;
    }
}

void things_step(int room, int running) {
    int i;

    for (i = 0; i < THINGS_MAX; i++) {
        Thing *t = &sThings[i];

        if (!t->used) {
            continue;
        }
        if (t->spent) {   /* (its owner has seen it go) */
            t->used = 0;
            continue;
        }
        t->events = 0;
        if (running && t->age != -1) {
            t->age++;
        }
        if (t->fading) {
            t->alpha -= 1.0f / 6.0f;
            if (t->alpha <= 0.0f) {
                t->used = 0;
                continue;
            }
        }
        if (t->room != room) {   /* (out of the room being played: still) */
            continue;
        }
        switch (t->state) {
        case THING_FLYING: fly(t); break;
        case THING_ROLLING: roll(t); break;
        default: drop(t); break;
        }
        t->was = t->at;
    }
}

/* ---- drawing: the ball and the powders as sprites, the others GAME_FIX.GFM models ---- */

static int sBall = -1, sPowder = -1;
static uint8_t *sGfm;
static size_t sGfmSize;
typedef struct GfmModel {
    int loaded;
    GpuMesh gpu;
    MeshDraw d;
    int csa;               /* its part's palette */
} GfmModel;
static GfmModel sModels[4];

/* a GFM model's first part (FixModel_Draw, gl_gfm_part): a strip of vertices - positions as
 * running differences from +0x20 (/ 16), UVs / 32768, colours, skip flags - as a triangle list */
static void gfm_load(int m) {
    GfmModel *g = &sModels[m];
    const uint8_t *part;
    int32_t off, nv, row[3], i, n = 0;
    const uint16_t *uv;
    const uint8_t *col, *flags;
    const int16_t *pos;
    MeshVertex *strip, *tris;

    g->loaded = -1;
    if (sGfm == NULL) {
        sGfm = files_read("GAME_FIX.GFM", &sGfmSize);
    }
    if (sGfm == NULL || (size_t)(m + 1) * 4 > sGfmSize) {
        return;
    }
    memcpy(&off, sGfm + m * 4, 4);
    if (off <= 0 || (size_t)off + 0x30 > sGfmSize) {
        return;
    }
    part = sGfm + off;
    memcpy(&nv, part, 4);
    memcpy(&g->csa, part + 4, 4);
    if (nv <= 2) {
        return;
    }
    {
        int32_t o[4];

        memcpy(o, part + 0x10, 16);
        uv = (const uint16_t *)(part + o[0]);
        col = part + o[1];
        pos = (const int16_t *)(part + o[2]);
        flags = part + o[3];
    }
    memcpy(row, part + 0x20, 12);
    strip = malloc((size_t)nv * sizeof(MeshVertex));
    tris = malloc((size_t)nv * 3 * sizeof(MeshVertex));
    for (i = 0; i < nv; i++) {
        MeshVertex *v = &strip[i];

        row[0] += pos[i * 3];
        row[1] += pos[i * 3 + 1];
        row[2] += pos[i * 3 + 2];
        memset(v, 0, sizeof(*v));
        v->x = (float)row[0] / 16.0f;
        v->y = (float)row[1] / 16.0f;
        v->z = (float)row[2] / 16.0f;
        v->s = (float)uv[i * 2] / 32768.0f;
        v->t = (float)uv[i * 2 + 1] / 32768.0f;
        memcpy(v->rgba, col + i * 4, 4);
        v->ny = 1.0f;
    }
    for (i = 2; i < nv; i++) {   /* (a set flag: the triangle ending there skipped) */
        if (flags[i]) {
            continue;
        }
        tris[n++] = strip[i - 2];
        tris[n++] = strip[i - 1];
        tris[n++] = strip[i];
    }
    render_mesh_upload(&g->gpu, tris, n);
    memset(&g->d, 0, sizeof(g->d));
    g->d.first = 0;
    g->d.count = n;
    g->d.texture = 0;
    g->d.blend = 1;
    g->loaded = 1;
    free(strip);
    free(tris);
}

static Mat4 placed_at(Vec3 p, float rx, float ry, float rz) {   /* (as the room's objects: z y x turns) */
    float cx = cosf(rx), sx = sinf(rx), cy = cosf(ry), sy = sinf(ry), cz = cosf(rz), sz = sinf(rz);
    Mat4 mx = mat4_identity(), my = mat4_identity(), mz = mat4_identity(), t = mat4_identity();

    mx.m[5] = cx;  mx.m[6] = sx;  mx.m[9] = -sx;  mx.m[10] = cx;
    my.m[0] = cy;  my.m[2] = -sy; my.m[8] = sy;   my.m[10] = cy;
    mz.m[0] = cz;  mz.m[1] = sz;  mz.m[4] = -sz;  mz.m[5] = cz;
    t.m[12] = p.x;
    t.m[13] = p.y;
    t.m[14] = p.z;
    return mat4_mul(t, mat4_mul(mz, mat4_mul(my, mx)));
}

static void model_draw(int m, int palette, Mat4 place, const Mat4 *view_proj, float alpha) {
    static const uint32_t kAll[8] = {~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u};
    GfmModel *g = &sModels[m];
    GpuTexture tex;
    int w, h;
    Mat4 mvp;

    if (g->loaded == 0) {
        gfm_load(m);
    }
    if (g->loaded != 1) {
        return;
    }
    tex = sprites_texture_of(0x10, 2, (g->csa + palette) & 0x1F, &w, &h);
    mvp = mat4_mul(*view_proj, place);
    render_mesh_tint(1.0f, 1.0f, 1.0f, alpha);
    render_mesh(&g->gpu, &mvp, &g->d, 1, &tex, 1, kAll);
    render_mesh_tint(1.0f, 1.0f, 1.0f, 1.0f);
}

static void sprite_batches(void) {
    if (sBall < 0) {
        sBall = sprites_new();
        if (sBall >= 0) {   /* (480, 64), palette 2 */
            sprites_texture(sBall, 0x10, 1, 2);
            sprites_cells(sBall, 0x1E0, 0x40, 32, 32, 512, 256, 1);
            sprites_flags(sBall, 0, 0x19);
        }
    }
    if (sPowder < 0) {
        sPowder = sprites_new();
        if (sPowder >= 0) {   /* (128, 64) */
            sprites_texture(sPowder, 0x10, 1, -1);
            sprites_cells(sPowder, 0x80, 0x40, 32, 32, 512, 256, 1);
            sprites_flags(sPowder, 0, 0x19);
        }
    }
}

static void sprite_of(Sprite *r, Vec3 at, float size, int cr, int cg, int cb, float alpha) {
    memset(r, 0, sizeof(*r));
    r->x = at.x;
    r->y = at.y;
    r->z = at.z;
    r->w = r->h = size;
    r->rgba[0] = (uint8_t)cr;
    r->rgba[1] = (uint8_t)cg;
    r->rgba[2] = (uint8_t)cb;
    r->rgba[3] = (uint8_t)(128.0f * alpha);
}

void things_draw(int room, const Mat4 *view_proj) {
    Sprite *ball, *powder;
    int i, nb = 0, np = 0;

    sprite_batches();
    ball = sBall >= 0 ? sprites_records(sBall, THINGS_MAX) : NULL;
    powder = sPowder >= 0 ? sprites_records(sPowder, THINGS_MAX) : NULL;
    for (i = 0; i < THINGS_MAX; i++) {
        Thing *t = &sThings[i];
        float turn;

        if (!t->used || t->room != room) {
            continue;
        }
        turn = (360.0f * (float)t->turn / 256.0f - 180.0f) * 3.14159265f / 180.0f;
        switch (t->kind) {
        case 0:   /* the ball: a unit up, yellowish */
            if (ball != NULL) {
                sprite_of(&ball[nb++], vec3(t->at.x, t->at.y + 1.0f, t->at.z), 0.675f, 128, 128, 64, t->alpha);
            }
            break;
        case 1: case 4: case 6:   /* the powders: purple, orange, white */
            if (powder != NULL) {
                int c = t->kind == 1 ? 0 : t->kind == 4 ? 1 : 2;
                static const int kCol[3][3] = {{64, 32, 128}, {128, 100, 0}, {128, 128, 128}};

                sprite_of(&powder[np++], t->at, 0.5f, kCol[c][0], kCol[c][1], kCol[c][2], t->alpha);
            }
            break;
        case 2: case 5: case 7:   /* the magnesias: model 1, palettes 0 / 1 / 2 */
            model_draw(1, t->kind == 2 ? 0 : t->kind == 5 ? 1 : 2, placed_at(t->at, 0.0f, turn, 0.0f), view_proj, t->alpha);
            break;
        case 3: {   /* the doll: model 2 lying flat, 0.3 up; its last 8 frames fading */
            float a = t->alpha;

            if (t->age > 8992) {
                a *= (float)(9000 - t->age) / 8.0f;
            }
            model_draw(2, 0, placed_at(vec3(t->at.x, t->at.y + 0.3f, t->at.z), -1.5707964f, 0.0f, turn), view_proj, a);
            break;
        }
        case 8:   /* the stone: model 3 */
            model_draw(3, 0, placed_at(t->at, 0.0f, turn, 0.0f), view_proj, t->alpha);
            break;
        default:
            break;
        }
    }
    if (sBall >= 0) {
        sprites_records(sBall, nb);
    }
    if (sPowder >= 0) {
        sprites_records(sPowder, np);
    }
}
