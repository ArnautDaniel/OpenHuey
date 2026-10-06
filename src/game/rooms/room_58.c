/* Room 0x58: its event handler class (vtable Room58_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room58_vtable[];
extern u8 D_0047ABD0[];

extern u8 D_0040F910[];
extern u8 D_0040FA20[];
extern u8 D_0040FAB0[];
extern u8 D_0040FC70[];
extern u32 D_00410030[];
extern u8 D_00410050[];
extern u32 D_0047ABD8[];

/* 0x002B4FF0 */
void *Room58_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room58_vtable, RoomBase_vtable); }

/* 0x002B5050 */
void *Room58_EnterScript(void) {
    return D_0040F910;
}

/* 0x002B5060 */
void *Room58_CharEnterScript(void) {
    return D_0040FA20;
}

/* 0x002B5070 */
void *Room58_Phase1Script(void) {
    return D_0040FAB0;
}

/* 0x002B5080 */
void *Room58_Phase5Script(void *o) { return D_0047ABD0; }   /* Room58_vtable +0x20 */

/* 0x002B5090 */
void *Room58_Phase2Script(void) {
    return D_0040FC70;
}

/* 0x002B50A0 */
u32 Room58_ActionScript(void *self, s32 i) {
    return D_00410030[i];
}

/* 0x002B50C0 */
void *Room58_Table38(void) {
    return D_00410050;
}

/* 0x002B50D0 */
u32 Room58_ObjectName(void *self, s32 i) {
    return D_0047ABD8[i];
}
