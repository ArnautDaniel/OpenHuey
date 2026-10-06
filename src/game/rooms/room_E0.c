/* Room 0xE0: its event handler class (vtable RoomE0_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "actor.h"

extern void *RoomBase_vtable[];
extern void *RoomE0_vtable[];
extern u8 RoomE0_Phase2Script_data[], RoomE0_ObjectNames[];
extern char D_0047B2F8[];
extern const char str_59_59_11[];

extern u8 RoomE0_EnterScript_data[];
extern u8 RoomE0_CharEnterScript_data[];
extern u8 RoomE0_Phase1Script_data[];
extern u8 RoomE0_Phase3Script_data[];
extern u8 RoomE0_Phase5Script_data[];
extern void *RoomE0_ActionScripts[];
extern u8 RoomE0_Table38_data[];
extern PTMF RoomE0_CmdTable[];
extern PTMF RoomE0_CondTable[];

/* 0x00378870 */
void *RoomE0_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomE0_vtable, RoomBase_vtable); }

/* 0x003788D0 */
void *RoomE0_EnterScript(void) {
    return RoomE0_EnterScript_data;
}

/* 0x003788E0 */
void *RoomE0_CharEnterScript(void) {
    return RoomE0_CharEnterScript_data;
}

/* 0x003788F0 */
void *RoomE0_Phase1Script(void) {
    return RoomE0_Phase1Script_data;
}

/* 0x00378900 */
void *RoomE0_Phase2Script(void *o) { return RoomE0_Phase2Script_data; }   /* RoomE0_vtable +0x14 */

/* 0x00378910 */
void *RoomE0_Phase3Script(void) {
    return RoomE0_Phase3Script_data;
}

/* 0x00378920 */
void *RoomE0_ActionScript(void *self, s32 i) {
    return RoomE0_ActionScripts[i];
}

/* 0x00378940 */
void *RoomE0_Phase5Script(void) {
    return RoomE0_Phase5Script_data;
}

/* 0x00378950 */
void *RoomE0_Table38(void) {
    return RoomE0_Table38_data;
}

/* 0x00378960 */
u32 RoomE0_ObjectName(void *o, s32 i) { return ((u32 *)RoomE0_ObjectNames)[i]; }   /* RoomE0_vtable +0x34 */

/* (self->*RoomE0_CondTable[i])(a, b) */
/* 0x00378980 */
s32 RoomE0_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomE0_CondTable[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x003789B0 */
s32 RoomE0_ProgressCall(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*RoomE0_CmdTable[i])(a, b) */
/* 0x003789D0 */
s32 RoomE0_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomE0_CmdTable[i & 0xFF], a, b);
}

/* (as Room48_Cmd04)  room 0x48 (Room48_Cmd04_ptmf): the player's Character_ChooseExit(0) */
/* 0x00378A00 */
s32 RoomE0_ChooseExit(void) {
    Character_ChooseExit(gCharPlayer, 0);
    return 1;
}

/* 0x00378A30 */
s32 RoomE0_ClockDraw(void) { return clock_draw(D_0047B2F8, str_59_59_11); }

/* (as Room21_Cmd04)  room 0x21 (D_00400BC8) */
/* 0x00378B80 */
s32 RoomE0_DoorLight(void *self, void *a1, u8 *cmd) {
    return lit_quad_in(0x1A, cmd, sQuadDoor, 0x20000040);
}

/* (as Room21_Cmd02)  room 0x21 (D_00400BA8) */
/* 0x00378D20 */
s32 RoomE0_WindowLight(void *self, void *a1, u8 *cmd) {
    return lit_quad(cmd, sQuadWindow, 0x10000040);
}

/* (as Room37_Fan)  room 0x37 (D_004469A0): the fan turns, except while a movie plays */
/* 0x00378EC0 */
s32 RoomE0_Fan(void) {
    if (VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress) != 0) {
        return 1;
    }
    fan_turn(*(const char *const *)RoomE0_ObjectNames);   /* (the first of its names) */
    return 1;
}

/* 0x00378F70 */
s32 RoomE0_ClockStart(void) { return clock_start(); }
