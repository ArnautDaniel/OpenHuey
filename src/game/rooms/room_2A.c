/* Room 0x2A: its event handler class (vtable D_0046E4C0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "scene_game_members.h"

extern void *D_0046DB80[];
extern void *D_0046E4C0[];
extern void *D_00474000[];   /* the room 0x2A effect (props.c) */
extern void *D_00478BC0[];   /* a 0x14-byte effect (props.c) */
extern u8 D_00404740[];
extern u8 D_004048E0[];
extern u8 D_00404A20[];
extern u8 D_00404CE0[];
extern u8 D_00404E00[];
extern u32 D_00405570[];
extern u8 D_00405620[];
extern u32 D_00405610[];

extern PTMF D_01990BD0[];

static void room2a_effect_init(void **obj) {
    obj[0] = D_00474000;
    obj[0x610 / 4] = D_00469D00;
    ((s32 *)obj)[0x614 / 4] = -1;
    obj[0x610 / 4] = D_0046FC30;
}

static void effect_14_init(void **obj) {
    obj[0] = D_00478BC0;
}

void *func_002B10A0(void *o, s32 flags) { return room_dtor(o, flags, D_0046E4C0, D_0046DB80); }

void *func_002B1100(void) {
    return D_00404740;
}

void *func_002B1110(void) {
    return D_004048E0;
}

void *func_002B1120(void) {
    return D_00404A20;
}

void *func_002B1130(void) {
    return D_00404CE0;
}

void *func_002B1140(void) {
    return D_00404E00;
}

u32 func_002B1150(void *self, s32 i) {
    return D_00405570[i];
}

void *func_002B1170(void) {
    return D_00405620;
}

u32 func_002B1180(void *self, s32 i) {
    return D_00405610[i];
}

/* (self->*D_01990BD0[i])(a, b) */
s32 func_002B11A0(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990BD0[i & 0xFF], a, b);
}

/* the 0x14-byte effect (D_00478BC0) started with the command's parameters (from byte 3) */
s32 func_002B11D0(void *self, void *a1, u8 *cmd) {
    u8 *mgr = gEffects;

    func_002D6090(mgr, Effect_New(mgr, 0x14, effect_14_init), cmd + 3);
    return 1;
}

/* room 0x2A: its effect (D_00474000) started at (220, 0, -100) */
s32 func_002B12D0(void) {
    u8 *mgr = gEffects;
    s32 slot = Effect_New(mgr, 0x900, room2a_effect_init);
    f32 at[4] __attribute__((aligned(16)));

    at[0] = 220.0f;
    at[2] = -100.0f;
    at[1] = 0.0f;
    at[3] = 1.0f;
    func_002D6090(mgr, slot, at);
    return 1;
}

/* the box and the grate, by byte 3: 0 the box's +0x28 on by 0.4; the grate's +0x14 (an angle)
 * 1 back 1.5 degrees, 2 on 0.5, 3 back 0.5 */
s32 func_002B14A0(void *self, void *a1, u8 *cmd) {
    VObject *objs = D_00456DF8;
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
