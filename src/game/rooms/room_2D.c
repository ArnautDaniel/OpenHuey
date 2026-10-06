/* Room 0x2D: its event handler class (vtable D_0046E540, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "progress.h"
#include "scene_game_members.h"
#include "msl.h"
#include "sce/libvu0.h"

extern void *D_0046DB80[];
extern void *D_0046E540[];
extern void *D_00472390[];   /* a 0xC0-byte effect */
extern u8 D_00405AC0[];
extern u8 D_00405B80[];
extern u8 D_00405BD0[];
extern u8 D_00405E30[];
extern u32 D_00406470[];
extern u8 D_00406530[];
extern u32 D_00406510[];

extern PTMF D_01990C10[];
extern PTMF D_01990C38[];

static void effect_c0b_init(void **obj) {
    obj[0] = D_00472390;
    obj[0x70 / 4] = D_00469D00;
    ((s32 *)obj)[0x74 / 4] = -1;
    obj[0x70 / 4] = D_0046FC30;
}

/* 0x002B1790 */
void *Room2D_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046E540, D_0046DB80); }

/* 0x002B17F0 */
void *Room2D_EnterScript(void) {
    return D_00405AC0;
}

/* 0x002B1800 */
void *Room2D_CharEnterScript(void) {
    return D_00405B80;
}

/* 0x002B1810 */
void *Room2D_Phase1Script(void) {
    return D_00405BD0;
}

/* 0x002B1820 */
void *Room2D_Phase2Script(void) {
    return D_00405E30;
}

/* 0x002B1830 */
u32 Room2D_ActionScript(void *self, s32 i) {
    return D_00406470[i];
}

/* 0x002B1850 */
void *Room2D_Table38(void) {
    return D_00406530;
}

/* 0x002B1860 */
u32 Room2D_ObjectName(void *self, s32 i) {
    return D_00406510[i];
}

/* (self->*D_01990C38[i])(a, b) */
/* 0x002B1880 */
s32 Room2D_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990C38[i & 0xFF], a, b);
}

/* Hewie (in state 0x7F, +0xF3564) within 5 of the spot by byte 3: 0 (-259.5, 190), 1 (-276,
 * 160) (others: what the caller left) */
/* 0x002B18B0 */
s32 Room2D_Cond00(void *self, void *a1, u8 *cmd) {
    Character *h = gCharacters[Progress_SlotOfId(gProgress, 1) & 0xFF];
    f32 dx = 0.0f, dz = 0.0f;

    if (h == NULL || AT(h, 0x28, u8) == 0 || AT(h, 0xF3564, s32) != 0x7F) {
        return 0;
    }
    switch (cmd[3]) {
    case 0:
        dx = 259.5f + AT(h, 0x10, f32);
        dz = AT(h, 0x18, f32) - 190.0f;
        break;
    case 1:
        dx = 276.0f + AT(h, 0x10, f32);
        dz = AT(h, 0x18, f32) - 160.0f;
        break;
    }
    return hook_sqrt(dz * dz + dx * dx) < 5.0f;
}

/* (self->*D_01990C10[i])(a, b) */
/* 0x002B19D0 */
s32 Room2D_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990C10[i & 0xFF], a, b);
}

/* 0x002B1A00 */
s32 Room2D_Cmd02(VObject *self, void *a1, u8 *cmd) {
    return hangers_swing(self, cmd, 4, 2, 5.0f, 1, 3, 1, 2);
}

/* something dropped (effect D_00472390, its slot in event var 1) from (-276.5, 3, 160), by
 * byte 3: 0 started (event var 0 the frame count); 1 a frame (2 while falling): it drifts 0.5
 * a frame in x and falls 0.05 x n(n+1)/2, gone below -10 */
