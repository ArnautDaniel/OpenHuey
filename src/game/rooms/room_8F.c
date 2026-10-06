/* Room 0x8F: its event handler class (vtable Room8F_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "actor.h"
#include "fiona.h"
#include "progress.h"

extern void *RoomBase_vtable[];
extern void *Room8F_vtable[];
extern const char *pstr_O_FIO_FIO_201_TEX, *pstr_O_FIH_FIH_201_TEX;

extern u32 Room8F_EnterScript_data[];
extern u32 Room8F_CharEnterScript_data[];
extern u32 Room8F_Phase1Script_data[];
extern u32 Room8F_Phase2Script_data[];
extern u32 Room8F_Phase3Script_data[];
extern u32 Room8F_ActionScripts[];
extern u32 Room8F_Table38_data[];

extern u32 Room8F_ObjectNames[];

extern PTMF Room8F_CmdTable[];
extern PTMF Room8F_CondTable[];

/* 0x00340D60 */
void *Room8F_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room8F_vtable, RoomBase_vtable); }

/* 0x00340DC0 */
void *Room8F_EnterScript(void) {
    return Room8F_EnterScript_data;
}

/* 0x00340DD0 */
void *Room8F_CharEnterScript(void) {
    return Room8F_CharEnterScript_data;
}

/* 0x00340DE0 */
void *Room8F_Phase1Script(void) {
    return Room8F_Phase1Script_data;
}

/* 0x00340DF0 */
void *Room8F_Phase2Script(void) {
    return Room8F_Phase2Script_data;
}

/* 0x00340E00 */
void *Room8F_Phase3Script(void) {
    return Room8F_Phase3Script_data;
}

/* 0x00340E10 */
u32 Room8F_ActionScript(void *self, s32 i) {
    return Room8F_ActionScripts[i];
}

/* 0x00340E30 */
void *Room8F_Table38(void) {
    return Room8F_Table38_data;
}

/* 0x00340E40 */
u32 Room8F_ObjectName(void *self, s32 i) {
    return Room8F_ObjectNames[i];
}

/* (self->*Room8F_CondTable[i])(a, b) */
/* 0x00340E60 */
s32 Room8F_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room8F_CondTable[i & 0xFF], a, b);
}

/* the pursuer is active and out of view: state 3 (+0xE8), or the camera's on-screen test
 * (+0xD4) fails */
/* 0x00340E90 */
s32 Room8F_Cond00(void) {
    if (gCharPursuer == NULL || AT(gCharPursuer, 0x28, u8) == 0) {
        return 0;
    }
    if (AT(gCharPursuer, 0xE8, s32) == 3) {
        return 1;
    }
    if ((u8)VCALL(gCamera, 0xD4, s32 (*)(VObject *, void *))(gCamera, (u8 *)gCharPursuer + 0x10) == 0) {
        return 1;
    }
    return 0;
}

/* (self->*Room8F_CmdTable[i])(a, b) */
/* 0x00340F20 */
s32 Room8F_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room8F_CmdTable[i & 0xFF], a, b);
}

/* 0x00340F50 */
s32 Room8F_Cmd03(void) { return slam_shake(); }

/* 0x00340FE0 */
s32 Room8F_Cmd02(void) {
    static const s16 spot[3] = {0, 1, 6};
    static const f32 b[3] = {0.0f, 0.0f, 0.0f}, c[3] = {0.0f, 0.0f, 0.0f}, d[3] = {1.0f, 1.0f, 1.0f};

    return grey_three(50.0f, spot, b, c, d);
}

/* byte 3: 0 / 1 a named progress call; 2 waits (2) for Progress_Speak(0, 0); else Progress_SpeechCall */
/* 0x00341300 */
s32 Room8F_Cmd01(void *self, void *a1, u8 *cmd) {
    switch (cmd[3]) {
    case 0:
        Progress_LoadSpeech(gProgress, pstr_O_FIO_FIO_201_TEX);
        return 1;
    case 1:
        Progress_LoadSpeech(gProgress, pstr_O_FIH_FIH_201_TEX);
        return 1;
    case 2:
        return Progress_Speak(gProgress, 0, 0) == 0 ? 2 : 1;
    }
    ((void (*)(Progress *))Progress_SpeechCall)(gProgress);
    return 1;
}

/* a struggle: byte 3 0 resets Fiona's shake tracking (+0x1AD710 / +0x1AD714); 1 adds her shakes
 * to script variable byte 4, with a grunt (voice 0x3D or 0x45 at random) when the cool-down
 * variable byte 6 is out (it then runs 45 / 60), and at 100 the event byte 5 (+0x5C) */
/* 0x003413C0 */
s32 Room8F_Cmd00(void *self, void *a1, u8 *cmd) {
    VObject *ev;
    s32 v, n;

    switch (cmd[3]) {
    case 0:
        AT(gCharPlayer, 0x1AD710, u8) = 1;
        AT(gCharPlayer, 0x1AD714, s32) = 0;
        break;
    case 1:
        ev = gEvents;
        v = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, cmd[4]);
        n = Fiona_Shakes((Fiona *)gCharPlayer);
        if (n != 0 && VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, cmd[6]) == 0) {
            if (VCALL(gRandom, 0x10, u32 (*)(VObject *))(gRandom) & 1) {
                Actor_PlaySound(&gCharPlayer->a, 0x3D, 5, 0, 0, NULL);
                VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, cmd[6], 45);
            } else {
                Actor_PlaySound(&gCharPlayer->a, 0x45, 5, 0, 0, NULL);
                VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, cmd[6], 60);
            }
        }
        n += v;
        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, cmd[4], n);
        if ((u32)n >= 100) {
            VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, cmd[5]);
        }
        break;
    }
    return 1;
}
