/* Room 0x11: its event handler class (vtable D_0046DF40, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046DF40[];
extern u8 D_0047A9F8[];

extern u8 D_003F7920[];
extern u8 D_003F7A20[];
extern u8 D_003F7AC0[];
extern u8 D_003F7E00[];
extern void *D_003F81D0[];
extern u8 D_003F81F0[];

void *func_002AC0A0(void *o, s32 flags) { return room_dtor(o, flags, D_0046DF40, D_0046DB80); }

void *func_002AC100(void) {
    return D_003F7920;
}

void *func_002AC110(void) {
    return D_003F7A20;
}

void *func_002AC120(void) {
    return D_003F7AC0;
}

void *func_002AC130(void) {
    return D_003F7E00;
}

void *func_002AC140(void *self, s32 i) {
    return D_003F81D0[i];
}

void *func_002AC160(void) {
    return D_003F81F0;
}

u32 func_002AC170(void *o, s32 i) { return ((u32 *)D_0047A9F8)[i]; }   /* D_0046DF40 +0x34 */
