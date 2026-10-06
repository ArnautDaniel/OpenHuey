/* Room 0x01: its event handler class (vtable D_0046B8B0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046B8B0[], *D_0046DB80[];

extern u8 D_003EE770[];
extern u8 D_003EE820[];
extern u8 D_003EE920[];
extern u8 D_003EEB90[];
extern u8 D_003EEBC0[];
extern u8 D_003EEC00[];
extern void *D_003EEDC0[];
extern u8 D_003EEDD8[];

/* 0x0020BCF0 */
void *Room01_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046B8B0, D_0046DB80); }

/* 0x0020BD50 */
void *Room01_EnterScript(void) {
    return D_003EE770;
}

/* 0x0020BD60 */
void *Room01_CharEnterScript(void) {
    return D_003EE820;
}

/* 0x0020BD70 */
void *Room01_Phase1Script(void) {
    return D_003EE920;
}

/* 0x0020BD80 */
void *Room01_Phase2Script(void) {
    return D_003EEB90;
}

/* 0x0020BD90 */
void *Room01_Phase3Script(void) {
    return D_003EEBC0;
}

/* 0x0020BDA0 */
void *Room01_Phase5Script(void) {
    return D_003EEC00;
}

/* 0x0020BDB0 */
void *Room01_ActionScript(void *self, s32 i) {
    return D_003EEDC0[i];
}

/* 0x0020BDD0 */
void *Room01_Table38(void) {
    return D_003EEDD8;
}
