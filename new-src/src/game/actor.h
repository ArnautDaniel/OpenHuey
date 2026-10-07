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
    GpuMesh shadow;         /* the contact shadow: a soft blob on the floor under it */
    /* what scripts set (fields in Forth) */
    Vec3 pos;
    float yaw;              /* radians about y */
    float scale;            /* model units to room units */
    float frame;            /* the time in the motion, in frames */
    float rate;             /* motion frames a tick (1: the game ran its motions at its 30 fps) */
    int32_t motion;         /* index in the motion bank, -1 the rest pose */
    int32_t loop;           /* the motion repeats (else it holds its last frame) */
    int32_t visible;
    float shadow_size;      /* the contact shadow's radius (room units; 0: none) */
    Vec3 lo, hi;            /* the model's bounds (bind pose, model units) */
    int lights[3];          /* this frame: the room lights on it */
    /* a cutscene's motion file driving it (NULL: its own motion): its animation 0x8000 at
     * drive_frame, whose root bone is in room space (pos is then set from it) */
    const uint8_t *drive;
    size_t drive_size;
    float drive_frame;
} Actor;

/* load O_FIN/FIN_000 (.PCK + .TEX): the actor's number, or -1 */
int actor_load(Actor *actors, const char *name);
void actor_free(Actor *a);
/* a tick: the motion moves on */
void actor_tick(Actor *a);
/* pose and skin it for this frame (before shadows and drawing) */
void actor_prepare(Actor *a);
/* its middle and size in the room */
Vec3 actor_center(const Actor *a);
float actor_radius(const Actor *a);
/* draw it (lit by the lights set with render_draw_lights) */
void actor_draw(Actor *a, const Mat4 *view_proj);
/* the motion has played to its end (non-looping) */
int actor_motion_done(const Actor *a);

#endif
