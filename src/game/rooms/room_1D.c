/* Room 0x1D: its event handler class (vtable D_0046E1C0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E1C0[];
extern u8 D_0047AA80[];
extern s32 func_00178980(Progress *p, s32 room, s32 exit);   /* the door at that exit is open */

extern u8 D_003FD370[];
extern u8 D_003FD420[];
extern u8 D_003FD470[];
extern u8 D_003FD580[];
extern u32 D_003FD950[];
extern u8 D_003FD660[];
extern u8 D_003FD9B0[];

extern PTMF D_019909B0[];
extern PTMF D_019909C8[];

void *func_002ADD10(void *o, s32 flags) { return room_dtor(o, flags, D_0046E1C0, D_0046DB80); }

void *func_002ADD70(void) {
    return D_003FD370;
}

void *func_002ADD80(void) {
    return D_003FD420;
}

void *func_002ADD90(void) {
    return D_003FD470;
}

void *func_002ADDA0(void) {
    return D_003FD580;
}

u32 func_002ADDB0(void *self, s32 i) {
    return D_003FD950[i];
}

void *func_002ADDD0(void) {
    return D_003FD660;
}

void *func_002ADDE0(void *o) { return D_0047AA80; }   /* D_0046E1C0 +0x20 */

void *func_002ADDF0(void) {
    return D_003FD9B0;
}

/* (self->*D_019909C8[i])(a, b) */
s32 func_002ADE00(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019909C8[i & 0xFF], a, b);
}

/* Fiona in move 5, room 0x1D's flag 0 not set and its exit 0's door shut */
s32 func_002ADE30(void) {
    Progress *p;

    if (AT(gCharPlayer, 0xFC, s32) != 5) {
        return 0;
    }
    p = gProgress;
    if (Progress_CurRoomFlag(p, 0x1D, 0) != 0 || func_00178980(p, 0x1D, 0) != 0) {
        return 0;
    }
    return 1;
}

/* (self->*D_019909B0[i])(a, b) */
s32 func_002ADEB0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019909B0[i & 0xFF], a, b);
}

/* room 0x1D (D_003FD990): door 0's +0x74 (0, or -0.08 by byte 3) */
s32 func_002ADEE0(void *self, void *a1, u8 *cmd) {
    VCALL(gDoors, 0x74, void (*)(VObject *, s32, f32))(gDoors, 0, cmd[3] == 0 ? 0.0f : -0x1.47ae140000000p-4f /* 0.08 */);
    return 1;
}

/* room 0x1D (D_003FD980): script variable 0 = 2 .. 5 at random */
s32 func_002ADF40(void) {
    s32 r = VCALL(gRandom, 0x10, s32 (*)(VObject *))(gRandom) & 3;

    VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 0, r + 2);
    return 1;
}
