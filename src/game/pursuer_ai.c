/* Pursuer methods and helpers, code 0x211C80..0x219460, and the stalker vtables' defaults
 * (0x179600..0x179970). See include/pursuer.h. */
#include "common.h"
#include "pursuer.h"

extern VObject *D_0044E558;   /* doors */
extern VObject *D_0044E568;   /* rooms */

/* ---- defaults shared by the stalker vtables (0x179600..0x179970) ---- */

extern void func_0029FB20(Pursuer *p);

void func_00179600(Pursuer *p) {
    func_0029FB20(p);
}

/* vtable +0x11C */
s32 func_00179610(Pursuer *p) {
    return 0;
}

/* vtable +0x2AC .. +0x2B8: nothing */
void func_00179710(Pursuer *p) {
}

void func_00179720(Pursuer *p) {
}

void func_00179730(Pursuer *p) {
}

void func_00179740(Pursuer *p) {
}

/* vtable +0x2C0: timer +0x1660 to 600 frames (10 s) */
void func_00179750(Pursuer *p) {
    PU(p, 0x1660, s32) = 600;
}

/* vtable +0x2C8: timer +0x1660 to `frames`, 0 = 900 (15 s) */
void func_00179760(Pursuer *p, s32 frames) {
    PU(p, 0x1660, s32) = frames != 0 ? frames : 900;
}

/* vtable +0xA0 / +0xA4: turn rates, 4 and 8 degrees (in radians) */
f32 func_00179830(Pursuer *p) {
    return 0x1.1df46a0000000p-4f /* 0.06981317 */;
}

f32 func_00179850(Pursuer *p) {
    return 0x1.1df46a0000000p-3f /* 0.13962634 */;
}

/* vtable +0x2DC .. +0x2FC: distances and factors */
f32 func_00179870(Pursuer *p) {
    return 60.0f;
}

f32 func_00179880(Pursuer *p) {
    return 0x1.eb851e0000000p-4f /* 0.12 */;
}

f32 func_001798A0(Pursuer *p) {
    return 20.0f;
}

f32 func_001798B0(Pursuer *p) {
    return 20.0f;
}

f32 func_001798C0(Pursuer *p) {
    return 20.0f;
}

f32 func_001798D0(Pursuer *p) {
    return 16.0f;
}

f32 func_001798E0(Pursuer *p) {
    return 24.0f;
}

f32 func_001798F0(Pursuer *p) {
    return 0x1.6666660000000p+0f /* 1.4 */;
}

f32 func_00179910(Pursuer *p) {
    return 0x1.3333340000000p-1f /* 0.6 */;
}

s32 func_00179930(Pursuer *p) {
    return -1;
}

s32 func_00179940(Pursuer *p) {
    return -1;
}

/* vtable +0x314 */
s32 func_00179950(Pursuer *p) {
    return 0;
}

extern void func_00212400(Pursuer *p);

/* vtable +0xE8 */
void func_00179960(Pursuer *p) {
    func_00212400(p);
}

/* ---- 0x211C80..0x219460 ---- */

/* vtable +0xE4: nothing */
void func_00212540(Pursuer *p) {
}

/* shut / open door `door` (doors vtable +0x1C) */
s32 func_00212DC0(Pursuer *p, s32 door) {
    return VCALL(D_0044E558, 0x1C, s32 (*)(VObject *, s32, s32, s32))(D_0044E558, door, 0, 0x60000);
}

s32 func_00212DE0(Pursuer *p, s32 door) {
    return VCALL(D_0044E558, 0x1C, s32 (*)(VObject *, s32, s32, s32))(D_0044E558, door, 1, 0x60000);
}

/* the room's side behind the exit the pursuer heads for (rooms vtable +0x50) */
s32 func_00217340(Pursuer *p) {
    return VCALL(D_0044E568, 0x50, s32 (*)(VObject *, s32, u32, s32))(D_0044E568, p->c.a.room, p->c.door, 1);
}

/* vtable +0xC8: reacting to a noise */
s32 func_002181C0(Pursuer *p) {
    return p->c.heardSlot != 0xFF;
}

/* vtable +0xA8: blocking nav triangle flags */
u32 func_00219450(Pursuer *p) {
    return 0x2C020028;
}
