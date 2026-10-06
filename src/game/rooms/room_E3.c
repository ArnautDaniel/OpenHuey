/* Room 0xE3: its event handler class (vtable D_0047A510, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A510[];
extern u8 D_0047B0E0[], D_0047B0E4[];
extern char D_0047B310[];
extern const char D_00463958[];

extern u8 D_00448D20[];
extern u8 D_00448DA0[];
extern u8 D_00448E30[];
extern u8 D_00448F30[];
extern u8 D_00449010[];
extern void *D_00449210[];
extern u8 D_00449240[];
extern PTMF D_01991D68[];
extern PTMF D_01991D78[];

void *func_00379540(void *o, s32 flags) { return room_dtor(o, flags, D_0047A510, D_0046DB80); }

void *func_003795A0(void) {
    return D_00448D20;
}

void *func_003795B0(void) {
    return D_00448DA0;
}

void *func_003795C0(void) {
    return D_00448E30;
}

void *func_003795D0(void *o) { return D_0047B0E0; }   /* D_0047A510 +0x14 */

void *func_003795E0(void *self, s32 i) {
    return D_00449210[i];
}

void *func_00379600(void) {
    return D_00448F30;
}

void *func_00379610(void) {
    return D_00449010;
}

void *func_00379620(void) {
    return D_00449240;
}

u32 func_00379630(void *o, s32 i) { return ((u32 *)D_0047B0E4)[i]; }   /* D_0047A510 +0x34 */

/* (self->*D_01991D78[i])(a, b) */
s32 func_00379650(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991D78[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_00379680(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991D68[i])(a, b) */
s32 func_003796A0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991D68[i & 0xFF], a, b);
}

s32 func_003796D0(void) { return clock_draw(D_0047B310, D_00463958); }
