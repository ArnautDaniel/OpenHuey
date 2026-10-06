/* Room 0x1D: its event handler class (vtable Room1D_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "progress.h"

extern void *RoomBase_vtable[];
extern void *Room1D_vtable[];
extern u8 Room1D_Phase5Script_data[];

extern u8 Room1D_EnterScript_data[];
extern u8 Room1D_CharEnterScript_data[];
extern u8 Room1D_Phase1Script_data[];
extern u8 Room1D_Phase2Script_data[];
extern u32 Room1D_ActionScripts[];
extern u8 Room1D_Phase3Script_data[];
extern u8 Room1D_Table38_data[];

extern PTMF Room1D_CmdTable[];
extern PTMF Room1D_CondTable[];

/* 0x002ADD10 */
void *Room1D_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room1D_vtable, RoomBase_vtable); }

/* 0x002ADD70 */
void *Room1D_EnterScript(void) {
    return Room1D_EnterScript_data;
}

/* 0x002ADD80 */
void *Room1D_CharEnterScript(void) {
    return Room1D_CharEnterScript_data;
}

/* 0x002ADD90 */
void *Room1D_Phase1Script(void) {
    return Room1D_Phase1Script_data;
}

/* 0x002ADDA0 */
void *Room1D_Phase2Script(void) {
    return Room1D_Phase2Script_data;
}

/* 0x002ADDB0 */
u32 Room1D_ActionScript(void *self, s32 i) {
    return Room1D_ActionScripts[i];
}

/* 0x002ADDD0 */
void *Room1D_Phase3Script(void) {
    return Room1D_Phase3Script_data;
}

/* 0x002ADDE0 */
void *Room1D_Phase5Script(void *o) { return Room1D_Phase5Script_data; }   /* Room1D_vtable +0x20 */

/* 0x002ADDF0 */
void *Room1D_Table38(void) {
    return Room1D_Table38_data;
}

/* (self->*Room1D_CondTable[i])(a, b) */
/* 0x002ADE00 */
s32 Room1D_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room1D_CondTable[i & 0xFF], a, b);
}

/* Fiona in move 5, room 0x1D's flag 0 not set and its exit 0's door shut */
/* 0x002ADE30 */
s32 Room1D_Cond00(void) {
    Progress *p;

    if (AT(gCharPlayer, 0xFC, s32) != 5) {
        return 0;
    }
    p = gProgress;
    if (Progress_CurRoomFlag(p, 0x1D, 0) != 0 || Progress_ExitOpen(p, 0x1D, 0) != 0) {
        return 0;
    }
    return 1;
}

/* (self->*Room1D_CmdTable[i])(a, b) */
/* 0x002ADEB0 */
s32 Room1D_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room1D_CmdTable[i & 0xFF], a, b);
}

/* room 0x1D (Room1D_Cmd01_ptmf): door 0's +0x74 (0, or -0.08 by byte 3) */
/* 0x002ADEE0 */
s32 Room1D_Cmd01(void *self, void *a1, u8 *cmd) {
    VCALL(gDoors, 0x74, void (*)(VObject *, s32, f32))(gDoors, 0, cmd[3] == 0 ? 0.0f : -0x1.47ae140000000p-4f /* 0.08 */);
    return 1;
}

/* room 0x1D (Room1D_Cmd00_ptmf): script variable 0 = 2 .. 5 at random */
/* 0x002ADF40 */
s32 Room1D_Cmd00(void) {
    s32 r = VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) & 3;

    VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 0, r + 2);
    return 1;
}
