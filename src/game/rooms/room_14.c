/* Room 0x14: its event handler class (vtable Room14_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room14_vtable[];
extern const char *pstr_Cartain;

extern u8 Room14_EnterScript_data[];
extern u8 Room14_CharEnterScript_data[];
extern u8 Room14_Phase1Script_data[];
extern u8 Room14_Phase2Script_data[];
extern void *Room14_ActionScripts[];
extern void *Room14_ObjectNames[];
extern u8 Room14_Table38_data[];
extern PTMF Room14_CmdTable[];

/* 0x002AC670 */
void *Room14_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room14_vtable, RoomBase_vtable); }

/* 0x002AC6D0 */
void *Room14_EnterScript(void) {
    return Room14_EnterScript_data;
}

/* 0x002AC6E0 */
void *Room14_CharEnterScript(void) {
    return Room14_CharEnterScript_data;
}

/* 0x002AC6F0 */
void *Room14_Phase1Script(void) {
    return Room14_Phase1Script_data;
}

/* 0x002AC700 */
void *Room14_Phase2Script(void) {
    return Room14_Phase2Script_data;
}

/* 0x002AC710 */
void *Room14_ActionScript(void *self, s32 i) {
    return Room14_ActionScripts[i];
}

/* 0x002AC730 */
void *Room14_Table38(void) {
    return Room14_Table38_data;
}

/* 0x002AC740 */
void *Room14_ObjectName(void *self, s32 i) {
    return Room14_ObjectNames[i];
}

/* (self->*Room14_CmdTable[i])(a, b) */
/* 0x002AC760 */
s32 Room14_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room14_CmdTable[i & 0xFF], a, b);
}

/* room 0x14: the curtain ("Cartain") animated by event variable 0 (var0_obj_anim: byte 3 picks
 * the frame range and direction). */
/* 0x002AC790 */
s32 Room14_Cmd00(void *self, void *a1, u8 *cmd) {
    return var0_obj_anim(pstr_Cartain, cmd);
}
