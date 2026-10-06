/* Room 0x99: its event handler class (vtable Room99_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room99_vtable[];
extern u8 Room99_EnterScript_data[], Room99_Phase2Script_data[];

extern u8 Room99_CharEnterScript_data[], Room99_Phase1Script_data[], Room99_Phase5Script_data[];
extern void *Room99_ActionScripts[];
extern u8 Room99_Table38_data[];

extern PTMF Room99_CmdTable[];

/* 0x00352AF0 */
void *Room99_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room99_vtable, RoomBase_vtable); }

/* 0x00352B50 */
void *Room99_EnterScript(void *o) { return Room99_EnterScript_data; }   /* Room99_vtable +0xC */

/* 0x00352B60 */
void *Room99_CharEnterScript(void) { return Room99_CharEnterScript_data; }

/* 0x00352B70 */
void *Room99_Phase1Script(void) { return Room99_Phase1Script_data; }

/* 0x00352B80 */
void *Room99_Phase2Script(void *o) { return Room99_Phase2Script_data; }   /* Room99_vtable +0x14 */

/* 0x00352B90 */
void *Room99_Phase5Script(void) { return Room99_Phase5Script_data; }

/* 0x00352BA0 */
void *Room99_ActionScript(void *self, s32 i) { return Room99_ActionScripts[i]; }

/* 0x00352BC0 */
void *Room99_Table38(void) { return Room99_Table38_data; }

/* (self->*Room99_CmdTable[i])(a, b) */
/* 0x00352BD0 */
s32 Room99_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room99_CmdTable[i & 0xFF], a, b);
}

/* 0x00352C00 */
s32 Room99_SlamShake(void) { return slam_shake(); }
