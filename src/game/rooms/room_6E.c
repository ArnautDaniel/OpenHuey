/* Room 0x6E: its event handler class (vtable D_004776D0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_004776D0[];

extern u32 D_0043A120[];
extern u32 D_0043A1A0[];
extern u32 D_0043A230[];
extern u32 D_0043A280[];
extern u32 D_0043A2B0[];
extern u32 D_0043A880[];
extern u32 D_0043A8B0[];
extern u32 D_0043A8D0[];

/* 0x00344D80 */
void *Room6E_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_004776D0, D_0046DB80); }

/* 0x00344DE0 */
void *Room6E_EnterScript(void) {
    return D_0043A120;
}

/* 0x00344DF0 */
void *Room6E_CharEnterScript(void) {
    return D_0043A1A0;
}

/* 0x00344E00 */
void *Room6E_Phase1Script(void) {
    return D_0043A230;
}

/* 0x00344E10 */
void *Room6E_Phase2Script(void) {
    return D_0043A280;
}

/* 0x00344E20 */
void *Room6E_Phase3Script(void) {
    return D_0043A2B0;
}

/* 0x00344E30 */
u32 Room6E_ActionScript(void *self, s32 i) {
    return D_0043A880[i];
}

/* 0x00344E50 */
void *Room6E_Table38(void) {
    return D_0043A8D0;
}

/* 0x00344E60 */
u32 Room6E_ObjectName(void *self, s32 i) {
    return D_0043A8B0[i];
}
