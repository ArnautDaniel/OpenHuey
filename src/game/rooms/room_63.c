/* Room 0x63: its event handler class (vtable Room63_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room63_vtable[];

extern u8 Room63_EnterScript_data[];
extern u8 Room63_CharEnterScript_data[];
extern u8 Room63_Phase1Script_data[];
extern u8 Room63_Phase2Script_data[];
extern u8 Room63_Phase5Script_data[];
extern void *Room63_ActionScripts[];
extern void *Room63_ObjectNames[];
extern u8 Room63_Table38_data[];

/* Field access by byte offset into objects whose layout is not yet known. */
#define S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

#define U32(p, off) (*(u32 *)((u8 *)(p) + (off)))

extern PTMF Room63_CmdTable[];

/* 0x00308990 */
void *Room63_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room63_vtable, RoomBase_vtable); }

/* 0x003089F0 */
void *Room63_EnterScript(void) {
    return Room63_EnterScript_data;
}

/* 0x00308A00 */
void *Room63_CharEnterScript(void) {
    return Room63_CharEnterScript_data;
}

/* 0x00308A10 */
void *Room63_Phase1Script(void) {
    return Room63_Phase1Script_data;
}

/* 0x00308A20 */
void *Room63_Phase2Script(void) {
    return Room63_Phase2Script_data;
}

/* 0x00308A30 */
void *Room63_Phase5Script(void) {
    return Room63_Phase5Script_data;
}

/* 0x00308A40 */
void *Room63_ActionScript(void *self, s32 i) {
    return Room63_ActionScripts[i];
}

/* 0x00308A60 */
void *Room63_Table38(void) {
    return Room63_Table38_data;
}

/* 0x00308A70 */
void *Room63_ObjectName(void *self, s32 i) {
    return Room63_ObjectNames[i];
}

/* (self->*Room63_CmdTable[i])(a, b) */
/* 0x00308A90 */
s32 Room63_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room63_CmdTable[i & 0xFF], a, b);
}

/* 0x00308AC0 */
s32 Room63_Cmd00(void *self, u8 *obj) {
    U32(obj, 0x104) = U32(gCharPlayer, 0x34);
    S32(obj, 0x108) = 0x204;
    obj[0xE1] = 0;
    S32(obj, 0xF4) = 6;
    return 1;
}
