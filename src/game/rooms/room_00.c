/* Room 0x00: its event handler class (vtable D_0046DBC0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046DBC0[];

extern u8 D_003ED800[];
extern u8 D_003ED960[];
extern u8 D_003ED9E0[];
extern u8 D_003EDBC0[];
extern u8 D_003EDC40[];
extern void *D_003EE6C0[];
extern void *D_003EE740[];
extern u8 D_003EE760[];
extern PTMF D_01990718[];

void *func_002A8980(void *o, s32 flags) { return room_dtor(o, flags, D_0046DBC0, D_0046DB80); }

void *func_002A89E0(void) {
    return D_003ED800;
}

void *func_002A89F0(void) {
    return D_003ED960;
}

void *func_002A8A00(void) {
    return D_003ED9E0;
}

void *func_002A8A10(void) {
    return D_003EDBC0;
}

void *func_002A8A20(void) {
    return D_003EDC40;
}

void *func_002A8A30(void *self, s32 i) {
    return D_003EE6C0[i];
}

void *func_002A8A50(void) {
    return D_003EE760;
}

void *func_002A8A60(void *self, s32 i) {
    return D_003EE740[i];
}

/* (self->*D_01990718[i])(a, b) */
s32 func_002A8A80(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990718[i & 0xFF], a, b);
}

/* Fiona's model +0xD0 (0, 1.5, -2.5) and +0xCC(1) when byte 3 is 0, else (0, 1.5, -1.5) and
 * +0xCC(0) */
s32 func_002A8AF0(void *self, void *a1, u8 *cmd) {
    void *m = gCharPlayer->motion;

    if (cmd[3] == 0) {
        VCALL(m, 0xD0, void (*)(void *, f32, f32, f32))(m, 0.0f, 1.5f, -2.5f);
        VCALL(m, 0xCC, void (*)(void *, s32))(m, 1);
    } else {
        VCALL(m, 0xD0, void (*)(void *, f32, f32, f32))(m, 0.0f, 1.5f, -1.5f);
        VCALL(m, 0xCC, void (*)(void *, s32))(m, 0);
    }
    return 1;
}

s32 func_002A8BA0(void *self, void *a1, u8 *cmd) {
    return glow4_spot(cmd, 0);
}
