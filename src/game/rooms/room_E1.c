/* Room 0xE1: its event handler class (vtable D_0047A490, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A490[];
extern u8 D_0047B0C8[], D_0047B0D0[];
extern char D_0047B300[];
extern const char D_00463930[];

extern u8 D_00448590[];
extern u8 D_004485E0[];
extern u8 D_00448660[];
extern u8 D_00448740[];
extern u8 D_004488A0[];
extern PTMF D_01991D28[];
extern PTMF D_01991D38[];

void *func_00378FE0(void *o, s32 flags) { return room_dtor(o, flags, D_0047A490, D_0046DB80); }

void *func_00379040(void) {
    return D_00448590;
}

void *func_00379050(void) {
    return D_004485E0;
}

void *func_00379060(void) {
    return D_00448660;
}

void *func_00379070(void *o) { return D_0047B0C8; }   /* D_0047A490 +0x14 */

void *func_00379080(void) {
    return D_00448740;
}

u32 func_00379090(void *o, s32 i) { return ((u32 *)D_0047B0D0)[i]; }   /* D_0047A490 +0x24 */

void *func_003790B0(void) {
    return D_004488A0;
}

/* (self->*D_01991D38[i])(a, b) */
s32 func_003790C0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991D38[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_003790F0(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991D28[i])(a, b) */
s32 func_00379110(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991D28[i & 0xFF], a, b);
}

s32 func_00379140(void) { return clock_draw(D_0047B300, D_00463930); }
