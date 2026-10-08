/* .TEX texture banks: a count, then 16-byte entries, each pointing (relative to itself) at its
 * image (indexed 4/8-bit or direct colour, rows of w texels) and its palette (CLUT). The CLUT is
 * stored as the PS2 uploaded it, so 256-colour palettes have entries 8..15 and 16..23 of every 32
 * swapped (the GS's CSM1 order). */
#ifndef TEX_H
#define TEX_H

#include <stddef.h>
#include <stdint.h>

typedef struct TexEntry {
    uint8_t psm;          /* pixel format (TEX_*) */
    uint8_t cpsm;         /* palette format: TEX_CT32 or TEX_CT16 */
    uint8_t pad[2];
    uint16_t w, h;
    uint16_t image_qwc;   /* image size in 16-byte units */
    uint16_t clut_qwc;    /* palette size in 16-byte units (follows the image) */
    int32_t data;         /* the image, from the start of this entry */
} TexEntry;

enum { TEX_CT32 = 0x00, TEX_CT24 = 0x01, TEX_CT16 = 0x02, TEX_T8 = 0x13, TEX_T4 = 0x14 };

/* the number of textures in a bank (0 if it doesn't look like one) */
int tex_count(const uint8_t *bank, size_t size);
const TexEntry *tex_entry(const uint8_t *bank, int i);
/* decode texture i to RGBA8 (w*h*4 bytes, alpha 0..255): 1 on success */
int tex_decode(const uint8_t *bank, size_t size, int i, uint8_t *rgba);
/* ... a 16-colour one with palette `csa` (0..31; -1 the first) */
int tex_decode_csa(const uint8_t *bank, size_t size, int i, int csa, uint8_t *rgba);

#endif
