/* Room 0xC4: its event handler class (vtable D_00478670, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00478670[];
extern u8 D_0047AF0C[], D_0047AF1C[];

extern u32 D_00440100[];
extern u32 D_00440140[];
extern u32 D_0047AF14[];
extern u32 D_0047AF18[];

void *func_0034B000(void *o, s32 flags) { return room_dtor(o, flags, D_00478670, D_0046DB80); }

void *func_0034B060(void *o) { return D_0047AF0C; }   /* D_00478670 +0xC */

void *func_0034B070(void) {
    return D_00440100;
}

void *func_0034B080(void) {
    return D_00440140;
}

u32 func_0034B090(void *self, s32 i) {
    return D_0047AF14[i];
}

void *func_0034B0B0(void *o) { return D_0047AF1C; }   /* D_00478670 +0x38 */

u32 func_0034B0C0(void *self, s32 i) {
    return D_0047AF18[i];
}
