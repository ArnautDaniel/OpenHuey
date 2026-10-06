/* Room 0x08: its event handler class (vtable D_0046DD00, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046DD00[];
extern s32 func_001788F0(Progress *p, u32 door);

extern u8 D_003F2500[];
extern u8 D_003F2640[];
extern u8 D_003F2780[];
extern u8 D_003F28A0[];
extern u8 D_003F2910[];
extern u8 D_003F2980[];
extern void *D_003F3180[];
extern void *D_003F3200[];
extern u8 D_003F3230[];
extern PTMF D_019907E0[];
extern PTMF D_01990800[];

void *func_002AA440(void *o, s32 flags) { return room_dtor(o, flags, D_0046DD00, D_0046DB80); }

void *func_002AA4A0(void) {
    return D_003F2500;
}

void *func_002AA4B0(void) {
    return D_003F2640;
}

void *func_002AA4C0(void) {
    return D_003F2780;
}

void *func_002AA4D0(void) {
    return D_003F28A0;
}

void *func_002AA4E0(void) {
    return D_003F2910;
}

void *func_002AA4F0(void) {
    return D_003F2980;
}

void *func_002AA500(void *self, s32 i) {
    return D_003F3180[i];
}

void *func_002AA520(void) {
    return D_003F3230;
}

void *func_002AA530(void *self, s32 i) {
    return D_003F3200[i];
}

/* (self->*D_01990800[i])(a, b) */
s32 func_002AA550(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990800[i & 0xFF], a, b);
}

/* room 0x08 (D_003F31F0): the stalker is there but not about */
s32 func_002AA580(void) {
    u8 *c = (u8 *)gCharSlot2;

    return c != NULL && AT(c, 0x28, u8) == 0;
}

/* door be16 cmd[3..4]: func_001788F0 */
s32 func_002AA5B0(void *self, void *a1, u8 *cmd) {
    return func_001788F0(gProgress, (cmd[3] << 8 | cmd[4]) & 0xFFFF);
}

/* (self->*D_019907E0[i])(a, b) */
s32 func_002AA5D0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019907E0[i & 0xFF], a, b);
}

/* room 0x08 (D_003F31D8): the hanging object named by the handler's string 0xA - byte 3 0 sets
 * it still (+0x30 / +0x38 0, travel +0x3C 0.9); 1: the player's travel (+0x3C, its last move's
 * length) past 5 makes it creak (sounds 4 / 5 by turns, event bit 7) and swing for 20 frames:
 * its tilt (+0x10) 1 + sin(phase) degrees, the phase (+0x30) on by 36 a frame */
s32 func_002AA600(VObject *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } k09 = {0x3F666666}, kPi = {0x40490FDB}, k2Pi = {0x40C90FDB};
    const char *name = VCALL(self, 0x34, const char *(*)(VObject *, s32))(self, 0xA);
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, name);
    f32 t, a;

    if (o == NULL) {
        return 1;
    }
    if (cmd[3] == 0) {
        AT(o, 0x30, s32) = 0;
        AT(o, 0x38, s32) = 0;
        AT(o, 0x3C, f32) = k09.f;
        return 1;
    }
    if (cmd[3] != 1) {
        return 1;
    }
    if (gCharPlayer != NULL) {
        f32 d[4] __attribute__((aligned(16)));

        sceVu0CopyVector(d, (f32 *)((u8 *)gCharPlayer + 0x40));
        sceVu0SubVector(d, (f32 *)((u8 *)gCharPlayer + 0x10), d);
        t = AT(o, 0x3C, f32) + __builtin_sqrtf(d[1] * d[1] + d[0] * d[0] + d[2] * d[2]);
        AT(o, 0x3C, f32) = t;
        if (!(t <= 5.0f)) {
            VObject *ev = gEvents;

            AT(o, 0x38, f32) = 20.0f;
            AT(o, 0x3C, f32) = 0.0f;
            if ((u8)VCALL(ev, 0x58, s32 (*)(VObject *, s32))(ev, 7) == 1) {
                VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 7);
                func_00122C20(&gCharPlayer->a, 4, 6, 0, 0, NULL);
            } else {
                VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 7);
                func_00122C20(&gCharPlayer->a, 5, 6, 0, 0, NULL);
            }
        }
    }
    t = AT(o, 0x38, f32);
    if (t <= 0.0f) {
        return 1;
    }
    AT(o, 0x38, f32) = t - 1.0f;
    if (t - 1.0f < 0.0f) {
        AT(o, 0x38, f32) = 0.0f;
    }
    a = AT(o, 0x30, f32) + 36.0f;
    AT(o, 0x30, f32) = a;
    if (!(a < 360.0f)) {
        AT(o, 0x30, f32) = a - 360.0f;
    }
    a = kPi.f * (1.0f + func_0031C248(kPi.f * AT(o, 0x30, f32) / 180.0f)) / 180.0f;
    AT(o, 0x10, f32) = a;
    if (!(a <= kPi.f)) {
        AT(o, 0x10, f32) = a - k2Pi.f;
    }
    return 1;
}

/* room 0x08 (D_003F31C0): the cutscene director's +0x6C 3 (byte 3 0) or 2 */
s32 func_002AA8C0(void *self, void *a1, u8 *cmd) {
    VCALL(gCutscene, 0x6C, void (*)(VObject *, s32))(gCutscene, cmd[3] == 0 ? 3 : 2);
    return 1;
}
