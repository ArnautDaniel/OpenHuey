/* Objects the room scripts spawn into the effect manager (SceneGame +0xF6E200 slots). */
#include "common.h"
#include "game.h"
#include "sce/libvu0.h"

extern VObject *D_0044E550;   /* the random number generator */

/* (class D_00474000, room 0x2A) +0xC reset: a random delay (0x5A..0x79) and its settings */
void func_00322430(u8 *o) {
    AT(o, 0x8EC, s32) = (VCALL(D_0044E550, 0x10, u32 (*)(VObject *))(D_0044E550) & 0x1F) + 0x5A;
    AT(o, 0x8F0, s32) = 0;
    AT(o, 0x8F4, s32) = 0;
    AT(o, 0x8F8, u8) = 0;
    AT(o, 0x618, s64) = -1;
    AT(o, 0x628, s32) = 0;
    AT(o, 0x62C, s32) = 0;
    AT(o, 0x630, s32) = 0x19;
    AT(o, 0x634, s16) = 1;
    AT(o, 0x636, s16) = 0;
    AT(o, 0x638, s16) = 0xE0;
    AT(o, 0x63A, s16) = 0x20;
    AT(o, 0x63C, s16) = 0x20;
    AT(o, 0x63E, s16) = 0x200;
    AT(o, 0x640, s16) = 0x100;
    AT(o, 0x642, u8) = 2;
    AT(o, 0x643, u8) = 0x10;
    AT(o, 0x644, u8) = 1;
    AT(o, 0x645, u8) = 0x10;
    AT(o, 0x646, u8) = 0xA;
}

extern void func_003219A0(u8 *o, s32 i);

typedef union {
    u32 u;
    f32 f;
} F32Bits;

/* +0x18 start: at its fixed spot by the room's (211.28, 2.125, -220.567), 16 particles spread
 * up to 100 to one side */
void func_00321D20(u8 *o, f32 *params) {
    static const F32Bits kX = {0x435347AE}, kZ = {0xC35C9127}, kSpeed = {0xBDCCCCCD};
    VObject *rng;
    s32 i;

    if (params == NULL) {
        return;
    }
    sceVu0CopyVector((f32 *)(o + 0x650), params);
    AT(o, 0x650, f32) = kX.f;
    AT(o, 0x654, f32) = 2.125f;
    AT(o, 0x658, f32) = kZ.f;
    AT(o, 0x65C, f32) = 1.0f;
    rng = D_0044E550;
    for (i = 0; i < 16; i++) {
        u8 *e;
        f32 r;

        func_003219A0(o, i);
        e = o + AT(o, 0x8F4, s32) * 0x300 + i * 0x30 + 0x10;
        r = VCALL(rng, 0x18, f32 (*)(VObject *))(rng);
        AT(e, 0x10, f32) = (AT(o, 0x650, f32) - 20.0f) + 100.0f * r;
    }
    AT(o, 0x8E0, f32) = 0.0f;
    AT(o, 0x8E4, f32) = 0.0f;
    AT(o, 0x8E8, f32) = kSpeed.f;
}

/* particle i of the current set (+0x10 + set * 0x300, 0x30 each): grey, scattered around the
 * spot, with random spin (+0x820 + i * 12), angles (+0x660 + i * 16) and sizes (+0x760) */
void func_003219A0(u8 *o, s32 i) {
    static const F32Bits k02 = {0x3E4CCCCD}, k005 = {0x3D4CCCCD}, k001 = {0x3C23D70A},
                         kM005 = {0xBD4CCCCD}, kPi = {0x40490FDB};
    VObject *rng = D_0044E550;
    u8 *p = o + AT(o, 0x8F4, s32) * 0x300 + i * 0x30 + 0x10;
    s32 k;

    AT(p, 0x0, s32) = 0x80;
    AT(p, 0x4, s32) = 0x80;
    AT(p, 0x8, s32) = 0x80;
    AT(p, 0xC, s32) = 0x80;
#define RND() VCALL(rng, 0x18, f32 (*)(VObject *))(rng)
    AT(p, 0x10, f32) = (AT(o, 0x650, f32) - 10.0f) + 10.0f * (RND() - 0.5f);
    AT(p, 0x14, f32) = AT(o, 0x654, f32) + RND();
    AT(p, 0x18, f32) = (20.0f + AT(o, 0x658, f32)) + 150.0f * RND();
    AT(p, 0x1C, f32) = 1.0f;
    AT(p, 0x20, f32) = 1.0f;
    AT(p, 0x24, f32) = 1.0f;
    AT(p, 0x28, f32) = 0.0f;
    AT(p, 0x2C, u32) = VCALL(rng, 0x10, u32 (*)(VObject *))(rng) & 0xF;
    AT(o, 0x820 + i * 12, f32) = k02.f + k005.f * RND();
    AT(o, 0x824 + i * 12, f32) = k001.f * RND();
    AT(o, 0x828 + i * 12, f32) = kM005.f * RND();
    for (k = 0; k < 3; k++) {
        AT(o, 0x660 + i * 16 + k * 4, f32) = kPi.f * (180.0f * RND()) / 180.0f;
    }
    for (k = 0; k < 3; k++) {
        AT(o, 0x760 + i * 12 + k * 4, f32) = 10.0f + 22.5f * (RND() - 0.5f);
    }
#undef RND
}
