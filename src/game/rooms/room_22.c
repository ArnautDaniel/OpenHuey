/* Room 0x22: its event handler class (vtable D_0046E300, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E300[];

extern u8 D_00400C60[];
extern u8 D_00400C90[];
extern u8 D_00400CD0[];
extern u8 D_00400D90[];
extern u8 D_00400E30[];
extern u32 D_00401070[];
extern u8 D_00401090[];
extern u32 D_0047AAF8[];

void *func_002AF7E0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E300, D_0046DB80); }

void *func_002AF840(void) {
    return D_00400C60;
}

void *func_002AF850(void) {
    return D_00400C90;
}

void *func_002AF860(void) {
    return D_00400CD0;
}

void *func_002AF870(void) {
    return D_00400D90;
}

void *func_002AF880(void) {
    return D_00400E30;
}

u32 func_002AF890(void *self, s32 i) {
    return D_00401070[i];
}

void *func_002AF8B0(void) {
    return D_00401090;
}

u32 func_002AF8C0(void *self, s32 i) {
    return D_0047AAF8[i];
}
