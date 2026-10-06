/* Room 0xC1: its event handler class (vtable D_004785B0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"
#include "stalker_progress.h"

extern void *D_0046DB80[];
extern void *D_004785B0[];

extern u32 D_0043EC80[];
extern u32 D_0043ECC0[];
extern u32 D_0043ED80[];
extern u32 D_0043EF00[];
extern u32 D_0043EF40[];
extern u32 D_0043F0B0[];
extern u32 D_0047AEF8[];
extern u32 D_0047AF00[];

extern PTMF D_01991978[];

void *func_0034A360(void *o, s32 flags) { return room_dtor(o, flags, D_004785B0, D_0046DB80); }

void *func_0034A3C0(void) {
    return D_0043EC80;
}

void *func_0034A3D0(void) {
    return D_0043ECC0;
}

void *func_0034A3E0(void) {
    return D_0043ED80;
}

void *func_0034A3F0(void) {
    return D_0043EF00;
}

void *func_0034A400(void) {
    return D_0043EF40;
}

u32 func_0034A410(void *self, s32 i) {
    return D_0047AEF8[i];
}

void *func_0034A430(void) {
    return D_0043F0B0;
}

u32 func_0034A440(void *self, s32 i) {
    return D_0047AF00[i];
}

/* (self->*D_01991978[i])(a, b) */
s32 func_0034A460(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991978[i & 0xFF], a, b);
}

s32 func_0034A490(void) {
    return func_002EC410((u8 *)gProgress + 0x764) == 0;
}
