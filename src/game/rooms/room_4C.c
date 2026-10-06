/* Room 0x4C: its event handler class (vtable D_0046E700, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "fiona.h"
#include "progress.h"
#include "snd_place.h"
#include "sce/libvu0.h"

extern void *D_0046DB80[];
extern void *D_0046E700[];
extern const char *D_0040AC10[];
extern s32 D_0047B250;                   /* room 0x4C: what the player has done so far */

extern u8 D_00409980[];
extern u8 D_00409A80[];
extern u8 D_00409AD0[];
extern u8 D_0040A020[];
extern u32 D_0040AB80[];
extern u8 D_0040AC60[];

extern PTMF D_01990CE0[];

/* 0x002B3540 */
void *Room4C_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046E700, D_0046DB80); }

/* 0x002B35A0 */
void *Room4C_EnterScript(void) {
    return D_00409980;
}

/* 0x002B35B0 */
void *Room4C_CharEnterScript(void) {
    return D_00409A80;
}

/* 0x002B35C0 */
void *Room4C_Phase1Script(void) {
    return D_00409AD0;
}

/* 0x002B35D0 */
void *Room4C_Phase2Script(void) {
    return D_0040A020;
}

/* 0x002B35E0 */
u32 Room4C_ActionScript(void *self, s32 i) {
    return D_0040AB80[i];
}

/* 0x002B3600 */
void *Room4C_Table38(void) {
    return D_0040AC60;
}

/* 0x002B3610 */
u32 Room4C_ObjectName(void *self, s32 i) {
    return (u32)D_0040AC10[i];
}

/* (self->*D_01990CE0[i])(a, b) */
/* 0x002B3630 */
s32 Room4C_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990CE0[i & 0xFF], a, b);
}

/* room 0x4C (D_0040AC08): character 0x11's model parts 0xA2 / 0xA4 / 0xAC get bit 2 when byte
 * 3 is 0, else lose it */
/* 0x002B3660 */
s32 Room4C_Cmd02(void *self, void *a1, u8 *cmd) {
    u8 *c = (u8 *)gCharacters[Progress_SlotOfId(gProgress, 0x11) & 0xFF];

#ifdef HG_NATIVE
    if (c == NULL) {   /* character 0x11 absent (the PS2 writes through junk) */
        return 1;
    }
#endif
    if (cmd[3] == 0) {
        AT(AT(c, 0xF0, u8 *), 0xA2, u8) |= 2;
        AT(AT(c, 0xF0, u8 *), 0xA4, u8) |= 2;
        AT(AT(c, 0xF0, u8 *), 0xAC, u8) |= 2;
    } else {
        AT(AT(c, 0xF0, u8 *), 0xA2, u8) &= ~2;
        AT(AT(c, 0xF0, u8 *), 0xA4, u8) &= ~2;
        AT(AT(c, 0xF0, u8 *), 0xAC, u8) &= ~2;
    }
    return 1;
}

/* room 0x4C (D_0040ABF0): byte 3 0 starts counting what the player does (her +0x1AD710 on), 1
 * adds this frame's (Fiona_Shakes); at 35 events bit 0x13 */
/* 0x002B3720 */
s32 Room4C_Cmd01(void *self, void *a1, u8 *cmd) {
    switch (cmd[3]) {
    case 0:
        D_0047B250 = 0;
        AT(gCharPlayer, 0x1AD710, u8) = 1;
        AT(gCharPlayer, 0x1AD714, s32) = 0;
        break;
    case 1:
        D_0047B250 += Fiona_Shakes((Fiona *)gCharPlayer);
        if (D_0047B250 >= 0x23) {
            VCALL(gEvents, 0x5C, void (*)(VObject *, s32))(gEvents, 0x13);
        }
        break;
    }
    return 1;
}

/* four room objects (D_0040AC10) pressed in (+0x24 down 0.2 to -0.7, a sound as each starts) while
 * event flag i is set, else back up 0.2 to 0; byte 3 0 all reset */
/* 0x002B37D0 */
s32 Room4C_Cmd00(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kStep = {0x3E4CCCCD}, kLow = {0xBF333333};
    VObject *objs = D_00456DF8, *ev = gEvents, *snd = gSound;
    s32 i;

    for (i = 0; i < 4; i++) {
        u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_0040AC10[i]);

        if (o == NULL) {
            continue;
        }
        if (cmd[3] == 0) {
            AT(o, 0x24, f32) = 0.0f;
        } else if (VCALL(ev, 0x58, s32 (*)(VObject *, s32))(ev, i & 0xFF) != 0) {
            if (AT(o, 0x24, f32) == 0.0f) {
                f32 at[4] __attribute__((aligned(16)));

                sceVu0CopyVector(at, (f32 *)(o + 0x20));
                Sound_PlayBankAt(snd, 6, 6, at, 0, 0);
            }
            AT(o, 0x24, f32) = AT(o, 0x24, f32) - kStep.f;
            if (AT(o, 0x24, f32) < kLow.f) {
                AT(o, 0x24, f32) = kLow.f;
            }
        } else {
            AT(o, 0x24, f32) = AT(o, 0x24, f32) + kStep.f;
            if (!(AT(o, 0x24, f32) <= 0.0f)) {
                AT(o, 0x24, f32) = 0.0f;
            }
        }
    }
    return 1;
}
