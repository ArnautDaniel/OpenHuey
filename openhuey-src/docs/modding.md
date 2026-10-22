# Modding OpenHuey

How to make a mod for OpenHuey (the Haunting Ground engine in this folder): what a mod can replace,
how its scripts plug into the game, and how to test one. Written so that someone new to the
project - or an AI assistant starting a fresh session - can pick it up from here. The design and
the reasons behind it are in `docs/library-and-mods.md`; the Forth language is `docs/forth.md`;
the actor kernel is `docs/actors.md`.

## In one minute

A mod is a folder (or a `.zip` of one) in `mods/` beside OpenHuey:

```
mods/my-mod/
  mod.fs                 the manifest: its name, version, what it needs
  library/data/...       files that replace the game's (same paths as the library's)
  scripts/my-mod.fs      its Forth: an actor of its own, a hook, words
```

Three samples to copy, in `examples/mods/` (in a release: `mod-examples/`):

| Sample | Shows |
|---|---|
| `room-tally` | an actor of its own: hears the game's broadcasts, keeps its own state, draws a line on screen, answers a message |
| `flower-scent` | a hook: changes what using an item does, keeping the game's rule for everything else |
| `free-play-entry` | a hook: adds an entry to the title menu |

Try one without installing it: `openhuey --mod examples/mods/room-tally`. Install one by copying its
folder into `mods/`; the title's MODS menu then turns it on or off and orders it.

## The manifest: mod.fs

OpenHuey reads it (it isn't run): a word per line, `\` and `( )` comments.

```forth
mod: my-mod              \ its name: letters, digits, - ; also its scripts' vocabulary
version: 1 0             \ major, minor
requires: openhuey 1 3        \ the mod API it was written for (below)
requires: other-mod 1 0  \ another mod it needs, at least this version
after: other-mod         \ load after that one, if it's on
```

- **The mod API** (`requires: openhuey M m`): the game's messages, broadcasts and hooks a mod uses.
  OpenHuey has 1.3 (`mod-api` at the console: 103). A mod needs the same major and a minor no newer.
  1.1 added `item-use-rule`, 1.2 `title-menu-hook`, 1.3 the mods' camera.
- A mod is **turned off** (and the reason shown in the MODS menu and on stderr) when: a
  `requires:` isn't met; its name is one of the game's vocabularies (`fiona`, `items` ...); its
  scripts fail to load. Nothing else stops the game.
- **Order**: `mods/order.txt` (`on: name` / `off: name` lines, written by the MODS menu). Mods it
  doesn't name are on, after those it names, by name. `after:` moves a mod below another. A later
  mod's files win over an earlier one's; hooks chain in load order.
- **A zip** works the same: `mods/my-mod.zip` with `mod.fs` at its top or inside one folder.
- **No sandbox**: a mod's scripts can do anything the game's can. OpenHuey says so when it loads one.

## Replacing the game's files (the files layer)

Every asset the engine reads goes through `files_read` (`src/core/files.c`), which looks in each
mod's `library/` first (the last mod in the order first), then in the library. So a mod replaces
a file by having one at the **same path**, under `library/data/` for the game's data (or
`library/` for the few files beside it, e.g. `library/exe/`). Folder names keep the game's case
(`SYSTEM`, not `system`); a file's own name may be any case (in a zip, case never matters).

The library holds the game's files converted to common formats (an import of the player's disc;
`docs/library-and-mods.md`). What a mod can replace, and in what form:

