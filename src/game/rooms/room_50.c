/* Room 0x50: its event handler class (vtable Room50_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room50_vtable[];

extern u8 Room50_EnterScript_data[];
extern u8 Room50_CharEnterScript_data[];
extern u8 Room50_Phase1Script_data[];
extern u8 Room50_Phase2Script_data[];
extern u8 Room50_Phase5Script_data[];
extern u32 Room50_ActionScripts[];
extern u8 Room50_Table38_data[];
extern u32 Room50_ObjectNames[];

extern PTMF Room50_CmdTable[];

/* 0x002B4690 */
void *Room50_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room50_vtable, RoomBase_vtable); }

/* 0x002B46F0 */
void *Room50_EnterScript(void) {
    return Room50_EnterScript_data;
}

/* 0x002B4700 */
void *Room50_CharEnterScript(void) {
    return Room50_CharEnterScript_data;
}

/* 0x002B4710 */
void *Room50_Phase1Script(void) {
    return Room50_Phase1Script_data;
}

/* 0x002B4720 */
void *Room50_Phase2Script(void) {
    return Room50_Phase2Script_data;
}

/* 0x002B4730 */
void *Room50_Phase5Script(void) {
    return Room50_Phase5Script_data;
}

/* 0x002B4740 */
u32 Room50_ActionScript(void *self, s32 i) {
    return Room50_ActionScripts[i];
}

/* 0x002B4760 */
void *Room50_Table38(void) {
    return Room50_Table38_data;
}

/* 0x002B4770 */
u32 Room50_ObjectName(void *self, s32 i) {
    return Room50_ObjectNames[i];
}

/* (self->*Room50_CmdTable[i])(a, b) */
/* 0x002B4790 */
s32 Room50_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room50_CmdTable[i & 0xFF], a, b);
}

/* 0x002B47C0 */
s32 Room50_Cmd00(void) {   /* progress flag 0x651 */
    return item238_sound(0x20000);
}
