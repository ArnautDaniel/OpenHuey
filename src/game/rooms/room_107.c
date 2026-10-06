/* Room 0x107: its event handler class (vtable D_0046FDC0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046FDC0[];
extern u8 D_0047AC78[];
extern s32 D_0047B274;

extern u8 D_00418AF0[];
extern u8 D_00418B60[];
extern u8 D_00418BA0[];
extern u8 D_00418BF0[];
extern void *D_00418D70[];
extern u8 D_00418DB0[];

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

extern PTMF D_01990EE8[];
extern PTMF D_01990F00[];

void *func_002E6F10(void *o, s32 flags) { return room_dtor(o, flags, D_0046FDC0, D_0046DB80); }

void *func_002E6F70(void) { return D_00418AF0; }

void *func_002E6F80(void) { return D_00418B60; }

void *func_002E6F90(void) { return D_00418BA0; }

void *func_002E6FA0(void) { return D_00418BF0; }

void *func_002E6FB0(void *o) { return D_0047AC78; }   /* D_0046FDC0 +0x20 */

void *func_002E6FC0(void *self, s32 i) { return D_00418D70[i]; }

void *func_002E6FE0(void) { return D_00418DB0; }

/* (self->*D_01990F00[i])(a, b) */
s32 func_002E6FF0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990F00[i & 0xFF], a, b);
}

s32 func_002E7020(void) {
    Effect_New(gEffects, 0x10, effect_10_init);
    return 1;
}

s32 func_002E70F0(void *self, void *a1, u8 *cmd) { return room_nudge(&D_0047B274, cmd, 2.0f); }

/* (self->*D_01990EE8[i])(a, b) */
s32 func_002E7190(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990EE8[i & 0xFF], a, b);
}

s32 func_002E71C0(void) {
    u8 *e = (u8 *)gProgress + 0x10D4;
    s32 i;

    for (i = 0; i < 4; i++, e += 16) {
        if (F(e, 0x4, s32) == 0x107 && e[0] >= 0x20) {
            return 1;
        }
    }
    return 0;
}
