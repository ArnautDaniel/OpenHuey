/* Room 0x86: its event handler class (vtable D_00474520, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00474520[];

extern u8 D_0042C770[];
extern u8 D_0042C790[];
extern u8 D_0042C810[];
extern u8 D_0042C860[];
extern void *D_0047ADB4[];
extern void *D_0047ADB8[];

extern PTMF D_019915E8[];

void *func_0032C690(void *o, s32 flags) { return room_dtor(o, flags, D_00474520, D_0046DB80); }

void *func_0032C6F0(void) {
    return D_0042C770;
}

void *func_0032C700(void) {
    return D_0042C790;
}

void *func_0032C710(void) {
    return D_0042C810;
}

void *func_0032C720(void *self, s32 i) {
    return D_0047ADB4[i];
}

void *func_0032C740(void) {
    return D_0042C860;
}

void *func_0032C750(void *self, s32 i) {
    return D_0047ADB8[i];
}

/* (self->*D_019915E8[i])(a, b) */
s32 func_0032C770(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019915E8[i & 0xFF], a, b);
}

s32 func_0032C7A0(void) { return slam_shake(); }
