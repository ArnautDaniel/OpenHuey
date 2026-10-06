/* Room 0xD8: its event handler class (vtable D_0047A270, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A270[];
extern u8 D_0047B090[], D_0047B098[], D_0047B0A0[];
extern s32 D_0047B2E8;
extern char D_0047B2E0[];
extern const char D_00463730[];

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

extern u8 D_00447BD0[];
extern u8 D_00447BF0[];
extern u8 D_00447CF0[];
extern u8 D_00447DA0[];
extern u8 D_00447E60[];
extern PTMF D_01991C50[];
extern PTMF D_01991C70[];

void *func_0036F930(void *o, s32 flags) { return room_dtor(o, flags, D_0047A270, D_0046DB80); }

void *func_0036F990(void) {
    return D_00447BD0;
}

void *func_0036F9A0(void) {
    return D_00447BF0;
}

void *func_0036F9B0(void) {
    return D_00447CF0;
}

void *func_0036F9C0(void *o) { return D_0047B090; }   /* D_0047A270 +0x14 */

void *func_0036F9D0(void) {
    return D_00447DA0;
}

void *func_0036F9E0(void *o) { return D_0047B098; }   /* D_0047A270 +0x20 */

u32 func_0036F9F0(void *o, s32 i) { return ((u32 *)D_0047B0A0)[i]; }   /* D_0047A270 +0x24 */

void *func_0036FA10(void) {
    return D_00447E60;
}

/* (self->*D_01991C70[i])(a, b) */
s32 func_0036FA20(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991C70[i & 0xFF], a, b);
}

s32 func_0036FA50(void) { return clock_draw(D_0047B2E0, D_00463730); }

/* as func_002E7560 */
s32 func_0036FBA0(void *self, void *a1, u8 *cmd) { return room_nudge(&D_0047B2E8, cmd, 1.0f); }

/* (self->*D_01991C50[i])(a, b) */
s32 func_0036FC40(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991C50[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0036FC70(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (as func_002E7880) last frame's noise requests (gProgress +0x10D4) of kind 0xD8 / 0xD7 and
   loudness 0x20 or more */
s32 func_0036FC90(void) {
    u8 *e = (u8 *)gProgress + 0x10D4;
    s32 i;

    for (i = 0; i < 4; i++, e += 16) {
        if (F(e, 0x4, s32) == 0xD8 && e[0] >= 0x20) {
            return 1;
        }
    }
    return 0;
}
