/* Room 0x35: its event handler class (vtable Room35_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room35_vtable[];
extern u8 Room35_EnterScript_data[];

extern u32 Room35_CharEnterScript_data[];
extern u32 Room35_ActionScripts[];
extern u32 Room35_ObjectNames[];

/* 0x00350FF0 */
void *Room35_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room35_vtable, RoomBase_vtable); }

/* 0x00351050 */
void *Room35_EnterScript(void *o) { return Room35_EnterScript_data; }   /* Room35_vtable +0xC */

/* 0x00351060 */
void *Room35_CharEnterScript(void) {
    return Room35_CharEnterScript_data;
}

/* 0x00351070 */
u32 Room35_ActionScript(void *self, s32 i) {
    return Room35_ActionScripts[i];
}

/* 0x00351090 */
u32 Room35_ObjectName(void *self, s32 i) {
    return Room35_ObjectNames[i];
}
