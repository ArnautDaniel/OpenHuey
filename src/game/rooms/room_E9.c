/* Room 0xE9: its event handler class (vtable D_0047A690, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A690[];
extern u8 D_0047B134[];
extern char D_0047B340[];
extern const char D_00463A48[];

extern u8 D_0044A2C0[];
extern u8 D_0044A380[];
extern u8 D_0044A3C0[];
extern u8 D_0044A470[];
extern void *D_0044A928[];
extern u8 D_0044A990[];
extern PTMF D_01991E60[];
extern PTMF D_01991E90[];

void *func_0037AB80(void *o, s32 flags) { return room_dtor(o, flags, D_0047A690, D_0046DB80); }

void *func_0037ABE0(void) {
    return D_0044A2C0;
}

void *func_0037ABF0(void) {
    return D_0044A380;
}

void *func_0037AC00(void) {
    return D_0044A3C0;
}

void *func_0037AC10(void *o) { return D_0047B134; }   /* D_0047A690 +0x14 */

void *func_0037AC20(void) {
    return D_0044A470;
}

void *func_0037AC30(void *self, s32 i) {
    return D_0044A928[i];
}

void *func_0037AC50(void) {
    return D_0044A990;
}

/* (self->*D_01991E90[i])(a, b) */
s32 func_0037AC60(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991E90[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0037AC90(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991E60[i])(a, b) */
s32 func_0037ACB0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991E60[i & 0xFF], a, b);
}

s32 func_0037ACE0(void) { return clock_draw_saved(D_0047B340, D_00463A48); }

/* (as func_002A8BA0) the same four spots for bytes 3..6 */
s32 func_0037AFB0(void *self, void *a1, u8 *cmd) {
    return glow4_spot(cmd, 3);
}

s32 func_0037B210(void) { return clock_save(); }

s32 func_0037B290(void) { return clock_stop(); }
