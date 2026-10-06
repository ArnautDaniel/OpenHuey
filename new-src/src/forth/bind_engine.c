/* The engine's Forth words. Scripts drive the game with these; C keeps the data.
 *
 * Structs are reached by address plus field words: `camera cam.yaw sf@` reads the camera's yaw
 * (a field word adds its offset). Floats in structs are 32-bit: sf@ / sf!. */
#include "../game/engine.h"

#include "../core/files.h"

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

Engine gEngine;

#define PRIM(name) static void name(Forth *f, Word *w __attribute__((unused)))
#define PUSH(x) forth_push(f, (Cell)(x))
#define POP() forth_pop(f)
#define FPUSH(x) forth_fpush(f, (Float)(x))
#define FPOP() forth_fpop(f)

/* a field word: ( addr -- addr+offset ) */
static void dofield(Forth *f, Word *w) {
    PUSH(POP() + w->body[0]);
}

static void field(Forth *f, const char *name, size_t offset) {
    forth_constant(f, name, (Cell)offset)->code = dofield;
}

/* ---- rooms ---- */

PRIM(p_room) {   /* ( id -- ) load a room */
    Cell id = POP();

    if (!room_load(&gEngine.room, (int)id)) {
        forth_error(f, "room: no room %lX", (long)id);
    }
}
PRIM(p_room_id) { PUSH(gEngine.room.id); }
PRIM(p_room_exists) { PUSH(room_exists((int)POP()) ? -1 : 0); }
PRIM(p_room_bounds) {   /* ( F: -- lx ly lz hx hy hz ) the solid part's box */
    const RoomMesh *m = &gEngine.room.mesh;

    if (m->nv == 0) {
        forth_error(f, "room-bounds: no room mesh");
    }
    FPUSH(m->lo.x); FPUSH(m->lo.y); FPUSH(m->lo.z);
    FPUSH(m->hi.x); FPUSH(m->hi.y); FPUSH(m->hi.z);
}
PRIM(p_room_group) {   /* ( group flag -- ) show or hide a visibility group */
    Cell on = POP(), g = POP();

    if (g <= 0 || g > 255) {
        forth_error(f, "room-group!: group %ld out of range", (long)g);
    }
    if (on) {
        gEngine.room.groups[g >> 5] |= 1u << (g & 31);
    } else {
        gEngine.room.groups[g >> 5] &= ~(1u << (g & 31));
    }
}
PRIM(p_floor_below) {   /* ( F: x y z -- y' ) ( -- flag ) the solid surface under a point */
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP(), h;

    if (room_floor_below(&gEngine.room, x, y, z, &h)) {
        FPUSH(h);
        PUSH(-1);
    } else {
        PUSH(0);
    }
}
PRIM(p_nav_tris) { PUSH(gEngine.room.nav.ntris); }   /* ( -- n ) */
PRIM(p_nav_move) {   /* ( F: x y z dx dz climb radius -- x' y' z' ) a step on the nav mesh */
    float radius = (float)FPOP(), climb = (float)FPOP(), dz = (float)FPOP(), dx = (float)FPOP();
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP();
    Vec3 p = navmesh_move(&gEngine.room.nav, vec3(x, y, z), dx, dz, climb, radius);

    FPUSH(p.x); FPUSH(p.y); FPUSH(p.z);
}
PRIM(p_nav_nearest) {   /* ( F: x y z -- x' y' z' ) the middle of the nearest nav triangle */
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP();
    Vec3 p = navmesh_nearest(&gEngine.room.nav, vec3(x, y, z));

    FPUSH(p.x); FPUSH(p.y); FPUSH(p.z);
}
PRIM(p_nav_at) {   /* ( F: x y z climb -- [y'] ) ( -- flag ) on the nav mesh? and its height */
    float climb = (float)FPOP(), z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP(), h;

    if (navmesh_find(&gEngine.room.nav, vec3(x, y, z), climb, &h) >= 0) {
        FPUSH(h);
        PUSH(-1);
    } else {
        PUSH(0);
    }
}
PRIM(p_nav_tri) {   /* ( F: x y z -- ) ( -- tri | -1 ) the nav triangle under a point */
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP(), h;

    PUSH(navmesh_find(&gEngine.room.nav, vec3(x, y, z), 4.0f, &h));
}
PRIM(p_tri_center) {   /* ( tri -- ) ( F: -- x y z ) */
    Cell i = POP();
    Vec3 c;

    if (i < 0 || i >= gEngine.room.nav.ntris) {
        forth_error(f, "tri-center: no nav triangle %ld", (long)i);
    }
    c = navmesh_center(&gEngine.room.nav, (int)i);
    FPUSH(c.x); FPUSH(c.y); FPUSH(c.z);
}

