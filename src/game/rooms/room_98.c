/* Room 0x98: its event handler class (vtable Room98_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room98_vtable[];
extern u8 Room98_EnterScript_data[], Room98_Phase2Script_data[];

extern u32 Room98_CharEnterScript_data[];
extern u32 Room98_Phase1Script_data[];
extern u32 Room98_Table38_data[];
extern u32 Room98_ActionScripts[];

extern PTMF Room98_CmdTable[];

/* 0x00350E60 */
void *Room98_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room98_vtable, RoomBase_vtable); }

/* 0x00350EC0 */
void *Room98_EnterScript(void *o) { return Room98_EnterScript_data; }   /* Room98_vtable +0xC */

/* 0x00350ED0 */
void *Room98_CharEnterScript(void) {
    return Room98_CharEnterScript_data;
}

/* 0x00350EE0 */
void *Room98_Phase1Script(void) {
    return Room98_Phase1Script_data;
}

/* 0x00350EF0 */
void *Room98_Phase2Script(void *o) { return Room98_Phase2Script_data; }   /* Room98_vtable +0x14 */

/* 0x00350F00 */
u32 Room98_ActionScript(void *self, s32 i) {
    return Room98_ActionScripts[i];
}

/* 0x00350F20 */
void *Room98_Table38(void) {
    return Room98_Table38_data;
}

/* (self->*Room98_CmdTable[i])(a, b) */
/* 0x00350F30 */
s32 Room98_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room98_CmdTable[i & 0xFF], a, b);
}

/* 0x00350F60 */
s32 Room98_SlamShake(void) { return slam_shake(); }
