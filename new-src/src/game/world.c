#include "world.h"

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
