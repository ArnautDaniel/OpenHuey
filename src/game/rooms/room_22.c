/* Room 0x22: its event handler class (vtable Room22_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room22_vtable[];

extern u8 Room22_EnterScript_data[];
extern u8 Room22_CharEnterScript_data[];
extern u8 Room22_Phase1Script_data[];
extern u8 Room22_Phase2Script_data[];
extern u8 Room22_Phase3Script_data[];
extern u32 Room22_ActionScripts[];
extern u8 Room22_Table38_data[];
extern u32 Room22_ObjectNames[];

/* 0x002AF7E0 */
void *Room22_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room22_vtable, RoomBase_vtable); }

/* 0x002AF840 */
void *Room22_EnterScript(void) {
    return Room22_EnterScript_data;
}

/* 0x002AF850 */
void *Room22_CharEnterScript(void) {
    return Room22_CharEnterScript_data;
}

/* 0x002AF860 */
void *Room22_Phase1Script(void) {
    return Room22_Phase1Script_data;
}

/* 0x002AF870 */
void *Room22_Phase2Script(void) {
    return Room22_Phase2Script_data;
}

/* 0x002AF880 */
void *Room22_Phase3Script(void) {
    return Room22_Phase3Script_data;
}

/* 0x002AF890 */
u32 Room22_ActionScript(void *self, s32 i) {
    return Room22_ActionScripts[i];
}

/* 0x002AF8B0 */
void *Room22_Table38(void) {
    return Room22_Table38_data;
}

/* 0x002AF8C0 */
u32 Room22_ObjectName(void *self, s32 i) {
    return Room22_ObjectNames[i];
}