/* ---- the house: exits and where they lead ---- */

PRIM(p_exit_tri) {   /* ( exit which -- tri | -1 ) the current room's exit triangles: 0 out, 1 in, 2 through */
    Cell which = POP(), exit = POP();

    PUSH(world_exit_tri(&gEngine.world, gEngine.room.id, (int)exit, (int)which));
}
PRIM(p_exit_leads) {   /* ( exit -- room exit' | -1 -1 ) where an exit of this room goes */
    Cell exit = POP();
    int to_exit = -1, room = world_exit_leads(&gEngine.world, gEngine.room.id, (int)exit, &to_exit);

    PUSH(room);
    PUSH(room < 0 ? -1 : to_exit);
}
PRIM(p_hud) {   /* ( addr len -- ) the line at the bottom of the screen; 0 0 hud clears it */
    Cell n = POP(), a = POP();

    snprintf(gEngine.hud, sizeof(gEngine.hud), "%.*s", (int)(n > 0 ? n : 0), n > 0 ? (const char *)a : "");
}
PRIM(p_room_info) {   /* ( -- ) a summary of the loaded room */
    const Room *r = &gEngine.room;
    int i, parts[MESH_PARTS] = {0};

    if (r->id < 0) {
        forth_printf(f, "no room\n");
        return;
    }
    for (i = 0; i < r->mesh.nd; i++) {
        parts[r->mesh.d[i].part]++;
    }
    for (i = 0; i < r->mesh.ndyn; i++) {
        parts[MESH_ANIMATED] += r->mesh.dyn[i].frames > 0;
    }
    forth_printf(f, "room %03X: %zu bytes, %d triangles in %d draws (solid %d, see-through %d, glow %d, mask %d), "
                    "%d moving (%d flip books), %d textures, %d nav triangles\n",
                 r->id, r->pac.size, r->mesh.nv / 3, r->mesh.nd, parts[MESH_SOLID], parts[MESH_SEE_THROUGH],
                 parts[MESH_GLOW], parts[MESH_BLOOM_MASK], r->mesh.ndyn, parts[MESH_ANIMATED], r->ntextures,
                 r->nav.ntris);
}

/* the room's camera setups (PAC section 5): 32-byte entries - eye x, y, z, field of view in
 * degrees (0: 60), target x, y, z, w - until an entry whose first word is -1 */
static const float *room_cameras(int *count) {
    size_t size;
    const uint8_t *sec = pac_section(&gEngine.room.pac, PAC_CAMERAS, &size);
    int n = 0;

    while (sec != NULL && (size_t)(n + 1) * 32 <= size) {
        int32_t first;

        memcpy(&first, sec + n * 32, 4);
        if (first == -1) {
            break;
        }
        n++;
    }
    *count = n;
    return (const float *)sec;
}
PRIM(p_room_cameras) {   /* ( -- n ) */
    int n;

    room_cameras(&n);
    PUSH(n);
}
PRIM(p_room_camera) {   /* ( i -- ) ( F: -- ex ey ez fov-radians tx ty tz ) */
    Cell i = POP();
    int n;
    const float *e = room_cameras(&n);
    float v[8];

    if (i < 0 || i >= n) {
        forth_error(f, "room-camera: no camera %ld (the room has %d)", (long)i, n);
    }
    memcpy(v, e + i * 8, sizeof(v));   /* (the file's floats may not be aligned for us) */
    FPUSH(v[0]); FPUSH(v[1]); FPUSH(v[2]);
    /* the game's field of view spans the width of a 4:3 picture (src/game/camera.c
     * Camera_ViewMatrix and the view-screen scales); ours is vertical */
    FPUSH(2.0 * atan(0.75 * tan((v[3] == 0.0f ? 60.0f : v[3]) * 3.14159265358979 / 360.0)));
    FPUSH(v[4]); FPUSH(v[5]); FPUSH(v[6]);
}

