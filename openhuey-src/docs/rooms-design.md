# Rooms as a program, not as bytecode

A proposal for rewriting the room scripts. It covers what goes wrong today, the model to replace
it, castle-2f-3 written in that model, and how we keep the behaviour while migrating.

## What goes wrong today

The 170 files in `scripts/story/rooms/` (about 69,000 lines) were converted from the game's
event bytecode by `tools/events2forth.py` and then renamed. They still run the original
interpreter's model, only spelled in Forth:

- **Opcodes as words, numbers as arguments**: `$F $54 fade`, `0 fiona-who $23 action`,
  `2 room-motion 5 self-anim-blend8`, `$2A message`.
- **The interpreter's machinery**: action slots per character, per-slot frame counters,
  `['] act03 goto`, scripts dropped when a character's "scripted" flag clears, counters that
  one character's script bumps for another to wait on, `yield` loops.
- **Global numbered state**: story flags, state flags, event bits and script variables, set
  and tested from anywhere.
- **Words that copy an original function instead of saying what it means**: `door-bits` copied
  `Doors_SetBits`, so the room's own parts never drew. A stand-in golem was added to make up
  for it, and it became a second golem once the parts drew.

Most of the recent bugs follow from this. The attack tutorial locks when a slot is dropped
mid-animation. A skipped scene leaves Hewie half-placed. A hiding camera depends on an action
slot's camera setup. None of this is what the game means. It is how the PS2 interpreter
happened to keep its books.

## Goals

- **The same behaviour**: what is said, when, where characters go, the odds, the timings. The
  decomp stays the specification.
- **Rooms read as what happens in them**: a bed you can hide under, a vase you can break, a
  scene that plays once, a remark the first time you look at a portrait.
- **Lifecycles that can't leak**: whatever takes hold of a character always lets go of it,
  including when the scene is skipped, the room is left or the game is over.
- **One owner per piece of state**, with a name. No numbered flags in room files.
- **Each room tested by behaviour**, from the original's script.

Not a goal: matching the original's internal scheduling (slots, slot order, frame-exact yields)
except where a player can see the difference.

## The model

### The room actor

Each room is one actor, spawned while the room is played. Its state is the original's event
bits and variables, as named fields (`portrait-1-remarked`, `came-while-hidden`). Its behaviour
is the room's rules. It declares its **things** when it comes in. The room never pokes Fiona,
Hewie or a stalker: it sends them messages and gets replies.

### Story facts

Everything that outlives a visit (Hewie found in the wire, Hewie freed, the golem's plate
used) is a **fact**, kept by the `story` actor and saved with the game:

```forth
fact hewie-freed      \ castle-2f-3 act11: after the tutorial
fact found-in-wire    \ front-garden-3 act00
```

Rooms ask `hewie-freed fact?` and say `hewie-freed fact!`. The numbers stay inside `story`,
only for reading the original's saves and checking against it.

### Things a room is made of

Most rooms are built from a handful of concepts. Each concept is a component with its own
tests. The room declares it, and the component owns the prompts, zones, cameras and animations
it needs.

| Concept | What it owns | Replaces |
|---|---|---|
| `spot` | a zone, the prompt offered, what happens (a remark, a remark only the first time, an item taken, an item used) | `offer-action` / `prep-arg` / per-act remark bits |
| `part` | a named piece of the room's model or a door's model, shown or hidden | `N part-shown`, `door-bits` |
| `breakable` | its parts (whole, shards), its floor block, what drops out, the fact that it is broken | 3 blocks per vase in enter and in phase 2 |
| `hiding-place` | the way in and out, the camera inside, the stalker's search and its odds, getting caught | castle-2f-3 act02 / act03 / phase logic |
| `door` | (already the `doors` subsystem) the room only declares locks, keys and parts | `door-unlock`, `room-doors-state` |
| `ambience` | its sounds in turn, its effects (the fan, the TV, the specks) | sound-step-var loops, cmd words |
| `scene` | see below | `movie-play` / `cutscene-start` / `scene-wait-ready` / `scene-play` sequences |

A concept that appears in only one room stays in that room's file, written as plain actor
code. Nothing is forced into a component.

### Scenes

A scene is declared, not choreographed:

```forth
scene: hewie-saved            \ castle-2f-3 act11's second half
  movie 7 2   cutscene 6
  cast  hewie stalker
  after:  hewie joins          \ commandable, full health, at his spot
          stalker leaves       \ his action ended, out by door $3D
          hewie-freed fact!
          $207 item            \ "Additional Dog Commands"
;scene
```

One `scenes` actor plays them. It holds the cast for the whole scene and releases them at
the end. A character's end position comes from the cutscene's last key, or from the
declaration. Skipped or played through, the `after:` part runs exactly once. Nobody can be
left in a scene's animation, because nobody else ever starts one.

### Directing characters

Room code that moves a character uses a **hold**:

