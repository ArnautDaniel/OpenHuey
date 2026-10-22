/* hg2: start-up and the main loop.
 *
 *   hg2 [options] [data-dir]
 *     --hidden          no window shown (tests, screenshots)
 *     --frames N        stop after N frames
 *     --eval "code"     run Forth after the scripts have loaded (repeatable)
 *     --import disc.iso the disc imported into the library beside the executable, then out
 *     --mod folder|zip  a mod turned on (repeatable; with --test the only mods)
 *     --mods folder     the folder of mods (instead of mods/ beside the library)
 *     --title           the title even without a window (its tests; else, hidden, a new game at once)
 *     --update          a release updated to the newest build (platform/update.c), then out
 *
 * The game data: a disc image or folder named (read as it is), else the library
 * (platform/library.c), else - with a window - the player is asked for the disc to import.
 *
 * Start-up: the window, the renderer, the Forth system with the engine's words, then
 * scripts/prelude.fs and scripts/game.fs - which decide everything else. Each frame: input,
 * game ticks at a fixed 30 a second, the original's frame rate (Forth tasks and hooks), then drawing
 * (only after a tick: the picture is 30 frames a second too). */
#include "core/files.h"
#include "forth/forth.h"
#include "game/engine.h"
#include "game/particles.h"
#include "game/progress.h"
#include "game/things.h"
#include "game/sprites.h"
#include "game/mods.h"
#include "data/exetables.h"
#include "game/areas.h"
#include "game/exits.h"
#include "hactor/hactor.h"
#include "platform/disc.h"
#include "platform/library.h"
#include "platform/snddrv.h"
#include "platform/update.h"
#include "platform/sound.h"
#include "platform/platform.h"
#include "platform/movie.h"
#include "render/render.h"
#include "render/post.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EVALS 16

static int sTestStatus;
static void p_exit_status(Forth *f, Word *w) {   /* ( n -- ) a test's exit status (--test) */
    (void)w;
    sTestStatus = (int)forth_pop(f);
}
static void p_error_count(Forth *f, Word *w) {   /* ( -- n ) errors raised (not in `catch`) so far */
    (void)w;
    forth_push(f, f->errors);
}

typedef struct Options {
    int hidden;
    long frames;   /* 0: run until closed */
    const char *evals[MAX_EVALS];
    int nevals;
    const char *data;
    const char *import; /* --import DISC: the disc imported into the library, then out */
    int update;         /* --update: updated to the newest release, then out */
    int title;          /* --title: the title, hidden too (tests) */
    const char *test;   /* --test FILE: run it after the scripts, headless, and exit with its status */
    const char *mods[MAX_EVALS];   /* --mod DIR: a mod's folder, on (the tests' only mods) */
    const char *mods_folder;       /* --mods DIR: the folder of mods instead of <base>mods/ (the tests' too) */
    int nmods;
} Options;

static int parse_options(int argc, char **argv, Options *o) {
    int i;

    memset(o, 0, sizeof(*o));
    o->data = getenv("HG_DATA");   /* (none: the disc chosen before, or ask) */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--hidden") == 0) {
            o->hidden = 1;
        } else if (strcmp(argv[i], "--frames") == 0 && i + 1 < argc) {
            o->frames = atol(argv[++i]);
        } else if (strcmp(argv[i], "--eval") == 0 && i + 1 < argc && o->nevals < MAX_EVALS) {
            o->evals[o->nevals++] = argv[++i];
        } else if (strcmp(argv[i], "--import") == 0 && i + 1 < argc) {
            o->import = argv[++i];
        } else if (strcmp(argv[i], "--title") == 0) {
            o->title = 1;
        } else if (strcmp(argv[i], "--update") == 0) {
            o->update = 1;
        } else if (strcmp(argv[i], "--mods") == 0 && i + 1 < argc) {
            o->mods_folder = argv[++i];
        } else if (strcmp(argv[i], "--mod") == 0 && i + 1 < argc && o->nmods < MAX_EVALS) {
            o->mods[o->nmods++] = argv[++i];
        } else if (strcmp(argv[i], "--test") == 0 && i + 1 < argc) {
            o->test = argv[++i];
            o->hidden = 1;
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "usage: openhuey [--hidden] [--frames N] [--eval code] [--test file.fs] [--mod folder|zip] [--mods folder] [--import disc.iso] [--update] [--title] [disc.iso | data folder]\n");
            return 0;
        } else {
            o->data = argv[i];
        }
    }
    return 1;
}

/* the tables only the game's executable has (data/exetables.h): the library's, else read from the
 * executable - on the disc ($HG_EXE, or beside the data folder), else the repository's baserom
 * (the developers') */
