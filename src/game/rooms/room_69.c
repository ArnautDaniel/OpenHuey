/* Room 0x69: its event handler class (vtable D_00477540, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "input.h"
#include "gl2d.h"
#include "ptmf.h"
#include "hewie.h"
#include "progress.h"

extern void *D_0046DB80[];
extern void *D_00477540[];
extern u8 D_0047AE7C[];
extern u8 D_0047AE80[];                  /* the dials' progress variables (0x18..0x1A) */
extern u32 D_00437E50[];
extern u32 D_00437F50[];
extern u32 D_00437FD0[];
extern u32 D_00438230[];
extern u32 D_00438290[];
extern u32 D_004386D0[];
extern u32 D_00438720[];

extern PTMF D_019918D8[];

/* a dial's angle for its setting (0..3, a quarter turn each) */
static inline __attribute__((always_inline)) f32 dial_angle(Progress *p, s32 k) {
    return 0x1.921fb6p+1f * (f32)(s32)((u8)Progress_GetVar(p, D_0047AE80[k]) * 90) / 180.0f;
}

/* 0x003438C0 */
void *Room69_dtor(void *o, s32 flags) { return room_dtor(o, flags, D_00477540, D_0046DB80); }

/* 0x00343920 */
void *Room69_EnterScript(void) {
    return D_00437E50;
}

/* 0x00343930 */
void *Room69_CharEnterScript(void) {
    return D_00437F50;
}

/* 0x00343940 */
void *Room69_Phase1Script(void) {
    return D_00437FD0;
}

/* 0x00343950 */
void *Room69_Phase2Script(void) {
    return D_00438230;
}

/* 0x00343960 */
void *Room69_Phase5Script(void *o) { return D_0047AE7C; }   /* D_00477540 +0x20 */

/* 0x00343970 */
void *Room69_Phase3Script(void) {
    return D_00438290;
}

/* 0x00343980 */
u32 Room69_ActionScript(void *self, s32 i) {
    return D_004386D0[i];
}

/* 0x003439A0 */
void *Room69_Table38(void) {
    return D_00438720;
}

/* 0x003439B0 */
u32 Room69_ObjectName(void *self, s32 i) {
    return (u32)D_00438700[i];
}

/* (self->*D_019918D8[i])(a, b) */
/* 0x003439D0 */
s32 Room69_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019918D8[i & 0xFF], a, b);
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
        VObject *objs = D_00456DF8;
        Progress *p = gProgress;
        s32 i;

        for (i = 0; i < 6; i++) {
            u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00438700[i]);

            if (o != NULL) {
                AT(o, 0x14, f32) = func_002E2D00(dial_angle(p, i % 3));
            }
        }
        break;
    }
    case 1: {
        VObject *ev = gEvents;
        u8 old = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0);
        u8 sel = old;

        if (D_0047E36C & 0x20) {
            VCALL(ev, 0x60, void (*)(VObject *, s32))(ev, 2);
            break;
        }
        if (D_0047E36C & 1) {
            sel = sel == 0 ? 2 : sel - 1;
        } else if (D_0047E36C & 4) {
            sel = sel < 2 ? sel + 1 : 0;
        }
        if (sel != old) {
            VObject *objs = D_00456DF8;
            u8 *o;

            if ((o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00438700[old])) != NULL) {
                AT(o, 0x0, u8) = 0;
            }
            if ((o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00438700[old + 3])) != NULL) {
                AT(o, 0x0, u8) = 1;
            }
            if ((o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00438700[sel])) != NULL) {
                AT(o, 0x0, u8) = 1;
            }
            if ((o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00438700[sel + 3])) != NULL) {
                AT(o, 0x0, u8) = 0;
            }
            ev = gEvents;
            VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 0, sel);
            VCALL(ev, 0x5C, void (*)(VObject *, s32))(ev, 6);
        } else {
            u8 *var = &D_0047AE80[sel];
            Progress *p = gProgress;
            u8 v = Progress_GetVar(p, *var);

            if (D_0047E364 & 8) {
                v = v == 0 ? 3 : v - 1;
            } else if (D_0047E364 & 2) {
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
        VObject *objs = D_00456DF8;
        u8 *o1 = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00438700[sel]);
        u8 *o2 = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00438700[sel + 3]);
        Progress *p4 = gProgress;
        f32 d;

#ifdef HG_NATIVE
        if (o1 == NULL || o2 == NULL) {   /* (the PS2 writes through junk) */
            break;
        }
#endif
        d = func_002E2D00(AT(o1, 0x14, f32) - dial_angle(p4, sel));
        if (!(d <= k4.f)) {
            AT(o1, 0x14, f32) = AT(o1, 0x14, f32) - k4.f;
            AT(o2, 0x14, f32) = AT(o2, 0x14, f32) - k4.f;
        } else if (d < kM4.f) {
            AT(o1, 0x14, f32) = AT(o1, 0x14, f32) + k4.f;
            AT(o2, 0x14, f32) = AT(o2, 0x14, f32) + k4.f;
        } else {
            Progress *p = gProgress;
            f32 a = func_002E2D00(dial_angle(p, sel));

            AT(o2, 0x14, f32) = a;
            AT(o1, 0x14, f32) = a;
            if ((u8)Progress_GetVar(p, D_0047AE80[0]) == 1 && (u8)Progress_GetVar(p4, D_0047AE80[1]) == 0 &&
                (u8)Progress_GetVar(p4, D_0047AE80[2]) == 2) {
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
