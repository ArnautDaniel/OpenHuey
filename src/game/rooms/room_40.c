/* Room 0x40: its event handler class (vtable Room40_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room40_vtable[];

extern u8 D_00406550[];
extern u8 D_00406630[];
extern u8 D_00406770[];
extern u8 D_00406930[];
extern u32 D_00407050[];
extern u8 D_004070D0[];
extern u32 D_00407090[];

extern PTMF D_01990C48[];

/* 0x002B2330 */
void *Room40_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room40_vtable, RoomBase_vtable); }

/* 0x002B2390 */
void *Room40_EnterScript(void) {
    return D_00406550;
}

/* 0x002B23A0 */
void *Room40_CharEnterScript(void) {
    return D_00406630;
}

/* 0x002B23B0 */
void *Room40_Phase1Script(void) {
    return D_00406770;
}

/* 0x002B23C0 */
void *Room40_Phase2Script(void) {
    return D_00406930;
}

/* 0x002B23D0 */
u32 Room40_ActionScript(void *self, s32 i) {
    return D_00407050[i];
}

/* 0x002B23F0 */
void *Room40_Table38(void) {
    return D_004070D0;
}

/* 0x002B2400 */
u32 Room40_ObjectName(void *self, s32 i) {
    return D_00407090[i];
}

/* (self->*D_01990C48[i])(a, b) */
/* 0x002B2420 */
s32 Room40_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990C48[i & 0xFF], a, b);
}

/* the scales ("tenbin") and their pans ("sara_l", "sara_r"): level (byte 3 0) or tipped */
/* 0x002B2450 */
s32 Room40_Cmd00(void *self, void *a1, u8 *cmd) {
    VObject *objs = gRoomObjects;
    u8 *o;

    o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_004070C0);
    if (o != NULL) {
        AT(o, 0x10, u32) = 0;
        AT(o, 0x14, u32) = 0;
        AT(o, 0x18, u32) = 0;
        if (cmd[3] == 0) {
            AT(o, 0x20, u32) = 0x40C669AD;
            AT(o, 0x24, u32) = 0x419E6666;
        } else {
            AT(o, 0x20, u32) = 0x40CB367A;
            AT(o, 0x24, u32) = 0x41A4CCCD;
        }
        AT(o, 0x28, u32) = 0x4188CCCD;
    }
    o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_004070C4);
    if (o != NULL) {
        AT(o, 0x10, u32) = 0;
        AT(o, 0x14, u32) = 0;
        AT(o, 0x18, u32) = 0;
        if (cmd[3] == 0) {
            AT(o, 0x20, u32) = 0x404322D1;
            AT(o, 0x24, u32) = 0x419E6666;
        } else {
            AT(o, 0x20, u32) = 0x40660419;
            AT(o, 0x24, u32) = 0x419A0000;
        }
        AT(o, 0x28, u32) = 0x4188CCCD;
    }
    o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_004070C8);
    if (o != NULL) {
        AT(o, 0x10, u32) = 0;
        AT(o, 0x14, u32) = 0;
        AT(o, 0x18, u32) = cmd[3] == 0 ? 0 : 0x3EDF66F3;
        AT(o, 0x20, u32) = 0x4093D14E;
        AT(o, 0x24, u32) = 0x41A4CCCD;
        AT(o, 0x28, u32) = 0x4188A7F0;
    }
    return 1;
}
