/* Room 0x8F: its event handler class (vtable D_00477440, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "actor.h"
#include "fiona.h"
#include "progress.h"

extern void *D_0046DB80[];
extern void *D_00477440[];
extern const char *D_004365E4, *D_004365E8;

extern u32 D_00435990[];
extern u32 D_00435A60[];
extern u32 D_00435AF0[];
extern u32 D_00435C20[];
extern u32 D_00435CA0[];
extern u32 D_00436520[];
extern u32 D_004365F0[];

extern u32 D_004365C0[];

extern PTMF D_019917B0[];
extern PTMF D_019917E0[];

/* 0x00340D60 */
void *Room8F_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00477440, D_0046DB80); }

/* 0x00340DC0 */
void *Room8F_EnterScript(void) {
    return D_00435990;
}

/* 0x00340DD0 */
void *Room8F_CharEnterScript(void) {
    return D_00435A60;
}

/* 0x00340DE0 */
void *Room8F_Phase1Script(void) {
    return D_00435AF0;
}

/* 0x00340DF0 */
void *Room8F_Phase2Script(void) {
    return D_00435C20;
}

/* 0x00340E00 */
void *Room8F_Phase3Script(void) {
    return D_00435CA0;
}

/* 0x00340E10 */
u32 Room8F_ActionScript(void *self, s32 i) {
    return D_00436520[i];
}

/* 0x00340E30 */
void *Room8F_Table38(void) {
    return D_004365F0;
}

/* 0x00340E40 */
u32 Room8F_ObjectName(void *self, s32 i) {
    return D_004365C0[i];
}

/* (self->*D_019917E0[i])(a, b) */
/* 0x00340E60 */
s32 Room8F_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019917E0[i & 0xFF], a, b);
}

/* the pursuer is active and out of view: state 3 (+0xE8), or the camera's on-screen test
 * (+0xD4) fails */
/* 0x00340E90 */
s32 Room8F_Cond00(void) {
    if (gCharPursuer == NULL || AT(gCharPursuer, 0x28, u8) == 0) {
        return 0;
    }
    if (AT(gCharPursuer, 0xE8, s32) == 3) {
        return 1;
    }
    if ((u8)VCALL(gCamera, 0xD4, s32 (*)(VObject *, void *))(gCamera, (u8 *)gCharPursuer + 0x10) == 0) {
        return 1;
    }
    return 0;
}

/* (self->*D_019917B0[i])(a, b) */
/* 0x00340F20 */
s32 Room8F_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019917B0[i & 0xFF], a, b);
}

/* 0x00340F50 */
s32 Room8F_Cmd03(void) { return slam_shake(); }

/* 0x00340FE0 */
s32 Room8F_Cmd02(void) {
    static const s16 spot[3] = {0, 1, 6};
    static const f32 b[3] = {0.0f, 0.0f, 0.0f}, c[3] = {0.0f, 0.0f, 0.0f}, d[3] = {1.0f, 1.0f, 1.0f};

    return grey_three(50.0f, spot, b, c, d);
}

/* byte 3: 0 / 1 a named progress call; 2 waits (2) for func_0016CD60(0, 0); else func_0016CD30 */
/* 0x00341300 */
s32 Room8F_Cmd01(void *self, void *a1, u8 *cmd) {
    switch (cmd[3]) {
    case 0:
        func_0016CEC0(gProgress, D_004365E4);
        return 1;
    case 1:
        func_0016CEC0(gProgress, D_004365E8);
        return 1;
    case 2:
        return func_0016CD60(gProgress, 0, 0) == 0 ? 2 : 1;
    }
    ((void (*)(Progress *))func_0016CD30)(gProgress);
    return 1;
}

/* a struggle: byte 3 0 resets Fiona's shake tracking (+0x1AD710 / +0x1AD714); 1 adds her shakes
 * to script variable byte 4, with a grunt (voice 0x3D or 0x45 at random) when the cool-down
 * variable byte 6 is out (it then runs 45 / 60), and at 100 the event byte 5 (+0x5C) */
/* 0x003413C0 */
s32 Room8F_Cmd00(void *self, void *a1, u8 *cmd) {
    VObject *ev;
    s32 v, n;

    switch (cmd[3]) {
    case 0:
        AT(gCharPlayer, 0x1AD710, u8) = 1;
        AT(gCharPlayer, 0x1AD714, s32) = 0;
        break;
    case 1:
        ev = gEvents;
        v = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, cmd[4]);
        n = func_00183190((Fiona *)gCharPlayer);
        if (n != 0 && VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, cmd[6]) == 0) {
            if (VCALL(gRandom, 0x10, u32 (*)(VObject *))(gRandom) & 1) {
                func_00122C20(&gCharPlayer->a, 0x3D, 5, 0, 0, NULL);
                VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, cmd[6], 45);
            } else {
                func_00122C20(&gCharPlayer->a, 0x45, 5, 0, 0, NULL);
                VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, cmd[6], 60);
            }
        }
        n += v;
        VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, cmd[4], n);
        if ((u32)n >= 100) {
            VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, cmd[5]);
        }
        break;
    }
    return 1;
}
