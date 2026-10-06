/* Room 0x0E: its event handler class (vtable D_0046DE80, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046DE80[];
extern u8 D_0047A9E8[];

extern u8 D_003F5540[];
extern u8 D_003F5610[];
extern u8 D_003F5710[];
extern u8 D_003F57E0[];
extern u8 D_003F5980[];
extern void *D_003F5E80[];
extern u8 D_003F5EA0[];

void *func_002AB8F0(void *o, s32 flags) { return room_dtor(o, flags, D_0046DE80, D_0046DB80); }

void *func_002AB950(void) {
    return D_003F5540;
}

void *func_002AB960(void) {
    return D_003F5610;
}

void *func_002AB970(void) {
    return D_003F5710;
}

void *func_002AB980(void) {
    return D_003F57E0;
}

void *func_002AB990(void) {
    return D_003F5980;
}

void *func_002AB9A0(void *self, s32 i) {
    return D_003F5E80[i];
}

void *func_002AB9C0(void) {
    return D_003F5EA0;
}

u32 func_002AB9D0(void *o, s32 i) { return ((u32 *)D_0047A9E8)[i]; }   /* D_0046DE80 +0x34 */
