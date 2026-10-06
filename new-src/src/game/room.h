/* A room: its PAC file, its mesh and textures on the GPU, and which visibility groups show. */
#ifndef ROOM_H
#define ROOM_H

#include "../data/navmesh.h"
#include "../data/pac.h"
#include "../data/roommesh.h"
#include "../render/render.h"

#define ROOM_MAX_TEXTURES 256

typedef struct Room {
    int id;                 /* -1: none loaded */
    Pac pac;
    RoomMesh mesh;
    NavMesh nav;            /* where characters can stand (empty in some rooms) */
    GpuMesh gpu;
    GpuTexture textures[ROOM_MAX_TEXTURES];
    int ntextures;
    uint32_t groups[8];     /* bit g: group g shown (group 0 always is) */
    GpuMesh moving;         /* the batches that move with the view or animate, rebuilt each frame */
    MeshVertex *moving_v;
    MeshDraw *moving_d;
} Room;

int room_exists(int id);
/* load room `id` in place of the current one; 0 if it can't be read */
int room_load(Room *r, int id);
void room_free(Room *r);
/* a tick: the room's flip books step */
void room_tick(Room *r);
/* draw it as the eye sees it: solid and see-through parts, the moving batches, the glows, then
 * the bloom mask */
void room_draw(Room *r, const Mat4 *view_proj, Vec3 eye, Vec3 forward);
/* the highest solid surface at (x, z) below height y: 1 and its height in *out, or 0 if none */
int room_floor_below(const Room *r, float x, float y, float z, float *out);

#endif
