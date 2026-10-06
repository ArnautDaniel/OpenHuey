/* Room 0x34: its event handler class (vtable Room34_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room34_vtable[];
extern u8 Room34_EnterScript_data[], Room34_Phase1Script_data[];

extern u32 Room34_CharEnterScript_data[];
extern u32 Room34_ActionScripts[];
extern u32 Room34_ObjectNames[];

/* 0x0034DBF0 */
void *Room34_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room34_vtable, RoomBase_vtable); }

/* 0x0034DC50 */
void *Room34_EnterScript(void *o) { return Room34_EnterScript_data; }   /* Room34_vtable +0xC */

/* 0x0034DC60 */
void *Room34_CharEnterScript(void) {
    return Room34_CharEnterScript_data;
}

/* 0x0034DC70 */
void *Room34_Phase1Script(void *o) { return Room34_Phase1Script_data; }   /* Room34_vtable +0x10 */

/* 0x0034DC80 */
u32 Room34_ActionScript(void *self, s32 i) {
    return Room34_ActionScripts[i];
}

/* 0x0034DCA0 */
u32 Room34_ObjectName(void *self, s32 i) {
    return Room34_ObjectNames[i];
}
