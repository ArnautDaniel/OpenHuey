/* Room 0x100: its event handler class (vtable D_0046B4F0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046B4F0[];
extern void *D_0046DB80[];

extern u8 D_00417190[];
extern u8 D_00417290[];
extern u8 D_00417310[];
extern u8 D_0047AC50[];

/* 0x0020B130 */
void *Room100_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046B4F0, D_0046DB80); }

/* 0x0020B190 */
void *Room100_EnterScript(void) {
    return D_0047AC50;
}

/* 0x0020B1A0 */
void *Room100_CharEnterScript(void) {
    return D_00417190;
}

/* 0x0020B1B0 */
void *Room100_Phase1Script(void) {
    return D_00417290;
}

/* 0x0020B1C0 */
void *Room100_Table38(void) {
    return D_00417310;
}
