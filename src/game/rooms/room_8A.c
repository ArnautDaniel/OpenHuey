/* Room 0x8A: its event handler class (vtable D_00477340, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00477340[];

extern u32 D_00433580[];
extern u32 D_00433600[];
extern u32 D_00433700[];
extern u32 D_004338F0[];
extern u32 D_00433C80[];
extern u32 D_00433CA0[];
extern u32 D_0047AE48[];

void *func_0033F840(void *o, s32 flags) { return room_dtor(o, flags, D_00477340, D_0046DB80); }

void *func_0033F8A0(void) {
    return D_00433580;
}

void *func_0033F8B0(void) {
    return D_00433600;
}

void *func_0033F8C0(void) {
    return D_00433700;
}

void *func_0033F8D0(void) {
    return D_004338F0;
}

u32 func_0033F8E0(void *self, s32 i) {
    return D_00433C80[i];
}

void *func_0033F900(void) {
    return D_00433CA0;
}

u32 func_0033F910(void *self, s32 i) {
    return D_0047AE48[i];
}