| What | Path (under `library/data/`) | Format |
|---|---|---|
| a texture | `<folder>/<NAME>.texbank/<i>.png` (texture i of bank NAME.TEX); a room's: `ST_xxx/ST_yyy.room/textures.texbank/<i>.png` (effects: `effects.texbank/`) | PNG, RGBA, any size up to 8192 a side: a bigger one (an HD pack: 2x, 4x) is drawn whole over the same surface. A 16-colour texture's other palettes are `<i>-<k>.png`, mapped in the bank's `bank.txt` (`src/data/texbank.h`) |
| a music track | `ADX00/AD_01.flac` (for `ADX00/AD_01.ADX`) | FLAC, 16-bit; loop with Vorbis comments `LOOPSTART` / `LOOPEND` (samples), or none: it loops end to start (`src/data/musicsrc.h`) |
| a sound effect | `<bank>.samples/<i>.wav` (sample i of the bank `<bank>.BD`; `bank.txt` lists them) | WAV, mono, 16-bit PCM; a loop as a `smpl` chunk; its pitch is the game's (the HD's), not the file's rate (`src/data/samples.h`) |
| text | `SUBSCR/MSG_BASE.txt`, `MSG_SUB.txt`, `MSG_END.txt`; a room's `ST_xxx/ST_yyy.room/messages.txt` | UTF-8 text: `#id` then the message's lines; the game's control codes as `{page}`, `{colour 5}` ...; buttons as `{cross}`, `{triangle}` ... (the full list: `src/data/msgtext.h`). A translation is these files |
| a movie | `X.mpg` + `X.flac` (for `X.SFD`) | an MPEG-1 / 2 program stream (video only) and its sound (none: silent) (`src/data/sfd.h`) |
| a room's mesh | `ST_xxx/ST_yyy.room/mesh.gltf` + `mesh.bin` | glTF 2.0 as OpenHuey writes it: the engine's draw words in `extras` "hga" (`src/data/roomgltf.h`) |
| a room's lights, cameras | `ST_xxx/ST_yyy.room/lights.txt`, `cameras.txt` | text, a record a line (`src/data/roomtext.h`) |
| a character model | `O_XXX/XXX_000.model/model.gltf` + `model.bin` | glTF 2.0, skinned, as OpenHuey writes it (`src/data/modelgltf.h`); the motions (`motions.bin`) stay the game's |
| one of the executable's tables | `library/exe/<name>.bin` (not under data/) | the table's bytes (`src/data/exetables.c` names them) |

**An HD texture pack made for PCSX2** (a `replacements/` folder of `.dds` / `.png` named by
PCSX2's hashes) becomes a mod with `tools/hdpack.py`: it matches the pack's pictures to the
library's textures by what they show, tile by tile (a PCSX2 picture is the PS2's memory at the
texture's place, so it can hold other textures in a room's unused parts), and writes the mod with
a report of what it matched. It needs Pillow and numpy:

```sh
python3 -m venv venv && venv/bin/pip install pillow numpy
venv/bin/python -I tools/hdpack.py <library> <pack>/SLUS-21075/replacements mods/hd-textures
```

