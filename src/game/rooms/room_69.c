/* Room 0x69: its event handler class (vtable Room69_vtable, see sRooms in event.c),
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
extern void *Room69_vtable[];
extern u8 Room69_Phase5Script_data[];
extern u8 kDialVars[];                  /* the dials' progress variables (0x18..0x1A) */
extern u32 Room69_EnterScript_data[];
extern u32 Room69_CharEnterScript_data[];
extern u32 Room69_Phase1Script_data[];
extern u32 Room69_Phase2Script_data[];
extern u32 Room69_Phase3Script_data[];
extern u32 Room69_ActionScripts[];
extern u32 Room69_Table38_data[];

extern PTMF Room69_CmdTable[];

/* a dial's angle for its setting (0..3, a quarter turn each) */
static inline __attribute__((always_inline)) f32 dial_angle(Progress *p, s32 k) {
    return 0x1.921fb6p+1f * (f32)(s32)((u8)Progress_GetVar(p, kDialVars[k]) * 90) / 180.0f;
}

/* 0x003438C0 */
void *Room69_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room69_vtable, RoomBase_vtable); }

/* 0x00343920 */
void *Room69_EnterScript(void) {
    return Room69_EnterScript_data;
}

/* 0x00343930 */
void *Room69_CharEnterScript(void) {
    return Room69_CharEnterScript_data;
}

/* 0x00343940 */
void *Room69_Phase1Script(void) {
    return Room69_Phase1Script_data;
}

/* 0x00343950 */
void *Room69_Phase2Script(void) {
    return Room69_Phase2Script_data;
}

/* 0x00343960 */
void *Room69_Phase5Script(void *o) { return Room69_Phase5Script_data; }   /* Room69_vtable +0x20 */

/* 0x00343970 */
void *Room69_Phase3Script(void) {
    return Room69_Phase3Script_data;
}

/* 0x00343980 */
u32 Room69_ActionScript(void *self, s32 i) {
    return Room69_ActionScripts[i];
}

/* 0x003439A0 */
void *Room69_Table38(void) {
    return Room69_Table38_data;
}

/* 0x003439B0 */
u32 Room69_ObjectName(void *self, s32 i) {
    return (u32)kDialNames[i];
}

/* (self->*Room69_CmdTable[i])(a, b) */
/* 0x003439D0 */
s32 Room69_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &Room69_CmdTable[i & 0xFF], a, b);
}

/* room 0x69 (D_004386F8): the three-dial lock by byte 3 - 0 the dials (and their lit twins) set
 * to their settings; 1 the player at it: up / down pick the dial (event variable 0; its lit twin
 * shown), left / right turn it (its setting, sound bit 3), cancel leaves (event bit 2); 2 the
 * picked dial turns 4 degrees a frame to its setting, then - 1, 0, 2 - the lock opens (event
 * bits 2 off, 4) */
/* 0x00343A00 */
s32 Room69_Cmd00(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } k4 = {0x3D8EFA35}, kM4 = {0xBD8EFA35};

    switch (cmd[3]) {
    case 0: {
        VObject *objs = gRoomObjects;
        Progress *p = gProgress;
        s32 i;

        for (i = 0; i < 6; i++) {
            u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, kDialNames[i]);

            if (o != NULL) {
                AT(o, 0x14, f32) = Angle_Wrap(dial_angle(p, i % 3));
            }
        }
        break;
    }
    case 1: {
        VObject *ev = gEvents;
        u8 old = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0);
        u8 sel = old;

        if (gMenuPressed & 0x20) {
            VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 2);
            break;
        }
        if (gMenuPressed & 1) {
            sel = sel == 0 ? 2 : sel - 1;
        } else if (gMenuPressed & 4) {
            sel = sel < 2 ? sel + 1 : 0;
        }
        if (sel != old) {
            VObject *objs = gRoomObjects;
            u8 *o;

            if ((o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, kDialNames[old])) != NULL) {
                AT(o, 0x0, u8) = 0;
            }
            if ((o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, kDialNames[old + 3])) != NULL) {
                AT(o, 0x0, u8) = 1;
            }
            if ((o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, kDialNames[sel])) != NULL) {
                AT(o, 0x0, u8) = 1;
            }
            if ((o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, kDialNames[sel + 3])) != NULL) {
                AT(o, 0x0, u8) = 0;
            }
            ev = gEvents;
            VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 0, sel);
            VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 6);
        } else {
            u8 *var = &kDialVars[sel];
            Progress *p = gProgress;
            u8 v = Progress_GetVar(p, *var);

            if (gMenuRepeat & 8) {
                v = v == 0 ? 3 : v - 1;
            } else if (gMenuRepeat & 2) {
                v = v < 3 ? v + 1 : 0;
            }
            if (v != (u8)Progress_GetVar(p, *var)) {
                AT(p, 0x9C + *var, u8) = v;
                VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 3);
            }
            VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 6);
        }
        break;
    }
    case 2: {
        VObject *ev = gEvents;
        u8 sel = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0);
        VObject *objs = gRoomObjects;
        u8 *o1 = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, kDialNames[sel]);
        u8 *o2 = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, kDialNames[sel + 3]);
        Progress *p4 = gProgress;
        f32 d;

#ifdef HG_NATIVE
        if (o1 == NULL || o2 == NULL) {   /* (the PS2 writes through junk) */
            break;
        }
#endif
        d = Angle_Wrap(AT(o1, 0x14, f32) - dial_angle(p4, sel));
        if (!(d <= k4.f)) {
            AT(o1, 0x14, f32) = AT(o1, 0x14, f32) - k4.f;
            AT(o2, 0x14, f32) = AT(o2, 0x14, f32) - k4.f;
        } else if (d < kM4.f) {
            AT(o1, 0x14, f32) = AT(o1, 0x14, f32) + k4.f;
            AT(o2, 0x14, f32) = AT(o2, 0x14, f32) + k4.f;
        } else {
            Progress *p = gProgress;
            f32 a = Angle_Wrap(dial_angle(p, sel));

            AT(o2, 0x14, f32) = a;
            AT(o1, 0x14, f32) = a;
            if ((u8)Progress_GetVar(p, kDialVars[0]) == 1 && (u8)Progress_GetVar(p4, kDialVars[1]) == 0 &&
                (u8)Progress_GetVar(p4, kDialVars[2]) == 2) {
                VObject *e = gEvents;

                VCALL(e, 0x60, void (*)(VObject *, s32))(e, 2);
                VCALL(e, 0x5C, void (*)(VObject *, s32))(e, 4);
            } else {
                VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 3);
            }
        }
        break;
    }
    default:
        return 1;
    }
    return 1;
}