static void load_world(World *w) {
    DiscFile *f = files_exist("../exe/doors.bin") ? NULL : disc_exe();
    uint8_t *data = NULL;
    size_t n = 0;
    Exe exe;

    if (files_exist("../exe/doors.bin")) {   /* (a library's: no executable wanted) */
        if (!world_load(w)) {
            fprintf(stderr, "openhuey: the library's exe/doors.bin or rooms.bin is damaged: rooms won't connect\n");
        }
        return;
    }
    if (f != NULL) {
        n = (size_t)disc_file_size(f);
        data = malloc(n);
        if (data != NULL && disc_file_read(f, data, n) != n) {
            free(data);
            data = NULL;
        }
        disc_file_close(f);
    }
    if (data == NULL && HG2_DEFAULT_EXE[0] != 0) {
        data = SDL_LoadFile(HG2_DEFAULT_EXE, &n);
    }
    if (exe_take(&exe, data, n)) {
        exetables_use_exe(&exe);
    }
    if (!world_load(w)) {
        fprintf(stderr, "openhuey: no SLUS_210.75 found (set HG_EXE): rooms won't connect\n");
    }
}

/* the scripts: $HG2_SCRIPTS, the repository's (a developer's build), or beside the executable (a
 * release: scripts/ next to it) */
static const char *script_dir(void) {
    static char dir[1024];
    char probe[1100];

    if (getenv("HG2_SCRIPTS") != NULL) {
        return getenv("HG2_SCRIPTS");
    }
    snprintf(probe, sizeof(probe), "%s/prelude.fs", HG2_SCRIPT_DIR);
    if (HG2_SCRIPT_DIR[0] != 0 && SDL_GetPathInfo(probe, NULL)) {
        return HG2_SCRIPT_DIR;
    }
    snprintf(dir, sizeof(dir), "%sscripts", SDL_GetBasePath() != NULL ? SDL_GetBasePath() : "./");
    return dir;
}

static int load_scripts(Forth *f) {
    const char *dir = script_dir();
    char path[1024];

    snprintf(path, sizeof(path), "%s/prelude.fs", dir);
    if (forth_include(f, path) != 0) {
        return 0;
    }
    forth_add_root(f, dir);   /* where USING: finds vocabularies */
    snprintf(path, sizeof(path), "%s/game.fs", dir);
    if (forth_include(f, path) != 0) {
        return 0;
    }
    mods_load_scripts(f, dir);   /* (a mod's failing is said, the game goes on) */
    return 1;
}

/* the room's lights for an actor, and its shadows: from its two lights with the most of the light
 * on it, each as dark as its share. The shares ease from frame to frame (SHARE_EASE), so walking
 * from one light to the next one shadow fades as the other comes - not one put out and another
 * cast at once where the brightest changes, or where a nav triangle's lights do */
#define SHADOWS_PER_ACTOR 2
#define SHADOWS_DRAWN 3    /* (one fading out as two are wanted) */
#define SHARE_EASE 0.15f   /* (of the way to the new share each frame: most of it in half a second) */
#define SHARE_LEAST 0.04f  /* (less: no shadow) */
#define SHOWN_STEP 0.05f   /* (a shadow's darkness changes at most this a frame: a fade, never a jump) */

/* a light's brightness at a point, as room_lights_at weighs it (0: none there) */
static float light_score(const Room *r, int i, Vec3 at, uint32_t mask) {
    const RoomLight *l = &r->lights[i];
    float score = (0.3f * l->color.x + 0.6f * l->color.y + 0.1f * l->color.z) * l->intensity, d;

    if (!(mask >> i & 1)) {
        return 0.0f;
    }
    if (l->range > 0.0f) {
        d = vec3_len(vec3_sub(at, l->pos));
        score = d < l->range ? score * (l->range - d) / l->range : 0.0f;
    }
    return fmaxf(score, 0.0f);
}

/* a shadow from light l (a view from it, fitted round the actor and reaching the floor beyond) */
static void cast_shadow(Actor *a, const RoomLight *l, Vec3 center, float radius, float strength) {
    Vec3 dir = vec3_sub(center, l->pos), up;
    float dist = vec3_len(dir);
    Mat4 proj, view, vp;
    int slot;

    if (dist < radius * 1.5f) {
        return;   /* the light is inside it */
    }
    dir = vec3_scale(dir, 1.0f / dist);
    up = fabsf(dir.y) > 0.95f ? vec3(1, 0, 0) : vec3(0, 1, 0);
    proj = mat4_perspective(2.0f * atanf(radius * 1.25f / dist), 1.0f, fmaxf(1.0f, dist - radius * 2.0f),
                            dist + radius * 6.0f + 200.0f);
    view = mat4_look(l->pos, dir, up);
    vp = mat4_mul(proj, view);
    slot = render_shadow_add(&vp, center, radius, strength);
    if (slot >= 0 && !a->shadow_off) {
        render_shadow_mesh(&a->gpu, a->model.d, a->model.nd);
    }
}

