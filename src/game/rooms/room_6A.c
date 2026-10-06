/* Room 0x6A: its event handler class (vtable Room6A_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room6A_vtable[];

extern u32 D_00438730[];
extern u32 D_004387A0[];
extern u32 D_004388C0[];
extern u32 D_004389F0[];
extern u32 D_00438A70[];
extern u32 D_00438CC0[];
extern u32 D_00438CF8[];
extern u32 D_00438D10[];

extern PTMF D_019918F0[];

/* 0x00344050 */
void *Room6A_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room6A_vtable, RoomBase_vtable); }

/* 0x003440B0 */
void *Room6A_EnterScript(void) {
    return D_00438730;
}

/* 0x003440C0 */
void *Room6A_CharEnterScript(void) {
    return D_004387A0;
}

/* 0x003440D0 */
void *Room6A_Phase1Script(void) {
    return D_004388C0;
}

/* 0x003440E0 */
void *Room6A_Phase2Script(void) {
    return D_004389F0;
}

/* 0x003440F0 */
void *Room6A_Phase5Script(void) {
    return D_00438A70;
}

/* 0x00344100 */
u32 Room6A_ActionScript(void *self, s32 i) {
    return D_00438CC0[i];
}

/* 0x00344120 */
void *Room6A_Table38(void) {
    return D_00438D10;
}

/* 0x00344130 */
u32 Room6A_ObjectName(void *self, s32 i) {
    return D_00438CF8[i];
}

/* (self->*D_019918F0[i])(a, b) */
/* 0x00344150 */
s32 Room6A_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019918F0[i & 0xFF], a, b);
}

/* room 0x6A (D_00438CF0): its object's animation (+0x74 forward, +0x78 back) at a point +0x7C
 * (0..1) by byte 3 - 0 / 1 from event variable 0 (12..30 over 18, 12..28 over 16), 2 / 3 at the
 * start / end */
/* 0x00344180 */
s32 Room6A_Cmd01(void *self, void *a1, u8 *cmd) {
    u8 *o = VCALL(gRoomObjects, 0x18, u8 *(*)(VObject *, const char *))(gRoomObjects, D_00438D00);
    u32 n;

    if (o == NULL) {
        return 1;
    }
    n = VCALL(gEvents, 0x34, u32 (*)(VObject *, s32))(gEvents, 0);
    switch (cmd[3]) {
    case 0:
        if (n < 0xC) {
            n = 0xC;
        }
        if (!(n < 0x1F)) {
            n = 0x1E;
        }
        AT(o, 0x74, s32) = 1;
        AT(o, 0x78, s32) = 0;
        AT(o, 0x7C, f32) = (f32)(n - 0xC) / 18.0f;
        break;
    case 1:
        if (n < 0xC) {
            n = 0xC;
        }
        if (!(n < 0x1D)) {
            n = 0x1C;
        }
        AT(o, 0x74, s32) = 0;
        AT(o, 0x78, s32) = 1;
        AT(o, 0x7C, f32) = (f32)(n - 0xC) / 16.0f;
        break;
    case 2:
    case 3:
        AT(o, 0x74, s32) = 0;
        AT(o, 0x78, s32) = 1;
        AT(o, 0x7C, f32) = (f32)(cmd[3] - 2);
        break;
    }
    if (!(AT(o, 0x7C, f32) <= 1.0f)) {
        AT(o, 0x7C, f32) = 1.0f;
    }
    if (AT(o, 0x7C, f32) < 0.0f) {
        AT(o, 0x7C, f32) = 0.0f;
    }
    return 1;
}

/* room 0x6A (D_00438CE0): a lit quad at x 120, z 47 .. 39, from 4 to 21 */
/* 0x00344390 */
s32 Room6A_Cmd00(void *self, void *a1, u8 *cmd) {
    static const u32 sQuad[16] = {
        0x42F00000, 0x41A80000, 0x423C0000, 0x3F800000, 0x42F00000, 0x41A80000, 0x421C0000, 0x3F800000,
        0x42F00000, 0x40800000, 0x423C0000, 0x3F800000, 0x42F00000, 0x40800000, 0x421C0000, 0x3F800000,
    };

    return lit_quad(cmd, sQuad, 0x80);
}
