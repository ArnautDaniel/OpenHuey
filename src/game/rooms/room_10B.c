/* Room 0x10B: its event handler class (vtable D_0046FEC0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_0046FEC0[];
extern void *D_0047A730[];
extern u8 D_00419A80[];
extern u8 D_00419AA0[];
extern u8 D_00419AE0[];
extern u8 D_00419B70[];
extern void *D_00419D78[];
extern u8 D_00419DA8[];
extern void *D_0047AC88[];

extern PTMF D_01990F68[];
extern PTMF D_01990F78[];

static void effect_4480_init(void **obj) {
    obj[0] = D_0047A730;
    obj[0x3010 / 4] = D_00469D00;
    ((s32 *)obj)[0x3014 / 4] = -1;
    obj[0x3010 / 4] = D_0046FC30;
}

void *func_002E78E0(void *o, s32 flags) { return room_dtor(o, flags, D_0046FEC0, D_0046DB80); }

void *func_002E7940(void) { return D_00419A80; }

void *func_002E7950(void) { return D_00419AA0; }

void *func_002E7960(void) { return D_00419AE0; }

void *func_002E7970(void) { return D_00419B70; }

void *func_002E7980(void *self, s32 i) { return D_00419D78[i]; }

void *func_002E79A0(void) { return D_00419DA8; }

void *func_002E79B0(void *self, s32 i) { return D_0047AC88[i]; }

/* (self->*D_01990F78[i])(a, b) */
s32 func_002E79D0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990F78[i & 0xFF], a, b);
}

s32 func_002E7A00(void) {
    return AT(gProgress, 0xFB6, s16) >= 100;
}

/* (self->*D_01990F68[i])(a, b) */
s32 func_002E7A20(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990F68[i & 0xFF], a, b);
}

/* byte 3 0: a 0x4480 effect is spawned and its slot kept in event var 0; else that slot's
 * effect is removed */
s32 func_002E7A50(void *self, void *a1, u8 *cmd) {
    if (cmd[3] == 0) {
        s32 slot = Effect_New(gEffects, 0x4480, effect_4480_init);

        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 0, slot);
    } else {
        func_002D6170(gEffects, VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 0));
    }
    return 1;
}
