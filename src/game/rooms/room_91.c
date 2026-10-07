/* Room 0x91: its event handler class (vtable Room91_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room91_vtable[];

extern u32 Room91_EnterScript_data[];
extern u32 Room91_CharEnterScript_data[];
extern u32 Room91_Phase1Script_data[];
extern u32 Room91_Phase2Script_data[];
extern u32 Room91_Phase3Script_data[];
extern u32 Room91_ActionScripts[];
extern u32 Room91_Table38_data[];

extern PTMF Room91_CmdTable[];

/* 0x00341580 */
void *Room91_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room91_vtable, RoomBase_vtable); }

/* 0x003415E0 */
void *Room91_EnterScript(void) {
    return Room91_EnterScript_data;
}

/* 0x003415F0 */
void *Room91_CharEnterScript(void) {
    return Room91_CharEnterScript_data;
}

/* 0x00341600 */
void *Room91_Phase1Script(void) {
    return Room91_Phase1Script_data;
}

/* 0x00341610 */
void *Room91_Phase2Script(void) {
    return Room91_Phase2Script_data;
}

/* 0x00341620 */
void *Room91_Phase3Script(void) {
    return Room91_Phase3Script_data;
}

/* 0x00341630 */
u32 Room91_ActionScript(void *self, s32 i) {
    return Room91_ActionScripts[i];
}

/* 0x00341650 */
void *Room91_Table38(void) {
    return Room91_Table38_data;
}

/* (self->*Room91_CmdTable[i])(a, b) */
/* 0x00341660 */
s32 Room91_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room91_CmdTable[i & 0xFF], a, b);
}

/* room 0x91: when the stalker is Lorenzo (kind 0xA) and his slam lands this frame, a camera
 * shake of 0.5 (slam_shake; a frame hook). */
/* 0x00341690 */
s32 Room91_Cmd03(void) { return slam_shake(); }

/* room 0x91: three grey smoke effects (Effect79B00, size 70) at the room's spots 1, 3, 0xB
 * (grey_three). */
/* 0x00341720 */
s32 Room91_Cmd02(void) {
    static const s16 spot[3] = {1, 3, 0xB};
    static const f32 b[3] = {0.0f, 0.0f, 0.0f}, c[3] = {0.0f, 0.0f, 0.5f}, d[3] = {0.5f, 0.5f, 0.5f};

    return grey_three(70.0f, spot, b, c, d);
}

/* script variable 0 down by the player's hit (1 from the weak blow 0x1A, else 5), not below 0 */
/* 0x00341A40 */
s32 Room91_Cmd01(void) {
    VObject *ev = gEvents;
    s32 v = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0);

    v -= AT(gCharPlayer, 0xFC, s32) == 0x1A ? 1 : 5;
    if (v < 0) {
        v = 0;
    }
    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 0, v);
    return 1;
}

/* room 0x91 (Room91_Cmd00_ptmf): door 0's +0x68 (0) */
/* 0x00341AD0 */
s32 Room91_Cmd00(void) {
    VCALL(gDoors, 0x68, void (*)(VObject *, s32, s32))(gDoors, 0, 0);
    return 1;
}
