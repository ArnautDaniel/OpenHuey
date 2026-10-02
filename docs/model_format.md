# Character models and motions (.PCK / .TEX / .MTN)

Worked out from the game's loader, renderer and motion player (function addresses below).

## Converting and viewing

Needs the game data extracted from `DATA.CVM` (folders `O_*`), Python 3 (no extra modules) and,
for the viewer, raylib 6.

    # everything: one .glb per model in build/models/<NAME>/ (about a minute)
    tools/hg_export_all.py "<data>" -o build/models

    # one model, with textures and extra motion files
    tools/hg_model.py "<data>/O_HEW/HEW_000.PCK" --tex "<data>/O_HEW/HEW_000.TEX" \
        --mtn "<data>/O_HEW/HEW_100.MTN" -o hewie.glb       # --fps N (default 30), --no-motions

    # viewer: browse the folder, play motions, pose bones (F1 shows the keys)
    cc -O2 -o build/hgview tools/viewer/hgview.c -lraylib -lm
    build/hgview build/models

The .glb files (glTF 2.0) hold: the skeleton (nodes under an "Armature" node, rest pose from the
bone records), the skinned mesh (one primitive per part), morphing parts (faces, hands) as their
own meshes with glTF morph targets (shape keys in Blender), textures as PNG (a solid copy for the
normal parts, one with alpha for the cut-out parts), and the motions as glTF animations named by
their motion id (`0100`, `HEW_100_0002`, ...). Blender: File > Import > glTF 2.0.

Known gaps: the human characters' hands (morph parts, kind 0) look glitchy - the converter's
reading of those parts is still wrong somewhere; root motion (special channels -1/-5/-6) is not exported, so motions play in place;
the playback rate is assumed (30 fps); motion and shape names are only numbers; the `.MRK` files,
resource 2 (per-bone shadow volumes) and `*_D000.MTN` (door data) are not decoded; `O_T00` uses
other formats (.MDL/.SHD/.SHP). Textures other than 8-bit indexed (psm 0x13) are not handled
(none seen so far).

## Characters (folder prefixes)

From looking at the converted models; names with `?` are guesses.

| Folder | What |
|---|---|
| FIN | Fiona (default outfit); FIN_001..006 variants; O_FIN_M: her motion files |
| FIB, FIC, FIF, FIH, FIM, FIO, FIS, FIW | Fiona's other outfits / story variants (FIF: frog suit, FIS: towel, ...) |
| HEW | Hewie (white shepherd); HEW_001..004 same mesh, different motion sets |
| HEG | Hewie as a German shepherd (brown) |
| HED | Hewie as a plush toy dog |
| DB0, DB1, DB2 | Debilitas |
| DNL | Daniella |
| RCG, RCT | Riccardo |
| LRO, LRY, LRM | Lorenzo (variants?); LRC, LRF: golden final forms?; LRH: a part of one? |
| HMA, HMB, SGM, GLM | other creatures (homunculi?, golem?) |
| CRW, RBT, FS0..FS2 | crow, rabbit, fish |
| HND, WHC, WIR, SHT, DNT | props: hand, wheelchair, wire spring, cloth sheet, DNT (unknown) |

`_200` files are second versions of the same model (most folders have one).

## Files per character (e.g. `O_HEW`, Hewie)

