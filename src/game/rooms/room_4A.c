/* Room 0x4A: its event handler class (vtable Room4A_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room4A_vtable[];

extern u8 D_004086A0[];
extern u8 D_00408700[];
extern u8 D_00408780[];
extern u8 D_00408840[];
extern u8 D_00408880[];
extern u32 D_00408B40[];
extern u8 D_00408B78[];
extern u32 D_00408B68[];

/* 0x002B2EF0 */
void *Room4A_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room4A_vtable, RoomBase_vtable); }

/* 0x002B2F50 */
void *Room4A_EnterScript(void) {
    return D_004086A0;
}

/* 0x002B2F60 */
void *Room4A_CharEnterScript(void) {
    return D_00408700;
}

/* 0x002B2F70 */
void *Room4A_Phase1Script(void) {
    return D_00408780;
}

/* 0x002B2F80 */
void *Room4A_Phase2Script(void) {
    return D_00408840;
}

/* 0x002B2F90 */
void *Room4A_Phase5Script(void) {
    return D_00408880;
}

/* 0x002B2FA0 */
u32 Room4A_ActionScript(void *self, s32 i) {
    return D_00408B40[i];
}

/* 0x002B2FC0 */
void *Room4A_Table38(void) {
    return D_00408B78;
}

/* 0x002B2FD0 */
u32 Room4A_ObjectName(void *self, s32 i) {
    return D_00408B68[i];
}