/* ---- the camera ---- */

PRIM(p_camera) { PUSH(&gEngine.camera); }

/* ---- actors ---- */

static Actor *actor_arg(Forth *f, Cell id) {
    if (id < 0 || id >= MAX_ACTORS || !gEngine.actors[id].used) {
        forth_error(f, "no actor %ld", (long)id);
    }
    return &gEngine.actors[id];
}
PRIM(p_actor_load) {   /* ( addr len -- id ) s" O_FIN/FIN_000" actor-load */
    Cell n = POP(), a = POP();
    char name[64];
    int id;

    snprintf(name, sizeof(name), "%.*s", (int)n, (const char *)a);
    id = actor_load(gEngine.actors, name);
    if (id < 0) {
        forth_error(f, "actor-load: can't load %s (.PCK / .TEX)", name);
    }
    PUSH(id);
}
PRIM(p_actor_free) { actor_free(actor_arg(f, POP())); }
PRIM(p_actor) { PUSH(actor_arg(f, POP())); }   /* ( id -- addr ) */
PRIM(p_motion_store) {   /* ( id motion-id -- ) play a motion from its start */
    Cell mid = POP();
    Actor *a = actor_arg(f, POP());
    int index = model_motion_find(&a->model, (int)mid);

    if (index < 0) {
        forth_error(f, "motion!: %s has no motion %lX", a->name, (long)mid);
    }
    if (index != a->motion) {
        a->motion = index;
        a->frame = 0.0f;
    }
}
PRIM(p_motion_fetch) {   /* ( id -- motion-id | -1 ) */
    Actor *a = actor_arg(f, POP());

    PUSH(a->motion < 0 ? -1 : model_motion_id(&a->model, a->motion));
}
PRIM(p_motion_done) { PUSH(actor_motion_done(actor_arg(f, POP())) ? -1 : 0); }
PRIM(p_motion_frames) {   /* ( id -- frames ) of the current motion */
    Actor *a = actor_arg(f, POP());

    PUSH(model_motion_frames(&a->model, a->motion));
}
PRIM(p_motions) {   /* ( id -- ) list the motions */
    Actor *a = actor_arg(f, POP());
    int i, n = model_motion_count(&a->model);

    for (i = 0; i < n; i++) {
        forth_printf(f, "%04X:%-4d%s", model_motion_id(&a->model, i), model_motion_frames(&a->model, i),
                     i % 8 == 7 ? "\n" : " ");
    }
    forth_printf(f, "\n%d motions\n", n);
}

/* ---- input ---- */

static const bool *keys(void) {
    static const bool kNone[SDL_SCANCODE_COUNT];

    return gEngine.input.keys != NULL ? gEngine.input.keys : kNone;
}
static Cell scancode(Forth *f) {
    Cell k = POP();

    if (k <= 0 || k >= SDL_SCANCODE_COUNT) {
        forth_error(f, "not a key: %ld", (long)k);
    }
    return k;
}
PRIM(p_key_down) {
    Cell k = scancode(f);

    PUSH(keys()[k] || gEngine.held[k] ? -1 : 0);
}
PRIM(p_key_hold) {   /* ( scancode flag -- ) hold a key down (or let it go) as if pressed */
    Cell on = POP(), k = scancode(f);

    gEngine.held[k] = on != 0;
}
PRIM(p_key_pressed) { PUSH(gEngine.input.pressed[scancode(f)] ? -1 : 0); }
PRIM(p_key_colon) {   /* key: name ( -- scancode ), immediate: `key: W`, `key: Left_Shift` */
    size_t n;
    const char *s = forth_parse_name(f, &n);
    char name[64], *p;
    SDL_Scancode k;

    if (n == 0 || n >= sizeof(name)) {
        forth_error(f, "key: a key name expected");
    }
    memcpy(name, s, n);
    name[n] = 0;
    for (p = name; *p != 0; p++) {   /* names with spaces are written with _ */
        if (*p == '_') {
            *p = ' ';
        }
    }
    k = SDL_GetScancodeFromName(name);
    if (k == SDL_SCANCODE_UNKNOWN) {
        forth_error(f, "key: no key called %s", name);
    }
    if (f->compiling) {
        forth_compile_literal(f, k);
    } else {
        PUSH(k);
    }
}
PRIM(p_mouse_dx) { FPUSH(gEngine.input.mouse_dx); }
PRIM(p_mouse_dy) { FPUSH(gEngine.input.mouse_dy); }
PRIM(p_mouse_down) {   /* ( button -- flag ) 1 left, 2 middle, 3 right */
    Cell b = POP();

    PUSH(b >= 1 && b <= 5 && (gEngine.input.mouse_buttons & SDL_BUTTON_MASK(b)) ? -1 : 0);
}

