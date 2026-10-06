/* Room 0x2A: its event handler class (vtable Room2A_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "scene_game_members.h"

extern void *RoomBase_vtable[];
extern void *Room2A_vtable[];
extern void *Room2AWisps_vtable[];   /* the room 0x2A effect (props.c) */
extern void *Effect78BC0_vtable[];   /* a 0x14-byte effect (props.c) */
extern u8 Room2A_EnterScript_data[];
extern u8 Room2A_CharEnterScript_data[];
extern u8 Room2A_Phase1Script_data[];
extern u8 Room2A_Phase2Script_data[];
extern u8 Room2A_Phase5Script_data[];
extern u32 Room2A_ActionScripts[];
extern u8 Room2A_Table38_data[];
extern u32 Room2A_ObjectNames[];

extern PTMF Room2A_CmdTable[];

static void room2a_effect_init(void **obj) {
    obj[0] = Room2AWisps_vtable;
    obj[0x610 / 4] = Helper469D00_vtable;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = QuadDrawer_vtable;
}

static void effect_14_init(void **obj) {
    obj[0] = Effect78BC0_vtable;
}

/* 0x002B10A0 */
void *Room2A_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room2A_vtable, RoomBase_vtable); }

/* 0x002B1100 */
void *Room2A_EnterScript(void) {
    return Room2A_EnterScript_data;
}

/* 0x002B1110 */
void *Room2A_CharEnterScript(void) {
    return Room2A_CharEnterScript_data;
}

/* 0x002B1120 */
void *Room2A_Phase1Script(void) {
    return Room2A_Phase1Script_data;
}

/* 0x002B1130 */
void *Room2A_Phase2Script(void) {
    return Room2A_Phase2Script_data;
}

/* 0x002B1140 */
void *Room2A_Phase5Script(void) {
    return Room2A_Phase5Script_data;
}

/* 0x002B1150 */
u32 Room2A_ActionScript(void *self, s32 i) {
    return Room2A_ActionScripts[i];
}

/* 0x002B1170 */
void *Room2A_Table38(void) {
    return Room2A_Table38_data;
}

/* 0x002B1180 */
u32 Room2A_ObjectName(void *self, s32 i) {
    return Room2A_ObjectNames[i];
}

/* (self->*Room2A_CmdTable[i])(a, b) */
/* 0x002B11A0 */
s32 Room2A_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room2A_CmdTable[i & 0xFF], a, b);
}

/* the 0x14-byte effect (Effect78BC0_vtable) started with the command's parameters (from byte 3) */
/* 0x002B11D0 */
s32 Room2A_Cmd03(void *self, void *a1, u8 *cmd) {
    u8 *mgr = gEffects;

    EffectMgr_Start(mgr, Effect_New(mgr, 0x14, effect_14_init), cmd + 3);
    return 1;
}

/* room 0x2A: its effect (Room2AWisps_vtable) started at (220, 0, -100) */
/* 0x002B12D0 */
s32 Room2A_Cmd02(void) {
    u8 *mgr = gEffects;
    s32 slot = Effect_New(mgr, 0x900, room2a_effect_init);
    f32 at[4] __attribute__((aligned(16)));

    at[0] = 220.0f;
    at[2] = -100.0f;
    at[1] = 0.0f;
    at[3] = 1.0f;
    EffectMgr_Start(mgr, slot, at);
    return 1;
}

/* the box and the grate, by byte 3: 0 the box's +0x28 on by 0.4; the grate's +0x14 (an angle)
 * 1 back 1.5 degrees, 2 on 0.5, 3 back 0.5 */
/* 0x002B14A0 */
s32 Room2A_Cmd00(void *self, void *a1, u8 *cmd) {
    VObject *objs = gRoomObjects;
    u8 *o;

    switch (cmd[3]) {
    case 0:
        o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00405618);
        if (o != NULL) {
            AT(o, 0x28, f32) = AT(o, 0x28, f32) + 0x1.99999ap-2f /* 0.4 */;
        }
        break;
    case 1:
        o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_0040561C);
        if (o != NULL) {
            AT(o, 0x14, f32) = AT(o, 0x14, f32) - 0x1.aceeap-6f /* 1.5 degrees */;
        }
        break;
    case 2:
        o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_0040561C);
        if (o != NULL) {
            AT(o, 0x14, f32) = AT(o, 0x14, f32) + 0x1.1df46ap-7f /* 0.5 degrees */;
        }
        break;
    case 3:
        o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_0040561C);
        if (o != NULL) {
            AT(o, 0x14, f32) = AT(o, 0x14, f32) - 0x1.1df46ap-7f;
        }
        break;
    }
    return 1;
}
