/* Room 0xE9: its event handler class (vtable RoomE9_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *RoomE9_vtable[];
extern u8 D_0047B134[];
extern char D_0047B340[];
extern const char D_00463A48[];

extern u8 D_0044A2C0[];
extern u8 D_0044A380[];
extern u8 D_0044A3C0[];
extern u8 D_0044A470[];
extern void *D_0044A928[];
extern u8 D_0044A990[];
extern PTMF D_01991E60[];
extern PTMF D_01991E90[];

/* 0x0037AB80 */
void *RoomE9_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomE9_vtable, RoomBase_vtable); }

/* 0x0037ABE0 */
void *RoomE9_EnterScript(void) {
    return D_0044A2C0;
}

/* 0x0037ABF0 */
void *RoomE9_CharEnterScript(void) {
    return D_0044A380;
}

/* 0x0037AC00 */
void *RoomE9_Phase1Script(void) {
    return D_0044A3C0;
}

/* 0x0037AC10 */
void *RoomE9_Phase2Script(void *o) { return D_0047B134; }   /* RoomE9_vtable +0x14 */

/* 0x0037AC20 */
void *RoomE9_Phase3Script(void) {
    return D_0044A470;
}

/* 0x0037AC30 */
void *RoomE9_ActionScript(void *self, s32 i) {
    return D_0044A928[i];
}

/* 0x0037AC50 */
void *RoomE9_Table38(void) {
    return D_0044A990;
}

/* (self->*D_01991E90[i])(a, b) */
/* 0x0037AC60 */
s32 RoomE9_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991E90[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x0037AC90 */
s32 RoomE9_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991E60[i])(a, b) */
/* 0x0037ACB0 */
s32 RoomE9_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991E60[i & 0xFF], a, b);
}

/* 0x0037ACE0 */
s32 RoomE9_Cmd03(void) { return clock_draw_saved(D_0047B340, D_00463A48); }

/* (as Room00_Cmd00) the same four spots for bytes 3..6 */
/* 0x0037AFB0 */
s32 RoomE9_Cmd02(void *self, void *a1, u8 *cmd) {
    return glow4_spot(cmd, 3);
}

/* 0x0037B210 */
s32 RoomE9_Cmd01(void) { return clock_save(); }

/* 0x0037B290 */
s32 RoomE9_Cmd00(void) { return clock_stop(); }
