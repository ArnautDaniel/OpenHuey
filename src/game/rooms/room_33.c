/* Room 0x33: its event handler class (vtable Room33_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room33_vtable[];
extern u8 Room33_EnterScript_data[], Room33_Phase1Script_data[];

extern u32 Room33_CharEnterScript_data[];
extern u32 Room33_ActionScripts[];
extern u32 Room33_ObjectNames[];

/* 0x003506E0 */
void *Room33_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room33_vtable, RoomBase_vtable); }

/* 0x00350740 */
void *Room33_EnterScript(void *o) { return Room33_EnterScript_data; }   /* Room33_vtable +0xC */

/* 0x00350750 */
void *Room33_CharEnterScript(void) {
    return Room33_CharEnterScript_data;
}

/* 0x00350760 */
void *Room33_Phase1Script(void *o) { return Room33_Phase1Script_data; }   /* Room33_vtable +0x10 */

/* 0x00350770 */
u32 Room33_ActionScript(void *self, s32 i) {
    return Room33_ActionScripts[i];
}

/* 0x00350790 */
u32 Room33_ObjectName(void *self, s32 i) {
    return Room33_ObjectNames[i];
}
