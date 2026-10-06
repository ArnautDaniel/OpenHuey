/* Room 0x59: its event handler class (vtable D_0046E940, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E940[];
extern s32 func_001770D0(Progress *p, s32 kind);   /* the slot of character kind (0xFF) */

extern u8 D_00410070[];
extern u8 D_004100B0[];
extern u8 D_00410110[];
extern u8 D_00410140[];
extern u32 D_00410B30[];
extern u8 D_00410BD0[];
extern u32 D_00410B90[];

extern PTMF D_01990DD8[];

void *func_002B50F0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E940, D_0046DB80); }

void *func_002B5150(void) {
    return D_00410070;
}

void *func_002B5160(void) {
    return D_004100B0;
}

void *func_002B5170(void) {
    return D_00410110;
}

void *func_002B5180(void) {
    return D_00410140;
}

u32 func_002B5190(void *self, s32 i) {
    return D_00410B30[i];
}

void *func_002B51B0(void) {
    return D_00410BD0;
}

u32 func_002B51C0(void *self, s32 i) {
    return D_00410B90[i];
}

/* (self->*D_01990DD8[i])(a, b) */
s32 func_002B51E0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990DD8[i & 0xFF], a, b);
}

/* room 0x59 (D_00410B80): character 0xFE's model +0x9E0 = 0.1 (byte 3 0) or 0 */
s32 func_002B5210(void *self, void *a1, u8 *cmd) {
    u8 *m = gCharacters[(u8)func_001770D0(gProgress, 0xFE)]->motion;

    if (cmd[3] == 0) {
        AT(m, 0x9E0, f32) = 0x1.99999a0000000p-4f /* 0.1 */;
    } else {
        AT(m, 0x9E0, f32) = 0.0f;
    }
    return 1;
}
