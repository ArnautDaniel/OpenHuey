/* Room 0x5D: its event handler class (vtable D_0046EA00, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046EA00[];

extern u8 D_00411B80[];
extern u8 D_00411C60[];
extern u8 D_00411D60[];
extern u8 D_00411FB0[];
extern u32 D_00412390[];
extern u8 D_004123F0[];

extern PTMF D_01990E20[];

/* 0x002B5E80 */
void *Room5D_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046EA00, D_0046DB80); }

/* 0x002B5EE0 */
void *Room5D_EnterScript(void) {
    return D_00411B80;
}

/* 0x002B5EF0 */
void *Room5D_CharEnterScript(void) {
    return D_00411C60;
}

/* 0x002B5F00 */
void *Room5D_Phase1Script(void) {
    return D_00411D60;
}

/* 0x002B5F10 */
void *Room5D_Phase2Script(void) {
    return D_00411FB0;
}

/* 0x002B5F20 */
u32 Room5D_ActionScript(void *self, s32 i) {
    return D_00412390[i];
}

/* 0x002B5F40 */
void *Room5D_Table38(void) {
    return D_004123F0;
}

/* 0x002B5F50 */
u32 Room5D_ObjectName(void *self, s32 i) {
    return (u32)D_004123D0[i];
}

/* (self->*D_01990E20[i])(a, b) */
/* 0x002B5F70 */
s32 Room5D_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990E20[i & 0xFF], a, b);
}

/* room 0x5D (D_004123C0): the lever at -60 / 0 / 60 degrees by byte 3 */
/* 0x002B5FA0 */
s32 Room5D_Cmd01(void *self, void *a1, u8 *cmd) {
    u8 *o = room_obj(D_004123E8);

    if (o != NULL) {
        switch (cmd[3]) {
        case 0:
            AT(o, 0x14, u32) = 0xBF860A92;
            break;
        case 2:
            AT(o, 0x14, u32) = 0x3F860A92;
            break;
        default:
            AT(o, 0x14, s32) = 0;
            break;
        }
    }
    return 1;
}

/* room 0x5D (D_004123B0): four objects 60 to the left */
/* 0x002B6040 */
s32 Room5D_Cmd00(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        u8 *o = room_obj(D_004123D0[i + 2]);

        if (o != NULL) {
            AT(o, 0x20, f32) = AT(o, 0x20, f32) - 60.0f;
        }
    }
    return 1;
}
