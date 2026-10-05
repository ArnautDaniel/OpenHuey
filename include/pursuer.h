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
#include "progress.h"
#include "sce/libvu0.h"
#include "effectmgr.h"

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

/* the game's characters and managers the pursuer code uses */
extern Character *gCharPlayer;    /* Fiona */
extern Character *gCharPartner;   /* Hewie */
extern VObject *D_0044E550;       /* random numbers */
extern VObject *D_0044E558;       /* doors */
extern VObject *D_0044E568;       /* rooms */
extern VObject *D_0044E4D0;       /* room objects */
extern VObject *D_0044E7A8;       /* controller vibration */
extern void *D_0044E570;          /* nav mesh */

extern void *D_0046D810[], *D_0046C220[], *D_00469C60[], *D_00469C20[];

/* The flags of an out-of-range nav triangle: the original takes the record pointer as NULL and
 * reads +0x3C anyway, i.e. a word of low kernel memory on the PS2. Kept for the difftest build;
 * natively an invalid triangle has no flags. */
#ifdef HG_NATIVE
#define NAV_BAD_TRI_FLAGS 0u
#else
#define NAV_BAD_TRI_FLAGS (*(volatile u32 *)0x3C)
#endif

/* ---- engine functions used (tools/pursuer/externs.py) ---- */
extern void func_00100490(void *obj);
extern void func_0010E640(f32 *out, const f32 *v, f32 s);
extern s32 func_00122B50(Actor *a, f32 *out);
extern void func_00122C20(Actor *a, s32 sound, s32 a2, s32 a3, s32 a4, void *a5);
extern s32 func_00122C90(Actor *a, u32 fromTri, u32 toTri, const f32 *from, const f32 *to, s32 a5);
extern s32 func_00123080(Actor *a, u32 fromTri, u32 toTri, const f32 *from, const f32 *to, u32 mask);
extern s32 func_00123470(void *self, u32 tri, f32 *pos);
extern s32 func_001235C0(Pursuer *p, Character *c, u32 i);
extern u32 func_00123710(void *self, s32 door, s32 side, const f32 *ofs, f32 *out);
extern s32 func_00123C60(Actor *a, s32 room, const f32 *pos);
extern s32 func_00123E20(Actor *a, const f32 *pos);
extern s32 func_001241F0(Actor *a, Actor *b, f32 margin, f32 vmargin);
extern s32 func_00124320(Actor *a, const f32 *pos, u32 tri, const f32 *to, u32 mask);
extern u32 func_00124480(Actor *a, const f32 *target, u32 mask);
extern f32 func_00124490(Actor *a, const f32 *pos);
extern f32 func_001244D0(Actor *a, const f32 *pos);
extern void func_00124530(Actor *a, f32 target, f32 step);
extern void func_001247E0(Actor *a, f32 *v);
extern void func_00124890(Actor *a, s32 a1);
extern void func_00124E40(Actor *a);
extern f32 func_001257B0(Character *c, u32 goalTri, const f32 *goal, u32 mask);
extern void func_00125900(Character *c);
extern void func_00125960(Character *c);
extern void func_00125A10(Character *c);
extern s32 func_00125AD0(Character *c);
extern void func_00125BA0(Character *c);
extern void func_00125BE0(Character *c);
extern void func_00125CC0(Character *c);
extern void func_00125D40(Character *c);
extern s32 func_00125D80(Character *c);
extern void func_00125E10(Character *c, const f32 *pos, s32 a2);
extern void func_00126360(Character *c);
extern void func_00126450(Character *c);
extern void func_001264C0(Character *c, s32 kind, f32 *pos);
extern s32 func_001264D0(Character *c, f32 *pos);
extern void func_00126810(Character *c);
extern void func_00126910(Character *c);
extern f32 func_00126E40(Character *c);
extern s32 func_00126F80(Character *c, s32 target, s32 unused2, s32 side, s32 unused4);
extern void func_00127060(Character *c);
extern s32 func_001270A0(Character *c);
extern s32 func_001270F0(Character *c);
extern s32 func_00127140(Character *c, s32 kind, u32 goalTri, const f32 *goal);
extern void func_001272B0(Character *c, f32 speed);
extern s32 func_001273D0(Character *c, u32 *triOut, f32 *posOut, f32 step);
extern void func_00127650(Character *c);
extern void func_00127660(Character *c);
extern u8 *func_0012BFB0(Pursuer *p);
extern void func_001777D0(Progress *pr, u32 slot);
extern void func_001777F0(Progress *pr, u32 slot);
extern u32 func_00177810(Progress *pr, u32 slot);
extern u32 func_00177830(Progress *pr, u32 slot);
extern u32 func_00177850(Progress *pr, u32 slot);
extern u32 func_00177870(Progress *pr, u32 slot);
extern void func_001779C0(Progress *pr, u32 door, u32 slot);
extern void func_001779F0(Progress *pr, u32 door, u32 slot);
extern u32 func_00177A20(Progress *pr, u32 i, u32 slot);
extern u32 func_00177BF0(Progress *pr, u32 i, u32 slot);
extern void func_00178070(Progress *pr, u32 slot, s32 a2, s32 a3, s32 a4, s32 a5, f32 f);
extern u32 func_00178300(Progress *pr, s32 room, s32 a2, u32 slot);
extern u32 func_001785B0(Progress *pr, s32 room, s32 exit);
extern void func_00178750(Progress *pr, s32 room, s32 a2);
extern u32 func_00178840(Progress *pr, s32 room, s32 exit);
extern u32 func_00178980(Progress *pr, s32 room, s32 exit);
extern void func_00178A90(Progress *pr, s32 room, s32 door, s32 a3);
extern void func_00178C10(Progress *pr, s32 room, s32 a2, u32 slot);
extern u32 func_00178DB0(Progress *pr, s32 room, s32 a2, u32 slot);
extern f32 *func_0017CE80(u8 *skel, s32 bone);
extern s32 func_001F4710(void *motion, s32 anim);
extern u32 func_001F4770(void *motion, s32 a1, s32 a2, s32 a3);
extern void func_001F6370(void *motion, f32 *out, f32 t);
extern void func_001F6AF0(void *motion);
extern void func_001F6E10(void *motion);
extern void func_001F6E30(void *motion);
extern u8 *func_0020D620(Pursuer *p);
extern u32 func_00211B00(Pursuer *p, u32 tri);
extern void func_002815E0(Pursuer *p, u32 door);
extern void func_00288150(Pursuer *p);
extern s32 func_00297300(Pursuer *p, u32 dir);
extern void func_00297C60(Pursuer *p);
extern void func_0029B8B0(Pursuer *p);
extern u8 *func_002CF140(Pursuer *p);
extern u8 *func_002DC460(Pursuer *p);
extern void func_002DC960(void *motion);
extern void func_002DCB40(void *motion);
extern void func_002DCDD0(void *motion, Pursuer *p, f32 a, f32 b);
extern void func_002DD110(void *motion, const f32 *pos, f32 *pitch, f32 *yaw);
extern void func_002DD310(void *motion, f32 a, f32 yaw, f32 b, f32 speed);
extern void func_002DDD20(void *motion, s32 anim, s32 variant);
extern void func_002DDE20(void *motion, s32 anim, s32 variant);
extern void func_002DDED0(void *motion, s32 anim, s32 variant);
extern f32 func_002E2BC0(f32 *v);
extern void func_002E2C10(f32 *out, f32 angle);
extern f32 func_002E2CA0(f32 *out, const f32 *v, f32 angle);
extern f32 func_002E2D00(f32 angle);
extern void func_002E2DA0(f32 *out, f32 (*m)[4], const f32 *v);
extern void func_002E2DD0(f32 *out, f32 (*m)[4], const f32 *v);
extern void func_002E3130(f32 (*m)[4], const f32 *pos, f32 angle);
extern void func_002E3190(f32 (*m)[4], f32 angle);
extern void func_002EF9E0(void *threat, f32 amount);
extern void func_002EFA50(void *threat, f32 amount);
extern u8 *func_002F9000(Pursuer *p);
extern u8 *func_002FC8F0(Pursuer *p);
extern u8 *func_00309410(Pursuer *p);
extern u8 *func_0030C1B0(Pursuer *p);
extern f32 func_0031C058(f32 x);
extern f32 func_0031C5C0(f32 x, f32 z);
extern u8 *func_00320150(Pursuer *p);
extern u8 *func_00331200(Pursuer *p);
extern u8 *func_00347290(Pursuer *p);
extern u8 *func_00348620(Pursuer *p);
extern u8 *func_003495B0(Pursuer *p);
extern u8 *func_0034D980(Pursuer *p);
extern u8 *func_00365D10(Pursuer *p);
extern void sceVu0ApplyMatrix(f32 *out, f32 (*m)[4], const f32 *v);
extern void func_0028B340(Pursuer *p);
extern s32 func_00177AB0(void *pr, s32 kind, u8 slot);
extern s32 func_00100B80(const PTMF *a, const PTMF *b);   /* __ptmf_cmpr: nonzero if they differ */
extern void func_00126270(Character *c);
extern u32 func_002DD420(void *motion, f32 *out, s32 foot, f32 lo, f32 hi);
extern u32 func_002DD860(void *motion, s32 foot, f32 t);
extern void func_002A8440(void *list, s32 kind, s32 room, u32 tri, s32 a4);
extern s32 func_002EC410(void *list);
extern void func_002DDC60(void *motion, s32 anim, s32 anim2, s32 variant);
extern void func_002DDBA0(void *motion, s32 anim, s32 anim2);
extern void func_0027AD80(Pursuer *p, u32 door);
extern void func_00166150(Character *hewie, Pursuer *p, s32 kind);   /* tell Hewie */
/* ---- end engine ---- */

