/* Room 0x15: its event handler class (vtable D_0046E040, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "progress.h"
#include "snd_place.h"

extern void *D_0046DB80[];
extern void *D_0046E040[];
extern const char *D_003FA760;

extern u8 D_003FA090[];
extern u8 D_003FA110[];
extern u8 D_003FA1C0[];
extern u8 D_003FA2C0[];
extern u8 D_003FA330[];
extern void *D_003FA700[];
extern void *D_003FA750[];
extern u8 D_003FA770[];
extern PTMF D_01990920[];
extern PTMF D_01990930[];

/* 0x002ACB60 */
void *Room15_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046E040, D_0046DB80); }

/* 0x002ACBC0 */
void *Room15_EnterScript(void) {
    return D_003FA090;
}

/* 0x002ACBD0 */
void *Room15_CharEnterScript(void) {
    return D_003FA110;
}

/* 0x002ACBE0 */
void *Room15_Phase1Script(void) {
    return D_003FA1C0;
}

/* 0x002ACBF0 */
void *Room15_Phase3Script(void) {
    return D_003FA330;
}

/* 0x002ACC00 */
void *Room15_ActionScript(void *self, s32 i) {
    return D_003FA700[i];
}

/* 0x002ACC20 */
void *Room15_Phase2Script(void) {
    return D_003FA2C0;
}

/* 0x002ACC30 */
void *Room15_Table38(void) {
    return D_003FA770;
}

/* 0x002ACC40 */
void *Room15_ObjectName(void *self, s32 i) {
    return D_003FA750[i];
}

/* (self->*D_01990930[i])(a, b) */
/* 0x002ACC60 */
s32 Room15_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990930[i & 0xFF], a, b);
}

/* room 0x15 (D_003FA738): Hewie is about in room 0xF in state 0x2F or 0x52 */
/* 0x002ACC90 */
s32 Room15_Cond00(void) {
    Character *c = gCharacters[(u8)func_001770D0(gProgress, 1)];
    s32 s;

    if (c == NULL || c->a.active == 0 || AT(c, 0x30, s32) != 0xF) {
        return 0;
    }
    s = AT(c, 0xF3564, s32);
    return s == 0x2F || s == 0x52;
}

/* (self->*D_01990920[i])(a, b) */
/* 0x002ACD20 */
s32 Room15_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990920[i & 0xFF], a, b);
}

/* a lid (D_003FA760, +0x24 its height, +0x34 its speed): byte 3 0 up, 1 shut; 2 falling and
 * bouncing shut (the first landing clears progress flag 0x50 and, unless the director says no,
 * thuds), 2 while moving; 3 a random rattle up, 2 while it stays below */
/* 0x002ACD50 */
s32 Room15_Cmd00(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kShut = {0xC1CA6666}, k01 = {0x3DCCCCCD}, kBounce = {0xBE4CCCCD},
        kStill = {0x3CA3D70A}, k04 = {0x3ECCCCCD};
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_003FA760);
    f32 v;

    if (o == NULL) {
        return 1;
    }
    switch (cmd[3]) {
    case 0:
        AT(o, 0x24, f32) = 0.0f;
        AT(o, 0x30, f32) = 1.0f;
        AT(o, 0x34, f32) = 0.0f;
        return 1;
    case 1:
        AT(o, 0x24, f32) = kShut.f;
        AT(o, 0x34, f32) = 0.0f;
        return 1;
    case 2:
        AT(o, 0x34, f32) = AT(o, 0x34, f32) - k01.f;
        AT(o, 0x24, f32) = AT(o, 0x24, f32) + AT(o, 0x34, f32);
        if (AT(o, 0x24, f32) < kShut.f) {
            if (!(AT(o, 0x30, f32) <= 0.0f)) {
                Progress *p = gProgress;

                AT(o, 0x30, f32) = -1.0f;
                AT(p, 0x7C, u32) &= 0xFFFEFFFF;
                if ((u8)VCALL(gCamDirector, 0x38, s32 (*)(VObject *, Progress *))(gCamDirector, p) == 0) {
                    f32 at[4] __attribute__((aligned(16)));

                    at[1] = 30.0f;
                    at[2] = 30.0f;
                    at[0] = 0.0f;
                    Sound_PlayBankAt(gSound, 2, 6, at, 0, 0);
                }
            }
            AT(o, 0x24, f32) = kShut.f;
            AT(o, 0x34, f32) = AT(o, 0x34, f32) * kBounce.f;
        }
        v = AT(o, 0x34, f32);
        if (v <= 0.0f) {
            v = -v;
        }
        if (v < kStill.f) {
            AT(o, 0x24, f32) = kShut.f;
            return 1;
        }
        return 2;
    case 3:
        if (AT(o, 0x34, f32) < 0.0f) {
            AT(o, 0x34, f32) = 1.0f;
            AT(o, 0x24, f32) = AT(o, 0x24, f32) + k04.f;
        } else {
            AT(o, 0x34, f32) = -1.0f;
            AT(o, 0x24, f32) = AT(o, 0x24, f32) + k01.f;
        }
        v = (0.0f + AT(o, 0x24, f32)) + k01.f * VCALL(gRandom, 0x1C, f32 (*)(VObject *))(gRandom);
        AT(o, 0x24, f32) = v;
        if (v <= 0.0f) {
            return 2;
        }
        AT(o, 0x24, f32) = 0.0f;
        return 1;
    }
    return 1;
}
