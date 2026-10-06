/* Room 0xE6: its event handler class (vtable D_0047A5D0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A5D0[];
extern u8 D_0047B100[], D_0047B108[], D_0047B110[];
extern char D_0047B328[];
extern const char D_00463A18[];

extern u8 D_004498F0[];
extern u8 D_00449910[];
extern u8 D_00449990[];
extern u8 D_004499E0[];
extern u8 D_00449AC0[];
extern PTMF D_01991DF0[];
extern PTMF D_01991E08[];

void *func_0037A2D0(void *o, s32 flags) { return room_dtor(o, flags, D_0047A5D0, D_0046DB80); }

void *func_0037A330(void) {
    return D_004498F0;
}

void *func_0037A340(void) {
    return D_00449910;
}

void *func_0037A350(void) {
    return D_00449990;
}

void *func_0037A360(void) {
    return D_004499E0;
}

u32 func_0037A370(void *o, s32 i) { return ((u32 *)D_0047B108)[i]; }   /* D_0047A5D0 +0x24 */

void *func_0037A390(void *o) { return D_0047B100; }   /* D_0047A5D0 +0x14 */

void *func_0037A3A0(void) {
    return D_00449AC0;
}

u32 func_0037A3B0(void *o, s32 i) { return ((u32 *)D_0047B110)[i]; }   /* D_0047A5D0 +0x34 */

/* (self->*D_01991E08[i])(a, b) */
s32 func_0037A3D0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991E08[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0037A400(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991DF0[i])(a, b) */
s32 func_0037A420(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991DF0[i & 0xFF], a, b);
}

s32 func_0037A450(void) { return clock_draw(D_0047B328, D_00463A18); }

/* the room object named by D_0047B110[0]: +0x24 -25.3, +0x34 0 */
s32 func_0037A5A0(void) {
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, *(const char **)D_0047B110);

    if (o != NULL) {
        AT(o, 0x24, u32) = 0xC1CA6666;   /* -25.3 */
        AT(o, 0x34, s32) = 0;
    }
    return 1;
}
