/* Room 0xD4: its event handler class (vtable RoomD4_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *RoomD4_vtable[];
extern u8 D_0047B06C[];
extern char D_0047B2B0[];
extern const char D_004636F8[];

extern u8 D_00447130[];
extern u8 D_00447230[];
extern u8 D_004472B0[];
extern u8 D_004472F0[];
extern PTMF D_01991B68[];
extern PTMF D_01991B78[];

/* 0x0036E800 */
void *RoomD4_dtor(void *o, s32 flags) { return room_dtor(o, flags, RoomD4_vtable, RoomBase_vtable); }

/* 0x0036E860 */
void *RoomD4_EnterScript(void *o) { return D_0047B06C; }   /* RoomD4_vtable +0xC */

/* 0x0036E870 */
void *RoomD4_CharEnterScript(void) {
    return D_00447130;
}

/* 0x0036E880 */
void *RoomD4_Phase1Script(void) {
    return D_00447230;
}

/* 0x0036E890 */
void *RoomD4_Phase3Script(void) {
    return D_004472B0;
}

/* 0x0036E8A0 */
void *RoomD4_Table38(void) {
    return D_004472F0;
}

/* (self->*D_01991B78[i])(a, b) */
/* 0x0036E8B0 */
s32 RoomD4_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B78[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
/* 0x0036E8E0 */
s32 RoomD4_Cond00(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991B68[i])(a, b) */
/* 0x0036E900 */
s32 RoomD4_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B68[i & 0xFF], a, b);
}

/* 0x0036E930 */
s32 RoomD4_Cmd00(void) { return clock_draw(D_0047B2B0, D_004636F8); }
