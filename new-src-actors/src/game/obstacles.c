/* The obstacles' geometry and motion (obstacles.h; the original's room_map.c). */
#include "obstacles.h"

#include <string.h>

#include "engine.h"

enum {
    BLOCKED = 0x20400000,    /* under it */
    RING_SIDE = 0x20800000,  /* the ring round it: a side */
    RING_CORNER = 0x20000000,
    WALKABLE = 0x100,        /* the floor it may go onto ... */
    NOT_ONTO = 0x440080      /* ... and what it may not */
};

typedef struct {
    int a, b;      /* its square */
    int dx, dz;    /* from the reference square, in squares */
    int w, d;      /* across, deep */
} Part;

typedef struct {
    int on, stopped;
    int a, b;          /* its reference square */
    int ta, tb;        /* the square it is moving to (-1 none) */
    int step, frames;  /* its move */
    Vec3 dir;          /* the push */
    Vec3 middle;
    float ox, oz;      /* its model's offset from the middle */
    int nparts;
    Part parts[3];
} Obstacle;

static Obstacle sObst[OBSTACLES_MAX];

/* the move (OBSTACLE.MTN's one: 30 frames, coming to 5 - two squares) */
static const float kMove[] = {
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0700918138f, 0.0951969326f, 0.142289117f, 0.211368367f,
    0.285176992f, 0.347637147f, 0.407967538f, 0.466168225f, 0.567985892f, 0.656211078f,
    0.769031882f, 0.576050103f, 0.290232599f, 0.0935284644f, 0.0210638456f,
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
};
#define MOVE_FRAMES ((int)(sizeof(kMove) / sizeof(kMove[0])))

#define KINDS_AT 0x47A938u   /* ObstacleKinds: a pointer each */

static NavMesh *nav(void) { return &gEngine.room.nav; }
static Obstacle *get(int i) { return i >= 0 && i < OBSTACLES_MAX ? &sObst[i] : NULL; }
static Obstacle *active(int i) {
    Obstacle *o = get(i);

    return o != NULL && o->on ? o : NULL;
}

/* ---- squares ---- */

static void step(int *a, int *b, float dx, float dz) {   /* (unchanged at the mesh's edge) */
    navmesh_square_step(nav(), a, b, dx, dz);
}
static void step_along(int *a, int *b, const Vec3 *dir, int n) {
    while (dir != NULL && n-- > 0) {
        step(a, b, dir->x, dir->z);
    }
}
/* the middle of square (a, b): its six corners' mean (the original's 1/6 as a float) */
static Vec3 square_middle(int a, int b) {
    static const union { uint32_t u; float f; } kSixth = {0x3E2AAAADu};
    const NavMesh *n = nav();
    Vec3 s = vec3(0, 0, 0);
    int k;

    if (a < 0 || a >= n->ntris || b < 0 || b >= n->ntris) {
        return s;
    }
    for (k = 0; k < 3; k++) {
        s = vec3_add(s, n->tris[a].v[k]);
    }
    for (k = 0; k < 3; k++) {
        s = vec3_add(s, n->tris[b].v[k]);
    }
    return vec3_scale(s, kSixth.f);
}
/* each part's square: from the reference square |dx| along +-x, then |dz| along +-z */
static void part_squares(Obstacle *o) {
    int i;

    for (i = 0; i < o->nparts; i++) {
        Part *p = &o->parts[i];
        int a = o->a, b = o->b, k;

        for (k = 0; k < (p->dx < 0 ? -p->dx : p->dx); k++) {
            step(&a, &b, p->dx >= 0 ? 1.0f : -1.0f, 0.0f);
        }
        for (k = 0; k < (p->dz < 0 ? -p->dz : p->dz); k++) {
            step(&a, &b, 0.0f, p->dz >= 0 ? 1.0f : -1.0f);
        }
        p->a = a;
        p->b = b;
    }
}

/* ---- visiting a part's squares (its area) or the ring round it ---- */