static void light_actor(Engine *e, Actor *a) {
    const Room *r = &e->room;
    Vec3 center = actor_center(a);
    float radius = actor_radius(a), score[16] = {0}, wanted[16], total = 0.0f, base, lum, fall = 1.0f, h, cut, sum;
    const RoomLight *l;
    int pick[SHADOWS_PER_ACTOR + 1], drawn[SHADOWS_DRAWN], tri, i, j, k, n = r->nlights < 16 ? r->nlights : 16;
    uint32_t mask;

    if (e->nstage > 0) {   /* (a stage: its lights, and no shadow map - there is nothing to fall on) */
        a->lights[0] = 0;
        a->lights[1] = e->nstage > 1 ? 1 : -1;
        a->lights[2] = e->nstage > 2 ? 2 : -1;
        a->shadow_room = 0;
        return;
    }
    room_lights_at(r, center, a->lights);
    tri = navmesh_find(&r->nav, center, 10.0f, &h);
    mask = tri >= 0 ? r->nav.tris[tri].lights : (r->nlights > 0 ? 1u : 0u);
    for (i = 0; i < n; i++) {
        score[i] = light_score(r, i, center, mask);
        total += score[i];
    }
    if (a->shadow_room != r->id + 1) {   /* (a new room, or new to it: the shares as they are) */
        for (i = 0; i < 16; i++) {
            a->shadow_share[i] = total > 0.0f && i < n ? score[i] / total : 0.0f;
        }
    } else {
        for (i = 0; i < 16; i++) {
            float want = total > 0.0f && i < n ? score[i] / total : 0.0f;

            a->shadow_share[i] += (want - a->shadow_share[i]) * SHARE_EASE;
        }
    }
    /* the shares' top two, and the next: the two wanted by how far each is above that third, so a
     * light taking over second place starts from nothing as the one it passes goes to nothing */
    for (k = 0; k <= SHADOWS_PER_ACTOR; k++) {
        pick[k] = -1;
        for (i = 0; i < n; i++) {
            int taken = 0, j;

            for (j = 0; j < k; j++) {
                taken |= pick[j] == i;
            }
            if (!taken && (pick[k] < 0 || a->shadow_share[i] > a->shadow_share[pick[k]])) {
                pick[k] = i;
            }
        }
    }
    for (i = 0; i < 16; i++) {
        wanted[i] = 0.0f;
    }
    cut = pick[SHADOWS_PER_ACTOR] >= 0 ? a->shadow_share[pick[SHADOWS_PER_ACTOR]] : 0.0f;
    for (sum = 0.0f, k = 0; k < SHADOWS_PER_ACTOR; k++) {
        sum += pick[k] >= 0 ? fmaxf(0.0f, a->shadow_share[pick[k]] - cut) : 0.0f;
    }
    if (pick[0] >= 0 && a->shadow_share[pick[0]] >= SHARE_LEAST && sum > 0.0f) {
        /* as dark as before from the brightest (a dim room's shadows lighter), times each one's part */
        l = &r->lights[pick[0]];
        if (l->range > 0.0f) {
            fall = fmaxf(0.0f, 1.0f - vec3_len(vec3_sub(center, l->pos)) / l->range);
        }
        lum = (0.3f * l->color.x + 0.6f * l->color.y + 0.1f * l->color.z) * l->intensity / 128.0f;
        base = gRender.shadow_strength * fminf(1.0f, fmaxf(0.6f, lum * fall * 4.0f)) *
               (e->extra.on ? fminf(1.0f, e->extra.scale) : 1.0f);   /* (a scene's dimmed lights cast less) */
        for (k = 0; k < SHADOWS_PER_ACTOR; k++) {
            if (pick[k] >= 0) {   /* (two lights alike: each a little lighter than one alone) */
                wanted[pick[k]] = base * fminf(1.0f, fmaxf(0.0f, a->shadow_share[pick[k]] - cut) / sum * 1.5f);
            }
        }
    }
    for (i = 0; i < 16; i++) {
        if (a->shadow_room != r->id + 1) {
            a->shadow_shown[i] = wanted[i];
        } else {
            a->shadow_shown[i] += fmaxf(-SHOWN_STEP, fminf(SHOWN_STEP, wanted[i] - a->shadow_shown[i]));
        }
    }
    a->shadow_room = r->id + 1;
    /* drawn: the darkest few as they are shown */
    for (k = 0; k < SHADOWS_DRAWN; k++) {
        int best = -1;

        for (i = 0; i < n; i++) {
            if (a->shadow_shown[i] > 0.01f && (best < 0 || a->shadow_shown[i] > a->shadow_shown[best])) {
                best = i;
            }
        }
        if (best < 0) {
            break;
        }
        cast_shadow(a, &r->lights[best], center, radius, a->shadow_shown[best]);
        drawn[k] = best;
        a->shadow_shown[best] = -a->shadow_shown[best];   /* (taken: not again this frame) */
    }
    for (j = 0; j < k; j++) {
        a->shadow_shown[drawn[j]] = -a->shadow_shown[drawn[j]];
    }
}

