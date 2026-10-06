/* Room 0x0D: its event handler class (vtable Room0D_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room0D_vtable[];
extern u8 Room0D_EnterScript_data[], Room0D_ActionScripts[], Room0D_ObjectNames[];

extern u8 Room0D_CharEnterScript_data[];
extern u8 Room0D_Phase1Script_data[];

/* 0x002AB820 */
void *Room0D_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room0D_vtable, RoomBase_vtable); }

/* a table of the room's (by vtable slot) */
/* 0x002AB880 */
void *Room0D_EnterScript(void *o) { return Room0D_EnterScript_data; }   /* Room0D_vtable +0xC */

/* 0x002AB890 */
void *Room0D_CharEnterScript(void) {
    return Room0D_CharEnterScript_data;
}

/* 0x002AB8A0 */
void *Room0D_Phase1Script(void) {
    return Room0D_Phase1Script_data;
}

/* 0x002AB8B0 */
u32 Room0D_ActionScript(void *o, s32 i) { return ((u32 *)Room0D_ActionScripts)[i]; }   /* Room0D_vtable +0x24 */

/* 0x002AB8D0 */
u32 Room0D_ObjectName(void *o, s32 i) { return ((u32 *)Room0D_ObjectNames)[i]; }   /* Room0D_vtable +0x34 */
