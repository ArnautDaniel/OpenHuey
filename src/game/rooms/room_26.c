/* Room 0x26: its event handler class (vtable Room26_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "snd_place.h"
#include "msl.h"
#include "sce/libvu0.h"

extern void *RoomBase_vtable[];
extern void *Room26_vtable[];
extern u8 D_00402D50[];
extern u8 D_00402E10[];
extern u8 D_00402EC0[];
extern u8 D_00402FA0[];

extern u32 D_004038B0[];
extern u8 D_00403970[];
extern u32 D_00403940[];

extern PTMF D_01990B58[];
extern PTMF D_01990B70[];

/* the creak of the chairs, at (2.09, 0.3, -2.09) */
static void chair_creak(VObject *snd, u32 id, f32 *pos, s32 vol) {
    AT(pos, 0x0, u32) = 0x40058ADB;
    AT(pos, 0x4, u32) = 0x3E99999A;
    AT(pos, 0x8, u32) = 0xC00582AA;
    Sound_PlayBankAt(snd, id, 6, pos, vol, 0);
}

/* 0x002B03C0 */
void *Room26_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room26_vtable, RoomBase_vtable); }

/* 0x002B0420 */
void *Room26_EnterScript(void) {
    return D_00402D50;
}

/* 0x002B0430 */
void *Room26_CharEnterScript(void) {
    return D_00402E10;
}

/* 0x002B0440 */
void *Room26_Phase1Script(void) {
    return D_00402EC0;
}

/* 0x002B0450 */
void *Room26_Phase2Script(void) {
    return D_00402FA0;
}

/* 0x002B0460 */
u32 Room26_ActionScript(void *self, s32 i) {
    return D_004038B0[i];
}

/* 0x002B0480 */
void *Room26_Table38(void) {
    return D_00403970;
}

/* 0x002B0490 */
u32 Room26_ObjectName(void *self, s32 i) {
    return D_00403940[i];
}

/* (self->*D_01990B70[i])(a, b) */
/* 0x002B04B0 */
s32 Room26_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990B70[i & 0xFF], a, b);
}

/* the partner's target (+0xF35E0 on, +0xF35F0) 3 above the rocking chair (movechair_2): its
 * seat 6 ahead, tipped by its rock (90 x +0x34 x sin +0x30 degrees) and turned with it */
/* 0x002B04E0 */
s32 Room26_Cmd02(void) {
    u8 *chair = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_00403960);
    f32 m[4][4] __attribute__((aligned(16)));
    f32 v[4] __attribute__((aligned(16)));
    f32 at[4] __attribute__((aligned(16)));

    v[0] = 0.0f;
    v[1] = 6.0f;
    v[2] = 0.0f;
    v[3] = 0.0f;
    sceVu0UnitMatrix(m);
    sceVu0RotMatrixX(m, m, 0x1.921fb6p+1f * (90.0f * AT(chair, 0x34, f32) * func_0031C248(0x1.921fb6p+1f * AT(chair, 0x30, f32) / 180.0f)) / 180.0f);
    sceVu0RotMatrixY(m, m, AT(chair, 0x14, f32));
    sceVu0ApplyMatrix(v, m, v);
    sceVu0AddVector(at, (f32 *)(chair + 0x20), v);
    at[1] = at[1] + 3.0f;
    AT(gCharPartner, 0xF35E0, u8) = 1;
    sceVu0CopyVector((f32 *)((u8 *)gCharPartner + 0xF35F0), at);
    return 1;
}

/* the three rocking chairs ("movechair_1..3"; +0x30 the rock's phase in degrees, +0x34 its
 * size, +0x38 how fast it dies down; +0x10 the tilt), by byte 3: 0 all still; 1 a rocking
 * step (6 degrees; each swing smaller, the first chair creaking at a volume by its size);
 * 2 / 3 set rocking at full size from their tilt now (swinging forward / back), with a creak */
