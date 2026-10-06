/* Room 0x10B: its event handler class (vtable Room10B_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "scene_game.h"

extern void *RoomBase_vtable[];
extern void *Room10B_vtable[];
extern void *AshFlakes_vtable[];
extern u8 Room10B_EnterScript_data[];
extern u8 Room10B_CharEnterScript_data[];
extern u8 Room10B_Phase1Script_data[];
extern u8 Room10B_Phase3Script_data[];
extern void *Room10B_ActionScripts[];
extern u8 Room10B_Table38_data[];
extern void *Room10B_ObjectNames[];

extern PTMF Room10B_CmdTable[];
extern PTMF Room10B_CondTable[];

static void effect_4480_init(void **obj) {
    obj[0] = AshFlakes_vtable;
    obj[0x3010 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0x3014 / 4] = -1;
    obj[0x3010 / 4] = QuadDrawer_vtable;
}

/* 0x002E78E0 */
void *Room10B_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room10B_vtable, RoomBase_vtable); }

/* 0x002E7940 */
void *Room10B_EnterScript(void) { return Room10B_EnterScript_data; }

/* 0x002E7950 */
void *Room10B_CharEnterScript(void) { return Room10B_CharEnterScript_data; }

/* 0x002E7960 */
void *Room10B_Phase1Script(void) { return Room10B_Phase1Script_data; }

/* 0x002E7970 */
void *Room10B_Phase3Script(void) { return Room10B_Phase3Script_data; }

/* 0x002E7980 */
void *Room10B_ActionScript(void *self, s32 i) { return Room10B_ActionScripts[i]; }

/* 0x002E79A0 */
void *Room10B_Table38(void) { return Room10B_Table38_data; }

/* 0x002E79B0 */
void *Room10B_ObjectName(void *self, s32 i) { return Room10B_ObjectNames[i]; }

/* (self->*Room10B_CondTable[i])(a, b) */
/* 0x002E79D0 */
s32 Room10B_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room10B_CondTable[i & 0xFF], a, b);
}

/* 0x002E7A00 */
s32 Room10B_Cond00(void) {
    return AT(gProgress, 0xFB6, s16) >= 100;
}

/* (self->*Room10B_CmdTable[i])(a, b) */
/* 0x002E7A20 */
s32 Room10B_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room10B_CmdTable[i & 0xFF], a, b);
}

/* byte 3 0: a 0x4480 effect is spawned and its slot kept in event var 0; else that slot's
 * effect is removed */
/* 0x002E7A50 */
s32 Room10B_Cmd00(void *self, void *a1, u8 *cmd) {
    if (cmd[3] == 0) {
        s32 slot = Effect_New(gEffects, 0x4480, effect_4480_init);

        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 0, slot);
    } else {
        EffectMgr_Remove(gEffects, VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 0));
    }
    return 1;
}
