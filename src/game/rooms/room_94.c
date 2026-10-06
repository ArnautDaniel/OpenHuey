/* Room 0x94: its event handler class (vtable D_00479910, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00479910[];
extern u8 D_0047AFE0[];

extern u8 D_00444390[];
extern u8 D_004443C0[];
extern u8 D_00444400[];
extern u8 D_00444460[];
extern void *D_004444C0[];
extern PTMF D_01991A68[];

void *func_0035D330(void *o, s32 flags) { return room_dtor(o, flags, D_00479910, D_0046DB80); }

void *func_0035D390(void) {
    return D_00444390;
}

void *func_0035D3A0(void) {
    return D_004443C0;
}

void *func_0035D3B0(void) {
    return D_00444400;
}

void *func_0035D3C0(void) {
    return D_00444460;
}

void *func_0035D3D0(void *o) { return D_0047AFE0; }   /* D_00479910 +0x20 */

void *func_0035D3E0(void *self, s32 i) {
    return D_004444C0[i];
}

/* (self->*D_01991A68[i])(a, b) */
s32 func_0035D400(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A68[i & 0xFF], a, b);
}

s32 func_0035D430(void) { return slam_shake(); }
