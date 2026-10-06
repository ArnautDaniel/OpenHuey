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

/* 0x002AB820 */
void *Room0D_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046DE40, D_0046DB80); }

/* a table of the room's (by vtable slot) */
/* 0x002AB880 */
void *Room0D_EnterScript(void *o) { return D_0047A9D0; }   /* D_0046DE40 +0xC */

/* 0x002AB890 */
void *Room0D_CharEnterScript(void) {
    return D_003F5490;
}

/* 0x002AB8A0 */
void *Room0D_Phase1Script(void) {
    return D_003F54D0;
}

/* 0x002AB8B0 */
u32 Room0D_ActionScript(void *o, s32 i) { return ((u32 *)D_0047A9DC)[i]; }   /* D_0046DE40 +0x24 */

/* 0x002AB8D0 */
u32 Room0D_ObjectName(void *o, s32 i) { return ((u32 *)D_0047A9E0)[i]; }   /* D_0046DE40 +0x34 */
