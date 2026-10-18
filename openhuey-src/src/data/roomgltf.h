/* A room's mesh as glTF 2.0 (docs/library-and-mods.md): the library's form of a PAC's section 3
 * - ST_000/ST_003.room/mesh.gltf + mesh.bin - as the engine builds it (data/roommesh.h), and read
 * back into the same RoomMesh.
 *
 *   mesh 0, node 0       the room's still triangles (room space), a primitive a draw, in order
 *   node 1.. (mesh 1..)  each moving batch (a parallax, a billboard, a flip book): its matrix, its
 *                        strip (mode 5) and the strip's breaks (_NO_TRI: 1 no triangle ends here)
 *   COLOR_0              the PS2's colours, 1.0 its 0x80 (so up to ~2: lit brighter than the texture)
 *   materials            the room's textures (textures.texbank/<i>.png beside it)
 *   extras "hga"         what the engine needs that glTF has no word for: a draw's part, blending,
 *                        group ...; a batch's movement; the scene's the solid part's bounds
 * Opened in Blender or any glTF viewer, it is the room, textured. */
#ifndef ROOMGLTF_H
#define ROOMGLTF_H

#include <stddef.h>
#include <stdint.h>

#include "roommesh.h"

#define ROOMGLTF_FORMAT 1

/* the glTF's JSON (malloc'd, `len` long) and its binary (malloc'd: named `bin_name` in the JSON) */
char *roomgltf_write(const RoomMesh *m, const char *bin_name, uint8_t **bin, size_t *bin_size, size_t *len);
/* read back (m empty first); 0 if it isn't one hga wrote (or can read) */
int roomgltf_read(RoomMesh *m, const char *json, size_t len, const uint8_t *bin, size_t bin_size);

#endif
