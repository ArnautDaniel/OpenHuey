/* Room 0x81: its event handler class (vtable D_004771C0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_004771C0[];

extern u8 D_00430AB0[];
extern u8 D_00430C40[];
extern u8 D_00430D80[];
extern u8 D_00430F30[];

extern u32 D_00430F90[];
extern u32 D_00430FB0[];
extern u32 D_00431450[];
extern u32 D_00431480[];
extern u32 D_00431490[];

void *func_0033EF80(void *o, s32 flags) { return room_dtor(o, flags, D_004771C0, D_0046DB80); }

void *func_0033EFE0(void) {
    return D_00430AB0;
}

void *func_0033EFF0(void) {
    return D_00430C40;
}

void *func_0033F000(void) {
    return D_00430D80;
}

void *func_0033F010(void) {
    return D_00430F30;
}

void *func_0033F020(void) {
    return D_00430FB0;
}

u32 func_0033F030(void *self, s32 i) {
    return D_00431450[i];
}

void *func_0033F050(void) {
    return D_00430F90;
}

void *func_0033F060(void) {
    return D_00431490;
}

u32 func_0033F070(void *self, s32 i) {
    return D_00431480[i];
}
