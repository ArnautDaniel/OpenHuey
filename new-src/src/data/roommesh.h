/* The room mesh (PAC section 3), turned into plain triangles.
 *
 * The section starts with six offsets (from the section start), one per part:
 *   0  the solid room                       3  (unused)
 *   1  see-through parts                    4  animated (flip-book) parts - not drawn yet
 *   2  the bloom mask (not drawn as colour) 5  glows: drawn without depth writes
 * A part is a list of batches, ended by a count of -1. A batch is:
 *   int count, texture (-1: none), flags, extra   - flags: bit 0 blend, bits 8..15 placement,
 *                                                   bits 24..31 visibility group;
 *                                                   extra bit 8: additive (part 5)
 *   float matrix[16]                              - row-major: batch-local to room space
 *   count x (s, t) floats, padding, count x RGBA bytes (0x80 = 1.0), padding,
 *   count x (x, y, z, flags) - a triangle strip; flags bit 15 set: no triangle ends here
 * (the padding after each array keeps the next one 16-byte aligned: it depends on count & 3). */
#ifndef ROOMMESH_H
#define ROOMMESH_H

#include "../core/mathx.h"

#include <stddef.h>
#include <stdint.h>

typedef struct MeshVertex {
    float x, y, z;
    float s, t;
    uint8_t rgba[4];   /* 0x80 = 1.0, as the PS2 had it */
} MeshVertex;

enum { MESH_SOLID, MESH_SEE_THROUGH, MESH_BLOOM_MASK, MESH_PART3, MESH_ANIMATED, MESH_GLOW, MESH_PARTS };

typedef struct MeshDraw {
    int first, count;      /* vertices (a triangle list) */
    int texture;           /* index in the room's texture bank, -1 none */
    uint8_t part;          /* MESH_* */
    uint8_t blend;         /* alpha blending */
    uint8_t additive;      /* added, not blended */
    uint8_t no_zwrite;
    uint8_t group;         /* visibility group: 0 always shown, others switched by the room */
} MeshDraw;

typedef struct RoomMesh {
    MeshVertex *v;
    int nv;
    MeshDraw *d;
    int nd;
    Vec3 lo, hi;           /* bounds of the solid part */
} RoomMesh;

/* build from the mesh section; 0 if it is malformed (m is then empty) */
int roommesh_build(RoomMesh *m, const uint8_t *sec, size_t size);
void roommesh_free(RoomMesh *m);

#endif
