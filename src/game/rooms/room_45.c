/* Room 0x45: its event handler class (vtable Room45_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room45_vtable[];
extern u8 D_0047AB50[];
extern u8 D_0047AB58[], D_0047AB70[];

extern u8 D_00407E70[];
extern u8 D_00407E90[];
extern u8 D_00407ED0[];
extern u32 D_0047AB60[];
extern u32 D_0047AB68[];

extern PTMF D_01990C68[];

/* 0x002B2820 */
void *Room45_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room45_vtable, RoomBase_vtable); }

/* 0x002B2880 */
void *Room45_EnterScript(void) {
    return D_00407E70;
}

/* 0x002B2890 */
void *Room45_CharEnterScript(void) {
    return D_00407E90;
}

/* 0x002B28A0 */
void *Room45_Phase1Script(void *o) { return D_0047AB50; }   /* Room45_vtable +0x10 */

/* 0x002B28B0 */
void *Room45_Phase2Script(void) {
    return D_00407ED0;
}

/* 0x002B28C0 */
void *Room45_Phase5Script(void *o) { return D_0047AB58; }   /* Room45_vtable +0x20 */

/* 0x002B28D0 */
u32 Room45_ActionScript(void *self, s32 i) {
    return D_0047AB60[i];
}

/* 0x002B28F0 */
void *Room45_Table38(void *o) { return D_0047AB70; }   /* Room45_vtable +0x38 */

/* 0x002B2900 */
u32 Room45_ObjectName(void *self, s32 i) {
    return D_0047AB68[i];
}

/* (self->*D_01990C68[i])(a, b) */
/* 0x002B2920 */
s32 Room45_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990C68[i & 0xFF], a, b);
}

/* 0x002B2950 */
s32 Room45_Cmd00(void) {
    return 1;
}
