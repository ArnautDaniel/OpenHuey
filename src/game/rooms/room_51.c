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

/* 0x002B4840 */
void *Room51_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046E800, D_0046DB80); }

/* 0x002B48A0 */
void *Room51_EnterScript(void) {
    return D_0040CF40;
}

/* 0x002B48B0 */
void *Room51_CharEnterScript(void) {
    return D_0040D000;
}

/* 0x002B48C0 */
void *Room51_Phase1Script(void) {
    return D_0040D060;
}

/* 0x002B48D0 */
void *Room51_Phase2Script(void) {
    return D_0040D1F0;
}

/* 0x002B48E0 */
void *Room51_Phase5Script(void *o) { return D_0047AB98; }   /* D_0046E800 +0x20 */

/* 0x002B48F0 */
u32 Room51_ActionScript(void *self, s32 i) {
    return D_0040E2E0[i];
}

/* 0x002B4910 */
void *Room51_Table38(void) {
    return D_0040E3A0;
}

/* 0x002B4920 */
u32 Room51_ObjectName(void *self, s32 i) {
    return D_0040E360[i];
}

/* (self->*D_01990D90[i])(a, b) */
/* 0x002B4940 */
s32 Room51_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990D90[i & 0xFF], a, b);
}

/* 0x002B4970 */
s32 Room51_Cmd01(void) {   /* progress flag 0x650 */
    return item238_sound(0x10000);
}

/* room 0x51 (D_0040E340): byte 3 0 door 0 set going (+0xC); else wait (2) while it moves */
/* 0x002B49F0 */
s32 Room51_Cmd00(void *self, void *a1, u8 *cmd) {
    if (cmd[3] != 0) {
        return VCALL(gDoors, 0x30, s32 (*)(VObject *, s32))(gDoors, 0) != 0 ? 1 : 2;
    }
    VCALL(gDoors, 0xC, void (*)(VObject *, s32, s32, s32, s32))(gDoors, 0, 1, 0, 0);
    return 1;
}
