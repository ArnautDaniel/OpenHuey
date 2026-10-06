/* Room 0x22: its event handler class (vtable D_0046E300, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E300[];

extern u8 D_00400C60[];
extern u8 D_00400C90[];
extern u8 D_00400CD0[];
extern u8 D_00400D90[];
extern u8 D_00400E30[];
extern u32 D_00401070[];
extern u8 D_00401090[];
extern u32 D_0047AAF8[];

/* 0x002AF7E0 */
void *Room22_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046E300, D_0046DB80); }

/* 0x002AF840 */
void *Room22_EnterScript(void) {
    return D_00400C60;
}

/* 0x002AF850 */
void *Room22_CharEnterScript(void) {
    return D_00400C90;
}

/* 0x002AF860 */
void *Room22_Phase1Script(void) {
    return D_00400CD0;
}

/* 0x002AF870 */
void *Room22_Phase2Script(void) {
    return D_00400D90;
}

/* 0x002AF880 */
void *Room22_Phase3Script(void) {
    return D_00400E30;
}

/* 0x002AF890 */
u32 Room22_ActionScript(void *self, s32 i) {
    return D_00401070[i];
}

/* 0x002AF8B0 */
void *Room22_Table38(void) {
    return D_00401090;
}

/* 0x002AF8C0 */
u32 Room22_ObjectName(void *self, s32 i) {
    return D_0047AAF8[i];
}
