/* Room 0x17: its event handler class (vtable D_0046F3F0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046F3F0[];
extern u8 D_0047AC10[];

extern u8 D_00414390[];
extern u8 D_004143B0[];
extern u8 D_00414430[];
extern void *D_0047AC18[];
extern void *D_0047AC20[];

void *func_002D2500(void *o, s32 flags) { return room_dtor(o, flags, D_0046F3F0, D_0046DB80); }

void *func_002D2560(void) { return D_00414390; }

void *func_002D2570(void) { return D_004143B0; }

void *func_002D2580(void) { return D_00414430; }

void *func_002D2590(void *o) { return D_0047AC10; }   /* D_0046F3F0 +0x14 */

void *func_002D25A0(void *self, s32 i) { return D_0047AC18[i]; }

void *func_002D25C0(void *self, s32 i) { return D_0047AC20[i]; }
