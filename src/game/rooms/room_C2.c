/* Room 0xC2: its event handler class (vtable D_004785F0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "actor.h"
#include "snd_place.h"
#include "stalker_progress.h"
#include "msl.h"

extern void *D_0046DB80[];
extern void *D_004785F0[];
extern u8 D_0047AF04[];
extern const char *D_0043F8D0;
extern u32 D_0043F0C0[];
extern u32 D_0043F1A0[];
extern u32 D_0043F230[];
extern u32 D_0043F480[];
extern u32 D_0043F4D0[];
extern u32 D_0043F880[];

extern PTMF D_01991990[];
extern PTMF D_019919A8[];

/* the turn (+0x14) of the room object named D_0043F8D0 and the creak (sound 0x80000002, bank 6)
 * at its edge, 45 out at height 152 */
static inline void swing_to(u8 *o, f32 a) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    f32 pos[4] __attribute__((aligned(16)));

    AT(o, 0x14, f32) = kPi.f * a / 180.0f;
    pos[1] = 152.0f;
    pos[0] = -45.0f * func_0031C058(AT(o, 0x14, f32));
    pos[2] = -45.0f * -func_0031C248(AT(o, 0x14, f32));
    pos[3] = 1.0f;
    Sound_PlayBankAt(gSound, 0x80000002, 6, pos, 0, 0);
}

/* 0x0034A4C0 */
void *RoomC2_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_004785F0, D_0046DB80); }

/* 0x0034A520 */
void *RoomC2_EnterScript(void) {
    return D_0043F0C0;
}

/* 0x0034A530 */
void *RoomC2_CharEnterScript(void) {
    return D_0043F1A0;
}

/* 0x0034A540 */
void *RoomC2_Phase1Script(void) {
    return D_0043F230;
}

/* 0x0034A550 */
void *RoomC2_Phase2Script(void) {
    return D_0043F480;
}

/* 0x0034A560 */
void *RoomC2_Phase5Script(void) {
    return D_0043F4D0;
}

/* 0x0034A570 */
u32 RoomC2_ActionScript(void *self, s32 i) {
    return D_0043F880[i];
}

/* 0x0034A590 */
void *RoomC2_Table38(void *o) { return D_0047AF04; }   /* D_004785F0 +0x38 */

/* 0x0034A5A0 */
u32 RoomC2_ObjectName(void *self, s32 i) {
    return ((u32 *)&D_0043F8D0)[i];   /* its table of names (one is reached by name too) */
}

/* (self->*D_019919A8[i])(a, b) */
/* 0x0034A5C0 */
s32 RoomC2_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019919A8[i & 0xFF], a, b);
}

/* 0x0034A5F0 */
s32 RoomC2_Cond00(void) {
    return Countdown_Seconds((u8 *)gProgress + 0x764) == 0;
}

/* (self->*D_01991990[i])(a, b) */
/* 0x0034A620 */
s32 RoomC2_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991990[i & 0xFF], a, b);
}

/* byte 3 to the player's func_00124F20 while she's active */
/* 0x0034A650 */
s32 RoomC2_Cmd01(void *self, void *a1, u8 *cmd) {
    if (gCharPlayer != NULL && AT(gCharPlayer, 0x28, u8) != 0) {
        func_00124F20(gCharPlayer, cmd[3]);
    }
    return 1;
}

/* byte 3 0 / 1: shut / open (pi/2); 2 / 3 opening / shutting by script variable 0 (0..120 frames,
 * eased by a sine) */
/* 0x0034A6A0 */
s32 RoomC2_Cmd00(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB};
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_0043F8D0);
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
        AT(o, 0x14, u32) = 0x3FC90FDB;   /* pi/2 */
        break;
    case 2:
        e = func_0031C248(kPi.f * (-90.0f + 180.0f * t) / 180.0f);
        swing_to(o, 45.0f * (1.0f + e));
        break;
    case 3:
        e = func_0031C248(kPi.f * (-90.0f + 180.0f * t) / 180.0f);
        swing_to(o, 90.0f - 45.0f * (1.0f + e));
        break;
    }
    return 1;
}
