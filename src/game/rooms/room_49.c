/* Room 0x49: its event handler class (vtable Room49_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "scene_game_members.h"

extern void *RoomBase_vtable[];
extern void *Room49_vtable[];
extern void *DepthRange_vtable[];
extern void *D_0047A390[];
extern u8 D_00407F30[];
extern u8 D_00407F80[];
extern u8 D_00407FE0[];
extern u8 D_004080A0[];
extern u32 D_00408610[];
extern u8 D_00408690[];
extern u32 D_00408670[];

extern PTMF D_01990C80[];

static void effect_1a60_init(void **obj) {
    obj[0] = D_0047A390;
    obj[0x1810 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0x1814 / 4] = -1;
    obj[0x1810 / 4] = QuadDrawer_vtable;
}

/* 0x002B2960 */
void *Room49_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room49_vtable, RoomBase_vtable); }

/* 0x002B29C0 */
void *Room49_EnterScript(void) {
    return D_00407F30;
}

/* 0x002B29D0 */
void *Room49_CharEnterScript(void) {
    return D_00407F80;
}

/* 0x002B29E0 */
void *Room49_Phase1Script(void) {
    return D_00407FE0;
}

/* 0x002B29F0 */
void *Room49_Phase2Script(void) {
    return D_004080A0;
}

/* 0x002B2A00 */
u32 Room49_ActionScript(void *self, s32 i) {
    return D_00408610[i];
}

/* 0x002B2A20 */
void *Room49_Table38(void) {
    return D_00408690;
}

/* 0x002B2A30 */
u32 Room49_ObjectName(void *self, s32 i) {
    return D_00408670[i];
}

/* (self->*D_01990C80[i])(a, b) */
/* 0x002B2A50 */
s32 Room49_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990C80[i & 0xFF], a, b);
}

/* byte 3 0: the effect D_0047A390 spawned, its slot in event var 0; 1: removed */
/* 0x002B2A80 */
s32 Room49_Cmd02(void *self, void *a1, u8 *cmd) {
    switch (cmd[3]) {
    case 0: {
        s32 slot = Effect_New(gEffects, 0x1A60, effect_1a60_init);

        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 0, slot);
        break;
    }
    case 1:
        EffectMgr_Remove(gEffects, VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 0));
        break;
    }
    return 1;
}

/* the depth range (effect 0x1C) opening with the cutscene from its frame 1156: 1 / 1 / 40 / 100,
 * the far two on by 1 a frame up to 80 / 140 */
/* 0x002B2BF0 */
s32 Room49_Cmd01(void) {
    u8 *fx = gRoomEffects;
    f32 t = (f32)(VCALL(gCutscene, 0x34, s32 (*)(VObject *))(gCutscene) - 1156);
    f32 r[4] __attribute__((aligned(16)));

    room_effect_slot_new(fx, 0x1C, DepthRange_vtable);
    r[0] = 1.0f;
    r[1] = 1.0f;
    r[2] = 40.0f + t;
    if (!(r[2] <= 80.0f)) {
        r[2] = 80.0f;
    }
    r[3] = 100.0f + t;
    if (!(r[3] <= 140.0f)) {
        r[3] = 140.0f;
    }
    RoomEffects_Send(fx, 0x1C, r);
    return 1;
}

/* a lit quad at x -43, z -15.12 .. 5.07, height 30.05 / 10.05 */
/* 0x002B2D50 */
s32 Room49_Cmd00(void *self, void *a1, u8 *cmd) {
    static const u32 sQuad[16] = {
        0xC22C0000, 0x41F06D5D, 0xC171FD22, 0x3F800000, 0xC22C0000, 0x41F06D5D, 0x40A228F6, 0x3F800000,
        0xC22C0000, 0x4120DABA, 0xC171FD22, 0x3F800000, 0xC22C0000, 0x4120DABA, 0x40A228F6, 0x3F800000,
    };

    return lit_quad(cmd, sQuad, 0x40);
}
