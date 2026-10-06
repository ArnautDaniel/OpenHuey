/* Room 0x50: its event handler class (vtable Room50_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room50_vtable[];

extern u8 D_0040C190[];
extern u8 D_0040C270[];
extern u8 D_0040C2C0[];
extern u8 D_0040C420[];
extern u8 D_0040C4C0[];
extern u32 D_0040CEB0[];
extern u8 D_0040CF30[];
extern u32 D_0040CF10[];

extern PTMF D_01990D80[];

/* 0x002B4690 */
void *Room50_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room50_vtable, RoomBase_vtable); }

/* 0x002B46F0 */
void *Room50_EnterScript(void) {
    return D_0040C190;
}

/* 0x002B4700 */
void *Room50_CharEnterScript(void) {
    return D_0040C270;
}

/* 0x002B4710 */
void *Room50_Phase1Script(void) {
    return D_0040C2C0;
}

/* 0x002B4720 */
void *Room50_Phase2Script(void) {
    return D_0040C420;
}

/* 0x002B4730 */
void *Room50_Phase5Script(void) {
    return D_0040C4C0;
}

/* 0x002B4740 */
u32 Room50_ActionScript(void *self, s32 i) {
    return D_0040CEB0[i];
}

/* 0x002B4760 */
void *Room50_Table38(void) {
    return D_0040CF30;
}

/* 0x002B4770 */
u32 Room50_ObjectName(void *self, s32 i) {
    return D_0040CF10[i];
}

/* (self->*D_01990D80[i])(a, b) */
/* 0x002B4790 */
s32 Room50_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990D80[i & 0xFF], a, b);
}

/* 0x002B47C0 */
s32 Room50_Cmd00(void) {   /* progress flag 0x651 */
    return item238_sound(0x20000);
}
