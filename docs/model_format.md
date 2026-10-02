# Character model format (.PCK / .TEX)

Worked out from the loader and renderer; `tools/hg_model.py` converts a model to glTF (.glb).

    tools/hg_model.py "<data>/O_HEW/HEW_000.PCK" --tex "<data>/O_HEW/HEW_000.TEX" -o hewie.glb

## Files per character (e.g. `O_HEW`, Hewie)

| File | Loaded by | What |
|---|---|---|
| `HEW_00n.PCK` | model vtbl +0xA0 (0x1F8090), n = costume 0..4 | skeleton, meshes, motion bank |
| `HEW_00n.MRK` | model vtbl +0xA4 (0x1F8010) | (index tables, not decoded yet) |
| `HEW_000.TEX` | model vtbl +0xA8 (0x1F7F00) | textures, by the mesh parts' texture id |
| `HEW_1xx.MTN` | | animations (not decoded yet) |

Hewie's model class: vtable `D_0046B240`, 0xB90 bytes at Character +0xF0, built by
`func_003A10B0` (base `func_0016F4B0`). Fiona's loader `func_001A4110` (vtbl +0x14) shows the
load: the .PCK goes raw to Character +0x1540, the .MRK to +0x1AA540 (Fiona).

## .PCK

`u32 count` (4), then `count` resource offsets (0 = none). The character's setup
(`func_001A3EE0` for Fiona) turns them into pointers on the model:

| Resource | Model field | What |
|---|---|---|
| 0 | +0x4C0 | skeleton + meshes |
| 1 | +0x4D0 | (optional; extra pass `func_001BD650` when present) |
| 2 | +0x4CC | used by the drawable at +0x1D0 (`func_001F2B80`) |
| 3 | +0x4C4 | motion bank (starts with the same count as the .MRK) |

### Resource 0: skeleton

`u32 bones, u32 meshTable, u32 ?, u32 boneByteTable` (offsets from resource 0), then
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

## .TEX

`u32 count`, 12 bytes pad, then 16-byte records:
`u32 psm, u16 width, u16 height, u16 ?, u16 ?, u32 offset (from the record)`.
psm 0x13 (PSMT8): `width * height` index bytes, then a 256-entry RGBA palette in PS2 CLUT
order (in each 32 entries, 8..15 and 16..23 swapped), alpha 0x80 = opaque.
