/* Fiona (vtable 0x46AAB0, 0x1AD740 bytes, SceneGame +0xC88840 = character slot 0). */
#ifndef FIONA_H
#define FIONA_H

#include "actor.h"

/* Fiona fields not understood yet, by offset. */
#define FI(f, off, type) (*(type *)((u8 *)(f) + (off)))

typedef struct Fiona {
    /* 0x000000 */ Character c;
    /* 0x001540 */ u8 pad1540[0x1AD540 - 0x1540];
    /* 0x1AD540 */ s32 msgId;          /* message shown while she is disabled, 0 = none */
    /* 0x1AD544 */ u8 pad1AD544[0x1AD580 - 0x1AD544];
    /* 0x1AD580 */ s32 unk1AD580;
    /* 0x1AD584 */ u8 pad1AD584[4];
    /* 0x1AD588 */ u8 unk1AD588;
    /* 0x1AD589 */ u8 pad1AD589[0x1AD5C0 - 0x1AD589];
    /* 0x1AD5C0 */ s32 unk1AD5C0;
    /* 0x1AD5C4 */ u8 pad1AD5C4[0xC];
    /* 0x1AD5D0 */ u8 unk1AD5D0;
    /* 0x1AD5D1 */ u8 unk1AD5D1;
    /* 0x1AD5D2 */ u8 pad1AD5D2[0x1AD5E0 - 0x1AD5D2];
    /* 0x1AD5E0 */ f32 savedYaw;
    /* 0x1AD5E4 */ u8 pad1AD5E4[0x1AD600 - 0x1AD5E4];
    /* 0x1AD600 */ Character *target;  /* character she interacts with */
    /* 0x1AD604 */ u8 pad1AD604[0x1AD620 - 0x1AD604];
    /* 0x1AD620 */ s32 targetParam;
    /* 0x1AD624 */ u8 pad1AD624[0x1AD62E - 0x1AD624];
    /* 0x1AD62E */ u16 unk1AD62E;
    /* 0x1AD630 */ u8 unk1AD630;
    /* 0x1AD631 */ u8 pad1AD631[0x1AD740 - 0x1AD631];
} Fiona;
_Static_assert(sizeof(Character) == 0x1540, "Character size");
_Static_assert(__builtin_offsetof(Fiona, msgId) == 0x1AD540, "msgId");
_Static_assert(__builtin_offsetof(Fiona, target) == 0x1AD600, "target");
_Static_assert(sizeof(Fiona) == 0x1AD740, "Fiona size");

#endif
