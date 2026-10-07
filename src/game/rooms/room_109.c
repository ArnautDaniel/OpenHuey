/* Room 0x109: its event handler class (vtable Room109_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "progress.h"

extern void *RoomBase_vtable[];
extern void *Room109_vtable[];
extern s32 D_0047B278;
extern s32 Kind26_MoveDone(Character *c);

extern u8 Room109_EnterScript_data[];
extern u8 Room109_CharEnterScript_data[];
extern u8 Room109_Phase1Script_data[];
extern u8 Room109_Phase5Script_data[];
extern void *Room109_ActionScripts[];
extern u8 Room109_Table38_data[];

extern PTMF Room109_CmdTable[];

/* 0x002E7460 */
void *Room109_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room109_vtable, RoomBase_vtable); }

/* 0x002E74C0 */
void *Room109_EnterScript(void) { return Room109_EnterScript_data; }

/* 0x002E74D0 */
void *Room109_CharEnterScript(void) { return Room109_CharEnterScript_data; }

/* 0x002E74E0 */
void *Room109_Phase1Script(void) { return Room109_Phase1Script_data; }

/* 0x002E74F0 */
void *Room109_Phase5Script(void) { return Room109_Phase5Script_data; }

/* 0x002E7500 */
void *Room109_ActionScript(void *self, s32 i) { return Room109_ActionScripts[i]; }

/* 0x002E7520 */
void *Room109_Table38(void) { return Room109_Table38_data; }

/* (self->*Room109_CmdTable[i])(a, b) */
/* 0x002E7530 */
s32 Room109_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room109_CmdTable[i & 0xFF], a, b);
}

/* room 0x109: byte 3 0 starts a 90-frame count; while it runs, character 0xE drifts up 3 and
 * sideways 1 a frame (room_nudge). */
/* 0x002E7560 */
s32 Room109_Cmd01(void *self, void *a1, u8 *cmd) { return room_nudge(&D_0047B278, cmd, 1.0f); }

/* character kind 0x1A: byte 3 0 starts Kind26_MoveTo(2, -6, 257); else waits (2) until
 * Kind26_MoveDone says done */
/* 0x002E7600 */
s32 Room109_Cmd00(void *self, void *a1, u8 *cmd) {
    Character *c = gCharacters[Progress_SlotOfId(gProgress, 0x1A) & 0xFF];

    if (cmd[3] == 0) {
        Kind26_MoveTo((u8 *)c, 2, -6.0f, 257.0f);
        return 1;
    }
    return Kind26_MoveDone(c) == 0 ? 2 : 1;
}
