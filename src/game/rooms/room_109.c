/* Room 0x109: its event handler class (vtable D_0046FE40, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "progress.h"

extern void *D_0046DB80[];
extern void *D_0046FE40[];
extern s32 D_0047B278;
extern s32 func_0032D150(Character *c);

extern u8 D_004193D0[];
extern u8 D_00419400[];
extern u8 D_00419530[];
extern u8 D_00419638[];
extern void *D_004196E0[];
extern u8 D_00419710[];

extern PTMF D_01990F30[];

/* 0x002E7460 */
void *Room109_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046FE40, D_0046DB80); }

/* 0x002E74C0 */
void *Room109_EnterScript(void) { return D_004193D0; }

/* 0x002E74D0 */
void *Room109_CharEnterScript(void) { return D_00419400; }

/* 0x002E74E0 */
void *Room109_Phase1Script(void) { return D_00419530; }

/* 0x002E74F0 */
void *Room109_Phase5Script(void) { return D_00419638; }

/* 0x002E7500 */
void *Room109_ActionScript(void *self, s32 i) { return D_004196E0[i]; }

/* 0x002E7520 */
void *Room109_Table38(void) { return D_00419710; }

/* (self->*D_01990F30[i])(a, b) */
/* 0x002E7530 */
s32 Room109_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990F30[i & 0xFF], a, b);
}

/* 0x002E7560 */
s32 Room109_Cmd01(void *self, void *a1, u8 *cmd) { return room_nudge(&D_0047B278, cmd, 1.0f); }

/* character kind 0x1A: byte 3 0 starts func_0032D270(2, -6, 257); else waits (2) until
 * func_0032D150 says done */
/* 0x002E7600 */
s32 Room109_Cmd00(void *self, void *a1, u8 *cmd) {
    Character *c = gCharacters[Progress_SlotOfId(gProgress, 0x1A) & 0xFF];

    if (cmd[3] == 0) {
        func_0032D270((u8 *)c, 2, -6.0f, 257.0f);
        return 1;
    }
    return func_0032D150(c) == 0 ? 2 : 1;
}
