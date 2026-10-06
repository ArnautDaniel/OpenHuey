/* Room 0x8D: its event handler class (vtable Room8D_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "actor.h"
#include "scene_game_members.h"

extern void *RoomBase_vtable[];
extern void *Room8D_vtable[];
/* ---- the marker effect Marker_vtable (effects.c) on character slot 3, its slot in script variable
 * 0, its size eased by variable 1 ---- */
extern void *Marker_vtable[];
extern u32 D_004348E0[];
extern u32 D_00434940[];
extern u32 D_00434A00[];
extern u32 D_00434B50[];
extern u32 D_00434BE0[];
extern u32 D_00434C00[];
extern u32 D_00434FE0[];
extern u32 D_00435040[];
extern u32 D_0047AE58[];

extern PTMF D_01991760[];

static void effect_70F90_init(void **obj) {
    obj[0] = Marker_vtable;
}

/* the marker's message { state, the model it follows, size, from slot } to the one in var 0 */
static inline s32 marker_send(s32 *msg) {
    s32 slot = VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 0);

    EffectMgr_Start(gEffects, slot, msg);
    return 1;
}

/* 0x00340060 */
void *Room8D_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room8D_vtable, RoomBase_vtable); }

/* 0x003400C0 */
void *Room8D_EnterScript(void) {
    return D_004348E0;
}

/* 0x003400D0 */
void *Room8D_CharEnterScript(void) {
    return D_00434940;
}

/* 0x003400E0 */
void *Room8D_Phase1Script(void) {
    return D_00434A00;
}

/* 0x003400F0 */
void *Room8D_Phase2Script(void) {
    return D_00434B50;
}

/* 0x00340100 */
void *Room8D_Phase3Script(void) {
    return D_00434BE0;
}

/* 0x00340110 */
void *Room8D_Phase5Script(void) {
    return D_00434C00;
}

/* 0x00340120 */
u32 Room8D_ActionScript(void *self, s32 i) {
    return D_00434FE0[i];
}

/* 0x00340140 */
void *Room8D_Table38(void) {
    return D_00435040;
}

/* 0x00340150 */
u32 Room8D_ObjectName(void *self, s32 i) {
    return D_0047AE58[i];
}

/* (self->*D_01991760[i])(a, b) */
/* 0x00340170 */
s32 Room8D_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01991760[i & 0xFF], a, b);
}

/* 0x003401A0 */
s32 Room8D_Cmd02(void) { return slam_shake(); }

/* byte 3: 0 made (lights 0x14 on characters 3 and 0) and 1 lit on character 3 (second kind, size
 * 1, sparks from its own slot); 2 ending, 3 ended; 4 / 6 sized 1 / 0.5; 5 / 7 shrinking with
 * variable 1 (a step a call, to 0.5 + v / 200 or v / 334) */
/* 0x00340230 */
s32 Room8D_Cmd01(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } k334 = {0x43A70000};
    VObject *ev;
    s32 msg[4];
    s32 slot, v;

    switch (cmd[3]) {
    case 0:
        Character_Set152C(gCharSlot3, 0x14);
        Character_Set152C(gCharSlot4, 0x14);
        slot = Effect_New(gEffects, 0x20, effect_70F90_init);
        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 0, slot);
        /* fall through */
    case 1:
        slot = VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 0);
        msg[0] = 6;
        msg[1] = AT(gCharSlot3, 0xF0, s32);
        msg[2] = 0x3F800000;   /* 1 */
        msg[3] = slot;
        EffectMgr_Start(gEffects, slot, msg);
        return 1;
    case 2:
        msg[0] = 2;
        return marker_send(msg);
    case 3:
        msg[0] = 3;
        return marker_send(msg);
    case 4:
        msg[0] = 4;
        msg[1] = 0x3F800000;
        return marker_send(msg);
    case 5:
    case 7:
        ev = gEvents;
        slot = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0);
        msg[0] = 4;
        v = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 1) - 1;
        if (v < 0) {
            v = 0;
        }
        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 1, v);
        if (cmd[3] == 5) {
            AT(&msg[1], 0, f32) = 0.5f + 0.5f * ((f32)v / 100.0f);
        } else {
            AT(&msg[1], 0, f32) = (f32)v / k334.f;
        }
        EffectMgr_Start(gEffects, slot, msg);
        return 1;
    case 6:
        msg[0] = 4;
        msg[1] = 0x3F000000;   /* 0.5 */
        return marker_send(msg);
    }
    return 1;
}

/* 0x003406A0 */
s32 Room8D_Cmd00(void) {
    static const s16 spot[3] = {7, 4, 3};
    static const f32 b[3] = {0.0f, 0.0f, 0.0f}, c[3] = {0.5f, 0.0f, 0.0f}, d[3] = {0.5f, 0.5f, 1.0f};

    return grey_three(60.0f, spot, b, c, d);
}
