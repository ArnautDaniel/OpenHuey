/* Room 0x51: its event handler class (vtable D_0046E800, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E800[];
extern u8 D_0047AB98[];

extern u8 D_0040CF40[];
extern u8 D_0040D000[];
extern u8 D_0040D060[];
extern u8 D_0040D1F0[];
extern u32 D_0040E2E0[];
extern u8 D_0040E3A0[];
extern u32 D_0040E360[];

extern PTMF D_01990D90[];

void *func_002B4840(void *o, s32 flags) { return room_dtor(o, flags, D_0046E800, D_0046DB80); }

void *func_002B48A0(void) {
    return D_0040CF40;
}

void *func_002B48B0(void) {
    return D_0040D000;
}

void *func_002B48C0(void) {
    return D_0040D060;
}

void *func_002B48D0(void) {
    return D_0040D1F0;
}

void *func_002B48E0(void *o) { return D_0047AB98; }   /* D_0046E800 +0x20 */

u32 func_002B48F0(void *self, s32 i) {
    return D_0040E2E0[i];
}

void *func_002B4910(void) {
    return D_0040E3A0;
}

u32 func_002B4920(void *self, s32 i) {
    return D_0040E360[i];
}

/* (self->*D_01990D90[i])(a, b) */
s32 func_002B4940(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990D90[i & 0xFF], a, b);
}

s32 func_002B4970(void) {   /* progress flag 0x650 */
    return item238_sound(0x10000);
}

/* room 0x51 (D_0040E340): byte 3 0 door 0 set going (+0xC); else wait (2) while it moves */
s32 func_002B49F0(void *self, void *a1, u8 *cmd) {
    if (cmd[3] != 0) {
        return VCALL(gDoors, 0x30, s32 (*)(VObject *, s32))(gDoors, 0) != 0 ? 1 : 2;
    }
    VCALL(gDoors, 0xC, void (*)(VObject *, s32, s32, s32, s32))(gDoors, 0, 1, 0, 0);
    return 1;
}
