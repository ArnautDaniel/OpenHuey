/* Room 0x95: its event handler class (vtable D_00479950, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00479950[];
extern u8 D_0047AFE8[];

extern u8 D_004444E0[];
extern u8 D_00444530[];
extern u8 D_00444570[];
extern u8 D_004445D0[];
extern void *D_004446B0[];
extern PTMF D_01991A78[];

void *func_0035D4C0(void *o, s32 flags) { return room_dtor(o, flags, D_00479950, D_0046DB80); }

void *func_0035D520(void) {
    return D_004444E0;
}

void *func_0035D530(void) {
    return D_00444530;
}

void *func_0035D540(void) {
    return D_00444570;
}

void *func_0035D550(void) {
    return D_004445D0;
}

void *func_0035D560(void *o) { return D_0047AFE8; }   /* D_00479950 +0x20 */

void *func_0035D570(void *self, s32 i) {
    return D_004446B0[i];
}

/* (self->*D_01991A78[i])(a, b) */
s32 func_0035D590(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A78[i & 0xFF], a, b);
}

s32 func_0035D5C0(void) { return slam_shake(); }
