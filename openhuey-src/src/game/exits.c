/* Where a room's exits are: see exits.h. */
#include "exits.h"

#include "../data/pac.h"
#include "areas.h"

#include <math.h>
#include <string.h>

#define PI_F 3.14159265358979f

/* the door of exit `exit` (section 7): its triangle, where it stands, its turn; 0: none */
static int exit_door(const Room *r, int exit, int *tri, Vec3 *stand, float *turn) {
    size_t size;
    const uint8_t *sec = pac_section(&r->pac, PAC_DOORS, &size);
    uint32_t off;
    float v[3];

    if (sec == NULL || exit < 0 || exit >= 8 || size < 32) {
        return 0;
    }
    memcpy(&off, sec + exit * 4, 4);
    if (off == 0 || (size_t)off + 0x24 > size) {
        return 0;
    }
    memcpy(tri, sec + off, 4);
    memcpy(v, sec + off + 0x10, sizeof(v));
    *stand = vec3(v[0], v[1], v[2]);
    memcpy(turn, sec + off + 0x20, 4);
    return 1;
}

static float wrap(float a) {
    while (!(a <= PI_F)) {
        a -= 2.0f * PI_F;
    }
    while (a < -PI_F) {
        a += 2.0f * PI_F;
    }
    return a;
}

static float cross2(Vec3 a, Vec3 b, Vec3 p) {   /* which side of a -> b p is (x / z) */
    return (b.x - a.x) * (p.z - a.z) - (b.z - a.z) * (p.x - a.x);
}

/* the height of triangle t's plane at (x, z) */
static float tri_height(const NavTri *t, float x, float z) {
    Vec3 a = t->v[0], b = t->v[1], c = t->v[2];
    float d = (b.z - c.z) * (a.x - c.x) + (c.x - b.x) * (a.z - c.z), w0, w1;

    if (d == 0.0f) {
        return a.y;
    }
    w0 = ((b.z - c.z) * (x - c.x) + (c.x - b.x) * (z - c.z)) / d;
    w1 = ((c.z - a.z) * (x - c.x) + (a.x - c.x) * (z - c.z)) / d;
    return w0 * a.y + w1 * b.y + (1.0f - w0 - w1) * c.y;
}

/* walk the nav mesh from triangle *tri toward `to` across the edges the way crosses: whether
 * it gets there (the original's door_walk: off the mesh, it doesn't) */
static int walk(const NavMesh *n, int *tri, Vec3 to) {
    int steps;

    for (steps = 0; steps < 256; steps++) {
        const NavTri *t;
        Vec3 c;
        int e, out = -1;

        if (*tri < 0 || *tri >= n->ntris) {
            return 0;
        }
        t = &n->tris[*tri];
        c = vec3((t->v[0].x + t->v[1].x + t->v[2].x) / 3.0f, 0.0f, (t->v[0].z + t->v[1].z + t->v[2].z) / 3.0f);
        for (e = 0; e < 3; e++) {   /* the edge between the centre and `to` passes */
            Vec3 a = t->v[e], b = t->v[(e + 1) % 3];

            if ((cross2(a, b, c) < 0.0f) != (cross2(a, b, to) < 0.0f) &&
                (cross2(c, to, a) < 0.0f) != (cross2(c, to, b) < 0.0f)) {
                out = e;
                break;
            }
        }
        if (out < 0) {
            return 1;   /* `to` is in this triangle */
        }
        *tri = t->next[out];
        if (*tri < 0) {
            return 0;
        }
    }
    return 0;
}

int exit_spot(const Room *r, const World *w, int exit, int which, Vec3 *out) {
    int tri;
    Vec3 stand, mid;
    float turn;

    if (which < 0 || which > 2) {
        return -1;
    }
    if (exit_door(r, exit, &tri, &stand, &turn) && r->id >= 0 && r->id < WORLD_ROOMS &&
        area_middle(r, w->exits[r->id][exit].camera & 0xFFFF, &mid)) {
        const float back[3] = {-4.0f, 12.0f, -12.0f};
        float a = turn, s, c;
        Vec3 to;

        /* Doors_Side: is the area's middle on the door's front? if not, the frame turns round */
        if (sinf(a) * (mid.x - stand.x) + cosf(a) * (mid.z - stand.z) < 0.0f) {
            a = wrap(PI_F + a);
        }
        s = sinf(a);
        c = cosf(a);
        to = vec3(stand.x + s * back[which], stand.y, stand.z + c * back[which]);
        if (walk(&r->nav, &tri, to)) {
            to.y = tri_height(&r->nav.tris[tri], to.x, to.z);
            *out = to;
            return tri;
        }
    }
    if (r->id < 0 || r->id >= WORLD_ROOMS || exit < 0 || exit >= ROOM_EXITS) {
        return -1;
    }
    tri = w->exits[r->id][exit].tri[which];   /* (the room table's own: -1 where it has none) */
    if (tri < 0 || tri >= r->nav.ntris) {
        return -1;
    }
    *out = navmesh_center(&r->nav, tri);
    return tri;
}
