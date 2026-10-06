/* Room 0x29: its event handler class (vtable D_0046E480, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "progress.h"

extern void *D_0046DB80[];
extern void *D_0046E480[];

extern u8 D_00403F90[];
extern u8 D_00404040[];
extern u8 D_00404080[];
extern u8 D_00404100[];
extern u8 D_00404198[];
extern u32 D_004046C0[];
extern u8 D_00404730[];
extern u32 D_00404710[];

extern PTMF D_01990BB0[];

/* 0x002B0E30 */
void *Room29_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046E480, D_0046DB80); }

/* 0x002B0E90 */
void *Room29_EnterScript(void) {
    return D_00403F90;
}

/* 0x002B0EA0 */
void *Room29_CharEnterScript(void) {
    return D_00404040;
}

/* 0x002B0EB0 */
void *Room29_Phase1Script(void) {
    return D_00404080;
}

/* 0x002B0EC0 */
void *Room29_Phase2Script(void) {
    return D_00404100;
}

/* 0x002B0ED0 */
void *Room29_Phase5Script(void) {
    return D_00404198;
}

/* 0x002B0EE0 */
u32 Room29_ActionScript(void *self, s32 i) {
    return D_004046C0[i];
}

/* 0x002B0F00 */
void *Room29_Table38(void) {
    return D_00404730;
}

/* 0x002B0F10 */
u32 Room29_ObjectName(void *self, s32 i) {
    return D_00404710[i];
}

/* (self->*D_01990BB0[i])(a, b) */
/* 0x002B0F30 */
s32 Room29_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990BB0[i & 0xFF], a, b);
}

/* no thing of kind 3 lies about */
/* 0x002B0F60 */
s32 Room29_Cond01(void) {
    return VCALL(gPlacedThings, 0x10, void *(*)(VObject *, s32, s32))(gPlacedThings, 3, 0) == NULL;
}

/* the stalker in play is chasing (+0x153C 2, 6 or 7, not +0xC4 2) with the progress state 2:
 * in this room, whether the camera sees it; elsewhere 1 */
/* 0x002B0FA0 */
s32 Room29_Cond00(void) {
    u8 *s = (u8 *)gCharSlot2;
    Progress *p;
    u8 k;

    if (s == NULL || AT(s, 0x28, u8) == 0 || AT(s, 0xC4, s32) == 2) {
        return 0;
    }
    k = AT(s, 0x153C, u8);
    if (k != 2 && k != 6 && k != 7) {
        return 0;
    }
    p = gProgress;
    if ((Progress_GameMode(p) & 0xFF) != 2) {
        return 0;
    }
    if (AT(s, 0x30, s32) == VCALL(p, 0xC, s32 (*)(Progress *))(p)) {
        return VCALL(gCamera, 0xD4, s32 (*)(VObject *, f32 *))(gCamera, (f32 *)(s + 0x10));
    }
    return 1;
}
