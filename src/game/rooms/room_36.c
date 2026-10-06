/* Room 0x36: its event handler class (vtable Room36_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room36_vtable[];
extern u8 Room36_EnterScript_data[];
extern u8 Room36_ObjectNames[];

extern void *Room36_ActionScripts[];
extern u8 Room36_CharEnterScript_data[];

/* 0x0035AEB0 */
void *Room36_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room36_vtable, RoomBase_vtable); }

/* 0x0035AF10 */
void *Room36_EnterScript(void *o) { return Room36_EnterScript_data; }   /* Room36_vtable +0xC */

/* 0x0035AF20 */
void *Room36_CharEnterScript(void) { return Room36_CharEnterScript_data; }

/* 0x0035AF30 */
void *Room36_ActionScript(void *self, s32 i) { return Room36_ActionScripts[i]; }

/* 0x0035AF50 */
u32 Room36_ObjectName(void *o, s32 i) { return ((u32 *)Room36_ObjectNames)[i]; }   /* Room36_vtable +0x34 */
