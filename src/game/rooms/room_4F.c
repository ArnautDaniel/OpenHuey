/* Room 0x4F: its event handler class (vtable D_0046E780, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E780[];
extern void *D_00476BD0[];
extern void *D_00476BB0[];
extern u8 D_0040B520[];
extern u8 D_0040B5C0[];
extern u8 D_0040B680[];
extern u8 D_0040B8A0[];
extern u32 D_0040C0A0[];
extern u8 D_0040C170[];

extern PTMF D_01990D70[];

static inline void effect476bd0_init(void **o) {
    o[0] = D_00476BD0;
    o[0x3040 / 4] = D_00469D00;
    ((s32 *)o)[0x3044 / 4] = -1;
    o[0x3040 / 4] = D_0046FC30;
    o[0x3078 / 4] = D_00469D00;
    ((s32 *)o)[0x307C / 4] = -1;
    o[0x3078 / 4] = D_0046FC30;
}

static void effect_476bb0_init(void **obj) {
    obj[0] = D_00476BB0;
    obj[0x6010 / 4] = D_00469D00;
    ((s32 *)obj)[0x6014 / 4] = -1;
    obj[0x6010 / 4] = D_0046FC30;
}

void *func_002B3F10(void *o, s32 flags) { return room_dtor(o, flags, D_0046E780, D_0046DB80); }

void *func_002B3F70(void) {
    return D_0040B520;
}

void *func_002B3F80(void) {
    return D_0040B5C0;
}

void *func_002B3F90(void) {
    return D_0040B680;
}

void *func_002B3FA0(void) {
    return D_0040B8A0;
}

u32 func_002B3FB0(void *self, s32 i) {
    return D_0040C0A0[i];
}

void *func_002B3FD0(void) {
    return D_0040C170;
}

u32 func_002B3FE0(void *self, s32 i) {
    return (u32)D_0040C140[i];
}

/* (self->*D_01990D70[i])(a, b) */
s32 func_002B4000(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990D70[i & 0xFF], a, b);
}

/* lower the object (D_0040C160)'s +0x14 by 0.025 a frame down to -0.78, then event 6 (+0x5C) */
s32 func_002B4280(void) {
    static const union { u32 u; f32 f; } kStep = {0x3CCCCCCD}, kLow = {0xBF47AE14};
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_0040C160);
    f32 y = AT(o, 0x14, f32) - kStep.f;

    AT(o, 0x14, f32) = y;
    if (y <= kLow.f) {
        AT(o, 0x14, f32) = kLow.f;
        VCALL(gEvents, 0x5C, void (*)(VObject *, s32))(gEvents, 6);
    }
    return 1;
}

/* byte 3 0: a D_00476BD0 effect (0x36C0 bytes) spawned, its slot kept in event var 0; else that
 * slot's effect removed */
s32 func_002B4310(void *self, void *a1, u8 *cmd) {
    if (cmd[3] == 0) {
        s32 slot = Effect_New(gEffects, 0x36C0, effect476bd0_init);

        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 0, slot);
    } else {
        func_002D6170(gEffects, VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 0));
    }
    return 1;
}

/* room 0x4F (D_0040C0F8): an effect (D_00476BB0, 0x7460 bytes), not started */
s32 func_002B4480(void) {
    Effect_New(gEffects, 0x7460, effect_476bb0_init);
    return 1;
}

/* room 0x4F (D_0040C0E8): object byte 3 by byte 4: 0 up (+0x10 0), 1 down (-0.65), 2 lowered a
 * step (0.02, not during a movie); once down, events bit 3 */
s32 func_002B4570(void *self, void *a1, u8 *cmd) {
    u8 *o = room_obj(D_0040C140[cmd[3]]);
    f32 a;

    switch (cmd[4]) {
    case 0:
        AT(o, 0x10, f32) = 0.0f;
        break;
    case 1:
        AT(o, 0x10, u32) = 0xBF266666;
        break;
    case 2:
        if (VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress) != 0) {
            return 1;
        }
        a = AT(o, 0x10, f32) - 0x1.47ae140000000p-6f /* 0.02 */;
        AT(o, 0x10, f32) = a;
        if (a <= -0x1.4ccccc0000000p-1f /* 0.65 */) {
            AT(o, 0x10, f32) = -0x1.4ccccc0000000p-1f /* 0.65 */;
            VCALL(gEvents, 0x5C, void (*)(VObject *, s32))(gEvents, 3);
        }
        break;
    }
    return 1;
}
