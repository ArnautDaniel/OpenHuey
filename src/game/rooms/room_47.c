/* Room 0x47: its event handler class (vtable Room47_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room47_vtable[];

extern u8 D_00424B50[];
extern u8 D_00424B70[];
extern u8 D_00424C10[];
extern u8 D_00424D70[];
extern void *D_00425290[];
extern u8 D_004252C0[];
extern void *D_0047AD28[];

/* 0x0030EF80 */
void *Room47_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room47_vtable, RoomBase_vtable); }

/* 0x0030EFE0 */
void *Room47_EnterScript(void) {
    return D_00424B50;
}

/* 0x0030EFF0 */
void *Room47_CharEnterScript(void) {
    return D_00424B70;
}

/* 0x0030F000 */
void *Room47_Phase1Script(void) {
    return D_00424C10;
}

/* 0x0030F010 */
void *Room47_Phase2Script(void) {
    return D_00424D70;
}

/* 0x0030F020 */
void *Room47_ActionScript(void *self, s32 i) {
    return D_00425290[i];
}

/* 0x0030F040 */
void *Room47_Table38(void) {
    return D_004252C0;
}

/* 0x0030F050 */
void *Room47_ObjectName(void *self, s32 i) {
    return D_0047AD28[i];
}
