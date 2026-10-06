/* Room 0x10: its event handler class (vtable Room10_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room10_vtable[];
extern u8 Room10_ObjectNames[];

extern u8 Room10_EnterScript_data[];
extern u8 Room10_CharEnterScript_data[];
extern u8 Room10_Phase1Script_data[];
extern u8 Room10_Phase2Script_data[];
extern u8 Room10_Phase3Script_data[];
extern u8 Room10_Phase5Script_data[];
extern void *Room10_ActionScripts[];
extern u8 Room10_Table38_data[];
extern PTMF Room10_CondTable[];

/* 0x002ABF00 */
void *Room10_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room10_vtable, RoomBase_vtable); }

/* 0x002ABF60 */
void *Room10_EnterScript(void) {
    return Room10_EnterScript_data;
}

/* 0x002ABF70 */
void *Room10_CharEnterScript(void) {
    return Room10_CharEnterScript_data;
}

/* 0x002ABF80 */
void *Room10_Phase1Script(void) {
    return Room10_Phase1Script_data;
}

/* 0x002ABF90 */
void *Room10_Phase2Script(void) {
    return Room10_Phase2Script_data;
}

/* 0x002ABFA0 */
void *Room10_Phase3Script(void) {
    return Room10_Phase3Script_data;
}

/* 0x002ABFB0 */
void *Room10_Phase5Script(void) {
    return Room10_Phase5Script_data;
}

/* 0x002ABFC0 */
void *Room10_ActionScript(void *self, s32 i) {
    return Room10_ActionScripts[i];
}

/* 0x002ABFE0 */
void *Room10_Table38(void) {
    return Room10_Table38_data;
}

/* 0x002ABFF0 */
u32 Room10_ObjectName(void *o, s32 i) { return ((u32 *)Room10_ObjectNames)[i]; }   /* Room10_vtable +0x34 */

/* (self->*Room10_CondTable[i])(a, b) */
/* 0x002AC010 */
s32 Room10_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room10_CondTable[i & 0xFF], a, b);
}

/* room 0x10 (Room10_Cond00_ptmf): the stalker is there, not about, in mode 2, 6 or 7 */
/* 0x002AC040 */
s32 Room10_Cond00(void) {
    u8 *c = (u8 *)gCharSlot2;
    u8 k;

    if (c == NULL || AT(c, 0x28, u8) != 0) {
        return 0;
    }
    k = AT(c, 0x153C, u8);
    return k == 2 || k == 6 || k == 7;
}
