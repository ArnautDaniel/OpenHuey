/* Room 0x13: its event handler class (vtable Room13_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room13_vtable[];

extern u8 D_003F91A0[];
extern u8 D_003F92E0[];
extern u8 D_003F93A0[];
extern u8 D_003F9530[];
extern u8 D_003F95F0[];
extern void *D_003F9980[];
extern void *D_003F99B0[];
extern u8 D_003F99C0[];
extern PTMF D_01990900[];

/* 0x002AC4D0 */
void *Room13_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room13_vtable, RoomBase_vtable); }

/* 0x002AC530 */
void *Room13_EnterScript(void) {
    return D_003F91A0;
}

/* 0x002AC540 */
void *Room13_CharEnterScript(void) {
    return D_003F92E0;
}

/* 0x002AC550 */
void *Room13_Phase1Script(void) {
    return D_003F93A0;
}

/* 0x002AC560 */
void *Room13_Phase2Script(void) {
    return D_003F9530;
}

/* 0x002AC570 */
void *Room13_Phase3Script(void) {
    return D_003F95F0;
}

/* 0x002AC580 */
void *Room13_ActionScript(void *self, s32 i) {
    return D_003F9980[i];
}

/* 0x002AC5A0 */
void *Room13_Table38(void) {
    return D_003F99C0;
}

/* 0x002AC5B0 */
void *Room13_ObjectName(void *self, s32 i) {
    return D_003F99B0[i];
}

/* (self->*D_01990900[i])(a, b) */
/* 0x002AC5D0 */
s32 Room13_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990900[i & 0xFF], a, b);
}

/* 0x002AC600 */
s32 Room13_Cmd00(void *a0, void *a1, u8 *arg) {
    u8 *kousi = VCALL(gRoomObjects, 0x18, u8 *(*)(VObject *, const char *))(gRoomObjects, D_003F99B8[0]);

    if (kousi != NULL) {
        AT(kousi, 0x14, f32) = arg[3] == 0 ? 0.0f : -0x1.921fb6p+0f;
    }
    return 1;
}
