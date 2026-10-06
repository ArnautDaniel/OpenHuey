/* Room 0x33: its event handler class (vtable D_00478B80, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00478B80[];
extern u8 D_0047AF70[], D_0047AF78[];

extern u32 D_00442DA0[];
extern u32 D_00442F48[];
extern u32 D_0047AF80[];

/* 0x003506E0 */
void *Room33_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00478B80, D_0046DB80); }

/* 0x00350740 */
void *Room33_EnterScript(void *o) { return D_0047AF70; }   /* D_00478B80 +0xC */

/* 0x00350750 */
void *Room33_CharEnterScript(void) {
    return D_00442DA0;
}

/* 0x00350760 */
void *Room33_Phase1Script(void *o) { return D_0047AF78; }   /* D_00478B80 +0x10 */

/* 0x00350770 */
u32 Room33_ActionScript(void *self, s32 i) {
    return D_00442F48[i];
}

/* 0x00350790 */
u32 Room33_ObjectName(void *self, s32 i) {
    return D_0047AF80[i];
}
