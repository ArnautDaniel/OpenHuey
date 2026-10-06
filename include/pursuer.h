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
#include "debilitas.h"
#include "model.h"
#include "pursuer_ai.h"

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
/* ---- end engine ---- */

/* ---- generated from the definitions (tools: protos.py) ---- */
/* ---- end generated ---- */

/* pursuer.c */
extern void func_00283EE0(Pursuer *p);
extern void func_002992F0(Pursuer *p);
extern s32 func_0029A850(Pursuer *p);
extern void func_0029D3E0(Pursuer *p);
extern void func_0029CE40(Pursuer *p, s32 a1, s32 *out);
extern void func_0029E600(Pursuer *p);
extern void func_0029EAA0(Pursuer *p);
extern void func_0029EE00(Pursuer *p);
extern void func_0028ED20(Pursuer *p);
extern void func_0027E5A0(Pursuer *p, u32 tri);
extern void func_0029A6D0(Pursuer *p);
extern void func_0027E560(Pursuer *p);
extern s32 func_00297AC0(Pursuer *p);
extern s32 func_00297B00(Pursuer *p);
extern s32 func_00297A70(Pursuer *p);
extern void func_00292120(Pursuer *p);
extern void func_00289810(Pursuer *p);
extern void func_00289F50(Pursuer *p);
extern s32 func_0029A870(Pursuer *p);
extern void func_00291C30(Pursuer *p);
extern void func_00285AB0(Pursuer *p);
extern void func_002990E0(Pursuer *p);
extern void func_002801B0(Pursuer *p);
extern void func_00299080(Pursuer *p);
extern s32 func_0029EE10(Pursuer *p);
extern void **func_00278490(void **obj, s32 flags);
extern s32 func_00297290(Pursuer *p, f32 *table, u32 n);
extern void func_00299300(Pursuer *p);
extern void func_0027CE80(Pursuer *p);
extern s32 func_00283870(Pursuer *p);
extern void func_0028D540(Pursuer *p);
extern void func_0028DBD0(Pursuer *p);
extern void func_002919B0(Pursuer *p);
extern void func_0027D130(Pursuer *p);
extern void func_0028D040(Pursuer *p);
extern s32 func_0029A8C0(Pursuer *p, s32 room);
extern s32 func_0029ED80(Pursuer *p, u32 tri, const f32 *heading, f32 *pos);
extern void func_002994B0(Pursuer *p);
extern void func_0029ED00(Pursuer *p);
extern void func_00298FF0(Pursuer *p);
extern void func_0029F2C0(Pursuer *p);   /* a character's quick unload (keeps its model) */
extern void func_0029F350(Pursuer *p);
extern void func_0028A100(Pursuer *p);
extern void func_0028AA80(Pursuer *p);
extern void func_0028A060(Pursuer *p);
extern void func_0028A9E0(Pursuer *p);
extern s32 func_0029CE50(Pursuer *p);
extern void func_002860D0(Pursuer *p);
extern void func_0029EC60(Pursuer *p);
extern void func_0027DFA0(Pursuer *p);
extern void func_0028A660(Pursuer *p);
extern u32 func_0029CB40(Pursuer *p);
extern void func_00283A50(Pursuer *p, s32 *bones, f32 *a, f32 *b);
extern void func_0029FB20(Pursuer *p);
extern void func_0027D1B0(Pursuer *p);
extern void func_00289500(Pursuer *p);
extern void func_0028CD70(Pursuer *p);
extern void func_0028D8A0(Pursuer *p);
extern void func_0029D410(Pursuer *p, s32 sound, s32 a2, s32 a3, s32 a4, void *a5);
extern void func_002837C0(Pursuer *p, u32 mask);
extern void func_0028A930(Pursuer *p);
extern void func_00289FA0(Pursuer *p);
extern void func_0028CF80(Pursuer *p);
extern void func_0028EFC0(Pursuer *p);
extern void func_0029EF80(Pursuer *p, s32 id);
extern void func_0027D810(Pursuer *p);
extern void func_0028A860(Pursuer *p);
extern void func_0028D7D0(Pursuer *p);
extern void func_0029E9D0(Pursuer *p);
extern void func_002927D0(Pursuer *p);
extern void func_0029E520(Pursuer *p);
extern u8 *func_002CF140(Pursuer *p);
extern u8 *func_002DC460(Pursuer *p);
extern void func_002DD310(u8 *p, f32 tx, f32 ty, f32 sx, f32 sy);
extern void func_00290810(Pursuer *p);
extern void func_0029E440(Pursuer *p);
extern void func_0029F040(Pursuer *p, u32 slot);
extern void func_00286AA0(Pursuer *p);
extern void func_0028D6E0(Pursuer *p);
extern void func_00294150(Pursuer *p);
extern void func_002947F0(Pursuer *p);
extern void func_00296ED0(Pursuer *p);
extern void func_0029E390(Pursuer *p);
extern s32 func_00284440(Pursuer *p);
extern u32 func_0029B4B0(Pursuer *p);
extern void func_0029EE70(Pursuer *p);
extern void func_002982A0(Pursuer *p, f32 amount);
extern void func_00280090(Pursuer *p);
extern s32 func_00297B40(Pursuer *p, s32 anim, s32 blend);
extern void func_00288030(Pursuer *p);
extern void func_0028A540(Pursuer *p);
extern void func_0028FC10(Pursuer *p);
extern void func_00291880(Pursuer *p);
extern void func_0028DAA0(Pursuer *p);
extern s32 func_0029CBE0(Pursuer *p, f32 *out);
extern void func_0027E440(Pursuer *p);
extern void func_00291760(Pursuer *p);
extern s32 func_00297160(Pursuer *p);
extern void func_0029A2A0(Pursuer *p);
extern Pursuer *func_00179970(Pursuer *p, s32 flags);
extern Pursuer *func_00172810(Pursuer *p, s32 flags);
extern void *func_00172910(void *p, s32 arg);
extern void *func_00172960(void *p, s32 arg);
extern void *func_001729B0(void *p, s32 arg);
extern void *func_00172A00(void *p, s32 arg);
extern void *func_00172A50(void *p, s32 arg);
extern void *func_00172AA0(void *p, s32 arg);
extern void *func_00172AF0(void *p, s32 arg);
extern void *func_00172B40(void *p, s32 arg);
extern void *func_00172B90(void *p, s32 arg);
extern void *func_00172BE0(void *p, s32 arg);
extern void *func_00172C30(void *p, s32 arg);
extern void *func_00172C80(void *p, u32 id, u32 arg);
extern void func_0028D5B0(Pursuer *p);
extern void func_00299370(Pursuer *p);
extern s32 func_0029CD00(Pursuer *p, u32 kind, s32 slot, u32 door);
extern void func_00285360(Pursuer *p);
extern void func_0028C420(Pursuer *p);
extern s32 func_00283EF0(Pursuer *p);
extern void func_0028D950(Pursuer *p);
extern void func_0029A3D0(Pursuer *p);
extern void func_0028CE20(Pursuer *p);
extern void func_002934C0(Pursuer *p);
extern void func_0028A700(Pursuer *p);
extern void func_00291600(Pursuer *p);
extern void func_00283AE0(Pursuer *p);
extern f32 func_002838E0(Pursuer *p);
extern void func_0029E210(Pursuer *p);
extern void func_00287B50(Pursuer *p);
extern void func_00289DD0(Pursuer *p);
extern void func_00284FD0(Pursuer *p);
extern s32 func_0027CA00(Pursuer *p);
extern void func_0029F120(Pursuer *p);
extern void func_002871E0(Pursuer *p);
extern void func_0028D0C0(Pursuer *p);
extern void func_00292170(Pursuer *p);
extern void func_00296FC0(Pursuer *p);
extern void func_00299140(Pursuer *p);
extern void func_0029EAB0(Pursuer *p);
extern void func_0029A520(Pursuer *p);
extern void func_0028BD40(Pursuer *p);
extern void func_0028F840(Pursuer *p);
extern void func_0027E5D0(Pursuer *p, s32 n);
extern void func_0028DC40(Pursuer *p);
extern void func_0027DDB0(Pursuer *p);
extern void func_0028F650(Pursuer *p);
extern void func_00290620(Pursuer *p);
extern void func_002854A0(Pursuer *p);
extern void func_002858B0(Pursuer *p);
extern void func_0027EEA0(Pursuer *p);
extern void func_00287950(Pursuer *p);
extern void func_0028E2D0(Pursuer *p);
extern void func_0027FE90(Pursuer *p);
extern void func_0028FD30(Pursuer *p);
extern void func_00291A20(Pursuer *p);
extern void func_0028AEC0(Pursuer *p);
extern void func_00285150(Pursuer *p);
extern void func_0028E0C0(Pursuer *p);
extern void func_002856A0(Pursuer *p);
extern void func_0028FA00(Pursuer *p);
extern void func_0027FC70(Pursuer *p);
extern u8 *func_0029F690(Pursuer *p);
extern void func_00289050(Pursuer *p);
extern void func_0027CEF0(Pursuer *p);
extern void func_00291EE0(Pursuer *p);
extern void func_0027D5B0(Pursuer *p);
extern void func_002895B0(Pursuer *p);
extern void func_0027D8D0(Pursuer *p);
extern void func_00291C80(Pursuer *p);
extern s32 func_0029D180(Pursuer *p, s32 room, s32 tri, u32 side);
extern u8 *func_0029F8C0(Pursuer *p);
extern void func_0028B0D0(Pursuer *p);
extern void func_0028ED50(Pursuer *p);
extern void func_0029AF20(Pursuer *p);
extern void func_0027DB30(Pursuer *p);
extern void func_002953F0(Pursuer *p);
extern void func_002908F0(Pursuer *p);
extern void func_00290B70(Pursuer *p);
extern void func_00289280(Pursuer *p);
extern void func_0029C8C0(Pursuer *p);
extern void func_0029D7F0(Pursuer *p);
extern void func_00283C50(Pursuer *p);
extern void func_0027F350(Pursuer *p);
extern void func_0029CEE0(Pursuer *p, s32 room, u32 found, s32 plan, s32 side);
extern void func_00287380(Pursuer *p);
extern void func_00280210(Pursuer *p, s32 exit);
extern void func_0029F3E0(Pursuer *p);
extern s32 func_0027F0A0(Pursuer *p, u32 exit);
extern void func_0028DE10(Pursuer *p);
extern void func_00298D20(Pursuer *p);
extern void func_00285B10(Pursuer *p);
extern void func_0029AC50(Pursuer *p);
extern void func_002849B0(Pursuer *p);
extern void func_00286F10(Pursuer *p);
extern void func_0028D260(Pursuer *p);
extern void func_0027CB90(Pursuer *p, u32 exit);
extern s32 func_00285DE0(Pursuer *p, u32 kind, f32 *heading, f32 *pos);
extern void func_0029B5C0(Pursuer *p);
extern void func_0029A940(Pursuer *p);
extern void func_0029B190(Pursuer *p);
extern void func_00299F80(Pursuer *p);
extern void func_00287620(Pursuer *p);
extern void func_0029D4C0(Pursuer *p, s32 anim);
extern void func_002902E0(Pursuer *p);
extern void func_0029FBD0(Pursuer *p);
extern void func_0027D260(Pursuer *p);
extern s32 func_00284C80(Pursuer *p);
extern s32 func_0029C570(Pursuer *p);
extern void func_0027E790(Pursuer *p);
extern void func_00287CD0(Pursuer *p);
extern void func_00286B90(Pursuer *p);
extern void func_0028C9E0(Pursuer *p);
extern void func_00290DF0(Pursuer *p);
extern s32 func_0029A710(Pursuer *p);
extern void func_0028FF30(Pursuer *p);
extern void func_0027EAF0(Pursuer *p);
extern void func_0028A190(Pursuer *p);
extern void func_0028AB10(Pursuer *p);
extern void func_002961D0(Pursuer *p);
extern void func_0029E610(Pursuer *p);
extern void func_002885B0(Pursuer *p);
extern void func_0028B970(Pursuer *p);
extern void func_0027E040(Pursuer *p);
extern void func_00284040(Pursuer *p);
extern void func_0028E4D0(Pursuer *p);
extern void func_0028E8E0(Pursuer *p);
extern void func_00288150(Pursuer *p);
extern void func_00284540(Pursuer *p);
extern void func_00291190(Pursuer *p);
extern void func_00278EB0(Pursuer *p);
extern void func_0028C560(Pursuer *p);
extern void func_00292310(Pursuer *p);
extern void func_0027A1E0(Pursuer *p);
extern void func_002804B0(Pursuer *p);
extern void func_0028BEF0(Pursuer *p);
extern void func_00289860(Pursuer *p);
extern void func_00294240(Pursuer *p);
extern void func_0028F080(Pursuer *p);
extern s32 func_00297300(Pursuer *p, u32 dir);
extern void func_00297C60(Pursuer *p);
extern void func_0027F5F0(Pursuer *p);
extern void func_00288970(Pursuer *p);
extern void func_0027A6A0(Pursuer *p);
extern void func_0028B340(Pursuer *p);
extern void func_0029DA80(Pursuer *p);
extern void func_002983C0(Pursuer *p);
extern void func_00299530(Pursuer *p);
extern void func_00286170(Pursuer *p);
extern void func_00296580(Pursuer *p);
extern void func_002815E0(Pursuer *p, u32 door);
extern void func_0027AD80(Pursuer *p, u32 door);
extern void func_00293620(Pursuer *p);
extern void func_002948E0(Pursuer *p);
extern void func_00295670(Pursuer *p);
extern void func_002928B0(Pursuer *p);
extern void func_002809E0(Pursuer *p);
extern void func_0029B8B0(Pursuer *p);
extern void func_00279350(Pursuer *p);
extern void func_0027B810(Pursuer *p, u32 door);
extern void func_00282010(Pursuer *p, u32 door);
extern void func_00127A40(Pursuer *p, u32 kind);
extern void func_00127B80(Pursuer *p);
extern s32 func_00127BB0(Pursuer *p);
extern s32 func_00127BC0(Pursuer *p);
extern s32 func_00127C00(Pursuer *p);
extern u8 *func_0012BFB0(Pursuer *p);
extern u8 *func_00320150(Pursuer *p);
extern u8 *func_00331200(Pursuer *p);
extern u8 *func_00347290(Pursuer *p);
extern u8 *func_00348620(Pursuer *p);
extern u8 *func_003495B0(Pursuer *p);
extern u8 *func_0034D980(Pursuer *p);
extern void *func_00172DE0(void *p, s32 arg, u32 id);
extern void *func_00172E20(void *p, s32 arg);
extern void *func_00172E70(void *p, s32 arg);
extern void *func_00172EC0(void *p, s32 arg);
extern void *func_00172F10(void *p, s32 arg);
extern void *func_00172F60(void *p, s32 arg);
extern void *func_00172FB0(void *p, u32 id, u32 arg);
extern void *func_00173110(void *p, s32 arg, u32 id);
extern void *func_00173150(void *p, s32 arg);
extern void *func_001731A0(void *p, s32 arg);
extern void *func_001731F0(void *p, s32 arg);
extern void *func_00173240(void *p, s32 arg);
extern void *func_00173290(void *p, s32 arg);
extern void *func_001732E0(void *p, s32 arg);
extern void *func_00173330(void *p, s32 arg);
extern void *func_00173380(void *p, s32 arg);
extern void *func_001733D0(void *p, s32 arg);
extern void *func_00173420(void *p, s32 arg);
extern void *func_00173470(void *p, s32 arg);
extern void *func_001734C0(void *p, s32 arg);
extern void *func_00173510(void *p, s32 arg);
extern void *func_00173560(void *p, s32 arg);
extern void *func_001735B0(void *p, s32 arg);
extern void *func_00173600(void *p, s32 arg);
extern u8 *func_00309410(Pursuer *p);
extern u8 *func_0030C1B0(Pursuer *p);

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
   +0x1794 (Stalker_ThinkTimers); then the model (+0x40) and the stance (+0x100) */
static inline void Stalker_ThinkTimers(Pursuer *p) {
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
}

static inline void Stalker_ThinkEnd(Pursuer *p) {
    Stalker_ThinkTimers(p);
    VCALL(p, 0x40, void (*)(Pursuer *))(p);
    VCALL(p, 0x100, void (*)(Pursuer *))(p);
}

#endif
