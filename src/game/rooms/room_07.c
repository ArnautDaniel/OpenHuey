/* Room 0x07: its event handler class (vtable Room07_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *Room07_vtable[], *RoomBase_vtable[];

extern u8 D_003F21E0[];
extern u8 D_003F21F0[];
extern u8 D_003F22D0[];
extern u8 D_003F24B0[];
extern u8 D_003F24F0[];
extern void *D_0047A9A8[];

/* 0x0020BB40 */
void *Room07_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room07_vtable, RoomBase_vtable); }

/* 0x0020BBA0 */
void *Room07_EnterScript(void) {
    return D_003F21E0;
}

/* 0x0020BBB0 */
void *Room07_CharEnterScript(void) {
    return D_003F21F0;
}

/* 0x0020BBC0 */
void *Room07_Phase1Script(void) {
    return D_003F22D0;
}

/* 0x0020BBD0 */
void *Room07_Phase2Script(void) {
    return D_003F24B0;
}

/* 0x0020BBE0 */
void *Room07_ActionScript(void *self, s32 i) {
    return D_0047A9A8[i];
}

/* 0x0020BC00 */
void *Room07_Table38(void) {
    return D_003F24F0;
}
