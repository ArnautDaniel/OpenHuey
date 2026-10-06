/* Room 0x09: its event handler class (vtable Room09_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room09_vtable[];

extern u8 D_003F3240[];
extern u8 D_003F32D0[];
extern u8 D_003F3330[];
extern u8 D_003F3400[];
extern u8 D_003F34A0[];
extern u8 D_003F34D8[];
extern void *D_003F3C00[];
extern void *D_003F3C60[];
extern u8 D_003F3C80[];
extern PTMF D_01990818[];

/* 0x002AA920 */
void *Room09_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room09_vtable, RoomBase_vtable); }

/* 0x002AA980 */
void *Room09_EnterScript(void) {
    return D_003F3240;
}

/* 0x002AA990 */
void *Room09_CharEnterScript(void) {
    return D_003F32D0;
}

/* 0x002AA9A0 */
void *Room09_Phase1Script(void) {
    return D_003F3330;
}

/* 0x002AA9B0 */
void *Room09_Phase2Script(void) {
    return D_003F3400;
}

/* 0x002AA9C0 */
void *Room09_Phase3Script(void) {
    return D_003F34A0;
}

/* 0x002AA9D0 */
void *Room09_Phase5Script(void) {
    return D_003F34D8;
}

/* 0x002AA9E0 */
void *Room09_ActionScript(void *self, s32 i) {
    return D_003F3C00[i];
}

/* 0x002AAA00 */
void *Room09_Table38(void) {
    return D_003F3C80;
}

/* 0x002AAA10 */
void *Room09_ObjectName(void *self, s32 i) {
    return D_003F3C60[i];
}

/* (self->*D_01990818[i])(a, b) */
/* 0x002AAA30 */
s32 Room09_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990818[i & 0xFF], a, b);
}

/* room 0x09 (D_003F3C50): the pursuer (about, not in state 2, in mode 2, 6 or 7) is in another
 * room than 9 (the player about too) */
/* 0x002AAA60 */
s32 Room09_Cond00(void) {
    Character *s = gCharPursuer, *p = gCharPlayer;
    u8 k;

    if (s == NULL || s->a.active == 0 || p == NULL || p->a.active == 0 || AT(s, 0xC4, s32) == 2) {
        return 0;
    }
    k = AT(s, 0x153C, u8);
    if (k != 2 && k != 6 && k != 7) {
        return 0;
    }
    return AT(s, 0x30, s32) != 9;
}
