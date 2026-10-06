/* Room 0x37: its event handler class (vtable D_00479FB0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00479FB0[];
extern u8 D_0047B028[];
extern const char *D_0047B02C;   /* "fan" (room 0x37) */

extern u8 D_00445BC0[];
extern u8 D_00445C20[];
extern u8 D_00445C70[];
extern u8 D_00445C90[];
extern u8 D_00445D70[];
extern void *D_00446960[];
extern PTMF D_01991AB0[];

/* 0x0036A300 */
void *Room37_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00479FB0, D_0046DB80); }

/* 0x0036A360 */
void *Room37_EnterScript(void) {
    return D_00445BC0;
}

/* 0x0036A370 */
void *Room37_CharEnterScript(void) {
    return D_00445C20;
}

/* 0x0036A380 */
void *Room37_Phase1Script(void) {
    return D_00445C70;
}

/* 0x0036A390 */
void *Room37_Phase2Script(void) {
    return D_00445C90;
}

/* 0x0036A3A0 */
void *Room37_Phase3Script(void) {
    return D_00445D70;
}

/* 0x0036A3B0 */
void *Room37_ActionScript(void *self, s32 i) {
    return D_00446960[i];
}

/* (self->*D_01991AB0[i])(a, b) */
/* 0x0036A3D0 */
s32 Room37_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991AB0[i & 0xFF], a, b);
}

/* room 0x37 (D_004469A0): the fan turns, except while a movie plays */
s32 func_0036A400(void) {
    if (VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress) != 0) {
        return 1;
    }
    fan_turn(D_0047B02C);
    return 1;
}

/* 0x0036A4B0 */
u32 Room37_ObjectName(void *o, s32 i) { return ((u32 *)D_0047B028)[i]; }   /* D_00479FB0 +0x34 */
