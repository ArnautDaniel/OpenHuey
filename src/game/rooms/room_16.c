/* Room 0x16: its event handler class (vtable D_0046B7F0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046B7F0[], *D_0046DB80[];

extern u8 D_003FA780[];
extern u8 D_003FA7C0[];
extern u8 D_003FA800[];
extern u8 D_003FA820[];
extern u8 D_003FA850[];
extern u8 D_003FAA08[];
extern void *D_0047AA38[];

void *func_0020BA60(void *o, s32 flags) { return room_dtor(o, flags, D_0046B7F0, D_0046DB80); }

void *func_0020BAC0(void) {
    return D_003FA780;
}

void *func_0020BAD0(void) {
    return D_003FA7C0;
}

void *func_0020BAE0(void) {
    return D_003FA800;
}

void *func_0020BAF0(void) {
    return D_003FA820;
}

void *func_0020BB00(void) {
    return D_003FA850;
}

void *func_0020BB10(void *self, s32 i) {
    return D_0047AA38[i];
}

void *func_0020BB30(void) {
    return D_003FAA08;
}
