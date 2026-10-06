/* Room 0x57: its event handler class (vtable Room57_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room57_vtable[];
extern u8 D_0047ABC0[];

extern u8 D_0040F490[];
extern u8 D_0040F4C0[];
extern u8 D_0040F540[];
extern u8 D_0040F5B0[];
extern u32 D_0040F8C0[];
extern u8 D_0040F8F8[];
extern u32 D_0040F8E8[];

/* 0x002B4EF0 */
void *Room57_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room57_vtable, RoomBase_vtable); }

/* 0x002B4F50 */
void *Room57_EnterScript(void) {
    return D_0040F490;
}

/* 0x002B4F60 */
void *Room57_CharEnterScript(void) {
    return D_0040F4C0;
}

/* 0x002B4F70 */
void *Room57_Phase1Script(void) {
    return D_0040F540;
}

/* 0x002B4F80 */
void *Room57_Phase2Script(void) {
    return D_0040F5B0;
}

/* 0x002B4F90 */
void *Room57_Phase5Script(void *o) { return D_0047ABC0; }   /* Room57_vtable +0x20 */

/* 0x002B4FA0 */
u32 Room57_ActionScript(void *self, s32 i) {
    return D_0040F8C0[i];
}

/* 0x002B4FC0 */
void *Room57_Table38(void) {
    return D_0040F8F8;
}

/* 0x002B4FD0 */
u32 Room57_ObjectName(void *self, s32 i) {
    return D_0040F8E8[i];
}
