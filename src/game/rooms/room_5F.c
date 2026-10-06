/* Room 0x5F: its event handler class (vtable Room5F_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *Room5F_vtable[];
extern void *RoomBase_vtable[];

extern u8 D_0041B130[];
extern u8 D_0041B1B0[];
extern u8 D_0041B270[];
extern u8 D_0041B370[];
extern u8 D_0041B450[];
extern u8 D_0041B560[];
extern void *D_0047ACA0[];

/* 0x0020B4C0 */
void *Room5F_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room5F_vtable, RoomBase_vtable); }

/* 0x0020B520 */
void *Room5F_EnterScript(void) {
    return D_0041B130;
}

/* 0x0020B530 */
void *Room5F_CharEnterScript(void) {
    return D_0041B1B0;
}

/* 0x0020B540 */
void *Room5F_Phase1Script(void) {
    return D_0041B270;
}

/* 0x0020B550 */
void *Room5F_Phase2Script(void) {
    return D_0041B370;
}

/* 0x0020B560 */
void *Room5F_Phase3Script(void) {
    return D_0041B450;
}

/* 0x0020B570 */
void *Room5F_ActionScript(void *self, s32 i) {
    return D_0047ACA0[i];
}

/* 0x0020B590 */
void *Room5F_Table38(void) {
    return D_0041B560;
}
