/* Room 0x20: its event handler class (vtable Room20_vtable, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "actor.h"
#include "hewie.h"
#include "scene_game_members.h"
#include "snd_place.h"
#include "msl.h"
#include "sce/libvu0.h"

extern void *RoomBase_vtable[];
extern void *Room20_vtable[];
extern const char *D_003FF110[];
extern void *Actor_vtable[];   /* Actor base vtable */
extern u8 D_003FE460[];
extern u8 D_003FE540[];
extern u8 D_003FE680[];
extern u8 D_003FE9C0[];
extern u8 D_003FEA60[];
extern u8 D_003FEAD0[];
extern u32 D_003FF030[];
extern u8 D_003FF130[];
extern u8 D_003FF150[];

extern PTMF D_019909F0[];
extern PTMF D_01990A40[];

/* nav triangle `i`'s record, or NULL */
static inline u8 *nav_tri(u32 i) {
    u8 *nm = (u8 *)gNavMesh;

    return (i < AT(nm, 0x8, u32) && AT(nm, 0x4, u8 *) != NULL) ? AT(nm, 0x4, u8 *) + i * 0x50 : NULL;
}

/* 0x002AE390 */
void *Room20_dtor(void *o, s32 flags) { return room_dtor(o, flags, Room20_vtable, RoomBase_vtable); }

/* 0x002AE3F0 */
void *Room20_EnterScript(void) {
    return D_003FE460;
}

/* 0x002AE400 */
void *Room20_CharEnterScript(void) {
    return D_003FE540;
}

/* 0x002AE410 */
void *Room20_Phase1Script(void) {
    return D_003FE680;
}

/* 0x002AE420 */
void *Room20_Phase2Script(void) {
    return D_003FE9C0;
}

/* 0x002AE430 */
void *Room20_Phase3Script(void) {
    return D_003FEA60;
}

/* 0x002AE440 */
void *Room20_Phase5Script(void) {
    return D_003FEAD0;
}

/* 0x002AE450 */
u32 Room20_ActionScript(void *self, s32 i) {
    return D_003FF030[i];
}

/* 0x002AE470 */
void *Room20_Table38(void) {
    return D_003FF130;
}

/* 0x002AE480 */
void *Room20_Table3C(void) {
    return D_003FF150;
}

/* 0x002AE490 */
u32 Room20_ObjectName(void *self, s32 i) {
    return (u32)D_003FF110[i];
}

/* (self->*D_01990A40[i])(a, b) */
/* 0x002AE4B0 */
s32 Room20_Condition(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_01990A40[i & 0xFF], a, b);
}

/* room 0x20 (Room20_Cond02_ptmf): object byte 3 becomes event point byte 4 (radii 5) */
/* 0x002AE4E0 */
s32 Room20_Cond02(void *self, void *a1, u8 *cmd) {
    u8 *o = room_obj(D_003FF110[cmd[3]]);

    if (o == NULL) {
        return 0;
    }
    VCALL(gEvents, 0x6C, void (*)(VObject *, u8, f32 *, f32, f32))(gEvents, cmd[4], (f32 *)(o + 0x20), 5.0f,
                                                                       5.0f);
    return 1;
}

/* room 0x20 (Room20_Cond01_ptmf): the character's script value is at least be32 bytes 3..6 */
/* 0x002AE570 */
s32 Room20_Cond01(void *self, u8 *chr, u8 *cmd) {
    u32 v = VCALL(gEvents, 0x38, u32 (*)(VObject *, s32))(gEvents, chr[0x153C]) & 0xFFFF;

    return !(v < ((u32)cmd[3] << 24 | (u32)cmd[4] << 16 | (u32)cmd[5] << 8 | cmd[6]));
}

/* room 0x20 (Room20_Cond00_ptmf): the player, free and within 5 of the falling object, knocks it - it
 * gets a push (0, 1, 1) turned by her facing, and its nav triangle */
/* 0x002AE5E0 */
s32 Room20_Cond00(void) {
    u8 *o = room_obj(D_003FF124);
    Character *p;
    f32 v[4] __attribute__((aligned(16)));
    f32 m[4][4] __attribute__((aligned(16)));

    if (o == NULL) {
        return 0;
    }
    p = gCharPlayer;
    if (p == NULL || p->a.active != 1 || AT(p, 0x29, u8) != 0) {
        return 0;
    }
    if (VCALL((VObject *)p, 0x74, s32 (*)(VObject *, f32 *))((VObject *)p, v) == 0) {
        return 0;
    }
    sceVu0SubVector(v, v, (f32 *)(o + 0x20));
    if (25.0f < v[1] * v[1] + v[0] * v[0] + v[2] * v[2]) {
        return 0;
    }
    sceVu0CopyVector(v, (f32 *)((u8 *)p + 0x50));
    v[3] = 1.0f;
    sceVu0UnitMatrix(m);
    sceVu0RotMatrix(m, m, v);
    v[2] = 1.0f;
    v[3] = 1.0f;
    v[0] = 0.0f;
    v[1] = 1.0f;
    sceVu0ApplyMatrix((f32 *)(o + 0x30), m, v);
    AT(o, 0x3C, s32) = VCALL((VObject *)gNavMesh, 0x3C, s32 (*)(VObject *, f32 *, s32))(
        (VObject *)gNavMesh, (f32 *)(o + 0x20), 0);
    return 1;
}

