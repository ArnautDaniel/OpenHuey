/* Room 0x2C: its event handler class (vtable Room2C_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room2C_vtable[];
extern u8 Room2C_ActionScripts[], Room2C_ObjectNames[];

extern u8 Room2C_CharEnterScript_data[];
extern u8 Room2C_Phase1Script_data[];
extern u8 Room2C_EnterScript_data[];

/* 0x002FEEB0 */
void *Room2C_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room2C_vtable, RoomBase_vtable); }

/* 0x002FEF10 */
void *Room2C_EnterScript(void) {
    return Room2C_EnterScript_data;
}

/* 0x002FEF20 */
void *Room2C_CharEnterScript(void) {
    return Room2C_CharEnterScript_data;
}

/* 0x002FEF30 */
void *Room2C_Phase1Script(void) {
    return Room2C_Phase1Script_data;
}

/* 0x002FEF40 */
u32 Room2C_ActionScript(void *o, s32 i) { return ((u32 *)Room2C_ActionScripts)[i]; }   /* Room2C_vtable +0x24 */

/* 0x002FEF60 */
u32 Room2C_ObjectName(void *o, s32 i) { return ((u32 *)Room2C_ObjectNames)[i]; }   /* Room2C_vtable +0x34 */
