/* Room 0x19: its event handler class (vtable D_0046E0C0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E0C0[];
extern s32 func_00177620(Progress *p);

extern u8 D_003FB3B0[];
extern u8 D_003FB420[];
extern u8 D_003FB520[];
extern u8 D_003FB720[];
extern u8 D_003FB840[];
extern void *D_003FBFD0[];
extern void *D_003FC020[];
extern u8 D_003FC040[];
extern PTMF D_01990950[];

void *func_002AD1C0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E0C0, D_0046DB80); }

void *func_002AD220(void) {
    return D_003FB3B0;
}

void *func_002AD230(void) {
    return D_003FB420;
}

void *func_002AD240(void) {
    return D_003FB520;
}

void *func_002AD250(void) {
    return D_003FB720;
}

void *func_002AD260(void) {
    return D_003FB840;
}

void *func_002AD270(void *self, s32 i) {
    return D_003FBFD0[i];
}

void *func_002AD290(void) {
    return D_003FC040;
}

void *func_002AD2A0(void *self, s32 i) {
    return D_003FC020[i];
}

/* (self->*D_01990950[i])(a, b) */
s32 func_002AD2C0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990950[i & 0xFF], a, b);
}

/* room 0x19 (D_003FC010): the pursuer (about, not in state 2, in mode 2, 6 or 7) while Hewie is
 * controlled: in another room, or 30 or more from the player */
s32 func_002AD2F0(void) {
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
    if ((u8)func_00177620(g) != 2) {
        return 0;
    }
    if (AT(s, 0x30, s32) != VCALL((VObject *)g, 0xC, s32 (*)(VObject *))((VObject *)g)) {
        return 1;
    }
    return !(func_00124490(&s->a, p->a.pos) < 30.0f);
}