/* the lights of the next lit draws: the actor's (or the fixed key light) */
/* a direction rotated by `angle` about a unit axis (Rodrigues) */
static Vec3 turn_about(Vec3 v, Vec3 axis, float angle) {
    float c = cosf(angle), s = sinf(angle);

    return vec3_add(vec3_add(vec3_scale(v, c), vec3_scale(vec3_cross(axis, v), s)),
                    vec3_scale(axis, vec3_dot(axis, v) * (1.0f - c)));
}

static void use_lights(Engine *e, const Actor *a) {
    DrawLight lights[3];
    Vec3 ambient = e->nstage > 0 ? e->stage_ambient : e->room.ambient;
    int n = 0, k;

    for (k = 0; k < 3; k++) {
        if (a->lights[k] >= 0) {
            const RoomLight *l = e->nstage > 0 ? &e->stage[a->lights[k]] : &e->room.lights[a->lights[k]];

            lights[n].pos = l->pos;
            lights[n].color = vec3_scale(l->color, l->intensity);
            lights[n].range = l->range;
            n++;
        }
    }
    if (e->extra.on && e->nstage == 0) {   /* (Lights_Scripted: the camera's lights into slots 2, then 1) */
        Vec3 eye = e->camera.pos, fwd = camera_forward(&e->camera), up = vec3(0, 1, 0);
        int slot = 2;

        for (k = 0; k < n; k++) {
            lights[k].color = vec3_scale(lights[k].color, e->extra.scale);
        }
        for (; n < 3; n++) {
            lights[n].pos = eye;
            lights[n].color = vec3(0, 0, 0);
            lights[n].range = 0.0f;
        }
        ambient = vec3_add(ambient, e->extra.ambient);
        for (k = 0; k < 2; k++) {
            Vec3 d = vec3_scale(fwd, -1.0f), axis;

            if (!e->extra.cam[k].on) {
                continue;
            }
            axis = vec3_norm(vec3_cross(up, d));
            d = turn_about(d, axis, e->extra.cam[k].up);
            d = turn_about(d, up, e->extra.cam[k].about);
            lights[slot].pos = vec3_add(eye, vec3_scale(d, 100000.0f));   /* (a direction: far off, no falloff) */
            lights[slot].color = e->extra.cam[k].color;
            lights[slot].range = 0.0f;
            slot--;
        }
    }
    if (e->nstage > 0) {
        render_draw_lights(ambient, lights, n);
    } else {
        render_draw_lights(ambient, lights, e->room.nlights > 0 || e->extra.on ? n : -1);
    }
}

/* the characters again, mirrored in each reflecting quad's plane (Reflection_Draw: those on the
 * eye's side of it; drawn faded by its strength, without their contact shadows) */
/* the side panels of castle 2F's reflecting door and window (Reflection_SidePanels): by kind (the
 * door's, the window's), the one each side */
static const Vec3 kReflPanels[4][4] = {
    {{50.0f, 30.0f, -21.3043f}, {50.0f, 30.0f, -28.75f}, {50.0f, 1.25f, -21.3043f}, {50.0f, 1.25f, -28.75f}},
    {{31.3043f, 30.0f, -10.0f}, {38.75f, 30.0f, -10.0f}, {31.3043f, 1.5f, -10.0f}, {38.75f, 1.5f, -10.0f}},
    {{10.0f, 22.0f, 58.6957f}, {10.0f, 22.0f, 51.25f}, {10.0f, 0.0f, 58.6957f}, {10.0f, 0.0f, 51.25f}},
    {{-8.6957f, 22.0f, 70.0f}, {-1.25f, 22.0f, 70.0f}, {-8.6957f, 0.0f, 70.0f}, {-1.25f, 0.0f, 70.0f}},
};

/* the characters on the eye's side drawn mirrored (`mvp`), faded by k, only on quad q (unless
 * over the whole picture) */
