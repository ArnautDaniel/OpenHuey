/* Room 0xE3: its event handler class (vtable RoomE3_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *RoomE3_vtable[];
extern u8 RoomE3_Phase2Script_data[], RoomE3_ObjectNames[];
extern char D_0047B310[];
extern const char str_59_59_14[];

extern u8 RoomE3_EnterScript_data[];
extern u8 RoomE3_CharEnterScript_data[];
extern u8 RoomE3_Phase1Script_data[];
extern u8 RoomE3_Phase3Script_data[];
extern u8 RoomE3_Phase5Script_data[];
extern void *RoomE3_ActionScripts[];
extern u8 RoomE3_Table38_data[];
extern PTMF RoomE3_CmdTable[];
extern PTMF RoomE3_CondTable[];

/* 0x00379540 */
void *RoomE3_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomE3_vtable, RoomBase_vtable); }

/* 0x003795A0 */
void *RoomE3_EnterScript(void) {
    return RoomE3_EnterScript_data;
}

/* 0x003795B0 */
void *RoomE3_CharEnterScript(void) {
    return RoomE3_CharEnterScript_data;
}

/* 0x003795C0 */
void *RoomE3_Phase1Script(void) {
    return RoomE3_Phase1Script_data;
}

/* 0x003795D0 */
void *RoomE3_Phase2Script(void *o) { return RoomE3_Phase2Script_data; }   /* RoomE3_vtable +0x14 */

/* 0x003795E0 */
void *RoomE3_ActionScript(void *self, s32 i) {
    return RoomE3_ActionScripts[i];
}

/* 0x00379600 */
void *RoomE3_Phase3Script(void) {
    return RoomE3_Phase3Script_data;
}

/* 0x00379610 */
void *RoomE3_Phase5Script(void) {
    return RoomE3_Phase5Script_data;
}

/* 0x00379620 */
void *RoomE3_Table38(void) {
    return RoomE3_Table38_data;
}

/* 0x00379630 */
u32 RoomE3_ObjectName(void *o, s32 i) { return ((u32 *)RoomE3_ObjectNames)[i]; }   /* RoomE3_vtable +0x34 */

/* (self->*RoomE3_CondTable[i])(a, b) */
/* 0x00379650 */
s32 RoomE3_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomE3_CondTable[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x00379680 */
s32 RoomE3_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*RoomE3_CmdTable[i])(a, b) */
/* 0x003796A0 */
s32 RoomE3_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomE3_CmdTable[i & 0xFF], a, b);
}

/* 0x003796D0 */
s32 RoomE3_Cmd00(void) { return clock_draw(D_0047B310, str_59_59_14); }
