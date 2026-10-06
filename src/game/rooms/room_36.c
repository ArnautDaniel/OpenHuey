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

/* 0x0035AEB0 */
void *Room36_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00479620, D_0046DB80); }

/* 0x0035AF10 */
void *Room36_EnterScript(void *o) { return D_0047AFC0; }   /* D_00479620 +0xC */

/* 0x0035AF20 */
void *Room36_CharEnterScript(void) { return D_00443E60; }

/* 0x0035AF30 */
void *Room36_ActionScript(void *self, s32 i) { return D_0047AFC8[i]; }

/* 0x0035AF50 */
u32 Room36_ObjectName(void *o, s32 i) { return ((u32 *)D_0047AFD0)[i]; }   /* D_00479620 +0x34 */