/* ---- the game loop ---- */

PRIM(p_on_tick) {   /* ( xt -- ) run xt every tick */
    Word *x = (Word *)POP();

    if (gEngine.nhooks >= ENGINE_HOOKS) {
        forth_error(f, "on-tick: too many");
    }
    gEngine.hooks[gEngine.nhooks++] = x;
}
PRIM(p_off_tick) {   /* ( xt -- ) */
    Word *x = (Word *)POP();
    int i;

    for (i = 0; i < gEngine.nhooks; i++) {
        if (gEngine.hooks[i] == x) {
            memmove(&gEngine.hooks[i], &gEngine.hooks[i + 1], (size_t)(gEngine.nhooks - i - 1) * sizeof(Word *));
            gEngine.nhooks--;
            i--;
        }
    }
}
PRIM(p_on_draw) {   /* ( xt -- ) run xt every frame, to draw 2D (draw-text, draw-rect) */
    Word *x = (Word *)POP();

    if (gEngine.ndraw_hooks >= ENGINE_HOOKS) {
        forth_error(f, "on-draw: too many");
    }
    gEngine.draw_hooks[gEngine.ndraw_hooks++] = x;
}
PRIM(p_off_draw) {   /* ( xt -- ) */
    Word *x = (Word *)POP();
    int i;

    for (i = 0; i < gEngine.ndraw_hooks; i++) {
        if (gEngine.draw_hooks[i] == x) {
            memmove(&gEngine.draw_hooks[i], &gEngine.draw_hooks[i + 1],
                    (size_t)(gEngine.ndraw_hooks - i - 1) * sizeof(Word *));
            gEngine.ndraw_hooks--;
            i--;
        }
    }
}

/* ---- 2D drawing (from on-draw hooks): a pen with a colour and a text size ---- */

static uint32_t sPenColor = 0xFFFFFFFF;
static Cell sPenScale = 2;

PRIM(p_pen_color) { sPenColor = (uint32_t)POP(); }   /* ( rgba -- ) 0xRRGGBBAA */
PRIM(p_pen_scale) { Cell s = POP(); sPenScale = s < 1 ? 1 : s > 8 ? 8 : s; }   /* ( n -- ) */
PRIM(p_draw_text) {   /* ( addr len x y -- ) */
    Cell y = POP(), x = POP(), n = POP(), a = POP();

    render_text((float)x, (float)y, (float)sPenScale, sPenColor, (const char *)a, (int)n);
}
PRIM(p_draw_rect) {   /* ( x y w h -- ) */
    Cell height = POP(), width = POP(), y = POP(), x = POP();

    render_rect((float)x, (float)y, (float)width, (float)height, sPenColor);
}
PRIM(p_screen_size) { PUSH(gEngine.width); PUSH(gEngine.height); }   /* ( -- w h ) */
PRIM(p_char_size) {   /* ( -- w h ) a character cell at the pen's size */
    PUSH((Cell)render_text_width((float)sPenScale, 1));
    PUSH((Cell)render_line_height((float)sPenScale));
}

