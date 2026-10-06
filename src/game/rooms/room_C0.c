/* Room 0xC0: its event handler class (vtable D_00474F40, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00474F40[];
extern void *D_00479A60[];
extern const char *D_0042E408;
extern void func_001267F0(void *c, s32 light);
extern PTMF D_01991610[];
extern PTMF D_01991650[];

static void effect_479a60_init(void **obj) {
    obj[0] = D_00479A60;
    obj[0x550 / 4] = D_00469D00;
    ((s32 *)obj)[0x554 / 4] = -1;
    obj[0x550 / 4] = D_0046FC30;
}

/* the EE's float to int: past the top it holds at 0x7FFFFFFF */
static inline s32 ee_ftoi(f32 v) {
    return v >= 2147483648.0f ? 0x7FFFFFFF : (s32)v;
}

void *func_0032DBF0(void *o, s32 flags) { return room_dtor(o, flags, D_00474F40, D_0046DB80); }

/* (self->*D_01991650[i])(a, b) */
s32 func_0032DCE0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991650[i & 0xFF], a, b);
}

/* rooms 0xC0 / 0xC1 / 0xC2 / 0xC3 (D_0042E3E0, D_0043F098, D_0043F8B8, D_004400C8): the timer at
 * progress +0x764 has run out */
s32 func_0032DD10(void) {
    return func_002EC410((u8 *)gProgress + 0x764) == 0;
}

/* (self->*D_01991610[i])(a, b) */
s32 func_0032DD40(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991610[i & 0xFF], a, b);
}

/* the screen fade (renderer +0x90): byte 3 0 full 0x80; else clearing from cutscene frame 120
 * (16 a frame) */
s32 func_0032DD70(void *self, void *a1, u8 *cmd) {
    s32 a = 0x80;

    if (cmd[3] != 0) {
        a = 0x80 - ((VCALL(gCutscene, 0x34, s32 (*)(VObject *))(gCutscene) - 120) << 4);
        if (a < 0) {
            a = 0;
        }
    }
    VCALL(gRenderer, 0x90, s32 (*)(VObject *, u32))(gRenderer, a << 24);
    return 1;
}

/* (as func_002ABC20) an effect D_00471060 with its box */
s32 func_0032DDF0(void) {
    s32 slot = Effect_New(gEffects, 0x840, effect_471060_init);
    f32 prm[9];

    prm[1] = -33.0f;
    prm[2] = 2.0f;
    prm[0] = 0.0f;
    prm[3] = -165.0f;
    prm[6] = 0.0f;
    prm[4] = 50.0f;
    prm[8] = 0.0f;
    prm[5] = 100.0f;
    prm[7] = -180.0f;
    func_002D6090(gEffects, slot, prm);
    return 1;
}

/* an effect D_00479A60 (0x640 bytes, its quad drawer at +0x550), not started */
s32 func_0032DF20(void) {
    Effect_New(gEffects, 0x640, effect_479a60_init);
    return 1;
}

/* the screen fade (renderer +0x70, alpha in the top byte) and the stalker's light by cutscene
 * frame: byte 3 0 up from frame 361 (2.04 a frame), 1 down to frame 605 (2.51), 2 up from 1140
 * and 3 down to 1203 (5.57); full (0x80) leaves it to light 0xA, else light 0x23 (its +0xE4
 * cleared); other bytes light 0x11 and the fade off (+0x64) */
s32 func_0032E010(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } k0 = {0x40028F5C}, k1 = {0x4020A3D7}, k2 = {0x40B23D71};
    u32 t = VCALL(gCutscene, 0x34, s32 (*)(VObject *))(gCutscene);
    s32 a;

    switch (cmd[3]) {
    case 0:
        a = ee_ftoi(k0.f * (f32)(t - 361));
        break;
    case 1:
        a = ee_ftoi(k1.f * (f32)(605 - t));
        break;
    case 2:
        a = ee_ftoi(k2.f * (f32)(t - 1140));
        break;
    case 3:
        a = ee_ftoi(k2.f * (f32)(1203 - t));
        break;
    default:
        func_001267F0(gCharSlot2, 0x11);
        VCALL(gRenderer, 0x64, void (*)(VObject *, u32, s32))(gRenderer, 0x808080, 0);
        return 1;
    }
    if (a >= 0x80) {
        func_001267F0(gCharSlot2, 0xA);
        return 1;
    }
    if (a < 0) {
        a = 0;
    }
    AT(gCharSlot2, 0xE4, u8) = 0;
    func_001267F0(gCharSlot2, 0x23);
    VCALL(gRenderer, 0x70, void (*)(VObject *, u32))(gRenderer, (u32)a << 24 | 0x808080);
    return 1;
}

/* the room object named D_0042E408 swung about its rest (+0x30 from +0x20): byte 3 0 starts it
 * (phase +0x34 0, amplitude +0x3C 0.25); 1 steps the phase on 60 degrees and the amplitude down
 * 0.05, x +0x20 / z +0x28 = rest + amplitude * sin, waiting (2) until it has died out; 2 puts it
 * at (-16.5, -5.6) */
s32 func_0032E2A0(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kStep = {0x3F860A92}, kPi = {0x40490FDB}, k2Pi = {0x40C90FDB},
                                          kDecay = {0x3D4CCCCD};
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_0042E408);
    f32 a;

    switch (cmd[3]) {
    case 0:
        sceVu0CopyVector((f32 *)(o + 0x30), (f32 *)(o + 0x20));
        AT(o, 0x34, s32) = 0;
        AT(o, 0x3C, u32) = 0x3E800000;   /* 0.25 */
        return 1;
    case 1:
        a = AT(o, 0x34, f32) + kStep.f;
        AT(o, 0x34, f32) = a;
        if (!(a <= kPi.f)) {
            AT(o, 0x34, f32) = a - k2Pi.f;
        }
        AT(o, 0x3C, f32) = AT(o, 0x3C, f32) - kDecay.f;
        AT(o, 0x20, f32) = AT(o, 0x30, f32) + AT(o, 0x3C, f32) * func_0031C248(AT(o, 0x34, f32));
        AT(o, 0x28, f32) = AT(o, 0x38, f32) + AT(o, 0x3C, f32) * func_0031C248(AT(o, 0x34, f32));
        return AT(o, 0x3C, f32) <= 0.0f ? 1 : 2;
    case 2:
        AT(o, 0x20, u32) = 0xC1840000;   /* -16.5 */
        AT(o, 0x28, u32) = 0xC0B340E1;   /* -5.6017 */
        return 1;
    }
    return 1;
}
