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
    float shadow;           /* how far its shadows reach (+0x2C: Shadow_Draw's extrusion) */
} RoomLight;

#define ROOM_DOORS 8

/* a door of the room (src/game/doors.c: PAC section 7 places it - by exit - and section 8 holds
 * its model): where it stands, its turn (Euler angles; y about the vertical) at rest, and its
 * swing from rest in degrees (-90: open) easing to the wanted one */
typedef struct RoomDoor {
    int present;
    const uint8_t *sides;   /* its two sides' nav triangles (section 7 +0x28: a count and the
                             * triangles, then the other side's) - the passage a door blocks */
    RoomMesh mesh;
    GpuMesh gpu;
    Vec3 pos, rot;
    Vec3 stand;             /* where it stands (section 7 +0x10: the doors' +0x30) */
    int tri;                /* its nav triangle (+0x0) */
    float swing, target;
    /* an animation of a character using it (Doors_StartAnim: FIN_D000.MTN's record - the swing a
     * frame), its frame and frames; closing (+0x78: it was open) */
    const uint8_t *keys;
    int key, nkeys, closing;
    int mode;               /* how it swings (Door_Swing +0x60): 0 still, 1 along `keys`, 2 by 5
                             * degrees a frame, 3 slammed (15) */
    int sounded;            /* this swing's creak / latch played (+0x71) */
    /* what happened this tick (for the doors actor): the sound it made (0x27 the creak, 0x28 the
     * latch; 0 none) and how loud its noise is (Door_PlaySound: 1 / 2 quiet, 3 / 4 loud, else
     * none); `settled` 1 it came to rest open, 0 shut, -1 neither */
    int sound, sound_how, settled;
    uint32_t groups[8];     /* its model's parts shown: bit g, group g (0 always) - the scripts'
                             * door bits (Doors_SetBits: door +0x120) */
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
    RoomLight own_lights[ROOM_MAX_LIGHTS];   /* as the room file has them (Lights +0x9E0) */
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
size_t room_look_get(int slot, uint8_t *out);   /* its parameters as last set (0: none) */
void room_free(Room *r);
/* door `exit`'s swing (degrees from shut; -90 open): eased there, or set at once */
void room_door_swing(Room *r, int exit, float degrees, int at_once);
/* the doors' animations for the one who uses them (O_FIN/FIN_D000.MTN: 8 records - the user's
 * spot by the door x, z and turn, then the door's swing a frame); loaded once */
int room_door_anims(void);
/* Doors_AnimUserSpot: where animation `anim` puts its user at door `exit` (its point turned with
 * the door, reached on the mesh from the door's triangle - failing that without its x) and the
 * user's heading: the triangle, -1 none */
int room_door_user_spot(const Room *r, int exit, int anim, Vec3 *at, float *yaw);
/* Doors_StartAnim: the door swings along animation `anim` from this frame; 0 if it can't */
int room_door_anim_start(Room *r, int exit, int anim);
/* Doors_Side: 1 if p is in front of the door (along its turn at rest from where it stands), 0
 * behind, -1 no door */
int room_door_side(const Room *r, int exit, Vec3 p);
/* Doors_Passage: `flags` set (or cleared) on side `side`'s triangles of the door */
void room_door_side_flags(Room *r, int exit, int side, uint32_t flags, int set);
/* Doors_OnSide: nav triangle `tri` is in the door's side `side` (0 / 1) list */
int room_door_on_side(const Room *r, int exit, int side, int tri);
/* Doors_AtDoor: within 20 of where it stands (5 up or down) */
int room_door_near(const Room *r, int exit, Vec3 p);
/* Doors_InArea: inside its area `kind` (D_003E51A0: quads in the door's frame), 5 up or down */
int room_door_in_area(const Room *r, int exit, int kind, Vec3 p);
/* the door at an exit lets characters through or not (Doors_RoomIn / Doors_Refresh): shut, its
 * near side's triangles take the passage flags 0x60000 (open: the far side's, where the door
 * now stands); `locks` (0x1000000 / 0x4000000: locked to the stalkers' / Hewie's sides) on the
 * near side as well */
void room_door_passage(Room *r, int exit, int open, uint32_t locks);
/* door `exit` swung open or shut by 5 degrees a frame (Door_Swing 2), or slammed shut (3) */
void room_door_move(Room *r, int exit, int open, int slam);
/* door `exit` turned to `radians` (its whole turn, as a cutscene keys it: Doors_TurnTo) */
void room_door_angle(Room *r, int exit, float radians);
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
