/* Room 0x54: its event handler class (vtable Room54_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room54_vtable[];

extern u8 D_00426D80[];
extern u8 D_00426EF0[];
extern u8 D_00426FF0[];
extern u8 D_004272A0[];
extern void *D_00427F90[];
extern void *D_00428050[];
extern u8 D_00428080[];

extern PTMF D_019911A0[];

/* 0x0030F940 */
void *Room54_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room54_vtable, RoomBase_vtable); }

/* 0x0030F9A0 */
void *Room54_EnterScript(void) {
    return D_00426D80;
}

/* 0x0030F9B0 */
void *Room54_CharEnterScript(void) {
    return D_00426EF0;
}

/* 0x0030F9C0 */
void *Room54_Phase1Script(void) {
    return D_00426FF0;
}

/* 0x0030F9D0 */
void *Room54_Phase2Script(void) {
    return D_004272A0;
}

/* 0x0030F9E0 */
void *Room54_ActionScript(void *self, s32 i) {
    return D_00427F90[i];
}

/* 0x0030FA00 */
void *Room54_Table38(void) {
    return D_00428080;
}

/* 0x0030FA10 */
void *Room54_ObjectName(void *self, s32 i) {
    return D_00428050[i];
}

/* (self->*D_019911A0[i])(a, b) */
/* 0x0030FA30 */
s32 Room54_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019911A0[i & 0xFF], a, b);
}