/* ---- generated from the definitions (tools: protos.py) ---- */
Pursuer *func_001276F0(Pursuer *p, s32 flags);
void *func_00127800(void **m, s32 flags);
void func_00127A40(Pursuer *p, u32 kind);
void func_00127B80(Pursuer *p);
s32 func_00127BB0(Pursuer *p);
s32 func_00127BC0(Pursuer *p);
s32 func_00127C00(Pursuer *p);
s32 func_00127C40(Pursuer *p, s32 exit);
s32 func_00127CC0(Pursuer *p);
void func_00127D00(Pursuer *p, s8 situation);
s32 func_00127FC0(Pursuer *p);
s32 func_00128080(Pursuer *p);
s32 func_00128090(Pursuer *p);
void func_00128210(Pursuer *p);
void func_00128390(Pursuer *p);
void func_001284C0(Pursuer *p);
void func_001286F0(Pursuer *p);
void func_001287B0(Pursuer *p);
void func_00128970(Pursuer *p);
void func_00128A20(Pursuer *p);
void func_00128CA0(Pursuer *p);
void func_00128DB0(Pursuer *p);
void func_00128FC0(Pursuer *p);
void func_00129090(Pursuer *p);
void func_001291C0(Pursuer *p);
void func_00129550(Pursuer *p);
void func_00129560(Pursuer *p);
void func_00129570(Pursuer *p);
void func_001297C0(Pursuer *p);
void func_00129AF0(Pursuer *p);
void func_00129B30(Pursuer *p);
void func_00129D10(Pursuer *p);
void func_00129DB0(Pursuer *p);
void func_0012A390(Pursuer *p);
void func_0012A4C0(Pursuer *p);
void func_0012B490(Pursuer *p);
void func_0012B860(Pursuer *p);
void func_0012B990(Pursuer *p);
void func_0012BAA0(Pursuer *p, Character *c);
void func_0012BBF0(Pursuer *p);
void func_0012BD10(Pursuer *p, u32 tri, const f32 *pos, s32 room);
s32 func_0012BE60(Pursuer *p);
s32 func_0012BE70(Pursuer *p);
void func_0012C030(Pursuer *p);
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
Pursuer *func_0020C3A0(Pursuer *p, s32 flags);
s32 func_0020C4B0(Pursuer *p);
void func_0020C4D0(Pursuer *p);
void func_0020C510(Pursuer *p, s32 side, f32 *out);
void func_0020C5B0(Pursuer *p, s32 kind, f32 *out);
void func_0020C660(Pursuer *p, s32 exit);
void func_0020C730(Pursuer *p, s32 exit);
void func_0020C7C0(Pursuer *p);
void func_0020CAE0(Pursuer *p, s32 *e, f32 *a, f32 *b);
void func_0020CC70(Pursuer *p, s8 situation);
void func_0020D1F0(Pursuer *p, s32 on);
s32 func_0020D2E0(Pursuer *p);
s32 func_0020D2F0(Pursuer *p);
f32 func_0020D300(Pursuer *p);
f32 func_0020D310(Pursuer *p);
f32 func_0020D320(Pursuer *p);
void func_0020D330(Pursuer *p);
u8 *func_0020D620(Pursuer *p);
u8 *func_0020D660(Pursuer *p);
void func_0020D6A0(Pursuer *p);
void func_00211C80(Pursuer *p, s32 a2);
s32 func_00211CF0(Pursuer *p, s32 exit);
u32 func_00211E00(Pursuer *p, s32 exit);
f32 func_00211F70(Pursuer *p, s32 room, u32 a, u32 b);
f32 func_00212060(Pursuer *p, s32 room, s32 a, s32 b);
s32 func_00212190(Pursuer *p, s32 room);
void func_00212240(Pursuer *p, s32 room);
s32 func_00212360(Pursuer *p);
s32 func_00212400(Pursuer *p);
void func_00212540(Pursuer *p);
f32 func_00212550(Pursuer *p, u32 exit);
f32 func_00212730(Pursuer *p, s32 exit);
u32 func_00212850(Pursuer *p);
u32 func_00212A80(Pursuer *p, u32 skip);
void func_00212CA0(Pursuer *p, u32 door);
void func_00212D30(Pursuer *p, u32 door);
s32 func_00212DC0(Pursuer *p, s32 door);
s32 func_00212DE0(Pursuer *p, s32 door);
u32 func_00212E00(Pursuer *p, u32 tri, const f32 *pos, u32 door);
s32 func_00212F40(Pursuer *p, u32 door, u32 side);
s32 func_00212FE0(Pursuer *p, s32 side);
s32 func_002131A0(void);
void func_00213270(Pursuer *p, u32 door);
s32 func_002134E0(Pursuer *p, u32 door, s32 tri, const f32 *pos);
s32 func_00213690(Pursuer *p, s32 tri);
s32 func_002138F0(Pursuer *p, s32 tri);
void func_00213B60(Pursuer *p, u32 mask);
f32 func_00213C60(Pursuer *p, u32 tri, const f32 *pos);
f32 func_00213D40(Pursuer *p, Character *c);
void func_00213E30(Pursuer *p);
u32 func_00213EC0(Pursuer *p, f32 heading, f32 a, f32 b);
u32 func_00213FA0(Pursuer *p, const f32 *pos, f32 a, f32 b);
f32 func_002140A0(Pursuer *p, f32 heading, f32 step);
f32 func_00214190(Pursuer *p, const f32 *pos, f32 step);
s32 func_002143D0(Pursuer *p, const f32 *pos);
s32 func_00214620(Pursuer *p, s32 unused);
s32 func_00214890(Pursuer *p, u32 *triOut, f32 *posOut, f32 step);
u32 func_00214940(Pursuer *p);
s32 func_00214A90(Pursuer *p, u32 tri);
s32 func_00214AF0(Pursuer *p);
f32 func_00214B90(Pursuer *p, u32 tri, const f32 *pos);
void func_00214C70(Pursuer *p);
void func_00214ED0(Pursuer *p);
s32 func_00215130(Pursuer *p);
s32 func_00215D80(Pursuer *p);
s32 func_00216960(Pursuer *p, Character *c);
s32 func_00216B20(Pursuer *p);
s32 func_00216C90(Pursuer *p);
u32 func_00216E00(Pursuer *p, u32 tri, const f32 *pos, f32 *out);
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
s32 func_002179F0(Pursuer *p, s32 a1, f32 f);
s32 func_00217B90(Pursuer *p, s32 a1, f32 f);
u32 func_00217D30(Pursuer *p, f32 heading, f32 dist);
s32 func_00217ED0(Pursuer *p, f32 angle, f32 dist);
s32 func_00217FC0(Pursuer *p, f32 dist);
void func_00218110(Pursuer *p);
s32 func_002181C0(Pursuer *p);
s32 func_002181D0(Pursuer *p, const f32 *from, const f32 *to, f32 heading, f32 range, f32 half);
s32 func_00218300(Pursuer *p, Actor *from, Actor *to, f32 heading, f32 range, f32 half);
s32 func_00218430(Pursuer *p, Character *c);
s32 func_002187D0(Pursuer *p, u32 tri, const f32 *pos);
s32 func_00218940(Pursuer *p);
s32 func_00218A30(Pursuer *p);
s32 func_00218B60(Pursuer *p);
s32 func_00218C20(Pursuer *p, u32 tri);
void func_00218C90(Pursuer *p, u32 exit);
void func_00218D80(Pursuer *p, u32 exit);
void func_00218E70(Pursuer *p);
void func_00218ED0(Pursuer *p, Character *c);
void func_00219100(Pursuer *p);
void func_00219310(Pursuer *p, u32 tri, const f32 *pos, s32 room);
u32 func_00219450(Pursuer *p);
void func_00219460(Pursuer *p);
void **func_00278490(void **obj, s32 flags);
void func_00278EB0(Pursuer *p);
void func_00279350(Pursuer *p);
void func_0027A1E0(Pursuer *p);
void func_0027A6A0(Pursuer *p);
void func_0027AD80(Pursuer *p, u32 door);
void func_0027B810(Pursuer *p, u32 door);
s32 func_0027CA00(Pursuer *p);
void func_0027CB90(Pursuer *p, u32 exit);
void func_0027CE80(Pursuer *p);
void func_0027CEF0(Pursuer *p);
void func_0027D130(Pursuer *p);
void func_0027D1B0(Pursuer *p);
void func_0027D260(Pursuer *p);
void func_0027D5B0(Pursuer *p);
void func_0027D810(Pursuer *p);
void func_0027D8D0(Pursuer *p);
void func_0027DB30(Pursuer *p);
void func_0027DDB0(Pursuer *p);
void func_0027DFA0(Pursuer *p);
void func_0027E040(Pursuer *p);
void func_0027E440(Pursuer *p);
void func_0027E560(Pursuer *p);
void func_0027E5A0(Pursuer *p, u32 tri);
void func_0027E5D0(Pursuer *p, s32 n);
void func_0027E790(Pursuer *p);
void func_0027EAF0(Pursuer *p);
void func_0027EEA0(Pursuer *p);
s32 func_0027F0A0(Pursuer *p, u32 exit);
void func_0027F350(Pursuer *p);
void func_0027F5F0(Pursuer *p);
void func_0027FC70(Pursuer *p);
void func_0027FE90(Pursuer *p);
void func_00280090(Pursuer *p);
void func_002801B0(Pursuer *p);
void func_00280210(Pursuer *p, s32 exit);
void func_002804B0(Pursuer *p);
void func_002809E0(Pursuer *p);
void func_002815E0(Pursuer *p, u32 door);
void func_00282010(Pursuer *p, u32 door);
void func_002837C0(Pursuer *p, u32 mask);
s32 func_00283870(Pursuer *p);
f32 func_002838E0(Pursuer *p);
void func_00283A50(Pursuer *p, s32 *bones, f32 *a, f32 *b);
void func_00283AE0(Pursuer *p);
void func_00283C50(Pursuer *p);
void func_00283EE0(Pursuer *p);
s32 func_00283EF0(Pursuer *p);
void func_00284040(Pursuer *p);
s32 func_00284440(Pursuer *p);
void func_00284540(Pursuer *p);
void func_002849B0(Pursuer *p);
s32 func_00284C80(Pursuer *p);
void func_00284FD0(Pursuer *p);
void func_00285150(Pursuer *p);
void func_00285360(Pursuer *p);
void func_002854A0(Pursuer *p);
void func_002856A0(Pursuer *p);
void func_002858B0(Pursuer *p);
void func_00285AB0(Pursuer *p);
void func_00285B10(Pursuer *p);
s32 func_00285DE0(Pursuer *p, u32 kind, f32 *heading, f32 *pos);
void func_002860D0(Pursuer *p);
void func_00286170(Pursuer *p);
void func_00286AA0(Pursuer *p);
void func_00286B90(Pursuer *p);
void func_00286F10(Pursuer *p);
void func_002871E0(Pursuer *p);
void func_00287380(Pursuer *p);
void func_00287620(Pursuer *p);
void func_00287950(Pursuer *p);
void func_00287B50(Pursuer *p);
void func_00287CD0(Pursuer *p);
void func_00288030(Pursuer *p);
void func_00288150(Pursuer *p);
void func_002885B0(Pursuer *p);
void func_00288970(Pursuer *p);
void func_00289050(Pursuer *p);
void func_00289280(Pursuer *p);
void func_00289500(Pursuer *p);
void func_002895B0(Pursuer *p);
void func_00289810(Pursuer *p);
void func_00289860(Pursuer *p);
void func_00289DD0(Pursuer *p);
void func_00289F50(Pursuer *p);
void func_00289FA0(Pursuer *p);
void func_0028A060(Pursuer *p);
void func_0028A100(Pursuer *p);
void func_0028A190(Pursuer *p);
void func_0028A540(Pursuer *p);
void func_0028A660(Pursuer *p);
void func_0028A700(Pursuer *p);
void func_0028A860(Pursuer *p);
void func_0028A930(Pursuer *p);
void func_0028A9E0(Pursuer *p);
void func_0028AA80(Pursuer *p);
void func_0028AB10(Pursuer *p);
void func_0028AEC0(Pursuer *p);
void func_0028B0D0(Pursuer *p);
void func_0028B340(Pursuer *p);
void func_0028B970(Pursuer *p);
void func_0028BD40(Pursuer *p);
void func_0028BEF0(Pursuer *p);
void func_0028C420(Pursuer *p);
void func_0028C560(Pursuer *p);
void func_0028C9E0(Pursuer *p);
void func_0028CD70(Pursuer *p);
void func_0028CE20(Pursuer *p);
void func_0028CF80(Pursuer *p);
void func_0028D040(Pursuer *p);
void func_0028D0C0(Pursuer *p);
void func_0028D260(Pursuer *p);
void func_0028D540(Pursuer *p);
void func_0028D5B0(Pursuer *p);
void func_0028D6E0(Pursuer *p);
void func_0028D7D0(Pursuer *p);
void func_0028D8A0(Pursuer *p);
void func_0028D950(Pursuer *p);
void func_0028DAA0(Pursuer *p);
void func_0028DBD0(Pursuer *p);
void func_0028DC40(Pursuer *p);
void func_0028DE10(Pursuer *p);
void func_0028E0C0(Pursuer *p);
void func_0028E2D0(Pursuer *p);
void func_0028E4D0(Pursuer *p);
void func_0028E8E0(Pursuer *p);
void func_0028ED20(Pursuer *p);
void func_0028ED50(Pursuer *p);
void func_0028EFC0(Pursuer *p);
void func_0028F080(Pursuer *p);
void func_0028F650(Pursuer *p);
void func_0028F840(Pursuer *p);
void func_0028FA00(Pursuer *p);
void func_0028FC10(Pursuer *p);
void func_0028FD30(Pursuer *p);
void func_0028FF30(Pursuer *p);
void func_002902E0(Pursuer *p);
void func_00290620(Pursuer *p);
void func_00290810(Pursuer *p);
void func_002908F0(Pursuer *p);
void func_00290B70(Pursuer *p);
void func_00290DF0(Pursuer *p);
void func_00291190(Pursuer *p);
void func_00291600(Pursuer *p);
void func_00291760(Pursuer *p);
void func_00291880(Pursuer *p);
void func_002919B0(Pursuer *p);
void func_00291A20(Pursuer *p);
void func_00291C30(Pursuer *p);
void func_00291C80(Pursuer *p);
void func_00291EE0(Pursuer *p);
void func_00292120(Pursuer *p);
void func_00292170(Pursuer *p);
void func_00292310(Pursuer *p);
void func_002927D0(Pursuer *p);
void func_002928B0(Pursuer *p);
void func_002934C0(Pursuer *p);
void func_00293620(Pursuer *p);
void func_00294150(Pursuer *p);
void func_00294240(Pursuer *p);
void func_002947F0(Pursuer *p);
void func_002948E0(Pursuer *p);
void func_002953F0(Pursuer *p);
void func_00295670(Pursuer *p);
void func_002961D0(Pursuer *p);
void func_00296580(Pursuer *p);
void func_00296ED0(Pursuer *p);
void func_00296FC0(Pursuer *p);
s32 func_00297160(Pursuer *p);
s32 func_00297290(Pursuer *p, f32 *table, u32 n);
s32 func_00297300(Pursuer *p, u32 dir);
s32 func_00297A70(Pursuer *p);
s32 func_00297AC0(Pursuer *p);
s32 func_00297B00(Pursuer *p);
s32 func_00297B40(Pursuer *p, s32 anim, s32 blend);
void func_00297C60(Pursuer *p);
void func_002982A0(Pursuer *p, f32 amount);
void func_002983C0(Pursuer *p);
void func_00298D20(Pursuer *p);
void func_00298FF0(Pursuer *p);
void func_00299080(Pursuer *p);
void func_002990E0(Pursuer *p);
void func_00299140(Pursuer *p);
void func_002992F0(Pursuer *p);
void func_00299300(Pursuer *p);
void func_00299370(Pursuer *p);
void func_002994B0(Pursuer *p);
void func_00299530(Pursuer *p);
void func_00299F80(Pursuer *p);
void func_0029A2A0(Pursuer *p);
void func_0029A3D0(Pursuer *p);
void func_0029A520(Pursuer *p);
void func_0029A6D0(Pursuer *p);
s32 func_0029A710(Pursuer *p);
s32 func_0029A850(Pursuer *p);
s32 func_0029A870(Pursuer *p);
s32 func_0029A8C0(Pursuer *p, s32 room);
void func_0029A940(Pursuer *p);
void func_0029AC50(Pursuer *p);
void func_0029AF20(Pursuer *p);
void func_0029B190(Pursuer *p);
u32 func_0029B4B0(Pursuer *p);
void func_0029B5C0(Pursuer *p);
void func_0029B8B0(Pursuer *p);
s32 func_0029C570(Pursuer *p);
void func_0029C8C0(Pursuer *p);
u32 func_0029CB40(Pursuer *p);
s32 func_0029CBE0(Pursuer *p, f32 *out);
s32 func_0029CD00(Pursuer *p, u32 kind, s32 slot, u32 door);
void func_0029CE40(Pursuer *p, s32 a1, s32 *out);
s32 func_0029CE50(Pursuer *p);
void func_0029CEE0(Pursuer *p, s32 room, u32 found, s32 plan, s32 side);
s32 func_0029D180(Pursuer *p, s32 room, s32 tri, u32 side);
void func_0029D3E0(Pursuer *p);
void func_0029D410(Pursuer *p, s32 sound, s32 a2, s32 a3, s32 a4, void *a5);
void func_0029D4C0(Pursuer *p, s32 anim);
void func_0029D7F0(Pursuer *p);
void func_0029DA80(Pursuer *p);
void func_0029E210(Pursuer *p);
void func_0029E390(Pursuer *p);
void func_0029E440(Pursuer *p);
void func_0029E520(Pursuer *p);
void func_0029E600(Pursuer *p);
void func_0029E610(Pursuer *p);
void func_0029E9D0(Pursuer *p);
void func_0029EAA0(Pursuer *p);
void func_0029EAB0(Pursuer *p);
void func_0029EC60(Pursuer *p);
void func_0029ED00(Pursuer *p);
s32 func_0029ED80(Pursuer *p);
void func_0029EE00(Pursuer *p);
s32 func_0029EE10(Pursuer *p);
void func_0029EE70(Pursuer *p);
void func_0029EF80(Pursuer *p, s32 id);
void func_0029F040(Pursuer *p, u32 slot);
void func_0029F120(Pursuer *p);
void func_0029F2C0(Pursuer *p);
void func_0029F350(Pursuer *p);
void func_0029F3E0(Pursuer *p);
u8 *func_0029F690(Pursuer *p);
u8 *func_0029F8C0(Pursuer *p);
void func_0029FB20(Pursuer *p);
void func_0029FBD0(Pursuer *p);
s32 func_002D7A60(void);
Pursuer *func_002D7A70(Pursuer *p, s32 flags);
void func_002D7CE0(Pursuer *p);
void func_002D7D20(Pursuer *p, s32 exit);
void func_002D7E10(Pursuer *p, s32 exit);
void func_002D7E20(Pursuer *p, Character *who);
void func_002D8120(Pursuer *p, s32 *e, f32 *a, f32 *b);
void func_002D8210(Pursuer *p, s8 situation);
void func_002D85D0(Pursuer *p);
void func_002D8690(Pursuer *p);
s32 func_002D8840(Pursuer *p, Character *who);
void func_002D8AC0(Pursuer *p);
void func_002D8CB0(Pursuer *p);
void func_002D8DF0(Pursuer *p);
void func_002D9500(Pursuer *p);
void func_002DA120(Pursuer *p);
void func_002DA4C0(Pursuer *p);
void func_002DA6B0(Pursuer *p);
void func_002DB480(Pursuer *p);
void func_002DB7F0(Pursuer *p);
void func_002DB900(Pursuer *p, s32 on);
s32 func_002DB950(Pursuer *p);
s32 func_002DB960(Pursuer *p);
s32 func_002DB990(Pursuer *p);
s32 func_002DB9E0(Pursuer *p);
s32 func_002DBA90(Pursuer *p, f32 *out);
void func_002DBD70(Pursuer *p);
void func_002DC070(Pursuer *p);
void func_002DC4E0(Pursuer *p);
Pursuer *func_002F8820(Pursuer *p, s32 flags);
f32 func_002F8940(Pursuer *p);
void func_002F8960(Pursuer *p, s32 kind, f32 *out);
void func_002F8A10(Pursuer *p);
void func_002F8A20(Pursuer *p, s8 situation);
f32 func_002F8CE0(Pursuer *p);
f32 func_002F8CF0(Pursuer *p);
void func_002F8D00(Pursuer *p);
void func_002F8D50(Pursuer *p);
u8 *func_002F9000(Pursuer *p);
u8 *func_002F9040(Pursuer *p);
void func_002F9080(Pursuer *p);
Pursuer *func_00309490(Pursuer *p, s32 flags);
s32 func_00309660(Pursuer *p);
void func_00309680(Pursuer *p);
void func_00309890(Pursuer *p);
void func_00309BA0(Pursuer *p, s32 *e, f32 *a, f32 *b);
void func_00309C90(Pursuer *p, s8 situation);
void func_0030B1E0(Pursuer *p);
void func_0030B7C0(Pursuer *p);
s32 func_0030BB70(Pursuer *p);
s32 func_0030BBF0(Pursuer *p);
s32 func_0030BC40(Pursuer *p);
s32 func_0030BC50(Pursuer *p);
void func_0030BE20(Pursuer *p);
void func_0030C230(Pursuer *p);
/* ---- end generated ---- */

