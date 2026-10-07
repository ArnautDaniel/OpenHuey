/* A room: its PAC file, its mesh and textures on the GPU, and which visibility groups show. */
#ifndef ROOM_H
#define ROOM_H

#include "../data/navmesh.h"
#include "../data/pac.h"
#include "../data/roommesh.h"
#include "../render/render.h"
#include "placed.h"

#define ROOM_MAX_TEXTURES 256
#define ROOM_MAX_LIGHTS 16

/* a room light (PAC section 4: 12 floats each - position, colour 0..128, intensity, range;
 * range 0 reaches everywhere: the moon) */
typedef struct RoomLight {
    Vec3 pos;
    Vec3 color;
    float intensity;
    float range;
} RoomLight;

#define ROOM_DOORS 8

/* a door of the room (src/game/doors.c: PAC section 7 places it - by exit - and section 8 holds
 * its model): where it stands, its turn (Euler angles; y about the vertical) at rest, and its
 * swing from rest in degrees (-90: open) easing to the wanted one */
typedef struct RoomDoor {
    int present;
    RoomMesh mesh;
    GpuMesh gpu;
    Vec3 pos, rot;
    float swing, target;
} RoomDoor;

typedef struct Room {
    int id;                 /* -1: none loaded */
    Pac pac;
    RoomMesh mesh;
    NavMesh nav;            /* where characters can stand (empty in some rooms) */
    GpuMesh gpu;
    GpuTexture textures[ROOM_MAX_TEXTURES];
    int ntextures;
    uint32_t groups[8];     /* bit g: group g shown (group 0 always is) */
    RoomDoor doors[ROOM_DOORS];   /* by exit */
    PlacedSet placed;       /* the objects the scripts show and move (sections 12, 15) */
    Vec3 ambient;           /* (0..128) */
    RoomLight lights[ROOM_MAX_LIGHTS];
    int nlights;
    GpuMesh moving;         /* the batches that move with the view or animate, rebuilt each frame */
    MeshVertex *moving_v;
    MeshDraw *moving_d;
} Room;

int room_exists(int id);
/* load room `id` in place of the current one; 0 if it can't be read */
int room_load(Room *r, int id);
/* the room's own look again, as its file has it (after scripts changed it) */
void room_reset_look(Room *r);
/* one effect of the room's look from its parameters, as the scripts send them: slot 0x1C depth
 * of field, 0x1D fog, 0x1E screen blend, 0x1F tint (NULL: that effect removed) */
void room_look_set(int slot, const uint8_t *d, size_t n);
void room_free(Room *r);
/* door `exit`'s swing (degrees from shut; -90 open): eased there, or set at once */
void room_door_swing(Room *r, int exit, float degrees, int at_once);
/* a tick: the room's flip books step, its doors swing */
void room_tick(Room *r);
/* draw it as the eye sees it: solid and see-through parts, the moving batches, the glows, then
 * the bloom mask */
void room_draw(Room *r, const Mat4 *view_proj, Vec3 eye, Vec3 forward);
/* the highest solid surface at (x, z) below height y: 1 and its height in *out, or 0 if none */
/* the up to 3 lights brightest at a point (those reaching its nav triangle: the game's
 * Lights_Brightest): their indices, -1 for none; how many */
int room_lights_at(const Room *r, Vec3 pos, int out[3]);
int room_floor_below(const Room *r, float x, float y, float z, float *out);

#endif
