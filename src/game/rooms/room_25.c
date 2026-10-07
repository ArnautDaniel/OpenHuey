/* Room 0x25: its event handler class (vtable Room25_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room25_vtable[];
extern void *RisingMotes_vtable[];   /* the rising motes */
extern u8 Room25_EnterScript_data[];
extern u8 Room25_CharEnterScript_data[];
extern u8 Room25_Phase1Script_data[];
extern u8 Room25_Phase2Script_data[];
extern u8 Room25_Phase3Script_data[];
extern u8 Room25_Phase5Script_data[];
extern u32 Room25_ActionScripts[];
extern u8 Room25_Table38_data[];
extern u32 Room25_ObjectNames[];

extern PTMF Room25_CmdTable[];
extern PTMF Room25_CondTable[];

static void motes_init(void **obj) {
    obj[0] = RisingMotes_vtable;
    obj[0x3010 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0x3014 / 4] = -1;
    obj[0x3010 / 4] = QuadDrawer_vtable;
}

/* 0x002B00F0 */
void *Room25_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room25_vtable, RoomBase_vtable); }

/* 0x002B0150 */
void *Room25_EnterScript(void) {
    return Room25_EnterScript_data;
}

/* 0x002B0160 */
void *Room25_CharEnterScript(void) {
    return Room25_CharEnterScript_data;
}

/* 0x002B0170 */
void *Room25_Phase1Script(void) {
    return Room25_Phase1Script_data;
}

/* 0x002B0180 */
void *Room25_Phase2Script(void) {
    return Room25_Phase2Script_data;
}

/* 0x002B0190 */
void *Room25_Phase3Script(void) {
    return Room25_Phase3Script_data;
}

/* 0x002B01A0 */
void *Room25_Phase5Script(void) {
    return Room25_Phase5Script_data;
}

/* 0x002B01B0 */
u32 Room25_ActionScript(void *self, s32 i) {
    return Room25_ActionScripts[i];
}

/* 0x002B01D0 */
void *Room25_Table38(void) {
    return Room25_Table38_data;
}

/* 0x002B01E0 */
u32 Room25_ObjectName(void *self, s32 i) {
    return Room25_ObjectNames[i];
}

/* (self->*Room25_CondTable[i])(a, b) */
/* 0x002B0200 */
s32 Room25_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room25_CondTable[i & 0xFF], a, b);
}

/* room 0x25: Fiona is active and in a reaction (action 4) with no character behind it (+0x100
 * 0xFF). */
/* 0x002B0230 */
s32 Room25_Cond00(void) {
    u8 *obj = (u8 *)gCharPlayer;

    if (obj == NULL || ((u8 *)gCharPlayer)[0x28] != 1 || *(s32 *)((u8 *)gCharPlayer + 0xF8) != 4 ||
        *(s32 *)((u8 *)gCharPlayer + 0x100) != 0xFF) {
        return 0;
    }
    return 1;
}

/* (self->*Room25_CmdTable[i])(a, b) */
/* 0x002B02A0 */
s32 Room25_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room25_CmdTable[i & 0xFF], a, b);
}

/* the rising motes started */
/* 0x002B02D0 */
s32 Room25_Cmd00(void) {
    Effect_New(gEffects, 0x3860, motes_init);
    return 1;
}
