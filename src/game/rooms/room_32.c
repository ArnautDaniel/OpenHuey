/* Room 0x32: its event handler class (vtable D_00473C90, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "progress.h"
#include "scene_game_members.h"

extern const char *const D_0042C358;

#include "gl2d.h"
#include "ptmf.h"

extern void *D_0046DB80[];
extern void *D_00473C90[];
extern void *D_0046EA40[];
extern void *D_00479560[];

extern u8 D_0042B0B0[];
extern u8 D_0042B160[];

extern u8 D_0042B200[];
extern u8 D_0042B520[];
extern void *D_0042C200[];
extern void *D_0042C2F0[];
extern u8 D_0042C360[];

extern PTMF D_019915A0[];

/* 0x00320E50 */
void *Room32_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00473C90, D_0046DB80); }

/* 0x00320EB0 */
void *Room32_EnterScript(void) {
    return D_0042B0B0;
}

/* 0x00320EC0 */
void *Room32_CharEnterScript(void) {
    return D_0042B160;
}

/* 0x00320ED0 */
void *Room32_Phase1Script(void) {
    return D_0042B200;
}

/* 0x00320EE0 */
void *Room32_Phase2Script(void) {
    return D_0042B520;
}

/* 0x00320EF0 */
void *Room32_ActionScript(void *self, s32 i) {
    return D_0042C200[i];
}

/* 0x00320F10 */
void *Room32_Table38(void) {
    return D_0042C360;
}

/* 0x00320F20 */
void *Room32_ObjectName(void *self, s32 i) {
    return D_0042C2F0[i];
}

/* (self->*D_019915A0[i])(a, b) */
/* 0x00320F40 */
s32 Room32_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019915A0[i & 0xFF], a, b);
}

/* the player's model tint: white (byte 3 0) or blue halved */
/* 0x00320F70 */
s32 Room32_Cmd05(void *self, void *a1, u8 *cmd) {
    VObject *m = gCharPlayer->motion;

    if (cmd[3] == 0) {
        VCALL(m, 0xC0, void (*)(VObject *, f32, f32, f32))(m, 1.0f, 1.0f, 1.0f);
    } else {
        VCALL(m, 0xC0, void (*)(VObject *, f32, f32, f32))(m, 1.0f, 1.0f, 0.5f);
    }
    return 1;
}

/* room 0x32 (D_0042C2D8): byte 3 0..3 the lit quad (room effect 0x1A) as room 0x21's; 4 and up
 * the mirror fragment's reflection (room effect 0x1A, D_00479560) on the object "a_fragment0":
 * 1.8 across, -0.1 down, strength 1, kind 2, alpha 0xFF */
/* 0x00320FF0 */
s32 Room32_Cmd04(void *self, void *a1, u8 *cmd) {
    u8 *o;

    if (cmd[3] < 4) {
        return lit_quad_in(0x1A, cmd, sQuadDoor, 0x20000040);
    }
    o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_0042C328);
    if (o != NULL) {
        u8 *fx;
        struct {
            f32 size, drop, strength;
            void *obj;
            s32 kind, alpha;
        } arg __attribute__((aligned(16)));

        AT(&arg.drop, 0, u32) = 0xBDCCCCCD;   /* -0.1 */
        AT(&arg.size, 0, u32) = 0x3FE66666;   /* 1.8 */
        arg.strength = 1.0f;
        fx = gRoomEffects;
        arg.obj = o;
        arg.kind = 2;
        arg.alpha = 0xFF;
        room_effect_slot_new(fx, 0x1A, D_00479560);
        func_00266C70(fx, 0x1A, &arg);
    }
    return 1;
}

/* a room callback: byte 3 0 a progress name, 1 wait for character 3 (2 while not), else done */
/* 0x003212A0 */
s32 Room32_Cmd03(void *self, void *a1, u8 *cmd) {
    switch (cmd[3]) {
    case 0:
        func_0016CEC0(gProgress, D_0042C358);
        return 1;
    case 1:
        return func_0016CD60(gProgress, 3, 0) == 0 ? 2 : 1;
    }
    ((void (*)(Progress *))func_0016CD30)(gProgress);
    return 1;
}

/* room 0x32 (D_0042C2B8) */
/* 0x00321340 */
s32 Room32_Cmd02(void *self, void *a1, u8 *cmd) {
    return lit_quad(cmd, sQuadWindow, 0x10000040);
}

/* room 0x32 (D_0042C2A8): the fan turns, except while a movie plays */
/* 0x003214E0 */
s32 Room32_Cmd01(void) {
    if (VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress) != 0) {
        return 1;
    }
    fan_turn(D_0042C354);
    return 1;
}

/* 0x00321590 */
s32 Room32_Cmd00(void) {
    room_effect_slot_new(gRoomEffects, 1, D_0046EA40);
    return 1;
}
