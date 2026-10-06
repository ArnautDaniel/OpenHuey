/* Room 0x88: its event handler class (vtable D_00477300, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00477300[];

extern u32 D_00432CA0[];
extern u32 D_00432DF0[];
extern u32 D_00432E90[];
extern u32 D_004330C0[];
extern u32 D_004331B0[];
extern u32 D_00433530[];
extern u32 D_00433560[];

extern u32 D_0047AE40[];

extern PTMF D_01991708[];

void *func_0033F680(void *o, s32 flags) { return room_dtor(o, flags, D_00477300, D_0046DB80); }

void *func_0033F6E0(void) {
    return D_00432CA0;
}

void *func_0033F6F0(void) {
    return D_00432DF0;
}

void *func_0033F700(void) {
    return D_00432E90;
}

void *func_0033F710(void) {
    return D_004330C0;
}

void *func_0033F720(void) {
    return D_004331B0;
}

u32 func_0033F730(void *self, s32 i) {
    return D_00433530[i];
}

void *func_0033F750(void) {
    return D_00433560;
}

u32 func_0033F760(void *self, s32 i) {
    return D_0047AE40[i];
}

/* (self->*D_01991708[i])(a, b) */
s32 func_0033F780(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991708[i & 0xFF], a, b);
}

s32 func_0033F7B0(void) { return slam_shake(); }
