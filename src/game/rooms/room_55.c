/* Room 0x55: its event handler class (vtable Room55_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "scene_game_members.h"

extern void *RoomBase_vtable[];
extern void *Room55_vtable[];
extern void *Room55Effect_vtable[], *FallingDrops_vtable[];
extern u8 Room55_EnterScript_data[];
extern u8 Room55_CharEnterScript_data[];
extern u8 Room55_Phase1Script_data[];
extern u8 Room55_Phase2Script_data[];
extern void *Room55_ActionScripts[];
extern void *Room55_ObjectNames[];
extern u8 Room55_Table38_data[];

extern PTMF Room55_CmdTable[];

static void effect_77E10_init(void **obj) {
    obj[0] = Room55Effect_vtable;
}

static void effect_79400_init(void **obj) {
    obj[0] = FallingDrops_vtable;
    obj[0x610 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = QuadDrawer_vtable;
}

/* 0x00305E90 */
void *Room55_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room55_vtable, RoomBase_vtable); }

/* 0x00305EF0 */
void *Room55_EnterScript(void) {
    return Room55_EnterScript_data;
}

/* 0x00305F00 */
void *Room55_CharEnterScript(void) {
    return Room55_CharEnterScript_data;
}

/* 0x00305F10 */
void *Room55_Phase1Script(void) {
    return Room55_Phase1Script_data;
}

/* 0x00305F20 */
void *Room55_Phase2Script(void) {
    return Room55_Phase2Script_data;
}

/* 0x00305F30 */
void *Room55_ActionScript(void *self, s32 i) {
    return Room55_ActionScripts[i];
}

/* 0x00305F50 */
void *Room55_Table38(void) {
    return Room55_Table38_data;
}

/* 0x00305F60 */
void *Room55_ObjectName(void *self, s32 i) {
    return Room55_ObjectNames[i];
}

/* (self->*Room55_CmdTable[i])(a, b) */
/* 0x00305F80 */
s32 Room55_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room55_CmdTable[i & 0xFF], a, b);
}

/* room 55 (D_004210F8): the 0x18-byte effect Room55Effect_vtable started with 0 or 1 by byte 3; 0 also
 * lays a floor quad (effect 0x1B: 60 x 60 at height 45), else the 0x6D0-byte effect FallingDrops_vtable
 * is made too */
/* 0x00305FB0 */
s32 Room55_Effect(void *self, void *a1, u8 *cmd) {
    u8 *mgr = gEffects;
    s32 slot = Effect_New(mgr, 0x18, effect_77E10_init);
    s32 on;

    if (cmd[3] == 0) {
        u8 *fx;
        u32 q[20] __attribute__((aligned(16)));

        on = 0;
        fx = gRoomEffects;
        room_effect_slot_new(fx, 0x1B, Reflection_vtable);
        q[0] = 0x41F00000;   /* (30, 45, -30) */
        q[1] = 0x42340000;
        q[2] = 0xC1F00000;
        q[3] = 0x3F800000;
        q[4] = 0xC1F00000;   /* (-30, 45, -30) */
        q[5] = 0x42340000;
        q[6] = 0xC1F00000;
        q[7] = 0x3F800000;
        q[8] = 0x41F00000;   /* (30, 45, 30) */
        q[9] = 0x42340000;
        q[10] = 0x41F00000;
        q[11] = 0x3F800000;
        q[12] = 0xC1F00000;  /* (-30, 45, 30) */
        q[13] = 0x42340000;
        q[14] = 0x41F00000;
        q[15] = 0x3F800000;
        q[16] = 0;
        q[17] = 0x80;
        q[18] = 0x3F800000;
        RoomEffects_Send(fx, 0x1B, q);
    } else {
        on = 1;
        Effect_New(mgr, 0x6D0, effect_79400_init);
    }
    EffectMgr_Start(mgr, slot, &on);
    return 1;
}
