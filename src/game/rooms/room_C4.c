/* Room 0xC4: its event handler class (vtable RoomC4_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *RoomC4_vtable[];
extern u8 RoomC4_EnterScript_data[], RoomC4_Table38_data[];

extern u32 RoomC4_CharEnterScript_data[];
extern u32 RoomC4_Phase1Script_data[];
extern u32 RoomC4_ActionScripts[];
extern u32 RoomC4_ObjectNames[];

/* 0x0034B000 */
void *RoomC4_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomC4_vtable, RoomBase_vtable); }

/* 0x0034B060 */
void *RoomC4_EnterScript(void *o) { return RoomC4_EnterScript_data; }   /* RoomC4_vtable +0xC */

/* 0x0034B070 */
void *RoomC4_CharEnterScript(void) {
    return RoomC4_CharEnterScript_data;
}

/* 0x0034B080 */
void *RoomC4_Phase1Script(void) {
    return RoomC4_Phase1Script_data;
}

/* 0x0034B090 */
u32 RoomC4_ActionScript(void *self, s32 i) {
    return RoomC4_ActionScripts[i];
}

/* 0x0034B0B0 */
void *RoomC4_Table38(void *o) { return RoomC4_Table38_data; }   /* RoomC4_vtable +0x38 */

/* 0x0034B0C0 */
u32 RoomC4_ObjectName(void *self, s32 i) {
    return RoomC4_ObjectNames[i];
}
