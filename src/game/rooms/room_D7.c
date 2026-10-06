/* Room 0xD7: its event handler class (vtable D_0047A230, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0047A230[];
extern u8 D_0047B07C[];
extern u8 D_0047B080[], D_0047B088[];
extern s32 D_0047B2D8;
extern char D_0047B2D0[];
extern const char D_00463728[];

#define F(p, off, T) (*(T *)((u8 *)(p) + (off)))

extern u8 D_00447A70[];
extern u8 D_00447A90[];
extern u8 D_00447AD0[];
extern u8 D_00447B00[];
extern u8 D_00447BC0[];
extern PTMF D_01991C00[];
extern PTMF D_01991C20[];

void *func_0036F4A0(void *o, s32 flags) { return room_dtor(o, flags, D_0047A230, D_0046DB80); }

void *func_0036F500(void) {
    return D_00447A70;
}

void *func_0036F510(void) {
    return D_00447A90;
}

void *func_0036F520(void) {
    return D_00447AD0;
}

void *func_0036F530(void *o) { return D_0047B07C; }   /* D_0047A230 +0x14 */

void *func_0036F540(void) {
    return D_00447B00;
}

void *func_0036F550(void *o) { return D_0047B080; }   /* D_0047A230 +0x20 */

u32 func_0036F560(void *o, s32 i) { return ((u32 *)D_0047B088)[i]; }   /* D_0047A230 +0x24 */

void *func_0036F580(void) {
    return D_00447BC0;
}

/* (self->*D_01991C20[i])(a, b) */
s32 func_0036F590(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991C20[i & 0xFF], a, b);
}

/* as func_002E7020 */
s32 func_0036F5C0(void) {
    Effect_New(gEffects, 0x10, effect_10_init);
    return 1;
}

s32 func_0036F690(void) { return clock_draw(D_0047B2D0, D_00463728); }

/* as func_002E70F0 */
s32 func_0036F7E0(void *self, void *a1, u8 *cmd) { return room_nudge(&D_0047B2D8, cmd, 2.0f); }

/* (self->*D_01991C00[i])(a, b) */
s32 func_0036F880(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991C00[i & 0xFF], a, b);
}

/* the progress object's +0x7C with the caller's arguments */
s32 func_0036F8B0(void *self, s32 a1, s32 a2, s32 a3) {
    return VCALL((VObject *)gProgress, 0x7C, s32 (*)(VObject *, s32, s32, s32))((VObject *)gProgress, a1, a2, a3);
}

s32 func_0036F8D0(void) {
    u8 *e = (u8 *)gProgress + 0x10D4;
    s32 i;

    for (i = 0; i < 4; i++, e += 16) {
        if (F(e, 0x4, s32) == 0xD7 && e[0] >= 0x20) {
            return 1;
        }
    }
    return 0;
}
