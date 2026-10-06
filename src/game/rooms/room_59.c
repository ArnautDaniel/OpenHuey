/* Room 0x59: its event handler class (vtable Room59_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "progress.h"

extern void *RoomBase_vtable[];
extern void *Room59_vtable[];

extern u8 D_00410070[];
extern u8 D_004100B0[];
extern u8 D_00410110[];
extern u8 D_00410140[];
extern u32 D_00410B30[];
extern u8 D_00410BD0[];
extern u32 D_00410B90[];

extern PTMF D_01990DD8[];

/* 0x002B50F0 */
void *Room59_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room59_vtable, RoomBase_vtable); }

/* 0x002B5150 */
void *Room59_EnterScript(void) {
    return D_00410070;
}

/* 0x002B5160 */
void *Room59_CharEnterScript(void) {
    return D_004100B0;
}

/* 0x002B5170 */
void *Room59_Phase1Script(void) {
    return D_00410110;
}

/* 0x002B5180 */
void *Room59_Phase2Script(void) {
    return D_00410140;
}

/* 0x002B5190 */
u32 Room59_ActionScript(void *self, s32 i) {
    return D_00410B30[i];
}

/* 0x002B51B0 */
void *Room59_Table38(void) {
    return D_00410BD0;
}

/* 0x002B51C0 */
u32 Room59_ObjectName(void *self, s32 i) {
    return D_00410B90[i];
}

/* (self->*D_01990DD8[i])(a, b) */
/* 0x002B51E0 */
s32 Room59_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990DD8[i & 0xFF], a, b);
}

/* room 0x59 (Room59_Cmd00_ptmf): character 0xFE's model +0x9E0 = 0.1 (byte 3 0) or 0 */
/* 0x002B5210 */
s32 Room59_Cmd00(void *self, void *a1, u8 *cmd) {
    u8 *m = gCharacters[(u8)Progress_SlotOfId(gProgress, 0xFE)]->motion;

    if (cmd[3] == 0) {
        AT(m, 0x9E0, f32) = 0x1.99999a0000000p-4f /* 0.1 */;
    } else {
        AT(m, 0x9E0, f32) = 0.0f;
    }
    return 1;
}
