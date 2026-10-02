/* Hewie (vtable 0x46A120, 0xF37C0 bytes, SceneGame +0xE35F80 = character slot 1). */
#ifndef HEWIE_H
#define HEWIE_H

#include "actor.h"

/* Hewie fields not understood yet, by offset. */
#define HW(h, off, type) (*(type *)((u8 *)(h) + (off)))

typedef struct Hewie {
    /* 0x00000 */ Character c;
    /* 0x01540 */ u8 pad1540[0xF37C0 - 0x1540];   /* +0x1540: his .PCK (model) */
} Hewie;
_Static_assert(sizeof(Hewie) == 0xF37C0, "Hewie size");

#endif
