/* Room 0x10B: its event handler class (vtable D_0046FEC0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "scene_game_members.h"

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

/* 0x002E78E0 */
void *Room10B_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_0046FEC0, D_0046DB80); }

/* 0x002E7940 */
void *Room10B_EnterScript(void) { return D_00419A80; }

/* 0x002E7950 */
void *Room10B_CharEnterScript(void) { return D_00419AA0; }

/* 0x002E7960 */
void *Room10B_Phase1Script(void) { return D_00419AE0; }

/* 0x002E7970 */
void *Room10B_Phase3Script(void) { return D_00419B70; }

/* 0x002E7980 */
void *Room10B_ActionScript(void *self, s32 i) { return D_00419D78[i]; }

/* 0x002E79A0 */
void *Room10B_Table38(void) { return D_00419DA8; }

/* 0x002E79B0 */
void *Room10B_ObjectName(void *self, s32 i) { return D_0047AC88[i]; }

/* (self->*D_01990F78[i])(a, b) */
/* 0x002E79D0 */
s32 Room10B_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990F78[i & 0xFF], a, b);
}

/* 0x002E7A00 */
s32 Room10B_Cond00(void) {
    return AT(gProgress, 0xFB6, s16) >= 100;
}

/* (self->*D_01990F68[i])(a, b) */
/* 0x002E7A20 */
s32 Room10B_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990F68[i & 0xFF], a, b);
}

/* byte 3 0: a 0x4480 effect is spawned and its slot kept in event var 0; else that slot's
 * effect is removed */
/* 0x002E7A50 */
s32 Room10B_Cmd00(void *self, void *a1, u8 *cmd) {
    if (cmd[3] == 0) {
        s32 slot = Effect_New(gEffects, 0x4480, effect_4480_init);

        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 0, slot);
    } else {
        EffectMgr_Remove(gEffects, VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 0));
    }
    return 1;
}
