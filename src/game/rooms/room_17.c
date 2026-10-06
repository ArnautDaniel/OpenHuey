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

/* 0x002D2500 */
void *Room17_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046F3F0, D_0046DB80); }

/* 0x002D2560 */
void *Room17_EnterScript(void) { return D_00414390; }

/* 0x002D2570 */
void *Room17_CharEnterScript(void) { return D_004143B0; }

/* 0x002D2580 */
void *Room17_Phase1Script(void) { return D_00414430; }

/* 0x002D2590 */
void *Room17_Phase2Script(void *o) { return D_0047AC10; }   /* D_0046F3F0 +0x14 */

/* 0x002D25A0 */
void *Room17_ActionScript(void *self, s32 i) { return D_0047AC18[i]; }

/* 0x002D25C0 */
void *Room17_ObjectName(void *self, s32 i) { return D_0047AC20[i]; }
