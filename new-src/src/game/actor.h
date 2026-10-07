/* Actors: characters placed in the world - a model with its textures, a position, a heading, a
 * size, and the motion it is playing. Forth decides what they do (which motion, where they go);
 * C plays the motions and skins and draws them. */
#ifndef ACTOR_H
#define ACTOR_H

#include "../data/model.h"
#include "../render/render.h"
#include "doglegs.h"

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
    /* the motion player as the original's (src/game/model.c Motion_Start / Motion_Advance):
     * its flags - 1 loops, 2 keeps the phase of the one before, 0x10 no time of its own,
     * 0x20 wrapped this tick (the end reached), 0x40 frozen, 0x400 at its last frame - and a
     * cross-fade from the motion before over fade_len ticks */
    int32_t mflags;
    int32_t prev_motion;
    float prev_frame;
    float fade, fade_len;
    /* the body motion's variant (Motion_PlayTable's second animation, -1 none) at its own time,
     * the weight of the motion itself against it (+0x1C: 1 the motion alone, 0 the variant);
     * the same for the motion fading out */
    int32_t variant;
    float vframe, vweight;
    int32_t prev_variant;
    float prev_vframe, prev_vweight;
    int32_t prev_old_flags, prev_old_frames;   /* the motion before's flags, length and time as */
    float prev_old_frame;                      /* this one started (its variant's phase) */
    const uint8_t *table;   /* the model's motion table (6 bytes a motion, in bank order: its fade
                             * frames s16, its pose u8, -, its flags u16), NULL none */
    int ntable;
    int32_t visible;
    float shadow_size;      /* the contact shadow's radius (room units; 0: none) */
    Vec3 lo, hi;            /* the model's bounds (bind pose, model units) */
    int lights[3];          /* this frame: the room lights on it */
    /* a cutscene's motion file driving it (NULL: its own motion): its animation 0x8000 at
     * drive_frame, whose root bone is in room space (pos is then set from it) */
    const uint8_t *drive;
    size_t drive_size;
    float drive_frame;
    Vec3 bones[MODEL_MAX_BONES];   /* where each bone is, in the room (as last drawn) */
    /* the parts that play motions of their own (motion parts 2..4; the original's three blend
     * channels): a body motion with data for a part plays it there too, else the part goes back
     * to its own motion (a motion with no body: Hewie's ears, tail and jaw overlays) */
    struct ActorPart {
        int32_t motion, own, prev, flags, pending;
        float frame, prev_frame, fade, fade_len;
    } parts[3];
    /* bones turned where it looks (Hewie's neck: DogModel_AdjustBone) */
    ModelTurn turns[8];
    int nturns;
    DogLegs legs;           /* a dog's feet planted and its legs fitted to them (Hewie) */
    /* its motions' event keys (NAME.MRK: a byte of flags a frame for each motion, by the bank's
     * id list; the original's motion +0x4D4), NULL none */
    uint8_t *events;
    size_t events_size;
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
/* motion `index` from its start, cross-faded over `blend` ticks, with `flags` (see mflags): its
 * body, and its share of the parts (or theirs back to their own) */
void actor_motion_start(Actor *a, int index, int flags, float blend);
/* the body only */
void actor_body_start(Actor *a, int index, int flags, float blend);
/* the body motion just started gets variant `index` (-1 none), in step with the motion before
 * when both have flag 2 */
void actor_body_variant(Actor *a, int index);
/* its root movement this frame (model space): the motion and its variant by their weight, the
 * one fading out crossed in (Motion_RootMovement) */
void actor_root_delta(const Actor *a, float *turn, Vec3 *step);
/* the original's Motion_PlayWhenFree with the motion table's fade and flags: a body motion if it
 * isn't playing; a part's motion becomes the part's own, started unless the body motion has the
 * part too (then later) or the part is still fading (then once it is done) */
void actor_motion_when_free(Actor *a, int index);
/* its table entry: fade frames, pose, flags; 0 if it has none */
int actor_motion_entry(const Actor *a, int index, int *blend, int *pose, int *flags);
/* Motion_EventFlags: the event key byte of the body motion (layer 0) or its variant (1) at its
 * time + dt frames; `loop` 1: a looping motion's time wraps (else outside it: 0) */
int actor_event_flags(const Actor *a, int layer, int dt, int loop);
/* where bone `b` is in the room (as last posed), its position if there is no such bone */
Vec3 actor_bone(const Actor *a, int b);

#endif