static void reflect_into(Engine *e, const Mat4 *proj, const Mat4 *view, const Mat4 *vp, const Mat4 *mvp, Vec3 eye,
                         Vec3 n, float d, float side, float k, const Vec3 *q, int only) {
    int i;

    if (q != NULL) {
        render_reflect_begin(vp, q);
    }
    render_camera(proj, view, eye, e->camera.znear, e->camera.zfar);
    for (i = 0; i < MAX_ACTORS; i++) {
        Actor *a = &e->actors[i];
        float alpha = a->alpha;
        int off = a->shadow_off;

        if (!a->used || !a->visible || (vec3_dot(n, a->pos) - d) * side <= 0.0f || (only >= 0 && i != only)) {
            continue;
        }
        a->alpha = alpha * fminf(k, 1.0f);
        a->shadow_off = 1;
        use_lights(e, a);
        actor_draw(a, mvp);
        a->alpha = alpha;
        a->shadow_off = off;
    }
    render_camera(proj, view, e->camera.pos, e->camera.znear, e->camera.zfar);
    if (q != NULL) {
        render_reflect_end(vp, q);
    }
}

/* a point's screen x in the game's pixels (512 across); 0 if behind the eye */
static float screen_x(const Mat4 *vp, Vec3 p) {
    const float *m = vp->m;
    float x = m[0] * p.x + m[4] * p.y + m[8] * p.z + m[12], w = m[3] * p.x + m[7] * p.y + m[11] * p.z + m[15];

    return w > 0.0f ? 256.0f * (x / w + 1.0f) : 0.0f;
}

static int in_view(const Mat4 *vp, const Vec3 *q) {   /* (all four corners) */
    const float *m = vp->m;
    int k;

    for (k = 0; k < 4; k++) {
        float x = m[0] * q[k].x + m[4] * q[k].y + m[8] * q[k].z + m[12], y = m[1] * q[k].x + m[5] * q[k].y + m[9] * q[k].z + m[13];
        float z = m[2] * q[k].x + m[6] * q[k].y + m[10] * q[k].z + m[14], w = m[3] * q[k].x + m[7] * q[k].y + m[11] * q[k].z + m[15];

        if (x > w || x < -w || y > w || y < -w || z > w || z < -w) {
            return 0;
        }
    }
    return 1;
}

static void draw_reflections(Engine *e, const Mat4 *proj, const Mat4 *view, const Mat4 *vp) {
    int r;

    for (r = 0; r < 4; r++) {
        const Vec3 *q = e->refl[r].q;
        Vec3 n = vec3_norm(vec3_cross(vec3_sub(q[1], q[0]), vec3_sub(q[2], q[0])));
        float d = vec3_dot(n, q[0]), side = vec3_dot(n, e->camera.pos) - d, k = (float)e->refl[r].strength / 128.0f;
        Mat4 m = mat4_identity(), mvp;
        Vec3 eye;
        int c, w;

        if (e->refl[r].strength <= 0 || side == 0.0f) {
            continue;
        }
        for (c = 0; c < 3; c++) {   /* p' = p - 2 (n.p - d) n */
            const float nc[3] = {n.x, n.y, n.z};

            for (w = 0; w < 3; w++) {
                m.m[c * 4 + w] -= 2.0f * nc[w] * nc[c];
            }
            m.m[12 + c] = 2.0f * d * nc[c];
        }
        mvp = mat4_mul(*vp, m);
        eye = vec3_sub(e->camera.pos, vec3_scale(n, 2.0f * side));
        reflect_into(e, proj, view, vp, &mvp, eye, n, d, side, k, e->refl[r].screen ? NULL : q, e->refl[r].only);
        if (e->refl[r].panels != 0 && !e->refl[r].screen) {
            /* the panels: the reflection again at half, moved by the quad's width on screen */
            float dx = (float)((int)(screen_x(vp, q[1]) * 16.0f) - (int)(screen_x(vp, q[0]) * 16.0f)) / 16.0f;
            int s;

            for (s = 0; s < 2 && dx != 0.0f; s++) {
                float sx = s == 0 ? dx : -dx, t = sx / 256.0f;   /* (in clip x, times w) */
                const Vec3 *p = kReflPanels[(e->refl[r].panels == 2 ? 0 : 2) + (sx < 0.0f ? 0 : 1)];
                Mat4 moved = mvp;

                if (!in_view(vp, p)) {
                    continue;
                }
                for (c = 0; c < 4; c++) {   /* (row 0 += t x row 3) */
                    moved.m[c * 4] += t * moved.m[c * 4 + 3];
                }
                reflect_into(e, proj, view, vp, &moved, eye, n, d, side, k * 0.5f, p, e->refl[r].only);
            }
        }
    }
}

/* how far a character has faded going out through a doorway (Character_RegionFade): in an
 * exit's area, by how far past its middle toward the outside it stands - none on the room's side
 * of the middle, all at the area's outer edge (its depth along the exit's way through) */
