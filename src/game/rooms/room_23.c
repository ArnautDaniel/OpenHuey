/* Room 0x23: its event handler class (vtable Room23_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "progress.h"

extern void *RoomBase_vtable[];
extern void *Room23_vtable[];

extern u8 Room23_EnterScript_data[];
extern u8 Room23_CharEnterScript_data[];
extern u8 Room23_Phase1Script_data[];
extern u8 Room23_Phase2Script_data[];
extern u32 Room23_ActionScripts[];
extern u8 Room23_Table38_data[];
extern u32 Room23_ObjectNames[];

extern PTMF Room23_CondTable[];
extern PTMF Room23_CmdTable[];

/* 0x002AF8E0 */
void *Room23_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room23_vtable, RoomBase_vtable); }

/* 0x002AF940 */
void *Room23_EnterScript(void) {
    return Room23_EnterScript_data;
}

/* 0x002AF950 */
void *Room23_CharEnterScript(void) {
    return Room23_CharEnterScript_data;
}

/* 0x002AF960 */
void *Room23_Phase1Script(void) {
    return Room23_Phase1Script_data;
}

/* 0x002AF970 */
void *Room23_Phase2Script(void) {
    return Room23_Phase2Script_data;
}

/* 0x002AF980 */
u32 Room23_ActionScript(void *self, s32 i) {
    return Room23_ActionScripts[i];
}

/* 0x002AF9A0 */
void *Room23_Table38(void) {
    return Room23_Table38_data;
}

/* 0x002AF9B0 */
u32 Room23_ObjectName(void *self, s32 i) {
    return Room23_ObjectNames[i];
}

/* (self->*Room23_CmdTable[i])(a, b) */
/* 0x002AF9D0 */
s32 Room23_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room23_CmdTable[i & 0xFF], a, b);
}

/* room 0x23 (Room23_Cmd01_ptmf): byte 3 0 a progress name, 1 wait for character 3 (2 while not), else
 * done */
/* 0x002AFA00 */
s32 Room23_Cmd01(void *self, void *a1, u8 *cmd) {
    switch (cmd[3]) {
    case 0:
        Progress_LoadSpeech(gProgress, pstr_O_DNL_DNL_201_TEX);
        return 1;
    case 1:
        return Progress_Speak(gProgress, 3, 0) == 0 ? 2 : 1;
    }
    ((void (*)(Progress *))Progress_SpeechCall)(gProgress);
    return 1;
}

/* room 0x23 (Room23_Cmd00_ptmf): Fiona's model +0x9A0 / +0x9A8: 0 (byte 3 1) or 0.12 / 0.2 */
/* 0x002AFAA0 */
s32 Room23_Cmd00(void *self, void *a1, u8 *cmd) {
    u8 *m = gCharacters[(u8)Progress_SlotOfId(gProgress, 3)]->motion;

    if (cmd[3] == 1) {
        AT(m, 0x9A0, f32) = 0.0f;
        AT(m, 0x9A8, f32) = 0.0f;
    } else {
        AT(m, 0x9A0, u32) = 0x3DF5C28F;   /* 0.12 */
        AT(m, 0x9A8, u32) = 0x3E4CCCCD;   /* 0.2 */
    }
    return 1;
}

/* (self->*Room23_CondTable[i])(a, b) */
/* 0x002AFB20 */
s32 Room23_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room23_CondTable[i & 0xFF], a, b);
}

/* room 0x23 (Room23_Cond00_ptmf): none of the six slots' PursuerGroup_Fields bits 0..3, and the stalker is
 * about but not active, in mode 2, 6 or 7 */
/* 0x002AFB50 */
s32 Room23_Cond00(void) {
    u32 acc = 0;
    s32 i;
    u8 *c;
    u8 k;

    for (i = 0; i < 6; i++) {
        acc |= (u8)PursuerGroup_Fields(gProgress, 0, i & 0xFF);
    }
    if (acc & 0xF) {
        return 0;
    }
    c = (u8 *)gCharSlot2;
    if (c == NULL || AT(c, 0x28, u8) == 1) {
        return 0;
    }
    k = AT(c, 0x153C, u8);
    return k == 2 || k == 6 || k == 7;
}
