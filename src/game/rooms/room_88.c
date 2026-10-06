/* Room 0x88: its event handler class (vtable Room88_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room88_vtable[];

extern u32 Room88_EnterScript_data[];
extern u32 Room88_CharEnterScript_data[];
extern u32 Room88_Phase1Script_data[];
extern u32 Room88_Phase2Script_data[];
extern u32 Room88_Phase3Script_data[];
extern u32 Room88_ActionScripts[];
extern u32 Room88_Table38_data[];

extern u32 Room88_ObjectNames[];

extern PTMF Room88_CmdTable[];

/* 0x0033F680 */
void *Room88_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room88_vtable, RoomBase_vtable); }

/* 0x0033F6E0 */
void *Room88_EnterScript(void) {
    return Room88_EnterScript_data;
}

/* 0x0033F6F0 */
void *Room88_CharEnterScript(void) {
    return Room88_CharEnterScript_data;
}

/* 0x0033F700 */
void *Room88_Phase1Script(void) {
    return Room88_Phase1Script_data;
}

/* 0x0033F710 */
void *Room88_Phase2Script(void) {
    return Room88_Phase2Script_data;
}

/* 0x0033F720 */
void *Room88_Phase3Script(void) {
    return Room88_Phase3Script_data;
}

/* 0x0033F730 */
u32 Room88_ActionScript(void *self, s32 i) {
    return Room88_ActionScripts[i];
}

/* 0x0033F750 */
void *Room88_Table38(void) {
    return Room88_Table38_data;
}

/* 0x0033F760 */
u32 Room88_ObjectName(void *self, s32 i) {
    return Room88_ObjectNames[i];
}

/* (self->*Room88_CmdTable[i])(a, b) */
/* 0x0033F780 */
s32 Room88_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room88_CmdTable[i & 0xFF], a, b);
}

/* 0x0033F7B0 */
s32 Room88_Cmd00(void) { return slam_shake(); }