/* 0x002B0640 */
s32 Room26_Cmd01(void *self, void *a1, u8 *cmd) {
    VObject *objs = D_00456DF8;
    u8 *chairs[3];
    f32 pos[4] __attribute__((aligned(16)));
    VObject *snd;
    s32 i;

    chairs[0] = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_0040395C);
    chairs[1] = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00403960);
    chairs[2] = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00403964);
    switch (cmd[3]) {
    case 0:
        for (i = 0; i < 3; i++) {
            if (chairs[i] != NULL) {
                AT(chairs[i], 0x3C, s32) = 0;
                AT(chairs[i], 0x38, s32) = 0;
                AT(chairs[i], 0x34, s32) = 0;
                AT(chairs[i], 0x30, s32) = 0;
            }
        }
        break;
    case 1:
        snd = gSound;
        for (i = 0; i < 3; i++) {
            u8 *c = chairs[i];

            if (c == NULL || AT(c, 0x34, f32) <= 0.0f) {
                continue;
            }
            AT(c, 0x30, f32) = AT(c, 0x30, f32) + 6.0f;
            if (!(AT(c, 0x30, f32) < 360.0f)) {
                AT(c, 0x30, f32) = AT(c, 0x30, f32) - 360.0f;
                AT(c, 0x34, f32) = AT(c, 0x34, f32) - AT(c, 0x38, f32);
                if (AT(c, 0x34, f32) < 0.0f) {
                    AT(c, 0x34, f32) = 0.0f;
                }
                if (i == 0) {
                    chair_creak(snd, 2, pos, (s8)(s32)(-100.0f * (1.0f - AT(c, 0x34, f32))));
                }
            }
            AT(c, 0x10, f32) = 0x1.921fb6p+1f * (10.0f * AT(c, 0x34, f32) * func_0031C248(0x1.921fb6p+1f * AT(c, 0x30, f32) / 180.0f)) / 180.0f;
            if (!(AT(c, 0x10, f32) <= 0x1.921fb6p+1f)) {
                AT(c, 0x10, f32) = AT(c, 0x10, f32) - 0x1.921fb6p+2f;
            }
        }
        break;
    case 2:
    case 3:
        snd = gSound;
        for (i = 0; i < 3; i++) {
            u8 *c = chairs[i];
            f32 a;

            if (c == NULL) {
                continue;
            }
            a = 180.0f * func_0031C4C0(180.0f * AT(c, 0x10, f32) / 0x1.921fb6p+1f / 10.0f) / 0x1.921fb6p+1f;
            AT(c, 0x30, f32) = cmd[3] == 2 ? a : 180.0f - a;
            AT(c, 0x34, f32) = 1.0f;
            AT(c, 0x38, u32) = 0x3D4CCCCD;   /* 0.05 */
            AT(c, 0x3C, s32) = 0;
            chair_creak(snd, 5, pos, 0);
        }
        break;
    }
    return 1;
}

/* the two doors ("left", "right") opening: byte 3 0 at once (2.25), else a step (0.075) */
/* 0x002B0B60 */
s32 Room26_Cmd00(void *self, void *a1, u8 *cmd) {
    VObject *objs = D_00456DF8;
    u8 *o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_00403948);

    if (o != NULL) {
        AT(o, 0x14, f32) = AT(o, 0x14, f32) - (cmd[3] == 0 ? 2.25f : 0x1.333334p-4f /* 0.075 */);
    }
    o = VCALL(objs, 0x18, u8 *(*)(VObject *, const char *))(objs, D_0040394C);
    if (o != NULL) {
        AT(o, 0x14, f32) = AT(o, 0x14, f32) + (cmd[3] == 0 ? 2.25f : 0x1.333334p-4f);
    }
    return 1;
}

/* (self->*D_01990B58[i])(a, b) */
/* 0x002B0C60 */
s32 Room26_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990B58[i & 0xFF], a, b);
}

/* the first rocking chair still rocking (+0x34 over 0.3) */
/* 0x002B0C90 */
s32 Room26_Cond00(void) {
    u8 *chair = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_0040395C);

    return !(AT(chair, 0x34, f32) <= 0x1.333334p-2f /* 0.3 */);
}
