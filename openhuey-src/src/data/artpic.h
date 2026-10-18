/* The art gallery's pictures (src/game/subscreen.c Gallery_StatePicture): SUBSCR/ARTnn.LEN holds a
 * group of eight - first their offsets (8 words), then each packed in runs (a byte with bit 7 set:
 * the next byte repeated n & 0x7F times; else n bytes as they are) to a 512-wide image of 8-bit
 * colour numbers (896 lines; from picture 21 on, 448), then its 256 colours (32-bit, as the PS2
 * uploads a CLUT: CSM1 order, alpha 0x80 opaque). */
#ifndef ARTPIC_H
#define ARTPIC_H

#include <stddef.h>
#include <stdint.h>

#define ARTPIC_W 512

/* picture `picture` (0..47; `file` its group's, 8 a group) as a .TEX bank of one texture (8-bit,
 * its CLUT after it: tex.h), malloc'd, `*size` its bytes and `*h` its lines; NULL if it isn't there */
uint8_t *artpic_bank(const uint8_t *file, size_t file_size, int picture, size_t *size, int *h);

#endif