/* The Pursuer destructor's body down to the Actor (each stalker's destructor sets its own vtable
 * and runs this inline): vtable +0x10 cleanup at each level, the model freed for slots 3..5. */
static inline void Pursuer_DestroyBase(Pursuer *p) {
    p->c.a.vtbl = D_0046D810;
    VCALL(p, 0x10, void (*)(Pursuer *))(p);
    if ((u32)p->c.a.slot >= 3 && (u32)p->c.a.slot < 6) {
        void **m = p->c.motion;

        if (m != NULL) {
            if (m != NULL) {
                VCALL(m, 0x8, void (*)(void *, s32))(m, 1);
            }
            p->c.motion = NULL;
        }
    }
    if (p != NULL) {
        p->c.a.vtbl = D_0046C220;
        VCALL(p, 0x10, void (*)(Pursuer *))(p);
        if (p != NULL) {
            p->c.a.vtbl = D_00469C60;
            if (p != NULL) {
                p->c.a.vtbl = D_00469C20;
            }
        }
    }
}

/* ---- helpers shared by the pursuer files ---- */

/* keep walking until the animation (+0x550) is over; returns 1 while walking */
static inline s32 Pursuer_WalkOn(Pursuer *p) {
    s32 over = MOTION_AT(p, 0x550, f32) <= 0.0f;

    if (((over ^ 1) & 0xFF) == 1) {
        if (PU(p, 0x15C0, u8) != 0xFF) {
            if (p->c.unk128 < p->c.unk124) {
                func_00214620(p, p->c.unk128);
            }
        } else {
            func_00125A10(&p->c);
        }
        return 1;
    }
    return 0;
}

