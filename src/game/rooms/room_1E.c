/* Room 0x1E: its event handler class (vtable Room1E_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room1E_vtable[];

extern u8 Room1E_EnterScript_data[];
extern u8 Room1E_CharEnterScript_data[];
extern u8 Room1E_Phase1Script_data[];
extern u8 Room1E_Phase2Script_data[];
extern u8 Room1E_Phase5Script_data[];
extern u32 Room1E_ActionScripts[];
extern u8 Room1E_Table38_data[];
extern u32 Room1E_ObjectNames[];

/* 0x002ADF90 */
void *Room1E_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room1E_vtable, RoomBase_vtable); }

/* 0x002ADFF0 */
void *Room1E_EnterScript(void) {
    return Room1E_EnterScript_data;
}

/* 0x002AE000 */
void *Room1E_CharEnterScript(void) {
    return Room1E_CharEnterScript_data;
}

/* 0x002AE010 */
void *Room1E_Phase1Script(void) {
    return Room1E_Phase1Script_data;
}

/* 0x002AE020 */
void *Room1E_Phase2Script(void) {
    return Room1E_Phase2Script_data;
}

/* 0x002AE030 */
void *Room1E_Phase5Script(void) {
    return Room1E_Phase5Script_data;
}

/* 0x002AE040 */
u32 Room1E_ActionScript(void *self, s32 i) {
    return Room1E_ActionScripts[i];
}

/* 0x002AE060 */
void *Room1E_Table38(void) {
    return Room1E_Table38_data;
}

/* 0x002AE070 */
u32 Room1E_ObjectName(void *self, s32 i) {
    return Room1E_ObjectNames[i];
}
