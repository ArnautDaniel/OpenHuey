/* Room 0x93: its event handler class (vtable Room93_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room93_vtable[];
extern u8 Room93_Phase5Script_data[];

extern u8 Room93_EnterScript_data[];
extern u8 Room93_CharEnterScript_data[];
extern u8 Room93_Phase1Script_data[];
extern u8 Room93_Phase2Script_data[];
extern void *Room93_ActionScripts[];
extern PTMF Room93_CmdTable[];

/* 0x0035D1A0 */
void *Room93_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room93_vtable, RoomBase_vtable); }

/* 0x0035D200 */
void *Room93_EnterScript(void) {
    return Room93_EnterScript_data;
}

/* 0x0035D210 */
void *Room93_CharEnterScript(void) {
    return Room93_CharEnterScript_data;
}

/* 0x0035D220 */
void *Room93_Phase1Script(void) {
    return Room93_Phase1Script_data;
}

/* 0x0035D230 */
void *Room93_Phase2Script(void) {
    return Room93_Phase2Script_data;
}

/* 0x0035D240 */
void *Room93_Phase5Script(void *o) { return Room93_Phase5Script_data; }   /* Room93_vtable +0x20 */

/* 0x0035D250 */
void *Room93_ActionScript(void *self, s32 i) {
    return Room93_ActionScripts[i];
}

/* (self->*Room93_CmdTable[i])(a, b) */
/* 0x0035D270 */
s32 Room93_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room93_CmdTable[i & 0xFF], a, b);
}

/* 0x0035D2A0 */
s32 Room93_SlamShake(void) { return slam_shake(); }
