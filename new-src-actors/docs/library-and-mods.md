# The game library and mods

Two pieces that build on each other: **importing** the player's disc once into a library of
game data in friendlier formats, and **mods** layered over that library and over the scripts.
This is a plan; nothing here is built yet. See `design.md` for the split it follows: assets and
file formats belong to C, behaviour to actors in Forth.

## Where we start

- The data comes from the disc today: on the first run hga asks for the `.iso` (an SDL file
  dialog, `src/platform/disc.c`), remembers its path (`disc.txt`) and reads it in place - the
  image, its `DATA.CVM`, or a folder `DATA.CVM` was extracted to.
- Almost every asset is read through one door: `files_read` / `files_exist`
  (`src/core/files.c`). The exceptions are the executable (`disc_exe`: the tables hga reads out
  of SLUS_210.75) and the movies (streamed through a `DiscFile`).
- The disc is 4.5 GB, the game ~1.5 GB of it: movies 938 MB (135 SFD), rooms 286 MB (168 PAC),
  music 123 MB (72 ADX), models 112 MB (115 PCK), textures 23 MB (229 TEX), sound banks 16 MB
  (134 HD / BD / SDT), the rest small.
- Behaviour is Forth scripts loaded at run time from the scripts folder, in vocabularies
  (`IN:` / `USING:`), each subsystem actors passing messages.

## Decisions

- **The library is portable**: it lives beside the executable (`library/`), so a game folder
  can be moved or copied whole.
- **Converted assets are the only copy**: where a converter exists for a kind of file, the
  library holds the converted file and not the original. Kinds without a converter yet are
  copied as they are.
- **Mods may use the game's own words**: a script mod is Forth with the same vocabularies the
  game's scripts use. There is no sandbox (Forth has none) - single-player modding, as usual;
  the mod loader says so.
- **Mods respect the actors**: the preferred way to change behaviour is an actor of the mod's
  own that hears the game's broadcasts and sends the game's messages, not editing the game's
  actors' state.

## The library

```
<game folder>/
  hga(.exe)
  scripts/            the game's scripts (shipped)
  library/            made by the import
    manifest.fs       what it was made from and how
    exe/              the executable's tables, converted (see below)
    SYSTEM/ ...       the disc's folders, each file converted or as it was
  mods/               the player's mods
  user/               saves, the system data (the unlocks, best times), settings
```

`user/` follows the library's choice and moves with the folder. (Today these are in
`~/.local/share/hg2/new-src/`; the first run with a library offers to move them over.)

### The import

`hga --import <disc.iso>` from a command line, or on the first run with no library: the
existing picker, then a progress window.

1. **Check the disc**: the executable's hash says it is SLUS_210.75 (Haunting Ground, USA).
   Anything else is refused by name ("this is the PAL disc", "not Haunting Ground").
2. **Unpack and convert**, file by file, into `library/` - converted where the kind has a
   converter, copied otherwise. Written to `library.new/` and renamed when done, so a failed or
   cancelled import leaves no half library.
3. **Write the manifest**: the disc's hash, hga's converter version, each kind's format version,
   and every file made (its name and size) - enough to check the library at start-up and say
   what's wrong ("library/SYSTEM/TITLE.png is missing: import again").

At start-up the engine reads the library through the same `files_read` door; the disc code
remains the import's reader, and the way to run straight from a disc image (`--disc`) for
development.

