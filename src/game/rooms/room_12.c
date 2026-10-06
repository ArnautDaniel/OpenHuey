/* Room 0x12: its event handler class (vtable Room12_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "scene_game.h"

extern void *RoomBase_vtable[];
extern void *Room12_vtable[];
extern void *SmokePuffs_vtable[];
extern u8 Room12_EnterScript_data[];
extern u8 Room12_CharEnterScript_data[];
extern u8 Room12_Phase1Script_data[];
extern u8 Room12_Phase2Script_data[];
extern u8 Room12_Phase3Script_data[];
extern u8 Room12_Phase5Script_data[];
extern void *Room12_ActionScripts[];
extern void *Room12_ObjectNames[];
extern u8 Room12_Table38_data[];
extern PTMF Room12_CmdTable[];
extern PTMF Room12_CondTable[];

static void smoke_puffs_init(void **obj) {
    obj[0] = SmokePuffs_vtable;
    obj[0x1810 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0x1814 / 4] = -1;
    obj[0x1810 / 4] = QuadDrawer_vtable;
}

/* 0x002AC190 */
void *Room12_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room12_vtable, RoomBase_vtable); }

/* 0x002AC1F0 */
void *Room12_EnterScript(void) {
    return Room12_EnterScript_data;
}

/* 0x002AC200 */
void *Room12_CharEnterScript(void) {
    return Room12_CharEnterScript_data;
}

/* 0x002AC210 */
void *Room12_Phase1Script(void) {
    return Room12_Phase1Script_data;
}

/* 0x002AC220 */
void *Room12_Phase2Script(void) {
    return Room12_Phase2Script_data;
}

/* 0x002AC230 */
void *Room12_Phase3Script(void) {
    return Room12_Phase3Script_data;
}

/* 0x002AC240 */
void *Room12_Phase5Script(void) {
    return Room12_Phase5Script_data;
}

/* 0x002AC250 */
void *Room12_ActionScript(void *self, s32 i) {
    return Room12_ActionScripts[i];
}

/* 0x002AC270 */
void *Room12_Table38(void) {
    return Room12_Table38_data;
}

/* 0x002AC280 */
void *Room12_ObjectName(void *self, s32 i) {
    return Room12_ObjectNames[i];
}

/* (self->*Room12_CondTable[i])(a, b) */
/* 0x002AC2A0 */
s32 Room12_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room12_CondTable[i & 0xFF], a, b);
}

/* room 0x12 (Room12_Cond00_ptmf): the pursuer is about, in a mode other than 0 and 3 */
/* 0x002AC2D0 */
s32 Room12_Cond00(void) {
    Character *s = gCharPursuer;

    return s != NULL && s->a.active != 0 && AT(s, 0xE8, s32) != 3 && AT(s, 0xE8, s32) != 0;
}

/* (self->*Room12_CmdTable[i])(a, b) */
/* 0x002AC340 */
s32 Room12_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room12_CmdTable[i & 0xFF], a, b);
}

/* room 0x12 (Room12_Cmd00_ptmf): byte 3 0: the smoke puffs (SmokePuffs_vtable), their slot in script
 * variable 0; else that slot started */
/* 0x002AC370 */
s32 Room12_Cmd00(void *self, void *a1, u8 *cmd) {
    if (cmd[3] == 0) {
        s32 slot = Effect_New(gEffects, 0x1C60, smoke_puffs_init);

        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 0, slot);
    } else {
        EffectMgr_Start(gEffects,
                      VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 0), NULL);
    }
    return 1;
}
