/* Room 0xE0: its event handler class (vtable D_0047A450, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "actor.h"

extern void *D_0046DB80[];
extern void *D_0047A450[];
extern u8 D_0047B0B0[], D_0047B0C0[];
extern char D_0047B2F8[];
extern const char D_00463928[];

extern u8 D_004480B0[];
extern u8 D_004480F0[];
extern u8 D_00448200[];
extern u8 D_00448320[];
extern u8 D_00448340[];
extern void *D_004484E0[];
extern u8 D_00448570[];
extern PTMF D_01991CD0[];
extern PTMF D_01991D18[];

void *func_00378870(void *o, s32 flags) { return room_dtor(o, flags, D_0047A450, D_0046DB80); }

void *func_003788D0(void) {
    return D_004480B0;
}

void *func_003788E0(void) {
    return D_004480F0;
}

void *func_003788F0(void) {
    return D_00448200;
}

void *func_00378900(void *o) { return D_0047B0B0; }   /* D_0047A450 +0x14 */

void *func_00378910(void) {
    return D_00448320;
}

void *func_00378920(void *self, s32 i) {
    return D_004484E0[i];
}

void *func_00378940(void) {
    return D_00448340;
}

void *func_00378950(void) {
    return D_00448570;
}

u32 func_00378960(void *o, s32 i) { return ((u32 *)D_0047B0C0)[i]; }   /* D_0047A450 +0x34 */

/* (self->*D_01991D18[i])(a, b) */
s32 func_00378980(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991D18[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_003789B0(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

/* (self->*D_01991CD0[i])(a, b) */
s32 func_003789D0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991CD0[i & 0xFF], a, b);
}

/* (as func_0030F2B0)  room 0x48 (D_00426830): the player's func_00124F20(0) */
s32 func_00378A00(void) {
    func_00124F20(gCharPlayer, 0);
    return 1;
}

s32 func_00378A30(void) { return clock_draw(D_0047B2F8, D_00463928); }

/* (as func_002AF300)  room 0x21 (D_00400BC8) */
s32 func_00378B80(void *self, void *a1, u8 *cmd) {
    return lit_quad_in(0x1A, cmd, sQuadDoor, 0x20000040);
}

/* (as func_002AF4E0)  room 0x21 (D_00400BA8) */
s32 func_00378D20(void *self, void *a1, u8 *cmd) {
    return lit_quad(cmd, sQuadWindow, 0x10000040);
}

/* (as func_0036A400)  room 0x37 (D_004469A0): the fan turns, except while a movie plays */
s32 func_00378EC0(void) {
    if (VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress) != 0) {
        return 1;
    }
    fan_turn(*(const char *const *)D_0047B0C0);   /* (the first of its names) */
    return 1;
}

s32 func_00378F70(void) { return clock_start(); }
