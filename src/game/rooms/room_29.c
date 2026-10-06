/* Room 0x29: its event handler class (vtable Room29_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "progress.h"

extern void *RoomBase_vtable[];
extern void *Room29_vtable[];

extern u8 Room29_EnterScript_data[];
extern u8 Room29_CharEnterScript_data[];
extern u8 Room29_Phase1Script_data[];
extern u8 Room29_Phase2Script_data[];
extern u8 Room29_Phase5Script_data[];
extern u32 Room29_ActionScripts[];
extern u8 Room29_Table38_data[];
extern u32 Room29_ObjectNames[];

extern PTMF Room29_CondTable[];

/* 0x002B0E30 */
void *Room29_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room29_vtable, RoomBase_vtable); }

/* 0x002B0E90 */
void *Room29_EnterScript(void) {
    return Room29_EnterScript_data;
}

/* 0x002B0EA0 */
void *Room29_CharEnterScript(void) {
    return Room29_CharEnterScript_data;
}

/* 0x002B0EB0 */
void *Room29_Phase1Script(void) {
    return Room29_Phase1Script_data;
}

/* 0x002B0EC0 */
void *Room29_Phase2Script(void) {
    return Room29_Phase2Script_data;
}

/* 0x002B0ED0 */
void *Room29_Phase5Script(void) {
    return Room29_Phase5Script_data;
}

/* 0x002B0EE0 */
u32 Room29_ActionScript(void *self, s32 i) {
    return Room29_ActionScripts[i];
}

/* 0x002B0F00 */
void *Room29_Table38(void) {
    return Room29_Table38_data;
}

/* 0x002B0F10 */
u32 Room29_ObjectName(void *self, s32 i) {
    return Room29_ObjectNames[i];
}

/* (self->*Room29_CondTable[i])(a, b) */
/* 0x002B0F30 */
s32 Room29_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room29_CondTable[i & 0xFF], a, b);
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
