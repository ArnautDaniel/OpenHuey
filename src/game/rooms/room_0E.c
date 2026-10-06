/* Room 0x0E: its event handler class (vtable Room0E_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room0E_vtable[];
extern u8 D_0047A9E8[];

extern u8 D_003F5540[];
extern u8 D_003F5610[];
extern u8 D_003F5710[];
extern u8 D_003F57E0[];
extern u8 D_003F5980[];
extern void *D_003F5E80[];
extern u8 D_003F5EA0[];

/* 0x002AB8F0 */
void *Room0E_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room0E_vtable, RoomBase_vtable); }

/* 0x002AB950 */
void *Room0E_EnterScript(void) {
    return D_003F5540;
}

/* 0x002AB960 */
void *Room0E_CharEnterScript(void) {
    return D_003F5610;
}

/* 0x002AB970 */
void *Room0E_Phase1Script(void) {
    return D_003F5710;
}

/* 0x002AB980 */
void *Room0E_Phase2Script(void) {
    return D_003F57E0;
}

/* 0x002AB990 */
void *Room0E_Phase3Script(void) {
    return D_003F5980;
}

/* 0x002AB9A0 */
void *Room0E_ActionScript(void *self, s32 i) {
    return D_003F5E80[i];
}

/* 0x002AB9C0 */
void *Room0E_Table38(void) {
    return D_003F5EA0;
}

/* 0x002AB9D0 */
u32 Room0E_ObjectName(void *o, s32 i) { return ((u32 *)D_0047A9E8)[i]; }   /* Room0E_vtable +0x34 */
