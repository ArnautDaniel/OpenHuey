/* Room 0x19: its event handler class (vtable Room19_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "actor.h"
#include "progress.h"

extern void *RoomBase_vtable[];
extern void *Room19_vtable[];

extern u8 D_003FB3B0[];
extern u8 D_003FB420[];
extern u8 D_003FB520[];
extern u8 D_003FB720[];
extern u8 D_003FB840[];
extern void *D_003FBFD0[];
extern void *D_003FC020[];
extern u8 D_003FC040[];
extern PTMF D_01990950[];

/* 0x002AD1C0 */
void *Room19_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room19_vtable, RoomBase_vtable); }

/* 0x002AD220 */
void *Room19_EnterScript(void) {
    return D_003FB3B0;
}

/* 0x002AD230 */
void *Room19_CharEnterScript(void) {
    return D_003FB420;
}

/* 0x002AD240 */
void *Room19_Phase1Script(void) {
    return D_003FB520;
}

/* 0x002AD250 */
void *Room19_Phase2Script(void) {
    return D_003FB720;
}

/* 0x002AD260 */
void *Room19_Phase3Script(void) {
    return D_003FB840;
}

/* 0x002AD270 */
void *Room19_ActionScript(void *self, s32 i) {
    return D_003FBFD0[i];
}

/* 0x002AD290 */
void *Room19_Table38(void) {
    return D_003FC040;
}

/* 0x002AD2A0 */
void *Room19_ObjectName(void *self, s32 i) {
    return D_003FC020[i];
}

/* (self->*D_01990950[i])(a, b) */
/* 0x002AD2C0 */
s32 Room19_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990950[i & 0xFF], a, b);
}

/* room 0x19 (D_003FC010): the pursuer (about, not in state 2, in mode 2, 6 or 7) while Hewie is
 * controlled: in another room, or 30 or more from the player */
/* 0x002AD2F0 */
s32 Room19_Cond00(void) {
    Character *s = gCharPursuer, *p = gCharPlayer;
    Progress *g;
    u8 k;

    if (s == NULL || s->a.active == 0 || p == NULL || p->a.active == 0 || AT(s, 0xC4, s32) == 2) {
        return 0;
    }
    k = AT(s, 0x153C, u8);
    if (k != 2 && k != 6 && k != 7) {
        return 0;
    }
    g = gProgress;
    if ((u8)Progress_GameMode(g) != 2) {
        return 0;
    }
    if (AT(s, 0x30, s32) != VCALL((VObject *)g, 0xC, s32 (*)(VObject *))((VObject *)g)) {
        return 1;
    }
    return !(Actor_Distance(&s->a, p->a.pos) < 30.0f);
}
