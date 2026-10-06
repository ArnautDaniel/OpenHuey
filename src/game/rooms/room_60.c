/* Room 0x60: its event handler class (vtable D_00471FA0, see sRooms in event.c),
 * the tables its getters give the event system, and the room's hooks
 * (commands, conditions, objects and effects). */
#include "common.h"
#include "game/rooms/rooms.h"
#include "gl2d.h"
#include "ptmf.h"
#include "memcard.h"

extern void *D_0046DB80[];
extern void *D_00471FA0[];
extern s32 func_001770D0(Progress *p, s32 kind);   /* the slot of character kind (0xFF) */
extern void func_0016CEC0(Progress *p, const char *name);
extern s32 func_0016CD60(Progress *p, s32 who, s32 arg);
extern void func_0016CD30(Progress *p);
extern void glr_layer(s32 layer);
extern void glr_strip(const f32 *mvp, s32 n, const f32 *xyzw, const f32 *st, const u8 *rgba, const void *tex,
                      u64 tex0, u32 prim);
extern void *D_004795A0[];
extern void *D_004795C0[];
extern u8 D_004280A0[];
extern u8 D_004281E0[];
extern u8 D_00428220[];
extern u8 D_00428460[];
extern u8 D_004284A0[];
extern void *D_00429090[];
extern void *D_004290C0[];
extern u8 D_004291A0[];

#define B7_W(p, off)  (*(s32 *)((u8 *)(p) + (off)))

extern void *D_0046F580[];
extern void func_002D63B0(void *p);

extern PTMF D_019911C0[];

static void effect_795A0_init(void **obj) {
    obj[0] = D_004795A0;
}

static void effect_795C0_init(void **obj) {
    obj[0] = D_004795C0;
}

#ifdef HG_NATIVE
#endif

void *func_00310170(void *o, s32 flags) { return room_dtor(o, flags, D_00471FA0, D_0046DB80); }

void *func_003101D0(void) {
    return D_004280A0;
}

void *func_003101E0(void) {
    return D_004281E0;
}

void *func_003101F0(void) {
    return D_00428220;
}

void *func_00310200(void) {
    return D_00428460;
}

void *func_00310210(void) {
    return D_004284A0;
}

void *func_00310220(void *self, s32 i) {
    return D_00429090[i];
}

void *func_00310240(void) {
    return D_004291A0;
}

/* (self->*D_019911C0[i])(a, b) */
s32 func_00310250(void *self, u32 i, s32 a, s32 b) {
    return ptmf_scall_r2(self, &D_019911C0[i & 0xFF], a, b);
}

/* room 0x60 (D_00429190): the player's model +0xCC 0 (byte 3 0) or 1 */
s32 func_00310280(void *self, void *a1, u8 *cmd) {
    VObject *m = gCharPlayer->motion;

    VCALL(m, 0xCC, void (*)(VObject *, s32))(m, cmd[3] == 0 ? 0 : 1);
    return 1;
}

/* character 0xFE's model +0x9E0 / +0x9E4 / +0x9E8: byte 3 0 -0.2 / 0.2 / -0.2; 1 eases them by
 * script variable 6 (a step a call, waiting (2) for 60) to 0 / 0.3 / 0; else 0 / 0.3 / 0 */
s32 func_003102E0(void *self, void *a1, u8 *cmd) {
    static const union { u32 u; f32 f; } k02 = {0x3E4CCCCD}, kN02 = {0xBE4CCCCD}, k01 = {0x3DCCCCCE},
                                          k03 = {0x3E99999A};
    u8 *m = gCharacters[(u8)func_001770D0(gProgress, 0xFE)]->motion;
    VObject *ev;
    s32 v;
    f32 a, b;

    if (cmd[3] == 0) {
        AT(m, 0x9E0, f32) = kN02.f;
        AT(m, 0x9E4, f32) = k02.f;
        AT(m, 0x9E8, f32) = kN02.f;
        return 1;
    }
    if (cmd[3] != 1) {
        AT(m, 0x9E0, s32) = 0;
        AT(m, 0x9E4, f32) = k03.f;
        AT(m, 0x9E8, s32) = 0;
        return 1;
    }
    ev = gEvents;
    v = VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 6);
    a = kN02.f + k02.f * (f32)v / 60.0f;
    b = k02.f + k01.f * (f32)v / 60.0f;
    AT(m, 0x9E0, f32) = a;
    AT(m, 0x9E4, f32) = b;
    AT(m, 0x9E8, f32) = a;
    VCALL(ev, 0x30, void (*)(VObject *, s32, s32))(ev, 6, v + 1);
    return v + 1 < 61 ? 2 : 1;
}

