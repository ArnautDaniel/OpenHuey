/* Room 0x37: its event handler class (vtable Room37_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room37_vtable[];
extern u8 Room37_ObjectNames[];
extern const char *D_0047B02C;   /* "fan" (room 0x37) */

extern u8 Room37_EnterScript_data[];
extern u8 Room37_CharEnterScript_data[];
extern u8 Room37_Phase1Script_data[];
extern u8 Room37_Phase2Script_data[];
extern u8 Room37_Phase3Script_data[];
extern void *Room37_ActionScripts[];
extern PTMF Room37_CmdTable[];

/* 0x0036A300 */
void *Room37_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room37_vtable, RoomBase_vtable); }

/* 0x0036A360 */
void *Room37_EnterScript(void) {
    return Room37_EnterScript_data;
}

/* 0x0036A370 */
void *Room37_CharEnterScript(void) {
    return Room37_CharEnterScript_data;
}

/* 0x0036A380 */
void *Room37_Phase1Script(void) {
    return Room37_Phase1Script_data;
}

/* 0x0036A390 */
void *Room37_Phase2Script(void) {
    return Room37_Phase2Script_data;
}

/* 0x0036A3A0 */
void *Room37_Phase3Script(void) {
    return Room37_Phase3Script_data;
}

/* 0x0036A3B0 */
void *Room37_ActionScript(void *self, s32 i) {
    return Room37_ActionScripts[i];
}

/* (self->*Room37_CmdTable[i])(a, b) */
/* 0x0036A3D0 */
s32 Room37_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room37_CmdTable[i & 0xFF], a, b);
}

/* room 0x37 (D_004469A0): the fan turns, except while a movie plays */
/* 0x0036A400 */
s32 Room37_Fan(void) {
    if (VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress) != 0) {
        return 1;
    }
    fan_turn(D_0047B02C);
    return 1;
}

/* 0x0036A4B0 */
u32 Room37_ObjectName(void *o, s32 i) { return ((u32 *)Room37_ObjectNames)[i]; }   /* Room37_vtable +0x34 */