float doorway_faded(const Engine *e, const Actor *a) {
    int exit;

    if (!a->doorway_fade || e->room.id < 0 || e->room.id >= WORLD_ROOMS) {
        return 0.0f;
    }
    for (exit = 0; exit < ROOM_EXITS; exit++) {
        int area = e->world.exits[e->room.id][exit].camera & 0xFFFF;
        Vec3 out, in, way, c0, c1, c2, edge, middle;
        float half, past;

        if (area == 0xFFFF || !area_inside(&e->room, area, a->pos)) {
            continue;
        }
        if (exit_spot(&e->room, &e->world, exit, 0, &out) < 0 || exit_spot(&e->room, &e->world, exit, 1, &in) < 0 ||
            !area_corner(&e->room, area, 0, &c0) || !area_corner(&e->room, area, 1, &c1) ||
            !area_corner(&e->room, area, 2, &c2) || !area_middle(&e->room, area, &middle)) {
            continue;
        }
        way = vec3_norm(vec3(in.x - out.x, 0.0f, in.z - out.z));   /* (from outside in) */
        edge = vec3(c1.x - c0.x, 0.0f, c1.z - c0.z);               /* (the area's side along the way) */
        if (fabsf(vec3_dot(way, vec3_norm(edge))) < 0.70710677f) {
            edge = vec3(c1.x - c2.x, 0.0f, c1.z - c2.z);
        }
        half = 0.5f * vec3_len(edge);
        edge = vec3_norm(edge);
        if (vec3_dot(way, edge) < 0.0f) {
            edge = vec3_scale(edge, -1.0f);
        }
        past = vec3_dot(vec3_sub(middle, a->pos), edge);   /* (how far outward of the middle) */
        return half > 0.0f ? fminf(fmaxf(past / half, 0.0f), 1.0f) : 0.0f;
    }
    return 0.0f;
}

static void draw_actor(Engine *e, Actor *a, const Mat4 *vp) {
    float alpha = a->alpha, faded = doorway_faded(e, a);

    a->alpha = alpha * (1.0f - faded);
    use_lights(e, a);
    actor_draw(a, vp);
    a->alpha = alpha;
}

static void draw(Engine *e) {
    Mat4 proj, view, vp;
    int i;

    platform_size(&e->width, &e->height);
    render_shadows_begin();
    for (i = 0; i < MAX_ACTORS; i++) {
        Actor *a = &e->actors[i];

        a->lights[0] = a->lights[1] = a->lights[2] = -1;
        if (a->used && a->visible) {
            actor_prepare(a);
            light_actor(e, a);
        }
    }
    render_shadows_end();
    render_begin(e->width, e->height, e->clear);
    proj = camera_proj(&e->camera, render_aspect());
    view = camera_view(&e->camera);
    vp = mat4_mul(proj, view);
    render_camera(&proj, &view, e->camera.pos, e->camera.znear, e->camera.zfar);
    if (e->alone > 0) {   /* (one model on the background, nothing else: the extras' model gallery) */
        if (e->alone - 1 < MAX_ACTORS && e->actors[e->alone - 1].used) {
            draw_actor(e, &e->actors[e->alone - 1], &vp);
        }
        goto the_2d;
    }
    room_draw(&e->room, &vp, e->camera.pos, camera_forward(&e->camera));
    for (i = 0; i < MAX_ACTORS; i++) {
        if (e->actors[i].used && e->actors[i].layer < 0x1E && e->actors[i].layer != 0x1C) {
            draw_actor(e, &e->actors[i], &vp);
        }
    }
    for (i = 0; i < MAX_ACTORS && !(e->actors[i].used && e->actors[i].layer == 0x1C); i++) {
    }
    if (i < MAX_ACTORS && post_soft_begin()) {   /* layer $1C: drawn softened (at half size) */
        for (i = 0; i < MAX_ACTORS; i++) {
            if (e->actors[i].used && e->actors[i].layer == 0x1C) {
                int off = e->actors[i].shadow_off;

                e->actors[i].shadow_off = 1;   /* (its shadow stays in the scene: not softened) */
                draw_actor(e, &e->actors[i], &vp);
                e->actors[i].shadow_off = off;
            }
        }
        post_soft_end();
    }
    draw_reflections(e, &proj, &view, &vp);
    for (i = 0; i < MAX_ACTORS; i++) {   /* (the later layers: in front of a reflection over everything) */
        if (e->actors[i].used && e->actors[i].layer >= 0x1E) {
            draw_actor(e, &e->actors[i], &vp);
        }
    }
    things_draw(e->room.id, &vp);   /* the things lying about (their sprites written for the next) */
    sprites_draw(&vp, &view);   /* the effects (after the opaque scene: they don't write depth) */
    if (sprites_glowing() && post_glow_begin()) {   /* the glow pass: the glowing ones again */
        sprites_draw_glow(&vp);
        post_glow_end();
    }
    if (post_shine_begin()) {   /* layer $14 again, for its shine */
        for (i = 0; i < MAX_ACTORS; i++) {
            if (e->actors[i].used && e->actors[i].layer == 0x14) {
                use_lights(e, &e->actors[i]);
                actor_draw(&e->actors[i], &vp);
            }
        }
        post_shine_end();
    }
the_2d:
    render_post();
    engine_draw_2d(e);   /* scripts' 2D (on-draw hooks) */
    if (e->hud[0] != 0) {
        float scale = e->height >= 900 ? 3.0f : 2.0f;

        render_text(16, (float)e->height - render_line_height(scale) - 16, scale, 0xF0E8C0FF, e->hud, (int)strlen(e->hud));
    }
    render_end();
    if (e->screenshot[0] != 0) {   /* (the game's picture: before the console goes over it) */
        if (render_screenshot(e->screenshot)) {
            forth_printf(e->forth, "saved %s\n", e->screenshot);
        } else {
            forth_printf(e->forth, "screenshot: can't write %s\n", e->screenshot);
        }
        e->screenshot[0] = 0;
    }
    console_draw(&e->console, e->width, e->height);
    render_end();
    platform_swap();
}