/* numbers as text (a few rotating buffers, like s") */
static char sNumText[4][48];
static int sNumNext;
PRIM(p_n_to_s) {   /* ( n -- addr len ) */
    char *b = sNumText[sNumNext++ % 4];

    snprintf(b, sizeof(sNumText[0]), "%ld", (long)POP());
    PUSH(b);
    PUSH(strlen(b));
}
PRIM(p_f_to_s) {   /* ( places -- addr len ) ( F: x -- ) */
    char *b = sNumText[sNumNext++ % 4];
    Cell places = POP();

    snprintf(b, sizeof(sNumText[0]), "%.*f", (int)(places < 0 ? 0 : places > 6 ? 6 : places), FPOP());
    PUSH(b);
    PUSH(strlen(b));
}

PRIM(p_gfx) { PUSH(&gRender); }

/* ---- files: the player's own folder, and writing Forth's output into a file ---- */

PRIM(p_user_dir) {   /* ( -- addr len ) where settings are kept (made if needed; ends with /) */
    static char *dir;

    if (dir == NULL) {
        dir = SDL_GetPrefPath("hg2", "new-src");
    }
    if (dir == NULL) {
        forth_error(f, "user-dir: %s", SDL_GetError());
    }
    PUSH(dir);
    PUSH(strlen(dir));
}
PRIM(p_file_exists) {   /* ( addr len -- flag ) */
    Cell n = POP(), a = POP();
    char path[1024];
    FILE *fp;

    snprintf(path, sizeof(path), "%.*s", (int)n, (const char *)a);
    fp = fopen(path, "rb");
    if (fp != NULL) {
        fclose(fp);
    }
    PUSH(fp != NULL ? -1 : 0);
}

static FILE *sOutFile;
static OutputFn sSavedOut;
static void *sSavedCtx;

static void file_output(void *ctx, const char *s, size_t n) {
    fwrite(s, 1, n, (FILE *)ctx);
}
PRIM(p_to_file) {   /* ( addr len -- ) what Forth prints goes into this file until end-file */
    Cell n = POP(), a = POP();
    char path[1024];

    if (sOutFile != NULL) {
        forth_error(f, "to-file: already writing a file");
    }
    snprintf(path, sizeof(path), "%.*s", (int)n, (const char *)a);
    sOutFile = fopen(path, "w");
    if (sOutFile == NULL) {
        forth_error(f, "to-file: can't write %s", path);
    }
    sSavedOut = f->out;
    sSavedCtx = f->out_ctx;
    forth_set_output(f, file_output, sOutFile);
}
PRIM(p_end_file) {
    if (sOutFile != NULL) {
        fclose(sOutFile);
        sOutFile = NULL;
        forth_set_output(f, sSavedOut, sSavedCtx);
    }
}
PRIM(p_xt_to_name) {   /* ( xt -- addr len ) a word's name */
    Word *x = (Word *)POP();

    PUSH(x->name);
    PUSH(x->len);
}

PRIM(p_ticks) { PUSH(gEngine.ticks); }
PRIM(p_dt) { FPUSH(1.0 / TICKS_PER_SECOND); }
PRIM(p_clear_color) {   /* ( F: r g b -- ) the background */
    gEngine.clear.z = (float)FPOP();
    gEngine.clear.y = (float)FPOP();
    gEngine.clear.x = (float)FPOP();
}
PRIM(p_screenshot) {   /* ( addr len -- ) save the frame as a PNG once drawn */
    Cell n = POP(), a = POP();

    snprintf(gEngine.screenshot, sizeof(gEngine.screenshot), "%.*s", (int)n, (const char *)a);
}
PRIM(p_console) { gEngine.console.open = (int)POP() != 0; }   /* ( flag -- ) */
PRIM(p_data_dir) { PUSH(files_root()); PUSH(strlen(files_root())); }

