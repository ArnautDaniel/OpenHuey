/* Room 0x10A: its event handler class (vtable Room10A_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room10A_vtable[];
extern u8 Room10A_Phase5Script_data[];
extern s32 D_0047B27C;

extern u8 Room10A_EnterScript_data[];
extern u8 Room10A_CharEnterScript_data[];
extern u8 Room10A_Phase1Script_data[];
extern u8 Room10A_Phase2Script_data[];
extern void *Room10A_ActionScripts[];
extern u8 Room10A_Table38_data[];

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

extern PTMF Room10A_CondTable[];
extern PTMF Room10A_CmdTable[];

/* 0x002E76A0 */
void *Room10A_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room10A_vtable, RoomBase_vtable); }

/* 0x002E7700 */
void *Room10A_EnterScript(void) { return Room10A_EnterScript_data; }

/* 0x002E7710 */
void *Room10A_CharEnterScript(void) { return Room10A_CharEnterScript_data; }

/* 0x002E7720 */
void *Room10A_Phase1Script(void) { return Room10A_Phase1Script_data; }

/* 0x002E7730 */
void *Room10A_Phase2Script(void) { return Room10A_Phase2Script_data; }

/* 0x002E7740 */
void *Room10A_Phase5Script(void *o) { return Room10A_Phase5Script_data; }   /* Room10A_vtable +0x20 */

/* 0x002E7750 */
void *Room10A_ActionScript(void *self, s32 i) { return Room10A_ActionScripts[i]; }

/* 0x002E7770 */
void *Room10A_Table38(void) { return Room10A_Table38_data; }

/* (self->*Room10A_CmdTable[i])(a, b) */
/* 0x002E7780 */
s32 Room10A_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room10A_CmdTable[i & 0xFF], a, b);
}

/* 0x002E77B0 */
s32 Room10A_Cmd00(void *self, void *a1, u8 *cmd) { return room_nudge(&D_0047B27C, cmd, 1.0f); }

/* (self->*Room10A_CondTable[i])(a, b) */
/* 0x002E7850 */
s32 Room10A_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room10A_CondTable[i & 0xFF], a, b);
}

/* 0x002E7880 */
s32 Room10A_Cond00(void) {
    u8 *e = (u8 *)gProgress + 0x10D4;
    s32 i;

    for (i = 0; i < 4; i++, e += 16) {
        if (F(e, 0x4, s32) == 0x10A && e[0] >= 0x20) {
            return 1;
        }
    }
    return 0;
}
