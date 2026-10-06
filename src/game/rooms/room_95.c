/* Room 0x95: its event handler class (vtable Room95_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room95_vtable[];
extern u8 Room95_Phase5Script_data[];

extern u8 Room95_EnterScript_data[];
extern u8 Room95_CharEnterScript_data[];
extern u8 Room95_Phase1Script_data[];
extern u8 Room95_Phase2Script_data[];
extern void *Room95_ActionScripts[];
extern PTMF Room95_CmdTable[];

/* 0x0035D4C0 */
void *Room95_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room95_vtable, RoomBase_vtable); }

/* 0x0035D520 */
void *Room95_EnterScript(void) {
    return Room95_EnterScript_data;
}

/* 0x0035D530 */
void *Room95_CharEnterScript(void) {
    return Room95_CharEnterScript_data;
}

/* 0x0035D540 */
void *Room95_Phase1Script(void) {
    return Room95_Phase1Script_data;
}

/* 0x0035D550 */
void *Room95_Phase2Script(void) {
    return Room95_Phase2Script_data;
}

/* 0x0035D560 */
void *Room95_Phase5Script(void *o) { return Room95_Phase5Script_data; }   /* Room95_vtable +0x20 */

/* 0x0035D570 */
void *Room95_ActionScript(void *self, s32 i) {
    return Room95_ActionScripts[i];
}

/* (self->*Room95_CmdTable[i])(a, b) */
/* 0x0035D590 */
s32 Room95_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room95_CmdTable[i & 0xFF], a, b);
}

/* 0x0035D5C0 */
s32 Room95_Cmd00(void) { return slam_shake(); }
