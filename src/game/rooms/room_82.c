/* Room 0x82: its event handler class (vtable D_0046B530, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046B530[];
extern void *D_0046DB80[];

extern u8 D_004314B0[];
extern u8 D_004315E0[];
extern u8 D_00431750[];
extern u8 D_00431A60[];
extern void *D_00431DB0[];
extern u8 D_00431DD0[];

/* 0x0020B1D0 */
void *Room82_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046B530, D_0046DB80); }

/* 0x0020B230 */
void *Room82_EnterScript(void) {
    return D_004314B0;
}

/* 0x0020B240 */
void *Room82_CharEnterScript(void) {
    return D_004315E0;
}

/* 0x0020B250 */
void *Room82_Phase1Script(void) {
    return D_00431750;
}

/* 0x0020B260 */
void *Room82_Phase2Script(void) {
    return D_00431A60;
}

/* 0x0020B270 */
void *Room82_ActionScript(void *self, s32 i) {
    return D_00431DB0[i];
}

/* 0x0020B290 */
void *Room82_Table38(void) {
    return D_00431DD0;
}
