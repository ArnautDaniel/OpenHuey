/* Room 0x91: its event handler class (vtable D_00477480, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00477480[];

extern u32 D_00436600[];
extern u32 D_00436690[];
extern u32 D_00436780[];
extern u32 D_00436A00[];
extern u32 D_00436A40[];
extern u32 D_00436C80[];
extern u32 D_00436CE0[];

extern PTMF D_019917F0[];

void *func_00341580(void *o, s32 flags) { return room_dtor(o, flags, D_00477480, D_0046DB80); }

void *func_003415E0(void) {
    return D_00436600;
}

void *func_003415F0(void) {
    return D_00436690;
}

void *func_00341600(void) {
    return D_00436780;
}

void *func_00341610(void) {
    return D_00436A00;
}

void *func_00341620(void) {
    return D_00436A40;
}

u32 func_00341630(void *self, s32 i) {
    return D_00436C80[i];
}

void *func_00341650(void) {
    return D_00436CE0;
}

/* (self->*D_019917F0[i])(a, b) */
s32 func_00341660(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019917F0[i & 0xFF], a, b);
}

s32 func_00341690(void) { return slam_shake(); }

s32 func_00341720(void) {
    static const s16 spot[3] = {1, 3, 0xB};
    static const f32 b[3] = {0.0f, 0.0f, 0.0f}, c[3] = {0.0f, 0.0f, 0.5f}, d[3] = {0.5f, 0.5f, 0.5f};

    return grey_three(70.0f, spot, b, c, d);
}

/* script variable 0 down by the player's hit (1 from the weak blow 0x1A, else 5), not below 0 */
s32 func_00341A40(void) {
    VObject *ev = gEvents;
    s32 v = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0);

    v -= AT(gCharPlayer, 0xFC, s32) == 0x1A ? 1 : 5;
    if (v < 0) {
        v = 0;
    }
    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 0, v);
    return 1;
}

/* room 0x91 (D_00436CA0): door 0's +0x68 (0) */
s32 func_00341AD0(void) {
    VCALL(gDoors, 0x68, void (*)(VObject *, s32, s32))(gDoors, 0, 0);
    return 1;
}
