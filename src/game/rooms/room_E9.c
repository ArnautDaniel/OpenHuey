/* Room 0xE9: its event handler class (vtable RoomE9_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *RoomE9_vtable[];
extern u8 RoomE9_Phase2Script_data[];
extern char D_0047B340[];
extern const char str_59_59_20[];

extern u8 RoomE9_EnterScript_data[];
extern u8 RoomE9_CharEnterScript_data[];
extern u8 RoomE9_Phase1Script_data[];
extern u8 RoomE9_Phase3Script_data[];
extern void *RoomE9_ActionScripts[];
extern u8 RoomE9_Table38_data[];
extern PTMF RoomE9_CmdTable[];
extern PTMF RoomE9_CondTable[];

/* 0x0037AB80 */
void *RoomE9_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomE9_vtable, RoomBase_vtable); }

/* 0x0037ABE0 */
void *RoomE9_EnterScript(void) {
    return RoomE9_EnterScript_data;
}

/* 0x0037ABF0 */
void *RoomE9_CharEnterScript(void) {
    return RoomE9_CharEnterScript_data;
}

/* 0x0037AC00 */
void *RoomE9_Phase1Script(void) {
    return RoomE9_Phase1Script_data;
}

/* 0x0037AC10 */
void *RoomE9_Phase2Script(void *o) { return RoomE9_Phase2Script_data; }   /* RoomE9_vtable +0x14 */

/* 0x0037AC20 */
void *RoomE9_Phase3Script(void) {
    return RoomE9_Phase3Script_data;
}

/* 0x0037AC30 */
void *RoomE9_ActionScript(void *self, s32 i) {
    return RoomE9_ActionScripts[i];
}

/* 0x0037AC50 */
void *RoomE9_Table38(void) {
    return RoomE9_Table38_data;
}

/* (self->*RoomE9_CondTable[i])(a, b) */
/* 0x0037AC60 */
s32 RoomE9_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomE9_CondTable[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x0037AC90 */
s32 RoomE9_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*RoomE9_CmdTable[i])(a, b) */
/* 0x0037ACB0 */
s32 RoomE9_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomE9_CmdTable[i & 0xFF], a, b);
}

/* room 0xE9: draws the countdown clock, frozen at the saved time while event flag 1 is set
 * (clock_draw_saved; a frame hook). */
/* 0x0037ACE0 */
s32 RoomE9_Cmd03(void) { return clock_draw_saved(D_0047B340, str_59_59_20); }

/* (as Room00_Cmd00) the same four spots for bytes 3..6 */
/* 0x0037AFB0 */
s32 RoomE9_Cmd02(void *self, void *a1, u8 *cmd) {
    return glow4_spot(cmd, 3);
}

/* room 0xE9: saves the countdown clock's time in script variables 0..2 (clock_save). */
/* 0x0037B210 */
s32 RoomE9_Cmd01(void) { return clock_save(); }

/* room 0xE9: stops the countdown clock (clock_stop: progress +0x1FBEC1 off, the camera director
 * +0x40 -1). */
/* 0x0037B290 */
s32 RoomE9_Cmd00(void) { return clock_stop(); }
