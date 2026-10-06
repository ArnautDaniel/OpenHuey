/* Room 0xD2: its event handler class (vtable D_0047A0F0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A0F0[];
extern u8 D_0047B054[];
extern char D_0047B2A0[];
extern const char D_004636E8[];

extern u8 D_00446D60[];
extern u8 D_00446E60[];
extern u8 D_00446EE0[];
extern u8 D_00446F20[];
extern PTMF D_01991B28[];
extern PTMF D_01991B38[];

void *func_0036E2D0(void *o, s32 flags) { return room_dtor(o, flags, D_0047A0F0, D_0046DB80); }

void *func_0036E330(void *o) { return D_0047B054; }   /* D_0047A0F0 +0xC */

void *func_0036E340(void) {
    return D_00446D60;
}

void *func_0036E350(void) {
    return D_00446E60;
}

void *func_0036E360(void) {
    return D_00446EE0;
}

void *func_0036E370(void) {
    return D_00446F20;
}

/* (self->*D_01991B38[i])(a, b) */
s32 func_0036E380(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B38[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0036E3B0(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991B28[i])(a, b) */
s32 func_0036E3D0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B28[i & 0xFF], a, b);
}

s32 func_0036E400(void) { return clock_draw(D_0047B2A0, D_004636E8); }
