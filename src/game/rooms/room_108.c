/* Room 0x108: its event handler class (vtable Room108_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "progress.h"

extern void *RoomBase_vtable[];
extern void *Room108_vtable[];
extern const char *pstr_O_HEW_HEW_201_TEX, *pstr_O_HEW_HEW_202_TEX;

extern u8 Room108_EnterScript_data[];
extern u8 Room108_CharEnterScript_data[];
extern u8 Room108_Phase1Script_data[];
extern u8 Room108_Phase2Script_data[];
extern void *Room108_ActionScripts[];
extern u8 Room108_Phase5Script_data[];
extern u8 Room108_Table38_data[];

extern void *Room108_ObjectNames[];

extern PTMF Room108_CmdTable[];

/* 0x002E7220 */
void *Room108_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room108_vtable, RoomBase_vtable); }

/* 0x002E7280 */
void *Room108_EnterScript(void) { return Room108_EnterScript_data; }

/* 0x002E7290 */
void *Room108_CharEnterScript(void) { return Room108_CharEnterScript_data; }

/* 0x002E72A0 */
void *Room108_Phase1Script(void) { return Room108_Phase1Script_data; }

/* 0x002E72B0 */
void *Room108_Phase2Script(void) { return Room108_Phase2Script_data; }

/* 0x002E72C0 */
void *Room108_ActionScript(void *self, s32 i) { return Room108_ActionScripts[i]; }

/* 0x002E72E0 */
void *Room108_Phase5Script(void) { return Room108_Phase5Script_data; }

/* 0x002E72F0 */
void *Room108_Table38(void) { return Room108_Table38_data; }

/* 0x002E7300 */
void *Room108_ObjectName(void *self, s32 i) { return Room108_ObjectNames[i]; }

/* (self->*Room108_CmdTable[i])(a, b) */
/* 0x002E7320 */
s32 Room108_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room108_CmdTable[i & 0xFF], a, b);
}

/* byte 3: 0 / 1 a named progress call; 2 waits (2) for Progress_Speak(1, 0), then the partner's
 * message slot shows progress +0x73EDC0; else Progress_SpeechCall and the slot is closed */
/* 0x002E7350 */
s32 Room108_Cmd00(void *self, void *a1, u8 *cmd) {
    Progress *p;

    switch (cmd[3]) {
    case 0:
        Progress_LoadSpeech(gProgress, pstr_O_HEW_HEW_201_TEX);
        break;
    case 1:
        Progress_LoadSpeech(gProgress, pstr_O_HEW_HEW_202_TEX);
        break;
    case 2:
        p = gProgress;
        if (Progress_Speak(p, 1, 0) == 0) {
            return 2;
        }
        VCALL(gBootMessage, 0x10, void (*)(VObject *, u32, s32, s32))(gBootMessage, gCharPartner->msgSlot,
                                                                       AT(p, 0x73EDC0, s32), 1);
        break;
    default:
        ((void (*)(Progress *))Progress_SpeechCall)(gProgress);
        VCALL(gBootMessage, 0x14, void (*)(VObject *, u32))(gBootMessage, gCharPartner->msgSlot);
        break;
    }
    return 1;
}
