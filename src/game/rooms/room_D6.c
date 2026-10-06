/* Room 0xD6: its event handler class (vtable RoomD6_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *RoomD6_vtable[];
extern u8 RoomD6_EnterScript_data[], RoomD6_Phase2Script_data[];
extern char D_0047B2C8[];
extern const char str_59_59_7[];

extern u8 RoomD6_CharEnterScript_data[];
extern u8 RoomD6_Phase1Script_data[];
extern u8 RoomD6_Phase3Script_data[];
extern u8 RoomD6_Phase5Script_data[];
extern void *RoomD6_ActionScripts[];
extern void *RoomD6_ObjectNames[];
extern u8 RoomD6_Table38_data[];
extern PTMF RoomD6_CmdTable[];
extern PTMF RoomD6_CondTable[];

/* 0x0036EE70 */
void *RoomD6_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomD6_vtable, RoomBase_vtable); }

/* 0x0036EED0 */
void *RoomD6_EnterScript(void *o) { return RoomD6_EnterScript_data; }   /* RoomD6_vtable +0xC */

/* 0x0036EEE0 */
void *RoomD6_CharEnterScript(void) {
    return RoomD6_CharEnterScript_data;
}

/* 0x0036EEF0 */
void *RoomD6_Phase1Script(void) {
    return RoomD6_Phase1Script_data;
}

/* 0x0036EF00 */
void *RoomD6_Phase2Script(void *o) { return RoomD6_Phase2Script_data; }   /* RoomD6_vtable +0x14 */

/* 0x0036EF10 */
void *RoomD6_Phase3Script(void) {
    return RoomD6_Phase3Script_data;
}

/* 0x0036EF20 */
void *RoomD6_ActionScript(void *self, s32 i) {
    return RoomD6_ActionScripts[i];
}

/* 0x0036EF40 */
void *RoomD6_Phase5Script(void) {
    return RoomD6_Phase5Script_data;
}

/* 0x0036EF50 */
void *RoomD6_Table38(void) {
    return RoomD6_Table38_data;
}

/* 0x0036EF60 */
void *RoomD6_ObjectName(void *self, s32 i) {
    return RoomD6_ObjectNames[i];
}

/* (self->*RoomD6_CondTable[i])(a, b) */
/* 0x0036EF80 */
s32 RoomD6_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomD6_CondTable[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x0036EFB0 */
s32 RoomD6_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*RoomD6_CmdTable[i])(a, b) */
/* 0x0036EFD0 */
s32 RoomD6_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomD6_CmdTable[i & 0xFF], a, b);
}

/* 0x0036F000 */
s32 RoomD6_Cmd01(void) { return clock_draw(D_0047B2C8, str_59_59_7); }

/* (as Room105_Cmd00) pushed by the character slot byte 4 names */
/* 0x0036F150 */
s32 RoomD6_Cmd00(VObject *self, void *a1, u8 *cmd) {
    return swing_three_by(self, cmd, 1);
}
