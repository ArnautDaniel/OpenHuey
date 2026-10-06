/* Room 0xC6: its event handler class (vtable D_004786F0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_004786F0[];
extern u8 D_0047AF30[];

extern u32 D_00441170[];
extern u32 D_004411A0[];
extern u32 D_004412A0[];
extern u32 D_00441400[];
extern u32 D_00441500[];
extern u32 D_00441510[];
extern u32 D_0047AF38[];

/* 0x0034B4C0 */
void *RoomC6_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_004786F0, D_0046DB80); }

/* 0x0034B520 */
void *RoomC6_EnterScript(void) {
    return D_00441170;
}

/* 0x0034B530 */
void *RoomC6_CharEnterScript(void) {
    return D_004411A0;
}

/* 0x0034B540 */
void *RoomC6_Phase1Script(void) {
    return D_004412A0;
}

/* 0x0034B550 */
void *RoomC6_Phase2Script(void) {
    return D_00441400;
}

/* 0x0034B560 */
void *RoomC6_Phase5Script(void *o) { return D_0047AF30; }   /* D_004786F0 +0x20 */

/* 0x0034B570 */
u32 RoomC6_ActionScript(void *self, s32 i) {
    return D_00441500[i];
}

/* 0x0034B590 */
void *RoomC6_Table38(void) {
    return D_00441510;
}

/* 0x0034B5A0 */
u32 RoomC6_ObjectName(void *self, s32 i) {
    return D_0047AF38[i];
}
