/* Room 0x6B: its event handler class (vtable D_00477610, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00477610[];
extern u8 D_0047AE84[];

extern u32 D_00438D30[];
extern u32 D_00438D60[];
extern u32 D_00438E20[];
extern u32 D_00438F10[];
extern u32 D_00439130[];
extern u32 D_00439150[];
extern u32 D_0047AE90[];

/* 0x00344870 */
void *Room6B_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00477610, D_0046DB80); }

/* 0x003448D0 */
void *Room6B_EnterScript(void) {
    return D_00438D30;
}

/* 0x003448E0 */
void *Room6B_CharEnterScript(void) {
    return D_00438D60;
}

/* 0x003448F0 */
void *Room6B_Phase1Script(void) {
    return D_00438E20;
}

/* 0x00344900 */
void *Room6B_Phase2Script(void) {
    return D_00438F10;
}

/* 0x00344910 */
void *Room6B_Phase5Script(void *o) { return D_0047AE84; }   /* D_00477610 +0x20 */

/* 0x00344920 */
u32 Room6B_ActionScript(void *self, s32 i) {
    return D_00439130[i];
}

/* 0x00344940 */
void *Room6B_Table38(void) {
    return D_00439150;
}

/* 0x00344950 */
u32 Room6B_ObjectName(void *self, s32 i) {
    return D_0047AE90[i];
}
