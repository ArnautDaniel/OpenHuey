/* Room 0xD9: its event handler class (vtable RoomD9_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"
#include "actor.h"

extern void *RoomBase_vtable[];
extern void *RoomD9_vtable[];
extern u8 D_0047B0A8[];
extern char D_0047B2F0[];
extern const char D_00463738[];

extern u8 D_00447E80[];
extern u8 D_00447E90[];
extern u8 D_00447F40[];
extern u8 D_00447F60[];
extern u8 D_00447FC8[];
extern PTMF D_01991C90[];
extern PTMF D_01991CB8[];

/* 0x0036FCF0 */
void *RoomD9_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomD9_vtable, RoomBase_vtable); }

/* 0x0036FD50 */
void *RoomD9_EnterScript(void) {
    return D_00447E80;
}

/* 0x0036FD60 */
void *RoomD9_CharEnterScript(void) {
    return D_00447E90;
}

/* 0x0036FD70 */
void *RoomD9_Phase1Script(void) {
    return D_00447F40;
}

/* 0x0036FD80 */
void *RoomD9_Phase3Script(void) {
    return D_00447F60;
}

/* 0x0036FD90 */
u32 RoomD9_ActionScript(void *o, s32 i) { return ((u32 *)D_0047B0A8)[i]; }   /* RoomD9_vtable +0x24 */

/* 0x0036FDB0 */
void *RoomD9_Table38(void) {
    return D_00447FC8;
}

/* (self->*D_01991CB8[i])(a, b) */
/* 0x0036FDC0 */
s32 RoomD9_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991CB8[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x0036FDF0 */
s32 RoomD9_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991C90[i])(a, b) */
/* 0x0036FE10 */
s32 RoomD9_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991C90[i & 0xFF], a, b);
}

/* (as Room48_Cmd04)  room 0x48 (Room48_Cmd04_ptmf): the player's Character_ChooseExit(0) */
/* 0x0036FE40 */
s32 RoomD9_Cmd02(void) {
    Character_ChooseExit(gCharPlayer, 0);
    return 1;
}

/* 0x0036FE70 */
s32 RoomD9_Cmd01(void) { return clock_draw(D_0047B2F0, D_00463738); }

/* 0x0036FFC0 */
s32 RoomD9_Cmd00(void) { return clock_start(); }
