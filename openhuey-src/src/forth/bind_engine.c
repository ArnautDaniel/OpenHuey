/* The engine's Forth words. Scripts drive the game with these; C keeps the data.
 *
 * Structs are reached by address plus field words: `camera cam.yaw sf@` reads the camera's yaw
 * (a field word adds its offset). Floats in structs are 32-bit: sf@ / sf!. */
#include "../game/engine.h"
#include "../data/exetables.h"
#include "../hactor/hactor.h"
#include "../game/areas.h"
#include "../game/camdirector.h"
#include "../game/cutscene.h"
#include "../game/exits.h"
#include "../game/messages.h"
#include "../platform/disc.h"
#include "../game/progress.h"
#include "../game/sprites.h"
#include "../game/particles.h"
#include "../game/rumble.h"
#include "../game/swimpath.h"
#include "../game/obstacles.h"
#include "../game/things.h"
#include "../render/post.h"
#include "../platform/snddrv.h"
#include "../platform/sound.h"
#include "../platform/movie.h"
#include "../platform/music.h"
#include "../platform/seq.h"

#include "../core/files.h"
#include "../data/artpic.h"
#include "../data/texbank.h"

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
    particles_new_room();   /* (the engine's own sparks and motes go with the room) */
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
PRIM(p_door_group) {   /* ( group flag -- ) every door's model part `group` shown or not (Doors_SetBits) */
    Cell on = POP(), g = POP();
    int i;

    if (g < 0 || g > 255) {
        forth_error(f, "door-group!: group %ld out of range", (long)g);
    }
    for (i = 0; i < ROOM_DOORS; i++) {
        RoomDoor *d = &gEngine.room.doors[i];

        if (!d->present) {
            continue;
        }
        if (on) {
            d->groups[g >> 5] |= 1u << (g & 31);
        } else {
            d->groups[g >> 5] &= ~(1u << (g & 31));
        }
    }
}
PRIM(p_light_own) {   /* ( i -- ) room light i back to the room file's (Lights_RestoreLight) */
    Cell i = POP();

    if (i >= 0 && i < gEngine.room.nlights) {
        gEngine.room.lights[i] = gEngine.room.own_lights[i];
    }
}
PRIM(p_light_scale) {   /* ( i which -- ) ( F: k -- ) room light i's intensity (which 1) or shadow
                         * length (2) times k (the scripts' 0x90) */
    float k = (float)FPOP();
    Cell which = POP(), i = POP();

    if (i >= 0 && i < gEngine.room.nlights) {
        if (which == 1) {
            gEngine.room.lights[i].intensity *= k;
        } else if (which == 2) {
            gEngine.room.lights[i].shadow *= k;
        }
    }
}
PRIM(p_door_group_q) {   /* ( exit group -- flag ) that door's model part `group` is shown */
    Cell g = POP(), i = POP();

    PUSH(i >= 0 && i < ROOM_DOORS && g >= 0 && g < 256 && gEngine.room.doors[i].present &&
                 (g == 0 || (gEngine.room.doors[i].groups[g >> 5] >> (g & 31) & 1u)) ? -1 : 0);
}
PRIM(p_light_fetch) {   /* ( i -- ) ( F: -- intensity shadow ) room light i's (0 0: none) */
    Cell i = POP();
    const RoomLight *l = i >= 0 && i < gEngine.room.nlights ? &gEngine.room.lights[i] : NULL;

    FPUSH(l != NULL ? l->intensity : 0.0f);
    FPUSH(l != NULL ? l->shadow : 0.0f);
}
PRIM(p_look_on) {   /* ( slot -- flag ) the look's effect in that slot ($1C..$1F) is on */
    Cell slot = POP();
    int on = slot == 0x1F ? gRoomLook.has_tint : slot == 0x1E ? gRoomLook.has_bloom :
             slot == 0x1D ? gRoomLook.has_fog : slot == 0x1C ? gRoomLook.has_dof : 0;

    PUSH(on ? -1 : 0);
}
/* ---- sprites (game/sprites.h): an effect's batch of camera-facing quads ---- */
PRIM(p_sprites_new) { PUSH(sprites_new()); }        /* ( -- b ) a batch, -1 none free */
PRIM(p_sprites_free) { sprites_free((int)POP()); }  /* ( b -- ) */
PRIM(p_sprites_free_all) { sprites_free_all(); }    /* ( -- ) */
PRIM(p_sprites_texture) {   /* ( b group id palette -- ) its texture: the cache's group + id, palette (-1 the first) */
    Cell pal = POP(), id = POP(), group = POP();

    sprites_texture((int)POP(), (int)group, (int)id, (int)pal);
}
PRIM(p_sprites_cells) {   /* ( b x y cw ch tw th frames -- ) its frames' cells of a tw x th sheet */
    Cell frames = POP(), th = POP(), tw = POP(), ch = POP(), cw = POP(), y = POP(), x = POP();

    sprites_cells((int)POP(), (int)x, (int)y, (int)cw, (int)ch, (int)tw, (int)th, (int)frames);
}
PRIM(p_sprites_flags) {   /* ( b flags layer -- ) SPRITE_* flags, the draw order */
    Cell layer = POP(), flags = POP();

    sprites_flags((int)POP(), (int)flags, (int)layer);
}
PRIM(p_sprites_offset) {   /* ( b -- ) ( F: cx cy -- ) the corners' offset */
    float cy = (float)FPOP(), cx = (float)FPOP();

    sprites_offset((int)POP(), cx, cy);
}
PRIM(p_sprites_corner) {   /* ( b k -- ) ( F: x y z -- ) its own corner k */
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP();
    Cell k = POP();

    sprites_corner((int)POP(), (int)k, x, y, z);
}
PRIM(p_sprites_uv) {   /* ( b -- ) ( F: u0 v0 u1 v1 -- ) its cell in texture coordinates */
    float v1 = (float)FPOP(), u1 = (float)FPOP(), v0 = (float)FPOP(), u0 = (float)FPOP();

    sprites_uv((int)POP(), u0, v0, u1, v1);
}
PRIM(p_sprite_quad) {   /* ( b i rgba -- ) ( F: x0 y0 z0 .. x3 y3 z3 -- ) record i: a quad with its own corners
                         * (0 1 2 3: two triangles 0 1 2 and 2 1 3; the cell's corners in that order) */
    float c[4][3];
    int k;
    Cell rgba = POP(), i = POP(), b = POP();
    Sprite *s = sprites_records((int)b, -1);

    for (k = 3; k >= 0; k--) {
        c[k][2] = (float)FPOP();
        c[k][1] = (float)FPOP();
        c[k][0] = (float)FPOP();
    }
    if (s != NULL && i >= 0 && i < SPRITES_PER_BATCH) {
        memset(&s[i], 0, sizeof(s[i]));
        s[i].rgba[0] = (uint8_t)(rgba >> 24);
        s[i].rgba[1] = (uint8_t)(rgba >> 16);
        s[i].rgba[2] = (uint8_t)(rgba >> 8);
        s[i].rgba[3] = (uint8_t)rgba;
        s[i].own = 1;
        memcpy(s[i].c, c, sizeof(c));
    }
}
PRIM(p_sprites_count) {   /* ( b n -- ) how many it draws (records 0..n-1) */
    Cell n = POP();

    sprites_records((int)POP(), (int)n);
}
PRIM(p_sprite_store) {   /* ( b i rgba frame -- ) ( F: x y z w h rot -- ) record i: rgba $RRGGBBAA
                          * (0x80 = 1.0), half sizes w h, turned rot about the view axis */
    float rot = (float)FPOP(), half_h = (float)FPOP(), half_w = (float)FPOP();
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP();
    Cell frame = POP(), rgba = POP(), i = POP(), b = POP();
    Sprite *s = sprites_records((int)b, -1);

    if (s != NULL && i >= 0 && i < SPRITES_PER_BATCH) {
        s[i] = (Sprite){x, y, z, half_w, half_h, rot, {(uint8_t)(rgba >> 24), (uint8_t)(rgba >> 16), (uint8_t)(rgba >> 8), (uint8_t)rgba},
                        (int)frame, 0, {{0}}, 0, {{0}}, 0, {{0}}};
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
/* ---- vectors in memory (3 floats: x y z), e.g. a character's position ---- */

static float *vec_arg(Forth *f) { return (float *)POP(); }
PRIM(p_vec_store) { float *v = vec_arg(f); v[2] = (float)FPOP(); v[1] = (float)FPOP(); v[0] = (float)FPOP(); }   /* ( v -- ) ( F: x y z -- ) */
PRIM(p_vec_fetch) { float *v = vec_arg(f); FPUSH(v[0]); FPUSH(v[1]); FPUSH(v[2]); }   /* ( v -- ) ( F: -- x y z ) */
PRIM(p_vec_copy) { float *src = vec_arg(f), *dst = vec_arg(f); memmove(dst, src, 12); }   /* ( dst src -- ) */
PRIM(p_vec_dist) {   /* ( a b -- ) ( F: -- d ) apart (Actor_Distance) */
    float *b = vec_arg(f), *a = vec_arg(f), x = b[0] - a[0], y = b[1] - a[1], z = b[2] - a[2];

    FPUSH(sqrtf(x * x + y * y + z * z));
}
PRIM(p_vec_dist_xz) {   /* ( a b -- ) ( F: -- d ) apart on the level */
    float *b = vec_arg(f), *a = vec_arg(f), x = b[0] - a[0], z = b[2] - a[2];

    FPUSH(sqrtf(x * x + z * z));
}
PRIM(p_vec_heading) {   /* ( a b -- ) ( F: -- yaw ) the heading from a to b (atan2 dx dz: 0 along +z) */
    float *b = vec_arg(f), *a = vec_arg(f);

    FPUSH(atan2f(b[0] - a[0], b[2] - a[2]));
}
PRIM(p_vec_ahead) {   /* ( dst src -- ) ( F: yaw d -- ) dst = src moved d along the heading */
    float d = (float)FPOP(), yaw = (float)FPOP(), *src = vec_arg(f), *dst = vec_arg(f);

    dst[0] = src[0] + sinf(yaw) * d;
    dst[1] = src[1];
    dst[2] = src[2] + cosf(yaw) * d;
}
PRIM(p_angle_wrap) {   /* ( F: a -- a' ) into -pi .. pi (Angle_Wrap) */
    float a = (float)FPOP();

    while (a > 3.14159265f) {
        a -= 6.28318531f;
    }
    while (a < -3.14159265f) {
        a += 6.28318531f;
    }
    FPUSH(a);
}

/* the nav mesh with positions in memory (vectors) and a blocked mask for this call */
static NavMesh *nav_masked(uint32_t mask) {
    gEngine.room.nav.block = mask;
    return &gEngine.room.nav;
}
PRIM(p_v_nav_move) {   /* ( v mask -- tri ) ( F: dx dz -- ) v moved by (dx, dz) over the mesh within
                        * the mask (sliding, following the floor): its triangle */
    uint32_t mask = (uint32_t)POP();
    float *v = vec_arg(f), dz = (float)FPOP(), dx = (float)FPOP(), h;
    NavMesh *n = nav_masked(mask);
    Vec3 p = navmesh_move(n, vec3(v[0], v[1], v[2]), dx, dz, 8.0f, 0.0f);

    v[0] = p.x;
    v[1] = p.y;
    v[2] = p.z;
    n->block = 0;
    PUSH(navmesh_find(n, p, 4.0f, &h));
}
/* ---- bodies (the actors' own: hactor.h) moved over the nav mesh; the actor being run moves
 * its own, anyone may look at any (Actor_Move / MoveAny / TurnToward / HeadingTo / TriTo /
 * FreeDistance / Touching) ---- */
static HBody *own_body(Forth *f, const char *what) {
    HBody *b = hactor_body(hactor_self());

    if (b == NULL || !b->has) {
        forth_error(f, "%s: no actor with a body is being run", what);
    }
    return b;
}
static HBody *any_body(Forth *f, Cell id, const char *what) {
    HBody *b = hactor_body((int)id);

    if (b == NULL || !b->has) {
        forth_error(f, "%s: actor %ld has no body", what, (long)id);
    }
    return b;
}
static void body_move(HBody *b, uint32_t mask, float dx, float dz) {
    NavMesh *n = nav_masked(mask);
    Vec3 p = navmesh_move(n, vec3(b->pos[0], b->pos[1], b->pos[2]), dx, dz, 8.0f, 0.0f);
    float h;
    int t;

    b->pos[0] = p.x;
    b->pos[1] = p.y;
    b->pos[2] = p.z;
    n->block = 0;
    t = navmesh_find(n, p, 4.0f, &h);
    if (t >= 0) {
        b->tri = t;
    }
}
PRIM(p_body_move) {   /* ( F: dx dz -- ) by (dx, dz) as far as its mask allows, sliding along walls */
    HBody *b = own_body(f, "body-move");
    float dz = (float)FPOP(), dx = (float)FPOP();

    body_move(b, b->mask, dx, dz);
}
PRIM(p_body_advance) {   /* ( F: x z speed -- ) ( -- there? ) straight toward (x, z) by at most speed, as
                          * far as its mask allows; there (within the step) - put on it */
    HBody *b = own_body(f, "body-advance");
    float speed = (float)FPOP(), z = (float)FPOP(), x = (float)FPOP();
    float dx = x - b->pos[0], dz = z - b->pos[2], d = sqrtf(dx * dx + dz * dz);

    if (d <= speed) {
        body_move(b, b->mask, dx, dz);
        PUSH(-1);
        return;
    }
    body_move(b, b->mask, dx / d * speed, dz / d * speed);
    PUSH(0);
}
PRIM(p_body_move_any) {   /* ( F: dx dz -- ) the same over any triangle */
    HBody *b = own_body(f, "body-move-any");
    float dz = (float)FPOP(), dx = (float)FPOP();

    body_move(b, 0, dx, dz);
}
PRIM(p_body_move_local) {   /* ( F: x z -- ) a step in its own frame (x to its right, z ahead) */
    HBody *b = own_body(f, "body-move-local");
    float z = (float)FPOP(), x = (float)FPOP(), c = cosf(b->yaw), sn = sinf(b->yaw);

    body_move(b, b->mask, c * x + sn * z, c * z - sn * x);
}
PRIM(p_body_climb) {   /* ( F: x y z -- ) a step in its own frame off the floor - up or down a ladder
                        * (Pursuer_Stairs: its root motion turned by its heading), its triangle kept */
    HBody *b = own_body(f, "body-climb");
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP(), c = cosf(b->yaw), sn = sinf(b->yaw);

    b->pos[0] += c * x + sn * z;
    b->pos[1] += y;
    b->pos[2] += c * z - sn * x;
}
PRIM(p_body_turn_toward) {   /* ( F: target step -- left ) its heading toward target by at most step */
    HBody *b = own_body(f, "body-turn-toward");
    float step = (float)FPOP(), target = (float)FPOP();
    float d = atan2f(sinf(target - b->yaw), cosf(target - b->yaw));

    if (fabsf(d) <= step) {
        b->yaw = target;
        FPUSH(0.0f);
        return;
    }
    b->yaw += d < 0.0f ? -step : step;
    b->yaw = atan2f(sinf(b->yaw), cosf(b->yaw));
    FPUSH(fabsf(atan2f(sinf(target - b->yaw), cosf(target - b->yaw))));
}
PRIM(p_body_heading_to) {   /* ( id v -- ) ( F: -- yaw ) from its body to v (its own heading when above / below) */
    float *v = vec_arg(f);
    HBody *b = any_body(f, POP(), "body-heading-to");
    float dx = v[0] - b->pos[0], dz = v[2] - b->pos[2];

    FPUSH(dx == 0.0f && dz == 0.0f ? b->yaw : atan2f(dx, dz));
}
PRIM(p_body_place_at) {   /* ( F: x y z -- ) its own, its triangle found under it (within its mask, else any) */
    HBody *b = own_body(f, "body-place-at");
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP(), h;
    NavMesh *n = nav_masked(b->mask);
    int t = navmesh_find(n, vec3(x, y, z), 4.0f, &h);

    n->block = 0;
    if (t < 0) {
        t = navmesh_find(n, vec3(x, y, z), 4.0f, &h);
    }
    b->pos[0] = x;
    b->pos[1] = y;
    b->pos[2] = z;
    b->tri = t;
    b->room = gEngine.room.id;
}
PRIM(p_body_tri_to) {   /* ( v mask -- tri ) walking straight from it to v (mask -1: its own): v's triangle, -1 a wall first */
    Cell mask = POP();
    float *v = vec_arg(f);
    HBody *b = own_body(f, "body-tri-to");
    NavMesh *n = nav_masked(mask == -1 ? b->mask : (uint32_t)mask);

    PUSH(navmesh_walk(n, b->tri, vec3(b->pos[0], b->pos[1], b->pos[2]), vec3(v[0], v[1], v[2]), NULL));
    n->block = 0;
}
PRIM(p_body_free) {   /* ( mask -- ) ( F: yaw d -- free ) how far ahead along yaw is free (mask -1: its own) */
    Cell mask = POP();
    HBody *b = own_body(f, "body-free");
    float dist = (float)FPOP(), yaw = (float)FPOP(), reach;
    NavMesh *n = nav_masked(mask == -1 ? b->mask : (uint32_t)mask);
    Vec3 a = vec3(b->pos[0], b->pos[1], b->pos[2]), e = vec3(a.x + sinf(yaw) * dist, a.y, a.z + cosf(yaw) * dist);
    int t = navmesh_walk(n, b->tri, a, e, &reach);

    n->block = 0;
    FPUSH(t >= 0 ? dist : reach);
}
PRIM(p_bodies_touching) {   /* ( a b -- flag ) ( F: margin vmargin -- ) within the lower one's height and their radii */
    float vm = (float)FPOP(), m = (float)FPOP();
    HBody *b = hactor_body((int)POP()), *a = hactor_body((int)POP());
    float dx, dz;

    if (a == NULL || b == NULL || !a->has || !b->has || a->room != b->room) {
        PUSH(0);
        return;
    }
    if (a->pos[1] < b->pos[1] ? b->pos[1] - a->pos[1] > a->height + vm : a->pos[1] - b->pos[1] > b->height + vm) {
        PUSH(0);
        return;
    }
    dx = a->pos[0] - b->pos[0];
    dz = a->pos[2] - b->pos[2];
    PUSH(sqrtf(dx * dx + dz * dz) <= a->radius + b->radius + m ? -1 : 0);
}
PRIM(p_v_walk) {   /* ( tri a b mask -- tri' ) straight from a (on tri) to b within the mask:
                    * b's triangle, -1 if a wall comes first (Actor_TriFrom) */
    uint32_t mask = (uint32_t)POP();
    float *b = vec_arg(f), *a = vec_arg(f);
    Cell tri = POP();
    NavMesh *n = nav_masked(mask);

    PUSH(navmesh_walk(n, (int)tri, vec3(a[0], a[1], a[2]), vec3(b[0], b[1], b[2]), NULL));
    n->block = 0;
}
PRIM(p_nav_crosses) {   /* ( tri a b mask -- flag ) a straight line from a (on tri) toward b crosses
                         * a triangle with all of mask's flags (to b's, or a wall) */
    uint32_t mask = (uint32_t)POP();
    float *b = vec_arg(f), *a = vec_arg(f);
    Cell tri = POP();

    PUSH(navmesh_crosses(&gEngine.room.nav, (int)tri, vec3(a[0], a[1], a[2]), vec3(b[0], b[1], b[2]), mask) ? -1 : 0);
}
PRIM(p_v_wall) {   /* ( tri a b mask -- flag ) ( F: -- yaw ) the wall a straight walk from a toward b
                    * meets: its edge's heading (Fiona_AlongWall); false: none */
    uint32_t mask = (uint32_t)POP();
    float *b = vec_arg(f), *a = vec_arg(f), yaw = 0.0f;
    Cell tri = POP();
    NavMesh *n = nav_masked(mask);
    int hit = navmesh_wall(n, (int)tri, vec3(a[0], a[1], a[2]), vec3(b[0], b[1], b[2]), &yaw);

    n->block = 0;
    FPUSH(yaw);
    PUSH(hit ? -1 : 0);
}
PRIM(p_nav_square_step) {   /* ( a b -- a' b' ok? ) ( F: dx dz -- ) a square of the pushable floor
                             * (two triangles) one square along (dx, dz) (Obstacle_StepSquare) */
    float dz = (float)FPOP(), dx = (float)FPOP();
    int b = (int)POP(), a = (int)POP();
    int r = navmesh_square_step(&gEngine.room.nav, &a, &b, dx, dz);

    PUSH(a);
    PUSH(b);
    PUSH(r == 0 ? -1 : 0);
}
/* ---- the obstacles (game/obstacles.c) ---- */
PRIM(p_obstacles_reset) { obstacles_reset(); }
PRIM(p_obstacle_placed) { PUSH(obstacle_on((int)POP()) ? -1 : 0); }   /* ( i -- flag ) */
PRIM(p_obstacle_make) {   /* ( i kind a b -- ok? ) */
    Cell b = POP(), a = POP(), kind = POP(), i = POP();

    PUSH(obstacle_make((int)i, (int)kind, (int)a, (int)b) == 0 ? -1 : 0);
}
PRIM(p_obstacle_stop) { obstacle_stop((int)POP()); }   /* ( i -- ) */
PRIM(p_obstacle_square) {   /* ( i -- a b ) its reference square */
    int a, b;

    obstacle_square((int)POP(), &a, &b);
    PUSH(a);
    PUSH(b);
}
PRIM(p_obstacle_at) {   /* ( i -- ) ( F: -- x y z ) where it stands (its model's place) */
    Vec3 p = obstacle_pos((int)POP());

    FPUSH(p.x); FPUSH(p.y); FPUSH(p.z);
}
PRIM(p_obstacle_middle) {   /* ( i -- ) ( F: -- x y z ) its middle */
    Vec3 p = obstacle_middle((int)POP());

    FPUSH(p.x); FPUSH(p.y); FPUSH(p.z);
}
PRIM(p_obstacles_ring_mask) { PUSH(obstacles_ring_mask((int)POP())); }   /* ( tri -- bits ) */
PRIM(p_obstacle_on_square) {   /* ( i tri -- flag ) */
    Cell t = POP(), i = POP();

    PUSH(obstacle_on_square((int)i, (int)t) ? -1 : 0);
}
PRIM(p_obstacle_can_push) {   /* ( i tris n -- flag ) ( F: dx dz -- ) tris: the other characters' triangles */
    float dz = (float)FPOP(), dx = (float)FPOP();
    Cell n = POP(), tris = POP(), i = POP();
    int t[16], k;

    for (k = 0; k < n && k < 16; k++) {
        t[k] = (int)((const Cell *)tris)[k];
    }
    PUSH(obstacle_can_push((int)i, vec3(dx, 0, dz), t, k) ? -1 : 0);
}
PRIM(p_obstacle_push) {   /* ( i -- ) ( F: dx dz -- ) */
    float dz = (float)FPOP(), dx = (float)FPOP();

    obstacle_push((int)POP(), vec3(dx, 0, dz));
}
PRIM(p_obstacle_step) { PUSH(obstacle_step((int)POP()) ? -1 : 0); }   /* ( i -- scrape? ) a frame of its move */
PRIM(p_obstacle_moved) { obstacle_moved((int)POP()); }   /* ( i -- ) */
/* ---- the nav links (ladders: the original's nav door regions) ---- */
static const NavLink *nav_link(Cell i) {
    NavMesh *n = &gEngine.room.nav;

    return i >= 0 && i < n->nlinks ? &n->links[i] : NULL;
}
PRIM(p_nav_next) {   /* ( tri k -- tri' ) the triangle across edge k (0..2), -1 none */
    Cell k = POP(), t = POP();
    NavMesh *n = &gEngine.room.nav;

    PUSH(t >= 0 && t < n->ntris && k >= 0 && k < 3 ? n->tris[t].next[k] : -1);
}
PRIM(p_nav_links) { PUSH(gEngine.room.nav.nlinks); }   /* ( -- n ) */
PRIM(p_nav_link_tri) {   /* ( i side -- tri ) NavMesh_DoorTri; -1 none */
    Cell sd = POP();
    const NavLink *l = nav_link(POP());

    PUSH(l != NULL && sd >= 0 && sd < 2 ? l->tri[sd] : -1);
}
PRIM(p_nav_link_yaw) {   /* ( i side -- ) ( F: -- yaw ) NavMesh_DoorFacing */
    Cell sd = POP();
    const NavLink *l = nav_link(POP());

    FPUSH(l != NULL && sd >= 0 && sd < 2 ? l->yaw[sd] : 0.0f);
}
PRIM(p_nav_link_spot) {   /* ( i side -- ) ( F: -- x y z ) NavMesh_DoorSpot */
    Cell sd = POP();
    const NavLink *l = nav_link(POP());
    Vec3 v = l != NULL && sd >= 0 && sd < 2 ? l->spot[sd] : vec3(0, 0, 0);

    FPUSH(v.x); FPUSH(v.y); FPUSH(v.z);
}
PRIM(p_nav_link_at) {   /* ( v i -- flag ) NavMesh_AtDoorRegion */
    Cell i = POP();
    float *v = vec_arg(f);

    PUSH(navmesh_link_at(&gEngine.room.nav, (int)i, vec3(v[0], v[1], v[2])) ? -1 : 0);
}
PRIM(p_nav_link_side) {   /* ( v tri i -- side ) NavMesh_DoorSide; -1 neither */
    Cell i = POP(), tri = POP();
    float *v = vec_arg(f);

    PUSH(navmesh_link_side(&gEngine.room.nav, (int)i, (int)tri, vec3(v[0], v[1], v[2])));
}
PRIM(p_nav_link_front) {   /* ( i side -- tri ) ( F: ox oz -- x y z ) Actor_DoorFront; -1 none */
    Cell sd = POP(), i = POP();
    float oz = (float)FPOP(), ox = (float)FPOP();
    Vec3 out = vec3(0, 0, 0);
    int t = navmesh_link_front(&gEngine.room.nav, (int)i, (int)sd, ox, oz, &out);

    FPUSH(out.x); FPUSH(out.y); FPUSH(out.z);
    PUSH(t);
}
PRIM(p_v_free) {   /* ( tri v mask -- ) ( F: yaw dist -- free ) how far from v along the heading is
                    * free, up to dist (Actor_FreeDistance) */
    uint32_t mask = (uint32_t)POP();
    float *v = vec_arg(f), dist = (float)FPOP(), yaw = (float)FPOP(), reach;
    Cell tri = POP();
    NavMesh *n = nav_masked(mask);
    Vec3 a = vec3(v[0], v[1], v[2]), b = vec3(v[0] + sinf(yaw) * dist, v[1], v[2] + cosf(yaw) * dist);
    int t = navmesh_walk(n, (int)tri, a, b, &reach);

    n->block = 0;
    FPUSH(t >= 0 ? dist : reach);
}
PRIM(p_v_path) {   /* ( from a to b mask -- n ) a way from a (on tri from) to b (on tri to) within the
                    * mask: its turning points (nav-path-point) */
    uint32_t mask = (uint32_t)POP();
    float *b = vec_arg(f);
    Cell to = POP();
    float *a = vec_arg(f);
    Cell from = POP();
    NavMesh *n = nav_masked(mask);

    sPathN = navmesh_path(n, (int)from, vec3(a[0], a[1], a[2]), (int)to, vec3(b[0], b[1], b[2]), sPath, PATH_MAX_POINTS);
    n->block = 0;
    PUSH(sPathN);
}
PRIM(p_v_path_curved) {   /* ( from a to b mask -- n ) as v-path, its corners rounded as the original's
                           * curved paths are (navmesh_curve) */
    uint32_t mask = (uint32_t)POP();
    float *b = vec_arg(f);
    Cell to = POP();
    float *a = vec_arg(f);
    Cell from = POP();
    NavMesh *n = nav_masked(mask);
    Vec3 straight[PATH_MAX_POINTS];
    int k = navmesh_path(n, (int)from, vec3(a[0], a[1], a[2]), (int)to, vec3(b[0], b[1], b[2]), straight, PATH_MAX_POINTS);

    sPathN = navmesh_curve(n, (int)from, vec3(a[0], a[1], a[2]), straight, k, sPath, PATH_MAX_POINTS);
    n->block = 0;
    PUSH(sPathN);
}
PRIM(p_v_tri) {   /* ( v -- tri ) the triangle under v (any) */
    float *v = vec_arg(f), h;
    NavMesh *n = nav_masked(0);

    PUSH(navmesh_find(n, vec3(v[0], v[1], v[2]), 4.0f, &h));
}
PRIM(p_nav_floor) {   /* ( -- found ) ( F: x y z -- y' ) the walk mesh's floor at (x, z) nearest
                       * height y (8 up at most); y as given if none */
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP(), h = y;
    NavMesh *n = nav_masked(0);
    int t = navmesh_find(n, vec3(x, y, z), 8.0f, &h);

    FPUSH(t >= 0 ? h : y);
    PUSH(t >= 0 ? -1 : 0);
}
PRIM(p_v_tri_in) {   /* ( v mask -- tri ) the triangle under v not of the mask's flags (-1 none) */
    uint32_t mask = (uint32_t)POP();
    float *v = vec_arg(f), h;
    NavMesh *n = nav_masked(mask);
    int t = navmesh_find(n, vec3(v[0], v[1], v[2]), 4.0f, &h);

    n->block = 0;
    PUSH(t);
}
PRIM(p_nav_walk) {   /* ( from -- tri ) ( F: ax ay az bx by bz -- reach ) straight from a toward b over the
                      * mesh (kept off the nav-block! flags): the triangle b is on or -1, and how far
                      * it got */
    float bz = (float)FPOP(), by = (float)FPOP(), bx = (float)FPOP();
    float az = (float)FPOP(), ay = (float)FPOP(), ax = (float)FPOP(), reach;
    Cell from = POP();

    PUSH(navmesh_walk(&gEngine.room.nav, (int)from, vec3(ax, ay, az), vec3(bx, by, bz), &reach));
    FPUSH(reach);
}
PRIM(p_nav_path_point) {   /* ( i -- ) ( F: -- x y z ) the last way's point i */
    Cell i = POP();
    Vec3 p = i >= 0 && i < sPathN ? sPath[i] : vec3(0, 0, 0);

    FPUSH(p.x);
    FPUSH(p.y);
    FPUSH(p.z);
}
PRIM(p_tri_normal) {   /* ( tri -- ) ( F: -- x y z ) its surface's normal, upward (0 1 0: none) */
    Cell i = POP();
    Vec3 n = vec3(0.0f, 1.0f, 0.0f);

    if (i >= 0 && i < gEngine.room.nav.ntris) {
        const NavTri *t = &gEngine.room.nav.tris[i];
        Vec3 a = vec3_sub(t->v[1], t->v[0]), b = vec3_sub(t->v[2], t->v[0]);
        Vec3 c = vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
        float l = sqrtf(c.x * c.x + c.y * c.y + c.z * c.z);

        if (l > 0.0f) {
            n = vec3(c.x / l, c.y / l, c.z / l);
            if (n.y < 0.0f) {
                n = vec3(-n.x, -n.y, -n.z);
            }
        }
    }
    FPUSH(n.x); FPUSH(n.y); FPUSH(n.z);
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
/* ---- the house's doors for any room (the original's gRooms queries; the exit words above are
 * the loaded room's) ---- */
PRIM(p_room_exit_door) {   /* ( room exit -- door | -1 ) */
    Cell exit = POP(), room = POP();

    PUSH(room >= 0 && room < WORLD_ROOMS && exit >= 0 && exit < ROOM_EXITS ? gEngine.world.exits[room][exit].door : -1);
}
PRIM(p_room_exit_leads) {   /* ( room exit -- room' exit' | -1 -1 ) */
    Cell exit = POP(), room = POP();
    int to_exit = -1, r = room >= 0 && room < WORLD_ROOMS ? world_exit_leads(&gEngine.world, (int)room, (int)exit, &to_exit) : -1;

    PUSH(r);
    PUSH(r < 0 ? -1 : to_exit);
}
PRIM(p_door_sides) {   /* ( door -- room0 exit0 tri0 room1 exit1 tri1 ) its two sides (-1s: none) */
    Cell d = POP();
    int k;

    for (k = 0; k < 2; k++) {
        const DoorSide *s = d >= 0 && d < gEngine.world.ndoors ? &gEngine.world.doors[d].side[k] : NULL;

        PUSH(s != NULL ? s->room : -1);
        PUSH(s != NULL ? s->exit : -1);
        PUSH(s != NULL ? s->tri : -1);
    }
}
PRIM(p_room_exit_tri) {   /* ( room exit which -- tri ) its triangle: 0 out, 1 in, 2 through */
    Cell which = POP(), exit = POP(), room = POP();

    PUSH(world_exit_tri(&gEngine.world, (int)room, (int)exit, (int)which));
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
PRIM(p_cam_on) { camdir_camera_on(&gCamDir, (int)POP()); }   /* ( id -- ) follow someone and take their setup (-1: nobody) */
PRIM(p_cam_followed) { PUSH(gCamDir.followed); }   /* ( -- id ) whom it follows (-1: nobody) */
PRIM(p_cam_target_id) { PUSH(gCamDir.target); }    /* ( -- id ) whom the director looks at (-1: nobody) */
PRIM(p_cam_player) { gCamDir.player = (int)POP(); }   /* ( id -- ) Fiona: whom it falls back on */
PRIM(p_cam_char_setup) {   /* ( who set path -- ) a character's camera setup */
    Cell path = POP(), set = POP();

    camdir_char_setup(&gCamDir, (int)POP(), (int)set, (int)path);
}
PRIM(p_cam_setup_of) {   /* ( who -- set path ) a character's camera setup (-1 -1: none) */
    Cell who = POP();

    PUSH(who >= 0 && who < HACTOR_MAX ? gCamDir.setups[who][0] : -1);
    PUSH(who >= 0 && who < HACTOR_MAX ? gCamDir.setups[who][1] : -1);
}
PRIM(p_cam_forget_setups) { camdir_forget_setups(&gCamDir); }   /* ( -- ) every character's setup gone */
PRIM(p_cam_take_followed) { camdir_take_followed(&gCamDir); }   /* ( -- ) whom it follows checked, their setup taken */
PRIM(p_cam_scene) { camdir_scene(&gCamDir, POP() != 0); }       /* ( flag -- ) a cutscene has the camera, or gives it back */
PRIM(p_cam_scene_q) { PUSH(gCamDir.scene ? -1 : 0); }           /* ( -- flag ) a cutscene has it (CamDirector +0x38) */
PRIM(p_cam_next_set) { camdir_next_set(&gCamDir); }             /* ( -- ) the room's next set, nobody followed */
PRIM(p_cam_rate) { camdir_rate(&gCamDir, (float)FPOP()); }      /* ( F: r -- ) how fast along a path (below 0: 6) */
PRIM(p_cam_shake) { gCamDir.shake = (float)FPOP(); }            /* ( F: a -- ) shaken by up to a each way (0: still) */
PRIM(p_cam_cut) { PUSH(camdir_cut(&gCamDir) ? -1 : 0); }        /* ( -- flag ) it cut to another set at its last update */
PRIM(p_cam_remember) { camdir_remember(&gCamDir); }             /* ( -- ) the setup remembered as taken */
PRIM(p_cam_started) { PUSH(gCamDir.started ? -1 : 0); }         /* ( -- flag ) a room has started it */
PRIM(p_cam_angle) { FPUSH(gCamDir.fov * 57.29578f); }   /* ( F: -- deg ) its view angle now, across a 4:3 picture */
PRIM(p_cam_stop) { gCamDir.started = 0; }                       /* ( -- ) a room change: nothing to direct till it starts */
PRIM(p_cam_ease) { camdir_ease(&gCamDir); }
PRIM(p_cam_event) { PUSH(gCamDir.event ? -1 : 0); }   /* ( -- flag ) an event's shot has the camera (the director idle) */
PRIM(p_cam_event_set) { gCamDir.event = POP() != 0; }  /* ( flag -- ) an event's shot takes the camera, or gives it back */
PRIM(p_cam_track) { camdir_track(&gCamDir); }
PRIM(p_cam_update) {   /* ( -- ) the camera placed, and the engine's camera made the director's */
    camdir_update(&gCamDir);
    camdir_apply(&gCamDir, &gEngine.camera);
}
PRIM(p_cam_restart) { camdir_restart(&gCamDir); }
PRIM(p_cam_place) {   /* ( F: ex ey ez lx ly lz -- ) the camera put here, looking there, a 60 degree view (an event's shot) */
    float lz = (float)FPOP(), ly = (float)FPOP(), lx = (float)FPOP();
    float ez = (float)FPOP(), ey = (float)FPOP(), ex = (float)FPOP();

    gCamDir.eye = vec3(ex, ey, ez);
    gCamDir.look = vec3(lx, ly, lz);
    gCamDir.fov = 1.0471976f;
    camdir_apply(&gCamDir, &gEngine.camera);
}
/* ---- the effects' particles (game/particles.h): made here, moved by the engine's tick ---- */
static Vec3 fpop3(Forth *f) {
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP();

    return vec3(x, y, z);
}
PRIM(p_flicker_new) {   /* ( slow F: x y z -- p ) a flickering sprite (-1: none) */
    Vec3 at = fpop3(f);

    PUSH(particles_flicker(at, (int)POP()));
}
PRIM(p_flame_new) {   /* ( kind F: x y z -- p ) a flame */
    Vec3 at = fpop3(f);

    PUSH(particles_flame(at, (int)POP()));
}
PRIM(p_swarm_new) {   /* ( count spread rgba F: x y z -- p ) specks about the point ($RRGGBBAA) */
    Cell rgba = POP(), spread = POP(), count = POP();
    Vec3 at = fpop3(f);

    PUSH(particles_swarm(at, (int)count, (float)spread, (uint32_t)rgba));
}
PRIM(p_splash_new) {   /* ( count spread rgba F: x y z from-x from-y from-z -- p ) a splash, away from `from` */
    Cell rgba = POP(), spread = POP(), count = POP();
    Vec3 from = fpop3(f), at = fpop3(f);

    PUSH(particles_splash(at, from, (int)count, (float)spread, (uint32_t)rgba));
}
PRIM(p_burst_new) {   /* ( kind r g b top F: x y z -- p ) a burst of dust */
    Cell top = POP(), b = POP(), g = POP(), r = POP(), kind = POP();
    Vec3 at = fpop3(f);

    PUSH(particles_burst(at, (int)kind, (int)r, (int)g, (int)b, (int)top));
}
/* ---- the things lying about (game/things.c; scripts/things.fs) ---- */
PRIM(p_thing_new) {   /* ( kind room tri F: x y z vx vy vz -- i | -1 ) a thing, flying */
    Vec3 v = fpop3(f), at = fpop3(f);
    Cell tri = POP(), room = POP(), kind = POP();

    PUSH(things_new((int)kind, (int)room, (int)tri, at, v));
}
PRIM(p_thing_free) { things_free((int)POP()); }   /* ( i -- ) */
PRIM(p_things_clear) { things_clear(); }
PRIM(p_thing_used) { PUSH(things_used((int)POP()) ? -1 : 0); }   /* ( i -- flag ) */
PRIM(p_thing_kind) { PUSH(things_kind((int)POP())); }   /* ( i -- kind ) */
PRIM(p_thing_room) { PUSH(things_room((int)POP())); }   /* ( i -- room ) where it lies */
PRIM(p_thing_tri) { PUSH(things_tri((int)POP())); }     /* ( i -- tri ) its nav triangle */
PRIM(p_thing_state) { PUSH(things_state((int)POP())); }   /* ( i -- state ) its motion (things.h) */
PRIM(p_thing_age) { PUSH(things_age((int)POP())); }     /* ( i -- frames ) since it was let go */
PRIM(p_thing_at) {   /* ( i -- v ) its place (a vector: read it, don't write it) */
    Vec3 *p = things_at((int)POP());
    static float none[3];

    PUSH(p != NULL ? (Cell)p : (Cell)none);
}
PRIM(p_thing_speed) {   /* ( i -- ) ( F: -- s ) how fast it moves (its velocity's length; 0 lying still) */
    Vec3 *v = things_v((int)POP());

    FPUSH(v != NULL ? sqrtf(v->x * v->x + v->y * v->y + v->z * v->z) : 0.0f);
}
PRIM(p_thing_age_store) { int i = (int)POP(); things_set_age(i, (int)POP()); }   /* ( age i -- ) */
PRIM(p_thing_place) {   /* ( i room tri F: x y z -- ) lying there (a save's) */
    Vec3 at = fpop3(f);
    Cell tri = POP(), room = POP();

    things_place((int)POP(), (int)room, (int)tri, at);
}
PRIM(p_thing_kick) {   /* ( i F: dx dy dz -- ) */
    Vec3 dv = fpop3(f);

    things_kick((int)POP(), dv);
}
PRIM(p_thing_fade) { things_fade((int)POP()); }   /* ( i -- ) */
PRIM(p_thing_events) { PUSH(things_events((int)POP())); }   /* ( i -- bits ) 1 burst, 2 bounced, 4 gone, 8 a puff */
PRIM(p_thing_turn) { PUSH(things_turn((int)POP())); }   /* ( i -- 0..255 ) */
PRIM(p_thing_reaches) {   /* ( body i F: across high -- flag ) the body and thing i meet (thing_spans,
                          * thing_near): the body's feet to its top against the thing's place up
                          * to `high` above it, and its place within `across` of the thing's */
    float high = (float)FPOP(), across = (float)FPOP();
    int i = (int)POP();
    HBody *b = hactor_body((int)POP());
    Vec3 *at = things_at(i);
    float cy, ctop, oy, otop, dx, dz;
    int spans;

    if (b == NULL || !b->has || at == NULL) {
        PUSH(0);
        return;
    }
    cy = b->pos[1];
    ctop = cy + b->height;
    oy = at->y;
    otop = oy + high;
    spans = (ctop < otop && !(ctop <= oy)) || (otop < ctop && !(otop <= cy));
    dx = b->pos[0] - at->x;
    dz = b->pos[2] - at->z;
    PUSH(spans && sqrtf(dx * dx + dz * dz) < across ? -1 : 0);
}
PRIM(p_things_step) { Cell running = POP(); things_step((int)POP(), running != 0); }   /* ( room running -- ) */
PRIM(p_spray) {   /* ( tri heavy F: x y z -- ) blood where a blow lands (the engine's: it goes by itself) */
    Cell heavy = POP(), tri = POP();
    Vec3 at = fpop3(f);

    particles_spray(at, (int)tri, heavy != 0);
}
PRIM(p_debris_cloud) {   /* ( r g b F: x y z -- ) droplets flung where a shot strikes (DropletFlash;
                           * the engine's: it goes by itself) */
    Cell b = POP(), g = POP(), r = POP();
    Vec3 at = fpop3(f);

    particles_drop_flash(at, (int)r, (int)g, (int)b);
}
PRIM(p_gun_smoke) {   /* ( F: x y z -- ) smoke rising from a gun as it fires (BoneSmoke; the engine's) */
    particles_bone_smoke(fpop3(f));
}
PRIM(p_spark) {   /* ( life F: x y z rise size growth -- ) a spark that grows and fades (LorenzoSpark; the engine's) */
    float growth = (float)FPOP(), size = (float)FPOP(), rise = (float)FPOP();
    Vec3 at = fpop3(f);

    particles_spark2(at, rise, size, growth, (int)POP());
}
PRIM(p_welling) {   /* ( tri F: x y z -- ) bubbles welling up where the leaving homunculus sinks away, then a stain (Cr19Bubbles; the engine's) */
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP();

    particles_welling(vec3(x, y, z), (int)POP());
}
PRIM(p_sink_ring) {   /* ( rising F: x y z -- ) a ring of light and sparks where someone sinks away (LightRing; the engine's) */
    Vec3 at = fpop3(f);

    particles_sink_ring(at, (int)POP());
}
PRIM(p_water_step) {   /* ( big v -- ) a step into water at v (Character_WaterStep): straight from it,
                       * on water (flags $2008000), at its surface - a ring and droplets; else nothing */
    float *v = vec_arg(f);
    int big = POP() != 0;
    HBody *b = own_body(f, "water-step");
    NavMesh *n = nav_masked(0);
    int t = navmesh_walk(n, b->tri, vec3(b->pos[0], b->pos[1], b->pos[2]), vec3(v[0], v[1], v[2]), NULL);

    n->block = 0;
    if (t < 0 || (gEngine.room.nav.tris[t].flags & 0x2008000u) != 0x2008000u) {
        return;
    }
    particles_water_step(vec3(v[0], navmesh_height(n, t, v[0], v[2]), v[2]), big);
}
PRIM(p_particles_engines) { particles_engines((int)POP()); }   /* ( p -- ) it goes by itself once ended */
PRIM(p_puffs) {   /* ( F: x y z -- ) the burst of a shove or a thing breaking (the engine's) */
    Vec3 at = fpop3(f);

    particles_puffs(at);
}
PRIM(p_shards_new) {   /* ( gs kind tex F: x y z u v du dv floor -- p ) shards textured by a cell of the room's */
    float floor = (float)FPOP(), dv = (float)FPOP(), du = (float)FPOP(), v = (float)FPOP(), u = (float)FPOP();
    Vec3 at = fpop3(f);
    Cell tex = POP(), kind = POP(), gs = POP();

    PUSH(particles_shards(at, (uint32_t)gs, (int)kind, (int)tex, u, v, du, dv, floor));
}
PRIM(p_stones_new) {   /* ( gs tex F: height u v du dv -- p ) falling stones (Effect79B00), the room's texture's part */
    float dv = (float)FPOP(), du = (float)FPOP(), v = (float)FPOP(), u = (float)FPOP(), height = (float)FPOP();
    Cell tex = POP(), gs = POP();

    PUSH(particles_stones(height, (uint32_t)gs, (int)tex, u, v, du, dv));
}
PRIM(p_debris_new) { PUSH(particles_debris((int)POP())); }   /* ( kind -- p ) debris falling down the water tower */
PRIM(p_reflect_set) {   /* ( slot screen strength F: x0 y0 z0 .. x3 y3 z3 -- ) a reflecting quad (Reflection);
                         * the strength's top bits its side panels ($10000000 a window's, $20000000 a door's) */
    Vec3 q[4];
    Cell strength = POP(), screen = POP(), slot = POP();
    int k;

    for (k = 3; k >= 0; k--) {
        q[k] = fpop3(f);
    }
    if (slot < 0 || slot >= 4) {
        return;
    }
    memcpy(gEngine.refl[slot].q, q, sizeof(q));
    gEngine.refl[slot].screen = screen != 0;
    gEngine.refl[slot].strength = (int)(strength & 0xFF);
    gEngine.refl[slot].panels = (int)((uint32_t)strength >> 28 & 0xF);
    gEngine.refl[slot].only = -1;
}
PRIM(p_reflect_only) {   /* ( slot model -- ) the reflection shows that one only (MirrorFragment: its character) */
    Cell model = POP(), slot = POP();

    if (slot >= 0 && slot < 4) {
        gEngine.refl[slot].only = (int)model;
    }
}
PRIM(p_reflect_off) {   /* ( slot -- ) */
    Cell slot = POP();

    if (slot >= 0 && slot < 4) {
        gEngine.refl[slot].strength = 0;
    }
}
PRIM(p_rising_smoke_new) { PUSH(particles_rising_smoke()); }   /* ( -- p ) the smoke rising from castle 2F's kiln */
PRIM(p_dust_shaft_new) { PUSH(particles_dust_shaft()); }   /* ( -- p ) dust down the old mansion's shaft of light */
PRIM(p_spiral_smoke_new) { PUSH(particles_spiral_smoke()); }   /* ( -- p ) smoke spiralling up the old mansion's fire */
PRIM(p_drips_new) { PUSH(particles_drips()); }   /* ( -- p ) water dripping in the old mansion's flooded room */
PRIM(p_one_drip_new) { PUSH(particles_one_drip()); }   /* ( -- p ) a drop now and then in the Chaos Forest */
PRIM(p_rising_motes_new) { PUSH(particles_rising_motes()); }   /* ( -- p ) motes rising in castle 1F's hall */
PRIM(p_embers_new) { PUSH(particles_embers((int)POP())); }   /* ( ending -- p ) embers up castle 1F's fireplace (-1 just lit) */
PRIM(p_flecks_new) {   /* ( F: x y z half-w depth tx ty tz -- ) ( -- p ) flecks drifting across an area */
    Vec3 turn = fpop3(f);
    float depth = (float)FPOP(), half_w = (float)FPOP();
    Vec3 at = fpop3(f);

    PUSH(particles_flecks(at, half_w, depth, turn));
}
PRIM(p_smoke_puffs_new) { PUSH(particles_smoke_puffs()); }   /* ( -- p ) smoke puffs rising in castle B1 */
PRIM(p_wall_shadow_new) { PUSH(particles_wall_shadow()); }   /* ( -- p ) a soft shadow on the old mansion's wall */
PRIM(p_fire_new) { Cell big = POP(); PUSH(particles_fire((int)POP(), big != 0)); }   /* ( spot big -- p ) a fire in the old mansion's B1 */
PRIM(p_steam_new) { PUSH(particles_steam((int)POP())); }   /* ( spot -- p ) puffs and a glow at an old mansion B1 spot */
PRIM(p_floor_glow_new) { PUSH(particles_floor_glow()); }   /* ( -- p ) the old mansion 3F's floor light's glow */
PRIM(p_tv_new) { PUSH(particles_tv((int)POP())); }   /* ( which -- p ) a TV in castle 2F showing the screen */
PRIM(p_particles_place) { Vec3 at = fpop3(f); particles_place((int)POP(), at); }   /* ( p F: x y z -- ) moved there */
PRIM(p_particles_rest) { Cell n = POP(); particles_rest((int)POP(), (int)n); }   /* ( p frames -- ) a flicker's rest */
PRIM(p_big_fire_new) { PUSH(particles_big_fire()); }   /* ( -- p ) the old mansion 1F's fire */
PRIM(p_light_shaft_new) { PUSH(particles_light_shaft()); }   /* ( -- p ) the old mansion B1's beam of light */
PRIM(p_glint_new) { PUSH(particles_glint((int)POP())); }   /* ( model -- p ) a glint on its bone 6 */
/* ( F: nx ny nz tx ty tz x y z -- x0 y0 z0 .. x3 y3 z3 ) a mirror fragment's reflecting square
 * (MirrorFragment_Draw's mask): 8 across in the plane of normal n - turned as its object is (x,
 * then y, then z) - through the object's place (x y z) */
static Vec3 turned_xyz(Vec3 p, Vec3 t) {   /* (turned about x, then y, then z) */
    Vec3 q = vec3(p.x, p.y * cosf(t.x) - p.z * sinf(t.x), p.z * cosf(t.x) + p.y * sinf(t.x));

    q = vec3(q.x * cosf(t.y) + q.z * sinf(t.y), q.y, q.z * cosf(t.y) - q.x * sinf(t.y));
    return vec3(q.x * cosf(t.z) - q.y * sinf(t.z), q.x * sinf(t.z) + q.y * cosf(t.z), q.z);
}
PRIM(p_mirror_square) {
    Vec3 at = fpop3(f), t = fpop3(f), n = vec3_norm(turned_xyz(fpop3(f), t)), u, v;
    int k;

    u = fabsf(n.y) > 0.9f ? vec3(1, 0, 0) : vec3(0, 1, 0);
    v = vec3_norm(vec3_cross(n, u));
    u = vec3_cross(v, n);
    for (k = 0; k < 4; k++) {
        Vec3 q = vec3_add(at, vec3_add(vec3_scale(v, k & 1 ? 4.0f : -4.0f), vec3_scale(u, k & 2 ? 4.0f : -4.0f)));

        FPUSH(q.x); FPUSH(q.y); FPUSH(q.z);
    }
}
PRIM(p_mag_burst_new) {   /* ( prima? F: x y z nx ny nz -- p ) a magnesia going off there, on that floor */
    Vec3 up = fpop3(f), at = fpop3(f);

    PUSH(particles_mag_burst(at, POP() != 0, up));
}
PRIM(p_smoke_trail_new) { PUSH(particles_smoke_trail((int)POP())); }   /* ( thing -- p ) smoke rising from a thing */
PRIM(p_window_flash_new) {   /* ( F: x y z turn -- p ) a dial's lens catching the light */
    float turn = (float)FPOP();
    Vec3 at = fpop3(f);

    PUSH(particles_window_flash(at, turn));
}
PRIM(p_marker_new) {   /* ( model other second? F: size -- p ) a stalker's glow and sparks */
    int second = POP() != 0, other = (int)POP(), model = (int)POP();

    PUSH(particles_marker(model, other, (float)FPOP(), second));
}
PRIM(p_haze_new) { PUSH(particles_haze()); }   /* ( -- p ) the old mansion 1F's haze */
PRIM(p_caustic_new) {   /* ( tex rgba F: x y z size step turn -- p ) a caustic laid flat there */
    float turn = (float)FPOP(), step = (float)FPOP(), size = (float)FPOP();
    Vec3 at = fpop3(f);
    Cell rgba = POP(), tex = POP();

    PUSH(particles_caustic(at, (int)tex, size, (uint32_t)rgba, step, turn));
}
PRIM(p_glow_new) {   /* ( rgba F: x y z size -- p ) a glow standing there */
    float size = (float)FPOP();
    Vec3 at = fpop3(f);

    PUSH(particles_glow(at, (uint32_t)POP(), size));
}
PRIM(p_wisp_column_new) { PUSH(particles_wisp_column()); }   /* ( -- p ) wisps rising up the House of Truth */
PRIM(p_spark_spray_new) { PUSH(particles_spark_spray((int)POP())); }   /* ( side -- p ) sparks sprayed across the House of Truth */
PRIM(p_heat_haze_new) { PUSH(particles_heat_haze()); }   /* ( -- p ) a heat haze over the House of Truth */
PRIM(p_pit_embers_new) { PUSH(particles_pit_embers((int)POP())); }   /* ( flames? -- p ) embers from the House of Truth's pit */
PRIM(p_leaves_new) { PUSH(particles_leaves()); }   /* ( -- p ) leaves blowing across the front garden */
PRIM(p_loop_sprite_new) { Vec3 at = fpop3(f); PUSH(particles_loop_sprite(at)); }   /* ( F: x y z -- p ) a looping sprite there */
PRIM(p_object_glow_new) { Vec3 at = fpop3(f); PUSH(particles_object_glow(at, (int)POP())); }   /* ( idx F: x y z -- p ) a glow under a planet */
PRIM(p_rain_new) { PUSH(particles_rain()); }   /* ( -- p ) drops falling and splashing */
PRIM(p_ash_new) { PUSH(particles_ash()); }   /* ( -- p ) ash flakes rising in the Chaos Forest */
PRIM(p_butterflies_new) {   /* ( count F: x y z -- p ) butterflies making for the point (their colours: the executable's) */
    Vec3 at = fpop3(f);

    PUSH(particles_butterflies(at, (int)POP(), exetable("butterflies", NULL)));
}
PRIM(p_butterflies_to) {   /* ( p count F: x y z -- ) somewhere new, and how many (0: as they are) */
    Vec3 at = fpop3(f);
    Cell count = POP();

    particles_butterflies_to((int)POP(), at, (int)count);
}
PRIM(p_wander_new) {   /* ( F: x y z rx rz tx ty tz -- p ) a quad wandering about the point (degrees) */
    float tz = (float)FPOP(), ty = (float)FPOP(), tx = (float)FPOP(), rz = (float)FPOP(), rx = (float)FPOP();
    Vec3 at = fpop3(f);

    PUSH(particles_wander(at, rx, rz, vec3(tx * 0.017453293f, ty * 0.017453293f, tz * 0.017453293f)));
}
PRIM(p_spirit_new) {   /* ( red F: x y z bob -- p ) a spirit's glow (creatures) */
    float bob = (float)FPOP();
    Vec3 at = fpop3(f);

    PUSH(particles_spirit(at, POP() != 0, bob));
}
PRIM(p_spirit_at) {   /* ( p shown F: x y z -- ) where it is now, and whether it shows */
    Vec3 at = fpop3(f);
    Cell shown = POP();

    particles_spirit_at((int)POP(), at, shown != 0);
}
PRIM(p_swim_path) {   /* ( i nx ny nz frames -- ) ( F: x y z -- ) swim path i set up from there (SwimPath_Setup) */
    Vec3 o = fpop3(f);
    Cell frames = POP(), nz = POP(), ny = POP(), nx = POP();

    swimpath_setup((int)POP(), (int)nx, (int)ny, (int)nz, (int)frames, o);
}
PRIM(p_swim_step) {   /* ( i -- ) ( F: x y z yaw -- x y z yaw ) a frame along swim path i (SwimPath_Step) */
    float yaw = (float)FPOP();
    Vec3 at = fpop3(f);

    swimpath_step((int)POP(), &at, &yaw);
    FPUSH(at.x); FPUSH(at.y); FPUSH(at.z); FPUSH(yaw);
}
PRIM(p_vanish_new) {   /* ( r g b a -- p ) ( F: x y z -- ) a creature's vanishing there (CreatureVanish) */
    Vec3 at = fpop3(f);
    Cell a = POP(), b = POP(), g = POP(), r = POP();

    PUSH(particles_vanish(at, (int)r, (int)g, (int)b, (int)a));
}
PRIM(p_spirit_vanish) { Cell seen = POP(); particles_spirit_vanish((int)POP(), seen != 0); }   /* ( p seen -- ) */
PRIM(p_particles_command) { Cell v = POP(); particles_command((int)POP(), (int)v); }   /* ( p v -- ) */
PRIM(p_particles_alive) { PUSH(particles_alive((int)POP()) ? -1 : 0); }             /* ( p -- flag ) */
PRIM(p_particles_children) { PUSH(particles_children((int)POP())); }                /* ( p -- n ) */
PRIM(p_particles_info) {   /* ( p -- kind frame rest ) (tests) its kind, its frame, frames left resting */
    int kind = 0, frame = 0, rest = 0;

    particles_info((int)POP(), &kind, &frame, &rest);
    PUSH(kind);
    PUSH(frame);
    PUSH(rest);
}
PRIM(p_particles_free) { particles_free((int)POP()); }                              /* ( p -- ) */
PRIM(p_frame_blend) { post_trail((float)POP() / 128.0f); }   /* ( a -- ) this frame over the last at a / 128 (0x80 all; one frame) */
PRIM(p_cam_retake) { camdir_retake(&gCamDir); }   /* ( -- ) the setup taken anew, as if it had changed (back from an event's shot) */
PRIM(p_cam_set) { PUSH(gCamDir.scene ? -1 : gCamDir.set); }   /* ( -- set ) the camera's setup (CamDirector +0x24: -1 while a cutscene has it) */
PRIM(p_cam_changed) {   /* ( -- flag ) its setup changed this frame (CamDirector +0x2C) */
    PUSH(gCamDir.changed || camdir_setup_changed(&gCamDir) ? -1 : 0);
}
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
PRIM(p_placed_op) {   /* ( addr len op arg -- ) the room's object by name: 0 hidden or not (arg: the
                         * original's +0 byte - PlacedObjects_Draw draws only while it is 0), 1 / 2
                         * animation arg once / looped, 3 animation stopped, 4 shown and back as defined,
                         * 5 its place and turn now kept as its defined ones (arg unused) */
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
        p->shown = arg == 0;
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
        p->shown = 1;
        p->rot = p->def_rot;
        p->pos = p->def_pos;
        break;
    case 5:
        p->def_rot = p->rot;
        p->def_pos = p->pos;
        break;
    }
}
static uint32_t *placed_arg(Forth *f) {   /* ( addr len off -- ) the named object's word */
    Cell off = POP(), n = POP(), a = POP();
    char name[32];
    Placed *p;

    snprintf(name, sizeof(name), "%.*s", (int)n, (const char *)a);
    p = placed_named(&gEngine.room.placed, name);
    return p != NULL ? placed_word(p, (int)off) : NULL;
}
PRIM(p_placed_f_fetch) {   /* ( addr len off -- ) ( F: -- v ) a float of the room's object by name
                            * (its offset as the original's: +0x10 the turn, +0x20 the place;
                            * 0 when there is no such object) */
    uint32_t *word = placed_arg(f);
    float v = 0.0f;

    if (word != NULL) {
        memcpy(&v, word, 4);
    }
    FPUSH(v);
}
PRIM(p_placed_f_store) {   /* ( addr len off -- ) ( F: v -- ) */
    uint32_t *word = placed_arg(f);
    float v = (float)FPOP();

    if (word != NULL) {
        memcpy(word, &v, 4);
    }
}
PRIM(p_placed_l_fetch) {   /* ( addr len off -- n ) a word of it as an integer */
    uint32_t *word = placed_arg(f);

    PUSH(word != NULL ? (Cell)(int32_t)*word : 0);
}
PRIM(p_placed_l_store) {   /* ( n addr len off -- ) */
    uint32_t *word = placed_arg(f);
    Cell v = POP();

    if (word != NULL) {
        *word = (uint32_t)v;
    }
}
PRIM(p_placed_here) {   /* ( addr len -- flag ) the room has an object of that name */
    Cell n = POP(), a = POP();
    char name[32];

    snprintf(name, sizeof(name), "%.*s", (int)n, (const char *)a);
    PUSH(placed_named(&gEngine.room.placed, name) != NULL ? -1 : 0);
}
PRIM(p_placed_shown) {   /* ( addr len -- flag ) the room's object of that name is drawn (its +0 0) */
    Cell n = POP(), a = POP();
    char name[32];
    const Placed *p;

    snprintf(name, sizeof(name), "%.*s", (int)n, (const char *)a);
    p = placed_named(&gEngine.room.placed, name);
    PUSH(p != NULL && p->shown ? -1 : 0);
}
PRIM(p_placed_at) {   /* ( addr len -- ) ( F: x y z -- ) the room's object by name moved there */
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP();
    Cell n = POP(), a = POP();
    char name[32];
    Placed *p;

    snprintf(name, sizeof(name), "%.*s", (int)n, (const char *)a);
    p = placed_named(&gEngine.room.placed, name);
    if (p != NULL) {
        p->pos = vec3(x, y, z);
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
PRIM(p_door_user_spot) {   /* ( exit anim -- tri ) ( F: -- x y z yaw ) where the door's animation puts its user
                           * (Doors_AnimUserSpot; tri -1: none) */
    Cell anim = POP(), exit = POP();
    Vec3 at = vec3(0.0f, 0.0f, 0.0f);
    float yaw = 0.0f;
    int tri = room_door_user_spot(&gEngine.room, (int)exit, (int)anim, &at, &yaw);

    FPUSH(at.x); FPUSH(at.y); FPUSH(at.z); FPUSH(yaw);
    PUSH(tri);
}
PRIM(p_door_anim) {   /* ( exit anim -- flag ) the door swings along that animation (Doors_StartAnim) */
    Cell anim = POP(), exit = POP();

    PUSH(room_door_anim_start(&gEngine.room, (int)exit, (int)anim) ? -1 : 0);
}
PRIM(p_door_side) {   /* ( exit -- side ) ( F: x y z -- ) 1 in front of it, 0 behind, -1 no door (Doors_Side) */
    Cell exit = POP();
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP();

    PUSH(room_door_side(&gEngine.room, (int)exit, vec3(x, y, z)));
}
PRIM(p_door_near) {   /* ( exit -- flag ) ( F: x y z -- ) within 20 of where it stands (Doors_AtDoor) */
    Cell exit = POP();
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP();

    PUSH(room_door_near(&gEngine.room, (int)exit, vec3(x, y, z)) ? -1 : 0);
}
PRIM(p_door_in_area) {   /* ( exit kind -- flag ) ( F: x y z -- ) in its area `kind` (Doors_InArea) */
    Cell kind = POP(), exit = POP();
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP();

    PUSH(room_door_in_area(&gEngine.room, (int)exit, (int)kind, vec3(x, y, z)) ? -1 : 0);
}
PRIM(p_door_side_flags) {   /* ( exit side flags set? -- ) the door's side triangles' flags set / cleared
                            * (Doors_Passage: the doors' +0x1C shut a side, +0x20 open it) */
    Cell set = POP(), flags = POP(), side = POP(), exit = POP();

    room_door_side_flags(&gEngine.room, (int)exit, (int)side, (uint32_t)flags, set != 0);
}
PRIM(p_door_on_side) {   /* ( exit side tri -- flag ) the triangle is in that side's list (Doors_OnSide) */
    Cell tri = POP(), side = POP(), exit = POP();

    PUSH(room_door_on_side(&gEngine.room, (int)exit, (int)side, (int)tri) ? -1 : 0);
}
PRIM(p_door_animating) {   /* ( exit -- flag ) its animation is still going (not Doors_IsIdle) */
    Cell exit = POP();

    PUSH(exit >= 0 && exit < ROOM_DOORS && gEngine.room.doors[exit].keys != NULL ? -1 : 0);
}
PRIM(p_door_at) {   /* ( exit -- flag ) ( F: -- x y z ) where the room's door at that exit stands
                     * (Doors_GetPos); false: no door there */
    Cell exit = POP();
    const RoomDoor *d = exit >= 0 && exit < ROOM_DOORS ? &gEngine.room.doors[exit] : NULL;

    if (d == NULL || !d->present) {
        FPUSH(0.0);
        FPUSH(0.0);
        FPUSH(0.0);
        PUSH(0);
        return;
    }
    FPUSH(d->pos.x);
    FPUSH(d->pos.y);
    FPUSH(d->pos.z);
    PUSH(-1);
}
PRIM(p_out_of_view) {   /* ( -- flag ) ( F: x y z -- ) outside the camera's view volume as the original
                         * sees it: a 4:3 picture, 1 to 2000 deep (Camera_OutOfView) */
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP();

    PUSH(camera_out_of_view(&gEngine.camera, vec3(x, y, z)) ? -1 : 0);
}
PRIM(p_to_screen) {   /* ( -- flag ) ( F: x y z -- sx sy ) a point of the room on the window (pixels;
                       * false: behind the camera) */
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP();
    float aspect = render_aspect(), ww = (float)gEngine.width, hh = (float)gEngine.height, pw = ww, ox = 0.0f;
    Mat4 vp = mat4_mul(camera_proj(&gEngine.camera, aspect), camera_view(&gEngine.camera));
    float cx = vp.m[0] * x + vp.m[4] * y + vp.m[8] * z + vp.m[12];
    float cy = vp.m[1] * x + vp.m[5] * y + vp.m[9] * z + vp.m[13];
    float cw = vp.m[3] * x + vp.m[7] * y + vp.m[11] * z + vp.m[15];

    if (gRender.aspect == 1) {   /* (4:3 pillarboxed) */
        pw = hh * 4.0f / 3.0f;
        ox = (ww - pw) / 2.0f;
    }
    if (cw <= 0.01f) {
        FPUSH(0.0);
        FPUSH(0.0);
        PUSH(0);
        return;
    }
    FPUSH(ox + (cx / cw * 0.5f + 0.5f) * pw);
    FPUSH((1.0f - (cy / cw * 0.5f + 0.5f)) * hh);
    PUSH(-1);
}
PRIM(p_placed_count) { PUSH(gEngine.room.placed.n); }   /* ( -- n ) the room's placed objects */
PRIM(p_placed_info) {   /* ( i -- addr len shown ) ( F: -- x y z ) its name, shown, where */
    Cell i = POP();
    const Placed *p = i >= 0 && i < gEngine.room.placed.n ? &gEngine.room.placed.p[i] : NULL;

    PUSH(p != NULL ? (Cell)p->name : (Cell)"");
    PUSH(p != NULL ? (Cell)strnlen(p->name, sizeof(p->name)) : 0);
    PUSH(p != NULL && p->shown ? -1 : 0);
    FPUSH(p != NULL ? p->pos.x : 0.0);
    FPUSH(p != NULL ? p->pos.y : 0.0);
    FPUSH(p != NULL ? p->pos.z : 0.0);
}
PRIM(p_area_count) { PUSH(area_count(&gEngine.room)); }   /* ( -- n ) */
PRIM(p_area_kind) { PUSH(area_kind(&gEngine.room, (int)POP())); }   /* ( area -- kind ) 1 a box, -1 none */
PRIM(p_area_corner) {   /* ( area k -- flag ) ( F: -- x y z ) */
    Cell k = POP(), a = POP();
    Vec3 c = vec3(0, 0, 0);
    int ok = area_corner(&gEngine.room, (int)a, (int)k, &c);

    FPUSH(c.x);
    FPUSH(c.y);
    FPUSH(c.z);
    PUSH(ok ? -1 : 0);
}
PRIM(p_game_tick) {   /* ( -- ) one whole frame of the game (tests: the actors, then the models and
                       * doors; the models posed as drawing would, so their bones are where a
                       * drawn frame leaves them) */
    int i;

    engine_tick(&gEngine);
    for (i = 0; i < MAX_ACTORS; i++) {
        if (gEngine.actors[i].used && gEngine.actors[i].visible) {
            actor_prepare(&gEngine.actors[i]);
        }
    }
}
PRIM(p_door_move) {   /* ( exit open? slam? -- ) the played room's door swung open / shut (5 degrees a frame), or slammed */
    Cell slam = POP(), open = POP(), exit = POP();

    room_door_move(&gEngine.room, (int)exit, open != 0, slam != 0);
}
PRIM(p_door_events) {   /* ( exit -- sound how settled ) what its swing did this tick (room.h RoomDoor) */
    Cell exit = POP();
    const RoomDoor *d;

    if (exit < 0 || exit >= ROOM_DOORS || !gEngine.room.doors[exit].present) {
        PUSH(0);
        PUSH(0);
        PUSH(-1);
        return;
    }
    d = &gEngine.room.doors[exit];
    PUSH(d->sound);
    PUSH(d->sound_how);
    PUSH(d->settled);
}
PRIM(p_door_turn) {   /* ( exit -- ) ( F: -- a ) the played room's door's turn at rest (Doors_GetRestTurn;
                      * 0 no door) */
    Cell e = POP();

    FPUSH(e >= 0 && e < ROOM_DOORS && gEngine.room.doors[e].present ? gEngine.room.doors[e].rot.y : 0.0);
}
PRIM(p_door_angle) {   /* ( exit -- ) ( F: -- degrees ) the played room's door's opening now (Doors_GetOpenAngle:
                       * -90 open; 0 no door) */
    Cell e = POP();

    FPUSH(e >= 0 && e < ROOM_DOORS && gEngine.room.doors[e].present ? gEngine.room.doors[e].swing : 0.0);
}
PRIM(p_door_present) { Cell e = POP(); PUSH(e >= 0 && e < ROOM_DOORS && gEngine.room.doors[e].present ? -1 : 0); }   /* ( exit -- flag ) */
PRIM(p_door_passage) {   /* ( exit open? locks -- ) the door's passage flags on the nav mesh */
    Cell locks = POP(), open = POP(), exit = POP();

    room_door_passage(&gEngine.room, (int)exit, open != 0, (uint32_t)locks);
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
PRIM(p_message_file) {   /* ( addr len -- flag ) a scene's own messages from a file: ids $2000.. */
    Cell n = POP(), a = POP();
    char path[128];

    snprintf(path, sizeof(path), "%.*s", (int)(n > 0 ? n : 0), n > 0 ? (const char *)a : "");
    PUSH(message_file(n > 0 ? path : NULL) ? -1 : 0);   /* (none: the table let go) */
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
static const MessageOption *message_option(Cell i) {
    return sMsg != NULL && i >= 0 && i < sMsg->noptions ? &sMsg->options[i] : NULL;
}
PRIM(p_message_option_at) {   /* ( i -- page line col ) where option i is written */
    const MessageOption *o = message_option(POP());

    PUSH(o ? o->page : -1);
    PUSH(o ? o->line : -1);
    PUSH(o ? o->col : 0);
}
PRIM(p_message_option_leads) {   /* ( i -- msg ) the message option i leads to (0xFFFF: none) */
    const MessageOption *o = message_option(POP());

    PUSH(o ? o->leads_to : 0xFFFF);
}
PRIM(p_message_choice_flags) { PUSH(sMsg != NULL ? sMsg->choice_flags : 0); }
PRIM(p_message_param_text) {   /* ( slot addr len -- ) parameter `slot` shows this text */
    Cell n = POP(), a = POP(), slot = POP();

    message_set_param_text((int)slot, (const char *)a, (int)n);
}
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
/* ---- the house's doors and routes (world.c, with the game's door states) ---- */
static int sRoute[64], sRouteN;
PRIM(p_route) {   /* ( from to kind max -- n ) a route between rooms (0 there, -1 none): its doors by route-door */
    Cell max = POP(), kind = POP(), to = POP(), from = POP();

    sRouteN = world_route(&gEngine.world, (int)from, (int)to, (int)kind, (int)max, sRoute, 64);
    PUSH(sRouteN);
}
PRIM(p_route_avoiding) {   /* ( from to kind max avoid -- n ) the same, not through the doors whose bits
                            * are set in avoid (cells: door d is bit d mod 32 of cell d / 32) */
    Cell *a = (Cell *)POP(), max = POP(), kind = POP(), to = POP(), from = POP();
    uint32_t bits[(WORLD_DOORS + 31) / 32];
    int i;

    for (i = 0; i < (WORLD_DOORS + 31) / 32; i++) {
        bits[i] = (uint32_t)a[i];
    }
    sRouteN = world_route_avoiding(&gEngine.world, (int)from, (int)to, (int)kind, (int)max, bits, sRoute, 64);
    PUSH(sRouteN);
}
PRIM(p_route_sides) {   /* ( from from-way to want kind max avoid -- n ) the same between halves of rooms:
                         * from half from-way of its room (0 / 1; else either) to half want of `to`
                         * (-1 either) - world_route_sides */
    Cell *a = (Cell *)POP(), max = POP(), kind = POP(), want = POP(), to = POP(), way = POP(), from = POP();
    uint32_t bits[(WORLD_DOORS + 31) / 32];
    int i;

    for (i = 0; i < (WORLD_DOORS + 31) / 32; i++) {
        bits[i] = (uint32_t)a[i];
    }
    sRouteN = world_route_sides(&gEngine.world, (int)from, (int)way, (int)to, (int)want, (int)kind, (int)max, bits,
                                sRoute, 64);
    PUSH(sRouteN);
}
PRIM(p_exit_way) {   /* ( room exit checked -- way ) the half of the room the exit opens onto: 1 / 0, -1 none
                      * (Rooms_ExitSideFlag) */
    Cell checked = POP(), exit = POP(), room = POP();

    PUSH(world_exit_way(&gEngine.world, (int)room, (int)exit, checked != 0));
}
PRIM(p_route_door) {   /* ( i -- door ) the route's i-th door, first first */
    Cell i = POP();

    if (i < 0 || i >= sRouteN || i >= 64) {
        forth_error(f, "route-door: the route has no door %ld", (long)i);
    }
    PUSH(sRoute[i]);
}
PRIM(p_exits_rebuild) { world_rebuild_exits(&gEngine.world); }   /* ( -- ) Rooms_Rebuild */
PRIM(p_door_open) { Cell d = POP(); PUSH(world_door_open(&gEngine.world, (int)d) ? -1 : 0); }   /* ( door -- flag ) */
PRIM(p_door_exit_in) {   /* ( door room -- exit | -1 ) */
    Cell room = POP(), d = POP();

    PUSH(world_door_exit(&gEngine.world, (int)d, (int)room));
}
PRIM(p_door_leads) {   /* ( door room -- room' | -1 ) */
    Cell room = POP(), d = POP();

    PUSH(world_door_leads(&gEngine.world, (int)d, (int)room));
}
PRIM(p_exit_stand) {   /* ( exit -- flag ) ( F: -- x y z ) where the played room's door at that exit stands */
    Cell exit = POP();
    const RoomDoor *d;

    if (exit < 0 || exit >= ROOM_DOORS || !gEngine.room.doors[exit].present) {
        PUSH(0);
        return;
    }
    d = &gEngine.room.doors[exit];
    FPUSH(d->stand.x); FPUSH(d->stand.y); FPUSH(d->stand.z);
    PUSH(-1);
}
PRIM(p_door_flags) {   /* ( door -- flags ) its fixed flags (the door table; bit 0 a doorway) */
    Cell d = POP();

    PUSH(d >= 0 && d < gEngine.world.ndoors ? (Cell)(gEngine.world.doors[d].flags & 0xFF) : 0);
}
/* ---- sound effects (platform/snddrv.c): the game's banks by number - 4 the sound set (D_n000),
 * 5 the common sounds (C_0000), 6 the room's (ST_xxx/ST1_xxx), loaded as they are wanted; 7 (the
 * pursuer's) and the rest as the scripts name them ---- */

static void path_arg(Forth *f, char *out, size_t n);
static int sWantSet;
static void sound_ready(void) {   /* the executable's tables, once */
    static int done;
    const uint8_t *c, *r, *l, *pos;

    if (done) {
        return;
    }
    c = exetable("sound-curves", NULL);
    r = exetable("sound-pan-right", NULL);
    l = exetable("sound-pan-left", NULL);
    pos = exetable("sound-positioned", NULL);
    if (c != NULL && r != NULL && l != NULL && pos != NULL) {
        snddrv_tables(c, r, l, pos);
        done = 1;
    }
}
/* HGA_SOUNDLOG=1: each sound said as it is played - the frame, who, the bank's file, the id */
static char sBankFile[8][128];
static void bank_named(int k, const char *name) {
    if (k >= 0 && k < 8) {
        snprintf(sBankFile[k], sizeof(sBankFile[k]), "%s", name);
    }
}
static void sound_log(uint32_t id, int bank) {
    static int on = -1;

    if (on < 0) {
        on = getenv("HGA_SOUNDLOG") != NULL;
    }
    if (on) {
        fprintf(stderr, "sound %6ld %-12s bank %d (%s) id $%X\n", (long)gEngine.ticks, hactor_name(hactor_self()), bank,
                bank >= 0 && bank < 8 ? sBankFile[bank] : "?", (unsigned)id);
    }
}
static void bank_want(int bank) {
    char name[64];

    sound_ready();
    switch (bank) {
    case 4:
        snprintf(name, sizeof(name), "D_%01X000", sWantSet & 0xF);
        snddrv_bank(4, name);
        bank_named(4, name);
        break;
    case 5:
        snddrv_bank(5, "C_0000");
        bank_named(5, "C_0000");
        break;
    case 6:
        if (gEngine.room.id >= 0) {
            snprintf(name, sizeof(name), "ST_%03X/ST1_%03X", gEngine.room.id & ~7, gEngine.room.id);
            snddrv_bank(6, name);
            bank_named(6, name);
        }
        break;
    }
}
/* a point of the room in the camera's view: x right, z ahead (Sound_SetPosition) */
static void view_point(float x, float y, float z, float at[3], float ahead[3]) {
    Mat4 view = camera_view(&gEngine.camera);
    Vec3 v = mat4_point(&view, vec3(x, y, z));

    at[0] = v.x;
    at[1] = v.y;
    at[2] = -v.z;
    ahead[0] = 0.0f;
    ahead[1] = 0.0f;
    ahead[2] = 1.0f;
}
PRIM(p_bank_sound) {   /* ( id bank -- ) a sound heard plainly (SndDriver_Play) */
    Cell bank = POP(), id = POP();

    bank_want((int)bank);
    sound_log((uint32_t)id, (int)bank);
    snddrv_play((uint32_t)id, (int)bank);
}
PRIM(p_sound_at) {   /* ( id bank vol pitch -- ) ( F: x y z -- ) a sound at a point of the room
                      * (Actor_PlaySound: louder or softer by vol / 128, pitch semitones up) */
    Cell pitch = POP(), vol = POP(), bank = POP(), id = POP();
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP(), at[3], ahead[3];

    bank_want((int)bank);
    view_point(x, y, z, at, ahead);
    sound_log((uint32_t)id, (int)bank);
    snddrv_play_placed((uint32_t)id, (int)bank, (int)vol, (int)pitch, at, ahead);
}
/* a sound at a point of the room, as it is (for the engine's own: the effects') */
void engine_sound_at(uint32_t id, int bank, Vec3 at) {
    float a[3], ahead[3];

    bank_want(bank);
    view_point(at.x, at.y, at.z, a, ahead);
    sound_log(id, bank);
    snddrv_play_placed(id, bank, 0, 0, a, ahead);
}
PRIM(p_bank_sound_at) {   /* ( id bank -- ) ( F: x y z -- ) a sound at a point, as it is */
    Cell bank = POP(), id = POP();
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP(), at[3], ahead[3];

    bank_want((int)bank);
    view_point(x, y, z, at, ahead);
    sound_log((uint32_t)id, (int)bank);
    snddrv_play_placed((uint32_t)id, (int)bank, 0, 0, at, ahead);
}
PRIM(p_sound_reverb) {   /* ( core v -- ) core 0 (music) / 1 (effects)'s reverb level, 0..0x3FFF */
    Cell v = POP(), k = POP();

    snddrv_reverb_volume((int)k, (int)v);
}
PRIM(p_sound_load) { bank_want((int)POP()); }   /* ( k -- ) bank 4 / 5 / 6 loaded now */
PRIM(p_sound_loaded) { PUSH(snddrv_bank_loaded((int)POP()) ? -1 : 0); }   /* ( k -- flag ) */
PRIM(p_sound_scale) { snddrv_progress_scale((float)FPOP()); }   /* ( F: v -- ) progress +0x1118 */
PRIM(p_sound_stop) {   /* ( id bank -- ) sound id's voices released (SndDriver_StopSound) */
    Cell bank = POP(), id = POP();

    snddrv_stop((uint32_t)id, (int)bank);
}
PRIM(p_sound_stop_voices) {   /* ( core0 core1 -- ) the voices in the masks off (StopMasks) */
    Cell b = POP(), a = POP();

    snddrv_stop_masks((uint32_t)a, (uint32_t)b);
}
PRIM(p_sound_stop_all) { snddrv_stop_all(); }   /* ( -- ) */
PRIM(p_sound_fade) { snddrv_placed_volume((int)POP()); }   /* ( 0..255 -- ) the positioned sounds' volume */
PRIM(p_stage_music_volume) { seq_master_volume((float)FPOP()); }   /* ( F: v -- ) the stage music's (the sequences') volume, 0..1 */
PRIM(p_sound_volume) {   /* ( F: sound master -- ) the effects' volume, 0..1 each */
    float m = (float)FPOP(), v = (float)FPOP();

    snddrv_volume(v, m);
}
PRIM(p_sound_bank) {   /* ( k addr len -- flag ) bank k holds NAME ("" none) */
    char name[128];
    Cell k;

    path_arg(f, name, sizeof(name));
    k = POP();
    sound_ready();
    bank_named((int)k, name);
    PUSH(snddrv_bank((int)k, name) ? -1 : 0);
}
PRIM(p_sound_bank_file) {   /* ( k -- addr len ) the file bank k was last given ("" none) */
    Cell k = POP();
    const char *s = k >= 0 && k < 8 ? sBankFile[k] : "";

    PUSH((Cell)s);
    PUSH((Cell)strlen(s));
}
PRIM(p_sound_set) {   /* ( set -- ) the sound set bank 4 holds (Progress_LoadSoundSet) */
    sWantSet = (int)POP();
    bank_want(4);
}
PRIM(p_common_sound) {   /* ( id -- ) play sound `id` of the common bank */
    Cell id = POP();

    bank_want(5);
    sound_log((uint32_t)id, 5);
    snddrv_play((uint32_t)id, 5);
}
PRIM(p_dot_voices) {   /* ( -- ) the effects' voices sounding: voice, bank, sound's entry, priority,
                        * volumes */
    int v, bank, entry, pri, l, r;

    for (v = 24; v < 48; v++) {
        if (snddrv_voice(v, &bank, &entry, &pri, &l, &r)) {
            forth_printf(f, "voice %d: bank %d (%s) entry %d pri %d  L %X R %X\n", v, bank, snddrv_bank_name(bank), entry, pri, l, r);
        }
    }
}

/* ---- a stage: lights of the scripts' own (the title) ---- */

PRIM(p_stage_light) {   /* ( i -- ) ( F: x y z r g b range -- ) light i (0..2) of the stage; colours 0..255 */
    Cell i = POP();
    float range = (float)FPOP(), b = (float)FPOP(), g = (float)FPOP(), r = (float)FPOP();
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP();

    if (i >= 0 && i < 3) {
        gEngine.stage[i] = (RoomLight){vec3(x, y, z), vec3(r, g, b), 1.0f, range, 0.0f};
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
static void movie_draw_at(float x, float y, float dw, float dh) {
    int pw, ph, fresh;
    const uint8_t *px = movie_picture(&pw, &ph, &fresh);

    if (px != NULL && (fresh || sMovieTex == 0 || pw != sMovieW || ph != sMovieH)) {
        sMovieTex = render_texture_stream(sMovieTex, px, pw, ph, &sMovieW, &sMovieH);
    }
    if (sMovieTex != 0 && px != NULL) {
        render_image(sMovieTex, x, y, dw, dh, 0xFFFFFFFFu);
    }
}
/* the last picture of a movie kept to show on its own (the game over's: GameOver_StateStart takes
 * it from the movie, GameOver_DrawMovieFrame draws it over everything) - the movies after it get
 * a texture of their own */
static GpuTexture sHeldTex;
PRIM(p_movie_hold) {   /* ( -- flag ) the picture shown now kept (none: false) */
    if (sHeldTex != 0) {
        render_texture_free(sHeldTex);
    }
    sHeldTex = sMovieTex;
    sMovieTex = 0;
    PUSH(sHeldTex != 0 ? -1 : 0);
}
PRIM(p_movie_unhold) {   /* ( -- ) the kept picture let go */
    if (sHeldTex != 0) {
        render_texture_free(sHeldTex);
        sHeldTex = 0;
    }
}
PRIM(p_movie_draw) {   /* ( x y w h -- ) the movie's picture there (window pixels) */
    Cell dh = POP(), dw = POP(), y = POP(), x = POP();

    movie_draw_at((float)x, (float)y, (float)dw, (float)dh);
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
PRIM(p_cutscene_leave_alone) { cutscene_leave_alone((int)POP()); }   /* ( actor -- ) the cast leaves it be (Cutscene_Mark82) */
PRIM(p_cutscene_keep_model) { cutscene_keep_model((int)POP()); }   /* ( actor -- ) its model left as it is (Cutscene_Mark83) */
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
PRIM(p_still) { gEngine.still = POP() != 0; }    /* ( flag -- ) the models and the room stand still (the actors run) */
PRIM(p_cs_active) { PUSH(cutscene_active() ? -1 : 0); }
PRIM(p_cs_total) { PUSH(cutscene_signal_total((int)POP())); }   /* ( bit -- n ) its count so far less one */
PRIM(p_cs_next) { cutscene_next_event(); }   /* ( -- ) the scene to its next event's frame */
PRIM(p_black_frame) { gEngine.black_frame = 1; }   /* ( -- ) the next picture black (under the 2D) */
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
PRIM(p_motions_load) {   /* ( addr len model -- flag ) a motion file (a room's .MTN, "\\" or "/") loaded for it */
    Actor *a = actor_arg(f, POP());
    Cell n = POP(), at = POP();
    char path[128];
    int i;

    snprintf(path, sizeof(path), "%.*s", (int)n, (const char *)at);
    for (i = 0; path[i] != 0; i++) {
        if (path[i] == '\\') {
            path[i] = '/';
        }
    }
    PUSH(actor_load_motions(a, path) ? -1 : 0);
}
PRIM(p_motions_use) {   /* ( on model -- ) its model plays the loaded motions too (after its own), or not */
    Actor *a = actor_arg(f, POP());

    actor_use_motions(a, POP() != 0);
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
PRIM(p_actor_load_textured) {   /* ( addr len tex-addr tex-len -- id ) its model, and another's textures */
    Cell tn = POP(), ta = POP(), n = POP(), a = POP();
    char name[64], tex[64];
    int id;

    snprintf(name, sizeof(name), "%.*s", (int)n, (const char *)a);
    snprintf(tex, sizeof(tex), "%.*s", (int)tn, (const char *)ta);
    id = actor_load_textured(gEngine.actors, name, tex);
    if (id < 0) {
        forth_error(f, "actor-load-textured: can't load %s", name);
    }
    PUSH(id);
}
PRIM(p_actor_free) { actor_free(actor_arg(f, POP())); }
PRIM(p_actor_texture_swap) {   /* ( model sel addr len -- flag ) its texture sel shown as that file's first */
    char name[96];
    Cell n = POP(), a = POP(), sel = POP();
    Actor *ac = actor_arg(f, POP());

    snprintf(name, sizeof(name), "%.*s", (int)n, (const char *)a);
    PUSH(actor_texture_swap(ac, (int)sel, name) ? -1 : 0);
}
PRIM(p_actor_texture_restore) { actor_texture_restore(actor_arg(f, POP())); }   /* ( model -- ) */
PRIM(p_cloth_force) {   /* ( model set -- ) ( F: x y z -- ) a hanging set's force (SpringSet +0x0:
                         * Fiona's hair is set 2 - FionaModel_SetVector, the model's +0xC8) */
    Vec3 v = fpop3(f);
    Cell set = POP();
    Actor *a = actor_arg(f, POP());

    if (a->cloth != NULL && set >= 0 && set < a->cloth->nsets) {
        a->cloth->sets[set].force = v;
    }
}
PRIM(p_cloth_back) {   /* ( model pose -- ) Daniella's back as the story sets it (its 0xCC op 4): 0 standing,
                       * 1 flat, 2 bent (DaniellaModel_BackCapsules) */
    Cell pose = POP();
    Actor *a = actor_arg(f, POP());

    cloth_back(a->cloth, (int)pose);
}
PRIM(p_actor_markers) {   /* ( id addr len -- ) its motions' event keys from that marker file (NAME, ".MRK" added):
                           * a model whose markers are another's (the slip's: O_FIN/FIN_000) */
    char name[128], path[160];
    Actor *a;

    path_arg(f, name, sizeof(name));
    a = actor_arg(f, POP());
    snprintf(path, sizeof(path), "%s.MRK", name);
    if (files_exist(path)) {
        free(a->events);
        a->events = files_read(path, &a->events_size);
    }
}
PRIM(p_actor) { PUSH(actor_arg(f, POP())); }   /* ( id -- addr ) */
PRIM(p_motion_store) {   /* ( id motion-id -- ) play a motion from its start */
    Cell mid = POP();
    Actor *a = actor_arg(f, POP());
    int index = model_motion_find(&a->model, (int)mid);

    if (index < 0) {
        forth_error(f, "motion!: %s has no motion %lX", a->name, (long)mid);
    }
    if (index != a->motion) {
        actor_motion_start(a, index, a->loop ? 1 : 0, 0.0f);
    }
}
PRIM(p_motion_play) {   /* ( id motion-id blend flags -- ) from its start, cross-faded over blend ticks, with
                         * the flags (1 loops, 2 in step, 8 no time of its own); a motion it lacks:
                         * nothing (the original's Motion_Start too) */
    Cell flags = POP(), blend = POP(), mid = POP();
    Actor *a = actor_arg(f, POP());
    int index = model_motion_find(&a->model, (int)mid);

    if (index >= 0) {
        actor_motion_start(a, index, (int)flags, (float)blend);
    }
}
PRIM(p_motion_events) {   /* ( id layer dt loop -- bits ) Motion_EventFlags: the motion's (layer 0) or its
                          * variant's (1) event keys at its time + dt (loop 1: a looping one wraps) */
    Cell loop = POP(), dt = POP(), layer = POP();
    Actor *a = actor_arg(f, POP());

    PUSH(actor_event_flags(a, (int)layer, (int)dt, (int)loop));
}
PRIM(p_motion_variant) {   /* ( id motion-id -- ) the body motion just started blends with this one
                           * (Motion_PlayTable's variant; act.vweight: 1 the motion alone .. 0 it) */
    Cell mid = POP();
    Actor *a = actor_arg(f, POP());

    actor_body_variant(a, mid < 0 ? -1 : model_motion_find(&a->model, (int)mid));
}
PRIM(p_motion_entry) {   /* ( id motion-id -- blend pose flags ) its model's table entry (0s: none) */
    Cell mid = POP();
    Actor *a = actor_arg(f, POP());
    int blend, pose, flags;

    actor_motion_entry(a, model_motion_find(&a->model, (int)mid), &blend, &pose, &flags);
    PUSH(blend);
    PUSH(pose);
    PUSH(flags);
}
PRIM(p_motion_index) {   /* ( id motion-id -- i | -1 ) its place in the model's motion list (Motion_AnimIndex) */
    Cell mid = POP();
    Actor *a = actor_arg(f, POP());

    PUSH(a != NULL ? model_motion_pos(&a->model, model_motion_find(&a->model, (int)mid)) : -1);
}
PRIM(p_motion_table) {   /* ( id addr len -- ) its motion table: the executable's table of that name
                          * (exetables.h: "motions-fiona" ...), an entry (6 bytes) a motion */
    char name[64];
    Actor *a;
    size_t size = 0;
    int n;

    path_arg(f, name, sizeof(name));
    a = actor_arg(f, POP());
    n = model_motion_count(&a->model);
    a->table = exetable(name, &size);
    a->ntable = a->table != NULL ? (n < (int)(size / 6) ? n : (int)(size / 6)) : 0;   /* (its motions past the table's: none) */
}
PRIM(p_exe_table) {   /* ( addr len -- addr n | 0 0 ) the executable's table of that name (read-only) */
    char name[64];
    size_t size = 0;
    const uint8_t *t;

    path_arg(f, name, sizeof(name));
    t = exetable(name, &size);
    PUSH(t);
    PUSH(t != NULL ? (Cell)size : 0);
}
PRIM(p_root_delta) {   /* ( id -- ) ( F: -- turn dx dy dz ) its motion's root movement this frame
                         * (model space, scaled to the room) */
    Actor *a = actor_arg(f, POP());
    float turn;
    Vec3 step;

    actor_root_delta(a, &turn, &step);
    FPUSH(turn);
    FPUSH(step.x * a->scale);
    FPUSH(step.y * a->scale);
    FPUSH(step.z * a->scale);
}
PRIM(p_bone_pos) {   /* ( id bone -- ) ( F: -- x y z ) where the bone is in the room (as last drawn) */
    Cell b = POP();
    Vec3 p = actor_bone(actor_arg(f, POP()), (int)b);

    FPUSH(p.x); FPUSH(p.y); FPUSH(p.z);
}
PRIM(p_bone_point) {   /* ( id bone -- ) ( F: ox oy oz -- x y z ) a point in the bone's own axes, in the room */
    Vec3 off = fpop3(f);
    Cell b = POP();
    Vec3 p = actor_bone_point(actor_arg(f, POP()), (int)b, off);

    FPUSH(p.x); FPUSH(p.y); FPUSH(p.z);
}
PRIM(p_motion_overlay) {   /* ( id motion-id -- ) Motion_PlayTableNoCheck: as its part's own (see
                            * actor_motion_when_free) */
    Cell mid = POP();
    Actor *a = actor_arg(f, POP());

    actor_motion_when_free(a, model_motion_find(&a->model, (int)mid));
}
PRIM(p_turns_clear) { actor_arg(f, POP())->nturns = 0; }   /* ( id -- ) no bones turned */
PRIM(p_turn_add) {   /* ( id bone -- ) ( F: pitch yaw -- ) a bone turned as it is posed (pitch about the
                      * model's x - positive: forward and down - then yaw about its up), its
                      * children with it; up to 8 */
    Cell bone = POP();
    Actor *a = actor_arg(f, POP());
    float yaw = (float)FPOP(), pitch = (float)FPOP();

    if (a->nturns < 8 && bone >= 0) {
        a->turns[a->nturns].bone = (int)bone;
        a->turns[a->nturns].pitch = pitch;
        a->turns[a->nturns].yaw = yaw;
        a->nturns++;
    }
}
PRIM(p_foot_down) {   /* ( id foot -- flag ) foot 0..3 (front right, front left, hind right, hind left)
                       * on the ground now (DogModel_FootDown: the contact tracks -5 / -6; while a
                       * motion fades in, down in both, or in the new one if only it says) */
    Cell foot = POP();
    Actor *a = actor_arg(f, POP());
    float c0[3], c1[3];
    int code = foot < 2 ? -5 : -6, k = (int)(foot & 1);
    int has0 = model_track_raw(&a->model, a->motion, code, a->frame, c0);
    int has1 = a->fade > 0.0f && a->prev_motion >= 0 &&
               model_track_raw(&a->model, a->prev_motion, code, a->prev_frame, c1);

    if (has0 && has1) {
        PUSH(c0[k] > 0.0f && c1[k] > 0.0f ? -1 : 0);
    } else {
        PUSH(has0 && c0[k] > 0.0f ? -1 : 0);
    }
}
PRIM(p_motion_track) {   /* ( id code prev? -- flag ) ( F: -- a b c ) a special track of its motion (prev?:
                         * the one fading out) at its time, raw (model units): -2 / -3 the right /
                         * left foot's place, -4 the feet's contact (0 right, 1 left; > 0 down), ...;
                         * false: it has none */
    Cell prev = POP(), code = POP();
    Actor *a = actor_arg(f, POP());
    float v[3] = {0.0f, 0.0f, 0.0f};
    int has = prev ? a->fade > 0.0f && a->prev_motion >= 0 &&
                         model_track_raw(&a->model, a->prev_motion, (int)code, a->prev_frame, v)
                   : model_track_raw(&a->model, a->motion, (int)code, a->frame, v);

    FPUSH(v[0]);
    FPUSH(v[1]);
    FPUSH(v[2]);
    PUSH(has ? -1 : 0);
}
/* a motion's time dt ticks on (back when < 0), wrapped into its length (motion_time: none of its
 * own - flag 0x10 - stays) */
static float frame_by(const Actor *a, int motion, float frame, float dt) {
    int len = model_motion_frames(&a->model, motion);
    float t = (a->mflags & 0x10) ? frame : frame + dt * a->rate;

    if (len <= 0) {
        return 0.0f;
    }
    while (t < 0.0f) {
        t += (float)len;
    }
    while (t >= (float)len) {
        t -= (float)len;
    }
    return t;
}
PRIM(p_motion_track_by) {   /* ( id code prev? -- flag ) ( F: dt -- a b c ) as motion-track, dt ticks
                            * on from its time (Motion_FootPos's dt: the stalkers' -1) */
    Cell prev = POP(), code = POP();
    Actor *a = actor_arg(f, POP());
    float dt = (float)FPOP(), v[3] = {0.0f, 0.0f, 0.0f};
    int has = prev ? a->fade > 0.0f && a->prev_motion >= 0 &&
                         model_track_raw(&a->model, a->prev_motion, (int)code, frame_by(a, a->prev_motion, a->prev_frame, dt), v)
                   : a->motion >= 0 && model_track_raw(&a->model, a->motion, (int)code, frame_by(a, a->motion, a->frame, dt), v);

    FPUSH(v[0]);
    FPUSH(v[1]);
    FPUSH(v[2]);
    PUSH(has ? -1 : 0);
}
PRIM(p_foot_at) {   /* ( id left? -- down? ) ( F: dt -- x y z ) a foot dt ticks on (Motion_FootPos):
                     * where it is in the room - its place track (-3 left, -2 right) cross-faded from
                     * the motion fading out, turned, scaled and placed as the actor - and whether
                     * the motion's contact track (-4) has it down; false, at the actor, without one */
    Cell left = POP() != 0;
    Actor *a = actor_arg(f, POP());
    float dt = (float)FPOP(), c[3], p[3] = {0.0f, 0.0f, 0.0f}, q[3];
    int code = left ? -3 : -2, down = 0, i;

    if (a->motion >= 0 && model_track_raw(&a->model, a->motion, -4, frame_by(a, a->motion, a->frame, dt), c)) {
        down = c[left] > 0.0f;
        model_track(&a->model, a->motion, code, frame_by(a, a->motion, a->frame, dt), p);
        if (a->fade > 0.0f && a->fade_len > 0.0f && a->prev_motion >= 0 &&
            model_track(&a->model, a->prev_motion, code, frame_by(a, a->prev_motion, a->prev_frame, dt), q)) {
            float x = a->fade / a->fade_len, w = x * x * (3.0f - 2.0f * x);   /* (fade_weight) */

            for (i = 0; i < 3; i++) {
                p[i] = q[i] * w + p[i] * (1.0f - w);
            }
        }
    }
    {
        float cs = cosf(a->yaw), sn = sinf(a->yaw), k = a->scale;

        FPUSH(a->pos.x + k * (p[0] * cs + p[2] * sn));
        FPUSH(a->pos.y + k * p[1]);
        FPUSH(a->pos.z + k * (p[2] * cs - p[0] * sn));
    }
    PUSH(down ? -1 : 0);
}
PRIM(p_dog_legs) {   /* ( id on -- ) its feet planted and its legs fitted (a dog's skeleton: Hewie) */
    Cell on = POP();
    Actor *a = actor_arg(f, POP());

    if ((on != 0) != (a->legs.on != 0)) {
        doglegs_reset(&a->legs);
    }
    a->legs.on = on != 0;
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
PRIM(p_rumble) { Cell frames = POP(), strength = POP(); rumble_jolt((int)POP(), (int)strength, (int)frames); }   /* ( channel strength frames -- ) the large motor */
PRIM(p_rumble_buzz) { Cell frames = POP(), on = POP(); rumble_buzz((int)POP(), on != 0, (int)frames); }      /* ( channel on frames -- ) the small motor */
PRIM(p_rumble_swell) {   /* ( channel from to frames -- ) the large motor going from one strength to another */
    Cell frames = POP(), to = POP(), from = POP();

    rumble_swell((int)POP(), (int)from, (int)to, (int)frames);
}
PRIM(p_rumble_allow) { rumble_allow(POP() != 0); }   /* ( flag -- ) the vibration allowed (the options) */
PRIM(p_game_volume) { sound_gain((float)FPOP()); }   /* ( F: v -- ) everything heard, 0..1 (the options' volume) */
PRIM(p_rumble_seq_clear) { rumble_sequence_clear(); }   /* ( -- ) the sequence emptied */
PRIM(p_rumble_seq_add) {   /* ( small? from to frames -- ) a step added to the sequence */
    Cell frames = POP(), to = POP(), from = POP();

    rumble_sequence_add(POP() != 0, (int)from, (int)to, (int)frames);
}
PRIM(p_rumble_now) {   /* ( -- small? large ) (tests) what goes to the pad now */
    int s, l;

    rumble_now(&s, &l);
    PUSH(s ? -1 : 0);
    PUSH(l);
}
PRIM(p_pad_stick) { FPUSH(gEngine.input.stick_x); FPUSH(gEngine.input.stick_y); }   /* ( F: -- x y ) a gamepad's left stick (0 0 none) */
PRIM(p_key_down) {
    Cell k = scancode(f);

    PUSH(keys()[k] || gEngine.held[k] ? -1 : 0);
}
PRIM(p_key_hold) {   /* ( scancode flag -- ) hold a key down (or let it go) as if pressed */
    Cell on = POP(), k = scancode(f);

    gEngine.held[k] = on != 0;
}
/* the controls (platform.h): the actions, each a key the scripts read, and the keys bound to them */
PRIM(p_controls) { PUSH(platform_actions()); }   /* ( -- n ) */
PRIM(p_control_key) { Cell i = POP(); PUSH(platform_action_key((int)i)); }   /* ( i -- scancode ) the scripts' key */
PRIM(p_control_binding) {   /* ( i which -- scancode ) its first (0) or second (1) key; 0 none */
    Cell which = POP(), i = POP();

    PUSH(platform_binding((int)i, (int)which));
}
PRIM(p_control_bind) {   /* ( scancode i which -- ) (0 and which 1: its second cleared) */
    Cell which = POP(), i = POP(), k = POP();

    platform_bind((int)i, (int)which, (SDL_Scancode)k);
}
PRIM(p_controls_reset) { platform_bindings_reset(); }   /* ( -- ) the defaults */
PRIM(p_control_bind_next) { platform_bind_next((int)POP()); }   /* ( i -- ) the next key pressed bound to i (Escape: not) */
PRIM(p_control_waiting) { PUSH(platform_binding_wanted()); }   /* ( -- i ) the action waiting for a key; -1 none */
PRIM(p_key_push) {   /* ( scancode -- ) (tests) the key pressed, as from the keyboard - through the
                      * controls; the only one pressed now */
    SDL_Event e;

    memset(gEngine.input.pressed, 0, sizeof(gEngine.input.pressed));
    memset(&e, 0, sizeof(e));
    e.type = SDL_EVENT_KEY_DOWN;
    e.key.scancode = (SDL_Scancode)POP();
    e.key.down = true;
    SDL_PushEvent(&e);
    platform_poll(&gEngine.input);
}
PRIM(p_key_name) {   /* ( scancode -- addr len ) as the keyboard calls it ("" none) */
    Cell k = POP();
    const char *name = k > 0 && k < SDL_SCANCODE_COUNT ? SDL_GetScancodeName((SDL_Scancode)k) : "";

    PUSH((Cell)name);
    PUSH((Cell)strlen(name));
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
PRIM(p_h_to_s) {   /* ( n -- addr len ) in hex, 0x.. */
    char *b = sNumText[sNumNext++ % 4];

    snprintf(b, sizeof(sNumText[0]), "0x%lX", (unsigned long)POP());
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
static RoomLook sKeptLook;
PRIM(p_look_keep) { sKeptLook = gRoomLook; }   /* ( -- ) the room's look as it is, kept */
PRIM(p_look_kept) { gRoomLook = sKeptLook; }   /* ( -- ) and put back */
PRIM(p_look_plain) {   /* ( -- ) none of it: no fog, tint, bloom, depth of field (the model gallery's) */
    gRoomLook.has_fog = gRoomLook.has_tint = gRoomLook.has_bloom = gRoomLook.has_dof = 0;
}
PRIM(p_look_get) {   /* ( slot addr -- n ) a look effect's parameters as last set, into addr (16 at most; 0 none) */
    Cell a = POP(), slot = POP();

    PUSH((Cell)room_look_get((int)slot, (uint8_t *)a));
}
PRIM(p_look_set) {   /* ( slot addr n -- ) a look effect's parameters (addr 0: removed) */
    Cell n = POP(), a = POP(), slot = POP();

    room_look_set((int)slot, a != 0 ? (const uint8_t *)a : NULL, (size_t)(n > 0 ? n : 0));
}

/* ---- files: the player's own folder, and writing Forth's output into a file ---- */

PRIM(p_doorway_faded) {   /* ( model -- ) ( F: -- f ) how far it has faded going out through a doorway (0..1) */
    Cell id = POP();

    FPUSH(id >= 0 && id < MAX_ACTORS && gEngine.actors[id].used ? doorway_faded(&gEngine, &gEngine.actors[id]) : 0.0f);
}
PRIM(p_actor_part_show) {   /* ( model part on -- ) one of its skinned parts shown or hidden */
    Cell on = POP(), part = POP(), id = POP();
    Actor *a = id >= 0 && id < MAX_ACTORS ? &gEngine.actors[id] : NULL;
    unsigned g = (unsigned)part + 1;

    if (a == NULL || part < 0 || g >= 256) {
        return;
    }
    if (on) {
        a->parts_shown[g >> 5] |= 1u << (g & 31);
    } else {
        a->parts_shown[g >> 5] &= ~(1u << (g & 31));
    }
}
PRIM(p_hga_version) {   /* ( -- addr len ) hga's version ("0.1.5") */
    static const char v[] = HGA_VERSION;

    PUSH((Cell)v);
    PUSH((Cell)(sizeof(v) - 1));
}
PRIM(p_testing) { PUSH(gEngine.testing ? -1 : 0); }   /* ( -- flag ) run by --test (nothing kept on disk) */
PRIM(p_user_dir) {   /* ( -- addr len ) where settings are kept (made if needed; ends with /) */
    const char *dir = disc_user_dir();

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

/* ---- the free camera (hga's own, for looking about and filming): F3 takes the camera from
 * wherever it is and gives it back; while it is free the arrows move it along the floor (up /
 * down ahead and back, left / right to the side), Page Up / Page Down raise and lower it, the
 * right mouse button held turns it with the mouse, Shift faster. Nothing else in the game is
 * told: the shots, the cutscenes and Fiona go on as they were. ---- */
static struct {
    int on, was_down;
    Camera cam;
} sFree;
static void freecam_tick(Engine *e) {
    const bool *k = keys();
    int down = k[SDL_SCANCODE_F3];

    if (down && !sFree.was_down) {
        sFree.on = !sFree.on;
        if (sFree.on) {
            sFree.cam = e->camera;
        }
    }
    sFree.was_down = down;
    if (!sFree.on) {
        return;
    }
    {
        float speed = (k[SDL_SCANCODE_LSHIFT] || k[SDL_SCANCODE_RSHIFT]) ? 4.0f : 1.0f;
        float fwd = (float)(k[SDL_SCANCODE_UP] - k[SDL_SCANCODE_DOWN]) * speed;
        float side = (float)(k[SDL_SCANCODE_RIGHT] - k[SDL_SCANCODE_LEFT]) * speed;
        float rise = (float)(k[SDL_SCANCODE_PAGEUP] - k[SDL_SCANCODE_PAGEDOWN]) * speed;
        Camera *c = &sFree.cam;

        if (e->input.mouse_buttons & SDL_BUTTON_MASK(3)) {   /* (right button held: look) */
            c->yaw += 0.005f * e->input.mouse_dx;
            c->pitch -= 0.005f * e->input.mouse_dy * c->up;
            c->pitch = fmaxf(-1.5f, fminf(1.5f, c->pitch));
        }
        c->pos.x += fwd * sinf(c->yaw) + side * cosf(c->yaw);
        c->pos.z += -fwd * cosf(c->yaw) + side * sinf(c->yaw);
        c->pos.y += rise * c->up;
        e->camera.pos = c->pos;
        e->camera.yaw = c->yaw;
        e->camera.pitch = c->pitch;
    }
}
PRIM(p_freecam) { PUSH(sFree.on ? -1 : 0); }   /* ( -- flag ) the free camera has the camera */
PRIM(p_freecam_set) {   /* ( on -- ) the free camera on (from the camera as it is now: set it first) or off */
    sFree.on = POP() != 0;
    sFree.cam = gEngine.camera;
}

void engine_tick(Engine *e) {
    int i;

    e->ticks++;
    movie_update();
    {   /* (the pad's rumble stops while the game is paused: a menu up) */
        static int was_paused;

        if (e->paused != was_paused) {
            rumble_enable(!e->paused);
            was_paused = e->paused;
        }
    }
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
    if (e->paused) {   /* (a menu up: only its actors have a frame) */
        hactor_held_frame(e->forth);
        if (e->alone > 0 && e->alone - 1 < MAX_ACTORS) {   /* (the menu's own model: it moves) */
            actor_tick(&e->actors[e->alone - 1]);
        }
        freecam_tick(e);
        render_look_tick();
        return;
    }
    hactor_frame(e->forth);   /* the actors' frame, then their models move on */
    if (e->still) {   /* (the game over: the world stands still behind it, its actors running) */
        freecam_tick(e);
        render_look_tick();
        return;
    }
    for (i = 0; i < MAX_ACTORS; i++) {
        actor_tick(&e->actors[i]);
    }
    particles_tick(e->camera.pos);   /* (the effects' sprites move) */
    rumble_tick();
    cutscene_camera(&e->camera);   /* a cutscene's shot with the camera in it has it */
    camdir_shake(&gCamDir, &e->camera);   /* (shaken, whoever placed it) */
    room_tick(&e->room);
    for (i = 0; i < ROOM_DOORS; i++) {   /* (Door_PlaySound: at the door, common sounds) */
        const RoomDoor *d = &e->room.doors[i];

        if (d->present && d->sound != 0) {
            float at[3], ahead[3];

            bank_want(5);
            view_point(d->pos.x, d->pos.y, d->pos.z, at, ahead);
            snddrv_play_placed((uint32_t)d->sound, 5, 0, 0, at, ahead);
        }
    }
    freecam_tick(e);   /* (last: over the shots and the cutscenes) */
    render_look_tick();
}

/* ---- the retained 2D layer: what actors put on the screen (the message window, prompts),
 * kept until they change it and drawn each frame, layer by layer over the picture ---- */
#define UI_LAYERS 20   /* (18: the mods' - scripts/mods.fs; 19 the galleries' fade) */
#define UI_ITEMS 256
typedef struct UiItem {
    int text;                 /* 0 a rectangle, 1 text, 2 the movie's picture, 3 a texture's part */
    float x, y, w, h;         /* (text: w the scale) */
    uint32_t rgba;
    char s[96];
    int group, id, csa;       /* (3) the texture cache's group + id, its palette */
    float u, v, uw, vh;       /* (3) the part of it, in texels */
} UiItem;
static UiItem sUi[UI_LAYERS][UI_ITEMS];
static int sUiN[UI_LAYERS];
static int ui_layer(Forth *f, Cell layer) {
    if (layer < 0 || layer >= UI_LAYERS) {
        forth_error(f, "ui: no layer %ld", (long)layer);
    }
    return (int)layer;
}
static UiItem *ui_add(Forth *f, Cell layer) {
    int l = ui_layer(f, layer);

    return sUiN[l] < UI_ITEMS ? &sUi[l][sUiN[l]++] : NULL;
}
/* who each cast model belongs to (the actor that cast it), told where a cutscene leaves it */
static int sCastOwner[CHARACTERS];
static Cell float_cell(float x) {
    double d = x;
    Cell c;

    memcpy(&c, &d, sizeof(c));
    return c;
}
static void cast_placed(int actor, Vec3 pos, float yaw) {
    int i, kind = hactor_kind("place");

    for (i = 0; i < CHARACTERS && kind >= 0; i++) {
        if (gEvents.chars[i].present && gEvents.chars[i].actor == actor && sCastOwner[i] >= 0) {
            Cell args[4] = {float_cell(pos.x), float_cell(pos.y), float_cell(pos.z), float_cell(yaw)};

            hactor_send(-1, sCastOwner[i], kind, args, 4);
        }
    }
}
static void cast_followed(int actor, Vec3 pos) {   /* (its body: where, its room, the triangle under it) */
    int i;
    float h;

    for (i = 0; i < CHARACTERS; i++) {
        HBody *b;

        if (!gEvents.chars[i].present || gEvents.chars[i].actor != actor || sCastOwner[i] < 0 ||
            (b = hactor_body(sCastOwner[i])) == NULL || !b->has) {
            continue;
        }
        b->pos[0] = pos.x;
        b->pos[1] = pos.y;
        b->pos[2] = pos.z;
        b->room = gEngine.room.id;
        b->tri = navmesh_find(&gEngine.room.nav, pos, 4.0f, &h);
    }
}
PRIM(p_chars_hide) {   /* ( -- ) every character's model hidden (the game over: each one's +0x29) */
    int i;

    for (i = 0; i < CHARACTERS; i++) {
        if (gEvents.chars[i].present && gEvents.chars[i].actor >= 0 && gEvents.chars[i].actor < MAX_ACTORS) {
            gEngine.actors[gEvents.chars[i].actor].visible = 0;
        }
    }
}
PRIM(p_cast_as) {   /* ( id model -- ) the character with script id `id` (0 Fiona, 1 Hewie..) has
                     * that model (-1: none): the cutscenes' cast finds it (gEvents.chars) */
    Cell model = POP(), id = POP();
    int i, free_i = -1;

    for (i = 0; i < CHARACTERS; i++) {
        ScriptChar *c = &gEvents.chars[i];

        if (c->present && c->id == id) {
            break;
        }
        if (!c->present && free_i < 0) {
            free_i = i;
        }
    }
    if (i == CHARACTERS) {
        i = free_i;
    }
    if (i < 0) {
        return;
    }
    gEvents.chars[i].present = model >= 0;
    gEvents.chars[i].id = (int32_t)id;
    gEvents.chars[i].actor = (int32_t)model;
    sCastOwner[i] = hactor_self();
    cutscene_on_place(cast_placed);
    cutscene_on_follow(cast_followed);
}
PRIM(p_cast_model) {   /* ( id -- model | -1 ) the model cast as script id `id` (cast-as) */
    Cell id = POP();
    int i;

    for (i = 0; i < CHARACTERS; i++) {
        if (gEvents.chars[i].present && gEvents.chars[i].id == id) {
            PUSH(gEvents.chars[i].actor);
            return;
        }
    }
    PUSH(-1);
}
PRIM(p_model_file) {   /* ( model -- addr len ) the files it was loaded from (no extension) */
    Actor *a = actor_arg(f, POP());

    PUSH((Cell)a->name);
    PUSH((Cell)strnlen(a->name, sizeof(a->name)));
}
PRIM(p_ui_clear) { sUiN[ui_layer(f, POP())] = 0; }   /* ( layer -- ) */
PRIM(p_ui_rect) {   /* ( layer x y w h rgba -- ) */
    Cell rgba = POP(), height = POP(), width = POP(), y = POP(), x = POP();
    UiItem *u = ui_add(f, POP());

    if (u != NULL) {
        *u = (UiItem){.text = 0, .x = (float)x, .y = (float)y, .w = (float)width, .h = (float)height, .rgba = (uint32_t)rgba};
    }
}
PRIM(p_ui_movie) {   /* ( layer x y w h -- ) the playing movie's picture */
    Cell height = POP(), width = POP(), y = POP(), x = POP();
    UiItem *u = ui_add(f, POP());

    if (u != NULL) {
        *u = (UiItem){.text = 2, .x = (float)x, .y = (float)y, .w = (float)width, .h = (float)height};
    }
}
PRIM(p_ui_held) {   /* ( layer x y w h -- ) the kept movie picture (movie-hold) */
    Cell height = POP(), width = POP(), y = POP(), x = POP();
    UiItem *u = ui_add(f, POP());

    if (u != NULL) {
        *u = (UiItem){.text = 3, .x = (float)x, .y = (float)y, .w = (float)width, .h = (float)height};
    }
}
PRIM(p_ui_text) {   /* ( layer addr len x y rgba scale -- ) */
    Cell scale = POP(), rgba = POP(), y = POP(), x = POP(), n = POP(), a = POP();
    UiItem *u = ui_add(f, POP());

    if (u != NULL) {
        *u = (UiItem){.text = 1, .x = (float)x, .y = (float)y, .w = (float)(scale < 1 ? 1 : scale), .rgba = (uint32_t)rgba};
        snprintf(u->s, sizeof(u->s), "%.*s", (int)(n > 0 ? n : 0), n > 0 ? (const char *)a : "");
    }
}
PRIM(p_ui_image) {   /* ( layer x y w h group id csa u v uw vh rgba -- ) part of a cache texture */
    Cell rgba = POP(), vh = POP(), uw = POP(), v = POP(), u = POP(), csa = POP(), id = POP(), group = POP();
    Cell height = POP(), width = POP(), y = POP(), x = POP();
    UiItem *it = ui_add(f, POP());

    if (it != NULL) {
        *it = (UiItem){3, (float)x, (float)y, (float)width, (float)height, (uint32_t)rgba, "", (int)group, (int)id,
                       (int)csa, (float)u, (float)v, (float)uw, (float)vh};
    }
}
PRIM(p_tex_info) {   /* ( group id -- w h tex ) a cache texture's size and GPU name (0: none) */
    Cell id = POP(), group = POP();
    int tw, th;
    unsigned t = sprites_texture_of((int)group, (int)id, -1, &tw, &th);

    PUSH(tw);
    PUSH(th);
    PUSH(t);
}
/* ---- the extras room's galleries (scripts/subscreen/galleries.fs) ---- */
static Camera sKeptCamera;
PRIM(p_art_picture) {   /* ( picture group -- h flag ) an art gallery picture (SUBSCR/ARTnn.LEN) as the
                         * group's one texture, 512 wide and h lines */
    Cell group = POP(), k = POP();
    char path[64];
    size_t n = 0, size = 0;
    uint8_t *file, *bank = NULL;
    TexBank *b;
    int h = 0;

    snprintf(path, sizeof(path), "SUBSCR/ART%02d.LEN", (int)(k / 8));
    if (k >= 0 && (file = files_read(path, &n)) != NULL) {
        bank = artpic_bank(file, n, (int)k, &size, &h);
        free(file);
    }
    b = bank != NULL ? texbank_take(bank, size) : NULL;
    if (b != NULL) {
        sprites_group_bank((int)group, b);
    }
    PUSH(h);
    PUSH(b != NULL ? -1 : 0);
}
PRIM(p_thumbnail) {   /* ( movie group -- flag ) the movie's picture (SUBSCR/THUMBNAIL.TXS: a bank each,
                       * after their offsets) as the group's texture */
    static uint8_t *thumbs;
    static size_t size;
    Cell group = POP(), k = POP();
    uint32_t at, end;
    uint8_t *copy;
    TexBank *b = NULL;

    if (thumbs == NULL) {   /* (read once: kept) */
        thumbs = files_read("SUBSCR/THUMBNAIL.TXS", &size);
    }
    if (thumbs != NULL && k >= 0 && (size_t)(k + 2) * 4 <= size) {
        memcpy(&at, thumbs + (k + 1) * 4, 4);
        memcpy(&end, thumbs + (k + 2) * 4, 4);
        if (end <= at || end > size) {
            end = (uint32_t)size;
        }
        if (at < end && (copy = malloc(end - at)) != NULL) {
            memcpy(copy, thumbs + at, end - at);
            b = texbank_take(copy, end - at);
        }
    }
    if (b != NULL) {
        sprites_group_bank((int)group, b);
    }
    PUSH(b != NULL ? -1 : 0);
}
PRIM(p_cam_keep) { sKeptCamera = gEngine.camera; }    /* ( -- ) the camera as it is, kept */
PRIM(p_cam_kept) { gEngine.camera = sKeptCamera; }    /* ( -- ) and put back */
PRIM(p_alone) {   /* ( id -- ) that model alone drawn, on the background (-1: the scene again) */
    Cell id = POP();

    gEngine.alone = id >= 0 && id < MAX_ACTORS ? (int)id + 1 : 0;
}
PRIM(p_tex_bank) {   /* ( addr len group -- flag ) a .TEX file into the texture cache's group (the menus) */
    Cell group = POP(), n = POP(), a = POP();
    char path[128];

    snprintf(path, sizeof(path), "%.*s", (int)(n > 0 ? n : 0), n > 0 ? (const char *)a : "");
    PUSH(sprites_group_file((int)group, path) ? -1 : 0);
}
/* ---- the game's own font (SUBSCR/MSG_BASE.TEX, the cache's group $14: 16 x 16 cells, 16 a row
 * from ' ') - each glyph its own width and drop (D_0044AD00: high nibble the blank columns at its
 * right, low nibble how far it sits down, of 16); not there: the built-in font ---- */
static const uint8_t kGlyphShape[96] = {
    0x70, 0x80, 0x80, 0x80, 0x80, 0x60, 0x70, 0xC0, 0xB0, 0xA0, 0x10, 0x40, 0x80, 0x60, 0x80, 0x80,
    0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x80, 0x80, 0x90, 0x90, 0x70, 0x60,
    0x80, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0xB0, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70,
    0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x00, 0x90, 0x20, 0x80, 0x70,
    0x00, 0x70, 0x80, 0x80, 0x80, 0x80, 0x80, 0x81, 0x80, 0xC0, 0xA0, 0x90, 0xC0, 0x70, 0x70, 0x70,
    0x81, 0x81, 0x90, 0x80, 0x80, 0x80, 0x80, 0x70, 0x70, 0x71, 0x70, 0x20, 0x00, 0x00, 0x00, 0x00,
};
#define FONT_GROUP 0x14
#define FONT_TEXTURE 0

static unsigned game_font(int *tw, int *th) {
    static int loaded;

    if (!loaded) {   /* (as SceneBoot loads it: once) */
        loaded = 1;
        sprites_group_file(FONT_GROUP, "SUBSCR/MSG_BASE.TEX");
    }
    return gRender.game_font ? sprites_texture_of(FONT_GROUP, FONT_TEXTURE, 0, tw, th) : 0;
}

/* how wide a line is drawn at `scale`, in whichever font draws it */
static float text_width(const char *s, int n, float scale) {
    int tw, th, i;
    float w = 0.0f;

    if (game_font(&tw, &th) == 0 || tw <= 0 || th <= 0) {
        return render_text_width(scale, n);
    }
    for (i = 0; i < n; i++) {
        int g = (unsigned char)s[i] - 0x20;

        if (g >= 0 && g < 96) {
            w += (float)(16 - (kGlyphShape[g] >> 4)) * scale / 2.0f + scale * 0.5f;
        } else if (g >= 96) {
            w += 8.0f * scale + scale * 0.5f;
        }
    }
    return w;
}
PRIM(p_text_width) {   /* ( addr len scale -- px ) */
    Cell scale = POP(), n = POP(), a = POP();

    PUSH((Cell)text_width((const char *)a, n > 0 ? (int)n : 0, (float)(scale < 1 ? 1 : scale)));
}

static int game_text(float x, float y, float scale, uint32_t rgba, const char *s) {
    int tw, th, i;
    unsigned t = game_font(&tw, &th);
    float cell = 8.0f * scale, tall = cell * 21.0f / 16.0f;   /* (a glyph 16 x 21, as the message window's: Task glyphW / glyphH) */

    if (t == 0 || tw <= 0 || th <= 0) {
        return 0;
    }
    for (i = 0; s[i] != 0; i++) {
        int c = (unsigned char)s[i], g = c - 0x20;
        float u, v, drop;

        if (g >= 96) {   /* past the letters (the pad's buttons, 0xC0 on): a whole cell */
            u = (float)((g & 0xF) * 16);
            v = (float)((g >> 4) * 16);
            render_image_uv(t, x, y, cell, tall, u / tw, v / th, (u + 16.0f) / tw, (v + 16.0f) / th, rgba);
            x += cell + scale * 0.5f;
            continue;
        }
        if (g < 0 || g >= 96) {
            continue;
        }
        u = (float)((g & 0xF) * 16);
        v = (float)((g >> 4) * 16);
        drop = (float)(kGlyphShape[g] & 0xF) * cell / 16.0f;
        if (c != ' ') {
            render_image_uv(t, x, y + drop, cell, tall, u / tw, v / th, (u + 16.0f) / tw, (v + 16.0f) / th, rgba);
        }
        x += (float)(16 - (kGlyphShape[g] >> 4)) * cell / 16.0f + scale * 0.5f;
    }
    return 1;
}

static void ui_draw(void) {
    int l, i;

    for (l = 0; l < UI_LAYERS; l++) {
        for (i = 0; i < sUiN[l]; i++) {
            const UiItem *u = &sUi[l][i];

            if (u->text == 3) {
                int tw, th;
                unsigned t = sprites_texture_of(u->group, u->id, u->csa, &tw, &th);

                if (t != 0 && tw > 0 && th > 0) {
                    render_image_uv(t, u->x, u->y, u->w, u->h, u->u / tw, u->v / th, (u->u + u->uw) / tw,
                                    (u->v + u->vh) / th, u->rgba);
                }
            } else if (u->text == 2) {
                movie_draw_at(u->x, u->y, u->w, u->h);
            } else if (u->text == 3) {
                if (sHeldTex != 0) {
                    render_image(sHeldTex, u->x, u->y, u->w, u->h, 0xFFFFFFFFu);
                }
            } else if (u->text) {
                if (!gRender.game_font || !game_text(u->x, u->y, u->w, u->rgba, u->s)) {
                    render_text(u->x, u->y, u->w, u->rgba, u->s, (int)strlen(u->s));
                }
            } else {
                render_rect(u->x, u->y, u->w, u->h, u->rgba);
            }
        }
    }
}

void engine_draw_2d(Engine *e) {
    int i;

    if (cutscene_letterbox()) {   /* (Cutscene_Letterbox: 56 lines of 448 at the top and bottom) */
        float bar = (float)e->height * 56.0f / 448.0f;

        render_rect(0.0f, 0.0f, (float)e->width, bar, 0x000000FFu);
        render_rect(0.0f, (float)e->height - bar, (float)e->width, bar, 0x000000FFu);
    }
    if (e->black_frame) {   /* (the renderer's rectangle over the screen, layer 0x33: alpha 0x80 is opaque) */
        render_rect(0.0f, 0.0f, (float)e->width, (float)e->height, 0x000000FFu);
        e->black_frame = 0;
    }

    ui_draw();
    for (i = 0; i < e->ndraw_hooks; i++) {
        if (forth_call(e->forth, e->draw_hooks[i]) != 0) {
            forth_printf(e->forth, "on-draw: %s removed after an error\n", e->draw_hooks[i]->name);
            memmove(&e->draw_hooks[i], &e->draw_hooks[i + 1], (size_t)(e->ndraw_hooks - i - 1) * sizeof(Word *));
            e->ndraw_hooks--;
            i--;
        }
    }
}

/* back as at the start (the game over's return to the title: a fresh game): the 2D layers
 * cleared, the movie closed, the music, sequences and sounds stopped, no cutscene */
void bind_engine_reset(void) {
    memset(sUiN, 0, sizeof(sUiN));
    movie_close();
    music_stop();
    seq_reset();
    snddrv_stop_all();
    cutscene_end();
}
static int sResetWanted;
PRIM(p_soft_reset) { sResetWanted = 1; }   /* ( -- ) a fresh game at this frame's end (the title) */
int bind_engine_reset_wanted(void) {
    int r = sResetWanted;

    sResetWanted = 0;
    return r;
}

void bind_engine(Forth *f) {
    Vocab *saved = f->m.current, *engine = forth_vocab(f, "engine");
    static const struct {
        const char *name;
        Code code;
    } prims[] = {
        {"room", p_room}, {"route", p_route}, {"route-avoiding", p_route_avoiding}, {"route-sides", p_route_sides}, {"exit-way", p_exit_way}, {"route-door", p_route_door}, {"door-open?", p_door_open}, {"world-exits-rebuild", p_exits_rebuild}, {"door-exit-in", p_door_exit_in}, {"door-leads", p_door_leads}, {"exit-stand", p_exit_stand}, {"room-id", p_room_id}, {"room-exists?", p_room_exists},
        {"room-bounds", p_room_bounds}, {"room-cameras", p_room_cameras}, {"room-camera", p_room_camera}, {"light-own", p_light_own}, {"light-scale", p_light_scale}, {"door-group?", p_door_group_q}, {"light@", p_light_fetch}, {"look-on?", p_look_on}, {"sprites", p_sprites_new}, {"sprites-free", p_sprites_free}, {"sprites-free-all", p_sprites_free_all}, {"sprites-texture", p_sprites_texture}, {"sprites-cells", p_sprites_cells}, {"sprites-flags", p_sprites_flags}, {"sprites-offset", p_sprites_offset}, {"sprites-corner", p_sprites_corner}, {"sprites-count", p_sprites_count}, {"sprite!", p_sprite_store}, {"sprites-uv", p_sprites_uv}, {"sprite-quad!", p_sprite_quad}, {"door-group!", p_door_group}, {"room-group!", p_room_group}, {"floor-below", p_floor_below},
        {"nav-tris", p_nav_tris}, {"nav-tri", p_nav_tri}, {"tri-center", p_tri_center}, {"tri-normal", p_tri_normal},
        {"exit-tri", p_exit_tri}, {"exit-leads", p_exit_leads}, {"room-exit-door", p_room_exit_door}, {"room-exit-leads", p_room_exit_leads}, {"door-sides", p_door_sides}, {"room-exit-tri", p_room_exit_tri}, {"hud", p_hud}, {"ui-clear", p_ui_clear}, {"ui-image", p_ui_image}, {"tex-bank", p_tex_bank}, {"tex-info", p_tex_info}, {"ui-rect", p_ui_rect}, {"ui-text", p_ui_text}, {"text-width", p_text_width}, {"ui-movie", p_ui_movie}, {"ui-held", p_ui_held}, {"movie-hold", p_movie_hold}, {"movie-unhold", p_movie_unhold}, {"cast-as", p_cast_as}, {"chars-hide", p_chars_hide}, {"cast-model", p_cast_model}, {"model-file", p_model_file}, {"nav-move", p_nav_move}, {"nav-nearest", p_nav_nearest}, {"nav-at", p_nav_at}, {".room", p_room_info},
        {"camera", p_camera},
        {"cam-new-room", p_cam_new_room}, {"cam-room-start", p_cam_room_start}, {"cam-setup", p_cam_setup},
        {"body-move", p_body_move}, {"body-move-any", p_body_move_any}, {"body-move-local", p_body_move_local}, {"body-climb", p_body_climb}, {"body-turn-toward", p_body_turn_toward}, {"body-heading-to", p_body_heading_to}, {"body-place-at", p_body_place_at}, {"body-tri-to", p_body_tri_to}, {"body-free", p_body_free}, {"bodies-touching?", p_bodies_touching}, {"cam-on", p_cam_on}, {"cam-followed", p_cam_followed}, {"cam-target", p_cam_target_id}, {"cam-player!", p_cam_player}, {"cam-char-setup", p_cam_char_setup}, {"cam-forget-setups", p_cam_forget_setups}, {"cam-setup-of", p_cam_setup_of}, {"cam-take-followed", p_cam_take_followed}, {"cam-scene!", p_cam_scene}, {"cam-scene?", p_cam_scene_q}, {"cam-next-set", p_cam_next_set}, {"cam-rate!", p_cam_rate}, {"cam-shake!", p_cam_shake}, {"cam-cut?", p_cam_cut}, {"cam-remember", p_cam_remember}, {"cam-started?", p_cam_started}, {"cam-stop", p_cam_stop}, {"cam-angle", p_cam_angle}, {"cam-ease", p_cam_ease}, {"cam-event?", p_cam_event}, {"cam-event!", p_cam_event_set}, {"cam-track", p_cam_track},
        {"cam-update", p_cam_update}, {"cam-restart", p_cam_restart}, {"cam-changed?", p_cam_changed}, {"cam-set@", p_cam_set}, {"actor-load-textured", p_actor_load_textured}, {"motions-load", p_motions_load}, {"motions-use", p_motions_use}, {"cam-place", p_cam_place}, {"cam-retake", p_cam_retake}, {"frame-blend", p_frame_blend}, {"flicker-new", p_flicker_new}, {"flame-new", p_flame_new}, {"swarm-new", p_swarm_new}, {"splash-new", p_splash_new}, {"burst-new", p_burst_new}, {"spray", p_spray}, {"thing-new", p_thing_new}, {"thing-free", p_thing_free}, {"things-clear", p_things_clear}, {"thing-used?", p_thing_used}, {"thing-kind", p_thing_kind}, {"thing-room", p_thing_room}, {"thing-tri", p_thing_tri}, {"thing-age", p_thing_age}, {"thing-state", p_thing_state}, {"thing-at", p_thing_at}, {"thing-speed", p_thing_speed}, {"thing-age!", p_thing_age_store}, {"thing-place", p_thing_place}, {"thing-kick", p_thing_kick}, {"thing-fade", p_thing_fade}, {"thing-events", p_thing_events}, {"thing-turn", p_thing_turn}, {"things-step", p_things_step}, {"thing-reaches?", p_thing_reaches}, {"puffs", p_puffs}, {"debris-cloud", p_debris_cloud}, {"gun-smoke", p_gun_smoke}, {"spark", p_spark}, {"particles-engines", p_particles_engines}, {"sink-ring", p_sink_ring}, {"water-step", p_water_step}, {"welling", p_welling}, {"shards-new", p_shards_new}, {"butterflies-new", p_butterflies_new}, {"butterflies-to", p_butterflies_to}, {"wander-new", p_wander_new}, {"spirit-new", p_spirit_new}, {"spirit-at", p_spirit_at}, {"spirit-vanish", p_spirit_vanish}, {"vanish-new", p_vanish_new}, {"stones-new", p_stones_new}, {"reflect!", p_reflect_set}, {"reflect-off", p_reflect_off}, {"reflect-only", p_reflect_only}, {"debris-new", p_debris_new}, {"rising-smoke-new", p_rising_smoke_new}, {"dust-shaft-new", p_dust_shaft_new}, {"spiral-smoke-new", p_spiral_smoke_new}, {"drips-new", p_drips_new}, {"one-drip-new", p_one_drip_new}, {"rising-motes-new", p_rising_motes_new}, {"embers-new", p_embers_new}, {"flecks-new", p_flecks_new}, {"smoke-puffs-new", p_smoke_puffs_new}, {"wall-shadow-new", p_wall_shadow_new}, {"fire-new", p_fire_new}, {"steam-new", p_steam_new}, {"floor-glow-new", p_floor_glow_new}, {"tv-new", p_tv_new}, {"big-fire-new", p_big_fire_new}, {"light-shaft-new", p_light_shaft_new}, {"glint-new", p_glint_new}, {"marker-new", p_marker_new}, {"window-flash-new", p_window_flash_new}, {"smoke-trail-new", p_smoke_trail_new}, {"mag-burst-new", p_mag_burst_new}, {"mirror-square", p_mirror_square}, {"haze-new", p_haze_new}, {"caustic-new", p_caustic_new}, {"glow-new", p_glow_new}, {"wisp-column-new", p_wisp_column_new}, {"spark-spray-new", p_spark_spray_new}, {"heat-haze-new", p_heat_haze_new}, {"leaves-new", p_leaves_new}, {"loop-sprite-new", p_loop_sprite_new}, {"object-glow-new", p_object_glow_new}, {"rain-new", p_rain_new}, {"ash-new", p_ash_new}, {"pit-embers-new", p_pit_embers_new}, {"particles-place", p_particles_place}, {"particles-rest!", p_particles_rest}, {"swim-path", p_swim_path}, {"swim-step", p_swim_step}, {"body-advance", p_body_advance}, {"particles-command", p_particles_command}, {"particles-alive?", p_particles_alive}, {"particles-children", p_particles_children}, {"particles-free", p_particles_free}, {"particles-info", p_particles_info},
        {".director", p_cam_info}, {"area-in?", p_area_in}, {"nav-path", p_nav_path}, {"v-nav-move", p_v_nav_move}, {"v-walk", p_v_walk}, {"v-wall", p_v_wall}, {"nav-links", p_nav_links}, {"nav-next", p_nav_next}, {"nav-link-tri", p_nav_link_tri}, {"nav-link-yaw", p_nav_link_yaw}, {"nav-link-spot", p_nav_link_spot}, {"nav-link-at?", p_nav_link_at}, {"nav-link-side", p_nav_link_side}, {"nav-link-front", p_nav_link_front}, {"v-free", p_v_free},
        {"v-path", p_v_path}, {"v-path-curved", p_v_path_curved}, {"v-tri", p_v_tri}, {"nav-floor", p_nav_floor}, {"v-tri-in", p_v_tri_in}, {"vec!", p_vec_store}, {"vec@", p_vec_fetch}, {"vec-copy", p_vec_copy},
        {"vec-dist", p_vec_dist}, {"vec-dist-xz", p_vec_dist_xz}, {"vec-heading", p_vec_heading}, {"vec-ahead", p_vec_ahead},
        {"angle-wrap", p_angle_wrap}, {"nav-walk", p_nav_walk}, {"nav-crosses?", p_nav_crosses}, {"nav-path-point", p_nav_path_point}, {"placed-op", p_placed_op}, {"placed-at", p_placed_at}, {"placed-f@", p_placed_f_fetch}, {"placed-f!", p_placed_f_store}, {"placed-l@", p_placed_l_fetch}, {"placed-l!", p_placed_l_store}, {"placed-here?", p_placed_here}, {"placed-shown?", p_placed_shown}, {"nav-square-step", p_nav_square_step}, {"obstacles-reset", p_obstacles_reset}, {"obstacle-placed?", p_obstacle_placed}, {"obstacle-make", p_obstacle_make}, {"obstacle-stop", p_obstacle_stop}, {"obstacle-square", p_obstacle_square}, {"obstacle-at", p_obstacle_at}, {"obstacle-middle", p_obstacle_middle}, {"obstacles-ring-mask", p_obstacles_ring_mask}, {"obstacle-on-square?", p_obstacle_on_square}, {"obstacle-can-push?", p_obstacle_can_push}, {"obstacle-push", p_obstacle_push}, {"obstacle-step", p_obstacle_step}, {"obstacle-moved", p_obstacle_moved}, {".placed", p_placed_list}, {"door-swing", p_door_swing}, {"room-door-at", p_door_at}, {"door-user-spot", p_door_user_spot},
        {"door-anim", p_door_anim}, {"door-side", p_door_side}, {"door-near?", p_door_near}, {"door-in-area?", p_door_in_area},
        {"door-animating?", p_door_animating}, {"door-on-side?", p_door_on_side}, {"door-side-flags", p_door_side_flags}, {"door-passage", p_door_passage}, {"door-move", p_door_move}, {"game-tick", p_game_tick}, {"door-events", p_door_events}, {"door-here?", p_door_present}, {"door-turn", p_door_turn}, {"door-angle", p_door_angle}, {"area-middle", p_area_middle}, {"to-screen", p_to_screen}, {"placed-count", p_placed_count}, {"placed-info", p_placed_info}, {"area-count", p_area_count}, {"area-kind", p_area_kind}, {"area-corner", p_area_corner}, {"area-cross", p_area_cross}, {"exit-area", p_exit_area}, {"movie-open", p_movie_open}, {"movie-status", p_movie_status},
        {"movie-frame", p_movie_frame}, {"cutscene-load", p_cs_start}, {"cutscene-run", p_cs_run}, {"cutscene-go", p_cs_go},
        {"cutscene-frame!", p_cs_frame_set}, {"cutscene-frame", p_cs_frame}, {"cutscene-update", p_cs_update}, {"cutscene-end", p_cs_end}, {"cutscene-leave-alone", p_cutscene_leave_alone}, {"cutscene-keep-model", p_cutscene_keep_model}, {"soft-reset", p_soft_reset},
        {"cutscene-status", p_cs_status}, {"cutscene-in-shot?", p_cs_in_shot}, {"cutscene-near?", p_cs_near_end}, {"cutscene-shot-at", p_cs_shot_at},
        {"cutscene-signals", p_cs_signals}, {"cutscene-active?", p_cs_active}, {"pause!", p_pause}, {"still!", p_still}, {"seq-bank", p_seq_bank}, {"seq-load", p_seq_load}, {"seq-play", p_seq_play},
        {"seq-playing?", p_seq_playing}, {"seq-volume", p_seq_volume}, {"seq-port-volume", p_seq_port_volume},
        {"seq-chan-volume", p_seq_chan_volume}, {"seq-midi", p_seq_midi}, {"seq-reset", p_seq_reset}, {"music-play", p_music_play}, {"music-stop", p_music_stop},
        {"music-pause", p_music_pause}, {"music-volume!", p_music_volume}, {"music-playing?", p_music_playing}, {"cutscene-signal-total", p_cs_total}, {"cutscene-letterbox-off", p_cs_letterbox_off}, {"black-frame", p_black_frame}, {"cutscene-next", p_cs_next}, {"movie-close", p_movie_close}, {"movie-pause", p_movie_pause}, {"movie-paused?", p_movie_paused}, {"movie-compose", p_movie_compose},
        {"movie-volume!", p_movie_volume}, {"movie-draw", p_movie_draw}, {"stage-light", p_stage_light}, {"stage-lights", p_stage_lights},
        {"stage-ambient", p_stage_ambient}, {"room-clear", p_room_clear}, {"common-sound", p_common_sound},
        {"bank-sound", p_bank_sound}, {"bank-sound-at", p_bank_sound_at}, {"sound-set!", p_sound_set}, {"sound-at", p_sound_at},
        {"stop-sound", p_sound_stop}, {"sound-load", p_sound_load}, {"sound-reverb!", p_sound_reverb}, {"sound-loaded?", p_sound_loaded}, {"sound-scale!", p_sound_scale}, {"sound-stop-voices", p_sound_stop_voices}, {"sound-stop-all", p_sound_stop_all}, {"sound-fade", p_sound_fade},
        {"sound-volume!", p_sound_volume}, {"stage-music-volume!", p_stage_music_volume}, {"sound-bank", p_sound_bank}, {"sound-bank-file", p_sound_bank_file}, {".voices", p_dot_voices}, {"exit-door", p_exit_door}, {"door-flags", p_door_flags},
        {"nav-block!", p_nav_block}, {"nav-group!", p_nav_group}, {"nav-tri-flags!", p_nav_tri_flags},
        {"nav-in-group?", p_nav_in_group}, {"nav-flags", p_nav_flags},
        {"message-layout", p_message_layout}, {"message-file", p_message_file}, {"message-lines", p_message_lines}, {"message-line", p_message_line},
        {"message-options", p_message_options}, {"message-option-at", p_message_option_at}, {"message-option-leads", p_message_option_leads},
        {"message-choice-flags", p_message_choice_flags}, {"message-param!", p_message_param}, {"message-param$", p_message_param_text}, {"exit-spot", p_exit_spot},
        {"actor-load", p_actor_load}, {"actor-free", p_actor_free}, {"cloth-force!", p_cloth_force}, {"cloth-back!", p_cloth_back}, {"actor-texture-swap", p_actor_texture_swap}, {"actor-texture-restore", p_actor_texture_restore}, {"actor-markers", p_actor_markers}, {"actor", p_actor},
        {"motion!", p_motion_store}, {"has-motion?", p_has_motion}, {"motion-play", p_motion_play}, {"motion-entry", p_motion_entry}, {"motion-index", p_motion_index}, {"motion-variant", p_motion_variant}, {"motion-events", p_motion_events},
        {"motion-table", p_motion_table}, {"exe-table", p_exe_table}, {"root-delta", p_root_delta}, {"bone-pos", p_bone_pos}, {"bone-point", p_bone_point}, {"foot-down?", p_foot_down}, {"dog-legs", p_dog_legs}, {"motion-track", p_motion_track}, {"motion-track-by", p_motion_track_by}, {"foot-at", p_foot_at}, {"motion-overlay", p_motion_overlay}, {"turns-clear", p_turns_clear}, {"turn+", p_turn_add}, {"motion@", p_motion_fetch}, {"motion-done?", p_motion_done},
        {"motion-frames", p_motion_frames}, {".motions", p_motions},
        {"rumble", p_rumble}, {"rumble-buzz", p_rumble_buzz}, {"rumble-swell", p_rumble_swell}, {"rumble-sequence-clear", p_rumble_seq_clear}, {"rumble-step", p_rumble_seq_add}, {"rumble-now", p_rumble_now}, {"rumble-allowed!", p_rumble_allow}, {"game-volume!", p_game_volume}, {"pad-stick", p_pad_stick}, {"key-down?", p_key_down}, {"key-hold", p_key_hold}, {"controls", p_controls}, {"control-key", p_control_key}, {"control-binding", p_control_binding}, {"control-bind", p_control_bind}, {"controls-reset", p_controls_reset}, {"control-bind-next", p_control_bind_next}, {"control-waiting", p_control_waiting}, {"key-name", p_key_name}, {"art-picture", p_art_picture}, {"thumbnail", p_thumbnail}, {"alone", p_alone}, {"cam-keep", p_cam_keep}, {"cam-kept", p_cam_kept}, {"key-push", p_key_push}, {"key-pressed?", p_key_pressed}, {"mouse-dx", p_mouse_dx},
        {"mouse-dy", p_mouse_dy}, {"freecam?", p_freecam}, {"freecam!", p_freecam_set}, {"mouse-down?", p_mouse_down},
        {"on-tick", p_on_tick}, {"off-tick", p_off_tick}, {"ticks", p_ticks}, {"dt", p_dt},
        {"on-draw", p_on_draw}, {"off-draw", p_off_draw}, {"pen-color", p_pen_color},
        {"pen-scale", p_pen_scale}, {"draw-text", p_draw_text}, {"draw-rect", p_draw_rect},
        {"screen-size", p_screen_size}, {"char-size", p_char_size}, {"n>s", p_n_to_s}, {"h>s", p_h_to_s}, {"f>s$", p_f_to_s},
        {"gfx", p_gfx}, {"room-look", p_look}, {"room-look-reset", p_look_reset}, {"look-set", p_look_set}, {"look-keep", p_look_keep}, {"look-kept", p_look_kept}, {"look-plain", p_look_plain}, {"look@", p_look_get}, {"user-dir", p_user_dir}, {"testing?", p_testing}, {"hga-version", p_hga_version}, {"actor-part-show", p_actor_part_show}, {"doorway-faded", p_doorway_faded}, {"file-exists?", p_file_exists}, {"to-file", p_to_file},
        {"end-file", p_end_file}, {"xt>name", p_xt_to_name},
        {"clear-color", p_clear_color}, {"out-of-view?", p_out_of_view}, {"screenshot", p_screenshot}, {"console!", p_console},

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
    field(f, "act.mflags", offsetof(Actor, mflags));    /* 32-bit: the motion's flags (0x20 wrapped this tick) */
    field(f, "act.fade", offsetof(Actor, fade));        /* sf@: ticks of cross-fade left (0: settled) */
    field(f, "act.alpha", offsetof(Actor, alpha));      /* sf@: drawn faded (1 as it is, 0 unseen) */
    field(f, "act.tint", offsetof(Actor, tint));        /* 32-bit: drawn tinted, $RRGGBBAA ($80 = 1.0) */
    field(f, "act.vweight", offsetof(Actor, vweight));  /* sf@: the motion against its variant (1: alone) */
    field(f, "act.variant", offsetof(Actor, variant));  /* 32-bit: the variant's index, -1 none */
    field(f, "act.visible", offsetof(Actor, visible));  /* 32-bit: l@ l! */
    field(f, "act.shadow", offsetof(Actor, shadow_size));
    field(f, "act.shadow-off", offsetof(Actor, shadow_off));   /* 32-bit: l@ l! */
    field(f, "act.layer", offsetof(Actor, layer));      /* 32-bit: its draw layer (0: $A) */
    field(f, "act.doorway-fade", offsetof(Actor, doorway_fade));   /* 32-bit: fades going through a doorway (on) */
    field(f, "act.parts-shown", offsetof(Actor, parts_shown));   /* 8 x 32-bit: its parts shown (bit: part + 1) */
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
    field(f, "gfx.game-font", offsetof(RenderSettings, game_font));
    field(f, "gfx.fullscreen", offsetof(RenderSettings, fullscreen));
    f->m.current = saved;
}