void engine_tick(Engine *e) {
    int i;

    e->ticks++;
    forth_run_tasks(e->forth);
    for (i = 0; i < e->nhooks; i++) {
        if (forth_call(e->forth, e->hooks[i]) != 0) {
            forth_printf(e->forth, "on-tick: %s removed after an error\n", e->hooks[i]->name);
            memmove(&e->hooks[i], &e->hooks[i + 1], (size_t)(e->nhooks - i - 1) * sizeof(Word *));
            e->nhooks--;
            i--;
        }
    }
    for (i = 0; i < MAX_ACTORS; i++) {
        actor_tick(&e->actors[i]);
    }
    room_tick(&e->room);
}

void engine_draw_2d(Engine *e) {
    int i;

    for (i = 0; i < e->ndraw_hooks; i++) {
        if (forth_call(e->forth, e->draw_hooks[i]) != 0) {
            forth_printf(e->forth, "on-draw: %s removed after an error\n", e->draw_hooks[i]->name);
            memmove(&e->draw_hooks[i], &e->draw_hooks[i + 1], (size_t)(e->ndraw_hooks - i - 1) * sizeof(Word *));
            e->ndraw_hooks--;
            i--;
        }
    }
}

void bind_engine(Forth *f) {
    Vocab *saved = f->m.current, *engine = forth_vocab(f, "engine");
    static const struct {
        const char *name;
        Code code;
    } prims[] = {
        {"room", p_room}, {"room-id", p_room_id}, {"room-exists?", p_room_exists},
        {"room-bounds", p_room_bounds}, {"room-cameras", p_room_cameras}, {"room-camera", p_room_camera}, {"room-group!", p_room_group}, {"floor-below", p_floor_below},
        {"nav-tris", p_nav_tris}, {"nav-tri", p_nav_tri}, {"tri-center", p_tri_center},
        {"exit-tri", p_exit_tri}, {"exit-leads", p_exit_leads}, {"hud", p_hud}, {"nav-move", p_nav_move}, {"nav-nearest", p_nav_nearest}, {"nav-at", p_nav_at}, {".room", p_room_info},
        {"camera", p_camera},
        {"actor-load", p_actor_load}, {"actor-free", p_actor_free}, {"actor", p_actor},
        {"motion!", p_motion_store}, {"motion@", p_motion_fetch}, {"motion-done?", p_motion_done},
        {"motion-frames", p_motion_frames}, {".motions", p_motions},
        {"key-down?", p_key_down}, {"key-hold", p_key_hold}, {"key-pressed?", p_key_pressed}, {"mouse-dx", p_mouse_dx},
        {"mouse-dy", p_mouse_dy}, {"mouse-down?", p_mouse_down},
        {"on-tick", p_on_tick}, {"off-tick", p_off_tick}, {"ticks", p_ticks}, {"dt", p_dt},
        {"on-draw", p_on_draw}, {"off-draw", p_off_draw}, {"pen-color", p_pen_color},
        {"pen-scale", p_pen_scale}, {"draw-text", p_draw_text}, {"draw-rect", p_draw_rect},
        {"screen-size", p_screen_size}, {"char-size", p_char_size}, {"n>s", p_n_to_s}, {"f>s$", p_f_to_s},
        {"gfx", p_gfx}, {"user-dir", p_user_dir}, {"file-exists?", p_file_exists}, {"to-file", p_to_file},
        {"end-file", p_end_file}, {"xt>name", p_xt_to_name},
        {"clear-color", p_clear_color}, {"screenshot", p_screenshot}, {"console!", p_console},
        {"data-dir", p_data_dir},
    };
    size_t i;

    forth_set_current(f, engine);   /* the engine's words: scripts say USING: engine ; */
    engine->state = VOCAB_LOADED;
    for (i = 0; i < sizeof(prims) / sizeof(prims[0]); i++) {
        forth_prim(f, prims[i].name, prims[i].code);
    }
    forth_prim(f, "key:", p_key_colon)->flags |= WORD_IMMEDIATE;

    field(f, "cam.x", offsetof(Camera, pos.x));
    field(f, "cam.y", offsetof(Camera, pos.y));
    field(f, "cam.z", offsetof(Camera, pos.z));
    field(f, "cam.yaw", offsetof(Camera, yaw));
    field(f, "cam.pitch", offsetof(Camera, pitch));
    field(f, "cam.fov", offsetof(Camera, fov));
    field(f, "cam.near", offsetof(Camera, znear));
    field(f, "cam.far", offsetof(Camera, zfar));
    field(f, "cam.up", offsetof(Camera, up));
    field(f, "act.x", offsetof(Actor, pos.x));
    field(f, "act.y", offsetof(Actor, pos.y));
    field(f, "act.z", offsetof(Actor, pos.z));
    field(f, "act.yaw", offsetof(Actor, yaw));
    field(f, "act.scale", offsetof(Actor, scale));
    field(f, "act.frame", offsetof(Actor, frame));
    field(f, "act.rate", offsetof(Actor, rate));
    field(f, "act.loop", offsetof(Actor, loop));        /* 32-bit: l@ l! */
    field(f, "act.visible", offsetof(Actor, visible));  /* 32-bit: l@ l! */
    field(f, "act.shadow", offsetof(Actor, shadow_size));
    /* the picture's settings (render.h RenderSettings): flags and counts are 32-bit (l@ l!),
     * the rest floats (sf@ sf!) */
    field(f, "gfx.msaa", offsetof(RenderSettings, msaa));
    field(f, "gfx.scale", offsetof(RenderSettings, scale));
    field(f, "gfx.aspect", offsetof(RenderSettings, aspect));
    field(f, "gfx.anisotropy", offsetof(RenderSettings, anisotropy));
    field(f, "gfx.ssao", offsetof(RenderSettings, ssao));
    field(f, "gfx.ssao-radius", offsetof(RenderSettings, ssao_radius));
    field(f, "gfx.ssao-strength", offsetof(RenderSettings, ssao_strength));
    field(f, "gfx.bloom", offsetof(RenderSettings, bloom));
    field(f, "gfx.bloom-threshold", offsetof(RenderSettings, bloom_threshold));
    field(f, "gfx.bloom-strength", offsetof(RenderSettings, bloom_strength));
    field(f, "gfx.fog", offsetof(RenderSettings, fog));
    field(f, "gfx.fog-density", offsetof(RenderSettings, fog_density));
    field(f, "gfx.fog-start", offsetof(RenderSettings, fog_start));
    field(f, "gfx.fog-r", offsetof(RenderSettings, fog_color.x));
    field(f, "gfx.fog-g", offsetof(RenderSettings, fog_color.y));
    field(f, "gfx.fog-b", offsetof(RenderSettings, fog_color.z));
    field(f, "gfx.exposure", offsetof(RenderSettings, exposure));
    field(f, "gfx.tonemap", offsetof(RenderSettings, tonemap));
    field(f, "gfx.saturation", offsetof(RenderSettings, saturation));
    field(f, "gfx.contrast", offsetof(RenderSettings, contrast));
    field(f, "gfx.vignette", offsetof(RenderSettings, vignette));
    field(f, "gfx.grain", offsetof(RenderSettings, grain));
    field(f, "gfx.shadows", offsetof(RenderSettings, shadows));
    field(f, "gfx.light-x", offsetof(RenderSettings, light_dir.x));
    field(f, "gfx.light-y", offsetof(RenderSettings, light_dir.y));
    field(f, "gfx.light-z", offsetof(RenderSettings, light_dir.z));
    field(f, "gfx.light-r", offsetof(RenderSettings, light_color.x));
    field(f, "gfx.light-g", offsetof(RenderSettings, light_color.y));
    field(f, "gfx.light-b", offsetof(RenderSettings, light_color.z));
    field(f, "gfx.ambient-r", offsetof(RenderSettings, ambient.x));
    field(f, "gfx.ambient-g", offsetof(RenderSettings, ambient.y));
    field(f, "gfx.ambient-b", offsetof(RenderSettings, ambient.z));
    field(f, "gfx.rim", offsetof(RenderSettings, rim));
    field(f, "gfx.debug", offsetof(RenderSettings, debug));
    field(f, "gfx.room-fog", offsetof(RenderSettings, room_fog));
    field(f, "gfx.room-tint", offsetof(RenderSettings, room_tint));
    field(f, "gfx.room-bloom", offsetof(RenderSettings, room_bloom));
    f->m.current = saved;
}
