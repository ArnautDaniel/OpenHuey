/* Room 0x32: its event handler class (vtable D_00473C90, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"

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

extern void func_0016CEC0(Progress *p, const char *name);
extern s32 func_0016CD60(Progress *p, s32 who, s32 arg);
extern void func_0016CD30(Progress *p);

extern PTMF D_019915A0[];

void *func_00320E50(void *o, s32 flags) { return room_dtor(o, flags, D_00473C90, D_0046DB80); }

void *func_00320EB0(void) {
    return D_0042B0B0;
}

void *func_00320EC0(void) {
    return D_0042B160;
}

void *func_00320ED0(void) {
    return D_0042B200;
}

void *func_00320EE0(void) {
    return D_0042B520;
}

void *func_00320EF0(void *self, s32 i) {
    return D_0042C200[i];
}

void *func_00320F10(void) {
    return D_0042C360;
}

void *func_00320F20(void *self, s32 i) {
    return D_0042C2F0[i];
}

/* (self->*D_019915A0[i])(a, b) */
s32 func_00320F40(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019915A0[i & 0xFF], a, b);
}

/* the player's model tint: white (byte 3 0) or blue halved */
s32 func_00320F70(void *self, void *a1, u8 *cmd) {
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
s32 func_00320FF0(void *self, void *a1, u8 *cmd) {
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
s32 func_003212A0(void *self, void *a1, u8 *cmd) {
    switch (cmd[3]) {
    case 0:
        func_0016CEC0(gProgress, D_0042C358);
        return 1;
    case 1:
        return func_0016CD60(gProgress, 3, 0) == 0 ? 2 : 1;
    }
    func_0016CD30(gProgress);
    return 1;
}

/* room 0x32 (D_0042C2B8) */
s32 func_00321340(void *self, void *a1, u8 *cmd) {
    return lit_quad(cmd, sQuadWindow, 0x10000040);
}

/* room 0x32 (D_0042C2A8): the fan turns, except while a movie plays */
s32 func_003214E0(void) {
    if (VCALL((VObject *)gProgress, 0x54, s32 (*)(VObject *))((VObject *)gProgress) != 0) {
        return 1;
    }
    fan_turn(D_0042C354);
    return 1;
}

s32 func_00321590(void) {
    room_effect_slot_new(gRoomEffects, 1, D_0046EA40);
    return 1;
}
