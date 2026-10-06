/* Room 0xD1: its event handler class (vtable D_0047A0B0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A0B0[];
extern u8 D_0047B048[], D_0047B050[];
extern char D_0047B298[];
extern const char D_004636E0[];

extern u8 D_00446C00[];
extern u8 D_00446C10[];
extern u8 D_00446C90[];
extern u8 D_00446CF0[];
extern u8 D_00446D40[];
extern PTMF D_01991B00[];
extern PTMF D_01991B18[];

void *func_0036DF30(void *o, s32 flags) { return room_dtor(o, flags, D_0047A0B0, D_0046DB80); }

void *func_0036DF90(void) {
    return D_00446C00;
}

void *func_0036DFA0(void) {
    return D_00446C10;
}

void *func_0036DFB0(void) {
    return D_00446C90;
}

void *func_0036DFC0(void *o) { return D_0047B048; }   /* D_0047A0B0 +0x14 */

void *func_0036DFD0(void) {
    return D_00446CF0;
}

u32 func_0036DFE0(void *o, s32 i) { return ((u32 *)D_0047B050)[i]; }   /* D_0047A0B0 +0x24 */

void *func_0036E000(void) {
    return D_00446D40;
}

/* (self->*D_01991B18[i])(a, b) */
s32 func_0036E010(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B18[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0036E040(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991B00[i])(a, b) */
s32 func_0036E060(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991B00[i & 0xFF], a, b);
}

s32 func_0036E090(void) { return clock_draw(D_0047B298, D_004636E0); }

/* as func_002E6E20 */
s32 func_0036E1E0(void) {
    Effect_New(gEffects, 0xC0, effect_C0_init);
    return 1;
}
