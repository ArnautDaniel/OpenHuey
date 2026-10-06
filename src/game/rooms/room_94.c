/* Room 0x94: its event handler class (vtable Room94_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room94_vtable[];
extern u8 D_0047AFE0[];

extern u8 D_00444390[];
extern u8 D_004443C0[];
extern u8 D_00444400[];
extern u8 D_00444460[];
extern void *D_004444C0[];
extern PTMF D_01991A68[];

/* 0x0035D330 */
void *Room94_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room94_vtable, RoomBase_vtable); }

/* 0x0035D390 */
void *Room94_EnterScript(void) {
    return D_00444390;
}

/* 0x0035D3A0 */
void *Room94_CharEnterScript(void) {
    return D_004443C0;
}

/* 0x0035D3B0 */
void *Room94_Phase1Script(void) {
    return D_00444400;
}

/* 0x0035D3C0 */
void *Room94_Phase2Script(void) {
    return D_00444460;
}

/* 0x0035D3D0 */
void *Room94_Phase5Script(void *o) { return D_0047AFE0; }   /* Room94_vtable +0x20 */

/* 0x0035D3E0 */
void *Room94_ActionScript(void *self, s32 i) {
    return D_004444C0[i];
}

/* (self->*D_01991A68[i])(a, b) */
/* 0x0035D400 */
s32 Room94_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A68[i & 0xFF], a, b);
}

/* 0x0035D430 */
s32 Room94_Cmd00(void) { return slam_shake(); }
