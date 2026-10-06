/* Room 0x53: its event handler class (vtable D_00471F20, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00471F20[];

extern u8 D_004268B0[];
extern u8 D_004268F0[];
extern u8 D_00426930[];
extern u8 D_00426950[];
extern void *D_00426D10[];
extern void *D_00426D40[];
extern u8 D_00426D70[];

/* 0x0030F850 */
void *Room53_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00471F20, D_0046DB80); }

/* 0x0030F8B0 */
void *Room53_EnterScript(void) {
    return D_004268B0;
}

/* 0x0030F8C0 */
void *Room53_CharEnterScript(void) {
    return D_004268F0;
}

/* 0x0030F8D0 */
void *Room53_Phase1Script(void) {
    return D_00426930;
}

/* 0x0030F8E0 */
void *Room53_Phase2Script(void) {
    return D_00426950;
}

/* 0x0030F8F0 */
void *Room53_ActionScript(void *self, s32 i) {
    return D_00426D10[i];
}

/* 0x0030F910 */
void *Room53_Table38(void) {
    return D_00426D70;
}

/* 0x0030F920 */
void *Room53_ObjectName(void *self, s32 i) {
    return D_00426D40[i];
}
