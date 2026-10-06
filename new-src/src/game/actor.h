/* Actors: characters placed in the world - a model with its textures, a position, a heading, a
 * size, and the motion it is playing. Forth decides what they do (which motion, where they go);
 * C plays the motions and skins and draws them. */
#ifndef ACTOR_H
#define ACTOR_H

#include "../data/model.h"
#include "../render/render.h"

#define MAX_ACTORS 16
#define ACTOR_MAX_TEXTURES 64

typedef struct Actor {
    int used;
    char name[64];          /* the files' path without extension, e.g. O_FIN/FIN_000 */
    Model model;
    GpuTexture textures[ACTOR_MAX_TEXTURES];
    int ntextures;
    GpuMesh gpu;
    MeshVertex *posed;      /* this frame's skinned vertices */
    /* what scripts set (fields in Forth) */
    Vec3 pos;
    float yaw;              /* radians about y */
    float scale;            /* model units to room units */
    float frame;            /* the time in the motion, in frames */
    float rate;             /* frames a tick (the game's motions run at 30 a second: 0.5) */
    int32_t motion;         /* index in the motion bank, -1 the rest pose */
    int32_t loop;           /* the motion repeats (else it holds its last frame) */
    int32_t visible;
} Actor;

/* load O_FIN/FIN_000 (.PCK + .TEX): the actor's number, or -1 */
int actor_load(Actor *actors, const char *name);
void actor_free(Actor *a);
/* a tick: the motion moves on */
void actor_tick(Actor *a);
/* skin and draw */
void actor_draw(Actor *a, const Mat4 *view_proj);
/* the motion has played to its end (non-looping) */
int actor_motion_done(const Actor *a);

#endif
