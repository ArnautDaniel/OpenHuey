/* Room 0x43: its event handler class (vtable D_0046E5C0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "ptmf.h"
#include "scene_game_members.h"

extern void *D_0046DB80[];
extern void *D_0046E5C0[];
extern u8 D_0047AB3C[];
extern void *D_00479580[];
extern u8 D_00407AE0[];
extern u8 D_00407B70[];
extern u8 D_00407C00[];
extern u8 D_00407CB0[];
extern u32 D_00407E20[];
extern u8 D_00407E50[];

extern PTMF D_01990C58[];

static void effect_6cf0_init(void **obj) {
    obj[0] = D_00479580;
    obj[0x6040 / 4] = D_00469D00;
    ((s32 *)obj)[0x6044 / 4] = -1;
    obj[0x6040 / 4] = D_0046FC30;
    obj[0x6078 / 4] = D_00469D00;
    ((s32 *)obj)[0x607C / 4] = -1;
    obj[0x6078 / 4] = D_0046FC30;
    obj[0x60B0 / 4] = D_00469D00;
    ((s32 *)obj)[0x60B4 / 4] = -1;
    obj[0x60B0 / 4] = D_0046FC30;
}

void *func_002B25E0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E5C0, D_0046DB80); }

void *func_002B2640(void) {
    return D_00407AE0;
}

void *func_002B2650(void) {
    return D_00407B70;
}

void *func_002B2660(void) {
    return D_00407C00;
}

void *func_002B2670(void) {
    return D_00407CB0;
}

void *func_002B2680(void *o) { return D_0047AB3C; }   /* D_0046E5C0 +0x20 */

u32 func_002B2690(void *self, s32 i) {
    return D_00407E20[i];
}

void *func_002B26B0(void) {
    return D_00407E50;
}

/* (self->*D_01990C58[i])(a, b) */
s32 func_002B26C0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990C58[i & 0xFF], a, b);
}

/* the effect D_00479580 (three quad drawers) started with parameter 0 */
s32 func_002B26F0(void) {
    u8 *mgr = gEffects;
    s32 slot = Effect_New(mgr, 0x6CF0, effect_6cf0_init);
    s32 arg = 0;

    func_002D6090(mgr, slot, &arg);
    return 1;
}
