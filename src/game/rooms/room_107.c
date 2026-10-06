/* Room 0x107: its event handler class (vtable Room107_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room107_vtable[];
extern u8 Room107_Phase5Script_data[];
extern s32 D_0047B274;

extern u8 Room107_EnterScript_data[];
extern u8 Room107_CharEnterScript_data[];
extern u8 Room107_Phase1Script_data[];
extern u8 Room107_Phase2Script_data[];
extern void *Room107_ActionScripts[];
extern u8 Room107_Table38_data[];

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

extern PTMF Room107_CondTable[];
extern PTMF Room107_CmdTable[];

/* 0x002E6F10 */
void *Room107_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room107_vtable, RoomBase_vtable); }

/* 0x002E6F70 */
void *Room107_EnterScript(void) { return Room107_EnterScript_data; }

/* 0x002E6F80 */
void *Room107_CharEnterScript(void) { return Room107_CharEnterScript_data; }

/* 0x002E6F90 */
void *Room107_Phase1Script(void) { return Room107_Phase1Script_data; }

/* 0x002E6FA0 */
void *Room107_Phase2Script(void) { return Room107_Phase2Script_data; }

/* 0x002E6FB0 */
void *Room107_Phase5Script(void *o) { return Room107_Phase5Script_data; }   /* Room107_vtable +0x20 */

/* 0x002E6FC0 */
void *Room107_ActionScript(void *self, s32 i) { return Room107_ActionScripts[i]; }

/* 0x002E6FE0 */
void *Room107_Table38(void) { return Room107_Table38_data; }

/* (self->*Room107_CmdTable[i])(a, b) */
/* 0x002E6FF0 */
s32 Room107_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room107_CmdTable[i & 0xFF], a, b);
}

/* 0x002E7020 */
s32 Room107_Cmd01(void) {
    Effect_New(gEffects, 0x10, effect_10_init);
    return 1;
}

/* 0x002E70F0 */
s32 Room107_Cmd00(void *self, void *a1, u8 *cmd) { return room_nudge(&D_0047B274, cmd, 2.0f); }

/* (self->*Room107_CondTable[i])(a, b) */
/* 0x002E7190 */
s32 Room107_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room107_CondTable[i & 0xFF], a, b);
}

/* 0x002E71C0 */
s32 Room107_Cond00(void) {
    u8 *e = (u8 *)gProgress + 0x10D4;
    s32 i;

    for (i = 0; i < 4; i++, e += 16) {
        if (F(e, 0x4, s32) == 0x107 && e[0] >= 0x20) {
            return 1;
        }
    }
    return 0;
}
