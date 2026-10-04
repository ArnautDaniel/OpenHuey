/* Pursuer: the shared base of the stalkers (Debilitas, Daniella, Riccardo, Lorenzo) and the
 * story characters loaded into character slot 2 by func_00171160 (0x28 kinds).
 *
 * Classes: Actor (vtable 0x469C20) -> Character (0x469C60) -> NPC (0x46C220, 64 entries:
 * navigation / doors / vision, code 0x211C80..0x219530, src/game/pursuer_ai.c) -> Pursuer
 * (0x46D810, 204 entries, dtor 0x172810, code 0x278490..0x29FF10, src/game/pursuer.c) -> each
 * kind (e.g. Debilitas 0x469D10, Daniella 0x46BBB0, Riccardo 0x46F6B0, Lorenzo 0x470720),
 * which overrides a few entries (+0x8 dtor, +0x30 update, +0xF4 / +0xF8 model load, ...).
 * The kinds' constructors only store Actor, Character and their own vtable; the middle ones
 * appear in the destructors. Shared defaults of the kinds: 0x179600..0x179970.
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

/* the animation player at Character +0xF0 */
#define MOTION_AT(p, off, type) (*(type *)((u8 *)(p)->c.motion + (off)))
#define MOTION_ANIM(p) MOTION_AT(p, 0x55C, s32)                         /* current animation id */
#define MOTION_KEYS(p) (*(u32 *)(MOTION_AT(p, 0x6A4, u8 *) + 0x18))     /* key flags of the frame */
#define MOTION_KEY_END 0x20                                             /* the animation ended */

/* the current behaviour step is over (+0x16EE) / start the next (+0x16F0) */
#define PURSUER_STEP_DONE(p) PU(p, 0x16EE, u8)
#define PURSUER_STEP_NEXT(p) PU(p, 0x16F0, u8)

