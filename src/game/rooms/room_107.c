/* Room 0x107: its event handler class (vtable Room107_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room107_vtable[];
extern u8 D_0047AC78[];
extern s32 D_0047B274;

extern u8 D_00418AF0[];
extern u8 D_00418B60[];
extern u8 D_00418BA0[];
extern u8 D_00418BF0[];
extern void *D_00418D70[];
extern u8 D_00418DB0[];

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

extern PTMF D_01990EE8[];
extern PTMF D_01990F00[];

/* 0x002E6F10 */
void *Room107_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room107_vtable, RoomBase_vtable); }

/* 0x002E6F70 */
void *Room107_EnterScript(void) { return D_00418AF0; }

/* 0x002E6F80 */
void *Room107_CharEnterScript(void) { return D_00418B60; }

/* 0x002E6F90 */
void *Room107_Phase1Script(void) { return D_00418BA0; }

/* 0x002E6FA0 */
void *Room107_Phase2Script(void) { return D_00418BF0; }

/* 0x002E6FB0 */
void *Room107_Phase5Script(void *o) { return D_0047AC78; }   /* Room107_vtable +0x20 */

/* 0x002E6FC0 */
void *Room107_ActionScript(void *self, s32 i) { return D_00418D70[i]; }

/* 0x002E6FE0 */
void *Room107_Table38(void) { return D_00418DB0; }

/* (self->*D_01990F00[i])(a, b) */
/* 0x002E6FF0 */
s32 Room107_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990F00[i & 0xFF], a, b);
}

/* 0x002E7020 */
s32 Room107_Cmd01(void) {
    Effect_New(gEffects, 0x10, effect_10_init);
    return 1;
}

/* 0x002E70F0 */
s32 Room107_Cmd00(void *self, void *a1, u8 *cmd) { return room_nudge(&D_0047B274, cmd, 2.0f); }

/* (self->*D_01990EE8[i])(a, b) */
/* 0x002E7190 */
s32 Room107_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990EE8[i & 0xFF], a, b);
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