/* (self->*D_019909F0[i])(a, b) */
/* 0x002AE760 */
s32 Room20_Command(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019909F0[i & 0xFF], a, b);
}

/* room 0x20 (Room20_Cmd05_ptmf): the character turns to face object byte 3 */
/* 0x002AE790 */
s32 Room20_Cmd05(void *self, u8 *chr, u8 *cmd) {
    u8 *o = room_obj(D_003FF110[cmd[3]]);

    if (o != NULL) {
        AT(chr, 0x10C, f32) = Angle_Wrap(func_0031C5C0(AT(o, 0x20, f32) - AT(chr, 0x10, f32),
                                                          AT(o, 0x28, f32) - AT(chr, 0x18, f32)));
        chr[0xE1] = 0;
        AT(chr, 0xF4, s32) = 0xF;
    }
    return 1;
}

/* room 0x20 (Room20_Cmd04_ptmf): a dust cloud where the falling object is */
/* 0x002AE830 */
s32 Room20_Cmd04(void) {
    u8 *o = room_obj(D_003FF124);
    s32 slot;
    struct {
        f32 pos[4];
        s32 kind, a, b, c, d;
    } prm __attribute__((aligned(16)));

    if (o == NULL) {
        return 1;
    }
    slot = Effect_New(gEffects, 0x720, dust_cloud_init);
    prm.pos[0] = AT(o, 0x20, f32);
    prm.pos[1] = AT(o, 0x24, f32);
    prm.pos[2] = AT(o, 0x28, f32);
    prm.pos[3] = 1.0f;
    prm.d = 0x10;
    prm.kind = 2;
    prm.c = 0x50;
    prm.b = 0x50;
    prm.a = 0x50;
    EffectMgr_Start(gEffects, slot, &prm);
    return 1;
}

/* room 0x20 (Room20_Cmd03_ptmf): the falling object falls (gravity 0.2 a frame on its velocity +0x30,
 * spinning 1 degree a frame) along the nav mesh, events bit 1 set while it lies on open floor;
 * landing on floor that isn't 0x10000 raises dust */
/* 0x002AE9C0 */
s32 Room20_Cmd03(void *self) {
    u8 *o = room_obj(D_003FF124);
    u8 *nm, *t;
    s32 tri;
    f32 y;
    f32 g[4] __attribute__((aligned(16)));
    Actor a;

    if (o == NULL) {
        return 1;
    }
    tri = AT(o, 0x3C, s32);
    if (tri == -1) {
        return 1;
    }
    if (tri & 0x80000000) {   /* landed */
        t = nav_tri(tri & 0x7FFFFFFF);
        if (t == NULL || (AT(t, 0x3C, u32) & 0x20020008)) {
            VCALL(gEvents, 0x60, void (*)(VObject *, s32))(gEvents, 1);
        }
        return 1;
    }
    g[1] = -0x1.99999a0000000p-3f /* 0.2 */;
    g[0] = 0.0f;
    g[3] = 1.0f;
    g[2] = 0.0f;
    sceVu0AddVector((f32 *)(o + 0x30), g, (f32 *)(o + 0x30));
    VCALL(gEvents, 0x60, void (*)(VObject *, s32))(gEvents, 1);
    a.vtbl = Actor_vtable;
    a.slot = 0x0FFFFFFF;
    a.flags24 = 0x1000000;
    y = AT(o, 0x24, f32);
    a.navMask = 0x20020008;
    a.navTri = tri;
    sceVu0CopyVector(a.pos, (f32 *)(o + 0x20));
    Actor_Move(&a, (f32 *)(o + 0x30));
    nm = (u8 *)gNavMesh;
    VCALL((VObject *)nm, 0x14, void (*)(VObject *, u32, f32 *))((VObject *)nm, a.navTri, a.pos);
    y += AT(o, 0x34, f32);
    sceVu0CopyVector((f32 *)(o + 0x20), a.pos);
    AT(o, 0x3C, u32) = a.navTri;
    if (a.navTri == (u32)-1) {
        a.vtbl = Actor_vtable;
        return 1;
    }
    if (!(y <= a.pos[1])) {
        f32 r;

        AT(o, 0x24, f32) = y;
        r = AT(o, 0x14, f32) + 0x1.1df46ap-6f /* 1 degree */;
        AT(o, 0x14, f32) = r;
        if (!(r <= 0x1.921fb6p+1f /* pi */)) {
            AT(o, 0x14, f32) = r - 0x1.921fb6p+2f /* 2 pi */;
        }
    } else {
        VCALL(gEvents, 0x60, void (*)(VObject *, s32))(gEvents, 7);
        AT(o, 0x38, f32) = 0.0f;
        AT(o, 0x34, f32) = 0.0f;
        AT(o, 0x30, f32) = 0.0f;
        t = (a.navTri < AT(nm, 0x8, u32) && AT(nm, 0x4, u8 *) != NULL) ? AT(nm, 0x4, u8 *) + a.navTri * 0x50 : NULL;
        if (t == NULL || (AT(t, 0x3C, u32) & 0x20020008)) {
            VCALL(gEvents, 0x60, void (*)(VObject *, s32))(gEvents, 1);
        }
        if (t != NULL && (AT(t, 0x3C, u32) & 0x2018000) != 0x10000) {
            Room20_Cmd04();
        }
        AT(o, 0x3C, u32) |= 0x80000000;
        VCALL(gEvents, 0x5C, void (*)(VObject *, s32))(gEvents, 1);
    }
    a.vtbl = Actor_vtable;
    return 1;
}

