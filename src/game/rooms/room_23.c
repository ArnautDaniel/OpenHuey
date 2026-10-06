/* Room 0x23: its event handler class (vtable D_0046E340, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "progress.h"
#include "stalker_progress.h"

extern void *D_0046DB80[];
extern void *D_0046E340[];

extern u8 D_004010A0[];
extern u8 D_004010E0[];
extern u8 D_00401140[];
extern u8 D_004011B0[];
extern u32 D_00401800[];
extern u8 D_00401880[];
extern u32 D_00401860[];

extern PTMF D_01990AD8[];
extern PTMF D_01990AF0[];

/* 0x002AF8E0 */
void *Room23_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046E340, D_0046DB80); }

/* 0x002AF940 */
void *Room23_EnterScript(void) {
    return D_004010A0;
}

/* 0x002AF950 */
void *Room23_CharEnterScript(void) {
    return D_004010E0;
}

/* 0x002AF960 */
void *Room23_Phase1Script(void) {
    return D_00401140;
}

/* 0x002AF970 */
void *Room23_Phase2Script(void) {
    return D_004011B0;
}

/* 0x002AF980 */
u32 Room23_ActionScript(void *self, s32 i) {
    return D_00401800[i];
}

/* 0x002AF9A0 */
void *Room23_Table38(void) {
    return D_00401880;
}

/* 0x002AF9B0 */
u32 Room23_ObjectName(void *self, s32 i) {
    return D_00401860[i];
}

/* (self->*D_01990AF0[i])(a, b) */
/* 0x002AF9D0 */
s32 Room23_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990AF0[i & 0xFF], a, b);
}

/* room 0x23 (D_00401848): byte 3 0 a progress name, 1 wait for character 3 (2 while not), else
 * done */
/* 0x002AFA00 */
s32 Room23_Cmd01(void *self, void *a1, u8 *cmd) {
    switch (cmd[3]) {
    case 0:
        Progress_LoadSpeech(gProgress, D_0040187C);
        return 1;
    case 1:
        return Progress_Speak(gProgress, 3, 0) == 0 ? 2 : 1;
    }
    ((void (*)(Progress *))Progress_SpeechCall)(gProgress);
    return 1;
}

/* room 0x23 (D_00401838): Fiona's model +0x9A0 / +0x9A8: 0 (byte 3 1) or 0.12 / 0.2 */
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

/* (self->*D_01990AD8[i])(a, b) */
/* 0x002AFB20 */
s32 Room23_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990AD8[i & 0xFF], a, b);
}

/* room 0x23 (D_00401828): none of the six slots' PursuerGroup_Fields bits 0..3, and the stalker is
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
