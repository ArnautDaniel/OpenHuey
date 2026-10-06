/* Room 0x36: its event handler class (vtable D_00479620, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00479620[];
extern u8 D_0047AFC0[];
extern u8 D_0047AFD0[];

extern void *D_0047AFC8[];
extern u8 D_00443E60[];

void *func_0035AEB0(void *o, s32 flags) { return room_dtor(o, flags, D_00479620, D_0046DB80); }

void *func_0035AF10(void *o) { return D_0047AFC0; }   /* D_00479620 +0xC */

void *func_0035AF20(void) { return D_00443E60; }

void *func_0035AF30(void *self, s32 i) { return D_0047AFC8[i]; }

u32 func_0035AF50(void *o, s32 i) { return ((u32 *)D_0047AFD0)[i]; }   /* D_00479620 +0x34 */
