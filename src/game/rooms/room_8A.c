/* Room 0x8A: its event handler class (vtable Room8A_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room8A_vtable[];

extern u32 D_00433580[];
extern u32 D_00433600[];
extern u32 D_00433700[];
extern u32 D_004338F0[];
extern u32 D_00433C80[];
extern u32 D_00433CA0[];
extern u32 D_0047AE48[];

/* 0x0033F840 */
void *Room8A_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room8A_vtable, RoomBase_vtable); }

/* 0x0033F8A0 */
void *Room8A_EnterScript(void) {
    return D_00433580;
}

/* 0x0033F8B0 */
void *Room8A_CharEnterScript(void) {
    return D_00433600;
}

/* 0x0033F8C0 */
void *Room8A_Phase1Script(void) {
    return D_00433700;
}

/* 0x0033F8D0 */
void *Room8A_Phase2Script(void) {
    return D_004338F0;
}

/* 0x0033F8E0 */
u32 Room8A_ActionScript(void *self, s32 i) {
    return D_00433C80[i];
}

/* 0x0033F900 */
void *Room8A_Table38(void) {
    return D_00433CA0;
}

/* 0x0033F910 */
u32 Room8A_ObjectName(void *self, s32 i) {
    return D_0047AE48[i];
}
