/* Room 0x68: its event handler class (vtable D_00471E60, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00471E60[];

extern u8 D_00424880[];
extern u8 D_00424890[];
extern u8 D_00424990[];
extern u8 D_00424AC0[];
extern u8 D_00424AF0[];
extern void *D_0047AD20[];
extern void *D_0047AD24[];

/* 0x0030EE90 */
void *Room68_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00471E60, D_0046DB80); }

/* 0x0030EEF0 */
void *Room68_EnterScript(void) {
    return D_00424880;
}

/* 0x0030EF00 */
void *Room68_CharEnterScript(void) {
    return D_00424890;
}

/* 0x0030EF10 */
void *Room68_Phase1Script(void) {
    return D_00424990;
}

/* 0x0030EF20 */
void *Room68_Phase2Script(void) {
    return D_00424AC0;
}

/* 0x0030EF30 */
void *Room68_Phase4Script(void) {
    return D_00424AF0;
}

/* 0x0030EF40 */
void *Room68_ActionScript(void *self, s32 i) {
    return D_0047AD20[i];
}

/* 0x0030EF60 */
void *Room68_ObjectName(void *self, s32 i) {
    return D_0047AD24[i];
}
