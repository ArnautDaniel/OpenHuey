/* Room 0x4E: its event handler class (vtable Room4E_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room4E_vtable[];
extern const char *D_0040B508;
extern void *D_00478BE0[];
extern u8 D_0040AE90[];
extern u8 D_0040AF40[];
extern u8 D_0040B040[];
extern u8 D_0040B160[];
extern u8 D_0040B200[];
extern u32 D_0040B4B0[];
extern u8 D_0040B510[];
extern u32 D_0040B500[];

extern PTMF D_01990D10[];

static void effect_78BE0_init(void **obj) {
    obj[0] = D_00478BE0;
}

/* 0x002B3970 */
void *Room4E_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room4E_vtable, RoomBase_vtable); }

/* 0x002B39D0 */
void *Room4E_EnterScript(void) {
    return D_0040AE90;
}

/* 0x002B39E0 */
void *Room4E_CharEnterScript(void) {
    return D_0040AF40;
}

/* 0x002B39F0 */
void *Room4E_Phase1Script(void) {
    return D_0040B040;
}

/* 0x002B3A00 */
void *Room4E_Phase2Script(void) {
    return D_0040B160;
}

/* 0x002B3A10 */
void *Room4E_Phase5Script(void) {
    return D_0040B200;
}

/* 0x002B3A20 */
u32 Room4E_ActionScript(void *self, s32 i) {
    return D_0040B4B0[i];
}

/* 0x002B3A40 */
void *Room4E_Table38(void) {
    return D_0040B510;
}

/* 0x002B3A50 */
u32 Room4E_ObjectName(void *self, s32 i) {
    return D_0040B500[i];
}

/* (self->*D_01990D10[i])(a, b) */
/* 0x002B3A70 */
s32 Room4E_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990D10[i & 0xFF], a, b);
}

/* room 0x4E (D_0040B4F8): the 0x10-byte effect D_00478BE0 made */
/* 0x002B3AA0 */
s32 Room4E_Cmd02(void) {
    Effect_New(gEffects, 0x10, effect_78BE0_init);
    return 1;
}

/* 0x002B3B70 */
s32 Room4E_Cmd01(void *self, void *a1, u8 *cmd) {
    return var0_anim(cmd, D_0040B508);
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
