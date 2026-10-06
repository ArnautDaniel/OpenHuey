/* Room 0xE5: its event handler class (vtable RoomE5_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *RoomE5_vtable[];
extern u8 RoomE5_Phase2Script_data[], RoomE5_ObjectNames[];
extern const char *D_0047B0FC;   /* a room object's name */
extern char D_0047B320[];
extern const char str_59_59_16[];

extern u8 RoomE5_EnterScript_data[];
extern u8 RoomE5_CharEnterScript_data[];
extern u8 RoomE5_Phase1Script_data[];
extern u8 RoomE5_Phase3Script_data[];
extern u8 RoomE5_Phase5Script_data[];
extern void *RoomE5_ActionScripts[];
extern u8 RoomE5_Table38_data[];
extern PTMF RoomE5_CmdTable[];
extern PTMF RoomE5_CondTable[];

/* 0x00379C20 */
void *RoomE5_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomE5_vtable, RoomBase_vtable); }

/* 0x00379C80 */
void *RoomE5_EnterScript(void) {
    return RoomE5_EnterScript_data;
}

/* 0x00379C90 */
void *RoomE5_CharEnterScript(void) {
    return RoomE5_CharEnterScript_data;
}

/* 0x00379CA0 */
void *RoomE5_Phase1Script(void) {
    return RoomE5_Phase1Script_data;
}

/* 0x00379CB0 */
void *RoomE5_Phase2Script(void *o) { return RoomE5_Phase2Script_data; }   /* RoomE5_vtable +0x14 */

/* 0x00379CC0 */
void *RoomE5_Phase3Script(void) {
    return RoomE5_Phase3Script_data;
}

/* 0x00379CD0 */
void *RoomE5_ActionScript(void *self, s32 i) {
    return RoomE5_ActionScripts[i];
}

/* 0x00379CF0 */
void *RoomE5_Phase5Script(void) {
    return RoomE5_Phase5Script_data;
}

/* 0x00379D00 */
void *RoomE5_Table38(void) {
    return RoomE5_Table38_data;
}

/* 0x00379D10 */
u32 RoomE5_ObjectName(void *o, s32 i) { return ((u32 *)RoomE5_ObjectNames)[i]; }   /* RoomE5_vtable +0x34 */

/* (self->*RoomE5_CondTable[i])(a, b) */
/* 0x00379D30 */
s32 RoomE5_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomE5_CondTable[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x00379D60 */
s32 RoomE5_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*RoomE5_CmdTable[i])(a, b) */
/* 0x00379D80 */
s32 RoomE5_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomE5_CmdTable[i & 0xFF], a, b);
}

/* 0x00379DB0 */
s32 RoomE5_Cmd01(void) { return clock_draw(D_0047B320, str_59_59_16); }

/* (as Room14_Cmd00) the same for the room object D_0047B0FC */
/* 0x00379F00 */
s32 RoomE5_Cmd00(void *self, void *a1, u8 *cmd) {
    return var0_obj_anim(D_0047B0FC, cmd);
}
