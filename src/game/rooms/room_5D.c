/* Room 0x5D: its event handler class (vtable Room5D_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room5D_vtable[];

extern u8 Room5D_EnterScript_data[];
extern u8 Room5D_CharEnterScript_data[];
extern u8 Room5D_Phase1Script_data[];
extern u8 Room5D_Phase2Script_data[];
extern u32 Room5D_ActionScripts[];
extern u8 Room5D_Table38_data[];

extern PTMF Room5D_CmdTable[];

/* 0x002B5E80 */
void *Room5D_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room5D_vtable, RoomBase_vtable); }

/* 0x002B5EE0 */
void *Room5D_EnterScript(void) {
    return Room5D_EnterScript_data;
}

/* 0x002B5EF0 */
void *Room5D_CharEnterScript(void) {
    return Room5D_CharEnterScript_data;
}

/* 0x002B5F00 */
void *Room5D_Phase1Script(void) {
    return Room5D_Phase1Script_data;
}

/* 0x002B5F10 */
void *Room5D_Phase2Script(void) {
    return Room5D_Phase2Script_data;
}

/* 0x002B5F20 */
u32 Room5D_ActionScript(void *self, s32 i) {
    return Room5D_ActionScripts[i];
}

/* 0x002B5F40 */
void *Room5D_Table38(void) {
    return Room5D_Table38_data;
}

/* 0x002B5F50 */
u32 Room5D_ObjectName(void *self, s32 i) {
    return (u32)Room5D_ObjectNames[i];
}

/* (self->*Room5D_CmdTable[i])(a, b) */
/* 0x002B5F70 */
s32 Room5D_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room5D_CmdTable[i & 0xFF], a, b);
}

/* room 0x5D (Room5D_Cmd01_ptmf): the lever at -60 / 0 / 60 degrees by byte 3 */
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

/* room 0x5D (Room5D_Cmd00_ptmf): four objects 60 to the left */
/* 0x002B6040 */
s32 Room5D_Cmd00(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        u8 *o = room_obj(Room5D_ObjectNames[i + 2]);

        if (o != NULL) {
            AT(o, 0x20, f32) = AT(o, 0x20, f32) - 60.0f;
        }
    }
    return 1;
}
