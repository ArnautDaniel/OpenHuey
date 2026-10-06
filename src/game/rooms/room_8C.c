/* Room 0x8C: its event handler class (vtable D_00477380, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00477380[];
extern u8 D_0047AE50[];
extern const char *D_00434888;
extern const char *D_00434890, *D_00434894;

extern u32 D_00433CC0[];
extern u32 D_00433D80[];
extern u32 D_00433E00[];
extern u32 D_00433F50[];
extern u32 D_004347E0[];
extern u32 D_00434870[];
extern u32 D_004348D0[];

extern PTMF D_01991720[];

void *func_0033F930(void *o, s32 flags) { return room_dtor(o, flags, D_00477380, D_0046DB80); }

void *func_0033F990(void) {
    return D_00433CC0;
}

void *func_0033F9A0(void) {
    return D_00433D80;
}

void *func_0033F9B0(void) {
    return D_00433E00;
}

void *func_0033F9C0(void) {
    return D_00433F50;
}

void *func_0033F9D0(void *o) { return D_0047AE50; }   /* D_00477380 +0x20 */

u32 func_0033F9E0(void *self, s32 i) {
    return D_004347E0[i];
}

void *func_0033FA00(void) {
    return D_004348D0;
}

u32 func_0033FA10(void *self, s32 i) {
    return D_00434870[i];
}

/* (self->*D_01991720[i])(a, b) */
s32 func_0033FA30(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991720[i & 0xFF], a, b);
}

/* the placed things of kinds 0, 2, 3, 5, 7 and 8 the event manager finds in area 0xB (+0x10):
 * their timer (+0xE4) to 300000 */
s32 func_0033FA60(void) {
    VObject *list = gPlacedThings, *ev = gEvents;
    s32 i;

    for (i = 0; i < 0x80; i++) {
        u8 *o = VCALL(list, 0xC, u8 *(*)(VObject *, s32))(list, i);

        if (o == NULL) {
            continue;
        }
        switch (AT(o, 0x20, u32)) {
        case 0:
        case 2:
        case 3:
        case 5:
        case 7:
        case 8:
            if ((u8)VCALL(ev, 0x10, s32 (*)(VObject *, void *, s32, s32))(ev, o + 0x10, 0xB, -1) == 1) {
                AT(o, 0xE4, s32) = 300000;
            }
            break;
        }
    }
    return 1;
}

/* Hewie's +0x14C8 to script variable 2 (byte 3 0), or back from it (1; 0 there gives 10) */
s32 func_0033FB40(void *self, void *a1, u8 *cmd) {
    VObject *ev;

    switch (cmd[3]) {
    case 0:
        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 2, AT(gCharPartner, 0x14C8, s32));
        break;
    case 1:
        ev = gEvents;
        if (VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 2) == 0) {
            AT(gCharPartner, 0x14C8, s32) = 10;
        } else {
            AT(gCharPartner, 0x14C8, s32) = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 2);
        }
        break;
    }
    return 1;
}

/* a turning machine: the wheel D_00434890 (angle +0x18, height +0x24 5.1) driven by the belt
 * D_00434894 (offset +0x20 wrapping at 10, height +0x24 -3, speed +0x30). Byte 3 0 sets it up
 * (speed 0.4); 1 runs it a frame (both shaking by up to 0.05); 2 also slows it by 0.01, waiting
 * (2) until it stops. (The wheel's wrap steps +0x10, not the angle.) */
s32 func_0033FC10(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } k51 = {0x40A33333}, k16Pi = {0x42490FDB}, k2Pi = {0x40C90FDB},
                                          kPi = {0x40490FDB}, k01 = {0x3DCCCCCD}, k001 = {0x3C23D70A};
    VObject *objs = D_00456DF8, *rnd;
    u8 *w = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00434890);
    u8 *b = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00434894);
    f32 v;

    switch (cmd[3]) {
    case 0:
        AT(w, 0x18, s32) = 0;
        AT(w, 0x24, f32) = k51.f;
        AT(b, 0x20, s32) = 0;
        AT(b, 0x24, u32) = 0xC0400000;   /* -3 */
        AT(b, 0x30, u32) = 0x3ECCCCCD;   /* 0.4 */
        return 1;
    case 1:
    case 2:
        break;
    default:
        return 1;
    }
    AT(w, 0x18, f32) = AT(w, 0x18, f32) + k2Pi.f * (AT(b, 0x30, f32) / k16Pi.f);
    if (!(AT(w, 0x10, f32) <= kPi.f)) {
        do {
            AT(w, 0x10, f32) = AT(w, 0x10, f32) - k2Pi.f;
        } while (!(AT(w, 0x10, f32) <= kPi.f));
    }
    rnd = gRandom;
    AT(w, 0x24, f32) = k51.f + k01.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    AT(b, 0x20, f32) = AT(b, 0x20, f32) + AT(b, 0x30, f32);
    AT(b, 0x24, f32) = -3.0f + k01.f * (VCALL(rnd, 0x18, f32 (*)(VObject *))(rnd) - 0.5f);
    if (!(AT(b, 0x20, f32) < 10.0f)) {
        do {
            AT(b, 0x20, f32) = AT(b, 0x20, f32) - 10.0f;
        } while (!(AT(b, 0x20, f32) < 10.0f));
    }
    if (cmd[3] != 2) {
        return 1;
    }
    v = AT(b, 0x30, f32) - k001.f;
    AT(b, 0x30, f32) = v;
    if (v <= 0.0f) {
        AT(b, 0x30, f32) = 0.0f;
        return 1;
    }
    return 2;
}

/* the room object named D_00434888 swung: byte 3 0 starts it (rest +0x30 from +0x20, phase
 * +0x34 0, amplitude +0x3C 1); 1 steps the phase back 60 degrees and the amplitude down 0.25,
 * height +0x28 = +0x38 + amplitude * sin, waiting (2) until it has died out */
s32 func_0033FE90(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kStep = {0x3F860A92}, kNegPi = {0xC0490FDB}, k2Pi = {0x40C90FDB};
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_00434888);
    f32 a;

    switch (cmd[3]) {
    case 0:
        sceVu0CopyVector((f32 *)(o + 0x30), (f32 *)(o + 0x20));
        AT(o, 0x34, s32) = 0;
        AT(o, 0x3C, u32) = 0x3F800000;   /* 1 */
        return 1;
    case 1:
        a = AT(o, 0x34, f32) - kStep.f;
        AT(o, 0x34, f32) = a;
        if (a < kNegPi.f) {
            AT(o, 0x34, f32) = a + k2Pi.f;
        }
        AT(o, 0x3C, f32) = AT(o, 0x3C, f32) - 0.25f;
        AT(o, 0x28, f32) = AT(o, 0x38, f32) + AT(o, 0x3C, f32) * func_0031C248(AT(o, 0x34, f32));
        return AT(o, 0x3C, f32) <= 0.0f ? 1 : 2;
    }
    return 1;
}

/* (as func_0033A8E0) */
s32 func_0033FFC0(void *self, void *a1, u8 *cmd) {
    return var_down_by_hit(cmd);
}
