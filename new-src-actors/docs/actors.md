# The actor kernel

C (`src/hactor/`) keeps the actors, their mailboxes and the schedule, so delivery stays fast
and traceable. Forth defines everything else.

**Names.** In C the kernel's actor is an `HActor` ("Haunting actor"). `Actor` (game/actor.h,
the Forth words `actor-load` and `act.*`) is something else: an animated model the renderer
draws. In the docs and scripts, "actor" means an HActor; an animated model is a "model".

Every actor has an **id**: a small number, given by `spawn`, that messages are addressed to.

## Forth words

### Messages

```forth
message heard ( loud room tri door source -- )   \ declares a message: a kind and its arity
```

A message has a kind (a small number) and up to 8 cell arguments. A float travels as a cell
(`f>cell` / `cell>f`). `message` takes the arity from the stack comment, so the comment is
the declaration.

```forth
loud room tri door src  listener send heard      \ queue it for `listener`
tick-args  broadcast tick                         \ to everyone subscribed to `tick`
```

`send` and `broadcast` parse the message name that follows them, so a misspelt message is an
error when the script is compiled, not when it runs.

### Behaviours

```forth
behaviour listening
  on heard ( loud room tri door src -- )  ... ;
  on tick ( -- )  ... ;
end-behaviour

behaviour stunned  extends listening   \ unhandled messages fall through to `listening`
  on tick ( -- )  ... ;
end-behaviour
```

A handler runs with its message's arguments on the stack, and must consume them. If it
doesn't, or it fails, the kernel names the actor and handler, and puts the stacks (data,
return, float) back as they were. A message the behaviour (and its parents) doesn't handle is
dropped, and the kernel counts it and reports it once per behaviour and kind (`.unhandled`).
The kernel's own messages (`tick`, `frame-end`, `spawned`, `killed`) are dropped quietly.

`become ( behaviour -- )` switches the current actor to another behaviour from the next
message on.

### State

```forth
state: listener-state
  cell  field threshold      \ quieter than this, it doesn't hear
  cell  field heard-loud
end-state
```

A field word gives the field's address in the **current** actor's state. The current actor is
the one whose message is being handled (`self`). Using a field with no current actor, or with
an actor whose state is of another type, is an error.

### Actors

```forth
listening listener-state  spawn" Debilitas"  ( -- id )
id subscribe tick          \ (or inside a handler: self subscribe tick)
id kill
self  ( -- id )            \ the actor being run
sender  ( -- id )          \ who sent the message being handled (-1: the kernel)
```

### The schedule and tools

- `actors-frame ( -- )`: one frame (tick, deliver, frame-end, deliver). The engine calls it
  each tick, and tests call it directly.
- `deliver ( -- )`: deliver what is queued now (tests).
- `trace-on` / `trace-off`: print every message as it is delivered (`frame  from -> to  kind
  args`).
- `.actors`: every actor with its behaviour, its state type and its mailbox count.
- `enter ( id -- )` / `leave-actor`: the console inside an actor. (Not `leave`: that is the
  loop word.) Its fields, `self`, `become`
  and sends are that actor's, so `fiona enter  fear f@ f.` reads Fiona's own fear. (Later the
  console is an actor itself, and `enter` opens that actor's REPL.)

## C (`hactor.h`)

```c
void bind_hactor(Forth *f);                 /* the words (vocabulary `actors`) */
void hactor_frame(Forth *f);                /* one frame: the engine calls it each tick */
void hactor_send(int from, int to, int kind, const Cell *args, int n);
void hactor_broadcast(int from, int kind, const Cell *args, int n);
void hactor_reset(void);
```

The engine sends some messages itself. The first kinds are the kernel's: `tick`, `frame-end`,
`spawned`, `killed`. More come with later subsystems: `input`, `room-loaded`,
`animation-event`.

Tests: `tests/test_actors.fs` (`ctest`).

## Rules

- A handler never waits. Long sequences (a cutscene, a scripted walk) are **tasks** an actor
  starts. The task sends messages and can wait for frames or for a reply message.
- Actors may read facts (C world and asset data) directly, but never another actor's state.
- Replies are messages: to ask an actor something, send it a message and handle its answer.
