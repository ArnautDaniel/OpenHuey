/* Room 0x0C: its event handler class (vtable D_0046DE00, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "renderer.h"
#include "scene_game_members.h"
#include "msl.h"
#include "sce/libvu0.h"

extern void *D_0046DB80[];
extern void *D_0046DE00[];
extern void *D_0046EC60[];

extern u8 D_003F4650[];
extern u8 D_003F46E0[];
extern u8 D_003F4720[];
extern u8 D_003F4850[];
extern void *D_003F53B0[];
extern PTMF D_01990860[];

void *func_002AB1F0(void *o, s32 flags) { return room_dtor(o, flags, D_0046DE00, D_0046DB80); }

void *func_002AB250(void) {
    return D_003F4650;
}

void *func_002AB260(void) {
    return D_003F46E0;
}

void *func_002AB270(void) {
    return D_003F4720;
}

void *func_002AB280(void) {
    return D_003F4850;
}

void *func_002AB290(void *self, s32 i) {
    return D_003F53B0[i];
}

void *func_002AB2B0(void *self, s32 i) {
    return (void *)D_003F5440[i];
}

/* (self->*D_01990860[i])(a, b) */
s32 func_002AB2D0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990860[i & 0xFF], a, b);
}

/* room 0x0C (D_003F5428): three objects (pair byte 4) swing: byte 3 0 set up (rest +0x30, phase
 * +0x34 half a turn apart, swing +0x3C 0.75 / 0.5), 1 a step (phase on 60 degrees, the swing
 * down 0.1, x = rest + swing x sin(phase); 2 once still), 2 all to x 10. 2 while any swings */
s32 func_002AB300(void *self, void *a1, u8 *cmd) {
    s32 moving = 0;
    s32 i;

    for (i = 0; i < 3; i++) {
        u8 *o = room_obj(D_003F5440[cmd[4] + i * 2 + 13]);
        f32 a;

        switch (cmd[3]) {
        case 0:
            sceVu0CopyVector((f32 *)(o + 0x30), (f32 *)(o + 0x20));
            AT(o, 0x34, f32) = 0.5f * (f32)i;
            AT(o, 0x3C, f32) = 0.5f + 0.25f * (f32)(u32)(i != 2);
            break;
        case 1:
            a = AT(o, 0x34, f32) + 0x1.0c1524p+0f /* 60 degrees */;
            AT(o, 0x34, f32) = a;
            if (!(a <= 0x1.921fb6p+1f /* pi */)) {
                AT(o, 0x34, f32) = a - 0x1.921fb6p+2f /* 2 pi */;
            }
            AT(o, 0x3C, f32) = AT(o, 0x3C, f32) - 0x1.99999ap-4f /* 0.1 */;
            AT(o, 0x20, f32) = AT(o, 0x30, f32) + AT(o, 0x3C, f32) * func_0031C248(AT(o, 0x34, f32));
            if (!(AT(o, 0x3C, f32) <= 0.0f)) {
                moving = 1;
                break;
            }
            AT(o, 0x20, f32) = 10.0f;
            break;
        case 2:
            AT(o, 0x20, f32) = 10.0f;
            break;
        }
    }
    return moving ? 2 : 1;
}

/* room 0x0C (D_003F5418): the screen darkened as the cutscene runs past frame 0x4AE (32 a
 * frame, up to 0x80) */
s32 func_002AB510(void) {
    u32 a = (u32)(VCALL(gCutscene, 0x34, s32 (*)(VObject *))(gCutscene) - 0x4AE) << 5;

    if (a > 0x80) {
        a = 0x80;
    }
    func_001B9000(gRenderer, a << 24);
    return 1;
}

/* room 0x0C (D_003F5408): script variable byte 3 down by the player's hit (byte 4: 1 from the
 * weak blow 0x1A, else 5) or the pursuer's (+0x108), not below 0 */
s32 func_002AB580(void *self, void *a1, u8 *cmd) {
    VObject *ev = gEvents;
    s32 v = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, cmd[3]);
    s32 k;

    if (cmd[4] != 0) {
        k = AT(gCharPlayer, 0xFC, s32) == 0x1A ? 1 : 5;
    } else {
        k = VCALL((VObject *)gCharPursuer, 0x108, s32 (*)(VObject *))((VObject *)gCharPursuer);
    }
    if (k > 0) {
        v -= k;
        if (v < 0) {
            v = 0;
        }
        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, cmd[3], v);
    }
    return 1;
}

/* room 0x0C (D_003F53F8): room effect 0x1C (a depth range) widening with the cutscene from frame
 * 0x2D0: near 1 + 1.5 t (at most 46), far 64.4 + 1.7 t (at most 116.5) */
s32 func_002AB660(void) {
    f32 t = (f32)(VCALL(gCutscene, 0x34, s32 (*)(VObject *))(gCutscene) - 0x2D0);
    f32 r[4];
    f32 v;

    room_effect_slot_new(gRoomEffects, 0x1C, D_0046EC60);
    v = 1.0f + 1.5f * t;
    r[0] = v <= 46.0f ? v : 46.0f;
    r[1] = v <= 46.0f ? v : 46.0f;
    v = 0x1.019999ap+6f /* 64.4 */ + 0x1.b33334p+0f /* 1.7 */ * t;
    r[2] = v <= 116.5f ? v : 116.5f;
    r[3] = v <= 116.5f ? v : 116.5f;
    func_00266C70(gRoomEffects, 0x1C, r);
    return 1;
}
