/* Room 0x81: its event handler class (vtable Room81_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room81_vtable[];

extern u8 Room81_EnterScript_data[];
extern u8 Room81_CharEnterScript_data[];
extern u8 Room81_Phase1Script_data[];
extern u8 Room81_Phase2Script_data[];

extern u32 Room81_Phase5Script_data[];
extern u32 Room81_Phase3Script_data[];
extern u32 Room81_ActionScripts[];
extern u32 Room81_ObjectNames[];
extern u32 Room81_Table38_data[];

/* 0x0033EF80 */
void *Room81_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room81_vtable, RoomBase_vtable); }

/* 0x0033EFE0 */
void *Room81_EnterScript(void) {
    return Room81_EnterScript_data;
}

/* 0x0033EFF0 */
void *Room81_CharEnterScript(void) {
    return Room81_CharEnterScript_data;
}

/* 0x0033F000 */
void *Room81_Phase1Script(void) {
    return Room81_Phase1Script_data;
}

/* 0x0033F010 */
void *Room81_Phase2Script(void) {
    return Room81_Phase2Script_data;
}

/* 0x0033F020 */
void *Room81_Phase3Script(void) {
    return Room81_Phase3Script_data;
}

/* 0x0033F030 */
u32 Room81_ActionScript(void *self, s32 i) {
    return Room81_ActionScripts[i];
}

/* 0x0033F050 */
void *Room81_Phase5Script(void) {
    return Room81_Phase5Script_data;
}

/* 0x0033F060 */
void *Room81_Table38(void) {
    return Room81_Table38_data;
}

/* 0x0033F070 */
u32 Room81_ObjectName(void *self, s32 i) {
    return Room81_ObjectNames[i];
}
