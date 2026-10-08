/* Sprites: what the original's quad drawer draws (src/game/effects.c QuadDrawer, gl_sprites) -
 * camera-facing textured quads, each a cell of a texture sheet, for the effects (flames, specks,
 * smoke...). An engine service: an effect (a Forth actor) keeps a batch, sets its texture and
 * cells, and writes its sprites' records each frame; the engine draws every batch after the
 * scene's opaque parts.
 *
 * Textures come as the original's texture cache has them (TexCache): an index of group + id, 64
 * of them - group 0x10 GAME_FIX.TEX (loaded at start), 0 the room's bank (PAC section 9), 0x15 its
 * second (section 11). */
#ifndef SPRITES_H
#define SPRITES_H

#include <stddef.h>
#include <stdint.h>

#include "../core/mathx.h"

#define SPRITE_BATCHES 64
#define SPRITES_PER_BATCH 512

enum {
    SPRITE_UPRIGHT = 1,    /* turned to the camera about y only */
    SPRITE_CORNERS = 2,    /* its own corners, not turned to the camera */
    SPRITE_QUARTER = 4,    /* the camera's turn, then a quarter about y */
    SPRITE_ADD = 0x40,     /* added to the picture */
    SPRITE_GLOW = 0x80     /* also glows (the original's glow pass: not yet) */
};

typedef struct Sprite {
    float x, y, z;
    float w, h;            /* half its size */
    float rot;             /* about the view axis */
    uint8_t rgba[4];       /* 0x80 = 1.0 */
    int frame;
} Sprite;

/* the cache's group `group` from a texture bank (NULL: gone); the bank must stay while it's used */
void sprites_group(int group, const uint8_t *bank, size_t size);
void sprites_init(void);    /* GAME_FIX.TEX as group 0x10 */

int sprites_new(void);      /* a batch (-1: none free) */
void sprites_free(int b);
void sprites_free_all(void);
/* its texture (group + id, palette -1 the first), the frame cells (the first at x y, cw x ch,
 * along rows of a tw x th sheet, `frames` of them), its flags (SPRITE_*) and draw layer */
void sprites_texture(int b, int group, int id, int palette);
void sprites_cells(int b, int x, int y, int cw, int ch, int tw, int th, int frames);
void sprites_flags(int b, int flags, int layer);
void sprites_offset(int b, float cx, float cy);           /* the corners' offset */
void sprites_corner(int b, int k, float x, float y, float z);   /* its own corner k (0..3) */
/* the batch's records: n of them (its count is n; -1: the count kept), to be written */
Sprite *sprites_records(int b, int n);

void sprites_draw(const Mat4 *view_proj, const Mat4 *view);

#endif
