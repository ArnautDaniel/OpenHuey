/* The house: how rooms connect. From two tables in the game's executable
 * (src/game/room_map.c):
 *   the door table (0x3DCFC0): 16-byte entries, two sides {u16 room, u8 exit, u8, s16 nav
 *     triangle} and u32 flags, ended by room 0xFFFF;
 *   the room table (0x3D8BC0): 0x40 bytes a room, 8 exits {s16 triangles: out, in, through;
 *     s16 camera area} (-1: use the door's triangle). */
#ifndef WORLD_H
#define WORLD_H

#include <stdint.h>
#include "../data/exe.h"

#define WORLD_DOORS 400
#define WORLD_ROOMS 0x110
#define ROOM_EXITS 8

typedef struct DoorSide {
    int room, exit, tri;
} DoorSide;

typedef struct Door {
    DoorSide side[2];
    uint32_t flags;
} Door;

typedef struct RoomExit {
    int tri[3];     /* out, in, through (-1: none) */
    int camera;     /* the camera area it belongs to */
    int door;       /* the door it leads through (-1: none) */
} RoomExit;

typedef struct World {
    int loaded;
    Exe exe;                /* the executable, kept: other tables are read from it as needed */
    Door doors[WORLD_DOORS];
    int ndoors;
    RoomExit exits[WORLD_ROOMS][ROOM_EXITS];
} World;

/* n bytes of the executable at a PS2 address (NULL: not there) */
const uint8_t *world_exe(const World *w, uint32_t vaddr, size_t n);
/* read the tables from the executable; 0 if it couldn't be read */
int world_load(World *w, const char *exe_path);
void world_rebuild_exits(World *w);   /* the exits through the doors not closed off (Rooms_Rebuild) */
/* where exit `exit` of `room` leads: the room (and its exit in *to_exit), or -1 */
int world_exit_leads(const World *w, int room, int exit, int *to_exit);
/* exit's nav triangle (which: 0 out, 1 in, 2 through), the door's own when the room table has
 * none; -1 if none */
int world_exit_tri(const World *w, int room, int exit, int which);


/* ---- the doors with the game's state (gProgress: locks, closed off) ---- */
/* which exit of `room` door `d` is (-1: not in that room, or closed off) (Rooms_DoorExit) */
int world_door_exit(const World *w, int d, int room);
/* the room door `d` leads to from `room` (-1: none, or closed off) (Rooms_DoorLeadsTo) */
int world_door_leads(const World *w, int d, int room);
/* is door `d` open: a doorway, or not locked and open (Progress_DoorOpen) */
int world_door_open(const World *w, int d);
/* a route from room `from` to `to` for a walker of `kind` (0: any; else the doors' one-way
 * flags count), through doors not locked or closed off, at most `max` doors (-1: no limit):
 * its doors into out[] (first first, up to nout), and their number; 0 already there; -1 none
 * (RoutePlanner_FindRoute with no doors to avoid, from no door, to any door) */
int world_route(const World *w, int from, int to, int kind, int max, int *out, int nout);
/* the same, not through the doors set in `avoid` (a bit a door: door d is bit d % 32 of
 * avoid[d / 32]; WORLD_DOORS bits) */
int world_route_avoiding(const World *w, int from, int to, int kind, int max, const uint32_t *avoid, int *out, int nout);

#endif
