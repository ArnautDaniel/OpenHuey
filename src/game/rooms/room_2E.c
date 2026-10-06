/* Room 0x2E: its event handler class (vtable D_00470EF0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "progress.h"
#include "scene_game_members.h"

extern void *D_0046DB80[];
extern void *D_00470EF0[];
extern u8 D_0047ACC4[];
extern void *D_0046EC60[];

extern u8 D_0041D2E0[];
extern u8 D_0041D330[];
extern u8 D_0041D3C0[];
extern u8 D_0041D4E0[];
extern void *D_0041D7A0[];
extern void *D_0041D808[];
extern PTMF D_01990FF0[];

/* 0x002FEF80 */
void *Room2E_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00470EF0, D_0046DB80); }

/* 0x002FEFE0 */
void *Room2E_EnterScript(void) {
    return D_0041D2E0;
}

/* 0x002FEFF0 */
void *Room2E_CharEnterScript(void) {
    return D_0041D330;
}

/* 0x002FF000 */
void *Room2E_Phase1Script(void) {
    return D_0041D3C0;
}

/* 0x002FF010 */
void *Room2E_Phase2Script(void *o) { return D_0047ACC4; }   /* D_00470EF0 +0x14 */

/* 0x002FF020 */
void *Room2E_Phase5Script(void) {
    return D_0041D4E0;
}

/* 0x002FF030 */
void *Room2E_ActionScript(void *self, s32 i) {
    return D_0041D7A0[i];
}

/* 0x002FF050 */
void *Room2E_ObjectName(void *self, s32 i) {
    return D_0041D808[i];
}

/* (self->*D_01990FF0[i])(a, b) */
/* 0x002FF070 */
s32 Room2E_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990FF0[i & 0xFF], a, b);
}

/* the pursuer's model's +0x9E8 by byte 3: 0 0.15, 1 0, else 0.05 */
/* 0x002FF0A0 */
s32 Room2E_Cmd04(void *self, void *a1, u8 *cmd) {
    u8 *m = AT(gCharacters[func_001770D0(gProgress, 0xFE) & 0xFF], 0xF0, u8 *);

    switch (cmd[3]) {
    case 0:
        AT(m, 0x9E8, f32) = 0x1.333334p-3f /* 0.15 */;
        break;
    case 1:
        AT(m, 0x9E8, s32) = 0;
        break;
    default:
        AT(m, 0x9E8, f32) = 0x1.99999ap-5f /* 0.05 */;
        break;
    }
    return 1;
}

/* the depth range (effect 0x1C) opening out with the cutscene from its frame 1260: 1 / 21 / 40
 * / 80 on by 0.4 a frame, up to 41 / 61 / 80 / 120 */
/* 0x002FF130 */
s32 Room2E_Cmd03(void) {
    u8 *fx = gRoomEffects;
    f32 t = (f32)(VCALL(gCutscene, 0x34, s32 (*)(VObject *))(gCutscene) - 1260);
    f32 r[4] __attribute__((aligned(16)));
    f32 d;

    room_effect_slot_new(fx, 0x1C, D_0046EC60);
    d = 0x1.999998p-2f /* 0.4 */ * t;
    r[0] = 1.0f + d;
    if (!(r[0] <= 41.0f)) {
        r[0] = 41.0f;
    }
    r[1] = 21.0f + d;
    if (!(r[1] <= 61.0f)) {
        r[1] = 61.0f;
    }
    r[2] = 40.0f + d;
    if (!(r[2] <= 80.0f)) {
        r[2] = 80.0f;
    }
    r[3] = 80.0f + d;
    if (!(r[3] <= 120.0f)) {
        r[3] = 120.0f;
    }
    func_00266C70(fx, 0x1C, r);
    return 1;
}

/* 0x002FF300 */
s32 Room2E_Cmd02(void) {
    return 1;
}

/* 0x002FF310 */
s32 Room2E_Cmd01(void) {
    return 1;
}

/* a lit quad at x -15.96, z -2 .. 6, height 111 / 91 */
/* 0x002FF320 */
s32 Room2E_Cmd00(void *self, void *a1, u8 *cmd) {
    static const u32 sQuad[16] = {
        0xC17F5810, 0x42DE0000, 0xC00001A3, 0x3F800000, 0xC17F5810, 0x42DE0000, 0x40BFFF2E, 0x3F800000,
        0xC17F5810, 0x42B60000, 0xC00001A3, 0x3F800000, 0xC17F5810, 0x42B60000, 0x40BFFF2E, 0x3F800000,
    };

    return lit_quad(cmd, sQuad, 0x80);
}