/* room 0x60 (D_00429170): character 0xFE's model +0x9FC 0.4 / +0xA00 1 (byte 3 0), or 0 */
s32 func_00310450(void *self, void *a1, u8 *cmd) {
    u8 *m = gCharacters[(u8)func_001770D0(gProgress, 0xFE)]->motion;

    if (cmd[3] == 0) {
        AT(m, 0x9FC, f32) = 0x1.99999a0000000p-2f /* 0.4 */;
        AT(m, 0xA00, u8) = 1;
    } else {
        AT(m, 0x9FC, f32) = 0.0f;
        AT(m, 0xA00, u8) = 0;
    }
    return 1;
}

/* (as func_00300A20)  byte 4 0 starts the 8-byte effect D_004795C0 (parameters from byte 3), its
 * slot kept in event variable byte 3 + 2; else that effect is ended */
s32 func_003104D0(void *self, void *a1, u8 *cmd) {
    if (cmd[4] == 0) {
        u8 *mgr = gEffects;
        s32 slot = Effect_New(mgr, 8, effect_795C0_init);

        func_002D6090(mgr, slot, cmd + 3);
        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, (cmd[3] + 2) & 0xFF, slot);
    } else {
        s32 slot = VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, (cmd[3] + 2) & 0xFF);

        func_002D6170(gEffects, slot);
    }
    return 1;
}

/* room 0x60 (as func_002AFA00, for character 0xFE): byte 3 0 a progress name, 1 wait for
 * character 0xFE (2 while not), else done */
s32 func_00310640(void *self, void *a1, u8 *cmd) {
    switch (cmd[3]) {
    case 0:
        func_0016CEC0(gProgress, D_00429130);
        return 1;
    case 1:
        return func_0016CD60(gProgress, 0xFE, 0) == 0 ? 2 : 1;
    }
    func_0016CD30(gProgress);
    return 1;
}

/* room 0x60 (D_00429148): the floor light (room effect 0x1B, 20 x 20 at y -0.2) by byte 3 - 1
 * removed with its glow effect (event variable 1); 0 made, with the glow (D_004795A0); then (and
 * for other values) its strength from event variable 0 (0..4: 0, 30, 60, 90, 128), also sent to
 * the glow */
s32 func_003106E0(void *self, void *a1, u8 *cmd) {
    u32 q[20] __attribute__((aligned(16)));
    VObject *ev;

    if (cmd[3] == 1) {
        func_002670F0(gRoomEffects, 0x1B);
        func_002D6170(gEffects, VCALL(gEvents, 0x34, s32 (*)(VObject *, s32))(gEvents, 1));
        return 1;
    }
    if (cmd[3] == 0) {
        s32 slot;

        room_effect_slot_new(gRoomEffects, 0x1B, D_00472F60);
        slot = Effect_New(gEffects, 0x10, effect_795A0_init);
        VCALL(gEvents, 0x30, void (*)(VObject *, s32, s32))(gEvents, 1, slot);
    }
    ev = gEvents;
    q[16] = 0;
    q[0] = 0x41200000;   /* (10, -0.2, -10) */
    q[8] = 0x41200000;
    q[1] = 0xBE4CCCCD;
    q[2] = 0xC1200000;
    q[3] = 0x3F800000;
    q[4] = 0xC1200000;   /* (-10, -0.2, -10) */
    q[5] = 0xBE4CCCCD;
    q[6] = 0xC1200000;
    q[12] = 0xC1200000;
    q[7] = 0x3F800000;
    q[9] = 0xBE4CCCCD;   /* (10, -0.2, 10) */
    q[13] = 0xBE4CCCCD;
    q[10] = 0x41200000;
    q[14] = 0x41200000;
    q[11] = 0x3F800000;
    q[15] = 0x3F800000;
    q[19] = 0;
    switch (VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 0)) {
    case 0:
        q[19] = 0;
        break;
    case 1:
        q[19] = 0x1E;
        break;
    case 2:
        q[19] = 0x3C;
        break;
    case 3:
        q[19] = 0x5A;
        break;
    case 4:
        q[19] = 0x80;
        break;
    }
    q[18] = 0x3F800000;
    q[17] = q[19];
    func_00266C70(gRoomEffects, 0x1B, q);
    if (q[19] != 0) {
        func_002D6090(gEffects, VCALL(ev, 0x34, s32 (*)(VObject *, s32))(ev, 1), &q[19]);
    }
    return 1;
}

