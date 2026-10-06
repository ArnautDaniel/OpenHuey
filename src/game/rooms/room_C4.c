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

/* 0x0034B000 */
void *RoomC4_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00478670, D_0046DB80); }

/* 0x0034B060 */
void *RoomC4_EnterScript(void *o) { return D_0047AF0C; }   /* D_00478670 +0xC */

/* 0x0034B070 */
void *RoomC4_CharEnterScript(void) {
    return D_00440100;
}

/* 0x0034B080 */
void *RoomC4_Phase1Script(void) {
    return D_00440140;
}

/* 0x0034B090 */
u32 RoomC4_ActionScript(void *self, s32 i) {
    return D_0047AF14[i];
}

/* 0x0034B0B0 */
void *RoomC4_Table38(void *o) { return D_0047AF1C; }   /* D_00478670 +0x38 */

/* 0x0034B0C0 */
u32 RoomC4_ObjectName(void *self, s32 i) {
    return D_0047AF18[i];
}
