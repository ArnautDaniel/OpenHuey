/* Room 0x1A: its event handler class (vtable D_0046E100, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E100[];
extern void *D_0046F5F0[];

extern u8 D_003FC060[];
extern u8 D_003FC0A0[];
extern u8 D_003FC110[];
extern u8 D_003FC220[];
extern u8 D_003FC290[];
extern void *D_003FC630[];
extern void *D_003FC678[];
extern u8 D_003FC690[];
extern PTMF D_01990960[];

void *func_002AD420(void *o, s32 flags) { return room_dtor(o, flags, D_0046E100, D_0046DB80); }

void *func_002AD480(void) {
    return D_003FC060;
}

void *func_002AD490(void) {
    return D_003FC0A0;
}

void *func_002AD4A0(void) {
    return D_003FC110;
}

void *func_002AD4B0(void) {
    return D_003FC220;
}

void *func_002AD4C0(void) {
    return D_003FC290;
}

void *func_002AD4D0(void *self, s32 i) {
    return D_003FC630[i];
}

void *func_002AD4F0(void) {
    return D_003FC690;
}

void *func_002AD500(void *self, s32 i) {
    return D_003FC678[i];
}

/* (self->*D_01990960[i])(a, b) */
s32 func_002AD520(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990960[i & 0xFF], a, b);
}

s32 func_002AD550(void) {   /* room effect 0 (D_0046F5F0) */
    room_effect_slot_new(gRoomEffects, 0, D_0046F5F0);
    return 1;
}

/* room 0x1A (D_003FC660): the fan turns */
s32 func_002AD600(void) {
    fan_turn(D_003FC680);
    return 1;
}
