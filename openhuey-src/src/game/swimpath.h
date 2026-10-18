/* Swim paths (src/game/rooms/room_61.c SwimPath_*): a fish's way round a box of water - a point at
 * random in each 10-unit cell of an nx x ny x nz grid, shuffled, and a smooth curve through them
 * one after another, `frames` a stretch. The old mansion's B1 pool (room $61) has three. */
#ifndef SWIMPATH_H
#define SWIMPATH_H

#include "../core/mathx.h"

#define SWIM_PATHS 4

/* path `i` set up anew from (ox, oy, oz); 0 if `i` or the grid is out of range */
int swimpath_setup(int i, int nx, int ny, int nz, int frames, Vec3 origin);
/* one frame along path `i`: `at` moved on the curve (kept in the box), `yaw` turned toward the
 * way it moved by at most 3.6 degrees (SwimPath_Step) */
void swimpath_step(int i, Vec3 *at, float *yaw);

#endif
