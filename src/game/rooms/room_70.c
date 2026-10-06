/* Room 0x70: its event handler class (vtable Room70_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room70_vtable[];

extern u32 D_0043B020[];
extern u32 D_0043B0D0[];
extern u32 D_0043B1D0[];
extern u32 D_0043B2C0[];
extern u32 D_0043B390[];
extern u32 D_0043B560[];
extern u32 D_0043B588[];
extern u32 D_0043B5A0[];

/* 0x00345000 */
void *Room70_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room70_vtable, RoomBase_vtable); }

/* 0x00345060 */
void *Room70_EnterScript(void) {
    return D_0043B020;
}

/* 0x00345070 */
void *Room70_CharEnterScript(void) {
    return D_0043B0D0;
}

/* 0x00345080 */
void *Room70_Phase1Script(void) {
    return D_0043B1D0;
}

/* 0x00345090 */
void *Room70_Phase2Script(void) {
    return D_0043B2C0;
}

/* 0x003450A0 */
void *Room70_Phase5Script(void) {
    return D_0043B390;
}

/* 0x003450B0 */
u32 Room70_ActionScript(void *self, s32 i) {
    return D_0043B560[i];
}

/* 0x003450D0 */
void *Room70_Table38(void) {
    return D_0043B5A0;
}

/* 0x003450E0 */
u32 Room70_ObjectName(void *self, s32 i) {
    return D_0043B588[i];
}
