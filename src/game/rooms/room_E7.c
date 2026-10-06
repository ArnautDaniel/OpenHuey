/* Room 0xE7: its event handler class (vtable RoomE7_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *RoomE7_vtable[];
extern u8 RoomE7_Phase2Script_data[], RoomE7_ObjectNames[];
extern char D_0047B330[];
extern const char str_59_59_18[];

extern u8 RoomE7_EnterScript_data[];
extern u8 RoomE7_CharEnterScript_data[];
extern u8 RoomE7_Phase1Script_data[];
extern u8 RoomE7_Phase3Script_data[];
extern u8 RoomE7_Phase5Script_data[];
extern void *RoomE7_ActionScripts[];
extern u8 RoomE7_Table38_data[];
extern PTMF RoomE7_CmdTable[];
extern PTMF RoomE7_CondTable[];

/* 0x0037A5F0 */
void *RoomE7_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomE7_vtable, RoomBase_vtable); }

/* 0x0037A650 */
void *RoomE7_EnterScript(void) {
    return RoomE7_EnterScript_data;
}

/* 0x0037A660 */
void *RoomE7_CharEnterScript(void) {
    return RoomE7_CharEnterScript_data;
}

/* 0x0037A670 */
void *RoomE7_Phase1Script(void) {
    return RoomE7_Phase1Script_data;
}

/* 0x0037A680 */
void *RoomE7_Phase2Script(void *o) { return RoomE7_Phase2Script_data; }   /* RoomE7_vtable +0x14 */

/* 0x0037A690 */
void *RoomE7_Phase3Script(void) {
    return RoomE7_Phase3Script_data;
}

/* 0x0037A6A0 */
void *RoomE7_ActionScript(void *self, s32 i) {
    return RoomE7_ActionScripts[i];
}

/* 0x0037A6C0 */
void *RoomE7_Phase5Script(void) {
    return RoomE7_Phase5Script_data;
}

/* 0x0037A6D0 */
void *RoomE7_Table38(void) {
    return RoomE7_Table38_data;
}

/* 0x0037A6E0 */
u32 RoomE7_ObjectName(void *o, s32 i) { return ((u32 *)RoomE7_ObjectNames)[i]; }   /* RoomE7_vtable +0x34 */

/* (self->*RoomE7_CondTable[i])(a, b) */
/* 0x0037A700 */
s32 RoomE7_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomE7_CondTable[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x0037A730 */
s32 RoomE7_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*RoomE7_CmdTable[i])(a, b) */
/* 0x0037A750 */
s32 RoomE7_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomE7_CmdTable[i & 0xFF], a, b);
}

/* 0x0037A780 */
s32 RoomE7_Cmd00(void) { return clock_draw(D_0047B330, str_59_59_18); }