/* 0x002B1D50 */
s32 Room2D_Cmd01(void *self, void *a1, u8 *cmd) {
    VObject *ev = gEvents;
    f32 p[4] __attribute__((aligned(16)));

    switch (cmd[3]) {
    case 0: {
        u8 *mgr;
        s32 slot;

        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 0, 0);
        mgr = gEffects;
        slot = Effect_New(mgr, 0xC0, effect_c0b_init);
        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, slot);
        p[1] = 3.0f;
        p[0] = -276.5f;
        p[2] = 160.0f;
        p[3] = 1.0f;
        func_002D6090(mgr, slot, p);
        break;
    }
    case 1: {
        u32 n = VCALL(ev, 0x34, u32 (*)(VObject *, s32))(ev, 0) + 1;
        s32 slot = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 1);

        p[0] = -276.5f + 0.5f * (f32)n;
        p[2] = 160.0f;
        p[3] = 1.0f;
        p[1] = 3.0f - 0x1.99999ap-5f /* 0.05 */ * (f32)((n * (n + 1)) >> 1);
        if (!(p[1] <= -10.0f)) {
            func_002D6090(gEffects, slot, p);
            VCALL(ev, 0x30, void (*)(VObject *, s32, u32))(ev, 0, n);
            return 2;
        }
        func_002D6090(gEffects, slot, NULL);
        break;
    }
    }
    return 1;
}

/* a hanging thing (the room's +0x34 (byte 3 + 2) object) swinging, by byte 4: 0 / 2 set
 * going (12 degrees) away from the partner / Fiona; 1 a step (22.5 degrees of its swing,
 * shrinking to 0.4 at each end; under half a degree it stops) - 2 while it swings */
/* 0x002B2050 */
s32 Room2D_Cmd00(VObject *self, void *a1, u8 *cmd) {
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, s32))(
        D_00456DF8, VCALL(self, 0x34, s32 (*)(VObject *, s32))(self, cmd[3] + 2));
    f32 d[4] __attribute__((aligned(16)));
    f32 a;

    if (o == NULL) {
        return 1;
    }
    switch (cmd[4]) {
    case 0:
    case 2:
        AT(o, 0x4, s32) = 0;
        AT(o, 0x30, f32) = 0.0f;
        AT(o, 0x34, f32) = 0x1.aceeap-3f /* 12 degrees */;
        sceVu0SubVector(d, (f32 *)(o + 0x20), cmd[4] == 0 ? gCharPartner->a.pos : gCharPlayer->a.pos);
        d[1] = 0.0f;
        sceVu0Normalize(d, d);
        AT(o, 0x38, f32) = d[2];
        AT(o, 0x3C, f32) = -d[0];
        break;
    case 1:
        if (AT(o, 0x4, s32) == 0) {
            AT(o, 0x30, f32) = a = AT(o, 0x30, f32) + 0x1.921fb6p-2f /* 22.5 degrees */;
            if (a <= 0.0f) {
                a = -a;
            }
            if (a < 0x1.1df46ap-6f /* 1 degree */) {
                AT(o, 0x34, f32) = AT(o, 0x34, f32) * 0x1.99999ap-2f /* 0.4 */;
            }
            if (!(AT(o, 0x30, f32) <= 0x1.921fb6p+0f)) {
                AT(o, 0x4, s32) = 1;
            }
        } else {
            AT(o, 0x30, f32) = a = AT(o, 0x30, f32) - 0x1.921fb6p-2f;
            if (a <= 0.0f) {
                a = -a;
            }
            if (a < 0x1.1df46ap-6f) {
                AT(o, 0x34, f32) = AT(o, 0x34, f32) * 0x1.99999ap-2f;
            }
            if (AT(o, 0x30, f32) < -0x1.921fb6p+0f) {
                AT(o, 0x4, s32) = 0;
            }
        }
        if (AT(o, 0x34, f32) <= 0x1.1df46ap-7f /* half a degree */) {
            AT(o, 0x10, f32) = 0.0f;
            AT(o, 0x18, f32) = 0.0f;
            break;
        }
        AT(o, 0x10, f32) = AT(o, 0x38, f32) * (AT(o, 0x34, f32) * func_0031C248(AT(o, 0x30, f32)));
        AT(o, 0x18, f32) = AT(o, 0x3C, f32) * (AT(o, 0x34, f32) * func_0031C248(AT(o, 0x30, f32)));
        return 2;
    }
    return 1;
}
