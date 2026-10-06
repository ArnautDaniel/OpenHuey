/* Room 0x81: its event handler class (vtable Room81_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room81_vtable[];

extern u8 D_00430AB0[];
extern u8 D_00430C40[];
extern u8 D_00430D80[];
extern u8 D_00430F30[];

extern u32 D_00430F90[];
extern u32 D_00430FB0[];
extern u32 D_00431450[];
extern u32 D_00431480[];
extern u32 D_00431490[];

/* 0x0033EF80 */
void *Room81_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room81_vtable, RoomBase_vtable); }

/* 0x0033EFE0 */
void *Room81_EnterScript(void) {
    return D_00430AB0;
}

/* 0x0033EFF0 */
void *Room81_CharEnterScript(void) {
    return D_00430C40;
}

/* 0x0033F000 */
void *Room81_Phase1Script(void) {
    return D_00430D80;
}

/* 0x0033F010 */
void *Room81_Phase2Script(void) {
    return D_00430F30;
}

/* 0x0033F020 */
void *Room81_Phase3Script(void) {
    return D_00430FB0;
}

/* 0x0033F030 */
u32 Room81_ActionScript(void *self, s32 i) {
    return D_00431450[i];
}

/* 0x0033F050 */
void *Room81_Phase5Script(void) {
    return D_00430F90;
}

/* 0x0033F060 */
void *Room81_Table38(void) {
    return D_00431490;
}

/* 0x0033F070 */
u32 Room81_ObjectName(void *self, s32 i) {
    return D_00431480[i];
}
