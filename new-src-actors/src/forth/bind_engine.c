/* The engine's Forth words. Scripts drive the game with these; C keeps the data.
 *
 * Structs are reached by address plus field words: `camera cam.yaw sf@` reads the camera's yaw
 * (a field word adds its offset). Floats in structs are 32-bit: sf@ / sf!. */
#include "../game/engine.h"
#include "../hactor/hactor.h"
#include "../game/areas.h"
#include "../game/camdirector.h"
#include "../game/cutscene.h"
#include "../game/exits.h"
#include "../game/messages.h"
#include "../platform/snddrv.h"
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
PRIM(p_cam_target) {   /* ( F: x y z -- ) where the one followed is now */
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP();

    gCamDir.target_at = vec3(x, y, z);
}
PRIM(p_cam_follow) {   /* ( id -- ) follow someone (10 units up; cam-target! says where), -1 nobody */
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
PRIM(p_route_door) {   /* ( i -- door ) the route's i-th door, first first */
    Cell i = POP();

    if (i < 0 || i >= sRouteN || i >= 64) {
        forth_error(f, "route-door: the route has no door %ld", (long)i);
    }
    PUSH(sRoute[i]);
}
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
    c = world_exe(&gEngine.world, 0x41D820, 0x600);
    r = world_exe(&gEngine.world, 0x3DF580, 0x1004);
    l = world_exe(&gEngine.world, 0x3E0580, 0x1004);
    pos = world_exe(&gEngine.world, 0x3D8990, 16);
    if (c != NULL && r != NULL && l != NULL && pos != NULL) {
        snddrv_tables(c, r, l, pos);
        done = 1;
    }
}
static void bank_want(int bank) {
    char name[64];

    sound_ready();
    switch (bank) {
    case 4:
        snprintf(name, sizeof(name), "D_%01X000", sWantSet & 0xF);
        snddrv_bank(4, name);
        break;
    case 5:
        snddrv_bank(5, "C_0000");
        break;
    case 6:
        if (gEngine.room.id >= 0) {
            snprintf(name, sizeof(name), "ST_%03X/ST1_%03X", gEngine.room.id & ~7, gEngine.room.id);
            snddrv_bank(6, name);
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
    snddrv_play((uint32_t)id, (int)bank);
}
PRIM(p_sound_at) {   /* ( id bank vol pitch -- ) ( F: x y z -- ) a sound at a point of the room
                      * (Actor_PlaySound: louder or softer by vol / 128, pitch semitones up) */
    Cell pitch = POP(), vol = POP(), bank = POP(), id = POP();
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP(), at[3], ahead[3];

    bank_want((int)bank);
    view_point(x, y, z, at, ahead);
    snddrv_play_placed((uint32_t)id, (int)bank, (int)vol, (int)pitch, at, ahead);
}
PRIM(p_bank_sound_at) {   /* ( id bank -- ) ( F: x y z -- ) a sound at a point, as it is */
    Cell bank = POP(), id = POP();
    float z = (float)FPOP(), y = (float)FPOP(), x = (float)FPOP(), at[3], ahead[3];

    bank_want((int)bank);
    view_point(x, y, z, at, ahead);
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
    PUSH(snddrv_bank((int)k, name) ? -1 : 0);
}
PRIM(p_sound_set) {   /* ( set -- ) the sound set bank 4 holds (Progress_LoadSoundSet) */
    sWantSet = (int)POP();
    bank_want(4);
}
PRIM(p_common_sound) {   /* ( id -- ) play sound `id` of the common bank */
    Cell id = POP();

    bank_want(5);
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
PRIM(p_motion_table) {   /* ( id vaddr -- ) its motion table: in the executable at vaddr, an entry a motion */
    Cell va = POP();
    Actor *a = actor_arg(f, POP());
    int n = model_motion_count(&a->model);

    a->table = world_exe(&gEngine.world, (uint32_t)va, (size_t)n * 6);
    a->ntable = a->table != NULL ? n : 0;
}
PRIM(p_exe_bytes) {   /* ( vaddr n -- addr | 0 ) the executable's bytes there (read-only) */
    Cell n = POP(), va = POP();

    PUSH(world_exe(&gEngine.world, (uint32_t)va, (size_t)n));
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
    hactor_frame(e->forth);   /* the actors' frame, then their models move on */
    for (i = 0; i < MAX_ACTORS; i++) {
        actor_tick(&e->actors[i]);
    }
    cutscene_camera(&e->camera);   /* a cutscene's shot with the camera in it has it */
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
    render_look_tick();
}

/* ---- the retained 2D layer: what actors put on the screen (the message window, prompts),
 * kept until they change it and drawn each frame, layer by layer over the picture ---- */
#define UI_LAYERS 8
#define UI_ITEMS 64
typedef struct UiItem {
    int text;                 /* 0 a rectangle, 1 text */
    float x, y, w, h;         /* (text: w the scale) */
    uint32_t rgba;
    char s[96];
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
PRIM(p_ui_clear) { sUiN[ui_layer(f, POP())] = 0; }   /* ( layer -- ) */
PRIM(p_ui_rect) {   /* ( layer x y w h rgba -- ) */
    Cell rgba = POP(), height = POP(), width = POP(), y = POP(), x = POP();
    UiItem *u = ui_add(f, POP());

    if (u != NULL) {
        *u = (UiItem){0, (float)x, (float)y, (float)width, (float)height, (uint32_t)rgba, ""};
    }
}
PRIM(p_ui_text) {   /* ( layer addr len x y rgba scale -- ) */
    Cell scale = POP(), rgba = POP(), y = POP(), x = POP(), n = POP(), a = POP();
    UiItem *u = ui_add(f, POP());

    if (u != NULL) {
        *u = (UiItem){1, (float)x, (float)y, (float)(scale < 1 ? 1 : scale), 0.0f, (uint32_t)rgba, ""};
        snprintf(u->s, sizeof(u->s), "%.*s", (int)(n > 0 ? n : 0), n > 0 ? (const char *)a : "");
    }
}
static void ui_draw(void) {
    int l, i;

    for (l = 0; l < UI_LAYERS; l++) {
        for (i = 0; i < sUiN[l]; i++) {
            const UiItem *u = &sUi[l][i];

            if (u->text) {
                render_text(u->x, u->y, u->w, u->rgba, u->s, (int)strlen(u->s));
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

void bind_engine(Forth *f) {
    Vocab *saved = f->m.current, *engine = forth_vocab(f, "engine");
    static const struct {
        const char *name;
        Code code;
    } prims[] = {
        {"room", p_room}, {"route", p_route}, {"route-avoiding", p_route_avoiding}, {"route-door", p_route_door}, {"door-open?", p_door_open}, {"door-exit-in", p_door_exit_in}, {"door-leads", p_door_leads}, {"exit-stand", p_exit_stand}, {"room-id", p_room_id}, {"room-exists?", p_room_exists},
        {"room-bounds", p_room_bounds}, {"room-cameras", p_room_cameras}, {"room-camera", p_room_camera}, {"room-group!", p_room_group}, {"floor-below", p_floor_below},
        {"nav-tris", p_nav_tris}, {"nav-tri", p_nav_tri}, {"tri-center", p_tri_center}, {"tri-normal", p_tri_normal},
        {"exit-tri", p_exit_tri}, {"exit-leads", p_exit_leads}, {"room-exit-door", p_room_exit_door}, {"room-exit-leads", p_room_exit_leads}, {"door-sides", p_door_sides}, {"room-exit-tri", p_room_exit_tri}, {"hud", p_hud}, {"ui-clear", p_ui_clear}, {"ui-rect", p_ui_rect}, {"ui-text", p_ui_text}, {"nav-move", p_nav_move}, {"nav-nearest", p_nav_nearest}, {"nav-at", p_nav_at}, {".room", p_room_info},
        {"camera", p_camera},
        {"cam-new-room", p_cam_new_room}, {"cam-room-start", p_cam_room_start}, {"cam-setup", p_cam_setup},
        {"body-move", p_body_move}, {"body-move-any", p_body_move_any}, {"body-move-local", p_body_move_local}, {"body-turn-toward", p_body_turn_toward}, {"body-heading-to", p_body_heading_to}, {"body-place-at", p_body_place_at}, {"body-tri-to", p_body_tri_to}, {"body-free", p_body_free}, {"bodies-touching?", p_bodies_touching}, {"cam-follow", p_cam_follow}, {"cam-target!", p_cam_target}, {"cam-ease", p_cam_ease}, {"cam-track", p_cam_track},
        {"cam-update", p_cam_update}, {"cam-restart", p_cam_restart}, {"cam-changed?", p_cam_changed},
        {".director", p_cam_info}, {"area-in?", p_area_in}, {"nav-path", p_nav_path}, {"v-nav-move", p_v_nav_move}, {"v-walk", p_v_walk}, {"v-wall", p_v_wall}, {"nav-links", p_nav_links}, {"nav-next", p_nav_next}, {"nav-link-tri", p_nav_link_tri}, {"nav-link-yaw", p_nav_link_yaw}, {"nav-link-spot", p_nav_link_spot}, {"nav-link-at?", p_nav_link_at}, {"nav-link-side", p_nav_link_side}, {"nav-link-front", p_nav_link_front}, {"v-free", p_v_free},
        {"v-path", p_v_path}, {"v-tri", p_v_tri}, {"nav-floor", p_nav_floor}, {"v-tri-in", p_v_tri_in}, {"vec!", p_vec_store}, {"vec@", p_vec_fetch}, {"vec-copy", p_vec_copy},
        {"vec-dist", p_vec_dist}, {"vec-dist-xz", p_vec_dist_xz}, {"vec-heading", p_vec_heading}, {"vec-ahead", p_vec_ahead},
        {"angle-wrap", p_angle_wrap}, {"nav-walk", p_nav_walk}, {"nav-path-point", p_nav_path_point}, {"placed-op", p_placed_op}, {".placed", p_placed_list}, {"door-swing", p_door_swing}, {"room-door-at", p_door_at}, {"door-user-spot", p_door_user_spot},
        {"door-anim", p_door_anim}, {"door-side", p_door_side}, {"door-near?", p_door_near}, {"door-in-area?", p_door_in_area},
        {"door-animating?", p_door_animating}, {"door-on-side?", p_door_on_side}, {"door-side-flags", p_door_side_flags}, {"door-passage", p_door_passage}, {"door-move", p_door_move}, {"game-tick", p_game_tick}, {"door-events", p_door_events}, {"door-here?", p_door_present}, {"area-middle", p_area_middle}, {"to-screen", p_to_screen}, {"placed-count", p_placed_count}, {"placed-info", p_placed_info}, {"area-count", p_area_count}, {"area-kind", p_area_kind}, {"area-corner", p_area_corner}, {"area-cross", p_area_cross}, {"exit-area", p_exit_area}, {"movie-open", p_movie_open}, {"movie-status", p_movie_status},
        {"movie-frame", p_movie_frame}, {"cutscene-load", p_cs_start}, {"cutscene-run", p_cs_run}, {"cutscene-go", p_cs_go},
        {"cutscene-frame!", p_cs_frame_set}, {"cutscene-frame", p_cs_frame}, {"cutscene-update", p_cs_update}, {"cutscene-end", p_cs_end},
        {"cutscene-status", p_cs_status}, {"cutscene-in-shot?", p_cs_in_shot}, {"cutscene-near?", p_cs_near_end}, {"cutscene-shot-at", p_cs_shot_at},
        {"cutscene-signals", p_cs_signals}, {"cutscene-active?", p_cs_active}, {"pause!", p_pause}, {"seq-bank", p_seq_bank}, {"seq-load", p_seq_load}, {"seq-play", p_seq_play},
        {"seq-playing?", p_seq_playing}, {"seq-volume", p_seq_volume}, {"seq-port-volume", p_seq_port_volume},
        {"seq-chan-volume", p_seq_chan_volume}, {"seq-midi", p_seq_midi}, {"seq-reset", p_seq_reset}, {"music-play", p_music_play}, {"music-stop", p_music_stop},
        {"music-pause", p_music_pause}, {"music-volume!", p_music_volume}, {"music-playing?", p_music_playing}, {"cutscene-signal-total", p_cs_total}, {"cutscene-letterbox-off", p_cs_letterbox_off}, {"movie-close", p_movie_close}, {"movie-pause", p_movie_pause}, {"movie-paused?", p_movie_paused}, {"movie-compose", p_movie_compose},
        {"movie-volume!", p_movie_volume}, {"movie-draw", p_movie_draw}, {"stage-light", p_stage_light}, {"stage-lights", p_stage_lights},
        {"stage-ambient", p_stage_ambient}, {"room-clear", p_room_clear}, {"common-sound", p_common_sound},
        {"bank-sound", p_bank_sound}, {"bank-sound-at", p_bank_sound_at}, {"sound-set!", p_sound_set}, {"sound-at", p_sound_at},
        {"stop-sound", p_sound_stop}, {"sound-load", p_sound_load}, {"sound-reverb!", p_sound_reverb}, {"sound-loaded?", p_sound_loaded}, {"sound-scale!", p_sound_scale}, {"sound-stop-voices", p_sound_stop_voices}, {"sound-stop-all", p_sound_stop_all}, {"sound-fade", p_sound_fade},
        {"sound-volume!", p_sound_volume}, {"sound-bank", p_sound_bank}, {".voices", p_dot_voices}, {"exit-door", p_exit_door}, {"door-flags", p_door_flags},
        {"nav-block!", p_nav_block}, {"nav-group!", p_nav_group}, {"nav-tri-flags!", p_nav_tri_flags},
        {"nav-in-group?", p_nav_in_group}, {"nav-flags", p_nav_flags},
        {"message-layout", p_message_layout}, {"message-lines", p_message_lines}, {"message-line", p_message_line},
        {"message-options", p_message_options}, {"message-option", p_message_option},
        {"message-choice-flags", p_message_choice_flags}, {"message-param!", p_message_param}, {"exit-spot", p_exit_spot},
        {"actor-load", p_actor_load}, {"actor-free", p_actor_free}, {"actor", p_actor},
        {"motion!", p_motion_store}, {"has-motion?", p_has_motion}, {"motion-play", p_motion_play}, {"motion-entry", p_motion_entry}, {"motion-index", p_motion_index}, {"motion-variant", p_motion_variant}, {"motion-events", p_motion_events},
        {"motion-table", p_motion_table}, {"exe-bytes", p_exe_bytes}, {"root-delta", p_root_delta}, {"bone-pos", p_bone_pos}, {"foot-down?", p_foot_down}, {"dog-legs", p_dog_legs}, {"motion-track", p_motion_track}, {"motion-overlay", p_motion_overlay}, {"turns-clear", p_turns_clear}, {"turn+", p_turn_add}, {"motion@", p_motion_fetch}, {"motion-done?", p_motion_done},
        {"motion-frames", p_motion_frames}, {".motions", p_motions},
        {"key-down?", p_key_down}, {"key-hold", p_key_hold}, {"key-pressed?", p_key_pressed}, {"mouse-dx", p_mouse_dx},
        {"mouse-dy", p_mouse_dy}, {"mouse-down?", p_mouse_down},
        {"on-tick", p_on_tick}, {"off-tick", p_off_tick}, {"ticks", p_ticks}, {"dt", p_dt},
        {"on-draw", p_on_draw}, {"off-draw", p_off_draw}, {"pen-color", p_pen_color},
        {"pen-scale", p_pen_scale}, {"draw-text", p_draw_text}, {"draw-rect", p_draw_rect},
        {"screen-size", p_screen_size}, {"char-size", p_char_size}, {"n>s", p_n_to_s}, {"h>s", p_h_to_s}, {"f>s$", p_f_to_s},
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
    field(f, "act.mflags", offsetof(Actor, mflags));    /* 32-bit: the motion's flags (0x20 wrapped this tick) */
    field(f, "act.fade", offsetof(Actor, fade));        /* sf@: ticks of cross-fade left (0: settled) */
    field(f, "act.vweight", offsetof(Actor, vweight));  /* sf@: the motion against its variant (1: alone) */
    field(f, "act.variant", offsetof(Actor, variant));  /* 32-bit: the variant's index, -1 none */
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
