#include "world.h"
#include "progress.h"

#include "../data/exe.h"

#include <string.h>

#define DOOR_TABLE 0x003DCFC0u
#define ROOM_TABLE 0x003D8BC0u

static int s16(const uint8_t *p) {
    int16_t x;

    memcpy(&x, p, 2);
    return x;
}

static int u16(const uint8_t *p) {
    uint16_t x;

    memcpy(&x, p, 2);
    return x;
}

const uint8_t *world_exe(const World *w, uint32_t vaddr, size_t n) {
    return w->exe.data != NULL ? exe_at(&w->exe, vaddr, n) : NULL;
}

int world_load(World *w, const char *exe_path) {
    Exe e;
    const uint8_t *doors, *rooms;
    int i, k, s;

    memset(w, 0, sizeof(*w));
    if (!exe_load(&e, exe_path)) {
        return 0;
    }
    doors = exe_at(&e, DOOR_TABLE, WORLD_DOORS * 16);
    rooms = exe_at(&e, ROOM_TABLE, WORLD_ROOMS * 0x40);
    if (doors == NULL || rooms == NULL) {
        exe_free(&e);
        return 0;
    }
    for (i = 0; i < WORLD_ROOMS; i++) {
        for (k = 0; k < ROOM_EXITS; k++) {
            const uint8_t *x = rooms + i * 0x40 + k * 8;
            RoomExit *re = &w->exits[i][k];

            re->tri[0] = s16(x);
            re->tri[1] = s16(x + 2);
            re->tri[2] = s16(x + 4);
            re->camera = s16(x + 6);
            re->door = -1;
        }
    }
    for (i = 0; i < WORLD_DOORS && u16(doors + i * 16) != 0xFFFF; i++) {
        Door *d = &w->doors[i];

        for (s = 0; s < 2; s++) {
            const uint8_t *side = doors + i * 16 + s * 6;

            d->side[s].room = u16(side);
            d->side[s].exit = side[2];
            d->side[s].tri = s16(side + 4);
            /* the first door listed for an exit is the one it uses (Rooms_Rebuild) */
            if (d->side[s].room < WORLD_ROOMS && d->side[s].exit < ROOM_EXITS &&
                w->exits[d->side[s].room][d->side[s].exit].door < 0) {
                w->exits[d->side[s].room][d->side[s].exit].door = i;
            }
        }
        memcpy(&d->flags, doors + i * 16 + 12, 4);
    }
    w->ndoors = i;
    w->loaded = 1;
    w->exe = e;
    return 1;
}

static const DoorSide *side_of(const World *w, int room, int exit, int other) {
    const Door *d;
    int s;

    if (room < 0 || room >= WORLD_ROOMS || exit < 0 || exit >= ROOM_EXITS || w->exits[room][exit].door < 0) {
        return NULL;
    }
    d = &w->doors[w->exits[room][exit].door];
    for (s = 0; s < 2; s++) {
        if (d->side[s].room == room && d->side[s].exit == exit) {
            return &d->side[s ^ other];
        }
    }
    return NULL;
}

int world_exit_leads(const World *w, int room, int exit, int *to_exit) {
    const DoorSide *there = side_of(w, room, exit, 1);

    if (there == NULL || there->room == room) {
        return -1;
    }
    *to_exit = there->exit;
    return there->room;
}

int world_exit_tri(const World *w, int room, int exit, int which) {
    const DoorSide *here;

    if (room < 0 || room >= WORLD_ROOMS || exit < 0 || exit >= ROOM_EXITS || which < 0 || which > 2) {
        return -1;
    }
    if (w->exits[room][exit].tri[which] >= 0) {
        return w->exits[room][exit].tri[which];
    }
    here = side_of(w, room, exit, 0);
    return here != NULL ? here->tri : -1;
}

/* ---- the doors with the game's state ---- */

static int closed_off(const World *w, int d) {
    return d < 0 || d >= w->ndoors || ((gProgress.closed_off[d / 32] >> (d % 32)) & 1);
}

int world_door_exit(const World *w, int d, int room) {
    int s;

    if (closed_off(w, d)) {
        return -1;
    }
    for (s = 0; s < 2; s++) {
        if (w->doors[d].side[s].room == room) {
            return w->doors[d].side[s].exit;
        }
    }
    return -1;
}

int world_door_leads(const World *w, int d, int room) {
    int s;

    if (closed_off(w, d)) {
        return -1;
    }
    for (s = 0; s < 2; s++) {
        if (w->doors[d].side[s].room == room) {
            return w->doors[d].side[s ^ 1].room;
        }
    }
    return -1;
}

