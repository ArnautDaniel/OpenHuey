/* Room 0x52: its event handler class (vtable D_0046E840, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046E840[];
extern void *D_00476BF0[];
extern u8 D_0040E3B0[];
extern u8 D_0040E480[];
extern u8 D_0040E550[];
extern u8 D_0040E680[];
extern u32 D_0040EC60[];
extern u8 D_0040ECF0[];
extern u32 D_0040ECA0[];

extern PTMF D_01990DA8[];
extern PTMF D_01990DB8[];

static inline void effect476bf0_init(void **o) {
    o[0] = D_00476BF0;
}

void *func_002B4A70(void *o, s32 flags) { return room_dtor(o, flags, D_0046E840, D_0046DB80); }

void *func_002B4AD0(void) {
    return D_0040E3B0;
}

void *func_002B4AE0(void) {
    return D_0040E480;
}

void *func_002B4AF0(void) {
    return D_0040E550;
}

void *func_002B4B00(void) {
    return D_0040E680;
}

u32 func_002B4B10(void *self, s32 i) {
    return D_0040EC60[i];
}

void *func_002B4B30(void) {
    return D_0040ECF0;
}

/* (self->*D_01990DB8[i])(a, b) */
s32 func_002B4B40(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990DB8[i & 0xFF], a, b);
}

/* 1 unless the object at +0x18 exists and its byte +0x28 is 1. */
s32 func_002B4B70(void) {
    u8 *p = *(u8 **)(gCreatures + 0x18);

    return !(p != NULL && p[0x28] == 1);
}

/* (self->*D_01990DA8[i])(a, b) */
s32 func_002B4BB0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990DA8[i & 0xFF], a, b);
}

s32 func_002B4BE0(void) {   /* a scene effect (D_00476BF0, 0x6E0 bytes) */
    Effect_New(gEffects, 0x6E0, effect476bf0_init);
    return 1;
}

u32 func_002B4CB0(void *self, s32 i) {
    return D_0040ECA0[i];
}
