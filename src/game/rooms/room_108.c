/* Room 0x108: its event handler class (vtable D_0046FE00, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "progress.h"

extern void *D_0046DB80[];
extern void *D_0046FE00[];
extern const char *D_004193A8, *D_004193AC;

extern u8 D_00418DC0[];
extern u8 D_00418E40[];
extern u8 D_00418F40[];
extern u8 D_00419050[];
extern void *D_00419370[];
extern u8 D_004190F0[];
extern u8 D_004193B0[];

extern void *D_004193A0[];

extern PTMF D_01990F18[];

/* 0x002E7220 */
void *Room108_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046FE00, D_0046DB80); }

/* 0x002E7280 */
void *Room108_EnterScript(void) { return D_00418DC0; }

/* 0x002E7290 */
void *Room108_CharEnterScript(void) { return D_00418E40; }

/* 0x002E72A0 */
void *Room108_Phase1Script(void) { return D_00418F40; }

/* 0x002E72B0 */
void *Room108_Phase2Script(void) { return D_00419050; }

/* 0x002E72C0 */
void *Room108_ActionScript(void *self, s32 i) { return D_00419370[i]; }

/* 0x002E72E0 */
void *Room108_Phase5Script(void) { return D_004190F0; }

/* 0x002E72F0 */
void *Room108_Table38(void) { return D_004193B0; }

/* 0x002E7300 */
void *Room108_ObjectName(void *self, s32 i) { return D_004193A0[i]; }

/* (self->*D_01990F18[i])(a, b) */
/* 0x002E7320 */
s32 Room108_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990F18[i & 0xFF], a, b);
}

/* byte 3: 0 / 1 a named progress call; 2 waits (2) for Progress_Speak(1, 0), then the partner's
 * message slot shows progress +0x73EDC0; else Progress_SpeechCall and the slot is closed */
/* 0x002E7350 */
s32 Room108_Cmd00(void *self, void *a1, u8 *cmd) {
    Progress *p;

    switch (cmd[3]) {
    case 0:
        Progress_LoadSpeech(gProgress, D_004193A8);
        break;
    case 1:
        Progress_LoadSpeech(gProgress, D_004193AC);
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
