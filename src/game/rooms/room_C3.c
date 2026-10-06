/* Room 0xC3: its event handler class (vtable D_00478630, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "scene_game_members.h"
#include "snd_place.h"
#include "stalker_progress.h"
#include "msl.h"

extern void *D_0046DB80[];
extern void *D_00478630[];
extern u8 D_0047AF08[];
extern const char *D_004400F0[];
extern const char *D_004400E0;
extern u32 D_0043F8F0[];
extern u32 D_0043F9F0[];

extern u32 D_0043FA80[];
extern u32 D_0043FC10[];
extern u32 D_0043FCC0[];
extern u32 D_00440080[];

extern PTMF D_019919C0[];
extern PTMF D_019919E8[];

/* (the other door: turned the other way, its creak at (45 sin, 142, 45 cos)) */
static inline void swing_to2(u8 *o, f32 a) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    f32 pos[4] __attribute__((aligned(16)));

    AT(o, 0x14, f32) = kPi.f * a / 180.0f;
    pos[1] = 142.0f;
    pos[0] = 45.0f * func_0031C248(AT(o, 0x14, f32));
    pos[3] = 1.0f;
    pos[2] = 45.0f * func_0031C058(AT(o, 0x14, f32));
    func_002FF650(gSound, 0x80000002, 6, pos, 0, 0);
}

void *func_0034A9B0(void *o, s32 flags) { return room_dtor(o, flags, D_00478630, D_0046DB80); }

void *func_0034AA10(void) {
    return D_0043F8F0;
}

void *func_0034AA20(void) {
    return D_0043F9F0;
}

void *func_0034AA30(void) {
    return D_0043FA80;
}

void *func_0034AA40(void) {
    return D_0043FC10;
}

void *func_0034AA50(void) {
    return D_0043FCC0;
}

u32 func_0034AA60(void *self, s32 i) {
    return D_00440080[i];
}

void *func_0034AA80(void *o) { return D_0047AF08; }   /* D_00478630 +0x38 */

u32 func_0034AA90(void *self, s32 i) {
    return ((u32 *)&D_004400E0)[i];   /* its table of names (one is reached by name too) */
}

/* (self->*D_019919E8[i])(a, b) */
s32 func_0034AAB0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019919E8[i & 0xFF], a, b);
}

s32 func_0034AAE0(void) {
    return func_002EC410((u8 *)gProgress + 0x764) == 0;
}

/* (self->*D_019919C0[i])(a, b) */
s32 func_0034AB10(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019919C0[i & 0xFF], a, b);
}

/* sound 3 (bank 6) at the room object named D_004400F0[0] */
s32 func_0034AB40(void) {
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_004400F0[0]);

    if (o != NULL) {
        func_002FF650(gSound, 3, 6, (f32 *)(o + 0x20), 0, 0);
    }
    return 1;
}

/* the player (active, +0xE0 clear) put in action 0xB / 0x21 / 0xFF unless held (7); then
 * progress +0x7B8 gets 50 */
s32 func_0034ABB0(void) {
    CharAction act;

    if (gCharPlayer == NULL || AT(gCharPlayer, 0x28, u8) == 0 || AT(gCharPlayer, 0xE0, u8) != 0) {
        return 1;
    }
    func_002A8410((u8 *)&act);
    act.state = 0xB;
    act.a = 0x21;
    act.b = 0xFF;
    if (AT(gCharPlayer, 0x14E8, s32) != 7) {
        char_set_action((u8 *)gCharPlayer, &act);
    }
    func_002EF9E0((u8 *)gProgress + 0x7B8, 50.0f);
    return 1;
}

/* (as func_0034A6A0, the room object named D_004400E0, opening to -pi/2) */
s32 func_0034AD00(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_004400E0);
    f32 t, e;

    if (o == NULL) {
        return 1;
    }
    t = (f32)(u32)VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 0) / 120.0f;
    switch (cmd[3]) {
    case 0:
        AT(o, 0x14, s32) = 0;
        break;
    case 1:
        AT(o, 0x14, u32) = 0xBFC90FDB;   /* -pi/2 */
        break;
    case 2:
        e = func_0031C248(kPi.f * (-90.0f + 180.0f * t) / 180.0f);
        swing_to2(o, -45.0f * (1.0f + e));
        break;
    case 3:
        e = func_0031C248(kPi.f * (-90.0f + 180.0f * t) / 180.0f);
        swing_to2(o, -90.0f - -45.0f * (1.0f + e));
        break;
    }
    return 1;
}