/* ---- generated from the definitions (tools: protos.py) ---- */
Pursuer *func_001710D0(Pursuer *p, s32 flags);
Pursuer *func_00172810(Pursuer *p, s32 flags);
void func_00179600(Pursuer *p);
s32 func_00179610(Pursuer *p);
void func_00179620(Pursuer *p);
void func_00179710(Pursuer *p);
void func_00179720(Pursuer *p);
void func_00179730(Pursuer *p);
void func_00179740(Pursuer *p);
void func_00179750(Pursuer *p);
void func_00179760(Pursuer *p, s32 frames);
void func_00179780(Pursuer *p, s32 side, f32 *out);
f32 func_00179830(Pursuer *p);
f32 func_00179850(Pursuer *p);
f32 func_00179870(Pursuer *p);
f32 func_00179880(Pursuer *p);
f32 func_001798A0(Pursuer *p);
f32 func_001798B0(Pursuer *p);
f32 func_001798C0(Pursuer *p);
f32 func_001798D0(Pursuer *p);
f32 func_001798E0(Pursuer *p);
f32 func_001798F0(Pursuer *p);
f32 func_00179910(Pursuer *p);
s32 func_00179930(Pursuer *p);
s32 func_00179940(Pursuer *p);
s32 func_00179950(Pursuer *p);
void func_00179960(Pursuer *p);
Pursuer *func_00179970(Pursuer *p, s32 flags);
void func_00211C80(Pursuer *p, s32 a2);
s32 func_00211CF0(Pursuer *p, s32 exit);
f32 func_00211F70(Pursuer *p, s32 room, u32 a, u32 b);
f32 func_00212060(Pursuer *p, s32 room, s32 a, s32 b);
s32 func_00212190(Pursuer *p, s32 room);
void func_00212240(Pursuer *p, s32 room);
s32 func_00212360(Pursuer *p);
s32 func_00212400(Pursuer *p);
void func_00212540(Pursuer *p);
f32 func_00212730(Pursuer *p, s32 exit);
void func_00212CA0(Pursuer *p, u32 door);
void func_00212D30(Pursuer *p, u32 door);
s32 func_00212DC0(Pursuer *p, s32 door);
s32 func_00212DE0(Pursuer *p, s32 door);
u32 func_00212E00(Pursuer *p, u32 tri, const f32 *pos, u32 door);
s32 func_00212F40(Pursuer *p, u32 door, u32 side);
s32 func_002131A0(void);
void func_00213B60(Pursuer *p, u32 mask);
f32 func_00213C60(Pursuer *p, u32 tri, const f32 *pos);
f32 func_00213D40(Pursuer *p, Character *c);
void func_00213E30(Pursuer *p);
u32 func_00213EC0(Pursuer *p, f32 heading, f32 a, f32 b);
u32 func_00213FA0(Pursuer *p, const f32 *pos, f32 a, f32 b);
f32 func_002140A0(Pursuer *p, f32 heading, f32 step);
s32 func_00214890(Pursuer *p, u32 *triOut, f32 *posOut, f32 step);
u32 func_00214940(Pursuer *p);
s32 func_00214A90(Pursuer *p, u32 tri);
s32 func_00214AF0(Pursuer *p);
f32 func_00214B90(Pursuer *p, u32 tri, const f32 *pos);
s32 func_00217110(Pursuer *p, const f32 *pos);
s32 func_00217260(Pursuer *p);
s32 func_002172F0(Pursuer *p, Character *c);
s32 func_00217340(Pursuer *p);
s32 func_00217370(Pursuer *p, Character *c);
s32 func_00217460(Pursuer *p, s32 slot);
s32 func_00217510(Pursuer *p);
s32 func_00217560(void);
s32 func_002175B0(Actor *a, Actor *b);
s32 func_00217600(Pursuer *p);
void func_00217680(Pursuer *p);
void func_002177D0(Pursuer *p);
s32 func_00217920(Pursuer *p);
s32 func_00217ED0(Pursuer *p, f32 angle, f32 dist);
s32 func_00217FC0(Pursuer *p, f32 dist);
void func_00218110(Pursuer *p);
s32 func_002181C0(Pursuer *p);
s32 func_002181D0(Pursuer *p, const f32 *from, const f32 *to, f32 heading, f32 range, f32 half);
s32 func_00218300(Pursuer *p, Actor *from, Actor *to, f32 heading, f32 range, f32 half);
s32 func_00218940(Pursuer *p);
s32 func_00218A30(Pursuer *p);
s32 func_00218B60(Pursuer *p);
s32 func_00218C20(Pursuer *p, u32 tri);
void func_00218C90(Pursuer *p, u32 exit);
void func_00218D80(Pursuer *p, u32 exit);
void func_00218E70(Pursuer *p);
void func_00219310(Pursuer *p, u32 tri, const f32 *pos, s32 room);
u32 func_00219450(Pursuer *p);
void func_00219460(Pursuer *p);
void **func_00278490(void **obj, s32 flags);
void func_0027CE80(Pursuer *p);
void func_0027D130(Pursuer *p);
void func_0027D1B0(Pursuer *p);
void func_0027D810(Pursuer *p);
void func_0027DFA0(Pursuer *p);
void func_0027E440(Pursuer *p);
void func_0027E560(Pursuer *p);
void func_0027E5A0(Pursuer *p, u32 tri);
void func_00280090(Pursuer *p);
void func_002801B0(Pursuer *p);
void func_002837C0(Pursuer *p, u32 mask);
s32 func_00283870(Pursuer *p);
void func_00283A50(Pursuer *p, s32 *bones, f32 *a, f32 *b);
void func_00283EE0(Pursuer *p);
s32 func_00283EF0(Pursuer *p);
s32 func_00284440(Pursuer *p);
void func_00285360(Pursuer *p);
void func_00285AB0(Pursuer *p);
void func_002860D0(Pursuer *p);
void func_00286AA0(Pursuer *p);
void func_00288030(Pursuer *p);
void func_00289500(Pursuer *p);
void func_00289810(Pursuer *p);
void func_00289F50(Pursuer *p);
void func_00289FA0(Pursuer *p);
void func_0028A060(Pursuer *p);
void func_0028A100(Pursuer *p);
void func_0028A540(Pursuer *p);
void func_0028A660(Pursuer *p);
void func_0028A700(Pursuer *p);
void func_0028A860(Pursuer *p);
void func_0028A930(Pursuer *p);
void func_0028A9E0(Pursuer *p);
void func_0028AA80(Pursuer *p);
void func_0028C420(Pursuer *p);
void func_0028CD70(Pursuer *p);
void func_0028CE20(Pursuer *p);
void func_0028CF80(Pursuer *p);
void func_0028D040(Pursuer *p);
void func_0028D540(Pursuer *p);
void func_0028D5B0(Pursuer *p);
void func_0028D6E0(Pursuer *p);
void func_0028D7D0(Pursuer *p);
void func_0028D8A0(Pursuer *p);
void func_0028D950(Pursuer *p);
void func_0028DAA0(Pursuer *p);
void func_0028DBD0(Pursuer *p);
void func_0028ED20(Pursuer *p);
void func_0028EFC0(Pursuer *p);
void func_0028FC10(Pursuer *p);
void func_00290810(Pursuer *p);
void func_00291600(Pursuer *p);
void func_00291760(Pursuer *p);
void func_00291880(Pursuer *p);
void func_002919B0(Pursuer *p);
void func_00291C30(Pursuer *p);
void func_00292120(Pursuer *p);
void func_002927D0(Pursuer *p);
void func_002934C0(Pursuer *p);
void func_00294150(Pursuer *p);
void func_002947F0(Pursuer *p);
void func_00296ED0(Pursuer *p);
s32 func_00297160(Pursuer *p);
s32 func_00297290(Pursuer *p, f32 *table, u32 n);
s32 func_00297A70(Pursuer *p);
s32 func_00297AC0(Pursuer *p);
s32 func_00297B00(Pursuer *p);
s32 func_00297B40(Pursuer *p, s32 anim, s32 blend);
void func_002982A0(Pursuer *p, f32 amount);
void func_00298FF0(Pursuer *p);
void func_00299080(Pursuer *p);
void func_002990E0(Pursuer *p);
void func_002992F0(Pursuer *p);
void func_00299300(Pursuer *p);
void func_00299370(Pursuer *p);
void func_002994B0(Pursuer *p);
void func_0029A2A0(Pursuer *p);
void func_0029A3D0(Pursuer *p);
void func_0029A6D0(Pursuer *p);
s32 func_0029A850(Pursuer *p);
s32 func_0029A870(Pursuer *p);
s32 func_0029A8C0(Pursuer *p, s32 room);
u32 func_0029B4B0(Pursuer *p);
u32 func_0029CB40(Pursuer *p);
s32 func_0029CBE0(Pursuer *p, f32 *out);
s32 func_0029CD00(Pursuer *p, u32 kind, s32 slot, u32 door);
void func_0029CE40(Pursuer *p, s32 a1, s32 *out);
s32 func_0029CE50(Pursuer *p);
void func_0029D3E0(Pursuer *p);
void func_0029D410(Pursuer *p, s32 sound, s32 a2, s32 a3, s32 a4, void *a5);
void func_0029E390(Pursuer *p);
void func_0029E440(Pursuer *p);
void func_0029E520(Pursuer *p);
void func_0029E600(Pursuer *p);
void func_0029E9D0(Pursuer *p);
void func_0029EAA0(Pursuer *p);
void func_0029EC60(Pursuer *p);
void func_0029ED00(Pursuer *p);
s32 func_0029ED80(Pursuer *p);
void func_0029EE00(Pursuer *p);
s32 func_0029EE10(Pursuer *p);
void func_0029EE70(Pursuer *p);
void func_0029EF80(Pursuer *p, s32 id);
void func_0029F040(Pursuer *p, u32 slot);
void func_0029F2C0(Pursuer *p);
void func_0029F350(Pursuer *p);
void func_0029FB20(Pursuer *p);
/* ---- end generated ---- */

#endif
