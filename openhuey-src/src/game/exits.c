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

/* which edge of triangle t (in x / z) the step from -> to leaves it by: 0..2; 3 with `to`
 * inside (on an edge counts); 4 with it outside and no edge crossed (NavTri_ExitEdge) */
static int leaves_by(const NavTri *t, Vec3 from, Vec3 to) {
    int e, inside = 0;

    for (e = 0; e < 3; e++) {
        Vec3 a = t->v[e], b = t->v[(e + 1) % 3];
        float ex = b.x - a.x, ez = b.z - a.z;
        float dx = to.x - from.x, dz = to.z - from.z;
        float p = (from.x - a.x) * ez, q = (from.z - a.z) * ex;
        float den = dx * ez - dz * ex, s, u;

        if (!((to.x - a.x) * ez - (to.z - a.z) * ex < 0.0f)) {
            inside++;
            continue;
        }
        if (p - q < 0.0f || den == 0.0f) {   /* (from outside this edge too; along it) */
            continue;
        }
        s = (q - p) / den;
        if (s < 0.0f || !(s <= 1.0f)) {
            continue;
        }
        u = fabsf(ex) <= fabsf(ez) ? (from.z + s * dz - a.z) / ez : (from.x + s * dx - a.x) / ex;
        if (u < 0.0f || !(u <= 1.0f)) {
            continue;
        }
        return e;
    }
    return inside == 3 ? 3 : 4;
}

/* walk the nav mesh from `from` in triangle *tri (then from each triangle's centre) to `to`:
 * whether it gets there, *tri the triangle holding it, or leaves the mesh (the original's
 * door_walk) */
static int walk(const NavMesh *n, int *tri, Vec3 from, Vec3 to) {
    int steps;

    for (steps = 0; steps < 256; steps++) {   /* (the original has no bound) */
        int e;

        if (*tri < 0 || *tri >= n->ntris) {
            return 0;
        }
        e = leaves_by(&n->tris[*tri], from, to);
        if (e == 3) {
            return 1;
        }
        if (e == 4) {
            return 0;
        }
        *tri = n->tris[*tri].next[e];
        if (*tri < 0) {
            return 0;
        }
        from = navmesh_center(n, *tri);
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
        if (walk(&r->nav, &tri, stand, to)) {
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