typedef int (*Visit)(int a, int b, int corner, void *ctx);   /* nonzero: stop */

static void flag_square(int a, int b, int set, uint32_t bits) {
    NavMesh *n = nav();
    int t[2] = {a, b}, k;

    for (k = 0; k < 2; k++) {
        if (t[k] >= 0 && t[k] < n->ntris) {
            n->tris[t[k]].flags = set ? n->tris[t[k]].flags | bits : n->tris[t[k]].flags & ~bits;
        }
    }
}
/* row by row, from the part's square moved `shift` squares along `dir` */
static void part_area(const Part *p, const Vec3 *dir, int shift, Visit visit, void *ctx) {
    int a = p->a, b = p->b, x, z;

    step_along(&a, &b, dir, shift);
    for (z = 0; z < p->d; z++) {
        int ra = a, rb = b;

        for (x = 0; x < p->w; x++) {
            visit(a, b, 0, ctx);
            step(&a, &b, 1.0f, 0.0f);
        }
        a = ra;
        b = rb;
        step(&a, &b, 0.0f, 1.0f);
    }
}
/* round it from the -x -z corner: each side, then its corner */
static int part_ring(const Part *p, const Vec3 *dir, int shift, Visit visit, void *ctx) {
    int a = p->a, b = p->b, side, k;

    step_along(&a, &b, dir, shift);
    step(&a, &b, -1.0f, 0.0f);
    step(&a, &b, 0.0f, -1.0f);
    for (side = 0; side < 4; side++) {
        float s = side < 2 ? 1.0f : -1.0f, dx = side & 1 ? 0.0f : s, dz = side & 1 ? s : 0.0f;
        int n = side & 1 ? p->d : p->w;

        for (k = 0; k < n; k++) {
            step(&a, &b, dx, dz);
            if (visit(a, b, 0, ctx)) {
                return 1;
            }
        }
        step(&a, &b, dx, dz);
        if (visit(a, b, 1, ctx)) {
            return 1;
        }
    }
    return 0;
}
static void each_area(Obstacle *o, const Vec3 *dir, int shift, Visit visit, void *ctx) {
    int i;

    for (i = 0; i < o->nparts; i++) {
        part_area(&o->parts[i], dir, shift, visit, ctx);
    }
}
static int each_ring(Obstacle *o, const Vec3 *dir, int shift, Visit visit, void *ctx) {
    int i;

    for (i = 0; i < o->nparts; i++) {
        if (part_ring(&o->parts[i], dir, shift, visit, ctx)) {
            return 1;
        }
    }
    return 0;
}

static int block_sq(int a, int b, int corner, void *ctx) { (void)corner, (void)ctx; flag_square(a, b, 1, BLOCKED); return 0; }
static int unblock_sq(int a, int b, int corner, void *ctx) { (void)corner, (void)ctx; flag_square(a, b, 0, BLOCKED); return 0; }
static int mark_sq(int a, int b, int corner, void *ctx) { (void)ctx; flag_square(a, b, 1, corner ? RING_CORNER : RING_SIDE); return 0; }
static int unmark_sq(int a, int b, int corner, void *ctx) { (void)corner, (void)ctx; flag_square(a, b, 0, RING_SIDE); return 0; }

static void block(Obstacle *o, const Vec3 *dir) { each_area(o, dir, 2, block_sq, NULL); }
static void unblock(Obstacle *o) { each_area(o, NULL, 0, unblock_sq, NULL); }
static void mark_ring(Obstacle *o, const Vec3 *dir) { each_ring(o, dir, 2, mark_sq, NULL); }
static void unmark_ring(Obstacle *o) { each_ring(o, NULL, 0, unmark_sq, NULL); }

/* ---- placing ---- */

void obstacles_reset(void) {
    int i;

    memset(sObst, 0, sizeof(sObst));
    for (i = 0; i < OBSTACLES_MAX; i++) {
        sObst[i].a = sObst[i].b = sObst[i].ta = sObst[i].tb = -1;
    }
}
int obstacle_on(int i) { return active(i) != NULL; }

