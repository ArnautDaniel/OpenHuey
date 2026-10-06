/* Room 0x97: its event handler class (vtable D_00477500, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00477500[];
extern u8 D_0047AE74[];

extern u32 D_00437D60[];
extern u32 D_00437D80[];
extern u32 D_00437DC0[];
extern u32 D_00437E40[];
extern u32 D_0047AE78[];

extern PTMF D_019918C8[];

void *func_00343730(void *o, s32 flags) { return room_dtor(o, flags, D_00477500, D_0046DB80); }

void *func_00343790(void) {
    return D_00437D60;
}

void *func_003437A0(void) {
    return D_00437D80;
}

void *func_003437B0(void) {
    return D_00437DC0;
}

void *func_003437C0(void *o) { return D_0047AE74; }   /* D_00477500 +0x14 */

u32 func_003437D0(void *self, s32 i) {
    return D_0047AE78[i];
}

void *func_003437F0(void) {
    return D_00437E40;
}

/* (self->*D_019918C8[i])(a, b) */
s32 func_00343800(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019918C8[i & 0xFF], a, b);
}

s32 func_00343830(void) { return slam_shake(); }
