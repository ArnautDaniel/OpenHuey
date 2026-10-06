/* Room 0xE1: its event handler class (vtable RoomE1_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *RoomE1_vtable[];
extern u8 RoomE1_Phase2Script_data[], RoomE1_ActionScripts[];
extern char D_0047B300[];
extern const char D_00463930[];

extern u8 RoomE1_EnterScript_data[];
extern u8 RoomE1_CharEnterScript_data[];
extern u8 RoomE1_Phase1Script_data[];
extern u8 RoomE1_Phase3Script_data[];
extern u8 RoomE1_Table38_data[];
extern PTMF RoomE1_CmdTable[];
extern PTMF RoomE1_CondTable[];

/* 0x00378FE0 */
void *RoomE1_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomE1_vtable, RoomBase_vtable); }

/* 0x00379040 */
void *RoomE1_EnterScript(void) {
    return RoomE1_EnterScript_data;
}

/* 0x00379050 */
void *RoomE1_CharEnterScript(void) {
    return RoomE1_CharEnterScript_data;
}

/* 0x00379060 */
void *RoomE1_Phase1Script(void) {
    return RoomE1_Phase1Script_data;
}

/* 0x00379070 */
void *RoomE1_Phase2Script(void *o) { return RoomE1_Phase2Script_data; }   /* RoomE1_vtable +0x14 */

/* 0x00379080 */
void *RoomE1_Phase3Script(void) {
    return RoomE1_Phase3Script_data;
}

/* 0x00379090 */
u32 RoomE1_ActionScript(void *o, s32 i) { return ((u32 *)RoomE1_ActionScripts)[i]; }   /* RoomE1_vtable +0x24 */

/* 0x003790B0 */
void *RoomE1_Table38(void) {
    return RoomE1_Table38_data;
}

/* (self->*RoomE1_CondTable[i])(a, b) */
/* 0x003790C0 */
s32 RoomE1_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomE1_CondTable[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x003790F0 */
s32 RoomE1_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*RoomE1_CmdTable[i])(a, b) */
/* 0x00379110 */
s32 RoomE1_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomE1_CmdTable[i & 0xFF], a, b);
}

/* 0x00379140 */
s32 RoomE1_Cmd00(void) { return clock_draw(D_0047B300, D_00463930); }
