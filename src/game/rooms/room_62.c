/* Room 0x62: its event handler class (vtable Room62_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "scene_game_members.h"
#include "snd_place.h"
#include "msl.h"
#include "sce/libvu0.h"

extern void *RoomBase_vtable[];
extern void *Room62_vtable[];
extern f32 D_0047B280;   /* room 0x62: the dropped thing's fall speed */

extern u8 D_00421C60[];
extern u8 D_00421CD0[];
extern u8 D_00421D50[];
extern u8 D_00421EF0[];
extern u8 D_00421FC0[];
extern void *D_004222D0[];
extern u8 D_00422320[];

extern PTMF D_019910B0[];

/* 0x00308AF0 */
void *Room62_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room62_vtable, RoomBase_vtable); }

/* 0x00308B50 */
void *Room62_EnterScript(void) {
    return D_00421C60;
}

/* 0x00308B60 */
void *Room62_CharEnterScript(void) {
    return D_00421CD0;
}

/* 0x00308B70 */
void *Room62_Phase1Script(void) {
    return D_00421D50;
}

/* 0x00308B80 */
void *Room62_Phase2Script(void) {
    return D_00421EF0;
}

/* 0x00308B90 */
void *Room62_Phase3Script(void) {
    return D_00421FC0;
}

/* 0x00308BA0 */
void *Room62_ActionScript(void *self, s32 i) {
    return D_004222D0[i];
}

/* 0x00308BC0 */
void *Room62_Table38(void) {
    return D_00422320;
}

/* 0x00308BD0 */
void *Room62_ObjectName(void *self, s32 i) {
    return (void *)D_0047AD08[i];
}

/* (self->*D_019910B0[i])(a, b) */
/* 0x00308BF0 */
s32 Room62_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019910B0[i & 0xFF], a, b);
}

/* room 0x62 (D_00422318): the room's effect 0 dropped by byte 3 - 0 at rest (speed 0), 1 raised
 * by 0.5; else it falls (gravity 0.5 a frame, turning 0.16) and bounces off 0.7 losing 70%
 * (a sound each bounce) until slower than 0.2 (+0x74 set: landed; 1), else still going (2) */
/* 0x00308C20 */
s32 Room62_Cmd01(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kBounce = {0xBE99999A};   /* -0.3 */
    u8 *e = RoomEffects_Get(gRoomEffects, 0);
    f32 v, y;

    switch (cmd[3]) {
    case 0:
        D_0047B280 = 0.0f;
        return 1;
    case 1:
        AT(e, 0x28, f32) = AT(e, 0x28, f32) + 0.5f;
        return 1;
    case 2:
        AT(e, 0x74, s32) = 0;
        break;
    default:
        AT(e, 0x74, s32) = 0;
        break;
    }
    AT(e, 0x28, f32) = AT(e, 0x28, f32) + 0x1.47ae14p-3f /* 0.16 */;
    v = D_0047B280 - 0.5f;
    D_0047B280 = v;
    y = AT(e, 0x24, f32) + v;
    AT(e, 0x24, f32) = y;
    if (y < 0x1.666666p-1f /* 0.7 */) {
        f32 at[4] __attribute__((aligned(16)));

        AT(e, 0x24, f32) = 0x1.666666p-1f;
        D_0047B280 = v * kBounce.f;
        sceVu0CopyVector(at, (f32 *)(e + 0x20));
        Sound_PlayBankAt(gSound, 0, 6, at, 0, 0);
        if (D_0047B280 < 0x1.99999ap-3f /* 0.2 */) {
            AT(e, 0x74, s32) = 1;
            return 1;
        }
    }
    return 2;
}

/* room 0x62 (D_00422300): object byte 4 swings: byte 3 0 starts it (phase +0x30 0, size +0x34
 * 0.01; object 0 with sound 7), else a step (+0x10 = size x sin(phase), phase on 60 degrees,
 * the size down 0.001); 2 until it is still */
/* 0x00308D70 */
s32 Room62_Cmd00(void *self, void *a1, u8 *cmd) {
    u8 *o = room_obj(D_0047AD08[cmd[4]]);

    if (cmd[3] == 0) {
        AT(o, 0x30, f32) = 0.0f;
        AT(o, 0x34, u32) = 0x3C23D70A;   /* 0.01 */
        if (cmd[4] == 0) {
            Sound_PlayBankAt(gSound, 7, 6, (f32 *)(o + 0x20), 0, 0);
        }
        return 1;
    }
    AT(o, 0x10, f32) = AT(o, 0x34, f32) * func_0031C248(0x1.921fb6p+2f /* 2 pi */ * AT(o, 0x30, f32) / 360.0f);
    AT(o, 0x30, f32) = AT(o, 0x30, f32) + 60.0f;
    AT(o, 0x34, f32) = AT(o, 0x34, f32) - 0x1.0624de0000000p-10f /* 0.001 */;
    return AT(o, 0x34, f32) <= 0.0f ? 1 : 2;
}
