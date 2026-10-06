/* Room 0x4B: its event handler class (vtable Room4B_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room4B_vtable[];
extern const char *D_00409940;

extern u8 D_00408B90[];
extern u8 D_00408CD0[];
extern u8 D_00408E50[];
extern u8 D_004090C0[];
extern u8 D_004092E0[];
extern u8 D_004092C0[];
extern u32 D_004098D0[];

extern u8 D_00409950[];
extern u32 D_00409938[];

extern PTMF D_01990CB0[];
extern PTMF D_01990CC8[];

/* 0x002B2FF0 */
void *Room4B_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room4B_vtable, RoomBase_vtable); }

/* 0x002B3050 */
void *Room4B_EnterScript(void) {
    return D_00408B90;
}

/* 0x002B3060 */
void *Room4B_CharEnterScript(void) {
    return D_00408CD0;
}

/* 0x002B3070 */
void *Room4B_Phase1Script(void) {
    return D_00408E50;
}

/* 0x002B3080 */
void *Room4B_Phase2Script(void) {
    return D_004090C0;
}

/* 0x002B3090 */
void *Room4B_Phase3Script(void) {
    return D_004092E0;
}

/* 0x002B30A0 */
void *Room4B_Phase5Script(void) {
    return D_004092C0;
}

/* 0x002B30B0 */
u32 Room4B_ActionScript(void *self, s32 i) {
    return D_004098D0[i];
}

/* 0x002B30D0 */
void *Room4B_Table38(void) {
    return D_00409950;
}

/* 0x002B30E0 */
u32 Room4B_ObjectName(void *self, s32 i) {
    return D_00409938[i];
}

/* (self->*D_01990CC8[i])(a, b) */
/* 0x002B3100 */
s32 Room4B_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990CC8[i & 0xFF], a, b);
}

/* 1 unless the object at +0x18 exists and its byte +0x28 is 1. */
/* 0x002B3130 */
s32 Room4B_Cond00(void) {
    u8 *p = *(u8 **)(gCreatures + 0x18);

    return !(p != NULL && p[0x28] == 1);
}

/* (self->*D_01990CB0[i])(a, b) */
/* 0x002B3170 */
s32 Room4B_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990CB0[i & 0xFF], a, b);
}

/* 0x002B31A0 */
s32 Room4B_Cmd01(void *self, void *a1, u8 *cmd) {
    return var0_anim(cmd, D_00409940);
}

/* room 0x4B (D_00409910): a lit quad at x -63.65 .. -55.65, z 104.5, from 4 to 21 */
/* 0x002B33B0 */
s32 Room4B_Cmd00(void *self, void *a1, u8 *cmd) {
    static const u32 sQuad[16] = {
        0xC27E999A, 0x41A80000, 0x42D10000, 0x3F800000, 0xC25E999A, 0x41A80000, 0x42D10000, 0x3F800000,
        0xC27E999A, 0x40800000, 0x42D10000, 0x3F800000, 0xC25E999A, 0x40800000, 0x42D10000, 0x3F800000,
    };

    return lit_quad(cmd, sQuad, 0x80);
}
