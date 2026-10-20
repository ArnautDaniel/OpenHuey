# The game library and mods

Two pieces that build on each other: **importing** the player's disc once into a library of
game data in friendlier formats, and **mods** layered over that library and over the scripts.
This is the plan; steps 1 (the plain library), 2 (textures to PNG) and 3 (mods: files and actors) are built. See `design.md` for the split it follows: assets and
file formats belong to C, behaviour to actors in Forth.

## Where we start

- The data comes from the disc today: on the first run OpenHuey asks for the `.iso` (an SDL file
  dialog, `src/platform/disc.c`), remembers its path (`disc.txt`) and reads it in place - the
  image, its `DATA.CVM`, or a folder `DATA.CVM` was extracted to.
- Almost every asset is read through one door: `files_read` / `files_exist`
  (`src/core/files.c`). The exceptions are the executable (`disc_exe`: the tables OpenHuey reads out
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
  OpenHuey(.exe)
  scripts/            the game's scripts (shipped)
  library/            made by the import
    manifest.txt      what it was made from and how
    SYSTEM.CNF ...    the disc's own files (SYSTEM.CNF, MODULES/)
    data/             DATA.CVM's folders, each file converted or as it was
    exe/              the executable's tables, a file each (the executable itself not kept)
  mods/               the player's mods
  user/               saves, the system data (the unlocks, best times), settings
```

`user/` follows the library's choice and moves with the folder. The first time it is made,
the saves and settings OpenHuey kept in `~/.local/share/hg2/new-src/` are copied into it (once: a
`.moved` mark). Where the executable's folder can't be written (an installed copy) both go to
the user's folder for programs' data instead; `OPENHUEY_HOME` names another folder (tests, tools).

### The import

`openhuey --import <disc.iso>` from a command line, or on the first run with no library: the
existing picker, then a progress window (the file at hand and how much is done; the import runs
on a thread of its own, so the window keeps answering while a big file is converted, and closing
it cancels the import).

1. **Check the disc**: the executable's hash says it is SLUS_210.75 (Haunting Ground, USA).
   Anything else is refused by name ("this is the PAL disc", "not Haunting Ground").
2. **Unpack and convert**, file by file, into `library/` - converted where the kind has a
   converter, copied otherwise. Written to `library.new/` and renamed when done, so a failed or
   cancelled import leaves no half library.
3. **Write the manifest** (plain text, read by the engine: `hga-library 1`, `disc <crc>`,
   `convert <kind> <format>` for each converter used, then `file <size> <path>` for each file
   written): enough to
   check the library at start-up and say what's wrong ("the library's data/SYSTEM/TITLE.TEX is
   missing or damaged" - and with a window, the player is asked for the disc again).

At start-up the engine reads the library through the same `files_read` door (the disc layer's
folder form: `library/data/`, its `../` the library). The disc code remains the import's reader,
and naming a disc image or an extracted folder on the command line still reads it as it is
(development; the tests use the developer's extracted folder when there is no library).

**The catch with "converted only"**: once the originals are gone, a newer converter can't be run
on the library - improving a format means importing again from the disc. The manifest records
the converter versions, and a library made by an older OpenHuey is still read (each loader keeps
reading its format's older versions) and offered a re-import, not demanded one.

### Converted formats

Each kind is a converter (disc format to library format, C, in OpenHuey - the player has no other
tools) and a loader (library format to what the engine needs). A kind is converted only once
both are complete and checked: its output must give the engine the same data the original did
(a test per kind decodes both and compares), so converting changes nothing in play.

| Kind | On the disc | In the library | Why |
|---|---|---|---|
| textures | TEX: swizzled, paletted, PS2 alpha | PNG (RGBA, alpha as 0..255), a sidecar for what PNG can't say (the palettes a texture switches between, its CLUT rows) | edit with anything; a texture pack is PNGs |
| music | ADX (ADPCM, loop points) | FLAC (lossless: the samples the game plays), its loop points in the file's tags | replace a track |
| sound banks | HD / BD / SDT (the SPU's programs and VAG samples) | a WAV per sample (built); the bank as a text file (programs, tones, envelopes: later - HD / SDT copied for now) | replace a sound |
| movies | SFD (MPEG video, ADX audio) | `.mpg` (the same video packets) and `.flac` (the sound) - lossless, chosen over re-encoding to WebM | a plain MPEG file any player opens; a mod's movie any MPEG-1 / 2 video |
| the executable's tables | addresses in SLUS_210.75 | a file each, by name (`library/exe/doors.bin`): the table's bytes; readable forms (data files) as mods want them | no addresses at run time; mods can change a table |
| messages | the message files (BIN), each room's text (a PAC section) | UTF-8 text files, one per table, ids kept | translation, new lines |
| rooms | PAC | a folder: the mesh as glTF, the textures as PNG, the text, lights and cameras as text (built); the rest of the PAC (nav mesh, events, doors, placed objects, effects, obstacles) as it is for now | the biggest modding payoff, the most work |
| models, motions | PCK, MTN | glTF, skinned (built); the motion bank and .MTN files as they are (glTF's animations can't play the game's keys exactly) | new costumes, new figures |

Order: **textures** first (smallest, most asked for, easy to check), then **music**, **messages**,
**the executable's tables**, **sound banks**, **movies**; rooms and models last. Until a kind's
turn its files are copied as they are and read by today's loaders.

### Textures

Each `.TEX` bank of DATA.CVM becomes a folder beside where it was, the bank's name with
`.texbank` for `.TEX` (`SYSTEM/TITLE.TEX` -> `SYSTEM/TITLE.texbank/`):

```
bank.txt     hga-texbank 1
             textures 2
             texture 0 512 512 19 1          (index, width, height, the PS2 pixel format, images)
             texture 1 512 256 20 5
             palettes 1 0 1 2 3 4 4 4 ...    (a 16-colour texture: the image for each of its 32 palettes)
0.png        texture 0
1.png 1-1.png 1-2.png 1-3.png 1-4.png        texture 1: one PNG per palette that looks different
```

The PNGs are the pixels the game decodes (RGBA, the PS2's alpha 0..128 as 0..255), so a texture
pack edits them with anything and keeps the same names. A PNG of another size is drawn as much of
it as fits (a mod's higher-resolution textures need the renderer to know the size: later).
`src/data/texbank.c` reads either form (the engine still meets raw banks: a room's `.PAC` holds
its own, read as they are until the rooms are converted, and a library imported before the
conversion keeps its `.TEX` files). `texbank-check` (a ctest) converts every bank of the
developer's data and compares each texture in each palette with the bank's own decoding.

### Music

Each `.ADX` track becomes a FLAC beside where it was (`ADX00/AD_01.ADX` -> `ADX00/AD_01.flac`):
16-bit, the ADX's rate and channels, written by OpenHuey's own encoder (`src/data/flacenc.c`: fixed
predictors, Rice-coded, the best stereo coding each 4096 samples; within 2% of `flac -5`; a
seek table, so a loop's jump is quick). FLAC because the conversion must change nothing: a lossy
format couldn't give the same samples back. 117 MB of ADX become 205 MB of FLAC.

Tags (Vorbis comments): `LOOPSTART` / `LOOPEND` (samples: after LOOPEND - 1 comes LOOPSTART) and
`STOPAT` (where it ends played once, not looped). A track with no tags loops end to start.

The catch, and why the FLAC is a little longer than the track: the player goes back by whole
ADX blocks, and ADX's prediction carries on across the jump - so the loop doesn't start the
second time as it did the first (differing for up to 3862 samples, by up to 11473: a click the
original makes too). Passes after the first all start alike. So the FLAC holds the first pass,
then the second pass's start for as long as it differs, and its LOOPSTART is that much into the
loop: every pass the same samples as the ADX's, through `src/data/musicsrc.c`, which the player
reads both with. A mod's track needs none of this - LOOPSTART and LOOPEND, or nothing.

`music-check` (a ctest) converts every track of the developer's data and plays both: once
through, and looped three passes - the same samples (63129 of second-pass starts kept in all).

### Messages

The message tables become text: `SUBSCR/MSG_BASE.BIN` -> `SUBSCR/MSG_BASE.txt` (and MSG_SUB,
MSG_END), and each room's own text (section 10 of its `.PAC`) -> `ST_000/ST_003.room/messages.txt`
(the room's folder: see Rooms). 9520 messages in 171 tables, 211 KB.

```
\ ST_000/ST_002.PAC: the room's text (section 10)
#0
Looks like this is used to
open the gate.
{page}Pull the lever?{choice 0 0}
#1
The lever has already been pulled.

```

`#id` then the message's lines: a line break in the file is the game's, and blank lines count
(message 1 above ends with one). The text is UTF-8 as the game's font draws it - ASCII, "®",
the accented letters ("risqué") - its buttons and symbols as `{cross}`, `{start}`, `{sun}`...,
the control codes as `{page}`, `{colour 5}`...`{/colour}`, `{option $4012}`...; any other byte
`{$xx}`. The whole list is in `src/data/msgtext.h`; a character the font hasn't, or a word it
doesn't know, is said with the file and line. Read back, a file becomes a table laid out as the
game's, so the game's code reads it unchanged. A translation is these files in a mod.

`messages-check` (a ctest) writes every table as text and reads it back: every message the same
bytes, measured by its own copy of the game's reader (the decomp's `TextCursor_Step`) - which is
how this was found to need a choice's two operand bytes (`include/text.h` said one).

