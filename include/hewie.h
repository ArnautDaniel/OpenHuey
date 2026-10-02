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

#define HEWIE_NAV_MASK 0x29020008
#define HEWIE_ACTION(h) HW(h, 0xF3564, s32)                  /* current action (func_00130AF0) */
#define HEWIE_SIDE(h) HW(h, 0xF3668, s32)                    /* side of the room (rooms +0x50) */
#define HEWIE_STATE(h) ((PTMF *)((u8 *)(h) + 0xF35D0))       /* behaviour (pointer to member) */
#define HEWIE_MSG(h) HW(h, 0xF3540, void *)                  /* his message image */
#define HEWIE_MRK(h) ((u8 *)(h) + 0xF1540)                   /* his .MRK data */
#define HEWIE_HP(h) ((h)->c.hp)

#endif
