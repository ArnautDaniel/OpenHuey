/* Room 0x0D: its event handler class (vtable D_0046DE40, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046DE40[];
extern u8 D_0047A9D0[], D_0047A9DC[], D_0047A9E0[];

extern u8 D_003F5490[];
extern u8 D_003F54D0[];

void *func_002AB820(void *o, s32 flags) { return room_dtor(o, flags, D_0046DE40, D_0046DB80); }

/* a table of the room's (by vtable slot) */
void *func_002AB880(void *o) { return D_0047A9D0; }   /* D_0046DE40 +0xC */

void *func_002AB890(void) {
    return D_003F5490;
}

void *func_002AB8A0(void) {
    return D_003F54D0;
}

u32 func_002AB8B0(void *o, s32 i) { return ((u32 *)D_0047A9DC)[i]; }   /* D_0046DE40 +0x24 */

u32 func_002AB8D0(void *o, s32 i) { return ((u32 *)D_0047A9E0)[i]; }   /* D_0046DE40 +0x34 */
