/* Room 0x11: its event handler class (vtable Room11_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room11_vtable[];
extern u8 Room11_ObjectNames[];

extern u8 Room11_EnterScript_data[];
extern u8 Room11_CharEnterScript_data[];
extern u8 Room11_Phase1Script_data[];
extern u8 Room11_Phase2Script_data[];
extern void *Room11_ActionScripts[];
extern u8 Room11_Table38_data[];

/* 0x002AC0A0 */
void *Room11_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room11_vtable, RoomBase_vtable); }

/* 0x002AC100 */
void *Room11_EnterScript(void) {
    return Room11_EnterScript_data;
}

/* 0x002AC110 */
void *Room11_CharEnterScript(void) {
    return Room11_CharEnterScript_data;
}

/* 0x002AC120 */
void *Room11_Phase1Script(void) {
    return Room11_Phase1Script_data;
}

/* 0x002AC130 */
void *Room11_Phase2Script(void) {
    return Room11_Phase2Script_data;
}

/* 0x002AC140 */
void *Room11_ActionScript(void *self, s32 i) {
    return Room11_ActionScripts[i];
}

/* 0x002AC160 */
void *Room11_Table38(void) {
    return Room11_Table38_data;
}

/* 0x002AC170 */
u32 Room11_ObjectName(void *o, s32 i) { return ((u32 *)Room11_ObjectNames)[i]; }   /* Room11_vtable +0x34 */