static void placed(Obstacle *o) {   /* (Obstacle_Placed) */
    o->middle = square_middle(o->a, o->b);
    part_squares(o);
    block(o, NULL);
    mark_ring(o, NULL);
}
static int32_t s32_at(const uint8_t *p) {
    int32_t v;

    memcpy(&v, p, 4);
    return v;
}
int obstacle_make(int i, int kind, int a, int b) {   /* (obstacle_make) */
    Obstacle *o = get(i);
    const uint8_t *ptr = world_exe(&gEngine.world, KINDS_AT + (uint32_t)kind * 4u, 4), *def;
    int n, k;

    if (o == NULL || kind < 0 || ptr == NULL || (def = world_exe(&gEngine.world, (uint32_t)s32_at(ptr), 12)) == NULL) {
        return -1;
    }
    n = s32_at(def + 8);
    if (n < 1 || n > 3 || (def = world_exe(&gEngine.world, (uint32_t)s32_at(ptr), 12 + 16 * (size_t)n)) == NULL) {
        return -1;
    }
    o->on = 1;
    memcpy(&o->ox, def, 4);
    memcpy(&o->oz, def + 4, 4);
    o->a = a;
    o->b = b;
    for (k = 0; k < n && o->nparts < 3; k++) {   /* (Obstacle_AddPart: 3 at most) */
        const uint8_t *src = def + 12 + 16 * k;
        Part *p = &o->parts[o->nparts++];

        p->dx = s32_at(src);
        p->dz = s32_at(src + 4);
        p->w = s32_at(src + 8);
        p->d = s32_at(src + 12);
    }
    placed(o);
    return 0;
}
void obstacle_stop(int i) {
    Obstacle *o = active(i);

    if (o != NULL) {
        o->stopped = 1;
    }
}
void obstacle_square(int i, int *a, int *b) {
    Obstacle *o = get(i);

    *a = o != NULL ? o->a : -1;
    *b = o != NULL ? o->b : -1;
}
Vec3 obstacle_middle(int i) {
    Obstacle *o = get(i);

    return o != NULL ? o->middle : vec3(0, 0, 0);
}
Vec3 obstacle_pos(int i) {
    Obstacle *o = get(i);

    return o != NULL ? vec3(o->middle.x + o->ox, o->middle.y, o->middle.z + o->oz) : vec3(0, 0, 0);
}

/* ---- queries ---- */

typedef struct {
    const int *tri;
    int n;
    uint32_t any, all;
    int own[2 * 3 * 64], nown;   /* (can_push: the squares under it now) */
} SquareTest;

static int has_tri(int a, int b, int corner, void *ctx) {   /* (a side, not a corner) */
    const SquareTest *t = ctx;

    return !corner && (a == t->tri[0] || b == t->tri[0]);
}
unsigned obstacles_ring_mask(int tri) {   /* (Obstacles_RingMask, Obstacle_OnRing) */
    SquareTest t = {.tri = &tri, .n = 1};
    unsigned mask = 0;
    int k;

    for (k = 0; k < OBSTACLES_MAX; k++) {
        if (sObst[k].on && each_ring(&sObst[k], NULL, 0, has_tri, &t)) {
            mask |= 1u << k;
        }
    }
    return mask;
}
int obstacle_on_square(int i, int tri) {   /* (Obstacle_IsSquare) */
    Obstacle *o = active(i);

    if (o == NULL) {
        return 0;
    }
    if (o->step == o->frames - 1) {
        return o->ta == tri || o->tb == tri;
    }
    return o->a == tri || o->b == tri;
}