/* play `anim` unless it is already playing (or ended and loops) */
static inline void Pursuer_PlayAnim(Pursuer *p, s32 anim) {
    u8 *m = p->c.motion;

    if (anim == AT(m, 0x55C, s32)) {
        s32 over = AT(m, 0x550, f32) <= 0.0f;

        if ((over ^ 1) & 0xFF) {
            return;
        }
        if (((AT(AT(m, 0x6A4, u8 *), 0x18, u32) & MOTION_KEY_END) != 0) != 1) {
            s32 i = func_001F4710(m, anim);
            u16 fl = i != -1 ? AT(AT(m, 0x874, u8 *) + i * 6, 0x4, u16) : 0;

            if (fl & 4) {
                return;
            }
        }
        func_002DDED0(p->c.motion, anim, -1);
    } else {
        func_002DDED0(m, anim, -1);
    }
}

/* play `anim`: restarted like Pursuer_PlayAnim if it is the current one, else blended in */
static inline void Pursuer_PlayAnimBlend(Pursuer *p, s32 anim) {
    u8 *m = p->c.motion;

    if (AT(m, 0x55C, s32) == anim) {
        Pursuer_PlayAnim(p, anim);
    } else {
        func_002DDE20(m, anim, -1);
    }
}

static inline void Pursuer_SetMove(Pursuer *p, PTMF *m) {
    ptmf_set((PTMF *)((u8 *)p + 0x17A0), m);
    PU(p, 0x17AC, s32) = AT(m, 0xC, s32);
    p->c.moveMode = AT(m, 0x10, s32);
    p->c.moveSub = AT(m, 0x14, s32);
    p->c.unk1530 = 0;
    p->c.unk1538 = 0;
    p->c.unk1534 = 0;
}

