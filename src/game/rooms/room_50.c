/* Room 0x50: its event handler class (vtable D_0046E7C0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E7C0[];

extern u8 D_0040C190[];
extern u8 D_0040C270[];
extern u8 D_0040C2C0[];
extern u8 D_0040C420[];
extern u8 D_0040C4C0[];
extern u32 D_0040CEB0[];
extern u8 D_0040CF30[];
extern u32 D_0040CF10[];

extern PTMF D_01990D80[];

void *func_002B4690(void *o, s32 flags) { return room_dtor(o, flags, D_0046E7C0, D_0046DB80); }

void *func_002B46F0(void) {
    return D_0040C190;
}

void *func_002B4700(void) {
    return D_0040C270;
}

void *func_002B4710(void) {
    return D_0040C2C0;
}

void *func_002B4720(void) {
    return D_0040C420;
}

void *func_002B4730(void) {
    return D_0040C4C0;
}

u32 func_002B4740(void *self, s32 i) {
    return D_0040CEB0[i];
}

void *func_002B4760(void) {
    return D_0040CF30;
}

u32 func_002B4770(void *self, s32 i) {
    return D_0040CF10[i];
}

/* (self->*D_01990D80[i])(a, b) */
s32 func_002B4790(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990D80[i & 0xFF], a, b);
}

s32 func_002B47C0(void) {   /* progress flag 0x651 */
    return item238_sound(0x20000);
}
