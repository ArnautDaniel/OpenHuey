/* The actor kernel: HActors ("Haunting actors" - not to be confused with `Actor`, an animated
 * model in game/actor.h) with private state, a behaviour (a table of message handlers written
 * in Forth) and messages queued between them. C keeps the actors, the queue and the schedule;
 * Forth declares messages, behaviours and state types and writes the handlers.
 *
 * A frame: `tick` to its subscribers, deliver, `frame-end` to its subscribers, deliver. Deliver
 * runs in rounds - what a round sends waits for the next - until the queue is empty (or a
 * limit: the rest waits for the next frame). Everything is first in, first out, and
 * broadcasts go in spawn order, so a frame always runs the same way.
 *
 * Docs: docs/actors.md. */
#ifndef HACTOR_H
#define HACTOR_H

#include "../forth/forth.h"

#define HACTOR_MAX 256      /* actors alive at once */
#define HMSG_ARGS 8         /* cells a message carries */
#define HKINDS_MAX 512      /* message kinds */
#define HNAME_MAX 31

/* the kernel's own message kinds */
enum { HK_TICK, HK_FRAME_END, HK_SPAWNED, HK_KILLED, HK_KERNEL_KINDS };

typedef struct HBehaviour HBehaviour;

/* the words (vocabulary `actors`) */
void bind_hactor(Forth *f);
/* one frame of the actors (the engine calls it each tick) */
void hactor_frame(Forth *f);
/* forget every actor and queued message (behaviours, kinds and state types stay) */
void hactor_reset(void);
/* from C: queue a message (from -1: the engine) / to every subscriber */
void hactor_send(int from, int to, int kind, const Cell *args, int n);
void hactor_broadcast(int from, int kind, const Cell *args, int n);

#endif
