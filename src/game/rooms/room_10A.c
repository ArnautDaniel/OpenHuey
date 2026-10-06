/* Room 0x10A: its event handler class (vtable D_0046FE80, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046FE80[];
extern u8 D_0047AC80[];
extern s32 D_0047B27C;

extern u8 D_00419730[];
extern u8 D_00419770[];
extern u8 D_00419870[];
extern u8 D_00419950[];
extern void *D_00419A30[];
extern u8 D_00419A60[];

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

extern PTMF D_01990F48[];
extern PTMF D_01990F58[];

void *func_002E76A0(void *o, s32 flags) { return room_dtor(o, flags, D_0046FE80, D_0046DB80); }

void *func_002E7700(void) { return D_00419730; }

void *func_002E7710(void) { return D_00419770; }

void *func_002E7720(void) { return D_00419870; }

void *func_002E7730(void) { return D_00419950; }

void *func_002E7740(void *o) { return D_0047AC80; }   /* D_0046FE80 +0x20 */

void *func_002E7750(void *self, s32 i) { return D_00419A30[i]; }

void *func_002E7770(void) { return D_00419A60; }

/* (self->*D_01990F58[i])(a, b) */
s32 func_002E7780(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990F58[i & 0xFF], a, b);
}

s32 func_002E77B0(void *self, void *a1, u8 *cmd) { return room_nudge(&D_0047B27C, cmd, 1.0f); }

/* (self->*D_01990F48[i])(a, b) */
s32 func_002E7850(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990F48[i & 0xFF], a, b);
}

s32 func_002E7880(void) {
    u8 *e = (u8 *)gProgress + 0x10D4;
    s32 i;

    for (i = 0; i < 4; i++, e += 16) {
        if (F(e, 0x4, s32) == 0x10A && e[0] >= 0x20) {
            return 1;
        }
    }
    return 0;
}
