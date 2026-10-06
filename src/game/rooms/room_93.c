/* Room 0x93: its event handler class (vtable D_004798D0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_004798D0[];
extern u8 D_0047AFD8[];

extern u8 D_00444170[];
extern u8 D_004441C0[];
extern u8 D_00444200[];
extern u8 D_00444270[];
extern void *D_00444360[];
extern PTMF D_01991A58[];

void *func_0035D1A0(void *o, s32 flags) { return room_dtor(o, flags, D_004798D0, D_0046DB80); }

void *func_0035D200(void) {
    return D_00444170;
}

void *func_0035D210(void) {
    return D_004441C0;
}

void *func_0035D220(void) {
    return D_00444200;
}

void *func_0035D230(void) {
    return D_00444270;
}

void *func_0035D240(void *o) { return D_0047AFD8; }   /* D_004798D0 +0x20 */

void *func_0035D250(void *self, s32 i) {
    return D_00444360[i];
}

/* (self->*D_01991A58[i])(a, b) */
s32 func_0035D270(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A58[i & 0xFF], a, b);
}

s32 func_0035D2A0(void) { return slam_shake(); }
