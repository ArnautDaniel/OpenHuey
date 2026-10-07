# The actor kernel

C (`src/actors/`) keeps the actors, their mailboxes and the schedule, so delivery stays fast
and traceable. Forth defines everything else.

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

A handler runs with its message's arguments on the stack, and must consume them. A message
the behaviour (and its parents) doesn't handle is dropped, and the kernel counts it and reports
it once per behaviour and kind (`.unhandled`).

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

## C (`actors.h`)

```c
int  actors_spawn(const char *name, Behaviour *b, int state_type);
void actors_send(int from, int to, int kind, const Cell *args, int n);
void actors_broadcast(int from, int kind, const Cell *args, int n);
void actors_frame(Forth *f);
```

The engine sends some messages itself. Kinds below 16 are the kernel's: `tick`, `frame-end`,
`spawned`, `killed`. More come with later subsystems: `input`, `room-loaded`, `animation-event`.

## Rules

- A handler never waits. Long sequences (a cutscene, a scripted walk) are **tasks** an actor
  starts. The task sends messages and can wait for frames or for a reply message.
- Actors may read facts (C world and asset data) directly, but never another actor's state.
- Replies are messages: to ask an actor something, send it a message and handle its answer.