`--max N` sets the largest a texture is written (default 2048 a side: 8x a room's 256).

The best start for any of these: import a disc (`openhuey --import disc.iso`), open the file in the
library, copy it into the mod at the same path, change it. For glTF, keep what Blender or another
tool round-trips: the `extras` and the accessor layout OpenHuey wrote; a mesh must still be one OpenHuey
reads (it says so on stderr if not).

What can't be replaced yet: the rooms' other data (navigation, events, doors, placed objects,
effects, obstacles: still inside each room's `.PAC`), the motion banks, the sound banks'
programs (`.HD` / `.SDT`).

## Scripts: an actor of your own

`scripts/<name>.fs` is loaded after the game's scripts, as the vocabulary `<name>`. Its folder is
also a root for `USING:`, so a bigger mod splits into `scripts/<name>/part.fs`
(`USING: <name>.part ;`).

```forth
\ scripts/my-mod.fs
IN: my-mod
USING: engine actors messages mods ;     \ the game's vocabularies it uses (see below)

state: my-state                        \ its own fields (an actor's state)
  cell field rooms-seen
end-state

behaviour my-behaviour
  on spawned ( -- )  self subscribe entered-room ;
  on entered-room ( room exit -- )  2drop  1 rooms-seen +! ;
end-behaviour

my-behaviour my-state s" my-mod" spawn constant me   \ spawned as the scripts load
```

The rules (they are the game's own: `docs/design.md`, `docs/actors.md`):

- **Everything is actors passing messages.** A mod's actor hears the game's broadcasts
  (`subscribe`) and sends the game's messages (`send`). It never writes another actor's state.
  To ask an actor something, send it a message and handle its answer (`room-tally` asks
  nothing; its test asks it, see below).
- **Messages are declared once** (`message name ( args -- )`), the stack comment is the
  declaration: `scripts/messages.fs` holds the game's, each with what it means. A mod declares its
  own the same way in its own file.
- **A handler takes exactly its message's arguments** off the stack and leaves nothing (the
  kernel reports one that doesn't, and puts the stacks back).
- **Facts** (the engine's C words: `room-id`, `body-pos`, the progress' flags `story-flag?`,
  `state-flag?` ...) may be read by anyone; they are `engine`, `facts`, `game-state`.
- **A handler never waits.** A sequence over time is a task (`spawn` a word; `yield`, `n wait`).

### The game's vocabularies a mod usually uses

| Vocabulary | Has |
|---|---|
| `engine` | the C words: rooms, actors' bodies, input (`key:`, `key-pressed?`), drawing (`ui-text`, `ui-rect`, `ui-clear`), sounds, files |
| `actors` | the kernel: `message`, `behaviour` / `on` / `end-behaviour`, `state:` / `field` / `end-state`, `spawn`, `send`, `broadcast`, `subscribe`, `self`, `sender`, `kill`, `actor-named`, `enter` / `leave-actor` |
| `messages` | every message of the game's (`scripts/messages.fs`) |
| `facts` | the progress' flags: `story-flag?`, `state-flag?`, `resident-flag?` |
| `flag-names`, `story-names`, `room-names` | names for flags, story flags, rooms (`front-garden-3` ...) |
| `keys` | the pad's buttons on the keyboard: `cross button-pressed?` ...; `stick@` |
| `mods` | what the game gives mods: `mod-layer` (the UI layer for mods) |
| `items`, `items.names` | the inventory's words and the items' names (`fresh-flowers` ...) |
| `game` | the actors' ids (`game:fiona`, `game:hewie`, `game:items` ...), `free-play` |
| `opening` | the title: `title-menu-hook`, `title-entry`, `title-gone` |

`vocab-words <name>` at the console lists a vocabulary; `.actors` lists the actors.

### Broadcasts worth hearing (subscribe to them)

| Message | Stack | When |
|---|---|---|
| `tick` | `--` | every game tick (30 a second), while play runs |
| `frame-end` | `--` | after each frame's messages: draw here |
| `held-tick` | `--` | instead of `tick` while the world is held (a menu up) |
| `entered-room` | `room exit --` | a room is in play (`leaving-room`, `arrived`: before it) |
| `danger` | `level --` | calm, followed, chased |
| `panic` | `stage level --` | Fiona's panic, each frame |
| `fiona-doing` | `mode sub cond cmd doing --` | what Fiona does, each frame |
| `hewie-doing` | `here action mode sub cond mood group commandable near reachable breakable way trust --` | Hewie, each frame |
| `stalker-doing` | `room mode sub cond hp hp-max kind alert came --` | the stalker in play, each frame |
| `noise-made` | `loud room door --` | Fiona's loudest noise this frame |
| `game-loaded` | `--` | a save was loaded: rebuild anything kept from the progress |

### Messages worth sending

| Message | To | Stack | Does |
|---|---|---|---|
| `go-to-room` | `s" rooms" actor-named` | `room exit --` | play that room (exit -1: no exit) |
| `give-item` / `take-item` | `game:items` | `id n --` / `id --` | into / out of the inventory |
| `remedy` | `game:fiona` | `fear recovery --` | her fear (0..100) and recovery changed by these |
| `fright` | the panic (`s" panic" actor-named`) | `amount --` | a fright (a float as a cell: `f>cell`) |
| `costume` | `game:fiona` | `n --` | her costume (0 her clothes, 1 the slip) |
| `do-action` | `game:hewie` | `action arg --` | Hewie's action |
| `stalker-order` | the stalker | `mode --` | 0 after her, 2 to her room, 3 searching |

All of them: `scripts/messages.fs`. Before sending one, read its handler (`grep -rn "on <name>"
scripts/`) to see what it really does.

### Drawing

UI layers are drawn in order over the picture; a mod draws on `mod-layer` (from `mods`): clear
it and draw each `frame-end` (as `room-tally` does). `ui-text ( layer addr len x y rgba scale -- )`
(the game's own font; bytes from `$C0` are the pad's buttons: `$C0` circle, `$C1` triangle,
`$C2` square, `$C3` cross, `$C4` L1, `$C6` R1), `ui-rect ( layer x y w h rgba -- )`,
`screen-size ( -- w h )`, `char-size ( -- w h )` at the pen's scale (`pen-scale ( n -- )`).

### The mods' camera (API 1.3)

A mod can take the camera: `mod-camera ( -- addr )` is its own camera (set its fields each tick:
`cam.x cam.y cam.z` the eye, `cam.yaw` - 0 looks along -z, positive turns right, as the
characters' yaw - `cam.pitch` positive up, `cam.fov` the vertical view in radians, `cam.near`),
`mod-camera! ( on -- )` gives it the view (from the camera as it is) or gives it back, and
`mod-camera? ( -- flag )` says whether it has it now: a cutscene's or an event's shot keeps the
game's camera and the mod's has it again after. Fiona walks by the camera, so she goes where the
mod's camera looks. `mouse-capture ( on -- )` holds the mouse in the window, its motion all look
(`mouse-dx`, `mouse-dy`: pixels this tick). The sample: `examples/mods/first-person` (through her
eyes - `body-head` her head, `body-yaw` her facing - the mouse or the arrows looking; F4 off).

## Hooks: when the game must ask

Where a mod needs to change what the game decides (not just react), the game offers a hook: a
deferred word with the game's own behaviour as its default. A mod keeps what it replaces, sets
its own, and calls the kept one for everything it doesn't handle - so several mods setting the
same hook chain in load order:

```forth
action-of item-use-rule constant game-rule         \ what was there (the game's, or an earlier mod's)
: my-rule ( place -- result )
    dup game-rule execute  dup no-use <> if  nip exit  then  drop   \ the game's answer first
    ( place ) ...  ;                                               \ then the mod's
' my-rule is item-use-rule
```

| Hook | In | Since | What |
|---|---|---|---|
| `item-use-rule ( place -- result )` | `items` | 1.1 | what using the item at `place` does (`id@` its item); the result as the game's rules': `no-use`, `used-up` (spends one), `marked`, `event-started`, `done-quietly` (something happened, nothing spent) |
| `title-menu-hook ( -- )` | `opening` | 1.2 | the title's entries from mods: call the kept one, then `title-entry ( addr len xt -- )` for each; picking one runs xt in the title's actor (`title-gone` first, to leave the title) |

New hooks are added when a mod needs one (and the mod API's minor goes up). Ask for one - or add
it: a `defer` in the subsystem's file with the game's word as its default, the call site going
through it, the table above and `MODS_API_MINOR` (`src/game/mods.h`) updated, and a sample.

## Testing a mod

OpenHuey runs Forth tests headless (`--test`): a test file is a vocabulary using `tester`, with
`T{ ... -> ... }T` checks, ending in `test-summary`.

```forth
\ tests/my-mod.fs
IN: test-my-mod
USING: tester engine actors messages room-names game my-mod ;

: frames ( n -- )  0 ?do  game-tick  loop ;
2 frames
testing the mod is on and its actor there
T{ s" my-mod" mod-on?  s" my-mod" actor-named 0< -> -1 0 }T
test-summary
```

```sh
SDL_AUDIO_DRIVER=dummy openhuey --mod path/to/my-mod --test tests/my-mod.fs
```

- In `--test` runs only `--mod` folders are on (never `mods/`), or the folder `--mods DIR` names.
- A headless run starts a new game at once (no title). `free-play` jumps past the opening
  (Hewie with her, the grate open); `<room> room!` (debug) moves her to a room.
- `game-tick` runs a tick; `deliver` delivers the messages queued now; `key: X true key-hold`
  holds a key (`cross`'s is X, `triangle`'s V; `scripts/keys.fs`).
- To read an actor's own fields in a test: `game:fiona enter  her-fear f@  leave-actor` (tests
  only: an actor never does this to another).
- To see it: `openhuey --mod my-mod --hidden --frames 3 --eval ": w 300 0 do game-tick loop ;  w  s\" /tmp/shot.png\" screenshot"`
  writes the frame after 300 ticks as a PNG. Use `OPENHUEY_HOME=/some/folder` for such runs: it keeps
  OpenHuey's saves and settings there instead of beside hga.
- The repository's own tests of the samples: `tests/mods/*.fs`, run by ctest
  (`CMakeLists.txt`: `mods_room_tally`, `mods_flower_scent`, `mods_free_play_entry`,
  `mods_menu`).

## Pitfalls (each one bit someone)

- **Your own words win over the game's.** A file's own definitions shadow every vocabulary it
  uses - the core too. A mod that defines `move` breaks the core `move` (copying bytes) for the
  rest of its file. Name things after what they mean in your mod (`reorder`, `shift-mod`).
- **Two used vocabularies with the same word**: in a file, the later in `USING:` wins; `vocab:word`
  reaches the other. At the console and in `--eval` the order is kept from line to line, and a
  vocabulary already in it keeps its place - say `vocab:word` there (`vectors` has a `pitch`,
  `game` a `camera`: `first-person:pitch`, `engine:camera`). If a word does something unexpected,
  check `see word` and which vocabulary has it.
- **Control words only inside a definition.** `if`, `?do`, `do`, `case` can't run at the top level
  of a file or at `--eval`: wrap them in a `: word ... ;`.
- **Locals**: the first `{: :}` of a definition can't be inside an `if` or a loop (later ones
  can); a local can't be named like an immediate word (`to`, `if`, `loop` ...).
- **`r@` inside a `do` loop is the loop's index**, not what you `>r`'d before it. Use a local.
- **Floats are on their own stack**: `f@ f! sf@ sf!`, `fdup`; a float in a message travels as a
  cell (`f>cell`, `cell>f`).
- **A message's arity is its declaration's stack comment.** A handler must take all of them.
- **Mods load after the game's scripts**: the game's actors already exist, and a mod spawns its
  own as its file loads; the first messages arrive at the first tick. A soft reset (the game
  over's return to the title) loads every script again, mods too.
- **Never write another actor's state** (its fields): send it a message. The game's rules depend
  on each actor being the only one changing its state.
- **Folder case** in `library/`: the game's (`SYSTEM/`, `ST_000/`); file names any case.
- **Exactness**: the game's data reads back exactly in the library's formats; a mod's replacement
  needn't, but keep the shape the reader expects (a WAV mono 16-bit, a texture the same shape (bigger is fine),
  message ids in order ...). OpenHuey says on stderr what it couldn't read.

## For an assistant starting here

- **Map**: `docs/subsystems.md` (which actor does what, in what order) and
  `docs/subsystems/<name>.md` (each one's messages, rules, status). `docs/forth.md` (the
  language), `docs/actors.md` (the kernel), `docs/library-and-mods.md` (the library and mods'
  design), this file.
- **Find a word**: `grep -rn ": word\b" scripts/`; a message: `grep -n "message name" scripts/messages.fs`
  and its handlers `grep -rn "on name" scripts/`; a C word: `src/forth/bind_engine.c` (the table
  at its end lists them by name).
- **The original game** is decompiled in `../src/` (C) - the reference for what the game does; the
  scripts cite its function names (`Panic_Breath`, `Items_Use` ...).
- **Build and check**: `cmake --build build`; `cd build && SDL_AUDIO_DRIVER=dummy ctest` (the
  tests need the game's data: the library, or the developer's extracted folder). Run any
  headless OpenHuey with `SDL_AUDIO_DRIVER=dummy` and `--hidden`.
- **The mods' code**: `src/game/mods.c` (finding, ordering, loading, the menu's list),
  `src/core/files.c` (the files layer), `src/core/zip.c`, `scripts/mods.fs`,
  `scripts/mods/menu.fs`; the hooks in `scripts/items.fs` and `scripts/opening.fs`.
