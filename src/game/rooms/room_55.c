/* Room 0x55: its event handler class (vtable D_00471020, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "scene_game_members.h"

extern void *D_0046DB80[];
extern void *D_00471020[];
extern void *D_00477E10[], *D_00479400[];
extern u8 D_00420B40[];
extern u8 D_00420B80[];
extern u8 D_00420CD0[];
extern u8 D_00420D80[];
extern void *D_004210D0[];
extern void *D_00421100[];
extern u8 D_00421110[];

extern PTMF D_01991088[];

static void effect_77E10_init(void **obj) {
    obj[0] = D_00477E10;
}

static void effect_79400_init(void **obj) {
    obj[0] = D_00479400;
    obj[0x610 / 4] = D_00469D00;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = D_0046FC30;
}

void *func_00305E90(void *o, s32 flags) { return room_dtor(o, flags, D_00471020, D_0046DB80); }

void *func_00305EF0(void) {
    return D_00420B40;
}

void *func_00305F00(void) {
    return D_00420B80;
}

void *func_00305F10(void) {
    return D_00420CD0;
}

void *func_00305F20(void) {
    return D_00420D80;
}

void *func_00305F30(void *self, s32 i) {
    return D_004210D0[i];
}

void *func_00305F50(void) {
    return D_00421110;
}

void *func_00305F60(void *self, s32 i) {
    return D_00421100[i];
}

/* (self->*D_01991088[i])(a, b) */
s32 func_00305F80(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991088[i & 0xFF], a, b);
}

/* room 55 (D_004210F8): the 0x18-byte effect D_00477E10 started with 0 or 1 by byte 3; 0 also
 * lays a floor quad (effect 0x1B: 60 x 60 at height 45), else the 0x6D0-byte effect D_00479400
 * is made too */
s32 func_00305FB0(void *self, void *a1, u8 *cmd) {
    u8 *mgr = gEffects;
    s32 slot = Effect_New(mgr, 0x18, effect_77E10_init);
    s32 on;

    if (cmd[3] == 0) {
        u8 *fx;
        u32 q[20] __attribute__((aligned(16)));

        on = 0;
        fx = gRoomEffects;
        room_effect_slot_new(fx, 0x1B, D_00472F60);
        q[0] = 0x41F00000;   /* (30, 45, -30) */
        q[1] = 0x42340000;
        q[2] = 0xC1F00000;
        q[3] = 0x3F800000;
        q[4] = 0xC1F00000;   /* (-30, 45, -30) */
        q[5] = 0x42340000;
        q[6] = 0xC1F00000;
        q[7] = 0x3F800000;
        q[8] = 0x41F00000;   /* (30, 45, 30) */
        q[9] = 0x42340000;
        q[10] = 0x41F00000;
        q[11] = 0x3F800000;
        q[12] = 0xC1F00000;  /* (-30, 45, 30) */
        q[13] = 0x42340000;
        q[14] = 0x41F00000;
        q[15] = 0x3F800000;
        q[16] = 0;
        q[17] = 0x80;
        q[18] = 0x3F800000;
        func_00266C70(fx, 0x1B, q);
    } else {
        on = 1;
        Effect_New(mgr, 0x6D0, effect_79400_init);
    }
    func_002D6090(mgr, slot, &on);
    return 1;
}
