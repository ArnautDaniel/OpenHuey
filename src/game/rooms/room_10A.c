/* Room 0x10A: its event handler class (vtable Room10A_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room10A_vtable[];
extern u8 D_0047AC80[];
extern s32 D_0047B27C;

extern u8 D_00419730[];
extern u8 D_00419770[];
extern u8 D_00419870[];
extern u8 D_00419950[];
extern void *D_00419A30[];
extern u8 D_00419A60[];

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

extern PTMF D_01990F48[];
extern PTMF D_01990F58[];

/* 0x002E76A0 */
void *Room10A_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room10A_vtable, RoomBase_vtable); }

/* 0x002E7700 */
void *Room10A_EnterScript(void) { return D_00419730; }

/* 0x002E7710 */
void *Room10A_CharEnterScript(void) { return D_00419770; }

/* 0x002E7720 */
void *Room10A_Phase1Script(void) { return D_00419870; }

/* 0x002E7730 */
void *Room10A_Phase2Script(void) { return D_00419950; }

/* 0x002E7740 */
void *Room10A_Phase5Script(void *o) { return D_0047AC80; }   /* Room10A_vtable +0x20 */

/* 0x002E7750 */
void *Room10A_ActionScript(void *self, s32 i) { return D_00419A30[i]; }

/* 0x002E7770 */
void *Room10A_Table38(void) { return D_00419A60; }

/* (self->*D_01990F58[i])(a, b) */
/* 0x002E7780 */
s32 Room10A_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990F58[i & 0xFF], a, b);
}

/* 0x002E77B0 */
s32 Room10A_Cmd00(void *self, void *a1, u8 *cmd) { return room_nudge(&D_0047B27C, cmd, 1.0f); }

/* (self->*D_01990F48[i])(a, b) */
/* 0x002E7850 */
s32 Room10A_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990F48[i & 0xFF], a, b);
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
