/* Room 0x34: its event handler class (vtable D_00478AA0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00478AA0[];
extern u8 D_0047AF58[], D_0047AF5C[];

extern u32 D_00442950[];
extern u32 D_00442C10[];
extern u32 D_0047AF60[];

/* 0x0034DBF0 */
void *Room34_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00478AA0, D_0046DB80); }

/* 0x0034DC50 */
void *Room34_EnterScript(void *o) { return D_0047AF58; }   /* D_00478AA0 +0xC */

/* 0x0034DC60 */
void *Room34_CharEnterScript(void) {
    return D_00442950;
}

/* 0x0034DC70 */
void *Room34_Phase1Script(void *o) { return D_0047AF5C; }   /* D_00478AA0 +0x10 */

/* 0x0034DC80 */
u32 Room34_ActionScript(void *self, s32 i) {
    return D_00442C10[i];
}

/* 0x0034DCA0 */
u32 Room34_ObjectName(void *self, s32 i) {
    return D_0047AF60[i];
}
