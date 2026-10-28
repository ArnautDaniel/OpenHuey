/* OpenHuey's side of the actor kernel. The kernel is ClankForth's (external/clankforth/src/actors.h, its
 * docs/actors.md): actors with private state, a behaviour (message handlers written in Forth) and messages queued
 * between them. OpenHuey's part is each actor's body - where it is in the house - and these names for the
 * kernel's calls, as the engine's C makes them. ("HActor": Haunting actor - not `Actor`, an animated model in
 * game/actor.h.) Docs: docs/actors.md. */
#ifndef HACTOR_H
#define HACTOR_H

#include "actors.h"

#define HACTOR_MAX FORTH_ACTORS_MAX   /* actors alive at once */
#define HMSG_ARGS FORTH_MSG_ARGS      /* cells a message carries */

/* where an actor physically is: a fact anyone may read, changed only by the actor itself
 * (docs/design.md, "Bodies"). The engine moves bodies over the nav mesh (bind_engine.c). */
typedef struct HBody {
    int has, room, tri;
    float pos[3];           /* x y z: a vector the vec words take (body-at) */
    float yaw, radius, height;
    uint32_t mask;          /* the nav triangles it can't stand on */
    int has_head;           /* its head is told (body-head!): where others look at it */
    float head[3];          /* (the model's head bone; else its top: pos + height) */
    int has_point;          /* its point of interest this frame (body-point!; Character +0x6C: her blow, his bite) */
    float point[3];
} HBody;

/* actor `id`'s body (NULL: no such actor); the actor being run (-1: none) */
HBody *hactor_body(int id);
int hactor_self(void);
const char *hactor_name(int id);   /* its name, "" if none */
int hactor_kind(const char *name);   /* a message kind by name, -1 if there is none */

/* the kernel's words and the bodies' (vocabulary `actors`) */
void bind_hactor(Forth *f);
/* one frame of the actors (the engine calls it each tick) */
void hactor_frame(Forth *f);
/* a frame while the world is held (`pause!`: a menu up): `held-tick` to its subscribers only,
 * then deliver - messages still go to everyone */
void hactor_held_frame(Forth *f);
/* forget every actor and queued message (behaviours, kinds and state types stay) */
void hactor_reset(void);
/* from C: queue a message (from -1: the engine) / to every subscriber */
void hactor_send(int from, int to, int kind, const Cell *args, int n);
void hactor_broadcast(int from, int kind, const Cell *args, int n);

#endif
