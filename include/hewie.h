/* Hewie (vtable 0x46A120, 0xF37C0 bytes, SceneGame +0xE35F80 = character slot 1). */
#ifndef HEWIE_H
#define HEWIE_H

#include "actor.h"
#include "common.h"

/* Hewie fields not understood yet, by offset. */
#define HW(h, off, type) (*(type *)((u8 *)(h) + (off)))

typedef struct Hewie {
    /* 0x00000 */ Character c;
    /* 0x01540 */ u8 pad1540[0xF37C0 - 0x1540];   /* +0x1540: his .PCK (model) */
} Hewie;
_Static_assert(sizeof(Hewie) == 0xF37C0, "Hewie size");

#define HEWIE_NAV_MASK 0x29020008
#define HEWIE_ACTION(h) HW(h, 0xF3564, s32)                  /* current action (Hewie_SetAction) */
#define HEWIE_SIDE(h) HW(h, 0xF3668, s32)                    /* side of the room (rooms +0x50) */
#define HEWIE_STATE(h) ((PTMF *)((u8 *)(h) + 0xF35D0))       /* behaviour (pointer to member) */
#define HEWIE_MSG(h) HW(h, 0xF3540, void *)                  /* his message image */
#define HEWIE_MRK(h) ((u8 *)(h) + 0xF1540)                   /* his .MRK data */
#define HEWIE_HP(h) ((h)->c.hp)

typedef struct Character Character;

/* A placement: room, side, triangle (... +0x38 the exit he came by). */
typedef struct HewiePlacement {
    /* 0x00 */ s32 room;
    /* 0x04 */ s32 side;
    /* 0x08 */ u32 tri;
    /* 0x0C */ u8 pad0C[0x2C];
    /* 0x38 */ u8 exit;
} HewiePlacement;

/* hewie.c */
extern s32 func_001669A0(Hewie *h);
extern s32 func_0013B2C0(Hewie *h, s32 act);
extern void func_00138DE0(Hewie *h, s32 n);
extern void func_00138E60(Hewie *h, s32 amount);
extern s32 func_0013EE40(Hewie *h, u32 tri, const f32 *pos, s32 direct, s32 keep);
extern void func_00138AD0(Hewie *h, s32 mode, s32 time);
extern void func_00166150(Hewie *h, Character *other, s32 delta);   /* tell Hewie */
extern void func_0013D1F0(Hewie *h, s32 add);   /* his trust */
extern f32 func_002E2D00(f32 a);   /* angle wrapped to -pi..pi */
extern void func_002E2DD0(f32 *out, f32 (*m)[4], const f32 *v);
extern void func_00136620(Hewie *h);   /* Hewie restarted (hewie.c) */
extern void func_0013A430(Hewie *h, s32 snd);
extern void func_00143550(Hewie *h, s32 blend);
extern void func_001480B0(Hewie *h);
extern void func_0015F750(Hewie *h);
extern void func_0014F5A0(Hewie *h);
extern void func_00154D40(Hewie *h);
extern void func_00154DB0(Hewie *h);
extern void func_00154E60(Hewie *h);
extern void func_00154E40(Hewie *h);
extern void func_001654E0(Hewie *h, s32 a, s32 anim);
extern void func_0014F5B0(Hewie *h);
extern void func_00154D50(Hewie *h);
extern void func_00154DC0(Hewie *h);
extern void func_00151740(Hewie *h);
extern void func_001506A0(Hewie *h);
extern void func_0014DFB0(Hewie *h);
extern s32 func_00138FD0(Hewie *h);   /* Hewie listening (hewie.c) */
extern void func_00150610(Hewie *h);
extern void func_0014B4D0(Hewie *h);
extern void func_001531F0(Hewie *h);
extern s32 func_00137650(Hewie *h, Character *other);
extern void func_0014EB40(Hewie *h);
extern s32 func_0013D4A0(Hewie *h, s32 once);
extern void func_0014B780(Hewie *h);
extern void func_0014B590(Hewie *h);
extern void func_00149DD0(Hewie *h);
extern void func_00153350(Hewie *h);
extern void func_00149270(Hewie *h);
extern s32 func_00138EC0(Hewie *h);
extern s32 func_001364F0(Hewie *h);
extern void func_00140050(Hewie *h);
extern void func_0014DE70(Hewie *h);
extern void func_00155670(Hewie *h);
extern void func_001557B0(Hewie *h);
extern void func_001558F0(Hewie *h);
extern void func_0015F760(Hewie *h);
extern void func_00147580(Hewie *h);
extern void func_00146AE0(Hewie *h);
extern s32 func_001382F0(Hewie *h);
extern void func_0015BB20(Hewie *h);
extern s32 func_00139060(Hewie *h);
extern void func_0015F8A0(Hewie *h);
extern void func_0014A790(Hewie *h);
extern void func_00165510(Hewie *h, s32 st);
extern void func_00150450(Hewie *h);
extern void func_0014A180(Hewie *h);
extern void func_00140B00(Hewie *h);
extern s32 func_001667C0(Hewie *h);
extern void func_001517C0(Hewie *h);
extern void func_00151B10(Hewie *h);
extern void func_0014EC10(Hewie *h);
extern void func_00153B00(Hewie *h);
extern void func_00139840(Hewie *h);
extern void func_00149370(Hewie *h);
extern void func_0015F0A0(Hewie *h);
extern Character *func_001379C0(Hewie *h);
extern u8 func_0013CDC0(Hewie *h, Character *from, s32 both);
extern void func_0013E680(Hewie *h);
extern void func_00140190(Hewie *h);
extern void func_001404E0(Hewie *h);
extern void func_001407C0(Hewie *h);
extern void func_001470C0(Hewie *h);
extern void func_001476C0(Hewie *h);
extern void func_00147B90(Hewie *h);
extern void func_001480C0(Hewie *h);
extern void func_001489D0(Hewie *h);
extern void func_001499F0(Hewie *h);
extern void func_0014B190(Hewie *h);
extern void func_0014BE80(Hewie *h);
extern void func_0014C210(Hewie *h);
extern void func_0014DA50(Hewie *h);
extern void func_0014EE20(Hewie *h);
extern void func_00151190(Hewie *h);
extern void func_00151D10(Hewie *h);
extern void func_00152D60(Hewie *h);
extern void func_00153700(Hewie *h);
extern void func_00153D20(Hewie *h);
extern void func_00154150(Hewie *h);
extern void func_001545A0(Hewie *h);
extern void func_00154860(Hewie *h);
extern void func_00155000(Hewie *h);
extern void func_001569C0(Hewie *h);
extern void func_0015A460(Hewie *h);
extern void func_0015A720(Hewie *h);
extern void func_0015AE10(Hewie *h);
extern void func_0015B130(Hewie *h);
extern void func_0015B660(Hewie *h);
extern void func_0015BD90(Hewie *h);
extern void func_0015C1E0(Hewie *h);
extern void func_0015CCA0(Hewie *h);
extern void func_0015DB60(Hewie *h);
extern void func_0015DF40(Hewie *h);
extern void func_0015E3A0(Hewie *h);
extern void func_0015E880(Hewie *h);
extern void func_0015ECC0(Hewie *h);
extern void func_0015F2E0(Hewie *h);
extern s32 func_001662A0(Hewie *h, HewiePlacement *pl);

#endif