**The catch with "converted only"**: once the originals are gone, a newer converter can't be run
on the library - improving a format means importing again from the disc. The manifest records
the converter versions, and a library made by an older hga is still read (each loader keeps
reading its format's older versions) and offered a re-import, not demanded one.

### Converted formats

Each kind is a converter (disc format to library format, C, in hga - the player has no other
tools) and a loader (library format to what the engine needs). A kind is converted only once
both are complete and checked: its output must give the engine the same data the original did
(a test per kind decodes both and compares), so converting changes nothing in play.

| Kind | On the disc | In the library | Why |
|---|---|---|---|
| textures | TEX: swizzled, paletted, PS2 alpha | PNG (RGBA, alpha as 0..255), a sidecar for what PNG can't say (the palettes a texture switches between, its CLUT rows) | edit with anything; a texture pack is PNGs |
| music | ADX (ADPCM, loop points) | Ogg Vorbis or Opus, its loop points in the file's tags | replace a track |
| sound banks | HD / BD / SDT (the SPU's programs and VAG samples) | a WAV or Ogg per sample, the bank as a text file (programs, tones, envelopes) | replace a sound |
| movies | SFD (MPEG-1 video, ADX audio) | WebM (VP9 or AV1, Opus) | smaller, decoded by the GPU or a common library, re-encodable at a higher size |
| the executable's tables | addresses in SLUS_210.75 | Forth data files (`library/exe/*.fs`) and binary blobs where a table is only ever read whole | no addresses at run time; mods can change a table |
| messages | the message files (BIN) | UTF-8 text files, one per table, ids kept | translation, new lines |
| rooms, nav meshes | PAC | later: glTF for the meshes, the rest (nav mesh, exits, cameras, areas) as data files | the biggest modding payoff, the most work |
| models, motions | PCK, MTN | later: glTF (skinned), motions as glTF animations | new costumes, new figures |

Order: **textures** first (smallest, most asked for, easy to check), then **music**, **messages**,
**the executable's tables**, **sound banks**, **movies**; rooms and models last. Until a kind's
turn its files are copied as they are and read by today's loaders.

Some formats carry engine behaviour in their layout (motion tables, a room's areas and
cameras): those convert into the engine's own structures written plainly, not into a general
format that loses them.

## Mods

A mod is a folder (or a `.zip` of one) in `mods/`:

```
mods/darker-castle/
  mod.fs          its manifest: name, version, what it needs, what it loads
  library/        files over the library's (same paths)
  scripts/        its Forth
```

`mod.fs`:

```forth
mod: darker-castle  1 0 version
requires: hga 2 0
after: better-textures        \ (load order: after this mod, if it's there)
loads: darker-castle.fs       \ (its scripts, in order)
```

`mods/order.fs` (written by the mods menu) lists the mods turned on, in order; a mod's
`after:` reorders, a missing `requires:` turns the mod off with a message naming it.

### Three layers

1. **Files.** `files_read` looks in each mod's `library/` (last in the order first), then in the
   library. A mod's texture is a PNG at the same path. The executable's tables, being data
   files in `library/exe/`, are overridden the same way. Nothing else is needed for texture
   packs, translations, music and sound replacements.
2. **Actors.** A mod's scripts are loaded after the game's, in its own vocabulary (`IN:
   mod.darker-castle`), and may `USING:` any of the game's. The way to change what happens is
   an actor of the mod's own: it subscribes to the game's broadcasts (`entered-room`,
   `fiona-doing`, `stalker-doing`, `hewie-doing`, `danger`, `panic` ...) and sends the game's
   messages (`do-action`, `go-to-room`, `fright`, `stalker-order` ...). It never pokes another
   actor's state. The game's messages are the mod API; `messages.fs` and the subsystem
   pages document them, and they become a promise - renamed only with a version bump.
3. **Hooks.** Where a mod needs to be asked, not told, the game offers named hooks: deferred
   words with a default, each documented as part of the API - for instance a room's entry
   (`room-entered-hook`), an item used (`item-use-hook`: the mod may claim it), the title's
   menu entries, the sub screen's pages, the clock of the mini-game. They are added as mods need
   them, not all at once. A mod sets a hook with `is` and calls what it replaced for the rest.

Replacing one of the game's script files outright (a mod's `scripts/story/rooms/castle-1f-4.fs`
over the game's) also works - the loader looks in the mods' `scripts/` first - but breaks with
every change to that file; the mods menu marks such mods "replaces game scripts".

### New content (later)

New rooms, items, costumes, figures and lines need tables a mod can add to, not just replace:
an item registry, the message tables, the room graph, the cast's kinds. Each comes with the
converted format for its kind (a room as files can be a new room) and an id space for mods (ids
from $1000 up, say, so they never collide with the game's).

## Order of work

1. **The library, plain**: `user/`, `library/` and `mods/` beside the executable; the import
   copying every file as it is, with the manifest and the checks; the engine reading
   `library/`; the first-run flow. No conversion yet - this alone ends reading from the disc.
2. **Textures to PNG** - the converter, the loader, the comparison test.
3. **Mods, files and actors**: the manifest, the load order, file overrides, mod scripts in
   their own vocabularies. A sample mod in the repository as a test.
4. **Music, messages, the executable's tables** converted; the first hooks, as the sample mods
   ask.
5. Sound banks, movies; then rooms and models.