/* a pendulum (room object D_003FF110[byte 3]): byte 4 0 still; 1 its phase +0x30 on by 2
 * degrees (a tick sound at (-85, 30, 90) each turn), swinging 15 degrees (+0x14) */
/* 0x002AED60 */
s32 Room20_Cmd02(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } kPi = {0x40490FDB}, kTwoPi = {0x40C90FDB};
    u32 mode = cmd[4];
    u8 *o = VCALL(D_00456DF8, 0x18, u8 *(*)(VObject *, const char *))(D_00456DF8, D_003FF110[cmd[3]]);

    if (mode == 0) {
        AT(o, 0x30, f32) = 0.0f;
    } else if (mode == 1) {
        AT(o, 0x30, f32) = AT(o, 0x30, f32) + 2.0f;
        if (!(AT(o, 0x30, f32) < 360.0f)) {
            f32 at[4] __attribute__((aligned(16)));

            AT(o, 0x30, f32) = AT(o, 0x30, f32) - 360.0f;
            at[0] = -85.0f;
            at[1] = 30.0f;
            at[2] = 90.0f;
            Sound_PlayBankAt(gSound, 0x40000001, 6, at, 0, 0);
        }
        AT(o, 0x14, f32) = kPi.f * (15.0f * func_0031C248(kPi.f * AT(o, 0x30, f32) / 180.0f)) / 180.0f;
        if (!(AT(o, 0x14, f32) <= kPi.f)) {
            AT(o, 0x14, f32) = AT(o, 0x14, f32) - kTwoPi.f;
        }
    }
    return 1;
}

/* room 0x20 (Room20_Cmd01_ptmf): the falling object back up in place */
/* 0x002AEEE0 */
s32 Room20_Cmd01(void) {
    u8 *o = room_obj(D_003FF124);

#ifdef HG_NATIVE
    if (o == NULL) {   /* (the PS2 writes through junk) */
        return 1;
    }
#endif
    o[0] = 0;
    AT(o, 0x20, u32) = 0x411A8F5C;   /* 9.66 */
    AT(o, 0x24, u32) = 0x3F028F5C;   /* 0.51 */
    AT(o, 0x28, u32) = 0x42BACCCD;   /* 93.4 */
    AT(o, 0x10, u32) = 0x3FC8F5C3;   /* 1.57 */
    AT(o, 0x14, u32) = 0x3F4A3D71;   /* 0.79 */
    AT(o, 0x18, u32) = 0xC048F5C3;   /* -3.14 */
    return 1;
}

/* room 0x20 (Room20_Cmd00_ptmf): the player's action byte 3 (7) at (-30, 0, 90) */
/* 0x002AEF60 */
s32 Room20_Cmd00(void *self, void *a1, u8 *cmd) {
    f32 at[4] __attribute__((aligned(16)));

    at[0] = -30.0f;
    at[2] = 90.0f;
    at[1] = 0.0f;
    at[3] = 1.0f;
    Actor_PlaySound(&gCharPlayer->a, cmd[3], 7, 0, 0, at);
    return 1;
}
