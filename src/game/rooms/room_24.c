/* Room 0x24: its event handler class (vtable Room24_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room24_vtable[];
extern u8 D_0047AB10[];

extern u8 D_00401890[];
extern u8 D_00401950[];
extern u8 D_004019D0[];
extern u8 D_00401B50[];
extern u32 D_00402270[];
extern u8 D_00402320[];
extern u32 D_004022F0[];

extern PTMF D_01990B10[];

/* 0x002AFC30 */
void *Room24_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room24_vtable, RoomBase_vtable); }

/* 0x002AFC90 */
void *Room24_EnterScript(void) {
    return D_00401890;
}

/* 0x002AFCA0 */
void *Room24_CharEnterScript(void) {
    return D_00401950;
}

/* 0x002AFCB0 */
void *Room24_Phase1Script(void) {
    return D_004019D0;
}

/* 0x002AFCC0 */
void *Room24_Phase2Script(void) {
    return D_00401B50;
}

/* 0x002AFCD0 */
void *Room24_Phase5Script(void *o) { return D_0047AB10; }   /* Room24_vtable +0x20 */

/* 0x002AFCE0 */
u32 Room24_ActionScript(void *self, s32 i) {
    return D_00402270[i];
}

/* 0x002AFD00 */
void *Room24_Table38(void) {
    return D_00402320;
}

/* 0x002AFD10 */
u32 Room24_ObjectName(void *self, s32 i) {
    return D_004022F0[i];
}

/* (self->*D_01990B10[i])(a, b) */
/* 0x002AFD30 */
s32 Room24_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990B10[i & 0xFF], a, b);
}
