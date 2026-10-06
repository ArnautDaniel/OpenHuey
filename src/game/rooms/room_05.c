/* Room 0x05: its event handler class (vtable Room05_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *Room05_vtable[], *RoomBase_vtable[];

extern u8 D_003F1800[];
extern u8 D_003F1840[];
extern u8 D_003F1930[];
extern u8 D_003F1A30[];
extern u8 D_003F1A50[];
extern void *D_003F1B40[];

/* 0x0020BC10 */
void *Room05_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room05_vtable, RoomBase_vtable); }

/* 0x0020BC70 */
void *Room05_EnterScript(void) {
    return D_003F1800;
}

/* 0x0020BC80 */
void *Room05_CharEnterScript(void) {
    return D_003F1840;
}

/* 0x0020BC90 */
void *Room05_Phase1Script(void) {
    return D_003F1930;
}

/* 0x0020BCA0 */
void *Room05_Phase2Script(void) {
    return D_003F1A30;
}

/* 0x0020BCB0 */
void *Room05_Phase5Script(void) {
    return D_003F1A50;
}

/* 0x0020BCC0 */
void *Room05_ActionScript(void *self, s32 i) {
    return D_003F1B40[i];
}
