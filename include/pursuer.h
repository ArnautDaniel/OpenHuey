/* Pursuer: the shared base of the stalkers (Debilitas, Daniella, Riccardo, Lorenzo) and the
 * story characters loaded into character slot 2 by func_00171160 (0x28 kinds).
 *
 * There is no vtable of its own: each kind's vtable (204 entries, e.g. Debilitas 0x469D10,
 * Daniella 0x46BBB0, Riccardo 0x46F6B0, Lorenzo 0x470720) repeats the shared methods
 * (code 0x278490..0x29FF10, helpers 0x211C80..0x219460, defaults 0x179600..0x179970) and
 * overrides a few (+0x8 dtor, +0x30 update, +0xF4 / +0xF8 model load, ...).
 * Objects are 0x1800 bytes (0x1840 for Riccardo and a few others). */
#ifndef PURSUER_H
#define PURSUER_H

#include "actor.h"

/* Pursuer fields not understood yet, by offset. */
#define PU(p, off, type) (*(type *)((u8 *)(p) + (off)))

typedef struct Pursuer {
    /* 0x0000 */ Character c;
    /* 0x1540 */ Character *target;   /* the character being chased */
    /* 0x1544 */ u8 pad1544[0x1800 - 0x1544];
} Pursuer;
_Static_assert(sizeof(Pursuer) == 0x1800, "Pursuer size");

#endif
