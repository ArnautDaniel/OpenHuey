/* Room 0x80: its event handler class (vtable D_00478570, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "memcard.h"

extern void *D_0046DB80[];
extern void *D_00478570[];
extern void *D_0047A430[];
extern u32 D_0043EA90[];
extern u32 D_0043EAE0[];
extern u32 D_0043EBA0[];
extern u32 D_0043EC20[];
extern u32 D_0047AEF0[];
extern u32 D_0047AEF4[];

extern void *D_0046F580[];
extern void func_002D63B0(void *p);

extern PTMF D_01991968[];

static void effect_7a430_init(void **obj) {
    obj[0] = D_0047A430;
}

void *func_0034A140(void *o, s32 flags) { return room_dtor(o, flags, D_00478570, D_0046DB80); }

void *func_0034A1A0(void) {
    return D_0043EA90;
}

void *func_0034A1B0(void) {
    return D_0043EAE0;
}

void *func_0034A1C0(void) {
    return D_0043EBA0;
}

void *func_0034A1D0(void) {
    return D_0043EC20;
}

u32 func_0034A1E0(void *self, s32 i) {
    return D_0047AEF0[i];
}

u32 func_0034A200(void *self, s32 i) {
    return D_0047AEF4[i];
}

/* (self->*D_01991968[i])(a, b) */
s32 func_0034A220(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991968[i & 0xFF], a, b);
}

/* (as func_002B11D0) the 0x14-byte effect D_0047A430 started with byte 3 as a word */
s32 func_0034A250(void *self, void *a1, u8 *cmd) {
    u8 *mgr = gEffects;
    s32 w = cmd[3];

    func_002D6090(mgr, Effect_New(mgr, 0x14, effect_7a430_init), &w);
    return 1;
}

/* destructor (vtable D_0047A430) */
void *func_003784F0(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_0047A430;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

/* (+0x18) set: +0x10 = the first word */
void func_00378550(u8 *o, s32 *prm) {
    if (prm != NULL) {
        AT(o, 0x10, s32) = prm[0];
    }
}
