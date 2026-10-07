/* hg2: start-up and the main loop.
 *
 *   hg2 [options] [data-dir]
 *     --hidden          no window shown (tests, screenshots)
 *     --frames N        stop after N frames
 *     --eval "code"     run Forth after the scripts have loaded (repeatable)
 *
 * Start-up: the window, the renderer, the Forth system with the engine's words, then
 * scripts/prelude.fs and scripts/game.fs - which decide everything else. Each frame: input,
 * game ticks at a fixed 60 a second (Forth tasks and hooks), then drawing. */
#include "core/files.h"
#include "forth/forth.h"
#include "game/engine.h"
#include "game/progress.h"
#include "platform/sound.h"
#include "platform/platform.h"
#include "render/render.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EVALS 16

typedef struct Options {
    int hidden;
    long frames;   /* 0: run until closed */
    const char *evals[MAX_EVALS];
    int nevals;
    const char *data;
} Options;

static int parse_options(int argc, char **argv, Options *o) {
    int i;

    memset(o, 0, sizeof(*o));
    o->data = getenv("HG_DATA") != NULL ? getenv("HG_DATA") : HG2_DEFAULT_DATA;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--hidden") == 0) {
            o->hidden = 1;
        } else if (strcmp(argv[i], "--frames") == 0 && i + 1 < argc) {
            o->frames = atol(argv[++i]);
        } else if (strcmp(argv[i], "--eval") == 0 && i + 1 < argc && o->nevals < MAX_EVALS) {
            o->evals[o->nevals++] = argv[++i];
        } else if (argv[i][0] == '-') {
            fprintf(stderr, "usage: hg2 [--hidden] [--frames N] [--eval code] [data-dir]\n");
            return 0;
        } else {
            o->data = argv[i];
        }
    }
    return 1;
}

/* the game's executable, for the tables only it has: $HG_EXE, next to the data folder, in it,
 * or the repository's baserom */
static void load_world(World *w, const char *data) {
    char path[1200];
    const char *env = getenv("HG_EXE");

    if (env != NULL && world_load(w, env)) {
        return;
    }
    snprintf(path, sizeof(path), "%s/../SLUS_210.75", data);
    if (world_load(w, path)) {
        return;
    }
    snprintf(path, sizeof(path), "%s/SLUS_210.75", data);
    if (world_load(w, path) || world_load(w, HG2_DEFAULT_EXE)) {
        return;
    }
    fprintf(stderr, "hg2: no SLUS_210.75 found (set HG_EXE): rooms won't connect\n");
}

static int load_scripts(Forth *f) {
    const char *dir = getenv("HG2_SCRIPTS") != NULL ? getenv("HG2_SCRIPTS") : HG2_SCRIPT_DIR;
    char path[1024];

    snprintf(path, sizeof(path), "%s/prelude.fs", dir);
    if (forth_include(f, path) != 0) {
        return 0;
    }
    forth_add_root(f, dir);   /* where USING: finds vocabularies */
    snprintf(path, sizeof(path), "%s/game.fs", dir);
    return forth_include(f, path) == 0;
}

/* the room's lights for an actor; its shadow from the strongest (a view from the light, fitted
 * round the actor and reaching the floor beyond) */
static void light_actor(Engine *e, Actor *a) {
    const Room *r = &e->room;
    Vec3 center = actor_center(a);
    float radius = actor_radius(a);
    const RoomLight *l;
    Vec3 dir, up;
    float dist, lum, fall = 1.0f;
    Mat4 proj, view, vp;
    int slot;

    room_lights_at(r, center, a->lights);
    if (a->lights[0] < 0) {
        return;
    }
    l = &r->lights[a->lights[0]];
    dir = vec3_sub(center, l->pos);
    dist = vec3_len(dir);
    if (dist < radius * 1.5f) {
        return;   /* the light is inside it */
    }
    if (l->range > 0.0f) {
        fall = fmaxf(0.0f, 1.0f - dist / l->range);
    }
    lum = (0.3f * l->color.x + 0.6f * l->color.y + 0.1f * l->color.z) * l->intensity / 128.0f;
    dir = vec3_scale(dir, 1.0f / dist);
    up = fabsf(dir.y) > 0.95f ? vec3(1, 0, 0) : vec3(0, 1, 0);
    proj = mat4_perspective(2.0f * atanf(radius * 1.25f / dist), 1.0f, fmaxf(1.0f, dist - radius * 2.0f),
                            dist + radius * 6.0f + 200.0f);
    view = mat4_look(l->pos, dir, up);
    vp = mat4_mul(proj, view);
    slot = render_shadow_add(&vp, center, radius, gRender.shadow_strength * fminf(1.0f, fmaxf(0.6f, lum * fall * 4.0f)));
    if (slot >= 0) {
        render_shadow_mesh(&a->gpu, a->model.d, a->model.nd);
    }
}

