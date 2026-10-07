/* Room 0xE6: its event handler class (vtable RoomE6_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *RoomE6_vtable[];
extern u8 RoomE6_Phase2Script_data[], RoomE6_ActionScripts[], RoomE6_ObjectNames[];
extern char D_0047B328[];
extern const char str_59_59_17[];

extern u8 RoomE6_EnterScript_data[];
extern u8 RoomE6_CharEnterScript_data[];
extern u8 RoomE6_Phase1Script_data[];
extern u8 RoomE6_Phase3Script_data[];
extern u8 RoomE6_Table38_data[];
extern PTMF RoomE6_CmdTable[];
extern PTMF RoomE6_CondTable[];

/* 0x0037A2D0 */
void *RoomE6_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomE6_vtable, RoomBase_vtable); }

/* 0x0037A330 */
void *RoomE6_EnterScript(void) {
    return RoomE6_EnterScript_data;
}

/* 0x0037A340 */
void *RoomE6_CharEnterScript(void) {
    return RoomE6_CharEnterScript_data;
}

/* 0x0037A350 */
void *RoomE6_Phase1Script(void) {
    return RoomE6_Phase1Script_data;
}

/* 0x0037A360 */
void *RoomE6_Phase3Script(void) {
    return RoomE6_Phase3Script_data;
}

/* 0x0037A370 */
u32 RoomE6_ActionScript(void *o, s32 i) { return ((u32 *)RoomE6_ActionScripts)[i]; }   /* RoomE6_vtable +0x24 */

/* 0x0037A390 */
void *RoomE6_Phase2Script(void *o) { return RoomE6_Phase2Script_data; }   /* RoomE6_vtable +0x14 */

/* 0x0037A3A0 */
void *RoomE6_Table38(void) {
    return RoomE6_Table38_data;
}

/* 0x0037A3B0 */
u32 RoomE6_ObjectName(void *o, s32 i) { return ((u32 *)RoomE6_ObjectNames)[i]; }   /* RoomE6_vtable +0x34 */

/* (self->*RoomE6_CondTable[i])(a, b) */
/* 0x0037A3D0 */
s32 RoomE6_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomE6_CondTable[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x0037A400 */
s32 RoomE6_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*RoomE6_CmdTable[i])(a, b) */
/* 0x0037A420 */
s32 RoomE6_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &RoomE6_CmdTable[i & 0xFF], a, b);
}

/* room 0xE6: draws the countdown clock at (431, 395): "MM:SS", or "59:59" once time is up
 * (clock_draw; a frame hook). */
/* 0x0037A450 */
s32 RoomE6_Cmd01(void) { return clock_draw(D_0047B328, str_59_59_17); }

/* the room object named by RoomE6_ObjectNames[0]: +0x24 -25.3, +0x34 0 */
/* 0x0037A5A0 */
s32 RoomE6_Cmd00(void) {
    u8 *o = VCALL(gRoomObjects, 0x18, u8 *(*)(VObject *, const char *))(gRoomObjects, *(const char **)RoomE6_ObjectNames);

    if (o != NULL) {
        AT(o, 0x24, u32) = 0xC1CA6666;   /* -25.3 */
        AT(o, 0x34, s32) = 0;
    }
    return 1;
}
