/* Room 0x13: its event handler class (vtable D_0046DFC0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046DFC0[];

extern u8 D_003F91A0[];
extern u8 D_003F92E0[];
extern u8 D_003F93A0[];
extern u8 D_003F9530[];
extern u8 D_003F95F0[];
extern void *D_003F9980[];
extern void *D_003F99B0[];
extern u8 D_003F99C0[];
extern PTMF D_01990900[];

void *func_002AC4D0(void *o, s32 flags) { return room_dtor(o, flags, D_0046DFC0, D_0046DB80); }

void *func_002AC530(void) {
    return D_003F91A0;
}

void *func_002AC540(void) {
    return D_003F92E0;
}

void *func_002AC550(void) {
    return D_003F93A0;
}

void *func_002AC560(void) {
    return D_003F9530;
}

void *func_002AC570(void) {
    return D_003F95F0;
}

void *func_002AC580(void *self, s32 i) {
    return D_003F9980[i];
}

void *func_002AC5A0(void) {
    return D_003F99C0;
}

void *func_002AC5B0(void *self, s32 i) {
    return D_003F99B0[i];
}

/* (self->*D_01990900[i])(a, b) */
s32 func_002AC5D0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990900[i & 0xFF], a, b);
}

s32 func_002AC600(void *a0, void *a1, u8 *arg) {
    u8 *kousi = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_003F99B8[0]);

    if (kousi != NULL) {
        AT(kousi, 0x14, f32) = arg[3] == 0 ? 0.0f : -0x1.921fb6p+0f;
    }
    return 1;
}
