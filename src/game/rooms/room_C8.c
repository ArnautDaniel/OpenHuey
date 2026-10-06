/* Room 0xC8: its event handler class (vtable D_00478730, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "memcard.h"
#include "scene_game_members.h"

extern void *D_0046DB80[];
extern void *D_00478730[];
extern u8 D_0047AF40[];
extern void *D_0047A410[];
extern u32 D_00441520[];
extern u32 D_00441560[];
extern u32 D_00441620[];
extern u32 D_004416A0[];
extern u32 D_004417A8[];
extern u32 D_0047AF3C[];

extern void *D_0046F580[];

extern PTMF D_01991A08[];

static void effect_47a410_init(void **obj) {
    obj[0] = D_0047A410;
}

void *func_0034B5C0(void *o, s32 flags) { return room_dtor(o, flags, D_00478730, D_0046DB80); }

void *func_0034B620(void) {
    return D_00441520;
}

void *func_0034B630(void) {
    return D_00441560;
}

void *func_0034B640(void) {
    return D_00441620;
}

void *func_0034B650(void) {
    return D_004416A0;
}

u32 func_0034B660(void *self, s32 i) {
    return D_004417A8[i];
}

void *func_0034B680(void *o) { return D_0047AF40; }   /* D_00478730 +0x38 */

u32 func_0034B690(void *self, s32 i) {
    return D_0047AF3C[i];
}

/* (self->*D_01991A08[i])(a, b) */
s32 func_0034B6B0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991A08[i & 0xFF], a, b);
}

/* an effect D_0047A410 (8 bytes), not started */
s32 func_0034B6E0(void) {
    Effect_New(gEffects, 0x8, effect_47a410_init);
    return 1;
}

/* destructor (vtable D_0047A410) */
/* 0x00378310 */
void *TurningModel_dtor(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0047A410;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}
