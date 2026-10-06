/* Room 0x46: its event handler class (vtable Room46_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "actor.h"
#include "hewie.h"
#include "progress.h"
#include "sce/libvu0.h"

extern void *RoomBase_vtable[];
extern void *Room46_vtable[];
extern u8 Room46_Phase5Script_data[];

extern u8 Room46_EnterScript_data[];
extern u8 Room46_CharEnterScript_data[];
extern u8 Room46_Phase1Script_data[];
extern u8 Room46_Phase2Script_data[];
extern u8 Room46_Phase3Script_data[];
extern void *Room46_ActionScripts[];
extern u8 Room46_Table38_data[];
extern void *Room46_ObjectNames[];

extern PTMF Room46_CmdTable[];
extern PTMF Room46_CondTable[];

/* 0x002E5740 */
void *Room46_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room46_vtable, RoomBase_vtable); }

/* 0x002E57A0 */
void *Room46_EnterScript(void) { return Room46_EnterScript_data; }

/* 0x002E57B0 */
void *Room46_CharEnterScript(void) { return Room46_CharEnterScript_data; }

/* 0x002E57C0 */
void *Room46_Phase1Script(void) { return Room46_Phase1Script_data; }

/* 0x002E57D0 */
void *Room46_Phase2Script(void) { return Room46_Phase2Script_data; }

/* 0x002E57E0 */
void *Room46_Phase3Script(void) { return Room46_Phase3Script_data; }

/* 0x002E57F0 */
void *Room46_Phase5Script(void *o) { return Room46_Phase5Script_data; }   /* Room46_vtable +0x20 */

/* 0x002E5800 */
void *Room46_ActionScript(void *self, s32 i) { return Room46_ActionScripts[i]; }

/* 0x002E5820 */
void *Room46_Table38(void) { return Room46_Table38_data; }

/* 0x002E5830 */
void *Room46_ObjectName(void *self, s32 i) { return Room46_ObjectNames[i]; }

/* (self->*Room46_CondTable[i])(a, b) */
/* 0x002E5850 */
s32 Room46_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room46_CondTable[i & 0xFF], a, b);
}

/* the pursuer is about and up to something (+0xE8 0 / 1) or out of sight */
/* 0x002E5880 */
s32 Room46_PursuerBusy(void) {
    Character *p = gCharPursuer;

    if (p != NULL && p->a.active != 0 &&
        (p->unkE8 == 0 || p->unkE8 == 1 || !(VCALL(gCamera, 0xD4, u32 (*)(VObject *, f32 *))(gCamera, p->a.pos) & 0xFF))) {
        return 1;
    }
    return 0;
}

/* (self->*Room46_CmdTable[i])(a, b) */
/* 0x002E5920 */
s32 Room46_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room46_CmdTable[i & 0xFF], a, b);
}

/* a character `c` hook by the command's byte 3: 0 its +0xE1 / +0xF4 cleared, 1 it turns
 * (pi - 0.1 x event var 2, at least 1.57; at the limit event +0x5C(3)) and var 2 goes up by 2,
 * 2 Character_RootMove; then while character kind 0x21 is right of x -84, event var 1 = 1 */
/* 0x002E5950 */
s32 Room46_CharHook(void *self, Character *c, u8 *cmd) {
    Character *who = gCharacters[Progress_SlotOfId(gProgress, 0x21) & 0xFF];
    VObject *ev;
    f32 a;

    switch (cmd[3]) {
    case 0:
        c->unkE1 = 0;
        c->unkF4 = 0;
        break;
    case 1:
        ev = gEvents;
        a = 0x1.921fb6p+1f /* pi */ - 0x1.99999ap-4f /* 0.1 */ * (f32)(u32)VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 2);
        if (a <= 0x1.91eb86p+0f /* 1.57 */) {
            a = 0x1.91eb86p+0f;
            VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 3);
        }
        a = Angle_Wrap(a);
        c->a.angle[1] = a;
        sceVu0UnitMatrix(c->a.rot);
        sceVu0RotMatrixY(c->a.rot, c->a.rot, a);
        ev = gEvents;
        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 2, VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 2) + 2);
        break;
    case 2:
        Character_RootMove(c);
        break;
    }
    if (!(who->a.pos[0] <= -84.0f)) {
        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 1, 1);
    }
    return 1;
}
