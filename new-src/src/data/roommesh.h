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
 * (the padding after each array keeps the next one 16-byte aligned: it depends on count & 3).
 *
 * Batches that move with the view or animate are kept apart (RoomMesh.dyn), to be rebuilt each
 * frame (src/game/room.c RoomMesh_Draw, RoomMesh_PlaceBatch, RoomMesh_Billboard,
 * RoomMesh_FlipBook):
 *   flags bits 8..11  parallax: shifted sideways by a fraction of the eye's position
 *   flags bits 12..15 billboard: turned to face the eye (2 upright, 4 fully)
 *   part 4 batches    flip books - quads whose texture steps through frames laid out in the
 *                     texture (the quad's own size apart, wrapping to the next row); flags
 *                     bits 16..23 the frame count, extra bits 0..3 ticks a frame, bits 4..7
 *                     the type: 2 holds the last frame; 1 3 4 6 face the eye (1 6 upright);
 *                     4 5 6 add their light without writing depth */
#ifndef ROOMMESH_H
#define ROOMMESH_H

#include "../core/mathx.h"

#include <stddef.h>
#include <stdint.h>

typedef struct MeshVertex {
    float x, y, z;
    float s, t;
    uint8_t rgba[4];   /* 0x80 = 1.0, as the PS2 had it */
    float nx, ny, nz;  /* the normal, for lit draws (characters); rooms carry their lighting in rgba */
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
    uint8_t solid_tex;     /* the texture's alpha is not transparency (characters' normal parts) */
    uint8_t lit;           /* lit by the scene's light (characters), not by its vertex colours alone */
    uint8_t mask;          /* marks the bloom mask (part 2) instead of drawing colour */
} MeshDraw;

/* a batch rebuilt every frame: its strip in its own space, and how it moves */
typedef struct DynBatch {
    MeshDraw draw;         /* (first, count: its vertices in RoomMesh.dv, a strip) */
    Mat4 local;            /* batch space to room space */
    uint8_t *no_tri;       /* per strip vertex: no triangle ends here */
    uint8_t parallax;      /* 0, or 2 4 8 16 */
    uint8_t billboard;     /* 0, or 2 upright, 4 facing */
    uint8_t flip_type;     /* part 4: the flip book's type */
    int frames, frame_ticks, frame, timer;
    float frame_w, frame_h;   /* the quad's texture size: one frame */
} DynBatch;

typedef struct RoomMesh {
    MeshVertex *v;
    int nv;
    MeshDraw *d;
    int nd;
    Vec3 lo, hi;           /* bounds of the solid part */
    MeshVertex *dv;        /* the moving batches' strips (batch space) */
    int ndv;
    DynBatch *dyn;
    int ndyn;
} RoomMesh;

/* a tick: flip books step */
void roommesh_tick(RoomMesh *m);
/* the moving batches for this view as triangles (room space) into out (room for ndv * 3) and
 * their draws (room for ndyn); the number of draws */
int roommesh_dynamic(const RoomMesh *m, Vec3 eye, Vec3 forward, MeshVertex *out, MeshDraw *draws);

/* build from the mesh section; 0 if it is malformed (m is then empty) */
int roommesh_build(RoomMesh *m, const uint8_t *sec, size_t size);
void roommesh_free(RoomMesh *m);

#endif
