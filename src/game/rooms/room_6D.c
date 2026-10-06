/* Room 0x6D: its event handler class (vtable Room6D_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room6D_vtable[];

extern u32 Room6D_EnterScript_data[];
extern u32 Room6D_CharEnterScript_data[];
extern u32 Room6D_Phase1Script_data[];
extern u32 Room6D_Phase2Script_data[];
extern u32 Room6D_Phase3Script_data[];
extern u32 Room6D_ObjectNames[];
extern u32 Room6D_Table38_data[];
extern u32 Room6D_ActionScripts[];

extern PTMF Room6D_CondTable[];

/* 0x00344B90 */
void *Room6D_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room6D_vtable, RoomBase_vtable); }

/* 0x00344BF0 */
void *Room6D_EnterScript(void) {
    return Room6D_EnterScript_data;
}

/* 0x00344C00 */
void *Room6D_CharEnterScript(void) {
    return Room6D_CharEnterScript_data;
}

/* 0x00344C10 */
void *Room6D_Phase1Script(void) {
    return Room6D_Phase1Script_data;
}

/* 0x00344C20 */
void *Room6D_Phase2Script(void) {
    return Room6D_Phase2Script_data;
}

/* 0x00344C30 */
void *Room6D_Phase3Script(void) {
    return Room6D_Phase3Script_data;
}

/* 0x00344C40 */
u32 Room6D_ActionScript(void *self, s32 i) {
    return Room6D_ActionScripts[i];
}

/* 0x00344C60 */
void *Room6D_Table38(void) {
    return Room6D_Table38_data;
}

/* 0x00344C70 */
u32 Room6D_ObjectName(void *self, s32 i) {
    return Room6D_ObjectNames[i];
}

/* (self->*Room6D_CondTable[i])(a, b) */
/* 0x00344C90 */
s32 Room6D_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room6D_CondTable[i & 0xFF], a, b);
}

/* 0x00344CC0 */
s32 Room6D_Cond01(void) {
    u8 *p = (u8 *)gCharPursuer;

    if (p == NULL || p[0x28] == 0) {
        return 0;
    }
    return *(s32 *)(p + 0xE8) == 0;
}

/* 0x00344D10 */
s32 Room6D_Cond00(void) {
    u8 *p = (u8 *)gCharPursuer;

    if (p == NULL || p[0x28] == 0 || *(s32 *)(p + 0xE8) == 0) {
        return 0;
    }
    return ((u8 *)gProgress)[0x1130] != 0xFE;
}
