/* Room 0x83: its event handler class (vtable Room83_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room83_vtable[];

extern u32 D_00431DE0[];
extern u32 D_00431E20[];
extern u32 D_00431E80[];
extern u32 D_00431EF0[];
extern u32 D_00431F90[];
extern u32 D_004323B0[];
extern u32 D_004323F0[];
extern u32 D_00432408[];

/* 0x0033F090 */
void *Room83_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room83_vtable, RoomBase_vtable); }

/* 0x0033F0F0 */
void *Room83_EnterScript(void) {
    return D_00431DE0;
}

/* 0x0033F100 */
void *Room83_CharEnterScript(void) {
    return D_00431E20;
}

/* 0x0033F110 */
void *Room83_Phase1Script(void) {
    return D_00431E80;
}

/* 0x0033F120 */
void *Room83_Phase2Script(void) {
    return D_00431EF0;
}

/* 0x0033F130 */
void *Room83_Phase3Script(void) {
    return D_00431F90;
}

/* 0x0033F140 */
u32 Room83_ActionScript(void *self, s32 i) {
    return D_004323B0[i];
}

/* 0x0033F160 */
void *Room83_Table38(void) {
    return D_00432408;
}

/* 0x0033F170 */
u32 Room83_ObjectName(void *self, s32 i) {
    return D_004323F0[i];
}
