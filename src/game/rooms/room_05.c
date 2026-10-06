/* Room 0x05: its event handler class (vtable D_0046B870, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046B870[], *D_0046DB80[];

extern u8 D_003F1800[];
extern u8 D_003F1840[];
extern u8 D_003F1930[];
extern u8 D_003F1A30[];
extern u8 D_003F1A50[];
extern void *D_003F1B40[];

void *func_0020BC10(void *o, s32 flags) { return room_dtor(o, flags, D_0046B870, D_0046DB80); }

void *func_0020BC70(void) {
    return D_003F1800;
}

void *func_0020BC80(void) {
    return D_003F1840;
}

void *func_0020BC90(void) {
    return D_003F1930;
}

void *func_0020BCA0(void) {
    return D_003F1A30;
}

void *func_0020BCB0(void) {
    return D_003F1A50;
}

void *func_0020BCC0(void *self, s32 i) {
    return D_003F1B40[i];
}