### The executable's tables

The engine and the scripts read 22 tables out of SLUS_210.75: the doors and rooms (how the house
connects), the obstacle kinds, the sounds' distance curves and stereo pan, the butterflies, the
dealt maze rows, Fiona's meeting offsets and twelve models' motion tables (their fades and flags).
`src/data/exetables.c` names each with its address and size; the import writes each to
`library/exe/<name>.bin` and keeps no executable. The engine asks by name -
`exetable("doors")`, `s" motions-hewie" motion-table`, `s" deal-rows" exe-table` - reading
`../exe/<name>.bin` through `files_read` (so a mod's over the library's), else (no library: the
developers') the executable at the table's address.

They are the game's bytes as they are, but for one: the obstacle kinds are pointers to each
kind's definition, so the file holds the definitions one after another instead. A motion table's
size is its model's motion count - the most any model using it has (measured over the tests);
a model with more gets no table entries past the file's end. Readable forms (text a mod edits)
come table by table as mods want them; the rooms and doors will go with the rooms' conversion.

Checked: ctest from a library holding no executable (strace: none opened) - every subsystem's
tests pass, the tables from `exe/`.

### Sound banks

A bank is NAME.HD (Sony's: programs, splits, samples, the samples' places), NAME.SDT (Capcom's
sound table) and NAME.BD (the samples, PS-ADPCM). The BD becomes a folder of WAVs -
`C_0000.BD` -> `C_0000.samples/`: `bank.txt` (each sample's index, its place in the BD, its rate)
and `<i>.wav` (mono, 16-bit, at the HD's rate; a loop as a `smpl` chunk) for each of the HD's
samples (its Vagi table). 134 banks, 1148 samples: 16 MB of ADPCM, 56 MB of WAV. The HD and SDT
are copied as they are for now: a text form of them wants their fields named one by one, and the
engine reads them in place (`snddrv.c`, `seq.c`) - next, as mods want to change a sound's
envelope or pitch rather than its sound.

The SPU's voices play either: `src/data/vag.h`'s cursor (the PS-ADPCM decoder moved out of
`spu.c`) reads a sample's ADPCM, or its WAV's samples, a 28-sample block at a time. As with the
music, ADPCM's prediction carries across a loop's jump, so a WAV holds the first pass and the
second pass's start for as long as it differs (whole blocks), its loop that much in. A mod's
WAV: mono 16-bit, its loop in a `smpl` chunk (or none: it stops at its end); its rate is the
HD's (the pitch the game gives it), not the file's.

`sound-check` (a ctest) converts every sample and plays both through the cursor: the same
samples, block by block, through three passes of each loop (58 loop).

### Rooms

A room's `.PAC` holds 17 sections. The converted ones go into a folder beside it -
`ST_000/ST_003.PAC` -> `ST_000/ST_003.room/` - and the library's PAC keeps only the rest
(`pac_strip`: their bytes and sizes as they were; 273 MB of PACs become 13 MB):

```
ST_000/ST_003.room/
  mesh.gltf + mesh.bin    the mesh (section 3): glTF 2.0, textured - opens in Blender
  textures.texbank/       the room's textures (section 9), effects.texbank/ the effects' (11)
  messages.txt            its text (10)
  lights.txt              its lights (4): ambient, then each light's position, colour, range ...
  cameras.txt             its camera setups (5): eye, field of view, target
```

- **The mesh** (`src/data/roomgltf.h`) is written as the engine builds it: the still triangles in
  room space, a primitive a draw (indexed: a strip's triangles share their vertices), and each
  moving batch (parallax, billboards, flip books) a node of its own with its matrix, its strip
  and the strip's breaks (`_NO_TRI`). The PS2's vertex colours are `COLOR_0` with 1.0 its 0x80
  (so lit brighter than the texture goes past 1). What glTF has no word for - a draw's part,
  blending, visibility group; a batch's movement; the bounds - is in `extras` ("hga"). Read back
  with cgltf (vendored), it is the same `RoomMesh`. 167 meshes, 178 MB (the section's 156).
- **Lights and cameras** (`src/data/roomtext.h`) are text, every word as the section has it (a
  float, or `#xxxxxxxx` where a float can't say the bits), what follows the records as `tail`
  hex. Read back, they are the section's bytes again, put back into the PAC as the room loads
  (`pac_attach`): a mod's `lights.txt` relights a room, over the library's or the PAC's own.
- **Not converted yet**: the nav meshes (0, 1, 16), the events and areas (2), the cameras'
  paths (6), the doors (7, 8), the placed objects (12, 15), the effects (13), the obstacles (14).
  Their formats are known only as far as the engine reads them; a text form of each needs its
  every field named, or it would be a hex dump in a text file. They stay in the PAC.

`room-check` (a ctest) builds every room's mesh and reads its glTF back (the same RoomMesh,
valid glTF by cgltf's checks), reads every lights / cameras text back (the same bytes, 329
sections), and strips every PAC (the rest the same bytes and sizes, 16-byte aligned).
`texbank-check` covers the rooms' texture banks with the others'. A frame of the first room drawn
from the library is the same picture as from the disc.

### Models

A model's `.PCK` (four resources: 0 the skeleton and meshes, 1 the morphing parts, 2 the shadow
volumes, 3 the motion bank) becomes `O_FIN/FIN_000.model/`:

```
model.gltf + model.bin    the skinned mesh and its skeleton (resources 0, 1): glTF 2.0
motions.bin               the motion bank (3), as it is
shadows.bin               the shadow volumes (2), as they are (OpenHuey doesn't draw them yet)
```

`src/data/modelgltf.h`: the bones are nodes at rest (so a viewer shows the model standing), the
mesh skinned by them (`JOINTS_0` / `WEIGHTS_0`, the inverse bind matrices as the game has them);
each bone's exact rest angles, the bone table and the draws' engine words in `extras`. The
materials point at the model's textures (`../FIN_000.texbank/`). The motions stay the game's:
the engine plays its keys - Euler angles between keys, special tracks, parts - as the original
does, which glTF's animations (quaternions, slerp) can't reproduce exactly; a readable form of
them is for later. The two .PCKs the engine doesn't build as models (`DNT_200`, `WIR_200`) are
copied as they are.

`model-check` (a ctest) writes every model as glTF and reads it back: the same `Model` - bones,
bone table, every vertex and draw (the draws in their passes' order: a draw's words say where its
vertices go), the motion bank's bytes; valid glTF. 113 models: 107 MB of PCK, 87 MB of glTF and
motions.

What the converted forms keep is what the engine reads: a field it doesn't (a flag bit nothing
uses, padding) isn't carried into the glTF. The disc keeps them: a later OpenHuey that needs one
re-imports.

### Movies

A movie (CRI Sofdec, `.SFD`) is an MPEG program stream: MPEG video and its sound as an ADX
stream (stream 0xC0). The import splits it, re-encoding nothing: `X.SFD` -> `X.mpg` (the stream
less its sound packets - every video packet byte for byte; a plain MPEG file ffmpeg, VLC or mpv
open) and `X.flac` (the sound, decoded once through as the player decodes it; none for the 8
silent ones). 135 movies: 894 MB of SFD, 588 MB of .mpg and 531 MB of FLAC. Re-encoding to
WebM would be a third the size, but lossy, an hour of encoding at the import and two encoders
built into OpenHuey - not worth it when the frames must stay the frames.

The player (`platform/movie.c`) reads X.mpg with X.flac's sound when they are there (a mod's
over the library's), else the SFD; the demux (`src/data/sfd.c`) is the one the import and the
check use too. A mod's movie: an MPEG-1 / 2 program stream as X.mpg, its sound (or none) as
X.flac.

`movie-check` (a ctest) splits every movie and reads both as the player does: every video
packet's payload the same bytes in the same order (299217), every sound sample the same as the
player's own ADX decoding gives (283 million); 8 silent. And a frame of the opening, drawn from
the SFD and from the library, is the same picture byte for byte.

Some formats carry engine behaviour in their layout (motion tables, a room's areas and
cameras): those convert into the engine's own structures written plainly, not into a general
format that loses them.

## Mods

(How to make one, step by step - the formats, the game's words and messages, hooks, testing,
pitfalls: **`docs/modding.md`**. This section is the design.)

A mod is a folder in `mods/` (beside `library/`; with `OPENHUEY_HOME`, in that folder), or a `.zip`
of one there: `mod.fs` at the zip's top, or in the one folder it holds (as zipping the folder
makes it). A zip is read in place - nothing is unpacked; its files stored or deflated (no
zip64, no encryption), and its paths' case never matters. The sample,
`examples/mods/room-tally` (in the releases, `mod-examples/`), is turned on by copying it into
`mods/`, or for one run with `openhuey --mod examples/mods/room-tally`.

```
mods/room-tally/
  mod.fs              its manifest
  library/            files over the library's: library/data/SYSTEM/TITLE.texbank/0.png ...
  scripts/            its Forth: scripts/room-tally.fs, the vocabulary named after the mod
```

`mod.fs` - read by OpenHuey, not run (`\` and `( )` comments):

```forth
mod: room-tally          \ its name: letters, digits, - (the vocabulary of its scripts)
version: 1 0
requires: openhuey 1 0        \ the mod API it is written for (mod-api); a mod, and its version
after: better-textures   \ loaded after that mod, if it's on
```

- **Order.** `mods/order.txt` (the title's MODS menu writes it) has `on: <name>` / `off: <name>`
  lines in load order; a mod it doesn't name is on, after those it does, by name. An `after:`
  moves a mod down below the one it names; `--mod` folders come last.
- **Off.** `off:` in order.txt; a `requires:` not met (OpenHuey's mod API: the same major, a minor
  no newer - or a mod missing, off or older); a name that is one of the game's vocabularies;
  scripts that fail to load (what they made before the error stays). Each is said on stderr
  with the reason, and `.mods` at the console lists them all.
- **The tests** see only `--mod` folders, never `mods/`.

### Three layers

1. **Files** (built). `files_read` (and the movies' stream) looks in each mod's `library/`,
   the last in the order first, then in the library. The paths are the library's: under
   `library/data/` DATA.CVM's files, the folders named as the game names them (`SYSTEM`, not
   `system`; a file's own name may be in any case), and `library/` itself for the disc's own.
   - **Textures.** A mod's PNG at the converted path stands for that texture
     (`<bank>.texbank/<i>.png`; for a 16-colour texture `<i>-<k>.png` are its other palettes'
     pictures, as bank.txt maps them). Over a bank not converted (the developer's extracted
     data, a library imported before step 2) `<i>.png` stands for the texture in every
     palette. A PNG of another size is drawn at its own size, filling the same surface, so an
     HD texture pack is PNGs 2x or 4x the originals (a 16-colour texture's palettes may differ
     in size; up to 8192 pixels a side). The rooms' are in `ST_xxx.room/textures.texbank/`.
   - Nothing else is needed for texture packs, music (a FLAC at the track's path),
     translations (the messages' text files) and sounds (a WAV in the bank's `.samples/`). The executable's tables, once they are data
     files in `library/exe/`, are overridden the same way.
2. **Actors** (built). A mod's `scripts/<name>.fs` is loaded after the game's scripts as the
   vocabulary `<name>` (`IN: room-tally`); its folder is a root for `USING:`, so a bigger mod
   splits into `room-tally.words` (`scripts/room-tally/words.fs`) and so on. It may `USING:` any
   of the game's vocabularies and `mods` (scripts/mods.fs: `mod-layer`, the UI layer for mods).
   The way to change what happens is an actor of the mod's own: it subscribes to the game's
   broadcasts (`entered-room`, `fiona-doing`, `stalker-doing`, `hewie-doing`, `danger`,
   `panic` ...) and sends the game's messages (`do-action`, `go-to-room`, `fright`,
   `stalker-order` ...). It never pokes another actor's state. The game's messages are the mod
   API (`mod-api`, `MODS_API_MAJOR/MINOR` in src/game/mods.h); `messages.fs` and the
   subsystem pages document them, and they become a promise - renamed only with a new major.
   A soft reset (the game over's return to the title) loads the mods' scripts again with the
   game's. There is no sandbox: OpenHuey says so when it loads a mod's scripts.
3. **Hooks** (the first built). Where a mod needs to be asked, not told, the game offers named
   hooks: deferred words with the game's own as the default, each part of the API (a new one
   raises the mod API's minor). A mod keeps what it replaces (`action-of`), sets its own with
   `is`, and calls the kept one for the rest - so mods setting the same hook chain in load order.
   They are added as mods need them, not all at once; candidates: a room's entry, the title's
   menu entries, the sub screen's pages, the mini-game's clock.

   | Hook | Vocabulary | Since | What |
   |---|---|---|---|
   | `item-use-rule ( place -- result )` | `items` | 1.1 | what using the item at `place` (a held place: `id@`) does; the result as the game's rules' (`no-use`, `used-up` ...: bits 1 / 2 spend one; `done-quietly`: something happened, nothing spent) |
   | `title-menu-hook ( -- )` | `opening` | 1.2 | the title's entries from mods: call what it replaced, then `title-entry ( addr len xt -- )` each (shown after the game's, before MODS / UPDATE / QUIT; picking one runs xt in the title's actor - `title-gone` first to leave the title) |

   The sample, `examples/mods/flower-scent`: the Fresh Flowers used where the game has no use for
   them calm Fiona a little (the game's `remedy` message) and are kept - asking the game's rule
   first, so the flowers' own use (the vase) is untouched. `mods_flower_scent` (ctest) runs it.

Later, if mods ask for it: replacing one of the game's script files outright (a mod's
`scripts/story/rooms/castle-1f-4.fs` over the game's). It would break with every change to that
file; the mods menu would mark such mods "replaces game scripts".

### The mods menu

The title shows MODS when `mods/` has any: each mod in load order, ON or OFF, and why one asked
for didn't load this run (its `requires:`, a failed script ...). The cross turns one on or off,
Left / Right (L1 / R1) move it in the order, the triangle goes back and writes `mods/order.txt`.
Changes apply when OpenHuey next starts: mods load with the game's scripts. (`scripts/mods/menu.fs`,
the list in `src/game/mods.c`; `--mods DIR` names another folder of mods - the tests use it.)

### Checked

- `mods_menu`, `mods_menu_after` (ctest): the menu on a copy of the samples (`--mods`) - a mod
  turned off and moved up, order.txt written, and in force when OpenHuey starts again.
- `mods_free_play_entry` (ctest): the title hook's sample - FREE PLAY among the entries.
- `mods_first_person` (ctest): the mods' camera's sample - the camera at Fiona's eyes and her
  yaw, her walking where it looks, an event's shot taking it and giving it back, F4.
- `mods_flower_scent` (ctest): the hook sample on - its rule set, the flowers calming her and
  kept, a remedy still the game's.
- `mods_room_tally` (ctest): the sample mod on - its actor there, counting rooms, answering
  `rooms-seen?` by message; `mods_room_tally_zipped` the same from a zip of it (cmake's own
  zip: deflated, the folder inside).
- `mods-check` (ctest): the order (order.txt, `after:`), every way a mod is turned off, the
  files layer (the later mod's file over the earlier's over the data, an off mod's unseen), a
  mod's PNG over a raw bank and over a converted one (its other palettes still the library's),
  a PNG of another size; zips with mod.fs at the top (stored) and in a folder (deflated), a
  zip's file over a folder mod's and beside the data, a damaged zip passed over.

### New content (later)

New rooms, items, costumes, figures and lines need tables a mod can add to, not just replace:
an item registry, the message tables, the room graph, the cast's kinds. Each comes with the
converted format for its kind (a room as files can be a new room) and an id space for mods (ids
from $1000 up, say, so they never collide with the game's).

## Updating

A release build updates itself to the newest build: the title's UPDATE (a release's only), or
`openhuey --update` from a command line. The newest build is the rolling `openhuey-latest`
release (whatever master built last), so any build other than this one counts as newer.

- **What it reads**: the release's `version.txt` (made by `tools/hga_release.sh`):
  `openhuey-version <build>` - the build's name, its version and commit (`0.1.2-c6b7113`, the
  CMake `HGA_BUILD`, shown on the UPDATE page and compared with this build's) - then each
  archive's name and size.
- **How**: the system's `curl` downloads the archive into `update/` beside the executable and its
  `tar` unpacks it (both come with Linux distributions and Windows 10 and later: OpenHuey has no
  network code of its own). Each thing at the archive's top - the program, `scripts/`, `docs/`,
  the libraries, the README - then takes the place of the one beside the executable, the old one
  moved into `update/old/` first (a running program can be moved, on Windows too, though not
  removed). If one can't be put in place, all go back. `library/`, `user/` and `mods/` are never
  touched, even if an archive had them. The next start removes `update/`.
- **After**: the UPDATE page's cross starts the new build and closes this one (`--update` says to
  start it again). The library stays: a newer OpenHuey still reads an older library (above,
  "The catch with converted only").
- Developer builds don't update themselves (`updater?` false; `--update` says to pull and build).
  `OPENHUEY_UPDATE_URL` (the release's folder) and `OPENHUEY_UPDATE_DIR` (the folder updated)
  point it elsewhere: the tests.
- The work is `src/platform/update.c` (on a thread of its own; `update-state`, `update-progress`
  for the page), the page `scripts/updater.fs`.

### Checked

- `update_run` (ctest, `tests/update/run.sh`): releases made in the test (`file://`) - a newer
  build put in place (the program, `scripts/` replaced whole, a new file added) with `user/`,
  `library/` and `mods/` left be; this build up to date; a download without the program, a
  short download and no release at all failing with nothing changed.
- `update_title` (ctest): UPDATE among the title's entries before QUIT; its page drawn, the
  update failing when there is no release and saying why, tried again.

## Order of work

1. **The library, plain**: `user/`, `library/` and `mods/` beside the executable; the import
   copying every file as it is, with the manifest and the checks; the engine reading
   `library/`; the first-run flow. No conversion yet - this alone ends reading from the disc.
   (Built.)
2. **Textures to PNG** - the converter, the loader, the comparison test. (Built.)
3. **Mods, files and actors**: the manifest, the load order, file overrides, mod scripts in
   their own vocabularies. A sample mod in the repository as a test. (Built.)
4. **Music, messages, the executable's tables** converted; the first hooks, as the sample mods
   ask. (Built: music, messages, the executable's tables; the first hook, item-use-rule.)
5. Sound banks (samples built), movies (built); then rooms and models (built: the meshes,
   textures, text, lights and cameras of rooms; models' meshes and skeletons - the rest of the
   rooms' sections and the motions as they are, for now).
