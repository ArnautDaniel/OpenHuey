/* Room 0x4A: its event handler class (vtable D_0046E680, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E680[];

extern u8 D_004086A0[];
extern u8 D_00408700[];
extern u8 D_00408780[];
extern u8 D_00408840[];
extern u8 D_00408880[];
extern u32 D_00408B40[];
extern u8 D_00408B78[];
extern u32 D_00408B68[];

void *func_002B2EF0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E680, D_0046DB80); }

void *func_002B2F50(void) {
    return D_004086A0;
}

void *func_002B2F60(void) {
    return D_00408700;
}

void *func_002B2F70(void) {
    return D_00408780;
}

void *func_002B2F80(void) {
    return D_00408840;
}

void *func_002B2F90(void) {
    return D_00408880;
}

u32 func_002B2FA0(void *self, s32 i) {
    return D_00408B40[i];
}

void *func_002B2FC0(void) {
    return D_00408B78;
}

u32 func_002B2FD0(void *self, s32 i) {
    return D_00408B68[i];
}
