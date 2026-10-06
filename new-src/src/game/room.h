/* A room: its PAC file, its mesh and textures on the GPU, and which visibility groups show. */
#ifndef ROOM_H
#define ROOM_H

#include "../data/pac.h"
#include "../data/roommesh.h"
#include "../render/render.h"

#define ROOM_MAX_TEXTURES 256

typedef struct Room {
    int id;                 /* -1: none loaded */
    Pac pac;
    RoomMesh mesh;
    GpuMesh gpu;
    GpuTexture textures[ROOM_MAX_TEXTURES];
    int ntextures;
    uint32_t groups[8];     /* bit g: group g shown (group 0 always is) */
} Room;

int room_exists(int id);
/* load room `id` in place of the current one; 0 if it can't be read */
int room_load(Room *r, int id);
void room_free(Room *r);
void room_draw(const Room *r, const Mat4 *view_proj);

#endif
