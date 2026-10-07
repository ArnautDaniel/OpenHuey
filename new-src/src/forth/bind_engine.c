/* The engine's Forth words. Scripts drive the game with these; C keeps the data.
 *
 * Structs are reached by address plus field words: `camera cam.yaw sf@` reads the camera's yaw
 * (a field word adds its offset). Floats in structs are 32-bit: sf@ / sf!. */
#include "../game/engine.h"
#include "../game/areas.h"
#include "../game/camdirector.h"
#include "../game/cutscene.h"
#include "../game/exits.h"
#include "../game/messages.h"
#include "../data/soundbank.h"
#include "../platform/sound.h"
#include "../platform/movie.h"
#include "../platform/music.h"
#include "../platform/seq.h"

#include "../core/files.h"

#include <math.h>
#include <stdlib.h>
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
#define PATH_MAX_POINTS 64
static Vec3 sPath[PATH_MAX_POINTS];
static int sPathN;
PRIM(p_nav_path) {   /* ( from to -- n ) ( F: ax ay az bx by bz -- ) a way between two points on their
                      * triangles, kept off the blocked flags (nav-block!): its turning points */
    float bz = (float)FPOP(), by = (float)FPOP(), bx = (float)FPOP();
    float az = (float)FPOP(), ay = (float)FPOP(), ax = (float)FPOP();
    Cell to = POP(), from = POP();

    sPathN = navmesh_path(&gEngine.room.nav, (int)from, vec3(ax, ay, az), (int)to, vec3(bx, by, bz), sPath, PATH_MAX_POINTS);
    PUSH(sPathN);
}
PRIM(p_nav_path_point) {   /* ( i -- ) ( F: -- x y z ) the last way's point i */
    Cell i = POP();
    Vec3 p = i >= 0 && i < sPathN ? sPath[i] : vec3(0, 0, 0);

    FPUSH(p.x);
    FPUSH(p.y);
    FPUSH(p.z);
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

/* ---- the camera director (game/camdirector.c) ---- */

PRIM(p_cam_new_room) { camdir_new_room(&gCamDir); }   /* ( -- ) the start of play: nothing set */
PRIM(p_cam_room_start) {   /* ( -- ) this room's camera sets and paths taken, the camera put */
    size_t size;
    int n;
    const float *sets = room_cameras(&n);
    const uint8_t *paths = pac_section(&gEngine.room.pac, PAC_SECTION6, &size);

    camdir_room_start(&gCamDir, sets, n, paths != NULL && size >= 8 ? (const int32_t *)paths : NULL);
}
PRIM(p_cam_setup) {   /* ( set path -- ) */
    Cell path = POP(), set = POP();

    camdir_set_setup(&gCamDir, (int)set, (int)path);
}
PRIM(p_cam_follow) {   /* ( actor -- ) follow an actor (10 units up), -1 nobody */
    Cell a = POP();

    camdir_follow(&gCamDir, (int)a, a < 0 ? vec3(0, 0, 0) : vec3(0, 10, 0));
}
PRIM(p_cam_ease) { camdir_ease(&gCamDir); }
PRIM(p_cam_track) { camdir_track(&gCamDir); }
PRIM(p_cam_update) {   /* ( -- ) the camera placed, and the engine's camera made the director's */
    camdir_update(&gCamDir);
    camdir_apply(&gCamDir, &gEngine.camera);
}
PRIM(p_cam_restart) { camdir_restart(&gCamDir); }
PRIM(p_cam_changed) { PUSH(camdir_setup_changed(&gCamDir) ? -1 : 0); }
PRIM(p_cam_info) {   /* ( -- ) for the console */
    forth_printf(f, "set %d path %d (of %d) following %d, t %.1f of %d..%d, fov %.1f deg\n", gCamDir.set,
                 gCamDir.path_no, gCamDir.npaths, gCamDir.target, gCamDir.path.t, gCamDir.path.tmin,
                 gCamDir.path.tmax, gCamDir.fov * 57.29578f);
    forth_printf(f, "eye %.1f %.1f %.1f  looking at %.1f %.1f %.1f\n", gCamDir.eye.x, gCamDir.eye.y, gCamDir.eye.z,
                 gCamDir.look.x, gCamDir.look.y, gCamDir.look.z);
}

/* ---- the room's event areas (game/areas.c) ---- */

PRIM(p_area_in) {   /* ( area -- flag ) ( F: x y z -- ) */
    Cell area = POP();
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP();

    PUSH(area_inside(&gEngine.room, (int)area, vec3(x, y, z)) ? -1 : 0);
}
PRIM(p_placed_op) {   /* ( addr len op arg -- ) the room's object by name: 0 shown (arg), 1 / 2 animation
                         * arg once / looped, 3 animation stopped, 4 hidden and back as defined */
    Cell arg = POP(), op = POP(), n = POP(), a = POP();
    char name[32];
    Placed *p;

    snprintf(name, sizeof(name), "%.*s", (int)n, (const char *)a);
    p = placed_named(&gEngine.room.placed, name);
    if (p == NULL) {
        return;
    }
    switch (op) {
    case 0:
        p->shown = arg != 0;
        break;
    case 1:
    case 2:
        placed_anim(&gEngine.room.placed, p, (int)arg, op == 2);
        break;
    case 3:
        p->keys = NULL;
        p->frames = p->frame = 0;
        break;
    case 4:
        p->shown = 0;
        p->rot = p->def_rot;
        p->pos = p->def_pos;
        break;
    }
}
PRIM(p_placed_list) {   /* ( -- ) the room's objects */
    int i;

    for (i = 0; i < gEngine.room.placed.n; i++) {
        const Placed *p = &gEngine.room.placed.p[i];

        forth_printf(f, "%-16s kind %d %s at %.1f %.1f %.1f, %d triangles\n", p->name, p->kind, p->shown ? "shown" : "hidden",
                     p->pos.x, p->pos.y, p->pos.z, p->mesh.nv / 3);
    }
}
PRIM(p_door_swing) {   /* ( exit at-once -- ) ( F: degrees -- ) the room's door at that exit swung (-90 open) */
    Cell now = POP(), exit = POP();

    room_door_swing(&gEngine.room, (int)exit, (float)FPOP(), now != 0);
}
PRIM(p_area_middle) {   /* ( area -- flag ) ( F: -- x y z ) its first and third corners' middle */
    Vec3 m = vec3(0, 0, 0);
    int ok = area_middle(&gEngine.room, (int)POP(), &m);

    FPUSH(m.x);
    FPUSH(m.y);
    FPUSH(m.z);
    PUSH(ok ? -1 : 0);
}
PRIM(p_area_cross) {   /* ( area -- n ) ( F: px py pz x y z -- ) 1 in, -1 out, 0 */
    Cell area = POP();
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP();
    float pz = (float)FPOP(), py = (float)FPOP(), px = (float)FPOP();

    PUSH(area_cross(&gEngine.room, (int)area, vec3(px, py, pz), vec3(x, y, z)));
}
PRIM(p_exit_spot) {   /* ( exit which -- tri ) ( F: -- x y z ) where to stand at an exit: 0 out, 1 in,
                         * 2 through (tri -1: nowhere; 0 0 0) */
    Cell which = POP(), exit = POP();
    Vec3 p = vec3(0, 0, 0);
    int tri = exit_spot(&gEngine.room, &gEngine.world, (int)exit, (int)which, &p);

    PUSH(tri);
    FPUSH(p.x);
    FPUSH(p.y);
    FPUSH(p.z);
}
/* ---- messages (game/messages.c): laid out in pages of lines, for the window in Forth ---- */

static const MessageLayout *sMsg;
PRIM(p_message_layout) {   /* ( id -- pages ) lay message `id` out (bit 15 system, 14 the second table) */
    sMsg = message_layout((int)POP());
    PUSH(sMsg->npages);
}
PRIM(p_message_lines) {   /* ( page -- n ) */
    Cell pg = POP();

    PUSH(sMsg != NULL && pg >= 0 && pg < sMsg->npages ? sMsg->pages[pg].nlines : 0);
}
PRIM(p_message_line) {   /* ( page line -- addr len ) */
    Cell ln = POP(), pg = POP();

    if (sMsg == NULL || pg < 0 || pg >= sMsg->npages || ln < 0 || ln >= sMsg->pages[pg].nlines) {
        PUSH(0);
        PUSH(0);
        return;
    }
    PUSH(sMsg->pages[pg].lines[ln]);
    PUSH(strlen(sMsg->pages[pg].lines[ln]));
}
PRIM(p_message_options) { PUSH(sMsg != NULL ? sMsg->noptions : 0); }   /* ( -- n ) */
PRIM(p_message_option) {   /* ( i -- page line col leads-to ) */
    Cell i = POP();
    const MessageOption *o = sMsg != NULL && i >= 0 && i < sMsg->noptions ? &sMsg->options[i] : NULL;

    PUSH(o ? o->page : -1);
    PUSH(o ? o->line : -1);
    PUSH(o ? o->col : 0);
    PUSH(o ? o->leads_to : 0xFFFF);
}
PRIM(p_message_choice_flags) { PUSH(sMsg != NULL ? sMsg->choice_flags : 0); }
PRIM(p_message_param) {   /* ( slot id -- ) parameter `slot` shows system message `id` */
    Cell id = POP(), slot = POP();

    message_set_param((int)slot, (int)id);
}

/* ---- the nav mesh's flags: what blocks whom, and the room's triangle groups ---- */

PRIM(p_nav_block) {   /* ( mask -- ) triangles with these flags are walls to the next moves */
    gEngine.room.nav.block = (uint32_t)POP();
}
/* the room's triangle groups (PAC section 14: a count, offsets of {n, triangles}) */
static const uint8_t *nav_group(int g, uint32_t *n) {
    size_t size;
    const uint8_t *sec = pac_section(&gEngine.room.pac, PAC_OBSTACLES, &size);
    uint32_t count, off;

    if (sec == NULL || size < 4) {
        return NULL;
    }
    memcpy(&count, sec, 4);
    if (g < 0 || (uint32_t)g >= count || (size_t)(g + 2) * 4 > size) {
        return NULL;
    }
    memcpy(&off, sec + 4 + g * 4, 4);
    if ((size_t)off + 4 > size) {
        return NULL;
    }
    memcpy(n, sec + off, 4);
    if ((size_t)off + 4 + (size_t)*n * 4 > size) {
        return NULL;
    }
    return sec + off + 4;
}
static void tri_flags(int t, int set, uint32_t bits) {
    NavMesh *nm = &gEngine.room.nav;

    if (t >= 0 && t < nm->ntris) {
        nm->tris[t].flags = set ? nm->tris[t].flags | bits : nm->tris[t].flags & ~bits;
    }
}
PRIM(p_nav_group) {   /* ( set? group bits -- ) the group's triangles' flags set or cleared (NavGroups) */
    uint32_t bits = (uint32_t)POP(), n, i, t;
    Cell g = POP(), set = POP();
    const uint8_t *tris = nav_group((int)g, &n);

    for (i = 0; tris != NULL && i < n; i++) {
        memcpy(&t, tris + i * 4, 4);
        tri_flags((int)t, set != 0, bits);
    }
}
PRIM(p_nav_tri_flags) {   /* ( set? tri bits -- ) one triangle's */
    uint32_t bits = (uint32_t)POP();
    Cell t = POP(), set = POP();

    tri_flags((int)t, set != 0, bits);
}
PRIM(p_nav_in_group) {   /* ( tri group -- flag ) */
    Cell g = POP(), t = POP();
    uint32_t n, i, k;
    const uint8_t *tris = nav_group((int)g, &n);
    int in = 0;

    for (i = 0; tris != NULL && i < n && !in; i++) {
        memcpy(&k, tris + i * 4, 4);
        in = (Cell)k == t;
    }
    PUSH(in ? -1 : 0);
}
PRIM(p_nav_flags) {   /* ( tri -- flags ) */
    Cell t = POP();

    PUSH(t >= 0 && t < gEngine.room.nav.ntris ? gEngine.room.nav.tris[t].flags : 0);
}

PRIM(p_exit_door) {   /* ( exit -- door | -1 ) the door this room's exit goes through (the door table) */
    Cell exit = POP();
    int room = gEngine.room.id;

    PUSH(room >= 0 && room < WORLD_ROOMS && exit >= 0 && exit < ROOM_EXITS ? gEngine.world.exits[room][exit].door : -1);
}
PRIM(p_door_flags) {   /* ( door -- flags ) its fixed flags (the door table; bit 0 a doorway) */
    Cell d = POP();

    PUSH(d >= 0 && d < gEngine.world.ndoors ? (Cell)(gEngine.world.doors[d].flags & 0xFF) : 0);
}
/* ---- sounds: the common bank (C_0000, the original's bank 5: Fiona's, Hewie's, the house's) ---- */

static SoundBank sCommon, sSet, sRoomBank;
static int sSetNo = -1, sRoomBankNo = -1, sWantSet;
static int common_bank(void) {
    return sCommon.hd != NULL || soundbank_load(&sCommon, "C_0000");
}
/* the game's banks by number: 4 the sound set (D_n000), 5 the common bank (C_0000), 6 the room's
 * (ST_xxx/ST1_xxx); NULL if not there */
static const SoundBank *bank_no(int bank) {
    char name[64];

    switch (bank) {
    case 5:
        return common_bank() ? &sCommon : NULL;
    case 4:
        if (sSetNo != sWantSet) {
            soundbank_free(&sSet);
            snprintf(name, sizeof(name), "D_%01X000", sWantSet & 0xF);
            sSetNo = sWantSet;
            soundbank_load(&sSet, name);
        }
        return sSet.hd != NULL ? &sSet : NULL;
    case 6:
        if (sRoomBankNo != gEngine.room.id) {
            soundbank_free(&sRoomBank);
            snprintf(name, sizeof(name), "ST_%03X/ST1_%03X", gEngine.room.id & ~7, gEngine.room.id);
            sRoomBankNo = gEngine.room.id;
            if (gEngine.room.id >= 0) {
                soundbank_load(&sRoomBank, name);
            }
        }
        return sRoomBank.hd != NULL ? &sRoomBank : NULL;
    }
    return NULL;
}
/* sound `id` (its low 16 bits) of bank `bank` at volume / pan */
static void play_bank(int id, int bank, float vol, float pan) {
    const SoundBank *b = bank_no(bank);
    int n;
    int16_t *pcm;

    if (b == NULL) {
        return;
    }
    pcm = soundbank_render(b, id & 0xFFFF, SOUND_RATE, &n);
    sound_play(pcm, n, vol, pan);
    free(pcm);
}
PRIM(p_bank_sound) {   /* ( id bank -- ) a sound heard plainly */
    Cell bank = POP(), id = POP();

    play_bank((int)id, (int)bank, 1.0f, 0.0f);
}
PRIM(p_bank_sound_at) {   /* ( id bank -- ) ( F: x y z -- ) a sound at a point: fainter further from the
                           * camera (full within 50, none past 600: the original's curves are its
                           * own data, not read yet), to the side it is on */
    Cell bank = POP(), id = POP();
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP();
    Vec3 d = vec3_sub(vec3(x, y, z), gEngine.camera.pos);
    float dist = vec3_len(d), vol = dist <= 50.0f ? 1.0f : fmaxf(0.0f, 1.0f - (dist - 50.0f) / 550.0f);
    float side = sinf(atan2f(d.x, -d.z) - gEngine.camera.yaw);

    if (vol > 0.0f) {
        play_bank((int)id, (int)bank, vol, side * 0.8f);
    }
}
PRIM(p_sound_set) { sWantSet = (int)POP(); }   /* ( set -- ) the sound set bank 4 holds */
PRIM(p_common_sound) {   /* ( id -- ) play sound `id` of the common bank */
    Cell id = POP();
    int n;
    int16_t *pcm;

    if (!common_bank()) {
        return;
    }
    pcm = soundbank_render(&sCommon, (int)id, SOUND_RATE, &n);
    sound_play(pcm, n, 1.0f, 0.0f);
    free(pcm);
}
PRIM(p_sound_to_wav) {   /* ( id bank addr len -- ) a bank's sound `id` into a .wav file (checking) */
    Cell len = POP(), a = POP(), bank = POP(), id = POP();
    char path[512];
    int n = 0;
    const SoundBank *sb = bank_no((int)bank);
    int16_t *pcm = sb != NULL ? soundbank_render(sb, (int)id & 0xFFFF, SOUND_RATE, &n) : NULL;
    FILE *fp;
    uint32_t u;
    uint16_t s;

    snprintf(path, sizeof(path), "%.*s", (int)len, (const char *)a);
    fp = fopen(path, "wb");
    if (fp == NULL) {
        free(pcm);
        forth_error(f, "sound-to-wav: can't write %s", path);
    }
    fwrite("RIFF", 1, 4, fp); u = 36 + n * 2; fwrite(&u, 4, 1, fp);
    fwrite("WAVEfmt ", 1, 8, fp); u = 16; fwrite(&u, 4, 1, fp);
    s = 1; fwrite(&s, 2, 1, fp); fwrite(&s, 2, 1, fp);
    u = SOUND_RATE; fwrite(&u, 4, 1, fp); u = SOUND_RATE * 2; fwrite(&u, 4, 1, fp);
    s = 2; fwrite(&s, 2, 1, fp); s = 16; fwrite(&s, 2, 1, fp);
    fwrite("data", 1, 4, fp); u = n * 2; fwrite(&u, 4, 1, fp);
    if (pcm != NULL) {
        fwrite(pcm, 2, (size_t)n, fp);
    }
    fclose(fp);
    free(pcm);
    forth_printf(f, "wrote %s: %d samples\n", path, n);
}

/* ---- a stage: lights of the scripts' own (the title) ---- */

PRIM(p_stage_light) {   /* ( i -- ) ( F: x y z r g b range -- ) light i (0..2) of the stage; colours 0..255 */
    Cell i = POP();
    float range = (float)FPOP(), b = (float)FPOP(), g = (float)FPOP(), r = (float)FPOP();
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP();

    if (i >= 0 && i < 3) {
        gEngine.stage[i] = (RoomLight){vec3(x, y, z), vec3(r, g, b), 1.0f, range};
    }
}
PRIM(p_stage_lights) { gEngine.nstage = (int)POP(); }   /* ( n -- ) how many (0: the room's again) */
PRIM(p_stage_ambient) {   /* ( F: r g b -- ) */
    float b = (float)FPOP(), g = (float)FPOP(), r = (float)FPOP();

    gEngine.stage_ambient = vec3(r, g, b);
}
PRIM(p_room_clear) { room_free(&gEngine.room); }   /* ( -- ) no room: nothing drawn round the actors */

/* ---- movies (platform/movie.c) ---- */

static GpuTexture sMovieTex;
static int sMovieW, sMovieH;
PRIM(p_movie_open) {   /* ( addr len -- flag ) start a movie (a path in the data folder) */
    Cell n = POP(), a = POP();
    char path[256];

    snprintf(path, sizeof(path), "%.*s", (int)n, (const char *)a);
    PUSH(movie_open(path) ? -1 : 0);
}
PRIM(p_movie_status) { PUSH(movie_status()); }   /* ( -- n ) 0 none, 1 playing, 2 over */
PRIM(p_movie_frame) { PUSH(movie_frame()); }     /* ( -- n ) the frame shown, -1 none */
PRIM(p_movie_close) { movie_close(); }
PRIM(p_movie_pause) { movie_pause((int)POP()); }   /* ( on -- ) */
PRIM(p_movie_compose) {   /* ( compo lo hi -- ) */
    Cell hi = POP(), lo = POP();

    movie_compose((int)POP(), (int)lo, (int)hi);
}
PRIM(p_movie_paused) { PUSH(movie_paused() ? -1 : 0); }
PRIM(p_movie_volume) { movie_volume((float)FPOP()); }   /* ( F: v -- ) */
PRIM(p_movie_draw) {   /* ( x y w h -- ) the movie's picture there (window pixels) */
    Cell dh = POP(), dw = POP(), y = POP(), x = POP();
    int pw, ph, fresh;
    const uint8_t *px = movie_picture(&pw, &ph, &fresh);

    if (px != NULL && (fresh || sMovieTex == 0 || pw != sMovieW || ph != sMovieH)) {
        sMovieTex = render_texture_stream(sMovieTex, px, pw, ph, &sMovieW, &sMovieH);
    }
    if (sMovieTex != 0 && px != NULL) {
        render_image(sMovieTex, (float)x, (float)y, (float)dw, (float)dh, 0xFFFFFFFFu);
    }
}

/* ---- the cutscene director (game/cutscene.c) ---- */

PRIM(p_cs_start) {   /* ( addr len -- flag ) start the scene in that folder (e.g. EV0006) */
    Cell n = POP(), a = POP();
    char name[64];

    snprintf(name, sizeof(name), "%.*s", (int)n, (const char *)a);
    PUSH(cutscene_start(name) ? -1 : 0);
}
PRIM(p_cs_run) { cutscene_run_state(); }              /* ( -- ) its state a step */
PRIM(p_cs_go) { cutscene_start_when_ready(); }        /* ( -- ) start when ready */
PRIM(p_cs_frame_set) { cutscene_set_frame((int)POP()); }   /* ( n -- ) */
PRIM(p_cs_frame) { PUSH(cutscene_frame()); }
PRIM(p_cs_update) { cutscene_update(); }
PRIM(p_cs_end) { cutscene_end(); }
PRIM(p_cs_status) { PUSH(cutscene_status()); }        /* ( -- n ) 1 loading 2 ready 3 playing 5 over */
PRIM(p_cs_in_shot) { PUSH(cutscene_playing_shot() ? -1 : 0); }
PRIM(p_cs_near_end) { PUSH(cutscene_near_end() ? -1 : 0); }
PRIM(p_cs_shot_at) { PUSH(cutscene_shot_at((int)POP())); }   /* ( frame -- shot ) -1 none */
PRIM(p_cs_signals) { PUSH(cutscene_signal_count((int)POP())); }   /* ( bit -- n ) since last frame */
PRIM(p_music_play) {   /* ( addr len loop paused -- flag ) a track (ADX file) streamed */
    Cell paused = POP(), loop = POP(), n = POP(), a = POP();
    char path[256];

    snprintf(path, sizeof(path), "%.*s", (int)n, (const char *)a);
    PUSH(music_play(path, loop != 0, paused != 0) ? -1 : 0);
}
PRIM(p_music_stop) { music_stop(); }
PRIM(p_music_pause) { music_pause(POP() != 0); }   /* ( on -- ) */
PRIM(p_music_volume) { music_volume((float)FPOP()); }   /* ( F: v -- ) 0..1 */
PRIM(p_music_playing) { PUSH(music_playing() ? -1 : 0); }
static void path_arg(Forth *f, char *out, size_t n) {   /* ( addr len -- ) */
    Cell len = POP(), a = POP();

    snprintf(out, n, "%.*s", (int)len, (const char *)a);
}
PRIM(p_seq_bank) { char p[128]; path_arg(f, p, sizeof(p)); PUSH(seq_bank(p) ? -1 : 0); }   /* ( addr len -- flag ) */
PRIM(p_seq_load) {   /* ( k addr len -- flag ) */
    char p[128];

    path_arg(f, p, sizeof(p));
    PUSH(seq_load((int)POP(), p) ? -1 : 0);
}
PRIM(p_seq_play) { Cell on = POP(); seq_play((int)POP(), on != 0); }   /* ( k on -- ) */
PRIM(p_seq_playing) { PUSH(seq_playing((int)POP()) ? -1 : 0); }
PRIM(p_seq_volume) { Cell v = POP(); seq_volume((int)POP(), (int)v); }   /* ( k v -- ) */
PRIM(p_seq_port_volume) { Cell v = POP(); seq_port_volume((int)POP(), (int)v); }
PRIM(p_seq_chan_volume) { Cell v = POP(), ch = POP(); seq_chan_volume((int)POP(), (int)ch, (int)v); }   /* ( k ch v -- ) */
PRIM(p_seq_midi) {   /* ( k status d1 d2 -- ) */
    Cell d2 = POP(), d1 = POP(), st = POP();

    seq_midi((int)POP(), (int)st, (int)d1, (int)d2);
}
PRIM(p_seq_reset) { seq_reset(); }
PRIM(p_pause) { gEngine.paused = POP() != 0; }   /* ( flag -- ) actors and the room stand still */
PRIM(p_cs_active) { PUSH(cutscene_active() ? -1 : 0); }
PRIM(p_cs_total) { PUSH(cutscene_signal_total((int)POP())); }   /* ( bit -- n ) its count so far less one */
PRIM(p_cs_letterbox_off) { cutscene_set_letterbox_off((int)POP() != 0); }   /* ( flag -- ) */

PRIM(p_exit_area) {   /* ( exit -- area ) the event area of this room's exit (the room table) */
    Cell exit = POP();
    int room = gEngine.room.id;

    if (room < 0 || room >= WORLD_ROOMS || exit < 0 || exit >= ROOM_EXITS) {
        PUSH(0xFFFF);
        return;
    }
    PUSH(gEngine.world.exits[room][exit].camera & 0xFFFF);
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
PRIM(p_root_delta) {   /* ( id -- ) ( F: -- turn dx dy dz ) its motion's root movement this frame
                         * (model space, scaled to the room) */
    Actor *a = actor_arg(f, POP());
    float turn;
    Vec3 step;

    model_root_delta(&a->model, a->motion, a->frame, &turn, &step);
    FPUSH(turn);
    FPUSH(step.x * a->scale);
    FPUSH(step.y * a->scale);
    FPUSH(step.z * a->scale);
}
PRIM(p_has_motion) {   /* ( id motion-id -- flag ) */
    Cell mid = POP();
    Actor *a = actor_arg(f, POP());

    PUSH(model_motion_find(&a->model, (int)mid) >= 0 ? -1 : 0);
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
PRIM(p_key_pressed) {
    Cell k = scancode(f);

    PUSH(gEngine.input.pressed[k] || gEngine.held_pressed[k] ? -1 : 0);
}
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
PRIM(p_look) { PUSH(&gRoomLook); }   /* ( -- addr ) the room's look, as it is now */
PRIM(p_look_reset) { room_reset_look(&gEngine.room); }   /* back to the room file's */
PRIM(p_look_set) {   /* ( slot addr n -- ) a look effect's parameters (addr 0: removed) */
    Cell n = POP(), a = POP(), slot = POP();

    room_look_set((int)slot, a != 0 ? (const uint8_t *)a : NULL, (size_t)(n > 0 ? n : 0));
}

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
    movie_update();
    for (i = 0; i < SDL_SCANCODE_COUNT; i++) {   /* keys scripts put down count as pressed once */
        e->held_pressed[i] = e->held[i] && !e->held_last[i];
        e->held_last[i] = e->held[i];
    }
    forth_run_tasks(e->forth);
    for (i = 0; i < e->nhooks; i++) {
        if (forth_call(e->forth, e->hooks[i]) != 0) {
            forth_printf(e->forth, "on-tick: %s removed after an error\n", e->hooks[i]->name);
            memmove(&e->hooks[i], &e->hooks[i + 1], (size_t)(e->nhooks - i - 1) * sizeof(Word *));
            e->nhooks--;
            i--;
        }
    }
    if (e->paused) {
        render_look_tick();
        return;
    }
    for (i = 0; i < MAX_ACTORS; i++) {
        actor_tick(&e->actors[i]);
    }
    cutscene_camera(&e->camera);   /* a cutscene's shot with the camera in it has it */
    room_tick(&e->room);
    render_look_tick();
}

void engine_draw_2d(Engine *e) {
    int i;

    if (cutscene_letterbox()) {   /* (Cutscene_Letterbox: 56 lines of 448 at the top and bottom) */
        float bar = (float)e->height * 56.0f / 448.0f;

        render_rect(0.0f, 0.0f, (float)e->width, bar, 0x000000FFu);
        render_rect(0.0f, (float)e->height - bar, (float)e->width, bar, 0x000000FFu);
    }

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
        {"cam-new-room", p_cam_new_room}, {"cam-room-start", p_cam_room_start}, {"cam-setup", p_cam_setup},
        {"cam-follow", p_cam_follow}, {"cam-ease", p_cam_ease}, {"cam-track", p_cam_track},
        {"cam-update", p_cam_update}, {"cam-restart", p_cam_restart}, {"cam-changed?", p_cam_changed},
        {".director", p_cam_info}, {"area-in?", p_area_in}, {"nav-path", p_nav_path}, {"nav-path-point", p_nav_path_point}, {"placed-op", p_placed_op}, {".placed", p_placed_list}, {"door-swing", p_door_swing}, {"area-middle", p_area_middle}, {"area-cross", p_area_cross}, {"exit-area", p_exit_area}, {"movie-open", p_movie_open}, {"movie-status", p_movie_status},
        {"movie-frame", p_movie_frame}, {"cutscene-load", p_cs_start}, {"cutscene-run", p_cs_run}, {"cutscene-go", p_cs_go},
        {"cutscene-frame!", p_cs_frame_set}, {"cutscene-frame", p_cs_frame}, {"cutscene-update", p_cs_update}, {"cutscene-end", p_cs_end},
        {"cutscene-status", p_cs_status}, {"cutscene-in-shot?", p_cs_in_shot}, {"cutscene-near?", p_cs_near_end}, {"cutscene-shot-at", p_cs_shot_at},
        {"cutscene-signals", p_cs_signals}, {"cutscene-active?", p_cs_active}, {"pause!", p_pause}, {"seq-bank", p_seq_bank}, {"seq-load", p_seq_load}, {"seq-play", p_seq_play},
        {"seq-playing?", p_seq_playing}, {"seq-volume", p_seq_volume}, {"seq-port-volume", p_seq_port_volume},
        {"seq-chan-volume", p_seq_chan_volume}, {"seq-midi", p_seq_midi}, {"seq-reset", p_seq_reset}, {"music-play", p_music_play}, {"music-stop", p_music_stop},
        {"music-pause", p_music_pause}, {"music-volume!", p_music_volume}, {"music-playing?", p_music_playing}, {"cutscene-signal-total", p_cs_total}, {"cutscene-letterbox-off", p_cs_letterbox_off}, {"movie-close", p_movie_close}, {"movie-pause", p_movie_pause}, {"movie-paused?", p_movie_paused}, {"movie-compose", p_movie_compose},
        {"movie-volume!", p_movie_volume}, {"movie-draw", p_movie_draw}, {"stage-light", p_stage_light}, {"stage-lights", p_stage_lights},
        {"stage-ambient", p_stage_ambient}, {"room-clear", p_room_clear}, {"common-sound", p_common_sound},
        {"bank-sound", p_bank_sound}, {"bank-sound-at", p_bank_sound_at}, {"sound-set!", p_sound_set}, {"sound-to-wav", p_sound_to_wav}, {"exit-door", p_exit_door}, {"door-flags", p_door_flags},
        {"nav-block!", p_nav_block}, {"nav-group!", p_nav_group}, {"nav-tri-flags!", p_nav_tri_flags},
        {"nav-in-group?", p_nav_in_group}, {"nav-flags", p_nav_flags},
        {"message-layout", p_message_layout}, {"message-lines", p_message_lines}, {"message-line", p_message_line},
        {"message-options", p_message_options}, {"message-option", p_message_option},
        {"message-choice-flags", p_message_choice_flags}, {"message-param!", p_message_param}, {"exit-spot", p_exit_spot},
        {"actor-load", p_actor_load}, {"actor-free", p_actor_free}, {"actor", p_actor},
        {"motion!", p_motion_store}, {"has-motion?", p_has_motion}, {"root-delta", p_root_delta}, {"motion@", p_motion_fetch}, {"motion-done?", p_motion_done},
        {"motion-frames", p_motion_frames}, {".motions", p_motions},
        {"key-down?", p_key_down}, {"key-hold", p_key_hold}, {"key-pressed?", p_key_pressed}, {"mouse-dx", p_mouse_dx},
        {"mouse-dy", p_mouse_dy}, {"mouse-down?", p_mouse_down},
        {"on-tick", p_on_tick}, {"off-tick", p_off_tick}, {"ticks", p_ticks}, {"dt", p_dt},
        {"on-draw", p_on_draw}, {"off-draw", p_off_draw}, {"pen-color", p_pen_color},
        {"pen-scale", p_pen_scale}, {"draw-text", p_draw_text}, {"draw-rect", p_draw_rect},
        {"screen-size", p_screen_size}, {"char-size", p_char_size}, {"n>s", p_n_to_s}, {"f>s$", p_f_to_s},
        {"gfx", p_gfx}, {"room-look", p_look}, {"room-look-reset", p_look_reset}, {"look-set", p_look_set}, {"user-dir", p_user_dir}, {"file-exists?", p_file_exists}, {"to-file", p_to_file},
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
    field(f, "gfx.room-dof", offsetof(RenderSettings, room_dof));
    /* the room's look (render.h RoomLook): flags 32-bit, colours 4 floats r g b a, distances floats */
    field(f, "look.fog", offsetof(RoomLook, has_fog));
    field(f, "look.fog-near-color", offsetof(RoomLook, fog_near_color));
    field(f, "look.fog-far-color", offsetof(RoomLook, fog_far_color));
    field(f, "look.fog-near", offsetof(RoomLook, fog_near));
    field(f, "look.fog-far", offsetof(RoomLook, fog_far));
    field(f, "look.tint", offsetof(RoomLook, has_tint));
    field(f, "look.tint-glow", offsetof(RoomLook, tint_glow));
    field(f, "look.tint-contrast", offsetof(RoomLook, tint_contrast));
    field(f, "look.bloom", offsetof(RoomLook, has_bloom));
    field(f, "look.bloom-color", offsetof(RoomLook, bloom));
    field(f, "look.bloom-mode", offsetof(RoomLook, bloom_mode));
    field(f, "look.dof", offsetof(RoomLook, has_dof));
    field(f, "look.dof-range", offsetof(RoomLook, dof));
    field(f, "gfx.room-lights", offsetof(RenderSettings, room_lights));
    field(f, "gfx.character-light", offsetof(RenderSettings, character_light));
    field(f, "gfx.shadow-maps", offsetof(RenderSettings, shadow_maps));
    field(f, "gfx.shadow-strength", offsetof(RenderSettings, shadow_strength));
    field(f, "gfx.room-tint", offsetof(RenderSettings, room_tint));
    field(f, "gfx.room-bloom", offsetof(RenderSettings, room_bloom));
    f->m.current = saved;
}
