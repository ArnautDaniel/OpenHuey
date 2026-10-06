/* Room 0x65: its event handler class (vtable D_0046B5B0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046B5B0[];
extern void *D_0046DB80[];

extern u8 D_00429640[];
extern u8 D_00429700[];
extern u8 D_0047AD50[];
extern u8 D_0047AD58[];

/* 0x0020B380 */
void *Room65_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046B5B0, D_0046DB80); }

/* 0x0020B3E0 */
void *Room65_EnterScript(void) {
    return D_0047AD50;
}

/* 0x0020B3F0 */
void *Room65_CharEnterScript(void) {
    return D_00429640;
}

/* 0x0020B400 */
void *Room65_Phase1Script(void) {
    return D_00429700;
}

/* 0x0020B410 */
void *Room65_Table38(void) {
    return D_0047AD58;
}
