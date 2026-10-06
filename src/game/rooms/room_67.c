/* Room 0x67: its event handler class (vtable D_0046B570, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046B570[];
extern void *D_0046DB80[];

extern u8 D_0041DD20[];
extern u8 D_0041DD90[];
extern u8 D_0041DE10[];
extern u8 D_0041DF60[];
extern u8 D_0041DF70[];
extern void *D_0041E0E0[];
extern u8 D_0041E0F0[];

/* 0x0020B2A0 */
void *Room67_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046B570, D_0046DB80); }

/* 0x0020B300 */
void *Room67_EnterScript(void) {
    return D_0041DD20;
}

/* 0x0020B310 */
void *Room67_CharEnterScript(void) {
    return D_0041DD90;
}

/* 0x0020B320 */
void *Room67_Phase1Script(void) {
    return D_0041DE10;
}

/* 0x0020B330 */
void *Room67_Phase2Script(void) {
    return D_0041DF70;
}

/* 0x0020B340 */
void *Room67_Phase5Script(void) {
    return D_0041DF60;
}

/* 0x0020B350 */
void *Room67_ActionScript(void *self, s32 i) {
    return D_0041E0E0[i];
}

/* 0x0020B370 */
void *Room67_Table38(void) {
    return D_0041E0F0;
}
