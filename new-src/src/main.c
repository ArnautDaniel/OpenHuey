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

static int load_scripts(Forth *f) {
    const char *dir = getenv("HG2_SCRIPTS") != NULL ? getenv("HG2_SCRIPTS") : HG2_SCRIPT_DIR;
    char path[1024];

    snprintf(path, sizeof(path), "%s/prelude.fs", dir);
    if (forth_include(f, path) != 0) {
        return 0;
    }
    /* scripts include each other relative to the scripts folder */
    snprintf(path, sizeof(path), ": scripts-dir s\" %s\" ;", dir);
    forth_eval(f, path, strlen(path), "main");
    snprintf(path, sizeof(path), "%s/game.fs", dir);
    return forth_include(f, path) == 0;
}

static void draw(Engine *e) {
    float aspect;
    Mat4 vp;

    platform_size(&e->width, &e->height);
    aspect = e->height > 0 ? (float)e->width / (float)e->height : 1.0f;
    vp = camera_view_proj(&e->camera, aspect);
    render_begin(e->width, e->height, e->clear);
    room_draw(&e->room, &vp);
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
    if (!platform_open("Haunting Ground (new-src)", 1280, 896, opt.hidden) || !render_init()) {
        return 1;
    }

    memset(e, 0, sizeof(*e));
    e->room.id = -1;
    e->camera = (Camera){vec3(0, 0, 0), 0, 0, 1.0f, 5.0f, 20000.0f, 1.0f};
    e->forth = forth_new(4 << 20);
    forth_set_output(e->forth, console_output, &e->console);
    bind_engine(e->forth);
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

    room_free(&e->room);
    forth_free(e->forth);
    render_shutdown();
    platform_close();
    return 0;
}
