/* Room 0x0A: its event handler class (vtable Room0A_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room0A_vtable[];
extern const char *pstr_syuukouki;

extern u8 Room0A_EnterScript_data[];
extern u8 Room0A_CharEnterScript_data[];
extern u8 Room0A_Phase1Script_data[];
extern u8 Room0A_Phase2Script_data[];
extern u8 Room0A_Phase3Script_data[];
extern void *Room0A_ActionScripts[];
extern void *Room0A_ObjectNames[];
extern u8 Room0A_Table38_data[];
extern PTMF Room0A_CmdTable[];

/* 0x002AAB00 */
void *Room0A_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room0A_vtable, RoomBase_vtable); }

/* 0x002AAB60 */
void *Room0A_EnterScript(void) {
    return Room0A_EnterScript_data;
}

/* 0x002AAB70 */
void *Room0A_CharEnterScript(void) {
    return Room0A_CharEnterScript_data;
}

/* 0x002AAB80 */
void *Room0A_Phase1Script(void) {
    return Room0A_Phase1Script_data;
}

/* 0x002AAB90 */
void *Room0A_Phase2Script(void) {
    return Room0A_Phase2Script_data;
}

/* 0x002AABA0 */
void *Room0A_Phase3Script(void) {
    return Room0A_Phase3Script_data;
}

/* 0x002AABB0 */
void *Room0A_ActionScript(void *self, s32 i) {
    return Room0A_ActionScripts[i];
}

/* 0x002AABD0 */
void *Room0A_Table38(void) {
    return Room0A_Table38_data;
}

/* 0x002AABE0 */
void *Room0A_ObjectName(void *self, s32 i) {
    return Room0A_ObjectNames[i];
}

/* (self->*Room0A_CmdTable[i])(a, b) */
/* 0x002AAC00 */
s32 Room0A_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room0A_CmdTable[i & 0xFF], a, b);
}

/* room 0x0A (Room0A_Cmd01_ptmf): an effect on its object at -2.88 */
/* 0x002AAC30 */
s32 Room0A_Cmd01(void) {
    obj_effect(room_obj(pstr_syuukouki), 0xC0384E89);
    return 1;
}

/* the dial pstr_syuukouki on progress var 3 */
/* 0x002AAD60 */
s32 Room0A_Cmd00(void *self, void *a1, u8 *cmd) {
    u8 *o = VCALL(gRoomObjects, 0x18, u8 *(*)(VObject *, const char *))(gRoomObjects, pstr_syuukouki);

    if (o == NULL) {
        return 1;
    }
    return dial_step(cmd[3], o, 3, 0, 0);
}
