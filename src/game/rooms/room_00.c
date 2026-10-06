/* Room 0x00: its event handler class (vtable Room00_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room00_vtable[];

extern u8 Room00_EnterScript_data[];
extern u8 Room00_CharEnterScript_data[];
extern u8 Room00_Phase1Script_data[];
extern u8 Room00_Phase2Script_data[];
extern u8 Room00_Phase3Script_data[];
extern void *Room00_ActionScripts[];
extern void *Room00_ObjectNames[];
extern u8 Room00_Table38_data[];
extern PTMF Room00_CondTable[];

/* 0x002A8980 */
void *Room00_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room00_vtable, RoomBase_vtable); }

/* 0x002A89E0 */
void *Room00_EnterScript(void) {
    return Room00_EnterScript_data;
}

/* 0x002A89F0 */
void *Room00_CharEnterScript(void) {
    return Room00_CharEnterScript_data;
}

/* 0x002A8A00 */
void *Room00_Phase1Script(void) {
    return Room00_Phase1Script_data;
}

/* 0x002A8A10 */
void *Room00_Phase2Script(void) {
    return Room00_Phase2Script_data;
}

/* 0x002A8A20 */
void *Room00_Phase3Script(void) {
    return Room00_Phase3Script_data;
}

/* 0x002A8A30 */
void *Room00_ActionScript(void *self, s32 i) {
    return Room00_ActionScripts[i];
}

/* 0x002A8A50 */
void *Room00_Table38(void) {
    return Room00_Table38_data;
}

/* 0x002A8A60 */
void *Room00_ObjectName(void *self, s32 i) {
    return Room00_ObjectNames[i];
}

/* (self->*Room00_CondTable[i])(a, b) */
/* 0x002A8A80 */
s32 Room00_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room00_CondTable[i & 0xFF], a, b);
}

/* Fiona's model +0xD0 (0, 1.5, -2.5) and +0xCC(1) when byte 3 is 0, else (0, 1.5, -1.5) and
 * +0xCC(0) */
/* 0x002A8AF0 */
s32 Room00_Cmd01(void *self, void *a1, u8 *cmd) {
    void *m = gCharPlayer->motion;

    if (cmd[3] == 0) {
        VCALL(m, 0xD0, void (*)(void *, f32, f32, f32))(m, 0.0f, 1.5f, -2.5f);
        VCALL(m, 0xCC, void (*)(void *, s32))(m, 1);
    } else {
        VCALL(m, 0xD0, void (*)(void *, f32, f32, f32))(m, 0.0f, 1.5f, -1.5f);
        VCALL(m, 0xCC, void (*)(void *, s32))(m, 0);
    }
    return 1;
}

/* 0x002A8BA0 */
s32 Room00_Cmd00(void *self, void *a1, u8 *cmd) {
    return glow4_spot(cmd, 0);
}
