/* Room 0x97: its event handler class (vtable Room97_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room97_vtable[];
extern u8 D_0047AE74[];

extern u32 D_00437D60[];
extern u32 D_00437D80[];
extern u32 D_00437DC0[];
extern u32 D_00437E40[];
extern u32 D_0047AE78[];

extern PTMF D_019918C8[];

/* 0x00343730 */
void *Room97_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room97_vtable, RoomBase_vtable); }

/* 0x00343790 */
void *Room97_EnterScript(void) {
    return D_00437D60;
}

/* 0x003437A0 */
void *Room97_CharEnterScript(void) {
    return D_00437D80;
}

/* 0x003437B0 */
void *Room97_Phase1Script(void) {
    return D_00437DC0;
}

/* 0x003437C0 */
void *Room97_Phase2Script(void *o) { return D_0047AE74; }   /* Room97_vtable +0x14 */

/* 0x003437D0 */
u32 Room97_ActionScript(void *self, s32 i) {
    return D_0047AE78[i];
}

/* 0x003437F0 */
void *Room97_Table38(void) {
    return D_00437E40;
}

/* (self->*D_019918C8[i])(a, b) */
/* 0x00343800 */
s32 Room97_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019918C8[i & 0xFF], a, b);
}

/* 0x00343830 */
s32 Room97_Cmd00(void) { return slam_shake(); }
