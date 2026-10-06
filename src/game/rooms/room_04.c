/* Room 0x04: its event handler class (vtable Room04_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "scene_game_members.h"

extern void *RoomBase_vtable[];
extern void *Room04_vtable[];
extern const char *D_003F17B8, *D_003F17C8;
extern void *MirrorFragment_vtable[];

extern u8 Room04_EnterScript_data[];
extern u8 Room04_CharEnterScript_data[];
extern u8 Room04_Phase1Script_data[];
extern u8 Room04_Phase2Script_data[];
extern void *Room04_ActionScripts[];
extern void *Room04_ObjectNames[];
extern u8 Room04_Table38_data[];
extern PTMF Room04_CmdTable[];

/* 0x002A9A50 */
void *Room04_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room04_vtable, RoomBase_vtable); }

/* 0x002A9AB0 */
void *Room04_EnterScript(void) {
    return Room04_EnterScript_data;
}

/* 0x002A9AC0 */
void *Room04_CharEnterScript(void) {
    return Room04_CharEnterScript_data;
}

/* 0x002A9AD0 */
void *Room04_Phase1Script(void) {
    return Room04_Phase1Script_data;
}

/* 0x002A9AE0 */
void *Room04_Phase2Script(void) {
    return Room04_Phase2Script_data;
}

/* 0x002A9AF0 */
void *Room04_ActionScript(void *self, s32 i) {
    return Room04_ActionScripts[i];
}

/* 0x002A9B10 */
void *Room04_Table38(void) {
    return Room04_Table38_data;
}

/* 0x002A9B20 */
void *Room04_ObjectName(void *self, s32 i) {
    return Room04_ObjectNames[i];
}

/* (self->*Room04_CmdTable[i])(a, b) */
/* 0x002A9B40 */
s32 Room04_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room04_CmdTable[i & 0xFF], a, b);
}

/* room 0x04 (Room04_Cmd03_ptmf): an effect on one object (byte 3 0: at 90 degrees) or the other (0) */
/* 0x002A9B70 */
s32 Room04_Cmd03(void *self, void *a1, u8 *cmd) {
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

/* room 0x04 (Room04_Cmd02_ptmf): byte 3 0: room effect 0x1B (MirrorFragment_vtable) on its object, a box (640,
 * -560, 1000, 0, 0x60); else the effect gone */
/* 0x002A9CE0 */
s32 Room04_Cmd02(void *self, void *a1, u8 *cmd) {
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
            room_effect_slot_new(gRoomEffects, 0x1B, MirrorFragment_vtable);
            RoomEffects_Send(gRoomEffects, 0x1B, &prm);
        }
    } else {
        RoomEffects_Release(gRoomEffects, 0x1B);
    }
    return 1;
}

/* room 0x04 (Room04_Cmd01_ptmf): five objects turned -75 / 75 degrees in turn */
/* 0x002A9E20 */
s32 Room04_Cmd01(void) {
    obj_angle(D_003F17CC, 0x14, 0xBFA78D37);
    obj_angle(D_003F17D0, 0x14, 0x3FA78D37);
    obj_angle(D_003F17D4, 0x14, 0xBFA78D37);
    obj_angle(D_003F17D8, 0x14, 0x3FA78D37);
    obj_angle(D_003F17DC, 0x14, 0xBFA78D37);
    return 1;
}

/* two dials (byte 3: D_003F17B8 on var 0, D_003F17C8 on var 1, from -90 degrees), byte 4 the step */
/* 0x002A9F30 */
s32 Room04_Cmd00(void *self, void *a1, u8 *cmd) {
    u8 *o;
    u32 var;

    if (cmd[3] == 0) {
        o = VCALL(gRoomObjects, 0x18, u8 *(*)(VObject *, const char *))(gRoomObjects, D_003F17B8);
        var = 0;
    } else {
        o = VCALL(gRoomObjects, 0x18, u8 *(*)(VObject *, const char *))(gRoomObjects, D_003F17C8);
        var = 1;
    }
    if (o == NULL) {
        return 1;
    }
    return dial_step(cmd[4], o, var & 0xFF, -90, 1);
}
