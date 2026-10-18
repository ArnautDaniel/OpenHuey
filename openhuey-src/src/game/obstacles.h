/* The obstacles: the things in the played room that can be pushed about (the original's
 * gObstacles, src/game/room_map.c Obstacle_* / Obstacles_*). Up to 5, each over the grid of the
 * pushable floor: a square is a pair of nav triangles, an obstacle covers up to 3 parts (each
 * some squares from its reference square, so many squares across and deep - its kind's, from the
 * executable's ObstacleKinds). The triangles under it are blocked (0x20400000) and the ring of
 * squares round each part marked - its sides 0x20800000 (walls to the characters, and the
 * pushable floor: Fiona pushes from it), its corners 0x20000000. Pushed, it moves two squares
 * along the push over its move (OBSTACLE.MTN: 30 frames).
 *
 * This is their geometry and motion, a fact anyone may ask about; the obstacles actor
 * (scripts/obstacles.fs) places them, pushes them, plays their sounds and moves their models. */
#ifndef OBSTACLES_H
#define OBSTACLES_H

#include "../core/mathx.h"

#define OBSTACLES_MAX 5

void obstacles_reset(void);                 /* none (a new room) */
int obstacle_on(int i);
/* obstacle i placed: of `kind`, its reference square (a, b) - its parts added to any it has
 * (Obstacle_AddPart), its stop and any move kept. 0, or -1 for a kind the game has not */
int obstacle_make(int i, int kind, int a, int b);
void obstacle_stop(int i);
void obstacle_square(int i, int *a, int *b);   /* its reference square */
Vec3 obstacle_pos(int i);                    /* where it stands: its middle by its model's offset */
Vec3 obstacle_middle(int i);                 /* its middle (its sound's place) */
/* which obstacles have triangle `tri` on the sides of the ring round them, a bit each */
unsigned obstacles_ring_mask(int tri);
/* `tri` is obstacle i's square (the one it is moving to, on its move's last frame) */
int obstacle_on_square(int i, int tri);
/* can it be pushed along `dir`: not stopped, none of the characters on triangles `others`
 * on the ring round it one or two squares along, and the place two squares along walkable floor
 * (0x100) without 0x440080 */
int obstacle_can_push(int i, Vec3 dir, const int *others, int n);
/* pushed along `dir`: its target two squares along, the floor under and round it moved there,
 * its move started (Obstacles_SetTarget, Pull, Settle, StartMove) */
void obstacle_push(int i, Vec3 dir);
/* a frame of its move: 1 at the frame its scrape is heard */
int obstacle_step(int i);
void obstacle_moved(int i);                  /* the move done at once */

#endif
