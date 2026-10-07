/* The room's placed objects (the original's gRoomObjects: src/game/room.c PlacedObjects_*,
 * PlacedObject_*, PlacedModel_Draw): things the scripts show, hide and move by name - a crate,
 * a lattice, rubble. PAC section 12 defines them: four counts (one per model kind), then their
 * offsets from +0x20; a definition is its name (16 bytes), its turn (+0x10, X Y Z) and place
 * (+0x20), then its model (from +0x30: +0x40 the vertex count, +0x44 the texture, then offsets
 * from the definition to its streams). Section 15 holds their animations: records (a name, +0x10
 * a count, +0x14 the offset of 16-byte tracks: frames, format, keys' offset).
 *
 * Model kinds: 0 rigid (s16 steps from an s32 start / 4096, scale 32), 2 the room's batch
 * layout (floats, its own colours), 1 / 3 two-frame morphs (s16 / 4096, scale 32; 1 lit, 3
 * coloured) - here their first frame. A name "a_..." is see-through, "g_..." a glow.
 * They start hidden: the scripts show them (event 0x50). */
#ifndef PLACED_H
#define PLACED_H

#include "../data/roommesh.h"
#include "../render/render.h"

#define PLACED_MAX 64

typedef struct Placed {
    char name[16];
    int kind;
    int shown, loop;
    Vec3 rot, pos;              /* now (+0x10, +0x20) */
    Vec3 def_rot, def_pos;      /* as defined */
    RoomMesh mesh;              /* model space */
    GpuMesh gpu;
    float scale;
    /* the animation playing: its keys, format (8: the turn, 9: the place - xyz floats), frames */
    const uint8_t *keys;
    int format, frames, frame;
} Placed;

typedef struct PlacedSet {
    Placed p[PLACED_MAX];
    int n;
    const uint8_t *anims;       /* section 15 (NULL none) */
    size_t anims_size;
} PlacedSet;

/* from sections 12 and 15 (NULL: none) */
void placed_load(PlacedSet *s, const uint8_t *defs, size_t defs_size, const uint8_t *anims, size_t anims_size);
void placed_free(PlacedSet *s);
Placed *placed_named(PlacedSet *s, const char *name);
/* start animation `id` of the object's record (PlacedObject_StartAnim); loop or once */
void placed_anim(PlacedSet *s, Placed *p, int id, int loop);
/* a tick: the animations step (PlacedObject_Animate) */
void placed_tick(PlacedSet *s);
/* the shown ones (with the room's textures) */
void placed_draw(PlacedSet *s, const Mat4 *view_proj, const GpuTexture *textures, int ntextures);

#endif
