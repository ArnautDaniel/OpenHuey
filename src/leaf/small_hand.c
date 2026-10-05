/* Small methods of assorted room-creature, effect and prop classes and room conditions, written
 * by hand from their instructions (2026-10-05). */
#include "common.h"
#include "game.h"
#include "progress.h"
#include "actor.h"
#include "pursuer.h"

extern void func_002E56C0(u8 *quad);
extern VObject *D_0044E558;   /* the doors */
extern VObject *gFileLoader;

/* rooms 0xC0 / 0xC1 / 0xC2 / 0xC3 (D_0042E3E0, D_0043F098, D_0043F8B8, D_004400C8): the timer at
 * progress +0x764 has run out */
s32 func_0032DD10(void) {
    return func_002EC410((u8 *)gProgress + 0x764) == 0;
}

s32 func_0034A490(void) {
    return func_002EC410((u8 *)gProgress + 0x764) == 0;
}

s32 func_0034A5F0(void) {
    return func_002EC410((u8 *)gProgress + 0x764) == 0;
}

s32 func_0034AAE0(void) {
    return func_002EC410((u8 *)gProgress + 0x764) == 0;
}

/* room 0x91 (D_00436CA0): door 0's +0x68 (0) */
s32 func_00341AD0(void) {
    VCALL(D_0044E558, 0x68, void (*)(VObject *, s32, s32))(D_0044E558, 0, 0);
    return 1;
}

s32 func_0035B000(void) {
    return 0x41000;
}

/* (a pursuer class) its threat: 50 in action 0x1001, else the pursuers' func_0029CB40 */
u32 func_00330D10(Pursuer *p) {
    if (PU(p, 0x175C, s32) == 0x1001) {
        return 0x32;
    }
    return func_0029CB40(p);
}

/* (a pursuer class) func_0029F120, then its model's +0x34 (1) */
void func_00346010(Pursuer *p) {
    func_0029F120(p);
    VCALL(p->c.motion, 0x34, void (*)(void *, s32))(p->c.motion, 1);
}

/* an effect's quad drawer (at `drawer`) given the current one of its records (`size` apart from
 * +0x10, the index at `idx`), and drawn */
static inline __attribute__((always_inline)) void quad_step(u8 *o, u32 drawer, u32 idx, u32 size) {
    AT(o, drawer + 0x10, u8 *) = o + AT(o, idx, s32) * size + 0x10;
    func_002E56C0(o + drawer);
}

void func_0033C110(u8 *o) {
    quad_step(o, 0x6010, 0x7450, 0x3000);
}

void func_0035F9B0(u8 *o) {
    quad_step(o, 0x550, 0x5C0, 0x2A0);
}

void func_003600D0(u8 *o) {
    quad_step(o, 0x70, 0xAC, 0x30);
}

void func_00368E80(u8 *o) {
    quad_step(o, 0xC10, 0xE58, 0x600);
}

void func_00369AE0(u8 *o) {
    quad_step(o, 0x1810, 0x20D8, 0xC00);
}

/* (+0x18) set: { u16, u16 } into +0x6 / +0x8 and on (+0x4 0); none: off (+0x4 1) */
void func_0035CEA0(u8 *o, u16 *prm) {
    if (prm == NULL) {
        AT(o, 0x4, u8) = 1;
        return;
    }
    AT(o, 0x4, u8) = 0;
    AT(o, 0x6, u16) = prm[0];
    AT(o, 0x8, u16) = prm[1];
}

/* (+0x10) still on */
s32 func_0035D180(u8 *o) {
    return AT(o, 0x4, u8) != 1;
}

void func_0035D190(u8 *o) {
    AT(o, 0x4, u8) = 0;
}

/* (+0x18) set: { +0xC, +0x8 (f32) }, +0x4 -1 */
void func_00360BC0(u8 *o, s32 *prm) {
    if (prm != NULL) {
        AT(o, 0xC, s32) = prm[0];
        AT(o, 0x8, s32) = prm[1];
        AT(o, 0x4, s32) = -1;
    }
}

/* (+0x10) counts +0x4 up to 5; 0 then */
s32 func_00361220(u8 *o) {
    if (AT(o, 0x4, s32) == 5) {
        return 0;
    }
    AT(o, 0x4, s32)++;
    return 1;
}

void func_00361250(u8 *o) {
    AT(o, 0x4, s32) = -1;
}

void func_003636C0(u8 *o) {
    AT(o, 0x16EE, u8) = 1;
}

void func_00371E00(u8 *o) {
    AT(o, 0x10C0, s32) = 0;
    AT(o, 0x10C4, u8) = 0;
}

/* (+0x18) set: +0x10 = the first word */
void func_00378550(u8 *o, s32 *prm) {
    if (prm != NULL) {
        AT(o, 0x10, s32) = prm[0];
    }
}

void func_0037BE70(u8 *o) {
    AT(o, 0x44, u8) = 0;
}

/* the file loader's +0x28 (0x4000000) is 2 */
s32 func_00260130(void) {
    return VCALL(gFileLoader, 0x28, s32 (*)(VObject *, u32))(gFileLoader, 0x4000000) == 2;
}
