/* Room 0x18: its event handler class (vtable Room18_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "scene_game.h"
#include "room.h"

extern void *RoomBase_vtable[];
extern void *Room18_vtable[];
extern u8 Room18_Phase5Script_data[];

extern u8 Room18_EnterScript_data[];
extern u8 Room18_CharEnterScript_data[];
extern u8 Room18_Phase1Script_data[];
extern u8 Room18_Phase2Script_data[];
extern u8 Room18_Phase3Script_data[];
extern void *Room18_ActionScripts[];
extern u8 Room18_Table38_data[];
extern PTMF Room18_CmdTable[];

/* 0x002AD020 */
void *Room18_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room18_vtable, RoomBase_vtable); }

/* 0x002AD080 */
void *Room18_EnterScript(void) {
    return Room18_EnterScript_data;
}

/* 0x002AD090 */
void *Room18_CharEnterScript(void) {
    return Room18_CharEnterScript_data;
}

/* 0x002AD0A0 */
void *Room18_Phase1Script(void) {
    return Room18_Phase1Script_data;
}

/* 0x002AD0B0 */
void *Room18_Phase2Script(void) {
    return Room18_Phase2Script_data;
}

/* 0x002AD0C0 */
void *Room18_Phase3Script(void) {
    return Room18_Phase3Script_data;
}

/* 0x002AD0D0 */
void *Room18_Phase5Script(void *o) { return Room18_Phase5Script_data; }   /* Room18_vtable +0x20 */

/* 0x002AD0E0 */
void *Room18_ActionScript(void *self, s32 i) {
    return Room18_ActionScripts[i];
}

/* 0x002AD100 */
void *Room18_Table38(void) {
    return Room18_Table38_data;
}

/* 0x002AD110 */
void *Room18_ObjectName(void *self, s32 i) {
    return (void *)Room18_ObjectNames[i];
}

/* (self->*Room18_CmdTable[i])(a, b) */
/* 0x002AD130 */
s32 Room18_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room18_CmdTable[i & 0xFF], a, b);
}

/* room 0x18 (Room18_Cmd00_ptmf): object byte 3's PlacedObject_ToDef */
/* 0x002AD160 */
s32 Room18_Cmd00(void *self, void *a1, u8 *cmd) {
    u8 *o = room_obj(Room18_ObjectNames[cmd[3]]);

    if (o != NULL) {
        PlacedObject_ToDef(o);
    }
    return 1;
}
