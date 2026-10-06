/* Room 0x17: its event handler class (vtable Room17_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room17_vtable[];
extern u8 Room17_Phase2Script_data[];

extern u8 Room17_EnterScript_data[];
extern u8 Room17_CharEnterScript_data[];
extern u8 Room17_Phase1Script_data[];
extern void *Room17_ActionScripts[];
extern void *Room17_ObjectNames[];

/* 0x002D2500 */
void *Room17_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room17_vtable, RoomBase_vtable); }

/* 0x002D2560 */
void *Room17_EnterScript(void) { return Room17_EnterScript_data; }

/* 0x002D2570 */
void *Room17_CharEnterScript(void) { return Room17_CharEnterScript_data; }

/* 0x002D2580 */
void *Room17_Phase1Script(void) { return Room17_Phase1Script_data; }

/* 0x002D2590 */
void *Room17_Phase2Script(void *o) { return Room17_Phase2Script_data; }   /* Room17_vtable +0x14 */

/* 0x002D25A0 */
void *Room17_ActionScript(void *self, s32 i) { return Room17_ActionScripts[i]; }

/* 0x002D25C0 */
void *Room17_ObjectName(void *self, s32 i) { return Room17_ObjectNames[i]; }
