# Design

## Goals

1. **Behave like the original.** Timings, distances, odds, what a character does when: these
   are the game. The decompilation (`src/`) is the specification, and each subsystem is
   checked against it (see *Checking against the original*).
2. **Read like a new program.** No raw offsets, vtable slots, PTMFs, "+0x16C8" fields or
   control flow kept only because the compiler laid it out that way. The names come from what
   things mean in the game. A comment may cite the original function a rule came from
   (`\ src: Character_Hearing`), and that is the only trace of the decompilation.
3. **Fail loudly and safely.** A mistake in a script prints an error naming the word and the
   actor; it never corrupts memory or crashes the engine.

## The split

| C | Forth |
|---|---|
| platform, input, timing, window | what input means |
| **assets**: loading, parsing and keeping every file (rooms, nav meshes, models, motions, textures, sounds, movies, the executable's tables) | asking for them by name or number (`room-load`, `figure-load`, `nav-tri-center`...) |
| rendering, skinning, posing figures | which figure plays which motion, where |
| geometry and world queries (floors, rays, nav triangles, the room graph and routes between rooms, door positions) | the rules that use them |
| **the actor kernel**: actors, mailboxes, delivery, scheduling, tracing | the actors: their state, behaviours and messages |
| the Forth VM and its tasks | game logic, scripts, tuning |

Assets and world data belong to C. Forth reads them through words and never lays out or
parses a file format. The game state that scripts own (story flags, door locks, inventory)
lives in actors.

## Actors

Everything with behaviour is an actor: Fiona, Hewie, each stalker, the doors, the camera,
the story, the music, the danger state, sound in the house. An actor has:

- **state**: named fields, private to it;
- **a behaviour**: a table of message handlers (`on heard ... ;`); it can `become` another
  (Debilitas searching, chasing, stunned);
- **a mailbox**: messages others `send` it.

Actors don't touch each other's state. They talk in messages, and the messages a subsystem
accepts and sends **are its API** (listed on its page in `docs/subsystems/`).

Two kinds of information flow are allowed:

- **Facts** may be read directly. These are C-owned world and asset data: where a nav triangle
  is, what the rooms connect to, the figure's current motion frame.
- **Changes** are messages. To open a door, hurt Fiona or start the music, you send a message
  to whoever owns it.

**Bodies.** Where a character physically is (its room, nav triangle, position, heading,
radius and height) is a fact too, kept in C as its *body*. Only the character moves its body,
but anyone may look: hearing, sight and distances all need it, the way a physics engine's
bodies are public. The same goes for **door states** (locked, open). They are kept in C because
routes through the house depend on them, but only the doors subsystem changes them.

Kernel details: `docs/actors.md`.

## A frame (30 a second, the original's rate)

```
input -> tasks -> [ tick ] -> deliver -> [ frame-end ] -> deliver -> figures advance -> draw
```

- `tick` goes to every actor subscribed to it: its frame's work (think, move, decide).
- **deliver**: the queued messages are handled in rounds. Messages sent during a round wait
  for the next round. It stops when the queue is empty, or after a limit (the rest carry over
  to the next frame and the kernel reports it).
- `frame-end` is for actors that collect during the frame and act on the whole of it (sound
  in the house decides who heard what).

Order is deterministic: rounds are first-in first-out, and broadcasts go out in actor-creation
order.

## Checking against the original

For each subsystem:

1. Read the original's functions and write the rules down on its page in `docs/subsystems/`,
   in game terms.
2. Build it as actors.
3. Test it: headless Forth tests with fixed inputs and expected outputs taken from the C. Later,
   traces from the decomp build (positions, states, messages per frame for a scripted scene)
   compared with ours.

## Conventions

- One subsystem per vocabulary (`IN: noise`), in its own file (`scripts/noise.fs`).
- Messages are declared once, in `scripts/messages.fs`, with their stack comments. A message
  name is a verb or an event: `hit`, `heard`, `open`, `entered-room`.
- Fields are declared with the actor's state (`state: ... end-state`) and named for what they
  mean. **Anything that lasts beyond one message is a field of the actor that owns it.** A
  module `variable` / `fvariable` is only scratch inside one word (Forth's locals). That's safe
  because handlers never interrupt one another. What another subsystem decides (the danger
  level, the panic) reaches an actor as a message, and it keeps its own copy in a field.
- **Names, not numbers.** Rooms are named (`scripts/room-names.fs`: `front-garden-2`, not
  `$13`). The names are placeholders (the pause map's area and the room's place on its page)
  until the rooms are walked and named for what they are. The same goes for doors, items,
  motions and the rest as their subsystems are built: a raw id appears only in a table that
  names it.
- Units: positions in the game's units; angles in radians inside, degrees only at the edges
  (data, scripts); time in frames (`30 = 1 second`).

## Flags by name

The game's state flags have names (`scripts/flag-names.fs`), from what the original's code does
with each: `world-held`, `hunted`, `no-running`, `hewie-commandable`... Never a flag by its
number, in code or in docs. A name marked "?" is only partly understood: rename it once it is.
The story flags are named the same way as they are worked out (`scripts/story-names.fs`).
`tools/flag_names.py` turns numbered flags in a file into names.
