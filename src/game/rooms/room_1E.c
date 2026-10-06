/* Room 0x1E: its event handler class (vtable D_0046E200, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E200[];

extern u8 D_003FD9C0[];
extern u8 D_003FDA40[];
extern u8 D_003FDB00[];
extern u8 D_003FDEC8[];
extern u8 D_003FDEE0[];
extern u32 D_003FE2C0[];
extern u8 D_003FE2F0[];
extern u32 D_0047AAA0[];

/* 0x002ADF90 */
void *Room1E_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046E200, D_0046DB80); }

/* 0x002ADFF0 */
void *Room1E_EnterScript(void) {
    return D_003FD9C0;
}

/* 0x002AE000 */
void *Room1E_CharEnterScript(void) {
    return D_003FDA40;
}

/* 0x002AE010 */
void *Room1E_Phase1Script(void) {
    return D_003FDB00;
}

/* 0x002AE020 */
void *Room1E_Phase2Script(void) {
    return D_003FDEC8;
}

/* 0x002AE030 */
void *Room1E_Phase5Script(void) {
    return D_003FDEE0;
}

/* 0x002AE040 */
u32 Room1E_ActionScript(void *self, s32 i) {
    return D_003FE2C0[i];
}

/* 0x002AE060 */
void *Room1E_Table38(void) {
    return D_003FE2F0;
}

/* 0x002AE070 */
u32 Room1E_ObjectName(void *self, s32 i) {
    return D_0047AAA0[i];
}
