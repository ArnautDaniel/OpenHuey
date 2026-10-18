/* Things lying about (the original's placed things, gPlacedThings: src/game/placed.c): what
 * Fiona throws, drops or sets down - the ball, the powders, the magnesias, the doll, the stone -
 * and the story's own. Up to 128, each a kind (0..10) in a room, on a nav triangle: flying under
 * gravity, bouncing off walls (or bursting, the powders), rolling and slowing on the floor,
 * dropping through holes. This is their motion and their drawing; what they do (going off, the
 * stalkers and creatures they reach, Hewie fetching the ball) is the things actor's
 * (scripts/things.fs).
 *
 * Kinds: 0 the ball (a sprite), 1 / 4 / 6 the powders (sprites; they burst on whatever they hit),
 * 2 / 5 / 7 the magnesias (GAME_FIX.GFM model 1, palettes 0..2), 3 the doll (model 2, lying flat),
 * 8 the stone (model 3), 9 a room object kept at it, 10 unseen. */
#ifndef THINGS_H
#define THINGS_H

#include "../core/mathx.h"

#define THINGS_MAX 128

enum { THING_FLYING, THING_ROLLING, THING_DROPPING };

/* what happened to it in the last step (things_events: cleared each step) */
enum {
    THING_BURST = 1,       /* a powder broke on what it hit (at its place) */
    THING_BOUNCED = 2,     /* it bounced (its sound) */
    THING_GONE = 4,        /* it fell through a hole and is gone */
    THING_PUFF = 8         /* (the doll) it glanced off a wall and dropped (a puff of dust) */
};

/* a new thing: kind, room, triangle (-1: under the point), at `at` with velocity `v` (flying);
 * its index, -1 none free */
int things_new(int kind, int room, int tri, Vec3 at, Vec3 v);
void things_free(int i);
void things_clear(void);
int things_used(int i);
int things_spent(int i);           /* burst or gone this step (freed at the next) */
int things_kind(int i);
int things_room(int i);
int things_tri(int i);
int things_state(int i);
int things_age(int i);
void things_set_age(int i, int age);
int things_turn(int i);            /* its turn (0..255 of a full turn) */
Vec3 *things_at(int i);            /* its place (written by its owner only through things_place) */
Vec3 *things_v(int i);             /* its velocity */
void things_place(int i, int room, int tri, Vec3 at);   /* (a loaded save: lying there, still) */
void things_kick(int i, Vec3 dv);  /* (Fiona's kick) its velocity added, a little off the floor, flying */
void things_fade(int i);           /* fading out (a sixth a frame), then gone */
int things_events(int i);
/* a step of the things in `room` (the others still); `running` (not in a scene, the world going):
 * all of them a frame older */
void things_step(int room, int running);
/* drawn: those in `room` (the models; the sprites as they're written) */
void things_draw(int room, const Mat4 *view_proj);

#endif
