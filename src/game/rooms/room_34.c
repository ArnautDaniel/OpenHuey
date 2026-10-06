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

void *func_0034DBF0(void *o, s32 flags) { return room_dtor(o, flags, D_00478AA0, D_0046DB80); }

void *func_0034DC50(void *o) { return D_0047AF58; }   /* D_00478AA0 +0xC */

void *func_0034DC60(void) {
    return D_00442950;
}

void *func_0034DC70(void *o) { return D_0047AF5C; }   /* D_00478AA0 +0x10 */

u32 func_0034DC80(void *self, s32 i) {
    return D_00442C10[i];
}

u32 func_0034DCA0(void *self, s32 i) {
    return D_0047AF60[i];
}