/* forget the search route (see func_0027E5D0) */
static inline void Pursuer_ClearRoute(Pursuer *p) {
    s32 k;

    for (k = 0; k < 8; k++) {
        PU(p, 0x15E0 + k * 8, s32) = -1;
        PU(p, 0x15E4 + k * 8, u8) = 0;
    }
    PU(p, 0x1620, u8) = 0xFF;
    PU(p, 0x1621, u8) = 0xFF;
    PU(p, 0x1794, s32) = 0;
}



/* ---- the stalkers' frame update (vtable +0x30), shared by their own versions ---- */

/* the start: blocking flags, senses (func_00215D80; while the behaviour is fresh, +0x16F6, they
   are recomputed, else forgotten), vtable +0x120, the threat */
static inline void Stalker_ThinkStart(Pursuer *p) {
    VCALL(p, 0x84, void (*)(Pursuer *))(p);
    p->c.a.navMask = p->c.a.unk2B == 1 ? 8 : VCALL(p, 0xA8, u32 (*)(Pursuer *))(p);
    p->c.pathReq->mask = p->c.a.navMask;
    func_00215D80(p);
    if (PU(p, 0x16F6, u8) == 1) {
        func_002177D0(p);
    } else {
        PU(p, 0x1544, u8) = 0;
        PU(p, 0x1545, u8) = 0;
        PU(p, 0x1546, u8) = 0;
        PU(p, 0x16CB, u8) = 0;
        PU(p, 0x16CC, u8) = 0;
    }
    VCALL(p, 0x120, void (*)(Pursuer *))(p);
    func_00297C60(p);
}

