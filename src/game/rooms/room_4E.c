/* Room 0x4E: its event handler class (vtable Room4E_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room4E_vtable[];
extern const char *pstr_Curtain_2;
extern void *Room4EEffect_vtable[];
extern u8 Room4E_EnterScript_data[];
extern u8 Room4E_CharEnterScript_data[];
extern u8 Room4E_Phase1Script_data[];
extern u8 Room4E_Phase2Script_data[];
extern u8 Room4E_Phase5Script_data[];
extern u32 Room4E_ActionScripts[];
extern u8 Room4E_Table38_data[];
extern u32 Room4E_ObjectNames[];

extern PTMF Room4E_CmdTable[];

static void effect_78BE0_init(void **obj) {
    obj[0] = Room4EEffect_vtable;
}

/* 0x002B3970 */
void *Room4E_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room4E_vtable, RoomBase_vtable); }

/* 0x002B39D0 */
void *Room4E_EnterScript(void) {
    return Room4E_EnterScript_data;
}

/* 0x002B39E0 */
void *Room4E_CharEnterScript(void) {
    return Room4E_CharEnterScript_data;
}

/* 0x002B39F0 */
void *Room4E_Phase1Script(void) {
    return Room4E_Phase1Script_data;
}

/* 0x002B3A00 */
void *Room4E_Phase2Script(void) {
    return Room4E_Phase2Script_data;
}

/* 0x002B3A10 */
void *Room4E_Phase5Script(void) {
    return Room4E_Phase5Script_data;
}

/* 0x002B3A20 */
u32 Room4E_ActionScript(void *self, s32 i) {
    return Room4E_ActionScripts[i];
}

/* 0x002B3A40 */
void *Room4E_Table38(void) {
    return Room4E_Table38_data;
}

/* 0x002B3A50 */
u32 Room4E_ObjectName(void *self, s32 i) {
    return Room4E_ObjectNames[i];
}

/* (self->*Room4E_CmdTable[i])(a, b) */
/* 0x002B3A70 */
s32 Room4E_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room4E_CmdTable[i & 0xFF], a, b);
}

/* room 0x4E (D_0040B4F8): the 0x10-byte effect Room4EEffect_vtable made */
/* 0x002B3AA0 */
s32 Room4E_Cmd02(void) {
    Effect_New(gEffects, 0x10, effect_78BE0_init);
    return 1;
}

/* 0x002B3B70 */
s32 Room4E_Cmd01(void *self, void *a1, u8 *cmd) {
    return var0_anim(cmd, pstr_Curtain_2);
}

/* room 0x4E (D_0040B4D8): a lit quad at x -44 .. -36, z 60, from 54 to 71 */
/* 0x002B3D80 */
s32 Room4E_Cmd00(void *self, void *a1, u8 *cmd) {
    static const u32 sQuad[16] = {
        0xC2300000, 0x428E0000, 0x42700000, 0x3F800000, 0xC2100000, 0x428E0000, 0x42700000, 0x3F800000,
        0xC2300000, 0x42580000, 0x42700000, 0x3F800000, 0xC2100000, 0x42580000, 0x42700000, 0x3F800000,
    };

    return lit_quad(cmd, sQuad, 0x80);
}
