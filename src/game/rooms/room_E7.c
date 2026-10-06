/* Room 0xE7: its event handler class (vtable RoomE7_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *RoomE7_vtable[];
extern u8 D_0047B118[], D_0047B11C[];
extern char D_0047B330[];
extern const char D_00463A38[];

extern u8 D_00449AD0[];
extern u8 D_00449B20[];
extern u8 D_00449BB0[];
extern u8 D_00449CE0[];
extern u8 D_00449D00[];
extern void *D_00449E90[];
extern u8 D_00449EC0[];
extern PTMF D_01991E18[];
extern PTMF D_01991E28[];

/* 0x0037A5F0 */
void *RoomE7_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomE7_vtable, RoomBase_vtable); }

/* 0x0037A650 */
void *RoomE7_EnterScript(void) {
    return D_00449AD0;
}

/* 0x0037A660 */
void *RoomE7_CharEnterScript(void) {
    return D_00449B20;
}

/* 0x0037A670 */
void *RoomE7_Phase1Script(void) {
    return D_00449BB0;
}

/* 0x0037A680 */
void *RoomE7_Phase2Script(void *o) { return D_0047B118; }   /* RoomE7_vtable +0x14 */

/* 0x0037A690 */
void *RoomE7_Phase3Script(void) {
    return D_00449CE0;
}

/* 0x0037A6A0 */
void *RoomE7_ActionScript(void *self, s32 i) {
    return D_00449E90[i];
}

/* 0x0037A6C0 */
void *RoomE7_Phase5Script(void) {
    return D_00449D00;
}

/* 0x0037A6D0 */
void *RoomE7_Table38(void) {
    return D_00449EC0;
}

/* 0x0037A6E0 */
u32 RoomE7_ObjectName(void *o, s32 i) { return ((u32 *)D_0047B11C)[i]; }   /* RoomE7_vtable +0x34 */

/* (self->*D_01991E28[i])(a, b) */
/* 0x0037A700 */
s32 RoomE7_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991E28[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x0037A730 */
s32 RoomE7_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991E18[i])(a, b) */
/* 0x0037A750 */
s32 RoomE7_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991E18[i & 0xFF], a, b);
}

/* 0x0037A780 */
s32 RoomE7_Cmd00(void) { return clock_draw(D_0047B330, D_00463A38); }