void *func_00310A10(void *self, s32 i) {
    return D_004290C0[i];
}

/* destructor (vtable D_004795C0) */
void *func_00358C20(u8 *o, s32 flags) {
    if (o != NULL) {
        AT(o, 0x0, void **) = D_004795C0;
        AT(o, 0x0, void **) = D_0046F580;
        if ((s16)flags > 0) {
            func_002D63B0(o);
        }
    }
    return o;
}

void func_00358C80(u8 *p, u8 *src) {
    if (src != NULL) {
        B7_W(p, 0x4) = *src;
    }
}

#ifdef HG_NATIVE

/* D_004795C0's +0x14 draw: at the corner +0x4 picks (0..3: (-15, -15), (15, -15), (15, 15),
 * (-15, 15); -1 none) a cone of light - its tip at height 16, an octagon of radius 4.5 at 10 -
 * added into the bloom's mask (layer 0x26) as a fan shaded from nothing at the tip to alpha
 * 0x20 round the rim, when all of it is in view */
void func_00358CA0(u8 *o) {
    f32 clip[4][4] __attribute__((aligned(16)));
    f32 pt[9][4] __attribute__((aligned(16)));
    f32 x, z, d;
    s32 k;

    switch (AT(o, 0x4, s32)) {
    case 0:
        x = -15.0f;
        z = -15.0f;
        break;
    case 1:
        x = 15.0f;
        z = -15.0f;
        break;
    case 2:
        x = 15.0f;
        z = 15.0f;
        break;
    case 3:
        x = -15.0f;
        z = 15.0f;
        break;
    default:
        return;
    }
    d = 4.5f * func_0031C248(0x1.921fb6p-1f /* pi / 4 */);
    pt[0][0] = x;        pt[0][1] = 16.0f; pt[0][2] = z;
    pt[1][0] = x + 4.5f; pt[1][1] = 10.0f; pt[1][2] = z;
    pt[2][0] = x + d;    pt[2][1] = 10.0f; pt[2][2] = z + d;
    pt[3][0] = x;        pt[3][1] = 10.0f; pt[3][2] = z + 4.5f;
    pt[4][0] = x - d;    pt[4][1] = 10.0f; pt[4][2] = z + d;
    pt[5][0] = x - 4.5f; pt[5][1] = 10.0f; pt[5][2] = z;
    pt[6][0] = x - d;    pt[6][1] = 10.0f; pt[6][2] = z - d;
    pt[7][0] = x;        pt[7][1] = 10.0f; pt[7][2] = z - 4.5f;
    pt[8][0] = x + d;    pt[8][1] = 10.0f; pt[8][2] = z - d;
    for (k = 0; k < 9; k++) {
        pt[k][3] = 1.0f;
    }
    VCALL(gCamera, 0x48, void (*)(VObject *, f32 (*)[4]))(gCamera, clip);
    for (k = 0; k < 9; k++) {
        f32 v[4] __attribute__((aligned(16)));

        sceVu0ApplyMatrix(v, clip, pt[k]);
        if (!(v[0] <= v[3]) || v[0] < -v[3] || !(v[1] <= v[3]) || v[1] < -v[3] || !(v[2] <= v[3]) || v[2] < -v[3]) {
            return;
        }
    }
    glr_layer(0x26);
    for (k = 1; k <= 8; k++) {   /* the fan, closing on its first rim point */
        f32 tri[3][4];
        f32 st[3][2] = {{0}};
        u8 col[3][4] = {{0, 0, 0, 0}, {0, 0, 0, 0x20}, {0, 0, 0, 0x20}};

        sceVu0CopyVector(tri[0], pt[0]);
        sceVu0CopyVector(tri[1], pt[k]);
        sceVu0CopyVector(tri[2], pt[k % 8 + 1]);
        AT(&tri[0][3], 0, u32) = AT(&tri[1][3], 0, u32) = AT(&tri[2][3], 0, u32) = 0;
        glr_strip(&clip[0][0], 3, &tri[0][0], &st[0][0], &col[0][0], NULL, 0,
                  0x40 | 0x10000 | 0x20000);   /* blended, GLR_PRIM_ADD, GLR_PRIM_NOZW */
    }
    glr_layer(-1);
}

#endif

s32 func_00359200(u8 *p) { return B7_W(p, 0x4) >= 0; }

void func_00359210(u8 *p) { B7_W(p, 0x4) = -1; }
