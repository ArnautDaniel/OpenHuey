/* Room 0x31: its event handler class (vtable Room31_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *RoomBase_vtable[];
extern void *Room31_vtable[];
extern void *TvScreenA_vtable[];

extern u8 D_00429E40[];
extern u8 D_00429E60[];
extern u8 D_00429F10[];
extern void *D_0042A0A0[];
extern void *D_0042A0E0[];

extern PTMF D_01991570[];

/* 0x0031E1C0 */
void *Room31_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room31_vtable, RoomBase_vtable); }

/* 0x0031E220 */
void *Room31_EnterScript(void) {
    return D_00429E40;
}

/* 0x0031E230 */
void *Room31_CharEnterScript(void) {
    return D_00429E60;
}

/* 0x0031E240 */
void *Room31_Phase1Script(void) {
    return D_00429F10;
}

/* 0x0031E250 */
void *Room31_ActionScript(void *self, s32 i) {
    return D_0042A0A0[i];
}

/* 0x0031E270 */
void *Room31_ObjectName(void *self, s32 i) {
    return D_0042A0E0[i];
}

/* (self->*D_01991570[i])(a, b) */
/* 0x0031E290 */
s32 Room31_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991570[i & 0xFF], a, b);
}

/* (as RoomE0_WindowLight) the window quad lit with flags 0x40 */
/* 0x0031E2C0 */
s32 Room31_WindowLight(void *self, void *a1, u8 *cmd) {
    return lit_quad(cmd, sQuadWindow, 0x40);
}

/* room 0x31 (D_0042A0C8): the fan turns, except while a movie plays (gProgress +0x54) */
/* 0x0031E460 */
s32 Room31_Fan(void) {
    if (VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress) != 0) {
        return 1;
    }
    fan_turn(D_0042A0E8);
    return 1;
}

/* rooms 0x31 / 0x32 (D_0042A0B8, D_0042C298): the room's effect 1 made anew as TvScreenA_vtable */
/* 0x0031E510 */
s32 Room31_RoomEffect(void) {
    room_effect_slot_new(gRoomEffects, 1, TvScreenA_vtable);
    return 1;
}
