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

extern f32 func_0031C058(f32 x);
extern void func_0021A290(u8 *d, s32 arg, s32);
extern void func_00219E10(u8 *d, s32 setup);
extern f32 func_002197A0(u8 *d, s32 n, f32 t);
extern void func_0025F6A0(void *o, f32 v);
extern void func_00219D70(u8 *d, f32 *out, f32 t);
extern void func_00219CD0(u8 *d, f32 *out, f32 t);
extern void func_002243D0(u8 *d, s32 n);

/* the room's camera at the start of play: the camera's range, its view angle (+0x84) and the
 * director's angle limits (+0x88, +0x80); with a target: follow it and put the camera at the
 * current setup's (+0x70) eye and look-at point, else (no setup) pick one (+0x2243D0) */
void func_002252B0(u8 *d, s32 target) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, k02 = {0x3E4CCCCD};
    VObject *cam;
    f32 fovDeg, limDeg;

    AT(d, 0x7C, s32) = -1;
    AT(d, 0x78, s32) = -1;
    VCALL(D_0044E4F0, 0x5C, s32 (*)(VObject *))(D_0044E4F0);
    cam = D_0044E4B8;
    VCALL(cam, 0xB0, void (*)(VObject *, f32))(cam, 1.0f);
    VCALL(cam, 0xB4, void (*)(VObject *, f32))(cam, 2000.0f);
    VCALL(cam, 0x8C, void (*)(VObject *, s32))(cam, AT(d, 0x6C, s32));
    VCALL(cam, 0x2C, void (*)(VObject *, void *))(cam, d + 0xA0);
    AT(d, 0x84, f32) = VCALL(cam, 0x64, f32 (*)(VObject *))(cam);
    AT(d, 0x88, f32) = kPi.f * (k02.f * (180.0f * AT(d, 0x84, f32) / kPi.f)) / 180.0f;
    limDeg = 180.0f * AT(d, 0x88, f32) / kPi.f;
    fovDeg = 180.0f * AT(d, 0x84, f32) / kPi.f;
    AT(d, 0x80, f32) = kPi.f * (fovDeg - (1.0f + func_0031C058(0.0f)) * limDeg) / 180.0f;
    AT(d, 0x8C, u8) = 0;
    AT(d, 0x90, s32) = 0;
    if (target != 0) {
        func_0021A290(d, target, 0);
        AT(d, 0x38, u8 *) = AT(d, 0xB0, u8 *) + 0x10;
        AT(d, 0x40, f32) = AT(d, 0xC0, f32);
        AT(d, 0x44, f32) = AT(d, 0xC4, f32);
        AT(d, 0x48, f32) = AT(d, 0xC8, f32);
        AT(d, 0x4C, f32) = 1.0f;
        if (AT(d, 0x70, s32) != -1) {
            f32 v[3];

            func_00219E10(d, AT(d, 0x70, s32));
            func_0025F6A0(d + 8, func_002197A0(d, 2, 0.0f));
            func_00219D70(d, v, 0.0f);
            cam = D_0044E4B8;
            VCALL(cam, 0x28, void (*)(VObject *, f32, f32, f32))(cam, v[0], v[1], v[2]);
            func_00219CD0(d, v, 0.0f);
            VCALL(cam, 0x1C, void (*)(VObject *, f32, f32, f32))(cam, v[0], v[1], v[2]);
        }
    }
    if (AT(d, 0x70, s32) == -1) {
        func_002243D0(d, 100);
    }
}
