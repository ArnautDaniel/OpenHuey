/* Fiona (vtable 0x46AAB0, 0x1AD740 bytes, SceneGame +0xC88840 = character slot 0). */
#ifndef FIONA_H
#define FIONA_H

#include "actor.h"
#include "common.h"

/* Fiona fields not understood yet, by offset. */
#define FI(f, off, type) (*(type *)((u8 *)(f) + (off)))

typedef struct Fiona {
    /* 0x000000 */ Character c;
    /* 0x001540 */ u8 pad1540[0x1AD540 - 0x1540];
    /* 0x1AD540 */ void *msgImage;     /* message image shown while she is disabled, NULL = none */
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
_Static_assert(__builtin_offsetof(Fiona, msgImage) == 0x1AD540, "msgImage");
_Static_assert(__builtin_offsetof(Fiona, target) == 0x1AD600, "target");
_Static_assert(sizeof(Fiona) == 0x1AD740, "Fiona size");

/* fiona.c */
extern void Fiona_SetTarget(Fiona *f, s32 slot, s32 param);
extern void Fiona_LowerRecovery(Fiona *f, s32 n);
extern void Fiona_CalmDown(Fiona *f, s32 n);
extern s32 Fiona_IsIdle(Fiona *f);   /* the player can be controlled */
extern void Fiona_StartAction4(Fiona *f, u32 arg, u32 flag);
extern void Fiona_ShowEquipment(Fiona *f);   /* her costume put on */
extern void Lists_Clear(u8 *o);
extern s32 Fiona_Shakes(Fiona *f);
extern void Fiona_ResetRecovery(Fiona *f);
extern void Fiona_HewieReact(Fiona *f, s32 n);

/* ---- (was panic.h) ---- */

/* panic.c: what other files call. */

/* panic.c */
extern void Panic_Update(u8 *o);
extern void Panic_SetLevel(u8 *o, s16 n);
extern void Panic_SetStage(u8 *o, u32 stage);
extern void Panic_Pause(u8 *o);
extern void Panic_Fright(u8 *o, f32 amount);   /* a fright (less with a charm on) */
extern void Panic_FrightRaw(u8 *o, f32 amount);

/* fiona.c */
extern void ScreenFade_Step(u8 *f, s32 kind);   /* jump a fade to its end */
extern void *Costume8Model_ctor(u8 *m);
extern void *Costume7Model_ctor(u8 *m, s32 kind);
extern void *Costume6Model_ctor(u8 *m, s32 kind);
extern void *Costume3Model_ctor(u8 *m);
extern void *Costume2Model_ctor(u8 *m);
extern void *FionaModel_ctor(u8 *m);
extern void ScreenFade_Level(u8 *fade, f32 t);   /* the screen fade's level, 0..1 */
extern void ScreenFade_Frame(u8 *fade, s32 mode);

#endif
