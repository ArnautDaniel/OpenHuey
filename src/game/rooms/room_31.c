/* Room 0x31: its event handler class (vtable D_00473460, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00473460[];
extern void *D_0046EA40[];

extern u8 D_00429E40[];
extern u8 D_00429E60[];
extern u8 D_00429F10[];
extern void *D_0042A0A0[];
extern void *D_0042A0E0[];

extern PTMF D_01991570[];

void *func_0031E1C0(void *o, s32 flags) { return room_dtor(o, flags, D_00473460, D_0046DB80); }

void *func_0031E220(void) {
    return D_00429E40;
}

void *func_0031E230(void) {
    return D_00429E60;
}

void *func_0031E240(void) {
    return D_00429F10;
}

void *func_0031E250(void *self, s32 i) {
    return D_0042A0A0[i];
}

void *func_0031E270(void *self, s32 i) {
    return D_0042A0E0[i];
}

/* (self->*D_01991570[i])(a, b) */
s32 func_0031E290(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991570[i & 0xFF], a, b);
}

/* (as func_00378D20) the window quad lit with flags 0x40 */
s32 func_0031E2C0(void *self, void *a1, u8 *cmd) {
    return lit_quad(cmd, sQuadWindow, 0x40);
}

/* room 0x31 (D_0042A0C8): the fan turns, except while a movie plays (gProgress +0x54) */
s32 func_0031E460(void) {
    if (VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress) != 0) {
        return 1;
    }
    fan_turn(D_0042A0E8);
    return 1;
}

/* rooms 0x31 / 0x32 (D_0042A0B8, D_0042C298): the room's effect 1 made anew as D_0046EA40 */
s32 func_0031E510(void) {
    room_effect_slot_new(gRoomEffects, 1, D_0046EA40);
    return 1;
}
