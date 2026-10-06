/* Small methods of assorted room-creature, effect and prop classes and room conditions, written
 * by hand from their instructions (2026-10-05). */
#include "common.h"
#include "game.h"
#include "progress.h"
#include "actor.h"
#include "pursuer.h"

extern void func_002E56C0(u8 *quad);
extern VObject *gDoors;   /* the doors */
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
    VCALL(gDoors, 0x68, void (*)(VObject *, s32, s32))(gDoors, 0, 0);
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

/* ---- second batch ---- */

extern void *func_00261090(u8 *items, u32 id, s32 n);
extern s32 func_00303E60(u8 *o, s32 a1);
extern void func_002927D0(Pursuer *p);
extern void func_00121220(u8 *o);
extern void func_002990E0(Pursuer *p);
extern void func_00299080(Pursuer *p);
extern void func_00125CC0(Character *c);
extern void func_002E2300(Character *c);
extern void func_0029E520(Pursuer *p);
extern void *func_00226810(void);
extern void *func_0016FCD0(u8 *m);
extern void func_00320A60(u8 *m);
extern void func_003206A0(u8 *m);
extern void *D_00472700[], *D_00474460[];
extern const u8 D_0044BF10[];
extern s32 D_003E5260, D_003E5264;
extern void *D_01976F98;
extern const u8 D_004573C0[];
extern s64 D_003EA900;   /* the timer's rate (ticks a second) */

/* the first free (0) of the 64 words in row `row` (0x100 bytes from +0x12E0); 64 if none */
u32 func_002605F0(u8 *o, u8 row) {
    u32 i;

    for (i = 0; i < 0x40; i++) {
        if (AT(o, 0x12E0 + row * 0x100 + i * 4, s32) == 0) {
            break;
        }
    }
    return i;
}

/* an item `id` (one, func_00261090) with its 8 bytes at +0x10 */
void *func_00261000(u8 *items, u32 id, u64 *data) {
    u8 *it = func_00261090(items, id, 1);

    if (it != NULL) {
        AT(it, 0x10, u64) = *data;
    }
    return it;
}

void func_0035C9E0(u8 *o) {
    AT(o, 0x5, u8) = 0xFF;
}

/* (the scene) its part +0x97980's func_00303E60 */
s32 func_00384C50(u8 *g, s32 a1) {
    return func_00303E60(g + 0x97980, a1);
}

s32 func_00384C60(u8 *g) {
    return AT(g, 0x97A8F, s8);
}

/* bit n of the scene's 0x97740 bitmap set / tested */
void func_00384C70(u8 *g, s32 n) {
    AT(g, 0x97740 + (n >> 5) * 4, u32) |= 1u << (n & 0x1F);
}

s32 func_00384CB0(u8 *g, s32 n) {
    return (AT(g, 0x97740 + (n >> 5) * 4, u32) & (1u << (n & 0x1F))) != 0;
}

u8 func_0038A2C0(void *o, u8 i) {
    return D_0044BF10[i];
}

/* model classes D_00472700 / D_00474460 over the plain one (func_0016FCD0) */
void *func_0038C890(u8 *m) {
    func_0016FCD0(m);
    AT(m, 0x0, void **) = D_00472700;
    return m;
}

void *func_0038C8D0(u8 *m) {
    func_0016FCD0(m);
    AT(m, 0x0, void **) = D_00474460;
    return m;
}

/* the first free (0) of the 128 halfwords from +0x15F8; 128 if none */
u32 func_003941C0(u8 *o) {
    u32 i;

    for (i = 0; i < 0x80; i++) {
        if (AT(o, 0x15F8 + i * 2, u16) == 0) {
            break;
        }
    }
    return i;
}

/* (possibly dead code: nothing in the game references it) */
void func_00226790(s32 v) {
    D_003E5260 = v;
}

/* (possibly dead code: nothing in the game references it) */
s32 func_002267A0(void) {
    return D_003E5260;
}

/* (possibly dead code: nothing in the game references it) */
void func_002267B0(s32 v) {
    D_003E5264 = v;
}

/* (possibly dead code: nothing in the game references it) */
s32 func_002267C0(void) {
    return D_003E5264;
}

/* the value behind func_00226810 set / read */
void func_00226820(s32 v) {
    AT(func_00226810(), 0x0, s32) = v;
}

s32 func_00226848(void) {
    return AT(func_00226810(), 0x0, s32);
}