/* the game's Forth and its scripts (game.fs spawns the actors: the title first) */
static void start_scripts(Engine *e, int hidden) {
    e->forth = forth_new(16 << 20);   /* (the converted event scripts take most) */
    forth_set_output(e->forth, console_output, &e->console);
    bind_engine(e->forth);
    bind_state(e->forth);
    bind_hactor(e->forth);
    bind_mods(e->forth);
    bind_update(e->forth);
    forth_prim(e->forth, "exit-status", p_exit_status);
    forth_prim(e->forth, "error-count", p_error_count);
    forth_constant(e->forth, "hidden?", hidden ? -1 : 0);   /* (no window: tests, screenshots - no title, a new game at once; --title: the title even so) */
    if (!load_scripts(e->forth)) {
        fprintf(stderr, "openhuey: the scripts didn't load (see above); the console is open\n");
        e->console.open = 1;
    }
}

/* a fresh game (`soft-reset`: the game over's return to the title, as the original makes its
 * title scene anew): everything the scripts made let go - the actors and their models, the
 * room, the effects and particles, the 2D, sounds, music, movie; the progress cleared but for its unlocks - and the scripts
 * started again */
static void soft_reset(Engine *e, int hidden) {
    int i;

    fprintf(stderr, "openhuey: a fresh game\n");
    hactor_reset();
    post_trail_off();   /* (the renderer made anew: no frame blend) */
    for (i = 0; i < MAX_ACTORS; i++) {
        if (e->actors[i].used) {
            actor_free(&e->actors[i]);
        }
    }
    room_free(&e->room);
    e->room.id = -1;
    particles_free_all();   /* (before the sprites: each system lets go of its own batches) */
    sprites_free_all();
    bind_engine_reset();
    progress_clear();   /* (the unlocks kept) */
    things_clear();
    e->nhooks = e->ndraw_hooks = 0;
    e->nstage = 0;
    memset(&e->extra, 0, sizeof(e->extra));
    e->hud[0] = 0;
    e->paused = 0;
    e->still = 0;
    e->clear = vec3(0, 0, 0);
    memset(e->held, 0, sizeof(e->held));
    e->camera = (Camera){vec3(0, 0, 0), 0, 0, 1.0f, 5.0f, 20000.0f, 1.0f, 0.0f};
    forth_free(e->forth);
    start_scripts(e, hidden);
}

