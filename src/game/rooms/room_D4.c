/* Room 0xD4: its event handler class (vtable D_0047A170, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A170[];
extern u8 D_0047B06C[];
extern char D_0047B2B0[];
extern const char D_004636F8[];

extern u8 D_00447130[];
extern u8 D_00447230[];
extern u8 D_004472B0[];
extern u8 D_004472F0[];
extern PTMF D_01991B68[];
extern PTMF D_01991B78[];

void *func_0036E800(void *o, s32 flags) { return room_dtor(o, flags, D_0047A170, D_0046DB80); }

void *func_0036E860(void *o) { return D_0047B06C; }   /* D_0047A170 +0xC */

void *func_0036E870(void) {
    return D_00447130;
}

void *func_0036E880(void) {
    return D_00447230;
}

void *func_0036E890(void) {
    return D_004472B0;
}

void *func_0036E8A0(void) {
    return D_004472F0;
}

/* (self->*D_01991B78[i])(a, b) */
s32 func_0036E8B0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B78[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0036E8E0(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991B68[i])(a, b) */
s32 func_0036E900(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B68[i & 0xFF], a, b);
}

s32 func_0036E930(void) { return clock_draw(D_0047B2B0, D_004636F8); }
