/* Room 0x21: its event handler class (vtable D_0046E2C0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "progress.h"

extern void *D_0046DB80[];
extern void *D_0046E2C0[];
extern void *D_0046EA40[];

extern u8 D_003FF170[];
extern u8 D_003FF200[];
extern u8 D_003FF340[];
extern u8 D_003FF650[];
extern u8 D_003FF810[];

extern u32 D_00400AD0[];
extern u8 D_00400C40[];
extern u32 D_00400C00[];

extern PTMF D_01990A70[];
extern PTMF D_01990AC8[];

/* 0x002AEFC0 */
void *Room21_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046E2C0, D_0046DB80); }

/* 0x002AF020 */
void *Room21_EnterScript(void) {
    return D_003FF170;
}

/* 0x002AF030 */
void *Room21_CharEnterScript(void) {
    return D_003FF200;
}

/* 0x002AF040 */
void *Room21_Phase1Script(void) {
    return D_003FF340;
}

/* 0x002AF050 */
void *Room21_Phase2Script(void) {
    return D_003FF650;
}

/* 0x002AF060 */
void *Room21_Phase3Script(void) {
    return D_003FF810;
}

/* 0x002AF070 */
u32 Room21_ActionScript(void *self, s32 i) {
    return D_00400AD0[i];
}

/* 0x002AF090 */
void *Room21_Table38(void) {
    return D_00400C40;
}

/* 0x002AF0A0 */
u32 Room21_ObjectName(void *self, s32 i) {
    return D_00400C00[i];
}

/* (self->*D_01990AC8[i])(a, b) */
/* 0x002AF0C0 */
s32 Room21_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990AC8[i & 0xFF], a, b);
}

/* room 0x21 (D_00400BF0): the pursuer, about and not in state 2, is in its mode 2 but in
 * another room than the current one (the player about too) */
/* 0x002AF0F0 */
s32 Room21_Cond00(void) {
    Character *s = gCharPursuer, *p = gCharPlayer;

    if (s == NULL || s->a.active == 0 || p == NULL || p->a.active == 0) {
        return 0;
    }
    if (AT(s, 0xC4, s32) == 2 || AT(s, 0x153C, u8) != 2) {
        return 0;
    }
    return AT(s, 0x30, s32) != VCALL((VObject *)gProgress, 0xC, s32 (*)(VObject *))((VObject *)gProgress);
}

/* (self->*D_01990A70[i])(a, b) */
/* 0x002AF1A0 */
s32 Room21_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990A70[i & 0xFF], a, b);
}

/* room 0x21 (D_00400BE0): Fiona's model +0x1570 (byte 3 0) / +0x1574 (1) = byte 4 */
/* 0x002AF1D0 */
s32 Room21_Cmd06(void *self, void *a1, u8 *cmd) {
    u8 *m = gCharacters[(u8)Progress_SlotOfId(gProgress, 3)]->motion;

    switch (cmd[3]) {
    case 1:
        AT(m, 0x1574, s32) = cmd[4];
        break;
    case 0:
        AT(m, 0x1570, s32) = cmd[4];
        break;
    }
    return 1;
}

/* room 0x21 (D_00400BD0): the player's model +0xC8 vector by byte 3 */
/* 0x002AF250 */
s32 Room21_Cmd05(void *self, void *a1, u8 *cmd) {
    VObject *m = gCharPlayer->motion;
    f32 v[4] __attribute__((aligned(16)));

    if (cmd[3] == 0) {
        v[0] = 0.0f;
        v[1] = 0x1.99999a0000000p-4f /* 0.1 */;
        v[2] = -0x1.47ae140000000p-7f /* 0.01 */;
    } else if (cmd[3] == 1) {
        v[2] = 0.0f;
        v[0] = 0x1.47ae140000000p-6f /* 0.02 */;
        v[1] = 0x1.99999a0000000p-4f /* 0.1 */;
    } else {
        v[0] = 0.0f;
        v[2] = 0.0f;
        v[1] = 0x1.99999a0000000p-4f /* 0.1 */;
    }
    VCALL(m, 0xC8, void (*)(VObject *, f32 *))(m, v);
    return 1;
}

/* room 0x21 (D_00400BC8) */
/* 0x002AF300 */
s32 Room21_Cmd04(void *self, void *a1, u8 *cmd) {
    return lit_quad_in(0x1A, cmd, sQuadDoor, 0x20000040);
}

/* Sets bit 1 of the flag byte three times (inlined setter calls); returns 1. */
/* 0x002AF4A0 */
s32 Room21_Cmd03(void) {
    u8 *obj = (u8 *)gCharPlayer;

    (*(u8 **)(obj + 0xF0))[0xAC] |= 2;
    (*(u8 **)(obj + 0xF0))[0xAC] |= 2;
    (*(u8 **)(obj + 0xF0))[0xAC] |= 2;
    return 1;
}

/* room 0x21 (D_00400BA8) */
/* 0x002AF4E0 */
s32 Room21_Cmd02(void *self, void *a1, u8 *cmd) {
    return lit_quad(cmd, sQuadWindow, 0x10000040);
}

/* room 0x21 (D_00400B98): the fan turns, except while a movie plays */
/* 0x002AF680 */
s32 Room21_Cmd01(void) {
    if (VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress) != 0) {
        return 1;
    }
    fan_turn(D_00400C38);
    return 1;
}

/* 0x002AF730 */
s32 Room21_Cmd00(void) {   /* room effect 1 (D_0046EA40) */
    room_effect_slot_new(gRoomEffects, 1, D_0046EA40);
    return 1;
}
