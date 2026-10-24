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
 * coloured) - here their first frame. A name "a_..." is see-through, "g_..." marks the bloom mask (no colour).
 * They start shown: the scripts hide them (event 0x50 - the original's +0 byte, 1 hidden). */
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
    /* the rooms' own values kept on the object (the original's object words at +0x30..+0x7C:
     * a lid's speed, a chair's rock; read and written by their offset, `placed-word`) */
    uint32_t words[0x80 / 4];
    /* a morph (kinds 1 / 3): its definition (in the room's data), the frames shown and their blend
     * (the original's +0x74 / +0x78 / +0x7C: the model's +0x34 / +0x38 / +0x3C) */
    const uint8_t *def, *def_end;
    int morph_a, morph_b;
    float morph_t;
} Placed;
/* the object's 32-bit word at the original's offset `off` (+0x10..+0x18 its turn, +0x20..+0x28
 * its place, the rest its own words); NULL past +0x7C */
uint32_t *placed_word(Placed *p, int off);

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
/* its renderer layer by its name (PlacedObject_Draw): "g_..." $26 (the bloom mask only), "a_..."
 * $19 (see-through), others 1 */
int placed_layer(const Placed *p);
/* the shown ones of renderer layer `layer` (with the room's textures) */
void placed_draw(PlacedSet *s, const Mat4 *view_proj, const GpuTexture *textures, int ntextures, int layer);

#endif