int world_door_open(const World *w, int d) {
    uint32_t state;

    if (d < 0 || d >= w->ndoors) {
        return 0;
    }
    if (w->doors[d].flags & 1) {   /* a doorway */
        return 1;
    }
    state = gProgress.doors[d];
    return !(state & 8) && (state & 2) != 0;
}

/* a door's one-way flag on `room`'s side (other: seen from the far side) for walkers of `kind`
 * (0: always counted; else only where the door says it counts): 1 / 0, or -1 (Rooms_DoorFromSide
 * / Rooms_DoorFromOther) */
static int door_way(const World *w, int d, int room, int kind, int other) {
    static const uint32_t flag[2][2] = {{0x10, 0x8}, {0x8, 0x10}}, check[2][2] = {{0x40, 0x80}, {0x80, 0x40}};
    uint32_t flags;
    int s;

    if (closed_off(w, d)) {
        return -1;
    }
    flags = w->doors[d].flags;
    for (s = 0; s < 2; s++) {
        if (w->doors[d].side[s].room == room && (!kind || (flags & check[other][s]))) {
            return (flags & flag[other][s]) ? 1 : 0;
        }
    }
    return -1;
}

typedef struct Step {
    int door, room, far, from, depth;
} Step;

#define ROUTE_STEPS 128

typedef struct Search {
    const World *w;
    int kind, way;   /* the way the step being searched from came through its door (-1: none) */
    const uint32_t *avoid;   /* doors not to take (a bit a door), or NULL */
    Step steps[ROUTE_STEPS];
    int head, tail, cur;
    uint32_t seen[(WORLD_DOORS + 31) / 32];
} Search;

/* the doors out of `room` as steps from the current one (RoutePlanner_QueueDoors) */
static int queue_doors(Search *q, int room) {
    const World *w = q->w;
    int i;

    for (i = 0; i < ROOM_EXITS; i++) {
        int d = room >= 0 && room < WORLD_ROOMS ? w->exits[room][i].door : -1;
        uint32_t bit;
        Step *s;

        if (d < 0 || closed_off(w, d)) {
            continue;
        }
        if ((q->way == 0 || q->way == 1) && q->way != door_way(w, d, room, q->kind, 0)) {
            continue;
        }
        bit = 1u << (d % 32);
        if (q->seen[d / 32] & bit) {
            continue;
        }
        if (!(q->avoid && (q->avoid[d / 32] & bit)) && !(gProgress.doors[d] & 8)) {   /* (not avoided, not locked; passable from any side) */
            s = &q->steps[q->tail++];
            if (q->tail >= ROUTE_STEPS) {
                return -1;
            }
            s->door = d;
            s->room = world_door_leads(w, d, room);
            s->far = door_way(w, d, room, q->kind, 1);
            s->from = q->cur;
            s->depth = q->cur >= 0 ? q->steps[q->cur].depth + 1 : 0;
        }
        q->seen[d / 32] |= bit;
    }
    return 0;
}

int world_route(const World *w, int from, int to, int kind, int max, int *out, int nout) {
    return world_route_avoiding(w, from, to, kind, max, NULL, out, nout);
}

int world_route_avoiding(const World *w, int from, int to, int kind, int max, const uint32_t *avoid, int *out, int nout) {
    static Search q;   /* (big: kept off the stack) */
    int n, k, i;

    if (from < 0 || from >= WORLD_ROOMS || to < 0 || to >= WORLD_ROOMS) {
        return -1;
    }
    if (from == to) {
        return 0;
    }
    memset(&q, 0, sizeof(q));
    q.w = w;
    q.kind = kind;
    q.avoid = avoid;
    q.way = -1;
    q.cur = -1;
    if (queue_doors(&q, from) < 0) {
        return -1;
    }
    while (q.head < q.tail) {
        Step *s = &q.steps[q.head];

        q.cur = q.head++;
        if (max >= 0 && s->depth >= max) {
            return -1;
        }
        q.way = s->far;
        if (s->room == to) {
            for (n = 0, k = q.cur; k >= 0; k = q.steps[k].from) {
                n++;
            }
            for (i = n - 1, k = q.cur; k >= 0; k = q.steps[k].from, i--) {
                if (i < nout) {
                    out[i] = q.steps[k].door;
                }
            }
            return n;
        }
        if (queue_doors(&q, s->room) < 0) {
            return -1;
        }
    }
    return -1;
}
