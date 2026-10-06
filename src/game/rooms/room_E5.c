/* Room 0xE5: its event handler class (vtable D_0047A590, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A590[];
extern u8 D_0047B0F4[], D_0047B0F8[];
extern const char *D_0047B0FC;   /* a room object's name */
extern char D_0047B320[];
extern const char D_004639E0[];

extern u8 D_00449570[];
extern u8 D_00449590[];
extern u8 D_004495E0[];
extern u8 D_00449640[];
extern u8 D_00449660[];
extern void *D_00449890[];
extern u8 D_004498E0[];
extern PTMF D_01991DC0[];
extern PTMF D_01991DD8[];

/* 0x00379C20 */
void *RoomE5_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0047A590, D_0046DB80); }

/* 0x00379C80 */
void *RoomE5_EnterScript(void) {
    return D_00449570;
}

/* 0x00379C90 */
void *RoomE5_CharEnterScript(void) {
    return D_00449590;
}

/* 0x00379CA0 */
void *RoomE5_Phase1Script(void) {
    return D_004495E0;
}

/* 0x00379CB0 */
void *RoomE5_Phase2Script(void *o) { return D_0047B0F4; }   /* D_0047A590 +0x14 */

/* 0x00379CC0 */
void *RoomE5_Phase3Script(void) {
    return D_00449640;
}

/* 0x00379CD0 */
void *RoomE5_ActionScript(void *self, s32 i) {
    return D_00449890[i];
}

/* 0x00379CF0 */
void *RoomE5_Phase5Script(void) {
    return D_00449660;
}

/* 0x00379D00 */
void *RoomE5_Table38(void) {
    return D_004498E0;
}

/* 0x00379D10 */
u32 RoomE5_ObjectName(void *o, s32 i) { return ((u32 *)D_0047B0F8)[i]; }   /* D_0047A590 +0x34 */

/* (self->*D_01991DD8[i])(a, b) */
/* 0x00379D30 */
s32 RoomE5_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991DD8[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x00379D60 */
s32 RoomE5_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991DC0[i])(a, b) */
/* 0x00379D80 */
s32 RoomE5_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991DC0[i & 0xFF], a, b);
}

/* 0x00379DB0 */
s32 RoomE5_Cmd01(void) { return clock_draw(D_0047B320, D_004639E0); }

/* (as Room14_Cmd00) the same for the room object D_0047B0FC */
/* 0x00379F00 */
s32 RoomE5_Cmd00(void *self, void *a1, u8 *cmd) {
    return var0_obj_anim(D_0047B0FC, cmd);
}