int main(int argc, char **argv) {
    static const bool kNoKeys[SDL_SCANCODE_COUNT];
    Engine *e = &gEngine;
    Options opt;
    Uint64 last, lag = 0;
    int typing = 0;   /* SDL's text input on (the console open) */
    const Uint64 tick_ns = 1000000000ull / TICKS_PER_SECOND;
    long frame = 0;
    int fullscreen = 0;   /* (the window as it is) */
    int i;

    if (!parse_options(argc, argv, &opt)) {
        return 2;
    }
    if (opt.update) {
        return update_run();
    }
    update_cleanup();   /* (an update's leftovers) */
    if (opt.import != NULL) {   /* (hga --import <disc>: the library made, nothing played) */
        return library_import_from(opt.import, opt.hidden) ? 0 : 1;
    }
    if (!library_choose(opt.data, HG2_DEFAULT_DATA, opt.hidden)) {
        return 1;
    }
    {   /* (tests: only --mod's, or a folder named with --mods) */
        char folder[1100];

        if (opt.mods_folder != NULL) {
            snprintf(folder, sizeof(folder), "%s/", opt.mods_folder);
        }
        mods_find(opt.mods_folder != NULL ? folder : opt.test != NULL ? NULL : library_mods_dir(), opt.mods, opt.nmods);
    }
    if (!platform_open("OpenHuey", 1280, 720, opt.hidden) || !render_init()) {
        return 1;
    }

    memset(e, 0, sizeof(*e));
    sprites_init();
    platform_size(&e->width, &e->height);   /* (frames ticked before the first draw - tests, --eval - see it) */
    e->room.id = -1;
    e->camera = (Camera){vec3(0, 0, 0), 0, 0, 1.0f, 5.0f, 20000.0f, 1.0f, 0.0f};
    load_world(&e->world);
    if (!sound_open()) {   /* (none: the game is silent) */
        fprintf(stderr, "openhuey: no sound (%s)\n", SDL_GetError());
    }
    {   /* the reverb's presets: libsd's, from the game's modules beside the data folder */
        size_t n = 0;
        uint8_t *libsd = files_read("../MODULES/LIBSD.IRX", &n);

        if (!snddrv_reverb_setup(libsd, n)) {
            fprintf(stderr, "openhuey: no MODULES/LIBSD.IRX on the disc: no reverb\n");
        }
        free(libsd);
    }
    e->testing = opt.test != NULL;
    movie_game_clock(opt.test != NULL || opt.frames > 0);   /* (headless runs: the same frames each run) */
    start_scripts(e, opt.hidden && !opt.title);
    for (i = 0; i < opt.nevals; i++) {
        forth_eval(e->forth, opt.evals[i], strlen(opt.evals[i]), "--eval");
    }
    if (opt.test != NULL) {   /* (its folder a root too: its USING:s, e.g. tester) */
        char root[1024], *slash;
        int failed;

        snprintf(root, sizeof(root), "%s", opt.test);
        slash = strrchr(root, '/');
        if (slash != NULL) {
            *slash = 0;
            forth_add_root(e->forth, root);
            slash = strrchr(root, '/');   /* and the folder above (tests/: shared helpers) */
            if (slash != NULL) {
                *slash = 0;
                forth_add_root(e->forth, root);
            }
        }
        failed = forth_include(e->forth, opt.test) != 0;
        return failed ? 1 : sTestStatus;
    }

    last = SDL_GetTicksNS();
    while (!e->forth->bye) {
        Uint64 now = SDL_GetTicksNS();
        int ticks = 0;

        platform_poll(&e->input);
        if (e->input.quit) {
            break;
        }
        if (e->input.fullscreen_toggle) {
            gRender.fullscreen = !gRender.fullscreen;
        }
        if (!opt.hidden && gRender.fullscreen != fullscreen) {   /* (the setting: F11, the menu, the saved one) */
            fullscreen = gRender.fullscreen;
            platform_fullscreen(fullscreen);
        }
        if (e->input.pressed[SDL_SCANCODE_GRAVE]) {
            e->console.open = !e->console.open;
            e->input.pressed[SDL_SCANCODE_GRAVE] = false;   /* (taken here, once) */
        }
        if (e->console.open != typing) {   /* (opened or shut by ` or by a script: console!) */
            typing = e->console.open;
            platform_text_input(typing);
        }
        if (e->console.open) {
            console_update(&e->console, &e->input, e->forth);
            /* the game sees no keys while typing */
            e->input.keys = kNoKeys;
            memset(e->input.pressed, 0, sizeof(e->input.pressed));
            e->input.mouse_dx = e->input.mouse_dy = 0;
        }

        /* fixed ticks: as many as real time asks for (at least one in --frames runs) */
        lag += now - last;
        last = now;
        if (opt.frames > 0 && lag < tick_ns) {
            lag = tick_ns;
        }
        while (lag >= tick_ns && ticks < 4) {
            engine_tick(e);
            memset(e->input.pressed, 0, sizeof(e->input.pressed));   /* presses count once */
            e->input.mouse_dx = e->input.mouse_dy = 0;
            lag -= tick_ns;
            ticks++;
        }
        if (ticks == 4) {
            lag = 0;   /* far behind (a stall): don't try to catch up */
        }
        if (bind_engine_reset_wanted()) {   /* (between frames: no handler running) */
            soft_reset(e, opt.hidden && !opt.title);
            continue;
        }

        if (ticks == 0 && opt.frames == 0) {   /* (the picture changes only with a tick: 30 a second) */
            SDL_DelayNS(tick_ns - lag > 2000000ull ? tick_ns - lag - 1000000ull : 0);
            continue;
        }
        draw(e);
        if (opt.frames > 0 && ++frame >= opt.frames) {
            break;
        }
    }

    for (i = 0; i < MAX_ACTORS; i++) {
        if (e->actors[i].used) {
            actor_free(&e->actors[i]);
        }
    }
    room_free(&e->room);
    forth_free(e->forth);
    render_shutdown();
    platform_close();
    return 0;
}
