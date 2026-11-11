# The actor kernel

The kernel is ClankForth's (the submodule `external/clankforth`): its page,
[external/clankforth/docs/actors.md](../external/clankforth/docs/actors.md), has messages, behaviours, state,
spawning and killing, handlers that wait (`wait`, `await`), monitors and links, failures, the schedule and the tools
(`.actors`, `.state`, `.recent`, `trace-on`). This page has what is OpenHuey's: the names, the bodies, the engine's
side.

**Names.** In C the kernel's actor is an `HActor` ("Haunting actor"). `Actor` (game/actor.h,
the Forth words `actor-load` and `act.*`) is something else: an animated model the renderer
draws. In the docs and scripts, "actor" means an HActor; an animated model is a "model".

## Bodies

Where a character physically is (docs/design.md, "Bodies"): a fact anyone may read, set only by the actor itself.
Each actor has one (`HBody`, src/hactor/hactor.h); the engine moves it over the nav mesh (`body-move` and the rest,
bind_engine.c).

```forth
room tri  x y z  body-place      \ ( room tri -- ) ( F: x y z -- ) in the house there
yaw body-turn   radius height body-size   body-off   \ (out of the house)
id body?  id body-room  id body-tri  id body-pos ( F: -- x y z )  id body-yaw  id body-dims ( F: -- radius height )
id body-at ( -- v )              \ its position as a vector (read it, don't write it)
x y z body-head!   id body-head ( -- v )        \ where its head is (else the top of its body)
x y z body-point!  body-point-off   id body-point ( -- v true | false )   \ its point of interest this frame
mask body-mask!  id body-mask    tri body-tri!   room body-room!
```

## C (`hactor.h`)

```c
void bind_hactor(Forth *f);                 /* the kernel (forth_actors) and the bodies' words (vocabulary `actors`) */
HBody *hactor_body(int id);                 /* NULL: no such actor */
int hactor_self(void);                      /* the actor being run, -1 none */
void hactor_frame(Forth *f);                /* one frame: the engine calls it each tick */
void hactor_held_frame(Forth *f);           /* the world held: `held-tick`, then deliver */
void hactor_send(int from, int to, int kind, const Cell *args, int n);
void hactor_broadcast(int from, int kind, const Cell *args, int n);
void hactor_reset(void);
```

They are the kernel's calls (`forth_actor_*`, actors.h) for the game's one Forth. The engine sends some messages
itself. The first kinds are the kernel's: `tick`, `frame-end`, `spawned`, `killed`, `held-tick`, `actor-failed`,
`start-over`, `died`. The subsystems' own are declared in `scripts/messages.fs` (`entered-room`, ...), each name
also its kind's constant.

**Ids and failures** (ClankForth 698c24e, its docs/hosts.md). An id is its actor's alone: the actor's slot (0 ..
255) plus 256 for each that had the slot before, so a kept id never names a newcomer. A table kept for each actor is
indexed by its slot - in C `forth_actor_slot(id)` (the camera's setups, camdirector.c, each with the id it was given
for: an old one's is none), in Forth `actor-slot` - and an id is checked with `hactor_body(id) != NULL` / `alive?`,
never against 256. A handler's error is said on the console and told to the errors actor, `failures` (debug.fs,
named first in game.fs with `errors-to`): the actor that failed lives on, as it always has here - with none named
the kernel has it die of it. The tasks' words are `start-task`, `start-held-task`, `stop-task` (the story's
activities, story/kit.fs); `spawn` and `kill` are the actors'.

Tests: `tests/test_actors.fs`, `tests/story/test_finishing.fs` (a failing handler in the game), and ClankForth's own (`ctest`).

## Rules

- A long sequence (a cutscene, a scripted walk) is a **task** an actor starts, as the scripts have it; a handler may
  also wait now (`await` a reply, `n wait`) - its actor is busy meanwhile, its messages kept for after.
- Actors may read facts (C world and asset data) directly, but never another actor's state.
- Replies are messages: to ask an actor something, send it a message and handle its answer (or `await` it).
