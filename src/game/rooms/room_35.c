/* Room 0x35: its event handler class (vtable Room35_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room35_vtable[];
extern u8 D_0047AF98[];

extern u32 D_00443080[];
extern u32 D_00443490[];
extern u32 D_004434B0[];

/* 0x00350FF0 */
void *Room35_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room35_vtable, RoomBase_vtable); }

/* 0x00351050 */
void *Room35_EnterScript(void *o) { return D_0047AF98; }   /* Room35_vtable +0xC */

/* 0x00351060 */
void *Room35_CharEnterScript(void) {
    return D_00443080;
}

/* 0x00351070 */
u32 Room35_ActionScript(void *self, s32 i) {
    return D_00443490[i];
}

/* 0x00351090 */
u32 Room35_ObjectName(void *self, s32 i) {
    return D_004434B0[i];
}