/* D_01976F98 = D_004573C0, func_00226810's value 0, then 0x80 */
/* (possibly dead code: nothing in the game references it) */
void func_002267D0(void) {
    D_01976F98 = (void *)D_004573C0;
    AT(func_00226810(), 0x0, s32) = 0;
    func_00226820(0x80);
}

/* ---- the timer: ticks at D_003EA900 a second ---- */

extern s64 func_0011CE88(s64 a, s64 b);   /* __divdi3 */

/* (possibly dead code: nothing in the game references it) */
s64 func_0025C2F8(s64 ticks) {
    return func_0011CE88(ticks, D_003EA900);
}

/* in microseconds, milliseconds, seconds */
/* (possibly dead code: nothing in the game references it) */
f32 func_0025C320(s32 ticks) {
    return (f32)ticks * 1000000.0f / (f32)(s32)D_003EA900;
}

/* (possibly dead code: nothing in the game references it) */
f32 func_0025C360(s32 ticks) {
    return (f32)ticks * 1000.0f / (f32)(s32)D_003EA900;
}

/* (possibly dead code: nothing in the game references it) */
f32 func_0025C3A0(s32 ticks) {
    return (f32)ticks / (f32)(s32)D_003EA900;
}

void func_0025C3D0(s64 rate) {
    D_003EA900 = rate;
}

/* a measure: { sum, min, max, count } cleared / one more value */
/* (possibly dead code: nothing in the game references it) */
void func_0025C3E0(u8 *s) {
    AT(s, 0x18, s32) = 0;
    AT(s, 0x8, s64) = (s64)((u64)-1 >> 1);
    AT(s, 0x0, s64) = 0;
    AT(s, 0x10, s64) = 0;
}

/* (possibly dead code: nothing in the game references it) */
void func_0025C400(u8 *s, s64 v) {
    s64 lo = AT(s, 0x8, s64), hi = AT(s, 0x10, s64);

    if (hi < v) {
        hi = v;
    }
    if (v < lo) {
        lo = v;
    }
    AT(s, 0x18, s32)++;
    AT(s, 0x0, s64) += v;
    AT(s, 0x8, s64) = lo;
    AT(s, 0x10, s64) = hi;
}

/* (pursuer classes) their func_002990E0 / func_00299080 with +0x17C8 on / off */
void func_0031F230(Pursuer *p) {
    func_002990E0(p);
    PU(p, 0x17C8, u8) = 1;
}

void func_0031F260(Pursuer *p) {
    func_00299080(p);
    PU(p, 0x17C8, u8) = 0;
}

/* +0x17C4 / +0x17C8 = 150, then func_002927D0 */
void func_0031FBE0(Pursuer *p) {
    PU(p, 0x17C4, s32) = 0x96;
    func_002927D0(p);
}

void func_0034BFE0(Pursuer *p) {
    PU(p, 0x17C8, s32) = 0x96;
    func_002927D0(p);
}

/* its quad drawer (+0x40) on its one record (+0x10), drawn */
void func_0031E980(u8 *o) {
    AT(o, 0x50, u8 *) = o + 0x10;
    func_002E56C0(o + 0x40);
}

/* (a creature class) func_00125CC0, its own block +0x61 0, +0x63 0xFF, mode 2 */
void func_0032A890(Character *c) {
    u8 *k = (u8 *)c + 0x1540;

    func_00125CC0(c);
    AT(k, 0x61, u8) = 0;
    AT(k, 0x63, u8) = 0xFF;
    AT(k, 0x0, s32) = 2;
}

/* +0x20, then func_002E2300 */
void func_0032C000(Character *c) {
    VCALL(c, 0x20, void (*)(Character *))(c);
    func_002E2300(c);
}

/* +0xE8 counted, then func_00121220 */
void func_00355940(u8 *o) {
    AT(o, 0xE8, s32)++;
    func_00121220(o);
}

/* a model's +0x850 on, then func_00320A60 and func_003206A0 */
void func_00320C70(u8 *m) {
    AT(m, 0x850, u8) = 1;
    func_00320A60(m);
    func_003206A0(m);
}

/* (as func_00346010) */
void func_00348980(Pursuer *p) {
    func_0029F120(p);
    VCALL(p->c.motion, 0x34, void (*)(void *, s32))(p->c.motion, 1);
}

f32 func_003659B0(void) {
    return 18.0f;
}

f32 func_003659C0(void) {
    return 12.0f;
}

/* func_0029E520, then +0x31C (1) */
void func_00365CD0(Pursuer *p) {
    func_0029E520(p);
    VCALL(p, 0x31C, void (*)(Pursuer *, s32))(p, 1);
}