/* the lights of the next lit draws: the actor's (or the fixed key light) */
static void use_lights(Engine *e, const Actor *a) {
    DrawLight lights[3];
    int n = 0, k;

    for (k = 0; k < 3; k++) {
        if (a->lights[k] >= 0) {
            const RoomLight *l = &e->room.lights[a->lights[k]];

            lights[n].pos = l->pos;
            lights[n].color = vec3_scale(l->color, l->intensity);
            lights[n].range = l->range;
            n++;
        }
    }
    render_draw_lights(e->room.ambient, lights, e->room.nlights > 0 ? n : -1);
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
    room_draw(&e->room, &vp, e->camera.pos, camera_forward(&e->camera));
    for (i = 0; i < MAX_ACTORS; i++) {
        if (e->actors[i].used) {
            use_lights(e, &e->actors[i]);
            actor_draw(&e->actors[i], &vp);
        }
    }
    render_post();
    engine_draw_2d(e);   /* scripts' 2D (on-draw hooks) */
    if (e->hud[0] != 0) {
        float scale = e->height >= 900 ? 3.0f : 2.0f;

        render_text(16, (float)e->height - render_line_height(scale) - 16, scale, 0xF0E8C0FF, e->hud, (int)strlen(e->hud));
    }
    console_draw(&e->console, e->width, e->height);
    render_end();
    if (e->screenshot[0] != 0) {
        if (render_screenshot(e->screenshot)) {
            forth_printf(e->forth, "saved %s\n", e->screenshot);
        } else {
            forth_printf(e->forth, "screenshot: can't write %s\n", e->screenshot);
        }
        e->screenshot[0] = 0;
    }
    platform_swap();
}

int main(int argc, char **argv) {
    static const bool kNoKeys[SDL_SCANCODE_COUNT];
    Engine *e = &gEngine;
    Options opt;
    Uint64 last, lag = 0;
    const Uint64 tick_ns = 1000000000ull / TICKS_PER_SECOND;
    long frame = 0;
    int i;

    if (!parse_options(argc, argv, &opt)) {
        return 2;
    }
    files_set_root(opt.data);
    if (!files_exist("SYSTEM") && !files_exist("ST_000/ST_000.PAC")) {
        fprintf(stderr, "hg2: no game data in \"%s\" (the extracted DATA.CVM folder). Pass it as an argument "
                        "or set HG_DATA.\n", opt.data);
    }
    if (!platform_open("Haunting Ground (new-src)", 1280, 720, opt.hidden) || !render_init()) {
        return 1;
    }

    memset(e, 0, sizeof(*e));
    e->room.id = -1;
    e->camera = (Camera){vec3(0, 0, 0), 0, 0, 1.0f, 5.0f, 20000.0f, 1.0f};
    load_world(&e->world, opt.data);
    e->forth = forth_new(16 << 20);   /* (the converted event scripts take most) */
    forth_set_output(e->forth, console_output, &e->console);
    sound_open();   /* (none: the game is silent) */
    bind_engine(e->forth);
    bind_state(e->forth);
    if (!load_scripts(e->forth)) {
        fprintf(stderr, "hg2: the scripts didn't load (see above); the console is open\n");
        e->console.open = 1;
    }
    for (i = 0; i < opt.nevals; i++) {
        forth_eval(e->forth, opt.evals[i], strlen(opt.evals[i]), "--eval");
    }

    last = SDL_GetTicksNS();
    while (!e->forth->bye) {
        Uint64 now = SDL_GetTicksNS();
        int was_open = e->console.open, ticks = 0;

        platform_poll(&e->input);
        if (e->input.quit) {
            break;
        }
        if (e->input.pressed[SDL_SCANCODE_GRAVE]) {
            e->console.open = !e->console.open;
        }
        if (e->console.open != was_open) {
            platform_text_input(e->console.open);
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
