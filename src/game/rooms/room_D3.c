/* Room 0xD3: its event handler class (vtable D_0047A130, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A130[];
extern u8 D_0047B058[];
extern u8 D_0047B060[], D_0047B068[];
extern char D_0047B2A8[];
extern const char D_004636F0[];

extern u8 D_00446F30[];
extern u8 D_00447030[];
extern u8 D_004470D0[];
extern u8 D_00447110[];
extern PTMF D_01991B48[];
extern PTMF D_01991B58[];

void *func_0036E550(void *o, s32 flags) { return room_dtor(o, flags, D_0047A130, D_0046DB80); }

void *func_0036E5B0(void *o) { return D_0047B058; }   /* D_0047A130 +0xC */

void *func_0036E5C0(void) {
    return D_00446F30;
}

void *func_0036E5D0(void) {
    return D_00447030;
}

void *func_0036E5E0(void *o) { return D_0047B060; }   /* D_0047A130 +0x14 */

void *func_0036E5F0(void) {
    return D_004470D0;
}

u32 func_0036E600(void *o, s32 i) { return ((u32 *)D_0047B068)[i]; }   /* D_0047A130 +0x24 */

void *func_0036E620(void) {
    return D_00447110;
}

/* (self->*D_01991B58[i])(a, b) */
s32 func_0036E630(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B58[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0036E660(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991B48[i])(a, b) */
s32 func_0036E680(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B48[i & 0xFF], a, b);
}

s32 func_0036E6B0(void) { return clock_draw(D_0047B2A8, D_004636F0); }
