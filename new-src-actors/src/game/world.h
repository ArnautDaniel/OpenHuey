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
/* where exit `exit` of `room` leads: the room (and its exit in *to_exit), or -1 */
int world_exit_leads(const World *w, int room, int exit, int *to_exit);
/* exit's nav triangle (which: 0 out, 1 in, 2 through), the door's own when the room table has
 * none; -1 if none */
int world_exit_tri(const World *w, int room, int exit, int which);

#endif
