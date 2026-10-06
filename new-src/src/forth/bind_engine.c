/* The engine's Forth words. Scripts drive the game with these; C keeps the data.
 *
 * Structs are reached by address plus field words: `camera cam.yaw sf@` reads the camera's yaw
 * (a field word adds its offset). Floats in structs are 32-bit: sf@ / sf!. */
#include "../game/engine.h"

#include "../core/files.h"

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
    forth_printf(f, "room %03X: %zu bytes, %d triangles in %d draws (solid %d, see-through %d, glow %d), %d textures\n",
                 r->id, r->pac.size, r->mesh.nv / 3, r->mesh.nd, parts[MESH_SOLID], parts[MESH_SEE_THROUGH],
                 parts[MESH_GLOW], r->ntextures);
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
    FPUSH((v[3] == 0.0f ? 60.0f : v[3]) * 3.14159265358979 / 180.0);
    FPUSH(v[4]); FPUSH(v[5]); FPUSH(v[6]);
}

/* ---- the camera ---- */

PRIM(p_camera) { PUSH(&gEngine.camera); }

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
PRIM(p_key_down) { PUSH(keys()[scancode(f)] ? -1 : 0); }
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
}

void bind_engine(Forth *f) {
    static const struct {
        const char *name;
        Code code;
    } prims[] = {
        {"room", p_room}, {"room-id", p_room_id}, {"room-exists?", p_room_exists},
        {"room-bounds", p_room_bounds}, {"room-cameras", p_room_cameras}, {"room-camera", p_room_camera}, {"room-group!", p_room_group}, {".room", p_room_info},
        {"camera", p_camera},
        {"key-down?", p_key_down}, {"key-pressed?", p_key_pressed}, {"mouse-dx", p_mouse_dx},
        {"mouse-dy", p_mouse_dy}, {"mouse-down?", p_mouse_down},
        {"on-tick", p_on_tick}, {"off-tick", p_off_tick}, {"ticks", p_ticks}, {"dt", p_dt},
        {"clear-color", p_clear_color}, {"screenshot", p_screenshot}, {"console!", p_console},
        {"data-dir", p_data_dir},
    };
    size_t i;

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
}
