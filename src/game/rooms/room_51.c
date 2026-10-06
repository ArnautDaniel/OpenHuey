/* Room 0x51: its event handler class (vtable Room51_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room51_vtable[];
extern u8 Room51_Phase5Script_data[];

extern u8 Room51_EnterScript_data[];
extern u8 Room51_CharEnterScript_data[];
extern u8 Room51_Phase1Script_data[];
extern u8 Room51_Phase2Script_data[];
extern u32 Room51_ActionScripts[];
extern u8 Room51_Table38_data[];
extern u32 Room51_ObjectNames[];

extern PTMF Room51_CmdTable[];

/* 0x002B4840 */
void *Room51_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room51_vtable, RoomBase_vtable); }

/* 0x002B48A0 */
void *Room51_EnterScript(void) {
    return Room51_EnterScript_data;
}

/* 0x002B48B0 */
void *Room51_CharEnterScript(void) {
    return Room51_CharEnterScript_data;
}

/* 0x002B48C0 */
void *Room51_Phase1Script(void) {
    return Room51_Phase1Script_data;
}

/* 0x002B48D0 */
void *Room51_Phase2Script(void) {
    return Room51_Phase2Script_data;
}

/* 0x002B48E0 */
void *Room51_Phase5Script(void *o) { return Room51_Phase5Script_data; }   /* Room51_vtable +0x20 */

/* 0x002B48F0 */
u32 Room51_ActionScript(void *self, s32 i) {
    return Room51_ActionScripts[i];
}

/* 0x002B4910 */
void *Room51_Table38(void) {
    return Room51_Table38_data;
}

/* 0x002B4920 */
u32 Room51_ObjectName(void *self, s32 i) {
    return Room51_ObjectNames[i];
}

/* (self->*Room51_CmdTable[i])(a, b) */
/* 0x002B4940 */
s32 Room51_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room51_CmdTable[i & 0xFF], a, b);
}

/* 0x002B4970 */
s32 Room51_Cmd01(void) {   /* progress flag 0x650 */
    return item238_sound(0x10000);
}

/* room 0x51 (Room51_Cmd00_ptmf): byte 3 0 door 0 set going (+0xC); else wait (2) while it moves */
/* 0x002B49F0 */
s32 Room51_Cmd00(void *self, void *a1, u8 *cmd) {
    if (cmd[3] != 0) {
        return VCALL(gDoors, 0x30, s32 (*)(VObject *, s32))(gDoors, 0) != 0 ? 1 : 2;
    }
    VCALL(gDoors, 0xC, void (*)(VObject *, s32, s32, s32, s32))(gDoors, 0, 1, 0, 0);
    return 1;
}
