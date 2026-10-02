/* The camera director (SceneGame +0xF6CBB0, second vtable at +0x64): drives the camera
 * (D_0044E4B8) through the room's camera setups. */
#include "common.h"
#include "game.h"
#include "sce/libvu0.h"

extern VObject *D_0044E4B8;   /* the camera */

/* reset for a new room: reset the camera, no setup selected, default light direction */
void func_00225550(u8 *d) {
    VObject *cam = D_0044E4B8;

    VCALL(cam, 0xC, void (*)(VObject *))(cam);
    VCALL(cam, 0x6C, void (*)(VObject *, f32))(cam, 0.0f);
    AT(d, 0x6C, s32) = 0;
    AT(d, 0x70, s32) = -1;
    AT(d, 0x7C, s32) = -1;
    AT(d, 0x78, s32) = -1;
    AT(d, 0xB4, s32) = 0;
    AT(d, 0xB0, s32) = 0;
    AT(d, 0x74, u8) = 0;
    AT(d, 0x68, u8) = 0;
    AT(d, 0xD0, f32) = 0.0f;
    AT(d, 0xD4, u32) = 0x3E4CCCCD;   /* 0.2f */
    AT(d, 0xD8, f32) = 1.0f;
    AT(d, 0xDC, f32) = 1.0f;
    sceVu0Normalize((f32 *)(d + 0xD0), (f32 *)(d + 0xD0));
    AT(d, 0xE0, f32) = 30.0f;
    AT(d, 0xE4, f32) = 0.0f;
    AT(d, 0x150, u8) = 0;
    AT(d, 0x69, u8) = 0;
    AT(d, 0x164, u8) = 0;
    ((void (*)(u8 *))AT(AT(d, 0x64, u8 *), 0x78, void *))(d);
}

extern VObject *D_0044E4F0;   /* the renderer */
extern const PTMF D_003E5240;  /* { 0, -1, func_00224770 } */
extern const PTMF D_003E5250;  /* { 0, -1, func_00224740 } */

/* two modes of the director (update state +0xE8, flag +0xF4); both clear the renderer's
 * work area (+0x5C) */
void func_00224330(u8 *d) {
    AT(d, 0xE8, PTMF) = D_003E5250;
    AT(d, 0xF4, u8) = 1;
    AT(d, 0x7C, s32) = -1;
    AT(d, 0x78, s32) = -1;
    VCALL(D_0044E4F0, 0x5C, s32 (*)(VObject *))(D_0044E4F0);
}

/* vt+0x78 (the default mode) */
void func_00224380(u8 *d) {
    AT(d, 0xE8, PTMF) = D_003E5240;
    AT(d, 0xF4, u8) = 0;
    AT(d, 0x7C, s32) = -1;
    AT(d, 0x78, s32) = -1;
    VCALL(D_0044E4F0, 0x5C, s32 (*)(VObject *))(D_0044E4F0);
}

/* (the director's interface, D_0044E4F8) +0xC follow `target` (its position, +0x10) with an
 * offset (x, y, z) */
void func_002246F0(u8 *f, u8 *target, f32 x, f32 y, f32 z) {
    AT(f, 0xB0, u8 *) = target;
    AT(f, 0xC0, f32) = x;
    AT(f, 0xC4, f32) = y;
    AT(f, 0xC8, f32) = z;
    AT(f, 0xCC, f32) = 1.0f;
    AT(f, 0x38, u8 *) = AT(f, 0xB0, u8 *) + 0x10;
    AT(f, 0x40, f32) = AT(f, 0xC0, f32);
    AT(f, 0x44, f32) = AT(f, 0xC4, f32);
    AT(f, 0x48, f32) = AT(f, 0xC8, f32);
    AT(f, 0x4C, f32) = 1.0f;
}

/* (interface) +0x28 the camera setup to use (+0x6C, +0x70) */
void func_00223E30(u8 *f, s32 a, s32 b) {
    AT(f, 0x6C, s32) = a;
    AT(f, 0x70, s32) = b;
}

/* +0x6C the mode flag +0xF4 */
s32 func_00223E20(u8 *d) {
    return AT(d, 0xF4, u8);
}