```forth
fiona hold
    14.5 -37.5 180 walk-to   \ replies when arrived
    slide-under-bed play
fiona release              \ also run if the room is left, a scene starts, she is caught
```

`hold` replaces `self-scripted`, action slots and `char-scripted`. While held, the character
ignores the player and its own moves. `release` always returns it to its own control,
standing. Each move replies when done, so a room waits on the move it started, not on a
counter another script bumps.

### Time

Each room task has its own timers (`240 frames-pass?`), not a slot frame counter reset by
whoever ran last.

## castle-2f-3 in this model

```forth
room: castle-2f-3          \ $21, Belli Castle 2F - the bedroom
  field came-while-hidden

  ambience  4 sounds every 30   fan   tv-screen
  spot portrait-1  zone 3  remark "I wonder whose portrait this is?"   once
  spot tv          zone 7  use tv-on/off                                \ act04/act05
  hiding-place bed
      in   from 14.5 -37.5 180   slide 5   lie 1
      out  slide 2   caught 3
      camera 3                 \ under the bed, looking out
      search  odds by progress var 2   ( will-find-her )
  part fireplace-glow 0

  on came-in  ( exit -- )
      $81 = found-in-wire fact? and hewie-freed fact? not and if  attack-tutorial  then ;
;room

: attack-tutorial ( -- )   \ act09 + act11
    hewie-trapped play-scene                      \ movie 5, cutscene 4
    fiona hold  -39.575 -27.807 90 place  lie play
    "Push up on the right analog stick to command the dog to attack." prompt
    240 frames-or attack-command wait
    fiona release
    hewie attack-stalker                          \ the stalker runs off by door 0
    hewie-saved play-scene ;
```

The real version will be longer, because every branch of the original is kept. But each line
says what happens, and nothing in it can leave Fiona stuck.

## Keeping the behaviour

- **The original's script is the spec.** For each room, `tools/` dumps the original's
  scripts readably (from the bytecode, never edited). The converted file moves to
  `scripts/story/rooms-ref/`, is no longer loaded, and is deleted once the room is verified.
- **Behaviour tests per room** (`tests/rooms/test_<room>.fs`), listed from the original's
  acts and branches: each prompt and its outcome, each scene and the state after it, each
  entry branch by fact.
- **Shared behaviour first**: crouching (shared act81/82), the save clocks, and the item and
  remark spots become components before any room uses them.

## Migration

1. Build the components (`spot`, `part`, `breakable`, `hiding-place`, `ambience`), the
   `scenes` actor and `hold` / `release` in Fiona, Hewie and the stalkers' kits, with their
   tests.
2. **castle-2f-3** as the first room, end to end: it has a hiding place, Hewie's tutorial,
   two scenes, the TV, remarks and ambience. You review it before we go further.
3. The rooms with open bugs: front-garden-3, castle-2f-8, castle-1f-4.
4. The shared pieces the remaining rooms need, as kit components with their tests (one PR):
   breakables (32 rooms), dial locks and plates (~15), mirrors and pushed obstacles (~10),
   scenes with the other stalkers as figures (Daniella, Riccardo, Lorenzo: ~20), a room's own
   variables (the script vars) as `own:` cells or facts, the stalker and summoner hooks. Each
   converted file moves to `scripts/story/rooms-ref/` (unloaded) once its place exists.
5. The rest by area, in story order, one PR per area (agreed 2026-10-26): the front garden and
   the lake pathway; castle 1F; castle 2F; castle 3F and B1; the old mansion 1F; its 2F, 3F
   and B1; the Chaos Forest; the water tower; the House of Truth; the off-map rooms - the
   Hewie-controlled clock rooms ($D0..$E9) and off-map-5a too. The old and new rooms coexist
   while it happens; a room file is either old or new, never mixed. Each room: every act,
   branch, remark, spot, scene and entry case of the original listed first, then the place,
   then `tests/places/test_<room>.fs`; each PR: the full suite, no "hides" warnings, the
   words' resolution unchanged where not meant to.
6. Once the last room is moved: the opcode words in `story/words.fs`, the slot machinery and
   `rooms-ref/` are deleted.

## Status

- Facts by name, saves by name (old saves still load): done.
- The kit, the `scenes` actor, `story` spawning places: done (docs/subsystems/story.md, "Places").
- castle-2f-3: done, every remark and spot, its scenes and story beats, the stalker's search,
  Hewie's find - `tests/places/test_castle_2f_3.fs`.
- front-garden-3, castle-2f-8, castle-1f-4: done; story/common.fs for what every room does.
- The shared pieces (step 4): done - the kit's components (story.md, "What most rooms have").
- The front garden and the lake pathway (step 5's first area): done, every room a place.
- Castle 1F (step 5): castle-1f-1, -4, -5, -6, -7, -9, -10, -11, -12, -14, -16, -17, -18, -19 done; the rest next.

## Questions (answered)

- Facts: names now (the saves write `fact <name>`).
- Coverage: every remark and examine spot of each room moved.