/* the end: frames in state/behaviour (+0x1784, +0x1780), the room wait +0x1664 / the route rest
   +0x17B4, the stand-down +0x1790, the cry hold +0x178C, the stun, the route growing back
   +0x1794; then the model (+0x40) and the stance (+0x100) */
static inline void Stalker_ThinkEnd(Pursuer *p) {
    if (PU(p, 0x1784, s32) != -1) {
        PU(p, 0x1784, s32)++;
    }
    if (PU(p, 0x1780, s32) != -1) {
        PU(p, 0x1780, s32)++;
    }
    if (PU(p, 0x1664, s32) != 0) {
        PU(p, 0x1664, s32)--;
    } else if (PU(p, 0x17B4, s32) != 0) {
        PU(p, 0x17B4, s32)--;
        if (PU(p, 0x17B4, s32) == 0) {
            PU(p, 0x1620, u8) = PU(p, 0x1621, u8);
        }
    }
    if (PU(p, 0x1790, s32) != 0 && p->c.moveSub != 9) {
        PU(p, 0x1790, s32)--;
        if (PU(p, 0x1790, s32) == 0) {
            p->c.a.unkC4 = 0;
            PU(p, 0x16F5, u8) = 0;
        }
    }
    if (PU(p, 0x178C, s32) != 0) {
        PU(p, 0x178C, s32)--;
        if (PU(p, 0x178C, s32) == 0) {
            PU(p, 0x1760, u8) = 0;
        }
    }
    func_00129AF0(p);
    if (PU(p, 0x1794, s32) != 0) {
        PU(p, 0x1794, s32)--;
        if (PU(p, 0x1794, s32) == 0 && PU(p, 0x1620, u8) < PU(p, 0x1621, u8)) {
            PU(p, 0x1621, u8) = PU(p, 0x1620, u8) + 1;
        }
    }
    VCALL(p, 0x40, void (*)(Pursuer *))(p);
    VCALL(p, 0x100, void (*)(Pursuer *))(p);
}


/* ---- the hit effect (0xE60 bytes, vtable 0x470F30, its part at +0xC10) the stalkers leave
   where a blow lands ---- */
extern void *D_00470F30[], *D_00469D00[], *D_0046FC30[];

typedef struct {
    f32 pos[4];
    u32 kind;      /* Daniella 0xFE; Riccardo 1 on Hewie, else 0 */
    f32 big;       /* 1.0 or 0 */
} HitEffectParams;

static inline void HitEffect_Init(void **obj) {
    obj[0] = D_00470F30;
    obj[0xC10 / 4] = D_00469D00;
    ((s32 *)obj)[0xC14 / 4] = -1;
    obj[0xC10 / 4] = D_0046FC30;
}

static inline void HitEffect_Spawn(HitEffectParams *hp) {
    u8 *mgr = D_0044E578;

    func_002D6090(mgr, Effect_New(mgr, 0xE60, HitEffect_Init), hp);
}

#endif
