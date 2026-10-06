/* Room 0x5E: its event handler class (vtable Room5E_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *Room5E_vtable[];
extern void *RoomBase_vtable[];

extern u8 D_00412410[];
extern u8 D_00412430[];
extern u8 D_00412510[];
extern u8 D_004125E0[];
extern u8 D_00412688[];
extern void *D_0047ABF0[];

/* 0x0020B5A0 */
void *Room5E_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room5E_vtable, RoomBase_vtable); }

/* 0x0020B600 */
void *Room5E_EnterScript(void) {
    return D_00412410;
}

/* 0x0020B610 */
void *Room5E_CharEnterScript(void) {
    return D_00412430;
}

/* 0x0020B620 */
void *Room5E_Phase1Script(void) {
    return D_00412510;
}

/* 0x0020B630 */
void *Room5E_Phase2Script(void) {
    return D_004125E0;
}

/* 0x0020B640 */
void *Room5E_ActionScript(void *self, s32 i) {
    return D_0047ABF0[i];
}

/* 0x0020B660 */
void *Room5E_Table38(void) {
    return D_00412688;
}