| File | Loaded by | What |
|---|---|---|
| `HEW_00n.PCK` | model vtbl +0xA0 (0x1F8090), n = costume 0..4 | skeleton, meshes, motion bank |
| `HEW_00n.MRK` | model vtbl +0xA4 (0x1F8010) | (index tables, not decoded yet) |
| `HEW_000.TEX` | model vtbl +0xA8 (0x1F7F00) | textures, by the mesh parts' texture id |
| `HEW_1xx.MTN` | | extra motion banks (same format as the PCK's motion bank) |

Hewie's model class: vtable `D_0046B240`, 0xB90 bytes at Character +0xF0, built by
`func_003A10B0` (base `func_0016F4B0`). Fiona's loader `func_001A4110` (vtbl +0x14) shows the
load: the .PCK goes raw to Character +0x1540, the .MRK to +0x1AA540 (Fiona).

## .PCK

`u32 count` (4), then `count` resource offsets (0 = none). The character's setup
(`func_001A3EE0` for Fiona) turns them into pointers on the model:

| Resource | Model field | What |
|---|---|---|
| 0 | +0x4C0 | skeleton + meshes |
| 1 | +0x4D0 | morphing parts: faces, hands (`func_001BD650`) |
| 2 | +0x4CC | per-bone shadow volumes (6 vertices each), drawable at +0x1D0 (`func_001F2B80`) |
| 3 | +0x4C4 | motion bank (starts with the same count as the .MRK) |

### Resource 0: skeleton

`u32 bones, u32 meshTable, u32 rigidTable, u32 boneByteTable` (offsets from resource 0), then
`bones` records of 0x70 bytes at +0x10 (`func_001F7C40` links them into the skeleton at
model +0x810):

| Offset | What |
|---|---|
| +0x00 | s32 parent (-1 = root) |
| +0x04..+0x2C | local rest data (rotation, translation) |
| +0x30 | 4x4 inverse bind matrix, row-vector convention (translation in the last row) |

### Resource 0: mesh table (`func_001BDDD0` / `func_001BD830` / `func_002B89C0`)

At resource 0 + `meshTable`: `u32 parts`, then 0x30-byte part records at +0x10. **Offsets in a
part record are relative to the record.**

| Offset | What |
|---|---|
| +0x00 | vertex count (0 = nothing to draw) |
| +0x04 | positions: 16-byte header (3 x s32 start) then the position deltas |
| +0x08 | UVs |
| +0x0C | normals |
| +0x10 | weights |
| +0x14 | bone slots |
| +0x18 | strip flags |
| +0x1C | texture id (index in the .TEX) |
| +0x20 | flags (bit 0: drawn in the second pass) |
| +0x24 | palette size (bones used by this part) |
| +0x28 | palette: that many u8 bone numbers |
| +0x2C | influences per vertex, 1..4 (also picks the VU1 microprogram) |

Each part is sent to VU1 in batches of up to 48 vertices, one unpack per stream:

| Stream | VIF unpack | Per vertex | Meaning |
|---|---|---|---|
| positions | V3-16, STMOD difference, row = header | 3 x s16 | **delta** from the previous vertex; / 4096 |
| UVs | V2-16 unsigned | 2 x u16 | / 32768 |
| normals | V3-16 | 3 x s16 | / 32768 |
| weights | S/V2/V3/V4-16 unsigned | n x u16 | / 32768 (sum = 1) |
| bone slots | S/V2/V3/V4-8 unsigned | n x u8 | / 4 = palette index |
| strip flags | S-8, masked into position w | u8 | 1 = no triangle at this vertex (strip start) |

Triangle strips run on across batches (the GS keeps its vertex queue). Positions are in model
space; the matrix for a palette bone is bone world x the record's +0x30 inverse bind matrix
(`func_001BD830`).

### Resource 0: rigid parts (eyeballs etc.)

At resource 0 + `rigidTable`: `u32 count`, then 0x20-byte records at +0x10 (offsets from the
record): vertex count, positions (same header + deltas), UVs, normals, strip flags, texture id,
?, bone. Each is attached to one bone (Hewie: both eyeballs on the head bone, and a small part of
the tongue). The head mesh has holes where the eyes go, so without these the eyes are missing.

### Resource 1: morphing parts (faces, hands)

`u32 count`, then 0x40-byte records at +0x10 (drawn by `func_001BD650` -> `func_001BC8D0` /
`func_001BCD30` / `func_001BD280`; packets `func_002B7A80` / `func_002B86A0`). Offsets from the record:

| Offset | What |
|---|---|
| +0x00 | number of shapes (Fiona's face 16, hands 10) |
| +0x04 | vertex count |
| +0x08 | UVs (2 x u16 / 32768) |
| +0x0C | strip flags (u32 per vertex, bit 15 = no triangle) |
| +0x10 | shape table: 8-byte entries (positions, normals), offsets from the entry |
| +0x14 | bone (the whole part follows it) |
| +0x18 | texture id |
| +0x1C | 1: face (blends the rest shape with shapes 1, 7, 8 by weights at model +0x48); 0: blends 2 shapes |
| +0x20 | flags (bit 0: second pass) |
| +0x30 | base point, 3 x s32 |

Shape positions: 3 x s16 per vertex + base point (VIF offset mode), / 4096; normals 3 x s16 /
32768. Shape 0 is the rest shape. Fiona's face: shapes 1-6 move the mouth, 7-8 the eyes
(blinks?), 9-12 the brows, 13-15 the whole face.

## .TEX

`u32 count`, 12 bytes pad, then 16-byte records:
`u32 psm, u16 width, u16 height, u16 ?, u16 ?, u32 offset (from the record)`.
psm 0x13 (PSMT8): `width * height` index bytes, then a 256-entry RGBA palette in PS2 CLUT
order (in each 32 entries, 8..15 and 16..23 swapped), alpha 0x80 = opaque. Alpha only means
transparency for the second-pass parts (cut-outs: fur, lashes, torn cloth; alpha-tested); the
normal parts' texture alpha is something else and is ignored.

Extra .TEX files are alternative textures for the same model, e.g. HEW_201/202: texture 1 with a
wounded / bandaged leg.

## Motions (PCK resource 3, and .MTN files)

Read by `func_001F4B80` / `func_001F4C10`, sampled by `func_001F36B0`, posed by `func_001F5930`.

Bank: `u32 records, u32 ?, u32 ?, u32 idMap`. Id map (at bank + idMap): `u32 count`, then at +0x10
`(u32 motion id, u32 record index)` pairs (Hewie: 0x000.., 0x100.., 0x200.., ...).
Records: 0x14 bytes at bank + 0x10, five part offsets (from the record, 0 = none). Parts cover
disjoint bones (Hewie: 0 special channels, 1 body, 2 ears, 3 tail, 4 jaw/tongue).
Part: `u32 tracks, u32 frames, u32 trackTable` (from the part). Track (0xC bytes):
`s32 code, u32 type, u32 keys (from the track)`. `code < 0`: special channel (-1 root motion,
-5/-6 ...); else bone = skeleton byte table[code + 1] (resource 0 + header[3]).
`type & 0x10000`: static (one key); low 16 bits:

| Type | Key | Values |
|---|---|---|
| 0 | 3 x s16 | Euler rotation x 2pi/65536 |
| 1 | 3 x s16 | translation / 256 |
| 2, 4 | 6 x s16 | rotation (as 0) + translation (as 1) |
| 3 | 4 x s16 | quaternion / 32768 |
| 5 | s16, s16, s16 | two raw values + one / 32768 |
| 6 | 8 x f32 | |
| 7 | 6 x f32 | rotation + translation |
| 8, 9 | 3 x f32 | |

One key per frame; the game interpolates linearly between frames (angles the short way round).
A track's values **replace** the bone's rest values (rest record +0x10 Euler, +0x20 translation);
a rotation-only track keeps the rest translation and vice versa. Bone local matrix
(`func_002E2E00`): rotate X, then Y, then Z, then translate (row-vector `Rx Ry Rz T`);
world = local x parent world.