static int someone_on(int a, int b, int corner, void *ctx) {
    const SquareTest *t = ctx;
    int k;

    (void)corner;
    for (k = 0; k < t->n; k++) {
        if (a == t->tri[k] || b == t->tri[k]) {
            return 1;
        }
    }
    return 0;
}
static int own_sq(int a, int b, int corner, void *ctx) {
    SquareTest *t = ctx;

    (void)corner;
    if (t->nown + 2 <= (int)(sizeof(t->own) / sizeof(t->own[0]))) {
        t->own[t->nown++] = a;
        t->own[t->nown++] = b;
    }
    return 0;
}
static int is_own(const SquareTest *t, int tri) {
    int k;

    for (k = 0; k < t->nown; k++) {
        if (t->own[k] == tri) {
            return 1;
        }
    }
    return 0;
}
static int test_sq(int a, int b, int corner, void *ctx) {
    SquareTest *t = ctx;
    const NavMesh *n = nav();
    int s[2] = {a, b}, k;

    (void)corner;
    for (k = 0; k < 2; k++) {
        uint32_t f = s[k] >= 0 && s[k] < n->ntris ? n->tris[s[k]].flags : 0;

        if (is_own(t, s[k])) {   /* (its own block lifted) */
            f &= ~(uint32_t)(BLOCKED & ~RING_CORNER);
        }
        t->any |= f;
        t->all &= f;
    }
    return 0;
}
/* (Obstacle_CanPush lifts its blocks, tests, and blocks its reference square again; here its own
 * squares' block is left out of the test instead - the same answer at rest, and nothing changed:
 * hga's Fiona also asks while it is moving, when blocking the reference square again would leave
 * the square it is leaving blocked) */
int obstacle_can_push(int i, Vec3 dir, const int *others, int n) {   /* (Obstacles_CanPush) */
    Obstacle *o = active(i);
    SquareTest t = {.tri = others, .n = n, .all = WALKABLE};

    if (o == NULL || o->stopped) {
        return 0;
    }
    /* (Obstacle_RingClear: nobody on the ring two, then one, squares along) */
    if (each_ring(o, &dir, 2, someone_on, &t) || each_ring(o, &dir, 1, someone_on, &t)) {
        return 0;
    }
    each_area(o, NULL, 0, own_sq, &t);   /* (Obstacle_AreaFree) */
    each_area(o, &dir, 2, test_sq, &t);
    return !(t.any & NOT_ONTO) && (t.all & WALKABLE);
}

/* ---- pushed, moving ---- */

void obstacle_push(int i, Vec3 dir) {
    Obstacle *o = active(i);
    int k, a, b;

    if (o == NULL) {
        return;
    }
    a = o->a;   /* (Obstacles_SetTarget) */
    b = o->b;
    step_along(&a, &b, &dir, 2);
    o->ta = a;
    o->tb = b;
    unblock(o);   /* (Obstacles_Pull) */
    unmark_ring(o);
    for (k = 0; k < OBSTACLES_MAX; k++) {   /* (Obstacles_Settle) */
        if (sObst[k].on && &sObst[k] != o) {
            mark_ring(&sObst[k], NULL);
        }
    }
    block(o, &dir);
    mark_ring(o, &dir);
    o->dir = dir;   /* (Obstacles_StartMove) */
    o->step = 0;
    o->frames = MOVE_FRAMES;
}
void obstacle_moved(int i) {   /* (Obstacle_Moved) */
    Obstacle *o = active(i);

    if (o == NULL || o->ta < 0) {
        return;
    }
    o->a = o->ta;
    o->b = o->tb;
    o->ta = o->tb = -1;
    part_squares(o);
    o->middle = square_middle(o->a, o->b);
    o->step = o->frames;
}
int obstacle_step(int i) {   /* (Obstacle_MoveFrame) */
    Obstacle *o = active(i);
    int scrape;

    if (o == NULL || o->step == o->frames) {
        return 0;
    }
    scrape = o->step == 6;
    o->step++;
    if (o->step == o->frames) {
        obstacle_moved(i);
    } else {
        o->middle = vec3_add(o->middle, vec3_scale(o->dir, kMove[o->step - 1]));
    }
    return scrape;
}
