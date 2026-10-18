/* Swim paths (src/game/rooms/room_61.c SwimPath_Setup / SwimPath_Step / SwimPath_PointOn). */
#include "swimpath.h"
#include <math.h>
#include <stdint.h>

#define SWIM_POINTS 64   /* (the largest grid the rooms use: 2 x 3 x 7) */
#define PI_F 3.14159265f

typedef struct SwimPath {
    Vec3 lo, hi;              /* the box */
    int n, frames, t, at;     /* its points, frames a stretch, the frame in it, the stretch's start */
    Vec3 pts[SWIM_POINTS];    /* (the original keeps them as 1/16ths in s16s: so are these) */
} SwimPath;

static SwimPath sPaths[SWIM_PATHS];
static uint32_t sSeed = 0x6C078965u;

static float rnd(void) {   /* 0 <= r < 1 */
    sSeed = sSeed * 1103515245u + 12345u;
    return (float)((sSeed >> 8) & 0xFFFF) / 65536.0f;
}
static float sixteenths(float v) { return (float)(int16_t)(int)(16.0f * v) * 0.0625f; }
static float wrap(float a) {
    while (a > PI_F) {
        a -= 2.0f * PI_F;
    }
    while (a < -PI_F) {
        a += 2.0f * PI_F;
    }
    return a;
}

int swimpath_setup(int i, int nx, int ny, int nz, int frames, Vec3 o) {
    SwimPath *s;
    int x, y, z, k;

    if (i < 0 || i >= SWIM_PATHS || nx * ny * nz <= 0 || nx * ny * nz > SWIM_POINTS || frames <= 0) {
        return 0;
    }
    s = &sPaths[i];
    s->n = 0;
    for (x = 0; x < nx; x++) {
        for (y = 0; y < ny; y++) {
            for (z = 0; z < nz; z++) {
                Vec3 *p = &s->pts[s->n++];

                p->x = sixteenths(o.x + 10.0f * ((float)x + rnd()));
                p->y = sixteenths(o.y + 10.0f * ((float)y + rnd()));
                p->z = sixteenths(o.z + 10.0f * ((float)z + rnd()));
            }
        }
    }
    for (k = 0; k < s->n; k++) {   /* (shuffled: swaps among the first 30) */
        int a = (int)(30.0f * rnd()), b = (int)(30.0f * rnd());
        Vec3 t;

        if (a >= s->n || b >= s->n) {
            continue;
        }
        t = s->pts[a];
        s->pts[a] = s->pts[b];
        s->pts[b] = t;
    }
    s->t = 0;
    s->at = 0;
    s->frames = frames;
    s->lo = o;
    s->hi = (Vec3){o.x + 10.0f * (float)nx, o.y + 10.0f * (float)ny, o.z + 10.0f * (float)nz};
    return 1;
}

/* point k's way on: 20 along the line from the point before it to the one after */
static Vec3 way_on(const SwimPath *s, int k) {
    Vec3 a = s->pts[(k + s->n - 1) % s->n], b = s->pts[(k + 1) % s->n];
    Vec3 d = {b.x - a.x, b.y - a.y, b.z - a.z};
    float len = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);

    if (len <= 0.0f) {
        return (Vec3){0.0f, 0.0f, 0.0f};
    }
    return (Vec3){20.0f * d.x / len, 20.0f * d.y / len, 20.0f * d.z / len};
}

void swimpath_step(int i, Vec3 *at, float *yaw) {
    const SwimPath *s;
    Vec3 c0, c1, c2, c3, d0, d1, was = *at;
    float t, u, w0, w1, w2, w3, a;
    int next;

    if (i < 0 || i >= SWIM_PATHS || sPaths[i].n <= 0) {
        return;
    }
    s = &sPaths[i];
    next = s->at + 1 < s->n ? s->at + 1 : 0;
    t = (float)s->t / (float)s->frames;
    u = 1.0f - t;
    w0 = u * u * u;
    w1 = 3.0f * u * u * t;
    w2 = 3.0f * u * t * t;
    w3 = t * t * t;
    c0 = s->pts[s->at];
    d0 = way_on(s, s->at);
    c1 = (Vec3){c0.x + d0.x, c0.y + d0.y, c0.z + d0.z};
    c3 = s->pts[next];
    d1 = way_on(s, next);
    c2 = (Vec3){c3.x - d1.x, c3.y - d1.y, c3.z - d1.z};
    at->x = fmaxf(s->lo.x, fminf(s->hi.x, w0 * c0.x + w1 * c1.x + w2 * c2.x + w3 * c3.x));
    at->y = fmaxf(s->lo.y, fminf(s->hi.y, w0 * c0.y + w1 * c1.y + w2 * c2.y + w3 * c3.y));
    at->z = fmaxf(s->lo.z, fminf(s->hi.z, w0 * c0.z + w1 * c1.z + w2 * c2.z + w3 * c3.z));
    if (++sPaths[i].t >= s->frames) {
        sPaths[i].t = 0;
        sPaths[i].at = s->at + 1 < s->n ? s->at + 1 : 0;
    }
    a = wrap(atan2f(at->x - was.x, at->z - was.z) - *yaw);
    a = fmaxf(-0.0628319f, fminf(0.0628319f, a));   /* (3.6 degrees a frame at most) */
    *yaw = wrap(*yaw + a);
}
