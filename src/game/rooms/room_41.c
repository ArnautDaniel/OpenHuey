/* Room 0x41: its event handler class (vtable D_0046B7B0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046B7B0[], *D_0046DB80[];

extern u8 D_0041CA90[];
extern u8 D_0041CB20[];
extern u8 D_0041CC00[];
extern u8 D_0041CD90[];
extern u8 D_0041CDF0[];
extern u8 D_0041CEE8[];
extern void *D_0041D100[];
extern u8 D_0041D120[];

/* 0x0020B970 */
void *Room41_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046B7B0, D_0046DB80); }

/* 0x0020B9D0 */
void *Room41_EnterScript(void) {
    return D_0041CA90;
}

/* 0x0020B9E0 */
void *Room41_CharEnterScript(void) {
    return D_0041CB20;
}

/* 0x0020B9F0 */
void *Room41_Phase1Script(void) {
    return D_0041CC00;
}

/* 0x0020BA00 */
void *Room41_Phase2Script(void) {
    return D_0041CD90;
}

/* 0x0020BA10 */
void *Room41_Phase3Script(void) {
    return D_0041CDF0;
}

/* 0x0020BA20 */
void *Room41_Phase5Script(void) {
    return D_0041CEE8;
}

/* 0x0020BA30 */
void *Room41_ActionScript(void *self, s32 i) {
    return D_0041D100[i];
}

/* 0x0020BA50 */
void *Room41_Table38(void) {
    return D_0041D120;
}
