/* Room 0xE4: its event handler class (vtable RoomE4_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *RoomE4_vtable[];
extern u8 RoomE4_Phase2Script_data[], RoomE4_ActionScripts[];
extern const char *RoomE4_ObjectNames;   /* a room object's name */
extern char D_0047B318[];
extern const char str_59_59_15[];

extern u8 RoomE4_EnterScript_data[];
extern u8 RoomE4_CharEnterScript_data[];
extern u8 RoomE4_Phase1Script_data[];
extern u8 RoomE4_Phase3Script_data[];
extern u8 RoomE4_Table38_data[];
extern PTMF RoomE4_CmdTable[];
extern PTMF RoomE4_CondTable[];

/* 0x00379820 */
void *RoomE4_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomE4_vtable, RoomBase_vtable); }

/* 0x00379880 */
void *RoomE4_EnterScript(void) {
    return RoomE4_EnterScript_data;
}

/* 0x00379890 */
void *RoomE4_CharEnterScript(void) {
    return RoomE4_CharEnterScript_data;
}

/* 0x003798A0 */
void *RoomE4_Phase1Script(void) {
    return RoomE4_Phase1Script_data;
}

/* 0x003798B0 */
void *RoomE4_Phase2Script(void *o) { return RoomE4_Phase2Script_data; }   /* RoomE4_vtable +0x14 */

/* 0x003798C0 */
void *RoomE4_Phase3Script(void) {
    return RoomE4_Phase3Script_data;
}

/* 0x003798D0 */
u32 RoomE4_ActionScript(void *o, s32 i) { return ((u32 *)RoomE4_ActionScripts)[i]; }   /* RoomE4_vtable +0x24 */

/* 0x003798F0 */
void *RoomE4_Table38(void) {
    return RoomE4_Table38_data;
}

/* 0x00379900 */
void *RoomE4_ObjectName(void *self, s32 i) {
    return ((void **)&RoomE4_ObjectNames)[i];   /* its table of names (one is reached by name too) */
}

/* (self->*RoomE4_CondTable[i])(a, b) */
/* 0x00379920 */
s32 RoomE4_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomE4_CondTable[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x00379950 */
s32 RoomE4_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*RoomE4_CmdTable[i])(a, b) */
/* 0x00379970 */
s32 RoomE4_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomE4_CmdTable[i & 0xFF], a, b);
}

/* 0x003799A0 */
s32 RoomE4_Cmd01(void) { return clock_draw(D_0047B318, str_59_59_15); }

/* (as Room0F_Cmd00) the room object RoomE4_ObjectNames by event variable 2 */
/* 0x00379AF0 */
s32 RoomE4_Cmd00(void *self, void *a1, u8 *cmd) {
    return var_fade(RoomE4_ObjectNames, 2, cmd);
}
