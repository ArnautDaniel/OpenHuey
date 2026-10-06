/* Room 0x5A: its event handler class (vtable Room5A_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "input.h"
#include "gl2d.h"
#include "ptmf.h"
#include "hewie.h"
#include "progress.h"

extern void *RoomBase_vtable[];
extern void *Room5A_vtable[];
extern u8 D_0047ABE0[];
extern const char *D_00410F38[];   /* the three dials' object names */
extern u8 D_0047B254[3];           /* their progress vars */

extern u8 D_00410BE0[];
extern u8 D_00410D00[];
extern u8 D_00410D90[];
extern u8 D_00410DC0[];

extern u32 D_00410F10[];
extern u8 D_00410F50[];

extern PTMF D_01990DE8[];

/* 0x002B5280 */
void *Room5A_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room5A_vtable, RoomBase_vtable); }

/* 0x002B52E0 */
void *Room5A_EnterScript(void *o) { return D_0047ABE0; }   /* Room5A_vtable +0xC */

/* 0x002B52F0 */
void *Room5A_CharEnterScript(void) {
    return D_00410BE0;
}

/* 0x002B5300 */
void *Room5A_Phase1Script(void) {
    return D_00410D00;
}

/* 0x002B5310 */
void *Room5A_Phase2Script(void) {
    return D_00410D90;
}

/* 0x002B5320 */
void *Room5A_Phase5Script(void) {
    return D_00410DC0;
}

/* 0x002B5330 */
u32 Room5A_ActionScript(void *self, s32 i) {
    return D_00410F10[i];
}

/* 0x002B5350 */
void *Room5A_Table38(void) {
    return D_00410F50;
}

/* 0x002B5360 */
u32 Room5A_ObjectName(void *self, s32 i) {
    return (u32)D_00410F38[i];
}

/* (self->*D_01990DE8[i])(a, b) */
/* 0x002B5380 */
s32 Room5A_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990DE8[i & 0xFF], a, b);
}

/* the three dials (D_00410F38, progress vars D_0047B254: 0..3, 90 degrees each), event var 0 the
 * one picked: byte 3 0 set (the original sets the picked one three times), 1 up / down picks
 * one (event var 0), left / right turns it (event +0x5C 3), cancel leaves (+0x60 2); 2 turning
 * to it 4 degrees a step, and there: solved at 1 / 0 / 2 (+0x60 2, +0x5C 4), else +0x60 3; 3
 * wait */
/* 0x002B53B0 */
s32 Room5A_Cmd00(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kStep = {0x3D8EFA35};
    VObject *ev = gEvents;
    u32 sel = (u8)VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0);
    VObject *objs = D_00456DF8;
    const char **name = &D_00410F38[sel];
    u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, *name);
    s32 i;

    switch (cmd[3]) {
    case 0: {
        Progress *p = gProgress;

        for (i = 0; i < 3; i++) {
            u8 *d = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, *name);

            if (d != NULL) {
                u32 v = (u8)Progress_GetVar(p, D_0047B254[i]);

                AT(d, 0x14, f32) = Angle_Wrap(kPi.f * (f32)(s32)(v * 90) / 180.0f);
            }
        }
        return 1;
    }
    case 1:
        if ((D_0047E36C >> 5) & 1) {
            VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 2);
            return 1;
        }
        if (D_0047E36C & 1) {
            sel = sel != 0 ? (sel - 1) & 0xFF : 2;
        } else if ((D_0047E36C >> 2) & 1) {
            sel = sel < 2 ? (sel + 1) & 0xFF : 0;
        }
        if (sel == (u32)VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0)) {
            u8 *var = &D_0047B254[sel];
            Progress *p = gProgress;
            u32 v = (u8)Progress_GetVar(p, *var);

            if ((D_0047E364 >> 3) & 1) {
                v = v != 0 ? (v - 1) & 0xFF : 3;
            } else if ((D_0047E364 >> 1) & 1) {
                v = v < 3 ? (v + 1) & 0xFF : 0;
            }
            if (v != (u8)Progress_GetVar(p, *var)) {
                AT(p, 0x9C + *var, u8) = v;
                VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 3);
            }
        } else {
            VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 0, sel);
        }
        return 1;
    case 2: {
        u8 *var = &D_0047B254[sel];
        Progress *p = gProgress;
        u32 v = (u8)Progress_GetVar(p, *var);
        f32 d = Angle_Wrap(AT(o, 0x14, f32) - kPi.f * (f32)(s32)(v * 90) / 180.0f);

        if (!(d <= kStep.f)) {
            AT(o, 0x14, f32) = AT(o, 0x14, f32) - kStep.f;
        } else if (d < -kStep.f) {
            AT(o, 0x14, f32) = AT(o, 0x14, f32) + kStep.f;
        } else {
            Progress *q = gProgress;

            v = (u8)Progress_GetVar(q, *var);
            AT(o, 0x14, f32) = Angle_Wrap(kPi.f * (f32)(s32)(v * 90) / 180.0f);
            if ((u8)Progress_GetVar(q, D_0047B254[0]) == 1 && (u8)Progress_GetVar(p, D_0047B254[1]) == 0 &&
                (u8)Progress_GetVar(p, D_0047B254[2]) == 2) {
                VObject *e = gEvents;

                VCALL(e, 0x60, void (*)(VObject *, s32))(e, 2);
                VCALL(e, 0x5C, void (*)(VObject *, s32))(e, 4);
            } else {
                VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 3);
            }
        }
        return 1;
    }
    case 3:
        return 2;
    }
    return 1;
}
