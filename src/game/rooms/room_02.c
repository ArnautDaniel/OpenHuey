/* Room 0x02: its event handler class (vtable D_0046DC00, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "panic.h"
#include "scene_game_members.h"
#include "msl.h"

extern void *D_0046DB80[];
extern void *D_0046DC00[];
extern const char *D_003F0404;
extern void *D_00470A70[];
extern u8 D_003EEDF0[];
extern u8 D_003EEF70[];
extern u8 D_003EF0D0[];
extern u8 D_003EF5B0[];
extern u8 D_003EF780[];
extern u8 D_003EF8E0[];
extern void *D_003F0310[];
extern void *D_003F03E0[];
extern u8 D_003F0410[];
extern u8 D_003F0430[];
extern PTMF D_01990730[];
extern PTMF D_01990760[];

static void effect_70A70_init(void **obj) {
    obj[0] = D_00470A70;
    obj[0xC10 / 4] = D_00469D00;
    ((s32 *)obj)[0xC14 / 4] = -1;
    obj[0xC10 / 4] = D_0046FC30;
}

void *func_002A8E00(void *o, s32 flags) { return room_dtor(o, flags, D_0046DC00, D_0046DB80); }

void *func_002A8E60(void) {
    return D_003EEDF0;
}

void *func_002A8E70(void) {
    return D_003EEF70;
}

void *func_002A8E80(void) {
    return D_003EF0D0;
}

void *func_002A8E90(void) {
    return D_003EF5B0;
}

void *func_002A8EA0(void) {
    return D_003EF780;
}

void *func_002A8EB0(void) {
    return D_003EF8E0;
}

void *func_002A8EC0(void *self, s32 i) {
    return D_003F0310[i];
}

void *func_002A8EE0(void) {
    return D_003F0410;
}

void *func_002A8EF0(void) {
    return D_003F0430;
}

void *func_002A8F00(void *self, s32 i) {
    return D_003F03E0[i];
}

/* (self->*D_01990760[i])(a, b) */
s32 func_002A8F20(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990760[i & 0xFF], a, b);
}

/* room 0x02 (D_003F03C8): the pursuer is about, in a mode other than 0, 1 or 5, and progress
 * +0x1130 isn't 0xFE */
s32 func_002A8F50(void) {
    Character *s = gCharPursuer;

    if (s == NULL || s->a.active == 0 || AT(s, 0xE8, s32) == 0 || AT(s, 0xE8, s32) == 1 || AT(s, 0xE8, s32) == 5) {
        return 0;
    }
    if (AT(gProgress, 0x1130, u8) == 0xFE) {
        return 0;
    }
    return 1;
}

/* room 0x02 (D_003F03C0): the player is about and down at floor level (y <= 0) */
s32 func_002A8FF0(void) {
    if (gCharPlayer != NULL && gCharPlayer->a.active != 0 && gCharPlayer->a.pos[1] <= 0.0f) {
        return 1;
    }
    return 0;
}

/* (self->*D_01990730[i])(a, b) */
s32 func_002A9050(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990730[i & 0xFF], a, b);
}

/* room 0x02 (D_003F03B0): the drum can's wobble by byte 3 - 0 still (rest height +0x38 = its
 * height), 2 struck (+0x34 strength 1), 1 each frame: the strength fades by 0.2 while it bobs
 * 0.2 x strength x sin(phase +0x30, on by 90 degrees) about the rest height */
s32 func_002A9080(void *self, void *a1, u8 *cmd) {
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_003F03FC);
    f32 t;

#ifdef HG_NATIVE
    if (o == NULL) {   /* (the PS2 writes through junk) */
        return 1;
    }
#endif
    switch (cmd[3]) {
    case 2:
        AT(o, 0x30, s32) = 0;
        AT(o, 0x34, f32) = 1.0f;
        break;
    case 1:
        t = AT(o, 0x34, f32) - 0x1.99999ap-3f /* 0.2 */;
        AT(o, 0x34, f32) = t;
        if (t <= 0.0f) {
            AT(o, 0x34, f32) = 0.0f;
            AT(o, 0x30, f32) = 0.0f;
            AT(o, 0x24, f32) = AT(o, 0x38, f32);
            break;
        }
        t = AT(o, 0x30, f32) + 90.0f;
        AT(o, 0x30, f32) = t;
        if (!(t < 360.0f)) {
            AT(o, 0x30, f32) = t - 360.0f;
        }
        AT(o, 0x24, f32) = AT(o, 0x38, f32) + 0x1.99999ap-3f /* 0.2 */ *
            (AT(o, 0x34, f32) * func_0031C248(0x1.921fb6p+1f /* pi */ * AT(o, 0x30, f32) / 180.0f));
        break;
    case 0:
        AT(o, 0x34, s32) = 0;
        AT(o, 0x30, s32) = 0;
        AT(o, 0x38, f32) = AT(o, 0x24, f32);
        break;
    default:
        return 1;
    }
    return 1;
}

/* room 0x02 (D_003F03A0): the 0xE60-byte effect D_00470A70 by byte 3 - 0 made (its slot kept
 * in event variable 0), 1 that one sent 0 (stop), else one more made and sent 1 */
s32 func_002A91F0(void *self, void *a1, u8 *cmd) {
    if (cmd[3] == 0) {
        s32 slot = Effect_New(gEffects, 0xE60, effect_70A70_init);

        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 0, slot);
    } else if (cmd[3] == 1) {
        s32 slot = VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 0);
        s32 arg = 0;

        func_002D6090(gEffects, slot, &arg);
    } else {
        u8 *mgr = gEffects;
        s32 slot = Effect_New(mgr, 0xE60, effect_70A70_init);
        s32 arg = 1;

        func_002D6090(mgr, slot, &arg);
    }
    return 1;
}

/* room 0x02 (D_003F0388): the panic (progress +0x7B8) raised to 80 */
s32 func_002A9460(void) {
    u8 *p = (u8 *)gProgress;
    f32 t = 80.0f - AT(p, 0x7BC, f32);

    if (!(t < 0.0f)) {
        func_002EFB70(p + 0x7B8, t);
    }
    return 1;
}

/* the room object D_003F0404's +0x24 by byte 3: 0 set (37.978 with progress flag 0x12, else
 * 11), 1 up 0.25 to 37.978 (then event +0x5C (2)), 2 up 0.25, else down 0.25 */
s32 func_002A94C0(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kTop = {0x4217E979};   /* 37.978 */
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_003F0404);

    if (o == NULL) {
        return 1;
    }
    switch (cmd[3]) {
    case 0:
        AT(o, 0x24, f32) = (AT(gProgress, 0x1C, u32) & 0x40000) ? kTop.f : 11.0f;
        break;
    case 1:
        AT(o, 0x24, f32) = AT(o, 0x24, f32) + 0.25f;
        if (!(AT(o, 0x24, f32) < kTop.f)) {
            VCALL(gEvents, 0x5C, void (*)(VObject *, s32))(gEvents, 2);
            AT(o, 0x24, f32) = kTop.f;
        }
        break;
    case 2:
        AT(o, 0x24, f32) = AT(o, 0x24, f32) + 0.25f;
        break;
    default:
        AT(o, 0x24, f32) = AT(o, 0x24, f32) - 0.25f;
        break;
    }
    return 1;
}
