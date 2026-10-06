/* Room 0x44: its event handler class (vtable Room44_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *Room44_vtable[];
extern void *RoomBase_vtable[];

extern u8 D_00415BA0[];
extern u8 D_00415C90[];
extern u8 D_00415DB0[];
extern u8 D_00415FA0[];
extern u8 D_00416030[];
extern u8 D_00416130[];
extern void *D_004164C0[];
extern u8 D_004164D0[];

/* 0x0020B7B0 */
void *Room44_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room44_vtable, RoomBase_vtable); }

/* 0x0020B810 */
void *Room44_EnterScript(void) {
    return D_00415BA0;
}

/* 0x0020B820 */
void *Room44_CharEnterScript(void) {
    return D_00415C90;
}

/* 0x0020B830 */
void *Room44_Phase1Script(void) {
    return D_00415DB0;
}

/* 0x0020B840 */
void *Room44_Phase2Script(void) {
    return D_00415FA0;
}

/* 0x0020B850 */
void *Room44_Phase3Script(void) {
    return D_00416030;
}

/* 0x0020B860 */
void *Room44_Phase5Script(void) {
    return D_00416130;
}

/* 0x0020B870 */
void *Room44_ActionScript(void *self, s32 i) {
    return D_004164C0[i];
}

/* 0x0020B890 */
void *Room44_Table38(void) {
    return D_004164D0;
}
