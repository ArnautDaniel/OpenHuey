/* Room 0x04: its event handler class (vtable D_0046DC80, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "scene_game_members.h"

extern void *D_0046DB80[];
extern void *D_0046DC80[];
extern const char *D_003F17B8, *D_003F17C8;
extern void *D_00479560[];

extern u8 D_003F0DF0[];
extern u8 D_003F0E80[];
extern u8 D_003F0F70[];
extern u8 D_003F1060[];
extern void *D_003F1740[];
extern void *D_003F17B0[];
extern u8 D_003F17F0[];
extern PTMF D_019907A0[];

void *func_002A9A50(void *o, s32 flags) { return room_dtor(o, flags, D_0046DC80, D_0046DB80); }

void *func_002A9AB0(void) {
    return D_003F0DF0;
}

void *func_002A9AC0(void) {
    return D_003F0E80;
}

void *func_002A9AD0(void) {
    return D_003F0F70;
}

void *func_002A9AE0(void) {
    return D_003F1060;
}

void *func_002A9AF0(void *self, s32 i) {
    return D_003F1740[i];
}

void *func_002A9B10(void) {
    return D_003F17F0;
}

void *func_002A9B20(void *self, s32 i) {
    return D_003F17B0[i];
}

/* (self->*D_019907A0[i])(a, b) */
s32 func_002A9B40(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019907A0[i & 0xFF], a, b);
}

/* room 0x04 (D_003F17A0): an effect on one object (byte 3 0: at 90 degrees) or the other (0) */
s32 func_002A9B70(void *self, void *a1, u8 *cmd) {
    u8 *o;
    u32 f;

    if (cmd[3] == 0) {
        o = room_obj(D_003F17B8);
        f = 0x3FC90FDB;
    } else {
        o = room_obj(D_003F17C8);
        f = 0;
    }
    obj_effect(o, f);
    return 1;
}

/* room 0x04 (D_003F1790): byte 3 0: room effect 0x1B (D_00479560) on its object, a box (640,
 * -560, 1000, 0, 0x60); else the effect gone */
s32 func_002A9CE0(void *self, void *a1, u8 *cmd) {
    if (cmd[3] == 0) {
        u8 *o = room_obj(D_003F17B4);

        if (o != NULL) {
            struct {
                f32 x, y, z;
                u8 *o;
                s32 a, b;
            } prm;

            prm.a = 0;
            prm.x = 640.0f;
            prm.y = -560.0f;
            prm.z = 1000.0f;
            prm.b = 0x60;
            prm.o = o;
            room_effect_slot_new(gRoomEffects, 0x1B, D_00479560);
            func_00266C70(gRoomEffects, 0x1B, &prm);
        }
    } else {
        func_002670F0(gRoomEffects, 0x1B);
    }
    return 1;
}

/* room 0x04 (D_003F1780): five objects turned -75 / 75 degrees in turn */
s32 func_002A9E20(void) {
    obj_angle(D_003F17CC, 0x14, 0xBFA78D37);
    obj_angle(D_003F17D0, 0x14, 0x3FA78D37);
    obj_angle(D_003F17D4, 0x14, 0xBFA78D37);
    obj_angle(D_003F17D8, 0x14, 0x3FA78D37);
    obj_angle(D_003F17DC, 0x14, 0xBFA78D37);
    return 1;
}

/* two dials (byte 3: D_003F17B8 on var 0, D_003F17C8 on var 1, from -90 degrees), byte 4 the step */
s32 func_002A9F30(void *self, void *a1, u8 *cmd) {
    u8 *o;
    u32 var;

    if (cmd[3] == 0) {
        o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_003F17B8);
        var = 0;
    } else {
        o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_003F17C8);
        var = 1;
    }
    if (o == NULL) {
        return 1;
    }
    return dial_step(cmd[4], o, var & 0xFF, -90, 1);
}
